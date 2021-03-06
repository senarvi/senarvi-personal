        %title  "Copyright (c) Seppo Enarvi  1996,1997"
        %subttl "Interrupt functions"

        include "declare.mac"


        include "system.mac"

        include "int.inc"
        include "dpmi.inc"


;北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北�
; Code
;北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北�
; [fold]  [

        _code

;哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪�
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

;哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪�
; [fold]  [
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
; [fold]  ]

;哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪�
; [fold]  [
; Enable interrupt request line

        proc    enableirq uses eax ecx edx, irqnum:dword
        mov     edx,PIC2_IO_BASE+PIC_MASK ; Second interrupt controller
        mov     ecx,[irqnum]
        cmp     ecx,8
        jae     @@highirq
        mov     edx,PIC1_IO_BASE+PIC_MASK ; First interrupt controller
@@highirq:

        in      al,dx
        mov     ah,11111110b
        rol     ah,cl
        and     al,ah
        out     dx,al
        ret
        endp
; [fold]  ]

;哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪�
; [fold]  [
; Disable interrupt request line

        proc    disableirq uses eax ecx edx, irqnum:dword
        mov     edx,PIC2_IO_BASE+PIC_MASK ; Second interrupt controller
        mov     ecx,[irqnum]
        cmp     ecx,8
        jae     @@highirq
        mov     edx,PIC1_IO_BASE+PIC_MASK ; First interrupt controller
@@highirq:

        in      al,dx
        mov     ah,11111110b
        rol     ah,cl
        and     al,ah

        mov     ah,00000001b
        rol     ah,cl
        or      al,ah
        out     dx,al
        ret
        endp
; [fold]  ]

;哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪�
; [fold]  [
; Initialize interrupt request line

        proc    initirq uses eax ecx edx ebx, irqnum:dword,irqhandler:dword
        mov     eax,[irqnum]

        ; Get corresponding interrupt number of interrupt request line
        cmp     eax,8
        jae     @@highirq
        add     eax,[master_pic_base]   ; IRQ 0-7
        jmp     @@intconverted
@@highirq:
        sub     eax,8
        add     eax,[slave_pic_base]    ; IRQ 8-15
@@intconverted:

        call    setintvec,eax,[irqhandler]
        call    enableirq,[irqnum]
        ret
        endp
; [fold]  ]

        ends
; [fold]  ]

;北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北�
; Uninitialized Data
;北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北�
; [fold]  [

                _udata

master_pic_base dd      ?
slave_pic_base  dd      ?

                ends
; [fold]  ]

        end

; [fold]  7
