

#ifndef Sfcc_CimClass_h
#define Sfcc_CimClass_h

#include "cimcdt.h"
#include "cimcft.h"

#include "CimObject.h"
#include "sfccPtr.h"
#include "CimString.h"
#include "CimData.h"
#include "CimIterator.h"

class CimcClass : public CimObject {
  friend class CimClient;
  friend class CimData;
  friend class sfccPtr<CimcClass, cimcConstClass>;
  friend class CimcIterator;
   cimcConstClass *cls;
  protected:
   CimcClass(cimcConstClass *pcls) { cls=pcls; };
   CimcClass(cimcData *d);
   int decRefCount();
   int incRefCount();
   void releaseEnc();
   cimcConstClass *getEnc() const { return cls; }
   CimData getPropertyAt(unsigned int pos,  CimString **pName);
  public:
  ~CimcClass();
   CimString getClassName();
   CimString getSuperClassName();
   CimIterator getPropertyIterator();
   CimIterator getPropertyQualifierIterator(const char *prop);
   CimData getProperty(const char *prop);
   CimData getQualifier(const char *qual);
   CimData getPropertyQualifier(const char *prop, const char *qual);
};

typedef sfccPtr<CimcClass,cimcConstClass> CimClass;

#endif
