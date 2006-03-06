# $Id: Makefile.am,v 1.6 2006/03/06 16:08:05 a3schuur Exp $

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
	cmcimacs.h \
	cimcClient.h \
	cimcdt.h \
	cimcft2.h \
	cimcft.h \
	cimcmacs.h
	
pkgincludedir = $(includedir)/CimClientLib

lib_LTLIBRARIES=libcmpisfcc.la \
   libcimcClient.la 

libcimcClient_la_SOURCES = \
   cimcClient.c
   
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
