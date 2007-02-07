
#ifndef Sfcc_CimClient_h
#define Sfcc_CimClient_h

#include <cimcClient.h>
#include <cimcmacs.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CimObjectPath.h"
#include "CimEnumeration.h"
#include "CimStatus.h"
#include "CimIterator.h"

//extern "C" cimcEnv *NewCimcEnv(const char *id, unsigned int , int*, char**);

class CimClient {
   cimcEnv *env;
   cimcClient *cc;
  public:
  ~CimClient();
   CimClient(const char* id);

   void connect();

   CimObjectPath makeObjectPath(const char *ns, const char *cn);

   CimObjectPathEnumeration enumerateClassNames(CimObjectPath &op, cimcFlags flags);
   CimClassEnumeration      enumerateClasses(CimObjectPath &op, cimcFlags flags);
   CimInstanceEnumeration   enumerateInstances(CimObjectPath &op, cimcFlags flags);

};

#endif
