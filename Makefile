# Makefile for installwatch
# $Id: Makefile,v 0.7.0.6 2008/11/09 07:49:34 izto Exp $

# Well, the only configurable part is the following variable.
# Make sure the directory you specify exists.

PREFIX ?= /usr/local

# End of configurable part

VERSION=0.7.0-lunar-1

CFLAGS ?=
BINDIR=$(PREFIX)/bin
LIBDIR=$(PREFIX)/lib

all: installwatch.so

installwatch.so: installwatch.o
	ld -shared -o installwatch.so installwatch.o -ldl -lc

installwatch.o: installwatch.c localdecls.h
	gcc $(CFLAGS) -Wall -c -D_GNU_SOURCE -DPIC -fPIC -D_REENTRANT -DVERSION=\"$(VERSION)\" installwatch.c

localdecls.h: create-localdecls
	./create-localdecls

install: all
	mkdir -p $(LIBDIR)
	mkdir -p $(BINDIR)
	if [ -r $(LIBDIR)/installwatch.so ]; then \
		rm -f  $(LIBDIR)/installwatch.so; \
	fi
	install installwatch.so $(LIBDIR)

	sed -e "s|#PREFIX#|$(PREFIX)|" < installwatch > $(BINDIR)/installwatch
	chmod 755 $(BINDIR)/installwatch

uninstall:
	rm -f $(LIBDIR)/installwatch.so
	rm -f $(BINDIR)/installwatch

clean:
	rm -f *~ *.bak *.o installwatch.so core localdecls.h libctest test-installwatch

tag:
	git tag v$(VERSION)

dist:
	git archive --format=tar --prefix="installwatch-$(VERSION)/" v$(VERSION) | xz > installwatch-$(VERSION).tar.xz

test: installwatch.so
	gcc -DLIBDIR=\".\" -Wall -DVERSION=\"$(VERSION)\" -o test-installwatch test-installwatch.c -ldl
	PREFIX=. LIBDIR=. ./installwatch ./test-installwatch
