
        include "declare.mac"


        include "math.mac"

        include "flame.inc"
        include "allocate.inc"
        include "video.inc"


        deccode32

;----------------------------------------------------------------------------
; [fold]  [
        proc    clearflamebuf c near
        mov     ecx,(FLAMEBUFFER_X*FLAMEBUFFER_Y)/4-1
@@loop:
        mov     [dword ptr fs:_flamebuf+ecx*4],0
        dec     ecx
        jge     @@loop
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
        proc    copyflamebuf c near
        mov     ecx,FLAMEBUFFER_X*(FLAMEBUFFER_Y-2)-1
        mov     ebx,(GRAPHBUFFER_Y-FLAMESQUARE_Y)*GRAPHBUFFER_X
@@copyflamel1:
        mov     edx,GRAPHBUFFER_X-FLAMESQUARE_X
@@copyflamel0:

        mov     al,[fs:_flamebuf+ecx]
        mov     ah,al
        mov     [fs:_screenbuf+ebx+edx],ax
        mov     [fs:_screenbuf+ebx+edx+GRAPHBUFFER_X],ax

        dec     ecx
        sub     edx,FLAMESQUARE_X
        jge     @@copyflamel0
        sub     ebx,FLAMESQUARE_Y*GRAPHBUFFER_X
        jge     @@copyflamel1
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; Greets MAC / PROPHECY
; [fold]  [
        proc    random c near
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
        mov     [word ptr randseed],bx
        mov     [word ptr randseed+2],ax
        xor     dx,dx
        div     cx
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
        proc    randomflame c near
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
        mov     [fs:_flamebuf+edi],dx
        dec     ecx
        jg      @@loop
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
        proc    flamescreen c near
        mov     ecx,FLAMEBUFFER_X+1
        xor     edx,edx
@@loop:
        movzx   edx,[byte ptr fs:_flamebuf+ecx+FLAMEBUFFER_X-1]
        cmul    edx,2
        add     eax,edx
        movzx   edx,[byte ptr fs:_flamebuf+ecx]
        cmul    edx,3
        add     eax,edx
        movzx   edx,[byte ptr fs:_flamebuf+ecx-FLAMEBUFFER_X+1]
        cmul    edx,2
        add     eax,edx
        cdiv    eax,8

        mov     [fs:_flamebuf+ecx-FLAMEBUFFER_X],al
        inc     ecx
        cmp     ecx,(FLAMEBUFFER_Y-2)*FLAMEBUFFER_X-1
        jle     @@loop
        ret
        endp
; [fold]  ]

        ends


        decdata

;----------------------------------------------------------------------------
randseed        dw      06425h,0a830h

        ends

        end

; [fold]  5
