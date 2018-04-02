
;----------------------------------------------------------------------------
        struc   saddr
off     dd      ?
sel     dw      ?
        ends

        struc   sword
lsb     db      ?
msb     db      ?
        ends

;----------------------------------------------------------------------------
LSB             =       0
MSB             =       1
LSW             =       0
MSW             =       2
LSD             =       0
MSD             =       4

;----------------------------------------------------------------------------
param = dword ebp+8

;----------------------------------------------------------------------------
PMCSEG          equ     code16
C16SEG          equ     code16
CSEG            equ     code32
DSEG            equ     code32
SSEG            equ     stack

;----------------------------------------------------------------------------
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

;----------------------------------------------------------------------------
DOS_DISPSTR     =       09h
DOS_OPENFILE    =       3dh
DOS_CLOSEFILE   =       3eh
DOS_READFILE    =       3fh

ACC_READONLY    =       0 shl 0
ACC_WRITEONLY   =       1 shl 0
ACC_READWRITE   =       2 shl 0
ACC_COMPAT      =       0 shl 4
ACC_DENYRW      =       1 shl 4
ACC_DENYWRITE   =       2 shl 4
ACC_DENYREAD    =       3 shl 4
ACC_DENYNONE    =       4 shl 4
ACC_CHILDINH    =       0 shl 7
ACC_NOCHILDINH  =       1 shl 7

MAXPATH         =       79

