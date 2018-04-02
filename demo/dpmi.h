#ifndef _DPMI_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include "system.h"


/*
 * CONSTANTS
 */

// Local Descriptor Table (LDT) management services

#define DPMI_ALLOCATE_DESCRIPTORS       0x0000
#define DPMI_FREE_DESCRIPTOR            0x0001
#define DPMI_MAP_RM_SEGMENT_TO_DESCRIPTOR 0x0002
#define DPMI_GET_SELECTOR_INCREMENT     0x0003
#define DPMI_GET_SEGMENT_BASE           0x0006
#define DPMI_SET_SEGMENT_BASE           0x0007
#define DPMI_SET_SEGMENT_LIMIT          0x0008
#define DPMI_SET_DESCRIPTOR_ACCESS_RIGHTS 0x0009
#define DPMI_CREATE_ALIAS_DESCRIPTOR    0x000a
#define DPMI_GET_DESCRIPTOR             0x000b
#define DPMI_SET_DESCRIPTOR             0x000c
#define DPMI_ALLOCATE_DESCRIPTOR        0x000d

// DOS memory management services

#define DPMI_ALLOCATE_DOS_MEMORY_BLOCK  0x0100
#define DPMI_FREE_DOS_MEMORY_BLOCK      0x0101
#define DPMI_RESIZE_DOS_MEMORY_BLOCK    0x0102

// Interrupt services

#define DPMI_GET_RM_INTERRUPT_VECTOR    0x0200
#define DPMI_SET_RM_INTERRUPT_VECTOR    0x0201
#define DPMI_GET_EXCEPTION_HANDLER      0x0202
#define DPMI_SET_EXCEPTION_HANDLER      0x0203
#define DPMI_GET_PM_INTERRUPT_VECTOR    0x0204
#define DPMI_SET_PM_INTERRUPT_VECTOR    0x0205

// Translation services

#define DPMI_SIMULATE_RM_INTERRUPT      0x0300
#define DPMI_CALL_RM_PROCEDURE_WITH_FAR_RETURN_FRAME 0x0301
#define DPMI_CALL_RM_PROCEDURE_WITH_IRET_FRAME 0x0302
#define DPMI_ALLOCATE_RM_CALLBACK_ADDRESS 0x0303
#define DPMI_FREE_RM_CALLBACK_ADDRESS    0x0304

// DPMI version

#define DPMI_GET_VERSION                0x0400

// Memory management services

#define DPMI_GET_FREE_MEMORY_INFORMATION 0x0500
#define DPMI_ALLOCATE_MEMORY_BLOCK      0x0501
#define DPMI_FREE_MEMORY_BLOCK          0x0502
#define DPMI_RESIZE_MEMORY_BLOCK        0x0503

// Page locking services

#define DPMI_LOCK_LINEAR_REGION         0x0600
#define DPMI_UNLOCK_LINEAR_REGION       0x0601
#define DPMI_GET_PAGE_SIZE              0x0604

// Demand paging performance tuning services

#define DPMI_MARK_PAGE_AS_DEMAND_PAGING_CANDIDATE 0x0702
#define DPMI_DISCARD_PAGE_CONTENTS      0x0703

// Physical address mapping

#define DPMI_PHYSICAL_ADDRESS_MAPPING   0x0800
#define DPMI_FREE_PHYSICAL_ADDRESS_MAPPING 0x0801

// Virtual interrupt state functions

#define DPMI_GET_AND_DISABLE_IF_STATE   0x0900
#define DPMI_GET_AND_ENABLE_IF_STATE    0x0901
#define DPMI_GET_IF_STATE               0x0902

// Vendor specific extensions

#define DOS4G_GET_API_ENTRY_POINT       0x0a00

// Coprocessor status

#define DPMI_GET_COPROCESSOR_STATUS     0x0e00
#define DPMI_SET_COPROCESSOR_EMULATION  0x0e01


/*
 * STRUCTURES
 */

// Free memory information structure

typedef struct dpmi_meminfodef {
    udword available_memory;
    udword available_unlocked_pages;
    udword available_locked_pages;
    udword linear_space_pages;
    udword unlocked_pages;
    udword available_physical_pages;
    udword total_physical_pages;
    udword free_linear_space;
    udword pages_in_paging_file;
    udword reserved[ 3 ];
} dpmi_meminfo;


/*
 * EXTERNALS
 */

extern intr *dpmi_getpmvect( ubyte intnum );
extern void dpmi_setpmvect( ubyte intnum, intr *handler() );
extern void intdpmi( uword function );


/*
 * INLINE FUNCTIONS
 */

#pragma aux dpmi_getpmvect = \
    "mov ax,0x0204", \
    "int 31h", \
    parm [bl] value [cx edx] modify [ax bl cx edx];

#pragma aux dpmi_setpmvect = \
    "mov ax,0x0205", \
    "int 31h", \
     parm [bl cx edx] modify [ax bl cx edx];

#pragma aux intdpmi = \
    "int 31h", \
    parm [ax] modify [ax]


#define _DPMI_H_INCLUDED
#ifdef __cplusplus
};
#endif
#endif
