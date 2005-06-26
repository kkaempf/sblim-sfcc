# $Id: Makefile.am,v 1.2 2005/06/26 21:46:02 a3schuur Exp $

AM_LFLAGS=-i
AM_YFLAGS=-d

BUILT_SOURCES=cimXmlResp.h

noinst_HEADERS=cimXmlParser.h cmcidt.h  cmcift.h  cmci.h  cmcimacs.h \
	genericlist.h  native.h  tool.h  utilList.h  utilStringBuffer.h
lib_LTLIBRARIES=libcmpisfcc.la

libcmpisfcc_la_SOURCES=args.c \
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

bin_PROGRAMS=test

test_SOURCES = test.c
test_LDADD = -lcmpisfcc
