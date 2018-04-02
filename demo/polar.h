/*
 *  polar.h     Polar coordinates
 *
 *  Copyright (C) Seppo Enarvi, 1997
 */
#ifndef _POLAR_H_INCLUDED

#ifndef __cplusplus
#error polar.h is for use with C++
#endif

#if __WATCOMC__ > 1000
#pragma pack(__push,1);
#else
#pragma pack(1);
#endif
class Polar {
    double  __r;
    double  __a;

public:
    Polar();
    Polar( Polar const & );
    Polar( double __radius, __angle = 0.0 );
    Polar( Complex const & );

    double magnitude () const;
    double angle () const;
};
#if __WATCOMC__ > 1000
#pragma pack(__pop);
#else
#pragma pack();
#endif

inline Polar::Polar() {
    __r = 0.0;
    __a = 0.0;
}

inline Polar::Polar( Polar const &__cv ) {
    __r = __cv.__r;
    __a = __cv.__a;
}

inline Polar::Polar( double __radius, __angle ) {
    __r = __radius;
    __a = __angle;
}

inline Polar::Polar( Complex const &__cv ) {
    __r = sqrt( __cv.real() * __cv.real() + __cv.imag() * __cv.imag() );
    __a = atan( __cv.imag() / __cv.real() );
}

inline double Polar::magnitude() const {
    return( __r );
}

inline double Polar::angle() const {
    return( __a );
}

#define _POLAR_H_INCLUDED
#endif
