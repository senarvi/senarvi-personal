
DMA_PAGE        =       0*dword
DMA_ADDRESS     =       1*dword
DMA_LENGTH      =       2*dword
DMA_MASK        =       3*dword
DMA_MODE        =       4*dword
DMA_RESET       =       5*dword
DMA_NEXT        =       6*dword

;----------------------------------------------------------------------------
DMA_MASK_CLEAR  = 0 shl 2
DMA_MASK_SET    = 1 shl 2

DMA_MODE_DEMAND         = 0 shl 6
DMA_MODE_SIGNAL         = 1 shl 6       ; Used most of the time
DMA_MODE_BLOCK          = 2 shl 6
DMA_MODE_CASCADE        = 3 shl 6
DMA_MODE_AUTOINIT       = 1 shl 4       ; Auto-initialize
DMA_MODE_SINGLE         = 0 shl 4       ; Single-cyckle
DMA_MODE_VERIFY         = 0 shl 2
DMA_MODE_WRITE          = 1 shl 2       ; From a device to memory
DMA_MODE_READ           = 2 shl 2       ; From memory to whomever polls it

