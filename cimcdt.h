
/*
 * cimc.h
 *
 * (C) Copyright IBM Corp. 2005
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * CIMC data types.
 *
*/

#ifndef _CIMCDT_H_
#define _CIMCDT_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// defintion of version numbers to be used by providers using CMBrokerVersion()
// They indicate what CIMC version is supported by both the broker and its adapter

   #define CIMCVersion051 51     //  0.51
   #define CIMCVersion060 60     //  0.60
   #define CIMCVersion070 70     //  0.70
   #define CIMCVersion080 80     //  0.80
   #define CIMCVersion085 85     //  0.85
   #define CIMCVersion086 86     //  0.86
   #define CIMCVersion087 87     //  0.87
   #define CIMCVersion090 90     //  0.90


// CIMC_VERSION compile switch should be used during MI compilation only.
// It is used define minimal version support needed from Management Broker.
// This value will be set in <mi-name>_Create<mi-type>MI.mi_version

#ifdef CIMC_VERSION
  #if (CIMC_VERSION==80)
     #define CIMC_VER_80 1
  #elif (CIMC_VERSION==85)
     #define CIMC_VER_85 1
  #elif (CIMC_VERSION==86)
     #define CIMC_VER_86 1
   #elif (CIMC_VERSION==87)
     #define CIMC_VER_87 1
   #elif (CIMC_VERSION==90)
     #define CIMC_VER_90 1
  #else
     #error Unsupported CIMC_VERSION defined
  #endif
#else
  #define CIMC_VER_86
#endif


// CIMC_VER_x switche is used by Management Broker implementations only.

// It defines the CIMC version supported by the Managmeent Broker.
// This value must be set in the ftVersion field of all functions tables.

// Version definitions are cummulative.
// A new version definition must #define all previous definitions.

// During MI loading MBs must ensure that
//  <mi-name>_Create<mi-type>MI.miVersion<=<mi-name>_Create<mi-type>MI.ftVersion
// If this is not the case, the MI might require higher version MB support.

#if   defined (CIMC_VER_90) || defined(CIMC_VER_ALL)
   // added Ext function table and getKeyList
  #define CIMC_VER_87
  #define CIMC_VER_86
  #define CIMC_VER_85
  #define CIMC_VER_80
  #define CIMCCurrentVersion CIMCVersion090
#elif   defined (CIMC_VER_87) || defined(CIMC_VER_ALL)
   // added evaluateUsingAccessor in _CIMCSelectExp
  #define CIMC_VER_86
  #define CIMC_VER_85
  #define CIMC_VER_80
  #define CIMCCurrentVersion CIMCVersion087
#elif defined (CIMC_VER_86) || defined(CIMC_VER_ALL)
   // enable() disable() support in _CIMCIndicationMIFT
   // toString() in _CIMCObjectPathFT
   // support for NULL return from <mi-name>_Create<mi-type>MI
  #define CIMC_VER_85
  #define CIMC_VER_80
  #define CIMCCurrentVersion CIMCVersion086
#elif   defined (CIMC_VER_85) || defined(CIMC_VER_ALL)
   // getMessage() globalization support in _CIMCBrokerEncFT
  #define CIMC_VER_80
  #define CIMCCurrentVersion CIMCVersion085
#elif defined (CIMC_VER_80) || defined(CIMC_VER_ALL)
  #define CIMCCurrentVersion CIMCVersion080
#else  // default version
  #define CIMC_VER_90
  #define CIMC_VER_87
  #define CIMC_VER_86
  #define CIMC_VER_85
  #define CIMC_VER_80
  #define CIMCCurrentVersion CIMCVersion090
#endif


   struct _cimcInstance;
   struct _cimcObjectPath;
   struct _cimcArgs;
   struct _cimcEnumeration;
   struct _cimcArray;
   struct _cimcString;
   struct _cimcDateTime;
   struct _cimcConstClass;
   
   typedef struct _cimcInstance       cimcInstance;
   typedef struct _cimcObjectPath     cimcObjectPath;
   typedef struct _cimcArgs           cimcArgs;
   typedef struct _cimcEnumeration    cimcEnumeration;
   typedef struct _cimcArray          cimcArray;
   typedef struct _cimcString         cimcString;
   typedef struct _cimcDateTime       cimcDateTime;
   typedef struct _cimcConstClass     cimcConstClass;

   struct _cimcInstanceFT;
   struct _cimcObjectPathFT;
   struct _cimcArgsFT;
   struct _cimcEnumerationFT;
   struct _cimcArrayFT;
   struct _cimcStringFT;
   struct _cimcDateTimeFT;

   typedef struct _cimcInstanceFT      cimcInstanceFT;
   typedef struct _cimcObjectPathFT    cimcObjectPathFT;
   typedef struct _cimcArgsFT          cimcArgsFT;
   typedef struct _cimcEnumerationFT   cimcEnumerationFT;
   typedef struct _cimcArrayFT         cimcArrayFT;
   typedef struct _cimcStringFT        cimcStringFT;
   typedef struct _cimcDateTimeFT      cimcDateTimeFT;


   typedef unsigned char              cimcBoolean;
   typedef unsigned short             cimcChar16;
   typedef unsigned char              cimcUint8;
   typedef unsigned short             cimcUint16;
   typedef unsigned long              cimcUint32;
#ifndef CIMC_PLATFORM_WIN32_IX86_MSVC
   typedef unsigned long long         cimcUint64;
#else
   typedef unsigned __int64           cimcUint64;
#endif
   typedef signed char                cimcSint8;
   typedef short                      cimcSint16;
   typedef long                       cimcSint32;
#ifndef CIMC_PLATFORM_WIN32_IX86_MSVC
   typedef long long                  cimcSint64;
#else
   typedef __int64                    cimcSint64;
#endif
   typedef float                      cimcReal32;
   typedef double                     cimcReal64;

   typedef struct _cimcValuePtr {
     void *ptr;
     unsigned int length;
   } cimcValuePtr;

   typedef union _cimcValue {
        cimcUint64           uint64;
        cimcUint32           uint32;
        cimcUint16           uint16;
        cimcUint8            uint8;
        cimcSint64           sint64;
        cimcSint32           sint32;
        cimcSint16           sint16;
        cimcSint8            sint8;
        cimcReal64           real64;
        cimcReal32           real32;
        cimcBoolean          boolean;
        cimcChar16           char16;

        cimcConstClass*      cls;
        cimcInstance*        inst;
        cimcObjectPath*      ref;
        cimcArgs*            args;
        cimcEnumeration*     Enum;
        cimcArray*           array;
        cimcString*          string;
        char*                chars;
        cimcDateTime*        dateTime;
        cimcValuePtr         dataPtr;

        cimcSint8            Byte;
        cimcSint16           Short;
        cimcSint32           Int;
        cimcSint64           Long;
        cimcReal32           Float;
        cimcReal64           Double;
   } cimcValue;


   typedef unsigned short cimcType;

        #define CIMC_null         0

        #define CIMC_SIMPLE       (2)
        #define CIMC_boolean      (2+0)
        #define CIMC_char16       (2+1)

        #define CIMC_REAL         ((2)<<2)
        #define CIMC_real32       ((2+0)<<2)
        #define CIMC_real64       ((2+1)<<2)

        #define CIMC_UINT         ((8)<<4)
        #define CIMC_uint8        ((8+0)<<4)
        #define CIMC_uint16       ((8+1)<<4)
        #define CIMC_uint32       ((8+2)<<4)
        #define CIMC_uint64       ((8+3)<<4)
        #define CIMC_SINT         ((8+4)<<4)
        #define CIMC_sint8        ((8+4)<<4)
        #define CIMC_sint16       ((8+5)<<4)
        #define CIMC_sint32       ((8+6)<<4)
        #define CIMC_sint64       ((8+7)<<4)
        #define CIMC_INTEGER      ((CIMC_UINT | CIMC_SINT))

        #define CIMC_ENC          ((16)<<8)
        #define CIMC_instance     ((16+0)<<8)
        #define CIMC_ref          ((16+1)<<8)
        #define CIMC_args         ((16+2)<<8)
        #define CIMC_class        ((16+3)<<8)
        #define CIMC_filter       ((16+4)<<8)
        #define CIMC_enumeration  ((16+5)<<8)
        #define CIMC_string       ((16+6)<<8)
        #define CIMC_chars        ((16+7)<<8)
        #define CIMC_dateTime     ((16+8)<<8)
        #define CIMC_ptr          ((16+9)<<8)
        #define CIMC_charsptr     ((16+10)<<8)

        #define CIMC_ARRAY        ((1)<<13)
        #define CIMC_SIMPLEA      (CIMC_ARRAY | CIMC_SIMPLE)
        #define CIMC_booleanA     (CIMC_ARRAY | CIMC_boolean)
        #define CIMC_char16A      (CIMC_ARRAY | CIMC_char16)

        #define CIMC_REALA        (CIMC_ARRAY | CIMC_REAL)
        #define CIMC_real32A      (CIMC_ARRAY | CIMC_real32)
        #define CIMC_real64A      (CIMC_ARRAY | CIMC_real64)

        #define CIMC_UINTA        (CIMC_ARRAY | CIMC_UINT)
        #define CIMC_uint8A       (CIMC_ARRAY | CIMC_uint8)
        #define CIMC_uint16A      (CIMC_ARRAY | CIMC_uint16)
        #define CIMC_uint32A      (CIMC_ARRAY | CIMC_uint32)
        #define CIMC_uint64A      (CIMC_ARRAY | CIMC_uint64)
        #define CIMC_SINTA        (CIMC_ARRAY | CIMC_SINT)
        #define CIMC_sint8A       (CIMC_ARRAY | CIMC_sint8)
        #define CIMC_sint16A      (CIMC_ARRAY | CIMC_sint16)
        #define CIMC_sint32A      (CIMC_ARRAY | CIMC_sint32)
        #define CIMC_sint64A      (CIMC_ARRAY | CIMC_sint64)
        #define CIMC_INTEGERA     (CIMC_ARRAY | CIMC_INTEGER)

        #define CIMC_ENCA         (CIMC_ARRAY | CIMC_ENC)
        #define CIMC_stringA      (CIMC_ARRAY | CIMC_string)
        #define CIMC_charsA       (CIMC_ARRAY | CIMC_chars)
        #define CIMC_dateTimeA    (CIMC_ARRAY | CIMC_dateTime)
        #define CIMC_instanceA    (CIMC_ARRAY | CIMC_instance)
        #define CIMC_refA         (CIMC_ARRAY | CIMC_ref)
        #define CIMC_ptrA         (CIMC_ARRAY | CIMC_ptr)
        #define CIMC_charsptrA    (CIMC_ARRAY | CIMC_charsptr)

	// the following are cimcObjectPath key-types synonyms
	// and are valid only when CIMC_keyValue of cimcValueState is set

        #define CIMC_keyInteger   (CIMC_sint64)
	#define CIMC_keyString    (CIMC_string)
	#define CIMC_keyBoolean   (CIMC_boolean)
	#define CIMC_keyRef       (CIMC_ref)

	// the following are predicate types only

        #define CIMC_charString      (CIMC_string)
        #define CIMC_integerString   (CIMC_string | CIMC_sint64)
        #define CIMC_realString      (CIMC_string | CIMC_real64)
        #define CIMC_numericString   (CIMC_string | CIMC_sint64 | CIMC_real64)
	#define CIMC_booleanString   (CIMC_string | CIMC_boolean)
	#define CIMC_dateTimeString  (CIMC_string | CIMC_dateTime)
        #define CIMC_classNameString (CIMC_string | CIMC_class)
        #define CIMC_nameString      (CIMC_string | ((16+10)<<8))

   typedef unsigned short cimcValueState;
        #define CIMC_goodValue (0)
        #define CIMC_nullValue (1<<8)
        #define CIMC_keyValue  (2<<8)
        #define CIMC_notFound  (4<<8)
        #define CIMC_badValue  (0x80<<8)

   typedef struct _cimcData {
      cimcType type;
      cimcValueState state;
      cimcValue value;
   } cimcData;


#ifdef CIMC_VER_87
   typedef cimcData cimcAccessor(const char*, void* parm);
#endif


#ifndef CIMC_NO_SYNONYM_SUPPORT
   #define CIMC_Byte    CIMC_sint8
   #define CIMC_Short   CIMC_sint16
   #define CIMC_Int     CIMC_sint32
   #define CIMC_Long    CIMC_sint64
   #define CIMC_Float   CIMC_real32
   #define CIMC_Double  CIMC_real64

   #define CIMC_ByteA   CIMC_sint8A
   #define CIMC_ShortA  CIMC_sint16A
   #define CIMC_IntA    CIMC_sint32A
   #define CIMC_LongA   CIMC_sint64A
   #define CIMC_FloatA  CIMC_real32A
   #define CIMC_DoubleA CIMC_real64A
#endif // CIMC_NO_SYNONYM_SUPPORT

   typedef unsigned int cimcCount;


   typedef unsigned int cimcFlags;

   #define CIMC_FLAG_LocalOnly          1
   #define CIMC_FLAG_DeepInheritance    2
   #define CIMC_FLAG_IncludeQualifiers  4
   #define CIMC_FLAG_IncludeClassOrigin 8

   #define cimcInvocationFlags "cimcInvocationFlags"
   #define cimcPrincipal "cimcPrincipal"

   typedef enum _cimcrc {
     CIMC_RC_OK                               =0,
     CIMC_RC_ERR_FAILED                       =1,
     CIMC_RC_ERR_ACCESS_DENIED                =2,
     CIMC_RC_ERR_INVALID_NAMESPACE            =3,
     CIMC_RC_ERR_INVALID_PARAMETER            =4,
     CIMC_RC_ERR_INVALID_CLASS                =5,
     CIMC_RC_ERR_NOT_FOUND                    =6,
     CIMC_RC_ERR_NOT_SUPPORTED                =7,
     CIMC_RC_ERR_CLASS_HAS_CHILDREN           =8,
     CIMC_RC_ERR_CLASS_HAS_INSTANCES          =9,
     CIMC_RC_ERR_INVALID_SUPERCLASS           =10,
     CIMC_RC_ERR_ALREADY_EXISTS               =11,
     CIMC_RC_ERR_NO_SUCH_PROPERTY             =12,
     CIMC_RC_ERR_TYPE_MISMATCH                =13,
     CIMC_RC_ERR_QUERY_LANGUAGE_NOT_SUPPORTED =14,
     CIMC_RC_ERR_INVALID_QUERY                =15,
     CIMC_RC_ERR_METHOD_NOT_AVAILABLE         =16,
     CIMC_RC_ERR_METHOD_NOT_FOUND             =17,
     CIMC_RC_DO_NOT_UNLOAD                    =50,
     CIMC_RC_NEVER_UNLOAD                     =51,
     CIMC_RC_ERROR_SYSTEM                     =100,
     CIMC_RC_ERROR                            =200
   } cimcrc;

   typedef struct _cimcStatus {
      cimcrc rc;
      cimcString *msg;
   } cimcStatus;


   /* Management Broker classification and feature support */

   #define CIMC_MB_Class_0     0x00000001
   #define CIMC_MB_Class_1     0x00000003
   #define CIMC_MB_Class_2     0x00000007

   #define CIMC_MB_Supports_PropertyMI         0x00000100
   #define CIMC_MB_Supports_IndicationMI       0x00000200
   #define CIMC_MB_Supports_IndicationPolling  0x00000400
   #define CIMC_MB_Supports_QueryNormalization 0x00000800
   #define CIMC_MB_Supports_Qualifier          0x00001000
   #define CIMC_MB_Supports_Schema             0x00003000


#ifdef __cplusplus
 };
#endif

#endif // _CIMCDT_H_



