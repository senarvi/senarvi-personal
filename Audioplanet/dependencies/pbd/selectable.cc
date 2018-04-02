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

    $Id: selectable.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include <pbd/pthread_spinlock.h>
#include <pbd/selectable.h>


using namespace std;
using namespace Select;

bool Selector::use_list_lock = false;

Selector::Selector ()

{
	_max_fd = -1;
	pthread_mutex_init (&list_lock, 0);
	use_list_lock = false;
}

void
Selector::add (int cond, Selectable *s)

{
	s->condition = cond;
	if (use_list_lock) {
		pthread_mutex_spinlock (&list_lock);
	}
	selectables.push_front (s);
	if (use_list_lock) {
		pthread_mutex_unlock (&list_lock);
	}

	if (s->_fd > _max_fd) {
		_max_fd = s->_fd;
	}
}

void
Selector::remove (Selectable *s)

{
	selectables.remove (s);
}

int
Selector::select (unsigned long usecs)

{
	int r;
	fd_set rfds;
	fd_set wfds;
	fd_set efds;
	struct timeval tv;
	Selectables::iterator i;

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&efds);

	if (use_list_lock) {
		pthread_mutex_spinlock (&list_lock);
	}
	for (i = selectables.begin(); i != selectables.end(); i++) {
		int fd;

		fd = (*i)->_fd;

		if ((*i)->condition & Select::Readable) {
			FD_SET(fd, &rfds);
		}
		if ((*i)->condition & Select::Writable) {
			FD_SET(fd, &wfds);
		}
		if ((*i)->condition & Select::Exception) {
			FD_SET(fd, &efds);
		}
	}
	if (use_list_lock) {
		pthread_mutex_unlock (&list_lock);
	}

  again:
	if (usecs > 1000000) {
		tv.tv_sec = usecs / 1000000;
		tv.tv_usec = usecs % 1000000;
	} else {
		tv.tv_sec = 0;
		tv.tv_usec = usecs;
	}

	if ((r = ::select (_max_fd + 1, &rfds, &wfds, &efds, &tv)) > 0) {
		return post_select (&rfds, &wfds, &efds);

	} else if (r < 0) {
		if (errno == EINTR) {
			goto again; /* XXX need to adjust timeout ? */
		}
		return -1;
	} else {
		return 0;
	}
}

int
Selector::post_select (fd_set *r, fd_set *w, fd_set *e)

{
	Selectables::iterator i;

	if (use_list_lock) {
		pthread_mutex_spinlock (&list_lock);
	}
	for (i = selectables.begin(); i != selectables.end(); i++) {
		int fd;

		fd = (*i)->_fd;

		if (((*i)->condition & Select::Exception) && 
		    FD_ISSET (fd, e)) {
			(*i)->selected (Select::Exception);
		}
		if (((*i)->condition & Select::Readable) &&
		    FD_ISSET (fd, r)) {
			(*i)->selected (Select::Readable);
		}
		if (((*i)->condition & Select::Writable) &&
		    FD_ISSET (fd, w)) {
			(*i)->selected (Select::Writable);
		}
	}
	if (use_list_lock) {
		pthread_mutex_unlock (&list_lock);
	}
	return 0;
}

Selectable::Selectable (int fd)

{
	_ok = false;

	if ((_fd = fd) >= 0) {
		_ok = true;
	}

	condition = 0;
	_type = fromFD;
}    

Selectable::Selectable (const string &pathstr, int mode, int flags)

{
	_ok = false;
	path = pathstr;

	if ((_fd = ::open (path.c_str(), mode, flags)) >= 0) {
		_ok = true;
	}

	condition = 0;
	_type = fromPath;
}    

Selectable::Selectable (FILE *f)

{
	_ok = false;

	if (f && (_fd = fileno (f)) >= 0) {
		_ok = true;
	}

	condition = 0;
	_type = fromFILE;
}    

void
Selectable::selected (unsigned int cond)

{
	if ((cond & Select::Exception)) {
		exceptioned (this, Select::Exception);
	}
	if ((cond & Select::Readable)) {
		readable (this, Select::Readable);
	}
	if ((cond & Select::Writable)) {
		writable (this, Select::Writable);
	}
}

Selectable::~Selectable ()

{
	switch (_type) {
	case fromPath:
		if (_fd >= 0) {
			::close (_fd);
		}
		break;
	default:
		break;
	}
}

#ifdef TEST_SELECTABLE


void 
foo (Selectable *s, Select::Condition cond)

{
	int c;
	int nread;
	char buf[64];

	if (cond & Select::Readable) {
		printf ("File descriptor %d, arg = 0x%x in condition 0x%x\n",
			s->fd(), s, cond);
		nread = ::read (s->fd(), buf, 64);
		printf ("Read %d bytes from fd %d\n", nread, s->fd());
	}
}

void
cleanup_stdin ()

{
	long flags;

	fcntl (fileno (stdin), F_GETFL, &flags);
	flags |= O_NONBLOCK;
	fcntl (fileno (stdin), F_SETFL, &flags);

	signal (SIGIO, SIG_IGN);
}

main ()

{
	Selector ioselector;
	Selectable *s;

	cleanup_stdin ();
	
	s = new Selectable (stdin);
	ioselector.add (Select::Readable, s);
	s->readable.connect (slot (&foo));

	s = new Selectable ("/dev/snd/midiC0D1", O_RDONLY|O_NDELAY);
	ioselector.add (Select::Readable, s);
	s->readable.connect (slot (&foo));

	while (ioselector.select (10 * 1000000) == 0);
}

#endif // TEST_SELECTABLE
