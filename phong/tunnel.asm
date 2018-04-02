
                include declare.mac
                decstart


RADIUS          =       128


                public  settunnel,drawtunnel

                include memory.inc
                include math.mac
                include sin.inc
                include project.inc

                extrn   angletab:byte
                extrn   marble_texmap:byte
                extrn   sintable:SIN_TYPE,costable:SIN_TYPE


                deccode32

;----------------------------------------------------------------------------
settunnel:
                mov     ebp,1
@@settunnell1:
                mov     ebx,1
@@settunnell0:


                lea     ecx,[GRAPHHEIGHT/2-1+ebp]
                cmul    ecx,GRAPHWIDTH
                add     ecx,GRAPHWIDTH/2-1
                lea     edi,[ecx+ebx]

                movzx   esi,angletab[edi]       ; 0..ff
                mov     eax,esi
                add     al,40h
                mov     esi,eax
                cdiv    esi,4                   ; 0..3f
                mov     eax,costable[esi*4]     ; 1..0
                sar     eax,SIN_FRACBITS-FRACBITS ; 100..0
                cmul    eax,RADIUS              ; 0..20.00
                cmul    eax,LENSDIST            ; 0..2000.00
                xor     edx,edx
                idiv    ebx                     ; 0..2000.00
                sar     eax,8                   ; 0..2000
                mov     fs:_tunneldepth[edi*4],eax

                sub     ecx,ebx
                mov     fs:_tunneldepth[ecx*4],eax

                mov     ecx,GRAPHHEIGHT/2
                sub     ecx,ebp
                cmul    ecx,GRAPHWIDTH
                add     ecx,GRAPHWIDTH/2
                lea     edi,[ecx+ebx]
                mov     fs:_tunneldepth[edi*4],eax

                sub     ecx,ebx
                mov     fs:_tunneldepth[ecx*4],eax


                inc     ebx
                cmp     ebx,GRAPHWIDTH/2
                jle     @@settunnell0
                inc     ebp
                cmp     ebp,GRAPHHEIGHT/2
                jle     @@settunnell1
                ret

;----------------------------------------------------------------------------
; Greets: Il£vatar of Nordic Line
drawtunnel:
                mov     ecx,GRAPHWIDTH*GRAPHHEIGHT-1
                mov     ebp,GRAPHHEIGHT-1
@@drawtunnell1:
                mov     ebx,GRAPHWIDTH-1
@@drawtunnell0:
                movzx   esi,angletab[ecx]
                add     esi,[tunnel_x]
                and     esi,0ffh
                mov     edi,fs:_tunneldepth[ecx*4]
;                sar     edi,1
                add     edi,[tunnel_y]
                and     edi,07fh
                cmul    edi,320
                add     esi,edi
                mov     al,marble_texmap[esi]
;                mov     eax,fs:_tunneldepth[ecx*4]
                mov     gs:_VGA_graphics_buffer[ecx],al
                dec     ecx
                dec     ebx
                jge     @@drawtunnell0
                dec     ebp
                jge     @@drawtunnell1

;                add     [tunnel_x],5
                add     [tunnel_y],30
                ret


;----------------------------------------------------------------------------
tunnel_x        dd      0
tunnel_y        dd      0

                endcode32
                end

