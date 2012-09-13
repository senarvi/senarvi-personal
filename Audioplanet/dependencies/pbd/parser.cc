
#include <cstdio>
#include <iostream>
#include <cstdarg>
#include <sys/types.h>

#include <FlexLexer.h>

#include <pbd/error.h>
#include <pbd/parser.h>

using namespace std;

Parser::Parser ()

{
	lastline[0] = '\0';
	thisline[0] = '\0';
	line = 0;
	col = 0;
	errcount = 0;
	_src = "unknown source";
	lexer = 0;
}

Parser::~Parser ()

{
}

void
Parser::parse_error (const char *message, ...)
{
  	size_t i = 0;
	char full_format[PARSE_ERROR_LINE_SIZE+1];
	char buf[PARSE_ERROR_LINE_SIZE+1];
	va_list ap;

	va_start(ap, message);
	sprintf (full_format, "%s (line %d): %s\n", _src.c_str(), line, message);
	vsprintf (buf, full_format, ap);
	error << buf;
	va_end(ap);
	errcount++;

	if (thisline[0] != '\0')
	{
		error << "context is: " << thisline << '\n';
		error << "            ";
		for (i=0; i < col; i++) {
			error << ' ';
		}
  		error << "^\n";

	} else {
		/*
		 * if a newline has already gone by, thisline is empty.
		 * we can try recovering context by looking at the last
		 * line
		 */

		size_t len = strlen (lastline);

		error << "context is (maybe): " << lastline << '\n';
		error << "                    ";
		for (i=0; i < len; i++) {
			error << ' ';
		}
  		error << "^\n";
	}
	
	error << endmsg;
}

void
Parser::switch_streams (istream *in, ostream *out)

{
	if (lexer) {
		lexer->switch_streams (in, out);
	}
}
