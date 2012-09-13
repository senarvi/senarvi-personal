// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

#ifndef IMAGE_COLOR_DETAIL_H
#define IMAGE_COLOR_DETAIL_H

#include <sstream>

#define BOOST_ENABLE_ASSERT_HANDLER
#include <boost/assert.hpp>
#include <boost/type_traits.hpp>


namespace image {
	
	
	template <typename T> class color_rgb;
	template <typename T> class color_hsv;
	
	
}


namespace image { namespace detail {
	
	
	/// \brief A generic class that gives the maximum value of a color
	/// component of given type.
	///
	template <typename T, bool IsFloat = boost::is_float<T>::value>
	struct max_value {};
	
	/// \brief A generic class that gives the maximum value of a color
	/// component of given type.
	///
	template <typename T>
	struct max_value<T, true>
	{
		static T value()
		{ return 1; }
	};
	
	/// \brief A generic class that gives the maximum value of a color
	/// component of given type.
	///
	template <typename T>
	struct max_value<T, false>
	{
		static T value()
		{ return std::numeric_limits<T>::max(); }
	};
	
	
	/// \brief A class for calculating the squared L2 distance between two
	/// HSV colors.
	///
	/// The calculation of a square root is aved when calculating the
	/// squared distance.
	///
	template <typename T, bool FP = boost::is_float<T>::value>
	struct difference_squared {};
	
	/// \brief A class for calculating the squared L2 distance between two
	/// HSV colors.
	///
	/// The calculation of a square root is aved when calculating the
	/// squared distance.
	///
	template <typename T>
	struct difference_squared<T, true>
	{
		static T calculate(const color_hsv<T> & x, const color_hsv<T> & y)
		{
			T dh = x.h - y.h;
			T ds = x.s - y.s;
			T dv = x.v - y.v;
			T result = dh*dh + ds*ds + dv*dv;
			
			BOOST_ASSERT(result >= 0);
			if (result > max_value<T>::value())
				result = max_value<T>::value();
			return result;
		}
	};
	
	/// \brief A class for calculating the squared L2 distance between two
	/// HSV colors.
	///
	/// The calculation of a square root is aved when calculating the
	/// squared distance.
	///
	template <typename T>
	struct difference_squared<T, false>
	{
		typedef long int BigT;

		static T calculate(const color_hsv<T> & x, const color_hsv<T> & y)
		{
			BigT dh = x.h; dh -= y.h;
			BigT ds = x.s; ds -= y.s;
			BigT dv = x.v; dv -= y.v;
			BigT result = dh*dh + ds*ds + dv*dv;
			
			BOOST_ASSERT(result >= 0);
			if (result > max_value<T>::value())
				result = max_value<T>::value();
			return static_cast<T>(result);
		}
	};
	
	
	/// \brief A class for calculating the L2 distance between two
	/// HSV colors.
	///
	template <typename T, bool FP = boost::is_float<T>::value>
	struct difference {};
	
	/// \brief A class for calculating the L2 distance between two
	/// HSV colors.
	///
	template <typename T>
	struct difference<T, true>
	{
		static T calculate(const color_hsv<T> & x, const color_hsv<T> & y)
		{
			T dh = x.h - y.h;
			T ds = x.s - y.s;
			T dv = x.v - y.v;
			T result = std::sqrt(dh*dh + ds*ds + dv*dv);
			
			BOOST_ASSERT(result >= 0);
			if (result > max_value<T>::value())
				result = max_value<T>::value();
			return result;
		}
	};
	
	/// \brief A class for calculating the L2 distance between two
	/// HSV colors.
	///
	template <typename T>
	struct difference<T, false>
	{
		typedef double BigT;
		
		static T calculate(const color_hsv<T> & x, const color_hsv<T> & y)
		{
			BigT dh = x.h; dh -= y.h;
			BigT ds = x.s; ds -= y.s;
			BigT dv = x.v; dv -= y.v;
			BigT result = std::sqrt(dh*dh + ds*ds + dv*dv);
			
			BOOST_ASSERT(result >= 0);
			// Result may be greater than the maximum value of a T
			// since we are calculating the difference in a 3
			// dimensional space.
			if (result > max_value<T>::value())
				result = max_value<T>::value();
			return static_cast<T>(result);
		}
	};
	
	
    /// \brief A generic class for converting a color from RGB
    /// representation to HSV representation.
	///
	/// Template parameter T specifices the type of each color
	/// component. If T is a floating point type, a value range of
	/// [0, 1] is assumed for each component.  For integral types
	/// the range [0, std::numeric_limits<T>::max()] is used.
	///
	template <typename T, bool FP = boost::is_float<T>::value>
	struct rgb_to_hsv {};

    /// \brief A generic class for converting a color from RGB
    /// representation to HSV representation.
	///
	/// Template parameter T specifices a floating point type of each color
	/// component. A value range of [0, 1] is assumed for each component.
	///
	template <typename T>
	struct rgb_to_hsv<T, true>
	{
		/// \brief Takes a color represented in floating point RGB values
		/// and returns the color in floating point HSV
		/// representation.
        ///
		static color_hsv<T> convert(const color_rgb<T> & x)
		{
			const T & r = x.r;
			const T & g = x.g;
			const T & b = x.b;
			color_hsv<T> result(0, 0, 0);
			T & h = result.h;
			T & s = result.s;
			T & v = result.v;
	
			T max_sample = std::max(std::max(r, g), b);
			T min_sample = std::min(std::min(r, g), b);
			T variance = max_sample - min_sample;

			v = max_sample;
	
			if (variance <= 0.0001) {
				s = 0;
				h = 0;
			}
			else {
				s = variance / max_sample;
		
				if (r == max_sample) {
					h = (g - b) / variance;
					if (h < 0)
						h += 6;
				}
				else if (g == max_sample) {
					h = 2 + (b - r) / variance;
				}
				else {
					BOOST_ASSERT(b == max_sample);
					h = 4 + (r - g) / variance;
				}
		
				h /= 6;
			}
	
			return result;
		}
	};

    /// \brief A generic class for converting a color from RGB
    /// representation to HSV representation.
	///
	/// Template parameter T specifices an integral type of each color
	/// component. A value range of [0, std::numeric_limits<T>::max()]
	/// is assumed for each component.
	///
	template <typename T>
	struct rgb_to_hsv<T, false>
	{
		typedef long int BigT;

		/// \brief Takes a color represented in integer RGB values and
		/// returns the color in integer HSV representation.
		///
		static color_hsv<T> convert(const color_rgb<T> & x)
		{
			const T r = static_cast<BigT>(x.r);
			const T g = static_cast<BigT>(x.g);
			const T b = static_cast<BigT>(x.b);
			BigT h = 0;
			BigT s = 0;
			BigT v = 0;
			
			BigT max_sample = std::max(std::max(r, g), b);
			BigT min_sample = std::min(std::min(r, g), b);
			BigT variance = max_sample - min_sample;
	
			v = max_sample;
	
			if (variance == 0) {
				s = 0;
				h = 0;
			}
			else {
				s = variance * max_value<T>::value() / max_sample;
		
				if (r == max_sample) {
					h = (g - b) * max_value<T>::value() / variance;
					if (h < 0)
						h += 6 * max_value<T>::value();
				}
				else if (g == max_sample) {
					h = (2 * max_value<T>::value()) +
						((b - r) * max_value<T>::value() / variance);
				}
				else {
					BOOST_ASSERT(b == max_sample);
					h = (4 * max_value<T>::value()) +
						((r - g) * max_value<T>::value() / variance);
				}
				
				h /= 6;
			}
			
			BOOST_ASSERT(h >= 0);
			BOOST_ASSERT(h <= max_value<T>::value());
			BOOST_ASSERT(s >= 0);
			BOOST_ASSERT(s <= max_value<T>::value());
			BOOST_ASSERT(v >= 0);
			BOOST_ASSERT(v <= max_value<T>::value());
			return color_hsv<T>(
				static_cast<T>(h),
				static_cast<T>(s),
				static_cast<T>(v));
		}
	};
	
	/// \brief Writes three values to a stream, separated by commas.
	///
	template <typename T>
	inline std::ostream & write_comma_separated(
		std::ostream & s, const T & x, const T & y, const T & z)
	{
	    s << x << "," << y << "," << z;
	    return s;
	}
	
	/// \brief Reads three comma separated values from a stream.
	///
	template <typename T>
	inline std::istream & read_comma_separated(
		std::istream & s, T & x, T & y, T & z)
	{
		using namespace std;
		
		string str;
		s >> str;
		
		string::size_type comma_1 = str.find(',');
		string::size_type comma_2 = str.find(',', comma_1 + 1);
		string x_str = str.substr(0, comma_1);
		string y_str = str.substr(comma_1 + 1, comma_2 - (comma_1 + 1));
		string z_str = str.substr(comma_2 + 1);
		
		istringstream x_iss(x_str);
		x_iss >> x;
		
		istringstream y_iss(y_str);
		y_iss >> y;
		
		istringstream z_iss(z_str);
		z_iss >> z;
		
	    return s;
	}
	
	
	/// \brief Get the red value from color_rgb<InT> as OutT.
	///
	template <
		typename InT,
		typename OutT,
		bool InIsFloat = boost::is_float<InT>::value,
		bool OutIsFloat = boost::is_float<OutT>::value>
	struct r_from_color_rgb {};

	/// \brief Get the red value from color_rgb<T>.
	///
	template <typename T>
	struct r_from_color_rgb<T, T, false, false>
	{
		static T get(const color_rgb<T> & in)
		{ return in.r; }
	};

	/// \brief Get the red value from color_rgb<InT> as OutT.
	///
	template <typename InT, typename OutT>
	struct r_from_color_rgb<InT, OutT, true, false>
	{
		static OutT get(const color_rgb<InT> & in)
		{ return static_cast<OutT>(in.r * std::numeric_limits<OutT>::max()); }
	};

	/// \brief Get the red value from color_rgb<InT> as OutT.
	///
	template <typename InT, typename OutT>
	struct r_from_color_rgb<InT, OutT, false, true>
	{
		static OutT get(const color_rgb<InT> & in)
		{ return static_cast<OutT>(in.r) / std::numeric_limits<InT>::max(); }
	};

	/// \brief Get the green value from color_rgb<InT> as OutT.
	///
	template <
		typename InT,
		typename OutT,
		bool InIsFloat = boost::is_float<InT>::value,
		bool OutIsFloat = boost::is_float<OutT>::value>
	struct g_from_color_rgb {};

	/// \brief Get the green value from color_rgb<T>.
	///
	template <typename T>
	struct g_from_color_rgb<T, T, false, false>
	{
		static T get(const color_rgb<T> & in)
		{ return in.g; }
	};

	/// \brief Get the green value from color_rgb<InT> as OutT.
	///
	template <typename InT, typename OutT>
	struct g_from_color_rgb<InT, OutT, true, false>
	{
		static OutT get(const color_rgb<InT> & in)
		{ return static_cast<OutT>(in.g * std::numeric_limits<OutT>::max()); }
	};

	/// \brief Get the green value from color_rgb<InT> as OutT.
	///
	template <typename InT, typename OutT>
	struct g_from_color_rgb<InT, OutT, false, true>
	{
		static OutT get(const color_rgb<InT> & in)
		{ return static_cast<OutT>(in.g) / std::numeric_limits<InT>::max(); }
	};

	/// \brief Get the blue value from color_rgb<InT> as OutT.
	///
	template <
		typename InT,
		typename OutT,
		bool InIsFloat = boost::is_float<InT>::value,
		bool OutIsFloat = boost::is_float<OutT>::value>
	struct b_from_color_rgb {};

	/// \brief Get the blue value from color_rgb<T>.
	///
	template <typename T>
	struct b_from_color_rgb<T, T, false, false>
	{
		static T get(const color_rgb<T> & in)
		{ return in.b; }
	};

	/// \brief Get the blue value from color_rgb<InT> as OutT.
	///
	template <typename InT, typename OutT>
	struct b_from_color_rgb<InT, OutT, true, false>
	{
		static OutT get(const color_rgb<InT> & in)
		{ return static_cast<OutT>(in.b * std::numeric_limits<OutT>::max()); }
	};

	/// \brief Get the blue value from color_rgb<InT> as OutT.
	///
	template <typename InT, typename OutT>
	struct b_from_color_rgb<InT, OutT, false, true>
	{
		static OutT get(const color_rgb<InT> & in)
		{ return static_cast<OutT>(in.b) / std::numeric_limits<InT>::max(); }
	};
	
	
	/// \brief Get the red value from an RGB or BGR array of InT samples as
	/// OutT.
	///
	template <
		typename InT,
		typename OutT,
		bool RGBOrder,
		bool InIsFloat = boost::is_float<InT>::value,
		bool OutIsFloat = boost::is_float<OutT>::value>
	struct r_from_array {};

	/// \brief Get the red value from an RGB array of T samples.
	///
	template <typename T>
	struct r_from_array<T, T, true, false, false>
	{
		static T get(const T * in)
		{ return in[0]; }
	};

	/// \brief Get the red value from an BGR array of T samples.
	///
	template <typename T>
	struct r_from_array<T, T, false, false, false>
	{
		static T get(const T * in)
		{ return in[2]; }
	};

	/// \brief Get the red value from an RGB array of InT samples as
	/// OutT.
	///
	template <typename InT, typename OutT>
	struct r_from_array<InT, OutT, true, true, false>
	{
		static OutT get(const InT * in)
		{ return static_cast<OutT>(in[0] * std::numeric_limits<OutT>::max()); }
	};

	/// \brief Get the red value from an BGR array of InT samples as
	/// OutT.
	///
	template <typename InT, typename OutT>
	struct r_from_array<InT, OutT, false, true, false>
	{
		static OutT get(const InT * in)
		{ return static_cast<OutT>(in[2] * std::numeric_limits<OutT>::max()); }
	};

	/// \brief Get the red value from an RGB array of InT samples as
	/// OutT.
	///
	template <typename InT, typename OutT>
	struct r_from_array<InT, OutT, true, false, true>
	{
		static OutT get(const InT * in)
		{ return static_cast<OutT>(in[0]) / std::numeric_limits<InT>::max(); }
	};

	/// \brief Get the red value from an BGR array of InT samples as
	/// OutT.
	///
	template <typename InT, typename OutT>
	struct r_from_array<InT, OutT, false, false, true>
	{
		static OutT get(const InT * in)
		{ return static_cast<OutT>(in[2]) / std::numeric_limits<InT>::max(); }
	};

	/// \brief Get the green value from an RGB or BGR array of InT samples as OutT.
	///
	template <
		typename InT,
		typename OutT,
		bool RGBOrder,
		bool InIsFloat = boost::is_float<InT>::value,
		bool OutIsFloat = boost::is_float<OutT>::value>
	struct g_from_array {};

	/// \brief Get the green value from an RGB array of T samples.
	///
	template <typename T>
	struct g_from_array<T, T, true, false, false>
	{
		static T get(const T * in)
		{ return in[1]; }
	};

	/// \brief Get the green value from an BGR array of T samples.
	///
	template <typename T>
	struct g_from_array<T, T, false, false, false>
	{
		static T get(const T * in)
		{ return in[1]; }
	};

	/// \brief Get the green value from an RGB array of InT samples as OutT.
	///
	template <typename InT, typename OutT>
	struct g_from_array<InT, OutT, true, true, false>
	{
		static OutT get(const InT * in)
		{ return static_cast<OutT>(in[1] * std::numeric_limits<OutT>::max()); }
	};

	/// \brief Get the green value from an BGR array of InT samples as OutT.
	///
	template <typename InT, typename OutT>
	struct g_from_array<InT, OutT, false, true, false>
	{
		static OutT get(const InT * in)
		{ return static_cast<OutT>(in[1] * std::numeric_limits<OutT>::max()); }
	};

	/// \brief Get the green value from an RGB array of InT samples as OutT.
	///
	template <typename InT, typename OutT>
	struct g_from_array<InT, OutT, true, false, true>
	{
		static OutT get(const InT * in)
		{ return static_cast<OutT>(in[1]) / std::numeric_limits<InT>::max(); }
	};

	/// \brief Get the green value from an BGR array of InT samples as OutT.
	///
	template <typename InT, typename OutT>
	struct g_from_array<InT, OutT, false, false, true>
	{
		static OutT get(const InT * in)
		{ return static_cast<OutT>(in[1]) / std::numeric_limits<InT>::max(); }
	};

	/// \brief Get the blue value from an RGB or BGR array of InT samples as
	/// OutT.
	///
	template <
		typename InT,
		typename OutT,
		bool RGBOrder,
		bool InIsFloat = boost::is_float<InT>::value,
		bool OutIsFloat = boost::is_float<OutT>::value>
	struct b_from_array {};

	/// \brief Get the blue value from an RGB array of T samples.
	///
	template <typename T>
	struct b_from_array<T, T, true, false, false>
	{
		static T get(const T * in)
		{ return in[2]; }
	};
	
	/// \brief Get the blue value from an BGR array of T samples.
	///
	template <typename T>
	struct b_from_array<T, T, false, false, false>
	{
		static T get(const T * in)
		{ return in[0]; }
	};
	
	/// \brief Get the blue value from an RGB array of InT samples as
	/// OutT.
	///
	template <typename InT, typename OutT>
	struct b_from_array<InT, OutT, true, true, false>
	{
		static OutT get(const InT * in)
		{ return static_cast<OutT>(in[2] * std::numeric_limits<OutT>::max()); }
	};
	
	/// \brief Get the blue value from an BGR array of InT samples as
	/// OutT.
	///
	template <typename InT, typename OutT>
	struct b_from_array<InT, OutT, false, true, false>
	{
		static OutT get(const InT * in)
		{ return static_cast<OutT>(in[0] * std::numeric_limits<OutT>::max()); }
	};

	/// \brief Get the blue value from an RGB array of InT samples as
	/// OutT.
	///
	template <typename InT, typename OutT>
	struct b_from_array<InT, OutT, true, false, true>
	{
		static OutT get(const InT * in)
		{ return static_cast<OutT>(in[2]) / std::numeric_limits<InT>::max(); }
	};

	/// \brief Get the blue value from an BGR array of InT samples as
	/// OutT.
	///
	template <typename InT, typename OutT>
	struct b_from_array<InT, OutT, false, false, true>
	{
		static OutT get(const InT * in)
		{ return static_cast<OutT>(in[0]) / std::numeric_limits<InT>::max(); }
	};
	
	
} }

#endif
