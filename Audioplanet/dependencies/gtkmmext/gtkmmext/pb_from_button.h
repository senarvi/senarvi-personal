#ifndef __pbd_gtkmm_pixmap_button_h__
#define __pbd_gtkmm_pixmap_button_h__

#include <gtk--.h>
#include <gtkmmext/shaped_window.h>

class Pix;

class Gtk::PixmapButton : public Gtk::Button

{
	Gtk::ShapedWindow *pixwin;
	GtkStateType get_state() {
		return (GtkStateType) GTK_WIDGET_STATE(GTK_WIDGET(gtkobj()));
	}

  protected:
	virtual void state_changed_impl (GtkStateType);

  public:
	Gtk::PixmapButton (Pix *);
};

#endif  // __pbd_gtkmm_pixmap_button_h__
