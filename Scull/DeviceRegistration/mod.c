/*************************************************************
 * This simple example register device with                  *
 * count and name that user enter, example:                  *
 * sudo insmod mod.ko sDeviceName=DeviceName nDeviceCount=3  * 
 *************************************************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>   	/* dev_t */
#include <linux/kdev_t.h>  	/* macro MAJOR/MINOR */
#include <linux/fs.h>      	/* regster device*/
#include <linux/moduleparam.h>  /* module parameters */

MODULE_LICENSE("GPL");

/* variables */
static unsigned int nDeviceCount = 1; 		/* # of devices */
static unsigned int nFirstMirror = 0; 		/* # of device array start value */
static int          iMajorNumber = 0; 		/* major # */
static int          iMinorNumber = 0; 		/* minor # */
static dev_t        tDevice      = 0; 		/* dev_t # */
static char *       sDeviceName  = "My_Device"; /* device name */
static int result; /* device number allocation result */

/* loading module parameters */
module_param(nDeviceCount,uint,S_IRUGO);
module_param(sDeviceName,charp,S_IRUGO);


static int hello_init(void)
{
  printk(KERN_INFO "**************************************\n");
  printk(KERN_INFO "mod:init start \n");
  
  result = alloc_chrdev_region(&tDevice,nFirstMirror,nDeviceCount,sDeviceName);
  
  if(result == 0)
  { 
    iMajorNumber = MAJOR(tDevice); //get major #
    iMinorNumber = MINOR(tDevice); //get minot #

    printk(KERN_INFO "  device registered successfully: \n");
    printk(KERN_INFO "  device name  : %s \n",sDeviceName);
    printk(KERN_INFO "  device number: %x \n",tDevice);
    printk(KERN_INFO "    major #    : %d \n",iMajorNumber);
    printk(KERN_INFO "    minor #    : %d \n",iMinorNumber);
    printk(KERN_INFO "  device count : %u \n",nDeviceCount);
  }
  else
    printk(KERN_ALERT "mod: ERROR: device not registere successfully \n");

  printk(KERN_INFO "mod:init end \n");

  return 0;
}

static void hello_exit(void)
{
  printk(KERN_INFO "mod:exit start \n");

  if(result==0)
  {
    unregister_chrdev_region(tDevice,nDeviceCount);
    printk(KERN_INFO " \t device unregistered \n");	  
  }  

  printk(KERN_INFO "mod:exit end \n");
  printk(KERN_INFO "**************************************\n");
}

module_init(hello_init);
module_exit(hello_exit);
