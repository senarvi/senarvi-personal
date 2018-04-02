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

    $Id: receiver.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <vector>

#include <pbd/receiver.h>
#include <pbd/transmitter.h>

using namespace SigC;

Receiver::Receiver () {}

Receiver::~Receiver ()

{
	hangup ();
}

void
Receiver::hangup ()
{
	vector<SigC::Connection *>::iterator i;

	for (i = connections.begin(); i != connections.end (); i++) {
		(*i)->disconnect ();
		delete *i;
	}

	connections.erase (connections.begin(), connections.end());
}

void
Receiver::listen_to (Transmitter &transmitter)

{
	SigC::Connection *c = new SigC::Connection;

	(*c) = transmitter.sender().connect (slot (*this, &Receiver::receive));

	connections.push_back (c);
}
