
#include "CimString.h"


int CimcString::decRefCount()
{
   return --(((cimcObject*)enc)->refCount);
}

int CimcString::incRefCount()
{
   return ++(((cimcObject*)enc)->refCount);
}

CimcString::CimcString(const CimcString& s) {
  enc=s.enc;
}

CimcString::CimcString(cimcString* c) {
   enc=c;
}

CimcString::CimcString() {
   enc=NULL;
}

CimcString::~CimcString() {
//   enc->ft->release(enc);
}

void CimcString::releaseEnc()
{
   if (enc) enc->ft->release(enc); 
   enc=NULL;
}

CimcString::operator const char* () const {
      return (const char*)enc->hdl;
}

CimcString::operator char* () const {
      return (char*)enc->hdl;
}

CimBoolean CimcString::equals(const char *str) const {
   return (strcmp((const char*)enc->hdl,str)==0);
}

CimBoolean CimcString::equals(const CimcString& str) const {
   return (strcmp((const char*)enc->hdl,(const char*)str)==0);
}

CimBoolean CimcString::equalsIgnoreCase(const char *str) const {
   return (strcasecmp((const char*)enc->hdl,str)==0);
}

CimBoolean CimcString::equalsIgnoreCase(const CimcString& str) const {
   return (strcasecmp((const char*)enc->hdl,(const char*)str)==0);
}

