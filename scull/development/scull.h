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
#define SCULL_QUANTUM_SIZE    1000     /* default scull quantum size in bytes */
#define SCULL_QUANTUM_COUNT   4000    /* # of quantums in 1 scull qset linked list */
#define SCULL_QSET_SIZE       42   /* default quantity of nodes in linked list squll_qset */

/*
 * Struct scull_qset realize scull memory utilization via linked list of nodes (struct scull_qset), each oh them have pointer
 * to the next node (*next), and pointer to pointer to data(**data), that stored in quantums. TODO: make it dynamically allocated
 */
struct scull_qset {
void              ** data;       /* pointer to array of pointers to squll quantums */
struct scull_qset *  p_next;     /* pointer to next node in linked list of scull_qset`s */
struct scull_qset *  p_previous; /* pointer to previous node in linked list of scull_qset`s */
}scull_qset;

/*
 * struct scull_dev used for storing device numbers, transferring
 * internal driver data between driver work functions, like open(),
 * relese(), read() and write() with "scul_"l preffix in name correspondly
 */

struct scull_dev {
dev_t          dev;           /* store big (12 bits) and little (20 bits) device number */
unsigned int   number_opens;  /* number of device opens */
char         * buffer;        /* pointer to character buffer for storing */
ssize_t        buffer_size;   /* wrote bytes in buffer by characters */
unsigned int   qset_size;     /* # of nodes (squll_qsets) in scull linked list */
unsigned int   quantum_size;  /* size in bytes of quantum */
unsigned int   quantum_count; /* count of quantum pointers in 1 node in scull_qset`s linked list*/
struct scull_qset * qset_head; /* pointer to linked list */
}scull_dev;

/* Uncomment for debug */
#define SCULL_DEBUG

#endif /* SCULL_CONFIG_H */