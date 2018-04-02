#ifndef _FIXED_H_INCLUDED

#include "system.h"


#define _FB 16					// fractional part bits /
#define _FR (1 << _FB)				// multiplier


#ifdef _FIXED_CLASS
#include <iostream.h>

/*
 * fixed point decimal number class
 */

class Fixed {

    sdword value;

public:

    // constructors

    Fixed();					// default
    Fixed( Fixed const & );			// from another Fixed
    Fixed( sdword );				// from an integer

    // destructor

    ~Fixed();

    // conversion operator

    operator sdword () const;			// to integer

    // assignment operators

    Fixed &operator = ( Fixed const & );	// Fixed = Fixed
    Fixed &operator = ( sdword );		// Fixed = integer
    Fixed &operator += ( Fixed const & );	// Fixed += Fixed
    Fixed &operator += ( sdword );		// Fixed += integer
    Fixed &operator -= ( Fixed const & );	// Fixed -= Fixed
    Fixed &operator -= ( sdword );		// Fixed -= integer
    Fixed &operator *= ( sdword );		// Fixed *= integer
    Fixed &operator /= ( sdword );		// Fixed /= integer

    // increment and decrement operators

    Fixed operator ++ ();			// increment
    Fixed operator -- ();			// decrement

    // unary operators

    Fixed operator + ();
    Fixed operator - ();			// negation

    // binary operators

    friend Fixed operator + ( Fixed const &,	// Fixed + Fixed
			      Fixed const & );
    friend Fixed operator + ( Fixed const &,	// Fixed + integer
			      sdword );
    friend Fixed operator + ( sdword,		// integer + Fixed
			      Fixed const & );
    friend Fixed operator - ( Fixed const &,	// Fixed - Fixed
			      Fixed const & );
    friend Fixed operator - ( Fixed const &,	// Fixed - integer
			      sdword );
    friend Fixed operator - ( sdword,		// integer - Fixed
			      Fixed const & );
    friend Fixed operator * ( Fixed const &,	// Fixed * Fixed
			      Fixed const & );
    friend Fixed operator * ( Fixed const &,	// Fixed * integer
			      sdword );
    friend Fixed operator * ( sdword,		// integer * Fixed
			      Fixed const & );
    friend Fixed operator / ( Fixed const &,	// Fixed / Fixed
			      Fixed const & );
    friend Fixed operator / ( Fixed const &,	// Fixed / integer
			      sdword );
    friend Fixed operator / ( sdword,		// integer / Fixed
			      Fixed const & );

    // comparsion operators

    friend int operator == ( Fixed const &,	// Fixed == Fixed?
			     Fixed const & );
    friend int operator == ( Fixed const &,	// Fixed == integer?
			     sdword );
    friend int operator == ( sdword,		// integer == Fixed?
			     Fixed const & );
    friend int operator != ( Fixed const &,	// Fixed != Fixed?
			     Fixed const & );
    friend int operator != ( Fixed const &,	// Fixed != integer?
			     sdword );
    friend int operator != ( sdword,		// integer != Fixed?
			     Fixed const & );
    friend int operator <  ( Fixed const &,	// Fixed < Fixed?
			     Fixed const & );
    friend int operator <  ( Fixed const &,	// Fixed < integer?
			     sdword );
    friend int operator <  ( sdword,		// integer < Fixed?
			     Fixed const & );
    friend int operator <= ( Fixed const &,	// Fixed <= Fixed?
			     Fixed const & );
    friend int operator <= ( Fixed const &,	// Fixed <= integer?
			     sdword );
    friend int operator <= ( sdword,		// integer <= Fixed?
			     Fixed const & );
    friend int operator >  ( Fixed const &,	// Fixed > Fixed?
			     Fixed const & );
    friend int operator >  ( Fixed const &,	// Fixed > integer?
			     sdword );
    friend int operator >  ( sdword,		// integer > Fixed?
			     Fixed const & );
    friend int operator >= ( Fixed const &,	// Fixed >= Fixed?
			     Fixed const & );
    friend int operator >= ( Fixed const &,	// Fixed >= integer?
			     sdword );
    friend int operator >= ( sdword,		// integer >= Fixed?
			     Fixed const & );

    // i/o operators

    friend istream &operator >> ( istream &,	// input
				  Fixed & );
    friend ostream &operator << ( ostream &,	// output
				  Fixed const & );
};

/*
 * constructors
 */

inline Fixed::Fixed() {
    value = 0;
}

inline Fixed::Fixed( Fixed const &__s ) {
    value = __s.value;
}

inline Fixed::Fixed( sdword __s ) {
    value = __s << _FB;
}

/*
 * assignment operators
 */

inline Fixed &Fixed::operator = ( Fixed const &__s ) {
    value = __s.value;
    return( *this );
}

inline Fixed &Fixed::operator = ( sdword __s ) {
    value = __s << _FB;
    return( *this );
}

inline Fixed &Fixed::operator += ( Fixed const &__s ) {
    value += __s.value;
    return( *this );
}

inline Fixed &Fixed::operator += ( sdword __s ) {
    value += __s << _FB;
    return( *this );
}

inline Fixed &Fixed::operator -= ( Fixed const &__s ) {
    value += __s.value;
    return( *this );
}

inline Fixed &Fixed::operator -= ( sdword __s ) {
    value += __s << _FB;
    return( *this );
}

inline Fixed &Fixed::operator *= ( sdword __s ) {
    value *= __s;
    return( *this );
}

inline Fixed &Fixed::operator /= ( sdword __s ) {
    value /= __s;
    return( *this );
}

/*
 * increment and decrement operators
 */

inline Fixed Fixed::operator ++ () {
    value += 1 << _FB;
    return( *this );
}

inline Fixed Fixed::operator -- () {
    value -= 1 << _FB;
    return( *this );
}

/*
 * unary operators
 */

inline Fixed Fixed::operator + () {
    return( *this );
}

inline Fixed Fixed::operator - () {
    value = -value;
    return( *this );
}

#else
typedef sdword Fixed;
#endif


#define _FIXED_H_INCLUDED
#endif
