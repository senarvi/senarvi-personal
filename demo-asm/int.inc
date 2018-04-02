
IRQ0            =       08h
IRQ1            =       09h
IRQ2            =       0ah
IRQ3            =       0bh
IRQ4            =       0ch
IRQ5            =       0dh
IRQ6            =       0eh
IRQ7            =       0fh
IRQ8            =       70h
IRQ9            =       71h
IRQ10           =       72h
IRQ11           =       73h
IRQ12           =       74h
IRQ13           =       75h
IRQ14           =       76h
IRQ15           =       77h

IRQ0ON          =       11111110b
IRQ1ON          =       11111101b
IRQ2ON          =       11111011b
IRQ3ON          =       11110111b
IRQ4ON          =       11101111b
IRQ5ON          =       11011111b
IRQ6ON          =       10111111b
IRQ7ON          =       01111111b

IRQ0OFF         =       00000001b
IRQ1OFF         =       00000010b
IRQ2OFF         =       00000100b
IRQ3OFF         =       00001000b
IRQ4OFF         =       00010000b
IRQ5OFF         =       00100000b
IRQ6OFF         =       01000000b
IRQ7OFF         =       10000000b

PIC1            =       21h
PIC2            =       0a1h

;----------------------------------------------------------------------------
TIMERINT        =       IRQ0
KBINT           =       IRQ1
DOSINT          =       21h
VIDEOINT        =       10h


        global  c initirq:proc,c disableirq:proc

