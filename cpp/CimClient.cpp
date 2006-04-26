

#include "CimClient.h"

#include "CimEnumeration.h"
#include "CimObjectPath.h"


char *makeMsgds(const char *m, int d1, const char *s1) 
{
   char s[512];
   snprintf(s,512,m,d1,s1);
   return strdup(s);
}

CimClient::CimClient(const char* id) 
{
   int rc;
   char *msg;
   env = NewCimcEnv(id, 0 , &rc, &msg);
   if (env==NULL) 
      throw (CimStatus(CIMC_RC_ERR_FAILED,
          makeMsgds("--- Failed to load cimc environment rc: %d %s",rc,msg)));
}

CimClient::~CimClient() 
{
   if (cc) cc->ft->release(cc);
   ReleaseCimcEnv(env);
}

void CimClient::connect() 
{
  cimcStatus st;
  cc = env->ft->connect(env, "host", NULL, "5988","","",&st);
  if (cc==NULL) throw(CimStatus(st));
}

CimObjectPath CimClient::makeObjectPath(const char *ns, const char *cn) 
{
   cimcStatus st;
   cimcObjectPath *op=env->ft->newObjectPath(env,"root/cimv2", NULL, &st);
   if (st.rc) throw (CimStatus(st));
   return CimObjectPath(op);
}

CimObjectPathEnumeration CimClient::enumerateClassNames(CimObjectPath &op, cimcFlags flags)
{
   cimcStatus st;
   cimcEnumeration *en=cc->ft->enumClassNames(cc,op->getEnc(),flags,&st);
   if (st.rc) throw (CimStatus(st));
   return CimObjectPathEnumeration(en);
}

CimClassEnumeration CimClient::enumerateClasses(CimObjectPath &op, cimcFlags flags)
{
   cimcStatus st;
   cimcEnumeration *en=cc->ft->enumClasses(cc,op->getEnc(),flags,&st);
   if (st.rc) throw (CimStatus(st));
   return CimClassEnumeration(en);
}

