#ifndef __pbd_gtkmm_choice_h__
#define __pbd_gtkmm_choice_h__

#include <gtk--.h>

namespace Gtkmmext {

class Choice : public Gtk::Window

{
 public:
	Choice (const std::string &prompt,
		    unsigned int nchoices,
		    const std::string *choices,
		    bool delete_when_done = false);
	virtual ~Choice ();

	/* This signal will be raised when a choice
	   is made or the choice window is deleted.
	   If the choice was to cancel, or the window
	   was deleted, then the argument will be -1.
	   Otherwise, it will be choice selected
	   of those presented, starting at zero.
	*/

	SigC::Signal1<void,int> choice_made;

  private:
	Gtk::VBox packer;
	Gtk::Label prompt_label;
	Gtk::HBox button_packer;
	Gtk::Button **buttons;
	int nchoices;
	bool delete_when_done;

	void _choice_made (int nbutton);
	gint closed (GdkEventAny *);
};

} /* namespace */

#endif  // __pbd_gtkmm_choice_h__
