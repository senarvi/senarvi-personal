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

    $Id: parser.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __qm_parser_h__
#define __qm_parser_h__

#include <string>
#include <iostream>
#include <cstdio>

#include <pbd/error.h>

class FlexLexer;

/* Note that this file cannot be included without either a preceding
   include of the header file output by Bison, or alternatively
   following the %union directive in the Bison file. This is necessary
   to supply the definition of YYSTYPE.
*/

#define PARSE_ERROR_LINE_SIZE 1024

class Parser {
 public:
	Parser ();
	virtual ~Parser ();

	virtual int parse (void *arg) = 0;  /* created by Bison */
	virtual void set_debug (bool debug_parse, bool debug_lex) = 0; 

	const std::string &src()    { return _src; }

	void switch_streams (std::istream *in, std::ostream *out);
	void set_src (const std::string &str) { 
		_src = str;
	}

	/* parse state */
	
	unsigned int line;
	unsigned int col;
	unsigned int errcount;

	void parse_error (const char *, ...);

	const char *parse_source () const {
		char *strp = new char [_src.length() + 32];
		sprintf (strp, "%s:%d", _src.c_str(), line);
		return strp;
	}

	void saveline (const char *txt) {
		int slen = PARSE_ERROR_LINE_SIZE - strlen(thisline);

		if (slen > 0) {
			strncat (thisline,txt,slen);
		} else {
			error << "Parser: lost track of context "
				 "(thisline is full)" 
			      << endmsg;
		}
	}

	void savechar (char c) {
		int slen = strlen(thisline);

		if (PARSE_ERROR_LINE_SIZE - slen > 0) {
			thisline[slen++] = c;
			thisline[slen] = '\0';
		} else {
			error << "Parser: lost track of context: "
				 "(thisline is full)" 
			      << endmsg;
		}
	}

	void newline () {
		strcpy (lastline, thisline);
		col = 0;
		thisline[0] = '\0';
		line++;
	}

	void count (const char *yyt) {
		col += strlen (yyt);
		saveline (yyt);
	}

	void count1 (char c) {
		col++;
		savechar (c);
	}

  protected:
	std::string    _src;
	char       lastline[PARSE_ERROR_LINE_SIZE+1];
	char       thisline[PARSE_ERROR_LINE_SIZE+1];
	FlexLexer *lexer;
};

#endif // __qm_parser_h__
