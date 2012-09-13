//  memorytracker.cc
//  Copyright (C) 1999-2000 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef DEBUG

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <malloc.h>
#include <execinfo.h>
#include <sys/stat.h>
#include <fcntl.h>


#define BACKTRACE_STEPS 10


static void* (*oldMallocHook)(std::size_t, const void*);
static void (*oldFreeHook)(void*, const void*);

static void dbgInitHook();
static void* dbgMallocHook(std::size_t, const void*);
static void dbgFreeHook(void*, const void*);

// Override initializing hook from the C library
//void (*__malloc_initialize_hook)() = dbgInitHook;


static int fd;  // Output file descriptor
FILE* file;


static void dbgInitHook() {

	fd = creat("mtracedata", S_IRUSR | S_IWUSR);
	file = fdopen(fd, "w");
	fprintf(file, "= Start\n");
	fflush(file);
	
	oldMallocHook = __malloc_hook;
	oldFreeHook = __free_hook;
	__malloc_hook = dbgMallocHook;
	__free_hook = dbgFreeHook;	
}


static void* dbgMallocHook(std::size_t size, const void*) {

	// Restore all old hooks
	__malloc_hook = oldMallocHook;
	__free_hook = oldFreeHook;
	
	// Call recursively
	void* ptr = malloc(size);

	// Save underlaying hooks
	oldMallocHook = __malloc_hook;
	oldFreeHook = __free_hook;
	
	
	void* callers[BACKTRACE_STEPS];
	int actual_steps = backtrace(callers, BACKTRACE_STEPS);
	char** symbols = backtrace_symbols(callers, actual_steps);
	
	// Find out where new was called from. If it was called by
	// new[], find out where new[] was called
	int i;
	for (i = 0; i < actual_steps - 1; i++) {
		if (std::strstr(symbols[i], "__builtin_vec_new") != NULL)
			break;
			
		if ((std::strstr(symbols[i], "__builtin_new") != NULL) &&
		    (std::strstr(symbols[i + 1], "__builtin_vec_new") == NULL))
			break;
	}

	if (i < actual_steps - 1) {
		// Write out to the mtrace data file
		fprintf(file, "@ [%p] + %p 0x%x\n", callers[i + 1], ptr, size);
		fflush(file);
	}
	
	
	// Restore our own hooks
	__malloc_hook = dbgMallocHook;
	__free_hook = dbgFreeHook;
	return ptr;
}


static void dbgFreeHook(void* ptr, const void*) {

	// Restore all old hooks
	__malloc_hook = oldMallocHook;
	__free_hook = oldFreeHook;
	
	// Call recursively
	free(ptr);
	
	// Save underlaying hooks
	oldMallocHook = __malloc_hook;
	oldFreeHook = __free_hook;
	
	
	void* callers[BACKTRACE_STEPS];
	int actual_steps = backtrace(callers, BACKTRACE_STEPS);
	char** symbols = backtrace_symbols(callers, actual_steps);
	
	// Find out where delete was called from. If it was called by
	// delete[], find out where delete[] was called
	int i;
	for (i = 0; i < actual_steps - 1; i++) {
		if (std::strstr(symbols[i], "__builtin_vec_delete") != NULL)
			break;
			
		if ((std::strstr(symbols[i], "__builtin_delete") != NULL) &&
		    (std::strstr(symbols[i + 1], "__builtin_vec_delete") == NULL))
			break;
	}

	if (i < actual_steps - 1) {
		// Write out to the mtrace data file
		fprintf(file, "@ [%p] - %p\n", callers[i + 1], ptr);
		fflush(file);
	}
	
	
	// Restore our own hooks
	__malloc_hook = dbgMallocHook;
	__free_hook = dbgFreeHook;
}

#endif  // DEBUG
