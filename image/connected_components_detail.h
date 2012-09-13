// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

#ifndef IMAGE_CONNECTED_COMPONENTS_DETAIL_H
#define IMAGE_CONNECTED_COMPONENTS_DETAIL_H


#include <cstddef>  // size_t
#include <functional>

#define BOOST_ENABLE_ASSERT_HANDLER
#include <boost/assert.hpp>
#include <boost/iterator/iterator_facade.hpp>


namespace image { namespace detail {
	
	
	template <typename Field>
	class component_tag
	{
	public:
		typedef Field field_type;
		
		component_tag(Field field = Field(), std::size_t label = 0)
			: field_(field), label_(label) {}
		
		const Field & field() const
		{ return field_; }
		
		void set_field(const Field & field)
		{ field_ = field; }
		
		std::size_t label() const
		{ return label_; }
		
		void set_label(std::size_t label)
		{ label_ = label; }
		
	private:
		Field field_;
		std::size_t label_;
	};
	
	
	/// \brief A label generator that generates a sequence of consecutive
	/// integers regardless of the field values.
	///
	/// The first number in the sequence may be specified as a parameter to
	/// the constructor.
	///
	template <typename Field>
	class label_all_fields : public std::unary_function<Field, std::size_t>
	{
	public:
		typedef Field field_type;
		
		/// \brief Construct a label generator.
		///
		label_all_fields()
			: value_(0) {}
		
		/// \brief Return the next integer.
		///
		std::size_t operator()(Field field)
		{ return value_++; }
		
		/// \brief Return the number of labels so far.
		///
		std::size_t num_labels()
		{ return value_; }
		
	private:
		/// \brief The next integer to be returned by operator().
		std::size_t value_;
	};
	
	
	/// \brief A label generator that generates a sequence of consecutive
	/// integers for components with true field and 0 for components with
	/// false field.
	///
	class label_true_fields : public std::unary_function<bool, std::size_t>
	{
	public:
		typedef bool field_type;
		
		/// \brief Construct a label generator.
		///
		label_true_fields()
			: value_(1) {}
		
		/// \brief Return the next integer or 0 if the field is false.
		///
		std::size_t operator()(bool field)
		{
			if (!field) return 0;
			return value_++;
		}
		
		/// \brief Return the number of labels so far, including the 0
		/// label, even if there's no component with 0 label.
		///
		std::size_t num_labels()
		{ return value_; }
		
	private:
		/// \brief The next integer to be returned by operator().
		std::size_t value_;
	};
	
	
	/// \brief A function object that gives a label to a node using another
	/// function object, but only if the node is a "delegate" node (the root
	/// of a component tree.
	///
	template <typename LabelDesignator, typename Node>
	class set_label_if_delegate : public std::unary_function<Node *, void>
	{
	public:
		typedef Node node_type;
		typedef typename node_type::payload_type payload_type;
		typedef typename payload_type::field_type field_type;
		typedef LabelDesignator label_designator_type;
		
		set_label_if_delegate(label_designator_type & label_designator)
			: label_designator_(label_designator) {}
		
		void operator()(boost::shared_ptr<Node> node)
		{
			if (node->is_delegate()) {
				field_type field = node->payload().field();
				std::size_t label = label_designator_(field);
				node->payload().set_label(label);
			}
		}
		
	private:
		label_designator_type & label_designator_;
	};
	
	
	/// \brief An iterator class for traversing the components found by
	/// a connected_components object.
	///
	class component_iterator : public boost::iterator_facade<
		component_iterator,
		const std::size_t,
		boost::bidirectional_traversal_tag,
		const std::size_t &,
		std::ptrdiff_t>
	{
	public:
		explicit component_iterator(std::size_t label = 0)
			: label_(label) {}
		
	private:
		friend class boost::iterator_core_access;
		
		void increment()
		{ label_++; }
		
		void decrement()
		{ label_--; }
		
		bool equal(const component_iterator & x) const
		{ return label_ == x.label_; }
		
		const std::size_t & dereference() const
		{ return label_; }
		
		std::size_t label_;
	};
	
	
} }
 
#endif
