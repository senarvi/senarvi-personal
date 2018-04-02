
                display "Aloitettu 1996 alussa"

                include declare.mac

                decstart

                decpmcode
                endpmcode
                deccode16
                endcode16
                deccode32
                endcode32
                decstack
                endstack


;----------------------------------------------------------------------------
                public  start
                public  exit

;----------------------------------------------------------------------------
                public  pause
                public  time
                public  facetaboff,vertextaboff
                public  lastfacenum,lastfaceoff
                public  lastvertexoff
                public  objoffset,objangle
                public  lightangle

;----------------------------------------------------------------------------
                extrn   init:near
                extrn   initmem:near,resetmem:near
                extrn   setnormals:near,rotate:near,drawpolys:near
                include video.mac
                extrn   setduckpal:near,setwavepal:near
                extrn   setflamepal:near,settunnelpal:near
                extrn   setsqrt:near
                extrn   clearbuffer:near,copybuffer:near,mblur:near
                extrn   cleartext:near,makehstr:near,writestr:near
                include int.mac
                include math.mac
                include system.mac

                extrn   clearflamebuf:near,copyflamebuf:near
                extrn   randomflame:near,flamescreen:near

                extrn   settunnel:near,drawtunnel:near

;----------------------------------------------------------------------------
                include misc.inc
                include memory.inc
                include video.inc
                extrn   st_vertextab:svect,st_facetab:sface
                extrn   ti_vertextab:svect,ti_facetab:sface
                extrn   ac_vertextab:svect,ac_facetab:sface
                extrn   duck_vertextab:svect,duck_facetab:sface
                extrn   buffera1:spixel,buffera2:spixel
                extrn   wavetab:dword
                include wave.inc


;############################################################################
; Code
;############################################################################

                deccode32

start:
;                mov     cx,320*200/4
;                xor     edi,edi
;@@clrbuff0:     mov     dword ptr fs:[edi],0
;                add     edi,4
;                dec     cx
;                jnz     @@clrbuff0

;                jmp     static
                jmp     duck
;                jmp     wave
;                jmp     flame
;                jmp     grill


;----------------------------------------------------------------------------
tunnel:
                mov     [buffera1.x],0
                mov     [buffera1.y],0
                mov     [buffera2.x],GRAPHWIDTH
                mov     [buffera2.y],GRAPHHEIGHT

                call    settunnelpal
                call    settunnel
@@tunnell0:
                call    drawtunnel
@@tunnell1:
                cmp     [pause],0
                jz      @@tunnell0
                jmp     @@tunnell1
;----------------------------------------------------------------------------
grill:
                mov     [buffera1.x],0
                mov     [buffera1.y],0
                mov     [buffera2.x],GRAPHWIDTH
                mov     [buffera2.y],GRAPHHEIGHT
                call    setduckpal
                call    setflamepal
                call    clearflamebuf
                call    clearbuffer

                mov     [objoffset.x],0
                mov     [objoffset.y],0
                mov     [objoffset.z],DUCK_ZOFFSET-500
                mov     [objangle.a],0
                mov     [objangle.b],0
                mov     [lightangle.a],0;(1 shl FRACBITS)/2
                mov     [lightangle.b],7*(1 shl FRACBITS)/8
                mov     [facetaboff],offset duck_facetab
                mov     [vertextaboff],offset duck_vertextab
                mov     [lastfacenum],DUCK_FACES-1
                mov     [lastfaceoff],(DUCK_FACES-1)*size sface
                mov     [lastvertexoff],(DUCK_VERTICES-1)*size svect
                call    setnormals

@@grilll0:
                call    clearbuffer
                call    randomflame
                call    flamescreen
                call    copyflamebuf

                call    rotate
                call    drawpolys
                mov     [buffera1.x],0
                mov     [buffera2.x],GRAPHWIDTH
                mov     [buffera2.y],GRAPHHEIGHT
                call    copybuffer

                add     byte ptr [objangle.b],9
                inc     [frames]
@@grilll3:
                cmp     [pause],0
                jz      @@grilll0
                jmp     @@grilll3

;----------------------------------------------------------------------------
flame:
                mov     [buffera1.x],0
                mov     [buffera1.y],0
                mov     [buffera2.x],GRAPHWIDTH
                mov     [buffera2.y],GRAPHHEIGHT
                call    setflamepal
                call    clearflamebuf

@@flamel0:
                call    copyflamebuf
                call    copybuffer
                call    randomflame
                call    flamescreen

@@flamel3:
                cmp     [pause],0
                jz      @@flamel0
                jmp     @@flamel3

;----------------------------------------------------------------------------
wave:
                call    setwavepal
                mov     [buffera1.x],0
                mov     [buffera1.y],0
                mov     [buffera2.x],GRAPHWIDTH
                mov     [buffera2.y],GRAPHHEIGHT

@@wavel0:
                call    copybuffer
                call    clearbuffer



@@wavel3:
                cmp     [pause],0
                je      @@wavel0
                jmp     @@wavel3


;----------------------------------------------------------------------------
duck:
                call    setduckpal

                mov     [objoffset.x],0
                mov     [objoffset.y],0
                mov     [objoffset.z],DUCK_ZOFFSET-500
                mov     [objangle.a],0
                mov     [objangle.b],0

                mov     [facetaboff],offset duck_facetab
                mov     [vertextaboff],offset duck_vertextab
                mov     [lastfacenum],DUCK_FACES-1
                mov     [lastfaceoff],(DUCK_FACES-1)*size sface
                mov     [lastvertexoff],(DUCK_VERTICES-1)*size svect
                call    setnormals

;                inittimer
                align   4
@@duckl0:

                call    copybuffer
                call    clearbuffer
                call    rotate
                call    drawpolys

                sub     byte ptr [objangle.a],4
                add     byte ptr [objangle.b],9
;                sub     byte ptr [lightangle.a],2
;                add     byte ptr [lightangle.b],1
                inc     [frames]

@@duckl1:
                cmp     [pause],0
                je      @@duckl0
                jmp     @@duckl1


;----------------------------------------------------------------------------
static:
                call    setduckpal

                mov     [objoffset.x],0
                mov     [objoffset.y],0
                mov     [objoffset.z],10000
                mov     [objangle.a],0
                mov     [objangle.b],0

                mov     [facetaboff],offset ac_facetab
                mov     [vertextaboff],offset ac_vertextab
                mov     [lastfacenum],AC_FACES-1
                mov     [lastfaceoff],(AC_FACES-1)*size sface
                mov     [lastvertexoff],(AC_VERTICES-1)*size svect
                call    setnormals

;                inittimer
                align   4
@@staticl0:

                call    copybuffer
                call    clearbuffer
                call    rotate
                call    drawpolys

                sub     byte ptr [objangle.a],4
                add     byte ptr [objangle.b],9
;                sub     byte ptr [lightangle.a],2
;                add     byte ptr [lightangle.b],1
                inc     [frames]

@@staticl1:
                cmp     [pause],0
                je      @@staticl0
                jmp     @@staticl1


;----------------------------------------------------------------------------
exit:
;                resettimer
                settext
                call    resetmem
                resetkb

                endint
                endprog


;############################################################################
; Data
;############################################################################

                align   dword

push_eax        label   dword
push_ax         label   word
push_al         db      ?
push_ah         db      ?,?,?
push_ecx        label   dword
push_cx         label   word
push_cl         db      ?
push_ch         db      ?,?,?
push_edx        label   dword
push_dx         label   word
push_dl         db      ?
push_dh         db      ?,?,?
push_ebx        label   dword
push_bx         label   word
push_bl         db      ?
push_bh         db      ?,?,?
push_ebp        label   dword
push_bp         dw      ?,?
push_esi        label   dword
push_si         dw      ?,?
push_edi        label   dword
push_di         dw      ?,?

;----------------------------------------------------------------------------
                align   4
facetaboff      dd      ?
vertextaboff    dd      ?
lastfaceoff     dd      ?
lastvertexoff   dd      ?
lastfacenum     dd      ?
lastvertexnum   dd      ?

objoffset       svect   ?
objangle        sangle  ?
lightangle      sangle  <19 shl FRACBITS / 20,18 shl FRACBITS / 20>

;----------------------------------------------------------------------------
time            dd      0
frames          dd      0
fpsstring       db      8 dup ('0'),'h fps',CR,00h

;----------------------------------------------------------------------------
circle          spixel  ?
circle_d        dd      ?
circle_r        dd      ?

;----------------------------------------------------------------------------
paramstr        db      '0x'
param1          db      8 dup ('0'),'  0x'
param2          db      8 dup ('0'),'  0x'
param3          db      8 dup ('0'),'  0x'
param4          db      8 dup ('0'),'  0x'
param5          db      8 dup ('0'),CR,00h
pause           db      0

                endcode32


;############################################################################
; Stack
;############################################################################

                decstack

                db      STACKLENGTH*10h dup (?)

                endstack


                end     init

