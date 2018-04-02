
FRACBITS        =       8
OBJ_FRACBITS    =       16
ROT_FRACBITS    =       8;9
NORMALFRACBITS  =       8;16;24
PAL_FRACBITS    =       8

MAX_SQRT        =       0ffffh

KB              =       1024

ZERO            =       0 shl FRACBITS
ONE             =       1 shl FRACBITS
HALF            =       ONE/2


                global  c pow:proc,c sqrt:proc
                global  c setsqrt:proc
                global  c makehstr:proc,c int2str:proc

