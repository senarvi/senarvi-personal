                .file   "math.s"

//                .include "asm.h"
FRACBITS	=	16

                .global _fDiv,_fMul
                .global _iSqrt

                .text


#----------------------------------------------------------------------------
# Fixed point multiply
#
# signed long fMul(signed long multiplicand,signed long multiplier)
#----------------------------------------------------------------------------

                .align  4
_fMul:
                pushl   %edx

                movl    2*4(%esp),%eax
                imull   3*4(%esp)
                shrdl   $FRACBITS,%edx,%eax

                popl    %edx
                ret

#----------------------------------------------------------------------------
# Fixed point divide
#
# signed long fDiv(signed long dividend,signed long divisor)
#----------------------------------------------------------------------------

                .align  4
_fDiv:
                pushl   %edx

                xorl    %eax,%eax
                movl    2*4(%esp),%edx
                shrdl   $32-FRACBITS,%edx,%eax
                sarl    $32-FRACBITS,%edx
                idivl   3*4(%esp)

                popl    %edx
                ret

#----------------------------------------------------------------------------
# Square root
# by Mike Dailly
#
# unsigned long iSqrt(unsigned long iSquare)
#----------------------------------------------------------------------------

                .align  4
_iSqrt:
                pushl   %ecx
                pushl   %edx
                pushl   %ebx
                pushl   %edi
                movl    5*4(%esp),%edx

                xorl    %eax,%eax
                movl    $1<<30,%edi
                movl    $16,%ecx
sqrtl0:
                movl    %eax,%ebx
                addl    %edi,%ebx
                shrl    $1,%eax
                cmpl    %edx,%ebx
                ja      sqrtf0
                subl    %ebx,%edx
                addl    %edi,%eax
sqrtf0:
                shrl    $2,%edi
                decl    %ecx
                jne     sqrtl0

                popl    %edi
                popl    %ebx
                popl    %edx
                popl    %ecx
                ret

