#include <string.h>

#include <pbd/basename.h>

char *
basename (const char *path)

{
	char *slash;

	if ((slash = strrchr (path, '/')) == 0) {
		return strdup (path);
	}
	
	if (*(slash+1) == '\0') {
		return strdup ("");
	}
	
	return strdup (slash+1);
}
