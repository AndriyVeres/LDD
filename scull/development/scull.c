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
int     scull_trim(struct scull_dev * dev);
int     scull_open(struct inode *inode, struct file *filp);
int     scull_release(struct inode *inode, struct file *filp);
ssize_t scull_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t scull_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);

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

	/* Filling scull file_operations */
	scull_fops.owner = THIS_MODULE;
	scull_fops.open  = scull_open;
	scull_fops.read  = scull_read;
	scull_fops.write = scull_write;

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
	printk(KERN_DEBUG "SCULL: free memory allocated to character buffer...\n");
#endif /* SCULL_DEBUG */
	if(scull_dev.buffer != NULL)
	{
		kfree(scull_dev.buffer);
	}
#ifdef SCULL_DEBUG
	else
	{
		printk(KERN_ALERT "SCULL: empty pointer to character buffer\n");
	}
#endif /* SCULL_DEBUG */


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
	return 0;
}

int scull_open(struct inode *inode, struct file *filp)
{
	/* increment counter for scull open times */
	scull_dev.number_opens++;

#ifdef SCULL_DEBUG
	printk(KERN_DEBUG "SCULL: debug, %s %d number_opens:%u\n",__func__,__LINE__,scull_dev.number_opens);
#endif /* SCULL_DEBUG */

	return 0;  /* success always for now */
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
	if(*f_pos + count > SCULL_BUFFER_SIZE) {
#ifdef SCULL_DEBUG
		printk(KERN_ALERT "SCULL: %s %d debug, f_pos + count > SCULL_BUFFER_SIZE\n",__func__,__LINE__);
		printk(KERN_ALERT "SCULL: %s %d debug, *f_pos:%llu\n",__func__,__LINE__,(unsigned long long)*f_pos);
		printk(KERN_ALERT "SCULL: %s %d debug, count:%llu\n",__func__,__LINE__,(unsigned long long)count);
		printk(KERN_ALERT "SCULL: %s %d debug, SCULL_BUFFER_SIZE:%d\n",__func__,__LINE__,SCULL_BUFFER_SIZE);
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


module_init(scull_init);
module_exit(scull_exit);
