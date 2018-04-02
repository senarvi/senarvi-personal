/*
    Copyright (C) 1999-2002 Paul Barton-Davis 

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

    $Id: gtk_ui.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <math.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include <gtk--.h>
#include <pbd/error.h>
#include <pbd/touchable.h>
#include <pbd/pthread_spinlock.h>
#include <pbd/failed_constructor.h>
#include <pbd/thread.h>

#include <gtkmmext/gtk_ui.h>
#include <gtkmmext/textviewer.h>
#include <gtkmmext/popup.h>

using namespace Gtkmmext;

pthread_t UI::gui_thread;
UI       *UI::theGtkUI = 0;
Pool     *UI::Request::requestPool;

UI::UI (const string &name, int *argc, char ***argv, 
	const string &rcfile)
	  
{
	theMain = new Gtk::Main (argc, argv);
	tips = new Gtk::Tooltips;

	Gtk::Style *style;
	Gtk::Label *a_widget1;
	Gtk::Label *a_widget2;
	Gtk::Label *a_widget3;
	Gtk::Label *a_widget4;

	_ok = false;
	_active = false;

	if (!theGtkUI) {
		theGtkUI = this;
		gui_thread = pthread_self ();
	} else {
		fatal << "duplicate UI requested" << endmsg;
		/* NOTREACHED */
	}

	if (setup_signal_pipe ()) {
		return;
	}

	load_rcfile (rcfile);
	
	a_widget1 = new Gtk::Label;
	a_widget2 = new Gtk::Label;
	a_widget3 = new Gtk::Label;
	a_widget4 = new Gtk::Label;

	errors = new TextViewer (850, 350);
	errors->text().set_editable (false); 
	errors->text().set_name ("ErrorText");
	string title;

	title = _ui_name;
	title += ": Log";
	errors->set_title (title);

	errors->delete_event.connect (bind (slot (just_hide_it),
					    (Gtk::Window *) errors));
	
	a_widget1->set_name ("FatalMessage");
	style = a_widget1->get_style ();
	fatal_message_style = style->gtkobj();

	a_widget2->set_name ("ErrorMessage");
	style = a_widget2->get_style ();
	error_message_style = style->gtkobj();

	a_widget3->set_name ("WarningMessage");
	style = a_widget3->get_style ();
	warning_message_style = style->gtkobj();

	a_widget4->set_name ("InfoMessage");
	style = a_widget4->get_style ();
	info_message_style = style->gtkobj();

	delete a_widget1;
	delete a_widget2;
	delete a_widget3;
	delete a_widget4;

	Request::requestPool = new Pool (sizeof (Request));
	
	_ok = true;
}

UI::~UI ()

{
	close (signal_pipe[0]);
	close (signal_pipe[1]);
	pthread_mutex_destroy (&request_lock);
}

int
UI::load_rcfile (const string& rcfile)

{
	if (rcfile.length()) {
		if (access (rcfile.c_str(), R_OK)) {
			error << "GtkUI: couldn't find rc file \"" 
			      << rcfile 
			      << '"'
			      << endmsg;
			return -1;
		}
		
		gtk_rc_parse (rcfile.c_str());
	}

	return 0;
}

void
UI::run (Receiver &old_receiver)

{
	listen_to (error);
	listen_to (info);
	listen_to (warning);
	listen_to (fatal);

	old_receiver.hangup ();
	starting ();
	_active = true;	
	theMain->run ();
	_active = false;
	stopping ();
	hangup ();
	return;
}

bool
UI::running ()
{
	return _active;
}

void
UI::kill ()
{
	pthread_kill (gui_thread, SIGKILL);
}

void
UI::quit ()
{
	request (Quit);
}

void
UI::do_quit ()
{
	Gtk::Main::quit();
}

void
UI::touch_display (Touchable *display)
{
	Request *req = new Request (TouchDisplay);
	
	req->display = display;
	send_request (req);
}	

void
UI::call_slot (SigC::Slot0<void> slot)
{
	Request *req = new Request (CallSlot, slot);
	send_request (req);
}	

void
UI::set_tip (Gtk::Widget *w, const gchar *tip, const gchar *hlp)
{
	Request *req = new Request (SetTip);

	req->widget = w;
	req->msg = tip;
	req->msg2 = hlp;

	send_request (req);
}

void
UI::set_state (Gtk::Widget *w, GtkStateType state)
{
	Request *req = new Request (StateChange);
	
	req->new_state = state;
	req->widget = w;

	send_request (req);
}

void
UI::idle_add (int (*func)(void *), void *arg)
{
	Request *req = new Request (AddIdle);

	req->function = func;
	req->arg = arg;

	send_request (req);
}

void
UI::timeout_add (unsigned int timeout, int (*func)(void *), void *arg)
{
	Request *req = new Request (AddTimeout);

	req->function = func;
	req->arg = arg;
	req->timeout = timeout;

	send_request (req);
}

/* END abstract_ui interfaces */

/* Handling requests */

int
UI::setup_signal_pipe ()

{
	/* setup the pipe that other threads send us notifications/requests
	   through.
	*/

	pthread_mutex_init (&request_lock, 0);
	atomic_set (&processing_requests, 0);

	if (pipe (signal_pipe)) {
		error << "UI: cannot create error signal pipe ("
		      << strerror (errno) << ")" 
		      << endmsg;

		return -1;
	}

	if (fcntl (signal_pipe[0], F_SETFL, O_NONBLOCK)) {
		error << "UI: cannot set O_NONBLOCK on "
			 "signal read pipe ("
		      << strerror (errno) << ")"
		      << endmsg;
		return -1;
	}

	if (fcntl (signal_pipe[1], F_SETFL, O_NONBLOCK)) {
		error << "UI: cannot set O_NONBLOCK on "
			 "signal write pipe ("
		      << strerror (errno) 
		      << ")" 
		      << endmsg;
		return -1;
	}

	/* add the pipe to the select/poll loop that GDK does */

	gdk_input_add (signal_pipe[0],
		       GDK_INPUT_READ,
		       UI::signal_pipe_callback,
		       this);

	return 0;
}

void
UI::handle_ui_requests (Request *newreq)

{
	Request *req;

	atomic_set (&processing_requests, 1);

	pthread_mutex_spinlock (&request_lock);

	if (newreq) { 
		requests.push (newreq);
	}

	while (requests.size()) {
		req = requests.front ();
		requests.pop ();
		pthread_mutex_unlock (&request_lock);

		switch (req->type) {
		case ErrorMessage:
			process_error_message (req->chn, req->msg);
			break;
			
		case Quit:
			do_quit ();
			break;

		case CallSlot:
			req->slot.call ();
			break;
			
		case TouchDisplay:
			req->display->touch ();
			if (req->display->delete_after_touch()) {
				delete req->display;
			}
			break;

		case StateChange:
			req->widget->set_state (req->new_state);
			break;

		case SetTip:
			/* XXX need to figure out how this works */
			break;
			
		case AddIdle:
			gtk_idle_add (req->function, req->arg);
			break;

		case AddTimeout:
			gtk_timeout_add (req->timeout, req->function, req->arg);
			break;

		default:
			error << "UI: unknown request type "
			      << (int) req->type
			      << endmsg;
		}	       

		delete req;

		pthread_mutex_spinlock (&request_lock);
	}

	pthread_mutex_unlock (&request_lock);

	atomic_set (&processing_requests, 0);
}

void
UI::signal_pipe_callback (void *arg, int fd, GdkInputCondition cond)

{
	char buf[64];
	
	/* flush (nonblocking) pipe */

	while (read (fd, buf, 64) > 0);

	((UI *) arg)->handle_ui_requests ();
}

void
UI::send_request (Request *req)

{
	if (instance() == 0) {
		return; /* XXX is this the right thing to do ? */
	}

	if (caller_is_gui_thread()) {
		handle_ui_requests (req);
	} else {
		char c = 0;
		
		pthread_mutex_spinlock (&request_lock);
		requests.push (req);
		pthread_mutex_unlock (&request_lock);

		if (atomic_read (&processing_requests) == 0) {
			write (signal_pipe[1], &c, 1);
		}
	}
}

void
UI::request (RequestType rt)

{
	send_request (new Request (rt));
}

/*======================================================================
  Error Display
  ======================================================================*/

void
UI::receive (Transmitter::Channel chn, const char *str)

{
	if (caller_is_gui_thread()) {
		process_error_message (chn, str);
	} else {
		Request *req = new Request (ErrorMessage);
		req->chn = chn;
		req->msg = strdup (str);
		send_request (req);
	}
}

void
UI::process_error_message (Transmitter::Channel chn, const char *str)

{
	GtkStyle *style;
	char *prefix;
	size_t prefix_len;
	bool fatal_received = false;

	if (!errors->is_realized()) {
		errors->show ();
	}

	errors->text().freeze();

	switch (chn) {
	case Transmitter::Fatal:
		prefix = "[FATAL]: ";
		style = fatal_message_style;
		prefix_len = 9;
		fatal_received = true;
		break;
	case Transmitter::Error:
		prefix = "[ERROR]: ";
		style = error_message_style;
		prefix_len = 9;
		break;
	case Transmitter::Info:
		prefix = "[INFO]: ";
		style = info_message_style;
		prefix_len = 8;
		break;
	case Transmitter::Warning:
		prefix = "[WARNING]: ";
		style = warning_message_style;
		prefix_len = 11;
		break;
	default:
		/* no choice but to use text/console output here */
		cerr << "programmer error in UI::check_error_messages\n";
		::exit (1);
	}
	
	if (fatal_received) {
		handle_fatal (str);
	} else {
		display_message (prefix, prefix_len, style, str);
		errors->text().thaw();
		
		if (!errors->is_visible()) {
			toggle_errors();
		}
	}
}

void
UI::toggle_errors ()

{
	if (!errors->is_visible()) {
		errors->show ();
	} else {
		errors->hide ();
	}
}

void
UI::display_message (const char *prefix, gint prefix_len, GtkStyle *style,
		       const char *msg)

{
	/* stupid, stupid, stupid Gtk-- */

	Gdk_Font pfont(style->font);
	Gdk_Font mfont (style->font);
	Gdk_Color pfg (&style->fg[GTK_STATE_ACTIVE]);
	Gdk_Color pbg (&style->bg[GTK_STATE_ACTIVE]);
	Gdk_Color mfg (&style->fg[GTK_STATE_NORMAL]);
	Gdk_Color mbg (&style->bg[GTK_STATE_NORMAL]);

	errors->text().insert (pfont, pfg, pbg, prefix, prefix_len);
	errors->text().insert (mfont, mfg, mbg, msg, strlen (msg));
	errors->text().insert (mfont, mfg, mbg, "\n", 1);
	errors->scroll_to_bottom ();
}	

void
UI::handle_fatal (const char *message)

{
	Gtk::Window win (GTK_WINDOW_DIALOG);
	Gtk::VBox packer;
	Gtk::Label label (message);
	Gtk::Button quit ("Press To Exit");

	win.set_default_size (400, 100);
	
	string title;
	title = _ui_name;
	title += ": Fatal Error";
	win.set_title (title);

	win.set_position (GTK_WIN_POS_MOUSE);
	win.add (packer);

	packer.pack_start (label, true, true);
	packer.pack_start (quit, false, false);
	quit.clicked.connect(SigC::slot(this,&UI::quit));
	
	win.show_all ();
	win.set_modal (true);

	theMain->run ();
	
	exit (1);
}

void
UI::popup_error (const char *text)

{
	PopUp *pup;

	if (!caller_is_gui_thread()) {
		error << "non-UI threads can't use UI::popup_error" 
		      << endmsg;
		return;
	}
	
	pup = new PopUp (GTK_WIN_POS_MOUSE, 0, true);
	pup->set_text (text);
	pup->touch ();
}


void
UI::flush_pending ()
	
{
	if (!caller_is_gui_thread()) {
		error << "non-UI threads cannot call UI::flush_pending()"
		      << endmsg;
		return;
	}

	gtk_main_iteration();

	while (gtk_events_pending()) {
		gtk_main_iteration();
	}
}

gint
UI::just_hide_it (GdkEventAny *ev, Gtk::Window *win)

{
	win->hide_all ();
	return TRUE;
}

GdkColor
UI::get_color (const string& prompt, bool& picked, gdouble *initial)
{
	GdkColor color;

	Gtk::ColorSelectionDialog color_dialog (prompt);


	color_dialog.set_modal (true);
	color_dialog.get_cancel_button()->clicked.connect (bind (slot (this, &UI::color_selection_done), false));
	color_dialog.get_ok_button()->clicked.connect (bind (slot (this, &UI::color_selection_done), true));
	color_dialog.delete_event.connect (slot (*this, &UI::color_selection_deleted));

	if (initial) {
		color_dialog.get_colorsel()->set_color (initial);
	}

	color_dialog.show_all ();
	color_picked = false;
	picked = false;

	Gtk::Main::run();

	color_dialog.hide_all ();

	if (color_picked) {
		gdouble f_rgba[4];

		color_dialog.get_colorsel()->get_color (f_rgba);
		
		color.red = (gushort) rint (f_rgba[0] * 65535);
		color.green = (gushort) rint (f_rgba[1] * 65535);
		color.blue = (gushort) rint (f_rgba[2] * 65535);

		picked = true;
	}

	return color;
}

void
UI::color_selection_done (bool status)
{
	color_picked = status;
	Gtk::Main::quit ();
}

gint
UI::color_selection_deleted (GdkEventAny *ev)
{
	Gtk::Main::quit ();
	return TRUE;
}
