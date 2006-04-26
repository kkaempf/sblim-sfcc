

#ifndef Sfcc_CimInstance_h
#define Sfcc_CimInstance_h

#include "cimcdt.h"
#include "cimcft.h"

#include "sfccPtr.h"
#include "CimObject.h"
#include "CimIterator.h"

class CimData;

class CimcInstance : public CimObject {
  friend class sfccPtr<CimcInstance, cimcInstance>;
  friend class CimcIterator;
  private:
   cimcInstance *inst;
   CimcInstance(cimcInstance* pinst) { inst=pinst; }
   int decRefCount();
   int incRefCount();
   void releaseEnc();
   cimcInstance *getEnc() const { return inst; }
   void setProperty(CimString *pName, CimData data);
   CimData getPropertyAt(unsigned int pos,  CimString *pName);
   CimData getQualifierAt(unsigned int pos,  CimString *qName);   
   CimData getPropertyQualifierAt(unsigned int pos,  const char *prop, CimString *qName);
  public: 
  ~CimcInstance();
   CimIterator getPropertyIterator();
   CimIterator getPropertyQualifierIterator(const char *prop);
   CimData getProperty(const char *prop);
   CimData getQualifier(const char *qual);
   CimData getPropertyQualifier(const char *prop, const char *qual);
};


typedef sfccPtr<CimcInstance,cimcInstance> CimInstance;

#endif
