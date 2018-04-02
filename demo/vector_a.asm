	%title	"Copyright (c) Seppo Enarvi  1996,1997"
	%subttl "Vector functions"

	include "declare.mac"


	include "system.mac"
	include "math.mac"
	include "vector.mac"

	include "video.inc"
	include "vector.inc"
	include "math.inc"
	include "system.inc"


ORIGFRACBITS = FRACBITS


;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; Code
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; [fold]  [

	_code

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
; Rotate object

	proc	rotate_
	pushad

;	 mov	 esi,eax
;	 mov	 edi,[(sobjdata esi).vend]
;	 aligncode
;@@loop:
;	 mov	 ebx,[(sobjdata esi).vtab]
;	 vmov	 <(svect ebx+edi)>,vertex
;	 sar	 [vertex.x],OBJ_FRACBITS
;	 sar	 [vertex.y],OBJ_FRACBITS
;	 sar	 [vertex.z],OBJ_FRACBITS
;	 vrot	 vertex, <(sobjdata esi).angle>, rotatetab+edi
;	 vadd	 rotatetab+edi, <(sobjdata esi).off>, rotatetab+edi
;	 sub	 edi,size svect
;	 jge	 @@loop

	popad
	ret
	endp
; [fold]  ]

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
; Sort faces by distance

	alignstart
	proc	sortfaces_
	pushad
	mov	[current_object],eax



	; Clears pointers to ends of temporary buffers
	macro	clrptrs buffer
	COUNT = 0
	rept	RADIX_SIZE
	mov	[buffer + COUNT*dword],0
	COUNT = COUNT+1
	endm
	endm



; [fold]  [
	macro	sortpass src_ptr,src_off,dest_ptr,dest_off,exit_label
	local	@@buffer_loop,@@entry_loop,@@entry_done

	clrptrs dest_ptr

	; RADIX_SIZE number of buffers

	mov	ebp,RADIX_SIZE-1
	aligncode
@@buffer_loop:



	; Every face in buffer

	mov	eax,[ds:src_ptr + ebp*dword]
	dec	eax
	jl	@@entry_done
	mov	[radixcount],eax

	aligncode
@@entry_loop:

	; Take offset of face stored in current location of source
	; radix buffer

	mov	ebx,[ds:src_off + ebp*dword]
	mov	esi,[radixcount]
	mov	esi,[ebx + esi*dword]



	; Offset to vertex from the face table

	mov	edi,[current_object]
	mov	ebx,[(sobjdata edi).ftab]

	; Calculate average of z-coordinates

	mov	edi,[(sface ebx+esi).a]	    ; Number of vertex a
	cmul	edi,<size sface/dword>	    ; Distance from rotatetab
	mov	eax,[rotatetab + edi*dword.z]

	mov	edi,[(sface ebx+esi).b]
	cmul	edi,<size sface/dword>
	add	eax,[rotatetab + edi*dword.z]

	mov	edi,[(sface ebx+esi).c]
	cmul	edi,<size sface/dword>
	add	eax,[rotatetab + edi*dword.z]

	; Get correct buffer number

	mov	edx,RADIX_SIZE-1
	shl	edx,cl
	and	eax,edx
	shr	eax,cl

	; Store offset of face into current location of destination
	; radix buffer

	mov	edx,[dest_off + eax*dword]
	mov	edi,[dest_ptr + eax*dword]
	mov	[edx + edi*dword],esi
	inc	[dest_ptr + eax*dword]



	dec	[radixcount]
	jge	@@entry_loop
@@entry_done:



	dec	ebp
	jge	@@buffer_loop
	endm
; [fold]  ]



	;------------------------------------
	; Sort first pass stright to buffer 1

	clrptrs radixptr1

	; Offset of face table and last face

	mov	esi,[current_object]
	mov	ebx,[(sobjdata esi).ftab]
	mov	ecx,[(sobjdata esi).fend]

	aligncode
@@loop1:
	; Calculate sum of z-coordinates

	mov	esi,[(sface ebx+ecx).a]	    ; Number of vertex a
	cmul	esi,<size sface/dword>	    ; Distance from rotatetab
	mov	eax,[rotatetab + esi*dword.z]

	mov	esi,[(sface ebx+ecx).b]
	cmul	esi,<size sface/dword>
	add	eax,[rotatetab + esi*dword.z]

	mov	esi,[(sface ebx+ecx).c]
	cmul	esi,<size sface/dword>
	add	eax,[rotatetab + esi*dword.z]

	; Store to the correct loaction

	and	eax,RADIX_SIZE-1	    ; Buffer number
	mov	edx,[radixoff1 + eax*dword]
	mov	edi,[radixptr1 + eax*dword]
	mov	[edx + edi*dword],ecx	    ; Offset of face
	inc	[radixptr1 + eax*dword]
	sub	ecx,size sface
	jge	@@loop1



	;---------------
	; Rest of passes

	mov	ecx,RADIX_BITS
	aligncode
@@loop2:

	sortpass radixptr1,radixoff1,radixptr2,radixoff2

	add	ecx,RADIX_BITS
	cmp	ecx,32
	jge	@@main_done

	sortpass radixptr2,radixoff2,radixptr1,radixoff1

	add	ecx,RADIX_BITS
;	 cmp	 ecx,32			 ; Not needed if 32 bits
;	 jl	 @@loop2		 ;
	jmp	@@loop2



	;--------------------------------
	; Copy from buffer 1 to faceorder

;	 popad
;	 ret

	; The last pass could be sorted straight to the faceorder!



	;--------------------------------
	; Copy from buffer 2 to faceorder

	align	4
@@main_done:
	mov	edi,offset faceorder
	mov	ebp,(RADIX_SIZE-1)*dword    ; RADIX_SIZE number of buffers

	aligncode
@@loop3:
	; Every face in buffer
	mov	ecx,[ds:radixptr2 + ebp]
	dec	ecx			    ; Buffer is empty?
	jl	@@f1

	aligncode
@@loop4:
	mov	ebx,[ds:radixoff2 + ebp]
	mov	eax,[ebx + ecx*dword]
	mov	[edi],eax
	add	edi,dword
	dec	ecx
	jg	@@loop4

@@f1:
	sub	ebp,dword
	jge	@@loop3
	popad
	ret
	endp
; [fold]  ]

	ends
; [fold]  ]

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; Data
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; [fold]  [

		_data

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		label	radixoff1 dword
		COUNT = 0

		rept	RADIX_SIZE
		dd	offset radixbuf + MAX_FACES*COUNT*4
		COUNT = COUNT + 1
		endm

		label	radixoff2 dword
		COUNT = 0

		rept	RADIX_SIZE
		dd	offset radixbuf + MAX_FACES*RADIX_SIZE*4\
					+ MAX_FACES*COUNT*4
		COUNT = COUNT + 1
		endm

radixptr1	dd	RADIX_SIZE dup (0)
radixptr2	dd	RADIX_SIZE dup (0)

		ends
; [fold]  ]

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; Uninitialized data
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; [fold]  (

		_udata

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
facetaboff	dd	?
vertextaboff	dd	?
smoothingtaboff dd	?
normaltaboff	dd	?
lastfaceoff	dd	?
lastfacenum	dd	?

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		label	pdata spdata
		spdata	?

		label	edge_delta sedge
		sedge	?
		label	edge_count sedge
		sedge	?
		label	edge_grad sedge
		sedge	?

		label	scan_delta sedge
		sedge	?
		label	scan_count sedge
		sedge	?
		label	scan_grad sedge
		sedge	?

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		label	vertex svect
		svect	?
		label	polyac svect
		svect	?
		label	polyab svect
		svect	?
		label	normal svect
		svect	?
npow2		dd	?
nlength		dd	?

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		label	normalsum svect
		svect	?
numconnected	dd	?

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		label	face sface
		sface	?

v2ratio		dd	?
v2deltax	dd	?

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
radixcount	dd	?

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
radixbuf	dd	2*(RADIX_SIZE*MAX_FACES) dup (?)
faceorder	dd	MAX_FACES dup (?)

		label	rotatetab svect
		svect	MAX_VERTICES dup (?)

		label	ledgebuf sedge
		sedge	GR_Y dup (?)
		label	redgebuf sedge
		sedge	GR_Y dup (?)

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
x_mul		dd	?
y_mul		dd	?

		ends
; [fold]  )

	end

; [fold]  6
