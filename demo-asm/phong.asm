
        include "declare.mac"


        include "vector.mac"
        include "system.mac"
        include "opcode.mac"

        include "vector.inc"
        include "math.inc"
        include "allocate.inc"
        include "video.inc"
        include "data.inc"
        include "init.inc"


;############################################################################
; Code
;############################################################################
; [fold]  [

        deccode32

;----------------------------------------------------------------------------
; [fold]  [
; Draw every scanline
;
; Regs modified:
;   all

        alignstart
fillpoly:
        mov     edx,[polyarea.b.y]
        mov     eax,edx
        sub     eax,[polyarea.a.y]
        jz      return
        mov     [edgecount],eax

        sar     edx,FRACBITS
        mov     [scanline],edx


        aligncode
@@fillpolyl0:
        mov     esi,[scanline]
        cmul    esi,<size sedge/4>

        ; counteihin voisi lis„t„ puolikkaan
xcount          = dword fs:_ledgebuf+esi*4+sedge.x
xend            = dword fs:_redgebuf+esi*4+sedge.x
ecount_u        = dword fs:_ledgebuf+esi*4+sedge.env.u
ecount_v_lsb    = byte fs:_ledgebuf+esi*4+sedge.env.v
ecount_v_msb    = byte fs:_ledgebuf+1+esi*4+sedge.env.v

        mov     ecx,[xend]
        sub     ecx,[xcount]
        sar     ecx,FRACBITS
        mov     [delta.x],ecx

        mov     edi,[scanline]
        cmul    edi,GRAPHBUFFER_X
        mov     eax,[xcount]
        sar     eax,FRACBITS
        add     edi,eax
        add     edi,[destbufoff]


        ; Change to 8 bit fractional part
;        mov     eax,ecount_v
;        cmul    eax,256
;        sar     eax,FRACBITS
;        mov     ecount_v,eax
;
;        mov     eax,[grad.env.v]
;        cmul    eax,256
;        sar     eax,FRACBITS
;        mov     [grad.env.v],eax

        mov     ecx,[ecount_u]
        mov     ah,[ecount_v_lsb]       ; Fractional part of v
;       mov     edx,egrad_v
        db      0bah
egrad_v dd      ?

        xor     ebx,ebx                 ; Environment map offset
        mov     bh,[ecount_v_msb]       ; Row (int part of v)
        mov     bl,ch                   ; Column (int part of u)


        aligncode
@@hlinel0:
        mov     al,[envmap+ebx]
;       add     ecx,egrad_u
        db      81h,0c1h
egrad_u dd      ?
        mov     bl,ch                   ; Column (int part of u)
        add     ah,dl                   ; Update fractional part of v
        adc     bh,dh                   ; Update row (int part of v)

        mov     [fs:edi],al
        inc     edi
        dec     [delta.x]
        jnl     @@hlinel0


        dec     [scanline]
        sub     [edgecount],ONE
        jg      @@fillpolyl0
        ret
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
; Interpolate values for edge buffers

        aligncode
getedge:
        ; counteihin voisi lis„t„ puolikkaan
        macro   getdelta edgedataoff,count,delta
        mov     eax,[ebx+edgedataoff]
        mov     edx,[esi+edgedataoff]
        mov     count,edx
        sub     eax,edx
        mov     delta,eax
        endm

        getdelta svdata.scr.x,[count.x],[delta.x]
        cmp     [leftedge],1
        jne     @@getedgef0
        getdelta svdata.env.u,[count.env.u],[delta.env.u]
        getdelta svdata.env.v,[count.env.v],[delta.env.v]
@@getedgef0:


        ; Calculate gradients
        mov     ecx,[ebx+svdata.scr.y]
        sub     ecx,[esi+svdata.scr.y]
        jz      return
        fixdiv  [delta.x],ecx
        mov     [grad.x],eax
        cmp     [leftedge],1
        jne     @@getedgef1
        fixdiv  [delta.env.u],ecx
        mov     [edge_egrad.u],eax
        fixdiv  [delta.env.v],ecx
        mov     [edge_egrad.v],eax
@@getedgef1:


        mov     ecx,[esi+svdata.scr.y]
        sar     ecx,FRACBITS
        cmul    ecx,<size sedge/4>
        mov     edx,[ebx+svdata.scr.y]
        sar     edx,FRACBITS
        cmul    edx,<size sedge/4>

        aligncode
@@loop:
        macro   addgrad count,gradient,edgeoff
        mov     eax,count
        mov     [fs:edi+ecx*4+edgeoff],eax
        add     eax,gradient
        mov     count,eax
        endm

        addgrad [count.x],[grad.x],sedge.x
        cmp     [leftedge],1
        jne     @@getedgef2
        addgrad [count.env.u],[edge_egrad.u],sedge.env.u
        addgrad [count.env.v],[edge_egrad.v],sedge.env.v
@@getedgef2:

        add     ecx,EDGESIZE
        cmp     ecx,edx
        jbe     @@loop
return:
        ret
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
; Draw polygon

drawpoly:
        mov     ebx,offset pdata.a
        mov     esi,offset pdata.b
        mov     edi,offset pdata.c

        ;------------------------
        ; Sort vertices verticaly

        mov     eax,[ebx+svdata.scr.y]
        cmp     eax,[esi+svdata.scr.y]
        jng     @@sortverticesf0
        xchg    ebx,esi

        mov     eax,[ebx+svdata.scr.y]
@@sortverticesf0:                       ; a < b
        cmp     eax,[edi+svdata.scr.y]
        jng     @@sortverticesf1
        xchg    ebx,edi

@@sortverticesf1:                       ; a < c
        mov     eax,[esi+svdata.scr.y]
        cmp     eax,[edi+svdata.scr.y]
        jng     @@sortverticesf2
        xchg    esi,edi

        mov     eax,[esi+svdata.scr.y]
@@sortverticesf2:                       ; b < c
        mov     ebp,[ebx+svdata.scr.y]
        mov     ecx,[edi+svdata.scr.y]

        ; v1 = ebx  v2 = esi  v3 = edi
        ; y1 = ebp  y2 = eax  y3 = ecx

        mov     [face.a],ebx
        mov     [face.b],esi
        mov     [face.c],edi
        mov     [polyarea.a.y],ebp
        mov     [polyarea.b.y],ecx


        ;---------------------------------------------
        ; Calculate the length of the longset scanline

        sub     ecx,ebp
        jnz     @@drawpolyf0
        ret
@@drawpolyf0:
        sub     eax,ebp                 ; (y2 - y1)
        mov     edx,eax
        sal     eax,FRACBITS
        sar     edx,32-FRACBITS
        idiv    ecx                     ; / (y3 - y1)
        mov     [v2ratio],eax


        mov     ecx,[edi+svdata.scr.x]
        sub     ecx,[ebx+svdata.scr.x]
        imul    ecx                     ; * (x3 - x1)
        shrd    eax,edx,FRACBITS

        add     eax,[ebx+svdata.scr.x]
        sub     eax,[esi+svdata.scr.x]  ; + (x1 - x2)
        jnz     @@drawpolyf1
        ret
@@drawpolyf1:
        mov     [v2deltax],eax


        macro   getscangrad bufferoff,dest
        mov     eax,[v2ratio]           ; (y2 - y1) / (y3 - y1)

        mov     ecx,[edi+bufferoff]
        sub     ecx,[ebx+bufferoff]
        imul    ecx                     ; * (i3 - i1)
        shrd    eax,edx,FRACBITS

        add     eax,[ebx+bufferoff]
        sub     eax,[esi+bufferoff]     ; + (i1 - i2)

        mov     edx,eax
        sal     eax,FRACBITS
        sar     edx,32-FRACBITS
        idiv    [v2deltax]              ; deltai / deltax
        mov     dest,eax
        endm

        getscangrad svdata.env.u,[egrad_u]      ; Environment map u-
        getscangrad svdata.env.v,[egrad_v]      ; & v-gradient


        ;-------
        ; a - c
        mov     esi,[face.a]
        mov     ebx,[face.c]
        mov     edi,_redgebuf
        mov     [leftedge],0
        cmp     [v2deltax],0
        jg      @@drawpolyf2
        mov     edi,_ledgebuf
        mov     [leftedge],1
@@drawpolyf2:
        call    getedge

        ; a - b
        mov     esi,[face.a]
        mov     ebx,[face.b]
        mov     edi,_ledgebuf
        mov     [leftedge],1
        cmp     [v2deltax],0
        jg      @@drawpolyf3
        mov     edi,_redgebuf
        mov     [leftedge],0
@@drawpolyf3:
        call    getedge

        ; b - c
        mov     esi,[face.b]
        mov     ebx,[face.c]
        mov     edi,_ledgebuf
        mov     [leftedge],1
        cmp     [v2deltax],0
        jg      @@drawpolyf4
        mov     edi,_redgebuf
        mov     [leftedge],0
@@drawpolyf4:
        call    getedge


        ;--------------------------------------------
        ; Get area used by the face

        if      TEXMAP_GETAREA

        macro   checkxlimit source
        local   @@xlimitf0,@@xlimitf1
        mov     eax,source
        cmp     eax,[bufferarea.a.x]
        jge     @@xlimitf0
        mov     [bufferarea.a.x],eax
@@xlimitf0:
        cmp     eax,[bufferarea.b.x]
        jle     @@xlimitf1
        mov     [bufferarea.b.x],eax
@@xlimitf1:
        endm

        checkxlimit [pdata.a.scr.x]
        checkxlimit [pdata.b.scr.x]
        checkxlimit [pdata.c.scr.x]

        ; y
        mov     eax,[polyarea.a.y]
        cmp     eax,[bufferarea.a.y]
        jnl     @@getareaf6
        mov     [bufferarea.a.y],eax
@@getareaf6:
        mov     eax,[polyarea.b.y]
        cmp     eax,[bufferarea.b.y]
        jng     @@getareaf7
        mov     [bufferarea.b.y],eax
@@getareaf7:

        endif

        call    fillpoly
        ret
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
; Shade object with phong environment map

        aligncode
phong:
        if      TEXMAP_GETAREA
        mov     [bufferarea.a.x],GRAPHWIDTH*ONE
        mov     [bufferarea.a.y],GRAPHHEIGHT*ONE
        mov     [bufferarea.b.x],0
        mov     [bufferarea.b.y],0
        endif

        mov     esi,[object_data]
        mov     ebx,[esi+sobjdata.ftab]
        mov     ebp,[esi+sobjdata.faces]


        aligncode
@@loop:
        mov     edi,[fs:_faceorder+ebp*dword]
        mov     esi,[object_data]
        mov     edx,[esi+sobjdata.mtab]


        macro   getvertex source
        mov     esi,[ebx+edi+sface.source]
        cmul    esi,3
        mov     [vertexnum.source],esi
        vmov    fs:_rotatetab+esi*dword+svect,pdata.source.coord
        endm

        getvertex a
        getvertex b
        getvertex c

        vproject pdata.a
        vproject pdata.b
        vproject pdata.c


        ;---------------------------------
        ; Check that polygon must be drawn

;        cmp     [pdata.a.scr.x],0
;        jnl     @@limitf0
;        mov     [pdata.a.scr.x],0
;@@limitf0:
;        cmp     [pdata.a.scr.y],0
;        jnl     @@limitf1
;        mov     [pdata.a.scr.y],0
;@@limitf1:
;        cmp     [pdata.b.scr.x],0
;        jnl     @@limitf2
;        mov     [pdata.b.scr.x],0
;@@limitf2:
;        cmp     [pdata.b.scr.y],0
;        jnl     @@limitf3
;        mov     [pdata.b.scr.y],0
;@@limitf3:
;        cmp     [pdata.c.scr.x],0
;        jnl     @@limitf4
;        mov     [pdata.c.scr.x],0
;@@limitf4:
;        cmp     [pdata.c.scr.y],0
;        jnl     @@limitf5
;        mov     [pdata.c.scr.y],0
;@@limitf5:
;
;        cmp     [pdata.a.scr.x],(GRAPHWIDTH-1)*ONE
;        jng     @@limitf6
;        mov     [pdata.a.scr.x],(GRAPHWIDTH-1)*ONE
;@@limitf6:
;        cmp     [pdata.a.scr.y],(GRAPHHEIGHT-1)*ONE
;        jng     @@limitf7
;        mov     [pdata.a.scr.y],(GRAPHHEIGHT-1)*ONE
;@@limitf7:
;        cmp     [pdata.b.scr.x],(GRAPHWIDTH-1)*ONE
;        jng     @@limitf8
;        mov     [pdata.b.scr.x],(GRAPHWIDTH-1)*ONE
;@@limitf8:
;        cmp     [pdata.b.scr.y],(GRAPHHEIGHT-1)*ONE
;        jng     @@limitf9
;        mov     [pdata.b.scr.y],(GRAPHHEIGHT-1)*ONE
;@@limitf9:
;        cmp     [pdata.c.scr.x],(GRAPHWIDTH-1)*ONE
;        jng     @@limitf10
;        mov     [pdata.c.scr.x],(GRAPHWIDTH-1)*ONE
;@@limitf10:
;        cmp     [pdata.c.scr.y],(GRAPHHEIGHT-1)*ONE
;        jng     @@limitf11
;        mov     [pdata.c.scr.y],(GRAPHHEIGHT-1)*ONE
;@@limitf11:


        fshown  pdata
        jng     @@noshown


        pushad
        mov     edi,[object_data]
        mov     esi,[vertexnum.a]
        cmul    esi,4
        add     esi,[edi+sobjdata.ntab]
        vrotxy  fs:esi+svect, edi+sobjdata.angle, pdata.a.env
        mov     esi,[vertexnum.b]
        cmul    esi,4
        add     esi,[edi+sobjdata.ntab]
        vrotxy  fs:esi+svect, edi+sobjdata.angle, pdata.b.env
        mov     esi,[vertexnum.c]
        cmul    esi,4
        add     esi,[edi+sobjdata.ntab]
        vrotxy  fs:esi+svect, edi+sobjdata.angle, pdata.c.env

        add     [pdata.a.env.u],ONE
        sal     [pdata.a.env.u],FRACBITS-1
        add     [pdata.a.env.v],ONE
        sal     [pdata.a.env.v],FRACBITS-1
        add     [pdata.b.env.u],ONE
        sal     [pdata.b.env.u],FRACBITS-1
        add     [pdata.b.env.v],ONE
        sal     [pdata.b.env.v],FRACBITS-1
        add     [pdata.c.env.u],ONE
        sal     [pdata.c.env.u],FRACBITS-1
        add     [pdata.c.env.v],ONE
        sal     [pdata.c.env.v],FRACBITS-1

        sal     [pdata.a.coord.x],FRACBITS
        sal     [pdata.a.coord.y],FRACBITS
        sal     [pdata.a.coord.z],FRACBITS
        sal     [pdata.b.coord.x],FRACBITS
        sal     [pdata.b.coord.y],FRACBITS
        sal     [pdata.b.coord.z],FRACBITS
        sal     [pdata.c.coord.x],FRACBITS
        sal     [pdata.c.coord.y],FRACBITS
        sal     [pdata.c.coord.z],FRACBITS

        call    drawpoly
        popad

@@noshown:
        dec     ebp
        jge     @@loop


        if      TEXMAP_GETAREA
        shr     [bufferarea.a.x],FRACBITS
        shr     [bufferarea.a.y],FRACBITS
        shr     [bufferarea.b.x],FRACBITS
        shr     [bufferarea.b.y],FRACBITS
        endif
        ret
; [fold]  ]

        ends
; [fold]  ]

;############################################################################
; Data
;############################################################################
; [fold]  [

                decdata

                align   dword
;----------------------------------------------------------------------------
vertexnum       sface   ?
edgecount       dd      ?
scanline        dd      ?

;----------------------------------------------------------------------------
leftedge        dd      ?

;----------------------------------------------------------------------------
polyarea        squad   ?

                ends
; [fold]  ]

        end

; [fold]  6
