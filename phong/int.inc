
TIMERINT        equ     IRQ0
KBINT           equ     IRQ1
DOSINT          equ     21h

;----------------------------------------------------------------------------
IRQ0            equ     08h
IRQ1            equ     09h
IRQ2            equ     0ah
IRQ3            equ     0bh
IRQ4            equ     0ch
IRQ5            equ     0dh
IRQ6            equ     0eh
IRQ7            equ     0fh
IRQ8            equ     70h
IRQ9            equ     71h
IRQ10           equ     72h
IRQ11           equ     73h
IRQ12           equ     74h
IRQ13           equ     75h
IRQ14           equ     76h
IRQ15           equ     77h

IRQ0ON          equ     11111110b
IRQ1ON          equ     11111101b
IRQ2ON          equ     11111011b
IRQ3ON          equ     11110111b
IRQ4ON          equ     11101111b
IRQ5ON          equ     11011111b
IRQ6ON          equ     10111111b
IRQ7ON          equ     01111111b

IRQ0OFF         equ     00000001b
IRQ1OFF         equ     00000010b
IRQ2OFF         equ     00000100b
IRQ3OFF         equ     00001000b
IRQ4OFF         equ     00010000b
IRQ5OFF         equ     00100000b
IRQ6OFF         equ     01000000b
IRQ7OFF         equ     10000000b

