
PIT0PORT        equ     40h+0
PIT1PORT        equ     40h+1
PIT2PORT        equ     40h+2
PITCPORT        equ     40h+3

PITSEL0         equ     0 shl 6
PITSEL1         equ     1 shl 6
PITSEL2         equ     2 shl 6
PITLATCH        equ     0 shl 4
PITLOADLSB      equ     1 shl 4
PITLOADMSB      equ     2 shl 4
PITLOADALL      equ     3 shl 4
PITMODE0        equ     0 shl 1
PITMODE1        equ     1 shl 1
PITMODE2        equ     2 shl 1
PITMODE3        equ     3 shl 1
PITMODE4        equ     4 shl 1
PITMODE5        equ     5 shl 1
PITBIN16        equ     0 shl 0
PITBCD4         equ     1 shl 0

PITFREQ         equ     1234ddh

CONTROLW        equ     PITSEL0+PITLOADALL+PITMODE2+PITBIN16

