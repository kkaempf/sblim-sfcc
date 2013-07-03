/*!
  \file enumeration.c
  \brief Native CMPIEnumeration implementation.

  This is the native CMPIEnumeration implementation as used for remote
  providers. It reflects the well-defined interface of a regular
  CMPIEnumeration, however, it works independently from the management broker.
  
  It is part of a native broker implementation that simulates CMPI data
  types rather than interacting with the entities in a full-grown CIMOM.

  (C) Copyright IBM Corp. 2003
 
  THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE 
  ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE 
  CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 
  You can obtain a current copy of the Eclipse Public License from
  http://www.opensource.org/licenses/eclipse-1.0.php

  \author Frank Scheffler
  $Revision: 1.3 $
*/

#include "cmcidt.h"
#include "cmcift.h"
#include "cmcimacs.h"

struct _CMCIConnectionFT;                           // new 
typedef struct _CMCIConnectionFT CMCIConnectionFT;  // new
#include <curl/curl.h>         // new
#include "cimXmlParser.h"      // new
#include "utilStringBuffer.h"  // new

#include "native.h"
#include <time.h>              // new
#include <sys/time.h>          // new
#include "conn.h"              // new

static CMPIStatus __eft_release ( CMPIEnumeration * );
static CMPIEnumeration * __eft_clone ( CMPIEnumeration * , CMPIStatus *);
static CMPIData __eft_getNext ( CMPIEnumeration * ,CMPIStatus * );
static CMPIArray * __eft_toArray ( CMPIEnumeration * ,CMPIStatus * );
static CMPIBoolean __eft_hasNext ( CMPIEnumeration * , CMPIStatus * );
static struct native_enum * __new_enumeration ( CMPIArray *, CMPIStatus * );

#ifdef DMALLOC
#include "dmalloc.h"
#endif

/*****************************************************************************/

static CMPIStatus __eft_release ( CMPIEnumeration * enumeration )
{
	struct native_enum * e = (struct native_enum *) enumeration;
	CMPIStatus st= { CMPI_RC_OK, NULL };
	if (e) {
		if (e->data)
		  st = CMRelease(e->data);
		free ( enumeration );
		return st;
	}

	CMReturn ( CMPI_RC_ERR_FAILED );
}


static CMPIEnumeration * __eft_clone ( CMPIEnumeration * enumeration,
				       CMPIStatus * rc )
{
	CMPIStatus tmp;
	struct native_enum * e = (struct native_enum *) enumeration;
	CMPIArray * data       = CMClone ( e->data, &tmp );

	if ( tmp.rc != CMPI_RC_OK ) {
		CMSetStatus ( rc, CMPI_RC_ERR_FAILED );
		return NULL;
	}

	return 
		(CMPIEnumeration *) __new_enumeration ( data, rc );
}


static CMPIData __eft_getNext ( CMPIEnumeration * enumeration,
				CMPIStatus * rc )
{
	struct native_enum * e = (struct native_enum *) enumeration;
	return CMGetArrayElementAt ( e->data, e->current++, rc );
}

static CMPIBoolean __eft_hasNext ( CMPIEnumeration * enumeration,
				   CMPIStatus * rc )
{
	struct native_enum * e = (struct native_enum *) enumeration;
	return ( e->current < CMGetArrayCount ( e->data, rc ) );
}

static CMPIArray * __eft_toArray ( CMPIEnumeration * enumeration,
				   CMPIStatus * rc )
{
	struct native_enum * e = (struct native_enum *) enumeration;
	CMSetStatus ( rc, CMPI_RC_OK );
	return e->data;
}


static struct native_enum * __new_enumeration ( CMPIArray * array,
						CMPIStatus * rc )
{
	static const CMPIEnumerationFT eft = {
		NATIVE_FT_VERSION,
		__eft_release,
		__eft_clone,
		__eft_getNext,
		__eft_hasNext,
		__eft_toArray
	};
	static const CMPIEnumeration e = {
		"CMPIEnumeration",
		(CMPIEnumerationFT*)&eft
	};

	struct native_enum * enumeration = (struct native_enum *)
		calloc ( 1, sizeof ( struct native_enum ) );

  enumeration->enumeration = e;
  enumeration->data = array; 	/* CMClone ( array, rc ) ? */
  CMSetStatus ( rc, CMPI_RC_OK );
  return enumeration;
}


CMPIEnumeration * native_new_CMPIEnumeration ( CMPIArray * array,
					       CMPIStatus * rc )
{
	return (CMPIEnumeration *) __new_enumeration ( array, rc );
}


/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
