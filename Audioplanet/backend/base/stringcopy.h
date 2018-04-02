// Seppo Enarvi, 2003


#ifndef BACKEND_BASE_STRINGCOPY_H
#define BACKEND_BASE_STRINGCOPY_H


#include <cstddef>


namespace ap {


    void copyStringHead(char * destString,
			const char * sourceString,
			std::size_t maxLength);

    void copyStringTail(char * destString,
			const char * sourceString,
			std::size_t maxLength);


}


#endif // BACKEND_BASE_STRINGCOPY_H
