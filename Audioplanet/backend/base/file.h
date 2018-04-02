//  backend/base/file.h
//  Copyright (C) 2001-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_BASE_FILE_H
#define BACKEND_BASE_FILE_H


#include <string>


namespace ap
{

// A class representing a file descriptor. Contains methods for opening and
// closing files and reading / writing simple objects from / to a file.
//
    class File
    {
    public:
	File();
	File(const std::string & fileName, int flags);
	File(int descriptor);
	
	operator int() const { return descriptor_; }
	
	
	void setDescriptor(int descriptor) { descriptor_ = descriptor; }
	
	void open(const std::string & fileName, int flags);
	void close();
	
	ssize_t read(void * buffer, size_t count) const;
	ssize_t write(const void * buffer, size_t count) const;
	
	void attachToStdin() const;
	void attachToStdout() const;
	
	
/*	const File & operator>>(int & x) const;
	const File & operator>>(unsigned int & x) const;
	const File & operator>>(Int32 & x) const;
	const File & operator>>(Uint32 & x) const;
	const File & operator>>(Float32 & x) const;
	const File & operator>>(Float64 & x) const;
	
	const File & operator<<(Int16 x) const;
	const File & operator<<(Uint16 x) const;
	const File & operator<<(Int32 x) const;
	const File & operator<<(Uint32 x) const;
	const File & operator<<(Float32 x) const;
	const File & operator<<(Float64 x) const;
	
	const File & operator>>(std::string & x) const;
	const File & operator<<(const std::string & x) const;*/
	
	
    private:
	
	template <typename T>
	void readSimple(T & x) const;
	template <typename T>
	void writeSimple(const T & x) const;
	
	
	int descriptor_;
    };


    inline File::File() :

	descriptor_(-1)
	
    {}


    inline File::File(const std::string & fileName, int flags)
    {
	open(fileName, flags);
    }


    inline File::File(int descriptor) :

	descriptor_(descriptor)
	
    {}


//
// Read / write simple objects from / to the file. These methods assume that
// the size of the object is constant.
//


    template <typename T>
    inline void File::readSimple(T & x) const
    {
	read(&x, sizeof(x));
    }


    template <typename T>
    inline void File::writeSimple(const T & x) const
    {
	write(&x, sizeof(x));
    }


//
// Simple read operators.
//


/*inline const File & File::operator>>(Int16 & x) const
  {
  readSimple(x);
  return *this;
  }


  inline const File & File::operator>>(Uint16 & x) const
  {
  readSimple(x);
  return *this;
  }


  inline const File & File::operator>>(Int32 & x) const
  {
  readSimple(x);
  return *this;
  }


  inline const File & File::operator>>(Uint32 & x) const
  {
  readSimple(x);
  return *this;
  }


  inline const File & File::operator>>(Float32 & x) const
  {
  readSimple(x);
  return *this;
  }


  inline const File & File::operator>>(Float64 & x) const
  {
  readSimple(x);
  return *this;
  }


//
// Simple write operators.
//


inline const File & File::operator<<(Int16 x) const
{
writeSimple(x);
return *this;
}


inline const File & File::operator<<(Uint16 x) const
{
    writeSimple(x);
    return *this;
}


inline const File & File::operator<<(Int32 x) const
{
    writeSimple(x);
    return *this;
}


inline const File & File::operator<<(Uint32 x) const
{
    writeSimple(x);
    return *this;
}


inline const File & File::operator<<(Float32 x) const
{
    writeSimple(x);
    return *this;
}


inline const File & File::operator<<(Float64 x) const
{
    writeSimple(x);
    return *this;
}


//
// String operators. These only transfer the length of the string and the actual
// characters using the simple transfer methods, they don't transfer the string
// object itself.
//


inline const File & File::operator>>(std::string & x) const
{
    std::size_t length;
    readSimple(length);

    char cString[length + 1];
    read(cString, length);
    cString[length] = 0;

    x = cString;
    return *this;
}


inline const File & File::operator<<(const std::string & x) const
{
    const char * cString = x.c_str();
    std::size_t length = std::strlen(cString);
	
    writeSimple(length);
    write(cString, length);
	
    return *this;
}*/

}  // namespace ap


#endif  // BACKEND_BASE_FILE_H

// Local Variables:
// mode:C++
// End:
