# $Id: Makefile.am,v 1.5 2006/09/05 14:55:00 mihajlov Exp $

docdir=$(datadir)/doc/$(PACKAGE)-$(VERSION)

lib_LTLIBRARIES=libcimcCppImpl.la

nobase_include_HEADERS =
  
noinst_PROGRAMS	= sfccTest

libcimcCppImpl_la_SOURCES = \
    sfccPtr.cpp \
    sfccSPtr.cpp \
    CimEnumeration.cpp \
    CimIterator.cpp \
    CimClient.cpp \
    CimStatus.cpp \
    CimString.cpp \
    CimClass.cpp \
    CimInstance.cpp \
    CimObjectPath.cpp \
    CimData.cpp \
    CimDateTime.cpp \
    CimArray.cpp

libcimcCppImpl_la_CPPFLAGS = -Wall -I$(srcdir)/.. -I.

sfccTest_SOURCES = sfccTest.cpp 
sfccTest_LDADD =  libcimcCppImpl.la ../libcimcClient.la
sfccTest_CPPFLAGS = -Wall -I$(srcdir)/.. -I.

EXTRA_DIST=$(PACKAGE).spec

install-data-local: 
	test -d $(DESTDIR)$(docdir) || $(mkdir_p) $(DESTDIR)$(docdir)
	$(INSTALL_DATA) $(srcdir)/README $(srcdir)/AUTHORS $(srcdir)/COPYING $(DESTDIR)$(docdir)


uninstall-local:
	rm -rf $(DESTDIR)$(docdir)
