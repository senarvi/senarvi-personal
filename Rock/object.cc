#include <stdlib.h>
//#include <dpmi.h>
#include <stdio.h>
#include "object.h"
#include "vector.h"
#include "init.h"

object rock;


//---------------------------------------------------------------------------
// Initialize object
//---------------------------------------------------------------------------

void object::init(signed long _numvertices,signed long _numfaces,
                  svertex *_vertextab,sface *_facetab,
                  svector _location,sangle _angle)
{
  /* Set variables */
  prevarea.a.x = GRAPH_MAX_X;
  prevarea.a.y = GRAPH_MAX_Y;
  prevarea.b.x = prevarea.b.y = 0;

  numvertices = _numvertices;
  numfaces    = _numfaces;
  vertextab   = _vertextab;
  facetab     = _facetab;
  location    = _location;
  angle       = _angle;

  /* Allocate memory */
  rotatetab  = (svertex *)malloc(numvertices * sizeof(svertex));
  fnormaltab = (svector *)malloc(numfaces    * sizeof(svector));
  vnormaltab = (svector *)malloc(numvertices * sizeof(svector));

  if (vnormaltab == NULL) {
    exittype = EXIT_NOMEM;
    exit(EXIT_FAILURE);
  }
}

//---------------------------------------------------------------------------
// Rotate object
//---------------------------------------------------------------------------

void object::rotate()
{
  dword vertexnum;
  cVector vertex;
  const sangle _angle = angle;

  for (vertexnum = numvertices - 1; vertexnum >= 0; vertexnum--) {
    vertex.x.value = vertextab[vertexnum].x;
    vertex.y.value = vertextab[vertexnum].y;
    vertex.z.value = vertextab[vertexnum].z;
    vertex.rotate(_angle);
    rotatetab[vertexnum].x = vertex.x.value;
    rotatetab[vertexnum].y = vertex.y.value;
    rotatetab[vertexnum].z = vertex.z.value;
  }
}

//---------------------------------------------------------------------------
// Get normal of vertex
//---------------------------------------------------------------------------

svector object::getvnormal(signed long vertex)
{
  signed long face;
  signed long numconnected = 0;
  svector vnormal = {0,0,0};

  for (face = numfaces - 1; face >= 0; face--)
    if ((facetab[face].a == vertex) || (facetab[face].b == vertex) ||
        (facetab[face].c == vertex)) {
      vnormal.x += fnormaltab[face].x;
      vnormal.y += fnormaltab[face].y;
      vnormal.z += fnormaltab[face].z;
      numconnected++;
    }
  return vnormal / numconnected;
}

