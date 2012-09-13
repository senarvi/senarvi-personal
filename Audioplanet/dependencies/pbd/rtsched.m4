#
# test for POSIX real-time scheduling support
#

AC_DEFUN(AC_POSIX_RTSCHED, [
	
	AC_MSG_CHECKING([POSIX Real-Time Scheduling support (compile-time)])
	AC_MSG_RESULT([])

	dnl
	dnl Check header to see if _POSIX_PRIORITY_SCHEDULING
	dnl are even defined
	dnl Older Solaris systems (e.g. Solaris 2.5.1/SunOS 5.5.1)
	dnl define _POSIX_THREAD_PRIORITY_SCHEDULING instead.
	dnl

	AC_EGREP_CPP(yes,
	    [ #include <unistd.h>
	      #ifdef _POSIX_PRIORITY_SCHEDULING
		yes
	      #else
	      # ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
		 yes
	      # endif 
	      #endif 
	    ],	
	    [ AC_CHECK_HEADER(sched.h,
			      [ ac_cv_have_posix_rtsched_hdr=yes
	                      ],   
			      [ ac_cv_have_posix_rtsched_hdr=no 
	                      ])
	    ],
	    [ ac_cv_have_posix_rtsched_hdr=no
	      AC_MSG_RESULT([not defined])
 	    ])
	
	if test x"$ac_cv_have_posix_rtsched_hdr" = xyes ; then
 
	    AC_MSG_CHECKING([POSIX Real-Time Scheduling support (run-time)])

	     ac_SLIBS="$LIBS"
	     LIBS=""
	     AC_TRY_LINK(
	        [ #include <unistd.h>
                  #include <sched.h>
	        ],
	        [
			struct sched_param sp;
     	                sched_setscheduler (0, SCHED_FIFO, &sp);
                ],
	    	[ POSIX_RTSCHED_LIBS= 
 	          AC_MSG_RESULT([yes])
                ],
	        [ ac_cv_have_posix_rtsched_hdr=no 
		  AC_MSG_RESULT([no])
                ])
	     LIBS="$ac_SLIBS"

	     dnl
	     dnl Solaris puts the relevant stuff in librt
	     dnl	

	     if test x$ac_cv_have_posix_rtsched_hdr = xno ; then
		     ac_SLIBS="$LIBS"
		     LIBS="-lrt"
		     AC_MSG_CHECKING([Solaris-style RT library])
		     AC_TRY_LINK(
		        [ #include <unistd.h>
	                  #include <sched.h>
		        ],
		        [
				struct sched_param sp;
	     	                sched_setscheduler (0, SCHED_FIFO, &sp);
	                ],
		    	[ POSIX_RTSCHED_LIBS=-lrt
 	          	  ac_cv_have_posix_rtsched_hdr=yes 
		  	  AC_MSG_RESULT([yes])
	                ],
		        [ AC_MSG_RESULT([no])
	                ])
     	             LIBS="$ac_SLIBS"
             fi

	     dnl
	     dnl Older Solaris systems put them in libposix4
	     dnl	

	     if test x$ac_cv_have_posix_rtsched_hdr = xno ; then
		     ac_SLIBS="$LIBS"
		     LIBS="-lposix4"
		     AC_MSG_CHECKING([Older Solaris-style POSIX4 library])
		     AC_TRY_LINK(
		        [ #include <unistd.h>
	                  #include <sched.h>
		        ],
		        [
				struct sched_param sp;
	     	                sched_setscheduler (0, SCHED_FIFO, &sp);
	                ],
		    	[ POSIX_RTSCHED_LIBS=-lposix4
 	          	  ac_cv_have_posix_rtsched_hdr=yes 
		  	  AC_MSG_RESULT([yes])
	                ],
		        [ AC_MSG_RESULT([no])
	                ])
     	             LIBS="$ac_SLIBS"
             fi
	fi

	dnl test this again in case it wasn't in a library

	if test x$ac_cv_have_posix_rtsched_hdr = xyes ; then
	    ifelse([$1], , :, [$1]);
	else 
	    AC_MSG_RESULT([no])
	    ifelse([$2], , :, [$2]);
        fi
])
  
