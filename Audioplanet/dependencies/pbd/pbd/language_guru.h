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

    $Id: language_guru.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __pbd_language_guru_h__
#define __pbd_language_guru_h__

#include <map>
#include <string>

using std::map;
using std::string;

class Parser;

class LanguageGuru {
  public:
	LanguageGuru (const string &search_path);
	Parser *get_parser (const string &language);

  private:
	typedef Parser * (*FactoryFunc)();
	typedef map<const char *, FactoryFunc> LanguageMap;
	LanguageMap *languages;
	string search_path;
};

#endif /* __pbd_language_guru_h__ */
