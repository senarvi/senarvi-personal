;袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴
; Video routines
; Copyright (c) Seppo Enarvi  1996,1997
;袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴

        include "declare.mac"

        include "video.inc"
        include "main.inc"
        include "memory.inc"
        include "allocate.inc"
        include "math.inc"
        include "system.inc"

        include "system.mac"
        include "math.mac"
        include "opcode.mac"


;############################################################################
; Code
;############################################################################
; [fold]  [

        deccode32

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
; Set video palette
;
; Registers modified:
;   none

        proc    setpalette c near
        pushad
        mov     ecx,GRAPHCOLORS-1

@@loop:
        mov     edx,3c8h
        mov     eax,ecx
        out     dx,al

        mov     edx,3c9h
        mov     al,[fs:_palette+ecx*2+ecx]
        out     dx,al
        mov     al,[fs:_palette+ecx*2+ecx+1]
        out     dx,al
        mov     al,[fs:_palette+ecx*2+ecx+2]
        out     dx,al

        dec     ecx
        jge     @@loop

        popad
        ret
        endp
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
; Set palette for phong
;
; In:
;   esi - offset to material
;
; Registers modified:
;   none

        proc    setphongpal c near
        pushad
ORG_FRACBITS    =       FRACBITS
FRACBITS        =       PAL_FRACBITS
        mov     [byte fs:_palette+0],0
        mov     [byte fs:_palette+1],0
        mov     [byte fs:_palette+2],0


        mov     edi,GRAPHCOLORS-1
@@loop:
        ; red
        mov     ebx,[esi+smaterial.ambient.r]
        mov     eax,edi
        mov     edx,[esi+smaterial.diffuse.r]
        mul     edx
        shrd    eax,edx,FRACBITS
        add     ebx,eax

        mov     eax,edi
        mov     ecx,[esi+smaterial.shine]
        call    pow
        mov     edx,[esi+smaterial.specular.r]
        mul     edx
        shrd    eax,edx,FRACBITS
        add     eax,ebx
        mov     [fs:_palette+edi*2+edi],al


        ; green
        mov     ebx,[esi+smaterial.ambient.g]
        mov     eax,edi
        mov     edx,[esi+smaterial.diffuse.g]
        mul     edx
        shrd    eax,edx,FRACBITS
        add     ebx,eax

        mov     eax,edi
        mov     ecx,[esi+smaterial.shine]
        call    pow
        mov     edx,[esi+smaterial.specular.g]
        mul     edx
        shrd    eax,edx,FRACBITS
        add     eax,ebx
        mov     [fs:_palette+edi*2+edi+1],al


        ; blue
        mov     ebx,[esi+smaterial.ambient.b]
        mov     eax,edi
        mov     edx,[esi+smaterial.diffuse.b]
        mul     edx
        shrd    eax,edx,FRACBITS
        add     ebx,eax

        mov     eax,edi
        mov     ecx,[esi+smaterial.shine]
        call    pow
        mov     edx,[esi+smaterial.specular.b]
        mul     edx
        shrd    eax,edx,FRACBITS
        add     eax,ebx
        mov     [fs:_palette+edi*2+edi+2],al

        dec     edi
        jg      @@loop


        call    setpalette
FRACBITS = ORG_FRACBITS
        popad
        ret
        endp
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
; Set linear palette
;
; Registers modified:
;   none

        proc    setlinearpal c near
        pushad

        mov     ecx,GRAPHCOLORS-1
@@loop:
        mov     eax,ecx
        mov     [fs:_palette+ecx*2+ecx],al
        mov     [fs:_palette+ecx*2+ecx+1],al
        mov     [fs:_palette+ecx*2+ecx+2],al
        dec     ecx
        jge     @@loop
        call    setpalette

        popad
        ret
        endp
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
        proc    setwavepal c near
        pushad
        mov     [byte fs:_palette+0],0
        mov     [byte fs:_palette+1],0
        mov     [byte fs:_palette+2],0

        mov     ecx,GRAPHCOLORS/2-1
@@setwavepall0:
        mov     [byte fs:_palette+ecx*2+ecx+0],0
        mov     [byte fs:_palette+ecx*2+ecx+1],0
        mov     eax,ecx
        cdiv    eax,2
        mov     [fs:_palette+ecx*2+ecx+2],al
        dec     ecx
        jg      @@setwavepall0

        mov     ecx,GRAPHCOLORS-1
@@setwavepall1:
        mov     eax,ecx
        sub     eax,GRAPHCOLORS/2
        cdiv    eax,2
        mov     [fs:_palette+ecx*2+ecx+0],al
        mov     [fs:_palette+ecx*2+ecx+1],al
        mov     [byte fs:_palette+ecx*2+ecx+2],63
        dec     ecx
        cmp     ecx,GRAPHCOLORS/2
        jge     @@setwavepall1

        call    setpalette
        popad
        ret
        endp
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
        proc    setflamepal c near
        pushad

        mov     [byte fs:_palette+0],0
        mov     [byte fs:_palette+1],0
        mov     [byte fs:_palette+2],0

        xor     ecx,ecx
@@setflamepall0:
        mov     [byte fs:_palette+ecx*2+ecx],cl
        mov     [byte fs:_palette+ecx*2+ecx+1],0
        mov     [byte fs:_palette+ecx*2+ecx+2],0
        inc     ecx
        cmp     ecx,40h
        jl      @@setflamepall0

@@setflamepall1:
        mov     [byte fs:_palette+ecx*2+ecx],3fh
        mov     [byte fs:_palette+ecx*2+ecx+1],cl
        mov     [byte fs:_palette+ecx*2+ecx+2],0
        inc     ecx
        cmp     ecx,80h
        jl      @@setflamepall1

@@setflamepall2:
        mov     [byte fs:_palette+ecx*2+ecx],3fh
        mov     [byte fs:_palette+ecx*2+ecx+1],3fh
        mov     [byte fs:_palette+ecx*2+ecx+2],cl
        inc     ecx
        cmp     ecx,0c0h
        jl      @@setflamepall2

;        mov     [duck_ambient.r],5
;        mov     [duck_ambient.g],0
;        mov     [duck_ambient.b],0
;        mov     [duck_diffuse.r],20
;        mov     [duck_diffuse.g],20
;        mov     [duck_diffuse.b],0
;        mov     [duck_specular.r],38
;        mov     [duck_specular.g],38
;        mov     [duck_specular.b],38
;        mov     [duck_shine],1
;        call    setduckpal

        call    setpalette
        popad
        ret
        endp
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
        proc    settunnelpal c near
        pushad

        mov     ecx,GRAPHCOLORS
@@settunnelpall0:
        mov     eax,ecx
        cdiv    eax,4
        mov     [fs:_palette+ecx*2+ecx],al
;        cmul    al,2
        mov     [fs:_palette+ecx*2+ecx+1],al
;        cmul    al,2
        mov     [fs:_palette+ecx*2+ecx+2],al
        dec     ecx
        jge     @@settunnelpall0

        call    setpalette
        popad
        ret
        endp
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
; Write ASCIIZ string to screen
;
; In:
;   [esi] - string
;   [textx],[texty] - current location of cursor
;
; Registers modified:
;   none

        alignstart
        proc    writestr c near
        pushad

        mov     ah,07h
@@loop:
        mov     al,[esi]
        or      al,al
        jz      @@writestrf0
        inc     esi
        cmp     al,CR
        je      @@writestrf1

        mov     edi,[texty]
        cmul    edi,TEXTWIDTH
        add     edi,[textx]
        mov     [gs:_VGA_text_buffer+edi*2],ax

        cmp     [textx],TEXTWIDTH-1
        jae     @@writestrf1
        inc     [textx]
        jmp     @@loop

@@writestrf1:
        mov     [textx],0
        inc     [texty]
        jmp     @@loop

@@writestrf0:
        popad
        ret
        endp 
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
; Clear text mode screen
;
; Registers modified:
;   none

        alignstart
        proc    cleartext c near
        pushad

        mov     edi,TEXTWIDTH*TEXTHEIGHT/2-1
        mov     eax,07000700h
@@loop:
        mov     [gs:_VGA_text_buffer+edi*4],eax
        dec     edi
        jge     @@loop
        mov     [textx],0
        mov     [texty],0

        popad
        ret 
        endp
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
; Clear video memory
;
; Registers modified:
;   none

        alignstart
        proc    clearvideo c near
        push    ecx
        mov     ecx,GRAPHWIDTH*GRAPHHEIGHT-4

        aligncode
@@loop:
        mov     [dword gs:_VGA_graphics_buffer+ecx],00000000h
        sub     ecx,4
        jge     @@loop
        pop     ecx
        ret 
        endp
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
; Clear video buffer
;
; Registers modified:
;   ecx ebx

        alignstart
        proc    clearbuffer c near
        mov     ebx,[destbufoff]
        mov     ecx,GRAPHWIDTH*GRAPHHEIGHT-12

        aligncode
@@loop:
        mov     [dword fs:0+ebx+ecx],0                  ; 8 bytes
        mov     [dword fs:0+ebx+ecx+4],0                ; 9 bytes
        mov     [dword fs:0+ebx+ecx+8],0                ; 9 bytes
        sub     ecx,12                                  ; 3 bytes
        jge     @@loop                                  ; 2 bytes
                                                        ; --------
                                                        ; 31 bytes
        ret 
        endp
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
; Clear all video buffers
;
; Registers modified:
;   none

        alignstart
        proc    clearbuffers c near
        push    ecx
        mov     ecx,NUM_BUFFERS*GRAPHHEIGHT*GRAPHWIDTH*byte-4

        aligncode
@@loop:
        mov     [dword fs:_screenbuf+ecx],00000000h
        sub     ecx,4
        jge     @@loop
        pop     ecx
        ret 
        endp
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
; Copy video buffer to screen
;
; Registers modified:
;   ecx ebx

        alignstart
        proc    copybuffer c near
        mov     ebx,[destbufoff]
        mov     ecx,GRAPHWIDTH*GRAPHHEIGHT-4

        aligncode
@@loop:
        mov     eax,[fs:0+ebx+ecx]
        mov     [gs:_VGA_graphics_buffer+ecx],eax
        sub     ecx,4
        jge     @@loop
        ret 
        endp
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
; Clear quadrangle area from video buffer
;
; Registers modified:
;   all

        alignstart
        proc    cleararea c near
        mov     ecx,[bufferarea.a.y]
        mov     edx,[bufferarea.b.x]
        mov     ebx,[bufferarea.b.y]

        sub     ebx,ecx
        sub     edx,[bufferarea.a.x]

        cmul    ecx,GRAPHWIDTH
        add     ecx,[bufferarea.a.x]

        cdiv    edx,8
        aligncode
@@loop2:
        mov     eax,edx                 ; deltax / 4
@@loop1:
        mov     [dword fs:_screenbuf+ecx+eax*8],0
        mov     [dword fs:_screenbuf+ecx+eax*8+4],0
        dec     eax
        jge     @@loop1
        add     ecx,GRAPHWIDTH
        dec     ebx                     ; deltay
        jge     @@loop2
        ret
        endp
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
; Copy quadrangle area to screen
;
; Registers modified:
;   all

        alignstart
        proc    copyarea c near
        mov     eax,[screenarea.a.x]
        cmp     eax,[bufferarea.a.x]
        jb      @@copyareaf0
        mov     eax,[bufferarea.a.x]
@@copyareaf0:

        mov     ecx,[screenarea.a.y]
        cmp     ecx,[bufferarea.a.y]
        jb      @@copyareaf1
        mov     ecx,[bufferarea.a.y]
@@copyareaf1:

        mov     edx,[screenarea.b.x]
        cmp     edx,[bufferarea.b.x]
        ja      @@copyareaf2
        mov     edx,[bufferarea.b.x]
@@copyareaf2:

        mov     ebx,[screenarea.b.y]
        cmp     ebx,[bufferarea.b.y]
        ja      @@copyareaf3
        mov     ebx,[bufferarea.b.y]
@@copyareaf3:

        mov     ebp,[bufferarea.a.x]
        mov     [screenarea.a.x],ebp
        mov     ebp,[bufferarea.a.y]
        mov     [screenarea.a.y],ebp
        mov     ebp,[bufferarea.b.x]
        mov     [screenarea.b.x],ebp
        mov     ebp,[bufferarea.b.y]
        mov     [screenarea.b.y],ebp


        sub     ebx,ecx
        sub     edx,eax

        cmul    ecx,GRAPHWIDTH
        add     ecx,eax

        cdiv    edx,4
        aligncode
@@loop2:
        mov     eax,edx                 ; deltax / 4
@@loop1:
        mov     esi,[fs:_screenbuf+ecx+eax*4]
        mov     [gs:_VGA_graphics_buffer+ecx+eax*4],esi
        dec     eax
        jge     @@loop1
        add     ecx,GRAPHWIDTH
        dec     ebx                     ; deltay
        jge     @@loop2
        ret
        endp
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
; Copy video buffer to screen with motion blur
;
; Registers modified:
;   all

        alignstart
        proc    motionblur c near
        mov     ebx,[destbufoff]
        mov     ebp,[prevbufoff]
        mov     ecx,GRAPHWIDTH*GRAPHHEIGHT-byte

;        aligncode
@@loop:
        movzx   eax,[byte fs:0+ebx+ecx]
        movzx   edx,[byte fs:0+ebp+ecx]
        add     eax,edx
        cdiv    eax,2
        mov     [gs:_VGA_graphics_buffer+ecx],dl
        mov     [fs:0+ebp+ecx],al
        dec     ecx
        jge     @@loop
        ret
        endp
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
; Draw line
;
; In:
;   ecx = X2
;   edx = Y2
;   ebx = highword 0
;   esi = X1
;   edi = Y1
;
; Registers modified:
;   all

        proc    drawbrsline c near
        sub     ecx,esi          ; u = x2 - x1
        mov     ebx,1            ; d1x = sgn u
        jg      @@drawlinef0
        mov     ebx,-1
        neg     ecx              ; m = abs u
@@drawlinef0:
        lea     esi,[edi*4+edi]
        cmul    esi,64

        sub     edx,edi          ; v = y2 - y1
        mov     eax,320          ; d1y = sgn v
        jg      @@drawlinef1
        mov     eax,-320
        neg     edx              ; n = abs v
@@drawlinef1:
        lea     edi,[eax+ebx]    ; d1 = x + d1x

        cmp     ecx,edx
        jge     @@drawlinef2
        xchg    ecx,edx          ; ecx >= edx
        mov     ebx,eax          ; d2 = y + d1y
@@drawlinef2:


        mov     eax,ecx
        shr     eax,1
        mov     ebp,ecx
        sub     edx,ecx
        sub     ebx,edi

@@loop:
        db      FS_OVERRIDE
        mov_esi_imm8 _screenbuf,brslinecolor
        dec     ecx
        jl      @@return

        add     esi,edi
        add     eax,edx
        jge     @@loop
        add     eax,ebp
        add     esi,ebx
        jmp     @@loop


@@return:
        ret
        endp
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
        proc    drawline c near
        enter   0,0
        pushad

        mov     ecx,[param]
        mov     edx,[param+4]
        mov     esi,[param+8]
        mov     edi,[param+12]

        cmul    ecx,320
        cmul    ebx,320
        mov     eax,[param+16]
        mov     [byte fs:_screenbuf+ecx+edx],al
        mov     [byte fs:_screenbuf+esi+edi],al

        popad
        leave
        ret
        endp

        ends
; [fold]  ]

;############################################################################
; Data
;############################################################################
; [fold]  [

        decdata

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
label   screenbufoff dword
COUNT = _screenbuf
rept    NUM_BUFFERS
  dd    COUNT
  COUNT = COUNT+(GRAPHWIDTH*GRAPHHEIGHT)
endm

prevbufoff      dd      ?
destbufoff      dd      ?
destbufnum      dd      ?

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
textx   dd      0
texty   dd      0

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
label   duckmat smaterial
scolor  <10, 10, 0>             ; Ambient
scolor  <20, 20, 0>             ; Diffuse
scolor  <33, 33, 33>            ; Specular
dd      4                       ; Shine

label   rockmat smaterial
scolor  <1,  1,  3>             ; Ambient
scolor  <10, 20, 30>            ; Diffuse
scolor  <30, 30, 30>            ; Specular
dd      3                       ; Shine

label   wavemat smaterial
scolor  <0,  0,  0>             ; Ambient
scolor  <10, 30, 30>            ; Diffuse
scolor  <33, 33, 33>            ; Specular
dd      3                       ; Shine

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
bufferarea      squad   ?
screenarea      squad   ?

        ends 
; [fold]  ]

        end

; [fold]  18
