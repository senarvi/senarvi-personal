#ifndef _vector_h_
#define _vector_h_

#include "types.h"
#include "math.h"
#include "graphics.h"

#define FOCUS    128
#define LENSDIST (FOCUS * 2)
#define X_MUL    (LENSDIST << FRACBITS)
#define Y_MUL    (LENSDIST << FRACBITS)

//---------------------------------------------------------------------------
typedef struct {
  fixed x,y,z;
} svector;

typedef svector svertex;

typedef struct {
  svertex a,b,c;
} spolygon;

typedef struct {
  signed long a,b,c;
} sface;

typedef struct {
  signed char a,b;
} sangle;

typedef struct {
  fixed u,v;
} smapcoord;

/*#define sina sin(angle.a)
#define cosa cos(angle.a)
#define sinb sin(angle.b)
#define cosb cos(angle.b)*/
//---------------------------------------------------------------------------
class cVector {
protected:

public:
  cFixed x,y,z;

  cVector();
  cVector(svector);
  cVector(const cVector&);

  void rotate(sangle);

private:
};

inline cVector::cVector() :x(0), y(0), z(0) {}
inline cVector::cVector(svector v) :x(v.x), y(v.y), z(v.z) {}
inline cVector::cVector(const cVector& v) :x(v.x), y(v.y), z(v.z) {}

inline void cVector::rotate(sangle angle)
{
  cFixed xsa    = x * xsin(angle.a),
         yca    = y * xcos(angle.a),
         xcaysa = (x + y) * (xcos(angle.a) + xsin(angle.a)) - xsa - yca;

  x = yca - xsa;
  y = +(z * xcos(angle.b)) - (xsin(angle.b) * xcaysa);
  z = -(z * xsin(angle.b)) - (xcos(angle.b) * xcaysa);
}

typedef struct {
  cVector a,b,c;
} polygon;

//---------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

/*extern bool faceshown(sspolygon poly);*/

#ifdef __cplusplus
}
#endif


//---------------------------------------------------------------------------
#define ax iScreen.a.x
#define ay iScreen.a.y
#define bx iScreen.b.x
#define by iScreen.b.y
#define cx iScreen.c.x
#define cy iScreen.c.y

inline bool bFaceShown(sspolygon iScreen)
{
  return (cy - ay) * (bx - ax) > (cx - ax) * (by - ay);
}

//---------------------------------------------------------------------------
inline svector operator +(svector source1,svector source2)
{
  svector result;
  result.x = source1.x + source2.x;
  result.y = source1.y + source2.y;
  result.z = source1.z + source2.z;
  return result;
}

//---------------------------------------------------------------------------
inline svector operator -(svector source1,svector source2)
{
  svector result;
  result.x = source1.x - source2.x;
  result.y = source1.y - source2.y;
  result.z = source1.z - source2.z;
  return result;
}

//---------------------------------------------------------------------------
inline svector operator *(svector source1,fixed source2)
{
  svector result;
  result.x = fMul(source1.x,source2);
  result.y = fMul(source1.y,source2);
  result.z = fMul(source1.z,source2);
  return result;
}

//---------------------------------------------------------------------------
inline svector operator /(svector source1,fixed source2)
{
  svector result;
  result.x = fDiv(source1.x,source2);
  result.y = fDiv(source1.y,source2);
  result.z = fDiv(source1.z,source2);
  return result;
}

//---------------------------------------------------------------------------
inline svector operator <<(svector source1,signed long source2)
{
  svector result;
  result.x = source1.x << source2;
  result.y = source1.y << source2;
  result.z = source1.z << source2;
  return result;
}

//---------------------------------------------------------------------------
inline svector operator >>(svector source1,signed long source2)
{
  svector result;
  result.x = source1.x >> source2;
  result.y = source1.y >> source2;
  result.z = source1.z >> source2;
  return result;
}

//---------------------------------------------------------------------------
inline signed long dotproduct(svector source1,svector source2)
{
  return fMul(source1.x,source2.x)
        +fMul(source1.y,source2.y)
        +fMul(source1.z,source2.z);
}

//---------------------------------------------------------------------------
inline svector crossproduct(svector source1,svector source2)
{
  svector result;
  result.x = fMul(source1.y,source2.z) - fMul(source1.z,source2.y);
  result.y = fMul(source1.z,source2.x) - fMul(source1.x,source2.z);
  result.z = fMul(source1.x,source2.y) - fMul(source1.y,source2.x);
  return result;
}

//---------------------------------------------------------------------------
inline spixel iProject(svertex fVertex)
{
  spixel iResult;
  iResult.x = (fMul(fDiv(fVertex.x,fVertex.z),X_MUL) >> FRACBITS)
              + GRAPH_MAX_X / 2;
  iResult.y = (fMul(fDiv(fVertex.y,fVertex.z),Y_MUL) >> FRACBITS)
              + GRAPH_MAX_Y / 2;
  return iResult;
}

/*inline spixel project(cVector v) return r
{
  r.x = ((v.x / v.z) * X_MUL) >> FRACBITS) + GRAPH_MAX_X / 2;
  r.y = ((v.y / v.z) * Y_MUL) >> FRACBITS) + GRAPH_MAX_Y / 2;
}*/

#endif /* !_vector_h_ */

