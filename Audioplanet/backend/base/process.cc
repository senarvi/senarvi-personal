//  backend/base/process.cc
//  Copyright (C) 2000-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include <cerrno>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <boost/assert.hpp>

#include "backend/base/process.h"
#include "backend/base/exceptions.h"
#include "backend/base/debug.h"
#include "backend/base/thread.h"
#include "backend/base/file.h"
#include "backend/base/log.h"


namespace ap
{

    namespace process
    {

	const char * executable = "audioplanet";
	Id mainProcessId = -1;


	Id create()
	{
	    Id pid = ::fork();
	
	    if (pid == -1)
		throw SystemException(THIS_LOCATION);
	
	    if (pid == 0)
		root.debug("NEW PROCESS");
	
	    if ((pid != 0) && (mainProcessId == -1))
		mainProcessId = id();
	
	    return pid;
	}


	static void createPipe(File & writeFile, File & readFile)
	{
	    int descriptors[2];
	
	    switch (::pipe(descriptors))
	    {
	    case 0:
		readFile.setDescriptor(descriptors[0]);
		writeFile.setDescriptor(descriptors[1]);
		return;
		
	    case -1:
		throw SystemException(THIS_LOCATION);
		
	    default:
		BOOST_ASSERT(false);
	    }
	}


// parentWriteFile is an empty File object that will, in the parent process,
// point to the input pipe of the child process. The parent process is
// responsible for closing parentWriteFile.
//
// childStdout is a File object that will be attached to the standard output
// of the child process. The caller can close childStdout as soon as the
// function returns.

	Id createWithPipes(File & parentWriteFile, File & parentReadFile)
	{
	    // childStdin and childStdout will be closed in both the processes after attached
	    // to the standard input / output of the child process.
	    File childStdin, childStdout;
	
	    // parentReadFile and parentWriteFile will be closed only in the child process.
	    createPipe(parentWriteFile, childStdin);
	    createPipe(childStdout, parentReadFile);
	
	    Id pid = create();
	
	    if (pid == 0)
	    {
		// Child process
		
		childStdin.attachToStdin();
		childStdout.attachToStdout();
		
		parentReadFile.close();
		parentWriteFile.close();
	    }
	
	    childStdin.close();
	    childStdout.close();
	
	    return pid;
	}


// childStdin is a File object that will be attached to the standard input
// of the child process. The caller can close childStdin as soon as the
// function returns.
//
// parentReadFile is an empty File object that will, in the parent process,
// point to the output pipe of the child process. The parent process is
// responsible for closing parentReadFile.

	Id createWithOutputPipe(File & childStdin, File & parentReadFile)
	{
	    File childStdout;  // Will be closed in both the processes after attached to the standard output of the child process.
	    createPipe(childStdout, parentReadFile);  // parentReadFile will be closed only in the child process.
	
	    Id pid = create();
	
	    if (pid == 0)
	    {
		// Child process
		
		childStdin.attachToStdin();
		childStdout.attachToStdout();
		
		parentReadFile.close();
	    }
	
	    childStdout.close();
	
	    return pid;
	}


// parentWriteFile is an empty File object that will, in the parent process,
// point to the input pipe of the child process. The parent process is
// responsible for closing parentWriteFile.
//
// childStdout is a File object that will be attached to the standard output
// of the child process. The caller can close childStdout as soon as the
// function returns.

	Id createWithInputPipe(File & parentWriteFile, File & childStdout)
	{
	    File childStdin;  // Will be closed in both the processes after attached to the standard input of the child process.
	    createPipe(parentWriteFile, childStdin);  // parentWriteFile will be closed only in the child process.
	
	    Id pid = create();
	
	    if (pid == 0)
	    {
		// Child process
		
		childStdin.attachToStdin();
		childStdout.attachToStdout();
		
		parentWriteFile.close();
	    }
	
	    childStdin.close();
	
	    return pid;
	}


	void terminate(int status)
	{
	    trace();
	
	
//		if ((id() == mainProcessId) || (mainProcessId == -1))
/*		if (id() == mainProcessId)
		std::exit(status);
		else if (mainProcessId == -1)
		std::exit(status);
		else
*/			::_exit(status);
	}


	Id id()
	{
	    return ::getpid();
	}


	Id detach()
	{
	    trace();
	
	
	    Id pid = ::waitpid(WAIT_ANY, NULL, WNOHANG);
	    if (pid < 0)
	    {
		if (errno != ECHILD)  // I think this should be obvious as we use WAIT_ANY, but it is not.
		    throw SystemException(THIS_LOCATION);
		else
		    return 0;
	    }
	    return pid;
	}


	int execute(const std::string & command)
	{
	    int pid = create();
	
	    if (pid == 0)
	    {
		// In the child process
		
		char * argv[4];
		argv[0] = "sh";
		argv[1] = "-c";
		
		argv[2] = new char[command.length() + 1];
		std::strcpy(argv[2], command.c_str());
		
		argv[3] = NULL;
		
		execv("/bin/sh", argv);
		terminate(127);  // Error
	    }
	
	    // In the parent process
	
	    do
	    {
		int status;
		if (waitpid(pid, &status, 0) == -1)
		{
		    // The child process has not exited yet. If an
		    // unblocked signal or a SIGCHLD was caught,
		    // continue waiting for the child process to exit,
		    // otherwise throw an exception
		    if (errno != EINTR)
			throw SystemException(THIS_LOCATION);
		}
		else
		    // The child process exited, return the return code
		    return WEXITSTATUS(status);
		
	    } while (true);
	}

    }  // namespace Process

}  // namespace ap
