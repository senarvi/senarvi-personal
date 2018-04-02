#include <math.h>
#include <conio.h>

#include "fixed.h"
#include "vector.h"
#include "video.h"
#include "system.h"


#define F_IN  (1 << 16)
#define F_OUT (1 << 8)
//#define F_OUT_ROT (1 << 8)


extern "C" Vector rotatetab[MAX_VERTICES];

objectdata *current_object;

fcolor scene_ambient = {0.07, 0.07, 0.07};

fVector *face_normal_table;


/*
 * Get color value for phong with material definitions and
 * angle between light vector and face normal vector (theta)
 *
 * Color value is between 0 and 1
 */
// [fold]  [

fcolor phong_color(double theta, material *s) {

  fcolor r;

  r.red =
    s->ambient.red * scene_ambient.red +
    s->diffuse.red * theta +
    s->specular.red * pow(theta, s->shininess) * s->shine_strength;

  r.green =
    s->ambient.green * scene_ambient.green +
    s->diffuse.green * theta +
    s->specular.green * pow(theta, s->shininess) * s->shine_strength;

  r.blue =
    s->ambient.blue * scene_ambient.blue +
    s->diffuse.blue * theta +
    s->specular.blue * pow(theta, s->shininess) * s->shine_strength;

  if (r.red   >= 1) r.red   = 1;
  if (r.green >= 1) r.green = 1;
  if (r.blue  >= 1) r.blue  = 1;

  return(r);
}
// [fold]  ]

/*
 * Set palette for phong
 *
 * Palette is used like a precalculated table accessed by angle
 * between light vector and face normal vector (theta), returning
 * the light intensity
 */
// [fold]  [

void set_phong_palette(material *s, udword base_color, udword last_color,
		       udword max_value) {

  udword index;
  fcolor r;

  for(index = 0; index <= last_color - base_color; index++) {
    r = phong_color((double)index / (last_color - base_color), s);

    outp(0x3c8, (ubyte)(base_color + index));

    outp(0x3c9, (ubyte)(r.red * max_value));
    outp(0x3c9, (ubyte)(r.green * max_value));
    outp(0x3c9, (ubyte)(r.blue * max_value));
  }
}
// [fold]  ]

/*
 * Count normal vectors connected to vertex, if there is any common
 * smoothing groups with face
 */
// [fold]  [

void count_normals(objectdata *s, udword vertex, udword face,
		   fVector *normal_sum, udword *num_connected) {

  sdword count;


  normal_sum->x = normal_sum->y = normal_sum->z = 0;
  *num_connected = 0;

  for (count = s->last_face; count >= 0; count--)

    if ((s->face_table[count].a == vertex ||
	 s->face_table[count].b == vertex ||
	 s->face_table[count].c == vertex) &&
	((s->smoothing_table[count] & s->smoothing_table[face]) != 0)) {

      normal_sum->x += face_normal_table[count].x;
      normal_sum->y += face_normal_table[count].y;
      normal_sum->z += face_normal_table[count].z;

      (*num_connected)++;
    }

  if (*num_connected == 0) {
    *normal_sum = face_normal_table[face];
    *num_connected = 1;
  }
}
// [fold]  ]

/*
 * Set normal vectors for each vertex of each face
 */
// [fold]  [

void set_normals(objectdata *s) {

  sdword  count;
  Vector  a, b, c;
  fVector normal_sum;
  udword  ai, bi, ci,
	  num_connected;
  double  ax, ay, az,
	  bx, by, bz,
	  cx, cy, cz,
	  acx, acy, acz,
	  abx, aby, abz,
	  nx, ny, nz, nl;


  face_normal_table = new fVector[s->last_face + 1];

  for (count = s->last_face; count >= 0; count--) {
    a = s->vertex_table[s->face_table[count].a];
    b = s->vertex_table[s->face_table[count].b];
    c = s->vertex_table[s->face_table[count].c];

    ax = (double)a.x / F_IN;
    ay = (double)a.y / F_IN;
    az = (double)a.z / F_IN;

    bx = (double)b.x / F_IN;
    by = (double)b.y / F_IN;
    bz = (double)b.z / F_IN;

    cx = (double)c.x / F_IN;
    cy = (double)c.y / F_IN;
    cz = (double)c.z / F_IN;

    acx = cx - ax;  acy = cy - ay;  acz = cz - az;
    abx = bx - ax;  aby = by - ay;  abz = bz - az;

    nx = acy * abz - acz * aby;
    ny = acz * abx - acx * abz;
    nz = acx * aby - acy * abx;

    nl = sqrt(nx * nx + ny * ny + nz * nz);

    face_normal_table[count].x = nx / nl;
    face_normal_table[count].y = ny / nl;
    face_normal_table[count].z = nz / nl;
  }

  for (count = s->last_face; count >= 0; count--) {
    ai = s->face_table[count].a;
    bi = s->face_table[count].b;
    ci = s->face_table[count].c;

    count_normals(s, ai, count, &normal_sum, &num_connected);
    s->normal_table[count].a.x = (Fixed)(normal_sum.x / num_connected * F_OUT);
    s->normal_table[count].a.y = (Fixed)(normal_sum.y / num_connected * F_OUT);
    s->normal_table[count].a.z = (Fixed)(normal_sum.z / num_connected * F_OUT);

    count_normals(s, bi, count, &normal_sum, &num_connected);
    s->normal_table[count].b.x = (Fixed)(normal_sum.x / num_connected * F_OUT);
    s->normal_table[count].b.y = (Fixed)(normal_sum.y / num_connected * F_OUT);
    s->normal_table[count].b.z = (Fixed)(normal_sum.z / num_connected * F_OUT);

    count_normals(s, ci, count, &normal_sum, &num_connected);
    s->normal_table[count].c.x = (Fixed)(normal_sum.x / num_connected * F_OUT);
    s->normal_table[count].c.y = (Fixed)(normal_sum.y / num_connected * F_OUT);
    s->normal_table[count].c.z = (Fixed)(normal_sum.z / num_connected * F_OUT);
  }

  delete face_normal_table;
}
// [fold]  ]

/*
 * Rotate object
 */

// [fold]  [
void rotate_2_axes(objectdata *s, double ra, double rb) {

  for (udword count = 0; count <= s->last_vertex; count++) {
    double vx = (double)s->vertex_table[count].x / 0x10000;
    double vy = (double)s->vertex_table[count].y / 0x10000;
    double vz = (double)s->vertex_table[count].z / 0x10000;

    double x = -vx * sin(ra)	       + vy * cos(ra);
    double y = -vx * cos(ra) * cos(rb) - vy * sin(ra) * cos(rb) + vz * sin(rb);
    double z = -vx * cos(ra) * sin(rb) - vy * sin(ra) * sin(rb) - vz * cos(rb);

/*
    double x = -vx * sin(ra)	       + vy * cos(ra);
    double z = -vx * cos(ra) * cos(rb) - vy * sin(ra) * cos(rb) + vz * sin(rb);
    double y = -vx * cos(ra) * sin(rb) - vy * sin(ra) * sin(rb) - vz * cos(rb);
*/

    rotatetab[count].x = (udword)x + s->pos.x;	// t„ss„ vaiheessa jo
    rotatetab[count].y = (udword)y + s->pos.y;	// pit„isi olla fixed .
    rotatetab[count].z = (udword)z + s->pos.z;
  }
}
// [fold]  ]

// [fold]  [
void rotate_3_axes(objectdata *s, double rx, double ry, double rz) {

  double s1 = sin(-rz), s2 = sin(-ry), s3 = sin(-rx);
  double c1 = cos(-rz), c2 = cos(-ry), c3 = cos(-rx);

  double xx = c2*c1,		 xy = -c2*s1,		 xz = -s2;
  double yx = c3*s1 - s3*s2*c1,	 yy = c3*c1 + s3*s2*s1,	 yz = -s3*c2;
  double zx = s3*s1 + c3*s2*c1,	 zy = s3*c1 - c3*s2*s1,	 zz = c3*c2;

  for (udword count = 0; count <= s->last_vertex; count++) {
    double x = (double)s->vertex_table[count].x / 0x10000;
    double y = (double)s->vertex_table[count].y / 0x10000;
    double z = (double)s->vertex_table[count].z / 0x10000;

    rotatetab[count].x = (udword)(x * xx + y * xy + z * xz) + s->pos.x;	 // t„ss„ vaiheessa jo
    rotatetab[count].y = (udword)(x * yx + y * yy + z * yz) + s->pos.y;	 // pit„isi olla fixed .
    rotatetab[count].z = (udword)(x * zx + y * zy + z * zz) + s->pos.z;
  }
}
// [fold]  ]

// [fold]  [
void rotate_object(objectdata *s) {

  double ax = (double)s->rot.axis.x / 0x100;
  double ay = (double)s->rot.axis.y / 0x100;
  double az = (double)s->rot.axis.z / 0x100;
  double a = (double)s->rot.angle / 0x100;


  double ca = cos(a), sa = sin(a);
  double nca = 1 - ca;

  double ax_ax = ax*ax, ax_ay = ax*ay, ax_az = ax*az;
  double ay_ax = ax_ay, ay_ay = ay*ay, ay_az = ay*az;
  double az_ax = ax_az, az_ay = ay_az, az_az = az*az;

  double ax_ax_nca = ax_ax * nca;
  double ax_ay_nca = ax_ay * nca;
  double ax_az_nca = ax_az * nca;
  double ay_ax_nca = ax_ay_nca;
  double ay_ay_nca = ay_ay * nca;
  double ay_az_nca = ay_az * nca;
  double az_ax_nca = ax_az_nca;
  double az_ay_nca = ay_az_nca;
  double az_az_nca = az_az * nca;

  double ax_sa = ax * sa;
  double ay_sa = ay * sa;
  double az_sa = az * sa;


  //	       [xx yx zx]
  // [X Y Z] * [xy yy zy] = [Xxx+Yxy+Zxz Xyx+Yyy+Zyz Xzx+Yzy+Zzz]
  //	       [xz yz zz]

  double xx = ax_ax_nca + ca;
  double xy = ax_ay_nca + az_sa;
  double xz = ax_az_nca - ay_sa;

  double yx = ay_ax_nca - az_sa;
  double yy = ay_ay_nca + ca;
  double yz = ay_az_nca + ax_sa;

  double zx = az_ax_nca + ay_sa;
  double zy = az_ay_nca - ax_sa;
  double zz = az_az_nca + ca;

  s->matrix[0][0] = (Fixed)(xx * F_OUT);
  s->matrix[0][1] = (Fixed)(yx * F_OUT);
  s->matrix[0][2] = (Fixed)(zx * F_OUT);

  s->matrix[1][0] = (Fixed)(xy * F_OUT);
  s->matrix[1][1] = (Fixed)(yy * F_OUT);
  s->matrix[1][2] = (Fixed)(zy * F_OUT);

  s->matrix[2][0] = (Fixed)(xz * F_OUT);
  s->matrix[2][1] = (Fixed)(yz * F_OUT);
  s->matrix[2][2] = (Fixed)(zz * F_OUT);


  for (udword count = 0; count <= s->last_vertex; count++) {
    double x = (double)s->vertex_table[count].x / F_IN;
    double y = (double)s->vertex_table[count].y / F_IN;
    double z = (double)s->vertex_table[count].z / F_IN;

    rotatetab[count].x = (Fixed)((x * xx + y * xy + z * xz) * _FR) + (s->pos.x << 8);
    rotatetab[count].y = (Fixed)((x * yx + y * yy + z * yz) * _FR) + (s->pos.y << 8);
    rotatetab[count].z = (Fixed)((x * zx + y * zy + z * zz) * _FR) + (s->pos.z << 8);
  }
}
// [fold]  ]


// [fold]  7
