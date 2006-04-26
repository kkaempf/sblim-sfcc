

#include "CimArray.h"

cimcArray *CimcArray::getEnc() const {
   return (cimcArray*)enc;
}

CimcArray::CimcArray(cimcArray *arr) {
   enc=arr;
}

CimcArray::CimcArray(cimcCount max, cimcType type) {
 //  enc=makeArray(cimcProviderBase::getBroker(),max,type);
}

CimcArray::CimcArray() {
}

 void CimcArray::operator=(int x) {
 }

CimcArrayIdx CimcArray::operator[](int idx) const {
   return CimcArrayIdx(*this,idx);
}
/*
void *CimcArray::makeArray(cimcBroker *mb, cimcCount max, cimcType type) {
   cimcStatus rc={CIMC_RC_OK,NULL};
   void *array=mb->eft->newArray(mb,max,type,&rc);
   if (rc.rc!=CIMC_RC_OK) throw cimcStatus(rc);
   return array;
}

cimcCount CimcArray::size() const {
   cimcStatus rc={CIMC_RC_OK,NULL};
   cimcCount c=getEnc()?getEnc()->ft->getSize(getEnc(),&rc):0;
   if (rc.rc!=CIMC_RC_OK) throw cimcStatus(rc);
   return c;
}
*/



CimcArrayIdx::CimcArrayIdx(const CimcArray &a, cimcCount i)
         : ar(a), idx(i)
{
}

CimcArrayIdx& CimcArrayIdx::operator=(const cimcData& v) {
   cimcStatus rc={CIMC_RC_OK,NULL};
   if (ar.getEnc()->ft->getSimpleType(ar.getEnc(),&rc)!=v._data.type)
      throw cimcStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   rc=ar.getEnc()->ft->setElementAt(ar.getEnc(),idx,(cimcValue*)&v._data.value,
				    v._data.type);
   if (rc.rc!=CIMC_RC_OK) throw cimcStatus(rc);
   return *this;
}


cimcData CimcArrayIdx::getData() const{
   cimcStatus rc={CIMC_RC_OK,NULL};
   cimcData d;
   d=ar.getEnc()->ft->getElementAt(ar.getEnc(),idx,&rc);
   if (rc.rc!=CIMC_RC_OK) throw cimcStatus(rc);
   return d;
}

CimcArrayIdx::operator cimcString() const{
   return getData();
}

CimcArrayIdx::operator const char* () const{
   return getData();
}

CimcArrayIdx::operator cimcDateTime() const{
   return getData();
}

CimcArrayIdx::operator cimcObjectPath() const{
   return getData();
}

CimcArrayIdx::operator cimcInstance() const{
   return getData();
}

CimcArrayIdx::operator cimcUint8() const{
   return getData();
}

CimcArrayIdx::operator cimcUint16() const{
   return getData();
}

CimcArrayIdx::operator cimcUint32() const{
   return getData();
}

CimcArrayIdx::operator cimcUint64() const{
   return getData();
}

CimcArrayIdx::operator cimcSint8() const{
   return getData();
}

CimcArrayIdx::operator cimcSint16() const{
   return getData();
}

CimcArrayIdx::operator cimcSint32() const{
   return getData();
}

CimcArrayIdx::operator cimcSint64() const{
   return getData();
}

CimcArrayIdx::operator cimcReal32() const{
   return getData();
}

CimcArrayIdx::operator cimcReal64() const{
   return getData();
}