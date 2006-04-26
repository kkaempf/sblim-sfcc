
#ifndef Sfcc_CimEnumeration_h
#define Sfcc_CimEnumeration_h

#include "cimcdt.h"
#include "cimcft.h"

#include "CimObjectPath.h"
#include "CimClass.h"
#include "CimInstance.h"

class CimcObjectPathEnumeration {
  friend class CimClient;
  friend class sfccPtr<CimcObjectPathEnumeration, cimcEnumeration>;
   cimcEnumeration *en;
   CimcObjectPathEnumeration(cimcEnumeration *pen);
   int decRefCount();
   int incRefCount();
   void releaseEnc();
   cimcEnumeration *getEnc() const { return en; }
  public: 
  ~CimcObjectPathEnumeration();
   CimObjectPath getNext();
   int hasNext();
};

class CimcInstanceEnumeration {
  friend class CimClient;
  friend class sfccPtr<CimcInstanceEnumeration, cimcEnumeration>;
   cimcEnumeration *en;
   CimcInstanceEnumeration(cimcEnumeration *pen);
   int decRefCount();
   int incRefCount();
   void releaseEnc();
   cimcEnumeration *getEnc() const { return en; }
  public: 
  ~CimcInstanceEnumeration();
   CimInstance getNext();
   int hasNext();
};

class CimcClassEnumeration {
  friend class CimClient;
  friend class sfccPtr<CimcClassEnumeration, cimcEnumeration>;
   cimcEnumeration *en;
   CimcClassEnumeration(cimcEnumeration *pen);
   int decRefCount();
   int incRefCount();
   void releaseEnc();
   cimcEnumeration *getEnc() const { return en; }
  public: 
  ~CimcClassEnumeration();
   CimClass getNext();
   int hasNext();
};




typedef sfccPtr <CimcObjectPathEnumeration, cimcEnumeration>  CimObjectPathEnumeration;
typedef sfccPtr <CimcClassEnumeration, cimcEnumeration>       CimClassEnumeration;
typedef sfccPtr <CimcInstanceEnumeration, cimcEnumeration>    CimInstanceEnumeration;

#endif
