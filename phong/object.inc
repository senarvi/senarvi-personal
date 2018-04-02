
include         project.inc              ; LENSDIST
include         video.inc                ; GRAPHMAXXY


ST_VERTICES     equ     58
ST_FACES        equ     112
TI_VERTICES     equ     16
TI_FACES        equ     28
AC_VERTICES     equ     60
AC_FACES        equ     120
DUCK_VERTICES   equ     270
DUCK_FACES      equ     516
MAX_VERTICES    equ     DUCK_VERTICES
MAX_FACES       equ     DUCK_FACES

ST_MAXXY        equ     1121
ST_MINZ         equ     -1977
ST_ZOFFSET      equ     ST_MAXXY*LENSDIST/GRAPHMAXXY-ST_MINZ
TI_MAXXY        equ     986
TI_MINZ         equ     -2000
TI_ZOFFSET      equ     TI_MAXXY*LENSDIST/GRAPHMAXXY-TI_MINZ
AC_MAXXY        equ     1274
AC_MINZ         equ     -2000
AC_ZOFFSET      equ     AC_MAXXY*LENSDIST/GRAPHMAXXY-AC_MINZ
DUCK_MAXXY      equ     1891
DUCK_MINZ       equ     -1920
DUCK_ZOFFSET    equ     DUCK_MAXXY*LENSDIST/GRAPHMAXXY-DUCK_MINZ;-1500

