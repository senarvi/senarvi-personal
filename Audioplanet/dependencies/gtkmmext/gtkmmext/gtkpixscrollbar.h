/*
    GDAM - Geoff & Dave's Audio Mixer
    Copyright (C) 1999    Dave Benson, Geoff Matters.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

    Contact:
        daveb@ffem.org <Dave Benson>
        geoff@ugcs.caltech.edu <Geoff Matters>
*/
#ifndef __GTK_PIX_SCROLLBAR_H_
#define __GTK_PIX_SCROLLBAR_H_

#include <gtk/gtkeventbox.h>

typedef struct _GtkPixScrollbar GtkPixScrollbar;
typedef struct _GtkPixScrollbarClass GtkPixScrollbarClass;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

GtkType gtk_pix_scrollbar_get_type();
#define GTK_TYPE_PIX_SCROLLBAR			(gtk_pix_scrollbar_get_type ())
#define GTK_PIX_SCROLLBAR(obj)              (GTK_CHECK_CAST ((obj), GTK_TYPE_PIX_SCROLLBAR, GtkPixScrollbar))
#define GTK_PIX_SCROLLBAR_CLASS(klass)      (GTK_CHECK_CLASS_CAST ((klass), GTK_TYPE_PIX_SCROLLBAR, GtkPixScrollbarClass))
#define GTK_IS_PIX_SCROLLBAR(obj)           (GTK_CHECK_TYPE ((obj), GTK_TYPE_PIX_SCROLLBAR))
#define GTK_IS_PIX_SCROLLBAR_CLASS(klass)   (GTK_CHECK_CLASS_TYPE ((klass), GTK_TYPE_PIX_SCROLLBAR))

struct _GtkPixScrollbarClass {
	GtkEventBoxClass event_box_class;
	void              (*motion)(GtkPixScrollbar*   pix_scrollbar);
};

struct _GtkPixScrollbar {
	GtkEventBox 			event_box;

	/* The background. */
	GdkPixmap*			background;
	GdkBitmap*			background_mask;
	GdkPoint			background_size;
	char*				background_xpm_file;

	/* The bauble. */
	GdkPixmap*			floater;
	GdkBitmap*			floater_mask;
	GdkPoint			floater_size;
	char*				floater_file;

	/* The trough start and extent. */
	int				trough_start_x;
	int				trough_start_y;
	int				trough_delta_x;
	int				trough_delta_y;

	/* For dragging. */
	gboolean			in_child;
	int				drag_offset_x;
	int				drag_offset_y;

	/* The underlying adjustment. */
	GtkAdjustment*			adjustment;

	/* The adjustment connection id. */
	guint				connect_id;
	guint				value_connect_id;

	/* Whether we have loaded the images yet... */
	unsigned			loaded_images : 1;

	/* Whether to update in between mouse_press and mouse_release. */
	unsigned			continuous : 1;

	/* Whether to update the floater to the click location
	 * even if they didn't start on the floater. */
	unsigned			drag_to_position : 1;

	/* Whether to jump to the slider origin on mouse off.  */
	unsigned			do_reset : 1;

	/*  Last allocated size to avoid needless config event redraws.  */
	gint				lastw;
	gint				lasth;

	gdouble				orig_value;
};

GtkWidget* gtk_pix_scrollbar_new                 (GdkPixmap *floater,
						  GdkBitmap *floater_mask,
						  GdkPixmap *background,
						  GdkBitmap *background_mask,
						  GtkAdjustment *adjustment,
						  int          do_reset);

void       gtk_pix_scrollbar_set_adjustment      (GtkPixScrollbar* button,
                                                  GtkAdjustment*   adjustment);
      
void       gtk_pix_scrollbar_motion              (GtkPixScrollbar* button);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
