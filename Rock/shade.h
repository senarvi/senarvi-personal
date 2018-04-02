#ifndef _shade_h_
#define _shade_h_

#include "graphics.h"
#include "math.h"

typedef struct {
  cFixed screenx,screeny;
  cFixed depth;
  cFixed i;
} sedge;


//---------------------------------------------------------------------------
inline sedge operator +(sedge source1,sedge source2)
{
  sedge result;
  result.screenx.value = source1.screenx.value + source2.screenx.value;
  result.screeny.value = source1.screeny.value + source2.screeny.value;
  result.depth.value   = source1.depth.value   + source2.depth.value;
  result.i.value       = source1.i.value       + source2.i.value;
  return result;
}

//---------------------------------------------------------------------------
inline sedge operator -(sedge source1,sedge source2)
{
  sedge result;
  result.screenx.value = source1.screenx.value - source2.screenx.value;
  result.screeny.value = source1.screeny.value - source2.screeny.value;
  result.depth.value   = source1.depth.value   - source2.depth.value;
  result.i.value       = source1.i.value       - source2.i.value;
  return result;
}

//---------------------------------------------------------------------------
inline sedge operator *(sedge source1,signed long source2)
{
  sedge result;
  result.screenx.value = fMul(source1.screenx.value,source2);
  result.screeny.value = fMul(source1.screeny.value,source2);
  result.depth.value   = fMul(source1.depth.value  ,source2);
  result.i.value       = fMul(source1.i.value      ,source2);
  return result;
}

//---------------------------------------------------------------------------
inline sedge operator /(sedge source1,signed long source2)
{
  sedge result;
  result.screenx.value = fDiv(source1.screenx.value,source2);
  result.screeny.value = fDiv(source1.screeny.value,source2);
  result.depth.value   = fDiv(source1.depth.value  ,source2);
  result.i.value       = fDiv(source1.i.value      ,source2);
  return result;
}

#endif /* !_shade_h_ */

