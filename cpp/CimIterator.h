

#ifndef Sfcc_CimcIterator_h
#define Sfcc_CimcIterator_h

#include "cimcdt.h"
#include "cimcft.h"
#include "sfccSPtr.h"
#include "sfccPtr.h"

class CimcClass;
class CimcInstance;
class CimcString;
class CimData;


typedef class sfccPtr<CimcString, cimcString> CimString;
typedef class sfccPtr<CimcClass, cimcConstClass> CimClass;
typedef class sfccPtr<CimcInstance, cimcInstance> CimInstance;

class CimcIterator {
  friend class sfccSPtr<CimcIterator>;
  friend class CimcInstance;
  friend class CimcClass;
  protected:
   enum iType {
      instQual,
      instProp,
      instPropQual,
      clsQual,
      clsProp,
      clsPropQual,
      clsMeth,
      clsMethQual,
      clsMethParm,
      clsMethParmQual
   };
  private:    
   CimClass *cls;
   CimInstance *inst;
   char *pName;
   char *mName;
   int count,next;
   iType type;
   int refCount;
  protected:
   CimcIterator();
   void set(CimcClass *c, char* pn, char* mn, int c, iType t);
   void set(CimcInstance *i, char* pn, char* mn, int c, iType t);
  public:
  ~CimcIterator();
   int hasNext();
   CimData getNext(CimString **name);
   CimData getNext();
   unsigned int getCount();
}; 


typedef sfccSPtr<CimcIterator> CimIterator;


#endif

