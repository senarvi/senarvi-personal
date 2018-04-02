
        include declare.mac


        include "math.inc"
        include "memory.inc"

        include "math.mac"


;############################################################################
; Code
;############################################################################
; [fold]  [

        deccode32

;----------------------------------------------------------------------------
; [fold]  [
; Power function, x^y
;
; In:
;   eax - x
;   ecx - y
;
; Out:
;   eax - result
;
; Registers modified:
;   none

        proc    pow c near
        push    ecx edx ebx

        mov     ebx,eax
@@loop:
        imul    ebx
        shrd    eax,edx,FRACBITS
        dec     ecx
        jnz     @@loop

        pop     ebx edx ecx
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
; Integer power function, x^y
;
; In:
;   eax - x
;   ecx - y
;
; Out:
;   eax - result
;
; Registers modified:
;   none

        proc    ipow c near
        push    ecx edx ebx

        mov     ebx,eax
@@loop:
        imul    ebx
        dec     ecx
        jnz     @@loop

        pop     ebx edx ecx
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
; Square root
; by Mike Dailly
;
; In:
;   edx - square
;
; Out:
;   eax - square root
;
; Regs modified:
;   eax

        proc    sqrt c near
        push    ecx edx ebx edi
        xor     eax,eax          ; partroot = 0
        mov     edi,1 shl 30     ; shiftadjustfactor = 2^30
        mov     ecx,16
sqrtl0:
        mov     ebx,eax          ; subtrahend = partroot
        add     ebx,edi          ; subtrahend += shiftadjustfactor
        shr     eax,1            ; partroot /= 2
        cmp     ebx,edx          ; compare subtrahend with square
        ja      sqrtf0
        sub     edx,ebx          ; square -= subtrahend
        add     eax,edi          ; partroot +=shiftadjustfactor
sqrtf0:
        shr     edi,2            ; shiftadjustfactor /= 4
        dec     ecx
        jne     sqrtl0
        pop     edi ebx edx ecx
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
; Make hexadecimal string from integer
;
; In:
;   edx - Number
;   cl - Number of digits to be converted (1 - 8)
;
; Out:
;   [edi] - String
;
; Registers modified:
;   none

        proc    makehstr c near
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

;----------------------------------------------------------------------------
; [fold]  [
; Make decimal string from integer
;
; In:
;   edx - Number
;   ecx - Number of digits to be converted (1 - 10)
;   [_radix] - 10
;
; Out:
;   [edi] - String
;
; Registers modified:
;   none

        proc    int2str c near
        pushad
        mov     ebx,edx

        mov     eax,[_radix]
        dec     ecx
        call    ipow            ; 10^numdigits
        mov     ecx,eax
@@loop:
        mov     eax,ecx
        xor     edx,edx
        div     [_radix]
        mov     ecx,eax

        mov     eax,ebx
        xor     edx,edx
        div     ecx

        xor     edx,edx         ; mod 10
        div     [_radix]

        add     edx,'0'
        mov     [edi],dl
        inc     edi
        cmp     ecx,1
        ja      @@loop

        popad
        ret
        endp
; [fold]  ]

        ends
; [fold]  ]

;############################################################################
; Data
;############################################################################
; [fold]  [

        decdata

_radix          dd      10

        ends
; [fold]  ]

        end

; [fold]  7
