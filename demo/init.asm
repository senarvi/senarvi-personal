        %title  "Copyright (c) Seppo Enarvi  1996,1997"
        %subttl "Initialization code"

        include "declare.mac"


        include "video.mac"
        include "int.mac"
        include "system.mac"

        include "init.inc"
        include "system.inc"
        include "bios.inc"
        include "dpmi.inc"
        include "int.inc"
        include "pit.inc"
        include "math.inc"
        include "video.inc"


;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; Code
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; [fold]  [

        _code

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
; Initialization

        proc    init c near
        pushad
        push    ds
        pop     es

        ; Allocate ground selector
        mov     eax,DPMI_ALLOCDESC      ; Allocate descriptor
        mov     ecx,1
        int     DPMI_INT
        mov     ebx,eax
        mov     eax,DPMI_SETSEGBASE     ; Set segment base address
        xor     ecx,ecx
        xor     edx,edx
        int     DPMI_INT
        mov     eax,DPMI_SETSEGLIMIT    ; Set segment limit
        dec     ecx
        dec     edx
        int     DPMI_INT
        mov     gs,bx

        call    getpicmappings
        call    initirq,imm32 KBIRQ,offset kbhandler
        call    initirq,imm32 PITIRQ,offset pithandler
        setpitfreq TIMER_FREQUENCY

        popad
        ret
        endp
; [fold]  ]

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
; Uninitialization

        proc    uninit c near
        resetpitfreq
        settext
        ret
        endp
; [fold]  ]

        ends
; [fold]  ]

        end

; [fold]  3
