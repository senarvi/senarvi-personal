#include <math.h>

#include "video.h"
#include "_math.h"
#include "tunnel.h"
#include "data.h"


#define X_OFF           (GR_X / 2 - 1)
#define Y_OFF           (GR_Y / 2 - 1)
#define TUNNEL_RADIUS   90
#define TUNNEL_LENSDIST 160
#define TUNNEL_MUL      (TUNNEL_RADIUS * TUNNEL_LENSDIST)


// [fold]  [
void set_tunnel() {

    pixel count;
    udword position, depth;

    // Both axes = 0 can't be calculated to avoid divide by zero

    for( count.y = 1; count.y <= Y_OFF; count.y++ )
        for( count.x = 0; count.x <= X_OFF; count.x++ ) {

            position = (Y_OFF + count.y) * GR_X + X_OFF + count.x;

            if( count.y <= count.x )
                depth = costable[ angletab[ position ] + 3 ] * TUNNEL_MUL / count.x >> SIN_FRACBITS;
            else
                depth = sintable[ angletab[ position ]     ] * TUNNEL_MUL / count.y >> SIN_FRACBITS;

/*            if( count.y <= count.x )
                eax = (udword)(
                    cos( (double)(angletab[ edi ] + 3) / 256 * PI2 ) *
                    TUNNEL_MUL /
                    count.x);
            else
                eax = (udword)(
                    sin( (double)angletab[ edi ] / 256 * PI2 ) *
                    TUNNEL_MUL /
                    count.y);*/

            tunneldepth[ position                                   ] = depth / 2;
            tunneldepth[ (Y_OFF + count.y) * GR_X + X_OFF - count.x ] = depth / 2;

            tunneldepth[ (Y_OFF - count.y) * GR_X + X_OFF + count.x ] = depth / 2;
            tunneldepth[ (Y_OFF - count.y) * GR_X + X_OFF - count.x ] = depth / 2;
        };

    // Horizontal line in the middle (y = 0)
    // Origin can't be calculated to avoid divide by zero

    count.y = 1;
    for( count.x = 1; count.x <= X_OFF; count.x++ ) {
        position = (Y_OFF + count.y) * GR_X + X_OFF + count.x;
        depth = costable[ angletab[ position ] + 3 ] * TUNNEL_MUL / count.x >> SIN_FRACBITS;

        tunneldepth[ position                                   - GR_X ] = depth / 2;
        tunneldepth[ (Y_OFF + count.y) * GR_X + X_OFF - count.x - GR_X ] = depth / 2;
    }

    // Right side

    count.x = X_OFF + 1;
    for( count.y = 0; count.y <= Y_OFF; count.y++ ) {
        position = (Y_OFF + count.y) * GR_X + X_OFF + count.x;
        depth = costable[ angletab[ position ] + 3 ] * TUNNEL_MUL / count.x >> SIN_FRACBITS;

        tunneldepth[ position                                   ] = depth / 2;
        tunneldepth[ (Y_OFF - count.y) * GR_X + X_OFF + count.x ] = depth / 2;
    }

    // Bottom

    count.y = Y_OFF + 1;
    for( count.x = 0; count.x <= X_OFF; count.x++ ) {
        position = (Y_OFF + count.y) * GR_X + X_OFF + count.x;
        depth = sintable[ angletab[ position ] ] * TUNNEL_MUL / count.y >> SIN_FRACBITS;

        tunneldepth[ position                                   ] = depth / 2;
        tunneldepth[ (Y_OFF + count.y) * GR_X + X_OFF - count.x ] = depth / 2;
    }

    // Origin

    tunneldepth[ Y_OFF * GR_X + X_OFF ] = 0x0fffffff;

    // Lower right corner

    count.x = X_OFF + 1;
    count.y = Y_OFF + 1;
    position = (Y_OFF + count.y) * GR_X + X_OFF + count.x;
    depth = costable[ angletab[ position ] + 3 ] * TUNNEL_MUL / count.x >> SIN_FRACBITS;
    tunneldepth[ position ] = depth / 2;

    // Middle right

    count.x = X_OFF + 1;
    count.y = 0;
    position = (Y_OFF + count.y) * GR_X + X_OFF + count.x;
    depth = costable[ 63 + 3 ] * TUNNEL_MUL / count.x >> SIN_FRACBITS;
    tunneldepth[ position ] = depth / 2;
}
// [fold]  ]
// [fold]  1
