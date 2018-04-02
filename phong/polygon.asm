
                include declare.mac


                decstart


HALF            equ     (1 shl FRACBITS)/2

EDGEDATAX       equ     3*4
EDGEDATAY       equ     4*4
EDGEDATANX      equ     5*4
EDGEDATANY      equ     6*4
EDGEDATANZ      equ     7*4
EDGEDATAU       equ     8*4
EDGEDATAV       equ     9*4


                public  drawpolys

                include vector.mac
                include rotate.mac

                include memory.inc
                extrn   facetaboff:dword
                extrn   lastfacenum:dword,lastfaceoff:dword
                extrn   objoffset:svect,objangle:svect
                extrn   marble_texmap:byte
                extrn   lightangle:svect
                extrn   sqrt:near
                include polygon.inc
                extrn   buffera1:spixel,buffera2:spixel


                deccode32

;----------------------------------------------------------------------------
; Regs modified:
;   eax ecx edx ebx esi

fillpoly:
                mov     edx,[polya2.y]
                mov     eax,edx
                sub     eax,[polya1.y]
                mov     [edgecount],eax
                sar     edx,FRACBITS
                mov     [scanline],edx
@@fillpolyl0:
                mov     esi,[scanline]
                cmul    esi,EDGESIZE


                ; counteihin voisi lis„t„ puolikkaan
                mov     eax,fs:_redgebuf[esi*4+EDGENX]
                mov     edx,fs:_ledgebuf[esi*4+EDGENX]
                mov     [ncount.x],edx
                sub     eax,edx
                mov     [deltan.x],eax

                mov     eax,fs:_redgebuf[esi*4+EDGENY]
                mov     edx,fs:_ledgebuf[esi*4+EDGENY]
                mov     [ncount.y],edx
                sub     eax,edx
                mov     [deltan.y],eax

                mov     eax,fs:_redgebuf[esi*4+EDGENZ]
                mov     edx,fs:_ledgebuf[esi*4+EDGENZ]
                mov     [ncount.z],edx
                sub     eax,edx
                mov     [deltan.z],eax


                mov     ecx,fs:_redgebuf[esi*4+EDGEX]
                sub     ecx,fs:_ledgebuf[esi*4+EDGEX]
                jz      @@hlinef0
                vfixdiv [deltan],ecx,[nadd]
@@hlinef0:
                mov     [deltax],ecx
                sar     [deltax],FRACBITS


                mov     ebx,[scanline]
                cmul    ebx,GRAPHBUFFER_X
                mov     eax,fs:_ledgebuf[esi*4+EDGEX]
                shr     eax,FRACBITS
                add     ebx,eax



                align   4
@@hlinel0:
                ;----------------
                ; i = (n / ûn^2) ù l
                vfixdot [ncount],[ncount],ecx

                xor     eax,eax
                mov     edx,[ncount.x]
                shrd    eax,edx,32-FRACBITS
                sar     edx,32-FRACBITS
                idiv    dword ptr fs:_sqrttab[ecx*dword]
                imul    [light.x]
                shrd    eax,edx,FRACBITS
                mov     ebp,eax

                xor     eax,eax
                mov     edx,[ncount.y]
                shrd    eax,edx,32-FRACBITS
                sar     edx,32-FRACBITS
                idiv    dword ptr fs:_sqrttab[ecx*dword]
                imul    [light.y]
                shrd    eax,edx,FRACBITS
                add     ebp,eax

                xor     eax,eax
                mov     edx,[ncount.z]
                shrd    eax,edx,32-FRACBITS
                sar     edx,32-FRACBITS
                idiv    dword ptr fs:_sqrttab[ecx*dword]
                imul    [light.z]
                shrd    eax,edx,FRACBITS
                add     eax,ebp


                mov     byte ptr fs:_screenbuf[ebx],1
                cmul    eax,GRAPHCOLORS
                sub     eax,2 shl FRACBITS
;                or      eax,eax
                jle     @@hlinef3
                sar     eax,FRACBITS
                mov     fs:_screenbuf[ebx],al
@@hlinef3:

                mov     eax,[nadd.x]
                add     [ncount.x],eax
                mov     eax,[nadd.y]
                add     [ncount.y],eax
                mov     eax,[nadd.z]
                add     [ncount.z],eax

                inc     ebx
                dec     [deltax]
                jnl     @@hlinel0


                dec     [scanline]
                sub     [edgecount],1 shl FRACBITS
                jg      @@fillpolyl0
                ret

;----------------------------------------------------------------------------
getedge:
                ; counteihin voisi lis„t„ puolikkaan
                mov     eax,ebx[EDGEDATAX]
                mov     edx,esi[EDGEDATAX]
                mov     [xcount],edx
                sub     eax,edx
                mov     [deltax],eax

                mov     eax,ebx[EDGEDATANX]
                mov     edx,esi[EDGEDATANX]
                mov     [ncount.x],edx
                sub     eax,edx
                mov     [deltan.x],eax

                mov     eax,ebx[EDGEDATANY]
                mov     edx,esi[EDGEDATANY]
                mov     [ncount.y],edx
                sub     eax,edx
                mov     [deltan.y],eax

                mov     eax,ebx[EDGEDATANZ]
                mov     edx,esi[EDGEDATANZ]
                mov     [ncount.z],edx
                sub     eax,edx
                mov     [deltan.z],eax


                mov     ecx,ebx[EDGEDATAY]
                sub     ecx,esi[EDGEDATAY]
                jz      @@getedgef0
                fixdiv  [deltax],ecx
                mov     [xadd],eax
                vfixdiv [deltan],ecx,[nadd]
@@getedgef0:


                mov     ecx,esi[EDGEDATAY]
                sar     ecx,FRACBITS
                cmul    ecx,EDGESIZE
                mov     edx,ebx[EDGEDATAY]
                sar     edx,FRACBITS
                cmul    edx,EDGESIZE

@@getedgel0:
                mov     eax,[xcount]
                mov     fs:edi[ecx*4+EDGEX],eax
                add     eax,[xadd]
                mov     [xcount],eax

                mov     eax,[ncount.x]
                mov     fs:edi[ecx*4+EDGENX],eax
                add     eax,[nadd.x]
                mov     [ncount.x],eax

                mov     eax,[ncount.y]
                mov     fs:edi[ecx*4+EDGENY],eax
                add     eax,[nadd.y]
                mov     [ncount.y],eax

                mov     eax,[ncount.z]
                mov     fs:edi[ecx*4+EDGENZ],eax
                add     eax,[nadd.z]
                mov     [ncount.z],eax


                add     ecx,EDGESIZE
                cmp     ecx,edx
                jbe     @@getedgel0
                ret

;----------------------------------------------------------------------------
drawpoly        macro
                sal     [polya.x],FRACBITS
                sal     [polya.y],FRACBITS
                sal     [polya.z],FRACBITS
                sal     [polyb.x],FRACBITS
                sal     [polyb.y],FRACBITS
                sal     [polyb.z],FRACBITS
                sal     [polyc.x],FRACBITS
                sal     [polyc.y],FRACBITS
                sal     [polyc.z],FRACBITS

                ;-------
                ; a - c
                mov     esi,offset edgedataa
                mov     ebx,offset edgedatac
                mov     edi,_redgebuf
                mov     eax,[screena.y]
                cmp     eax,[screenc.y]
                jle     @@drawpolyf0
                mov     esi,offset edgedatac
                mov     ebx,offset edgedataa
                mov     edi,_ledgebuf
@@drawpolyf0:
                mov     eax,esi[EDGEDATAY]
                mov     [polya1.y],eax
                mov     eax,ebx[EDGEDATAY]
                mov     [polya2.y],eax
                call    getedge

                ; a - b
                mov     esi,offset edgedataa
                mov     ebx,offset edgedatab
                mov     edi,_ledgebuf
                mov     eax,[screena.y]
                cmp     eax,[screenb.y]
                jle     @@drawpolyf1
                mov     esi,offset edgedatab
                mov     ebx,offset edgedataa
                mov     edi,_redgebuf
@@drawpolyf1:
                mov     eax,esi[EDGEDATAY]
                cmp     eax,[polya1.y]
                jnl     @@drawpolyf8
                mov     [polya1.y],eax
@@drawpolyf8:
                mov     eax,ebx[EDGEDATAY]
                cmp     eax,[polya2.y]
                jng     @@drawpolyf9
                mov     [polya2.y],eax
@@drawpolyf9:
                call    getedge

                ; b - c
                mov     esi,offset edgedatab
                mov     ebx,offset edgedatac
                mov     edi,_ledgebuf
                mov     eax,[screenb.y]
                cmp     eax,[screenc.y]
                jle     @@drawpolyf2
                mov     esi,offset edgedatac
                mov     ebx,offset edgedatab
                mov     edi,_redgebuf
@@drawpolyf2:
                mov     eax,esi[EDGEDATAY]
                cmp     eax,[polya1.y]
                jnl     @@drawpolyf6
                mov     [polya1.y],eax
@@drawpolyf6:
                mov     eax,ebx[EDGEDATAY]
                cmp     eax,[polya2.y]
                jng     @@drawpolyf7
                mov     [polya2.y],eax
@@drawpolyf7:
                call    getedge


                ;--------------------------
                ; Get area used by the face

                ; a.x
                mov     eax,[screena.x]
                cmp     eax,[buffera1.x]
                jge     @@drawpolyf20
                mov     [buffera1.x],eax
@@drawpolyf20:
                cmp     eax,[buffera2.x]
                jle     @@drawpolyf21
                mov     [buffera2.x],eax
@@drawpolyf21:

                ; b.x
                mov     eax,[screenb.x]
                cmp     eax,[buffera1.x]
                jge     @@drawpolyf22
                mov     [buffera1.x],eax
@@drawpolyf22:
                cmp     eax,[buffera2.x]
                jle     @@drawpolyf23
                mov     [buffera2.x],eax
@@drawpolyf23:

                ; c.x
                mov     eax,[screenc.x]
                cmp     eax,[buffera1.x]
                jge     @@drawpolyf24
                mov     [buffera1.x],eax
@@drawpolyf24:
                cmp     eax,[buffera2.x]
                jle     @@drawpolyf25
                mov     [buffera2.x],eax
@@drawpolyf25:

                ; y
                mov     eax,[polya1.y]
                cmp     eax,[buffera1.y]
                jnl     @@drawpolyf14
                mov     [buffera1.y],eax
@@drawpolyf14:
                mov     eax,[polya2.y]
                cmp     eax,[buffera2.y]
                jng     @@drawpolyf15
                mov     [buffera2.y],eax
@@drawpolyf15:
                call    fillpoly
                endm

;----------------------------------------------------------------------------
; Sort faces by distance
; Ehk„ t„st„ selvi„„ nelj„ll„ kerralla / obun py”r„hdys?!!

sortfaces:
                pushad

                ; Clear pointers for buffer 1
                COUNT = 0
                rept    RADIX_SIZE
                mov     radixptr1[COUNT*4],0
                COUNT = COUNT+1
                endm

                ;------------------------------------------
                ; Sort first pass stright to radix buffer 1

                ; Offset of face table and last face
                mov     ebx,[facetaboff]
                mov     ecx,[lastfaceoff]
@@sortfacesl0:
                mov     esi,ebx[ecx].a          ; Number of vertex a
                ; Take distance from rotatetab
                TEMP = size sface/4
                cmul    esi,TEMP
                mov     eax,fs:_rotatetab[esi*4].z
                and     eax,RADIX_SIZE-1        ; Buffer number

                mov     edx,radixoff1[eax*4]
                mov     edi,radixptr1[eax*4]
                mov     fs:edx[edi*4],ecx       ; Offset of face
                inc     radixptr1[eax*4]
                sub     ecx,size sface
                jge     @@sortfacesl0


                ;---------------
                ; Rest of passes

                mov     ecx,RADIX_BITS
@@sortfacesl5:

                ;--------------------------
                ; From buffer 1 to buffer 2

                ; Clear pointers for buffer 2
                COUNT = 0
                rept    RADIX_SIZE
                mov     radixptr2[COUNT*4],0
                COUNT = COUNT+1
                endm

                ; RADIX_SIZE number of buffers
                xor     ebp,ebp
@@sortfacesl2:

                ; Every face in buffer
                mov     [radixcount],0
                cmp     ds:radixptr1[ebp*4],0
                jz      @@sortfacesf0
@@sortfacesl1:
                ; Take offset of face stored in current location of current
                ; source radix buffer
                mov     ebx,ds:radixoff1[ebp*4]
                mov     esi,[radixcount]
                mov     esi,fs:ebx[esi*4]

                ; Offset to vertex from face table
                mov     ebx,[facetaboff]
                mov     edi,ebx[esi].a
                cmul    edi,3

                ; Distance component of vector
                mov     eax,fs:_rotatetab[edi*4].z
                mov     edx,RADIX_SIZE-1
                shl     edx,cl
                and     eax,edx
                shr     eax,cl

                ; Store offset of face into current location of destination
                ; radix buffer
                mov     edx,radixoff2[eax*4]
                mov     edi,radixptr2[eax*4]
                mov     fs:edx[edi*4],esi

                inc     radixptr2[eax*4]
                inc     [radixcount]
                mov     edx,[radixcount]
                cmp     edx,ds:radixptr1[ebp*4]
                jl      @@sortfacesl1;f0?!
@@sortfacesf0:

                inc     ebp
                cmp     ebp,RADIX_SIZE
                jl      @@sortfacesl2

                add     ecx,RADIX_BITS
                cmp     ecx,32
                jge     @@sortfacesf1


                ;--------------------------
                ; ...and back to buffer 1

                ; Clear pointers for buffer 1
                COUNT = 0
                rept    RADIX_SIZE
                mov     radixptr1[COUNT*4],0
                COUNT = COUNT+1
                endm

                ; RADIX_SIZE number of buffers
                xor     ebp,ebp
@@sortfacesl4:

                ; Every face in buffer
                mov     [radixcount],0
                cmp     ds:radixptr2[ebp*4],0
                jz      @@sortfacesf2
@@sortfacesl3:
                ; Take offset of last radix buffer
                mov     ebx,ds:radixoff2[ebp*4]
                ; Take offset of face stored in current location of source
                ; radix buffer
                mov     esi,[radixcount]
                mov     esi,fs:ebx[esi*4]
                mov     ebx,[facetaboff]
                mov     edi,ebx[esi].a          ; Vertex a in face table
                cmul    edi,3
                ; Distance component of vector
                mov     eax,fs:_rotatetab[edi*4].z
                mov     edx,RADIX_SIZE-1
                shl     edx,cl
                and     eax,edx
                shr     eax,cl

                ; Store offset of face into current location of destination
                ; radix buffer
                mov     edx,radixoff1[eax*4]
                mov     edi,radixptr1[eax*4]
                mov     fs:edx[edi*4],esi
                inc     radixptr1[eax*4]
                inc     [radixcount]
                mov     edx,[radixcount]
                cmp     edx,ds:radixptr2[ebp*4]
                jl      @@sortfacesl3
@@sortfacesf2:

                inc     ebp
                cmp     ebp,RADIX_SIZE
                jl      @@sortfacesl4

                add     ecx,RADIX_BITS
;                cmp     ecx,32
;                jl      @@sortfacesl5
                jmp     @@sortfacesl5


                ;--------------------------------
                ; Copy from buffer 1 to faceorder

;                popad
;                ret


                ;--------------------------------
                ; Copy from buffer 2 to faceorder
@@sortfacesf1:
                xor     edi,edi
                xor     ebp,ebp                 ; RADIX_SIZE number of buffers
@@sortfacesl7:
                ; Every face in buffer
                xor     ecx,ecx
                cmp     ds:radixptr2[ebp*4],ecx
                jz      @@sortfacesf3
@@sortfacesl6:
                mov     ebx,ds:radixoff2[ebp*4]
                mov     eax,fs:ebx[ecx*4]
                mov     fs:_faceorder[edi*4],eax
                inc     edi
                inc     ecx
                cmp     ecx,ds:radixptr2[ebp*4]
                jl      @@sortfacesl6
@@sortfacesf3:
                inc     ebp
                cmp     ebp,RADIX_SIZE
                jl      @@sortfacesl7
                popad
                ret

;----------------------------------------------------------------------------
drawpolys:
                call    sortfaces

                pushad
                mov     [buffera1.x],GRAPHWIDTH shl FRACBITS
                mov     [buffera1.y],GRAPHHEIGHT shl FRACBITS
                mov     [buffera2.x],0
                mov     [buffera2.y],0

                mov     ebx,[facetaboff]
                mov     ebp,[lastfacenum]
@@drawpolysl0:
                mov     edi,fs:_faceorder[ebp*4]


                ; corner a
                mov     esi,ebx[edi.a]
                cmul    esi,3
                mov     [vertexnum.a],esi
                vmov    fs:_rotatetab[esi*4],[polya]

                ; corner b
                mov     esi,ebx[edi.b]
                cmul    esi,3
                mov     [vertexnum.b],esi
                vmov    fs:_rotatetab[esi*4],[polyb]

                ; corner c
                mov     esi,ebx[edi.c]
                cmul    esi,3
                mov     [vertexnum.c],esi
                vmov    fs:_rotatetab[esi*4],[polyc]

                project polya,screena
                project polyb,screenb
                project polyc,screenc

                cmp     [screena.x],(GRAPHWIDTH-1) shl FRACBITS
                ja      @@drawpolysf0
                cmp     [screena.y],(GRAPHHEIGHT-1) shl FRACBITS
                ja      @@drawpolysf0
                cmp     [screenb.x],(GRAPHWIDTH-1) shl FRACBITS
                ja      @@drawpolysf0
                cmp     [screenb.y],(GRAPHHEIGHT-1) shl FRACBITS
                ja      @@drawpolysf0
                cmp     [screenc.x],(GRAPHWIDTH-1) shl FRACBITS
                ja      @@drawpolysf0
                cmp     [screenc.y],(GRAPHHEIGHT-1) shl FRACBITS
                ja      @@drawpolysf0
                fshown  screena,screenb,screenc
                jng     @@drawpolysf0

                pushad
                mov     esi,[vertexnum.a]
                rotv    fs:_pnormaltab[esi*4],[objangle],[normala]
                mov     esi,[vertexnum.b]
                rotv    fs:_pnormaltab[esi*4],[objangle],[normalb]
                mov     esi,[vertexnum.c]
                rotv    fs:_pnormaltab[esi*4],[objangle],[normalc]
                rotv    [orglight],[lightangle],[light]

                drawpoly
                popad

@@drawpolysf0:
                dec     ebp
                jge     @@drawpolysl0


                shr     [buffera1.x],FRACBITS
                shr     [buffera1.y],FRACBITS
                shr     [buffera2.x],FRACBITS
                shr     [buffera2.y],FRACBITS
                popad
                ret


;############################################################################
; Data
;############################################################################

                align   dword

radixoff1       label   dword
                COUNT = 0
                rept    RADIX_SIZE
                dd      _radixbuf+MAX_FACES*COUNT*4
                COUNT = COUNT + 1
                endm
radixoff2       label   dword
                COUNT = 0
                rept    RADIX_SIZE
                dd      _radixbuf+MAX_FACES*RADIX_SIZE*4+MAX_FACES*COUNT*4
                COUNT = COUNT + 1
                endm
radixptr1       dd      RADIX_SIZE dup (0)
radixptr2       dd      RADIX_SIZE dup (0)
radixcount      dd      ?

;----------------------------------------------------------------------------
vertexnum       sface   ?
edgecount       dd      ?
scanline        dd      ?

;----------------------------------------------------------------------------
edgedataa       label   dword
polya           svect   ?
screena         spixel  ?
normala         svect   ?
texturea        smap    ?

edgedatab       label   dword
polyb           svect   ?
screenb         spixel  ?
normalb         svect   ?
textureb        smap    ?

edgedatac       label   dword
polyc           svect   ?
screenc         spixel  ?
normalc         svect   ?
texturec        smap    ?

;----------------------------------------------------------------------------
orglight        svect   <-1 shl FRACBITS,0 shl FRACBITS,0 shl FRACBITS>
light           svect   ?

;----------------------------------------------------------------------------
deltax          dd      ?
deltan          svect   ?

xadd            dd      ?
nadd            svect   ?

xcount          dd      ?
ncount          svect   ?

;----------------------------------------------------------------------------
polya1          spixel  ?
polya2          spixel  ?

                endcode32
                end

