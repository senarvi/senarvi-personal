/*
 * 3ds2dat.cpp
 *
 * Muuntaa Autodesk 3D Studion objekti-tiedostoja datatiedostoksi
 *
 * Copyright (C) Seppo Enarvi   1996-1998
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>

#define _Packed __attribute__ ((packed))
using namespace std;

#include "system.h"

typedef int objecttype;

typedef int BOOL;
#define FALSE 0
#define TRUE 1

#define FRAC_BITS 16
#define MUL (1 << FRAC_BITS)

/*
 * Structures
 */

// [fold]  [
typedef _Packed struct chunkdef {
  uword   id;
  udword  next;
} chunk;
// [fold]  ]

// [fold]  [
typedef _Packed struct vector_struct_def {
  float x, y, z;
} vector_struct;
// [fold]  ]

// [fold]  [
typedef _Packed struct face_struct_def {
  uword a, b, c, d;
} face_struct;
// [fold]  ]

// [fold]  [
typedef _Packed struct mapping_coordinate_struct_def {
  float u, v;
} mapping_coordinate_struct;
// [fold]  ]

// [fold]  [
typedef struct name_entry_def {
  string name;
  udword data;
  name_entry_def *next;
} name_entry;
// [fold]  ]

/*
 * Classes
 */

// [fold]  [
class Face {

private:

  uword _a, _b, _c, _d;

public:

  float a() { return(this->_a); }
  float b() { return(this->_b); }
  float c() { return(this->_c); }
  float d() { return(this->_d); }

  void read(istream &strm);
  friend istream &operator >> (istream &strm, Face &f);
  friend ostream &operator << (ostream &strm, Face const &f);
};

// [fold]  [
void Face::read(istream &strm) {

  face_struct buffer;


  strm.read((char *)&buffer, sizeof(face_struct));

  this->_a = buffer.a;
  this->_b = buffer.b;
  this->_c = buffer.c;
  this->_d = buffer.d;
}
// [fold]  ]

// [fold]  [
istream &operator >> (istream &strm, Face &f) {

  f.read(strm);
  return(strm);
}
// [fold]  ]

// [fold]  [
ostream &operator << (ostream &strm, Face const &f) {

  strm << "{" << f._a << ", " << f._b << ", " << f._c << "}";
  return(strm);
}
// [fold]  ]


// [fold]  ]

// [fold]  [
class Vector {

private:

  float _x, _y, _z;
  BOOL _fixed;

public:

  Vector();
  Vector(Vector const &v);
  Vector(float const x, float const y, float const z);

  float x() { return(this->_x); }
  float y() { return(this->_y); }
  float z() { return(this->_z); }

  friend Vector operator * (Vector const &v, float const f);
  friend Vector operator * (float const f, Vector const &v);

  void set_fixed(BOOL const b);

  void read(istream &strm);
  friend istream &operator >> (istream &strm, Vector &v);
  friend ostream &operator << (ostream &strm, Vector const &v);
};

// [fold]  [
Vector::Vector() {

  _x = _y = _z = 0;
  _fixed = FALSE;
}
// [fold]  ]

// [fold]  [
Vector::Vector(Vector const &v) {

  _x = v._x;
  _y = v._y;
  _z = v._z;
  _fixed = v._fixed;
}
// [fold]  ]

// [fold]  [
Vector::Vector(float const x, float const y, float const z) {

  _x = x;
  _y = y;
  _z = z;
  _fixed = FALSE;
}
// [fold]  ]

// [fold]  [
Vector operator * (Vector const &v, float const f) {

  Vector r;


  r._x = v._x * f;
  r._y = v._y * f;
  r._z = v._z * f;

  return(r);
}
// [fold]  ]

// [fold]  [
Vector operator * (float const f, Vector const &v) {

  Vector r;


  r._x = v._x * f;
  r._y = v._y * f;
  r._z = v._z * f;

  return(r);
}
// [fold]  ]

// [fold]  [
void Vector::set_fixed(BOOL const b) {

  if (b == TRUE) this->_fixed = TRUE;
  else this->_fixed = FALSE;
}
// [fold]  ]

// [fold]  [
void Vector::read(istream &strm) {

  vector_struct buffer;


  strm.read((char *)&buffer, sizeof(vector_struct));

  this->_x = buffer.x;
  this->_y = buffer.y;
  this->_z = buffer.z;
}
// [fold]  ]

// [fold]  [
istream &operator >> (istream &strm, Vector &v) {

  v.read(strm);
  return(strm);
}
// [fold]  ]

// [fold]  [
ostream &operator << (ostream &strm, Vector const &v) {

  strm << "{" << v._x << ", " << v._y << ", " << v._z << "}";

  return(strm);
}
// [fold]  ]


// [fold]  ]

// [fold]  [
class MapVector {

private:

  float _u, _v;

public:

  float MapVector::u() { return(this->_u); }
  float MapVector::v() { return(this->_v); }

  void read(istream &strm);
  friend istream &operator >> (istream &strm, MapVector &mv);
  friend ostream &operator << (ostream &strm, MapVector const &mv);
};

// [fold]  [
void MapVector::read(istream &strm) {

  mapping_coordinate_struct buffer;


  strm.read((char *)&buffer, sizeof(mapping_coordinate_struct));

  this->_u = buffer.u;
  this->_v = buffer.v;
}
// [fold]  ]

// [fold]  [
istream &operator >> (istream &strm, MapVector &mv) {

  mv.read(strm);
  return(strm);
}
// [fold]  ]

// [fold]  [
ostream &operator << (ostream &strm, MapVector const &mv) {

  strm << "{" << mv._u << ", " << mv._v << "}";

  return(strm);
}
// [fold]  ]


// [fold]  ]

// [fold]  [
class key_num_info {

public:

    udword position, rotation, scale;
};
// [fold]  ]

// [fold]  [
class position_key {

public:

  udword frame;
  Vector position;
};
// [fold]  ]

// [fold]  [
class rotation_key {

public:

  udword frame;
  Vector axis;
  float  angle;
};
// [fold]  ]

// [fold]  [
class scale_key {

public:

  udword frame;
  Vector scale;
};
// [fold]  ]

// [fold]  [
class Matrix {

  float _data[4][4];

public:

  void read(istream &strm);
  friend istream &operator >> (istream &strm, Matrix &m);
  friend ostream &operator << (ostream &strm, Matrix const &m);
};

// [fold]  [
void Matrix::read(istream &strm) {

  float buffer[4][3];


  strm.read((char *)&buffer, sizeof(buffer));

  this->_data[0][0] = buffer[0][0];
  this->_data[0][1] = buffer[0][1];
  this->_data[0][2] = buffer[0][2];
  this->_data[0][3] = 0;
  this->_data[1][0] = buffer[1][0];
  this->_data[1][1] = buffer[1][1];
  this->_data[1][2] = buffer[1][2];
  this->_data[1][3] = 0;
  this->_data[2][0] = buffer[2][0];
  this->_data[2][1] = buffer[2][1];
  this->_data[2][2] = buffer[2][2];
  this->_data[2][3] = 0;
  this->_data[3][0] = buffer[3][0];
  this->_data[3][1] = buffer[3][1];
  this->_data[3][2] = buffer[3][2];
  this->_data[3][3] = 1;
}
// [fold]  ]

// [fold]  [
istream &operator >> (istream &strm, Matrix &m) {

  m.read(strm);
  return(strm);
}
// [fold]  ]

// [fold]  [
ostream &operator << (ostream &strm, Matrix const &m) {

  strm << setfill(' ')
       << setw(11) << m._data[0][0] << ", "
       << setw(12) << m._data[0][1] << ", "
       << setw(12) << m._data[0][2] << ", "
       << setw(12) << m._data[0][3] << ", " << endl
       << setw(11) << m._data[1][0] << ", "
       << setw(12) << m._data[1][1] << ", "
       << setw(12) << m._data[1][2] << ", "
       << setw(12) << m._data[1][3] << ", " << endl
       << setw(11) << m._data[2][0] << ", "
       << setw(12) << m._data[2][1] << ", "
       << setw(12) << m._data[2][2] << ", "
       << setw(12) << m._data[2][3] << ", " << endl
       << setw(11) << m._data[3][0] << ", "
       << setw(12) << m._data[3][1] << ", "
       << setw(12) << m._data[3][2] << ", "
       << setw(12) << m._data[3][3] << endl;

  return(strm);
}
// [fold]  ]


// [fold]  ]

// [fold]  [
class Object {

public:

  enum object_type {undefined, mesh, omni_light, spotlight, camera};

private:

  objecttype _type;

public:

  string _name;
  Object *next;

  BOOL mapped;
  BOOL corr;

  uword num_vertices;
  Vector *vertex_table;
  MapVector *mapping_table;
  Matrix translation_matrix;

  uword num_faces;
  Face *face_table;
  udword *smoothing_table;

  Vector position;
  Vector target;

  key_num_info num_keys;
  position_key *position_key_table;
  rotation_key *rotation_key_table;
  scale_key *scale_key_table;

  float hotspot, falloff;
  float bank_angle, lens_size;

  string name();
  objecttype type();
  void set_type(objecttype t);

  Object();

  void read_name(ifstream *strm);
};

// [fold]  [
string Object::name() {
  return(this->_name);
}
// [fold]  ]

// [fold]  [
objecttype Object::type() {
  return(this->_type);
}
// [fold]  ]

// [fold]  [
void Object::set_type(objecttype t) {
  this->_type = t;
}
// [fold]  ]

// [fold]  [
Object::Object() {
  this->next = NULL;
  this->_type = undefined;
}
// [fold]  ]

// [fold]  [
void Object::read_name(ifstream *strm) {

  char c;

  this->_name = "";
  c = strm->get();
  while (c != 0) {
    this->_name += c;
    c = strm->get();
  }
}
// [fold]  ]


// [fold]  ]

/*
 * Functions
 */

// [fold]  [
void skip_chunk(ifstream *f, chunk c) {

  f->seekg(c.next - sizeof(chunk), ios::cur);
}
// [fold]  ]

// [fold]  [
Object *import(ifstream *f, string *obj_name) {

  udword  file_length;
  Object  *s = new(Object);

  chunk   chunk_buffer;
  float   float_buffer[2];
  udword  udword_buffer[1];
  uword   uword_buffer[3];
  char    char_buffer[10];

  udword  count;


  f->read((char *)&chunk_buffer, sizeof(chunk));
  file_length = chunk_buffer.next;

  while (f->tellg() < file_length) {
    switch (chunk_buffer.id) {
      case 0x4000:  // Object description block
        s->read_name(f);
        if (s->name() == *obj_name) s->corr = TRUE;
        else s->corr = FALSE;
        break;

      case 0x4100:  // Triangular mesh object
        if (!s->corr) {
          skip_chunk(f, chunk_buffer);
          break;
        }

        s->set_type(Object::mesh);
        break;

      case 0x4110:  // Vertex list
        if (!s->corr) {
          skip_chunk(f, chunk_buffer);
          break;
        }

        f->read((char *)&uword_buffer, sizeof(uword));
        s->num_vertices = uword_buffer[0];
        s->vertex_table = new(Vector[s->num_vertices]);

        for (count = 0; count <= s->num_vertices - 1; count++)
          *f >> s->vertex_table[count];
        break;

      case 0x4120:  // Face list
        if (!s->corr) {
          skip_chunk(f, chunk_buffer);
          break;
        }

        f->read((char *)&uword_buffer, sizeof(uword));
        s->num_faces = uword_buffer[0];
        s->face_table = new(Face[s->num_faces]);

        for(count = 0; count <= s->num_faces - 1; count++)
          *f >> s->face_table[count];
        break;

      case 0x4140:  // Mapping coordinates
        if (!s->corr) {
          skip_chunk(f, chunk_buffer);
          break;
        }

        s->mapped = 1;
        f->read((char *)&uword_buffer, sizeof(uword));
        s->num_vertices = uword_buffer[0];
        s->mapping_table = new(MapVector[s->num_vertices]);

        for(count = 0; count <= s->num_vertices - 1; count++)
          *f >> s->mapping_table[count];
        break;

      case 0x4150:  // Smoothing groups
        if (!s->corr) {
          skip_chunk(f, chunk_buffer);
          break;
        }

        s->smoothing_table = new(udword[s->num_faces]);

        for(count = 0; count <= s->num_faces - 1; count++)
          f->read((char *)&s->smoothing_table[count], sizeof(udword));
        break;

      case 0x4160:  // Translation matrix
        if (!s->corr) {
          skip_chunk(f, chunk_buffer);
          break;
        }

        *f >> s->translation_matrix;
        break;

      case 0x4600:  // Light
        if (!s->corr) {
          skip_chunk(f, chunk_buffer);
          break;
        }

        s->set_type(Object::omni_light);
        *f >> s->position;
        break;

      case 0x4610:  // Spotlight
        if (!s->corr) {
          skip_chunk(f, chunk_buffer);
          break;
        }

        s->set_type(Object::spotlight);
        *f >> s->target;
        f->read((char *)&float_buffer, 2 * sizeof(float));
        s->hotspot = float_buffer[0];
        s->falloff = float_buffer[1];
        break;

      case 0x4700:  // Camera
        if (!s->corr) {
          skip_chunk(f, chunk_buffer);
          break;
        }

        s->set_type(Object::camera);
        s->position.read(*f);
        s->target.read(*f);
        f->read((char *)&float_buffer, 2 * sizeof(float));
        s->bank_angle = float_buffer[0];
        s->lens_size = float_buffer[1];
        break;


      default:  // Undefined chunk
        skip_chunk(f, chunk_buffer);

      case 0x4d4d:  // Main chunks
      case 0x3d3d:  // Mesh block
        break;
    }
    f->read((char *)&chunk_buffer, sizeof(chunk));
  }

  f->clear();
  f->seekg(0, ios::beg);
  return(s);
}
// [fold]  ]

// [fold]  [
void write_udw(ofstream *f, udword s) {

  f->write((char *)&s, sizeof(udword));
}
// [fold]  ]

// [fold]  [
name_entry *exportx(ofstream *df, ofstream *cf, ofstream *hf, Object *s,
                   string *obj_name) {

  udword u_dw, c;
  name_entry *ne = new name_entry;


  ne->name = *obj_name;

  udword face_table_o = df->tellp();
  for (c = 0; c <= s->num_faces - 1; c++) {
    write_udw(df, (udword)(s->face_table[c].a()));
    write_udw(df, (udword)(s->face_table[c].b()));
    write_udw(df, (udword)(s->face_table[c].c()));
  }

  udword vertex_table_o = df->tellp();
  for (c = 0; c <= s->num_vertices - 1; c++) {
    write_udw(df, (udword)(s->vertex_table[c].x() * MUL));
    write_udw(df, (udword)(s->vertex_table[c].y() * MUL));
    write_udw(df, (udword)(s->vertex_table[c].z() * MUL));
  }

  udword mapping_table_o = (udword)-1;
  if (s->mapped) {
    mapping_table_o = df->tellp();
    for (c = 0; c <= s->num_vertices - 1; c++) {
      write_udw(df, (udword)(s->mapping_table[c].u() * MUL));
      write_udw(df, (udword)(s->mapping_table[c].v() * MUL));
    }
  }

  udword smoothing_table_o = df->tellp();
  for (c = 0; c <= s->num_faces - 1; c++)
    write_udw(df, s->smoothing_table[c]);

  ne->data = df->tellp();

  write_udw(df, face_table_o);
  write_udw(df, vertex_table_o);
  write_udw(df, mapping_table_o);
  write_udw(df, smoothing_table_o);
  write_udw(df, (udword)-1);

  write_udw(df, s->num_faces - 1);
  write_udw(df, s->num_vertices - 1);

  write_udw(df, s->num_faces - 1);
  write_udw(df, s->num_vertices - 1);

  write_udw(df, 0);
  write_udw(df, 0);
  write_udw(df, 0);

  write_udw(df, 0);
  write_udw(df, 0);
  write_udw(df, 0);
  write_udw(df, 0);

  return ne;
}
// [fold]  ]

// [fold]  [
void close_out(ofstream *df, ofstream *cf, ofstream *hf,
               name_entry *name_list) {

  name_entry *first_name = name_list;


  *cf << endl
      << "#include \"system.h\"" << endl
      << "#include \"object.h\"" << endl
      << endl
      << endl;
  *hf << endl;

  // objectdata *myobject_data;
  while (name_list != NULL) {
    *cf << "objectdata *" << name_list->name << "_data;" << endl;
    *hf << "extern objectdata *" << name_list->name << "_data;" << endl;
    name_list = name_list->next;
  }

  *hf << endl
      << "extern void set_object_data(void *);" << endl
      << endl;
  *cf << endl
      << endl
      << "void set_object_data(void *data_ptr) {" << endl
      << endl
      << "  udword data_address = (udword)data_ptr;" << endl
      << endl
      << endl;

  // myobject_data = (objectdata *)(data_address + n);
  name_list = first_name;
  while (name_list != NULL) {
    *cf << "  " << name_list->name << "_data = (objectdata *)(data_address + " << name_list->data << ");" << endl;
    name_list = name_list->next;
  }
  *cf << endl;

  name_list = first_name;
  while (name_list != NULL) {
    *cf << endl;
    // myobject_data->x_table = (X *)(data_address + (udword)myobject_data->x_table);
    *cf << "  " << name_list->name << "_data->face_table = (Face *)(data_address + (udword)" << name_list->name << "_data->face_table);" << endl;
    *cf << "  " << name_list->name << "_data->vertex_table = (Vector *)(data_address + (udword)" << name_list->name << "_data->vertex_table);" << endl;
    *cf << "  " << name_list->name << "_data->mapping_table = (MapVector *)(data_address + (udword)" << name_list->name << "_data->mapping_table);" << endl;
    *cf << "  " << name_list->name << "_data->smoothing_table = (udword *)(data_address + (udword)" << name_list->name << "_data->smoothing_table);" << endl;
    // myobject_data->normal_table = normal_table_1;
    *cf << "  " << name_list->name << "_data->normal_table = normal_table_1;" << endl;
    // myobject_data->x_end *= sizeof(X);
    *cf << "  " << name_list->name << "_data->face_end *= sizeof(Face);" << endl;
    *cf << "  " << name_list->name << "_data->vertex_end *= sizeof(Vector);" << endl;
    name_list = name_list->next;
  }

  *cf << "}" << endl
      << endl;

  df->close();
  cf->close();
  hf->close();
}
// [fold]  ]

// [fold]  [
int main(int argc, char *argv[]) {

  ifstream ilf, mf;
  ofstream of, cf, hf;
  string in_obj, out_obj;
  name_entry *name_list = NULL;
  name_entry *next_name;


  cout << endl
       << "3D Studio mesh data converter" << endl
       << "Copyright (C) Seppo Enarvi 1996,1997" << endl;

  if (argc < 2) {
    cerr << "No input list file given" << endl;
    exit(EXIT_FAILURE);
  }
  ilf.open(argv[1]/*, ios::in | ios::nocreate | ios::text*/);

  while (!ilf.eof()) {
    string cmd;
    ilf >> cmd;

    if (cmd == "destination") {
      string ofname, cfname, hfname;
      ilf >> ofname >> cfname >> hfname;

      if (of.is_open()) close_out(&of, &cf, &hf, name_list);
      of.open(ofname.c_str(), /*ios::out | ios::truncate |*/ ios::binary);
      cf.open(cfname.c_str()/*, ios::out | ios::truncate | ios::text*/);
      hf.open(hfname.c_str()/*, ios::out | ios::truncate | ios::text*/);

      cout << endl
           << "data output for " << ofname << ", " << cfname << ", " << hfname
           << endl;
    }

    if (cmd == "open_file") {
      string mfname;
      ilf >> mfname;

      if (mf.is_open()) mf.close();
      mf.open(mfname.c_str(), ios::in | /*ios::nocreate |*/ ios::binary);

      cout << "  reading from " << mfname << endl;
    }

    if (cmd == "import_mesh") {
      ilf >> in_obj >> out_obj;

      cout << "    mesh: " << in_obj << " => " << out_obj << endl;
      next_name = name_list;
      name_list = exportx(&of, &cf, &hf, import(&mf, &in_obj), &out_obj);
      name_list->next = next_name;
    }

/*    if (cmd == "import_mesh_ani") {
      ilf >> in_obj >> out_obj;

      cout << "    mesh + animation: " << in_obj << " => " << out_obj << endl;
    }*/

    cout.flush();
  }

  ilf.close();
  if (mf.is_open()) mf.close();
  if (of.is_open()) close_out(&of, &cf, &hf, name_list);
  return EXIT_SUCCESS;
}
// [fold]  ]

// [fold]  43
