

#include "CimObjectPath.h"
#include "CimStatus.h"


int CimcObjectPath::decRefCount()
{
   return --(((cimcObject*)op)->refCount);
}

int CimcObjectPath::incRefCount()
{
   return ++(((cimcObject*)op)->refCount);
}

CimcObjectPath::~CimcObjectPath()  
{ 
   //if (op) op->ft->release(op); 
}

CimcObjectPath::CimcObjectPath(cimcObjectPath *pop)
{
   op=pop; 
}

CimcObjectPath::CimcObjectPath(cimcData *d) 
{ 
   if (d->type==CIMC_ref) op=d->value.ref;
   else throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH,"--- CimObjectPath type conflict");
}

void CimcObjectPath::releaseEnc()
{
   if (op) op->ft->release(op);
   op=NULL;
}

CimString CimcObjectPath::getClassName() 
{
   return CimString(op->ft->getClassName(op,NULL));
}

void CimcObjectPath::setClassName(const char *cn) 
{
   op->ft->setClassName(op,cn);
}


CimString CimcObjectPath::getHostname() 
{
   return CimString(op->ft->getHostname(op,NULL));
}

void CimcObjectPath::setHostname(const char *hn) 
{
   op->ft->setHostname(op,hn);
}


CimString CimcObjectPath::getNameSpace() 
{
   return CimString(op->ft->getNameSpace(op,NULL));
}

void CimcObjectPath::setNameSpace(const char *ns) 
{
   op->ft->setNameSpace(op,ns);
}

CimData CimcObjectPath::getKey(const char* kn)
{
   cimcStatus st;
   CimData d=CimData(op->ft->getKey(op, kn, &st));
   return d;
}

void CimcObjectPath::addKey(const char* kn, const CimData& d)
{
   cimcStatus st;
   st = op->ft->addKey(op, kn, (cimcValue*)&d._data.value, d._data.type);
}
