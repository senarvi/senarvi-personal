;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
	struc	svect
x	dd	?
y	dd	?
z	dd	?
	ends

	struc	spoly
	label	a svect
	svect	?
	label	b svect
	svect	?
	label	c svect
	svect	?
	ends

	struc	sface
a	dd	?
b	dd	?
c	dd	?
	ends

	struc	sangle
a	dd	?
b	dd	?
r	dd	?
	ends

	struc	srotation
x	dd	?
y	dd	?
z	dd	?
a	dd	?
	ends

	struc	svdata
	label	spa svect
	svect	?				; Space coordinates
	label	scr spixel
	spixel	?				; Screen pixel
	label	env smap
	smap	?				; Environment map coordinates
	label	tex smap
	smap	?				; Texture map coordinates
	ends

	struc	spdata
	label	a svdata
	svdata	?
	label	b svdata
	svdata	?
	label	c svdata
	svdata	?
	ends

	struc	sedge
x	dd	?				; Screen X-coordinate
z	dd	?				; Space Z-coordinate
	label	env smap
	smap	?				; Environment map coordinates
	label	tex smap
	smap	?				; Texture map coordinates
	ends
	if	size sedge mod 4 ne 0
	err	"Structure sedge must be dividable by 4"
	endif

	struc	sobjdata
ftab	dd	?				; Face table
vtab	dd	?				; Vertex table
mtab	dd	?				; Map table
stab	dd	?				; Smoothing table

ntab	dd	?				; Normal table

faces	dd	?				; Number of faces - 1
verts	dd	?				; Number of vertices - 1

fend	dd	?				; Offset to the last face
vend	dd	?				; Offset to the last vertex

	label	off svect			; Position
	svect	?
	label	rot srotation			; Rotation
	srotation ?

matrix	dd	3 * 3 dup (?)			; Rotation matrix (row,col)
						; (1,1) (1,2) (1,3) (2,1) ...
	ends

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
RADIX_BITS	=	4
RADIX_SIZE	=	1 shl RADIX_BITS


MAX_VERTICES	=	1024
MAX_FACES	=	2048


NORMAL_DIVISOR	=	4


U_TILE		=	1
V_TILE		=	1


TEXMAP_GETAREA	=	0
TEXMAP_ZBUFFER	=	0

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
	global	c rotate_:proc,c sortfaces_:proc

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
	global	radixbuf:dword:2*(RADIX_SIZE*MAX_FACES)
	global	faceorder:dword:MAX_FACES
	global	rotatetab:svect:MAX_VERTICES

	global	ledgebuf:sedge:GR_Y,redgebuf:sedge:GR_Y

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
	global	current_object:dword
	global	objoffset:svect,objangle:sangle

	global	face:sface
	global	v2ratio:dword,v2deltax:dword

	global	pdata:spdata
	global	edge_delta:sedge,edge_count:sedge,edge_grad:sedge
	global	scan_delta:sedge,scan_count:sedge,scan_grad:sedge

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
	global	x_mul:dword,y_mul:dword

