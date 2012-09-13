/*
    Copyright (C) 2000 Paul Barton-Davis 

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

    $Id: spinner.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <stdio.h> /* for sprintf, sigh */
#include <gtk--.h>
#include <gtkmmext/spinner.h>
#include <math.h>

using namespace Gtkmmext;

/*---------------------------------------------------------------
  These are a set of "private" objects that serve no other purpose
  than to group one or more Gtk::Arrows together in certain ways.
  ---------------------------------------------------------------*/

namespace Gtkmmext {
  
class DoubleArrow 
{
public:
    DoubleArrow (GtkArrowType at1, GtkArrowType at2, GtkShadowType t) :
	    arrow1 (at1, t), arrow2 (at2, t) {
	    arrow1.set_name ("DoubleArrow");
	    arrow2.set_name ("DoubleArrow");
    };
protected:
    Gtk::Arrow arrow1;
    Gtk::Arrow arrow2;
};

class DoubleArrowHorizontal : public Gtk::HBox, public DoubleArrow

{
public:
    DoubleArrowHorizontal (GtkArrowType at1,
			       GtkArrowType at2,
			       GtkShadowType t) :
	    DoubleArrow (at1, at2, t) {
	    pack_start (arrow1, true, true);
	    pack_start (arrow2, true, true);
    }
};

class DoubleArrowVertical : public Gtk::VBox, public DoubleArrow

{
public:
    DoubleArrowVertical (GtkArrowType at1,
			     GtkArrowType at2,
			     GtkShadowType t) :
	    DoubleArrow (at1, at2, t) {
	    pack_start (arrow1, true, true);
	    pack_start (arrow2, true, true);
    }
};

class DoubleLeftArrow : public DoubleArrowHorizontal

{
public:
    DoubleLeftArrow (GtkShadowType t) :
	    DoubleArrowHorizontal (GTK_ARROW_LEFT, GTK_ARROW_LEFT, t) {}
};

class DoubleRightArrow : public DoubleArrowHorizontal

{
public:
    DoubleRightArrow (GtkShadowType t) :
	    DoubleArrowHorizontal (GTK_ARROW_RIGHT, GTK_ARROW_RIGHT, t) {}
};

class LeftRightArrow : public DoubleArrowHorizontal

{
public:
    LeftRightArrow (GtkShadowType t) :
	    DoubleArrowHorizontal (GTK_ARROW_LEFT, GTK_ARROW_RIGHT, t) {}
};

class DoubleUpArrow : public DoubleArrowVertical

{
public:
    DoubleUpArrow (GtkShadowType t) :
	    DoubleArrowVertical (GTK_ARROW_UP, GTK_ARROW_UP, t) {}
};

class DoubleDownArrow : public DoubleArrowVertical

{
public:
    DoubleDownArrow (GtkShadowType t) :
	    DoubleArrowVertical (GTK_ARROW_DOWN, GTK_ARROW_DOWN, t) {}
};

class UpDownArrow : public DoubleArrowVertical

{
public:
    UpDownArrow (GtkShadowType t) :
	    DoubleArrowVertical (GTK_ARROW_UP, GTK_ARROW_DOWN, t) {}
};

typedef UpDownArrow DoubleUpDownArrow;
typedef LeftRightArrow DoubleLeftRightArrow;

} /* namespace */


/*---------------------------------------------------------------*/

Spinner::Spinner (Spinner_Layout layout,
		  Gtk::Adjustment &adj,
		  gfloat cr,
		  bool show_step,
		  bool show_page,
		  bool show_up,
		  bool show_down,
		  gint row_pack_options,
		  gint col_pack_options,
		  bool all_arrows_horizontal)

	: Gtk::Table (6, 6),
	  AutoSpin (adj, cr)
	
{
	with_step = show_step;
	with_page = show_page;
	with_up = show_up;
	with_down = show_down;
	filter = default_filter;
	filter_arg = 0;

	step_up = 0;
	step_down = 0;
	page_up = 0;
	page_down = 0;

	if (with_step) {
		if (with_up) {
			step_up = new Gtk::Button;

			step_up->button_press_event.connect(SigC::bind(SigC::slot(this,&Spinner::button_press),StepUp));
			step_up->button_release_event.connect(SigC::slot(this,&Spinner::button_release));
		}

		if (with_down) {
			step_down = new Gtk::Button;
			
			step_down->button_press_event.connect(SigC::bind(SigC::slot(this,&Spinner::button_press),StepDown));
			step_down->button_release_event.connect(SigC::slot(this,&Spinner::button_release));
		}
	}
	if (with_page) {
		if (with_up) {
			page_up = new Gtk::Button;

			page_up->button_press_event.connect(SigC::bind(SigC::slot(this,&Spinner::button_press),PageUp));
			page_up->button_release_event.connect(SigC::slot(this,&Spinner::button_release));
		}

		if (with_down) {
			page_down = new Gtk::Button;
			
			page_down->button_press_event.connect(SigC::bind(SigC::slot(this,&Spinner::button_press),PageDown));
			
			page_down->button_release_event.connect(SigC::slot(this,&Spinner::button_release));
		}
	}

	attach (entry, 2,3,2,3, row_pack_options, col_pack_options);

	switch (layout) {
	case LeftAndRight:
		if (with_page) {
			if (with_up) {
				attach (*page_up, 0,1,2,3);
				if (with_down) {
					page_up->add 
						(*(new DoubleRightArrow
						 (GTK_SHADOW_OUT)));
				} else {
					page_up->add 
						(*(new LeftRightArrow
						   (GTK_SHADOW_OUT)));
				}
			}
			if (with_down) {
				attach (*page_down, 4,5,2,3);
				if (with_up) {
					page_down->add 
						(*(new DoubleLeftArrow
						 (GTK_SHADOW_OUT)));
				} else {
					page_down->add 
						(*(new DoubleLeftRightArrow
						 (GTK_SHADOW_OUT)));
				}
			}
		} 
		if (with_step) {
			if (with_up) {
				attach (*step_up, 1,2,2,3);
				if (with_down) {
					step_up->add 
						(*(new Gtk::Arrow
						   (GTK_ARROW_RIGHT, GTK_SHADOW_OUT)));
				} else {
					step_up->add 
						(*(new LeftRightArrow
						   (GTK_SHADOW_OUT)));
				}
			}
			if (with_down) {
				attach (*step_down, 3,4,2,3);
				if (with_up) {
					step_down->add 
						(*(new Gtk::Arrow
						 (GTK_ARROW_LEFT, GTK_SHADOW_OUT)));
				} else {
					step_down->add 
						(*(new LeftRightArrow
						 (GTK_SHADOW_OUT)));
				}
			}
		}
		break;

	case TopAndBottom:
		if (with_page) {
			if (with_up) {
				attach (*page_up, 2,3,0,1);
				if (with_down) {
					if (all_arrows_horizontal) {
						page_up->add 
							(*(new DoubleRightArrow
							 (GTK_SHADOW_OUT)));
					} else {
						page_up->add 
							(*(new DoubleUpArrow
							 (GTK_SHADOW_OUT)));
					}
				} else {
					if (all_arrows_horizontal) {
						page_up->add 
							(*(new DoubleLeftRightArrow
							 (GTK_SHADOW_OUT)));
					} else {
						page_up->add 
							(*(new DoubleUpDownArrow
							 (GTK_SHADOW_OUT)));
					}
				}
			}
			if (with_down) {
				attach (*page_down, 2,3,4,5);
				if (with_up) {
					if (all_arrows_horizontal) {
						page_down->add 
							(*(new DoubleLeftArrow
							 (GTK_SHADOW_OUT)));
					} else {
						page_down->add 
							(*(new DoubleDownArrow
							 (GTK_SHADOW_OUT)));
					}
				} else {
					if (all_arrows_horizontal) {
						page_down->add 
							(*(new DoubleLeftRightArrow
							 (GTK_SHADOW_OUT)));
					} else {
						page_down->add 
							(*(new DoubleUpDownArrow
							 (GTK_SHADOW_OUT)));
					}
				}
			}
		}
		if (with_step) {
			if (with_up) {
				attach (*step_up, 2,3,1,2);
				if (with_down) {
					if (all_arrows_horizontal) {
						step_up->add 
							(*(new Gtk::Arrow
							 (GTK_ARROW_RIGHT, GTK_SHADOW_OUT)));
					} else {
						step_up->add 
							(*(new Gtk::Arrow
							 (GTK_ARROW_UP, GTK_SHADOW_OUT)));
					}
				} else {
					if (all_arrows_horizontal) {
						step_up->add 
							(*(new LeftRightArrow
							 (GTK_SHADOW_OUT)));
					} else {
						step_up->add 
							(*(new UpDownArrow
							 (GTK_SHADOW_OUT)));
					}
				}
			}
			if (with_down) {
				attach (*step_down, 2,3,3,4);
				if (with_up) {
					if (all_arrows_horizontal) {
						step_down->add 
							(*(new Gtk::Arrow
							 (GTK_ARROW_LEFT, GTK_SHADOW_OUT)));
					} else {
						step_down->add 
							(*(new Gtk::Arrow
							   (GTK_ARROW_DOWN, GTK_SHADOW_OUT)));
					}
				} else {
					if (all_arrows_horizontal) {
						step_down->add 
							(*(new LeftRightArrow
							   (GTK_SHADOW_OUT)));
					} else {
						step_down->add 
							(*(new UpDownArrow
							   (GTK_SHADOW_OUT)));
					}
				}
			}
		}
		break;
		
	case StepLR_PageTB:
		if (with_page) {
			if (with_up) {
				attach (*page_up, 2,3,1,2);
				if (with_down) {
					if (all_arrows_horizontal) {
						page_up->add 
							(*(new DoubleRightArrow
							 (GTK_SHADOW_OUT)));
					} else {
						page_up->add 
							(*(new DoubleUpArrow
							   (GTK_SHADOW_OUT)));
					}
				} else {
					if (all_arrows_horizontal) {
						page_up->add 
							(*(new DoubleLeftRightArrow
							   (GTK_SHADOW_OUT)));
					} else {
						page_up->add 
							(*(new DoubleUpDownArrow
							   (GTK_SHADOW_OUT)));
					}
				}
			}
			if (with_down) {
				attach (*page_down, 2,3,3,4);
				if (with_up) {
					if (all_arrows_horizontal) {
						page_down->add 
							(*(new DoubleLeftArrow
							   (GTK_SHADOW_OUT)));
					} else {
						page_down->add 
							(*(new DoubleDownArrow
							   (GTK_SHADOW_OUT)));
					}
				} else {
					if (all_arrows_horizontal) {
						page_down->add 
							(*(new DoubleLeftRightArrow
							   (GTK_SHADOW_OUT)));
					} else {
						page_down->add 
							(*(new DoubleUpDownArrow
							   (GTK_SHADOW_OUT)));
					}
				}
			}
		} 
		if (with_step) {
			if (with_up) {
				attach (*step_up, 1,2,2,3);
				if (with_down) {
					step_up->add 
						(*(new Gtk::Arrow
						 (GTK_ARROW_RIGHT, GTK_SHADOW_OUT)));
				} else {
					step_up->add 
						(*(new LeftRightArrow
						   (GTK_SHADOW_OUT)));
				}
			}
			if (with_down) {
				attach (*step_down, 3,4,2,3);
				if (with_up) {
					step_down->add 
						(*(new Gtk::Arrow
						   (GTK_ARROW_LEFT, GTK_SHADOW_OUT)));
				} else {
					step_down->add 
						(*(new LeftRightArrow
						   (GTK_SHADOW_OUT)));
				}
			}
		}
		break;
		
	case PageLR_StepTB:
		if (with_page) {
			if (with_up) {
				attach (*page_up, 1,2,2,3);
				if (with_down) {
					page_up->add 
						(*(new DoubleRightArrow
						   (GTK_SHADOW_OUT)));
				} else {
					page_up->add 
						(*(new DoubleLeftRightArrow
						   (GTK_SHADOW_OUT)));
				}
			}
			if (with_down) {
				attach (*page_down, 3,4,2,3);
				if (with_up) {
					page_down->add 
						(*(new DoubleLeftArrow
						   (GTK_SHADOW_OUT)));
				} else {
					page_down->add 
						(*(new DoubleLeftRightArrow
						   (GTK_SHADOW_OUT)));
				}
			}
		}
		
		if (with_step) {
			if (with_up) {
				attach (*step_up, 2,3,1,2);
				if (with_down) {
					if (all_arrows_horizontal) {
						step_up->add 
							(*(new Gtk::Arrow
							   (GTK_ARROW_RIGHT, GTK_SHADOW_OUT)));
					} else {
						step_up->add 
							(*(new Gtk::Arrow
							   (GTK_ARROW_UP, GTK_SHADOW_OUT)));
					}
				} else {
					if (all_arrows_horizontal) {
						step_up->add 
							(*(new LeftRightArrow
							   (GTK_SHADOW_OUT)));
					} else {
						step_up->add 
							(*(new UpDownArrow
							   (GTK_SHADOW_OUT)));
					}
				}
			}
		
			if (with_down) {
				attach (*step_down, 2,3,3,4);
				if (with_up) {
					if (all_arrows_horizontal) {
						step_down->add 
							(*(new Gtk::Arrow
							   (GTK_ARROW_LEFT, GTK_SHADOW_OUT)));
					} else {
						step_down->add 
							(*(new Gtk::Arrow
							   (GTK_ARROW_DOWN, GTK_SHADOW_OUT)));
					}
				} else {
					if (all_arrows_horizontal) {
						step_down->add 
							(*(new LeftRightArrow
							   (GTK_SHADOW_OUT)));
					} else {
						step_down->add 
							(*(new UpDownArrow
							   (GTK_SHADOW_OUT)));
					}
				}
			}
		}
		break;
	}

	get_adjustment().value_changed.connect(SigC::slot(this,&Spinner::value_changed));

	if (step_up)   step_up->show_all ();
	if (step_down) step_down->show_all ();
	if (page_up)   page_up->show_all ();
	if (page_down) page_down->show_all ();

	filter (*this, get_adjustment().get_value(), filter_arg);
}

gint
Spinner::button_release (GdkEventButton *ev)

{
	stop_spinning (ev);
	clicked (ev);
	return FALSE;
}

gint
Spinner::button_press (GdkEventButton *ev, ButtonType bt)

{
	AutoSpin::button_press (ev);
	return FALSE;
}

void
Spinner::default_filter (Spinner &spinner, gfloat val, void *arg)

{
	char buf[32];
	sprintf (buf, "%.2f", val);
	spinner.get_entry ().set_text (buf);
}

void
Spinner::value_changed ()

{
	filter (*this, get_adjustment().get_value(), filter_arg);
}

void
Spinner::set_filter (SpinnerFilterFunction func, void *arg)

{
	filter = func;
	filter_arg = arg;

	/* change display, potentially */

	filter (*this, get_adjustment().get_value (), filter_arg);
}


Gtk::Button *
Spinner::get_button (ButtonType bt)

{
	switch (bt) {
	case StepUp:
		return step_up;

	case StepDown:
		return step_down;

	case PageUp:
		return page_up;

	case PageDown:
		return page_down;

	}

	return 0;
}
