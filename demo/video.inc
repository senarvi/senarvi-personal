;����������������������������������������������������������������������������
	struc	spixel
x	dd	?
y	dd	?
	ends

#	struc	squad
#	label	a spixel
#	spixel	?
#	label	b spixel
#	spixel	?
#	ends	squad

#	struc	scolor
#r	dd	?
#g	dd	?
#b	dd	?
#	ends

#	struc	svideomem
#r	db	?
#g	db	?
#b	db	?
#	ends

#	struc	smaterial
#	label	ambient scolor
#	scolor	?
#	label	diffuse scolor
#	scolor	?
#	label	specular scolor
#	scolor	?
#shine	dd	?
#	ends

	struc	smap
u	dd	?
v	dd	?
	ends

;����������������������������������������������������������������������������
TXT_MODE	=	03h
TXT_X		=	80
TXT_Y		=	25
CR		=	0Dh
LF		=	0Ah

GR_MODE		=	13h
GR_X		=	320
GR_Y		=	200
GR_SIZE		=	GR_X*GR_Y
GR_BITS		=	8
GR_COLORS	=	1 shl GR_BITS
GR_BUFFERS	=	2

;����������������������������������������������������������������������������
	global	screenbufoff:dword:GR_BUFFERS
	global	prevbufoff:dword,destbufoff:dword,destbufnum:dword

	global	text_cursor:spixel
	global	bufferarea:squad

	global	c setphongpal_:proc,c setlinearpal_:proc

	global	c writestr:proc
	global	c cleartext:proc
	global	c clearbuffers:proc,c clearvideo:proc
	global	c clearbuffer_:proc,c copybuffer_:proc
	global	c cleararea_:proc,c copyarea_:proc
	global	c motionblur_:proc

	global	c drawbrsline:proc,c drawline:proc

