


#include "CimInstance.h"
#include "CimObjectPath.h"
#include "CimDateTime.h"
#include "CimEnumeration.h"

#include "sfccPtr.h"

#define Tc CimcInstance 
#define Te cimcInstance
#include "sfccPtrImpl.h"
#undef Tc
#undef Te   

#define Tc CimcClass 
#define Te cimcConstClass
#include "sfccPtrImpl.h"
#undef Tc
#undef Te   

#define Tc CimcObjectPath
#define Te cimcObjectPath
#include "sfccPtrImpl.h"
#undef Tc
#undef Te   

#define Tc CimcDateTime
#define Te cimcDateTime
#include "sfccPtrImpl.h"
#undef Tc
#undef Te   

#define Tc CimcString
#define Te cimcString
#include "sfccPtrImpl.h"
#undef Tc
#undef Te   

#define Tc CimcClassEnumeration
#define Te cimcEnumeration
#include "sfccPtrImpl.h"
#undef Tc
#undef Te   

#define Tc CimcInstanceEnumeration
#define Te cimcEnumeration
#include "sfccPtrImpl.h"
#undef Tc
#undef Te   

#define Tc CimcObjectPathEnumeration
#define Te cimcEnumeration
#include "sfccPtrImpl.h"
#undef Tc
#undef Te   

