/**-----------------------------------------------+
 * @author  Andriy Veres <andriy-pilot@ukr.net>   |
 * @version 0.1                                   |
 * @license GPL v3                                |
 -------------------------------------------------+*/

#include <linux/module.h>  /* MODULE_LICENSE() MODULE_AUTHOR() */
#include <linux/init.h>    /* init() exit() */
#include <linux/types.h>   /* Used for uint8 e.t.c types */
#include <linux/kdev_t.h>  /* Undefined what it is for */
#include <linux/fs.h>      /* Used for working with inode */
#include <linux/errno.h>   /* Used for errors handling */
#include <linux/cdev.h>    /* Used for device registration */
#include <linux/uaccess.h> /* Required for copy_to/from_user */
#include <linux/slab.h>    /* Required for kzalloc/kfree */

#include "scull.h"

/* global sctructs, avaliable from any place of driver */
/* TODO: place under private data */
struct cdev            scull_cdev;
struct file_operations scull_fops;


MODULE_AUTHOR     ("Andriy Veres");
MODULE_VERSION    ("0.1");
MODULE_LICENSE    ("GPL v3");
MODULE_DESCRIPTION("Simple character device driver for write and read character text");


/* scull work file_operations functions for work with driver, realized minimum quantity need for work */
/* TODO: place it into dedicated .c/.h files need update build system, how? */

/* SCULL interface */
int     scull_trim(struct scull_dev * dev);
int     scull_open(struct inode *inode, struct file *filp);
int     scull_release(struct inode *inode, struct file *filp);
ssize_t scull_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t scull_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);

/*SCULL helpers functions */
int scull_qset_alloc(struct scull_dev * dev);
int scull_qset_free(struct scull_dev * dev);



static int scull_init(void)
{
	int alloc_result = 0;
	scull_dev.buffer = NULL;

	/*
	 * Character device registration:
	 * 0           - success
	 * negative - unsuccess
	*/
	printk(KERN_ALERT "SCULL: device number registration...\n");
	alloc_result = alloc_chrdev_region(&scull_dev.dev, SCULL_DEV_FIRSTMIRROR, SCULL_DEV_COUNT,SCULL_DEV_NAME);

	/* Checking character device number registration */
	if(alloc_result != 0)
	{
		printk(KERN_DEBUG "SCULL: unsuccess device number registration error code:%d\n",alloc_result);
		goto BAD;
	}
	else
	{
		printk(KERN_ALERT "SCULL: device: %X MAJOR: %d MINOR: %d count: %u\n",scull_dev.dev,MAJOR(scull_dev.dev),MINOR(scull_dev.dev),SCULL_DEV_COUNT);
		printk(KERN_ALERT "SCULL: success device number registration\n");
	}

	/* allocating character buffer */
#ifdef SCULL_DEBUG
	printk(KERN_ALERT "SCULL: memory allocation for character buffer...\n");
#endif /* SCULL_DEBUG */
	scull_dev.buffer = kzalloc(SCULL_BUFFER_SIZE,GFP_KERNEL);
	if(scull_dev.buffer == NULL)
	{
#ifdef SCULL_DEBUG
		printk(KERN_ALERT "SCULL: unsuccessful memory allocation for character buffer\n");
		goto BAD;
#endif /* SCULL_DEBUG */
	}
#ifdef SCULL_DEBUG
	else
	{
		printk(KERN_DEBUG "SCULL: successful memory allocation for character buffer with size:%d\n",SCULL_BUFFER_SIZE);
	}
#endif /* SCULL_DEBUG */

	/* Set  scull memory configuration */
	/*TODO: realize with module param */
	scull_dev.quantum_count = SCULL_QUANTUM_COUNT;
	scull_dev.quantum_size = SCULL_QUANTUM_SIZE;
	scull_dev.qset_head = NULL;
	scull_dev.qset_size = SCULL_QSET_SIZE;

	/* Allocation scull linked list */
	scull_qset_alloc(&scull_dev);

	

	/* Filling scull file_operations */
	scull_fops.owner   = THIS_MODULE;
	scull_fops.open    = scull_open;
	scull_fops.read    = scull_read;
	scull_fops.write   = scull_write;
	scull_fops.release = scull_release;

	/* initialize number of device opens counter */
	scull_dev.number_opens = 0;

	/* Initialize a scull_dev structure  remembering fops, making it ready to add to the system with cdev_add */
	cdev_init(&scull_cdev, &scull_fops);             /* initialize already allocated character device */

	/* Make device avaliable in kernel */
	cdev_add(&scull_cdev, MKDEV(MAJOR(scull_dev.dev),MINOR(scull_dev.dev)), SCULL_DEV_COUNT);

	return 0;

BAD:
	return -1;
}

static void scull_exit(void)
{
	/* Remove a character device from the system */
	cdev_del(&scull_cdev);

	/* reset number of device opens counter */
	scull_dev.number_opens = 0;

	/* free memory, allocated for characted buffer */
#ifdef SCULL_DEBUG
	printk(KERN_DEBUG "SCULL: free memory, allocated for character buffer at address:%X...\n", scull_dev.buffer);
#endif /* SCULL_DEBUG */
	if(scull_dev.buffer != NULL) {
		kfree(scull_dev.buffer);
		scull_dev.buffer = NULL;
#ifdef SCULL_DEBUG
		printk(KERN_DEBUG "SCULL: success free memory, allocated for character buffer\n");
#endif /* SCULL_DEBUG */
	}
#ifdef SCULL_DEBUG
	else {
		printk(KERN_ALERT "SCULL: empty pointer to character \n");
	}
#endif /* SCULL_DEBUG */

	/* free memory, allocated for scull linked list */
#ifdef SCULL_DEBUG
	printk(KERN_DEBUG "SCULL: try to free memory, allocated for qset that stored in %X...\n",&scull_dev);	
#endif /* SCULL_DEBUG */
	scull_qset_free(&scull_dev);


	/* Character device number deregistration */
	printk(KERN_DEBUG "SCULL: device number deregistration...\n");
#ifdef SCULL_DEBUG
	printk(KERN_DEBUG "SCULL: debug, %s %d device: %X MAJOR: %d MINOR: %d count: %u\n",__func__,__LINE__,scull_dev.dev,MAJOR(scull_dev.dev),MINOR(scull_dev.dev),SCULL_DEV_COUNT);
#endif /* SCULL_DEBUG */
	unregister_chrdev_region(scull_dev.dev, SCULL_DEV_COUNT);   /* Character device number deregistration */
	printk(KERN_DEBUG "SCULL: success device number deregistration\n");
}

int scull_trim(struct scull_dev * dev)
{
/* TODO: realize this functionality not only like stub */
#ifdef SCULL_DEBUG
	printk(KERN_DEBUG "SCULL: debug, %s %d\n",__func__,__LINE__);
#endif /* SCULL_DEBUG */

	/* pointer to next node in linked list, scull_qset`s, pointer to current node */
	struct scull_qset * p_next = dev->qset_head->p_next,* p_curr = dev->qset_head;
	unsigned int qset_size = dev->qset_size;
	unsigned int i;

	if(p_curr == NULL) {
		/* TODO: update when bad case */
	}

	/* Walk through linked list */
	do {
		/* zero memory, allocated for quantums in qset */
		for(i=0;i<dev->quantum_count;++i) {
			memset(p_curr->data[i],0,dev->quantum_size);
		}
		p_next = p_curr->p_next;
		p_curr = p_next;
	} while(p_next != NULL);
	
	
	return 0;
}

int scull_open(struct inode *inode, struct file *filp)
{
	/* increment counter for scull open times */
	scull_dev.number_opens++;

#ifdef SCULL_DEBUG
	printk(KERN_DEBUG "SCULL: debug, %s %d number_opens:%u\n",__func__,__LINE__,scull_dev.number_opens);
#endif /* SCULL_DEBUG */

	filp->private_data = &scull_cdev; /* for another scull interface functions */ /* TODO: replace to containter_of macro */
	/* Set the device lenght zero if device was open only for write */
#ifdef SCULL_DEBUG
	printk(KERN_DEBUG "SCULL: debug, %s %d (filp->f_flags %X\n",__func__,__LINE__,filp->f_flags);
#endif /* SCULL_DEBUG */
	if ((filp->f_flags & O_ACCMODE) == O_WRONLY) {
		scull_trim(&scull_dev); /* Replace to incapsuled in something */
#ifdef SCULL_DEBUG
		printk(KERN_DEBUG "SCULL: debug, %s %d (filp->f_flags & O_ACCMODE) == O_WRONLY\n",__func__,__LINE__);
#endif /* SCULL_DEBUG */
	}
	/* TODO: check else handling if necessary */

	return 0; /* success always */
}


int scull_release(struct inode *inode, struct file *filp)
{
	/* derement counter for scull open times */
	scull_dev.number_opens--;

#ifdef SCULL_DEBUG
	printk(KERN_DEBUG "SCULL: debug, %s %d number_opens:%u\n",__func__,__LINE__,scull_dev.number_opens);
#endif /* SCULL_DEBUG */

	return 0;
}

ssize_t scull_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	ssize_t result = 0;

	if(*f_pos != 0) {
#ifdef SCULL_DEBUG
		printk(KERN_DEBUG "SCULL: debug, %s %d (*f_pos != 0) f_pos:%llu\n",__func__,__LINE__,(unsigned long long)*f_pos);
#endif /* SCULL_DEBUG */
		return 0; /* all data was copied to user in previous  call, because f_pos is shifted with respect to zero */
	}
	else { /* f_pos equal to zero, so reading from beginning*/
		result = copy_to_user(buf,scull_dev.buffer+*f_pos,scull_dev.buffer_size);
#ifdef SCULL_DEBUG
		printk(KERN_DEBUG "SCULL: debug, %s %d f_pos:%llu\n",__func__,__LINE__,(unsigned long long)*f_pos);
		printk(KERN_DEBUG "SCULL: debug, %s %d fcopy_to_user returns %llu\n",__func__,__LINE__,(unsigned long long)result);
#endif /* SCULL_DEBUG */

		if(result != 0) { /* fault to copy fo user, unread bytes returned by copy_to_user to result variable */
#ifdef SCULL_DEBUG
			printk(KERN_DEBUG "SCULL: debug, %s %d (result != 0) return -EFAULT;\n",__func__,__LINE__);
#endif /* SCULL_DEBUG */
			return -EFAULT;
		}
		*f_pos += scull_dev.buffer_size; /* shift f_pos with respect of zero */
#ifdef SCULL_DEBUG
		printk(KERN_DEBUG "SCULL: debug, %s %d successfuly read:%llu bytes\n",__func__,__LINE__,(unsigned long long)scull_dev.buffer_size);
#endif /* SCULL_DEBUG */
		return scull_dev.buffer_size;
	}
}

ssize_t scull_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	ssize_t result = count;

	/* check position and size according to character buffer size */
	if(*f_pos + count > scull_dev->quantum_size * scull_dev->quantum_count * scull_dev->qset_size) {
#ifdef SCULL_DEBUG
		printk(KERN_ALERT "SCULL: %s %d debug, f_pos + count > size of scull allocated memory\n",__func__,__LINE__);
		printk(KERN_ALERT "SCULL: %s %d debug, *f_pos:%llu\n",__func__,__LINE__,(unsigned long long)*f_pos);
		printk(KERN_ALERT "SCULL: %s %d debug, count:%llu\n",__func__,__LINE__,(unsigned long long)count);
		printk(KERN_ALERT "SCULL: %s %d debug, size of scull allocated memory:%d\n",__func__,__LINE__,scull_dev->quantum_size * scull_dev->quantum_count * scull_dev->qset_size);
#endif /* SCULL_DEBUG */
		return -EFAULT;
	}

	/* copy data from buffer to user */
	/* TODO: realize write with f_pos shift */
	result = copy_from_user(scull_dev.buffer,buf,count);

	if(result == 0) { /* no unread bytes, so all is successfull */
		scull_dev.buffer_size = count;
#ifdef SCULL_DEBUG
	printk(KERN_DEBUG "SCULL: debug, %s %d write:%llu bytes\n",__func__,__LINE__,(unsigned long long)result);
#endif /* SCULL_DEBUG */
		return count; /* inform that we read successfull count bytes */
	}
	else {
		return -EFAULT; /* return error code */
	}

#ifdef SCULL_DEBUG
		 printk(KERN_DEBUG "SCULL: debug, %s %d result: %llu\n",__func__,__LINE__,(unsigned long long)result);
#endif /* SCULL_DEBUG */

	return result;
}

int scull_qset_alloc(struct scull_dev * dev)
{
	struct scull_qset * p_qset = NULL, * p_previous = NULL, * p_next = NULL;
	unsigned int i,j;

	dev->qset_head = NULL; /* Initialize qset_head pointer */

	/* Allocation memory for qset (node in linked list) and create first element in linked list */
	dev->qset_head =  kzalloc(sizeof(struct scull_qset),GFP_KERNEL);

	if(dev->qset_head == NULL) {
		/* TODO: process bad case */
	}
	
	dev->qset_head->p_previous = NULL; /* Head of linked list */
	/* Allocate memory for qset pointers array */
	dev->qset_head->data = kzalloc(sizeof(void *)*(dev->quantum_count),GFP_KERNEL);
	for(j = 0;j < dev->quantum_count;++j) {
		/* Allocating memory for each qset in quantun */
		dev->qset_head->data[j] = kzalloc(dev->quantum_size,GFP_KERNEL);
		/* TODO: update bad case */
	}
	p_previous = dev->qset_head; /* storing previous element */

	/* Create linked list of qset`s without first element, as it was created above */
	for(i=1;i<dev->qset_size;++i) {
		/* Allocation memory for qset (node in linked list) */
		p_qset =  kzalloc(sizeof(struct scull_qset),GFP_KERNEL);

		if(p_qset == NULL) {
		/* TODO: process bad case */
		}

		p_previous->p_next = p_qset;             /* Link from previous node to current node */

		/* Allocate memory for qset pointers array */
		p_qset->data = kzalloc(sizeof(void *)*(dev->quantum_count),GFP_KERNEL);
		for(j = 0;j < dev->quantum_count;++j) {
			/* Allocating memory for each qset in quantun */
			p_qset->data[j] = kzalloc(dev->quantum_size,GFP_KERNEL);
			/* TODO: update bad case */
		}
		p_previous = p_qset; /* Update pointer to previous node */
	}
	p_previous->p_next = NULL; /* End of linked list */
	return 0; /* Success always for now */
}
int scull_qset_free(struct scull_dev * dev)
{
	struct scull_qset * p_curr = NULL, * p_previous = NULL, * p_next = NULL;
	unsigned int i, j;

	/* Check qset_head pointer */
	if(dev->qset_head == NULL) {
		/* TODO: process bad case */
#ifdef SCULL_DEBUG
		printk(KERN_WARNING "SCULL: %s %d debug, NULL pointer in dev->qset_head\n",__func__,__LINE__);
#endif /* SCULL_DEBUG */
	}
	

	p_curr = dev->qset_head;

	/* Walk through linked list of scull_qset`s */
	while(p_curr != NULL) {
#ifdef SCULL_DEBUG /* TODO: update bad case */
		printk(KERN_DEBUG "SCULL: debug p_curr (%X) linked list iteration \n",p_curr);
#endif /* SCULL_DEBUG */
		if(p_curr->data == NULL) {
#ifdef SCULL_DEBUG /* TODO: update bad case */
			printk(KERN_WARNING "SCULL: qset with NULL address when try to free\n");
#endif /* SCULL_DEBUG */
		}

		/* Free memory for each qset in quantun */
		for(j = 0;j < dev->quantum_count;++j) {
#ifdef SCULL_DEBUG
			printk(KERN_DEBUG "SCULL:debug iteration [%d] in qset`s array\n",j);
#endif /* SCULL_DEBUG */
			if(p_curr->data[j] != NULL) {
				kfree(p_curr->data[j]);
#ifdef SCULL_DEBUG
				printk(KERN_DEBUG "SCULL:debug success free p_curr->data[%d] at \n",j,p_curr->data[j]);
#endif /* SCULL_DEBUG */
				p_curr->data[j] = NULL;
			}
			else {
#ifdef SCULL_DEBUG	/* TODO: update bad case */
				printk(KERN_WARNING "SCULL: p_curr->data[%d] is NULL \n",j);	
#endif /* SCULL_DEBUG */
			}	
		}
		/* Free memory for qset pointers array */
#ifdef SCULL_DEBUG
		printk(KERN_WARNING "SCULL: debug try to free p_curr->data at %X\n",p_curr->data);	
#endif /* SCULL_DEBUG */
		kfree(p_curr->data);
#ifdef SCULL_DEBUG
		printk(KERN_WARNING "SCULL: debug success free p_curr->data at %X\n",p_curr->data);	
#endif /* SCULL_DEBUG */
		p_curr->data = NULL;
			
		p_previous = p_curr;     /* Update pointer to previous node */
		p_curr = p_curr->p_next; /* Go to the next node */
		kfree(p_previous);       /* Free current node */
	}
	return 0; /* Success always for now */

}


module_init(scull_init);
module_exit(scull_exit);
