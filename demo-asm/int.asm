        %title  "Copyright (c) Seppo Enarvi  1996,1997"
        %subttl "Interrupt code"


        include declare.mac

        include "int.inc"
        include "system.inc"
        include "pmode.inc"
        include "init.inc"


;############################################################################
; Code
;############################################################################
; [fold]  (

        deccode32

;----------------------------------------------------------------------------
; [fold]  (
; Initialize interrupt request line
;
; Regs modified:
;   none

__irqnum = param
__irqhandler = param+4

        proc    initirq c near
        enter   0,0
        pushad

        mov     eax,DPMI_SETINTVEC
        mov     ebx,[__irqnum]
        mov     cx,[word c32sel]
        mov     edx,[__irqhandler]
        cmp     ebx,8
        jae     @@highirq

        add     ebx,8                    ; IRQ 0-7
        cli
        int     DPMI_INT
        sti
        mov     edx,PIC1                 ; First interrupt controller
        jmp     @@setpicmask

@@highirq:
        add     ebx,68h                  ; IRQ 8-15
        cli
        int     DPMI_INT
        sti
        mov     edx,PIC2                 ; Second interrupt controller

@@setpicmask:
        in      al,dx
        mov     ah,11111110b
        mov     ecx,[__irqnum]
        rol     ah,cl
        and     al,ah
        out     dx,al

        popad
        leave
        ret
        endp
; [fold]  )

;----------------------------------------------------------------------------
; [fold]  [
; Reset interrupt request line
;
; Regs modified:
;   none

__irqnum = param

        proc    disableirq
        enter   0,0
        pushad

        mov     edx,PIC2
        mov     ecx,[__irqnum]
        cmp     ecx,8
        jae     @@highirq               ; IRQ 8-15
        mov     edx,PIC1                ; IRQ 0-7
@@highirq:

        in      al,dx
        mov     ah,11111110b
        rol     ah,cl
        and     al,ah

        mov     ah,00000001b
        rol     ah,cl
        or      al,ah
        out     dx,al
;        in      al,dx
;        and     al,IRQ5ON
;        or      al,IRQ5OFF
;        out     dx,al

        popad
        leave
        ret
        endp
; [fold]  ]

        ends
; [fold]  )

;############################################################################
; Data
;############################################################################
; [fold]  [

        decdata

        align   dword
;----------------------------------------------------------------------------

        ends
; [fold]  ]

        end

; [fold]  4
