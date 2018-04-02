#ifndef _VECTOR_H_INCLUDED

#include "fixed.h"
#include "video.h"
#include "system.h"


#define MAX_VERTICES 1024
#define MAX_FACES    2048


typedef _Packed struct vectordef {
  Fixed x, y, z;
} vector;
typedef vector Vector;

typedef _Packed struct fvectordef {
  double x, y, z;
} fvector;
typedef fvector fVector;

typedef _Packed struct mapcoorddef {
  Fixed u, v;
} mapcoord;
typedef mapcoord MapVector;

typedef _Packed struct facedef {
  Fixed a, b, c;
} face;
typedef face Face;

typedef _Packed struct planedef {
  Vector a, b, c;
} plane;
typedef plane Plane;

typedef _Packed struct angledef {
  udword a, b;
  udword r;
} angle;
typedef angle Angle;

typedef _Packed struct rotationdef {
  Vector axis;
  udword angle;
} rotation;
typedef rotation Rotation;

typedef _Packed struct materialdef {
  fcolor ambient;
  fcolor diffuse;
  fcolor specular;
  double shininess;
  double shine_strength;
} material;

typedef _Packed struct objectdatadef {
  Face	      *face_table;
  Vector      *vertex_table;
  MapVector   *mapping_table;			// Texture mapping coordinates
  udword      *smoothing_table;			// Smoothing groups

  Plane	      *normal_table;

  udword      last_face;			// Number of faces - 1
  udword      last_vertex;			// Number of vertices - 1

  udword      face_end;				// Offset to the last face
  udword      vertex_end;			// Offset to the last vertex

  Vector      pos;
  rotation    rot;

  Fixed	      matrix[3][3];			// Rotation matrix [row][col]

} objectdata;


extern fcolor scene_ambient;
extern "C" objectdata *current_object;


extern void set_normals(objectdata *s);
extern void rotate_object(objectdata *s);
extern "C" void _sortfaces(objectdata *s);
extern void set_phong_palette( material *__s, udword base_color,
			       udword last_color, udword max_value );


#define _VECTOR_H_INCLUDED
#endif
