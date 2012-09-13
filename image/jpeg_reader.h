// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

#ifndef IMAGE_JPEG_READER_H
#define IMAGE_JPEG_READER_H

#include <string>
#include <vector>
#include <csetjmp>

#ifdef USE_LOG4CXX
#include <log4cxx/logger.h>
#endif

#include <boost/utility.hpp>
#include <boost/scoped_array.hpp>
#include <boost/filesystem/path.hpp>

// stdio.h has to be included before jpeglib.h.
#include <stdio.h>
#include <jpeglib.h>

#include "color.h"


namespace image {
	
	
	/// \brief A class for reading JPEG files using libjpeg.
	///
	class jpeg_reader : boost::noncopyable {
	
	public:	
		/// Constructs a jpeg_reader object and opens a file for reading.
		///
		jpeg_reader(const boost::filesystem::path & file_path);
		
		/// Destructs *this and closes the associated file.
		///
		~jpeg_reader() throw ();
		
		
		/// Read the image from the file to an internal buffer.
		///
		void read();
		
		const JSAMPLE * image_buffer() const
		{ return image_buffer_.get(); }
		
		unsigned int image_width() const
		{ return image_width_; }
		
		unsigned int image_height() const
		{ return image_height_; }
		
		
	private:
		/// \brief Our private extension to the libjpeg error manager.
		///
		/// We want error_exit_cb to do a longjmp(). But we need to make the
		/// setjmp buffer accessible to error_exit_cb. To do this, we make a
		/// private extension of the standard JPEG error handler object.
		///
		struct extended_error_mgr {
			/// The standard error_mgr structure is in the address where
			/// extended_error_mgr begins.
			struct jpeg_error_mgr standard_error_mgr;
			/// The location where the error handler should return is stored
			/// after the standard error_mgr structure.
			std::jmp_buf return_context;
		};
		
		
		/// \brief A replacement for the standard error_exit function. Returns
		/// control to the previously saved context.
		///
		static void error_exit_cb(j_common_ptr decompress_info);
		
		
#ifdef USE_LOG4CXX
		log4cxx::LoggerPtr logger_;
#endif
		
		// (This struct must live as long as the main JPEG parameter struct, to avoid
		// dangling-pointer problems.)
		struct extended_error_mgr err_;
		
		FILE * stream_;
		
		/// Contains the decompression parameters and pointers to working space (which is
		/// allocated as needed by the JPEG library.
		struct jpeg_decompress_struct decompress_info_;
		
		boost::scoped_array<JSAMPLE> image_buffer_;
		unsigned int image_width_;
		unsigned int image_height_;
		unsigned int num_components_;
	};
	
	
	template <typename OutputIter>
	void open_jpeg_image(
		const boost::filesystem::path & file_path,
		OutputIter result,
		unsigned int & width,
		unsigned int & height)
	{
#ifdef USE_LOG4CXX
		log4cxx::LoggerPtr logger =
			log4cxx::Logger::getLogger(_T("genera.open_jpeg_image"));
		LOG4CXX_DEBUG(logger, _T("Reading ") << file_path.string() << _T("."));
#endif
		
		jpeg_reader reader(file_path);
		reader.read();
		
		width = reader.image_width();
		height = reader.image_height();
		const JSAMPLE * in_first = reader.image_buffer();
		const JSAMPLE * in_last = in_first + (width * height * 3);
		
		typedef typename OutputIter::container_type::value_type::component_type
			out_sample_t;
		typedef color_rgb_from_array<JSAMPLE, out_sample_t, true>
			pixel_from_buffer;
		
		for (; in_first != in_last; in_first += 3, ++result)
		{
			*result = pixel_from_buffer::get(in_first);
		}
	}
	
	
}

#endif
