# autoconf tests for SMP system

AC_DEFUN(AC_SYS_SMP,
[
AC_MSG_WARN([
*******************************************************************
This configure script checks for a multiprocessor hardware
configuration.  As a result, the application or library that it is
used to help construct is *NOT* portable.
*******************************************************************
])
AC_MSG_CHECKING([for multiprocessor hardware configuration])
if test -f /proc/cpuinfo ; then
	if test `grep -c '^processor' /proc/cpuinfo` -gt 1 ; then
		ac_cv_sys_smp="yes"
		AC_DEFINE(HAVE_SMP,1,[define if your hardware is a multiprocessor])
	        AC_MSG_RESULT(["yes"]) 
	else 
	        AC_MSG_RESULT(["no"]) 
        fi	  
else
    AC_MSG_RESULT(["no"]) 
    ac_cv_sys_smp="no"
    ifelse([$2], , :, [$2])
fi
])

