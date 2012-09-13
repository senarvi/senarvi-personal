// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_monitor.hpp>
#include <boost/smart_ptr.hpp>

#include "image/assertion_error.h"
#include "connected_components_test.h"
#include "jpeg_test.h"

using namespace std;
using namespace boost;
using namespace boost::unit_test;
using namespace image;


static void assertion_error_translator(const assertion_error & e)
{
	cerr << e.message() << endl;
    throw e;
}


template <typename TestClass>
void add_class_test(test_suite * ts_ptr, TestClass * tc_ptr)
{
	shared_ptr<TestClass> shared_tc_ptr(tc_ptr);
	ts_ptr->add(BOOST_CLASS_TEST_CASE(&TestClass::run, shared_tc_ptr));
}


test_suite * init_unit_test_suite(int argc, char* argv[])
{
	test_suite * ts_ptr = BOOST_TEST_SUITE("image_tests");
	
	unit_test_monitor.register_exception_translator<
		assertion_error>(&assertion_error_translator);
	
	add_class_test(ts_ptr, new connected_components_test());
	add_class_test(ts_ptr, new jpeg_test());
	
	return ts_ptr;
}
