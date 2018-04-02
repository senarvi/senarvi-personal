
TEXTMODE        equ     03h
TEXTWIDTH       equ     80
TEXTHEIGHT      equ     25
CR              equ     0Dh
LF              equ     0Ah

GRAPHMODE       equ     13h
GRAPHWIDTH      equ     320
GRAPHHEIGHT     equ     200
GRAPHBUFFER_X   equ     GRAPHWIDTH
GRAPHBUFFER_Y   equ     GRAPHHEIGHT
GRAPHCOLORS     equ     256
GRAPHMAXXY      equ     GRAPHHEIGHT/2

FLAME_PAL       =       0
FLAME_COLORS    =       256

SCREENASPECT    =       (4 shl FRACBITS)/3
GRAPHASPECT     =       (GRAPHWIDTH shl FRACBITS)/GRAPHHEIGHT

