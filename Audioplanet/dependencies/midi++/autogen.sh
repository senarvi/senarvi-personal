#!/bin/sh

cat midi.m4 > acinclude.m4
aclocal $ACLOCAL_FLAGS && autoheader && automake --add-missing && autoconf

