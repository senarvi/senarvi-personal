//  threads.h
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef THREADS_H
#define THREADS_H

#ifdef __GNUG__
#pragma interface
#endif


#include <set>
#include <string>

#include "system/debug.h"
#include "system/username.h"
#include "system/thread.h"
#include "system/socket.h"
#include "progresswindow.h"


namespace threads {


	class ThreadEntity : public Thread {

	public:

		ThreadEntity() EXCEPTIONS;
		virtual ~ThreadEntity() EXCEPTIONS;

		void add(Thread & thread) EXCEPTIONS;
		void remove(Thread & thread) EXCEPTIONS;
		bool contains(Thread::Id threadId) EXCEPTIONS;

		void run() EXCEPTIONS;
		void cleanup() EXCEPTIONS;


	private:

		typedef std::set<Thread *> Threads;

		Threads threads_;
		Mutex mutex_;

	};


	class DiscreteProcess : public ThreadEntity {

		class ProgressWindow : public GUI::ProgressWindow {

		public:

			inline ProgressWindow(const std::string & title, ThreadEntity * threadEntity) EXCEPTIONS;

		private:

			ThreadEntity * threadEntity_;

			void atCancel() EXCEPTIONS;

		};


		ProgressWindow progressWindow_;

	public:

		DiscreteProcess(const std::string & windowTitle) EXCEPTIONS;
		~DiscreteProcess() EXCEPTIONS;

	};


	inline DiscreteProcess::ProgressWindow::ProgressWindow(const std::string & title,
	                                                       ThreadEntity * threadEntity) EXCEPTIONS :
			GUI::ProgressWindow(title),
			threadEntity_(threadEntity) {}


}  // namespace threads


#endif  // THREADS_H

// Local Variables:
// mode:C++
// End:
