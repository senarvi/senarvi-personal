//  backend/base/process.h
//  Copyright (C) 2000-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_BASE_PROCESS_H
#define BACKEND_BASE_PROCESS_H


#include <string>


class Pipe;
class File;


namespace ap
{

    namespace process
    {
	
	typedef ::pid_t Id;
	
	
	// Create a child process. The PID of the child process is returned in the parent's
	// thread of execution, and a 0 is returned in the child's thread of execution.
	extern Id create();
	
	// Like createProcess but also connect the standard input and output of the child
	// process to a pipe or a file.
	extern Id createWithPipes(File & parentWriteFile, File & parentReadFile);
	extern Id createWithOutputPipe(File & childStdin, File & parentReadFile);
	extern Id createWithInputPipe(File & parentWriteFile, File & childStdout);
	
	// If the current process is the main process, simply call exit. Otherwise
	// terminate the current process without calling any functions registered to be
	// called at program termination. Status is returned to the parent process as the
	// process's exit status.
	extern void terminate(int status);
	
	// Return the process ID of the current process.
	inline Id id();
	
	// If a child process has exited, free any system resources used by the child and
	// return the child's pid. Otherwise return 0.
	extern Id detach();
	
	// Execute the command and return the return code
	extern int executeCommand(const std::string & command);
	
	
	extern const char* executable;
	
    }  // namespace process

}  // namespace ap


#endif  // BACKEND_BASE_PROCESS_H

// Local Variables:
// mode:C++
// End:
