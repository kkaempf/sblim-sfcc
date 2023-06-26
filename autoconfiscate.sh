#!/bin/sh
set -e

autoupdate || true
aclocal
autoheader
test `which glibtoolize 2> /dev/null` && glibtoolize || libtoolize
automake -af
autoconf
