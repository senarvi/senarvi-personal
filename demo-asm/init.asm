        %title  "Copyright (c) Seppo Enarvi  1996,1997"
        %subttl "Initialization code"


        include declare.mac


        include "video.mac"
        include "system.mac"
        include "int.mac"
        include "math.mac"

        include "init.inc"
        include "system.inc"
        include "pmode.inc"
        include "math.inc"
        include "video.inc"
        include "int.inc"
        include "kb.inc"
        include "memory.inc"


;############################################################################
; Data
;############################################################################
; [fold]  [

                decdata

                align   dword
;----------------------------------------------------------------------------
cputype         dd      0
pmtype          dd      0

;----------------------------------------------------------------------------
; [fold]  [
; Memory variables

c32sel          dd      0
c32base         dd      CSEG
c32limit        dd      ?
c32access       dd      DTYPE+DPAGE+DUSE32+DDPL+DCODE+DACC

sbase           dd      SSEG

memsel          dd      0
membase         dd      ?
memlimit        dd      ?
memhandle       dd      ?
memstats        smemstats ?
memfree         dd      ?

basememseg      dd      0
basememaddress  dd      ?
basememfree     dd      0
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
; Exception handler

exception_type  dd      ?
errorcode       dd      ?

                ; Registers saved
eax_save        dd      ?
ecx_save        dd      ?
edx_save        dd      ?
ebx_save        dd      ?
esp_save        dd      ?
ebp_save        dd      ?
esi_save        dd      ?
edi_save        dd      ?

eip_save        dd      ?

cs_save         dd      0
ds_save         dd      0
es_save         dd      0
fs_save         dd      0
gs_save         dd      0
ss_save         dd      0

eflags_save     dd      ?
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
; Messages

; [fold]  [
msg0            db      'by ravage / STATiC 1996',CR
                db      'Initializing...',CR
                db      CR
                db      'PMODE 3.0 protected mode interface kernel',CR
                db      'Copyright (c) 1994, by Tran (a.k.a. Thomas Pytel)'
                db      CR,00h

; [fold]  ]
; [fold]  [
                ; File reading error messages
fileerr         db      'Error reading file $'

; [fold]  ]
; [fold]  [
                ; Protected mode initialization error messages
pmerrtab        dd      offset pmerr0,offset pmerr1,offset pmerr2
                dd      offset pmerr3,offset pmerr4,offset pmerr5
                dd      offset pmerr6
pmerr0          db      'Not enough base memory$'
pmerr1          db      '80386 or better not detected$'
pmerr2          db      'System already in protected mode and no VCPI or '
                db      'DPMI found$'
pmerr3          db      'DPMI host is not 32bit$'
pmerr4          db      'Could not enable A20 gate$'
pmerr5          db      'Could not enter DPMI 32bit protected mode$'
pmerr6          db      'Could not allocate needed DPMI selectors$'

; [fold]  ]
; [fold]  [
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

; [fold]  ]
; [fold]  [
                ; Protected mode type messages
pmmsgtab        dd      offset pmmsg4,offset pmmsg5,offset pmmsg6
                dd      offset pmmsg7
pmmsg4          db      '- Raw protected mode installed',CR,00h
pmmsg5          db      '- XMS protected mode installed',CR,00h
pmmsg6          db      '- VCPI protected mode installed',CR
                db      '  Please use raw/XMS for maximum speed',CR,00h
pmmsg7          db      '- DPMI protected mode installed',CR
                db      '  Please use raw/XMS for maximum speed',CR,00h

; [fold]  ]
; [fold]  [
                ; Memory messages
memmsg0         db      '- '
memmsg1         db      7 dup ('0'),' bytes memory available',CR,'  '
memmsg4         db      10 dup ('0'),' bytes memory required',CR,00h
memmsg2         db      '  Memory allocated at '
memmsg3         db      8 dup ('0'),CR,00h

memerr          db      '  Not enough memory',CR,00h

; [fold]  ]
; [fold]  [
                ; Exception handler messages
excmsgtab       dd      offset excmsg0,offset excmsg1,offset excmsg2
                dd      offset excmsg3,offset excmsg4,offset excmsg5
                dd      offset excmsg6,offset excmsg7,offset excmsg8
                dd      offset excmsg9,offset excmsg10,offset excmsg11
                dd      offset excmsg12,offset excmsg13,offset excmsg14
                dd      offset excmsg15,offset excmsg16
excmsg0         db      'Divide error',CR,00h
excmsg1         db      'Debug exception',CR,00h
excmsg2         db      'Nonmaskable interrupt',CR,00h
excmsg3         db      'Breakpoint',CR,00h
excmsg4         db      'Overflow',CR,00h
excmsg5         db      'Bounds check',CR,00h
excmsg6         db      'Invalid opcode',CR,00h
excmsg7         db      'Coprocessor not available',CR,00h
excmsg8         db      'Double fault',CR,00h
excmsg9         db      'Coprocessor segment overrun',CR,00h
excmsg10        db      'Invalid TSS',CR,00h
excmsg11        db      'Segment not present',CR,00h
excmsg12        db      'Stack exception',CR,00h
excmsg13        db      'General protection exception',CR,00h
excmsg14        db      'Page fault',CR,00h
excmsg15        db      '',CR,00h
excmsg16        db      'Coprocessor error',CR,00h


excmsg          db      'Abnormal program termination',CR
                db      CR
                db      'Exception occured at eip='
exceip          db      8 dup (?),' esp='
excesp          db      8 dup (?),CR

                db      CR
                db      'General registers:',CR
                db      '  eax='
exceax          db      8 dup (?),' ecx='
excecx          db      8 dup (?),' edx='
excedx          db      8 dup (?),' ebx='
excebx          db      8 dup (?),CR
                db      '  esi='
excesi          db      8 dup (?),' edi='
excedi          db      8 dup (?),' ebp='
excebp          db      8 dup (?),CR

                db      CR
                db      'Segment registers:',CR
                db      '  cs='
exccs           db      4 dup (?),' base='
exccsbase       db      8 dup (?),' limit='
exccslimit      db      8 dup (?),CR
                db      '  ds='
excds           db      4 dup (?),' base='
excdsbase       db      8 dup (?),' limit='
excdslimit      db      8 dup (?),CR
                db      '  es='
exces           db      4 dup (?),' base='
excesbase       db      8 dup (?),' limit='
exceslimit      db      8 dup (?),CR
                db      '  fs='
excfs           db      4 dup (?),' base='
excfsbase       db      8 dup (?),' limit='
excfslimit      db      8 dup (?),CR
                db      '  gs='
excgs           db      4 dup (?),' base='
excgsbase       db      8 dup (?),' limit='
excgslimit      db      8 dup (?),CR
                db      '  ss='
excss           db      4 dup (?),' base='
excssbase       db      8 dup (?),' limit='
excsslimit      db      8 dup (?),CR,00h

invalidmsg      db      'invalid '
; [fold]  ]

; [fold]  ]

                ends
; [fold]  ]

;############################################################################
; 32-bit code
;############################################################################
; [fold]  [

        deccode32

;----------------------------------------------------------------------------
; [fold]  [
; Allocate memory

        proc    initmem c near
        pushad

        mov     eax,DPMI_GETFREEMEM     ; Get free memory information
        mov     edi,offset memstats
        int     DPMI_INT

        mov     edx,[memstats.free]
        cdiv    edx,KB                   ; In kilobytes
        mov     [memfree],edx
        mov     ecx,7
        mov     edi,offset memmsg1
        call    int2str
        mov     edx,[memreq]
;        cdiv    edx,KB
        mov     ecx,10
        mov     edi,offset memmsg4
        call    int2str
        mov     esi,offset memmsg0
        call    writestr

        mov     eax,[memreq]
        cmp     [dword memstats+MEMFREE],eax
        jb      @@error
        dec     eax
        mov     [memlimit],eax

        mov     eax,DPMI_ALLOCMEM       ; Allocate memory block
        mov     bx,[word memlimit+MSW]
        mov     cx,[word memlimit+LSW]
        int     DPMI_INT
        mov     [word membase+MSW],bx
        mov     [word membase+LSW],cx
        mov     [word memhandle+MSW],si
        mov     [word memhandle+LSW],di
        xor     eax,eax                 ; Allocate descriptor
        mov     ecx,1
        int     DPMI_INT
        mov     [word memsel],ax
        mov     fs,ax

        mov     eax,DPMI_SETSEGBASE     ; Set segment base address
        mov     bx,[word memsel]
        mov     cx,[word membase+MSW]
        mov     dx,[word membase+LSW]
        int     DPMI_INT
        inc     eax                     ; Set segment limit
        mov     cx,[word memlimit+MSW]
        mov     dx,[word memlimit+LSW]
        int     DPMI_INT
        popad
        ret

@@error:
        mov     esi,offset memerr       ; Write error message
        call    writestr
        popad
        showcursor
        enderr
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
; Free allocated memory

        proc    resetmem c near
        pushad

        mov     eax,DPMI_FREEDESC       ; Free descriptor
        mov     bx,[word memsel]
        int     DPMI_INT
        mov     eax,DPMI_FREEMEM        ; Free memory block
        mov     si,[word memhandle+MSW]
        mov     di,[word memhandle+LSW]
        int     DPMI_INT

        popad
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
; 32-bit initialization code

init32:
        initkb  kbint
        setint  0,int00handler
        setint  2,int02handler
        setint  6,int06handler
        setint  12,int12handler
        setint  13,int13handler
        setint  14,int14handler

        hidecursor
        call    cleartext
        mov     esi,offset msg0
        call    writestr
        mov     esi,[cputype]
        mov     esi,[cpumsgtab+esi*dword]
        call    writestr
        mov     esi,[pmtype]
        mov     esi,[pmmsgtab+esi*dword]
        call    writestr

        call    initmem
        mov     edx,[membase]
        mov     ecx,8
        mov     edi,offset memmsg3
        call    makehstr
        mov     esi,offset memmsg2
        call    writestr

        mov     eax,[basememseg]
        shl     eax,4
        mov     [basememaddress],eax
        jmp     start
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
; Exception handler

int00handler:
        mov     [exception_type],0
        jmp     exception_handler
int02handler:
        mov     [exception_type],2
        jmp     exception_handler
int06handler:
        mov     [exception_type],6
        jmp     exception_handler
int12handler:
        mov     [exception_type],12
        pop     [errorcode]
        jmp     exception_handler
int13handler:
        mov     [exception_type],13
        pop     [errorcode]
        jmp     exception_handler
int14handler:
        mov     [exception_type],14
        pop     [errorcode]

exception_handler:
        pop     [eip_save]
        pop     [cs_save]               ; Padded to 4 bytes
        pop     [eflags_save]

        mov     [eax_save],eax
        mov     [ecx_save],ecx
        mov     [edx_save],edx
        mov     [ebx_save],ebx
        mov     [esp_save],esp
        mov     [ebp_save],ebp
        mov     [esi_save],esi
        mov     [edi_save],edi

        mov     [word ds_save],ds
        mov     [word es_save],es
        mov     [word fs_save],fs
        mov     [word gs_save],gs
        mov     [word ss_save],ss

        endint

        call    cleartext
        settext

        ;------------------------------
        ; Display name of the exception

        mov     ebx,[exception_type]
        mov     esi,[excmsgtab+ebx*4]
        call    writestr

        ;---------------------------------------
        ; Store 32-bit registers into the string

        mov     cl,8

        mov     edx,[eip_save]
        mov     edi,offset exceip
        call    makehstr

        mov     edx,[eax_save]
        mov     edi,offset exceax
        call    makehstr
        mov     edx,[ecx_save]
        mov     edi,offset excecx
        call    makehstr
        mov     edx,[edx_save]
        mov     edi,offset excedx
        call    makehstr
        mov     edx,[ebx_save]
        mov     edi,offset excebx
        call    makehstr

        mov     edx,[esp_save]
        mov     edi,offset excesp
        call    makehstr
        mov     edx,[ebp_save]
        mov     edi,offset excebp
        call    makehstr
        mov     edx,[esi_save]
        mov     edi,offset excesi
        call    makehstr
        mov     edx,[edi_save]
        mov     edi,offset excedi
        call    makehstr

        ;---------------------------------------
        ; Store segment registers into the string

        mov     cl,4

        mov     edx,[cs_save]
        mov     edi,offset exccs
        call    makehstr
        mov     edx,[ds_save]
        mov     edi,offset excds
        call    makehstr
        mov     edx,[es_save]
        mov     edi,offset exces
        call    makehstr
        mov     edx,[fs_save]
        mov     edi,offset excfs
        call    makehstr
        mov     edx,[gs_save]
        mov     edi,offset excgs
        call    makehstr
        mov     edx,[ss_save]
        mov     edi,offset excss
        call    makehstr

        ;------------------------------------------------------
        ; Store segment base addresses & limits into the string

        ; CS
        mov     eax,DPMI_GETSEGBASE
        mov     ebx,[cs_save]
        int     DPMI_INT
        jc      @@invalidcs
        shl     ecx,16
        mov     cx,dx
        mov     edx,ecx
        mov     cl,8
        mov     edi,offset exccsbase
        call    makehstr
        lsl     edx,[cs_save]
        mov     edi,offset exccslimit
        call    makehstr
        jmp     @@getdsinfo
@@invalidcs:
        mov     esi,offset invalidmsg
        mov     edi,offset exccsbase
        mov     ecx,8
rep     movsb
        mov     esi,offset invalidmsg
        mov     edi,offset exccslimit
        mov     ecx,8
rep     movsb

        ; DS
@@getdsinfo:
        mov     eax,DPMI_GETSEGBASE
        mov     ebx,[ds_save]
        int     DPMI_INT
        jc      @@invalidds
        shl     ecx,16
        mov     cx,dx
        mov     edx,ecx
        mov     cl,8
        mov     edi,offset excdsbase
        call    makehstr
        lsl     edx,[ds_save]
        mov     edi,offset excdslimit
        call    makehstr
        jmp     @@getesinfo
@@invalidds:
        mov     esi,offset invalidmsg
        mov     edi,offset excdsbase
        mov     ecx,8
rep     movsb
        mov     esi,offset invalidmsg
        mov     edi,offset excdslimit
        mov     ecx,8
rep     movsb

        ; ES
@@getesinfo:
        mov     eax,DPMI_GETSEGBASE
        mov     ebx,[es_save]
        int     DPMI_INT
        jc      @@invalides
        shl     ecx,16
        mov     cx,dx
        mov     edx,ecx
        mov     cl,8
        mov     edi,offset excesbase
        call    makehstr
        lsl     edx,[es_save]
        mov     edi,offset exceslimit
        call    makehstr
        jmp     @@getfsinfo
@@invalides:
        mov     esi,offset invalidmsg
        mov     edi,offset excesbase
        mov     ecx,8
rep     movsb
        mov     esi,offset invalidmsg
        mov     edi,offset exceslimit
        mov     ecx,8
rep     movsb

        ; FS
@@getfsinfo:
        mov     eax,DPMI_GETSEGBASE
        mov     ebx,[fs_save]
        int     DPMI_INT
        jc      @@invalidfs
        shl     ecx,16
        mov     cx,dx
        mov     edx,ecx
        mov     cl,8
        mov     edi,offset excfsbase
        call    makehstr
        lsl     edx,[fs_save]
        mov     edi,offset excfslimit
        call    makehstr
        jmp     @@getgsinfo
@@invalidfs:
        mov     esi,offset invalidmsg
        mov     edi,offset excfsbase
        mov     ecx,8
rep     movsb
        mov     esi,offset invalidmsg
        mov     edi,offset excfslimit
        mov     ecx,8
rep     movsb

        ; GS
@@getgsinfo:
        mov     eax,DPMI_GETSEGBASE
        mov     ebx,[gs_save]
        int     DPMI_INT
        jc      @@invalidgs
        shl     ecx,16
        mov     cx,dx
        mov     edx,ecx
        mov     cl,8
        mov     edi,offset excgsbase
        call    makehstr
        lsl     edx,[gs_save]
        mov     edi,offset excgslimit
        call    makehstr
        jmp     @@getssinfo
@@invalidgs:
        mov     esi,offset invalidmsg
        mov     edi,offset excgsbase
        mov     ecx,8
rep     movsb
        mov     esi,offset invalidmsg
        mov     edi,offset excgslimit
        mov     ecx,8
rep     movsb

        ; SS
@@getssinfo:
        mov     eax,DPMI_GETSEGBASE
        mov     ebx,[ss_save]
        int     DPMI_INT
        jc      @@invalidss
        shl     ecx,16
        mov     cx,dx
        mov     edx,ecx
        mov     edx,[sbase]
        mov     cl,8
        mov     edi,offset excssbase
        call    makehstr
        lsl     edx,[ss_save]
        mov     edi,offset excsslimit
        call    makehstr
        jmp     @@displaystring
@@invalidss:
        mov     esi,offset invalidmsg
        mov     edi,offset excssbase
        mov     ecx,8
rep     movsb
        mov     esi,offset invalidmsg
        mov     edi,offset excsslimit
        mov     ecx,8
rep     movsb


        ;---------------
        ; Display string

@@displaystring:
        mov     esi,offset excmsg
        call    writestr

        call    resetmem
        endprog
; [fold]  ]

                ends
; [fold]  ]

;############################################################################
; 16-bit code
;############################################################################
; [fold]  [

        deccode16

;----------------------------------------------------------------------------
; [fold]  [
; Initialize PMODE

initpm:
;        p8086
        call    _pm_info
        jc      @@error
        mov     [byte cputype],cl
        mov     [byte pmtype],ch

        mov     dx,[es:_PSP_next_paragraph] ; End of memory allocation block
        mov     cx,[word heapseg]
        sub     dx,cx
        mov     [word basememfree],dx   ; Amount of free base memory
        xor     ax,ax
        add     bx,[word basememreq]    ; Paragraphs needed for pmode data
        mov     [word basememreq],bx
        cmp     dx,bx
        jb      @@error
        mov     es,cx
        add     cx,bx                   ; End of PMODE data
        mov     [word basememseg],cx
        call    _pm_init
        jc      @@error
;        p486                            ; In protected mode

        shl     [c32base],4
        shl     [sbase],4
        mov     eax,[sbase]
        sub     eax,[c32base]
        dec     eax                     ; Length of code segment - 1
        shr     eax,12                  ; Page aligned
        mov     [c32limit],eax

        xor     ax,ax                   ; Allocate descriptor
        mov     cx,1
        int     DPMI_INT
        mov     [word c32sel],ax
        mov     bx,ax
        mov     ax,DPMI_SETSEGBASE      ; Set segment base address
        mov     cx,[word c32base+MSW]
        mov     dx,[word c32base+LSW]
        int     DPMI_INT
        inc     ax                      ; Set segment limit
        mov     cx,[word c32limit+MSW]
        mov     dx,[word c32limit+LSW]
        int     DPMI_INT
        inc     ax                      ; Set descriptor access rights
        mov     cx,[word c32access]
        int     DPMI_INT

        inc     ax                      ; Create alias descriptor
        int     DPMI_INT
        mov     ds,ax
        mov     es,ax
        mov     ax,SELZERO
        mov     gs,ax
        ret

@@error:
        movzx   eax,ax
        mov     edx,[pmerrtab+eax*dword]
        mov     ah,DOS_DISPSTR
        int     DOSINT
        enderr
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
; 16-bit initialization code

init:
;        p8086
        mov     ax,DSEG
        mov     ds,ax
        call    initpm
;        p486
        jmp32   <[word c32sel]>,init32  ; Jump to the 32bit code segment
; [fold]  ]

        ends
; [fold]  ]

        end

; [fold]  19
