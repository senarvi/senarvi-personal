        %title  "Copyright (c) Seppo Enarvi  1996,1997"
        %subttl "Main code"

        if1
        display ""
        display "ravage/STATiC"
        display "Since the beginning of 1996"
        display ""
        display "Greets:"
        display "Tran"
        display "Mike Dailly"
        display "Il£vatar of Nordic Line"
        display "EMiMENT DOOM"
        display "Lobon"
        display ""
        endif


        include "declare.mac"


        include "video.mac"
        include "int.mac"
        include "math.mac"
        include "system.mac"

        include "main.inc"
        include "init.inc"
        include "int.inc"
        include "kb.inc"
        include "allocate.inc"
        include "math.inc"
        include "video.inc"
        include "object.inc"
        include "vector.inc"
        include "flame.inc"
        include "tunnel.inc"
        include "bumpmap.inc"
        include "smoke.inc"


        PAUSECHECK = 0
        COUNTFPS   = 0

        macro   setpitchain handler
        if      COUNTFPS
        mov     [frames],0
        endif
        mov     [time],0
        mov     [pitchainint],offset handler
        endm

        macro   mainloop
        if      COUNTFPS
        add     [frames],ONE
        endif
        mov     eax,[time]
        if      PAUSECHECK
@@pauseloop:
        cmp     [pauseflag],1
        je      @@pauseloop
        mov     [time],eax
        endif
        cmp     eax,[duration]
        jb      @@loop
        endm


;############################################################################
; Code
;############################################################################
; [fold]  (

        deccode32

;----------------------------------------------------------------------------
; [fold]  [
        proc    stc_bump c near
        mov     [bufferarea.a.x],0
        mov     [bufferarea.a.y],0
        mov     [bufferarea.b.x],GRAPHWIDTH
        mov     [bufferarea.b.y],GRAPHHEIGHT

        mov     esi,offset rockmat
        call    setphongpal


        align   32
@@loop:
        call    copyarea
        call    cleararea
        call    bumpmap
        mainloop
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
        proc    rock c near
        mov     [rock_data.off.x],0
        mov     [rock_data.off.y],0
        mov     [rock_data.off.z],4*ROCK_ZOFFSET/3
        mov     [rock_data.angle.a],0
        mov     [rock_data.angle.b],0

        mov     [object_data],offset rock_data

        mov     esi,offset rockmat
        call    setphongpal

        setpitchain rock_pitchain


        aligncode
@@loop:
        mov     eax,[objcount.a]
        and     eax,0ffh
        mov     [rock_data.angle.a],eax
        mov     edx,[objcount.b]
        and     edx,0ffh
        mov     [rock_data.angle.b],edx

        if      TEXMAP_GETAREA
        call    cleararea
        else
        call    clearbuffer
        endif

        call    rotate
        call    sortfaces
        call    texturemap

        if      TEXMAP_GETAREA
        call    copyarea
        else
        call    copybuffer
        endif
;        call    motionblur
        mainloop
        ret


        alignstart
rock_pitchain:
        sub     [byte ptr objcount.a],2
        inc     [byte ptr objcount.b]
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
        proc    tunnel c near
        mov     [bufferarea.a.x],0
        mov     [bufferarea.a.y],0
        mov     [bufferarea.b.x],GRAPHWIDTH
        mov     [bufferarea.b.y],GRAPHHEIGHT

        call    settunnel
        call    settunnelpal

        setpitchain tunnel_pitchain


        aligncode
@@loop:
        mov     eax,[tunnel_count.x]
        mov     [tunnel_x],eax
        mov     eax,[tunnel_count.y]
        mov     [tunnel_y],eax

        call    drawtunnel
        call    copyarea
        mainloop
        ret
        endp


        alignstart
tunnel_pitchain:
        add     [tunnel_count.y],3
        ret
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
        proc    flame c near
        mov     [bufferarea.a.x],0
        mov     [bufferarea.a.y],0
        mov     [bufferarea.b.x],GRAPHWIDTH
        mov     [bufferarea.b.y],GRAPHHEIGHT

        call    clearflamebuf
        call    setflamepal

        setpitchain nullchainint


@@loop:
        call    copyflamebuf
        call    copyarea
        call    randomflame
        call    flamescreen
        mainloop
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
        proc    wave c near
        mov     eax,100
        mov     [wave_amplitude],eax
        call    setwave

        mov     esi,offset wavemat
        call    setphongpal

        setpitchain wave_pitchain


        aligncode
@@loop:
        call    make_wave
        call    calc_bumpmap
        mov     eax,[wave_count]
        mov     [wave_add],eax
;        cdiv    eax,4
;        and     eax,127
;        mov     [wave_amplitude],eax
        mainloop
        endp


wave_pitchain:
        sub     [wave_count],7
        ret
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
        proc    static c near
        mov     [obj12_data.off.x],0
        mov     [obj12_data.off.y],0
        mov     [obj12_data.off.z],1000
        mov     [obj12_data.angle.a],0
        mov     [obj12_data.angle.b],0

        mov     esi,offset rockmat
        call    setphongpal

        setpitchain rock_pitchain


        aligncode
@@loop:
        mov     eax,[objcount.a]
        and     eax,0ffh
        mov     [obj12_data.angle.a],eax
        mov     edx,[objcount.b]
        and     edx,0ffh
        mov     [obj12_data.angle.b],edx

        call    clearbuffer

        mov     [object_data],offset obj12_data
        call    rotate
        call    sortfaces
        call    phong

        call    copybuffer
        mainloop
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
        proc    stc_smoke c near
        call    setlinearpal

        setpitchain nullchainint


        aligncode
@@loop:
        call    clearbuffer
        call    smoke
        call    copybuffer
        mainloop
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  (
; Main code

start:
        mov     [object_data],offset rock_data
        call    setnormals
        call    clearbuffers
        call    clearvideo
        setgraph

;        mov     [duration],TUNNEL_DURATION
;        call    tunnel
;        call    clearbuffers
;        call    clearvideo

;        mov     [duration],ROCK_DURATION
;        call    rock
;        call    clearbuffers
;        call    clearvideo

;        mov     [duration],FLAME_DURATION
;        call    flame
;        call    clearbuffers
;        call    clearvideo

;        mov     [duration],WAVE_DURATION
;        call    wave
;        call    clearbuffers
;        call    clearvideo

;        mov     [duration],1000
;        call    stc_smoke
;        call    clearbuffers
;        call    clearvideo

        jmp     exit
; [fold]  )

;----------------------------------------------------------------------------
; [fold]  [
        proc    pithandler far
        cli
        pushad
        inc     [time]
        call    [pitchainint]
        endint
        popad
        sti
        iretd
        endp

nullchainint:
        ret
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
        proc    kbhandler far
        cli
        push    eax

        in      al,KB_PORT
        cmp     al,KB_ESC
        je      intexit
        cmp     al,KB_P
        sete    [pauseflag]

        endint
        pop     eax
        sti
        iretd
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
        proc    printfps c near
        enter   0,0

        mov     ecx,[param]
        fixdiv  [time],ecx              ; Convert time to seconds
        or      eax,eax
        jz      @@printfpsf0
        mov     ecx,eax
        mov     eax,100
        mul     [frames]
        div     ecx                     ; 100 * frames / time in seconds

        mov     edx,eax
        mov     ecx,5
        lea     edi,[fpsstring]
        call    int2str
        lea     esi,[fpsstring]
        call    writestr

@@printfpsf0:
        leave
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  (
intexit:
        mov     esp,STACKLENGTH*10h
        endint
        sti

exit:
        resetpit
        call    resetmem
        settext
        if      COUNTFPS
        call    printfps c,TIMER_FREQUENCY
        endif
        endprog
; [fold]  )

        ends
; [fold]  )

;############################################################################
; Data
;############################################################################
; [fold]  [

                decdata

;----------------------------------------------------------------------------
memreq          dd      MEMREQ
heapseg         dd      SSEG+STACKLENGTH
basememreq      dd      0

;----------------------------------------------------------------------------
objcount        sangle  ?
wave_count      dd      600
tunnel_count    spixel  <0,0>

;----------------------------------------------------------------------------
pitchainint     dd      offset nullchainint

time            dd      0
duration        dd      -1
frames          dd      0
fpsstring       db      5 dup ('0'),' frames / 100 seconds',CR,00h

;----------------------------------------------------------------------------
pauseflag       db      0

;----------------------------------------------------------------------------
wave_add        dd      0
tunnel_x        dd      0
tunnel_y        dd      0

        ends
; [fold]  ]

;############################################################################
; Stack
;############################################################################
; [fold]  [

        decstack

        db      STACKLENGTH*10h dup (?)

        ends
; [fold]  ]

        end     init16
                                     
; [fold]  15
