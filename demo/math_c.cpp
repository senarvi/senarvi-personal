#include <math.h>

#include "system.h"
#include "_math.h"


void calculate_sin_tables() {

    udword count;

    for( count = 0; count <= SIN_ENTRIES - 1; count++ )
        sintable[ count ] = (udword)(
            sin( (double)count / SIN_ENTRIES * PI2 ) * (1 << SIN_FRACBITS)
        );

    for( count = SIN_ENTRIES / 4 * 3; count <= SIN_ENTRIES - 1; count++ )
        costable[ count ] = (udword)(
            cos( (double)count / SIN_ENTRIES * PI2 ) * (1 << SIN_FRACBITS)
        );
}
