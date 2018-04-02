                .file   "vector.s"

//                .include "asm.h"
FRACBITS	=	16

                .global _faceshown

                .text


#----------------------------------------------------------------------------
# Face shown in screen
#
# Out:
#   1 - face is shown
#   0 - face is hidden

_faceshown:
                pushl   %edx

                movl    (1+4+0)*4(%esp),%eax
                subl    (1+0+0)*4(%esp),%eax
                movl    (1+2+1)*4(%esp),%edx
                subl    (1+0+1)*4(%esp),%edx
                imull   %edx
                shrdl   $FRACBITS,%edx,%eax
                movl    %eax,%ecx
                movl    (1+4+1)*4(%esp),%eax
                subl    (1+0+1)*4(%esp),%eax
                movl    (1+2+0)*4(%esp),%edx
                subl    (1+0+0)*4(%esp),%edx
                imull   %edx
                shrdl   $FRACBITS,%edx,%eax
                cmpl    %eax,%ecx
                setgb   %al
                xorb    %ah,%ah

                popl    %edx
                ret
