// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

/// \brief Unit test cases for cartesian_vector.
/// \author Seppo Enarvi  senarvi@iki.fi
/// \date 2005

#ifndef CARTESIAN_VECTOR_TEST_H
#define CARTESIAN_VECTOR_TEST_H


#include <sstream>
#include <vector>

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "geom.h"


class cartesian_vector_test
{
	typedef geom::cartesian_vector<int, 2> int_xy;
	typedef geom::cartesian_vector<float, 2> float_xy;
	typedef geom::cartesian_vector<double, 2> double_xy;
	
public:
	void test_int_xy()
	{
		// default initialization
		int_xy v;
		BOOST_CHECK_EQUAL(v.x(), 0);
		BOOST_CHECK_EQUAL(v.y(), 0);
		
		// is_zero()
		BOOST_CHECK(is_zero(v));
		
		// assignments
		v = int_xy(1, -2);
		BOOST_CHECK_EQUAL(v.x(), 1);
		BOOST_CHECK_EQUAL(v.y(), -2);
		v *= 2;
		BOOST_CHECK_EQUAL(v.x(), 2);
		BOOST_CHECK_EQUAL(v.y(), -4);
		v /= -2;
		BOOST_CHECK_EQUAL(v.x(), -1);
		BOOST_CHECK_EQUAL(v.y(), 2);
		v += int_xy(2, -2);
		BOOST_CHECK_EQUAL(v.x(), 1);
		BOOST_CHECK_EQUAL(v.y(), 0);
		v -= int_xy(-2, 2);
		BOOST_CHECK_EQUAL(v.x(), 3);
		BOOST_CHECK_EQUAL(v.y(), -2);
		
		BOOST_CHECK_THROW(v /= 0, std::invalid_argument);
		
		BOOST_CHECK_EQUAL(v.x(), 3);
		BOOST_CHECK_EQUAL(v.y(), -2);
		
		v = +v;
		BOOST_CHECK_EQUAL(v.x(), 3);
		BOOST_CHECK_EQUAL(v.y(), -2);
		v = -v;
		BOOST_CHECK_EQUAL(v.x(), -3);
		BOOST_CHECK_EQUAL(v.y(), 2);
		v = v * 2;
		BOOST_CHECK_EQUAL(v.x(), -6);
		BOOST_CHECK_EQUAL(v.y(), 4);
		
		BOOST_CHECK_EQUAL(3.5 * v, float_xy(-21,14));
		
		BOOST_CHECK_EQUAL(v.x(), -6);
		BOOST_CHECK_EQUAL(v.y(), 4);
		v = v / 2;
		BOOST_CHECK_EQUAL(v.x(), -3);
		BOOST_CHECK_EQUAL(v.y(), 2);
		int_xy w(9, -6);
		BOOST_CHECK_EQUAL(dot_product(v, w), (-3 * 9) + (2 * -6));
		
		std::ostringstream oss;
		oss << v;
		BOOST_CHECK_EQUAL(oss.str(), "(-3,2)");
		
		std::istringstream iss("(-17,89)");
		iss >> v;
		BOOST_CHECK_EQUAL(v.x(), -17);
		BOOST_CHECK_EQUAL(v.y(), 89);
		
		rotate(v, geom::degrees_to_radians(90));
		BOOST_CHECK(std::abs(v.x() - (-89)) <= 1);
		BOOST_CHECK(std::abs(v.y() - (-17)) <= 1);
	}
	
	void test_double_xy()
	{
		double_xy v(2, 4);
		float_xy v30 = float(30) * v;
		BOOST_CHECK_EQUAL(v30.x(), 60);
		BOOST_CHECK_EQUAL(v30.y(), 120);
		
		// implicit type conversion.
		int_xy w(3, 5);
		v += w;
		BOOST_CHECK_EQUAL(v.x(), 5);
		BOOST_CHECK_EQUAL(v.y(), 9);
		v -= w;
		BOOST_CHECK_EQUAL(v.x(), 2);
		BOOST_CHECK_EQUAL(v.y(), 4);
		
		// assignment
		v = w;
		BOOST_CHECK_EQUAL(v.x(), 3);
		BOOST_CHECK_EQUAL(v.y(), 5);
	}
	
	void test_vector()
	{
		std::vector<int_xy> v(5);
		std::vector<int_xy> w(5, int_xy(3, -5));
	}
	
	void run()
	{
		test_int_xy();
		test_double_xy();
		test_vector();
    }
};


#endif
