// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

// If the compressor requires full-image buffers (for entropy-coding
// optimization or a multi-scan JPEG file), it will create temporary
// files for anything that doesn't fit within the maximum-memory setting.
// (Note that temp files are NOT needed if you use the default parameters.)
// On some systems you may need to set up a signal handler to ensure that
// temporary files are deleted if the program is interrupted.  See libjpeg.doc.
 
#define BOOST_ENABLE_ASSERT_HANDLER
#include <boost/assert.hpp>

#include "jpeg_writer.h"
#include "jpeg_error.h"
#include "system_error.h"


namespace image {
	
	
	using namespace std;
	//using namespace log4cxx;
	namespace fs = boost::filesystem;
	
	
	jpeg_writer::jpeg_writer(
		const fs::path & file_path,
		const JSAMPLE * image_buffer,
		unsigned int image_width,
		unsigned int image_height,
		unsigned int num_components,
		unsigned int quality,
		bool overwrite)
		: /*logger_(Logger::getLogger(_T("genera.jpeg_writer"))),*/
		  image_buffer_(image_buffer),
		  image_width_(image_width),
		  image_height_(image_height),
		  num_components_(num_components),
		  quality_(quality)
	{
		BOOST_ASSERT(quality_ <= 100);
		
		// Open the stdio stream.
		if (overwrite) {
			stream_ = ::fopen(file_path.string().c_str(), "wb");
			if (stream_ == 0)
				throw system_error("fopen");
		}
		else {
			stream_ = ::fopen(file_path.string().c_str(), "ab");
			if (stream_ == 0)
				throw system_error("fopen");
			
			long stream_position = ::ftell(stream_);
			if (stream_position == -1) {
				int rc = ::fclose(stream_);
				if (rc != 0) {
//					LOG4CXX_WARN(logger_, _T("Unable to close JPEG file."));
				}
				throw system_error("ftell");
			}
			
			if (stream_position != 0) {
				// The file exists alrady and is not empty.
				int rc = ::fclose(stream_);
				if (rc != 0) {
//					LOG4CXX_WARN(logger_, _T("Unable to close JPEG file."));
				}
				throw file_exists(file_path);
			}
		}
		
		// We set up the normal JPEG error routines, then override error_exit.
		compress_info_.err = jpeg_std_error(&err_.standard_error_mgr);
		err_.standard_error_mgr.error_exit = jpeg_writer::error_exit_cb;
		
		// We use C's setjmp/longjmp facility to return control. This means that
		// the routine which calls the JPEG library must first execute a
		// setjmp() call to establish the return point.
		if (setjmp(err_.return_context)) {
			// jpeg_create_compress has signaled an error.
			int rc = ::fclose(stream_);
			if (rc != 0) {
//				LOG4CXX_WARN(logger_, _T("Unable to close JPEG file."));
			}
			throw jpeg_error("jpeg_create_compress", compress_info_);
		}
	
		jpeg_create_compress(&compress_info_);
	}
	
	
	jpeg_writer::~jpeg_writer() throw ()
	{
		if (setjmp(err_.return_context) != 0) {
			// jpeg_destroy_compress has signaled an error and longjmp is
			// called.
//			LOG4CXX_WARN(logger_, _T("Unable to destroy JPEG decompressor."));
		}
		else {
			jpeg_destroy_compress(&compress_info_);
		}
		
		int rc = ::fclose(stream_);
		if (rc != 0) {
//			LOG4CXX_WARN(logger_, _T("Unable to close JPEG file."));
		}
	}
	
	
	void jpeg_writer::write()
	{
		if (setjmp(err_.return_context))
			// The JPEG library has signaled an error.
			throw jpeg_error("jpeg_writer::write", compress_info_);
		
		// Use the library-supplied code to send compressed data to an stdio
		// stream.
		jpeg_stdio_dest(&compress_info_, stream_);
		
		compress_info_.image_width = image_width_;
		compress_info_.image_height = image_height_;
		compress_info_.input_components = num_components_;
		compress_info_.in_color_space = JCS_RGB;
		
		unsigned int row_stride = image_width_ * num_components_;
		
		jpeg_set_defaults(&compress_info_);
		jpeg_set_quality(
			&compress_info_,
			quality_,
			TRUE);  // Limit to baseline-JPEG values.
	
		jpeg_start_compress(
			&compress_info_,
			TRUE);  // Ensures that we will write a complete interchange-JPEG file.
			        // Pass TRUE unless you are very sure of what you're doing.
	
		// jpeg_write_scanlines expects an array of pointers to scanlines.
		// Here the array is only one element long
		JSAMPROW scanline_ptrs[1];
		scanline_ptrs[0] = const_cast<JSAMPLE *>(image_buffer_);
	
		// Here we use the library's state variable cinfo.next_scanline as the
		// loop counter, so that we don't have to keep track ourselves.
		while (compress_info_.next_scanline < compress_info_.image_height) {
			unsigned int num_written = jpeg_write_scanlines(
				&compress_info_, scanline_ptrs, 1);
			// Partial writes can only occur if you use a data destination module
			// that can demand suspension of the compressor.
			BOOST_ASSERT(num_written == 1);
	
			scanline_ptrs[0] += row_stride;
		}
	
		::jpeg_finish_compress(&compress_info_);
	}
	
	
	/// \brief A replacement for the standard error_exit function. Returns
	/// control to the previously saved context.
	///
	void jpeg_writer::error_exit_cb(j_common_ptr compress_info)
	{
		extended_error_mgr & err =
			*reinterpret_cast<extended_error_mgr *>
			(compress_info->err);
		
		longjmp(err.return_context, 1);
	}
	
	
}
