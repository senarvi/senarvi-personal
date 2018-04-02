
                .486p
                locals


NUMVERTICES     equ     16
NUMFACES        equ     28

MAXXY           equ     986
MINZ            equ     -2000


                public  vertextable,facetable


DATA            segment para public use32 'DATA'

vertextable     dw        477, -986, 2000
                dw        477,  987, 2000
                dw        477,  987, 1213
                dw        477,  466, 1213
                dw        477,  466,-2000
                dw        477, -465,-1999
                dw        477, -465, 1213
                dw        477, -986, 1213
                dw       -476, -986, 2000
                dw       -476,  987, 2000
                dw       -476,  987, 1213
                dw       -476,  466, 1213
                dw       -476,  466,-2000
                dw       -476, -465,-1999
                dw       -476, -465, 1213
                dw       -476, -986, 1213

facetable       dw          0,    7,    6
                dw          1,    0,    6
                dw          6,    5,    4
                dw          6,    4,    3
                dw          1,    6,    3
                dw          2,    1,    3
                dw          0,    1,    9
                dw          0,    9,    8
                dw          1,    2,   10
                dw          1,   10,    9
                dw          2,    3,   11
                dw          2,   11,   10
                dw          3,    4,   12
                dw          3,   12,   11
                dw          4,    5,   13
                dw          4,   13,   12
                dw          5,    6,   14
                dw          5,   14,   13
                dw          6,    7,   15
                dw          6,   15,   14
                dw          7,    0,    8
                dw          7,    8,   15
                dw         14,   15,    8
                dw         14,    8,    9
                dw         12,   13,   14
                dw         11,   12,   14
                dw         11,   14,    9
                dw         11,    9,   10

zoffset         dw      OBJECTMAXXY*LENSDIST/GRAPHMAXXY-OBJECTMINZ

DATA            ends
                end
