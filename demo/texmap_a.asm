	.intel_syntax
	
	%title	"Copyright (c) Seppo Enarvi  1996,1997"
	%subttl "Texture mapped phong"

	include "declare.mac"


	include "system.mac"
	include "vector.mac"
	include "math.mac"

	include "system.inc"
	include "video.inc"
	include "vector.inc"
	include "math.inc"
	include "data.inc"


	public	texturemap_
	public	zbuffer
FRACBITS16 = 16
TEXMAP_ZBUFFER = 1


;�����������������������������������������������������������������������������
; Code
;�����������������������������������������������������������������������������
; [fold]  (

	_code

;����������������������������������������������������������������������������
; [fold]  (
; Draw every scanline
;
; Regs modified:
;   all

	alignstart
fillpoly:


	if	TEXMAP_ZBUFFER
	;-----------------------
	; Polyfill with Z-buffer
	;-----------------------


	sar	[polyarea.b.y],FRACBITS16
	sar	[polyarea.a.y],FRACBITS16
	mov	edx,[polyarea.b.y]
	mov	eax,edx
	sub	eax,[polyarea.a.y]
	jz	return
	mov	[edgecount],eax
	mov	[scanline],edx


	aligncode
@@fillpolyl0:
	mov	esi,[scanline]
	cmul	esi,<size sedge/4>

	; counteihin voisi lis�t� puolikkaan
xcount		= dword esi*4+ledgebuf.x
xend		= dword esi*4+redgebuf.x
zcount		= dword esi*4+ledgebuf.z
ecount_u	= dword esi*4+ledgebuf.env.u
ecount_v_lsb	= byte	esi*4+LSB+ledgebuf.env.v
ecount_v_msb	= byte	esi*4+MSB+ledgebuf.env.v
tcount_u	= dword esi*4+ledgebuf.tex.u
tcount_v	= dword esi*4+ledgebuf.tex.v

	mov	ecx,[xend]
	sub	ecx,[xcount]
	sar	ecx,FRACBITS
	mov	[scan_delta.x],ecx

	mov	edi,[scanline]
	cmul	edi,GR_X
	mov	eax,[xcount]
	sar	eax,FRACBITS
	add	edi,eax
	mov	[zbufferndx],edi
	add	[zbufferndx],offset zbuffer
	add	edi,[destbufoff]


	mov	eax,[zcount]
	mov	[scan_count.z],eax
	mov	eax,[tcount_u]
	mov	[scan_count.tex.u],eax
	mov	eax,[tcount_v]
	mov	[scan_count.tex.v],eax

	mov	ecx,[ecount_u]
	mov	ah,[ecount_v_lsb]		; Fractional part of v
	mov	edx,[scan_grad.env.v]

	xor	ebx,ebx				; Environment map offset
	mov	bh,[ecount_v_msb]		; Row (int part of v)
	mov	bl,ch				; Column (int part of u)


	aligncode
@@hlinel0:
	mov	al,[envmap+ebx]
	add	ecx,[scan_grad.env.u]
	mov	bl,ch				; Column (int part of u)
	add	ah,dl				; Update fractional part of v
	adc	bh,dh				; Update row (int part of v)

	mov	esi,[scan_count.tex.u]
	sar	esi,FRACBITS
	mov	ebp,[scan_count.tex.v]
	and	ebp,0ff00h
	push	eax
	movzx	eax,al
	cmul	eax,3
	movzx	esi,[ds:marble_texmap+ebp+esi]
	cmul	esi,6
	add	eax,esi
	cdiv	eax,4

	mov	esi,[scan_grad.tex.u]
	add	[scan_count.tex.u],esi
	mov	esi,[scan_grad.tex.v]
	add	[scan_count.tex.v],esi

	shr	al,1
	add	al,128
	mov	esi,[zbufferndx]
	mov	ebp,[scan_count.z]
	cmp	ebp,[esi]
;	 ja	 @@hlinef0;jg?
	mov	[edi],al
	mov	[esi],ebp
@@hlinef0:


;	 mov	 eax,[esi]
;	 sar	 eax,17
;	 mov	 [edi],al



	pop	eax
	inc	edi

	inc	[zbufferndx]
	mov	esi,[scan_grad.z]
	add	[scan_count.z],esi

	dec	[scan_delta.x]
	jnl	@@hlinel0


	dec	[scanline]
;	 sub	 [edgecount],ONE
	dec	[edgecount]
	jg	@@fillpolyl0


	else
	;--------------------------
	; Polyfill without Z-buffer
	;--------------------------


	sar	[polyarea.b.y],FRACBITS16
	sar	[polyarea.a.y],FRACBITS16
	mov	edx,[polyarea.b.y]
	mov	eax,edx
	sub	eax,[polyarea.a.y]
	jz	return
	mov	[edgecount],eax
	mov	[scanline],edx


	aligncode
@@fillpolyl0:
	mov	esi,[scanline]
	cmul	esi,<size sedge/4>

xcount		= dword esi*4+ledgebuf.x
xend		= dword esi*4+redgebuf.x
ecount_u	= dword esi*4+ledgebuf.env.u
ecount_v	= dword esi*4+ledgebuf.env.v
ecount_v_lsb	= byte	esi*4+LSB+ledgebuf.env.v
ecount_v_msb	= byte	esi*4+MSB+ledgebuf.env.v
tcount_u	= dword esi*4+ledgebuf.tex.u
tcount_v	= dword esi*4+ledgebuf.tex.v


	mov	ecx,[xend]
	sub	ecx,[xcount]
	sar	ecx,FRACBITS
	mov	[scan_delta.x],ecx

	mov	edi,[scanline]
	cmul	edi,GR_X
	mov	eax,[xcount]
	sar	eax,FRACBITS
	add	edi,eax
	add	edi,[destbufoff]


	; Change to 8 bit fractional part
;	 mov	 eax,ecount_v
;	 cmul	 eax,256
;	 sar	 eax,FRACBITS
;	 mov	 ecount_v,eax
;
;	 mov	 eax,tcount_v
;	 cmul	 eax,256
;	 sar	 eax,FRACBITS
;	 mov	 tcount_v,eax
;
;	 mov	 eax,[grad.env.v]
;	 cmul	 eax,256
;	 sar	 eax,FRACBITS
;	 mov	 [grad.env.v],eax
;
;	 mov	 eax,[grad.tex.v]
;	 cmul	 eax,256
;	 sar	 eax,FRACBITS
;	 mov	 [grad.tex.v],eax

	mov	eax,[tcount_u]
	mov	[scan_count.tex.u],eax
	mov	eax,[tcount_v]
	mov	[scan_count.tex.v],eax

	mov	ecx,[ecount_u]
	mov	ah,[ecount_v_lsb]		; Fractional part of v
;	mov	edx,egrad_v
;	 db	 0bah
;egrad_v dd	 ?
	mov	edx,[scan_grad.env.v]

	xor	ebx,ebx				; Environment map offset
	mov	bh,[ecount_v_msb]		; Row (int part of v)
	mov	bl,ch				; Column (int part of u)


	aligncode
@@hlinel0:

	mov	al,[envmap+ebx]
;	add	ecx,egrad_u
;	 db	 81h,0c1h
;egrad_u dd	 ?
	add	ecx,[scan_grad.env.u]
	mov	bl,ch				; Column (int part of u)
	add	ah,dl				; Update fractional part of v
	adc	bh,dh				; Update row (int part of v)

	mov	esi,[scan_count.tex.u]
	sar	esi,FRACBITS
;	 and	 esi,0ffh
	mov	ebp,[scan_count.tex.v]
	and	ebp,0ff00h
	push	eax
	movzx	eax,al
	cmul	eax,3
	movzx	esi,[ds:marble_texmap+ebp+esi]
	cmul	esi,6
	add	eax,esi
	cdiv	eax,4

;	 add_imm32 count.tex.u,tgrad_u
;	 add_imm32 count.tex.v,tgrad_v
	mov	esi,[scan_grad.tex.u]
	add	[scan_count.tex.u],esi
	mov	esi,[scan_grad.tex.v]
	add	[scan_count.tex.v],esi

	shr	al,1;*
	add	al,128;*
	mov	[edi],al
	pop	eax
	inc	edi
	dec	[scan_delta.x]
	jnl	@@hlinel0


	dec	[scanline]
	sub	[edgecount],ONE
	jg	@@fillpolyl0


	endif
	ret
; [fold]  )

;����������������������������������������������������������������������������
; [fold]  [
; Interpolate values for edge buffers

	; counteihin voisi lis�t� puolikkaan
	macro	getdelta srcitem,destitem
	mov	eax,[(svdata ebx).srcitem]
	mov	edx,[(svdata esi).srcitem]
	mov	[edge_count.destitem],edx
	sub	eax,edx
	mov	[edge_delta.destitem],eax
	endm

	macro	addgrad item
	mov	eax,[edge_count.item]
	mov	[edi+ecx*4+sedge.item],eax
	add	eax,[edge_grad.item]
	mov	[edge_count.item],eax
	endm


	; Left edge ---------------

	aligncode
getleftedge:
	mov	edi,offset ledgebuf

	getdelta scr.x,x
	if	TEXMAP_ZBUFFER
	getdelta spa.z,z
	endif
	getdelta env.u,env.u
	getdelta env.v,env.v
	getdelta tex.u,tex.u
	getdelta tex.v,tex.v

	; Calculate gradients
	mov	ecx,[(svdata ebx).scr.y]
	sub	ecx,[(svdata esi).scr.y]
	jz	return
	fixdiv	[edge_delta.x],ecx
	mov	[edge_grad.x],eax
	if	TEXMAP_ZBUFFER
	fixdiv	[edge_delta.z],ecx
	mov	[edge_grad.z],eax
	endif
	fixdiv	[edge_delta.env.u],ecx
	mov	[edge_grad.env.u],eax
	fixdiv	[edge_delta.env.v],ecx
	mov	[edge_grad.env.v],eax
	fixdiv	[edge_delta.tex.u],ecx
	mov	[edge_grad.tex.u],eax
	fixdiv	[edge_delta.tex.v],ecx
	mov	[edge_grad.tex.v],eax


	mov	ecx,[(svdata esi).scr.y]
	sar	ecx,FRACBITS
	cmul	ecx,<size sedge/4>
	mov	edx,[(svdata ebx).scr.y]
	sar	edx,FRACBITS
	cmul	edx,<size sedge/4>

	aligncode
@@loop:
	addgrad x
	if	TEXMAP_ZBUFFER
	addgrad z
	endif
	addgrad env.u
	addgrad env.v
	addgrad tex.u
	addgrad tex.v
	add	ecx,size sedge/4
	cmp	ecx,edx
	jbe	@@loop
return:
	ret


	; Right edge --------------

	aligncode
getrightedge:
	mov	edi,offset redgebuf

	getdelta scr.x,x

	mov	ecx,[(svdata ebx).scr.y]
	sub	ecx,[(svdata esi).scr.y]
	jz	return
	fixdiv	[edge_delta.x],ecx
	mov	[edge_grad.x],eax


	mov	ecx,[(svdata esi).scr.y]
	sar	ecx,FRACBITS
	cmul	ecx,<size sedge/4>
	mov	edx,[(svdata ebx).scr.y]
	sar	edx,FRACBITS
	cmul	edx,<size sedge/4>

	aligncode
@@loop:
	addgrad x
	add	ecx,size sedge/4
	cmp	ecx,edx
	jbe	@@loop
	ret
; [fold]  ]

;����������������������������������������������������������������������������
; [fold]  (
; Draw polygon

drawpoly:
	mov	ebx,offset pdata.a
	mov	esi,offset pdata.b
	mov	edi,offset pdata.c

	;------------------------
	; Sort vertices verticaly

	mov	eax,[(svdata ebx).scr.y]
	cmp	eax,[(svdata esi).scr.y]
	jnge	@@sortverticesf0
	xchg	ebx,esi

	mov	eax,[(svdata ebx).scr.y]
@@sortverticesf0:				; a < b
	cmp	eax,[(svdata edi).scr.y]
	jnge	@@sortverticesf1
	xchg	ebx,edi

@@sortverticesf1:				; a < c
	mov	eax,[(svdata esi).scr.y]
	cmp	eax,[(svdata edi).scr.y]
	jnge	@@sortverticesf2
	xchg	esi,edi

	mov	eax,[(svdata esi).scr.y]
@@sortverticesf2:				; b < c
	mov	ebp,[(svdata ebx).scr.y]
	mov	ecx,[(svdata edi).scr.y]

	; v1 = ebx  v2 = esi  v3 = edi
	; y1 = ebp  y2 = eax  y3 = ecx

	mov	[face.a],ebx
	mov	[face.b],esi
	mov	[face.c],edi
	mov	[polyarea.a.y],ebp
	mov	[polyarea.b.y],ecx







	;---------------------------------------------
	; Calculate the length of the longset scanline

	sub	ecx,ebp
	jz	@@return


	cmp	ecx,1 shl FRACBITS16
	jg	@@ok
	cmp	ecx,-1 shl FRACBITS16
	jl	@@ok
	ret
@@ok:
	; T�ss� jossain tulee overflowta ********************************


	sub	eax,ebp				; (y2 - y1)
	mov	edx,eax
	sal	eax,FRACBITS16
	sar	edx,32-FRACBITS16
	idiv	ecx				; / (y3 - y1)
;	 sar	 eax,8
	mov	[v2ratio],eax
	sar	[v2ratio],8;*

	mov	ecx,[(svdata edi).scr.x]
	sub	ecx,[(svdata ebx).scr.x]
	imul	ecx				; * (x3 - x1)
	shrd	eax,edx,FRACBITS16;*

	add	eax,[(svdata ebx).scr.x]
	sub	eax,[(svdata esi).scr.x]	; + (x1 - x2)
	jnz	@@drawpolyf1
	ret
@@drawpolyf1:
	sar	eax,8
	mov	[v2deltax],eax





	sar	[pdata.a.scr.x],8
	sar	[pdata.a.scr.y],8
	sar	[pdata.b.scr.x],8
	sar	[pdata.b.scr.y],8
	sar	[pdata.c.scr.x],8
	sar	[pdata.c.scr.y],8

	and	[pdata.a.scr.y],0ffffff00h
	and	[pdata.b.scr.y],0ffffff00h
	and	[pdata.c.scr.y],0ffffff00h





	macro	getscangrad bufferoff,dest
	mov	eax,[v2ratio]			; (y2 - y1) / (y3 - y1)

	mov	ecx,[edi+bufferoff]
	sub	ecx,[ebx+bufferoff]
	imul	ecx				; * (i3 - i1)
	shrd	eax,edx,FRACBITS

	add	eax,[ebx+bufferoff]
	sub	eax,[esi+bufferoff]		; + (i1 - i2)

	mov	edx,eax
	sal	eax,FRACBITS
	sar	edx,32-FRACBITS
	idiv	[v2deltax]		; deltai / deltax
	mov	dest,eax
	endm

;	 getscangrad svdata.env.u,[egrad_u]
;	 getscangrad svdata.env.v,[egrad_v]
;	 getscangrad svdata.tex.u,[tgrad_u]
;	 getscangrad svdata.tex.v,[tgrad_v]
	if	TEXMAP_ZBUFFER
	getscangrad svdata.spa.z,[scan_grad.z]
	endif
	getscangrad svdata.env.u,[scan_grad.env.u] ; Environment map
	getscangrad svdata.env.v,[scan_grad.env.v] ; scanline gradients
	getscangrad svdata.tex.u,[scan_grad.tex.u] ; Texture map
	getscangrad svdata.tex.v,[scan_grad.tex.v] ; scanline gradient


	cmp	[v2deltax],0
	jl	@@drawpolyf3

	mov	esi,[face.a]
	mov	ebx,[face.c]
	call	getrightedge

	mov	esi,[face.a]
	mov	ebx,[face.b]
	call	getleftedge

	mov	esi,[face.b]
	mov	ebx,[face.c]
	call	getleftedge

	call	fillpoly


	if	TEXMAP_GETAREA
	jmp	@@drawpolyf2
	elseif
	ret
	endif


@@drawpolyf3:
	mov	esi,[face.a]
	mov	ebx,[face.c]
	call	getleftedge

	mov	esi,[face.a]
	mov	ebx,[face.b]
	call	getrightedge

	mov	esi,[face.b]
	mov	ebx,[face.c]
	call	getrightedge

	call	fillpoly


	if	TEXMAP_GETAREA

	;---------------------------
	; Get area used by the face

	macro	checkxlimit source
	local	@@xlimitf0,@@xlimitf1
	mov	eax,source
	cmp	eax,[bufferarea.a.x]
	jge	@@xlimitf0
	mov	[bufferarea.a.x],eax
@@xlimitf0:
	cmp	eax,[bufferarea.b.x]
	jle	@@xlimitf1
	mov	[bufferarea.b.x],eax
@@xlimitf1:
	endm

@@drawpolyf2:
	checkxlimit [pdata.a.scr.x]
	checkxlimit [pdata.b.scr.x]
	checkxlimit [pdata.c.scr.x]

	; y
	sar	[polyarea.a.y],8
	sar	[polyarea.b.y],8
	mov	eax,[polyarea.a.y]
	cmp	eax,[bufferarea.a.y]
	jnl	@@getareaf6
	mov	[bufferarea.a.y],eax
@@getareaf6:
	mov	eax,[polyarea.b.y]
	cmp	eax,[bufferarea.b.y]
	jng	@@getareaf7
	mov	[bufferarea.b.y],eax
@@getareaf7:

	endif

@@return:
	ret
; [fold]  )

;����������������������������������������������������������������������������
; [fold]  [
; Texturemap object

	aligncode
	proc	texturemap_
	pushad

	mov	[current_object],eax
	mov	ebx,[(sobjdata eax).ftab]
	if	TEXMAP_ZBUFFER
	mov	ebp,[(sobjdata eax).fend]
	else
	mov	ebp,[(sobjdata eax).faces]
	endif

	if	TEXMAP_GETAREA
	mov	[bufferarea.a.x],GR_X*ONE
	mov	[bufferarea.a.y],GR_Y*ONE
	mov	[bufferarea.b.x],0
	mov	[bufferarea.b.y],0
	endif


	aligncode
@@loop:
	mov	esi,[current_object]
	if	TEXMAP_ZBUFFER
	mov	edi,ebp
	else
	mov	edi,[ds:faceorder + ebp*dword]	; face * 3 * 4
	endif					; = 3 vertices / face
	mov	ecx,[(sobjdata esi).mtab]


	macro	getvertex source
	mov	esi,[(sface ebx+edi).source]

	mov	eax,[esi*size smap+(smap ecx).u]
	if	U_TILE gt 1
	cmul	eax,U_TILE
	endif
	mov	[pdata.source.tex.u],eax

	mov	eax,[esi*size smap+(smap ecx).v]
	if	V_TILE gt 1
	cmul	eax,V_TILE
	endif
	mov	[pdata.source.tex.v],eax

	cmul	esi,3
	mov	[vertexnum.source],esi

	vproject esi*dword+rotatetab,pdata.source.scr
	if	TEXMAP_ZBUFFER
	mov	eax,[esi*dword+rotatetab.z]
	mov	[pdata.source.spa.z],eax
	endif
	endm

	getvertex a
	getvertex b
	getvertex c


	CONTINUE_RANGE=256*ONE/4

	macro	continuemap source,tile
	local	@@high,@@higha,@@highb,@@highc,@@middle

	; If any of the texture coordinates is in the upper portion of the
	; last texture tile
	cmp	[pdata.a.tex.source],tile*10000h-CONTINUE_RANGE
	jg	@@high
	cmp	[pdata.b.tex.source],tile*10000h-CONTINUE_RANGE
	jg	@@high
	cmp	[pdata.c.tex.source],tile*10000h-CONTINUE_RANGE
	jg	@@high
	jmp	@@middle

	; Make every coordinate, that is in the lower portion, to go into the
	; next texture tile
@@high:
	cmp	[pdata.a.tex.source],CONTINUE_RANGE
	jnl	@@higha
	add	[pdata.a.tex.source],tile*10000h
@@higha:
	cmp	[pdata.b.tex.source],CONTINUE_RANGE
	jnl	@@highb
	add	[pdata.b.tex.source],tile*10000h
@@highb:
	cmp	[pdata.c.tex.source],CONTINUE_RANGE
	jnl	@@highc
	add	[pdata.c.tex.source],tile*10000h
@@highc:
@@middle:
	endm

	continuemap u,U_TILE
	continuemap v,V_TILE


	;---------------------------------
	; Check that polygon must be drawn

	xor	eax,eax
	cmp	[pdata.a.scr.x],eax
	jl	@@noshown
	cmp	[pdata.a.scr.y],eax
	jl	@@noshown
	cmp	[pdata.b.scr.x],eax
	jl	@@noshown
	cmp	[pdata.b.scr.y],eax
	jl	@@noshown
	cmp	[pdata.c.scr.x],eax
	jl	@@noshown
	cmp	[pdata.c.scr.y],eax
	jl	@@noshown

	cmp	[pdata.a.scr.x],(GR_X - 1) * _FR
	jg	@@noshown
	cmp	[pdata.b.scr.x],(GR_X - 1) * _FR
	jg	@@noshown
	cmp	[pdata.c.scr.x],(GR_X - 1) * _FR
	jg	@@noshown
	cmp	[pdata.a.scr.y],(GR_Y - 1) * _FR
	jg	@@noshown
	cmp	[pdata.b.scr.y],(GR_Y - 1) * _FR
	jg	@@noshown
	cmp	[pdata.c.scr.y],(GR_Y - 1) * _FR
	jg	@@noshown


	fshown	pdata
	jng	@@noshown


	pushad
	lea	esi,[edi*2+edi]			; (face * 3 * 4) * 3
	mov	edi,[current_object]		; = 3 vertices * 3 vectors
	add	esi,[(sobjdata edi).ntab]	; / face
	vrotxy	<(svect esi)>,	   <(sobjdata edi).matrix>, pdata.a.env
	vrotxy	<(svect esi+3*4)>, <(sobjdata edi).matrix>, pdata.b.env
	vrotxy	<(svect esi+6*4)>, <(sobjdata edi).matrix>, pdata.c.env


	add	[pdata.a.env.u],ONE
	sal	[pdata.a.env.u],FRACBITS-1
	add	[pdata.a.env.v],ONE
	sal	[pdata.a.env.v],FRACBITS-1
	add	[pdata.b.env.u],ONE
	sal	[pdata.b.env.u],FRACBITS-1
	add	[pdata.b.env.v],ONE
	sal	[pdata.b.env.v],FRACBITS-1
	add	[pdata.c.env.u],ONE
	sal	[pdata.c.env.u],FRACBITS-1
	add	[pdata.c.env.v],ONE
	sal	[pdata.c.env.v],FRACBITS-1

	call	drawpoly
	popad

@@noshown:
	if	TEXMAP_ZBUFFER
	sub	ebp,(3*dword)
	else
	dec	ebp
	endif
	jge	@@loop


	if	TEXMAP_GETAREA
	shr	[bufferarea.a.x],FRACBITS
	shr	[bufferarea.a.y],FRACBITS
	shr	[bufferarea.b.x],FRACBITS
	shr	[bufferarea.b.y],FRACBITS
	endif

	popad
	ret
	endp
; [fold]  ]

	ends
; [fold]  )

;�����������������������������������������������������������������������������
; Uninitialized data
;�����������������������������������������������������������������������������
; [fold]  [

		_udata

;����������������������������������������������������������������������������
		label	vertexnum sface
		sface	?
edgecount	dd	?
scanline	dd	?

;����������������������������������������������������������������������������
leftedge	dd	?

;����������������������������������������������������������������������������
#		label	polyarea squad
#		squad	?

;����������������������������������������������������������������������������
zbuffer		dd	GR_X*GR_Y dup (?)
zbufferndx	dd	?




x1		dd	?
x2		dd	?
x3		dd	?
x4		dd	?

		ends
; [fold]  ]

	end

; [fold]  6
