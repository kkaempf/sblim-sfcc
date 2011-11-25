#!/bin/sh
set -e

aclocal
autoheader
test -x `which glibtoolize` && glibtoolize || libtoolize
automake -af
autoconf
