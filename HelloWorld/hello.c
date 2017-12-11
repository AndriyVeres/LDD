#include <linux/module.h>
#include <linux/init.h>
#include <linux/version.h>

int myVariableA = 0;
EXPORT_SYMBOL(myVariableA);

static int hello_init(void)
{
//Print current kernel version string
printk(KERN_INFO "Kernel version: \"  \" \n");
return 0;
}

static void hello_exit(void)
{
printk(KERN_ALERT "module exit\n");
}

module_init(hello_init);
module_exit(hello_exit);
