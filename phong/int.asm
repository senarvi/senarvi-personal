 
                include declare.mac


                decstart


                include misc.inc
                include kb.inc
                extrn   pause:byte
                extrn   objangle:svect
                extrn   time:dword

                public  okbint,opitint
                public  kbint,pitint

                extrn   exit:near
                include system.mac


                deccode32

;----------------------------------------------------------------------------
kbint:
                cli
                push    eax
                in      al,KBPORT
                cmp     al,KBESC
                je      exit
                cmp     al,KBP
                sete    [pause]
                endint
                pop     eax
                sti
                iretd

;----------------------------------------------------------------------------
pitint:
                cli
                push    eax
                inc     [objangle.z]
                dec     [objangle.y]
                dec     [objangle.x]
                inc     [time]
                endint
                pop     eax
                sti
                iretd

clock_ticks     dd      0
;----------------------------------------------------------------------------
okbint          df      ?
opitint         df      ?

                endcode32
                end
