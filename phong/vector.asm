
                include declare.mac


                decstart


                public  setnormals

                extrn   sqrt:near
                include math.mac
                include vector.mac

                extrn   facetaboff:dword,vertextaboff:dword
                extrn   lastfaceoff:dword
                include memory.inc
                ORIGFRACBITS=FRACBITS
                NORMALFRACBITS=8;24


                deccode32

;----------------------------------------------------------------------------
; Searches for faces containing the vertex and counts the normal
; vectors of faces together
;
; In:
;   ecx - last face to check
;   edx - vertex to search for
;   ebx - face table
;
; Out:
;   [normalsum] - sum of normal vectors of faces containing the vertex
;   [numconnected] - number of faces containing the vertex
;
; Regs modified:
;   eax ecx

searchvertex:
                mov     eax,ebx[ecx.a]
                cmp     eax,edx
                je      @@searchvertexf0
                mov     eax,ebx[ecx.b]
                cmp     eax,edx
                je      @@searchvertexf0
                mov     eax,ebx[ecx.c]
                cmp     eax,edx
                jne     @@searchvertexf1
@@searchvertexf0:
                vadd    [normalsum],fs:_normaltab[ecx],[normalsum]
                inc     [numconnected]
@@searchvertexf1:
                sub     ecx,size sface
                jge     searchvertex
                ret

;----------------------------------------------------------------------------
; set normal vectors of each vertex

setnormals:
                pushad
                mov     ebx,[facetaboff]

;----------------------------------------------------------------------------
; get normal vectors of each face

                mov     ebp,[vertextaboff]
                mov     edi,[lastfaceoff]
@@setnormalsl0:
                ; vertex a
                mov     esi,ebx[edi.a]
                cmul    esi,3
                vmov    ds:ebp[esi*4],[polya]
                ; vertex b
                mov     esi,ebx[edi.b]
                cmul    esi,3
                vmov    ds:ebp[esi*4],[polyb]
                ; vertex c
                mov     esi,ebx[edi.c]
                cmul    esi,3
                vmov    ds:ebp[esi*4],[polyc]

                ; normal vector
                vsub    [polyc],[polya],[polyac]
                vsub    [polyb],[polya],[polyab]
                vintcrs [polyac],[polyab],[normal]

                ; n / û(n.x^2 + n.y^2 + n.z^2)
                mov     ecx,3
                vintdiv [normal],ecx,[normal]

;                mov     eax,[normal.x]
;                cmp     [testmax],eax
;                jge     @@testf0
;                mov     [testmax],eax
;@@testf0:       cmp     [testmin],eax
;                jle     @@testf1
;                mov     [testmin],eax
;@@testf1:
;                mov     [normal.x],1
;                mov     [normal.y],1
;                mov     [normal.z],1
;                mov     eax,[testmin]
;                mov     [out1],eax
;                mov     eax,[testmax]
;                mov     [out2],eax

                vintdot [normal],[normal],[npow2]
                mov     edx,[npow2]
                call    sqrt
                mov     [nlength],eax
                FRACBITS=NORMALFRACBITS
                vfixdiv [normal],[nlength],fs:_normaltab[edi]
                FRACBITS=ORIGFRACBITS

                ; next face
                sub     edi,size sface
                jge     @@setnormalsl0


;----------------------------------------------------------------------------
; get average of normal vectors surrounding the vertex

                mov     edi,[lastfaceoff]
@@setnormalsl1:
                mov     edx,ebx[edi.a]
                mov     ebp,edx
                cmul    ebp,3
                mov     [normalsum.x],0 shl FRACBITS
                mov     [normalsum.y],0 shl FRACBITS
                mov     [normalsum.z],0 shl FRACBITS
                mov     [numconnected],0
                mov     ecx,[lastfaceoff]
                call    searchvertex
                sal     [numconnected],NORMALFRACBITS
                vfixdiv [normalsum],[numconnected],fs:_pnormaltab[ebp*4]

                mov     edx,ebx[edi.b]
                mov     ebp,edx
                cmul    ebp,3
                mov     [normalsum.x],0 shl FRACBITS
                mov     [normalsum.y],0 shl FRACBITS
                mov     [normalsum.z],0 shl FRACBITS
                mov     [numconnected],0
                mov     ecx,[lastfaceoff]
                call    searchvertex
                sal     [numconnected],NORMALFRACBITS
                vfixdiv [normalsum],[numconnected],fs:_pnormaltab[ebp*4]

                mov     edx,ebx[edi.c]
                mov     ebp,edx
                cmul    ebp,3
                mov     [normalsum.x],0 shl FRACBITS
                mov     [normalsum.y],0 shl FRACBITS
                mov     [normalsum.z],0 shl FRACBITS
                mov     [numconnected],0
                mov     ecx,[lastfaceoff]
                call    searchvertex
                sal     [numconnected],NORMALFRACBITS
                vfixdiv [normalsum],[numconnected],fs:_pnormaltab[ebp*4]

                sub     edi,size sface
                jge     @@setnormalsl1

                popad
                ret


testmax dd -80000000h
testmin dd 7fffffffh
out1 dd 0
out2 dd 0
out3 dd 0
out4 dd 0
out5 dd 0
public out1,out2,out3,out4,out5

;----------------------------------------------------------------------------
                align   4
polya           svect   ?
polyb           svect   ?
polyc           svect   ?
polyac          svect   ?
polyab          svect   ?
normal          svect   ?
npow2           dd      ?
nlength         dd      ?

;----------------------------------------------------------------------------
normalsum       svect   ?
numconnected    dd      ?

                endcode32
                end
