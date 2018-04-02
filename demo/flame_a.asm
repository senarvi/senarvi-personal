
        include "declare.mac"


        include "system.mac"
        include "math.mac"

        include "flame.inc"
        include "math.inc"
        include "video.inc"


;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; Code
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; [fold]  [

        _code

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
        proc    clearflamebuf_
        pushad

        mov     ecx,FLAMEBUFFER_SIZE/4-1
@@loop:
        mov     [dword flamebuf+ecx*4],0
        dec     ecx
        jge     @@loop

        popad
        ret
        endp
; [fold]  ]

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
        proc    copyflamebuf_
        pushad

        mov     ecx,FLAMEBUFFER_X*(FLAMEBUFFER_Y-2)-1
        mov     ebx,(GR_Y-FLAMESQUARE_Y)*GR_X
@@loop2:
        mov     edx,GR_X-FLAMESQUARE_X
@@loop1:

        mov     al,[flamebuf+ecx]
;        mov     ah,al
;        lea     edi,[ebx+edx]
;        add     edi,[destbufoff]
;        mov     [edi],ax
;        mov     [edi+GR_X],ax

        cmp     [byte edi],0
        jne     @@f0
        mov     [byte edi],al
@@f0:
        cmp     [byte edi+1],0
        jne     @@f1
        mov     [byte edi+1],al
@@f1:
        cmp     [byte edi+GR_X],0
        jne     @@f2
        mov     [byte edi+GR_X],al
@@f2:
;        cmp     [byte edi+GR_X+1],0
;        jne     @@f3
        mov     [byte edi+GR_X+1],al
;@@f3:


        dec     ecx
        sub     edx,FLAMESQUARE_X
        jge     @@loop1
        sub     ebx,FLAMESQUARE_Y*GR_X
        jge     @@loop2

        popad
        ret
        endp
; [fold]  ]

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
        proc    random
        mov     ax,[randseed]
        mov     bx,[randseed+2]
        mov     si,ax
        mov     di,bx
        mov     dl,ah
        mov     ah,al
        mov     al,bh
        shl     ebx,8
        rcr     dl,1
        rcr     ax,1
        rcr     bx,1
        add     bx,di
        adc     ax,si
        add     bx,62e9h
        adc     ax,3619h
        mov     [word randseed],bx
        mov     [word randseed+2],ax
        xor     dx,dx
        div     cx
        ret
        endp
; [fold]  ]

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
        proc    randomflame_
        pushad

        xor     edx,edx
        mov     ecx,6
@@loop:
        push    ecx
        mov     ecx,FLAMEBUFFER_X-(2*5)
        call    random
        lea     edi,[(FLAMEBUFFER_Y-3)*FLAMEBUFFER_X+5+edx]
        pop     ecx

        push    ecx
        push    edi
        mov     ecx,128
        call    random
        pop     edi
        pop     ecx


        mov     dh,dl
        mov     [word flamebuf+edi],dx
        dec     ecx
        jg      @@loop

        popad
        ret
        endp
; [fold]  ]

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
        proc    calculateflame_
        pushad

        mov     ecx,FLAMEBUFFER_X+1
        xor     edx,edx
@@loop:
        movzx   edx,[flamebuf+ecx+FLAMEBUFFER_X-1]
        cmul    edx,2
        add     eax,edx
        movzx   edx,[flamebuf+ecx]
        cmul    edx,3
        add     eax,edx
        movzx   edx,[flamebuf+ecx-FLAMEBUFFER_X+1]
        cmul    edx,2
        add     eax,edx
        cdiv    eax,8

        mov     [flamebuf+ecx-FLAMEBUFFER_X],al
        inc     ecx
        cmp     ecx,(FLAMEBUFFER_Y-2)*FLAMEBUFFER_X-1
        jle     @@loop

        popad
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
randseed        dw      06425h,0a830h

        ends
; [fold]  ]

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; Uninitialized data
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; [fold]  [

                _udata

flamebuf        db      FLAMEBUFFER_SIZE dup (?)

                ends
; [fold]  ]

        end

; [fold]  8
