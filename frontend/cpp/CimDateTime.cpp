
#include "CimDateTime.h"


int CimcDateTime::decRefCount()
{
   return --(((cimcObject*)enc)->refCount);
}

int CimcDateTime::incRefCount()
{
   return ++(((cimcObject*)enc)->refCount);
}

CimcDateTime::CimcDateTime(const CimcDateTime& s) {
  enc=s.enc;
}

CimcDateTime::CimcDateTime(cimcDateTime* c) {
   enc=c;
}

CimcDateTime::CimcDateTime() {
   enc=NULL;
}

CimcDateTime::~CimcDateTime() {
//   enc->ft->release(enc);
}

void CimcDateTime::releaseEnc()
{
   if (enc) enc->ft->release(enc); 
   enc=NULL;
}

CimcDateTime::operator cimcUint64() {
      return enc->ft->getBinaryFormat(enc,NULL);
}

CimcDateTime::operator CimString() {
      return CimString(enc->ft->getStringFormat(enc,NULL));
}

CimBoolean CimcDateTime::isInterval() {
   return enc->ft->isInterval(enc,NULL);
}

