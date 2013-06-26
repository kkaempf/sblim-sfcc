#!/bin/sh
set -e

aclocal
autoheader
test `which glibtoolize 2> /dev/null` && glibtoolize || libtoolize
automake -af
autoconf -f
