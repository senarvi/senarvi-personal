/*
    Copyright (C) 2002 Paul Davis 

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

    $Id: pthread_utils.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <map>
#include <iostream>
#include <string>

#include <pbd/pthread_utils.h>

using std::string;
using std::cerr;
using std::endl;

typedef std::map<string,pthread_t> ThreadMap;
static ThreadMap all_threads;

/* XXX NOT THREAD-SAFE. THIS IS, FOR NOW, A QUICK AND EASY WRAPPER
   TO MAKE KILLING ALL PERSISTENT THREADS REASONABLY EASY.
*/

int  
pthread_create_and_store (string name, pthread_t  *thread, pthread_attr_t *attr, void * (*start_routine)(void *), void * arg)
{
	int ret;

	if ((ret = pthread_create (thread, attr, start_routine, arg)) == 0) {
		std::pair<string,pthread_t> newpair;
		newpair.first = name;
		newpair.second = *thread;
		all_threads.insert (newpair);
	}
	
	return ret;
}

void
pthread_kill_all (int signum) 
{	
	for (ThreadMap::iterator i = all_threads.begin(); i != all_threads.end(); ++i) {
		if (i->second != pthread_self()) {
			cerr << "killing " << i->first << " thread" << endl;
			pthread_kill (i->second, signum);
		}
	}
	all_threads.clear();
}

void
pthread_cancel_all () 
{	
	for (ThreadMap::iterator i = all_threads.begin(); i != all_threads.end(); ++i) {
		if (i->second != pthread_self()) {
			cerr << "stopping " << i->first << " thread" << endl;
			pthread_cancel (i->second);
		}
	}
	all_threads.clear();
}

void
pthread_cancel_one (pthread_t thread) 
{	
	for (ThreadMap::iterator i = all_threads.begin(); i != all_threads.end(); ++i) {
		if (i->second == thread) {
			cerr << "stopping " << i->first << " thread" << endl;
			all_threads.erase (i);
			break;
		}
	}

	pthread_cancel (thread);
}
