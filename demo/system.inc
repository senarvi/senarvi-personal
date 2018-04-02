;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
CACHESIZE       =       16              ; 16 for 486, 32 for Pentium

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
        struc   saddr
off     dd      ?
sel     dw      ?
        ends

        struc   sword
lsb     db      ?
msb     db      ?
        ends

        struc   ver
major   dw      ?
minor   dw      ?
        ends

        struc   stream
addr    dd      ?
size    dd      ?
        ends

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
LSB             =       0
MSB             =       1
LSW             =       0
MSW             =       2
LSD             =       0
MSD             =       4

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
imm16           equ     small
imm32           equ     large

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
CS_OVERRIDE     =       2Eh
SS_OVERRIDE     =       36h
DS_OVERRIDE     =       3Eh
ES_OVERRIDE     =       26h
FS_OVERRIDE     =       64h
GS_OVERRIDE     =       65h

OPERAND_SIZE    =       66h
ADDRESS_SIZE    =       67h

REP_PREFIX      =       0f3h
REPE_PREFIX     =       0f3h
REPZ_PREFIX     =       0f3h
REPNE_PREFIX    =       0f2h
REPNZ_PREFIX    =       0f2h

LOCK_PREFIX     =       0f0h

