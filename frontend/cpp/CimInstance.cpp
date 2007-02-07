

#include "CimInstance.h"
#include "CimString.h"
#include "CimData.h"


int CimcInstance::decRefCount()
{
   return --(((cimcObject*)inst)->refCount);
}

int CimcInstance::incRefCount()
{
   return ++(((cimcObject*)inst)->refCount);   
}

CimcInstance::~CimcInstance() 
{ 
//   if (inst) inst->ft->release(inst); 
}

void CimcInstance::releaseEnc()
{
   if (inst) inst->ft->release(inst); 
   inst=NULL;
}

CimIterator CimcInstance::getPropertyIterator() 
{
   CimIterator it;
   it->set(this, NULL, NULL, inst->ft->getPropertyCount(inst,NULL), CimcIterator::instProp);   
   return it;
}
  
CimIterator CimcInstance::getPropertyQualifierIterator(const char *prop) 
{
   CimIterator it;
   it->set(this, (char*)prop, NULL, 
      inst->ft->getPropertyQualifierCount(inst,(char*)prop,NULL), CimcIterator::instPropQual);   
   return it;
}


void CimcInstance::setProperty(CimString *pName, CimData data)
{
   cimcStatus st=inst->ft->setProperty(inst,(const char*)pName,data.getValue(),data.getType());
}

CimData CimcInstance::getProperty(const char *prop)
{
   return CimData(inst->ft->getProperty(inst,(char*)prop,NULL));
}

CimData CimcInstance::getQualifier(const char *qual)
{
   return CimData(inst->ft->getQualifier(inst,(char*)qual,NULL));
}

CimData CimcInstance::getPropertyQualifier(const char *prop, const char *qual)
{
   return CimData(inst->ft->getPropertyQualifier(inst,(char*)prop,(char*)qual,NULL));
}


CimData CimcInstance::getPropertyAt(unsigned int pos,  CimString *pName) 
{
   cimcData d;
   cimcString *name;
   d=inst->ft->getPropertyAt(inst,pos, pName ? &name: NULL, NULL);
   if (pName) *pName=CimString(name);
   return CimData(d);
}

CimData CimcInstance::getQualifierAt(unsigned int pos,  CimString *qName) 
{
   cimcData d;
   cimcString *name;
   d=inst->ft->getQualifierAt(inst,pos, qName ? &name : NULL ,NULL);
   if (qName) *qName=CimString(name);
   return CimData(d);
}

CimData CimcInstance::getPropertyQualifierAt(unsigned int pos,  const char *prop, CimString *qName)  
{
   cimcData d;
   cimcString *name;
   d=inst->ft->getPropertyQualifierAt(inst,prop,pos, qName ? &name : NULL ,NULL);
   if (qName) *qName=CimString(name);
   return CimData(d);
}
