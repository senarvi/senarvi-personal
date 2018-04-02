        %title  "Copyright (c) Seppo Enarvi  1996,1997"
        %subttl "Mathematics"

        include "declare.mac"


        include "system.mac"
        include "math.mac"

        include "video.inc"
        include "math.inc"
        include "memory.inc"


;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; Code
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; [fold]  [

        _code

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
; Fixed point power function, x^y
;
; In:
;   fixed - x
;   dword - y
;
; Out:
;   eax - result

        proc    fixpow uses ecx edx ebx, x:fixed,y:dword
        mov     eax,ONE
        mov     ecx,[y]
        or      ecx,ecx
        jz      @@done
        mov     ebx,[x]
@@loop:
        imul    ebx
        shrd    eax,edx,FRACBITS
        dec     ecx
        jnz     @@loop
@@done:
        ret
        endp
; [fold]  ]

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
; Integer power function, x^y
;
; In:
;   dword - x
;   dword - y
;
; Out:
;   eax - result

        proc    ipow uses ecx edx ebx, x:dword,y:dword
        mov     eax,1
        mov     ecx,[y]
        or      ecx,ecx
        jz      @@done
        mov     ebx,[x]
@@loop:
        imul    ebx
        dec     ecx
        jnz     @@loop
@@done:
        ret
        endp
; [fold]  ]

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
; Square root
; by Mike Dailly
;
; In:
;   edx - square
;
; Out:
;   eax - square root

        proc    sqrt uses ecx edx ebx edi
        xor     eax,eax          ; partroot = 0
        mov     edi,1 shl 30     ; shiftadjustfactor = 2^30
        mov     ecx,16
@@loop:
        mov     ebx,eax          ; subtrahend = partroot
        add     ebx,edi          ; subtrahend += shiftadjustfactor
        cdiv    eax,2            ; partroot /= 2
        cmp     ebx,edx          ; compare subtrahend with square
        ja      @@f0
        sub     edx,ebx          ; square -= subtrahend
        add     eax,edi          ; partroot += shiftadjustfactor
@@f0:
        cdiv    edi,4            ; shiftadjustfactor /= 4
        dec     ecx
        jnz     @@loop
        ret
        endp
; [fold]  ]

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
; Make hexadecimal string from integer
;
; In:
;   edx - Number
;   cl - Number of digits to be converted (1 - 8)
;
; Out:
;   [edi] - String

        proc    makehstr
        pushad

        shl     cl,2
        sub     cl,4
@@loop:
        mov     eax,edx
        shr     eax,cl
        and     eax,0fh

        cmp     eax,9
        ja      @@letter
        add     eax,'0'
        jmp     @@digit
@@letter:
        sub     eax,10
        add     eax,'A'
@@digit:
        mov     [edi],al
        inc     edi
        sub     cl,4
        jge     @@loop

        popad
        ret
        endp
; [fold]  ]

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
; Make decimal string from integer
;
; In:
;   dword - number
;   dword - number of digits to be converted (1 - 10)
;   dword - offset where to store the string

        proc    int2str number:dword,digits:dword,destoff:dword
        local   rad:dword
        pushad
        mov     [rad],10
        mov     edi,[destoff]

        call    ipow,[rad],[digits]   ; x = 10^digits
        mov     ecx,eax
@@loop:
        xor     edx,edx
        mov     eax,ecx
        div     [rad]
        mov     ecx,eax                 ; x = 10^(digits-n)

        xor     edx,edx
        mov     eax,[number]
        div     ecx                     ; number / x

        xor     edx,edx
        div     [rad]                 ; mod 10

        add     edx,'0'
        mov     [edi],dl
        inc     edi
        cmp     ecx,1
        ja      @@loop

        popad
        ret
        endp
; [fold]  ]

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
; Calculate 2d distance
;
; In:
;   dword - x coordinate
;   dword - y coordinate

        proc    dist2d uses ecx edx, x:dword,y:dword
        mov     eax,[x]
        imul    eax
        mov     ecx,eax

        mov     eax,[y]
        imul    eax

        lea     edx,[ecx+eax]
        call    sqrt
        ret
        endp
; [fold]  ]

        ends
; [fold]  ]

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; Data
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; [fold]  [

                _data

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
_radix          dd      10

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                macro   dummy
label   sintable dword
dd           0,    402,    804,   1205,   1606,   2006
dd        2404,   2801,   3196,   3590,   3981,   4370
dd        4756,   5139,   5520,   5897,   6270,   6639
dd        7005,   7366,   7723,   8076,   8423,   8765
dd        9102,   9434,   9760,  10080,  10394,  10702
dd       11003,  11297,  11585,  11866,  12140,  12406
dd       12665,  12916,  13160,  13395,  13623,  13842
dd       14053,  14256,  14449,  14635,  14811,  14978
dd       15137,  15286,  15426,  15557,  15679,  15791
dd       15893,  15986,  16069,  16143,  16207,  16261
dd       16305,  16340,  16364,  16379

label   costable dword
dd       16384,  16379,  16364,  16340,  16305,  16261
dd       16207,  16143,  16069,  15986,  15893,  15791
dd       15679,  15557,  15426,  15286,  15137,  14978
dd       14811,  14635,  14449,  14256,  14053,  13842
dd       13623,  13395,  13160,  12916,  12665,  12406
dd       12140,  11866,  11585,  11297,  11003,  10702
dd       10394,  10080,   9760,   9434,   9102,   8765
dd        8423,   8076,   7723,   7366,   7005,   6639
dd        6270,   5897,   5520,   5139,   4756,   4370
dd        3981,   3590,   3196,   2801,   2404,   2006
dd        1606,   1205,    804,    402,      0,   -402
dd        -804,  -1205,  -1606,  -2006,  -2404,  -2801
dd       -3196,  -3590,  -3981,  -4370,  -4756,  -5139
dd       -5520,  -5897,  -6270,  -6639,  -7005,  -7366
dd       -7723,  -8076,  -8423,  -8765,  -9102,  -9434
dd       -9760, -10080, -10394, -10702, -11003, -11297
dd      -11585, -11866, -12140, -12406, -12665, -12916
dd      -13160, -13395, -13623, -13842, -14053, -14256
dd      -14449, -14635, -14811, -14978, -15137, -15286
dd      -15426, -15557, -15679, -15791, -15893, -15986
dd      -16069, -16143, -16207, -16261, -16305, -16340
dd      -16364, -16379, -16384, -16379, -16364, -16340
dd      -16305, -16261, -16207, -16143, -16069, -15986
dd      -15893, -15791, -15679, -15557, -15426, -15286
dd      -15137, -14978, -14811, -14635, -14449, -14256
dd      -14053, -13842, -13623, -13395, -13160, -12916
dd      -12665, -12406, -12140, -11866, -11585, -11297
dd      -11003, -10702, -10394, -10080,  -9760,  -9434
dd       -9102,  -8765,  -8423,  -8076,  -7723,  -7366
dd       -7005,  -6639,  -6270,  -5897,  -5520,  -5139
dd       -4756,  -4370,  -3981,  -3590,  -3196,  -2801
dd       -2404,  -2006,  -1606,  -1205,   -804,   -402

dd           0,    402,    804,   1205,   1606,   2006
dd        2404,   2801,   3196,   3590,   3981,   4370
dd        4756,   5139,   5520,   5897,   6270,   6639
dd        7005,   7366,   7723,   8076,   8423,   8765
dd        9102,   9434,   9760,  10080,  10394,  10702
dd       11003,  11297,  11585,  11866,  12140,  12406
dd       12665,  12916,  13160,  13395,  13623,  13842
dd       14053,  14256,  14449,  14635,  14811,  14978
dd       15137,  15286,  15426,  15557,  15679,  15791
dd       15893,  15986,  16069,  16143,  16207,  16261
dd       16305,  16340,  16364,  16379
                endm

                ends
; [fold]  ]

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; Uninitialized data
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; [fold]  [

        _udata

        label   sintable dword
        dd      SIN_NUM / 4 dup (?)
        label   costable dword
        dd      SIN_NUM dup (?)

        ends
; [fold]  ]

        end

; [fold]  9
