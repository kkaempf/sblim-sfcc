/*!
  \file instance.c
  \brief Native CMPIInstance implementation.

  This is the native CMPIInstance implementation as used for remote
  providers. It reflects the well-defined interface of a regular
  CMPIInstance, however, it works independently from the management broker.
  
  It is part of a native broker implementation that simulates CMPI data
  types rather than interacting with the entities in a full-grown CIMOM.

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
#include <stdlib.h>
#include <string.h>
#include "cmcidt.h"
#include "cmcift.h"
#include "cmcimacs.h"
#include "tool.h"
#include "native.h"


/****************************************************************************/

static void __release_list ( char ** list )
{
	if ( list ) {

		char ** tmp = list;

		while ( *tmp ) tool_mm_add ( *tmp++ );
		tool_mm_add ( list );
	}
}


static char ** __duplicate_list ( char ** list, int mem_state )
{
	char ** result = NULL;
	
	if ( list ) {
		size_t size = 1;
		char ** tmp = list;

		while ( *tmp++ ) ++size;

		result = tool_mm_alloc ( mem_state, size * sizeof ( char * ) );

		for ( tmp = result; *list; tmp++ ) {

			*tmp = strdup ( *list++ );
			if ( mem_state == TOOL_MM_ADD ) tool_mm_add ( *tmp );
		}
	}

	return result;
}


static int __contained_list ( char ** list, const char * name )
{
	if ( list ) {

		while ( *list )
			if ( strcmp ( *list++, name ) == 0 ) return 1;
	}
	return 0;
}


/****************************************************************************/


static CMPIStatus __ift_release ( CMPIInstance * instance )
{
	struct native_instance * i = (struct native_instance *) instance;

	if ( i->mem_state == TOOL_MM_NO_ADD ) {

		i->mem_state = TOOL_MM_ADD;

		tool_mm_add ( i );
		tool_mm_add ( i->classname );
		tool_mm_add ( i->namespace );
		
		__release_list ( i->property_list );
		__release_list ( i->key_list );

		propertyFT.release ( i->props );

		CMReturn ( CMPI_RC_OK );
	}

	CMReturn ( CMPI_RC_ERR_FAILED );
}


static CMPIInstance * __ift_clone ( CMPIInstance * instance, CMPIStatus * rc )
{
	struct native_instance * i   = (struct native_instance *) instance;
	struct native_instance * new = 
		(struct native_instance *) 
		tool_mm_alloc ( TOOL_MM_NO_ADD,
				sizeof ( struct native_instance ) );

	new->classname     = strdup ( i->classname );
	new->namespace     = strdup ( i->namespace );
	new->property_list = __duplicate_list ( i->property_list,
						TOOL_MM_NO_ADD );
	new->key_list      = __duplicate_list ( i->key_list, TOOL_MM_NO_ADD );
	new->props     = propertyFT.clone ( i->props, rc );

	return (CMPIInstance *) new;
}


static CMPIData __ift_getProperty ( CMPIInstance * instance,
				    char * name,
				    CMPIStatus * rc )
{
	struct native_instance * i = (struct native_instance *) instance;

	return propertyFT.getDataProperty ( i->props, name, rc );
}


static CMPIData __ift_getPropertyAt ( CMPIInstance * instance, 
				      unsigned int index,
				      CMPIString ** name,
				      CMPIStatus * rc )
{
	struct native_instance * i = (struct native_instance *) instance;

	return propertyFT.getDataPropertyAt ( i->props, index, name, rc );
}


static unsigned int __ift_getPropertyCount ( CMPIInstance * instance, 
					     CMPIStatus * rc )
{
	struct native_instance * i = (struct native_instance *) instance;
  
	return propertyFT.getPropertyCount ( i->props, rc );
}


static CMPIStatus __ift_setProperty ( CMPIInstance * instance,
				      char * name,
				      CMPIValue * value,
				      CMPIType type )
{
	struct native_instance * i = (struct native_instance *) instance;

	if ( i->filtered == 0 ||
	     i->property_list == NULL ||
	     __contained_list ( i->property_list, name ) ||
	     __contained_list ( i->key_list, name ) ) {

		if ( propertyFT.setProperty ( i->props,
					      i->mem_state,
					      name, 
					      type,
					      value ) ) {
			propertyFT.addProperty ( &i->props, 
						 i->mem_state,
						 name, 
						 type, 
						 0, 
						 value );
		}

	}
	CMReturn ( CMPI_RC_OK );
}


static CMPIObjectPath * __ift_getObjectPath ( CMPIInstance * instance,
					      CMPIStatus * rc )
{
	int j;
	struct native_instance * i = (struct native_instance *) instance;   

	CMPIObjectPath * cop   = newCMPIObjectPath ( i->namespace,
							     i->classname,
							     rc );

	if ( rc && rc->rc != CMPI_RC_OK )
		return NULL;

	j = propertyFT.getPropertyCount ( i->props, NULL );

	while ( j-- ) {
		CMPIStatus tmp;
		CMPIString * keyName;
		CMPIData d = propertyFT.getDataPropertyAt ( i->props,
							    j,   
							    &keyName,
							    &tmp );
		if ( d.state & CMPI_keyValue ) {
			CMAddKey ( cop,
				   CMGetCharPtr ( keyName ),
				   &d.value, 
				   d.type );
		}
	}
	return cop;
}


static CMPIStatus __ift_setPropertyFilter ( CMPIInstance * instance,
					    char ** propertyList,
					    char ** keys )
{  

	struct native_instance * i = (struct native_instance *) instance;

	if ( i->filtered && i->mem_state == TOOL_MM_NO_ADD ) {

		__release_list ( i->property_list );
		__release_list ( i->key_list );
	}

	i->filtered = 1;
	i->property_list = __duplicate_list ( propertyList, i->mem_state );
	i->key_list      = __duplicate_list ( keys, i->mem_state );

	CMReturn ( CMPI_RC_OK );
}   



CMPIInstance * native_new_CMPIInstance ( CMPIObjectPath * cop,
					 CMPIStatus * rc )
{
	static CMPIInstanceFT ift = {
		NATIVE_FT_VERSION,
		__ift_release,
		__ift_clone,
		__ift_getProperty,
		__ift_getPropertyAt,
		__ift_getPropertyCount,
		__ift_setProperty,
		__ift_getObjectPath,
		__ift_setPropertyFilter
	};
	static CMPIInstance i = {
		"CMPIInstance",
		&ift
	};

	struct native_instance * instance =
		(struct native_instance *) 
		tool_mm_alloc ( TOOL_MM_ADD, sizeof ( struct native_instance ) );

	CMPIStatus tmp1, tmp2, tmp3;
	CMPIString * str;

	instance->instance     = i;
	instance->mem_state    = TOOL_MM_ADD;

	if (cop) {
	   int j = CMGetKeyCount ( cop, &tmp1 );
           str = CMGetClassName ( cop, &tmp2 );
	   instance->classname = CMGetCharPtr ( str );
	   str = CMGetNameSpace ( cop, &tmp3 );
	   instance->namespace = CMGetCharPtr ( str );

	   if ( tmp1.rc != CMPI_RC_OK ||
	        tmp2.rc != CMPI_RC_OK ||
	        tmp3.rc != CMPI_RC_OK ) {
		   if ( rc ) CMSetStatus ( rc, CMPI_RC_ERR_FAILED );
	   } 
           else {
	     while ( j-- && ( tmp1.rc == CMPI_RC_OK ) ) {
			CMPIString * keyName;
			CMPIData tmp = CMGetKeyAt ( cop, j, &keyName, &tmp1 );
			propertyFT.addProperty ( &instance->props,
						 TOOL_MM_ADD,
						 CMGetCharPtr ( keyName ),
						 tmp.type,
						 tmp.state,
						 &tmp.value );
		}

                
		if ( rc ) CMSetStatus ( rc, tmp1.rc );
	   }
        }   

	return (CMPIInstance *) instance;
}

void setInstNsAndCn(CMPIInstance *ci, char *ns, char *cn)
{
	struct native_instance * i = (struct native_instance *) ci;
   if (cn) i->classname=strdup(cn);
   if (ns) i->namespace=strdup(ns);
}

/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
