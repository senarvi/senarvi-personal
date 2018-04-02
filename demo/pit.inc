
PIT0PORT        =       40h+0
PIT1PORT        =       40h+1
PIT2PORT        =       40h+2
PITCPORT        =       40h+3

PITSEL0         =       0 shl 6
PITSEL1         =       1 shl 6
PITSEL2         =       2 shl 6
PITLATCH        =       0 shl 4
PITLOADLSB      =       1 shl 4
PITLOADMSB      =       2 shl 4
PITLOADALL      =       3 shl 4
PITMODE0        =       0 shl 1
PITMODE1        =       1 shl 1
PITMODE2        =       2 shl 1
PITMODE3        =       3 shl 1
PITMODE4        =       4 shl 1
PITMODE5        =       5 shl 1
PITBIN16        =       0 shl 0
PITBCD4         =       1 shl 0

PITFREQ         =       1234ddh

CONTROLW        =       PITSEL0+PITLOADALL+PITMODE2+PITBIN16
