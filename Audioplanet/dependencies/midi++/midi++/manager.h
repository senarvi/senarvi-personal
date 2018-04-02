/*
    Copyright (C) 1998 Paul Barton-Davis
    
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

    $Id: manager.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __midi_manager_h__
#define __midi_manager_h__

#include <midi++/types.h>
#include <midi++/port.h>

/* This really shouldn't be defined here, but I can't figure out where
   it should live without creating cross-library dependencies.
*/

#ifndef LESS_CONST_CHAR_P
template <>
struct less<const char *>
{
	bool operator()(const char* s1, const char* s2) const {
		return strcmp(s1, s2) < 0;
	}
};
#define LESS_CONST_CHAR_P
#endif // LESS_CONST_CHAR_P

namespace MIDI {

class Manager {
  public:
	~Manager ();
	
	Port *add_port (PortRequest &);
	int   remove_port (const char *port);

	Port *port (const string &name);
	Port *port (size_t number);

	size_t    nports () { return ports_by_device.size(); }

	/* defaults for clients who are not picky */
	
	Port *inputPort;
	Port *outputPort;
	channel_t inputChannelNumber;
	channel_t outputChannelNumber;

	int set_input_port (size_t port);
	int set_input_port (const char *);
	int set_output_port (size_t port);
	int set_output_port (const char *);
	int set_input_channel (channel_t);
	int set_output_channel (channel_t);

	int foreach_port (int (*func)(const Port &, size_t n, void *), 
			  void *arg);

	typedef map<const char *, Port *> PortMap;

	const PortMap& get_midi_ports() const { return ports_by_tag; } 

	static Manager *instance () {
		if (theManager == 0) {
			theManager = new Manager;
		}
		return theManager;
	}

	static int parse_port_request (const string &str, Port::Type type);

  private:
	/* This is a SINGLETON pattern */
	
	Manager ();

	static Manager *theManager;
	PortMap         ports_by_device; /* canonical */
	PortMap         ports_by_tag;    /* may contain duplicate Ports */

	void close_ports ();
};

}; /* namespace MIDI */

#endif  // __midi_manager_h__
