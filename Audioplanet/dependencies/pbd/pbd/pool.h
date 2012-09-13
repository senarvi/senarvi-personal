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

    $Id: pool.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __qm_pool_h__
#define __qm_pool_h__

#include <vector>

class Pool 
{
  public:
	Pool (size_t sz) { item_bytes = sz; block_size = 64;}
	Pool () { item_bytes = 0; block_size = 64; }
	~Pool ();
	void set_item_size (size_t sz)  { item_bytes = sz; }
	void set_block_size (size_t sz) { block_size = sz; }
	void use_mlock (bool yn);
	void *alloc ();
	void release (void *);

  private:
	bool lock_down;
	size_t block_size;
	size_t item_bytes;
	std::vector<void *> free_list;
	std::vector<void *> block_list;

	void more ();

};

#endif // __qm_pool_h__
