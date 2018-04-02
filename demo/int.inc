;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PIC1_IO_BASE    =       020h
PIC2_IO_BASE    =       0a0h

PIC_ICW1        =       0
PIC_ICW2        =       1
PIC_ICW3        =       1
PIC_ICW4        =       1

PIC_OCW1        =       1
PIC_MASK        =       PIC_OCW1
PIC_OCW2        =       0
PIC_OCW3        =       0

PIC_REQUEST     =       0
PIC_INSERVICE   =       0

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PITIRQ          =       0
KBIRQ           =       1
VIDEOIRQ        =       2
COM2IRQ         =       3
COM1IRQ         =       4
LPTIRQ          =       7
RTCIRQ          =       8
MOUSEIRQ        =       12

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
        global  c setintvec:proc
        global  c getpicmappings:proc
        global  c enableirq:proc,c disableirq:proc
        global  c initirq:proc

