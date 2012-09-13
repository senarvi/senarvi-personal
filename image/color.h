// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

#ifndef IMAGE_COLOR_H
#define IMAGE_COLOR_H

#include <limits>
#include <list>
#include <cmath>

#define BOOST_ENABLE_ASSERT_HANDLER
#include <boost/assert.hpp>
#include <boost/type_traits.hpp>

#include "color_detail.h"


namespace image {
	
	
	template <typename T> class color_hsv;
	
	
    /// \brief A color in RGB (red, green, blue) representation.
	///
	template <typename T>
	class color_rgb
	{
	public:
		/// The type of each component (red, green, blue).
		typedef T component_type;
		
		
		/// Red value.
		T r;
		/// Green value.
		T g;
		/// Blue value.
		T b;
		
		
		/// \brief Create an RGB color with given red, green and blue values.
		///
		color_rgb(T r_arg, T g_arg, T b_arg)
			: r(r_arg), g(g_arg), b(b_arg) {}
		
		/// \brief Create an RGB color with given gray value.
		///
		color_rgb(T gray)
			: r(gray), g(gray), b(gray) {}
		
		/// \brief Create an RGB color with red = green = blue = 0.
		///
		color_rgb()
			: r(0), g(0), b(0) {}
		
		
		/// \brief Return the black RGB color.
		///
		static color_rgb black()
		{ return color_rgb(0, 0, 0); }
		
		/// \brief Return the white RGB color.
		///
		static color_rgb white()
		{ return color_rgb(
			detail::max_value<T>::value(),
			detail::max_value<T>::value(),
			detail::max_value<T>::value()); }
		
		/// \brief Return the red RGB color.
		///
		static color_rgb red()
		{ return color_rgb(detail::max_value<T>::value(), 0, 0); }
		
		/// \brief Return the green RGB color.
		///
		static color_rgb green()
		{ return color_rgb(0, detail::max_value<T>::value(), 0); }
		
		/// \brief Return the blue RGB color.
		///
		static color_rgb blue()
		{ return color_rgb(0, 0, detail::max_value<T>::value()); }
		
		/// \brief Return the yellow RGB color.
		///
		static color_rgb yellow()
		{
			return color_rgb(
				detail::max_value<T>::value(),
				detail::max_value<T>::value(),
				0);
		}
		
		/// \brief Create an HSV color from an RGB color.
		///
		operator color_hsv<T>()
		{ return detail::rgb_to_hsv<T>::convert(*this); }
		
		/// \brief color *= scalar
		///
		color_rgb & operator*=(component_type s)
		{ r *= s; g *= s; b *= s; return *this; }
		
		/// \brief color /= scalar
		///
		color_rgb & operator/=(component_type s)
		{ r /= s; g /= s; b /= s; return *this; }
		
		/// \brief color * scalar
		///
		color_rgb operator*(component_type s) const
		{
			color_rgb result(*this);
			result *= s;
			return result;
		}
		
		/// \brief color / scalar
		///
		color_rgb operator/(component_type s) const
		{
			color_rgb result(*this);
			result /= s;
			return result;
		}
		
		/// \brief color += color
		///
		color_rgb & operator+=(const color_rgb & c)
		{ r += c.r; g += c.g; b += c.b; return *this; }
		
		/// \brief color -= color
		///
		color_rgb & operator-=(const color_rgb & c)
		{ r -= c.r; g -= c.g; b -= c.b; return *this; }
		
		/// \brief color + color
		///
		color_rgb operator+(const color_rgb & c) const
		{
			color_rgb result(*this);
			result += c;
			return result;
		}
		
		/// \brief color - color
		///
		color_rgb operator-(const color_rgb & c) const
		{
			color_rgb result(*this);
			result -= c;
			return result;
		}
	};
	
	
    /// \brief A color in HSV (hue, saturation, value) representation.
	///
	template <typename T>
	class color_hsv
	{
	public:
		/// \brief The type of each component (hue, saturation, value).
		typedef T component_type;
	
		/// \brief Hue.
		T h;
		/// \brief Saturation.
		T s;
		/// \brief Value.
		T v;
	
		/// \brief Create an HSV color with given hue, saturation and value.
		///
		color_hsv(T h_arg, T s_arg, T v_arg)
			: h(h_arg), s(s_arg), v(v_arg) {}
		
		/// \brief Create an HSV color with given gray value.
		///
		color_hsv(T gray)
			: h(0), s(0), v(gray) {}
		
		/// \brief Create an HSV color with hue = saturation = value = 0.
		///
		color_hsv()
			: h(0), s(0), v(0) {}
	
		/// \brief Create an HSV color from an RGB color.
		///
		/// This constructor is explicit, so that in an implicit
		/// assignment the compiler will choose color_rgb::operator
		/// color_hsv instead and won't display any warnings.
		///
		explicit color_hsv(const color_rgb<T> & rgb)
		{
			color_hsv<T> hsv = detail::rgb_to_hsv<T>::convert(rgb);
			h = hsv.h;
			s = hsv.s;
			v = hsv.v;
		}
	};
	
	
	template <typename ColorT>
	class to_grayscale {};
	
	template <typename T>
	class to_grayscale<color_rgb<T> >
	{
	public:
		/// \brief The type of a color.
		typedef color_rgb<T> color_type;
		/// \brief The type of each component of a color.
		typedef typename color_type::component_type component_type;
		
		component_type operator()(const color_type & c) const
		{ return std::max(std::max(c.r, c.g), c.b); }
	};
	
	template <typename T>
	class to_grayscale<color_hsv<T> >
	{
	public:
		/// \brief The type of a color.
		typedef color_hsv<T> color_type;
		/// \brief The type of each component of a color.
		typedef typename color_type::component_type component_type;
		
		component_type operator()(const color_type & c) const
		{ return c.v; }
	};
	
	
	/// \brief A function object that returns true if the L2 distance
	/// between two colors is less than or equal to a given threshold.
	///
	template <typename ColorT>
	class l2_distance_le
	{
	public:
		/// \brief The type of a color.
		typedef ColorT color_type;
		/// \brief The type of each component of a color.
		typedef typename ColorT::component_type component_type;
		
		l2_distance_le(component_type threshold)
			: threshold_(threshold) {}
		
		bool operator()(const color_type & x, const color_type & y) const
		{
			return detail::difference_squared<component_type>::calculate(x, y)
				<= threshold_ * threshold_;
		}
		
	private:
		component_type threshold_;
	};


	/// \brief A class that stores a color range as color samples and
	/// a maximum distance from a sample.
	///
	template <typename ColorT, typename ThresholdF = l2_distance_le<ColorT> >
	class sampled_colors
	{
	public:
		/// The type of a color.
		typedef ColorT color_type;
		/// The type of each component of a color.
		typedef typename ColorT::component_type component_type;
		/// A function object that returns true if a color is close enough.
		typedef ThresholdF threshold_function;
		
		/// \brief Creates a sampled_colors object with given sample
		/// radius.
		///
		/// The color range will contain all colors within the sample
		/// radius from any color sample.
		///
		sampled_colors(component_type sample_radius) :
			inside_(threshold_function(sample_radius)) {}
	
		/// \brief Creates a sampled_colors object with a threshold
		/// function.
		///
		/// The function object should return true if the color given
		/// as a second parameter is close enough the sample given as
		/// the first parameter, so that the color is inside the color
		/// range.
		///
		sampled_colors(threshold_function f) :
			inside_(f) {}
	
		/// \brief Adds a color sample.  All colors within the sample
		/// radius from the color sample will be added to the color
		/// range.
		///
		void add_sample(const color_type & sample)
		{ samples_.push_back(sample); }
		
		/// \brief Finds out if the given color is inside the color range.
		///
		bool contains(const color_type & color) const
		{
			typename samples_t::const_iterator iter;
			for (iter = samples_.begin(); iter != samples_.end(); ++iter)
			{
				if (inside_(color, *iter))
					return true;
			}
			return false;
		}
		
	private:
		typedef std::list<color_type> samples_t;
	
		samples_t samples_;
		threshold_function inside_;
	};
	
	
	/// \brief Fill an RGB or BGR array of OutT with samples from color_rgb<InT>.
	///
	template <typename InT, typename OutT, bool RGBOrder>
	struct array_from_color_rgb {};

	/// \brief Fill an RGB array of OutT with samples from color_rgb<InT>.
	///
	template <typename InT, typename OutT>
	struct array_from_color_rgb<InT, OutT, true>
	{
		static void fill(const color_rgb<InT> & in, OutT * out)
		{
			*(out++) = detail::r_from_color_rgb<InT, OutT>::get(in);
			*(out++) = detail::g_from_color_rgb<InT, OutT>::get(in);
			*(out++) = detail::b_from_color_rgb<InT, OutT>::get(in);
		}
	};

	/// \brief Fill an BGR array of OutT with samples from color_rgb<InT>.
	///
	template <typename InT, typename OutT>
	struct array_from_color_rgb<InT, OutT, false>
	{
		static void fill(const color_rgb<InT> & in, OutT * out)
		{
			*(out++) = detail::b_from_color_rgb<InT, OutT>::get(in);
			*(out++) = detail::g_from_color_rgb<InT, OutT>::get(in);
			*(out++) = detail::r_from_color_rgb<InT, OutT>::get(in);
		}
	};
	
	
	/// \brief Get color_rgb<OutT> from an RGB or BGR array of InT samples.
	///
	template <
		typename InT,
		typename OutT,
		bool RGBOrder>
	struct color_rgb_from_array
	{
		/// \brief Get the red, green and blue values from an RGB or
		/// BGR array and create a color_rgb<OutT>.
		///
		static color_rgb<OutT> get(const InT * in)
		{
			return color_rgb<OutT>(
				detail::r_from_array<InT, OutT, RGBOrder>::get(in),
				detail::g_from_array<InT, OutT, RGBOrder>::get(in),
				detail::b_from_array<InT, OutT, RGBOrder>::get(in));
		}
	};
	
	
	/// \brief Fill an RGB or BGR array of OutT samples from an RGB or
	/// BGR array of InT samples.
	///
	template <
		typename InT,
		typename OutT,
		bool InRGBOrder,
		bool OutRGBOrder>
	struct array_from_array {};

	/// \brief Fill an RGB array of OutT samples from an RGB or
	/// BGR array of InT samples.
	///
	template <typename InT, typename OutT, bool InRGBOrder>
	struct array_from_array<InT, OutT, InRGBOrder, true>
	{
		static void fill(const InT * in, OutT * out)
		{
			*(out++) = detail::r_from_array<InT, OutT, InRGBOrder>::get(in);
			*(out++) = detail::g_from_array<InT, OutT, InRGBOrder>::get(in);
			*(out++) = detail::b_from_array<InT, OutT, InRGBOrder>::get(in);
		}
	};

	/// \brief Fill an BGR array of OutT samples from an RGB or
	/// BGR array of InT samples.
	///
	template <typename InT, typename OutT, bool InRGBOrder>
	struct array_from_array<InT, OutT, InRGBOrder, false>
	{
		static void fill(const InT * in, OutT * out)
		{
			*(out++) = detail::b_from_array<InT, OutT, InRGBOrder>::get(in);
			*(out++) = detail::g_from_array<InT, OutT, InRGBOrder>::get(in);
			*(out++) = detail::r_from_array<InT, OutT, InRGBOrder>::get(in);
		}
	};
	
	
	/// \brief Write a color_rgb<T> to an output stream.
	///
	template <typename T>
	inline std::ostream & operator<<(std::ostream & s, const color_rgb<T> & c)
	{ return detail::write_comma_separated(c.r, c.g, c.b); }
	
	/// \brief Write a color_rgb<unsigned char> to an output stream.
	///
	template <>
	inline std::ostream & operator<< <unsigned char>(
		std::ostream & s, const color_rgb<unsigned char> & c)
	{
		return detail::write_comma_separated(
			s,
			static_cast<unsigned int>(c.r),
			static_cast<unsigned int>(c.g),
			static_cast<unsigned int>(c.b));
	}
	
	/// \brief Write a color_hsv<T> to an output stream.
	///
	template <typename T>
	inline std::ostream & operator<<(std::ostream & s, const color_hsv<T> & c)
	{ return detail::write_comma_separated(c.h, c.s, c.v); }
	
	/// \brief Write a color_hsv<unsigned char> to an output stream.
	///
	template <>
	inline std::ostream & operator<< <unsigned char>(
		std::ostream & s, const color_hsv<unsigned char> & c)
	{
		return detail::write_comma_separated(
			s,
			static_cast<unsigned int>(c.h),
			static_cast<unsigned int>(c.s),
			static_cast<unsigned int>(c.v));
	}
	
	/// \brief Read a color_rgb<T> from an input stream.
	///
	template <typename T>
	inline std::istream & operator>>(std::istream & s, color_rgb<T> & c)
	{ return detail::read_comma_separated(s, c.r, c.g, c.b); }
	
	/// \brief Read a color_rgb<unsigned char> from an input stream.
	///
	template <>
	inline std::istream & operator>><unsigned char>(
		std::istream & s, color_rgb<unsigned char> & c)
	{
		unsigned int int_r, int_g, int_b;
		detail::read_comma_separated(s, int_r, int_g, int_b);
		
		if (int_r > std::numeric_limits<unsigned char>::max())
			int_r = std::numeric_limits<unsigned char>::max();
		if (int_g > std::numeric_limits<unsigned char>::max())
			int_g = std::numeric_limits<unsigned char>::max();
		if (int_b > std::numeric_limits<unsigned char>::max())
			int_b = std::numeric_limits<unsigned char>::max();
		
		c.r = static_cast<unsigned char>(int_r);
		c.g = static_cast<unsigned char>(int_g);
		c.b = static_cast<unsigned char>(int_b);
		
		return s;
	}
	
	/// \brief Read a color_hsv<T> from an input stream.
	///
	template <typename T>
	inline std::istream & operator>>(std::istream & s, color_hsv<T> & c)
	{ return detail::read_comma_separated(s, c.r, c.g, c.b); }
	
	/// \brief Read a color_hsv<unsigned char> from an input stream.
	///
	template <>
	inline std::istream & operator>><unsigned char>(
		std::istream & s, color_hsv<unsigned char> & c)
	{
		unsigned int int_h, int_s, int_v;
		detail::read_comma_separated(s, int_h, int_s, int_v);
		
		if (int_h > std::numeric_limits<unsigned char>::max())
			int_h = std::numeric_limits<unsigned char>::max();
		if (int_s > std::numeric_limits<unsigned char>::max())
			int_s = std::numeric_limits<unsigned char>::max();
		if (int_v > std::numeric_limits<unsigned char>::max())
			int_v = std::numeric_limits<unsigned char>::max();
		
		c.h = static_cast<unsigned char>(int_h);
		c.s = static_cast<unsigned char>(int_s);
		c.v = static_cast<unsigned char>(int_v);
		
		return s;
	}
	
	
}


#endif
