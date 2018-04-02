#ifndef __gtk_popupselector_h__
#define __gtk_popupselector_h__

#include <gtkmmext/selector.h>
#include <pbd/touchable.h>

namespace Gtkmmext {

class PopupSelector : public Gtk::Window, public Touchable
{
 public:
	PopupSelector (const char *title, SelectorRefillFunction,
		       void *arg, 
		       bool delete_on_cancel = true,
		       size_t ncols = 1);
	SigC::Signal2<void,PopupSelector *, SelectionResult *> selection_made;

	Gtk::CList& clist() { return selector.clist(); }

	void touch() {
		selector.rescan();
	}
	
 protected:
	gint delete_event_impl (GdkEventAny *);
	gint map_event_impl (GdkEventAny *);

  private:
	Gtk::Button cancel;
	Gtk::Button accept;
	Gtk::Button rescan;
	Gtk::HBox hpacker;
	Gtk::VBox vpacker;
	Selector selector;
	bool delete_on_cancel;

	void cancelled ();
	void selector_made_selection (Selector *, SelectionResult *);
};

} /* namespace */

#endif  // __gtk_popupselector_h__
