

#ifndef Sfcc_CimData_h
#define Sfcc_CimData_h

#include "cimcdt.h"
#include "cimcft.h"

#include "CimString.h"
#include "sfccPtr.h"

class CimcObjectPath;
typedef class sfccPtr<CimcObjectPath, cimcObjectPath> CimObjectPath;
class CimcInstance;
typedef class sfccPtr<CimcInstance, cimcInstance> CimInstance;
class CimcDateTime;
typedef class sfccPtr<CimcDateTime, cimcDateTime> CimDateTime;

class CimData {
  friend class CimcObjectPath; 
  friend class CimcInstance; 
  friend class CimcIterator;
  friend class CimcClass; 
  friend class CimcDateTime; 
  
  protected:
   cimcData _data;
   union _cimc {
      CimString *string;      
      CimInstance *inst;      
      CimObjectPath *ref;
      CimDateTime *dateTime;
   } cim;
   CimData(cimcData data);
   
   cimcData getEnc();
   cimcValue* getValue();
   cimcType getType();
  public:
  ~CimData();
   CimData();
   CimData(cimcSint8 d);
   CimData(cimcSint16 d);
   CimData(cimcSint32 d);
   CimData(cimcSint64 d);
   CimData(cimcUint8 d);
   CimData(cimcUint16 d);
   CimData(cimcUint32 d);
   CimData(cimcUint64 d);
   CimData(cimcReal32 d);
   CimData(cimcReal64 d);
   CimData(const CimString& d);
   CimData(const char* d);
   CimData(const CimObjectPath& d);
   CimData(const CimDateTime& d);
//   CimData(const CimArray& d);
   operator CimString() const;
   operator const char* () const;
   operator CimDateTime() const;
   operator cimcSint8() const;
   operator cimcSint16() const;
   operator cimcSint32() const;
   operator cimcSint64() const;
   operator unsigned char() const;
   operator unsigned short() const;
   operator cimcUint32() const;
   operator cimcUint64() const;
   operator cimcReal32() const;
   operator cimcReal64() const;
//   operator CimArray() const;
   operator CimInstance() const;
   operator CimObjectPath() const;
   int isNullValue() const;
   int isNotFound() const;
};

#endif



