/*
    GDAM - Geoff & Dave's Audio Mixer
    Copyright (C) 1999    Dave Benson, Geoff Matters.
    Modified 2000 by Paul Davis to remove dependency on GDAM
    Modified 2002 by Paul Davis to provide state-dependent sensitivity

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
       <pbd@op.net> Paul Davis
*/

#include <gtkmmext/gtkpixscrollbar.h>
#include <gtk/gtksignal.h>
#include <gtk/gtkmain.h>
#include <stdlib.h>

/* 
 * XXX: currently the entire scrollbar is redrawn when
 *      the bauble moves.
 */

/* XXX: memory leak from pixmaps, names  in destroy */

enum {
	MOTION,
	LAST_SIGNAL
};
static guint pix_scrollbar_signals[LAST_SIGNAL] = { 0 };

static int  gtk_pix_scrollbar_expose_event          (GtkWidget*       widget,
                                                     GdkEventExpose*  event);
static int  gtk_pix_scrollbar_configure_event       (GtkWidget*       widget,
                                                     GdkEventConfigure*event);
static void gtk_pix_scrollbar_draw                  (GtkWidget*       widget,
                                                     GdkRectangle*    area);
static void gtk_pix_scrollbar_draw_default          (GtkWidget *widget);
static void gtk_pix_scrollbar_draw_focus            (GtkWidget *widget);
static void gtk_pix_scrollbar_size_request          (GtkWidget*       widget,
                                                     GtkRequisition*  event);
static int  gtk_pix_scrollbar_press_event           (GtkWidget*       widget,
                                                     GdkEventButton*  event);
static int  gtk_pix_scrollbar_release_event         (GtkWidget*       widget,
                                                     GdkEventButton*  event);
static int  gtk_pix_scrollbar_enter_notify_event    (GtkWidget*       widget,
                                                     GdkEventCrossing*  event);
static int  gtk_pix_scrollbar_leave_notify_event    (GtkWidget*       widget,
                                                     GdkEventCrossing*event);
static int  gtk_pix_scrollbar_motion_event          (GtkWidget*       widget,
                                                     GdkEventMotion*  event);
static void update_adjustment(GtkPixScrollbar* pix_scrollbar,
                              int              cur_x,
			      int              cur_y,
			      int	       button,
			      int              mask);
static GtkObjectClass* parent_class;
static void gtk_pix_scrollbar_init(GtkPixScrollbar* pix_scrollbar)
{
  GTK_WIDGET_SET_FLAGS (pix_scrollbar, GTK_CAN_FOCUS | GTK_RECEIVES_DEFAULT);
  gtk_widget_add_events (GTK_WIDGET (pix_scrollbar),
			 GDK_EXPOSURE_MASK
			 | GDK_STRUCTURE_MASK
			 | GDK_ENTER_NOTIFY_MASK
			 | GDK_LEAVE_NOTIFY_MASK
			 | GDK_BUTTON_RELEASE_MASK
			 | GDK_BUTTON_PRESS_MASK | GDK_MOTION_NOTIFY | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);
  pix_scrollbar->drag_to_position = 1;
}
static void gtk_pix_scrollbar_class_init(GtkPixScrollbarClass* klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
  GtkObjectClass *object_class = GTK_OBJECT_CLASS (klass);
  widget_class->expose_event = gtk_pix_scrollbar_expose_event;
  widget_class->configure_event = gtk_pix_scrollbar_configure_event;
  widget_class->button_press_event = gtk_pix_scrollbar_press_event;
  widget_class->button_release_event = gtk_pix_scrollbar_release_event;
  widget_class->motion_notify_event = gtk_pix_scrollbar_motion_event;
  widget_class->enter_notify_event = gtk_pix_scrollbar_enter_notify_event;
  widget_class->leave_notify_event = gtk_pix_scrollbar_leave_notify_event;
  widget_class->size_request = gtk_pix_scrollbar_size_request;
  widget_class->draw = gtk_pix_scrollbar_draw;
  widget_class->draw_focus = gtk_pix_scrollbar_draw_focus;
  widget_class->draw_default = gtk_pix_scrollbar_draw_default;
  pix_scrollbar_signals[MOTION] =
    gtk_signal_new ("motion",
		    GTK_RUN_FIRST,
		    object_class->type,
		    GTK_SIGNAL_OFFSET (GtkPixScrollbarClass, motion),
		    gtk_marshal_NONE__NONE, GTK_TYPE_NONE, 0);
  gtk_object_class_add_signals (object_class,
				pix_scrollbar_signals, LAST_SIGNAL);
}

GtkType gtk_pix_scrollbar_get_type()
{
  static GtkType pix_scrollbar_type = 0;
  if (!pix_scrollbar_type)
    {
      static const GtkTypeInfo pix_scrollbar_info = {
	"GtkPixScrollbar",
	sizeof (GtkPixScrollbar),
	sizeof (GtkPixScrollbarClass),
	(GtkClassInitFunc) gtk_pix_scrollbar_class_init,
	(GtkObjectInitFunc) gtk_pix_scrollbar_init,
	/* reserved_1 */ NULL,
	/* reserved_2 */ NULL,
	(GtkClassInitFunc) NULL
      };
      GtkType parent = GTK_TYPE_EVENT_BOX;
      parent_class = gtk_type_class (parent);
      pix_scrollbar_type = gtk_type_unique (parent, &pix_scrollbar_info);
    }
  return pix_scrollbar_type;
}

static inline gfloat get_fraction(GtkPixScrollbar* pix_scrollbar)
{
  gfloat fraction;
  if (pix_scrollbar->adjustment != NULL)
    {
      GtkAdjustment *adjustment = pix_scrollbar->adjustment;
      fraction = (adjustment->value - adjustment->lower)
	/ (adjustment->upper - adjustment->lower);
      fraction = CLAMP (fraction, 0.0, 1.0);
    }
  else
    {
      fraction = 0.5;
    }
  return fraction;
}
static void
gtk_pix_scrollbar_draw_focus (GtkWidget *widget)
{
  gtk_widget_draw (widget, NULL);
}

static void
gtk_pix_scrollbar_draw_default (GtkWidget *widget)
{
  gtk_widget_draw (widget, NULL);
}


static void gtk_pix_scrollbar_draw(GtkWidget* widget, GdkRectangle* area)
{
  GtkPixScrollbar *pix_scrollbar = GTK_PIX_SCROLLBAR (widget);
  GdkWindow *window = widget->window;
  GdkGCValues values;
  int window_width = widget->allocation.width;
  int window_height = widget->allocation.height;
  int x, y;
  gfloat fraction;
  int pix_width, pix_height;

  if (!GTK_WIDGET_DRAWABLE (widget))
    {
      return;
    }

  gdk_window_set_back_pixmap (widget->window, NULL, TRUE);
  gdk_window_clear_area (widget->window,
			 area->x, area->y, area->width, area->height);
  if (GTK_WIDGET_HAS_DEFAULT (widget) &&
      /* GTK_BUTTON (widget)->relief == GTK_RELIEF_NORMAL && */
      pix_scrollbar->background == NULL)
    {
      gtk_paint_box (widget->style, widget->window,
		     GTK_STATE_NORMAL, GTK_SHADOW_IN,
		     area, widget, "buttondefault",
		     area->x, area->y, area->width, area->height);
    }


  gdk_gc_get_values (widget->style->fg_gc[0], &values);

  if (pix_scrollbar->background)
    {
      pix_width = pix_scrollbar->background_size.x;
      pix_height = pix_scrollbar->background_size.y;
      x = (window_width - pix_width) / 2;
      y = (window_height - pix_height) / 2;
      gdk_gc_set_clip_origin (widget->style->bg_gc[0], x, y);
      gdk_gc_set_clip_mask (widget->style->bg_gc[0],
			    pix_scrollbar->background_mask);

      gdk_window_copy_area (window,
			    widget->style->bg_gc[0],
			    x,
			    y,
			    pix_scrollbar->background,
			    0, 0, pix_width, pix_height);
      gdk_gc_set_clip_origin (widget->style->bg_gc[0], 0, 0);
      gdk_gc_set_clip_mask (widget->style->bg_gc[0], NULL);
    }
  else
    {
      x = y = 0;
    }

  gdk_gc_set_clip_mask (widget->style->fg_gc[0], NULL);
  gdk_gc_set_clip_mask (widget->style->fg_gc[0], pix_scrollbar->floater_mask);
  fraction = get_fraction (pix_scrollbar);
  pix_width = pix_scrollbar->floater_size.x;
  pix_height = pix_scrollbar->floater_size.y;

  x += (fraction * pix_scrollbar->trough_delta_x);
  x += pix_scrollbar->trough_start_x;
  y += (fraction * pix_scrollbar->trough_delta_y);
  y += pix_scrollbar->trough_start_y;
  x -= pix_width / 2;
  y -= pix_height / 2;

  gdk_gc_set_clip_origin (widget->style->fg_gc[0], x, y);
  gdk_window_copy_area (window,
			widget->style->fg_gc[0],
			x,
			y,
			pix_scrollbar->floater, 0, 0, pix_width, pix_height);

  gdk_gc_set_clip_mask (widget->style->fg_gc[0], NULL);
  gdk_gc_set_clip_origin (widget->style->fg_gc[0],
			  values.clip_x_origin, values.clip_y_origin);
}

/* update:
 *     in_child
 *     drag_offset_x
 *     drag_offset_y
 */
static void recompute_in_child(GtkPixScrollbar* pix_scrollbar,
                               int              press_x,
			       int              press_y)
{
  gfloat fraction;
  GtkAdjustment *adj = pix_scrollbar->adjustment;
  int width, height;
  int floater_x, floater_y;
  GtkAllocation *allocation;

  /*
   * Make press_x, press_y relative to the centered
   * trough pixmap.
   */
  allocation = &GTK_WIDGET (pix_scrollbar)->allocation;
  press_x -= (allocation->width - pix_scrollbar->background_size.x) / 2;
  press_y -= (allocation->height - pix_scrollbar->background_size.y) / 2;

  fraction = (adj->value - adj->lower) / (adj->upper - adj->lower);
  floater_x = pix_scrollbar->trough_start_x
    + fraction * pix_scrollbar->trough_delta_x;
  floater_y = pix_scrollbar->trough_start_y
    + fraction * pix_scrollbar->trough_delta_y;
  press_x -= floater_x;
  press_y -= floater_y;
  width = pix_scrollbar->floater_size.x;
  height = pix_scrollbar->floater_size.y;
  if ((-width / 2 <= press_x && press_x <= +width / 2)
      && (-height / 2 <= press_y && press_y <= +height / 2))
    {
      pix_scrollbar->in_child = TRUE;
      pix_scrollbar->drag_offset_x = press_x;
      pix_scrollbar->drag_offset_y = press_y;
    }
  else
    {
      pix_scrollbar->in_child = FALSE;
    }
}

static int gtk_pix_scrollbar_press_event(GtkWidget*       widget,
                                         GdkEventButton*  event)
{
  GtkPixScrollbar *pix_scrollbar = GTK_PIX_SCROLLBAR (widget);
  gfloat fraction;
  int x_offset, y_offset;
  int width2, height2;
  int press_x = event->x;
  int press_y = event->y;

  gtk_grab_add (widget);
  recompute_in_child (pix_scrollbar, press_x, press_y);
  update_adjustment (pix_scrollbar, press_x, press_y, event->button, event->state);
  gtk_widget_draw_default (GTK_WIDGET (pix_scrollbar));

  return TRUE;
}

static void reset_adjustment(GtkPixScrollbar* pix_scrollbar)
{
  GtkAdjustment *adj;
  GtkObject *object;
  g_return_if_fail (pix_scrollbar->adjustment != NULL);
  adj = pix_scrollbar->adjustment;

  object = GTK_OBJECT (adj);
  gtk_signal_handler_block (object, pix_scrollbar->connect_id);
  gtk_signal_handler_block (object, pix_scrollbar->value_connect_id);
  gtk_adjustment_set_value (adj, pix_scrollbar->orig_value);
  gtk_signal_handler_unblock (object, pix_scrollbar->value_connect_id);
  gtk_signal_handler_unblock (object, pix_scrollbar->connect_id);
  gtk_widget_draw_default (GTK_WIDGET (pix_scrollbar));
}

static void update_adjustment(GtkPixScrollbar* pix_scrollbar,
                              int              cur_x,
			      int              cur_y,
			      int	       button,
			      int              mask)
{
  int x, y;
  int trough_x = pix_scrollbar->trough_delta_x;
  int trough_y = pix_scrollbar->trough_delta_y;
  gfloat value;
  gfloat press_value;
  GtkAdjustment *adj;
  GtkObject *object;
  GtkAllocation *allocation;
  gfloat fraction;
  gfloat scale;
  gfloat top, bot;

  g_return_if_fail (pix_scrollbar->adjustment != NULL);
  adj = pix_scrollbar->adjustment;
  allocation = &GTK_WIDGET (pix_scrollbar)->allocation;

  /*
   * Make cur_x, cur_y relative to the centered
   * trough pixmap.
   */
  cur_x -= (allocation->width - pix_scrollbar->background_size.x) / 2;
  cur_y -= (allocation->height - pix_scrollbar->background_size.y) / 2;

  if (!pix_scrollbar->drag_to_position && pix_scrollbar->in_child)
    {
      cur_x += pix_scrollbar->drag_offset_x;
      cur_y += pix_scrollbar->drag_offset_y;
    }
  x = cur_x - pix_scrollbar->trough_start_x;
  y = cur_y - pix_scrollbar->trough_start_y;
  fraction = x * trough_x + y * trough_y;
  fraction /= trough_x * trough_x + trough_y * trough_y;

  if (mask & GDK_CONTROL_MASK) {
	  if (mask & GDK_MOD1_MASK) {
		  scale = 0.05;
	  } else {
		  scale = 0.1;
	  }
  } else {
	  scale = 1.0;
  }

  if (scale != 1.0) {
	  top = adj->value + (scale * (adj->upper - adj->value));
	  bot = adj->value - (scale * (adj->value - adj->lower));
  } else {
	  top = adj->upper;
	  bot = adj->lower;
  }

  press_value = fraction * (top - bot) + bot;

  if (pix_scrollbar->drag_to_position)
    {
      /* This case moves the scrollbar to an absolute
       * mouse position. 
       */
      value = press_value;
    }
  else if (button == 5)
    {
      /* TODO: check adjustment limits on button 4,5?? */
      /* TODO: are these bindings wrong for horizontal scrollbars? */
      value = adj->value + adj->page_size;
    }
  else if (button == 4)
    {
      value = adj->value - adj->page_size;
    }
  else if (pix_scrollbar->in_child)
    {
      value = press_value;
    }
  else if (button == 2)
    {
      value = press_value;
    }
  else
    {

      /* Otherwise, move by some amount */

      value = adj->value;

      if (press_value < value - adj->page_size)
	{
	  value -= adj->page_size;
	}
      else if (press_value > value - adj->page_size)
	{
	  value += adj->page_size;
	}
      else
	{
	  value = press_value;
	}
    }

  object = GTK_OBJECT (adj);
  gtk_signal_handler_block (object, pix_scrollbar->connect_id);
  gtk_signal_handler_block (object, pix_scrollbar->value_connect_id);
  gtk_adjustment_set_value (adj, value);
  gtk_signal_handler_unblock (object, pix_scrollbar->value_connect_id);
  gtk_signal_handler_unblock (object, pix_scrollbar->connect_id);
  gtk_widget_draw_default (GTK_WIDGET (pix_scrollbar));
}

static int gtk_pix_scrollbar_release_event(GtkWidget*       widget,
                                           GdkEventButton*  event)
{
  gboolean clicked = FALSE;
  GtkPixScrollbar *pix_scrollbar = GTK_PIX_SCROLLBAR (widget);
  int x, y;
  guint state;

  gtk_grab_remove (widget);

  if (event->window != widget->window)
    {
      gdk_window_get_pointer (widget->window, &x, &y, &state);
    }
  else
    {
      x = event->x;
      y = event->y;
      state = event->state;
    }

  if ((state & GDK_SHIFT_MASK) && pix_scrollbar->do_reset) 
    {
      reset_adjustment (pix_scrollbar);
      return TRUE;
    }
  else if ((state & GDK_CONTROL_MASK|GDK_SHIFT_MASK|GDK_MOD1_MASK) == GDK_MOD1_MASK)
    {
       GtkObject *object = GTK_OBJECT (pix_scrollbar->adjustment);
       gtk_signal_handler_block (object, pix_scrollbar->connect_id);
       gtk_signal_handler_block (object, pix_scrollbar->value_connect_id);
       gtk_adjustment_set_value (pix_scrollbar->adjustment, 0.0);
       gtk_signal_handler_unblock (object, pix_scrollbar->value_connect_id);
       gtk_signal_handler_unblock (object, pix_scrollbar->connect_id);
       gtk_widget_draw_default (GTK_WIDGET (pix_scrollbar));
       return FALSE;
    }
	  
  pix_scrollbar->in_child = FALSE;
  update_adjustment (pix_scrollbar, x, y, event->button, state);
  gtk_pix_scrollbar_motion (pix_scrollbar);

  return TRUE;
}

static int gtk_pix_scrollbar_expose_event(GtkWidget*       widget,
                                          GdkEventExpose*  event)
{
  GtkPixScrollbar *pix_scrollbar = GTK_PIX_SCROLLBAR (widget);
  gtk_widget_draw_default (GTK_WIDGET (pix_scrollbar));
  return FALSE;
}
static int gtk_pix_scrollbar_configure_event(GtkWidget*       widget,
                                             GdkEventConfigure*event)
{
  GtkPixScrollbar *pix_scrollbar = GTK_PIX_SCROLLBAR (widget);
  if (pix_scrollbar->lastw == event->width &&
      pix_scrollbar->lasth == event->height)
    return FALSE;
  pix_scrollbar->lastw = event->width;
  pix_scrollbar->lasth = event->height;
  gtk_widget_draw_default (GTK_WIDGET (pix_scrollbar));
  return FALSE;
}
static int gtk_pix_scrollbar_enter_notify_event(GtkWidget*       widget,
                                                GdkEventCrossing*event)
{
  GtkPixScrollbar *pix_scrollbar = GTK_PIX_SCROLLBAR (widget);
  GtkWidget *event_widget;
  return FALSE;
}
static int gtk_pix_scrollbar_leave_notify_event(GtkWidget*        widget,
                                                GdkEventCrossing* event)
{
  GtkPixScrollbar *pix_scrollbar = GTK_PIX_SCROLLBAR (widget);
  GtkWidget *event_widget;
  event_widget = gtk_get_event_widget ((GdkEvent *) event);
  if (event_widget != widget || event->detail == GDK_NOTIFY_INFERIOR)
    return FALSE;
  gtk_widget_draw_default (widget);
  return FALSE;
}
static void gtk_pix_scrollbar_size_request    (GtkWidget*       widget,
                                               GtkRequisition*  requisition)
{
  GtkPixScrollbar *pix_scrollbar = GTK_PIX_SCROLLBAR (widget);
  requisition->width = pix_scrollbar->background_size.x;
  requisition->height = pix_scrollbar->background_size.y;
}

static int  gtk_pix_scrollbar_motion_event          (GtkWidget*       widget,
                                                     GdkEventMotion*  event)
{
  int x, y;
  guint state;
  GtkPixScrollbar *pix_scrollbar;
  pix_scrollbar = GTK_PIX_SCROLLBAR (widget);
  if (event->is_hint)
    {
      gdk_window_get_pointer (widget->window, &x, &y, &state);
    }
  else
    {
      if (event->window != widget->window)
	{
	  gdk_window_get_pointer (widget->window, &x, &y, &state);
	}
      else
	{
	  x = event->x;
	  y = event->y;
	  state = event->state;
	}
    }

  update_adjustment (pix_scrollbar, x, y, 0, state);
  gtk_pix_scrollbar_motion (pix_scrollbar);

  return 0;
}

void gtk_pix_scrollbar_motion(GtkPixScrollbar* pix_scrollbar)
{
  gtk_signal_emit (GTK_OBJECT (pix_scrollbar), pix_scrollbar_signals[MOTION]);
}

/*
 *   string1:    background:floater
 *   string2:    startx:starty:deltax:deltay:cur_value:min_value:max_value
 */

GtkWidget* gtk_pix_scrollbar_new   (GdkPixmap     *floater,
				    GdkBitmap     *floater_mask,
				    GdkPixmap     *background,
				    GdkBitmap     *background_mask,
				    GtkAdjustment *adjustment,
				    int            do_reset)

{
  GtkPixScrollbar *pix_scrollbar;

  g_return_if_fail(floater != NULL);
  g_return_if_fail(adjustment != NULL);

  pix_scrollbar = GTK_PIX_SCROLLBAR (gtk_type_new (GTK_TYPE_PIX_SCROLLBAR));

  pix_scrollbar->floater = floater;
  pix_scrollbar->floater_mask = floater_mask;

  gdk_window_get_size (floater,  
		       (gint *) &pix_scrollbar->floater_size.x,
		       (gint *) &pix_scrollbar->floater_size.y);

  if (background) {
	  gdk_window_get_size (background,
			       (gint *) &pix_scrollbar->background_size.x,
			       (gint *) &pix_scrollbar->background_size.y);
	  pix_scrollbar->background = background;
	  pix_scrollbar->background_mask = background_mask;
  } else {
	  pix_scrollbar->background = NULL;
  }

  /* heuristic:

     if the background is wider than it is tall, assume we want
     horizontal motion across it.

     if the background is taller than it is wide (or equal), assume
     we want vertical motion across it.
  */

  if (pix_scrollbar->background_size.x < pix_scrollbar->background_size.y) {
	  /* vertical */
	  pix_scrollbar->trough_start_x = pix_scrollbar->background_size.x / 2;
	  pix_scrollbar->trough_start_y = pix_scrollbar->background_size.y - pix_scrollbar->floater_size.x;
	  pix_scrollbar->trough_delta_x = 0;
	  pix_scrollbar->trough_delta_y = pix_scrollbar->floater_size.y - pix_scrollbar->background_size.y;
  } else {
	  /* horizontal */
	  pix_scrollbar->trough_start_x = pix_scrollbar->floater_size.x / 2;
	  pix_scrollbar->trough_start_y = pix_scrollbar->background_size.y / 2;
	  pix_scrollbar->trough_delta_x = pix_scrollbar->background_size.x - pix_scrollbar->floater_size.x;
	  pix_scrollbar->trough_delta_y = 0;
  }

  pix_scrollbar->do_reset = do_reset;

  pix_scrollbar->orig_value = adjustment->value;
  gtk_pix_scrollbar_set_adjustment (pix_scrollbar, adjustment);

  return GTK_WIDGET (pix_scrollbar);
}

void gtk_pix_scrollbar_set_adjustment(GtkPixScrollbar*  pix_scrollbar,
                                      GtkAdjustment*    adjustment)
{
  GtkAdjustment *old_adj = pix_scrollbar->adjustment;
  if (old_adj == adjustment)
    return;
  if (old_adj != NULL)
    {
      gtk_signal_disconnect (GTK_OBJECT (old_adj), pix_scrollbar->connect_id);
      gtk_signal_disconnect (GTK_OBJECT (old_adj),
			     pix_scrollbar->value_connect_id);
      gtk_object_unref (GTK_OBJECT (old_adj));
    }
  pix_scrollbar->adjustment = adjustment;
  gtk_object_ref (GTK_OBJECT (adjustment));
  if (adjustment == NULL)
    {
      pix_scrollbar->value_connect_id = 0;
      pix_scrollbar->connect_id = 0;
    }
  else
    {
      pix_scrollbar->value_connect_id
	= gtk_signal_connect_object
	(GTK_OBJECT (adjustment),
	 "value_changed",
	 GTK_SIGNAL_FUNC (gtk_widget_queue_draw), GTK_OBJECT (pix_scrollbar));
      pix_scrollbar->connect_id
	= gtk_signal_connect_object
	(GTK_OBJECT (adjustment),
	 "changed",
	 GTK_SIGNAL_FUNC (gtk_widget_queue_draw), GTK_OBJECT (pix_scrollbar));
    }
}
