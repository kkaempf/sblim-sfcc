
#include "CimData.h"
#include "CimStatus.h"
#include "CimObjectPath.h"
#include "CimInstance.h"
#include "CimString.h"
#include "CimDateTime.h"
#include "CimArray.h"


cimcData CimData::getEnc() {
   return _data;
}

cimcValue* CimData::getValue() {
   return &_data.value;
}

cimcType CimData::getType() {
   return _data.type;
}


CimData::CimData(cimcData data) {
   this->_data=data;
   if (data.type & CIMC_ENC) switch (data.type) {
      case CIMC_string: cim.string=new CimString(data.value.string);
      break;
      case CIMC_ref: cim.ref=new CimObjectPath(data.value.ref);
      break;
      case CIMC_dateTime: cim.dateTime=new CimDateTime(data.value.dateTime);
      break;
      case CIMC_instance: cim.inst=new CimInstance(data.value.inst);
      break;
  }
}

CimData::~CimData() {
   if (_data.type & CIMC_ENC) switch (_data.type) {
      case CIMC_string: delete cim.string;
      break;
      case CIMC_ref: delete cim.ref;
      break;
      case CIMC_dateTime: delete cim.dateTime;
      break;
      case CIMC_instance: delete cim.inst;
      break;
   }
}



CimData::CimData(cimcSint8 d) {
   _data.state=CIMC_goodValue;
   _data.value.sint8=d;
   _data.type=CIMC_sint8;
}

CimData::CimData(cimcSint16 d) {
   _data.state=CIMC_goodValue;
   _data.value.sint16=d;
   _data.type=CIMC_sint16;
}

CimData::CimData(cimcSint32 d) {
   _data.state=CIMC_goodValue;
   _data.value.sint32=d;
   _data.type=CIMC_sint32;
}

CimData::CimData(cimcSint64 d) {
   _data.state=CIMC_goodValue;
   _data.value.sint64=d;
   _data.type=CIMC_sint64;
}

CimData::CimData(cimcUint8 d) {
   _data.state=CIMC_goodValue;
   _data.value.uint8=d;
   _data.type=CIMC_uint8;
}

CimData::CimData(cimcUint16 d) {
   _data.state=CIMC_goodValue;
   _data.value.uint16=d;
   _data.type=CIMC_uint16;
}

CimData::CimData(cimcUint32 d) {
   _data.state=CIMC_goodValue;
   _data.value.uint32=d;
   _data.type=CIMC_uint32;
}

CimData::CimData(cimcUint64 d) {
   _data.state=CIMC_goodValue;
   _data.value.uint64=d;
   _data.type=CIMC_uint64;
}

CimData::CimData(cimcReal32 d) {
   _data.state=CIMC_goodValue;
   _data.value.real32=d;
   _data.type=CIMC_real32;
}

CimData::CimData(cimcReal64 d) {
   _data.state=CIMC_goodValue;
   _data.value.real64=d;
   _data.type=CIMC_real64;
}

CimData::CimData(const CimString& d) {
   _data.state=CIMC_goodValue;
   _data.value.chars=strdup((const char*)(&d)->enc);
   _data.type=CIMC_chars;
}

CimData::CimData(const char* d) {
  _data.state=CIMC_goodValue;
  _data.value.chars=(char*)d;
  _data.type=CIMC_chars;
}

CimData::CimData(const CimDateTime& d) {
   _data.state=CIMC_goodValue;
   cim.dateTime=&((CimDateTime&)d);
   _data.type=CIMC_dateTime;
}

CimData::CimData(const CimArray& d) {
   _data.state=CIMC_goodValue;
   cim.array=&((CimArray&)d);
   _data.type=(d->getEnc()->ft)->getSimpleType(d->getEnc(),0) | CIMC_ARRAY;
}



CimData::operator CimString() const {
   if (_data.type!=CIMC_string)
     throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   return *cim.string;
}

CimData::operator const char* () const {
   if (_data.type == CIMC_string)
      return (const char *)cim.string;
   else if (_data.type == CIMC_chars)
      return _data.value.chars;
   else
      throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
}

CimData::operator CimDateTime() const {
   if (_data.type!=CIMC_dateTime)
      throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   return *cim.dateTime;
}

CimData::operator cimcSint8() const {
   if (_data.type!=CIMC_sint8)
      throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   else return _data.value.sint8;
}
CimData::operator cimcSint16() const {
   if (_data.type!=CIMC_sint16)
      throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   else return _data.value.sint16;
}

CimData::operator cimcSint32() const {
   if (_data.type!=CIMC_sint32)
      throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   else return _data.value.sint32;
}

CimData::operator cimcSint64() const {
   if (_data.type!=CIMC_sint64)
      throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   else return _data.value.sint64;
}

CimData::operator unsigned char() const {
   if (_data.type!=CIMC_uint8 && _data.type!=CIMC_boolean)
      throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   if (_data.type==CIMC_uint8)
      return _data.value.uint8;
   else return _data.value.boolean;
}

CimData::operator unsigned short() const {
   if (_data.type!=CIMC_uint16 && _data.type!=CIMC_char16)
      throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   if (_data.type==CIMC_uint16)
      return _data.value.uint16;
   else return _data.value.char16;
}

CimData::operator cimcUint32() const {
   if (_data.type!=CIMC_uint32)
      throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   else return _data.value.uint32;
}

CimData::operator cimcUint64() const {
   if (_data.type!=CIMC_uint64)
      throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   else return _data.value.uint64;
}

CimData::operator cimcReal32() const {
   if (_data.type!=CIMC_real32)
      throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   else return _data.value.real32;
}

CimData::operator cimcReal64() const {
   if (_data.type!=CIMC_real64)
      throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   else return _data.value.real64;
}

CimData::CimData(const CimObjectPath& d) {
  _data.state=d->getEnc()==0?CIMC_nullValue:CIMC_goodValue;
  _data.value.ref=(cimcObjectPath*)d->getEnc();
  _data.type=CIMC_ref;
}

CimData::operator CimInstance() const {
   if (_data.type!=CIMC_instance)
      throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   else return *cim.inst;
}

CimData::operator CimObjectPath() const {
   if (_data.type!=CIMC_ref)
      throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   else return *cim.ref;
}

CimData::operator CimArray() const {
   if (!(_data.type&CIMC_ARRAY))
      throw CimStatus(CIMC_RC_ERR_TYPE_MISMATCH);
   else return *cim.array;
}

int CimData::isNullValue() const {
  return (_data.state & CIMC_nullValue);
}

int CimData::isNotFound() const {
  return (_data.state & CIMC_notFound);
}

