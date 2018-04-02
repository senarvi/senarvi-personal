
                include declare.mac

                decstart


                public  rotate

                include rotate.mac
                include vector.mac

                extrn   vertextaboff:dword,lastvertexoff:dword
                include memory.inc
                extrn   objoffset:svect,objangle:svect


                deccode32

;----------------------------------------------------------------------------
rotate:
                pushad
                mov     esi,[vertextaboff]
                mov     edi,[lastvertexoff]
                align   4
@@rotatel0:
                rotv    esi[edi],[objangle],fs:_rotatetab[edi]
                vadd    fs:_rotatetab[edi],[objoffset],fs:_rotatetab[edi]
                sub     edi,size svect
                jge     @@rotatel0
                popad
                ret

                endcode32
                end
