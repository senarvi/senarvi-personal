#ifndef __gtkmmext_spinner_h__
#define __gtkmmext_spinner_h__

#include <gtk--.h>
#include <gtkmmext/auto_spin.h>

namespace Gtkmmext {

class Spinner;

typedef void (*SpinnerFilterFunction)(Spinner &, gfloat, void *);

enum Spinner_Layout {
	LeftAndRight,
	TopAndBottom,
	StepLR_PageTB,
	PageLR_StepTB
};

class Spinner : public Gtk::Table, public AutoSpin

{
  public:
	enum ButtonType { 
		StepUp,
		StepDown,
		PageUp,
		PageDown
	};

	Spinner (Spinner_Layout layout,
		     Gtk::Adjustment &adj,
		     gfloat cr = 0,
		     bool with_step = true,
		     bool with_page = true,
		     bool with_up = true,
		     bool with_down = true,
		     gint row_pack_options = GTK_FILL|GTK_EXPAND,
		     gint col_pack_options = GTK_FILL|GTK_EXPAND,
		     bool all_arrows_horizontal = true);

	Gtk::Button *get_button (ButtonType);

	void set_filter (SpinnerFilterFunction func, void *arg = 0);
	Gtk::Entry &get_entry() { return entry; }

	SigC::Signal1<void,GdkEventButton *> clicked;
	
  private:
	Gtk::Entry  entry;
	Gtk::Button *step_up;
	Gtk::Button *step_down;
	Gtk::Button *page_up;
	Gtk::Button *page_down;
	bool with_step;
	bool with_page;
	bool with_up;
	bool with_down;
	SpinnerFilterFunction filter;
	void *filter_arg;

	gint button_release (GdkEventButton *);
	gint button_press (GdkEventButton *, ButtonType);
	void value_changed ();
	static void default_filter (Spinner &, gfloat, void *);
};

} /* namespace */
	
#endif /* __gtkmmext_spinner_h__ */
