// Seppo Enarvi, 2003


#include <cstring>


using namespace std;


//
// Copy at most maxLength characters from the beginning of sourceString and a \0 to
// destString. The \0 is not counted in maxLength, so destString has to be able to
// hold maxLength + 1 characters.
//
void copyStringHead(char * destString, const char * sourceString, size_t maxLength)
{
    size_t length = strlen(sourceString);

    if (length > maxLength) {
	std::strncpy(destString, sourceString, maxLength);
	destString[maxLength] = '\0';
    } else
	strcpy(destString, sourceString);
}


//
// Copy at most maxLength characters from the end of sourceString and a \0 to
// destString. The \0 is not counted in maxLength, so destString has to be able to
// hold maxLength + 1 characters.
//
void copyStringTail(char * destString, const char * sourceString, size_t maxLength)
{
    size_t length = strlen(sourceString);
    size_t firstCharToCopy;

    if (length > maxLength)
	firstCharToCopy = length - maxLength;
    else
	firstCharToCopy = 0;

    strcpy(destString, &sourceString[firstCharToCopy]);
}
