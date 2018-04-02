;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

DPMI_GETINTVEC          =       0204h
DPMI_SETINTVEC          =       0205h
DPMI_RMINT              =       0300h
DPMI_RMCALLFAR          =       0301h
DPMI_RMCALLINT          =       0302h
DPMI_GETVERSION         =       0400h
DPMI_GETFREEMEM         =       0500h
DPMI_ALLOCMEM           =       0501h
DPMI_FREEMEM            =       0502h
DPMI_RESIZEBLOCK        =       0503h

DPMI_INT                =       31h

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
                                struc   dpmi_meminfo
available_memory                dd      ?
available_unlocked_pages        dd      ?
available_locked_pages          dd      ?
linear_space_pages              dd      ?
unlocked_pages                  dd      ?
available_physical_pages        dd      ?
total_physical_pages            dd      ?
free_linear_space               dd      ?
pages_in_paging_file            dd      ?
reserved                        dd      3 dup (?)
                                ends

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
        struc   dpmi_regs
        label   edi dword
di      dw      ?
        dw      ?
        label   esi dword
si      dw      ?
        dw      ?
        label   ebp dword
bp      dw      ?
        dw      ?
        dd      ?

        label   ebx dword
        label   bx word
bl      db      ?
bh      db      ?
        dw      ?
        label   edx dword
        label   dx word
dl      db      ?
dh      db      ?
        dw      ?
        label   ecx dword
        label   cx word
cl      db      ?
ch      db      ?
        dw      ?
        label   eax dword
        label   ax word
al      db      ?
ah      db      ?
        dw      ?

flags   dw      ?
es      dw      ?
ds      dw      ?
fs      dw      ?
gs      dw      ?
ip      dw      ?
cs      dw      ?
sp      dw      ?
ss      dw      ?
        ends

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
        global  rmregs:dpmi_regs

