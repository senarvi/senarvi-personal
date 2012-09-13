// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-
//
/// \author Seppo Enarvi  senarvi@iki.fi
/// \date 2003 - 2005
//
// Abstract data types and algorithms of analytic geometry

#ifndef GEOM_CARTESIAN_VECTOR_H
#define GEOM_CARTESIAN_VECTOR_H


#include <iostream>  // ostream, basic_istream
#include <iterator>  // iterator_traits
#include <cmath>
#include <stdexcept>  // invalid_argument
#include <sstream>  // ostringstream

#include <boost/type_traits/is_float.hpp>


namespace geom {
	
	
	template <typename T, bool FP = boost::is_float<T>::value>
	struct policy {};
	
	template <typename T>
	struct policy<T, true>
	{
		static T square_root(T x)
		{ return std::sqrt(x); }
	};
	
	
	namespace detail {
		
		
		/// \brief A generic class that calculates the square root of an integer
		/// or a floating point number.
		///
		template <typename T, bool FP = boost::is_float<T>::value>
		struct square_root {};
		
		template <typename T>
		struct square_root<T, true>
		{
			static T calculate(T x)
			{ return std::sqrt(x); }
		};
		
		template <typename T>
		struct square_root<T, false>
		{
			static T calculate(T x)
			{ return static_cast<T>(std::sqrt(static_cast<double>(x))); }
		};
		
		/*
		template <bool FP>
		struct orientation {};
		
		template <>
		struct orientation<true>
		{
			template <typename T>
			static T calculate(T x, T y)
			{
				if ((x == 0) && (y == 0))
					throw invalid_argument(
						"a zero vector given to "
						"orientation<true>::calculate(x, y)");
				
				// atan(y / x) is not a good idea if y >> x.
				scalar_type result = std::acos(x / length());
				if (y < 0)
					result = (2*M_PI) - result;
				
				return result;
			}
		};
		
		template <>
		struct orientation<false>
		{
			template <typename T>
			static T calculate(T x, T y)
			{
				if ((x == 0) && (y == 0))
					throw invalid_argument(
						"a zero vector given to "
						"orientation<false>::calculate(x, y)");
				
				double x_float = x;
				double y_float = y;
				
				// atan(y / x) is not a good idea if y >> x.
				double result = std::acos(x / length<true>(x, y));
					std::acos(static_cast<double>(x) / length<false>(x, y));
				if (y < 0)
					result = (2*M_PI) - result;
				
				return result;
			}
		};
		*/
		
	}
	
	
	/// \brief A vector with D cartesian coordinates of type T.
	///
	/// Optimized for speed, assuming that T does not throw exceptions.
	///
	template <typename T, unsigned int D>
	struct cartesian_vector {};
	
	
	/// \brief A vector with 2 cartesian coordinates of type T.
	///
	/// Optimized for speed, assuming that T does not throw exceptions.
	///
	template <typename T>
	struct cartesian_vector<T, 2>
	{
		typedef T value_type;
		typedef value_type scalar_type;
		typedef cartesian_vector<value_type, 2> vector_type;
		
		
	public:
	    /// \brief Creates a cartesian_vector<T, 2> object with x = y = 0.
	    ///
	    cartesian_vector()
	    	: x_(0), y_(0) {}
		
	    /// \brief Creates a cartesian_vector<T, 2> object with given x and y
	    /// coordinates.
	    ///
	    /// \param x_arg Initial x coordinate.
	    /// \param y_arg Initial y coordinate.
	    ///
	    cartesian_vector(scalar_type x_arg, scalar_type y_arg)
	    	: x_(x_arg), y_(y_arg) {}
		
		/// \brief Creates a cartesian_vector<T, 2> object from a
		/// cartesian_vector<U, 2> object.
		///
		template <typename U>
		cartesian_vector(const cartesian_vector<U, 2> & v)
			: x_(v.x()), y_(v.y()) {}
		
		
		scalar_type & x() { return x_; }
		const scalar_type & x() const { return x_; }
		scalar_type & y() { return y_; }
		const scalar_type & y() const { return y_; }
		
		
		/// \brief vector = vector
		///
		template <typename U>
		vector_type & operator=(const cartesian_vector<U, 2> & v)
		{ x_ = v.x(); y_ = v.y(); return *this; }
		
	    /// \brief vector *= scalar
	    ///
	    vector_type & operator*=(scalar_type s)
	    { x_ *= s; y_ *= s; return *this; }
		
	    /// \brief vector /= scalar
	    ///
		/// \pre { s != 0 }
		///	
	    vector_type & operator/=(scalar_type s)
	    {
			if (s == 0)
				throw std::invalid_argument(
					"zero divisor given to "
					"cartesian_vector<T, 2>::operator/=(s)");
			
			x_ /= s;
			y_ /= s;
			return *this;
		}
		
	    /// \brief vector += vector
	    ///
	    /// The type of the coordinates of the rhs vector is not a separate
	    /// template type, since then it would be problematic to decide the
	    /// return type.
	    ///
	    vector_type & operator+=(const vector_type & v)
	    { x_ += v.x(); y_ += v.y(); return *this; }
		
	    /// \brief vector -= vector
	    ///
	    /// The type of the coordinates of the rhs vector is not a separate
	    /// template type, since then it would be problematic to decide the
	    /// return type.
	    ///
	    vector_type & operator-=(const vector_type & v)
	    { x_ -= v.x(); y_ -= v.y(); return *this; }
	    
	    
	private:
	    /// \brief A component of the vector.
		scalar_type x_, y_;
	};
	
	
	// arithmetic
	
    /// \brief +vector
    ///
	template <typename T, unsigned int D>
	inline cartesian_vector<T, D>
	operator+(const cartesian_vector<T, D> & v)
	{ return v; }
	
	/// \brief -vector
	///
	template <typename T, unsigned int D>
	inline cartesian_vector<T, D>
	operator-(const cartesian_vector<T, D> & v)
    { return cartesian_vector<T, D>(-v.x(), -v.y()); }
	
	/// \brief vector * scalar
	///	
	template <typename T, unsigned int D, typename Scalar>
	inline cartesian_vector<T, D>
	operator*(const cartesian_vector<T, D> & v, Scalar s)
	{
		cartesian_vector<T, D> result(v);
		result *= s;
		return result;
	}
	
	/// \brief scalar * vector
	///	
	template <typename T, unsigned int D, typename Scalar>
	inline cartesian_vector<T, D>
	operator*(Scalar s, const cartesian_vector<T, D> & v)
	{ return v * s; }
	
	/// \brief vector / scalar
	///	
	/// \pre { s != 0 }
	///	
	template <typename T, unsigned int D, typename Scalar>
    inline cartesian_vector<T, D>
    operator/(const cartesian_vector<T, D> & v, Scalar s)
    {
    	if (s == 0)
    		throw std::invalid_argument(
    			"zero divisor given to operator/(v, s)");
    	
		cartesian_vector<T, D> result(v);
		result /= s;
		return result;
    }
	
	/// \brief vector . vector
    ///
    /// The type of the coordinates of the rhs vector is not a separate
    /// template type, since then it would be problematic to decide the
    /// return type.
	///
	template <typename T>
    inline T
    dot_product(const cartesian_vector<T, 2> & v,
                const cartesian_vector<T, 2> & w)
	{ return v.x() * w.x() + v.y() * w.y(); }
	
    /// \brief vector + vector
    ///	
    /// The type of the coordinates of the rhs vector is not a separate
    /// template type, since then it would be problematic to decide the
    /// return type.
	///
	template <typename T, unsigned int D>
    inline cartesian_vector<T, D>
    operator+(const cartesian_vector<T, D> & v,
              const cartesian_vector<T, D> & w)
    {
		cartesian_vector<T, D> result(v);
		result += w;
		return result;
    }
	
    /// \brief vector - vector
    ///	
    /// The type of the coordinates of the rhs vector is not a separate
    /// template type, since then it would be problematic to decide the
    /// return type.
	///
	template <typename T, unsigned int D>
    inline cartesian_vector<T, D>
    operator-(const cartesian_vector<T, D> & v,
              const cartesian_vector<T, D> & w)
    {
		cartesian_vector<T, D> result(v);
		result -= w;
		return result;
    }
	
	
	// operations
	
	/// \brief Returns true iff the vector is a zero vector.
	///
	template <typename T>
	inline bool
	is_zero(const cartesian_vector<T, 2> & v)
	{ return v.x() == 0 && v.y() == 0; }
	
	/// \brief Returns the length (magnitude) of the vector.
	///
	template <typename T>
    inline T
	abs(const cartesian_vector<T, 2> & v)
	{ return detail::square_root<T>::calculate(dot_product(v, v)); }
	
	
    /// \brief Calculates the angle of the vector relative to the positive x
    /// axis, in radians.
    ///
    /// In the usual case, where the positive x axis is pointing
    /// right and the positive y axis is pointing up, a positive angle
    /// indicates counter-clockwise rotation.
    ///
	template <typename T>
	inline T
	orientation(const cartesian_vector<T, 2> & v)
    { /*return detail::orientation<boost::is_float<scalar_type>::value>(x, y);*/ }
	
    /// \brief Returns true if the two vectors are parallel to each other.
    ///
	template <typename T, typename U>
	inline bool
	are_parallel(const cartesian_vector<T, 2> & v,
	             const cartesian_vector<U, 2> & w)
    { return v.x() * w.y() == v.y() * w.x(); }
	
    /// \brief Rotates a 2 dimensional vector angle degrees about z axis.
    ///
	template <typename T>
	inline void
	rotate(const cartesian_vector<T, 2> & v, T angle)
    {
    	typedef cartesian_vector<T, 2> vector_type;
    	typedef typename vector_type::scalar_type scalar_type;
    	
		// Convert the angle to radians.
		scalar_type radians = angle / 180 * M_PI;
		
		scalar_type c = std::cos(radians);
		scalar_type s = std::sin(radians);
		
		scalar_type new_x = v.x() * c - v.y() * s;
		scalar_type new_y = v.x() * s + v.y() * c;
		
		v = cartesian_vector<T, 2>(new_x, new_y);  // Atomic assignment.
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
			throw std::invalid_argument(
				"an empty polygon given to "
				"inside_polygon(v, polygon_begin, polygon_end)");
		Iter j(polygon_end - 1);
	    
		while (i != polygon_end) {
			// Cast a ray in the direction of the positive y axis.
			scalar_type edge_width = j->x() - i->x();
			scalar_type edge_height = j->y() - i->y();
			scalar_type edge_x = i->x() + edge_width * (y - i->y()) / edge_height;
			
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
			throw std::invalid_argument(
				"an empty polygon given to "
				"inside_convex_polygon(v, polygon_begin, polygon_end)");
		Iter j(polygon_end - 1);
		
		while (i != polygon_end) {
			// Cast a ray in the direction of the positive y axis.
			scalar_type edge_width = j->x() - i->x();
			scalar_type edge_height = j->y() - i->y();
			scalar_type edge_x = i->x() + edge_width * (y() - i->y()) / edge_height;
			
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
	
	/// Normalizes the vector to a specific length (by default 1).
	///	
	/// \pre { !is_zero(v) }
	///	
	template <typename T, unsigned int D, typename Scalar>
    inline void
    normalize(cartesian_vector<T, D> & v, Scalar length = 1)
    {
    	if (is_zero(v))
    		throw std::invalid_argument(
    			"a zero vector given to normalize(v, length)");
		
		v /= abs(v);
    }
    
    /// \brief Returns a unit vector in the direction of v.
    ///
    /// \pre { !is_zero(v) }
    ///	
    /// The direction from an object A to another object B can be specified as a
    /// vector with tail at A and head at B. However, since this vector has
    /// length equal to the distance between the objects in addition to encoding
    /// the direction from the first to the second, it is natural to instead
    /// consider the unit vector (sometimes called the direction vector), which
    /// decouples the distance from the direction.
    /// [http://mathworld.wolfram.com/Direction.html]
	///
	template <typename T, unsigned int D>
    inline cartesian_vector<T, D>
    direction(const cartesian_vector<T, D> & v)
    {
    	if (v.is_zero())
    		throw std::invalid_argument("a zero vector given to direction(v)");
		
    	cartesian_vector<T, D> result(v);
    	normalize(result);
    	return result;
    }
    
	/// \brief Returns the distance between two vectors.
	///
    /// The type of the coordinates of the second vector is not a separate
    /// template type, since then it would be problematic to decide the
    /// return type.
	///
	template <typename T, unsigned int D>
	inline T
	distance(const cartesian_vector<T, D> & v,
             const cartesian_vector<T, D> & w)
	{ return abs(v - w); }
	
	/// \brief Returns the angle between v and w in radians, in the range
	/// [0, 2 pi[
	///
    /// \pre { !is_zero(v) && !is_zero(w) }
    ///	
    /// The type of the coordinates of the second vector is not a separate
    /// template type, since then it would be problematic to decide the
    /// return type.
	///
	template <typename T, unsigned int D>
	inline T
	angle(const cartesian_vector<T, D> & v,
          const cartesian_vector<T, D> & w)
	{
		T abs_product = abs(v) * abs(w);
		
		if (abs_product == 0)
			throw std::invalid_argument("zero vectors given to angle(v, w)");
		
		return std::acos(dot_product(v, w) / abs_product);
	}
	
	
	/// \brief Returns true, iff the angle between v and w, theta, realizes 0 <
	/// theta < pi (that is, vector w is on the left side when looking at
	/// the direction of v).
	///
	template <typename T1, typename T2, unsigned int D>
	inline bool
	positively_oriented(const cartesian_vector<T1, D> & v,
	                    const cartesian_vector<T2, D> & w)
	{ return (v.x() * w.y()) - (v.y() * w.x()) > 0; }
	
	/// \brief Returns true, iff the angle between v and w, theta, realizes pi <
	/// theta < 2 pi (that is, vector w is on the right side when looking
	/// at the direction of v).
	///
	template <typename T1, typename T2, unsigned int D>
	inline bool
	negatively_oriented(const cartesian_vector<T1, D> & v,
	                    const cartesian_vector<T2, D> & w)
	{ return (v.x() * w.y()) - (v.y() * w.x()) < 0; }
	
	/// \brief Return value is
	///   - > 0, if v1, v2 and v3 form a counter-clockwise oriented triangle,
	///   - < 0, if v1, v2 and v3 form a clockwise oriented triangle and
	///   - 0 otherwise.
	///
    /// The type of the coordinates of the second and the third vector are not
    /// separate template types, since then it would be problematic to decide
    /// the return type.
	///
	template <typename T, unsigned int D>
	inline T
	orientation(const cartesian_vector<T, D> & v1,
	            const cartesian_vector<T, D> & v2,
	            const cartesian_vector<T, D> & v3)
	{
		cartesian_vector<T, D> v = v1 - v2;
		cartesian_vector<T, D> w = v3 - v2;
		return (w.x() * v.y()) - (w.y() * v.x());
	}
	
    /// \brief Returns the projection of v onto the line defined by a
    /// point on the line (point_on_line) and the direction of the line
    /// (line_direction), which need not be a unit vector.
    ///
    /// \pre { line_direction != 0 }
    ///	
	template <typename T, unsigned int D>
    inline cartesian_vector<T, D>
    projection(const cartesian_vector<T, D> & v,
	           const cartesian_vector<T, D> & point_on_line,
	           cartesian_vector<T, D> line_direction)
    {
    	if (line_direction.is_zero())
    		throw std::invalid_argument(
    			"a zero vector given as a line direction to projection()");
		
		normalize(line_direction);
		cartesian_vector<T, D> w = v - point_on_line;
		T length = dot_product(w, line_direction);
		return length * line_direction;
    }
	
	
	/// \brief Writes the components of a vector to a stream.
	///
	template <typename T>
	inline std::ostream &
	operator<<(std::ostream & s, const cartesian_vector<T, 2> & v)
	{
		std::ostringstream oss;
		oss.flags(s.flags());
		oss.imbue(s.getloc());
		oss.precision(s.precision());
		oss << '(' << v.x() << "," << v.y() << ')';
		return s << oss.str();
	}
	
	/// \brief Reads the components of a vector from a stream.
	///
	template <typename T, typename CharT, class Traits>
	inline std::basic_istream<CharT, Traits> &
	operator>>(std::basic_istream<CharT, Traits> & s,
	           cartesian_vector<T, 2> & v)
	{
		using namespace std;
		
		T x, y;
		CharT ch;
		
		s >> ch;
		if (ch == '(') {
			s >> x >> ch;
			if (ch == ',') {
				s >> y >> ch;
				if (ch == ')')
					v = cartesian_vector<T, 2>(x, y);
				else
					s.setstate(ios_base::failbit);
			}
			else
				s.setstate(ios_base::failbit);
		}
		else {
			s.putback(ch);
			s >> x >> y;
			v = cartesian_vector<T, 2>(x, y);  // Atomic assignment.
		}
		
		return s;
	}
	
	
}


#endif
