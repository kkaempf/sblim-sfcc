

#include "CimEnumeration.h"

int CimcObjectPathEnumeration::decRefCount()
{
   --(((cimcObject*)en)->refCount);
   return ((cimcObject*)en)->refCount;
}

int CimcObjectPathEnumeration::incRefCount()
{
   ++(((cimcObject*)en)->refCount);
   return ((cimcObject*)en)->refCount;
}

CimcObjectPathEnumeration::~CimcObjectPathEnumeration()
{ 
//   if (en) en->ft->release(en); 
}

void CimcObjectPathEnumeration::releaseEnc()
{
   if (en) en->ft->release(en); 
   en=NULL;
}

CimcObjectPathEnumeration::CimcObjectPathEnumeration(cimcEnumeration *pen) 
{
   en=pen;
}

CimObjectPath CimcObjectPathEnumeration::getNext() 
{
   cimcStatus st;
   CimObjectPath t=CimObjectPath(en->ft->getNext(en,&st).value.ref);
   if (st.rc) throw(CimStatus(st));
   return t;
} 
  
int CimcObjectPathEnumeration::hasNext() 
{
   return en->ft->hasNext(en,NULL);
}   


int CimcInstanceEnumeration::decRefCount()
{
   return --(((cimcObject*)en)->refCount);
}

int CimcInstanceEnumeration::incRefCount()
{
   return ++(((cimcObject*)en)->refCount);
}

CimcInstanceEnumeration::~CimcInstanceEnumeration()
{ 
//   if (en) en->ft->release(en); 
}

void CimcInstanceEnumeration::releaseEnc()
{
   if (en) en->ft->release(en); 
   en=NULL;
}

CimcInstanceEnumeration::CimcInstanceEnumeration(cimcEnumeration *pen) 
{
   en=pen;
}

CimInstance CimcInstanceEnumeration::getNext() 
{
   cimcStatus st;
   CimInstance t=CimInstance(en->ft->getNext(en,&st).value.inst);
   if (st.rc) throw(CimStatus(st));
   return t;
}   
   
int CimcInstanceEnumeration::hasNext() 
{
   return en->ft->hasNext(en,NULL);
}   


int CimcClassEnumeration::decRefCount()
{
   return --(((cimcObject*)en)->refCount);
}

int CimcClassEnumeration::incRefCount()
{
   return ++(((cimcObject*)en)->refCount);
}

CimcClassEnumeration::~CimcClassEnumeration()
{ 
//   if (en) en->ft->release(en); 
}

void CimcClassEnumeration::releaseEnc()
{
   if (en) en->ft->release(en); 
   en=NULL;
}

CimcClassEnumeration::CimcClassEnumeration(cimcEnumeration *pen) 
{
   en=pen;
}
 
CimClass CimcClassEnumeration::getNext() 
{
   cimcStatus st;
   CimClass t=CimClass(en->ft->getNext(en,&st).value.cls);
   if (st.rc) throw(CimStatus(st));
   return t;
}
      
int CimcClassEnumeration::hasNext() 
{
   return en->ft->hasNext(en,NULL);
}   
