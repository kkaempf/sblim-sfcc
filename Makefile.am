# $Id: Makefile.am,v 1.5 2005/09/21 13:16:51 mihajlov Exp $

LT_CURRENT=0
LT_REVISION=0
LT_AGE=0

AM_LFLAGS=-i
AM_YFLAGS=-d

BUILT_SOURCES = \
	cimXmlResp.h

noinst_HEADERS = \
	cimXmlParser.h \
	genericlist.h \
	utilStringBuffer.h

pkginclude_HEADERS = \
	utilList.h \
	native.h \
	cimXmlParser.h \
	cmcidt.h \
	cmcift.h \
	cmci.h \
	cmcimacs.h
pkgincludedir = $(includedir)/CimClientLib

lib_LTLIBRARIES=libcmpisfcc.la

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
                   utilStringBuffer.c \
                   genericlist.c \
                   cimXmlResp.y \
                   cimXmlParser.c

libcmpisfcc_la_LIBADD = -lcurl
libcmpisfcc_la_LDFLAGS = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

SUBDIRS = . TEST

AM_CPPFLAGS = -I../../ -I$(top_builddir)/src

#@INC_AMINCLUDE@
