MAINTAINERCLEANFILES = autom4te.cache Makefile.in 

new_p_h = $(wildcard *_p.h)
noinst_HEADERS = $(new_p_h)

#
# doing this as part of maintainer-clean causes all kinds of problems
#

scrub:
	rm -rf $(new_p_h)
