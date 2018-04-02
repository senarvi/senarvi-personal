/*
 * 3ds2cpp.cpp
 *
 * Muuntaa Autodesk 3D Studion objekti-tiedoston
 * C-kielen taulukoiksi
 *
 * Copyright (C) Seppo Enarvi   1996,1997
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

#include "system.h"


#define DIR "\"

#define FRAC_BITS 16
#define MUL (1 << FRAC_BITS)


typedef int   bool;

#define TRUE  ((bool)1)
#define FALSE ((bool)0)


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
  bool _fixed;

public:

  Vector();
  Vector(Vector const &v);
  Vector(float const x, float const y, float const z);

  float x() { return(this->_x); }
  float y() { return(this->_y); }
  float z() { return(this->_z); }

  friend Vector operator * (Vector const &v, float const f);
  friend Vector operator * (float const f, Vector const &v);

  void set_fixed(bool const b);

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
void Vector::set_fixed(bool const b) {

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

typedef int ot;

// [fold]  [
class Object {

public:

  enum object_type {undefined, mesh, omni_light, spotlight, camera};

private:

  ot _type;

public:

  String _name;
  Object *next;

  int mapped;

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

  String name();
  ot type();
  void set_type(ot t);

  Object();

  void read_name(ifstream *strm);
};

// [fold]  [
String Object::name() {
  return(this->_name);
}
// [fold]  ]

// [fold]  [
ot Object::type() {
  return(this->_type);
}
// [fold]  ]

// [fold]  [
void Object::set_type(ot t) {
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
 * Errors
 */

// [fold]  [
void invalid_file() {
    cerr << "Invalid 3DS file" << endl;
    exit( EXIT_FAILURE );
}
// [fold]  ]

// [fold]  [
void warning( uword chunk_id, udword file_offset ) {
    cout << "Warning! Undefined chunk: " << setw( 4 ) << setfill( '0' )
         << hex << setiosflags( ios::uppercase ) << chunk_id;
    cout << " at " << setw( 8 ) << file_offset << endl;
}
// [fold]  ]

/*
 * Initialization
 */

// [fold]  [
void init( int num_params, char *param_table[],
           ifstream *input_file, ofstream *output_file,
           ofstream *header_file, ofstream *include_file ) {

    String name;


    cout << endl;

    if( num_params = 0 ) {
        cerr << "Usage: 3ds2cpp <file name without extension>" << endl;
        exit( EXIT_FAILURE );
    }

    name = param_table[ 1 ];
    name += ".3ds";
    input_file->open( name, ios::in | ios::nocreate | ios::binary );

    if( !input_file->is_open() ) {
        cerr << "File not found: " << param_table[ 1 ] << ".3ds" << endl;
        exit( EXIT_FAILURE );
    }

    name = param_table[ 1 ];
    name += ".cpp";
    output_file->open( name, ios::out | ios::truncate | ios::text );

    name = param_table[ 1 ];
    name += ".h";
    header_file->open( name, ios::out | ios::truncate | ios::text );

    name = param_table[ 1 ];
    name += ".inc";
    include_file->open( name, ios::out | ios::truncate | ios::text );
}
// [fold]  ]

/*
 * Input
 */

// [fold]  [
Object *read_data(ifstream *input_file) {

  udword file_length;

  chunk   chunk_buffer;
  float   float_buffer[2];
  udword  udword_buffer[1];
  uword   uword_buffer[3];
  char    char_buffer[10];

  Object *s = NULL;
  Object *next_object;

  udword count;


  input_file->read((char *)&chunk_buffer, sizeof(chunk));
  if (chunk_buffer.id != 0x4d4d) invalid_file();
  file_length = chunk_buffer.next;

  while (input_file->tellg() < file_length) {
    switch (chunk_buffer.id) {
      case 0x4d4d:  // Main chunks
      case 0x3d3d:  // Mesh block
      case 0xb000:  // Keyframer block
      case 0xb002:  // Object keyframer information
        break;

      case 0x4000:  // Object description block
        next_object = s;
        s = new(Object);
        s->next = next_object;
        s->read_name(input_file);
        break;

      case 0x4100:  // Triangular mesh object
        s->set_type(Object::mesh);
        break;

      case 0x4110:  // Vertex list
        input_file->read((char *)&uword_buffer, sizeof(uword));
        s->num_vertices = uword_buffer[0];
        s->vertex_table = new(Vector[s->num_vertices]);

        for (count = 0; count <= s->num_vertices - 1; count++)
          *input_file >> s->vertex_table[count];
        break;

      case 0x4120:  // Face list
        input_file->read((char *)&uword_buffer, sizeof(uword));
        s->num_faces = uword_buffer[0];
        s->face_table = new(Face[s->num_faces]);

        for(count = 0; count <= s->num_faces - 1; count++)
          *input_file >> s->face_table[count];
        break;

      case 0x4140:  // Mapping coordinates
        s->mapped = 1;
        input_file->read((char *)&uword_buffer, sizeof(uword));
        s->num_vertices = uword_buffer[0];
        s->mapping_table = new(MapVector[s->num_vertices]);

        for(count = 0; count <= s->num_vertices - 1; count++)
          *input_file >> s->mapping_table[count];
        break;

      case 0x4150:  // Smoothing groups
        s->smoothing_table = new(udword[s->num_faces]);

        for(count = 0; count <= s->num_faces - 1; count++)
          input_file->read((char *)&s->smoothing_table[count], sizeof(udword));
        break;

      case 0x4160:  // Translation matrix
        *input_file >> s->translation_matrix;
        break;

      case 0x4600:  // Light
        s->set_type(Object::omni_light);
        *input_file >> s->position;
        break;

      case 0x4610:  // Spotlight
        s->set_type(Object::spotlight);
        *input_file >> s->target;
        input_file->read((char *)&float_buffer, 2 * sizeof(float));
        s->hotspot = float_buffer[0];
        s->falloff = float_buffer[1];
        break;

      case 0x4700:  // Camera
        s->set_type(Object::camera);
        s->position.read(*input_file);
        s->target.read(*input_file);
        input_file->read((char *)&float_buffer, 2 * sizeof(float));
        s->bank_angle = float_buffer[0];
        s->lens_size = float_buffer[1];
        break;

      case 0xb020:  // Object Position
        input_file->read(char_buffer, 10);
        input_file->read((char *)&s->num_keys.position, sizeof(udword));
        s->position_key_table = new(position_key[s->num_keys.position]);

        for (count = 0; count <= s->num_keys.position - 1; count++) {
          input_file->read((char *)&s->position_key_table[count].frame,
            sizeof(udword));
          input_file->read(char_buffer, 2);
          s->position_key_table[count].position.read(*input_file);
        }
        break;

      case 0xb021:  // Object Rotation
        input_file->read(char_buffer, 10);
        input_file->read((char *)&s->num_keys.rotation, sizeof(udword));
        s->rotation_key_table = new(rotation_key[s->num_keys.rotation]);

        for (count = 0; count <= s->num_keys.rotation - 1; count++) {
          input_file->read((char *)&s->rotation_key_table[count].frame,
            sizeof(udword));
          input_file->read(char_buffer, 2);
          input_file->read((char *)&float_buffer, sizeof(float));
          s->rotation_key_table[count].angle = float_buffer[0];
          *input_file >> s->rotation_key_table[count].axis;
        }
        break;

      case 0xb022:  // Object Scale
        input_file->read(char_buffer, 10);
        input_file->read((char *)&s->num_keys.scale, sizeof(udword));
        s->scale_key_table = new(scale_key[s->num_keys.scale]);

        for (count = 0; count <= s->num_keys.scale - 1; count++) {
          input_file->read((char *)&s->scale_key_table[count].frame,
            sizeof(udword));
          input_file->read(char_buffer, 2);
          *input_file >> s->scale_key_table[count].scale;
        }
        break;

      default:  // Undefined chunk
        warning(chunk_buffer.id, input_file->tellg() - sizeof(chunk));
        input_file->seekg(chunk_buffer.next - sizeof(chunk), ios::cur);
    }
    input_file->read((char *)&chunk_buffer, sizeof(chunk));
  }

  return(s);
}
// [fold]  ]

/*
 * Output
 */

// [fold]  [
void display_info( Object *current_object ) {

    Object *next_object;

    while( current_object != NULL ) {
        cout << current_object->name() << endl << dec;
        switch( current_object->type() ) {
            case Object::undefined:
                cout << "  Warning! Invalid object" << endl;
                break;

            case Object::mesh:
                cout << "  Tri-mesh" << endl;
                cout << "  Vertices:   " << current_object->num_vertices << endl;
                cout << "  Faces:      " << current_object->num_faces << endl;
                break;

            case Object::omni_light:
                cout << "  Omni light" << endl;
                cout << "  Position:   X=" << current_object->position.x() <<
                                     " Y=" << current_object->position.y() <<
                                     " Z=" << current_object->position.z() << endl;
                break;

            case Object::spotlight:
                cout << "  Spotlight" << endl;
                cout << "  Position:   X=" << current_object->position.x() <<
                                     " Y=" << current_object->position.y() <<
                                     " Z=" << current_object->position.z() << endl;
                cout << "  Target:     X=" << current_object->target.x() <<
                                     " Y=" << current_object->target.y() <<
                                     " Z=" << current_object->target.z() << endl;
                cout << "  Hotspot:    " << current_object->hotspot << endl;
                cout << "  Falloff:    " << current_object->falloff << endl;
                break;

            case Object::camera:
                cout << "  Camera" << endl;
                cout << "  Position:   X=" << current_object->position.x() <<
                                     " Y=" << current_object->position.y() <<
                                     " Z=" << current_object->position.z() << endl;
                cout << "  Target:     X=" << current_object->target.x() <<
                                     " Y=" << current_object->target.y() <<
                                     " Z=" << current_object->target.z() << endl;
                cout << "  Bank angle: " << current_object->bank_angle << endl;
                cout << "  Lens size:  " << current_object->lens_size << endl;
                break;
        }
        cout << endl;
        next_object = current_object->next;
        free( current_object );
        current_object = next_object;
    }
}
// [fold]  ]

// [fold]  [
void print_data( ofstream *output_file, ofstream *header_file,
                 ofstream *include_file, String *file_name, Object *s ) {

  Object *next_object;
  int count, object_count;


  *output_file

<< "/*"                                                     << endl
<< " * " << *file_name << ".cpp"                            << endl
<< " *"                                                     << endl
<< " * Generated automatically by 3ds2cpp"                  << endl
<< " * Autodesk 3D Studio mesh file to C++ array converter" << endl
<< " * Copyright (C) Seppo Enarvi  1996,1997"               << endl
<< " *"                                                     << endl
<< " * Fractional part is " << FRAC_BITS << " bits"         << endl
<< " */"                                                    << endl
                                                            << endl
<< "#include \"vector.h\""                                  << endl
<< "#include \"object.h\""                                  << endl
<< "#include \"" << *file_name << ".h\""                    << endl
                                                            << endl
                                                            << endl;

  *header_file

<< "/*"                                                     << endl
<< " * " << *file_name << ".h"                              << endl
<< " *"                                                     << endl
<< " * Generated automatically by 3ds2cpp"                  << endl
<< " * Autodesk 3D Studio mesh file to C++ array converter" << endl
<< " * Copyright (C) Seppo Enarvi  1996,1997"               << endl
<< " */"                                                    << endl
                                                            << endl
<< "#ifndef _" << file_name->upper() << "_H_INCLUDED"       << endl
                                                            << endl
<< "#include \"vector.h\""                                  << endl
                                                            << endl
                                                            << endl;

  *include_file

<< ";"                                                      << endl
<< "; " << *file_name << ".inc"                             << endl
<< ";"                                                      << endl
<< "; Generated automatically by 3ds2cpp"                   << endl
<< "; Autodesk 3D Studio mesh file to C++ array converter"  << endl
<< "; Copyright (C) Seppo Enarvi  1996,1997"                << endl
<< ";"                                                      << endl
                                                            << endl
<< "        include \"vector.inc\""                         << endl
                                                            << endl
                                                            << endl;


  object_count = 1;
  while( s != NULL ) {
    if( s->type() == Object::mesh ) {


      String s_vertices = s->name().upper();
      s_vertices += "_VERTICES";
      String s_faces = s->name().upper();
      s_faces += "_FACES";

      String s_name = s->name();

      udword last_vertex = s->num_vertices - 1;
      udword last_face = s->num_faces - 1;


      /*
       * Vertices
       */

      *output_file

<< "Vector " << s_name << "_vertex_table[" << s_vertices << "] = {" << endl
<< hex << setiosflags( ios::showbase );

      for( count = 0; count <= last_vertex - 1; count++ )
        *output_file

/*<< "{" << (udword)(s->vertex_table[ count ].x() * MUL) << ", "
       << (udword)(s->vertex_table[ count ].y() * MUL) << ", "
       << (udword)(s->vertex_table[ count ].z() * MUL) << "}," << endl;*/
       << s->vertex_table[count] << "," << endl;

      *output_file

<< "{" << (udword)(s->vertex_table[ last_vertex ].x() * MUL) << ", "
       << (udword)(s->vertex_table[ last_vertex ].y() * MUL) << ", "
       << (udword)(s->vertex_table[ last_vertex ].z() * MUL) << "}" << endl
<< "};" << endl << endl
<< dec << resetiosflags( ios::showbase );


      /*
       * Faces
       */

      *output_file << "Face " << s_name << "_face_table[" << s_faces
                   << "] = {" << endl;

      for( count = 0; count <= last_face - 1; count++ )
        *output_file << s->face_table[count] << "," << endl;

      *output_file << s->face_table[last_face] << endl
                   << "};" << endl << endl;


      /*
       * Mapping coordinates
       */

      if( s->mapped ) {
        *output_file

<< "MapVector " << s_name << "_mapping_table[" << s_vertices << "] = {"
<< endl << hex << setiosflags( ios::showbase );

        for( count = 0; count <= last_vertex - 1; count++ )
          *output_file

<< "{" << (udword)(s->mapping_table[ count ].u() * MUL) << ", "
       << (udword)(s->mapping_table[ count ].v() * MUL) << "}," << endl;

        *output_file

<< "{" << (udword)(s->mapping_table[ last_vertex ].u() * MUL) << ", "
       << (udword)(s->mapping_table[ last_vertex ].v() * MUL) << "}" << endl
<< "};" << endl << endl
<< dec << resetiosflags( ios::showbase );

      }


      /*
       * Smoothing groups
       */

      *output_file

<< "Fixed " << s_name << "_smoothing_table[" << s_faces << "] = {"
<< endl << hex;

      for( count = 0; count <= last_face - 1; count++ )
        *output_file

<< "0x" << setw( 8 ) << setfill( '0' )
<< s->smoothing_table[ count ] << "," << endl;

      *output_file

<< "0x" << setw( 8 ) << setfill( '0' )
<< s->smoothing_table[ last_face ] << endl
<< "};" << endl << endl
<< dec;


      /*
       * Translation matrix
       */

      *output_file

<< "Fixed " << s_name << "_translation_matrix[4][4] = {" << endl
<< s->translation_matrix
<< "};" << endl << endl;


      /*
       * Position keys
       */

      *output_file

<< "PositionKey " << s_name << "_position_table[" << s->num_keys.position
<< "] = {" << endl;

      for(count = 0; count <= (signed long)(s->num_keys.position - 2); count++)
        *output_file

<< "{" << s->position_key_table[count].frame << ", "
       << s->position_key_table[count].position << "}," << endl;

      *output_file

<< "{" << s->position_key_table[count].frame << ", "
       << s->position_key_table[count].position << "}" << endl
<< "};" << endl << endl;


      /*
       * Rotation keys
       */

      *output_file

<< "RotationKey " << s_name << "_rotation_table[" << s->num_keys.rotation
<< "] = {" << endl;

      for(count = 0; count <= (signed long)(s->num_keys.rotation - 2); count++)
        *output_file

<< "{" << s->rotation_key_table[count].frame << ", "
       << s->rotation_key_table[count].axis << ", "
       << s->rotation_key_table[count].angle << "}," << endl;

      *output_file

<< "{" << s->rotation_key_table[count].frame << ", "
       << s->rotation_key_table[count].axis << ", "
       << s->rotation_key_table[count].angle << "}" << endl
<< "};" << endl << endl;


      /*
       * Scale keys
       */

      *output_file

<< "ScaleKey " << s_name << "_scale_table[" << s->num_keys.scale
<< "] = {" << endl;

      for(count = 0; count <= (signed long)(s->num_keys.scale - 2); count++)
        *output_file

<< "{" << s->scale_key_table[count].frame << ", "
       << s->scale_key_table[count].scale << "}," << endl;

      *output_file

<< "{" << s->scale_key_table[count].frame << ", "
       << s->scale_key_table[count].scale << "}" << endl
<< "};" << endl << endl;


      /*
       * Object data structure
       */

      *output_file

<< "objectdata " << s_name << "_data = {"                               << endl


<< "    " << s_name << "_face_table,"                                   << endl
<< "    " << s_name << "_vertex_table,"                                 << endl;

      if( s->mapped )
        *output_file

<< "    " << s_name << "_mapping_table,"                                << endl;

      else
        *output_file

<< "    NULL,"                                                          << endl;

      *output_file

<< "    " << s_name << "_smoothing_table,"                              << endl
                                                                        << endl
<< "    normal_table_" << object_count << ","                           << endl
                                                                        << endl
<< "    " << s_faces << " - 1, "
          << s_vertices << " - 1,"                                      << endl
<< "    (" << s_faces << " - 1) * sizeof(Face),"                        << endl
<< "    (" << s_vertices << " - 1) * sizeof(Vector),"                   << endl
                                                                        << endl
<< "    {0, 0, 2000},"                                                  << endl
<< "    {0, 0}"                                                         << endl
<< "};"                                                                 << endl
                                                                        << endl
                                                                        << endl;

      *header_file

<< "extern objectdata " << s_name << "_data;"                           << endl
                                                                        << endl
<< "#define " << s_vertices << " " << s->num_vertices                   << endl
<< "#define " << s_faces << " " << s->num_faces                         << endl
                                                                        << endl;

      *include_file

<< "        global  " << s_name << "_data:objectdata"                   << endl
                                                                        << endl
<< s_vertices << " = " << s->num_vertices                               << endl
<< s_faces << " = " << s->num_faces                                     << endl;


      object_count++;
    }
    next_object = s->next;
    free( s );
    s = next_object;
  }

  *header_file << endl
               << "#define _" << file_name->upper() << "_H_INCLUDED" << endl
               << "#endif" << endl;
}
// [fold]  ]

/*
 * Main program
 */

// [fold]  [
int main(int argc, char *argv[]) {

  ifstream input_file;
  ofstream output_file, header_file, include_file;
  String file_name;

  Object *next_object;


  init(argc, argv, &input_file, &output_file, &header_file, &include_file);
  file_name = argv[1];
  print_data(&output_file, &header_file, &include_file, &file_name,
             read_data(&input_file));

  input_file.close();
  output_file.close();
  header_file.close();
  include_file.close();
  return(0);
}
// [fold]  ]
// [fold]  43
