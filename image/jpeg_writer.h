// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

#ifndef IMAGE_JPEG_WRITER_H
#define IMAGE_JPEG_WRITER_H

#include <string>
#include <vector>
#include <csetjmp>

//#include <log4cxx/logger.h>
#include <boost/utility.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/smart_ptr.hpp>


// stdio.h has to be included before jpeglib.h.
#include <stdio.h>  // FILE
#include <jpeglib.h>


#include "color.h"


namespace image {
	
	
	/// \brief A class for writing JPEG files using libjpeg.
	///
	class jpeg_writer : boost::noncopyable
	{
	public:
		/// \brief Constructs a jpeg_writer object and opens a file for writing.
		///
		/// \param overwrite True, if the file should be overwritten in case it
		///                  already exists and is not empty, or false, if
		/// 				 file_exists should be thrown in that case.
		///
		/// The standard input image format is a rectangular array of pixels,
		/// with each pixel having the same number of "component" values (color
		/// channels). Each pixel row is an array of JSAMPLEs (which typically
		/// are unsigned chars). If you are working with color data, then the
		/// color values for each pixel must be adjacent in the row; for
		/// example, R,G,B,R,G,B,R,G,B,... for 24-bit RGB color.
	 	///
		jpeg_writer(
			const boost::filesystem::path & file_path,
			const JSAMPLE * image_buffer,  // in top-to-bottom order
			unsigned int image_width,
			unsigned int image_height,
			unsigned int num_components,
			unsigned int quality,  // 0..100
			bool overwrite = true);
		
		/// \brief Destructs *this and closes the associated file.
		///
		~jpeg_writer() throw ();
		
		/// \brief Writes the image in JPEG format to the file.
		///
		void write();
		
		
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
		static void error_exit_cb(j_common_ptr compress_info);
		
		
//		log4cxx::LoggerPtr logger_;
		
		// (This struct must live as long as the main JPEG parameter struct, to
		// avoid dangling-pointer problems.)
		struct extended_error_mgr err_;
		
		// Pointer to an stdio stream that represents the output file.
		FILE * stream_;
		
		/// Contains the JPEG compression parameters and pointers to working
		/// space (which is allocated as needed by the JPEG library).
		struct jpeg_compress_struct compress_info_;
		
		const JSAMPLE * image_buffer_;
		unsigned int image_width_;
		unsigned int image_height_;
		unsigned int num_components_;
		unsigned int quality_;
	};
	
	
	template <typename InputIter, typename Size>
	void save_image_as_jpeg(
		const boost::filesystem::path & file_path,
		InputIter in_first,
		Size width,
		Size height,
		unsigned int quality,  // 0..100
		bool overwrite = true)
	{
		InputIter in_last = in_first + (width * height);
		boost::scoped_array<JSAMPLE> out_buffer(
			new JSAMPLE[(in_last - in_first) * 3]);
		JSAMPLE * out_ptr = out_buffer.get();
		
		typedef typename std::iterator_traits<InputIter>::value_type color_type;
		typedef typename color_type::component_type in_sample_t;
		typedef typename image::array_from_color_rgb<in_sample_t, JSAMPLE, true>
			pixel_to_buffer;
		
		for (; in_first != in_last; in_first += 1, out_ptr += 3)
		{
			pixel_to_buffer::fill(*in_first, out_ptr);
		}
		
		jpeg_writer writer(
			file_path,
			out_buffer.get(),
			width, height,
			3,
			quality,
			overwrite);
		writer.write();
	}
	
	
}

#endif
