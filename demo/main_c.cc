/*
 * ravage / STATiC, 1997, 1998
 *
 * Pmode/w has problems with:
 *   emm386 auto
 *   4dos xms swapping
 *
 * Greets:
 *  Tran
 *  Aloen
 *  Lobon
 */

#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <math.h>
#include <fstream.h>
#include <streambu.h>
#include <string.hpp>

#include "system.h"
#include "_math.h"
#include "vector.h"
#include "video.h"
#include "object.h"
#include "meshdata.h"
#include "flame.h"
#include "bumpmap.h"
#include "tunnel.h"
#include "kb.h"
#include "data.h"
#include "pit.h"


extern "C" void _texturemap(objectdata *__s);
extern "C" udword zbuffer[GR_X * GR_Y];


extern "C" void (intr *prev_int_08)(), (intr *prev_int_09)();
extern "C" void (intr pit_handler)(), (intr kb_handler)();
extern "C" volatile udword pause_flag;
extern "C" volatile udword exit_flag;
extern "C" void (*timer_chain)();


void texmap_timer();
void exit_function(), reset_vectors(), free_mesh_data();

ubyte *mesh_data;

volatile udword time_count;
volatile double rx;
volatile double ry;
volatile double rz;
volatile double ra;


void clear_zbuffer() {

  for (sdword ndx = GR_X * GR_Y - 1; ndx >= 0; ndx--) {
    zbuffer[ndx] = UDWORD_MAX;
  }
}

void sphere_loop(udword time) {

  set_normals(sphere1_data);
/*  set_normals(sphere2_data);
  set_normals(sphere3_data);*/
  set_phong_palette(&rock_material, 0, 255, 63);
  sphere1_data->pos.z = (500 << 8);

  time_count = 0;
  set_pit_freq(50);
  timer_chain = texmap_timer;


  ry = 0;
  rx = rz = 1 / sqrt(2);
  sphere1_data->rot.axis.x = (udword)(rx * 0x100);
  sphere1_data->rot.axis.y = (udword)(ry * 0x100);
  sphere1_data->rot.axis.z = (udword)(rz * 0x100);

/*  ry = 1;
  rx = rz = 0;
  sphere2_data->rot.axis.x = (udword)(rx * 0x100);
  sphere2_data->rot.axis.y = (udword)(ry * 0x100);
  sphere2_data->rot.axis.z = (udword)(rz * 0x100);

  rz = 1;
  rx = ry = 0;
  sphere3_data->rot.axis.x = (udword)(rx * 0x100);
  sphere3_data->rot.axis.y = (udword)(ry * 0x100);
  sphere3_data->rot.axis.z = (udword)(rz * 0x100);*/

  ra = 0;


  while (time_count < time) {
    _clearbuffer();
    clear_zbuffer();

    sphere1_data->rot.angle = (udword)(ra * 0x100);
/*    sphere2_data->rot.angle = (udword)(ra * 0x100);
    sphere3_data->rot.angle = (udword)(ra * 0x100);*/

    rotate_object(sphere1_data);
//    _sortfaces(sphere1_data);
    _texturemap(sphere1_data);

/*    rotate_object(sphere2_data);
    _sortfaces(sphere2_data);
    _texturemap(sphere2_data);

    rotate_object(sphere3_data);
    _sortfaces(sphere3_data);
    _texturemap(sphere3_data);*/

    _copybuffer();
    while (pause_flag != 0);
    if (exit_flag == 1) exit(EXIT_SUCCESS);
  }
}

void texmap_timer() {

    ra += 0.02;
    time_count++;
}

/*
 * the elite-alus
 */
// [fold]  [

/*void elus_fly( udword time ) {

    set_normals( &Object02_data );
    set_phong_palette( &rock_material, 0, 255, 63 );
    time_count = 0;

    set_pit_freq( 70 );
    timer_chain = texmap_timer;

    while( time_count < time ) {
//	  Object02_data.rotation.a = rotate_count.a / 10 & (SIN_ENTRIES - 1);
//	  Object02_data.rotation.b = rotate_count.b / 10 & (SIN_ENTRIES - 1);
//	  _rotate( &Object02_data );
	_sortfaces( &Object02_data );
	_clearbuffer();
	_texturemap( &Object02_data );
	_copybuffer();
	while( pause_flag != 0 );
    }
} */
// [fold]  ]

/*
 * flame
 */
// [fold]  [

void flame_loop( udword time ) {

    fcolor color1 = {0, 0, 0},
	   color2 = {1, 0, 0},
	   color3 = {1, 1, 0},
	   color4 = {1, 1, 1};

    udword time_count;

    set_linear_palette( &color1, &color2, 0,   31,  63 );
    set_linear_palette( &color2, &color3, 32,  63,  63 );
    set_linear_palette( &color3, &color4, 64,  127, 63 );
    set_phong_palette( &rock_material, 128, 255, 63 );

//    set_normals( &rock_data );
    _clearflamebuf();

    time_count = time;

    while( time_count != 0 ) {
/*	  rock_data.rotation.a += 3;
	rock_data.rotation.a &= SIN_ENTRIES - 1;
	rock_data.rotation.b += 5;
	rock_data.rotation.b &= SIN_ENTRIES - 1;
	_rotate( &rock_data );
	_sortfaces( &rock_data );*/

	_randomflame();
	_calculateflame();
	_clearbuffer();
//	  _texturemap( &rock_data );
	_copyflamebuf();
	_copybuffer();
	time_count--;
    }
}
// [fold]  ]

/*
 * bump map
 */
// [fold]  [

void bumpmap_loop( udword time ) {

    udword count, time_count;

    set_phong_palette( &wave_material, 0, 255, 63 );

    for( count = 0; count <= GR_SIZE - 1; count++ )
	heightmap[ count ] = stc_heightmap[ count ] * 8;

    count = 0;
    time_count = time;

    while( time_count != 0 ) {
	bump_light.a = sintable[ count ] >> (SIN_FRACBITS - 6);
	bump_light.b = costable[ count ] >> (SIN_FRACBITS - 6);
	count = (count + 1) & (SIN_ENTRIES - 1);

	_bumpmap();
	_copybuffer();

	time_count--;
    }
}
// [fold]  ]

/*
 * tunnel
 */
// [fold]  [

void tunnel_loop( udword time ) {

    fcolor color1 = {0, 0, 0},
	   color2 = {1, 0, 1};

    udword fwd_count = 0, roll_count = 0,
	   time_count;

    set_linear_palette( &color1, &color2, 0, 255, 63 );
    set_tunnel();

    time_count = time;
    while( time_count != 0 ) {
	_clearbuffer();
	_drawtunnel();
	_copybuffer();

	fwd_count += 15;
	roll_count += 15;

	tunnel_fwd = fwd_count;
	tunnel_roll = roll_count / 4;
	time_count--;
    }
}
// [fold]  ]

/*
 * wave bump map
 */
// [fold]  [

void wave_loop( udword time ) {

    udword time_count;

    wave_amplitude = 100;
    _setwave();
    set_phong_palette( &wave_material, 0, 255, 63 );

    time_count = time;

    while( time_count != 0 ) {
	_makewave();
	_bumpmap();
	_copybuffer();
	wave_add -= 40;
	time_count--;
    }
}
// [fold]  ]

void null_function() {}

/*
 * main program
 */

// [fold]  [
void main() {

  String const data_file_name = "demo.dat";


  // initialization

  atexit(exit_function);
  graph_mode();

  ifstream data_file;
  data_file.open(data_file_name, ios::in | ios::nocreate | ios::binary);
  data_file.seekg(0, ios::end);
  int data_size = data_file.tellg();
  data_file.seekg(0, ios::beg);
  if (!data_file.is_open()) {
    cerr << "File not found: " << data_file_name << endl;
    exit(EXIT_FAILURE);
  }
  mesh_data = new ubyte[data_size];
  if (mesh_data == NULL) {
    cerr << "Not enough memory" << endl;
    exit(EXIT_FAILURE);
  }
  atexit(free_mesh_data);
  data_file.read(mesh_data, data_size);
  data_file.close();
  set_object_data(mesh_data);

  prev_int_08 = _dos_getvect(0x08);
  prev_int_09 = _dos_getvect(0x09);
  atexit(reset_vectors);
  timer_chain = null_function;
  _dos_setvect(0x08, pit_handler);
  _dos_setvect(0x09, kb_handler);

  calculate_sin_tables();
  calculate_video_parameters(GR_X, GR_Y, (double)4 / 3, (double)45 / 360 * PI2);

  // main program

  sphere_loop(20 * 70);
/*  elus_fly(50 * 70);
  flame_loop(100);
  bumpmap_loop(50);
  wave_loop(40);
  tunnel_loop(100);*/
}
// [fold]  ]

// [fold]  [
void exit_function() {

  reset_pit_freq();
  text_mode();
}

void reset_vectors() {

  _dos_setvect(0x08, prev_int_08);
  _dos_setvect(0x09, prev_int_09);
}

void free_mesh_data() {

  free(mesh_data);
}
// [fold]  ]

// [fold]  7
