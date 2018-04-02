#include <stdlib.h>
//#include <sys\farptr.h>
#include "shade.h"
#include "object.h"
#include "graphics.h"
#include "vector.h"

sedge *ledgebuffer = (sedge *)malloc(GRAPH_MAX_Y * sizeof(sedge)),
      *redgebuffer = (sedge *)malloc(GRAPH_MAX_Y * sizeof(sedge));

ssquare polyarea;


//---------------------------------------------------------------------------
void fillpoly(ssquare *polyarea)
{
  dword  iXCount,iYCount;
  sedge  fEdgeDelta,fEdgeCount,fEdgeAdd;
  cFixed half;

  half.value = ((0x10 / 2) << (FRACBITS - 4));

  for (iYCount =  polyarea->a.y >> FRACBITS;
       iYCount <= polyarea->b.y >> FRACBITS; iYCount++) {
    fEdgeDelta = redgebuffer[iYCount] - ledgebuffer[iYCount];

    if (fEdgeDelta.screenx.value != 0 << FRACBITS) {
      fEdgeAdd.depth.value = 1 << FRACBITS;
      fEdgeAdd.i.value     = 1 << FRACBITS;
//      fEdgeAdd.depth = fEdgeDelta.depth / fEdgeDelta.screenx;
//      fEdgeAdd.i     = fEdgeDelta.i     / fEdgeDelta.screenx;
    }
    else {
      fEdgeAdd.depth.value = 0 << FRACBITS;
      fEdgeAdd.i.value     = 0 << FRACBITS;
    }

    if (fEdgeAdd.depth.value < 0 << FRACBITS)
         fEdgeCount.depth = -half + ledgebuffer[iYCount].depth;
    else fEdgeCount.depth = +half + ledgebuffer[iYCount].depth;
    if (fEdgeAdd.i.value     < 0 << FRACBITS)
         fEdgeCount.i     = -half + ledgebuffer[iYCount].i;
    else fEdgeCount.i     = +half + ledgebuffer[iYCount].i;

    for (iXCount =  ledgebuffer[iYCount].screenx.value >> FRACBITS;
         iXCount <= redgebuffer[iYCount].screenx.value >> FRACBITS;
         iXCount++) {
      if (fEdgeCount.i.value > 0)
        videobuffer[iYCount * GRAPH_MAX_X + iXCount]
          = (fEdgeCount.i.value * 255) >> FRACBITS;
      fEdgeCount = fEdgeCount + fEdgeAdd;
    }
  }
}

//---------------------------------------------------------------------------
void getedge(sedge *edgebuffer,sedge *fEdge1,sedge *fEdge2)
{
  sedge  fEdgeDelta,fEdgeAdd,fEdgeCount;
  dword  iScanLine;
  cFixed half;

  half.value = ((0x10 / 2) << (FRACBITS - 4));

  fEdgeDelta = *fEdge2 - *fEdge1;
  if (fEdgeDelta.screeny.value != 0 << FRACBITS) {
    fEdgeAdd.screenx = fEdgeDelta.screenx / fEdgeDelta.screeny;
    fEdgeAdd.depth   = fEdgeDelta.depth   / fEdgeDelta.screeny;
    fEdgeAdd.i       = fEdgeDelta.i       / fEdgeDelta.screeny;
  }
  else {
    fEdgeAdd.screenx.value = 0 << FRACBITS;
    fEdgeAdd.depth.value   = 0 << FRACBITS;
    fEdgeAdd.i.value       = 0 << FRACBITS;
  }
  if (fEdgeAdd.screenx.value < 0 << FRACBITS)
       fEdgeCount.screenx = -half + fEdge1->screenx;
  else fEdgeCount.screenx = +half + fEdge1->screenx;
  if (fEdgeAdd.depth.value < 0 << FRACBITS)
       fEdgeCount.depth   = -half + fEdge1->depth;
  else fEdgeCount.depth   = +half + fEdge1->depth;
  if (fEdgeAdd.i.value < 0 << FRACBITS)
       fEdgeCount.i       = -half + fEdge1->i;
  else fEdgeCount.i       = +half + fEdge1->i;

  for (iScanLine =  fEdge1->screeny.value >> FRACBITS;
       iScanLine <= fEdge2->screeny.value >> FRACBITS; iScanLine++) {
    edgebuffer[iScanLine] = fEdgeCount;
    fEdgeCount.screenx = fEdgeCount.screenx + fEdgeAdd.screenx;
    fEdgeCount.depth   = fEdgeCount.depth   + fEdgeAdd.depth;
  }
}

//---------------------------------------------------------------------------
void drawpolygon(spolygon *fPoly,sspolygon *iScreen)
{
  sedge fEdgeA,fEdgeB,fEdgeC,
        *fEdge1,*fEdge2;
  sedge *edgebuffer;

  fEdgeA.screenx     = (cFixed)iScreen->a.x;
  fEdgeA.screeny     = (cFixed)iScreen->a.y;
  fEdgeA.depth.value = fPoly->a.z;
  fEdgeA.i.value     = fEdgeA.depth.value / 500;
  fEdgeB.screenx     = (cFixed)iScreen->b.x;
  fEdgeB.screeny     = (cFixed)iScreen->b.y;
  fEdgeB.depth.value = fPoly->b.z;
  fEdgeB.i.value     = fEdgeB.depth.value / 500;
  fEdgeC.screenx     = (cFixed)iScreen->c.x;
  fEdgeC.screeny     = (cFixed)iScreen->c.y;
  fEdgeC.depth.value = fPoly->c.z;
  fEdgeC.i.value     = fEdgeC.depth.value / 500;

  if (fEdgeA.screeny.value < fEdgeC.screeny.value) {
    fEdge1 = &fEdgeA;
    fEdge2 = &fEdgeC;
    edgebuffer = ledgebuffer;
  }
  else {
    fEdge1 = &fEdgeC;
    fEdge2 = &fEdgeA;
    edgebuffer = redgebuffer;
  }
  polyarea.a.x = fEdge1->screenx.value;
  polyarea.a.y = fEdge1->screeny.value;
  polyarea.b.x = fEdge2->screenx.value;
  polyarea.b.y = fEdge2->screeny.value;
  getedge(edgebuffer,fEdge1,fEdge2);

  if (fEdgeA.screeny.value < fEdgeB.screeny.value) {
    fEdge1 = &fEdgeA;
    fEdge2 = &fEdgeB;
    edgebuffer = redgebuffer;
  }
  else {
    fEdge1 = &fEdgeB;
    fEdge2 = &fEdgeA;
    edgebuffer = ledgebuffer;
  }
  if (fEdge1->screenx.value < polyarea.a.x)
    polyarea.a.x = fEdge1->screenx.value;
  if (fEdge1->screeny.value < polyarea.a.y)
    polyarea.a.y = fEdge1->screeny.value;
  if (fEdge2->screenx.value > polyarea.b.x)
    polyarea.b.x = fEdge2->screenx.value;
  if (fEdge2->screeny.value > polyarea.b.y)
    polyarea.b.y = fEdge2->screeny.value;
  getedge(edgebuffer,fEdge1,fEdge2);

  if (fEdgeB.screeny.value < fEdgeC.screeny.value) {
    fEdge1 = &fEdgeB;
    fEdge2 = &fEdgeC;
    edgebuffer = redgebuffer;
  }
  else {
    fEdge1 = &fEdgeC;
    fEdge2 = &fEdgeB;
    edgebuffer = ledgebuffer;
  }
  if (fEdge1->screenx.value < polyarea.a.x)
    polyarea.a.x = fEdge1->screenx.value;
  if (fEdge1->screeny.value < polyarea.a.y)
    polyarea.a.y = fEdge1->screeny.value;
  if (fEdge2->screenx.value > polyarea.b.x)
    polyarea.b.x = fEdge2->screenx.value;
  if (fEdge2->screeny.value > polyarea.b.y)
    polyarea.b.y = fEdge2->screeny.value;
  getedge(edgebuffer,fEdge1,fEdge2);

  fillpoly(&polyarea);

}

//---------------------------------------------------------------------------
void object::draw()
{
  dword face;
  spolygon fPoly;
  sspolygon iScreen;
/*  cPolygon cPoly;*/

  area.a.x = GRAPH_MAX_X << FRACBITS;
  area.a.y = GRAPH_MAX_Y << FRACBITS;
  area.b.x = area.b.y = 0 << FRACBITS;

  for (face = numfaces - 1; face >= 0; face--) {
    fPoly.a = location + rotatetab[facetab[face].a];
    fPoly.b = location + rotatetab[facetab[face].b];
    fPoly.c = location + rotatetab[facetab[face].c];
/*    cPoly.a = (cVector)(location + rotatetab[facetab[face].a]);
    cPoly.b = (cVector)(location + rotatetab[facetab[face].b]);
    cPoly.c = (cVector)(location + rotatetab[facetab[face].c]);*/

    iScreen.a = iProject(fPoly.a);
    iScreen.b = iProject(fPoly.b);
    iScreen.c = iProject(fPoly.c);
/*    iScreen.a = project(cPoly.a);
    iScreen.b = project(cPoly.b);
    iScreen.c = project(cPoly.c);*/

    if (bFaceShown(iScreen)) {
      drawpolygon(&fPoly,&iScreen);
      if (polyarea.a.x < area.a.x) area.a.x = polyarea.a.x;
      if (polyarea.a.y < area.a.y) area.a.y = polyarea.a.y;
      if (polyarea.b.x > area.b.x) area.b.x = polyarea.b.x;
      if (polyarea.b.y > area.b.y) area.b.y = polyarea.b.y;
    }

  }
  area.a.x = area.a.x >> FRACBITS;
  area.a.y = area.a.y >> FRACBITS;
  area.b.x = area.b.x >> FRACBITS;
  area.b.y = area.b.y >> FRACBITS;
}

