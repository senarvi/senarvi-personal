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

    $Id: pool.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <sys/mman.h>

#include <vector>

#include <pbd/pool.h>
#include <pbd/stl_delete.h>

using namespace std;

Pool::~Pool ()
{
	vector_delete(&block_list);
}

void
Pool::more ()

{
	size_t i;
	char *block;
	void *ptr;
	size_t this_block_size;

	/* since ::operator new() uses this, its important that we
	   use a "lower level" allocator to get more space.
	*/

	this_block_size = block_size;

	ptr = malloc (this_block_size * item_bytes);
	block_list.push_back (ptr);
	block = static_cast<char *> (ptr);

	if (lock_down) {
		mlock (ptr, this_block_size * item_bytes);
	}

	for (i = 0; i < this_block_size; i++) {
		free_list.push_back 
			(static_cast<void *> (block + (i * item_bytes)));
	}
}

void
Pool::use_mlock (bool yn)

{
	lock_down = yn;

	if (!yn) {
		return;
	}

	/* wire all non-wired memory */

	for (vector<void *>::iterator v = block_list.begin();
	     v != block_list.end();
	     v++) {
		mlock (*v, item_bytes);
	}

	/* if we have none, get some more */

	if (free_list.empty()) {
		more ();
	}
}

void *
Pool::alloc ()
{
	void *ptr;

	if (free_list.size() == 0) {
		more ();
	}
	
	ptr = free_list.back();
	free_list.pop_back();
	return ptr;
};

void		
Pool::release (void *ptr)

{
	free_list.push_back (ptr);
}
