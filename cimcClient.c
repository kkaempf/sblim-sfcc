
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
   cimcEnv *rv=NULL;
   
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
   rv->hdl=library;
   return rv;
}

void ReleaseCimcEnv(cimcEnv *env)
{
   void *lib=NULL;
   if (env) lib=env->ft->release(env);
//   if (lib) dlclose(lib);
}


#ifdef mmmmmmmmmm

cimcStatus cimcClsRelease (cimcConstClass* ccls)
{
   return ccls->ft->release(ccls);
}

cimcConstClass* cimcClsClone(cimcConstClass* ccls, cimcStatus* rc)
{
   return ccls->ft->clone(ccls,rc);
}


cimcString* cimcClsGetClassName(cimcConstClass * ccls, cimcStatus * rc )
{
   return ccls->ft->getClassName(ccls,rc);
}

cimcData cimcClsGetProperty(cimcConstClass* ccls, const char *name, cimcStatus* rc)
{
   return ccls->ft->getProperty(ccls,name,rc);
}


cimcData cimcClsGetPropertyAt(cimcConstClass* ccls, unsigned int index, cimcString** name,
	       cimcStatus* rc)
{
   return ccls->ft->getPropertyAt(ccls,index,name,rc);
}

unsigned int cimcClsGetPropertyCount(cimcConstClass* ccls, cimcStatus* rc)
{
   return ccls->ft->getPropertyCount(ccls,rc);
}

cimcData cimcClsGetQualifier(cimcConstClass * ccls,const char *name, cimcStatus* rc)
{
   return ccls->ft->getQualifier(ccls,name,rc);
}

cimcData cimcClsGetQualifierAt(cimcConstClass * ccls,unsigned int index, cimcString** name, cimcStatus* rc)
{
   return ccls->ft->getQualifierAt(ccls,index,name,rc);
}

unsigned int cimcClsGetQualifierCount(cimcConstClass * ccls, cimcStatus* rc)
{
   return ccls->ft->getQualifierCount(ccls,rc);
}

cimcData cimcClsGetPropertyQualifier(cimcConstClass * ccls, const char *pname, const char *qname, 
          cimcStatus* rc)
{
   return ccls->ft->getPropertyQualifier(ccls,pname,qname,rc);
}

cimcData cimcClsGetPropertyQualifierAt(cimcConstClass * ccls, const char *pname, unsigned int index,
          cimcString** name, cimcStatus* rc)
{
   return ccls->ft->getPropertyQualifierAt(ccls,pname,index,name,rc);
}

unsigned int cimcClsGetPropertyQualifierCount(cimcConstClass * ccls, const char *pname, cimcStatus* rc)
{
   return ccls->ft->gtPropertyQualifierCount(ccls,pname,rc);
}


#else 

cimcStatus cimcClsRelease(cimcConstClass * ccls)
{
   return ccls->ft->release(ccls);
}

cimcConstClass *cimcClsClone(cimcConstClass * ccls, cimcStatus * rc)
{
   return ccls->ft->clone(ccls,rc);
}

cimcString *cimcClsGetClassName(cimcConstClass* ccls, cimcStatus * rc)
{
   return ccls->ft->getClassName(ccls,rc);
}

cimcString *cimcClsGetSuperClassName(cimcConstClass* ccls, cimcStatus * rc)
{
   return ccls->ft->getSuperClassName(ccls,rc);
}

 
cimcData cimcClsGetProperty(cimcConstClass * ccls, const char *prop, cimcStatus * rc)
{
   return ccls->ft->getProperty(ccls,prop,rc);
}

cimcData cimcClsGetPropertyAt(cimcConstClass * ccls, cimcCount i,
                          cimcString ** name,  cimcStatus * rc)
{
   return ccls->ft->getPropertyAt(ccls,i,name,rc);
}

cimcCount cimcClsGetPropertyCount(cimcConstClass * ccls, cimcStatus * rc)
{
   return ccls->ft->getPropertyCount(ccls,rc);
}
 
cimcData cimcClsGetQualifier(cimcConstClass * ccls, const char *qual, cimcStatus * rc)
{
   return ccls->ft->getQualifier(ccls,qual,rc);
}

cimcData cimcClsGetQualifierAt(cimcConstClass * ccls, cimcCount i,
                              cimcString ** name, cimcStatus * rc)
{
   return ccls->ft->getQualifierAt(ccls,i,name,rc);
}

cimcCount cimcClsGetQualifierCount(cimcConstClass * ccls, cimcStatus * rc)
{
   return ccls->ft->getQualifierCount(ccls,rc);
}


cimcData cimcClsGetPropQualifier(cimcConstClass * ccls, const char *prop,
                               const char *qual, cimcStatus * rc)
{
   return ccls->ft->getPropQualifier(ccls,prop,qual,rc);
}

cimcData cimcClsGetPropQualifierAt(cimcConstClass * ccls, const char *prop,
               cimcCount i, cimcString ** name,cimcStatus * rc)
{
   return ccls->ft->getPropQualifierAt(ccls,prop,i,name,rc);
}

cimcCount cimcClsGetPropQualifierCount(cimcConstClass * ccls, const char * prop, cimcStatus * rc)
{
   return ccls->ft->getPropQualifierCount(ccls,prop,rc);
}
 
cimcArray *cimcClsGetKeyList(cimcConstClass * ccls)
{
   return ccls->ft->getKeyList(ccls);
}

char *cimcClsToString(cimcConstClass * ccls)
{
   return ccls->ft->toString(ccls);
}

void cimcClsRelocate(cimcConstClass * ccls)
{
   return ccls->ft->relocate(ccls);
}

const char *cimcClsGetCharClassName(cimcConstClass * ccls)
{
   return ccls->ft->getCharClassName(ccls);
}

const char *cimcClsGetCharSuperClassName(cimcConstClass * ccls)
{
   return ccls->ft->getCharSuperClassName(ccls);
}

cimcBoolean cimcClsIsAssociation(cimcConstClass * ccls)
{
   return ccls->ft->isAssociation(ccls);
}

cimcBoolean cimcClsIsAbstract(cimcConstClass * ccls)
{
   return ccls->ft->isAbstract(ccls);
}

cimcBoolean cimcClsIsIndication(cimcConstClass * ccls)
{
   return ccls->ft->isIndication(ccls);
}

cimcData cimcClsGetPropQualAt(cimcConstClass * ccls, cimcCount p, cimcCount i, cimcString ** name,
           cimcStatus * rc)
{
   return ccls->ft->getPropQualAt(ccls,p,i,name,rc);
}

#endif



   //---------------------------------------------------
   //--
   //	 cimcInstance Function Calls
   //--
   //---------------------------------------------------


cimcStatus cimcInstRelease(cimcInstance* inst)
{
   return inst->ft->release(inst);
}

cimcInstance* cimcInstClone(cimcInstance* inst, cimcStatus* rc)
{
   return inst->ft->clone(inst,rc);
}

cimcData cimcInstGetProperty(cimcInstance* inst, const char *name, cimcStatus* rc)
{
   return inst->ft->getProperty(inst,name,rc);
}

cimcData cimcInstGetPropertyAt(cimcInstance* inst, unsigned int index, cimcString** name,
	       cimcStatus* rc)
{
   return inst->ft->getPropertyAt(inst,index,name,rc);
}

unsigned int cimcInstGetPropertyCount(cimcInstance* inst, cimcStatus* rc)
{
   return inst->ft->getPropertyCount(inst,rc);
}

cimcStatus cimcInstSetProperty(cimcInstance* inst, const char *name,
               void* value, cimcType type)
{
   return inst->ft->setProperty(inst,name,value,type);
}

cimcObjectPath* cimcInstGetObjectPath(cimcInstance* inst, cimcStatus* rc)
{
   return inst->ft->getObjectPath(inst,rc);
}

cimcStatus cimcInstSetPropertyFilter(cimcInstance* inst, char **propertyList, char **keys)
{
   return inst->ft->setPropertyFilter(inst,propertyList,keys);
}

cimcData cimcInstGetQualifier(cimcInstance* inst, const char *name, cimcStatus* rc)
{
   return inst->ft->getQualifier(inst,name,rc);
}

cimcData cimcInstGetQualifierAt(cimcInstance* inst, unsigned int index, cimcString** name,
      cimcStatus* rc)
{
   return inst->ft->getQualifierAt(inst,index,name,rc);
}

unsigned int cimcInstGetQualifierCount(cimcInstance* inst, cimcStatus* rc)
{
   return inst->ft->getQualifierCount(inst,rc);
}

cimcData cimcInstGetPropertyQualifier(cimcInstance* inst, const char *pname, const char *qname, 
          cimcStatus* rc)
{
   return inst->ft->getPropertyQualifier(inst,pname,qname,rc);
}

cimcData cimcInstGetPropertyQualifierAt(cimcInstance* inst, const char *pname, unsigned int index, 
          cimcString** name, cimcStatus* rc)
{
   return inst->ft->getPropertyQualifierAt(inst,pname,index,name,rc);
}

unsigned int cimcInstGetPropertyQualifierCount(cimcInstance* inst, const char *pname, cimcStatus* rc)
{
   return inst->ft->getPropertyQualifierCount(inst,pname,rc);
}




   //---------------------------------------------------
   //--
   //	 cimcObjectPath Function Calls
   //--
   //---------------------------------------------------


cimcStatus cimcOpRelease(cimcObjectPath* op)
{
   return op->ft->release(op);
}

cimcObjectPath* cimcOpClone(cimcObjectPath* op, cimcStatus* rc)
{
   return op->ft->clone(op,rc);
}

cimcStatus cimcOpSetNameSpace(cimcObjectPath* op, const char *ns)
{
   return op->ft->setNameSpace(op,ns);
}

cimcString* cimcOpGetNameSpace(cimcObjectPath* op, cimcStatus* rc)
{
   return op->ft->getNameSpace(op,rc);
}

cimcStatus cimcOpSetHostname(cimcObjectPath* op, const char *hn)
{
   return op->ft->setHostname(op,hn);
}

cimcString* cimcOpGetHostname(cimcObjectPath* op, cimcStatus* rc)
{
   return op->ft->getHostname(op,rc);
}

cimcStatus cimcOpSetClassName(cimcObjectPath* op, const char *cn)
{
   return op->ft->setClassName(op,cn);
}

cimcString* cimcOpGetClassName(cimcObjectPath* op, cimcStatus* rc)
{
   return op->ft->getClassName(op,rc);
}

cimcStatus cimcOpAddKey(cimcObjectPath* op, const char *name, void* value, cimcType type)
{
   return op->ft->addKey(op,name,value,type);
}

cimcData cimcOpGetKey(cimcObjectPath* op, const char *name, cimcStatus* rc)
{
   return op->ft->getKey(op,name,rc);
}

cimcData cimcOpGetKeyAt(cimcObjectPath* op, unsigned int index, cimcString** name,
	       cimcStatus* rc)
{
   return op->ft->getKeyAt(op,index,name,rc);
}

unsigned int cimcOpGetKeyCount(cimcObjectPath* op, cimcStatus* rc)
{
   return op->ft->getKeyCount(op,rc);
}

cimcStatus cimcOpSetNameSpaceFromObjectPath(cimcObjectPath* op, cimcObjectPath* src)
{
   return op->ft->setNameSpaceFromObjectPath(op,src);
}

cimcStatus cimcOpSetHostAndNameSpaceFromObjectPath(cimcObjectPath* op, cimcObjectPath* src)
{
   return op->ft->setHostAndNameSpaceFromObjectPath(op,src);
}

cimcString *cimcOpToString(cimcObjectPath* op, cimcStatus *rc)
{
   return op->ft->toString(op,rc);
}






   //---------------------------------------------------
   //--
   //	 cimcArgs Function Calls
   //--
   //---------------------------------------------------


cimcStatus cimcArgsRelease(cimcArgs* as)
{
   return as->ft->release(as);
}

cimcArgs* cimcArgsClone(cimcArgs* as, cimcStatus* rc)
{
   return as->ft->clone(as,rc);
}

cimcStatus cimcArgsAddArg(cimcArgs* as, const char *name, void* value, cimcType type)
{
   return as->ft->addArg(as,name,value,type);
}

cimcData cimcArgsGetArg(cimcArgs* as, const char *name, cimcStatus* rc)
{
   return as->ft->getArg(as,name,rc);
}

cimcData cimcArgsGetArgAt(cimcArgs* as, unsigned int index, cimcString** name,
       cimcStatus* rc)
{
   return as->ft->getArgAt(as,index,name,rc);
}

unsigned int cimcArgsGetArgCount(cimcArgs* as, cimcStatus* rc)
{
   return as->ft->getArgCount(as,rc);
}



   //---------------------------------------------------
   //--
   //	 cimcString Function Calls
   //--
   //---------------------------------------------------


cimcStatus cimcStrRelease(cimcString* st)
{
   return st->ft->release(st);
}

cimcString* cimcStrClone(cimcString* st, cimcStatus* rc)
{
   return st->ft->clone(st,rc);
}


char *cimcStrGetCharPtr(cimcString* st, cimcStatus* rc)
{
   return st->ft->getCharPtr(st,rc);
}






   //---------------------------------------------------
   //--
   //	 cimcArray Function Calls
   //--
   //---------------------------------------------------



cimcStatus cimcArrRelease(cimcArray* ar)
{
   return ar->ft->release(ar);
} 

cimcArray* cimcArrClone(cimcArray* ar, cimcStatus* rc)
{
   return ar->ft->clone(ar,rc);
} 

cimcCount cimcArrGetSize(cimcArray* ar, cimcStatus* rc)
{
   return ar->ft->getSize(ar,rc);
} 

cimcType cimcArrGetSimpleType(cimcArray* ar, cimcStatus* rc)
{
   return ar->ft->getSimpleType(ar,rc);
} 

cimcData cimcArrGetElementAt(cimcArray* ar, cimcCount index, cimcStatus* rc)
{
   return ar->ft->getElementAt(ar,index,rc);
} 

cimcStatus cimcArrSetElementAt(cimcArray* ar, cimcCount index, void* value, cimcType type)
{
   return ar->ft->setElementAt(ar,index,value,type);
} 





   //---------------------------------------------------
   //--
   //	_cimcEnumeration Function Calls
   //--
   //---------------------------------------------------



cimcStatus cimcEnmRelease(cimcEnumeration* en)
{
   return en->ft->release(en);
}

cimcEnumeration* cimcEnmClone(cimcEnumeration* en, cimcStatus* rc)
{
   return en->ft->clone(en,rc);
}

cimcData cimcEnmGetNext(cimcEnumeration* en, cimcStatus* rc)
{
   return en->ft->getNext(en,rc);
}

cimcBoolean cimcEnmHasNext(cimcEnumeration* en, cimcStatus* rc)
{
   return en->ft->hasNext(en,rc);
}

cimcArray* cimcEnmToArray(cimcEnumeration* en, cimcStatus* rc)
{
   return en->ft->toArray(en,rc);
}





   //---------------------------------------------------
   //--
   //	_cimcDateTime Function Calls
   //--
   //---------------------------------------------------



cimcStatus cimcDtRelease(cimcDateTime* dt)
{
   return dt->ft->release(dt);
}

cimcDateTime* cimcDtClone(cimcDateTime* dt, cimcStatus* rc)
{
   return dt->ft->clone(dt,rc);
}

cimcUint64 cimcDtGetBinaryFormat(cimcDateTime* dt, cimcStatus* rc)
{
   return dt->ft->getBinaryFormat(dt,rc);
}

cimcString* cimcDtGetStringFormat(cimcDateTime* dt, cimcStatus* rc)
{
   return dt->ft->getStringFormat(dt,rc);
}

cimcBoolean cimcDtIsInterval(cimcDateTime* dt, cimcStatus* rc)
{
   return dt->ft->isInterval(dt,rc);
}





   //---------------------------------------------------
   //--
   //	_cimcClient Function Calls
   //--
   //---------------------------------------------------



cimcStatus cimcClntRelease(cimcClient* cl)
{
   return cl->ft->release(cl);
}

cimcClient *cimcClntClone(cimcClient * cl, cimcStatus * rc)
{
   return cl->ft->clone(cl,rc);
}
cimcConstClass* cimcClntGetClass(cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, char** properties, cimcStatus* rc)
{
   return cl->ft->getClass(cl,op,flags,properties,rc);
}

cimcEnumeration* cimcClntEnumClassNames(cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, cimcStatus* rc)
{
   return cl->ft->enumClassNames(cl,op,flags,rc);
}

cimcEnumeration* cimcClntEnumClasses(cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, cimcStatus* rc)
{
   return cl->ft->enumClasses(cl,op,flags,rc);
}

cimcInstance* cimcClntGetInstance(cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, char** properties, cimcStatus* rc)
{
   return cl->ft->getInstance(cl,op,flags,properties,rc);
}

cimcObjectPath* cimcClntCreateInstance(cimcClient* cl,
                 cimcObjectPath* op, cimcInstance* inst, cimcStatus* rc)
{
   return cl->ft->createInstance(cl,op,inst,rc);
}

cimcStatus cimcClntSetInstance(cimcClient* cl,
                 cimcObjectPath* op, cimcInstance* inst, cimcFlags flags, char ** properties)
{
   return cl->ft->setInstance(cl,op,inst,flags,properties);
}

cimcStatus cimcClntDeleteInstance(cimcClient* cl,
                 cimcObjectPath* op)
{
   return cl->ft->deleteInstance(cl,op);
}

cimcEnumeration* cimcClntExecQuery(cimcClient* cl,
                 cimcObjectPath* op, const char *query, const char *lang, cimcStatus* rc)
{
   return cl->ft->execQuery(cl,op,query,lang,rc);
}

cimcEnumeration* cimcClntEnumInstanceNames(cimcClient* cl,
                 cimcObjectPath* op, cimcStatus* rc)
{
   return cl->ft->enumInstanceNames(cl,op,rc);
}

cimcEnumeration* cimcClntEnumInstances(cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, char** properties, cimcStatus* rc)
{
   return cl->ft->enumInstances(cl,op,flags,properties,rc);
}

cimcEnumeration* cimcClntAssociators(cimcClient* cl,
                 cimcObjectPath* op, const char *assocClass, const char *resultClass,
                 const char *role, const char *resultRole, cimcFlags flags, 
                 char** properties, cimcStatus* rc)
{
   return cl->ft->associators(cl,op,assocClass,resultClass,role,resultRole,flags,properties,rc);
}

cimcEnumeration* cimcClntAssociatorNames(cimcClient* cl,
                 cimcObjectPath* op, const char *assocClass, const char *resultClass,
                 const char *role, const char *resultRole, cimcStatus* rc)
{
   return cl->ft->associatorNames(cl,op,assocClass,resultClass,role,resultRole,rc);
}

cimcEnumeration* cimcClntReferences(cimcClient* cl,
                 cimcObjectPath* op, const char *resultClass ,const char *role ,
                 cimcFlags flags, char** properties, cimcStatus* rc)
{
   return cl->ft->references(cl,op,resultClass,role,flags,properties,rc);
}

cimcEnumeration* cimcClntReferenceNames(cimcClient* cl,
                 cimcObjectPath* op, const char *resultClass ,const char *role,
                 cimcStatus* rc)
{
   return cl->ft->referenceNames(cl,op,resultClass,role,rc);
}

cimcData cimcClntInvokeMethod(cimcClient* cl,
                 cimcObjectPath* op,const char *method,
                 cimcArgs* in, cimcArgs* out, cimcStatus* rc)
{
   return cl->ft->invokeMethod(cl,op,method,in,out,rc);
}

cimcStatus cimcClntSetProperty(cimcClient* cl,
                 cimcObjectPath* op, const char *name, void* value, cimcType type)
{
   return cl->ft->setProperty(cl,op,name,value,type);
}

cimcData cimcClntGetProperty(cimcClient *cl, 
                 cimcObjectPath *op, const char *name, cimcStatus *rc)
{
   return cl->ft->getProperty(cl,op,name,rc);
}




   //---------------------------------------------------
   //--
   //	 cimcEnv Function Calls
   //--
   //---------------------------------------------------



cimcClient* cimcEnvConnect(cimcEnv *ce, const char *hn, const char *scheme, const char *port,
                const char *user, const char *pwd, cimcStatus *rc)
{
   return ce->ft->connect(ce,hn,scheme,port,user,pwd,rc);
}

cimcClient* cimcEnvConnect2(cimcEnv *ce, const char *hn, const char *scheme, const char *port,
                const char *user, const char *pwd, 
                int verifyMode, const char * trustStore,
                const char * certFile, const char * keyFile,
                cimcStatus *rc)
{
   return ce->ft->connect2(ce,hn,scheme,port,user,pwd,verifyMode,trustStore,certFile,keyFile,rc);
}

cimcInstance* cimcEnvNewInstance(cimcEnv *ce, const cimcObjectPath* op, cimcStatus* rc)
{
   return ce->ft->newInstance(ce,op,rc);
}

cimcObjectPath* cimcEnvNewObjectPath(cimcEnv *ce, const char *ns, const char *cn, cimcStatus* rc)
{
   return ce->ft->newObjectPath(ce,ns,cn,rc);
}

cimcArgs* cimcEnvNewArgs(cimcEnv *ce, cimcStatus* rc)
{
   return ce->ft->newArgs(ce,rc);
}

cimcString* cimcEnvNewString(cimcEnv *ce, const char *data, cimcStatus* rc)
{
   return ce->ft->newString(ce,data,rc);
}

cimcArray* cimcEnvNewArray(cimcEnv *ce, cimcCount max, cimcType type, cimcStatus* rc)
{
   return ce->ft->newArray(ce,max,type,rc);
}

cimcDateTime* cimcEnvNewDateTime(cimcEnv *ce, cimcStatus* rc)
{
   return ce->ft->newDateTime(ce,rc);
}

cimcDateTime* cimcEnvNewDateTimeFromBinary(cimcEnv *ce, cimcUint64 binTime, cimcBoolean interval, 
           cimcStatus* rc)
{
   return ce->ft->newDateTimeFromBinary(ce,binTime,interval,rc);
}

cimcDateTime* cimcEnvNewDateTimeFromChars(cimcEnv *ce, const char *utcTime, cimcStatus* rc)
{
   return ce->ft->newDateTimeFromChars(ce,utcTime,rc);
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
