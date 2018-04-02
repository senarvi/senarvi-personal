
                include video.inc


FOCUS           equ     128
LENSDIST        equ     FOCUS*2

XMUL            =       ((LENSDIST shl FRACBITS)*((GRAPHASPECT shl FRACBITS)/SCREENASPECT)) shr FRACBITS
YMUL            =       LENSDIST shl FRACBITS

