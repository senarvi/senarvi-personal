/*
    Copyright (C) 1999 Paul Barton-Davis 

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

    $Id: irix_platform.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef _LANGUAGE_C
#define _LANGUAGE_C
#define _CANCEL_LC_
#endif

extern "C" {
#include <sys/fpu.h>
}


#ifdef _CANCEL_LC_
#undef _LANGUAGE_C
#undef _CANCEL_LC_
#endif

#include <pbd/error.h>

#include <sys/signal.h>
#include <dmedia/midi.h>

#include <pbd/irix_platform.h>

// hack
char* sgi_midi_register;

static void
no_fpu_exceptions ()

{
   	union fpc_csr f;
	f.fc_word = get_fpc_csr();
   	f.fc_struct.flush = 1;
	set_fpc_csr(f.fc_word);
}

IrixPlatform::~IrixPlatform()
{
	if(sgi_midi_register)
		mdUnRegister(sgi_midi_register);
}
	
int
IrixPlatform::pre_config ()

{

	
	// since quasimodo is alpha, get rid of quasimodoXX interfaces...
	// but the code does not work if there are many QuasimodoXX interfaces
	// it seems that unregistering one and immediatly after, calling mdInit
	// does not unregister it at all...
	// Anyway, there should be only one Quasimodo interface, so...
	int num_interface = mdInit();
	
	for(int i=0; i<num_interface; ++i) {
		char *name=mdGetName(i);
		char prefix[10];
		if(strlen(name)>=9) { // maybe QuasimodoXX
			strncpy(prefix, name, 9);
		}
		if(!strcmp(prefix, "Quasimodo")) {
			warning << "since quasimodo is in alpha stage, we unregister the '" << name << "' midi interface" << endmsg;
			mdUnRegister(name);
			i--;
			num_interface=mdInit();
		}
	}
	
	sgi_midi_register=mdRegister("Quasimodo");
	if(!sgi_midi_register) {
		error << "unable to register a midi interface (mdRegister)." << endmsg;
		return 1;
	}
	return 0;
}

int
IrixPlatform::post_config ()

{
	return 0;
}

int
IrixPlatform::pre_ui ()

{
	return 0;
}

int
IrixPlatform::post_ui ()

{
	return 0;
}

int
IrixPlatform::dsp_startup()
{
	static int i=0;
	no_fpu_exceptions ();
	info << "setting no fpu exception #" << ++i << endmsg;
	return 0;
}
