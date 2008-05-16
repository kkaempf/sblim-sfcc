
/*
 * $Id: cimcclient.c,v 1.3 2008/05/16 22:32:35 mchasal Exp $
 *
 * © Copyright IBM Corp. 2007
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Viktor Mihajlovski <mihajlov@de.ibm.com>
 *
 * Description:
 *
 * CIM C Client Loader Implementation
 *
 */

#include "cimc.h"

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

/*
 * Canonical CIM C API entry points
 */

#define LIBLEN 512
#define ENTLEN 256
#define ERRLEN 1024


CIMCEnv* NewCIMCEnv(const char *id, unsigned int options, int *rc, char **msg)
{
   char         libName[LIBLEN+1];
   char         entry[ENTLEN+1];
   void        *library; 
   InitCimcEnv  init=NULL;
   CIMCEnv     *rv=NULL;
   
    libName[LIBLEN]=0;
    entry[ENTLEN]=0;

    if ((strcmp(id, "XML") != 0) && (strcmp(id, "SfcbLocal") != 0)) {
        *msg=calloc(1,ERRLEN+1);
        *rc=3;
        snprintf(*msg,ERRLEN,"Invalid connection type '%s'. Must be 'XML' or 'SfcbLocal'.",id);
    } else {
        snprintf(libName, LIBLEN, "libcimcClient%s.so",id);
        library = dlopen(libName, RTLD_NOW);
        if (library==NULL) {
            *msg=calloc(1,ERRLEN+1);
            *rc=1;
            snprintf(*msg,ERRLEN,"Unable to load %s library %s",libName,dlerror());
        } else {
            snprintf(entry, ENTLEN,"_Create_%s_Env", id);
            init = (InitCimcEnv)dlsym(library, entry);
            if (init==NULL) {
                *msg=calloc(1,ERRLEN+1);
                *rc=2;
                snprintf(*msg,ERRLEN,"Unable to locate entry %s in %s",entry,libName);
            } else {
                *rc=0;
                *msg=NULL;
                rv = init(id,options,rc,msg); 
                rv->hdl=library;
            }
        }
    }
      
    return rv;
}

void ReleaseCIMCEnv(CIMCEnv *env)
{
   void *lib=NULL;
   if (env) {
     lib = env->hdl;
     env->ft->release(env);
     if (lib) {
       dlclose(lib);
     }
   }
}
