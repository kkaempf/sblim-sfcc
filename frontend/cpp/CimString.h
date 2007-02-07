
#ifndef Sfcc_CimString_h
#define Sfcc_CimString_h

#include "cimcdt.h"
#include "cimcft.h"
#include "string.h"
#include "sfccPtr.h"


#ifndef CimBoolean
#define CimBoolean cimcBoolean
#define CimRc      cimcrc
#endif

#include "CimStatus.h"

class CimcString {
   friend class CimData;
   friend class CimcClass;
   friend class CimcInstance;
   friend class CimcObjectPath;
   friend class sfccPtr<CimcString,cimcString>;;
private:
protected:
   cimcString *enc;
   CimcString(cimcString* c);
   int decRefCount();
   int incRefCount();
   void releaseEnc();
   cimcString *getEnc() { return enc; }
public:
   CimcString();
   CimcString(const CimcString& s);
  ~CimcString();
   operator const char* () const;
   operator char* () const;
   CimBoolean equals(const char *str) const;
   CimBoolean equals(const CimcString& str) const;
   CimBoolean equalsIgnoreCase(const char *str) const;
   CimBoolean equalsIgnoreCase(const CimcString& str) const;
};

typedef sfccPtr<CimcString, cimcString> CimString;
#endif


