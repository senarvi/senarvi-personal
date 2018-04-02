#############################################################################
.equ            _FB,    16
.equ            _FR,    1 << _FB
.equ            FRACBITS, 8
.equ            OBJ_FRACBITS, 16
.equ            ROT_FRACBITS, 8 #9
.equ            NORMALFRACBITS, 8 #16;24

#############################################################################
#MAX_SQRT	 .equ	 0ffffh

.equ            SIN_FRACBITS, 14
.equ            SIN_NUM, 256                      # Must be power of two

#############################################################################
.equ            _BYTE,  byte
.equ            _WORD,  word
.equ            _DWORD, dword
.equ            _QWORD, qword
.equ            _PARA,  16
.equ            _KILO,  1024
.equ            _PAGE,  4096

.equ            ZERO,   0 << FRACBITS
.equ            ONE,    1 << FRACBITS
.equ            HALF,   ONE/2

#############################################################################
#		typedef fixed dword

#############################################################################
		.global	fixpow,ipow
		.global	sqrt
		.global	makehstr,int2str
		.global	dist2d

		.global	sintable,costable

