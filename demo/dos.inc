;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; Interrupt functions
DOS_DISPCHR     =       02h
DOS_DISPSTR     =       09h
DOS_OPENFILE    =       3dh
DOS_CLOSEFILE   =       3eh
DOS_READFILE    =       3fh
DOS_MOVEFILEPTR =       42h
DOS_ALLOCMEM    =       48h
DOS_FREEMEM     =       49h
DOS_RESIZEBLOCK =       4ah
DOS_LOADEXEC    =       4bh
DOS_ENDPROGRAM  =       4ch

DOS_INT         =       21h

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; File access byte
FA_READONLY     =       0 shl 0
FA_WRITEONLY    =       1 shl 0
FA_READWRITE    =       2 shl 0
FA_COMPAT       =       0 shl 4
FA_DENYRW       =       1 shl 4
FA_DENYWRITE    =       2 shl 4
FA_DENYREAD     =       3 shl 4
FA_DENYNONE     =       4 shl 4
FA_CHILDINH     =       0 shl 7
FA_NOCHILDINH   =       1 shl 7

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
MOVE_BEGINNING  =       0
MOVE_CURRENT    =       1
MOVE_END        =       2

EXECUTE         =       0
LOADONLY        =       1

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
DOS_MAXPATH     =       260

