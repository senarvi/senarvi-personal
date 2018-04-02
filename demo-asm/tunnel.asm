
        include "declare.mac"


        include "math.mac"

        include "tunnel.inc"
        include "allocate.inc"
        include "math.inc"
        include "vector.inc"
        include "data.inc"


; [fold]  [
        deccode32

;----------------------------------------------------------------------------
; [fold]  [
        proc    settunnel c near
        mov     ebp,1
@@settunnell1:
        mov     ebx,1
@@settunnell0:


        lea     ecx,[GRAPHHEIGHT/2-1+ebp]
        cmul    ecx,GRAPHWIDTH
        add     ecx,GRAPHWIDTH/2-1
        lea     edi,[ecx+ebx]

        movzx   esi,[angletab+edi]       ; [0..40[

        cmp     ebp,ebx
        jle     @@settunnelf0
        mov     eax,[sintable+esi*4]
        mov     edx,TUNNEL_MUL          ; [0..MUL[
        imul    edx
        cdq
        idiv    ebp             ; / Y
        sar     eax,SIN_FRACBITS
        jmp     @@settunnelf1

@@settunnelf0:
        mov     eax,[costable+esi*4+12]  ; ]1..0]
        mov     edx,TUNNEL_MUL
        imul    edx
        cdq
        idiv    ebx             ; / X
        sar     eax,SIN_FRACBITS

@@settunnelf1:
        mov     [fs:_tunneldepth+edi*4],eax
        sub     ecx,ebx
        mov     [fs:_tunneldepth+ecx*4+4],eax

        mov     ecx,GRAPHHEIGHT/2
        sub     ecx,ebp
        cmul    ecx,GRAPHWIDTH
        add     ecx,GRAPHWIDTH/2-1
        lea     edi,[ecx+ebx]
        mov     [fs:_tunneldepth+edi*4],eax
        sub     ecx,ebx
        mov     [fs:_tunneldepth+ecx*4+4],eax


        inc     ebx
        cmp     ebx,GRAPHWIDTH/2
        jle     @@settunnell0
        inc     ebp
        cmp     ebp,GRAPHHEIGHT/2
        jle     @@settunnell1
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
        proc    setwave c near
        mov     ebp,1
@@setwavel1:
        mov     ebx,1
@@setwavel0:
        mov     eax,ebx
        imul    ebx
        mov     ecx,eax

        mov     eax,ebp
        imul    ebp
        lea     edx,[ecx+eax]
        call    sqrt

        lea     ecx,[GRAPHHEIGHT/2-1+ebp]
        cmul    ecx,GRAPHWIDTH
        add     ecx,GRAPHWIDTH/2-1
        lea     edi,[ecx+ebx]
        mov     [fs:_tunneldepth+edi*4],eax
        sub     ecx,ebx
        mov     [fs:_tunneldepth+ecx*4+4],eax

        mov     ecx,GRAPHHEIGHT/2
        sub     ecx,ebp
        cmul    ecx,GRAPHWIDTH
        add     ecx,GRAPHWIDTH/2-1
        lea     edi,[ecx+ebx]
        mov     [fs:_tunneldepth+edi*4],eax
        sub     ecx,ebx
        mov     [fs:_tunneldepth+ecx*4+4],eax

        inc     ebx
        cmp     ebx,GRAPHWIDTH/2
        jle     @@setwavel0
        inc     ebp
        cmp     ebp,GRAPHHEIGHT/2
        jle     @@setwavel1
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
; Draw tunnel
;
; Greets: Il£vatar of Nordic Line

        proc    drawtunnel c near
        mov     ecx,GRAPHWIDTH*GRAPHHEIGHT-1
        mov     ebp,GRAPHHEIGHT-1
@@drawtunnell1:
        mov     ebx,GRAPHWIDTH-1
@@drawtunnell0:

        mov     edi,[fs:_tunneldepth+ecx*dword]
        cdiv    edi,2
        cmp     edi,MAX_TUNNEL_DEPTH ; Do not draw if we are too far
        jg      @@drawtunnelf0
        mov     edx,edi         ; Save depth for sine

        add     edi,[tunnel_y]          ; Depth + y-count
        and     edi,256-1               ; Y-coordinate for texmap
        cmul    edi,256
        movzx   esi,[angletab+ecx*byte] ; X-coordinate for texmap

        cdiv    edx,8
        mov     eax,[tunnel_y]
        cdiv    eax,4
        add     eax,edx                 ; Depth/8 + y-count
        and     eax,256-1               ; Index for sine table
        mov     eax,[sintable+eax*dword]
        sar     eax,SIN_FRACBITS-7
        add     esi,eax;[tunnel_x]      ; Add x-count or sine

        and     esi,256-1
        movzx   eax,[tunnel_texmap+esi+edi]
        mov     [byte ptr fs:_screenbuf+ecx],0
        cmul    edx,4
        sub     eax,edx
        jl      @@drawtunnelf0
        mov     [fs:_screenbuf+ecx],al
@@drawtunnelf0:

        dec     edx
        dec     ecx
        dec     ebx
        jge     @@drawtunnell0
        dec     ebp
        jge     @@drawtunnell1
        ret
        endp
; [fold]  ]

        ends
; [fold]  ]

; [fold]  [
        decdata

;tunnel_x        dd      0
;tunnel_y        dd      0

        ends
; [fold]  ]

        end

; [fold]  5
