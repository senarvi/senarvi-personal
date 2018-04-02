
                include "video.inc"


SIN_TYPE        equ     dword
SIN_FRACBITS    =       14
SIN_NUM         =       256


        global  sintable:SIN_TYPE:SIN_NUM,costable:SIN_TYPE:SIN_NUM
        global  angletab:byte:GR_BUFSIZE
        global  marble_texmap:byte:256*256,tunnel_texmap:byte:256*256
        global  envmap:byte:256*256
        global  stc_heightmap:byte:GR_BUFSIZE
        global  stc_smoke_image:byte:GR_BUFSIZE

