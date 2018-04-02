        %title  "Copyright (c) Seppo Enarvi  1996,1997"
        %subttl "Smoke effect"


        include "declare.mac"


        include "smoke.inc"
        include "data.inc"
        include "allocate.inc"


;############################################################################
; Code
;############################################################################
; [fold]  (

        deccode32

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  (
        proc    smoke c near
        mov     esi,GR_BUFX/2
        mov     edi,GR_BUFY/2

        mov     edx,(GR_BUFY-1)*GR_BUFX
@@smokel1:
        mov     ecx,GR_BUFX-1
@@smokel0:
        mov     al,[stc_smoke_image+ecx+edx]
        mov     [fs:_screenbuf+ecx+edx],al
        dec     ecx
        jge     @@smokel0
        sub     edx,GR_BUFX
        jge     @@smokel1

        mov     esi,1
        mov     edi,1
        mov     ecx,10
        mov     edx,10
        mov     eax,0ffh
        call    drawline c,esi,edi,ecx,edx,eax
        ret
        endp
; [fold]  )

        ends
; [fold]  )

;############################################################################
; Data
;############################################################################
; [fold]  [

        decdata

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
smokedata       dd      ?

        ends
; [fold]  ]

        end
; [fold]  3
