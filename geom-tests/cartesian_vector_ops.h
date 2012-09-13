// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

/// \brief Operations for cartesian_vector.
/// \author Seppo Enarvi  senarvi@iki.fi
/// \date 2003 - 2005

#ifndef CARTESIAN_VECTOR_OPS_H
#define CARTESIAN_VECTOR_OPS_H


#include <iterator>	 // iterator_traits
#include <cmath>  // M_PI, cos, sin, acos, sqrt
#include <boost/type_traits/is_float.hpp>


namespace geom {	
	
	
	/// \brief Returns true iff a vector is a zero vector.
	/// \param v A vector.
	/// \return True if v is a zero vector, false otherwise.
	///
	template <typename T>
	inline bool
	is_zero(const cartesian_vector<T, 2> & v)
	{ return v.x() == 0 && v.y() == 0; }
	template <typename T>
	inline bool
	is_zero(const cartesian_vector<T, 3> & v)
	{ return v.x() == 0 && v.y() == 0 && v.z() == 0; }
	
	/// \brief Returns the length (magnitude) of a vector.
	/// \param v A vector.
	/// \return A double reprsenting the length of v.
	///
	/// Square root is calculated using doubles, since it's faster to use FPU.
	///
	template <typename T>
	inline double
	abs(const cartesian_vector<T, 2> & v)
	{ return std::sqrt(double(dot_product(v, v))); }
	
	/// \brief Normalizes the vector to a specific length (by default 1).
	/// \pre !is_zero(v)
	/// \exception std::invalid_argument is_zero(v)
	///	
	template <typename T, unsigned int D, typename ScalarT>
	inline void
	normalize(cartesian_vector<T, D> & v, ScalarT length = 1)
	{
		if (is_zero(v))
			throw std::invalid_argument
				("a zero vector given to normalize(v, length)");
		
		v /= abs(v);
	}
	
	/// \brief Rotates a vector a given angle about z axis.
	/// \param v A vector to be rotated.
	/// \param angle The angle in radians, how much the vector will be rotated.
	///
	/// Calculation is done using doubles. Using an integral type would be too
	/// imprecise to be of any use.
	///
	template <typename T>
	inline void
	rotate(cartesian_vector<T, 2> & v, double angle)
	{
		double c = std::cos(angle);
		double s = std::sin(angle);
		
		// A temporary variable is needed.
		v = cartesian_vector<T, 2>(static_cast<T>(c * v.x() - s * v.y()),
		                           static_cast<T>(s * v.x() + c * v.y()));
	}
	
	
	/// \brief Returns the distance between two vectors.
	/// \param v A vector.
	/// \param w A vector.
	/// \return The distance between v and w.
	///
	/// Square root is calculated using doubles, since it's faster to use FPU.
	///
	template <typename T, typename U, unsigned int D>
	inline double
	distance(const cartesian_vector<T, D> & v,
			 const cartesian_vector<U, D> & w)
	{ return abs(v - w); }
	
	/// \brief Returns the angle between v and w in radians, in the range
	/// [0, 2 pi[
	/// \pre !is_zero(v) && !is_zero(w)
	/// \param v A vector.
	/// \param w A vector.
	/// \return A double representing the angle between v and w in radians.
	/// \exception std::invalid_argument is_zero(v) || is_zero(w)
	///	
	/// The angle is returned as a double. Using an integral type would be too
	/// imprecise to be of any use.
	///
	template <typename T, typename U, unsigned int D>
	inline double
	angle(const cartesian_vector<T, D> & v,
		  const cartesian_vector<U, D> & w)
	{
		double abs_product = abs(v) * abs(w);
		
		if (abs_product == 0)
			throw std::invalid_argument("zero vectors given to angle(v, w)");
		
		return std::acos(double(dot_product(v, w)) / abs_product);
	}
	
	/// \brief Returns true iff the two vectors are parallel to each other.
	/// \param v A vector.
	/// \param w A vector.
	/// \return True if v and w are parallel to each other, false otherwise.
	///
	template <typename T, typename U>
	inline bool
	are_parallel(const cartesian_vector<T, 2> & v,
				 const cartesian_vector<U, 2> & w)
	{ return v.x() * w.y() == v.y() * w.x(); }
	
	
	/// \brief Calculates the angle of the vector relative to the positive x
	/// axis about the z axis.
	/// \param v A vector.
	/// \return A double representing the angle of v relative to the positive x
	/// axis about the z axis (a positive angle indicates counter-clockwise
	/// rotation when looking from the positive z axis).
	///
	/// Calculation is done using doubles. Using an integral type would be too
	/// imprecise to be of any use.
	///
	template <typename T>
	inline double
	orientation(const cartesian_vector<double, 2> & v)
	{
		double abs_v = abs(v);
		
		if (abs_v == 0)
			throw std::invalid_argument
				("a zero vector given to orientation(v)");
		
		// atan(y / x) is not a good idea if y >> x.
		double theta = std::acos(v.x() / abs_v);
		
		if (v.y() < 0)
			return (2 * M_PI) - theta;
		else
			return theta;
	}
	
	/// \brief Return value is
	///	  - > 0, if v1, v2 and v3 form a counter-clockwise oriented triangle,
	///	  - < 0, if v1, v2 and v3 form a clockwise oriented triangle and
	///	  - 0 otherwise.
	///
	template <typename T, typename U, typename V, unsigned int D>
	inline T
	orientation(const cartesian_vector<T, D> & v1,
				const cartesian_vector<U, D> & v2,
				const cartesian_vector<V, D> & v3)
	{
		cartesian_vector<T, D> v(v1);
		v1 -= v2;
		cartesian_vector<T, D> w(v3);
		v3 -= v2;
		return (w.x() * v.y()) - (w.y() * v.x());
	}
	
	/// \brief Returns true, iff the angle between v and w, theta, realizes 0 <
	/// theta < pi (that is, vector w is on the left side when looking at
	/// the direction of v).
	///
	template <typename T, typename U>
	inline bool
	positively_oriented(const cartesian_vector<T, 2> & v,
						const cartesian_vector<U, 2> & w)
	{ return (v.x() * w.y()) - (v.y() * w.x()) > 0; }
	
	/// \brief Returns true, iff the angle between v and w, theta, realizes pi <
	/// theta < 2 pi (that is, vector w is on the right side when looking
	/// at the direction of v).
	///
	template <typename T, typename U>
	inline bool
	negatively_oriented(const cartesian_vector<T, 2> & v,
						const cartesian_vector<U, 2> & w)
	{ return (v.x() * w.y()) - (v.y() * w.x()) < 0; }
	
	
	/// \brief Returns the orthogonal projection of a vector from a D-
	///        dimensional vector space V1 to a D-1 -dimensional subspace V2.
	/// \param v A vector that will be projected.
	/// \param origin The origin of V2.
	/// \param b The basis of V2 relative to origin.
	/// \return The orthogonal projection of v onto b relative to origin.
	/// \pre b is a basis in V2, i.e. it spans V2 and the vectors are linearly
	///      independent.
	/// \exception std::invalid_argument Vectors b is not a basis.
	///
	template <typename T, typename U, typename V>
	inline cartesian_vector<T, 2>
	orthogonal_projection(cartesian_vector<T, 2> v,
	                      const cartesian_vector<U, 2> & origin,
	                      cartesian_vector<V, 2> b)
	{
		if (b.is_zero())
			throw std::invalid_argument
				("a zero vector given as the basis to "
				 "orthogonal_projection(v, origin, b)");
		
		normalize(b);
		v -= origin;
		return dot_product(v, b) * b;
	}
	template <typename T, typename U, typename V, typename W>
	inline cartesian_vector<T, 3>
	orthogonal_projection(cartesian_vector<T, 3> v,
	                      const cartesian_vector<U, 3> & origin,
	                      cartesian_vector<V, 3> b1,
	                      cartesian_vector<W, 3> b2)
	{
		if (are_parallel(b1, b2))
			throw std::invalid_argument
				("the basis given to orthogonal_projection(v, origin, b1, b2) "
				 "does not span a 2-dimensional vector space");
		
		normalize(b1);
		normalize(b2);
		v -= origin;
		// TODO: Make b1, b2 orthogonal.
		return
			dot_product(v, b1) * b1 +
			dot_product(v, b2) * b2;
	}
	
	
	template <typename T, unsigned int D, typename Iter>
	inline bool
	inside_polygon(const cartesian_vector<T, D> & v,
				   Iter polygon_begin,
				   Iter polygon_end)
	{
		typedef typename std::iterator_traits<Iter>::value_type vector_type;
		typedef typename vector_type::scalar_type scalar_type;
		
		bool result = false;
		scalar_type x = v.x();
		scalar_type y = v.y();
		
		Iter i(polygon_begin);
		if (i == polygon_end)
			throw std::invalid_argument
				("an empty polygon given to "
				 "inside_polygon(v, polygon_begin, polygon_end)");
		Iter j(polygon_end - 1);
		
		while (i != polygon_end) {
			// Cast a ray in the direction of the positive y axis.
			scalar_type edge_width = j->x() - i->x();
			scalar_type edge_height = j->y() - i->y();
			scalar_type edge_x =
				i->x() + edge_width * (y - i->y()) / edge_height;
			
			if ((((i->y() <= y) && (y() < j->y)) ||
				 ((j->y() <= y) && (y() < i->y))) &&
				(x < edge_x))
				//				result = !result;
				return true;			
			j = i++;
		}
		return result;
	}
	
	template <typename T, unsigned int D, typename Iter>
	inline bool
	inside_convex_polygon(const cartesian_vector<T, 2> & v,
						  Iter polygon_begin,
						  Iter polygon_end)
	{
		typedef typename std::iterator_traits<Iter>::value_type vector_type;
		typedef typename vector_type::scalar_type scalar_type;
		
		bool result = false;
		scalar_type x = v.x();
		scalar_type y = v.y();
		
		Iter i(polygon_begin);
		if (i == polygon_end)
			throw std::invalid_argument
				("an empty polygon given to "
				 "inside_convex_polygon(v, polygon_begin, polygon_end)");
		Iter j(polygon_end - 1);
		
		while (i != polygon_end) {
			// Cast a ray in the direction of the positive y axis.
			scalar_type edge_width = j->x() - i->x();
			scalar_type edge_height = j->y() - i->y();
			scalar_type edge_x =
				i->x() + edge_width * (y() - i->y()) / edge_height;
			
			if ((((i->y() <= y()) && (y < j->y())) ||
				 ((j->y <= y()) && (y < i->y()))) &&
				(x < edge_x))
				{
					if (result == false)
						result = true;
					else
						// Line can pass through at most two edges of a convex
						// polygon.
						return false;
				}

			j = i++;
		}
		return result;
	}
	
	
}
	

#endif
