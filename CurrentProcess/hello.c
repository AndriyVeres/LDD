#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>

static int hello_init(void)
{
//Print current process
printk(KERN_INFO "The process is \" %s \" (pid %i) \n", current->comm, current->pid);
return 0;
}

static void hello_exit(void)
{
printk(KERN_ALERT "module exit\n");
}

module_init(hello_init);
module_exit(hello_exit);