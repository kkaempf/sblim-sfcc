
#ifndef Sfcc_CimObjectPath_h
#define Sfcc_CimObjectPath_h

#include "cimcdt.h"
#include "cimcft.h"

#include "sfccPtr.h"
#include "CimStatus.h"
#include "CimString.h"
#include "CimData.h"

class CimData;

class CimcObjectPath {
  friend class CimClient;
  friend class CimData;
  friend class sfccPtr<CimcObjectPath, cimcObjectPath>;
   cimcObjectPath *op;
  protected:
   CimcObjectPath(cimcObjectPath *pop);
   CimcObjectPath(cimcData *d);
   int decRefCount();
   int incRefCount();
   cimcObjectPath *getEnc() const {  return op; }
   void releaseEnc();
  public:
  ~CimcObjectPath();
   CimString getClassName();
   void      setClassName(const char* cn);
   CimString getHostname();
   void      setHostname(const char* hn);
   CimString getNameSpace();
   void      setNameSpace(const char* ns);
   CimData   getKey(const char* pn);
   void      addKey(const char* pn, const CimData &data);
};

typedef sfccPtr<CimcObjectPath,cimcObjectPath> CimObjectPath;

#endif
