//  backend/base/stacktrace.h
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_BASE_STACKTRACE_H
#define BACKEND_BASE_STACKTRACE_H


namespace ap
{

	class StackTrace
	{
		void ** array_;
		int length_;

	public:

		StackTrace(int maxLength);
		~StackTrace();

		// firstSymbol:
		//   0  StackTrace::StackTrace
		//   1  the calling function
		void display(int firstSymbol) const;
	};

}  // namespace ap


#endif  // BACKEND_BASE_STACKTRACE_H

// Local Variables:
// mode:C++
// End:
