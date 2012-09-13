
SUBDIRS = pbd
sysdep_src = @SYSDEP_SRC@

MAINTAINERCLEANFILES = autom4te.cache  Makefile.in aclocal.m4 configure config.h.in \
                        stamp-h.in pbd-config libpbd.spec \
                        config.guess mkinstalldirs \
                        missing install-sh config.sub ltconfig \
                        ltmain.sh acinclude.m4 \
			config.log config.status depcomp

noinst_LIBRARIES    = libpbd.a

libpbd_a_SOURCES = \
     pthread_utils.cc \
     mountpoint.cc \
     xml++.cc \
     undo.cc \
     basename.cc \
     dmalloc.cc \
     ellipsoid.cc \
     forkexec.cc \
     language_guru.cc \
     pathscanner.cc \
     parser.cc \
     platform_factory.cc \
     pool.cc \
     precision_timer.cc \
     pthread_spinlock.cc \
	 pthread_utils.cc \
     receiver.cc \
     restartable_rw.cc \
     ringbuffer.cc \
     rt.cc \
     rtthread.cc \
     selectable.cc \
     strsub.cc \
     cxx_strdup.cc \
     textreceiver.cc \
     thread.cc \
     transmitter.cc \
     unescape.cc \
     version.cc \
     linux_platform.cc

XTRA_SRC = \
  irix_platform.cc \
  solaris_platform.cc \
  linux_platform.cc

EXTRA_DIST = $(XTRA_SRC) \
	     libpbd.spec.in \
	     libpbd.pc.in
