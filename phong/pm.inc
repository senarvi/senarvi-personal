
PM_RAW          equ     0
PM_XMS          equ     1
PM_VCPI         equ     2
PM_DPMI         equ     3

CPL             equ     0

DTYPE           equ     0001111110010000b
DPAGE           equ     1 shl 15
DBYTE           equ     0 shl 15
DUSE32          equ     1 shl 14
DUSE16          equ     0 shl 14
DDPL            equ     CPL shl 5
DCODE           equ     1 shl 3 + 0 shl 2 + 1 shl 1
DDATA           equ     0 shl 3
DEXPDOWN        equ     1 shl 2
DEXPUP          equ     0 shl 2
DRW             equ     1 shl 1
DREAD           equ     0 shl 1
DACC            equ     1 shl 0
DNOTACC         equ     0 shl 0

PM_FREEDESC     equ     0001h
PM_SETSEGBASE   equ     0007h
PM_GETINTVEC    equ     0204h
PM_SETINTVEC    equ     0205h
PM_GETFREEMEM   equ     0500h
PM_ALLOCMEM     equ     0501h
PM_FREEMEM      equ     0502h

PMINT           equ     31h

MEMFREE         equ     0
