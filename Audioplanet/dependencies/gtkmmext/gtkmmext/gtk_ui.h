/*
    Copyright (C) 1999 Paul Barton-Davis 

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

    $Id: gtk_ui.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __pbd_gtk_ui_h__
#define __pbd_gtk_ui_h__

#include <string>
#include <queue>

#include <pthread.h>
#include <gtk--.h>
#include <pbd/abstract_ui.h>
#include <pbd/atomic.h>
#include <pbd/pool.h>
#include <pbd/error.h>

using std::string;
using std::queue;

class Touchable;

namespace Gtkmmext {

class TextViewer;

class UI : public AbstractUI

{
  public:
	/* this constructor is used to build a standalone GUI */

	UI (const string &name, 
	    int *argc, char **argv[], 
	    const string &rcfile);

	virtual ~UI ();
	static UI *instance() { return theGtkUI; }

	/* Abstract UI interfaces */

	bool running ();
	void quit    ();
	void kill    ();
	void request (RequestType); 
	void run (Receiver &old_receiver);
	void call_slot (SigC::Slot0<void>);
	void touch_display (Touchable *);
	void receive (Transmitter::Channel, const char *);
	bool caller_is_gui_thread () { 
		return pthread_equal (gui_thread, pthread_self());
	}

	/* Gtk-UI specific interfaces */

	void set_tip (Gtk::Widget *, const gchar *txt, const gchar *hlp = 0);
	void set_state (Gtk::Widget *w, GtkStateType state);
	void idle_add (int (*)(void *), void *);
	void timeout_add (unsigned int, int (*)(void *), void *);
	void popup_error (const char *text);
	void flush_pending ();
	void toggle_errors ();
	int load_rcfile (const string&);

	template<class T> static gint idle_delete (T *obj) { delete obj; return false; }
	template<class T> static void delete_when_idle (T *obj) {
		Gtk::Main::idle.connect (bind (slot (&UI::idle_delete<T>), obj));
	}

	GdkColor get_color (const string& prompt, bool& picked, gdouble *initial = 0);

	/* starting is sent just before we enter the main loop,
	   stopping just after we return from it (at the top level)
	*/

	SigC::Signal0<void> starting;
	SigC::Signal0<void> stopping;

	static gint just_hide_it (GdkEventAny *, Gtk::Window *);

  protected:
	virtual void handle_fatal (const char *);
	virtual void display_message (const char *prefix, gint prefix_len, 
				      GtkStyle *style, const char *msg);

	/* stuff to invoke member functions in another
	   thread so that we can keep the GUI running.
	*/

	template<class UI_CLASS> struct thread_arg {
	    UI_CLASS *ui;
	    void (UI_CLASS::*func)(void *);
	    void *arg;
	};

	template<class UI_CLASS> static void *start_other_thread (void *arg);
	template<class UI_CLASS> void other_thread (void (UI_CLASS::*func)(void *), void *arg = 0);

  private:
	struct Request {
	    RequestType type;
	    union { 
		Touchable *display;
		const char *msg;
		GtkStateType new_state;
		int (*function)(void *);
	    };
	    union {
		Gtk::Widget *widget;
		Transmitter::Channel chn;
		void *arg;
	    };
	    union {
		const char *msg2;
		unsigned int timeout;
	    };
	    
	    /* sigh, can't put this in a union */

	    SigC::Slot0<void> slot;

	    Request (RequestType t) : type(t), msg(0) {}
	    Request (RequestType t, SigC::Slot0<void> sl) : type(t), slot (sl) {}

	    ~Request () { 
		    if (type == ErrorMessage && msg) {
			    delete [] msg;
		    }
	    }

	    static Pool *requestPool;

	    void *operator new(size_t ignored) {
		    return requestPool->alloc ();
	    };

	    void operator delete(void *ptr, unsigned int size) {
		    requestPool->release (ptr);
	    }

	};

	static UI *theGtkUI;
	static pthread_t gui_thread;
	bool _active;
	string _ui_name;
	Gtk::Main *theMain;
	Gtk::Tooltips *tips;
	TextViewer *errors;

	/* error message display styles */

	GtkStyle *error_message_style;
	GtkStyle *info_message_style;
	GtkStyle *warning_message_style;
	GtkStyle *fatal_message_style;

	int signal_pipe[2];
	queue<Request *> requests;
	pthread_mutex_t request_lock;
	atomic_t processing_requests;
	int setup_signal_pipe ();

	void handle_ui_requests (Request *r = 0);
	void send_request (Request *);
	static void signal_pipe_callback (void *, gint, GdkInputCondition);
	void process_error_message (Transmitter::Channel, const char *);
	void do_quit ();

	void color_selection_done (bool status);
	gint color_selection_deleted (GdkEventAny *);
	bool color_picked;
};

template<class UI_CLASS> void *
UI::start_other_thread (void *arg)

{
	thread_arg<UI_CLASS> *ta = (thread_arg<UI_CLASS> *) arg;
	(ta->ui->*ta->func)(ta->arg);
	delete ta;
	return 0;
}

template<class UI_CLASS> void
UI::other_thread (void (UI_CLASS::*func)(void *), void *arg)

{
	pthread_t thread_id;
	thread_arg<UI_CLASS> *ta = new thread_arg<UI_CLASS>;

	ta->ui = dynamic_cast<UI_CLASS *> (this);
	if (ta->ui == 0) {
		error << "UI::other thread called illegally"
		      << endmsg;
		return;
	}
	ta->func = func;
	ta->arg = arg;
	pthread_create (&thread_id, 0, start_other_thread, ta);
}


} /* namespace */

#endif /* __pbd_gtk_ui_h__ */
