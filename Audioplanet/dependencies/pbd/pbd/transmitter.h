/*
    Copyright (C) 1998-99 Paul Barton-Davis 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    $Id: transmitter.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __libmisc_transmitter_h__
#define __libmisc_transmitter_h__

#include <sstream>
#include <iostream>

#include <sigc++/signal_system.h>

using std::cout;
using std::cerr;
using std::endl;

class Transmitter : public std::stringstream, public SigC::Object

{
  public:
	enum Channel {
		Info,
		Error,
		Warning,
		Fatal,
		Throw
	};

	Transmitter (Channel);

	SigC::Signal2<void,Channel, const char *> &sender() { 
		return *send;
	}

	bool does_not_return ();

  protected:
	virtual void deliver ();
	friend std::ostream& endmsg (std::ostream &);

  private:
	Channel channel;
	SigC::Signal2<void, Channel, const char *> *send;

	SigC::Signal2<void, Channel, const char *> info;
	SigC::Signal2<void, Channel, const char *> warning;
	SigC::Signal2<void, Channel, const char *> error;
	SigC::Signal2<void, Channel, const char *> fatal;
};

/* for EGCS 2.91.66, if this function is not compiled within the same
   compilation unit as the one where a ThrownError is thrown, then 
   nothing will catch the error. This is a pretty small function, so
   inlining it here seems like a reasonable workaround.
*/

inline std::ostream &
endmsg (std::ostream &ostr)

{
	Transmitter *t;

	/* There is a serious bug in the Cygnus/GCC libstdc++ library:
	   cout is not actually an ostream, but a trick was played
	   to make the compiler think that it is. This will cause
	   the dynamic_cast<> to fail with SEGV. So, first check to
	   see if ostr == cout, and handle it specially.
	*/

	if (&ostr == &cout) {
		cout << endl;
		return ostr;
	} else if (&ostr == &cerr) {
		cerr << endl;
		return ostr;
	}

	if ((t = dynamic_cast<Transmitter *> (&ostr)) != 0) {
		t->deliver ();
	} else {
		/* hmm. not a Transmitter, so just put a newline on
		   it and assume that that will be enough.
		*/
		
		ostr << endl;
	}

	return ostr;
}

extern "C" { void pbd_c_error (const char *); }

#endif // __libmisc_transmitter_h__
