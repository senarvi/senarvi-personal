
SUBDIRS                 = gtkmmext
MAINTAINERCLEANFILES = autom4te.cache Makefile.in aclocal.m4 configure config.h.in \
                          stamp-h.in  $(GENSIG_LINK_HDRS) \
			  libgtkmmext.spec \
			  config.guess mkinstalldirs \
                          missing install-sh config.sub ltconfig \
                          ltmain.sh acinclude.m4 config.log \
			  config.status depcomp

SUFFIXES                = .gen_h
INCLUDES                = -I$(top_srcdir)

#GTKMMPROC = $(shell gtkmm-config --proc)
GTKMMPROC = ../gtkmm/src/gtkmmproc/gtkmmproc

# this rule tries to get gtkmmproc to put the *_p.h file in gtkmmext/private.
# as a result, the *.cc file it generates is in gtkmmext, so we have
# to manually move it back up to this level before proceeding. 
# we also have to delete the reference to "private/class_p.h", because 
# it is useless (and, for some strange reason, redundant), and change
# the inclusion of "class.h" to <gtkmmext/class.h>.

%.cc: %.gen_h
	class=$(notdir $(basename $^)) && \
	$(GTKMMPROC) -l $$class $(top_srcdir)/ gtkmmext && \
	mv gtkmmext/$${class}.cc . && \
	sed  \
	  -e '/#include "private\/'$${class}'_p.h"/d' \
	  -e 's%#include "'$${class}'.h"%#include <gtkmmext/'$${class}'.h>%' \
	       < $${class}.cc > $${class}.XX && \
	mv $${class}.XX $${class}.cc

BUILT_SOURCES := marked_curve.cc \
                 transparency.cc \
	         rgb.cc \
	         levelhold.cc \
	         databox.cc

BUILT_HDRS := gtkmmext/marked_curve.h \
	      gtkmmext/transparency.h \
	      gtkmmext/rgb.h \
	      gtkmmext/levelhold.h \
              private/marked_curve_p.h \
              private/transparency_p.h \
              private/databox_p.h \
              private/rgb_p.h \
              private/levelhold_p.h \
	      gtkmmext/databox.h

noinst_LIBRARIES = libgtkmmext.a

libgtkmmext_a_SOURCES = \
	slider_controller.cc \
        pix.cc \
        shaped_window.cc \
        utils.cc \
        prompter.cc \
        hexentry.cc \
	auto_spin.cc \
        spinner.cc \
        selector.cc \
        textviewer.cc \
	popup_selector.cc \
	popup.cc \
	choice.cc \
	click_box.cc \
	led.cc \
	pixmap_button.cc \
	version.cc \
        gtk_ui.cc \
	gtk_ellipsoid.cc \
        generic_motion_feedback.cc \
	motionfeedback.cc \
\
        gtkrgb.c \
        gtkdatabox.c \
	gtklevelhold.c \
\
	gtktransparency.c \
        gdkutils.c  \
	gtkmarkedcurve.c \
        gtkutils.c \
	gtkpixscrollbar.c \
\
	$(BUILT_SOURCES)

EXTRA_DIST = libgtkmmext.pc.in $(BUILT_SOURCES) $(BUILT_HDRS)

#
# doing this as part of maintainer-clean causes all kinds of problems
#

scrub:
	rm -rf $(BUILT_HDRS) $(BUILT_SOURCES)


