/*
    Copyright (C) 2000 Paul Barton-Davis 

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

    $Id: language_guru.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <unistd.h>
// #include <ltdl.h>

#include <pbd/error.h>
#include <pbd/parser.h>

#include <pbd/language_guru.h>

LanguageGuru::LanguageGuru (const string &path)

{
	languages = new LanguageMap;
	search_path = path;
}

Parser *
LanguageGuru::get_parser (const string &language)

{
#if 0
	string path;
	lt_dlhandle handle;
	FactoryFunc factory;
	LanguageMap::iterator i;

	/* if the language is XXX, we are looking for "language_XXX.so" */

	/* XXX fix me ... */

	path = search_path;
	path += '/';
	path += "language_";
	path += language;
	path += ".so";

	if ((i = languages->find (path.c_str())) != languages->end()) {
		factory = (*i).second;

	} else {
		if ((handle = lt_dlopen (path.c_str())) == 0) {

			error << "language \""
			      << language
			      << "\" cannot be loaded from \""
			      << path
			      << "\": "
			      << lt_dlerror()
			      << endmsg;

			return 0;
		}
		
		factory = (FactoryFunc) lt_dlsym (handle, "factory");
		
		if (factory == 0) {
			return 0;
		}

		pair<const char *, FactoryFunc> newpair;

		newpair.first = strdup (path.c_str());
		newpair.second = factory;

		languages->insert (newpair);
	}

	return factory ();
#endif
	return NULL;
}
