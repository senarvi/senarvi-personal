dnl Process this file with autoconf to produce a configure script.
AC_INIT(choice.cc)

AC_CONFIG_AUX_DIR(.)
AC_CANONICAL_HOST
AC_VALIDATE_CACHED_SYSTEM_TUPLE()

LIBGTKMMEXT_MAJOR_VERSION=2
LIBGTKMMEXT_MINOR_VERSION=23
LIBGTKMMEXT_MICRO_VERSION=0

AC_SUBST(LIBGTKMMEXT_MAJOR_VERSION)
AC_SUBST(LIBGTKMMEXT_MINOR_VERSION)
AC_SUBST(LIBGTKMMEXT_MICRO_VERSION)

BETA=

LIBGTKMMEXT_VERSION=$LIBGTKMMEXT_MAJOR_VERSION.$LIBGTKMMEXT_MINOR_VERSION.${LIBGTKMMEXT_MICRO_VERSION}${BETA}
LIBGTKMMEXT_RELEASE=$LIBGTKMMEXT_MAJOR_VERSION-$LIBGTKMMEXT_MINOR_VERSION-${LIBGTKMMEXT_MICRO_VERSION}${BETA}

AC_SUBST(LIBGTKMMEXT_SO_VERSION)
AC_SUBST(LIBGTKMMEXT_VERSION)
AC_SUBST(LIBGTKMMEXT_RELEASE)

AM_INIT_AUTOMAKE(libgtkmmext,${LIBGTKMMEXT_VERSION})
AM_CONFIG_HEADER(config.h)

dnl Checks for programs.

dnl ====================================================================
dnl you do have C++, right ?
dnl ====================================================================

AC_PROG_CC
AC_PROG_CXX
if test "$ac_cv_prog_cxx" = "no" ; then
   AC_MSG_ERROR([*** libmidi is C++. You don't appear to have a C++ compiler])
fi

CXXFLAGS="-g -D_REENTRANT"
OPT_CXXFLAGS="-D_REENTRANT -O6 -fomit-frame-pointer -ffast-math -fstrength-reduce -funroll-loops -fmove-all-movables"
CFLAGS="-g -D_REENTRANT"
OPT_CFLAGS="-D_REENTRANT -O6 -fomit-frame-pointer -ffast-math -fstrength-reduce -funroll-loops -fmove-all-movables"
if test x"$GXX" = xyes ; then
   CXXFLAGS="$CXXFLAGS -Wall"
fi
if test x"$GXX" = xyes ; then
   OPT_CXXFLAGS="$OPT_CXXFLAGS -Wall"
fi

AC_ARG_ENABLE(optimize,
     [  --enable-optimize       ask the compiler for its best optimizations.],
     [ if test "x$enable_optimize" != "xno" ; then 
	  CXXFLAGS="$OPT_CXXFLAGS" ;
	  CFLAGS="$OPT_CFLAGS" ; fi 
     ])

AC_OBJEXT
AC_PROG_RANLIB

dnl Checks for libraries.

AM_PATH_GLIB(1.0.0, , [
   AC_MSG_ERROR([*** libgtkmmext requires glib, but it doesn't appear to be installed])
])

AM_PATH_GTK(1.0.0, , [
   AC_MSG_ERROR([*** libgtkmmext requires GTK+, but it doesn't appear to be installed])
])

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h)

dnl Checks for typedefs, structures, and compiler characteristics.
dnl AC_C_CONST
dnl AC_TYPE_SIZE_T

dnl Checks for library functions.
AC_FUNC_VPRINTF

AM_BUILD_ENVIRONMENT

CXXFLAGS="$CXXFLAGS $GUI_INCLUDES $GLIB_CFLAGS $GTK_CFLAGS"
CFLAGS="$CFLAGS $GUI_INCLUDES $GLIB_CFLAGS $GTK_CFLAGS"

AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)

AC_OUTPUT([Makefile
	version.cc
	gtkmmext/Makefile
	gtkmmext/private/Makefile
	libgtkmmext.pc
	libgtkmmext.spec
])

