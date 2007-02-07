
#ifndef Sfcc_CimDateTime_h
#define Sfcc_CimDateTime_h

#include "cimcdt.h"
#include "cimcft.h"
#include "sfccPtr.h"


#ifndef CimBoolean
#define CimBoolean cimcBoolean
#define CimRc      cimcrc
#endif

class CimcString;

typedef class sfccPtr<CimcString, cimcString> CimString;

class CimcDateTime {
   friend class CimData;
   friend class CimcClass;
   friend class CimcInstance;
   friend class CimcObjectPath;
   friend class sfccPtr<CimcDateTime,cimcDateTime>;;
private:
protected:
   cimcDateTime *enc;
   CimcDateTime(cimcDateTime* c);
   void releaseEnc();
   cimcDateTime *getEnc() { return enc; }
   int decRefCount();
   int incRefCount();
public:
   CimcDateTime();
   CimcDateTime(const CimcDateTime& s);
  ~CimcDateTime();
   CimBoolean isInterval();
   operator CimString();
   operator cimcUint64();
};

typedef sfccPtr<CimcDateTime, cimcDateTime> CimDateTime;

#endif


