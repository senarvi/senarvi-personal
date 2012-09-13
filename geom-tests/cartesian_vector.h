// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

/// \class cartesian_vector cartesian_vector.h geom/cartesian_vector.h
/// \brief cartesian_vector<T, D> is a vector with D cartesian coordinates of
///        type T.
/// \author Seppo Enarvi  senarvi@iki.fi
/// \date 2003 - 2005
///
/// The class is optimized for speed, assuming that T does not throw exceptions.
/// Some operations assume a right-handed, i.e. positively oriented, basis. The
/// scalar type of the result of an operation is determined by the first scalar
/// parameter, except when the operation would be to imprecise to perform using
/// an integral type, in which case it is always double. There is also an
/// implicit type conversion from a vector with the same dimension, but possibly
/// different scalar type. Of course, implicit conversion is done only for the
/// arguments of non-template functions.

#ifndef GEOM_CARTESIAN_VECTOR_H
#define GEOM_CARTESIAN_VECTOR_H


#include <iostream>	 // ostream, basic_istream
#include <stdexcept>  // invalid_argument
#include <sstream>	// ostringstream


namespace geom {
	
	
	template <typename T, unsigned int D>
	struct cartesian_vector {};
	
	
	// 2D cartesian_vector
	
	template <typename T>
	class cartesian_vector<T, 2>
	{
		// type definitions
		
	public:
		/// The type of a single coordinate.
		typedef T value_type;
		typedef T scalar_type;
		/// The type of this class.
		typedef cartesian_vector<T, 2> vector_type;
		
		
		// constructors
		
		/// \brief Creates a zero vector.
		///
		cartesian_vector()
			: x_(0), y_(0) {}
		
		/// \brief Creates a vector with given coordinates.
		/// \param x_arg Initial x coordinate.
		/// \param y_arg Initial y coordinate.
		///
		cartesian_vector(scalar_type x_arg, scalar_type y_arg)
			: x_(x_arg), y_(y_arg) {}
		
		/// \brief Explicit or implicit conversion from another vector with the
		///        same dimension, but possibly different scalar type.
		/// \param v A vector where the coordinates will be copied from.
		///
		template <typename U>
		cartesian_vector(const cartesian_vector<U, 2> & v)
			: x_(v.x()), y_(v.y()) {}
		
		
		// coordinates
		
		/// \brief Returns a reference to the x coordinate.
		/// \return A reference to the x coordinate.
		///
		scalar_type & x() { return x_; }
		const scalar_type & x() const { return x_; }
		
		/// \brief Returns a reference to the y coordinate.
		/// \return A reference to the y coordinate.
		///
		scalar_type & y() { return y_; }
		const scalar_type & y() const { return y_; }
		
		
		// operators
		
		/// \brief vector *= scalar
		/// \param s A scalar multiplier.
		/// \return A reference to this object.
		///
		vector_type & operator*=(scalar_type s)
		{ x_ *= s; y_ *= s; return *this; }
		
		/// \brief vector /= scalar
		/// \pre s != 0
		/// \param s A scalar divisor.
		/// \return A reference to this object.
		/// \exception std::invalid_argument s == 0
		///	
		vector_type & operator/=(scalar_type s)
		{
			if (s == 0)
				throw std::invalid_argument
					("zero divisor given to cartesian_vector::operator/=(s)");
			
			x_ /= s;
			y_ /= s;
			return *this;
		}
		
		/// \brief vector += vector
		/// \param v A vector addend.
		/// \return A reference to this object.
		///
		vector_type & operator+=(const cartesian_vector<T, 2> & v)
		{ x_ += v.x(); y_ += v.y(); return *this; }
		
		/// \brief vector -= vector
		/// \param v A vector subtrahend.
		/// \return A reference to this object.
		///
		vector_type & operator-=(const cartesian_vector<T, 2> & v)
		{ x_ -= v.x(); y_ -= v.y(); return *this; }
		
		
		// private data
		
	private:
		/// \brief A coordinate of the vector.
		scalar_type x_, y_;
	};
	
	
	/// \brief +vector
	/// \param v A vector operand.
	/// \return v
	///
	template <typename T, unsigned int D>
	inline cartesian_vector<T, D>
	operator+(const cartesian_vector<T, D> & v)
	{ return v; }
	
	/// \brief -vector
	/// \param v A vector operand.
	/// \return -v
	///
	template <typename T, unsigned int D>
	inline cartesian_vector<T, D>
	operator-(const cartesian_vector<T, D> & v)
	{ return cartesian_vector<T, D>(-v.x(), -v.y()); }
	
	/// \brief vector == vector
	/// \param v A vector lhs operand.
	/// \param w A vector rhs operand.
	/// \return v == w
	///	
	template <typename T, typename U>
	inline bool
	operator==(const cartesian_vector<T, 2> & v,
			  const cartesian_vector<U, 2> & w)
	{ return v.x() == w.x() && v.y() == w.y(); }
	template <typename T, typename U>
	inline bool
	operator==(const cartesian_vector<T, 3> & v,
			  const cartesian_vector<U, 3> & w)
	{ return v.x() == w.x() && v.y() == w.y() && v.z() == w.z(); }
	
	/// \brief vector != vector
	/// \param v A vector lhs operand.
	/// \param w A vector rhs operand.
	/// \return v != w
	///	
	template <typename T, typename U>
	inline bool
	operator!=(const cartesian_vector<T, 2> & v,
			  const cartesian_vector<U, 2> & w)
	{ return v.x() != w.x() || v.y() != w.y(); }
	template <typename T, typename U>
	inline bool
	operator!=(const cartesian_vector<T, 3> & v,
			  const cartesian_vector<U, 3> & w)
	{ return v.x() != w.x() || v.y() != w.y() || v.z() != w.z(); }
	
	/// \brief vector * scalar
	/// \param v A vector multiplicand.
	/// \param s A scalar multiplier.
	/// \return v * s
	///	
	template <typename T, unsigned int D, typename ScalarT>
	inline cartesian_vector<T, D>
	operator*(const cartesian_vector<T, D> & v, ScalarT s)
	{
		cartesian_vector<T, D> result(v);
		result *= s;
		return result;
	}
	
	/// \brief scalar * vector
	/// \param s A scalar multiplicand.
	/// \param v A vector multiplier.
	/// \return s * v
	///	
	template <typename ScalarT, typename T, unsigned int D>
	inline cartesian_vector<ScalarT, D>
	operator*(ScalarT s, const cartesian_vector<T, D> & v)
	{
		cartesian_vector<ScalarT, D> result(v);
		result *= s;
		return result;
	}
	
	/// \brief vector / scalar
	/// \pre s != 0
	/// \param v A vector dividend.
	/// \param s A scalar divisor.
	/// \return v / s
	/// \exception std::invalid_argument s == 0
	///	
	template <typename T, unsigned int D, typename ScalarT>
	inline cartesian_vector<T, D>
	operator/(const cartesian_vector<T, D> & v, ScalarT s)
	{
		if (s == 0)
			throw std::invalid_argument
				("zero divisor given to operator/(v, s)");
		
		cartesian_vector<T, D> result(v);
		result /= s;
		return result;
	}
	
	/// \brief vector . vector
	/// \param v A vector multiplicand.
	/// \param w A vector multiplier.
	/// \return v . w
	///
	template <typename T, typename U>
	inline T
	dot_product(const cartesian_vector<T, 2> & v,
				const cartesian_vector<U, 2> & w)
	{
		return v.x() * w.x() + v.y() * w.y();
	}
	template <typename T, typename U>
	inline T
	dot_product(const cartesian_vector<T, 3> & v,
				const cartesian_vector<U, 3> & w)
	{
		return v.x() * w.x() + v.y() * w.y() + v.z() * w.z();
	}
	
	/// \brief vector x vector
	/// \param v A vector multiplicand.
	/// \param w A vector multiplier.
	/// \return v x w
	///
	template <typename T, typename U>
	inline T
	cross_product(const cartesian_vector<T, 2> & v,
	              const cartesian_vector<U, 2> & w)
	{
		return v.x() * w.y() - v.y() * w.x();
	}
	template <typename T, typename U>
	inline cartesian_vector<T, 3>
	cross_product(const cartesian_vector<T, 3> & v,
	              const cartesian_vector<U, 3> & w)
	{
		return cartesian_vector<T, 3>(v.y() * w.z() - v.z() * w.y(),
		                              v.z() * w.x() - v.x() * w.z(),
		                              v.x() * w.y() - v.y() * w.x());
	}
	
	/// \brief vector + vector
	/// \param v A vector lhs addend.
	/// \param w A vector rhs addend.
	/// \return v + w
	///	
	template <typename T, typename U, unsigned int D>
	inline cartesian_vector<T, D>
	operator+(const cartesian_vector<T, D> & v,
			  const cartesian_vector<U, D> & w)
	{
		cartesian_vector<T, D> result(v);
		result += w;
		return result;
	}
	
	/// \brief vector - vector
	/// \param v A vector subtractor.
	/// \param w A vector subtrahend.
	/// \return v - w
	///
	template <typename T, typename U, unsigned int D>
	inline cartesian_vector<T, D>
	operator-(const cartesian_vector<T, D> & v,
			  const cartesian_vector<U, D> & w)
	{
		cartesian_vector<T, D> result(v);
		result -= w;
		return result;
	}
	
	
	/// \brief Writes the coordinates of a vector to a stream.
	/// \param s An output stream.
	/// \param v A vector that will be written to the stream.
	/// \return s
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
	template <typename T>
	inline std::ostream &
	operator<<(std::ostream & s, const cartesian_vector<T, 3> & v)
	{
		std::ostringstream oss;
		oss.flags(s.flags());
		oss.imbue(s.getloc());
		oss.precision(s.precision());
		oss << '(' << v.x() << "," << v.y() << "," << v.z() << ')';
		return s << oss.str();
	}
	
	/// \brief Reads the coordinates of a vector from a stream.
	/// \param s An input stream.
	/// \param v A vector where the read coordinates will be stored in.
	/// \return s
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
	template <typename T, typename CharT, class Traits>
	inline std::basic_istream<CharT, Traits> &
	operator>>(std::basic_istream<CharT, Traits> & s,
			   cartesian_vector<T, 3> & v)
	{
		using namespace std;
		
		T x, y, z;
		CharT ch;
		
		s >> ch;
		if (ch == '(') {
			s >> x >> ch;
			if (ch == ',') {
				s >> y >> ch;
				if (ch == ',') {
					s >> z >> ch;
					if (ch == ')')
						v = cartesian_vector<T, 3>(x, y, z);
					else
						s.setstate(ios_base::failbit);
				}
				else
					s.setstate(ios_base::failbit);
			}
			else
				s.setstate(ios_base::failbit);
		}
		else {
			s.putback(ch);
			s >> x >> y >> z;
			v = cartesian_vector<T, 3>(x, y, z);  // Atomic assignment.
		}
		
		return s;
	}
	
	
}


#endif
