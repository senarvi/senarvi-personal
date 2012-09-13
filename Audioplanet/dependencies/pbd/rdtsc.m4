# autoconf tests for Pentium-like Cycle Counter

AC_DEFUN(AC_ARCH_RDTSC,
[
AC_REQUIRE([AC_CANONICAL_HOST])
AC_MSG_WARN([
*******************************************************************
This configure script checks for a processor-dependent instruction.
As a result, the application or library that it is used to help
construct is *NOT* portable to systems which might be otherwise
compatible.
*******************************************************************
])
AC_MSG_CHECKING([for the rdtsc instruction])
case "$host_cpu" in
dnl
dnl is this lame or what as a test for a Pentium-like processor ?
dnl
	*86) possible_rdtsc=yes ;;
	*)   possible_rdtsc=no ;;
esac
if test $possible_rdtsc = "yes" ; then
AC_TRY_RUN([
typedef unsigned long long int tsc_t;

tsc_t rdtsc(void)
{
        tsc_t x;
        __asm__ volatile ("rdtsc"
                          : "=A" (x));
        return x;
}

main () 

{
	tsc_t cycles = rdtsc();
	exit (0);
}
], [
    AC_MSG_RESULT(["yes"]) 
    ac_cv_arch_rdtsc="yes"
    ifelse([$1], , :, [$1])
], [
    AC_MSG_RESULT(["no"]) 
    ac_cv_arch_rdtsc="no"
    ifelse([$2], , :, [$2])
], [AC_MSG_WARN([No support for cross-compiling when looking for rdtsc])])
else
    AC_MSG_RESULT(["no"]) 
    ac_cv_arch_rdtsc="no"
    ifelse([$2], , :, [$2])
fi
])

