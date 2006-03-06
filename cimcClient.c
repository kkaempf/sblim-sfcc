
/*
 * cmpic.h
 *
 * (C) Copyright IBM Corp. 2005
 * (C) Copyright Intel Corp. 2005
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * cimc Client function tables.
 *
 */

#include "cimcClient.h"
#include <dlfcn.h>
#include <stdlib.h>

typedef cimcEnv *(*InitCimcEnv) (const char *id, unsigned int options, int *rc, char **msg);

cimcEnv* NewCimcEnv(const char *id, unsigned int options, int *rc, char **msg)
{
   char libName[512];
   char entry[255];
   void *library; 
   InitCimcEnv init=NULL;
   cimcEnv *rv;
   
   sprintf(libName, "libcimcClient%s.so",id);
   
   library = dlopen(libName, RTLD_NOW);
   if (library==NULL) {
      *msg=(char*)malloc(256);
      *rc=1;
      snprintf(*msg,256,"Unable to load %s library %s",libName,dlerror());
   }
   else {
      sprintf(entry, "_Create_%s_Env", id);
      init = (InitCimcEnv)dlsym(library, entry);
      if (init==NULL) {
         *msg=(char*)malloc(512);
         *rc=2;
         snprintf(*msg,512,"Unable to locate entry %s in %s",entry,libName);
      }
      else {
         *rc=0;
         *msg=NULL;
         rv = init(id,options,rc,msg); 
      }
   }
   return rv;
}


int main (int argc, char *argv[])
{
   cimcEnv *cEnv;
   int rc;
   char *msg;
   
   
   cEnv=NewCimcEnv("local",0,&rc,&msg);
   
   if (rc) {
      fprintf(stderr,"--- Unable to load cimc environment rc: %d\n    %s\n",rc,msg);
   }
   return rc;
}
