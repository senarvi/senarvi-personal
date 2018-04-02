
                include "video.inc"
                include "math.inc"

;----------------------------------------------------------------------------
        struc   svect
x       dd      ?
y       dd      ?
z       dd      ?
        ends

        struc   spoly
a       svect   ?
b       svect   ?
c       svect   ?
        ends

        struc   sface
a       dd      ?
b       dd      ?
c       dd      ?
        ends

        struc   sangle
a       dd      ?
b       dd      ?
        ends

        struc   svdata
coord   svect   ?                       ; Coordinates
scr     spixel  ?                       ; Screen pixel
env     smap    ?                       ; Environment map coordinates
tex     smap    ?                       ; Texture map coordinates
        ends

        struc   spdata
a       svdata  ?
b       svdata  ?
c       svdata  ?
        ends

        struc   sedge
x       dd      ?                       ; X-coordinate
env     smap    ?                       ; Environment map coordinates
tex     smap    ?                       ; Texture map coordinates
        ends

        struc   sobjdata
ftab    dd      ?                       ; Face table
vtab    dd      ?                       ; Vertex table
mtab    dd      ?                       ; Map table

faces   dd      ?                       ; Number of faces - 1
verts   dd      ?                       ; Number of vertices - 1

fend    dd      ?                       ; Offset to the last face
vend    dd      ?                       ; Offset to the last vertex

off     svect   ?                       ; Position of the object
angle   sangle  ?                       ; Angle of the object

ntab    dd      ?                       ; Normal table
        ends

;----------------------------------------------------------------------------
NORMAL_DIVISOR  =       4


EDGEX           =       0*dword
EDGENX          =       1*dword
EDGENY          =       2*dword
EDGEU           =       3*dword
EDGEV           =       4*dword
EDGESIZE        =       5

EDGEDATAS       =       3*dword
EDGEDATAE       =       5*dword
EDGEDATAT       =       7*dword


U_TILE          =       1
V_TILE          =       1


FOCUS           =       128
LENSDIST        =       FOCUS*2

XMUL            =       ((LENSDIST shl FRACBITS)*((GRAPHASPECT shl FRACBITS)/SCREENASPECT)) shr FRACBITS
YMUL            =       LENSDIST shl FRACBITS


TEXMAP_GETAREA  =       0

;----------------------------------------------------------------------------
        global  c setnormals:proc
        global  c rotate:proc,c sortfaces:proc
        global  phong:near,texturemap:near

        global  facetaboff:dword,vertextaboff:dword,maptaboff:dword
        global  lastfaceoff:dword,lastvertexoff:dword
        global  lastfacenum:dword,lastvertexnum:dword

        global  object_data:dword
        global  rock_data:sobjdata,duck_data:sobjdata
        global  obj11_data:sobjdata,obj12_data:sobjdata
        global  st_data:sobjdata,ti_data:sobjdata,ac_data:sobjdata

        global  objoffset:svect,objangle:sangle

        global  face:sface
        global  v2ratio:dword,v2deltax:dword

        global  pdata:spdata
        global  delta:sedge,count:sedge,grad:sedge
        global  edge_egrad:smap,edge_tgrad:smap

