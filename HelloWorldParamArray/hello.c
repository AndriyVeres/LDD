#include <linux/module.h>
#include <linux/init.h>

/* for ability to load module parameters*/
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");

/* default array*/
unsigned int pArray[4]={1,1,1,1};
unsigned int nQuantity = 4;

/* load parameters array */
/* SOOMETHING WRONG HEAR */
module_param_array(pArray,uint,&nQuantity,S_IRUGO);


static int hello_init(void)
{
  /* while counter */
  unsigned int i = 0;

  //Print current kernel version string
  printk(KERN_INFO "Kernel module init: \n");


  while(i < nQuantity) {
    printk(KERN_INFO "%u ,", pArray[nQuantity]);
    i++;
  }

return 0;
}

static void hello_exit(void)
{
  printk(KERN_ALERT "module exit\n");
}

module_init(hello_init);
module_exit(hello_exit);
