        .intel_syntax

        include "declare.mac"


        include "system.mac"
        include "math.mac"

        include "tunnel.inc"
        include "math.inc"
        include "video.inc"
        include "vector.inc"
        include "data.inc"


;�����������������������������������������������������������������������������
; Code
;�����������������������������������������������������������������������������
; [fold]  [

        _code

;����������������������������������������������������������������������������
; [fold]  [
        proc    settunnel_
        mov     ebp,1
@@loop2:
        mov     ebx,1
@@loop1:
        lea     ecx,[GR_Y/2-1+ebp]
        cmul    ecx,GR_X
        add     ecx,GR_X/2-1
        lea     edi,[ecx+ebx]

        movzx   esi,[angletab + edi]    ; [0..40[

        cmp     ebp,ebx
        jle     @@f1
        mov     eax,[sintable + esi*4]
        mov     edx,TUNNEL_MUL          ; [0..MUL[
        imul    edx
        cdq
        idiv    ebp                     ; / Y
        sar     eax,SIN_FRACBITS
        jmp     @@f2

@@f1:
        mov     eax,[costable + (esi+3)*4] ; ]1..0]
        mov     edx,TUNNEL_MUL
        imul    edx
        cdq
        idiv    ebx                     ; / X
        sar     eax,SIN_FRACBITS

@@f2:
        cdiv    eax,2
        mov     [tunneldepth + edi*4],eax
        sub     ecx,ebx
        mov     [tunneldepth + (ecx+1)*4],eax

        mov     ecx,GR_Y/2
        sub     ecx,ebp
        cmul    ecx,GR_X
        add     ecx,GR_X/2-1
        lea     edi,[ecx+ebx]
        mov     [tunneldepth + edi*4],eax
        sub     ecx,ebx
        mov     [tunneldepth + (ecx+1)*4],eax


        inc     ebx
        cmp     ebx,GR_X/2
        jle     @@loop1
        inc     ebp
        cmp     ebp,GR_Y/2
        jle     @@loop2
        ret
        endp
; [fold]  ]

;����������������������������������������������������������������������������
; [fold]  [
        proc    setwave_
        pushad

        mov     ebp,1
@@loop2:
        mov     ebx,1
@@loop1:
        mov     eax,ebx
        imul    ebx
        mov     ecx,eax

        mov     eax,ebp
        imul    ebp
        lea     edx,[ecx+eax]
        call    sqrt                            ; Distance = �(x�+y�)

        lea     ecx,[GR_Y/2-1+ebp]
        cmul    ecx,GR_X
        add     ecx,GR_X/2-1
        lea     edi,[ecx+ebx]
        mov     [tunneldepth + edi*dword],eax
        sub     ecx,ebx
        mov     [tunneldepth + (ecx+1)*dword],eax

        mov     ecx,GR_Y/2
        sub     ecx,ebp
        cmul    ecx,GR_X
        add     ecx,GR_X/2-1
        lea     edi,[ecx+ebx]
        mov     [tunneldepth + edi*dword],eax
        sub     ecx,ebx
        mov     [tunneldepth + (ecx+1)*dword],eax

        inc     ebx
        cmp     ebx,GR_X/2
        jle     @@loop1
        inc     ebp
        cmp     ebp,GR_Y/2
        jle     @@loop2

        popad
        ret
        endp
; [fold]  ]

;����������������������������������������������������������������������������
; [fold]  [
; Draw tunnel

        proc    drawtunnel_
        pushad

        mov     ecx,GR_SIZE-1
        mov     ebp,GR_Y-1
@@loop2:

        mov     ebx,GR_X-1
@@loop1:
        mov     edx,[tunneldepth + ecx*dword]
;        cmp     edx,MAX_TUNNEL_DEPTH        ; Do not draw if we are too far
;        jg      @@done

        mov     edi,edx                     ; Save depth for sine
        add     edi,[tunnel_fwd]            ; Depth + tunnel_fwd
        and     edi,256-1                   ; Y-coordinate for texture map
        cmul    edi,256
        movzx   esi,[angletab + ecx*byte]

        mov     eax,edx                     ; Save depth for fade
        cdiv    eax,8
        add     eax,[tunnel_roll]           ; Depth / 8 + roll
        and     eax,SIN_NUM-1               ; Index for sine table
        mov     eax,[sintable + eax*dword]
        chgfrac eax,SIN_FRACBITS,7          ; Change to 7-bit fractional part
        add     esi,eax                     ; Add sine or tunnel_roll

        and     esi,256-1                   ; X-coordinate for texture map
        movzx   eax,[tunnel_texmap + esi+edi]

        add     ecx,[destbufoff]
;        mov     [byte ptr ecx],0
        cdiv    edx,2
        sub     eax,edx                     ; Fade tunnel to the end
        jl      @@black
        mov     [ecx],al
@@black:
        sub     ecx,[destbufoff]

;@@done:
        dec     ecx
        dec     ebx
        jge     @@loop1
        dec     ebp
        jge     @@loop2

        popad
        ret
        endp
; [fold]  ]

        ends
; [fold]  ]

;�����������������������������������������������������������������������������
; Data
;�����������������������������������������������������������������������������
; [fold]  [

                _data

tunnel_fwd      dd      0
tunnel_roll     dd      0

                ends
; [fold]  ]

;�����������������������������������������������������������������������������
; Uninitialized data
;�����������������������������������������������������������������������������
; [fold]  [

                _udata

tunneldepth     dd      GR_SIZE dup (?)

                ends
; [fold]  ]

        end

; [fold]  6
