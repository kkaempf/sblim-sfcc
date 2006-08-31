# $Id: Makefile.am,v 1.9 2006/08/31 12:19:15 mihajlov Exp $

LT_CURRENT=0
LT_REVISION=0
LT_AGE=0

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
	cimcft2.h \
	cimcft.h \
	cimcfc.h \
	cimcmacs.h \
	sfcUtil/utilft.h \
	sfcUtil/hashtable.h

pkgincludedir = $(includedir)/CimClientLib

lib_LTLIBRARIES= \
   libcmpisfcc.la \
   libsfcUtil.la \
   libcimcClient.la

libsfcUtil_la_SOURCES = \
	sfcUtil/hashtable.c \
	sfcUtil/utilFactory.c \
	sfcUtil/utilHashtable.c \
	sfcUtil/utilStringBuffer.c

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
                   genericlist.c \
                   cimXmlResp.y \
                   cimXmlParser.c

libcmpisfcc_la_LIBADD = -lcurl -lsfcUtil
libcmpisfcc_la_DEPENDENCIES = libsfcUtil.la
libcmpisfcc_la_LDFLAGS = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

SUBDIRS = . TEST

AM_CPPFLAGS = -I../../ -I$(top_builddir)/src -I$(srcdir)/sfcUtil

#@INC_AMINCLUDE@
