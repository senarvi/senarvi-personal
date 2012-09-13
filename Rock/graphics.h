#ifndef _video_h_
#define _video_h_

//#include <sys\farptr.h>
#include "types.h"

#define GRAPH_MAX_X 320
#define GRAPH_MAX_Y 200
//#define GRAPH_MEM 0x000a0000

//---------------------------------------------------------------------------
typedef struct {
  dword x,y;
} spixel;

typedef struct {
  spixel a,b;
} ssquare;

typedef struct {
  spixel a,b,c;
} sspolygon;

//---------------------------------------------------------------------------
extern unsigned char *videobuffer;
extern unsigned char *GRAPH_MEM;

extern void setpalette();

#ifdef __cplusplus
extern "C" {
#endif

extern void setgraph(),settext();

#ifdef __cplusplus
}
#endif


//---------------------------------------------------------------------------
inline spixel operator +(spixel source1,spixel source2)
{
  spixel result;
  result.x = source1.x + source2.x;
  result.y = source1.y + source2.y;
  return result;
}

//---------------------------------------------------------------------------
inline spixel operator -(spixel source1,spixel source2)
{
  spixel result;
  result.x = source1.x - source2.x;
  result.y = source1.y - source2.y;
  return result;
}

//---------------------------------------------------------------------------
inline spixel operator <<(spixel source1,signed long source2)
{
  spixel result;
  result.x = source1.x << source2;
  result.y = source1.y << source2;
  return result;
}

//---------------------------------------------------------------------------
inline spixel operator >>(spixel source1,signed long source2)
{
  spixel result;
  result.x = source1.x >> source2;
  result.y = source1.y >> source2;
  return result;
}

//---------------------------------------------------------------------------
inline void clearbuffer()
{
  signed long count;

  for (count = GRAPH_MAX_X * GRAPH_MAX_Y - 4; count >= 0; count -= 4)
    *(unsigned long *)(videobuffer + count) = 0x00000000;
}

//---------------------------------------------------------------------------
inline void copybuffer()
{
  signed long count;

  for (count = GRAPH_MAX_X * GRAPH_MAX_Y - 4; count >= 0; count -= 4)
//    _farnspokel(GRAPH_MEM + count,*(unsigned long *)(videobuffer + count));
    *(unsigned long *)(GRAPH_MEM + count) =
      *(unsigned long *)(videobuffer + count);
}

#endif /* !_video_h_ */

