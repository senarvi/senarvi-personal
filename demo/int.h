#ifndef _INT_H_INCLUDED


#define PIC1_IO_BASE 0x20
#define PIC2_IO_BASE 0xa0

#define PIC_ICW1 0
#define PIC_ICW2 1
#define PIC_ICW3 1
#define PIC_ICW4 1

#define PIC_OCW1 1
#define PIC_OCW2 1
#define PIC_OCW3 1

#define PIC_MASK PIC_OCW1

#define PIC_REQUEST     0
#define PIC_INSERVICE   0


#define PIT_IRQ     0
#define KB_IRQ      1
#define VIDEO_IRQ   2
#define COM2_IRQ    3
#define COM1_IRQ    4
#define LPT_IRQ     5
#define RTC_IRQ     6
#define MOUSE_IRQ   7


#define _INT_H_INCLUDED
#endif
