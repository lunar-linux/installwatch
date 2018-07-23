AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = autogen.sh
DISTCLEANFILES =

lib_LTLIBRARIES = installwatch.la
installwatch_la_SOURCES = installwatch.c
installwatch_la_CFLAGS = -Wall -D_REENTRANT $(DEBUG_CFLAGS)
installwatch_la_LDFLAGS = -module -shared -avoid-version
installwatch_la_LIBADD = -ldl -lc


check_PROGRAMS = test-installwatch
test_installwatch_SOURCES = test-installwatch.c
test_installwatch_CFLAGS = -Wall -DVERSION=\"$(VERSION)\" -DLIBDIR=\"$(top_builddir)/.libs\"
test_installwatch_LDADD = -ldl

installwatchdir = $(bindir)
installwatch_SCRIPTS = installwatch
EXTRA_DIST += installwatch.in
DISTCLEANFILES += installwatch

installwatch: installwatch.in
	$(AM_V_GEN) $(SED) -e "s|\@PREFIX\@|$(prefix)|" \
                     -e "s|\@VERSION\@|$(VERSION)|" $< > $@
	$(CHMOD) +x $@

check-local:
	PREFIX=. LIBDIR=$(top_builddir)/.libs ./installwatch ./test-installwatch
