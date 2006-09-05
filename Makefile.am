# $Id: Makefile.am,v 1.10 2006/09/05 14:54:59 mihajlov Exp $

Libcmpisfcc_CURRENT=0
Libcmpisfcc_REVISION=0
Libcmpisfcc_AGE=0

LibsfcUtil_CURRENT=0
LibsfcUtil_REVISION=0
LibsfcUtil_AGE=0

LibcimcClient_CURRENT=0
LibcimcClient_REVISION=0
LibcimcClient_AGE=0

AM_LFLAGS=-i
AM_YFLAGS=-d

BUILT_SOURCES = \
	cimXmlResp.h

noinst_HEADERS = \
	cimXmlParser.h \
	genericlist.h 

pkginclude_HEADERS = \
	native.h \
	cimXmlParser.h \
	cmcidt.h \
	cmcift.h \
	cmci.h \
	cmcimacs.h \
	cimcClient.h \
	cimcdt.h \
	cimcfc.h \
	cimcft.h \
	cimcft2.h \
	cimcmacs.h \
	sfcUtil/hashtable.h \
	sfcUtil/utilft.h

pkgincludedir = $(includedir)/CimClientLib

lib_LTLIBRARIES= libsfcUtil.la libcimcClient.la libcmpisfcc.la 

libsfcUtil_la_SOURCES = \
	sfcUtil/hashtable.c \
	sfcUtil/utilFactory.c \
	sfcUtil/utilHashtable.c \
	sfcUtil/utilStringBuffer.c \
        sfcUtil/libsfcUtil.Versions

libcimcClient_la_SOURCES = \
        cimcClient.c \
        libcimcClient.Versions

libcmpisfcc_la_SOURCES = \
                   args.c \
                   array.c \
                   datetime.c \
                   enumeration.c \
                   instance.c \
                   constClass.c \
                   objectpath.c \
                   property.c \
                   qualifier.c \
                   string.c \
                   value.c \
                   client.c \
                   genericlist.c \
                   cimXmlResp.y \
        cimXmlParser.c \
        libcmpisfcc.Versions

libcmpisfcc_la_LIBADD = -lcurl -lsfcUtil
libcmpisfcc_la_DEPENDENCIES = libsfcUtil.la
libcmpisfcc_la_LDFLAGS = \
       -version-info $(Libcmpisfcc_CURRENT):$(Libcmpisfcc_REVISION):$(Libcmpisfcc_AGE) \
       -Wl,--version-script,$(srcdir)/libcmpisfcc.Versions

libsfcUtil_la_LDFLAGS = \
       -version-info $(LibsfcUtil_CURRENT):$(LibsfcUtil_REVISION):$(LibsfcUtil_AGE) \
       -Wl,--version-script,$(srcdir)/sfcUtil/libsfcUtil.Versions

libcimcClient_la_LDFLAGS = \
       -version-info $(LibcimcClient_CURRENT):$(LibcimcClient_REVISION):$(LibcimcClient_AGE) \
       -Wl,--version-script,$(srcdir)/libcimcClient.Versions

SUBDIRS = . TEST

AM_CPPFLAGS = -I../../ -I$(top_builddir)/src -I$(srcdir)/sfcUtil

#@INC_AMINCLUDE@
