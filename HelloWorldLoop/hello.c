#include <linux/module.h>
#include <linux/init.h>

/* for ability to load module parameters*/
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");

/*parameters*/
char * pName = NULL;
unsigned int nQuantity = 1;

/* load parameters*/
module_param(nQuantity,int,S_IRUGO);
module_param(pName,charp,S_IRUGO);

static int hello_init(void)
{
  //Print current kernel version string
  printk(KERN_INFO "Kernel module init: \n");

  while(nQuantity != 0) {
    printk(KERN_INFO "%s", pName);
    printk(KERN_INFO "\n");
    nQuantity--;
  }
return 0;
}

static void hello_exit(void)
{
  printk(KERN_ALERT "module exit\n");
}

module_init(hello_init);
module_exit(hello_exit);
