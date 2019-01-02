/**-----------------------------------------------+
 * @author  Andriy Veres <andriy-pilot@ukr.net>   |
 * @version 0.1                                   |
 * @license GPL v3                                |
 -------------------------------------------------+*/

#ifndef KLCD_CONFIG_H
#define KLCD_CONFIG_H

#include <linux/types.h>  /* struct dev_t    */
#include <linux/fs.h>     /* file operations */
#include <linux/cdev.h>   /* struct cdev     */

#define KLCD_DEV_COUNT       1
#define KLCD_DEV_NAME        "klcd"
#define KLCD_DEV_FIRSTMIRROR 0
#define KLCD_BUFFER_SIZE     10 /* size of character buffer */

/* Hardcoded GPIO pins configuration */
#define LCD_RS  2
#define LCD_RW  3
#define LCD_E   4
#define LCD_DB0 17
#define LCD_DB1 27
#define LCD_DB2 22
#define LCD_DB3 10
#define LCD_DB4 9
#define LCD_DB5 11
#define LCD_DB6 5
#define LCD_DB7 6

/* Hardcodec GDB0-7 bits mask */
#define LCD_DB0_BITMASK 0b00000001
#define LCD_DB1_BITMASK 0b00000010
#define LCD_DB2_BITMASK 0b00000100
#define LCD_DB3_BITMASK 0b00001000
#define LCD_DB4_BITMASK 0b00010000
#define LCD_DB5_BITMASK 0b00100000
#define LCD_DB6_BITMASK 0b01000000
#define LCD_DB7_BITMASK 0b10000000

/* GPIO DIRECTION */
#define OUTPUT  1
#define INPUT   0

/*Delay configuration */
#define DELAY mdelay(20) /* TODO: find minimum value */

/*
 * struct klcd_dev used for storing device numbers, transferring
 * internal driver data between driver work functions, like open(),
 * relese(), read() and write() with "scul_"l preffix in name correspondly
 */

struct klcd_dev {
dev_t          dev;            /* store big (12 bits) and little (20 bits) device number */
unsigned int   number_opens;   /* number of device opens */
char         * buffer;         /* pointer to character buffer for storing */
ssize_t        buffer_size;    /* wrote bytes in buffer by characters */
}klcd_dev;

/* Uncomment for debug */
#define KLCD_DEBUG

/* Change prints debug level */
#define KLCD_DEBUG_LEVEL KERN_ALERT

#endif /* KLCD_CONFIG_H */