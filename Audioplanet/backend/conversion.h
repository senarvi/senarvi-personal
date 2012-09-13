//  conversion.h
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef LIBAUDIOPLANET_CONVERSION_H
#define LIBAUDIOPLANET_CONVERSION_H


#include <limits>

#include "system/exception.h"
#include "system/types.h"


namespace AP {


static Uint64 powerOf2(unsigned int x)
{
	Uint64 result = 1;
	
	for (unsigned int i = 0; i < x; i++)
		result *= 2;
	
	return result;
}


	namespace Interpretation {


		// Notice:
		// Integer interpreters only work when converting numbers with less bits
		// to numbers with more bits


		namespace LittleEndian {


			template <typename T>
			class IntegerInterpreter {

			private:

				unsigned int outputSampleSize() const EXCEPTIONS {

					return NumBits<T>::value;
				}

			public:

				void initialize(unsigned int inputSampleSize, bool inputSampleSigned) EXCEPTIONS {

					filter_ = powerOf2(inputSampleSize) - 1;
					shift_ = outputSampleSize() - inputSampleSize;

					if ((std::numeric_limits<T>::is_signed) && (!inputSampleSigned))
						bias_ = -powerOf2(inputSampleSize - 1);
					else if ((!std::numeric_limits<T>::is_signed) && inputSampleSigned)
						throw FlowOfControlException(LOCATION);  // Not implemented
					else
						bias_ = 0;
				}


				T read(void * pointer) const EXCEPTIONS
				{
					T * castedPointer = static_cast<T *>(pointer);
					T sample = *castedPointer;
					
					sample &= filter_;
					sample += bias_;
					sample <<= shift_;
					
					return sample;
				}


			private:

				Int64 bias_;
				Uint64 filter_;
				Uint64 shift_;

			};


		}  // namespace LittleEndian


		namespace BigEndian {
		}  // namespace BigEndian


	}  // namespace Interpretation


}  // namespace AP





/*
inline void convertInt8ToInt16(gint8 *original_data, buffer_index_t length,
			       gint16 *new_data) {

    trace("convertInt8ToInt16");


    for (int i = 0; i < length; i++)
	new_data[i * 2] = static_cast<gint16>(original_data[i]);
}

inline void convertInt32ToInt16(gint32 *original_data, buffer_index_t length,
				gint16 *new_data) {

    trace("convertInt32ToInt16");


    for (int i = 0; i < length; i++)
	new_data[i * 2] = static_cast<gint16>(original_data[i]);
}

inline void convertFloat32ToInt16(gfloat32 *original_data, buffer_index_t length,
				  gint16 *new_data) {

    trace("convertFloat32ToInt16");


    for (int i = 0; i < length; i++)
	new_data[i * 2] = static_cast<gint16>(original_data[i]);
}

inline void convertFloat64ToInt16(gfloat64 *original_data, buffer_index_t length,
				  gint16 *new_data) {

    trace("convertFloat64ToInt16");


    for (int i = 0; i < length; i++)
	new_data[i] = static_cast<gint16>(original_data[i]);
}

inline void convertFloat80ToInt16(gfloat80 *original_data, buffer_index_t length,
				  gint16 *new_data) {

    trace("convertFloat80ToInt16");


    for (int i = 0; i < length; i++)
	new_data[i] = static_cast<gint16>(original_data[i]);
}

void convertToInt16(const void *original_data, Channel::unit_t original_type,
		    buffer_index_t length, gint16 *new_data) {

    trace("convertToInt16");


    switch (original_type) {
    case Channel::int_8:
	convertInt8ToInt16(static_cast<gint8 *>(original_data), length, new_data);
	break;

    case Channel::int_16:
	memcpy(new_data, original_data, length * 2);
	break;

    case Channel::int_32:
	convertInt32ToInt16(static_cast<gint32 *>(original_data), length, new_data);
	break;

    case Channel::fp_32:
	convertFloat32ToInt16(static_cast<gfloat32 *>(original_data), length, new_data);
	break;

    case Channel::fp_64:
	convertFloat64ToInt16(static_cast<gfloat64 *>(original_data), length, new_data);
	break;

    case Channel::fp_80:
	convertFloat80ToInt16(static_cast<gfloat80 *>(original_data), length, new_data);
    }
}
*/



#endif  // LIBAUDIOPLANET_CONVERSION_H

// Local Variables:
// mode:C++
// End:

