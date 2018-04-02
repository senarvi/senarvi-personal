
        include "declare.mac"


        include "math.mac"

        include "bumpmap.inc"
        include "allocate.inc"
        include "memory.inc"
        include "data.inc"


        BUMP_OFFSET = 1*GRAPHWIDTH                      ; First pixel included
        BUMP_END    = (GRAPHHEIGHT-1)*GRAPHWIDTH        ; Last pixel will not
                                                        ; be included
        WAVE_LENGTH    = 8                              ; 1 / length_of_wave
;        WAVE_AMPLITUDE = 7                              ; Amplitude of wave


;############################################################################
; Code
;############################################################################
; [fold]  [

        deccode32

;----------------------------------------------------------------------------
; [fold]  [
        proc    bumpmap c near
        mov     ecx,(GRAPHHEIGHT-1)*GRAPHWIDTH-2
        xor     eax,eax
@@loop:
        xor     edx,edx
        mov     al,[stc_heightmap+ecx-1]
        sub     al,[stc_heightmap+ecx+1]
        mov     dl,[stc_heightmap+ecx-GRAPHWIDTH]
        sub     dl,[stc_heightmap+ecx+GRAPHWIDTH]
        add     dl,256/2
        cmul    edx,256
        mov     al,[envmap+edx+256/2+eax]
        mov     [fs:_screenbuf+ecx],al
        dec     ecx
        cmp     ecx,BUMP_OFFSET
        jge     @@loop
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
        proc    calc_bumpmap c near
        row = GRAPHWIDTH*dword
        col = dword

        mov     ecx,BUMP_END-4*byte-BUMP_OFFSET

@@loop:
        macro   getnormals source,temp,dest
        current = fs:_heightmap+(BUMP_OFFSET+ecx+source)*dword

        mov     temp,[current-3*col]    ; x2
        mov     eax,[current-col]       ; x / x2
        sub     temp,eax                ; x2
        sub     eax,[current+col]       ; x
        mov     edx,[current-row]       ; y
        sub     edx,[current+row]       ; y
        add     edx,256/2
        cmul    edx,256
        mov     dest,[envmap+edx+256/2+eax]
        endm

        macro   getnormals2 source,temp,dest
        current = fs:_heightmap+(BUMP_OFFSET+ecx+source)*dword

        mov     edx,[current-row]       ; y2
        sub     edx,[current+row]       ; y2
        add     edx,256/2
        cmul    edx,256
        mov     dest,[envmap+edx+256/2+temp]
        endm

        getnormals 3,esi,bh
        add     bh,[stc_heightmap+BUMP_OFFSET+ecx+3]
        getnormals 2,edi,bl
        add     bl,[stc_heightmap+BUMP_OFFSET+ecx+2]
        shl     ebx,16
        getnormals2 1,esi,bh
        add     bh,[stc_heightmap+BUMP_OFFSET+ecx+1]
        getnormals2 0,edi,bl
        add     bl,[stc_heightmap+BUMP_OFFSET+ecx]


        mov     [gs:_VGA_graphics_buffer+BUMP_OFFSET+ecx],ebx
        sub     ecx,4*byte
        jge     @@loop
        ret
        endp
; [fold]  ]

;----------------------------------------------------------------------------
; [fold]  [
        proc    make_wave c near
        mov     ecx,GRAPHHEIGHT*GRAPHWIDTH-byte
@@loop:
        mov     eax,[fs:_tunneldepth+ecx*dword]
        cmul    eax,WAVE_LENGTH
        add     eax,[wave_add]
        and     eax,SIN_NUM-1
        mov     eax,[costable+eax*dword]
        cdq
        imul    [wave_amplitude]

        sar     eax,SIN_FRACBITS;-WAVE_AMPLITUDE
        mov     [fs:_heightmap+ecx*dword],eax
        dec     ecx
        jge     @@loop
        ret
        endp
; [fold]  ]

        ends
; [fold]  ]

;############################################################################
; Data
;############################################################################
; [fold]  [

        decdata

;wave_add        dd      0
wave_amplitude  dd      150
wave_fade       dd      50

        ends
; [fold]  ]

        end

; [fold]  5
