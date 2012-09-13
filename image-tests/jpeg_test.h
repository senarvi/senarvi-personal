// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

#ifndef JPEG_TEST_H
#define JPEG_TEST_H


#include <vector>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>  // system_complete

#include "image/jpeg_reader.h"
#include "image/jpeg_writer.h"
#include "image/color.h"


class jpeg_test
{
public:
	typedef unsigned char sample_type;
	typedef image::color_rgb<sample_type> color_type;
	typedef std::vector<color_type> image_type;
	
	void test_pattern(
		image_type::const_iterator upper_left,
		unsigned int image_width,
		unsigned int image_height)
	{
		image_type::const_iterator upper_right =
			upper_left + image_width - 1;
		image_type::const_iterator lower_left =
			upper_left + ((image_height - 1) * image_width);
		image_type::const_iterator lower_right =
			lower_left + image_width - 1;
		
		BOOST_CHECK_EQUAL(upper_left->r, 255);
		BOOST_CHECK_EQUAL(upper_left->g, 255);
		BOOST_CHECK_EQUAL(upper_left->b, 255);
		BOOST_CHECK_EQUAL(upper_right->r, 0);
		BOOST_CHECK_EQUAL(upper_right->g, 0);
		BOOST_CHECK_EQUAL(upper_right->b, 0);
		BOOST_CHECK_EQUAL(lower_left->r, 254);
		BOOST_CHECK_EQUAL(lower_left->g, 0);
		BOOST_CHECK_EQUAL(lower_left->b, 0);
		BOOST_CHECK_EQUAL(lower_right->r, 0);
		BOOST_CHECK_EQUAL(lower_right->g, 255);
		BOOST_CHECK_EQUAL(lower_right->b, 255);
	}
	
	void run()
	{
		using namespace std;
		using namespace boost::filesystem;
		using namespace image;
		
		path in_file = system_complete(path("../test_pattern.jpg", native));
		image_type in_image;
		unsigned int image_width;
		unsigned int image_height;
		open_jpeg_image(
			in_file, back_inserter(in_image), image_width, image_height);
		test_pattern(in_image.begin(), image_width, image_height);
		
		path out_file = system_complete(path("test_output.jpg", native));
		save_image_as_jpeg(out_file, in_image.begin(), 16, 16, 100);
		
		image_type out_image;
		open_jpeg_image(
			out_file, back_inserter(out_image), image_width, image_height);
		test_pattern(in_image.begin(), image_width, image_height);
	}
};

#endif
