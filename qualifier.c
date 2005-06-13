/*!
  \file qualifier.c
  \brief Native qualifier implementation.

  This module implements a native qualifier, which is not public to any
  provider programmer. It is used to implement various other data types
  natively, such as instances, object-paths and args.

  It provides means to maintain linked lists of named properties including
  functionality to add, remove, clone and release them.

  (C) Copyright IBM Corp. 2003
 
  THIS FILE IS PROVIDED UNDER THE TERMS OF THE COMMON PUBLIC LICENSE 
  ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE 
  CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 
  You can obtain a current copy of the Common Public License from
  http://oss.software.ibm.com/developerworks/opensource/license-cpl.html

  \author Frank Scheffler
  $Revision: 1.1 $
*/

#include <stdio.h>
#include <string.h>
#include "cmcidt.h"
#include "cmcift.h"
#include "cmcimacs.h"
#include "tool.h"
#include "native.h"

//! Storage container for commonly needed data within native CMPI data types.
/*!
  This structure is used to build linked lists of data containers as needed
  for various native data types.
*/
struct native_qualifier {
	char * name;		        //!< Qualifier identifier.
	CMPIType type;		        //!< Associated CMPIType.
	CMPIValueState state; 	        //!< Current value state.
	CMPIValue value;	        //!< Current value.
	struct native_qualifier * next;	//!< Pointer to next qualifier.
};


/****************************************************************************/

static CMPIData __convert2CMPIData ( struct native_qualifier * qual,
				     CMPIString ** qualname )
{
	CMPIData result;

	if ( qual != NULL ) {
		result.type  = qual->type;
		result.state = qual->state;
		result.value = qual->value;

		if ( qualname ) {
			*qualname  = native_new_CMPIString ( qual->name,
							     NULL );
		}

	} else {
		result.state = CMPI_nullValue;
	}

	return result;
}


/**
 * returns non-zero if already existant
 */
static int __addQualifier ( struct native_qualifier ** qual,
			   int mm_add, 
			   const char * name,
			   CMPIType type,  
			   CMPIValueState state, 
			   CMPIValue * value )
{
	CMPIValue v;

	if ( *qual == NULL ) {
		struct native_qualifier * tmp = *qual =
			(struct native_qualifier *) 
			tool_mm_alloc ( mm_add,
					sizeof ( struct native_qualifier ) );
  
		tmp->name = strdup ( name );

		if ( mm_add == TOOL_MM_ADD ) tool_mm_add ( tmp->name );

		if ( type == CMPI_chars ) {

			type = CMPI_string;
			v.string = native_new_CMPIString ( (char *) value,
							   NULL );
			value = &v;
		}

		tmp->type  = type;

		if ( type != CMPI_null ) {
			tmp->state = state;

			if ( mm_add == TOOL_MM_ADD ) {

				tmp->value = *value;
			} else {
			
				CMPIStatus rc;
				tmp->value = native_clone_CMPIValue ( type,
								      value,
								      &rc );
				// what if clone() fails???
			}
		} else tmp->state = CMPI_nullValue;

		return 0;
	}
	return ( strcmp ( (*qual)->name, name ) == 0 ||
		 __addQualifier ( &( (*qual)->next ), 
				 mm_add, 
				 name, 
				 type, 
				 state, 
				 value ) );
}


/**
 * returns -1 if non-existant
 */
static int __setQualifier ( struct native_qualifier * qual, 
			   int mm_add,
			   const char * name, 
			   CMPIType type,
			   CMPIValue * value )
{
	CMPIValue v;
	if ( qual == NULL ) {
		return -1;
	}

	if ( strcmp ( qual->name, name ) == 0 ) {

		CMPIStatus rc;

		if ( ! ( qual->state & CMPI_nullValue ) )
			native_release_CMPIValue ( qual->type, &qual->value );

		if ( type == CMPI_chars ) {

			type = CMPI_string;
			v.string = native_new_CMPIString ( (char *) value,
							   NULL );
			value = &v;
		}

		qual->type  = type;

		if ( type != CMPI_null ) {
			qual->value =
				( mm_add == TOOL_MM_ADD )?
				*value:
				native_clone_CMPIValue ( type, value, &rc );

			// what if clone() fails ???

		} else qual->state = CMPI_nullValue;

		return 0;
	}
	return __setQualifier ( qual->next, mm_add, name, type, value);
}


static struct native_qualifier * __getQualifier ( struct native_qualifier * qual, 
						const char * name )
{
	if ( ! qual || ! name ) {
		return NULL;
	}
	return ( strcmp ( qual->name, name ) == 0 )?
		qual: __getQualifier ( qual->next, name );
}


static CMPIData __getDataQualifier ( struct native_qualifier * qual, 
				    const char * name,
				    CMPIStatus * rc )
{
	struct native_qualifier * p = __getQualifier ( qual, name );

	if ( rc ) CMSetStatus ( rc,
				( p )?
				CMPI_RC_OK:
				CMPI_RC_ERR_NO_SUCH_PROPERTY );

	return __convert2CMPIData ( p, NULL );
}


static struct native_qualifier * __getQualifierAt
( struct native_qualifier * qual, unsigned int pos )
{
	if ( ! qual ) {
		return NULL;
	} 

	return ( pos == 0 )?
		qual: __getQualifierAt ( qual->next, --pos );
}


static CMPIData __getDataQualifierAt ( struct native_qualifier * qual, 
				      unsigned int pos,
				      CMPIString ** qualname,
				      CMPIStatus * rc )
{
	struct native_qualifier * p = __getQualifierAt ( qual, pos );

	if ( rc ) CMSetStatus ( rc,
				( p )?
				CMPI_RC_OK:
				CMPI_RC_ERR_NO_SUCH_PROPERTY );

	return __convert2CMPIData ( p, qualname );
}


static CMPICount __getQualifierCount ( struct native_qualifier * qual,
				      CMPIStatus * rc )
{
	CMPICount c = 0;

	if ( rc ) CMSetStatus ( rc, CMPI_RC_OK );

	while ( qual != NULL ) {
		c++;
		qual = qual->next;
	}

	return c;
}


static void __release ( struct native_qualifier * qual )
{
	for ( ; qual; qual = qual->next ) {
		tool_mm_add ( qual );
		tool_mm_add ( qual->name );
		native_release_CMPIValue ( qual->type, &qual->value );
	}
}


static struct native_qualifier * __clone ( struct native_qualifier * qual,
					  CMPIStatus * rc )
{
	struct native_qualifier * result;
	CMPIStatus tmp;

	if ( qual == NULL ) {

		if ( rc ) CMSetStatus ( rc, CMPI_RC_OK );
		return NULL;
	}

	result = 
		(struct native_qualifier * ) 
		tool_mm_alloc ( TOOL_MM_NO_ADD,
				sizeof ( struct native_qualifier ) );

	result->name  = strdup ( qual->name );
	result->type  = qual->type;
	result->state = qual->state;
	result->value = native_clone_CMPIValue ( qual->type,
						 &qual->value,
						 &tmp );

	if ( tmp.rc != CMPI_RC_OK ) {

		result->state = CMPI_nullValue;
	}
  
	result->next  = __clone ( qual->next, rc );
	return result;
}


/**
 * Global function table to access native_qualifier helper functions.
 */
struct native_qualifierFT qualifierFT = {
	__addQualifier,
	__setQualifier,
	__getDataQualifier,
	__getDataQualifierAt,
	__getQualifierCount,
	__release,
	__clone
};

/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
