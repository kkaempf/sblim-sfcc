
#include <CimStatus.h>
#include <stdio.h>

struct errEntry {
      void (*fnc) (cimcStatus& st);
};



static void failed(cimcStatus& st)
{
   throw CimStatusFailed(st);
}

static void accessDenied(cimcStatus& st)
{
   throw CimStatusAccessDenied(st);
}
static void invalidNamespace(cimcStatus& st)
{
   throw CimStatusInvalidNamespace(st);
}
static void invalidParameter(cimcStatus& st)
{
   throw CimStatusInvalidParameter(st);
}
static void invalidClass(cimcStatus& st)
{
   throw CimStatusInvalidClass(st);
}
static void notFound(cimcStatus& st)
{
   throw CimStatusNotFound(st);
}

static errEntry errors[]={ 
   {failed},
   {accessDenied},
   {invalidParameter},
   {invalidClass},
   {notFound}
};


void CimStatus::throwException(cimcStatus& st)
{
   errors[st.rc-1].fnc(st);
}

CimStatus::CimStatus() {
   rc=0;
   msg=NULL;
}
   
CimStatus::CimStatus(int prc, char *pmsg) {
   rc=prc;
   msg=pmsg;
}
   
CimStatus::CimStatus(cimcStatus &st) {
   rc=st.rc;
   msg=(char*)st.msg->hdl;
}

CimStatusFailed::CimStatusFailed(cimcStatus &st) : CimStatus(st) 
{
}

CimStatusAccessDenied::CimStatusAccessDenied(cimcStatus &st) : CimStatus(st) 
{
}

CimStatusInvalidNamespace::CimStatusInvalidNamespace(cimcStatus &st) : CimStatus(st) 
{
}

CimStatusInvalidParameter::CimStatusInvalidParameter(cimcStatus &st) : CimStatus(st) 
{
}

CimStatusInvalidClass::CimStatusInvalidClass(cimcStatus &st) : CimStatus(st) 
{
}

CimStatusNotFound::CimStatusNotFound(cimcStatus &st) : CimStatus(st) 
{
}
