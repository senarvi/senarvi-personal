
        include "object.inc"
        include "vector.inc"
        include "system.inc"
        include "video.inc"
        include "flame.inc"


RADIX_BITS      =       4
RADIX_SIZE      =       1 shl RADIX_BITS


_allocstart     =       00000000h
_screenbuf      =       _allocstart
_radixbuf       =       _screenbuf   + NUM_BUFFERS*GR_BUFSIZE*byte
_faceorder      =       _radixbuf    + 2*(RADIX_SIZE*MAX_FACES*dword)
_rotatetab      =       _faceorder   + MAX_FACES*dword
_normaltab      =       _rotatetab   + MAX_VERTICES*size svect
_normaltab1     =       _normaltab   + MAX_FACES*size sface
_normaltab2     =       _normaltab1  + MAX_VERTICES*size svect
_normaltab3     =       _normaltab2  + MAX_VERTICES*size svect
_ledgebuf       =       _normaltab3  + MAX_VERTICES*size svect
_redgebuf       =       _ledgebuf    + GRAPHHEIGHT*size sedge
_palette        =       _redgebuf    + GRAPHHEIGHT*size sedge
_wavetab        =       _palette     + GRAPHCOLORS*size svideomem
_flamebuf       =       _wavetab     + GR_BUFSIZE*byte
_tunneldepth    =       _flamebuf    + FLAMEBUFFER_X*FLAMEBUFFER_Y*byte
_heightmap      =       _tunneldepth + GR_BUFSIZE*dword
_datafile       =       _heightmap   + GR_BUFSIZE*dword
_allocend       =       _datafile    + 0

                struc   alloc
screenbuf       db      NUM_BUFFERS*GR_BUFSIZE dup (?)
radixbuf        dd      2*(RADIX_SIZE*MAX_FACES) dup (?)
faceorder       dd      MAX_FACES dup (?)
rotatetab       svect   MAX_VERTICES dup (?)
normaltab1      svect   MAX_VERTICES dup (?)
normaltab2      svect   MAX_VERTICES dup (?)
normaltab3      svect   MAX_VERTICES dup (?)
ledgebuf        sedge   GR_BUFY dup (?)
redgebuf        sedge   GR_BUFY dup (?)
palette         svideomem GRAPHCOLORS dup (?)
wavetab         db      GR_BUFSIZE dup (?)
flamebuf        db      FLAMEBUFFER_X*FLAMEBUFFER_Y dup (?)
tunneldepth     dd      GR_BUFSIZE dup (?)
heightmap       dd      GR_BUFSIZE dup (?)
                ends


MEMREQ          =       (_allocend+0fffh) and 0fffff000h
STACKLENGTH     =       40h
_heap           =       SSEG+STACKLENGTH

