
DOS_DISPSTR     equ     09h
DOS_OPENFILE    equ     3dh
DOS_CLOSEFILE   equ     3eh
DOS_READFILE    equ     3fh

ACC_READONLY    equ     0 shl 0
ACC_WRITEONLY   equ     1 shl 0
ACC_READWRITE   equ     2 shl 0
ACC_COMPAT      equ     0 shl 4
ACC_DENYRW      equ     1 shl 4
ACC_DENYWRITE   equ     2 shl 4
ACC_DENYREAD    equ     3 shl 4
ACC_DENYNONE    equ     4 shl 4
ACC_CHILDINH    equ     0 shl 7
ACC_NOCHILDINH  equ     1 shl 7

