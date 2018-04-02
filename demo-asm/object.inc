
                include "vector.inc"
                include "video.inc"

;----------------------------------------------------------------------------
ST_VERTICES     =       58
ST_FACES        =       112
TI_VERTICES     =       16
TI_FACES        =       28
AC_VERTICES     =       60
AC_FACES        =       120
DUCK_VERTICES   =       270
DUCK_FACES      =       516
ROCK_VERTICES   =       396
ROCK_FACES      =       768
OBJ11_VERTICES  =       327
OBJ11_FACES     =       621
OBJ12_VERTICES  =       712
OBJ12_FACES     =       1408
MAX_VERTICES    =       OBJ12_VERTICES
MAX_FACES       =       OBJ12_FACES

ST_MAXXY        =       1121
ST_MINZ         =       -1977
ST_ZOFFSET      =       ST_MAXXY*LENSDIST/GRAPHMAXXY-ST_MINZ
TI_MAXXY        =       986
TI_MINZ         =       -2000
TI_ZOFFSET      =       TI_MAXXY*LENSDIST/GRAPHMAXXY-TI_MINZ
AC_MAXXY        =       1274
AC_MINZ         =       -2000
AC_ZOFFSET      =       AC_MAXXY*LENSDIST/GRAPHMAXXY-AC_MINZ
DUCK_MAXXY      =       1891
DUCK_MINZ       =       -1920
DUCK_ZOFFSET    =       DUCK_MAXXY*LENSDIST/GRAPHMAXXY-DUCK_MINZ
ROCK_MAXXY      =       140
ROCK_MINZ       =       -140
ROCK_ZOFFSET    =       ROCK_MAXXY*LENSDIST/GRAPHMAXXY-ROCK_MINZ

;----------------------------------------------------------------------------
                global  st_vertextab:svect:ST_VERTICES
                global  st_facetab:sface:ST_FACES

                global  ti_vertextab:svect:TI_VERTICES
                global  ti_facetab:sface:TI_FACES

                global  ac_vertextab:svect:AC_VERTICES
                global  ac_facetab:sface:AC_FACES

                global  duck_vertextab:svect:DUCK_VERTICES
                global  duck_facetab:sface:DUCK_FACES

                global  rock_vertextab:svect:ROCK_VERTICES
                global  rock_facetab:sface:ROCK_FACES
                global  rock_maptab:smap:ROCK_VERTICES

                global  obj11_vertextab:svect:OBJ11_VERTICES
                global  obj11_facetab:sface:OBJ11_FACES
                global  obj11_maptab:smap:OBJ11_VERTICES

                global  obj12_vertextab:svect:OBJ12_VERTICES
                global  obj12_facetab:sface:OBJ12_FACES
                global  obj12_maptab:smap:OBJ12_VERTICES

