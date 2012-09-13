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

    $Id: pixmap_button.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __pbd_gtkmm_pixmap_button_h__
#define __pbd_gtkmm_pixmap_button_h__

#include <gtk--.h>
#include <gtkmmext/shaped_window.h>

namespace Gtkmmext {

class Pix;

class PixmapButton : public ShapedWindow

{
  public:
	PixmapButton (Pix *, const char *widget_name = 0, 
		      bool sensitive_to_state = true,
		      int button_mask = ~0);
	SigC::Signal0<void> pressed;
	SigC::Signal0<void> released;
	SigC::Signal0<void> clicked;
	SigC::Signal0<void> double_clicked;
	SigC::Signal0<void> triple_clicked;

	void set_button_mask (int mask) {
		button_mask = mask;
	}

	void click_on_press () {
		mode = ClickOnPress;
	}
	void click_on_release () { 
		mode = ClickOnRelease;
	}

	bool get_active () {
		return active;
	}

	void set_active (bool yn) {
		active = yn;
		map_active ();
	}

	void toggle_active () {
		set_active (!active);
	}

 protected:
	virtual gint button_press_event_impl(GdkEventButton*);
	virtual gint button_release_event_impl(GdkEventButton*);
	virtual gint enter_notify_event_impl(GdkEventCrossing*);
	virtual gint leave_notify_event_impl(GdkEventCrossing*);

  private:
	enum ClickMode {
		ClickOnPress,
		ClickOnRelease,
	};
	ClickMode mode;
	GtkStateType pre_enter_state;
	int button_mask;
	bool active;
	bool prelight_sensitive;

	void map_active () {
		if (active) {
			if ((GtkStateType) 
			    GTK_WIDGET_STATE(GTK_WIDGET(gtkobj())) !=
			    GTK_STATE_SELECTED) {
				set_state (GTK_STATE_SELECTED);
			} 
		} else {
			if ((GtkStateType) 
			    GTK_WIDGET_STATE(GTK_WIDGET(gtkobj())) !=
			    GTK_STATE_NORMAL) {
				set_state (GTK_STATE_NORMAL);
			} 
		}
	}

};

} /* namespace */

#endif  // __pbd_gtkmm_pixmap_button_h__


