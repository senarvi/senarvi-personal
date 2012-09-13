// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-
//
/// \author Seppo Enarvi  senarvi@iki.fi
/// \date 2003 - 2005
//
// Abstract data types and algorithms of analytic geometry

#include <boost/test/included/unit_test_framework.hpp>
#include <boost/smart_ptr.hpp>

#include "cartesian_vector_test.h"


using boost::unit_test::test_suite;
using namespace boost;


test_suite *
init_unit_test_suite(int argc, char * argv[])
{
	test_suite * tests(BOOST_TEST_SUITE("geomtests"));
	
	shared_ptr<cartesian_vector_test> cvt(new cartesian_vector_test());
	tests->add(BOOST_CLASS_TEST_CASE(&cartesian_vector_test::run, cvt));
	
	return tests;
}
