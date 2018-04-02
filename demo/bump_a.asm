        .intel_syntax

        %title  "Copyright (c) Seppo Enarvi  1996,1997"
        %subttl "Bumpmap effects"

        .include "declare.mac"


        .include "system.mac"
        .include "math.mac"

        .include "video.inc"
        .include "bumpmap.inc"
        .include "vector.inc"
        .include "math.inc"
        .include "tunnel.inc"
        .include "memory.inc"
        .include "data.inc"


        BUMP_OFFSET = 1*GR_X                            # First pixel included
        BUMP_END    = (GR_Y-1)*GR_X                     # Last pixel not
                                                        #  included
        WAVE_LENGTH    = 8                              # 1 / length_of_wave
#        WAVE_AMPLITUDE = 7                             # Amplitude of wave


#�����������������������������������������������������������������������������
# Code
#�����������������������������������������������������������������������������
# [fold]  [

        .text

#����������������������������������������������������������������������������
# [fold]  [
        proc    bumpmap_
        pushad

        col = dword
        row = GR_X*col

        macro   limit source
        local   @@f0,@@f1
        cmp     source,-(256/2)
        jg      @@f0
        mov     source,-(256/2)
@@f0:
        cmp     source,256/2
        jl      @@f1
        mov     source,256/2
@@f1:
        endm

        macro   getnormals source,temp,dest
        current = heightmap+(BUMP_OFFSET+ecx+source)*dword

        mov     temp,[current-3*col]    # x2
        mov     eax,[current-col]       # x / x2
        sub     temp,eax                # x2
        add     temp,[bump_light.a]
        limit   temp

        sub     eax,[current+col]       # x
        add     eax,[bump_light.a]
        limit   eax

        mov     edx,[current-row]       # y
        sub     edx,[current+row]       # y
        add     edx,[bump_light.b]
        limit   edx

#        mov     eax,[current-col]
#        add     eax,[current-col-row]
#        add     eax,[current-col+row]
#
#        mov     ebp,[current+col]
#        add     ebp,[current+col-row]
#        add     ebp,[current+col+row]
#
#        cdiv    eax,2
#        cdiv    ebp,2
#        sub     eax,ebp
#
#        mov     edx,[current-row]
#        add     edx,[current-row-col]
#        add     edx,[current-row+col]
#
#        mov     ebp,[current+row]
#        add     ebp,[current+row-col]
#        add     ebp,[current+row+col]
#
#        cdiv    edx,2
#        cdiv    ebp,2
#        sub     edx,ebp

        cmul    edx,256
        mov     dest,[envmap+(256/2)*256+edx+256/2+eax]
        endm

        macro   getnormals2 source,temp,dest
        current = heightmap+(BUMP_OFFSET+ecx+source)*dword

        mov     edx,[current-row]       # y2
        sub     edx,[current+row]       # y2
        add     edx,[bump_light.b]
        limit   edx

        cmul    edx,256
        mov     dest,[envmap+(256/2)*256+edx+256/2+temp]
        endm


        mov     ecx,BUMP_END-4-BUMP_OFFSET
@@loop:
        getnormals 3,esi,bh
        getnormals 2,edi,bl
        shl     ebx,16
        getnormals2 1,esi,bh
        getnormals2 0,edi,bl
#        getnormals 1,esi,bh
#        getnormals 0,ebp,bl

        mov     edi,[destbufoff]
        mov     [edi+BUMP_OFFSET+ecx],ebx
        sub     ecx,4*byte
        jge     @@loop

        popad
        ret
        endp
# [fold]  ]

#����������������������������������������������������������������������������
# [fold]  [
        proc    makewave_
        pushad

        mov     ecx,(GR_SIZE-1)*dword
@@loop:
        mov     ebp,[tunneldepth + ecx]

        # Length of wave
        mov     eax,ebp
        cdq
        imul    ebp
        cdiv    eax,16
#        cmul    eax,WAVE_LENGTH

        add     eax,[wave_add]
        and     eax,SIN_NUM-1
        mov     eax,[costable + eax*dword]

        # Height of wave
        mov     ebx,350
        cmul    ebp,2
        sub     ebx,ebp
        cdq
        imul    ebx

        sar     eax,SIN_FRACBITS#-WAVE_AMPLITUDE
        mov     [heightmap + ecx],eax
        sub     ecx,dword
        jge     @@loop

        popad
        ret
        endp
# [fold]  ]

        ends
# [fold]  ]

#�����������������������������������������������������������������������������
# Data
#�����������������������������������������������������������������������������
# [fold]  [

                _data

#����������������������������������������������������������������������������
wave_add        dd      0
wave_amplitude  dd      150
wave_fade       dd      50

                ends
# [fold]  ]

#�����������������������������������������������������������������������������
# Uninitialized data
#�����������������������������������������������������������������������������
# [fold]  [

                _udata

heightmap       dd      GR_SIZE dup (?)

                label   bump_light sangle
                sangle  ?

                ends
# [fold]  ]

        end

# [fold]  5
