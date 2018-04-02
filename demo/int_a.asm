	%title	"Copyright (c) Seppo Enarvi  1997"
	%subttl "Interrupt handlers"

	include "declare.mac"


	include "system.mac"

	include "kb.inc"


	model	flat  ; Watcom names functions in a personal way

	global	__GETDS:near
	global	_chain_intr_:near
	global	exit_:near

	global	_prev_int_08:fword,_prev_int_09:fword
	global	pit_handler_:far,kb_handler_:far
	global	_pause_flag:dword,_exit_flag:dword
	global	_timer_chain:dword


;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; Code
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; [fold]  (

	_code

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  (
; Keyboard handler

	proc	kb_handler_ far
	pushad					; preserve machine status
	push	ds es fs gs
	mov	ebp,esp
	cld
	call	__GETDS

	; check if an important key
	in	al,KB_PORT			; read key code
	cmp	al,KB_ESC			; escape?
	je	@@escape
	cmp	al,KB_P				; pause?
	je	@@pause
	cmp	al,KB_RELEASE + KB_P		; pause released?
	je	@@continue

	; chain to the previous keyboard handler
	mov	dx,[word _prev_int_09 + 4]	; segment
	mov	eax,[dword _prev_int_09]	; offset
	call	_chain_intr_			; doesn't return

@@escape:
	; set exit flag
	mov	[_exit_flag],1
	jmp	@@return

	; exit program
	mov	al,20h
	out	20h,al
	xor	eax,eax
	call	exit_

@@pause:
	; set pause flag
	mov	[_pause_flag],1
	jmp	@@return

@@continue:
	; clear pause flag
	mov	[_pause_flag],0

@@return:
	; done
	mov	al,20h
	out	20h,al
	pop	gs fs es ds
	popad
	iretd
	endp
; [fold]  )

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; [fold]  [
; Timer handler

	proc	pit_handler_ far
	pushad					; preserve machine status
	push	ds es fs gs
	mov	ebp,esp
	cld
	call	__GETDS

	call	[_timer_chain]

	mov	al,20h
	out	20h,al
	pop	gs fs es ds
	popad
	iretd
	endp
; [fold]  ]

	ends
; [fold]  )

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; Uninitialized data
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; [fold]  [

		_udata

_pause_flag	dd	?
_exit_flag	dd	?
_prev_int_08	df	?
		align	4
_prev_int_09	df	?
		align	4
_timer_chain	dd	?

		ends
; [fold]  ]

	end
; [fold]  4
