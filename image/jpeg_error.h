// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

#ifndef IMAGE_JPEG_ERROR_H
#define IMAGE_JPEG_ERROR_H

#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>  // runtime_error
#include <boost/filesystem/path.hpp>

// stdio.h has to be included before jpeglib.h can be included.
#include <stdio.h>
#include <jpeglib.h>


namespace image {
	
	
	/// \brief An exception class for libjpeg errors.
	///
	class jpeg_error : public std::exception {
		
	public:
		/// \brief Construct an exception object from a compression error.
		///
		/// The error message is read from a jpeg_compress_struct structure that
		/// has to be given as a parameter to the constructor.
		///
		explicit
		jpeg_error(
			const std::string & function_name,
			jpeg_compress_struct & compress_info) throw () :
			
			function_name_(function_name),
			error_code_(compress_info.err->msg_code)
		{
			(*compress_info.err->format_message)(
				reinterpret_cast<j_common_ptr>(&compress_info),
				message_);
		}
		
		/// \brief Construct an exception object from a decompression error.
		///
		/// The error message is read from a jpeg_decompress_struct structure that
		/// has to be given as a parameter to the constructor.
		///
		explicit
		jpeg_error(
			const std::string & function_name,
			jpeg_decompress_struct & decompress_info) throw () :
			
			function_name_(function_name),
			error_code_(decompress_info.err->msg_code)
		{
			(*decompress_info.err->format_message)(
				reinterpret_cast<j_common_ptr>(&decompress_info),
				message_);
		}
	
		/// \brief Destruct the exception object.
		///
		virtual ~jpeg_error() throw () {}
		
		
		/// \brief Return a pointer to the error message C string.
		///
		/// The pointer is only valid until the exception object is destructed.
		///
		const char * what() const throw () {
			return message_;
		}
		
		/// \brief Return a string containing the error message.
		///
		const std::string message() const {
			std::ostringstream oss;
	        oss <<
				"In function `" << function_name_ << "':" << std::endl <<
				"Libjpeg error " << error_code_ << ": " << what();
			return oss.str();
		}
	
	
	private:
		const std::string function_name_;
		const int error_code_;
		char message_[JMSG_LENGTH_MAX];
	};
	
	
	/// \brief An exception that is thrown if a jpeg_writer is created with
	/// the name of a file that exists already, but overwriting is not
	/// requested.
	///
	class file_exists : public std::runtime_error
	{
	public:
		explicit file_exists(const boost::filesystem::path & file_path)
			: std::runtime_error("File exists already."),
			  file_path_(file_path) {}
		
		/// \brief Destruct the exception object.
		///
		~file_exists() throw () {}
		
		const boost::filesystem::path & file_path() const
		{ return file_path_; }
		
	private:
		boost::filesystem::path file_path_;
	};
	
	
}

#endif
