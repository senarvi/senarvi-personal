
                .486p
                locals


NUMVERTICES     equ     58
NUMFACES        equ     112

MAXXY           equ     1121
MINZ            equ     -1977


                public  vertextable,facetable


DATA            segment para public use32 'DATA'

vertextable     dw       -146,  987, 1274
                dw       -146, -986, 1274
                dw        203,  987, 1274
                dw       -191,  466,  487
                dw       -191, -465,  487
                dw       -150,  987, 1213
                dw       -150,  466, 1213
                dw       -150, -465, 1213
                dw       -150, -986, 1213
                dw        474,  466,  443
                dw        474, -465,  443
                dw       1122,  466, -171
                dw       1122, -465, -171
                dw       1122,  466,-1407
                dw       1122, -465,-1407
                dw        557,  466,-1977
                dw        557, -465,-1977
                dw       -501,  466,-1977
                dw       -501, -465,-1977
                dw      -1121,  466,-1362
                dw      -1121, -465,-1362
                dw      -1121,  466, -659
                dw      -1121, -465, -659
                dw       -146,  466, -659
                dw       -146, -465, -659
                dw       -146,  466,-1191
                dw       -146, -465,-1191
                dw        203,  466,-1191
                dw        203,  466, -387
                dw        203, -465, -387
                dw       -457,  466, -304
                dw       -457, -465, -304
                dw      -1121,  466,  260
                dw      -1121, -465,  260
                dw      -1121,  987, 1396
                dw      -1121,  466, 1213
                dw      -1121, -465, 1213
                dw      -1121, -986, 1396
                dw      -1121, -986, 1213
                dw      -1121,  987, 1213
                dw       -545,  987, 1977
                dw       -545, -986, 1977
                dw        513,  987, 1977
                dw        513, -986, 1977
                dw       1078,  987, 1451
                dw       1078, -986, 1451
                dw       1078,  466,  747
                dw       1078, -465,  747
                dw       1078,  987, 1213
                dw       1078,  466, 1213
                dw       1078, -465, 1213
                dw       1078, -986, 1213
                dw        203,  466,  747
                dw        203, -465,  747
                dw        203,  466, 1213
                dw        203, -465, 1213
                dw        203, -986, 1213
                dw        203,  987, 1213

facetable       dw         45,    0,    2
                dw          1,    0,   45
                dw          3,    7,    4
                dw          0,    1,    5
                dw          6,    5,    1
                dw          6,    1,    7
                dw          3,    6,    7
                dw          1,    8,    7
                dw          4,    9,    3
                dw          4,   10,    9
                dw         12,   11,    9
                dw         12,    9,   10
                dw         11,   12,   13
                dw         12,   14,   13
                dw         16,   15,   14
                dw         14,   15,   13
                dw         18,   17,   15
                dw         18,   15,   16
                dw         19,   17,   18
                dw         18,   20,   19
                dw         22,   21,   20
                dw         20,   21,   19
                dw         24,   23,   21
                dw         22,   24,   21
                dw         23,   26,   25
                dw         26,   23,   24
                dw         14,   27,   25
                dw         14,   25,   26
                dw         27,   29,   28
                dw         14,   29,   27
                dw         28,   29,   30
                dw         29,   31,   30
                dw         33,   32,   30
                dw         31,   33,   30
                dw         36,   34,   35
                dw         32,   36,   35
                dw         36,   37,   34
                dw         37,   36,   38
                dw         35,   34,   39
                dw         33,   36,   32
                dw         41,   40,   37
                dw         37,   40,   34
                dw         43,   42,   40
                dw         43,   40,   41
                dw         45,   44,   43
                dw         44,   42,   43
                dw         48,   44,   45
                dw         49,   48,   45
                dw         46,   49,   45
                dw         50,   47,   46
                dw         46,   45,   50
                dw         50,   45,   51
                dw         46,   53,   52
                dw         47,   53,   46
                dw         45,    2,   54
                dw         54,    2,   57
                dw         52,   55,   54
                dw         45,   54,   55
                dw         56,   45,   55
                dw         53,   55,   52
                dw         10,    4,   14
                dw         10,   14,   12
                dw         17,   11,   13
                dw         17,   13,   15
                dw         14,   20,   16
                dw         16,   20,   18
                dw         23,   17,   19
                dw         19,   21,   23
                dw         20,   24,   22
                dw         17,   23,   25
                dw         24,   20,   26
                dw         20,   14,   26
                dw         11,   17,   27
                dw         25,   27,   17
                dw         28,    9,   27
                dw         11,   27,    9
                dw          9,   30,    3
                dw          9,   28,   30
                dw         32,    3,   30
                dw         35,    5,    6
                dw         35,    6,    3
                dw         35,    3,   32
                dw         36,    4,    7
                dw          4,   36,   14
                dw         14,   36,   29
                dw         36,   31,   29
                dw         31,   36,   33
                dw         36,    7,    8
                dw          8,   38,   36
                dw         38,    8,    1
                dw         37,   38,    1
                dw         34,    0,    5
                dw          5,   39,   34
                dw         35,   39,    5
                dw         41,    1,   45
                dw         41,   37,    1
                dw         44,    0,   34
                dw          0,   44,    2
                dw         44,   34,   40
                dw         40,   42,   44
                dw          2,   44,   48
                dw         41,   45,   43
                dw         49,   46,   52
                dw         48,   57,    2
                dw         48,   49,   54
                dw         54,   49,   52
                dw         55,   47,   50
                dw         56,   50,   51
                dw         55,   50,   56
                dw         51,   45,   56
                dw         54,   57,   48
                dw         47,   55,   53

zoffset         dw      OBJECTMAXXY*LENSDIST/GRAPHMAXXY-OBJECTMINZ

DATA            ends
                end
