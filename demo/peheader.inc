;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
; Constants

PE_MACHINE_UNKNOWN      =       0000h           ; Unknown machine
PE_MACHINE_I386         =       014ch           ; Intel 386+ and compatible
PE_MACHINE_R3000B       =       0160h           ; MIPS (big endian)
PE_MACHINE_R3000        =       0162h           ; MIPS (little endian)
PE_MACHINE_R4000        =       0166h           ; MIPS (little endian)
PE_MACHINE_R10000       =       0168h           ; MIPS (little endian)
PE_MACHINE_ALPHA        =       0184h           ; Alpha AXP
PE_MACHINE_POWERPC      =       01F0h           ; Power PC (little endian)

PE_NO_RELOC_INFO        =       0001h           ; Relocation info stripped
PE_EXECUTABLE_IMAGE     =       0002h           ; File is executable
PE_NO_LINE_NUMBERS      =       0004h           ; Line nunbers stripped
PE_NO_LOCAL_SYMBOLS     =       0008h           ; Local symbols stripped
PE_BYTES_REVERSED_LO    =       0080h           ; Bytes of machine word reversed
PE_32BIT_MACHINE        =       0100h           ; 32-bit word machine
PE_DEBUG_STRIPPED       =       0200h           ; Debugging info stripped
PE_SYSTEM_FILE          =       1000h           ; System file
PE_DLL_FILE             =       2000h           ; File is a dll
PE_BYTES_REVERSED_HI    =       8000h           ; Bytes of machine word reversed
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
                struc   mz_header
signature       dw      ?               ; .EXE signature, "MZ"
bytesinlastpage dw      ?               ; Number of bytes in last 512-byte page
pages           dw      ?               ; Number of 512-byte page
relocentries    dw      ?               ; Number of relocation entries
headersize      dw      ?               ; Header size in paragraphs
minalloc        dw      ?               ; Minimum paragraphs to allocation
maxalloc        dw      ?               ; Maximum paragraphs to allocation
initialss       dw      ?               ; Relative to start of executable
initialsp       dw      ?
checksum        dw      ?               ; One's complement of sum of all words
initialip       dw      ?
initialcs       dw      ?               ; Relative to start of executable
reloctable      dw      ?               ; Offset of relocation table
overlay         dw      ?               ; Normally 0 = main program
                dw      2 dup (?)
behaviorbits    dw      ?
                dw      13 dup (?)      ; Reserved for additional behavior info
newheader       dd      ?               ; Offset of new executable header
                ends
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
                struc   pe_header
signature       dd      ?               ; "PE"
machinetype     dw      ?
sections        dw      ?
datestamp       dd      ?               ; Time/date stamp
symboltable     dd      ?
symbolentries   dd      ?
optheadersize   dw      ?               ; Size of optional header
attributes      dw      ?
                ends
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
                struc   pe_optheader
signature       dw      ?
linkerversion   db      ?
linkerrevision  db      ?
codesize        dd      ?               ; Code sections
datasize        dd      ?               ; Initialized data sections
udatasize       dd      ?               ; Uninitialized data sections
entrypoint      dd      ?               ; Main program entry point
codebase        dd      ?               ; Code section load address
database        dd      ?               ; Data section load address
imagebase       dd      ?               ; Preferred load address
sectionalign    dd      ?               ; Section alignment in memory
filealign       dd      ?               ; Section alignment in file
osver           ver     ?               ; Required os version
imagever        ver     ?
subsystemver    ver     ?
                dd      ?               ; Reserved
imagesize       dd      ?               ; Includes all headers
headersize      dd      ?               ; MZ/PE header and section table
filechecksum    dd      ?               ; Drivers and services
subsystem       dw      ?               ; Required subsystem
dllflags        dw      ?               ; Obsolete
stackreserve    dd      ?               ; Size of stack space to reserve
stackcommit     dd      ?               ; Size of stack space to commit
heapreserve     dd      ?               ; Size of local heap space to reserve
heapcommit      dd      ?               ; Size of local heap space to commit
loaderflags     dd      ?               ; Obsolete
datadirentries  dd      ?               ; Data directory
exporttable     stream  ?
importtable     stream  ?
resourcetable   stream  ?
exceptiontable  stream  ?
securitytable   stream  ?
basereloctable  stream  ?               ; Base relocation table
debugdata       stream  ?
copyright       stream  ?               ; Copyright string
globalptrreg    stream  ?               ; Global pointer register
tlstable        stream  ?               ; Thread local storage table
loadconfigtable stream  ?               ; Load configuration table
                stream  5 dup (?)       ; Reserved
                ends
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
                struc   section_header
sectionname     db      8 dup (?)
loadsize        dd      ?               ; Size in memory
loadaddress     dd      ?               ; Offset in memory
rawsize         dd      ?               ; Size in file
rawaddress      dd      ?               ; Offset in file
reloctable      dd      ?
linenumtable    dd      ?
relocentries    dw      ?
linenumentries  dw      ?
attributes      dd      ?
                ends
; [fold]  ]

;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
; [fold]  [
                struc   export_dir
exportflags     dd      ?
datestamp       dd      ?               ; Time/date stamp
exportversion   ver     ?
filename        dd      ?
ordinalbase     dd      ?               ; Starting ordinal number (bias)
addressentries  dd      ?
nameentries     dd      ?
addresstable    dd      ?
nametable       dd      ?
ordinaltable    dd      ?
                ends
; [fold]  ]

; [fold]  6
