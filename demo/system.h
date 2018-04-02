#ifndef _SYSTEM_H_INCLUDED


#define PIT_INT   0x08
#define KB_INT    0x09
#define VIDEO_INT 0x0a
#define COM2_INT  0x0b
#define COM1_INT  0x0c
#define LPT_INT   0x0d
#define RTC_INT   0x0e
#define MOUSE_INT 0x0f

#define intr __interrupt __far


typedef signed char        sbyte;
typedef unsigned char      ubyte;
typedef signed short int   sword;
typedef unsigned short int uword;
typedef signed long int    sdword;
typedef unsigned long int  udword;

#define SBYTE_MIN  -0x80
#define SBYTE_MAX   0x7f
#define UBYTE_MIN   0x00
#define UBYTE_MAX   0xff
#define SWORD_MIN  -0x8000
#define SWORD_MAX   0x7fff
#define UWORD_MIN   0x0000
#define UWORD_MAX   0xffff
#define SDWORD_MIN -0x80000000
#define SDWORD_MAX  0x7fffffff
#define UDWORD_MIN  0x00000000
#define UDWORD_MAX  0xffffffff


#define _SYSTEM_H_INCLUDED
#endif
