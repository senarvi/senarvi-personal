## Process this file with automake to produce Makefile.in

MAINTAINERCLEANFILES = autom4te.cache Makefile.in aclocal.m4 configure config.h.in \
                          stamp-h.in libmidi++.spec \
			  config.guess mkinstalldirs \
                          missing install-sh config.sub ltconfig \
                          ltmain.sh acinclude.m4 config.log config.status \
			  depcomp

SUBDIRS                 = midi++
INCLUDES                = -I$(top_srcdir)

noinst_LIBRARIES           = libmidipp.a

libmidipp_a_LIBADD       = @SYSDEP_OBJS@
libmidipp_a_DEPENDENCIES = @SYSDEP_OBJS@
libmidipp_a_SOURCES      = \
           port_request.cc \
           mmc.cc \
	   midicontrollable.cc \
           fd_midiport.cc \
	   midi.cc \
	   midiport.cc \
	   midiparser.cc \
	   midichannel.cc \
	   midifactory.cc \
	   midimanager.cc \
	   fifomidi.cc \
    	   version.cc

XTRA_SRC = irix_rawmidi.cc
EXTRA_DIST = $(XTRA_SRC) midi.m4 libmidi++.spec.in \
	     libmidi++.pc.in

#bin_DIR = .
#noinst_PROGRAMS = mmc miditrace

#mmc_SOURCES = mmctest.cc
#mmc_LDFLAGS = `midi++-config --libs`

#miditrace_SOURCES = miditrace.cc
#miditrace_LDFLAGS = `midi++-config --libs`
