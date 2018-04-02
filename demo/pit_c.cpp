#include <conio.h>

#include "system.h"
#include "pit.h"


void set_pit_freq( uword frequency ) {
    outp( 0x43, 0 );
    outp( 0x40, (ubyte)(PIT_FREQUENCY / frequency & 0x00ff) );
    outp( 0x40, (ubyte)(PIT_FREQUENCY / frequency >> 8) );
}

void reset_pit_freq() {
    outp( 0x43, PIT_CONTROLW );
    outp( 0x40, 0 );
    outp( 0x40, 0 );
}
