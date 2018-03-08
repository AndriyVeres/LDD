#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/errno.h>

#include "scull.h"



MODULE_LICENSE("GNU");
MODULE_AUTHOR("Andriy Veres");

/*!
 * data, that store big (12 bits) and little (20 bits) 
 * device numbers (driver and device numbers correspondly).
 *
 */
dev_t scull_dev; 

static int scull_init(void)
{
    int alloc_result = 0;
    /*! 
     * Character device registration:
     * 0        - success
     * negative - unsuccess
     */
    printk(KERN_ALERT "SCULL: device registration...\n");
    alloc_result = alloc_chrdev_region(&scull_dev, SCULL_DEV_FIRSTMIRROR, SCULL_DEV_COUNT,SCULL_DEV_NAME);

    /*! 
     * Checking character device registration
     */
    if(alloc_result != 0)
    {
        printk(KERN_ALERT "SCULL: unsuccess device registration error code:%d\n",alloc_result);
    }
    else
    {
#ifdef SCULL_DEBUG
        printk(KERN_ALERT "SCULL: debug, %s %d device: %X MAJOR: %d MINOR: %d count: %u\n",__func__,__LINE__,scull_dev,MAJOR(scull_dev),MINOR(scull_dev),SCULL_DEV_COUNT);
#endif /* SCULL_DEBUG */
        printk(KERN_ALERT "SCULL: success device registration\n");
    }

    return 0;
}

static void scull_exit(void)
{
    /*! 
     * Character device unregistration:
     */
    printk(KERN_ALERT "SCULL: device unregistration...\n");
#ifdef SCULL_DEBUG
    printk(KERN_ALERT "SCULL: debug, %s %d device: %X MAJOR: %d MINOR: %d count: %u\n",__func__,__LINE__,scull_dev,MAJOR(scull_dev),MINOR(scull_dev),SCULL_DEV_COUNT);
#endif /* SCULL_DEBUG */
    unregister_chrdev_region(scull_dev, SCULL_DEV_COUNT);
    printk(KERN_ALERT "SCULL: done device registration\n");
 
}

module_init(scull_init);
module_exit(scull_exit);