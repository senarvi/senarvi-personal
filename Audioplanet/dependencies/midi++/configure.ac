dnl Process this file with autoconf to produce a configure script.

AC_INIT(fd_midiport.cc)

AC_CONFIG_AUX_DIR(.)
AC_CANONICAL_HOST
AC_VALIDATE_CACHED_SYSTEM_TUPLE()

LIBMIDI_MAJOR_VERSION=1
LIBMIDI_MINOR_VERSION=6
LIBMIDI_MICRO_VERSION=0

LIBMIDIPP_MAJOR_VERSION=${LIBMIDI_MAJOR_VERSION}
LIBMIDIPP_MINOR_VERSION=${LIBMIDI_MINOR_VERSION}
LIBMIDIPP_MICRO_VERSION=${LIBMIDI_MICRO_VERSION}

AC_SUBST(LIBMIDIPP_MAJOR_VERSION)
AC_SUBST(LIBMIDIPP_MINOR_VERSION)
AC_SUBST(LIBMIDIPP_MICRO_VERSION)

BETA=

LIBMIDI_VERSION=$LIBMIDI_MAJOR_VERSION.$LIBMIDI_MINOR_VERSION.${LIBMIDI_MICRO_VERSION}${BETA}
LIBMIDI_RELEASE=$LIBMIDI_MAJOR_VERSION-$LIBMIDI_MINOR_VERSION-${LIBMIDI_MICRO_VERSION}${BETA}

AC_SUBST(LIBMIDI_SO_VERSION)
AC_SUBST(LIBMIDI_VERSION)
AC_SUBST(LIBMIDI_RELEASE)

AM_INIT_AUTOMAKE(libmidi++,${LIBMIDI_VERSION})

AM_CONFIG_HEADER(config.h)

dnl alsa_midi=yes
dnl oss_midi=no
dnl solaris_midi=no
dnl irix_midi=yes

dnl have_alsa=no
dnl have_oss=no

dnl ====================================================================
dnl you do have C++, right ?
dnl ====================================================================

AC_PROG_CXX
if test "$ac_cv_prog_cxx" = "no" ; then
   AC_MSG_ERROR([*** libmidi is C++. You don't appear to have a C++ compiler])
fi

CXXFLAGS="-g -D_REENTRANT"
OPT_CXXFLAGS="-D_REENTRANT -O6 -fomit-frame-pointer -ffast-math -fstrength-reduce -funroll-loops -fmove-all-movables"
if test x"$GXX" = xyes ; then
   CXXFLAGS="$CXXFLAGS -Wall"
fi
if test x"$GXX" = xyes ; then
   OPT_CXXFLAGS="$OPT_CXXFLAGS -Wall"
fi

AC_LANG_CPLUSPLUS
AC_OBJEXT
AC_PROG_RANLIB

AM_PATH_MIDI([], [
   AC_MSG_WARN([No MIDI support selected - only NULL or FIFO midi will be available])
])

dnl ==================================================
dnl are controller values float, or 16 bit ints ?
dnl ==================================================

use_float_for_controller_values=yes

AC_ARG_ENABLE(float_controllers,
[  --enable-float-controllers   Use floats for controller values [default=yes]],
if test "$enableval" = "no"; then
   use_float_for_controller_values=no
fi
)

if test "$use_float_for_controller_values" = "yes"; then
   CONTROLLER_VALUES_TYPE=float
else
   CONTROLLER_VALUES_TYPE=unsigned short
fi

AC_SUBST(CONTROLLER_VALUES_TYPE)

dnl Note: for ALSA and OSS we only use the raw MIDI interfaces,
dnl not the API specified by the ALSA library or the OSS
dnl /dev/sequencer device.

if test "$use_alsa_midi" = "yes" ; then
   SYSDEP_HDRS="$SYSDEP_HDRS midi/alsa_rawmidi.h"
   AC_DEFINE([ALSA_RAWMIDI_SUPPORT],,[Define if ALSA Raw MIDI support is available])
   AC_MSG_RESULT([Support for ALSA raw MIDI included.])
fi
if test "$use_oss_midi" = "yes" ; then
   SYSDEP_HDRS="$SYSDEP_HDRS midi/oss_rawmidi.h"
   AC_DEFINE([OSS_RAWMIDI_SUPPORT],,[Define if OSS Raw MIDI support is available])
   AC_MSG_RESULT([Support for OSS raw MIDI included.])
fi

dnl XXX This is wrong. No such variable is ever defined
dnl by AM_PATH_MIDI. Does Solaris have no MIDI support ?    
dnl For now, it doesn't hurt to include the object.

if test "$use_solaris_midi" != "yes" ; then
   SYSDEP_HDRS="$SYSDEP_HDRS midi/solaris_rawmidi.h"
   AC_DEFINE([SOLARIS_RAWMIDI_SUPPORT],,[Define if Solaris Raw MIDI support is available])
   AC_MSG_RESULT([Support for Solaris raw MIDI included.])
fi

if test "$use_irix_midi" = "yes" ; then
   SYSDEP_SRC="$SYSDEP_SRC irix_rawmidi.cc"
   SYSDEP_HDRS="$SYSDEP_HDRS midi/irix_rawmidi.h"
   AC_DEFINE([IRIX_RAWMIDI_SUPPORT],,[Define if IRIX Raw MIDI support is available])
   AC_MSG_RESULT([Support for IRIX raw MIDI included.])
fi

AC_SUBST(SYSDEP_SRC)
AC_SUBST(SYSDEP_HDRS)
SYSDEP_OBJS=`echo $SYSDEP_SRC | sed 's/\.cc/.lo/g'`
AC_SUBST(SYSDEP_OBJS)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(fcntl.h unistd.h)

dnl Checks for typedefs, structures, and compiler characteristics.
dnl AC_C_CONST
dnl AC_TYPE_SIZE_T

dnl Checks for library functions.
AC_TYPE_SIGNAL
AC_CHECK_FUNCS(strdup)

dnl LTLIBOBJS=`echo "$LIBOBJS" | sed 's/\.o/.lo/g'`
dnl AC_SUBST(LTLIBOBJS)

AM_BUILD_ENVIRONMENT

CXXFLAGS="$CXXFLAGS $GUI_INCLUDES"

AC_OUTPUT([Makefile
	version.cc
	midi++/Makefile
        libmidi++.pc
	libmidi++.spec
])




