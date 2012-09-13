// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

#ifndef IMAGE_CONNECTED_COMPONENTS_H
#define IMAGE_CONNECTED_COMPONENTS_H


#include <cstddef>  // size_t
#include <vector>
#include <iterator>  // iterator_traits

#define BOOST_ENABLE_ASSERT_HANDLER
#include <boost/assert.hpp>
#include <boost/function.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include "disjoint_set.h"
#include "connected_components_detail.h"


namespace image {
	
	
	/// \brief A class for finding the connected components within an image.
	///
	/// \author Seppo Enarvi (senarvi@iki.fi)
	/// \date 2002 - 2005
	///
	/// Based on "Fast Connected Components on Images" by Ali Rahimi
	/// (ali@mit.edu), http://web.media.mit.edu/~rahimi/connected/
	///
	template <typename InputIter,
	          typename LabelDesignator = detail::label_all_fields<bool> >
	class connected_components
	{
	public:
		typedef typename std::iterator_traits<InputIter>::value_type color_type;
		typedef LabelDesignator label_designator_type;
		typedef typename label_designator_type::field_type field_type;
		typedef boost::function<field_type (color_type)> field_designator_type;
		typedef detail::component_iterator iterator;
		typedef detail::component_tag<field_type> tag;
		typedef disjoint_set_forest<tag> component_forest_type;
		typedef typename component_forest_type::node node_type;
		typedef std::vector<node_type *> node_raster_type;
		typedef typename node_raster_type::iterator node_iterator;
		typedef typename node_raster_type::const_iterator node_const_iterator;
		
		
	    connected_components(
			InputIter image_first,
			InputIter image_last,
			unsigned int image_width,
			field_designator_type field_designator =
				boost::bind(std::equal_to<bool>(), _1, true),
			label_designator_type label_designator =
				detail::label_all_fields<bool>(),
			bool k8_connectivity = true);
		
		
		std::size_t num_components() const
		{ return num_components_; }
		
		
		/// \brief Set to true all the pixels of the output mask that the
		/// component with given label contains.
		///
		template <typename OutputIter>
		void get_component(std::size_t label, OutputIter result)
		{
			node_const_iterator iter;
			for (iter = node_raster_.begin();
				 iter != node_raster_.end();
				 ++iter)
			{
				if ((*iter)->find_set().payload().label() == label)
					*result = true;
				++result;
			}
		}
		
		
		/// \brief Create a raster with a unique integer label
		/// (0..num_components()-1) indicating the component that each
		/// pixel belongs to.
		///
		template <typename OutputIter>
		void get_label_raster(OutputIter result)
		{
			node_const_iterator iter;
			for (iter = node_raster_.begin();
				 iter != node_raster_.end();
				 ++iter)
			{
				*result = (*iter)->find_set().payload().label();
				++result;
			}
		}
		
		
		/// \brief Find the field of the component with given label.
		///
		field_type field(std::size_t label)
		{
			node_const_iterator iter;
			for (iter = node_raster_.begin();
				 iter != node_raster_.end();
				 ++iter)
			{
				node_type & set = (*iter)->find_set();
				if (set.payload().label() == label)
					return set.payload().field();
			}
			BOOST_ASSERT(false);
			return field_type();
		}
		
		
		iterator begin()
		{ return iterator(0); }
		
		iterator end()
		{ return iterator(num_components_); }
		
		node_iterator node_raster_begin()
		{ return node_raster_.begin(); }
	
		node_iterator node_raster_end()
		{ return node_raster_.end(); }
		
	
	private:
	    component_forest_type component_forest_;
		node_raster_type node_raster_;
		InputIter image_first_;
		std::size_t num_components_;
		
		
		node_type * create_node(field_type field)
		{ return component_forest_.create_set(tag(field)); }
		
		node_type * node(InputIter iter)
		{
			node_type * result = node_raster_[iter - image_first_];
			BOOST_ASSERT(result != 0);
			return result;
		}
		
		void set_node(InputIter iter, node_type * value)
		{ node_raster_[iter - image_first_] = value; }
	
		const field_type & field(InputIter iter)
		{ return node(iter)->payload().field(); }
	};
	
	
	template <typename InputIter, typename Field>
	connected_components<InputIter, Field>::connected_components(
		InputIter image_first,
		InputIter image_last,
		unsigned int image_width,
		field_designator_type field_designator,
		label_designator_type label_designator,
		bool k8_connectivity)
		: node_raster_(image_last - image_first, static_cast<node_type *>(0)),
		  image_first_(image_first)
	{
		typedef typename std::iterator_traits<InputIter>::difference_type diff;
		BOOST_ASSERT((image_last - image_first) % image_width == 0);
		
		// The first row. For each pixel create a new node or give it
		// the node of the previous pixel in case field_designator
		// gives the same result as with the previous pixel.
		//
		{
			if (image_first_ != image_last)
				set_node(
					image_first_, create_node(field_designator(*image_first_)));
			
			InputIter left;
			InputIter right(image_first_);
			InputIter next_row(image_first_ + image_width);
	
			while (true)
			{
				left = right;
				++right;
				if (right == next_row) break;
	
				field_type right_field = field_designator(*right);
				if ((right_field > 0 && field(left) > 0) ||
					(right_field <= 0 && field(left) <= 0)) {
					set_node(right, node(left));
				} else {
					set_node(right, create_node(right_field));
				}
			}
		}
		
		
		// The subsequent rows. Continue until there's no more
		// image_width pixels in the image.
		//
		{
			InputIter lower_row;
			InputIter upper_row(image_first_);
			InputIter next_row(image_first_ + image_width);
	
			while (next_row != image_last)
			{
				lower_row = upper_row;
				upper_row = next_row;
				next_row += image_width;
	
				// Label the first pixel on upper row.
				//
				{
					field_type upper_field = field_designator(*upper_row);
					if ((upper_field > 0 && field(lower_row) > 0) ||
						(upper_field <= 0 && field(lower_row) <= 0)) {
						set_node(upper_row, node(lower_row));
					} else {
						set_node(upper_row, create_node(upper_field));
					}
				}
				
				// Label subsequent pixels on upper row.
				//
				{
					InputIter upper_left;
					InputIter upper_right(upper_row);
					InputIter lower_left;
					InputIter lower_right(lower_row);
					while (true)
					{
						upper_left = upper_right;
						++upper_right;
						lower_left = lower_right;
						++lower_right;
						if (upper_right == next_row) break;
						
						node_type * upper_right_node = 0;
						
						// [a][b]
						// [c][d]
						field_type a_field = field_designator(*upper_left);
						field_type b_field = field_designator(*upper_right);
						field_type c_field = field_designator(*lower_left);
						field_type d_field = field_designator(*lower_right);
						
						// [a][ ]
						// [ ][d]
						if (k8_connectivity &&
							((d_field > 0 && a_field > 0) ||
							(d_field <= 0 && a_field <= 0)))
						{
							node(upper_left)->link(*node(lower_right));
						}
						
						// [a][b]
						// [ ][ ]
						if ((b_field > 0 && a_field > 0) ||
							(b_field <= 0 && a_field <= 0))
						{
							upper_right_node = node(upper_left);
						}
						
						// [ ][b]
						// [c][ ]
						if (k8_connectivity &&
							((b_field > 0 && c_field > 0) ||
							(b_field <= 0 && c_field <= 0)))
						{
							if (upper_right_node != 0) {
								upper_right_node->link(*node(lower_left));
							} else {
								upper_right_node = node(lower_left);
							}
						}
						
						// [ ][b]
						// [ ][d]
						if ((b_field > 0 && d_field > 0) ||
							(b_field <= 0 && d_field <= 0))
						{
							if (upper_right_node != 0) {
								upper_right_node->link(*node(lower_right));
							} else {
								upper_right_node = node(lower_right);
							}
						}
						
						if (upper_right_node == 0)
							upper_right_node = create_node(b_field);
						
						set_node(upper_right, upper_right_node);
					}
				}
			}
		}
		
		
		// Give a unique label for the delegate (root) node of each tree in the
		// component forest.
		//
		std::for_each(
			component_forest_.nodes_begin(),
			component_forest_.nodes_end(),
			detail::set_label_if_delegate<label_designator_type, node_type>(
				label_designator));
		num_components_ = label_designator.num_labels();
	}
	
	
	/// \brief Fill a raster with the labels of every component, regardless of
	/// their field value.
	///
	template <typename ImageIter,
	          typename FieldDesignator,
	          typename LabelRasterIter>
	inline void find_true_false_components(
		ImageIter image_first,
		ImageIter image_last,
		unsigned int image_width,
		FieldDesignator field_designator,
		LabelRasterIter label_raster_first,
		std::size_t & num_components,
		bool k8_connectivity = true)
	{
		using namespace detail;
		
		// Note: g++ is unable to deduce bool from field_designator_type.
		connected_components<ImageIter, label_all_fields<bool> > cc(
			image_first, image_last, image_width,
			field_designator,
			label_all_fields<bool>(),
			k8_connectivity);
		
		num_components = cc.num_components();
		cc.get_label_raster(label_raster_first);
	}
	
	
	/// \brief Fill a raster with 0, where field_designator returns false, and
	/// with the labels of the components, where field_designator returns true.
	///
	template <typename ImageIter,
	          typename FieldDesignator,
	          typename LabelRasterIter>
	inline void find_true_components(
		ImageIter image_first,
		ImageIter image_last,
		unsigned int image_width,
		FieldDesignator field_designator,
		LabelRasterIter label_raster_first,
		std::size_t & num_components,
		bool k8_connectivity = true)
	{
		using namespace detail;
		
		connected_components<ImageIter, label_true_fields> cc(
			image_first, image_last, image_width,
			field_designator,
			label_true_fields(),
			k8_connectivity);
		
		num_components = cc.num_components();
		cc.get_label_raster(label_raster_first);
	}
	
	
}
 
#endif
