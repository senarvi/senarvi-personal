
        include "math.inc"

;----------------------------------------------------------------------------
        struc   spixel
x       dd      ?
y       dd      ?
        ends

        struc   squad
a       spixel  ?
b       spixel  ?
        ends

        struc   scolor
r       dd      ?
g       dd      ?
b       dd      ?
        ends

        struc   svideomem
r       db      ?
g       db      ?
b       db      ?
        ends

        struc   smaterial
ambient scolor  ?
diffuse scolor  ?
specular scolor ?
shine   dd      ?
        ends

        struc   smap
u       dd      ?
v       dd      ?
        ends

;----------------------------------------------------------------------------
TEXTMODE        =       03h
TEXTWIDTH       =       80
TEXTHEIGHT      =       25
CR              =       0Dh
LF              =       0Ah

GRAPHMODE       =       13h
GRAPHWIDTH      =       320
GRAPHHEIGHT     =       200
GRAPHBUFFER_X   =       GRAPHWIDTH
GRAPHBUFFER_Y   =       GRAPHHEIGHT
GRAPHCOLORS     =       256
GRAPHMAXXY      =       GRAPHHEIGHT/2

NUM_BUFFERS     =       2

FLAME_PAL       =       0
FLAME_COLORS    =       256

SCREENASPECT    =       (4 shl FRACBITS)/3
GRAPHASPECT     =       (GRAPHWIDTH shl FRACBITS)/GRAPHHEIGHT

;----------------------------------------------------------------------------
TEXMAP_GETAREA  =       1

;----------------------------------------------------------------------------
        global  screenbufoff:dword:NUM_BUFFERS
        global  prevbufoff:dword,destbufoff:dword,destbufnum:dword

        global  textx:dword,texty:dword
        global  bufferarea:squad

        global  c setwavepal:proc,c setflamepal:proc,c settunnelpal:proc
        global  c setphongpal:proc,c setlinearpal:proc

        global  c writestr:proc
        global  c cleartext:proc
        global  c clearbuffers:proc,c clearvideo:proc
        global  c clearbuffer:proc,c copybuffer:proc
        global  c cleararea:proc,c copyarea:proc
        global  c motionblur:proc

        global  duckmat:smaterial,rockmat:smaterial
        global  wavemat:smaterial

