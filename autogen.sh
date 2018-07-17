#!/bin/sh
aclocal -I m4 && \
autoheader && \
libtoolize && \
autoconf && \
automake --add-missing --force-missing --copy
