//  libaudioplanet/messagetype.h
//  Copyright (C) 2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef LIBAUDIOPLANET_MESSAGETYPE_H
#define LIBAUDIOPLANET_MESSAGETYPE_H


namespace ap
{

	enum MessageType
	{
		INCOMING_FORMAT,
		INCOMING_PACKETS,
		END_OF_STREAM
	};


//
// File operators
//
/*

inline const File & operator>>(const File & file, MessageType & messageType) EXCEPTIONS
{
file.read(&messageType, sizeof(messageType));
return file;
}


inline const File & operator<<(const File & file, const MessageType & messageType) EXCEPTIONS
{
	file.write(&messageType, sizeof(messageType));
	return file;
}
*/

inline std::istream & operator>>(std::istream & s,
				 MessageType & messageType)
{
    s.read(reinterpret_cast<char *>(&messageType), sizeof(messageType));
    return s;
}


inline std::ostream & operator<<(std::ostream & s,
				 const MessageType & messageType)
{
    s.write(reinterpret_cast<const char *>(&messageType), sizeof(messageType));
    return s;
}


//
// MessageTypeException
//


inline std::string toString(MessageType messageType)
{
	switch (messageType)
	{
	case INCOMING_FORMAT: return "INCOMING_FORMAT";
	case INCOMING_PACKETS: return "INCOMING_PACKETS";
	case END_OF_STREAM: return "END_OF_STREAM";
	default: return boost::lexical_cast<std::string>(static_cast<int>(messageType));
	}
}


class MessageTypeMismatchException : std::exception
{
public:
	const char * what() const throw ()
	{
		return "A message with invalid type received.";
	}
};


}  // namespace ap


#endif  // LIBAUDIOPLANET_MESSAGETYPE_H

// Local Variables:
// mode:C++
// End:
