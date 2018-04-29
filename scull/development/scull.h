/**-----------------------------------------------+
 * @author  Andriy Veres <andriy-pilot@ukr.net>   |
 * @version 0.1                                   |
 * @license GPL v3                                |
 -------------------------------------------------+*/

#ifndef SCULL_CONFIG_H
#define SCULL_CONFIG_H

#include <linux/types.h>  /* struct dev_t    */
#include <linux/fs.h>     /* file operations */
#include <linux/cdev.h>   /* struct cdev     */

#define SCULL_DEV_COUNT       1
#define SCULL_DEV_NAME        "scull"
#define SCULL_DEV_FIRSTMIRROR 0
#define SCULL_BUFFER_SIZE     132000 /* size of character buffer */


/*
 * struct scull_dev used for storing device numbers, transferring
 * internal driver data between driver work functions, like open(),
 * relese(), read() and write() with "scul_"l preffix in name correspondly
 */

struct scull_dev {
dev_t          dev;            /* store big (12 bits) and little (20 bits) device number */
unsigned int   number_opens;   /* number of device opens */
char         * buffer;         /* pointer to character buffer for storing */
ssize_t        buffer_size;    /* wrote bytes in buffer by characters */
}scull_dev;

/* Uncomment for debug */
/* #define SCULL_DEBUG */

#endif /* SCULL_CONFIG_H */