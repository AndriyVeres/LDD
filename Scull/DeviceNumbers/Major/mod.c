/****************************************
 * This simple example increase major   *
 * part off dev_t and print result dev_t* 
 ****************************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>   /* dev_t */
#include <linux/kdev_t.h>  /* macro */

MODULE_LICENSE("GPL");

/* variables */
static unsigned int uDeviceCount = 1; /* # of devices */
static int          iMajorNumber = 0; /* major # */
static int          iMinorNumber = 0; /* minor # */
static dev_t        tDevice      = 0; /* dev_t # */
static int          i            = 0; /* counter for loop */

static int hello_init(void)
{
  printk(KERN_INFO "mod:init start \n");
  printk(KERN_INFO "mod:tDevice before:%x \n",tDevice);
	
  while(i<255)
  {
    iMajorNumber = i;
    tDevice = MKDEV(iMajorNumber, iMinorNumber);
    printk(KERN_INFO "mod:iMajorNumber = %d, tDevice = %x \n",iMajorNumber,tDevice);
    i++;
  }

  printk(KERN_INFO "mod:init end \n");
  return 0;
}

static void hello_exit(void)
{
  printk(KERN_ALERT "mod:exit \n");
}

module_init(hello_init);
module_exit(hello_exit);
