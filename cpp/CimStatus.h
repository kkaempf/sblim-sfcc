
#ifndef Sfcc_CimStatus_h
#define Sfcc_CimStatus_h

#include "cimcdt.h"
#include "cimcft.h"



class CimStatus {
   int rc;
   char *msg;
  public: 
   static void throwException(cimcStatus&); 
   CimStatus(); 
   CimStatus(int prc, char *pmsg=NULL);
   CimStatus(cimcStatus &st);
   operator char*() { return msg; }
   operator int() { return rc; };
};

class CimStatusFailed : public CimStatus
{
  public: 
   CimStatusFailed(cimcStatus &st); 
};

class CimStatusAccessDenied : public CimStatus
{
  public: 
   CimStatusAccessDenied(cimcStatus &st);
};

class CimStatusInvalidNamespace : public CimStatus
{
  public: 
   CimStatusInvalidNamespace(cimcStatus &st);
};

class CimStatusInvalidParameter : public CimStatus
{
  public: 
   CimStatusInvalidParameter(cimcStatus &st);
};

class CimStatusInvalidClass : public CimStatus
{
  public: 
   CimStatusInvalidClass(cimcStatus &st);
};

class CimStatusNotFound : public CimStatus
{
  public: 
   CimStatusNotFound(cimcStatus &st);
};


/*
FAILED                       =1,
ACCESS_DENIED                =2,
INVALID_NAMESPACE            =3,
INVALID_PARAMETER            =4,
INVALID_CLASS                =5,
NOT_FOUND                    =6,
NOT_SUPPORTED                =7,
CLASS_HAS_CHILDREN           =8,
CLASS_HAS_INSTANCES          =9,
INVALID_SUPERCLASS           =10,
ALREADY_EXISTS               =11,
NO_SUCH_PROPERTY             =12,
TYPE_MISMATCH                =13,
QUERY_LANGUAGE_NOT_SUPPORTED =14,
INVALID_QUERY                =15,
METHOD_NOT_AVAILABLE         =16,
METHOD_NOT_FOUND             =17,
*/
#endif
