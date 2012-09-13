// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

// Some operating modes may require temporary files. On some systems you may
// need to set up a signal handler to ensure that temporary files are
// deleted if the program is interrupted.  See libjpeg.doc.

#include <cstring>

#define BOOST_ENABLE_ASSERT_HANDLER
#include <boost/assert.hpp>

#include "jpeg_error.h"
#include "jpeg_reader.h"
#include "system_error.h"


namespace image {
	
	
	using namespace std;
	
	
	jpeg_reader::jpeg_reader(const boost::filesystem::path & file_path)
#ifdef USE_LOG4CXX
	: logger_(Logger::getLogger(_T("genera.jpeg_reader")))
#endif
	{
		stream_ = ::fopen(file_path.string().c_str(), "rb");
		if (stream_ == 0)
			throw system_error("fopen");
		
		// We set up the normal JPEG error routines, then override error_exit.
		decompress_info_.err = jpeg_std_error(&err_.standard_error_mgr);
		err_.standard_error_mgr.error_exit = error_exit_cb;
		
		// We use C's setjmp/longjmp facility to return control. This means that
		// the routine which calls the JPEG library must first execute a
		// setjmp() call to establish the return point.
		if (::setjmp(err_.return_context)) {
			// jpeg_create_decompress has signaled an error.
			int rc = ::fclose(stream_);
			if (rc != 0) {
#ifdef USE_LOG4CXX
				LOG4CXX_WARN(logger_, _T("Unable to close JPEG file."));
#endif
			}
			throw jpeg_error("jpeg_create_decompress", decompress_info_);
		}
		
		jpeg_create_decompress(&decompress_info_);
	}
	
	
	jpeg_reader::~jpeg_reader() throw ()
	{
		if (setjmp(err_.return_context) != 0) {
			// jpeg_destroy_decompress signaled an error and longjmp is called.
#ifdef USE_LOG4CXX
			LOG4CXX_WARN(logger_, _T("Unable to destroy JPEG decompressor."));
#endif
		}
		else
			jpeg_destroy_decompress(&decompress_info_);
		
		int rc = ::fclose(stream_);
		if (rc != 0) {
#ifdef USE_LOG4CXX
			LOG4CXX_WARN(logger_, _T("Unable to close JPEG file."));
#endif
		}
		
		if (err_.standard_error_mgr.num_warnings > 0) {
#ifdef USE_LOG4CXX
			LOG4CXX_WARN(logger_,
				_T("Corrupt-data warnings in JPEG decompressor."));
#endif
		}
	}
	
	
	void jpeg_reader::read()
	{
		if (setjmp(err_.return_context))
			// The JPEG library has signaled an error.
			throw jpeg_error("jpeg_reader::read", decompress_info_);
		
		jpeg_stdio_src(&decompress_info_, stream_);
		
		int rc = jpeg_read_header(&decompress_info_, TRUE);
		// Suspension is not possible with the stdio data source and
		// we passed TRUE to reject a tables-only JPEG file as an error.
		BOOST_ASSERT(rc == JPEG_HEADER_OK);
		
		// We don't need to change any of the defaults set by jpeg_read_header()
		
		rc = jpeg_start_decompress(&decompress_info_);
		// Suspension is not possible with the stdio data source.
		BOOST_ASSERT(rc == TRUE);
		
		image_width_ = decompress_info_.output_width;
		image_height_ = decompress_info_.output_height;
		num_components_ = decompress_info_.output_components;
		unsigned int row_stride = image_width_ * num_components_;
		image_buffer_.reset(new JSAMPLE[image_height_ * row_stride]);
		
		// We need a one-scanline-high JSAMPLE array as a work buffer, and we
		// will let the JPEG memory manager allocate it for us. We don't need to
		// remember to deallocate the buffer separately: it will go away
		// automatically when the JPEG object is cleaned up.
		
		// We cheated a bit by calling alloc_sarray() after jpeg_start_decompress();
		// we should have done it beforehand to ensure that the space would be
		// counted against the JPEG max_memory setting.  In some systems the above
		// code would risk an out-of-memory error.  However, in general we don't
		// know the output image dimensions before jpeg_start_decompress(), unless we
		// call jpeg_calc_output_dimensions().
		
		JSAMPARRAY scanline_ptrs = (*decompress_info_.mem->alloc_sarray)(
			reinterpret_cast<j_common_ptr>(&decompress_info_),
			JPOOL_IMAGE,
			row_stride,
			1);
	
		JSAMPLE * dest_ptr = image_buffer_.get();
		
		// We use the library's state variable cinfo.output_scanline as the loop
		// counter.
		while (decompress_info_.output_scanline < decompress_info_.output_height) {
			// jpeg_read_scanlines expects an array of pointers to scanlines. Here
			// the array is only one element long, but you could ask for more than
			// one scanline at a time if that's more convenient.
			unsigned int num_read =
				jpeg_read_scanlines(&decompress_info_, scanline_ptrs, 1);
			// We asked for only one line and we weren't using a suspending data
			// source.
			BOOST_ASSERT(num_read == 1);
	
			memcpy(dest_ptr, scanline_ptrs[0], row_stride);
			dest_ptr += row_stride;
		}
	
		rc = ::jpeg_finish_decompress(&decompress_info_);
		// Suspension is not possible with the stdio data source.
		BOOST_ASSERT(rc == TRUE);
	}
	
	
	/// \brief A replacement for the standard error_exit function. Returns
	/// control to the previously saved context.
	void jpeg_reader::error_exit_cb(j_common_ptr decompress_info)
	{
		extended_error_mgr & err =
			*reinterpret_cast<extended_error_mgr *>
			(decompress_info->err);
		
		longjmp(err.return_context, 1);
	}
	
	
}
