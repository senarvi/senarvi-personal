
                include dos.inc
                include object.inc
                include polygon.inc

;----------------------------------------------------------------------------
svect           struc
x               dd      ?
y               dd      ?
z               dd      ?
svect           ends

spixel          struc
x               dd      ?
y               dd      ?
spixel          ends

sface           struc
a               dd      ?
b               dd      ?
c               dd      ?
sface           ends

sangle          struc
a               dd      ?
b               dd      ?
sangle          ends

smap            struc
u               dd      ?
v               dd      ?
smap            ends

scolor          struc
red             dd      ?
green           dd      ?
blue            dd      ?
scolor          ends

;----------------------------------------------------------------------------
MAXPATH         equ     79

DATAFILENAME    equ     'DEMO.DAT'
DATAFILEACCESS  equ     ACC_READONLY+ACC_DENYRW+ACC_NOCHILDINH

;----------------------------------------------------------------------------
MAX_SQRT        equ     0ffffh

;----------------------------------------------------------------------------
LSB             equ     0
MSB             equ     1
LSW             equ     0
MSW             equ     2
OFF             equ     0
SEL             equ     4

FRACBITS = 8

;----------------------------------------------------------------------------
PMCSEG          equ     pmcode
CSEG            equ     code32
DSEG            equ     code32
SSEG            equ     stack

SELZERO         equ     10h

