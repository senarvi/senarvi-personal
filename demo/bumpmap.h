#ifndef _BUMPMAP_H_INCLUDED
/*#ifdef __cplusplus
extern "C" {
#endif*/

#include "system.h"
#include "video.h"
#include "vector.h"


extern "C" udword heightmap[ GR_SIZE ];
extern "C" angle bump_light;
extern "C" udword wave_add, wave_amplitude;
extern material wave_material;


extern "C" void _bumpmap();
extern "C" void _makewave();


#define _BUMPMAP_H_INCLUDED
/*#ifdef __cplusplus
};
#endif*/
#endif
