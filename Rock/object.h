#ifndef _object_h_
#define _object_h_

#include "vector.h"
#include "graphics.h"
#include "rock.h"

//---------------------------------------------------------------------------
class object {
public:
  void init(signed long obj_numvertices,signed long obj_numfaces,
            svertex *obj_vertextab,sface *obj_facetab,
            svector obj_location,sangle obj_angle);
  svector getvnormal(signed long vertex);
  void rotate();
  void draw();

//---------------------------------------------------------------------------
  inline void show()
  {
    signed long xcount,ycount;
    ssquare     temparea;

    temparea = area;
    if (prevarea.a.x < area.a.x) area.a.x = prevarea.a.x;
    if (prevarea.a.y < area.a.y) area.a.y = prevarea.a.y;
    if (prevarea.b.x > area.b.x) area.b.x = prevarea.b.x;
    if (prevarea.b.y > area.b.y) area.b.y = prevarea.b.y;
    prevarea = temparea;

    for (ycount = area.a.y * GRAPH_MAX_X; ycount <= area.b.y * GRAPH_MAX_X;
         ycount += GRAPH_MAX_X)
      for (xcount = area.a.x; xcount <= area.b.x; xcount += 4) {
//        _farnspokel(GRAPH_MEM + ycount + xcount,
//                    *(unsigned long *)(videobuffer + ycount + xcount));
        *(unsigned long *)(GRAPH_MEM + ycount + xcount) =
          *(unsigned long *)(videobuffer + ycount + xcount);
        *(unsigned long *)(videobuffer + ycount + xcount)=0x7f7f7f7f;
      }
  }

  sangle angle;

//---------------------------------------------------------------------------
private:
  signed long numvertices;
  signed long numfaces;
  svertex     *vertextab,*rotatetab;
  sface       *facetab;
  svector     *fnormaltab,*vnormaltab;

  svector     location;

  ssquare     area,prevarea;
};

extern object rock;

#endif /* !_object_h_ */

