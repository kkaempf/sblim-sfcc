#
# Remote CMPI Makefile
#

include setting.cmpi

CFLAGS    = -Wall -g -DDEBUG
CPPFLAGS  = -I$(CIMOMINC) -I. -Inative

LIBRARIES = libCmpiSfcc.so 
BINARIES  = test

SOURCES.test     = test.c

SOURCES.CmpiSfcc = args.c \
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
                   cimXmlResp.c \
                   cimXmlParser.c

SOURCES  = $(SOURCES.CmpiSfcc) \
           $(SOURCES.test)

OBJECTS  = $(SOURCES:.c=.o)
DEPFILES = $(SOURCES:.c=.dep)

include rules

.PHONY: $(SUBDIRS)

all: $(LIBRARIES) $(BINARIES) $(SUBDIRS)

clean: 
	$(RM) $(OBJECTS) $(BINARIES) $(LIBRARIES)

docs:
	doxygen doxygen.conf

install: all
	install -m 755 libCmpiSfcc.so $(CIMOMLIB)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

cimXmlResp.c: cimXmlResp.y cimXmlParser.h
	/bin/sh ./ylwrap cimXmlResp.y y.tab.c cimXmlResp.c y.tab.h cimXmlResp.h y.output cimXmlResp.output -- bison -y  -d
   
libCmpiSfcc.so: LOADLIBES += -lcurl 
libCmpiSfcc.so: $(SOURCES.CmpiSfcc:.c=.o)
	$(LINK_LIB)

test: LOADLIBES += -lCmpiSfcc -L. -lsfcBrokerCore
test: $(SOURCES.test:.c=.o)
	$(LINK_BIN)

