#ifndef __MATH_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include "system.h"


#define SIN_FRACBITS 14
// SIN_ENTRIES must be power of two
#define SIN_ENTRIES  256


#define E	    2.71828182846
#define PI	    3.14159265359

#define PI2	    6.28318530718

#define SQRT_2	    1.41421356237
#define SQRT_PI2    2.50662827463


extern udword sintable[SIN_ENTRIES];
extern udword costable[SIN_ENTRIES];

extern void calculate_sin_tables();


#define __MATH_H_INCLUDED
#ifdef __cplusplus
};
#endif
#endif
