
                include declare.mac
                decstart
                deccode32
                endcode32
                decstack
                endstack


                public  init
                public  initmem,resetmem

                extrn   getdatapath:near
                extrn   _pm_info:far,_pm_init:far
                extrn   start:near
                include system.mac
                include int.mac
                extrn   setsqrt:near
                include video.mac
                extrn   cleartext:near,writestr:near
                extrn   makehstr:near,makedstr:near

                extrn   datapath:byte,datapathl:dword
                include pm.inc
                include misc.inc
                include int.inc
                include memory.inc
                include dos.inc


;############################################################################
; 16-bit code
;############################################################################

                deccode16

;----------------------------------------------------------------------------
; Read data file into memory

readdatafile:
                call    getdatapath             ; Get data file location

                ; Open file
                mov     ah,DOS_OPENFILE
                mov     al,DATAFILEACCESS
                lea     dx,[datapath]
                int     DOSINT
                jc      @@readdataf2
                mov     [datafilehandle],ax

                ; Read data
;                mov     ah,DOS_READFILE
;                mov     bx,[datafilehandle]
;                mov     dx,_freebasemem
;                mov     ds,dx
;                xor     dx,dx
;                mov     cx,DATAFILEEND-DATAFILEMEM
;                int     DOSINT
;                mov     ax,DSEG
;                mov     ds,ax
;                jc      @@readdataf2

                ; Close file
                mov     ah,DOS_CLOSEFILE
                mov     bx,[datafilehandle]
                int     DOSINT
                ret

@@readdataf2:
                mov     ah,DOS_DISPSTR
                lea     dx,[fileerr]
                int     DOSINT
                mov     ecx,[datapathl]
                mov     datapath[ecx],'$'
                lea     dx,[datapath]
                int     DOSINT
                enderr

;----------------------------------------------------------------------------
; Initialize PMODE

pmerror:
                movzx   eax,ax
                mov     dx,pmerrtab[eax*2]
                mov     ah,DOS_DISPSTR
                int     DOSINT
                enderr

initpm:
                call    _pm_info
                jc      pmerror
                mov     byte ptr [cputype],cl
                mov     byte ptr [pmtype],ch

                ; End of memory allocation block
                mov     dx,es:[_PSP_next_paragraph]
                mov     cx,_freebasemem
                sub     dx,cx
                xor     ax,ax
                cmp     dx,bx            ; Paragraphs needed for pmode data
                jb      pmerror
                mov     es,cx
                call    _pm_init
                jc      pmerror
                .486p                    ; Now in protected mode

                shl     [c32address],4
                shl     [saddress],4
                mov     eax,[saddress]
                sub     eax,[c32address]
                mov     [c32limit],eax

                xor     ax,ax            ; Allocate descriptor
                mov     cx,1
                int     PMINT
                mov     [c32sel],ax
                mov     ax,PM_SETSEGBASE ; Set segment base address
                mov     bx,[c32sel]
                mov     cx,word ptr c32address[MSW]
                mov     dx,word ptr c32address[LSW]
                int     PMINT
                inc     ax               ; Set segment limit
                mov     cx,word ptr c32limit[MSW]
                mov     dx,word ptr c32limit[LSW]
                int     PMINT
                inc     ax               ; Set descriptor access rights
                mov     cx,[c32access]
                int     PMINT

                inc     ax               ; Create alias descriptor
                int     PMINT
                mov     ds,ax
                mov     es,ax
                mov     ax,SELZERO
                mov     gs,ax
                ret

;----------------------------------------------------------------------------
; 16-bit initialization

init:
                .486
                mov     ax,DSEG
                mov     ds,ax
;                call    readdatafile
                call    initpm
                jmp32   [c32sel],init32 ; Jump to the 32bit code segment

                endcode16

;############################################################################
; 32-bit code
;############################################################################

                deccode32

;----------------------------------------------------------------------------
; Allocate memory

initmem:
                mov     ax,PM_GETFREEMEM ; Get free memory information
                mov     edi,offset memstats
                int     PMINT
                mov     ebx,dword ptr memstats[MEMFREE]
                shr     ebx,10
                mov     [memfree],ebx

                mov     edi,offset memmsg1
                mov     ecx,10000000
                call    makedstr
                mov     esi,offset memmsg0
                call    writestr

                cmp     dword ptr memstats[MEMFREE],MEMREQ
                jb      @@initmemf0
                mov     ax,PM_ALLOCMEM   ; Allocate memory block
                mov     bx,word ptr memlimit[MSW]
                mov     cx,word ptr memlimit[LSW]
                int     PMINT
                mov     word ptr memaddress[MSW],bx
                mov     word ptr memaddress[LSW],cx
                mov     word ptr memhandle[MSW],si
                mov     word ptr memhandle[LSW],di
                xor     ax,ax            ; Allocate descriptor
                mov     cx,1
                int     PMINT
                mov     [memsel],ax
                mov     fs,ax

                mov     ax,PM_SETSEGBASE ; Set segment base address
                mov     bx,[memsel]
                mov     cx,word ptr memaddress[MSW]
                mov     dx,word ptr memaddress[LSW]
                int     PMINT
                inc     ax               ; Set segment limit
                mov     cx,word ptr memlimit[MSW]
                mov     dx,word ptr memlimit[LSW]
                int     PMINT
                ret


@@initmemf0:
                mov     esi,offset memerr ; Write error message
                call    writestr
                showcursor
                enderr

;----------------------------------------------------------------------------
; Free allocated memory

resetmem:
                mov     ax,PM_FREEDESC   ; Free descriptor
                mov     bx,word ptr [memsel]
                int     PMINT
                mov     ax,PM_FREEMEM    ; Free memory block
                mov     si,word ptr memhandle[MSW]
                mov     di,word ptr memhandle[LSW]
                int     PMINT
                ret


;----------------------------------------------------------------------------
; 32-bit initialization

init32:
                hidecursor
                call    cleartext
                mov     esi,offset msg0
                call    writestr
                mov     esi,[cputype]
                mov     esi,cpumsgtab[esi*dword]
                call    writestr
                mov     esi,[pmtype]
                mov     esi,pmmsgtab[esi*dword]
                call    writestr

                call    initmem
                mov     cl,32-4
                mov     edx,offset memaddress
                mov     edi,offset memmsg3
                call    makehstr
                mov     esi,offset memmsg2
                call    writestr

                initkb
                call    setsqrt
                setgraph
                jmp     start


;############################################################################
; Data
;############################################################################

;----------------------------------------------------------------------------
datafilehandle  dw      ?

cputype         dd      0
pmtype          dd      0

;----------------------------------------------------------------------------
; Messages

msg0            db      'Initializing...',CR
                db      CR
                db      'PMODE 3.0 protected mode interface kernel',CR
                db      'Copyright (c) 1994, by Tran (a.k.a. Thomas Pytel)'
                db      CR,00h

                ; File reading error messages
fileerr         db      'Error reading file $'

                ; Protected mode initialization error messages
pmerrtab        dw      offset pmerr0,offset pmerr1,offset pmerr2
                dw      offset pmerr3,offset pmerr4,offset pmerr5
                dw      offset pmerr6
pmerr0          db      'Not enough base memory$'
pmerr1          db      '80386 or better not detected$'
pmerr2          db      'System already in protected mode and no VCPI or '
                db      'DPMI found$'
pmerr3          db      'DPMI host is not 32bit$'
pmerr4          db      'Could not enable A20 gate$'
pmerr5          db      'Could not enter DPMI 32bit protected mode$'
pmerr6          db      'Could not allocate needed DPMI selectors$'

                ; CPU messages
cpumsgtab       dd      offset cpumsg0,offset cpumsg1,offset cpumsg2
                dd      offset cpumsg3,offset cpumsg4,offset cpumsg5
                dd      offset cpumsg6
cpumsg0         db      '- 8088/8086 detected',CR,00h
cpumsg1         db      'Error detecting processor',CR,00h
cpumsg2         db      '- 80286 detected',CR,00h
cpumsg3         db      '- 80386 detected',CR,00h
cpumsg4         db      '- 80486 detected',CR,00h
cpumsg5         db      '- Pentium detected',CR,00h
cpumsg6         db      'Error detecting processor',CR,00h

                ; Protected mode type messages
pmmsgtab        dd      offset pmmsg4,offset pmmsg5,offset pmmsg6
                dd      offset pmmsg7
pmmsg4          db      '- Raw protected mode installed',CR,00h
pmmsg5          db      '- XMS protected mode installed',CR,00h
pmmsg6          db      '- VCPI protected mode installed',CR
                db      '  Please use raw/XMS for maximum speed',CR,00h
pmmsg7          db      '- DPMI protected mode installed',CR
                db      '  Please use raw/XMS for maximum speed',CR,00h

                ; Memory messages
memstats        db      48 dup (0)
memfree         dd      ?
memmsg0         db      '- '
memmsg1         db      7 dup ('0'),'k bytes memory available',CR
                db      '  '

                MEMREQK = MEMREQ shr 10
                VALUE = MEMREQK / 10000
                COUNT = 10000
                rept 3
                COUNT = COUNT / 10
                db      '0' + (VALUE/COUNT) mod 10
                endm

                VALUE = MEMREQK mod 10000
                COUNT = 10000
                rept 4
                COUNT = COUNT / 10
                db      '0' + (VALUE/COUNT) mod 10
                endm

                db      'k bytes memory required',CR,00h
memmsg2         db      '  Memory allocated at '
memmsg3         db      8 dup ('0'),CR,00h

                ; Memory error messages
memerr          db      '  Not enough memory',CR,00h

;----------------------------------------------------------------------------
; Memory variables

c32sel          dw      ?
c32address      dd      CSEG
c32limit        dd      ?
c32access       dw      DTYPE+DPAGE+DUSE32+DDPL+DCODE+DACC

saddress        dd      SSEG

memsel          dw      ?
memaddress      dd      ?
memlimit        dd      MEMREQ-1
memhandle       dd      ?

                endcode32
                end

