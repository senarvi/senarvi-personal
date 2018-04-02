
                include declare.mac
                decstart


                public  clearflamebuf,copyflamebuf
                public  randomflame,flamescreen

                include memory.inc
                include flame.inc
                include video.inc

                include math.mac


                deccode32

;----------------------------------------------------------------------------
clearflamebuf:
                mov     ecx,(FLAMEBUFFER_X*FLAMEBUFFER_Y)/4-1
@@clearflamel0:
                mov     dword ptr fs:_flamebuf[ecx*4],0
                dec     ecx
                jge     @@clearflamel0
                ret

;----------------------------------------------------------------------------
copyflamebuf:
                mov     ecx,FLAMEBUFFER_X*(FLAMEBUFFER_Y-2)-1
                mov     ebx,(GRAPHBUFFER_Y-FLAMESQUARE_Y)*GRAPHBUFFER_X
@@copyflamel1:
                mov     edx,GRAPHBUFFER_X-FLAMESQUARE_X
@@copyflamel0:

                mov     al,fs:_flamebuf[ecx]
                mov     ah,al
                mov     fs:_screenbuf[ebx+edx],ax
                mov     fs:_screenbuf[ebx+edx+GRAPHBUFFER_X],ax

                dec     ecx
                sub     edx,FLAMESQUARE_X
                jge     @@copyflamel0
                sub     ebx,FLAMESQUARE_Y*GRAPHBUFFER_X
                jge     @@copyflamel1
                ret

;----------------------------------------------------------------------------
; Greets MAC / PROPHECY
random:
                mov      ax,[randseed]
                mov      bx,[randseed+2]
                mov      si,ax
                mov      di,bx
                mov      dl,ah
                mov      ah,al
                mov      al,bh
                shl     ebx,8
                rcr      dl,1
                rcr      ax,1
                rcr      bx,1
                add      bx,di
                adc      ax,si
                add      bx,62e9h
                adc      ax,3619h
                mov      word ptr [randseed],bx
                mov      word ptr [randseed+2],ax
                xor      dx,dx
                div      cx
                ret

;----------------------------------------------------------------------------
randomflame:
                xor     edx,edx
                mov     ecx,6
@@randomflamel0:
                push    ecx
                mov     ecx,FLAMEBUFFER_X-(2*5)
                call    random
                lea     edi,[(FLAMEBUFFER_Y-3)*FLAMEBUFFER_X+5+edx]
                pop     ecx

                push    ecx
                push    edi
                mov     ecx,128
                call    random
                pop     edi
                pop     ecx


                mov     dh,dl
                mov     fs:_flamebuf[edi],dx
                dec     ecx
                jg      @@randomflamel0
                ret

;----------------------------------------------------------------------------
flamescreen:
                mov     ecx,FLAMEBUFFER_X+1
                xor     edx,edx
@@flamescreenl0:
                movzx   edx,byte ptr fs:_flamebuf[ecx+FLAMEBUFFER_X-1]
                cmul    edx,2
                add     eax,edx
                movzx   edx,byte ptr fs:_flamebuf[ecx]
                cmul    edx,3
                add     eax,edx
                movzx   edx,byte ptr fs:_flamebuf[ecx-FLAMEBUFFER_X+1]
                cmul    edx,2
                add     eax,edx
                cdiv    eax,8

                mov     fs:_flamebuf[ecx-FLAMEBUFFER_X],al
                inc     ecx
                cmp     ecx,(FLAMEBUFFER_Y-2)*FLAMEBUFFER_X-1
                jle     @@flamescreenl0
                ret


;----------------------------------------------------------------------------
randseed        dw      06425h,0a830h

                endcode32
                end

