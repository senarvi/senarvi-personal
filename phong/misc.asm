
                include declare.mac


                decstart


                include misc.inc
                include memory.inc


                public  sqrt,pow
                public  setsqrt
                public  makehstr,makedstr

                public  rmregs
                public  rm_eax,rm_ecx,rm_edx,rm_ebx
                public  rm_esp,rm_ebp,rm_esi,rm_edi
                public  rm_ax,rm_cx,rm_dx,rm_bx
                public  rm_sp,rm_bp,rm_si,rm_di
                public  rm_ah,rm_al,rm_ch,rm_cl
                public  rm_dh,rm_dl,rm_bh,rm_bl


                deccode32

;----------------------------------------------------------------------------
; Set square root table

setsqrt:
                pushad

                mov     ecx,MAX_SQRT
@@setsqrtl0:
                mov     edx,ecx
                shl     edx,FRACBITS
                call    sqrt
                mov     fs:_sqrttab[ecx*dword],eax
                dec     ecx
                jge     @@setsqrtl0

                popad
                ret

;----------------------------------------------------------------------------
; Power function, x^y
;
; In:
;   eax - x
;   ecx - y
;
; Out:
;   eax - result
;
; Regs modified:
;   eax ecx edx ebx

pow:
                mov       ebx,eax
@@powl0:
                imul      ebx
                shrd      eax,edx,FRACBITS
                dec       ecx
                jnz       @@powl0
                ret

;----------------------------------------------------------------------------
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

sqrt:
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

;----------------------------------------------------------------------------
; Make hexadecimal string from number
;
; In:
;   edx - Number
;   ecx - Bits in number - 4
;
; Out:
;   [edi] - String

makehstr:
                mov     eax,edx
                shr     eax,cl
                and     eax,0fh

                cmp     eax,9
                ja      @@makehstrf0
                add     eax,'0'
                jmp     @@makehstrf1
@@makehstrf0:
                sub     eax,10
                add     eax,'A'
@@makehstrf1:
                mov     [edi],al

                inc     edi
                sub     ecx,4
                jge     makehstr
                ret

;----------------------------------------------------------------------------
; Make decimal string from number
;
; In:
;   ebx - Number
;   ecx - Possible values in number
;
; Out:
;   [edi] - String

makedstr:
                mov     eax,ecx
                xor     edx,edx
                div     [num10]
                mov     ecx,eax

                mov     eax,ebx
                xor     edx,edx
                div     ecx

                xor     edx,edx
                div     [num10]

                add     edx,'0'
                mov     [edi],dl
                inc     edi
                cmp     ecx,1
                ja      makedstr
                ret


                align   4
rmregs          label   dataptr
rm_edi          label   dword
rm_di           dw      ?,?
rm_esi          label   dword
rm_si           dw      ?,?
rm_ebp          label   dword
rm_bp           dw      ?,?
rm_esp          label   dword            ; ignored
                dw      ?,?
rm_ebx          label   dword
rm_bx           label   word
rm_bl           db      ?
rm_bh           db      ?,?,?
rm_edx          label   dword
rm_dx           label   word
rm_dl           db      ?
rm_dh           db      ?,?,?
rm_ecx          label   dword
rm_cx           label   word
rm_cl           db      ?
rm_ch           db      ?,?,?
rm_eax          label   dword
rm_ax           label   word
rm_al           db      ?
rm_ah           db      ?,?,?
rm_flags        dw      ?
rm_es           dw      ?
rm_ds           dw      ?
rm_fs           dw      ?
rm_gs           dw      ?
rm_ip           dw      ?                ; ignored
rm_cs           dw      ?                ; ignored
rm_sp           dw      0                ; PMODE will provide stack
rm_ss           dw      0

;----------------------------------------------------------------------------
num10           dd      10

                endcode32
                end
