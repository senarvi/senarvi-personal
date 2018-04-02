//  generalfunctors.h
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_BASE_GENERALFUNCTORS_H
#define BACKEND_BASE_GENERALFUNCTORS_H


namespace ap
{

    template <typename T>
    class Delete : public std::unary_function<T, void>
    {
    public:
	typedef typename std::unary_function<T, void>::result_type result_type;
	typedef typename std::unary_function<T, void>::argument_type argument_type;

	result_type operator()(const argument_type & x)
	{
	    delete x;
	}
    };


    template <typename Container>
    class MaxSize : public std::unary_function<Container, void>
    {
    public:
	typedef typename Container::size_type size_type;
	typedef typename std::unary_function<Container, void>::result_type result_type;
	typedef typename std::unary_function<Container, void>::argument_type argument_type;

	MaxSize() : value_(0) {}

	size_type value() const { return value_; }

	result_type operator()(const argument_type & x)
	{
	    value_ = std::max(x.size(), value_);
	}

    private:
	size_type value_;
    };

}  // namespace system


#endif  // BACKEND_BASE_GENERALFUNCTORS_H

// Local Variables:
// mode:C++
// End:
