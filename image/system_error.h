// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

#ifndef IMAGE_SYSTEM_ERROR_H
#define IMAGE_SYSTEM_ERROR_H


#include <exception>
#include <string>


namespace image {
	
	
	/// \brief An exception class for errors returned by operating system calls
	/// and other library functions that use the operating system error
	/// reporting mechanism.
	///
	class system_error : public std::exception
	{
	public:
		/// \brief Constructs an exception object with error code taken from the
		/// operating system error reporting mechanism, and the name of the
		/// failed function given as a parameter.
		///
		explicit system_error(
			const char * function_name) throw ();
		
		/// \brief Constructs an exception object with error code and the name
		/// of the failed function given as a parameter.
		///
		explicit system_error(
			int error_code,
			const char * function_name) throw ();
		
		/// \brief A virtual destructor to let the destructors of any derived
		/// classes to get called.
		///
		virtual ~system_error() throw () {}
		
		
		/// \brief Returns a C-style string identifying the exception.
		///
		const char * what() const throw ();
		
		/// \brief Constructs an error message.
		///
		std::string message() const;
		
		/// \brief Returns the system error code.
		///
		int error_code() const
		{ return error_code_; }
		
	private:
		int error_code_;
		const char * function_name_;
	};
	
	
}


#endif
