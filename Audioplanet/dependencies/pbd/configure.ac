dnl Process this file with autoconf to produce a configure script.
AC_INIT(pbd)

AC_CONFIG_AUX_DIR(.)
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AC_VALIDATE_CACHED_SYSTEM_TUPLE()

LIBPBD_MAJOR_VERSION=1
LIBPBD_MINOR_VERSION=17
LIBPBD_MICRO_VERSION=2

AC_SUBST(LIBPBD_MAJOR_VERSION)
AC_SUBST(LIBPBD_MINOR_VERSION)
AC_SUBST(LIBPBD_MICRO_VERSION)

BETA=

LIBPBD_VERSION=$LIBPBD_MAJOR_VERSION.$LIBPBD_MINOR_VERSION.${LIBPBD_MICRO_VERSION}${BETA}
LIBPBD_RELEASE=$LIBPBD_MAJOR_VERSION-$LIBPBD_MINOR_VERSION-${LIBPBD_MICRO_VERSION}${BETA}

AC_SUBST(LIBPBD_SO_VERSION)
AC_SUBST(LIBPBD_VERSION)
AC_SUBST(LIBPBD_RELEASE)

AM_INIT_AUTOMAKE(libpbd,${LIBPBD_VERSION})

AM_CONFIG_HEADER(config.h)

dnl =======================
dnl Platform-specific stuff
dnl =======================

dnl -----------------------------------------------------------
dnl (1): system-dependent header files needed to allow
dnl      quasimodo source code to not have #ifdef SYSTYPE junk
dnl -----------------------------------------------------------

case "$target_os" in

	linux*)
		SYSDEP_SRC="$SYSDEP_SRC linux_platform.cc";
		AC_DEFINE(LINUX_PLATFORM_SUPPORT,,[define if your platform is linux])
		;;

	irix*)
		SYSDEP_SRC="$SYSDEP_SRC irix_platform.cc";
		AC_DEFINE(IRIX_PLATFORM_SUPPORT,,[define if your platform is irix])
		;;

	solaris*)
		SYSDEP_SRC="$SYSDEP_SRC solaris_platform.cc";
		AC_DEFINE(SOLARIS_PLATFORM_SUPPORT,,[define if your platform is solaris])
		;;

        *) AC_MSG_ERROR([
=============================================================================
Platform-specific parts of libpbd have not yet been ported to $target_os.
=============================================================================
	        ])
	        ;;
esac

SYSDEP_OBJS=`echo $SYSDEP_SRC | sed 's/\.cc/.o/g'`
AC_SUBST(SYSDEP_OBJS)
AC_SUBST(SYSDEP_SRC)

dnl Checks for programs.

AC_PROG_CXX
if test "$ac_cv_prog_cxx" = "no" ; then
   AC_MSG_ERROR([*** libpbd is C++. You don't appear to have a C++ compiler])
fi

CXXFLAGS="-g -D_REENTRANT"
OPT_CXXFLAGS="-D_REENTRANT -O6 -fomit-frame-pointer -ffast-math -fstrength-reduce -funroll-loops -fmove-all-movables"
if test x"$GXX" = xyes ; then
   CXXFLAGS="$CXXFLAGS -Wall"
fi
if test x"$GXX" = xyes ; then
   OPT_CXXFLAGS="$OPT_CXXFLAGS -Wall"
fi

AC_ARG_ENABLE(optimize,
     [  --enable-optimize       ask the compiler for its best optimizations.],
     [ if test "x$enable_optimize" != "xno" ; then CXXFLAGS="$OPT_CXXFLAGS" ; fi ])

AC_OBJEXT
AC_PROG_RANLIB

AM_PATH_GLIB(1.0.0, , 
   [ AC_MSG_ERROR([*** libpbd requires glib, but it doesn't appear to be installed])])

AC_LANG_CPLUSPLUS

dnl Checks for libraries.

dnl Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS(fcntl.h limits.h sys/time.h unistd.h)

dnl Checks for typedefs, structures, and compiler characteristics.
dnl AC_C_CONST
AC_TYPE_OFF_T
AC_TYPE_PID_T
dnl AC_TYPE_SIZE_T
AC_HEADER_TIME

dnl Checks for library functions.
AC_FUNC_MEMCMP
AC_TYPE_SIGNAL
AC_FUNC_VPRINTF
AC_CHECK_FUNCS(regcomp select strdup strerror strtol)
AC_CHECK_LIB(pthread, pthread_create, ,[AC_MSG_ERROR([you have no POSIX thread support])])

AC_POSIX_RTSCHED
AC_SUBST(POSIX_RTSCHED_LIBS)

dnl ==================================================
dnl Check for architecture specific functions
dnl ==================================================

AC_ARCH_RDTSC([AC_DEFINE(HAVE_RDTSC,,[define if your processor supports the `rdtsc' instruction])])
AC_SYS_SMP([AC_DEFINE(HAVE_SMP,,[define if the intended hardware configuration is SMP])])

AM_BUILD_ENVIRONMENT

CFLAGS="$CFLAGS $GLIB_CFLAGS"
CXXFLAGS="$CXXFLAGS $GLIB_CFLAGS"
LIBS="$LIBS $GLIB_LIBS $POSIX_RTSCHED_LIBS"

AC_OUTPUT([Makefile
           version.cc
           pbd/Makefile
])
