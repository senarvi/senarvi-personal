// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

/// \class numeric_ops numeric_ops.h geom/numeric_ops.h
/// \brief A traits class that performs the numeric operations of <cmath>,
///        converting between integral and floating-point types if necessary.
/// \author Seppo Enarvi  senarvi@iki.fi
/// \date 2003 - 2005

#ifndef NUMERIC_OPS_H
#define NUMERIC_OPS_H

#include <cmath>


namespace geom {
	
	
	/// \brief A policy class that selects a floating-point implementation for
	///        the numeric operations from the C standard library. The global
	///        functions are overloaded for float, double and long double, so
	///        passing an integral value would be ambiguous.
	///
	/// The type of the result is also a template type, but the default type is
	/// that of the argument. static_cast is used to cast the result. This
	/// allows avoiding a useless cast if an operation is performed on an
	/// integral variable, but the result is wanted as another type of variable.
	///
	template <typename Arg,
	          typename Result = Arg,
	          bool IsFloat = boost::is_float<Arg>::value>
	struct numeric_ops {};
	template <typename Arg, typename Result>
	struct numeric_ops<Arg, Result, true>
	{
		static Result sqrt(Arg x)
		{ return static_cast<Result>(std::sqrt(x)); }
	};
	template <typename Arg, typename Result>
	struct numeric_ops<Arg, Result, false>
	{
		static Result sqrt(Arg x)
		{ return static_cast<Result>(std::sqrt(double(x))); }
	};
	
	
}
	

#endif
