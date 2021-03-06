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

    $Id: stl_delete.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __libmisc_stl_delete_h__
#define __libmisc_stl_delete_h__

/* To actually use any of these deletion functions, you need to
   first include the revelant container type header.
*/
#if defined(_CPP_VECTOR) || defined(__SGI_STL_VECTOR)
template<class T> void vector_delete (std::vector<T *> *vec) 
{
	typename std::vector<T *>::iterator i;
	
	for (i = vec->begin(); i != vec->end(); i++) {
		delete *i;
	}
	vec->clear ();
}
#endif // _CPP_VECTOR || __SGI_STL_VECTOR

#if defined(_CPP_MAP) || defined(__SGI_STL_MAP)
template<class K, class T> void map_delete (std::map<K, T *> *m) 
{
	typename std::map<K, T *>::iterator i;

	for (i = m->begin(); i != m->end(); i++) {
		delete (*i).second;
	}
	m->clear ();
}
#endif // _CPP_MAP || __SGI_STL_MAP

#if defined(_CPP_LIST) || defined(__SGI_STL_LIST)
template<class T> void list_delete (std::list<T *> *l) 
{
	typename std::list<T *>::iterator i;

	for (i = l->begin(); i != l->end(); i++) {
		delete (*i);
	}

	l->clear ();
}
#endif // _CPP_LIST || __SGI_STL_LIST

#if defined(_CPP_SLIST) || defined(__SGI_STL_SLIST)
template<class T> void slist_delete (slist<T *> *l) 
{
	typename slist<T *>::iterator i;

	for (i = l->begin(); i != l->end(); i++) {
		delete (*i);
	}

	l->clear ();
}
#endif // _CPP_SLIST || __SGI_STL_SLIST

#if defined(_CPP_SET) || defined(__SGI_STL_SET)
template<class T> void set_delete (set<T *> *sset) 
{
	typename set<T *>::iterator i;
	
	for (i = sset->begin(); i != sset->end(); i++) {
		delete *i;
	}
	sset->erase (sset->begin(), sset->end());
}
#endif // _CPP_SET || __SGI_STL_SET

#endif // __libmisc_stl_delete_h__
