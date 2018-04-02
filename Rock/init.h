#ifndef _init_h_
#define _init_h_

#include "hardware.h"

//---------------------------------------------------------------------------
#define SPEED 30

#define PIT_CONTROL ((0 << 6) + (3 << 4) + (2 << 1) + (0 << 0))
#define PIT_COUNTER (0x1234dd / SPEED)

#define EXIT_BREAK  -1
#define EXIT_NORMAL 0
#define EXIT_USER   1
#define EXIT_NOMEM  2

//---------------------------------------------------------------------------
extern int exittype;

extern void init();

#endif /* !_init_h_ */

