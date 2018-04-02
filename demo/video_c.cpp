#include <conio.h>
#include <math.h>

#include "system.h"
#include "fixed.h"
#include "video.h"


#define F_OUT (1 << 8)


ubyte *video_memory = (ubyte *)0xa0000;

extern "C" Fixed x_mul, y_mul;


/*
 * Set linear gradiation from source color to destination color to the
 * palette.
 */
// [fold]  [

void set_linear_palette(fcolor *src_color, fcolor *dest_color,
    udword base_color, udword last_color, udword max_value) {

  udword index;
  udword max_color = last_color - base_color;
  fcolor count;
  fcolor increment;

  increment.red = (dest_color->red - src_color->red) / max_color;
  increment.green = (dest_color->green - src_color->green) / max_color;
  increment.blue = (dest_color->blue - src_color->blue) / max_color;

  // Interpolate color values
  count = *src_color;

  // index = 0..1 in number of colors steps
  for (index = 0; index <= max_color; index++) {
    outp(0x3c8, (ubyte)(base_color + index));

    outp(0x3c9, (ubyte)(count.red * max_value));
    outp(0x3c9, (ubyte)(count.green * max_value));
    outp(0x3c9, (ubyte)(count.blue * max_value));

    count.red += increment.red;
    count.green += increment.green;
    count.blue += increment.blue;
  }
}
// [fold]  ]

/*
 * Calculate video parameters.
 */
// [fold]  [

void calculate_video_parameters(int x_numpixels, int y_numpixels,
    double output_aspect, double field_of_vision) {

  double video_aspect = (double)x_numpixels / y_numpixels;
  double x_correction = video_aspect / output_aspect;
  double y_correction = 1;
  double width = (double)x_numpixels / x_correction;
  double lens_distance = (width / 2) / tan(field_of_vision / 2);
  double x_multiplier = x_correction * lens_distance;
  double y_multiplier = y_correction * lens_distance;


  x_mul = (Fixed)(x_multiplier * _FR);
  y_mul = (Fixed)(y_multiplier * _FR);
}
// [fold]  ]

// [fold]  2
