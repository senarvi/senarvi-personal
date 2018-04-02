
                include declare.mac


                decstart


                include video.inc
                include misc.inc
                include memory.inc
                extrn   areamin:spixel,areamax:spixel

                extrn   pow:near

                public  textx,texty
                public  buffera1,buffera2

                public  setduckpal,setwavepal,setflamepal,settunnelpal
                public  writestr
                public  cleartext
                public  clearbuffer,copybuffer
                public  mblur


                include math.mac         ; cmul


                deccode32

flame_palette   label   byte
                include \prog\asm\source\fire!\palette.inc

;----------------------------------------------------------------------------
setpalette:
                mov     ecx,GRAPHCOLORS-1
@@setpalettel0:
                mov     edx,3c8h
                mov     eax,ecx
                out     dx,al

                mov     edx,3c9h
                mov     al,fs:_palette[ecx*2+ecx]
                out     dx,al
                mov     al,fs:_palette[ecx*2+ecx+1]
                out     dx,al
                mov     al,fs:_palette[ecx*2+ecx+2]
                out     dx,al

                dec     ecx
                jge     @@setpalettel0
                ret

;----------------------------------------------------------------------------
setduckpal:
PAL_FRACBITS    =       FRACBITS
FRACBITS        =       8

                mov     byte ptr fs:_palette[0],0
                mov     byte ptr fs:_palette[1],0
                mov     byte ptr fs:_palette[2],0

                mov     ecx,GRAPHCOLORS-1
@@setduckpall0:
                ; red
                mov     ebx,[duck_ambient.red]
                mov     eax,ecx
                mov     edx,[duck_diffuse.red]
                mul     edx
                shrd    eax,edx,FRACBITS
                add     ebx,eax

                push    ecx ebx
                mov     eax,ecx
                mov     ecx,[duck_shine]
                call    pow
                pop     ebx ecx
                mov     edx,[duck_specular.red]
                mul     edx
                shrd    eax,edx,FRACBITS
                add     eax,ebx
                mov     fs:_palette[ecx*2+ecx],al


                ; green
                mov     ebx,[duck_ambient.green]
                mov     eax,ecx
                mov     edx,[duck_diffuse.green]
                mul     edx
                shrd    eax,edx,FRACBITS
                add     ebx,eax

                push    ecx ebx
                mov     eax,ecx
                mov     ecx,[duck_shine]
                call    pow
                pop     ebx ecx
                mov     edx,[duck_specular.green]
                mul     edx
                shrd    eax,edx,FRACBITS
                add     eax,ebx
                mov     fs:_palette[ecx*2+ecx+1],al


                ; blue
                mov     ebx,[duck_ambient.blue]
                mov     eax,ecx
                mov     edx,[duck_diffuse.blue]
                mul     edx
                shrd    eax,edx,FRACBITS
                add     ebx,eax

                mov     eax,ecx
                push    ecx ebx
                mov     ecx,[duck_shine]
                call    pow
                pop     ebx ecx
                mov     edx,[duck_specular.blue]
                mul     edx
                shrd    eax,edx,FRACBITS
                add     eax,ebx
                mov     fs:_palette[ecx*2+ecx+2],al

                dec     ecx
                jg      @@setduckpall0
                call    setpalette

FRACBITS        =       PAL_FRACBITS
                ret

;----------------------------------------------------------------------------
setwavepal:
                mov     byte ptr fs:_palette[0],0
                mov     byte ptr fs:_palette[1],0
                mov     byte ptr fs:_palette[2],0

                mov     ecx,GRAPHCOLORS/2-1
@@setwavepall0:
                mov     byte ptr fs:_palette[ecx*2+ecx+0],0
                mov     byte ptr fs:_palette[ecx*2+ecx+1],0
                mov     eax,ecx
                shr     eax,1
                mov     fs:_palette[ecx*2+ecx+2],al
                dec     ecx
                jg      @@setwavepall0

                mov     ecx,GRAPHCOLORS-1
@@setwavepall1:
                mov     eax,ecx
                sub     eax,GRAPHCOLORS/2
                shr     eax,1
                mov     fs:_palette[ecx*2+ecx+0],al
                mov     fs:_palette[ecx*2+ecx+1],al
                mov     byte ptr fs:_palette[ecx*2+ecx+2],63
                dec     ecx
                cmp     ecx,GRAPHCOLORS/2
                jge     @@setwavepall1
                call    setpalette
                ret

;----------------------------------------------------------------------------
setflamepal:
                pushad

                mov     byte ptr fs:_palette[0],0
                mov     byte ptr fs:_palette[1],0
                mov     byte ptr fs:_palette[2],0

                xor     ecx,ecx
@@setflamepall0:
                mov     byte ptr fs:_palette[ecx*2+ecx],cl
                mov     byte ptr fs:_palette[ecx*2+ecx+1],0
                mov     byte ptr fs:_palette[ecx*2+ecx+2],0
                inc     ecx
                cmp     ecx,40h
                jl      @@setflamepall0

@@setflamepall1:
                mov     byte ptr fs:_palette[ecx*2+ecx],3fh
                mov     byte ptr fs:_palette[ecx*2+ecx+1],cl
                mov     byte ptr fs:_palette[ecx*2+ecx+2],0
                inc     ecx
                cmp     ecx,80h
                jl      @@setflamepall1

@@setflamepall2:
                mov     byte ptr fs:_palette[ecx*2+ecx],3fh
                mov     byte ptr fs:_palette[ecx*2+ecx+1],3fh
                mov     byte ptr fs:_palette[ecx*2+ecx+2],cl
                inc     ecx
                cmp     ecx,0c0h
                jl      @@setflamepall2

                mov     [duck_ambient.red],5
                mov     [duck_ambient.green],0
                mov     [duck_ambient.blue],0
                mov     [duck_diffuse.red],20
                mov     [duck_diffuse.green],20
                mov     [duck_diffuse.blue],0
                mov     [duck_specular.red],38
                mov     [duck_specular.green],38
                mov     [duck_specular.blue],38
                mov     [duck_shine],1
                call    setduckpal

                mov     ecx,GRAPHCOLORS/2
@@setflamepall3:
                mov     al,flame_palette[ecx*2+ecx]
                mov     fs:_palette[ecx*2+ecx],al
                mov     al,flame_palette[ecx*2+ecx+1]
                mov     fs:_palette[ecx*2+ecx+1],al
                mov     al,flame_palette[ecx*2+ecx+2]
                mov     fs:_palette[ecx*2+ecx+2],al
                dec     ecx
                jge     @@setflamepall3

                call    setpalette
                popad
                ret

;----------------------------------------------------------------------------
settunnelpal:
                mov     ecx,GRAPHCOLORS
@@settunnelpall0:
                mov     eax,ecx
                cdiv    eax,4
                mov     fs:_palette[ecx*2+ecx],al
;                cmul    al,2
                mov     fs:_palette[ecx*2+ecx+1],al
;                cmul    al,2
                mov     fs:_palette[ecx*2+ecx+2],al
                dec     ecx
                jge     @@settunnelpall0
                call    setpalette
                ret

;----------------------------------------------------------------------------
writestr:
                mov     ah,07h
@@writestrl0:
                mov     al,[esi]
                or      al,al
                jz      @@writestrf0
                inc     esi
                cmp     al,CR
                je      @@writestrf1

                mov     edi,[texty]
                cmul    edi,80
                add     edi,[textx]
                mov     gs:_VGA_text_buffer[edi*2],ax

                cmp     [textx],TEXTWIDTH-1
                jae     @@writestrf1
                inc     [textx]
                jmp     @@writestrl0

@@writestrf1:
                mov     [textx],0
                inc     [texty]
                jmp     @@writestrl0

@@writestrf0:
                ret

;----------------------------------------------------------------------------
cleartext:
                mov     edi,TEXTWIDTH*TEXTHEIGHT/2-1
                mov     eax,07000700h
@@cleartextf0:
                mov     gs:_VGA_text_buffer[edi*4],eax
                dec     edi
                jge     @@cleartextf0
                ret

;----------------------------------------------------------------------------
clearbuffer:
                mov     eax,[buffera1.x]
                mov     ecx,[buffera1.y]
                mov     edx,[buffera2.x]
                mov     ebx,[buffera2.y]

                sub     edx,eax
                inc     edx
                sub     ebx,ecx
                inc     ebx

                mov     ebp,GRAPHWIDTH
                sub     ebp,edx

                mov     edi,ecx
                cmul    edi,GRAPHWIDTH
                add     edi,eax


                mov     ecx,ebx          ; deltay
@@clearbuff1:
                mov     eax,edx          ; deltax
@@clearbuff0:
                mov     byte ptr fs:_screenbuf[edi],0
                mov     dword ptr fs:_zbuf[edi*4],7fffffffh
                inc     edi
                dec     eax
                ja      @@clearbuff0
                add     edi,ebp
                dec     ecx
                ja      @@clearbuff1
                ret

;----------------------------------------------------------------------------
mblur:          mov     edi,ebx
@@mblurf1:      cmp     byte ptr fs:_screenbuf[edi+esi],00h
                jz      @@mblurf0
                inc     byte ptr fs:_screenbuf[edi+esi]
@@mblurf0:      inc     edi
                cmp     edi,ecx
                jbe     @@mblurf1
                add     esi,GRAPHWIDTH
                cmp     esi,edx
                jbe     mblur
                ret

;----------------------------------------------------------------------------
copybuffer:
                mov     eax,[screena1.x]
                cmp     eax,[buffera1.x]
                jb      @@copybufferf0
                mov     eax,[buffera1.x]
@@copybufferf0:

                mov     ecx,[screena1.y]
                cmp     ecx,[buffera1.y]
                jb      @@copybufferf1
                mov     ecx,[buffera1.y]
@@copybufferf1:
                cmul    ecx,GRAPHWIDTH

                mov     edx,[screena2.x]
                cmp     edx,[buffera2.x]
                ja      @@copybufferf2
                mov     edx,[buffera2.x]
@@copybufferf2:

                mov     ebx,[screena2.y]
                cmp     ebx,[buffera2.y]
                ja      @@copybufferf3
                mov     ebx,[buffera2.y]
@@copybufferf3:
                cmul    ebx,GRAPHWIDTH

                mov     ebp,[buffera1.x]
                mov     [screena1.x],ebp
                mov     ebp,[buffera1.y]
                mov     [screena1.y],ebp
                mov     ebp,[buffera2.x]
                mov     [screena2.x],ebp
                mov     ebp,[buffera2.y]
                mov     [screena2.y],ebp

@@copybufferl0:
                mov     esi,eax
@@copybufferl1:
                mov     ebp,fs:_screenbuf[esi+ecx]
                mov     gs:_VGA_graphics_buffer[esi+ecx],ebp
                add     esi,4
                cmp     esi,edx
                jbe     @@copybufferl1
                add     ecx,GRAPHWIDTH
                cmp     ecx,ebx
                jbe     @@copybufferl0
                ret


;----------------------------------------------------------------------------
textx           dd      0
texty           dd      0

;----------------------------------------------------------------------------
duck_ambient    scolor  <5, 5, 0>
duck_diffuse    scolor  <20,20,0>
duck_specular   scolor  <38,38,38>
duck_shine      dd      10

;----------------------------------------------------------------------------
buffera1        spixel  <0,0>
buffera2        spixel  <GRAPHWIDTH-1,GRAPHHEIGHT-1>
screena1        spixel  <0,0>
screena2        spixel  <GRAPHWIDTH-1,GRAPHHEIGHT-1>

                endcode32
                end

