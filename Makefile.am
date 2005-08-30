# $Id: Makefile.am,v 1.3 2005/08/30 11:32:12 a3schuur Exp $

AM_LFLAGS=-i
AM_YFLAGS=-d

BUILT_SOURCES = \
	cimXmlResp.h

noinst_HEADERS = \
	cimXmlParser.h \
	genericlist.h \
	tool.h \
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

#SUBDIRS = . TEST

AM_CPPFLAGS = -I../../ -I$(top_builddir)/src
