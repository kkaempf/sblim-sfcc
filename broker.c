/*!
  \file broker.c
  \brief Native CMPI broker encapsulated functionality.

  This module implements a complete CMPI broker encapsulated function table
  (CMPIBrokerEncFT) natively. Thus, CMPI data types may be created remotely
  without the need to connect to the CIMOM.

  (C) Copyright IBM Corp. 2003
 
  THIS FILE IS PROVIDED UNDER THE TERMS OF THE COMMON PUBLIC LICENSE 
  ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE 
  CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 
  You can obtain a current copy of the Common Public License from
  http://oss.software.ibm.com/developerworks/opensource/license-cpl.html

  \author Frank Scheffler
  $Revision: 1.2 $
*/

#include "cmpidt.h"
#include "cmpift.h"
#include "cmpimacs.h"
#include "tool.h"
#include "native.h"
#include "debug.h"


static CMPIInstance * __beft_newInstance ( CMPIBroker * broker,
					   CMPIObjectPath * cop,
					   CMPIStatus * rc )
{
	TRACE_NORMAL(("Creating new native CMPIInstance."));
	return native_new_CMPIInstance ( cop, rc );
}


static CMPIObjectPath * __beft_newObjectPath ( CMPIBroker * broker,
					       char * nameSpace,
					       char * classname,
					       CMPIStatus * rc )
{
	TRACE_NORMAL(("Creating new native CMPIObjectPath."));
	return native_new_CMPIObjectPath ( nameSpace, classname, rc );
}


static CMPIArgs * __beft_newArgs ( CMPIBroker * broker, CMPIStatus * rc )
{
	TRACE_NORMAL(("Creating new native CMPIArgs."));
	return native_new_CMPIArgs ( rc );
}


static CMPIString * __beft_newString ( CMPIBroker * broker,
				       char * str,
				       CMPIStatus * rc )
{
	TRACE_NORMAL(("Creating new native CMPIString."));
	TRACE_INFO(("String: %s", str ));
	return native_new_CMPIString ( str, rc );
}


static CMPIArray * __beft_newArray ( CMPIBroker * broker,
				     CMPICount size,
				     CMPIType type,
				     CMPIStatus * rc )
{
	TRACE_NORMAL(("Creating new native CMPIArray."));
	TRACE_INFO(("type: 0x%x\nsize: %d", type, size ));
	return native_new_CMPIArray ( size, type, rc );
}


static CMPIDateTime * __beft_newDateTime ( CMPIBroker * broker,
					   CMPIStatus * rc )
{
	TRACE_NORMAL(("Creating new native CMPIDateTime."));
	return native_new_CMPIDateTime ( rc );
}


static CMPIDateTime * __beft_newDateTimeFromBinary ( CMPIBroker * broker,
						     CMPIUint64 time,
						     CMPIBoolean interval,
						     CMPIStatus * rc )
{
	TRACE_NORMAL(("Creating new native CMPIDateTime."));
	TRACE_INFO(("time: %lld\ninterval: %d", time, interval ));
	return native_new_CMPIDateTime_fromBinary ( time, interval, rc );
}


static CMPIDateTime * __beft_newDateTimeFromChars ( CMPIBroker * broker,
						    char * string,
						    CMPIStatus * rc )
{
	TRACE_NORMAL(("Creating new native CMPIDateTime."));
	TRACE_INFO(("time: %s", string ));
	return native_new_CMPIDateTime_fromChars ( string, rc );
}


static CMPISelectExp * __beft_newSelectExp ( CMPIBroker * broker,
					     char * queryString,
					     char * language,
					     CMPIArray ** projection,
					     CMPIStatus * rc )
{
	TRACE_NORMAL(("Creating new native CMPISelectExp."));
	return native_new_CMPISelectExp ( queryString,
					  language,
					  projection,
					  rc );
}


static CMPIBoolean __beft_classPathIsA ( CMPIBroker * broker,
					 CMPIObjectPath * cop,
					 char * type,
					 CMPIStatus * rc )
{
	TRACE_CRITICAL(("This operation is not yet supported."));
	if ( rc ) CMSetStatus ( rc, CMPI_RC_ERR_NOT_SUPPORTED );
	return 0;
}


static CMPIString * __beft_toString ( CMPIBroker * broker,
				      void * object,
				      CMPIStatus * rc )
{
	TRACE_CRITICAL(("This operation is not yet supported."));
	if ( rc ) CMSetStatus ( rc, CMPI_RC_ERR_NOT_SUPPORTED );
	return NULL;
}


static CMPIBoolean __beft_isOfType ( CMPIBroker * broker,
				     void * object,
				     char * type,
				     CMPIStatus * rc )
{
	char * t = * ( (char **) object );

	TRACE_NORMAL(("Verifying encapsulated object type."));

	if ( rc ) CMSetStatus ( rc, CMPI_RC_OK );
	return ( strcmp ( t, type ) == 0 );
}


static CMPIString * __beft_getType ( CMPIBroker * broker,
				     void * object,
				     CMPIStatus * rc )
{
	TRACE_NORMAL(("Returning encapsulated object type."));
	return __beft_newString ( broker, *( (char **) object ), rc );
}

/****************************************************************************/


CMPIBrokerEncFT native_brokerEncFT = {
	NATIVE_FT_VERSION,
	__beft_newInstance,
	__beft_newObjectPath,
	__beft_newArgs,
	__beft_newString,
	__beft_newArray,
	__beft_newDateTime,
	__beft_newDateTimeFromBinary,
	__beft_newDateTimeFromChars,
	__beft_newSelectExp,
	__beft_classPathIsA,
	__beft_toString,
	__beft_isOfType,
	__beft_getType
};

/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
