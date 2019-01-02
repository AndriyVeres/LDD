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
#include <linux/gpio.h>    /* for work with GPIO */
#include <linux/delay.h>     /* delays */

#include "klcd.h"

/* global sctructs, avaliable from any place of driver */
/* TODO: place under private data */
struct cdev            klcd_cdev;
struct file_operations klcd_fops;


MODULE_AUTHOR     ("Andriy Veres");
MODULE_VERSION    ("0.1");
MODULE_LICENSE    ("GPL");
MODULE_DESCRIPTION("Simple character device driver for write and read character text");


/* klcd work file_operations functions for work with driver, realized minimum quantity need for work */
/* TODO: place it into dedicated .c/.h files need update build system, how? */
int     klcd_trim(struct klcd_dev * dev);
int     klcd_open(struct inode *inode, struct file *filp);
int     klcd_release(struct inode *inode, struct file *filp);
ssize_t klcd_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t klcd_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
void    klcd_gpio_init(void);
void    klcd_gpio_validation(void);
void    klcd_set_all_gpio_direction(unsigned int direction);
void    klcd_set_gpio_direction(unsigned int gpio_num, unsigned int direction);
void    klcd_clear_display(void);
void    klcd_gpio_db_set(u_int8_t data);
void    klcd_all_gpio_export(void);
void    klcd_all_gpio_unexport(void);
void    klcd_all_gpio_request(void);
void    klcd_all_gpio_free(void);
void    klcd_all_gpio_set(bool value);





static int klcd_init(void)
{
	int alloc_result = 0;

	/*
	 * Character device registration:
	 * 0           - success
	 * negative - unsuccess
	*/
	printk(KLCD_DEBUG_LEVEL "KLCD: device number registration...\n");
	alloc_result = alloc_chrdev_region(&klcd_dev.dev, KLCD_DEV_FIRSTMIRROR, KLCD_DEV_COUNT,KLCD_DEV_NAME);

	/* Checking character device number registration */
	if(alloc_result != 0)
	{
		printk(KLCD_DEBUG_LEVEL "KLCD: unsuccess device number registration error code:%d\n",alloc_result);
		goto BAD;
	}
	else
	{
		printk(KLCD_DEBUG_LEVEL "KLCD: device: %X MAJOR: %d MINOR: %d count: %u\n",klcd_dev.dev,MAJOR(klcd_dev.dev),MINOR(klcd_dev.dev),KLCD_DEV_COUNT);
		printk(KLCD_DEBUG_LEVEL "KLCD: success device number registration\n");
	}

	/* Filling scull file_operations */
	klcd_fops.owner = THIS_MODULE;
	klcd_fops.open  = klcd_open;
	klcd_fops.read  = klcd_read;
	klcd_fops.write = klcd_write;

	/* initialize gpio pins */
	klcd_gpio_init();

	/* initialize number of device opens counter */
	klcd_dev.number_opens = 0;

	/* Initialize a klcd_dev structure  remembering fops, making it ready to add to the system with cdev_add */
	cdev_init(&klcd_cdev, &klcd_fops);             /* initialize already allocated character device */

	/* Make device avaliable in kernel */
	cdev_add(&klcd_cdev, MKDEV(MAJOR(klcd_dev.dev),MINOR(klcd_dev.dev)), KLCD_DEV_COUNT);

	return 0;

BAD:
	return -1;
}

static void klcd_exit(void)
{
	/* Remove a character device from the system */
	cdev_del(&klcd_cdev);

	/* reset number of device opens counter */
	klcd_dev.number_opens = 0;

	/* Character device number deregistration */
	printk(KLCD_DEBUG_LEVEL "KLCD: device number deregistration...\n");
#ifdef KLCD_DEBUG
	printk(KLCD_DEBUG_LEVEL "KLCD: debug, %s %d device: %X MAJOR: %d MINOR: %d count: %u\n",__func__,__LINE__,klcd_dev.dev,MAJOR(klcd_dev.dev),MINOR(klcd_dev.dev),KLCD_DEV_COUNT);
#endif /* KLCD_DEBUG */
	unregister_chrdev_region(klcd_dev.dev, KLCD_DEV_COUNT);   /* Character device number deregistration */
	printk(KLCD_DEBUG_LEVEL "KLCD: success device number deregistration\n");

	klcd_all_gpio_free();
}

int klcd_trim(struct klcd_dev * dev)
{
/* TODO: realize this functionality not only like stub */
#ifdef KLCD_DEBUG
	printk(KLCD_DEBUG_LEVEL "KLCD: debug, %s %d\n",__func__,__LINE__);
#endif /* KLCD_DEBUG */
	return 0;
}

int klcd_open(struct inode *inode, struct file *filp)
{
	/* increment counter for scull open times */
	klcd_dev.number_opens++;

#ifdef KLCD_DEBUG
	printk(KLCD_DEBUG_LEVEL "KLCD: debug, %s %d number_opens:%u\n",__func__,__LINE__,klcd_dev.number_opens);
#endif /* KLCD_DEBUG */

	return 0;  /* success always for now */
}


int klcd_release(struct inode *inode, struct file *filp)
{
	/* derement counter for klcd open times */
	klcd_dev.number_opens--;

#ifdef KLCD_DEBUG
	printk(KLCD_DEBUG_LEVEL "KLCD: debug, %s %d number_opens:%u\n",__func__,__LINE__,klcd_dev.number_opens);
#endif /* KLCD_DEBUG */

	return 0;
}

ssize_t klcd_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
#ifdef KLCD_DEBUG
		printk(KLCD_DEBUG_LEVEL "KLCD: debug, %s\n",__func__);
#endif /* KLCD_DEBUG */
		return 0;
}

ssize_t klcd_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
#ifdef KLCD_DEBUG
	printk(KLCD_DEBUG_LEVEL "KLCD: debug, %s\n",__func__);
#endif /* KLCD_DEBUG */
	return count;
}

void klcd_gpio_validation(void)
{
if(!gpio_is_valid(LCD_RS))
	goto BAD;
if(!gpio_is_valid(LCD_RW))
	goto BAD;
if(!gpio_is_valid(LCD_E))
	goto BAD;
if(!gpio_is_valid(LCD_DB0))
	goto BAD;
if(!gpio_is_valid(LCD_DB1))
	goto BAD;
if(!gpio_is_valid(LCD_DB2))
	goto BAD;
if(!gpio_is_valid(LCD_DB3))
	goto BAD;
if(!gpio_is_valid(LCD_DB4))
	goto BAD;
if(!gpio_is_valid(LCD_DB5))
	goto BAD;
if(!gpio_is_valid(LCD_DB6))
	goto BAD;
if(!gpio_is_valid(LCD_DB7))
	goto BAD;

	printk(KLCD_DEBUG_LEVEL "KLCD: %s success GPIO validation, exit\n",__func__);
	return;
BAD: 
	printk(KLCD_DEBUG_LEVEL "KLCD: %s error during GPIO validation, exit\n",__func__);
	return;
}
void klcd_gpio_init(void)
{
	klcd_gpio_validation();
	klcd_all_gpio_request();
	klcd_set_all_gpio_direction(OUTPUT);
	klcd_all_gpio_export();
	//klcd_all_gpio_set(false); /* export didn`t help */
	gpio_set_value(LCD_E,true); /* Read mode for beginning */	
	gpio_set_value(LCD_RS,false); /* data mode for beginning */
	gpio_set_value(LCD_RW,false); /* data mode for beginning */
	klcd_clear_display();
}

void klcd_gpio_exit(void)
{
	klcd_all_gpio_set(false);
	klcd_all_gpio_unexport();
	klcd_all_gpio_free();
}


void klcd_set_all_gpio_direction(unsigned int direction)
{
	/* Management pins*/
	klcd_set_gpio_direction(LCD_RS,direction);
	klcd_set_gpio_direction(LCD_RW,direction);
	klcd_set_gpio_direction(LCD_E,direction);
	/* Data pins */
	klcd_set_gpio_direction(LCD_DB0,direction);
	klcd_set_gpio_direction(LCD_DB1,direction);
	klcd_set_gpio_direction(LCD_DB2,direction);
	klcd_set_gpio_direction(LCD_DB3,direction);
	klcd_set_gpio_direction(LCD_DB4,direction);
	klcd_set_gpio_direction(LCD_DB5,direction);
	klcd_set_gpio_direction(LCD_DB6,direction);
	klcd_set_gpio_direction(LCD_DB7,direction);
}

void klcd_set_gpio_direction(unsigned int gpio_num, unsigned int direction)
{
if(INPUT == direction)
	gpio_direction_input(gpio_num); /* set GPIO direction to input */
else if(OUTPUT == direction)
	gpio_direction_output(gpio_num,true); /* set GPIO  direction to putput and setup low voltage level on pin */
else
	printk(KLCD_DEBUG_LEVEL "KLCD: %s wrong direction\n",__func__);
}

void klcd_clear_display(void)
{

	//step1
	mdelay(150);                   /* delay after power ON*/

	//step2
	klcd_gpio_db_set(0x30);      /* DB 0-7 zero */
	gpio_set_value(LCD_E,false); /*Enable signal High */
	mdelay(30);
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;


	//step3
	klcd_gpio_db_set(0x30);
	gpio_set_value(LCD_E,false); /*Enable signal High */
	mdelay(30);
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;

	
	//step4
	klcd_gpio_db_set(0x30);
	gpio_set_value(LCD_E,false); /*Enable signal High */
	mdelay(30);
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;

	
	//step5
	klcd_gpio_db_set(0x38);
	gpio_set_value(LCD_E,false); /*Enable signal High */
	mdelay(30);
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;

	
	//step6
	klcd_gpio_db_set(0x08);
	gpio_set_value(LCD_E,false); /*Enable signal High */
	mdelay(30);
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;

	
	//step7 .. clear display .. since has to write to all the
	//screen memory .. takes longer to execute
	klcd_gpio_db_set(0x01);
	gpio_set_value(LCD_E,false); /*Enable signal High */
	mdelay(30);
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	mdelay(300);

	
	//step8
	klcd_gpio_db_set(0x06);
	gpio_set_value(LCD_E,false); /*Enable signal High */
	mdelay(30);
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;

	
	//---end of lcd initialization ---
	
	//step 9: turn on the lcd + show cursor
	klcd_gpio_db_set(0x0F);
	gpio_set_value(LCD_E,false); /*Enable signal High */
	mdelay(30);
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;

#if 0
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	gpio_set_value(LCD_RS,true); /*Data */
	klcd_gpio_db_set(0x00);      /* DB 0-7 zero */
	DELAY;
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */

	mdelay(5);                   /* delay after first command*/
	gpio_set_value(LCD_RW,false); /*Write */
	gpio_set_value(LCD_RS,false); /*Instruction */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	klcd_gpio_db_set(0x30);      /* DB 0-7 zero */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	gpio_set_value(LCD_RS,true); /*Data */
	klcd_gpio_db_set(0x00);      /* DB 0-7 zero */
	DELAY;
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */


	mdelay(5);                   /* delay after first command*/
	gpio_set_value(LCD_RW,false); /*Write */
	gpio_set_value(LCD_RS,false); /*Instruction */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	klcd_gpio_db_set(0x30);      /* DB 0-7 zero */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	gpio_set_value(LCD_RS,true); /*Data */
	klcd_gpio_db_set(0x00);      /* DB 0-7 zero */
	DELAY;
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */



	mdelay(5);                   /* delay after first command*/
	gpio_set_value(LCD_RW,false); /*Write */
	gpio_set_value(LCD_RS,false); /*Instruction */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	klcd_gpio_db_set(0x3C);      /* DB 0-7 zero */
	//klcd_gpio_db_set(0x30);      /* DB 0-7 zero */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	gpio_set_value(LCD_RS,true); /*Data */
	klcd_gpio_db_set(0x00);      /* DB 0-7 zero */
	DELAY;
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */

	mdelay(5);                   /* delay after first command*/
	gpio_set_value(LCD_RW,false); /*Write */
	gpio_set_value(LCD_RS,false); /*Instruction */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	klcd_gpio_db_set(0x08);      /* DB 0-7 zero */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	gpio_set_value(LCD_RS,true); /*Data */
	klcd_gpio_db_set(0x00);      /* DB 0-7 zero */
	DELAY;
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */


	mdelay(5);                   /* delay after first command*/
	gpio_set_value(LCD_RW,false); /*Write */
	gpio_set_value(LCD_RS,false); /*Instruction */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	klcd_gpio_db_set(0x01);      /* DB 0-7 zero */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	gpio_set_value(LCD_RS,true); /*Data */
	klcd_gpio_db_set(0x00);      /* DB 0-7 zero */
	DELAY;
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */


	mdelay(5);                   /* delay after first command*/
	gpio_set_value(LCD_RW,false); /*Write */
	gpio_set_value(LCD_RS,false); /*Instruction */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	klcd_gpio_db_set(0x05);      /* DB 0-7 zero */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	gpio_set_value(LCD_RS,true); /*Data */
	klcd_gpio_db_set(0x00);      /* DB 0-7 zero */
	DELAY;
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */


	mdelay(5);                   /* delay after first command*/
	gpio_set_value(LCD_RW,false); /*Write */
	gpio_set_value(LCD_RS,false); /*Instruction */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	klcd_gpio_db_set(0x0F);      /* DB 0-7 zero */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	gpio_set_value(LCD_RS,true); /*Data */
	klcd_gpio_db_set(0x00);      /* DB 0-7 zero */
	DELAY;
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */


	mdelay(5);                   /* delay after first command*/
	gpio_set_value(LCD_RW,false); /*Write */
	gpio_set_value(LCD_RS,false); /*Instruction */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	klcd_gpio_db_set(0x01);      /* DB 0-7 zero */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	gpio_set_value(LCD_RS,true); /*Data */
	klcd_gpio_db_set(0x00);      /* DB 0-7 zero */
	DELAY;
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
#endif

	
	mdelay(5);                   /* delay after first command*/
	gpio_set_value(LCD_RW,false); /*Write */
	gpio_set_value(LCD_RS,true); /*Instruction */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	klcd_gpio_db_set(0b01001101);      /* DB 0-7 zero */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;

	mdelay(5);                   /* delay after first command*/
	gpio_set_value(LCD_RW,false); /*Write */
	gpio_set_value(LCD_RS,true); /*Instruction */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	klcd_gpio_db_set(0b01000001);      /* DB 0-7 zero */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;

	mdelay(5);                   /* delay after first command*/
	gpio_set_value(LCD_RW,false); /*Write */
	gpio_set_value(LCD_RS,true); /*Instruction */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	klcd_gpio_db_set(0b01001101);      /* DB 0-7 zero */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;

	mdelay(5);                   /* delay after first command*/
	gpio_set_value(LCD_RW,false); /*Write */
	gpio_set_value(LCD_RS,true); /*Instruction */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	klcd_gpio_db_set(0b01001111);      /* DB 0-7 zero */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;

	mdelay(5);                   /* delay after first command*/
	gpio_set_value(LCD_RW,false); /*Write */
	gpio_set_value(LCD_RS,true); /*Instruction */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	klcd_gpio_db_set(0b11000000);      /* DB 0-7 zero */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;

	
	mdelay(5);                   /* delay after first command*/
	gpio_set_value(LCD_RW,false); /*Write */
	gpio_set_value(LCD_RS,true); /*Instruction */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	klcd_gpio_db_set(0b01001011);      /* DB 0-7 zero */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;


	
	mdelay(5);                   /* delay after first command*/
	gpio_set_value(LCD_RW,false); /*Write */
	gpio_set_value(LCD_RS,true); /*Instruction */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
	klcd_gpio_db_set(0b01000001);      /* DB 0-7 zero */
	DELAY;
	gpio_set_value(LCD_E,false); /*Enable signal High */
	DELAY;	
	gpio_set_value(LCD_RW,true); /*Read */
	DELAY;
	gpio_set_value(LCD_E,true); /*Enable signal High */
	DELAY;
} 

void klcd_gpio_db_set(u_int8_t data)
{
	gpio_set_value(LCD_DB0,(bool)(data & LCD_DB0_BITMASK));
	gpio_set_value(LCD_DB1,(bool)(data & LCD_DB1_BITMASK));
	gpio_set_value(LCD_DB2,(bool)(data & LCD_DB2_BITMASK));
	gpio_set_value(LCD_DB3,(bool)(data & LCD_DB3_BITMASK));
	gpio_set_value(LCD_DB4,(bool)(data & LCD_DB4_BITMASK));
	gpio_set_value(LCD_DB5,(bool)(data & LCD_DB5_BITMASK));
	gpio_set_value(LCD_DB6,(bool)(data & LCD_DB6_BITMASK));
	gpio_set_value(LCD_DB7,(bool)(data & LCD_DB7_BITMASK));

	printk(KLCD_DEBUG_LEVEL "%x%x%x%x%x%x%x%x\n", (bool)(data & LCD_DB7_BITMASK),
												  (bool)(data & LCD_DB6_BITMASK),
												  (bool)(data & LCD_DB5_BITMASK),
												  (bool)(data & LCD_DB4_BITMASK),
												  (bool)(data & LCD_DB3_BITMASK),
												  (bool)(data & LCD_DB2_BITMASK),
												  (bool)(data & LCD_DB1_BITMASK),
												  (bool)(data & LCD_DB0_BITMASK));
}

void    klcd_all_gpio_request(void)
{
	/* Management pins*/
	gpio_request(LCD_RS, "sysfs");
	gpio_request(LCD_RW, "sysfs");
	gpio_request(LCD_E,  "sysfs");
	/* Data pins */
	gpio_request(LCD_DB0,"sysfs");
	gpio_request(LCD_DB1,"sysfs");
	gpio_request(LCD_DB2,"sysfs");
	gpio_request(LCD_DB3,"sysfs");
	gpio_request(LCD_DB4,"sysfs");
	gpio_request(LCD_DB5,"sysfs");
	gpio_request(LCD_DB6,"sysfs");
	gpio_request(LCD_DB7,"sysfs");
}

void    klcd_all_gpio_export(void)
{
	/* Management pins*/
	gpio_export(LCD_RS, false);
	gpio_export(LCD_RW, false);
	gpio_export(LCD_E,  false);
	/* Data pins */
	gpio_export(LCD_DB0,false);
	gpio_export(LCD_DB1,false);
	gpio_export(LCD_DB2,false);
	gpio_export(LCD_DB3,false);
	gpio_export(LCD_DB4,false);
	gpio_export(LCD_DB5,false);
	gpio_export(LCD_DB6,false);
	gpio_export(LCD_DB7,false);
}

void klcd_all_gpio_unexport(void)
{
	/* Management pins*/
	gpio_unexport(LCD_RS);
	gpio_unexport(LCD_RW);
	gpio_unexport(LCD_E);
	/* Data pins */
	gpio_unexport(LCD_DB0);
	gpio_unexport(LCD_DB1);
	gpio_unexport(LCD_DB2);
	gpio_unexport(LCD_DB3);
	gpio_unexport(LCD_DB4);
	gpio_unexport(LCD_DB5);
	gpio_unexport(LCD_DB6);
	gpio_unexport(LCD_DB7);
}



void    klcd_all_gpio_free(void)
{
	/* Management pins*/
	gpio_free(LCD_RS);
	gpio_free(LCD_RW);
	gpio_free(LCD_E);
	/* Data pins */
	gpio_free(LCD_DB0);
	gpio_free(LCD_DB1);
	gpio_free(LCD_DB2);
	gpio_free(LCD_DB3);
	gpio_free(LCD_DB4);
	gpio_free(LCD_DB5);
	gpio_free(LCD_DB6);
	gpio_free(LCD_DB7);
}

void    klcd_all_gpio_set(bool value)
{
	/* Management pins*/
	gpio_set_value(LCD_RS, value);
	gpio_set_value(LCD_RW, value);
	gpio_set_value(LCD_E,  value);
	/* Data pins */
	gpio_set_value(LCD_DB0,value);
	gpio_set_value(LCD_DB1,value);
	gpio_set_value(LCD_DB2,value);
	gpio_set_value(LCD_DB3,value);
	gpio_set_value(LCD_DB4,value);
	gpio_set_value(LCD_DB5,value);
	gpio_set_value(LCD_DB6,value);
	gpio_set_value(LCD_DB7,value);
}

module_init(klcd_init);
module_exit(klcd_exit);
