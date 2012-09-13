// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

#ifndef IMAGE_ASSERTION_ERROR_H
#define IMAGE_ASSERTION_ERROR_H


#include <stdexcept>  // logic_error
#include <string>


namespace image {
	
	
	/// \brief An exception class for failed assertions.
	///
	/// assertion_error is thrown by the boost::assertion_failed handler, when
	/// BOOST_ASSERT is called when BOOST_ENABLE_ASSERT_HANDLER macro is
	/// defined.
	///
	class assertion_error : public std::logic_error
	{
	public:
		/// \brief Constructs an exception with the information given to
		/// assertion_failed.
		///
		explicit assertion_error(
			const char * expression,
			const char * function_name,
			const char * file_name,
			long int line_number) throw ();
		
		/// \brief Destroys the assertion_error object.
		///
		/// An explicit constructor is required because of the virtual
		/// destructor in std::logic_error.
		///	
		virtual ~assertion_error() throw ()
		{}
		
		
		/// \brief Constructs an error message.
		///
		std::string message() const;
		
		
	private:
		const char * expression_;
		const char * function_name_;
		const char * file_name_;
		long int line_number_;
	};
	
	
}


#endif
