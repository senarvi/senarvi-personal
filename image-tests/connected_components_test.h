// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

#ifndef CONNECTED_COMPONENTS_TEST_H
#define CONNECTED_COMPONENTS_TEST_H


#include <vector>
#include <boost/bind.hpp>

#include "image/connected_components.h"


class connected_components_test
{
public:
	void run()
	{
		using namespace std;
		using namespace image;
		
		static const unsigned int image_width = 5;
		static const unsigned int image_height = 5;
		static const unsigned int image_size = image_width * image_height;
		
		static const int image[image_size] = {
			0, 0, 1, 1, 0,
			0, 0, 0, 0, 1,
			0, 1, 1, 0, 0,
			0, 0, 1, 1, 1,
			1, 0, 1, 1, 0 };
		
		const int * image_first = image;
		const int * image_last = image + image_size;
		
		
		typedef vector<size_t> labels_type;
		labels_type labels(image_last - image_first);
		size_t num_components;
		
		find_true_false_components(
			image_first, image_last, image_width,
			boost::bind(std::equal_to<bool>(), _1, 1),
			labels.begin(),
			num_components,
			true);
		
		
		BOOST_CHECK_EQUAL(num_components, size_t(5));
		
		BOOST_CHECK_EQUAL(labels[0], size_t(0));
		BOOST_CHECK_EQUAL(labels[1], size_t(0));
		BOOST_CHECK_EQUAL(labels[2], size_t(1));
		BOOST_CHECK_EQUAL(labels[3], size_t(1));
		BOOST_CHECK_EQUAL(labels[4], size_t(0));
		
		BOOST_CHECK_EQUAL(labels[5], size_t(0));
		BOOST_CHECK_EQUAL(labels[6], size_t(0));
		BOOST_CHECK_EQUAL(labels[7], size_t(0));
		BOOST_CHECK_EQUAL(labels[8], size_t(0));
		BOOST_CHECK_EQUAL(labels[9], size_t(1));
		
		BOOST_CHECK_EQUAL(labels[10], size_t(0));
		BOOST_CHECK_EQUAL(labels[11], size_t(2));
		BOOST_CHECK_EQUAL(labels[12], size_t(2));
		BOOST_CHECK_EQUAL(labels[13], size_t(0));
		BOOST_CHECK_EQUAL(labels[14], size_t(0));
		
		BOOST_CHECK_EQUAL(labels[15], size_t(0));
		BOOST_CHECK_EQUAL(labels[16], size_t(0));
		BOOST_CHECK_EQUAL(labels[17], size_t(2));
		BOOST_CHECK_EQUAL(labels[18], size_t(2));
		BOOST_CHECK_EQUAL(labels[19], size_t(2));
		
		BOOST_CHECK_EQUAL(labels[20], size_t(3));
		BOOST_CHECK_EQUAL(labels[21], size_t(0));
		BOOST_CHECK_EQUAL(labels[22], size_t(2));
		BOOST_CHECK_EQUAL(labels[23], size_t(2));
		BOOST_CHECK_EQUAL(labels[24], size_t(4));
		
		
		find_true_components(
			image_first, image_last, image_width,
			boost::bind(std::equal_to<bool>(), _1, 1),
			labels.begin(),
			num_components,
			true);
		
		
		BOOST_CHECK_EQUAL(num_components, size_t(4));
		
		BOOST_CHECK_EQUAL(labels[0], size_t(0));
		BOOST_CHECK_EQUAL(labels[1], size_t(0));
		BOOST_CHECK_EQUAL(labels[2], size_t(1));
		BOOST_CHECK_EQUAL(labels[3], size_t(1));
		BOOST_CHECK_EQUAL(labels[4], size_t(0));
		
		BOOST_CHECK_EQUAL(labels[5], size_t(0));
		BOOST_CHECK_EQUAL(labels[6], size_t(0));
		BOOST_CHECK_EQUAL(labels[7], size_t(0));
		BOOST_CHECK_EQUAL(labels[8], size_t(0));
		BOOST_CHECK_EQUAL(labels[9], size_t(1));
		
		BOOST_CHECK_EQUAL(labels[10], size_t(0));
		BOOST_CHECK_EQUAL(labels[11], size_t(2));
		BOOST_CHECK_EQUAL(labels[12], size_t(2));
		BOOST_CHECK_EQUAL(labels[13], size_t(0));
		BOOST_CHECK_EQUAL(labels[14], size_t(0));
		
		BOOST_CHECK_EQUAL(labels[15], size_t(0));
		BOOST_CHECK_EQUAL(labels[16], size_t(0));
		BOOST_CHECK_EQUAL(labels[17], size_t(2));
		BOOST_CHECK_EQUAL(labels[18], size_t(2));
		BOOST_CHECK_EQUAL(labels[19], size_t(2));
		
		BOOST_CHECK_EQUAL(labels[20], size_t(3));
		BOOST_CHECK_EQUAL(labels[21], size_t(0));
		BOOST_CHECK_EQUAL(labels[22], size_t(2));
		BOOST_CHECK_EQUAL(labels[23], size_t(2));
		BOOST_CHECK_EQUAL(labels[24], size_t(0));
    }
};


#endif
