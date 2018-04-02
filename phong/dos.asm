
                include declare.mac
                decstart
                deccode32
                endcode32

                public  getdatapath
                public  datapath,datapathl

                include memory.inc
                include misc.inc


;############################################################################
; 16-bit code
;############################################################################

                deccode16

;----------------------------------------------------------------------------
; Get environment variable

getenvvar:
                mov     fs,es:[_PSP_environment]
                xor     ecx,ecx
@@getcmdlinef4:
                xor     edx,edx
@@getcmdlinef2:
;                mov     al,cmdlinestr[edx]
                cmp     fs:[ecx],al
                jne     @@getcmdlinef0
                cmp     byte ptr fs:[ecx],'='
                je      @@getcmdlinef1
                inc     ecx
                inc     edx
                jmp     @@getcmdlinef2

@@getcmdlinef0:
                cmp     byte ptr fs:[ecx],0
                jz      @@getcmdlinef3
                inc     ecx
                jmp     @@getcmdlinef0

@@getcmdlinef3:
                inc     ecx
                cmp     byte ptr fs:[ecx],0
                jnz     @@getcmdlinef4

                ; Ei l”ytyny...
@@getcmdlinef1:
                ret

;----------------------------------------------------------------------------
; Get program path and include name of the data file
;
; In:
;   es - Segment for PSP
;
; Out:
;   [datapath] - path to data directory (including '\')
;   [datapathl] - lengt of path
;
; Regs used:
;   al ecx edx fs

getdatapath:
                ; Search end of the environment block
                mov     fs,es:[_PSP_environment]
                xor     edx,edx
@@getdatapathf0:
                inc     edx
                cmp     byte ptr fs:[edx-1],0
                jnz     @@getdatapathf0
                inc     edx
                cmp     byte ptr fs:[edx-1],0
                jnz     @@getdatapathf0

                ; Get command line
                xor     ecx,ecx
@@getdatapathf2:
                mov     al,fs:[edx+2][ecx]
                or      al,al
                jz      @@getdatapathf1
                mov     datapath[ecx],al
                inc     ecx
                jmp     @@getdatapathf2
@@getdatapathf1:

                ; Get length of the path
@@getdatapathf4:
                dec     ecx
                mov     al,datapath[ecx]
                cmp     al,'\'
                jne     @@getdatapathf4
                inc     ecx

                ; Include name of data file
                xor     edx,edx
@@readdataf0:
                mov     al,datafilename[edx]
                or      al,al
                jz      @@readdataf1
                mov     datapath[ecx+edx],al
                inc     edx
                jmp     @@readdataf0
@@readdataf1:
                add     ecx,edx
                mov     [datapathl],ecx
                mov     datapath[ecx],00h ; End-of-string character
                ret

                endcode16


;############################################################################
; Data
;############################################################################

                deccode32

datafilename    db      DATAFILENAME,00h
datapath        db      MAXPATH dup ('?')
datapathl       dd      ?

                endcode32
                end

