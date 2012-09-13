#ifndef _math_h_
#define _math_h_

//#include <Integer.h>
#include "types.h"
#include "sin.h"

#define FRACBITS 16
#define V_FRACBITS 16
#define MAP_FRACBITS 16
#define SIN_FRACBITS 16

/*inline fixed sin(unsigned char a) { return sintab[a]; }
inline fixed cos(unsigned char a) { return costab[a]; }*/

//---------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

extern fixed fMul(fixed multiplicand,fixed multiplier);
extern fixed fDiv(fixed dividend,fixed divisor);

extern unsigned long iSqrt(unsigned long iSquare);

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
class cFixed {
protected:

public:
  fixed value;

  cFixed();
  cFixed(dword);
  cFixed(const cFixed&);

/*  cFixed operator + (const cFixed&);
  cFixed operator - (const cFixed&);
  cFixed operator + (const cFixed&,const cFixed&);
  cFixed operator - (const cFixed&,const cFixed&);
  cFixed operator * (const cFixed&,const cFixed&);
  cFixed operator / (const cFixed&,const cFixed&);*/
};

inline cFixed::cFixed() :value(0) {}
inline cFixed::cFixed(dword x) :value(x << FRACBITS) {}
inline cFixed::cFixed(const cFixed& x) :value(x.value) {}

inline cFixed operator + (const cFixed& x)
{ cFixed r; r.value = x.value; return r; }

inline cFixed operator - (const cFixed& x)
{ cFixed r; r.value = -x.value; return r; }

inline cFixed operator + (const cFixed& x,const cFixed& y)
{ cFixed r; r.value = x.value + y.value; return r; }

inline cFixed operator - (const cFixed& x,const cFixed& y)
{ cFixed r; r.value = x.value - y.value; return r; }

inline cFixed operator * (const cFixed& x,const cFixed& y)
{
  cFixed r;
  asm ("pushl   %%edx\n"

       "movl    %1,%%eax\n"
       "imull   %2\n"
       "shrdl   $16,%%edx,%%eax\n"
       "movl    %%eax,%0\n"

       "popl    %%edx"

       : "=a" (r.value)
       : "b" (x.value), "c" (y.value));
  return r;
}

inline cFixed operator / (const cFixed& x,const cFixed& y)
{
  cFixed r;
  asm ("pushl   %%edx\n"

       "xorl    %%eax,%%eax\n"
       "movl    %1,%%edx\n"
       "shrdl   $32-16,%%edx,%%eax\n"
       "sar     $32-16,%%edx\n"
       "idivl   %2\n"
       "movl    %%eax,%0\n"

       "popl    %%edx"

       : "=a" (r.value)
       : "b" (x.value), "c" (y.value));
  return r;
}

inline cFixed xsin(unsigned char x)
{ cFixed r; r.value = sintab[x]; return r; }

inline cFixed xcos(unsigned char x)
{ cFixed r; r.value = costab[x]; return r; }

#endif /* !_math_h_ */

