# $Id: Makefile.am,v 1.12 2007/02/07 18:16:50 mihajlov Exp $

Libcimcclient_CURRENT=0
Libcimcclient_REVISION=0
Libcimcclient_AGE=0

LibcimcClientXML_CURRENT=0
LibcimcClientXML_REVISION=0
LibcimcClientXML_AGE=0

Libcmpisfcc_CURRENT=1
Libcmpisfcc_REVISION=0
Libcmpisfcc_AGE=0

AM_LFLAGS=-i
AM_YFLAGS=-d

BUILT_SOURCES = \
	cimXmlResp.h cimXmlResp.c

noinst_HEADERS = \
	backend/cimxml/cimXmlParser.h \
	backend/cimxml/genericlist.h \
	backend/cimxml/sfcUtil/hashtable.h \
	backend/cimxml/sfcUtil/utilft.h

cimcinclude_HEADERS = \
	cimc/cimc.h \
	cimc/cimcdt.h \
	cimc/cimcft.h 

legacyinclude_HEADERS = \
	frontend/sfcc/native.h \
	frontend/sfcc/cmcidt.h \
	frontend/sfcc/cmcift.h \
	frontend/sfcc/cmci.h \
	frontend/sfcc/cmcimacs.h 

legacyincludedir = $(includedir)/CimClientLib
cimcincludedir = $(includedir)/cimc

lib_LTLIBRARIES= libcimcclient.la libcmpisfcc.la \
	libcimcClientXML.la 

libcimcclient_la_SOURCES = \
                   cimc/cimcclient.c \
		   cimc/libcimcclient.Versions

libcmpisfcc_la_SOURCES = \
                   frontend/sfcc/sfcclient.c \
		   frontend/sfcc/libcmpisfcc.Versions

libcimcClientXML_la_SOURCES = \
                   backend/cimxml/args.c \
                   backend/cimxml/array.c \
                   backend/cimxml/datetime.c \
                   backend/cimxml/enumeration.c \
                   backend/cimxml/instance.c \
                   backend/cimxml/constClass.c \
                   backend/cimxml/objectpath.c \
                   backend/cimxml/property.c \
                   backend/cimxml/qualifier.c \
                   backend/cimxml/string.c \
                   backend/cimxml/value.c \
                   backend/cimxml/client.c \
                   backend/cimxml/genericlist.c \
                   backend/cimxml/cimXmlResp.y \
	           backend/cimxml/cimXmlParser.c \
		   backend/cimxml/sfcUtil/hashtable.c \
	   	   backend/cimxml/sfcUtil/utilFactory.c \
		   backend/cimxml/sfcUtil/utilHashtable.c \
	           backend/cimxml/sfcUtil/utilStringBuffer.c \
		   backend/cimxml/libcimcClientXML.Versions

libcimcclient_la_LDFLAGS = \
       -version-info $(Libcimcclient_CURRENT):$(Libcimcclient_REVISION):$(Libcimcclient_AGE) \
       -Wl,--version-script,$(srcdir)/cimc/libcimcclient.Versions
libcimcclient_la_LIBADD = -ldl

libcmpisfcc_la_CPPFLAGS = -I$(srcdir)/cimc -I$(srcdir)/backend/cimxml -I$(srcdir)/frontend/sfcc
libcmpisfcc_la_LIBADD = -lcimcclient
libcmpisfcc_la_DEPENDENCIES = libcimcclient.la
libcmpisfcc_la_LDFLAGS = \
       -version-info $(Libcmpisfcc_CURRENT):$(Libcmpisfcc_REVISION):$(Libcmpisfcc_AGE) \
       -Wl,--version-script,$(srcdir)/frontend/sfcc/libcmpisfcc.Versions

libcimcClientXML_la_CPPFLAGS = -I$(srcdir)/backend/cimxml/sfcUtil -I$(srcdir)/backend/cimxml  -I$(srcdir)/frontend/sfcc -I$(srcdir)/cimc
libcimcClientXML_la_LIBADD = -lcurl
libcimcClientXML_la_LDFLAGS = \
       -version-info $(LibcimcClientXML_CURRENT):$(LibcimcClientXML_REVISION):$(LibcimcClientXML_AGE) \
       -Wl,--version-script,$(srcdir)/backend/cimxml/libcimcClientXML.Versions

SUBDIRS = . TEST

#@INC_AMINCLUDE@
