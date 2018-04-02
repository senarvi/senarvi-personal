#ifndef _VIDEO_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include "system.h"


typedef struct pixeldef {
    udword x, y;
} pixel;

typedef struct colordef {
    udword red, green, blue;
} color;

typedef struct fcolordef {
    double red, green, blue;
} fcolor;


#define GR_X	  320
#define GR_Y	  200
#define GR_SIZE	  (GR_X * GR_Y)
#define GR_COLORS 256


extern ubyte *video_memory;
extern ubyte *destbufoff;


extern void graph_mode();
extern void text_mode();
extern void _clearbuffer();
extern void _copybuffer();
extern void set_linear_palette(fcolor *src_color, fcolor *dest_color,
	      udword base_color, udword max_color, udword max_value);
extern void calculate_video_parameters(int x_numpixels, int y_numpixels,
	      double output_aspect, double field_of_vision);


#pragma aux graph_mode = \
    "mov eax,13h", \
    "int 10h", \
    modify [eax];

#pragma aux text_mode = \
    "mov eax,3h", \
    "int 10h", \
    modify [eax];


#define _VIDEO_H_INCLUDED
#ifdef __cplusplus
};
#endif
#endif
