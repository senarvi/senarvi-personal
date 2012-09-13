#ifndef GEOM_H
#define GEOM_H


#include <cmath>  // M_PI

#include "cartesian_vector.h"
#include "cartesian_vector_ops.h"


namespace geom {
	
	
	/// \brief Convert an angle from degrees to radians.
	/// \param degrees An angle in degrees to be converted.
	/// \result The angle converted to radians.
	///
	/// Calculation is done using doubles. Using an integral type would be too
	/// imprecise to be of any use.
	///
	inline double
	degrees_to_radians(double degrees)
	{ return M_PI * degrees / 180; }
	
	
}


#endif
