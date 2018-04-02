        %title  "Copyright (c) Seppo Enarvi  1996,1997"
        %subttl "DOS Protected Mode Interface"

        include "declare.mac"


        include "dpmi.inc"


;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; Code
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; [fold]  (

        _code

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
; Set interrupt vector

        proc    setintvec uses eax ecx edx ebx, intnum:dword,inthandler:dword
        mov     eax,DPMI_SETINTVEC
        mov     ebx,[intnum]
        mov     cx,cs
        mov     edx,[inthandler]
        int     DPMI_INT
        ret
        endp
; [fold]  ]

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  (
; Get current PIC mappings
;
; This procedure should be called before any of the IRQ functions

        proc    getpicmappings uses eax ecx edx ebx
        mov     eax,DPMI_GETVERSION
        int     DPMI_INT
        xor     eax,eax
        mov     al,dh
        mov     [master_pic_base],eax
        mov     al,dl
        mov     [slave_pic_base],eax
        ret
        endp
; [fold]  )

        ends
; [fold]  )

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; Uninitialized data
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; [fold]  [

                _udata

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                label   rmregs dpmi_regs
                dpmi_regs ?

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
master_pic_base dd      ?
slave_pic_base  dd      ?

                ends
; [fold]  ]

        end
; [fold]  4
