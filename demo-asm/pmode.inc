
PM_RAW          =       0
PM_XMS          =       1
PM_VCPI         =       2
PM_DPMI         =       3

CPL             =       0

DTYPE           =       0001111110010000b
DPAGE           =       1 shl 15
DBYTE           =       0 shl 15
DUSE32          =       1 shl 14
DUSE16          =       0 shl 14
DDPL            =       CPL shl 5
DCODE           =       1 shl 3 + 0 shl 2 + 1 shl 1
DDATA           =       0 shl 3
DEXPDOWN        =       1 shl 2
DEXPUP          =       0 shl 2
DRW             =       1 shl 1
DREAD           =       0 shl 1
DACC            =       1 shl 0
DNOTACC         =       0 shl 0

DPMI_ALLOCDESC  =       0000h
DPMI_FREEDESC   =       0001h
DPMI_GETSELINC  =       0003h
DPMI_GETSEGBASE =       0006h
DPMI_SETSEGBASE =       0007h
DPMI_SETSEGLIMIT =      0008h
DPMI_SETDESCACC =       0009h
DPMI_CREATEALIAS =      000ah
DPMI_GETDESC    =       000bh
DPMI_SETDESC    =       000ch
DPMI_GETINTVEC  =       0204h
DPMI_SETINTVEC  =       0205h
DPMI_RMINT      =       0300h
DPMI_RMCALLFAR  =       0301h
DPMI_RMCALLINT  =       0302h
DPMI_GETVERSION =       0400h
DPMI_GETFREEMEM =       0500h
DPMI_ALLOCMEM   =       0501h
DPMI_FREEMEM    =       0502h
DPMI_RESIZEMEM  =       0503h

DPMI_INT        =       31h

MEMFREE         =       0

SELCODE         =       08h             ; PMCSEG selector
SELZERO         =       10h             ; selector of entire memory space
SELCALLBACKDS   =       18h             ; callback DS selector
SELREAL         =       20h             ; real mode attributes selector
SELVCPITSS      =       28h             ; TSS selector for VCPI
SELVCPICODE     =       30h             ; VCPI call code selector
SYSSELECTORS    =       9               ; number of system selectors in GDT

;----------------------------------------------------------------------------
        global  _pm_selectors:word,_pm_pagetables:byte
        global  _pm_callbacks:byte

        global  _pm_mode:byte

        global  _pm_pmstacklen:word,_pm_pmstacks:byte
        global  _pm_rmstacklen:word,_pm_rmstacks:byte

        global  pmstackbase:dword,pmstacktop:dword
        global  rmstackbase:word,rmstacktop:word


        global  _pm_info:far,_pm_init:far


        global  rmregs:dataptr

        global  rm_eax:dword,rm_ecx:dword,rm_edx:dword,rm_ebx:dword
        global  rm_esp:dword,rm_ebp:dword,rm_esi:dword,rm_edi:dword

        global  rm_ax:word,rm_cx:word,rm_dx:word,rm_bx:word
        global  rm_sp:word,rm_bp:word,rm_si:word,rm_di:word

        global  rm_ah:byte,rm_al:byte,rm_ch:byte,rm_cl:byte
        global  rm_dh:byte,rm_dl:byte,rm_bh:byte,rm_bl:byte

