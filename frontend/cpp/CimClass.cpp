
#include "CimClass.h"
#include "CimStatus.h"
#include "CimString.h"
#include "CimData.h"

int CimcClass::decRefCount()
{
   return --(((cimcObject*)cls)->refCount);
}

int CimcClass::incRefCount()
{
   return ++(((cimcObject*)cls)->refCount);
}

CimcClass::~CimcClass()  
{ 
//   if (cls) cls->ft->release(cls); 
}

void CimcClass::releaseEnc()
{
   if (cls) cls->ft->release(cls);
   cls=NULL;
}

CimString CimcClass::getClassName() 
{
   return CimString(cls->ft->getClassName(cls,NULL));
}

CimString CimcClass::getSuperClassName() 
{
   return CimString(cls->ft->getSuperClassName(cls,NULL));
}

CimIterator CimcClass::getPropertyIterator() 
{
   CimIterator it;
   it->set(this, NULL, NULL, cls->ft->getPropertyCount(cls,NULL), CimcIterator::clsProp);   
   return it;
}
  
CimIterator CimcClass::getPropertyQualifierIterator(const char *prop) 
{
   CimIterator it;
   it->set(this, (char*)prop, NULL, 
      cls->ft->getPropQualifierCount(cls,(char*)prop,NULL), CimcIterator::clsPropQual);   
   return it;
}

/*
void CimcInstance::setProperty(CimString *pName, CimData data)
{
   cimcStatus st=inst->ft->setProperty(inst,(const char*)pName,data.getValue(),data.getType());
}
*/
CimData CimcClass::getProperty(const char *prop)
{
   return CimData(cls->ft->getProperty(cls,(char*)prop,NULL));
}

CimData CimcClass::getQualifier(const char *qual)
{
   return CimData(cls->ft->getQualifier(cls,(char*)qual,NULL));
}

CimData CimcClass::getPropertyQualifier(const char *prop, const char *qual)
{
   return CimData(cls->ft->getPropQualifier(cls,(char*)prop,(char*)qual,NULL));
}


CimData CimcClass::getPropertyAt(unsigned int pos,  CimString **pName) 
{
   cimcData d;
   cimcString *name;
   d=cls->ft->getPropertyAt(cls, pos, &name, NULL);
   if (pName) *pName= new CimString(name);
   return CimData(d);
}

