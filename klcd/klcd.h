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

#define UNDEFINED_CMD 0x2A
#define UNDEFINED_VAL 0x00


typedef klcd_cmd_t u_int8_t;

struct char_to_cmd{
    char       symbol;
    klcd_cmd_t code;
}

struct utf8_to_cmd{
    u_int16_t  symbol;
    klcd_cmd_t code;
}

struct utf32_to_cmd{
    u_int32_t  symbol;
    klcd_cmd_t code;
}


/* ASCII to KLCD SYMBOLS */
struct char_to_cmd ascii_to_wh1602b_table[128] {
{ 0x00,  UNDEFINED_CMD },
{ 0x01,  UNDEFINED_CMD },
{ 0x02,  UNDEFINED_CMD },
{ 0x03,  UNDEFINED_CMD },
{ 0x04,  UNDEFINED_CMD },
{ 0x05,  UNDEFINED_CMD },
{ 0x06,  UNDEFINED_CMD },
{ 0x07,  UNDEFINED_CMD },
{ 0x08,  UNDEFINED_CMD },
{ 0x09,  UNDEFINED_CMD },
{ 0x0A,  UNDEFINED_CMD },
{ 0x0B,  UNDEFINED_CMD },
{ 0x0C,  UNDEFINED_CMD },
{ 0x0D,  UNDEFINED_CMD },
{ 0x0E,  UNDEFINED_CMD },
{ 0x0F,  UNDEFINED_CMD },
{ 0x11,  UNDEFINED_CMD },
{ 0x12,  UNDEFINED_CMD },
{ 0x13,  UNDEFINED_CMD },
{ 0x14,  UNDEFINED_CMD },
{ 0x15,  UNDEFINED_CMD },
{ 0x16,  UNDEFINED_CMD },
{ 0x17,  UNDEFINED_CMD },
{ 0x18,  UNDEFINED_CMD },
{ 0x19,  UNDEFINED_CMD },
{ 0x1A,  UNDEFINED_CMD },
{ 0x1B,  UNDEFINED_CMD },
{ 0x1C,  UNDEFINED_CMD },
{ 0x1D,  UNDEFINED_CMD },
{ 0x1E,  UNDEFINED_CMD },
{ 0x1F,  UNDEFINED_CMD },
{  ' ',  0x20          },
{  '!',  0x21          },
{  '\"', 0x22          },
{  '#',  0x23          },
{  '$',  0x24          },
{  '%',  0x25          },
{  '&',  0x26          },
{  '\'', 0x27          },
{  '(',  0x28          },
{  ')',  0x29          },
{  '*',  0x2A          },
{  '+',  0x2B          },
{  ',',  0x2C          },
{  '-',  0x2D          },
{  '.',  0x2E          },
{  '/',  0x2F          },
{  '0',  0x30          },
{  '1',  0x31          },
{  '2',  0x32          },
{  '3',  0x33          },
{  '4',  0x34          },
{  '5',  0x35          },
{  '6',  0x36          },
{  '7',  0x37          },
{  '8',  0x38          },
{  '9',  0x39          },
{  ':',  0x3A          },
{  ';',  0x3B          },
{  '<',  0x3C          },
{  '=',  0x3D          },
{  '>',  0x3E          },
{  '?',  0x3F          },
{  '@',  0x40          },
{  'A',  0x41          },
{  'B',  0x42          },
{  'C',  0x43          },
{  'D',  0x44          },
{  'E',  0x45          },
{  'F',  0x46          },
{  'G',  0x47          },
{  'H',  0x48          },
{  'I',  0x49          },
{  'J',  0x4A          },
{  'K',  0x4B          },
{  'L',  0x4C          },
{  'M',  0x4D          },
{  'N',  0x4E          },
{  'O',  0x4F          },
{  'P',  0x50          },
{  'Q',  0x51          },
{  'R',  0x52          },
{  'S',  0x53          },
{  'T',  0x54          },
{  'U',  0x55          },
{  'V',  0x56          },
{  'W',  0x57          },
{  'X',  0x58          },
{  'Y',  0x59          },
{  'Z',  0x5A          },
{  '[',  0x5B          },
{  '\\', UNDEFINED_CMD },
{  ']',  0x5D          },
{  '^',  0x5E          },
{  '_',  0x5F          },
{  '`',  0x60          },
{  'a',  0x61          },
{  'b',  0x62          },
{  'c',  0x63          },
{  'd',  0x64          },
{  'e',  0x65          },
{  'f',  0x66          },
{  'g',  0x67          },
{  'h',  0x68          },
{  'I',  0x69          },
{  'j',  0x6A          },
{  'k',  0x6B          },
{  'l',  0x6C          },
{  'm',  0x6D          },
{  'n',  0x6E          },
{  'o',  0x6F          },
{  'p',  0x70          },
{  'q',  0x71          },
{  'r',  0x72          },
{  's',  0x73          },
{  't',  0x74          },
{  'u',  0x75          },
{  'v',  0x76          },
{  'w',  0x77          },
{  'x',  0x78          },
{  'y',  0x79          },
{  'z',  0x7A          },
{  '{',  UNDEFINED_CMD },
{  '|',  UNDEFINED_CMD },
{  '}',  UNDEFINED_CMD },
{  '~',  UNDEFINED_CMD },
{ 0x7F,  UNDEFINED_CMD }};


/* 0x80 - 0x9F - UNDEFINED commands */

struct utf8_to_cmd utf8_cyrylic_to_wh1602b_table[] {
{ 0x0401, 0xA2      }, /* cyrillic capital letter IO */
{ 0x0402, UNDEFINED_CMD },
{ 0x0403, UNDEFINED_CMD },
{ 0x0404, UNDEFINED_CMD },
{ 0x0405, UNDEFINED_CMD },
{ 0x0406, UNDEFINED_CMD },
{ 0x0407, UNDEFINED_CMD },
{ 0x0408, UNDEFINED_CMD },
{ 0x0409, UNDEFINED_CMD },
{ 0x040A, UNDEFINED_CMD },
{ 0x040B, UNDEFINED_CMD },
{ 0x040C, UNDEFINED_CMD },
{ 0x040D, UNDEFINED_CMD },
{ 0x040E, UNDEFINED_CMD },
{ 0x040F, UNDEFINED_CMD },
{ 0x0410, 0x41          }, /* cyrillic capital letter A */
{ 0x0411, 0xA0          }, /* cyrillic capital letter BE */
{ 0x0411, 0x42          }, /* cyrillic capital letter VE */
{ 0x0413, 0xA1          }, /* cyrillic capital letter GHE */
{ 0x0414, 0xE0          }, /* cyrillic capital letter DE */
{ 0x0415, 0x45          }, /* cyrillic capital letter IE */
{ 0x0416, 0xA3          }, /* cyrillic capital letter ZHE */
{ 0x0417, 0xA4          }, /* cyrillic capital letter ZE */
{ 0x0418, 0xA5          }, /* cyrillic capital letter I */
{ 0x0419, 0xA6          }, /* cyrillic capital letter SHORT I */
{ 0x041A, 0x4B          }, /* cyrillic capital letter KA */
{ 0x041B, 0xA7          }, /* cyrillic capital letter EL */
{ 0x041C, 0x4D          }, /* cyrillic capital letter EM */
{ 0x041D, 0x48          }, /* cyrillic capital letter EN */
{ 0x041E, 0x4F          }, /* cyrillic capital letter O */
{ 0x041F, 0xA8          }, /* cyrillic capital letter PE */
{ 0x0420, 0x50          }, /* cyrillic capital letter ER */
{ 0x0421, 0x43          }, /* cyrillic capital letter ES */
{ 0x0422, 0x54          }, /* cyrillic capital letter TE */
{ 0x0423, 0xA9          }, /* cyrillic capital letter U */
{ 0x0424, 0xAA          }, /* cyrillic capital letter EF */
{ 0x0425, 0x58          }, /* cyrillic capital letter HA */
{ 0x0426, 0xE1          }, /* cyrillic capital letter TSE */
{ 0x0427, 0xAB          }, /* cyrillic capital letter CHE */
{ 0x0428, 0xAC          }, /* cyrillic capital letter SHA */
{ 0x0429, 0xE2          }, /* cyrillic capital letter SHCHA */
{ 0x042A, 0xAD          }, /* cyrillic capital letter HARD sign */
{ 0x042B, 0xAE          }, /* cyrillic capital letter YERU */
{ 0x042C, UNDEFINED_CMD }, /* cyrillic capital letter SOFT sign */
{ 0x042D, 0xAF          }, /* cyrillic capital letter E */
{ 0x042E, 0xB0          }, /* cyrillic capital letter YU */
{ 0x042F, 0xB1          }, /* cyrillic capital letter YA */
{ 0x0430, 0x61          }, /* cyrillic small letter A */
{ 0x0431, 0xB2          }, /* cyrillic small letter BE */
{ 0x0431, 0xB3          }, /* cyrillic small letter VE */
{ 0x0433, 0xB4          }, /* cyrillic small letter GHE */
{ 0x0434, 0xE3          }, /* cyrillic small letter DE */
{ 0x0435, 0x65          }, /* cyrillic small letter IE */
{ 0x0436, 0xB6          }, /* cyrillic small letter ZHE */
{ 0x0437, 0xB7          }, /* cyrillic small letter ZE */
{ 0x0438, 0xB8          }, /* cyrillic small letter I */
{ 0x0439, 0xB9          }, /* cyrillic small letter SHORT I */
{ 0x043A, 0xBA          }, /* cyrillic small letter KA */
{ 0x043B, 0xBB          }, /* cyrillic small letter EL */
{ 0x043C, 0xBC          }, /* cyrillic small letter EM */
{ 0x043D, 0xBD          }, /* cyrillic small letter EN */
{ 0x043E, 0X6F          }, /* cyrillic small letter O */
{ 0x043F, 0XBE          }, /* cyrillic small letter PE */
{ 0x0440, 0x73          }, /* cyrillic small letter ER */
{ 0x0441, 0X63          }, /* cyrillic small letter ES */
{ 0x0442, 0xBF          }, /* cyrillic small letter TE */
{ 0x0443, 0X79          }, /* cyrillic small letter U */
{ 0x0444, 0XE4          }, /* cyrillic small letter EF */
{ 0x0445, 0X78          }, /* cyrillic small letter HA */
{ 0x0446, 0xE1          }, /* cyrillic small letter TSE */
{ 0x0447, 0XC0          }, /* cyrillic small letter CHE */
{ 0x0448, 0xC1          }, /* cyrillic small letter SHA */
{ 0x0449, 0xE6          }, /* cyrillic small letter SHCHA */
{ 0x044A, 0xAD          }, /* cyrillic small letter HARD sign */
{ 0x044B, 0xC3          }, /* cyrillic small letter YERU */
{ 0x044C, 0xC3          }, /* cyrillic small letter SOFT sign */
{ 0x044D, 0xC5          }, /* cyrillic small letter E */
{ 0x044E, 0xC6          }, /* cyrillic small letter YU */
{ 0x044F, 0xC7          }, /* cyrillic small letter YA */
{ 0x0450, UNDEFINED_CMD }, 
{ 0x0451, 0xB5          }}; /*cyrillic small letter IO */


/* UTF-16  TODO: find coding correspondances for UNDEFINED  */
struct utf32_to_cmd utf32_symbol_to_wh1602b_table[] {
{ 0xA5,          0x5C },  /* Japan Yena, China Yuahn */
{ UNDEFINED_VAL, 0x7B },  /* upward 10 */
{ UNDEFINED_VAL, 0x7C },  /* upward 12 */
{ UNDEFINED_VAL, 0x7D },  /* upward 15 */
{ 0x21B2,        0x7E },  /* downwards arrow with tip leftwards */
{ UNDEFINED_VAL, 0x7F },  /* possible Greek ksi */
{ 0x226A,        0xC8 },  /* much less-than */
{ 0x226B,        0xC9 },  /* much greater-than */
{ 0x201E,        0xCA },  /* left double low-9 qotation */
{ 0x0022,        0xCB },  /* right couble quotation */
{ 0x02116,       0xCC },  /* numero sign */
{ 0x00BF,        0xCD },  /* inverted question */
{ 0x2A0D,        0xCE },  /* finite part integral */
{ 0x00A3,        0xCF },  /* pound */
{ UNDEFINED_VAL, 0xD0 },
{ UNDEFINED_VAL, 0xD1 },
{ UNDEFINED_VAL, 0xD2 },
{ UNDEFINED_VAL, 0xD3 },
{ UNDEFINED_VAL, 0xD4 },
{ 0x231B,        0xD5 },  /* hourglass */
{ UNDEFINED_VAL, 0xD6 },
{ 0x2160,        0xD7 },  /* roman numeral one */
{ 0x2161,        0xD8 },  /* roman numeral two */
{ 0x2B61,        0xD9 },  /* upwards triangle-headed arrow */
{ 0x2b63,        0xDA },  /* downwards triangle-headed arrow */
{ 0x2B70,        0xDB },  /* leftwards triangle-headed arrow to bar */
{ 0x2B72,        0xDC },  /* rightwards triangle-headed arrow to bar */
{ 0x2B64,        0xDD },  /* left right triangle-headed arrow */
{ UNDEFINED_VAL, 0xDE },
{ UNDEFINED_VAL, 0xDF },
{ 0x00B4,        0xE7 },  /* acute aaccent */
{ 0x0308,        0xE8 },  /* combining diaeresis */
{ 0x02DC,        0xE9 },  /* small tilde */
{ 0x00E9,        0xEA },  /* latin small letter E with acute */
{ 0x00E7,        0xEB },  /* latin small letter C with cedilla*/
{ UNDEFINED_VAL, 0xEC },
{ 0x1F514,       0xED },  /* bell */
{ 0x2B21,        0xEE },  /* white hexagon */
{ 0x00B0,        0xEF },  /* Degree Sign */
{ 0x00BC,        0xF0 },  /* Vulgar Fraction One Quarter */
{ 0x2153,        0xF1 },  /* Vulgar Fraction One Third */
{ 0x00BD,        0xF2 },  /* Vulgar Fraction One Half */
{ 0x00BE,        0xF3 },  /* Vulgar Fraction Three Quarters */
{ UNDEFINED_VAL, 0xF4 },
{ 0x10FB,        0xF5 },  /* Georgian paragraph separator */
{ UNDEFINED_VAL, 0xF6 },
{ UNDEFINED_VAL, 0xF7 },
{ UNDEFINED_VAL, 0xF8 },
{ UNDEFINED_VAL, 0xF9 },
{ UNDEFINED_VAL, 0xFA },
{ UNDEFINED_VAL, 0xFB },
{ UNDEFINED_VAL, 0xFC },
{ 0x00A7,        0xFD },  /* Section Sign */
{ UNDEFINED_VAL, 0xFE },
{ 0x25AE,        0xFF }}; /* Black Vertical Rectangle */

inline klcd_cmd_t ascii_to_wh1602b(char symbol);
inline klcd_cmd_t utf8_cyrilic_to_wh1602b(u_int8_t symbol);
inline klcd_cmd_t utf32_symbol_to_wh1602b(u_int32_t symbol);


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
