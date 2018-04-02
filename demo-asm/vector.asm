
        include "declare.mac"


        include "system.mac"
        include "math.mac"
        include "vector.mac"

        include "vector.inc"
        include "system.inc"
        include "math.inc"
        include "allocate.inc"


ORIGFRACBITS = FRACBITS


;############################################################################
; Code
;############################################################################
; [fold]  [

        deccode32

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
; Rotate object

        proc    rotate c near
        pushad

        mov     esi,[object_data]
        mov     edi,[esi+sobjdata.vend]
        aligncode
@@loop:
        mov     ebx,[esi+sobjdata.vtab]
        vmov    ebx+edi+svect,vertex
        sar     [vertex.x],OBJ_FRACBITS
        sar     [vertex.y],OBJ_FRACBITS
        sar     [vertex.z],OBJ_FRACBITS
        vrot    vertex, esi+sobjdata.angle, fs:_rotatetab+edi+svect
        vadd    fs:_rotatetab+edi+svect, esi+sobjdata.off, fs:_rotatetab+edi+svect
        sub     edi,size svect
        jge     @@loop

        popad
        ret
        endp
; [fold]  ]

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Search for faces containing the vertex and counts the normal vectors of
; [fold]  [
; faces together
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

        alignstart
searchvertex:
        FRACBITS=NORMALFRACBITS

        mov     eax,[ebx+ecx+sface.a]
        cmp     eax,edx
        je      @@searchvertexf0
        mov     eax,[ebx+ecx+sface.b]
        cmp     eax,edx
        je      @@searchvertexf0
        mov     eax,[ebx+ecx+sface.c]
        cmp     eax,edx
        jne     @@searchvertexf1
@@searchvertexf0:
        vadd    normalsum,fs:_normaltab+ecx+svect,normalsum
        add     [numconnected],ONE
@@searchvertexf1:
        sub     ecx,size sface
        jge     searchvertex

        FRACBITS=ORIGFRACBITS
        ret
; [fold]  ]

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
; Set normal vectors of each vertex

        alignstart
        proc    setnormals c near
        enter   0,0
        pushad
        mov     esi,[object_data]
        mov     eax,[esi+sobjdata.ftab]
        mov     [facetaboff],eax
        mov     eax,[esi+sobjdata.fend]
        mov     [lastfaceoff],eax
        mov     eax,[esi+sobjdata.vtab]
        mov     [vertextaboff],eax
        mov     eax,[esi+sobjdata.ntab]
        mov     [normaltab],eax
        mov     ebx,[facetaboff]

        ;--------------------------------
        ; Get normal vectors of each face

        mov     ebp,[vertextaboff]
        mov     edi,[lastfaceoff]
        aligncode
@@loop1:
        ; Vertex a
        mov     esi,[ebx+edi+sface.a]
        cmul    esi,3
        vmov    ds:0+ebp+esi*4+svect,pdata.a.coord
        ; Vertex b
        mov     esi,[ebx+edi+sface.b]
        cmul    esi,3
        vmov    ds:0+ebp+esi*4+svect,pdata.b.coord
        ; Vertex c
        mov     esi,[ebx+edi+sface.c]
        cmul    esi,3
        vmov    ds:0+ebp+esi*4+svect,pdata.c.coord

        sar     [pdata.a.coord.x],OBJ_FRACBITS
        sar     [pdata.a.coord.y],OBJ_FRACBITS
        sar     [pdata.a.coord.z],OBJ_FRACBITS
        sar     [pdata.b.coord.x],OBJ_FRACBITS
        sar     [pdata.b.coord.y],OBJ_FRACBITS
        sar     [pdata.b.coord.z],OBJ_FRACBITS
        sar     [pdata.c.coord.x],OBJ_FRACBITS
        sar     [pdata.c.coord.y],OBJ_FRACBITS
        sar     [pdata.c.coord.z],OBJ_FRACBITS

        ; Normal vector
        vsub    pdata.c.coord,pdata.a.coord,polyac
        vsub    pdata.b.coord,pdata.a.coord,polyab
        vintcrs polyac,polyab,normal

        ;-----------------------------
        ; Normalize vector
        ; n / û(n.x^2 + n.y^2 + n.z^2)

        ; Make sure we won't overflow
        cdiv    [normal.x],NORMAL_DIVISOR
        cdiv    [normal.y],NORMAL_DIVISOR
        cdiv    [normal.z],NORMAL_DIVISOR

        vintdot normal,normal,[npow2]
        mov     edx,[npow2]
        call    sqrt
        mov     [nlength],eax
        FRACBITS=NORMALFRACBITS
        vfixdiv normal,[nlength],fs:_normaltab+edi+svect
        FRACBITS=ORIGFRACBITS

        ; Next face
        sub     edi,size sface
        jge     @@loop1


        ;------------------------------------------------------
        ; Get average of normal vectors surrounding each vertex

        mov     esi,[normaltab]
        mov     edi,[lastfaceoff]
        aligncode
@@loop2:
        FRACBITS=NORMALFRACBITS
        mov     edx,[ebx+edi+sface.a]
        mov     ebp,edx
        cmul    ebp,3
        mov     [normalsum.x],ZERO
        mov     [normalsum.y],ZERO
        mov     [normalsum.z],ZERO
        mov     [numconnected],ZERO
        mov     ecx,[lastfaceoff]
        call    searchvertex
        FRACBITS=ORIGFRACBITS
        vfixdiv normalsum,[numconnected],fs:esi+ebp*4+svect

        FRACBITS=NORMALFRACBITS
        mov     edx,[ebx+edi+sface.b]
        mov     ebp,edx
        cmul    ebp,3
        mov     [normalsum.x],ZERO
        mov     [normalsum.y],ZERO
        mov     [normalsum.z],ZERO
        mov     [numconnected],ZERO
        mov     ecx,[lastfaceoff]
        call    searchvertex
        FRACBITS=ORIGFRACBITS
        vfixdiv normalsum,[numconnected],fs:esi+ebp*4+svect

        FRACBITS=NORMALFRACBITS
        mov     edx,[ebx+edi+sface.c]
        mov     ebp,edx
        cmul    ebp,3
        mov     [normalsum.x],ZERO
        mov     [normalsum.y],ZERO
        mov     [normalsum.z],ZERO
        mov     [numconnected],ZERO
        mov     ecx,[lastfaceoff]
        call    searchvertex
        FRACBITS=ORIGFRACBITS
        vfixdiv normalsum,[numconnected],fs:esi+ebp*4+svect

        sub     edi,size sface
        jge     @@loop2
        popad
        leave
        ret
        endp
; [fold]  ]

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
; Sort faces by distance

        alignstart
        proc    sortfaces c near
        pushad

        ; Clear pointers for buffer 1
        COUNT = 0
        rept    RADIX_SIZE
        mov     [radixptr1+COUNT*4],0
        COUNT = COUNT+1
        endm

        ;------------------------------------------
        ; Sort first pass stright to radix buffer 1

        ; Offset of face table and last face
        mov     esi,[object_data]
        mov     ebx,[esi+sobjdata.ftab]
        mov     ecx,[esi+sobjdata.fend]
        aligncode
@@sortfacesl0:


        mov     esi,[ebx+ecx+sface.a]   ; Number of vertex a
        ; Take distance from rotatetab
        cmul    esi,<size sface/4>
        mov     eax,[fs:_rotatetab+esi*4+svect.z]
        mov     esi,[ebx+ecx+sface.b]
        cmul    esi,<size sface/4>      ;|--***************
        add     eax,[fs:_rotatetab+esi*4+4+svect.z]
        mov     esi,[ebx+ecx+sface.c]
        cmul    esi,<size sface/4>
        add     eax,[fs:_rotatetab+esi*4+8+svect.z]
        and     eax,RADIX_SIZE-1        ; Buffer number



        mov     edx,[radixoff1+eax*4]
        mov     edi,[radixptr1+eax*4]
        mov     [fs:0+edx+edi*4],ecx      ; Offset of face
        inc     [radixptr1+eax*4]
        sub     ecx,size sface
        jge     @@sortfacesl0


        ;---------------
        ; Rest of passes

        mov     ecx,RADIX_BITS
        aligncode
@@sortfacesl5:

        ;--------------------------
        ; From buffer 1 to buffer 2

        ; Clear pointers for buffer 2
        COUNT = 0
        rept    RADIX_SIZE
        mov     [radixptr2+COUNT*4],0
        COUNT = COUNT+1
        endm

        ; RADIX_SIZE number of buffers
        mov     ebp,RADIX_SIZE-1
        aligncode
@@sortfacesl2:

        ; Every face in buffer
        cmp     [ds:radixptr1+ebp*4],0
        jz      @@sortfacesf0
        mov     eax,[ds:radixptr1+ebp*4]
        dec     eax
        mov     [radixcount],eax
        aligncode
@@sortfacesl1:
        ; Take offset of face stored in current location of source
        ; radix buffer
        mov     ebx,[ds:radixoff1+ebp*4]
        mov     esi,[radixcount]
        mov     esi,[fs:0+ebx+esi*4]

        ; Offset to vertex from the face table
        mov     edi,[object_data]
        mov     ebx,[edi+sobjdata.ftab]
        mov     edi,[ebx+esi+sface.a]
        cmul    edi,<size sface/4>

        ; Distance component of vector
        mov     eax,[fs:_rotatetab+edi*4+svect.z]
        mov     edx,RADIX_SIZE-1
        shl     edx,cl
        and     eax,edx
        shr     eax,cl

        ; Store offset of face into current location of destination
        ; radix buffer
        mov     edx,[radixoff2+eax*4]
        mov     edi,[radixptr2+eax*4]
        mov     [fs:0+edx+edi*4],esi

        inc     [radixptr2+eax*4]
        dec     [radixcount]
        jge     @@sortfacesl1
@@sortfacesf0:

        dec     ebp
        jge     @@sortfacesl2

        add     ecx,RADIX_BITS
        cmp     ecx,32
        jge     @@sortfacesf1


        ;--------------------------
        ; ...and back to buffer 1

        ; Clear pointers for buffer 1
        COUNT = 0
        rept    RADIX_SIZE
        mov     [radixptr1+COUNT*4],0
        COUNT = COUNT+1
        endm

        mov     ebp,RADIX_SIZE-1        ; RADIX_SIZE number of buffers
@@sortfacesl4:

        ; Every face in buffer
        cmp     [ds:radixptr2+ebp*4],0
        jz      @@sortfacesf2
        mov     eax,[ds:radixptr2+ebp*4]
        dec     eax
        mov     [radixcount],eax
        aligncode
@@sortfacesl3:
        ; Take offset of face stored in current location of source
        ; radix buffer
        mov     ebx,[ds:radixoff2+ebp*4]
        mov     esi,[radixcount]
        mov     esi,[fs:0+ebx+esi*4]

        ; Offset to vertex from the face table
        mov     edi,[object_data]
        mov     ebx,[edi+sobjdata.ftab]
        mov     edi,[ebx+esi+sface.a]
        cmul    edi,3

        ; Distance component of vector
        mov     eax,[fs:_rotatetab+edi*4+svect.z]
        mov     edx,RADIX_SIZE-1
        shl     edx,cl
        and     eax,edx
        shr     eax,cl

        ; Store offset of face into current location of destination
        ; radix buffer
        mov     edx,[radixoff1+eax*4]
        mov     edi,[radixptr1+eax*4]
        mov     [fs:0+edx+edi*4],esi
        inc     [radixptr1+eax*4]
        dec     [radixcount]
        jge     @@sortfacesl3
@@sortfacesf2:

        dec     ebp
        jge     @@sortfacesl4

        add     ecx,RADIX_BITS
;        cmp     ecx,32                  ; Not needed if 32 bits
;        jl      @@sortfacesl5           ;
        jmp     @@sortfacesl5


        ;--------------------------------
        ; Copy from buffer 1 to faceorder

;        popad
;        ret


        ;--------------------------------
        ; Copy from buffer 2 to faceorder
        aligncode
@@sortfacesf1:
        mov     edi,_faceorder
        mov     ebp,RADIX_SIZE-1         ; RADIX_SIZE number of buffers
        aligncode
@@sortfacesl7:
        ; Every face in buffer
        cmp     [ds:radixptr2+ebp*4],0
        jz      @@sortfacesf3
        mov     ecx,[ds:radixptr2+ebp*4]
        dec     ecx
        aligncode
@@sortfacesl6:
        mov     ebx,[ds:radixoff2+ebp*4]
        mov     eax,[fs:ebx+ecx*4]
        mov     [fs:edi],eax
        add     edi,4
        dec     ecx
        jge     @@sortfacesl6
@@sortfacesf3:
        dec     ebp
        jge     @@sortfacesl7
        popad
        ret
        endp
; [fold]  ]

        ends
; [fold]  ]

;############################################################################
; Data
;############################################################################
; [fold]  [

        decdata

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
facetaboff      dd      ?
vertextaboff    dd      ?
maptaboff       dd      ?
lastfaceoff     dd      ?
lastvertexoff   dd      ?
lastfacenum     dd      ?
lastvertexnum   dd      ?
normaltab       dd      ?

object_data     dd      ?

rock_data       sobjdata <offset rock_facetab,offset rock_vertextab,\
                          offset rock_maptab,ROCK_FACES-1,ROCK_VERTICES-1,\
                          (ROCK_FACES-1)*size sface,\
                          (ROCK_VERTICES-1)*size svect,?,?,_normaltab1>
duck_data       sobjdata <offset duck_facetab,offset duck_vertextab,?,\
                          DUCK_FACES-1,DUCK_VERTICES-1,\
                          (DUCK_FACES-1)*size sface,\
                          (DUCK_VERTICES-1)*size svect,?,?,_normaltab2>
obj11_data      sobjdata <offset obj11_facetab,offset obj11_vertextab,\
                          offset obj11_maptab,OBJ11_FACES-1,\
                          OBJ11_VERTICES-1,(OBJ11_FACES-1)*size sface,\
                          (OBJ11_VERTICES-1)*size svect,?,?,_normaltab1>
obj12_data      sobjdata <offset obj12_facetab,offset obj12_vertextab,\
                          offset obj12_maptab,OBJ12_FACES-1,\
                          OBJ12_VERTICES-1,(OBJ12_FACES-1)*size sface,\
                          (OBJ12_VERTICES-1)*size svect,?,?,_normaltab1>
st_data         sobjdata <offset st_facetab,offset st_vertextab,?,\
                          ST_FACES-1,ST_VERTICES-1,\
                          (ST_FACES-1)*size sface,\
                          (ST_VERTICES-1)*size svect,?,?,_normaltab1>
ti_data         sobjdata <offset ti_facetab,offset ti_vertextab,?,\
                          TI_FACES-1,TI_VERTICES-1,\
                          (TI_FACES-1)*size sface,\
                          (TI_VERTICES-1)*size svect,?,?,_normaltab2>
ac_data         sobjdata <offset ac_facetab,offset ac_vertextab,?,\
                          AC_FACES-1,AC_VERTICES-1,\
                          (AC_FACES-1)*size sface,\
                          (AC_VERTICES-1)*size svect,?,?,_normaltab3>

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
objoffset       svect   ?
objangle        sangle  ?

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                label   radixoff1 dword
                COUNT = 0
                rept    RADIX_SIZE
                dd      _radixbuf+MAX_FACES*COUNT*4
                COUNT = COUNT + 1
                endm
                label   radixoff2 dword
                COUNT = 0
                rept    RADIX_SIZE
                dd      _radixbuf+MAX_FACES*RADIX_SIZE*4+MAX_FACES*COUNT*4
                COUNT = COUNT + 1
                endm
radixptr1       dd      RADIX_SIZE dup (0)
radixptr2       dd      RADIX_SIZE dup (0)
radixcount      dd      ?

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
vertex          svect   ?
polyac          svect   ?
polyab          svect   ?
normal          svect   ?
npow2           dd      ?
nlength         dd      ?

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
normalsum       svect   ?
numconnected    dd      ?

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
face            sface   ?

v2ratio         dd      ?
v2deltax        dd      ?

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
pdata           spdata  ?

delta           sedge   ?
count           sedge   ?
grad            sedge   ?

edge_egrad      smap    ?
edge_tgrad      smap    ?

        ends
; [fold]  ]

        end

; [fold]  6
