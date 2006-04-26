

#include "CimIterator.h"
#include "CimClass.h"
#include "CimInstance.h"
#include "CimString.h"


CimcIterator::CimcIterator()
{
   inst=NULL;
   cls=NULL;
}
 
void CimcIterator::set(CimcClass *cl, char* pn, char* mn, int c, iType t) 
{ 
   cls=new CimClass(cl->getEnc());
   inst=NULL;
   type=t;
   next=0;
   count=c;
   pName=pn;
   mName=mn;
} 
 
void CimcIterator::set(CimcInstance *i, char* pn, char* mn, int c, iType t) 
{ 
   inst=new CimInstance(i->getEnc());
   cls=NULL;
   type=t;
   next=0;
   count=c;
   pName=pn;
   mName=mn;
} 

CimcIterator::~CimcIterator() {
//   fprintf(stderr,"--- deleting iterator\n");
   if (cls) delete cls;
   else if (inst) delete inst;
} 

int CimcIterator::hasNext() 
{
   return count>next;
}
  
CimData CimcIterator::getNext(CimString **name) 
{
   if (!hasNext()) { }  
   switch (type) {
   case instProp:
    //  return (*inst)->getPropertyAt(next++,name);
   case instQual:
   //   return (*inst)->getQualifierAt(next++,name);
   case instPropQual:
   //   return (*inst)->getPropertyQualifierAt(next++,pName,name);
   break;
   case clsQual:
//      return (*cls)->getQualifierAt(next++,name);
   case clsProp:
      return (*cls)->getPropertyAt(next++,name);
   case clsPropQual:
//      return inst->getPropertyQualifierAt(next++,name,pName);
   case clsMeth:
//      return inst->getMethodAt(next++,name)
   case clsMethQual:
//      return inst->getMethodQualifierAt(next++,name,mName);
   case clsMethParm:
//      return inst->getMethodParam(next++,name,mName);
   case clsMethParmQual:
//      return inst->getMethodParamQualifier(next++,name,mName,pName)
;
   }   
}   

CimData CimcIterator::getNext() 
{
   CimString *name;
   CimData d=getNext(&name);
   delete name;
   return d;
}

unsigned int CimcIterator::getCount()
{
   return count;
}   
