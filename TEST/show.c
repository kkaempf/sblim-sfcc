/*
 * show.c
 *
 * (C) Copyright IBM Corp. 2005
 * (C) Copyright Intel Corp. 2005
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
 *    Test case show functions for instance, object paths
 */
#include <cmci.h>
#include <native.h>
#include <unistd.h>
#include <string.h>

#include "show.h"

char *
type2Chars (CMPIType type)
{
   if (type & CMPI_ARRAY) {
     char buf[512];
     snprintf(buf,511, "Array of %s", type2Chars(type & ~CMPI_ARRAY));
     return strdup(buf);
   }
   else if (type & CMPI_ENC) {

      switch (type) {
      case CMPI_instance:
	return "Instance";
      case CMPI_ref:
	return "Reference";
      case CMPI_args:
	return "Args";
      case CMPI_filter:
	return "Filter";
      case CMPI_string:
	return "String";
      case CMPI_chars:
	return "Chars";
      case CMPI_numericString:
	return "NumericString";
      case CMPI_booleanString:
	return "BooleanString";
      case CMPI_dateTimeString:
	return "DateTimeString";
      case CMPI_dateTime:
	return "DateTime";
      default:
	return "***Encoding***";
      }

   }
   else if (type & CMPI_SIMPLE) {

      switch (type) {
      case CMPI_boolean:
	return "Boolean";
      case CMPI_char16:
	return "Char16";
      default:
	return "***Simple***";
      }

   }
   else if (type & CMPI_INTEGER) {

      switch (type) {
      case CMPI_uint8:
	return "UInt8";
      case CMPI_sint8:
	return "SInt8";
      case CMPI_uint16:
	return "UInt16";
      case CMPI_sint16:
	return "SInt16";
      case CMPI_uint32:
	return "UInt32";
      case CMPI_sint32:
	return "SInt32";
      case CMPI_uint64:
	return "UInt64";
      case CMPI_sint64:
	return "SInt64";
      default:
	return "***Integer***";
      }

   }
   else if (type & CMPI_REAL) {

      switch (type) {
      case CMPI_real32:
	return "Real32";
      case CMPI_real64:
	return "Real64";
      default:
	return "***Real***";
      }

   }
   return "***Unknown***";
}


void showObjectPath( CMPIObjectPath * objectpath )
{
   CMPIString * namespace = objectpath->ft->getNameSpace(objectpath, NULL);
   CMPIString * classname = objectpath->ft->getClassName(objectpath, NULL);
   int numkeys = objectpath->ft->getKeyCount(objectpath, NULL);
   int i;
   char *cv;

    if (namespace && namespace->hdl)
    {
        printf("namespace=%s\n", (char *)namespace->hdl);
    }

    if (classname && classname->hdl)
    {
        printf("classname=%s\n", (char *)classname->hdl);
    }

    if (numkeys)
    {
      printf("keys:\n");
        for (i=0; i<numkeys; i++)
        {
         CMPIString * keyname;
            CMPIData data = objectpath->ft->getKeyAt(objectpath, i,
                                                     &keyname, NULL);
            printf("\t%s=%s\n", (char *)keyname->hdl,
                   cv=value2Chars(data.type, &data.value));
	 if(cv) free(cv);
	 if(keyname) CMRelease(keyname);
      }
   }

   if (classname) CMRelease(classname); 
   if (namespace) CMRelease(namespace);
}

static char *CMPIState_str(CMPIValueState state)
{
    char *retval;

    switch (state) {
    case CMPI_goodValue:
        retval = "CMPI_goodValue";
        break;

    case CMPI_nullValue:
        retval = "CMPI_nullValue";
        break;

    case CMPI_keyValue:
        retval = "CMPI_keyValue";
        break;

    case CMPI_notFound:
        retval = "CMPI_notFound";
        break;

    case CMPI_badValue:
        retval = "CMPI_badValue";
        break;

    default:
        retval = "!Unknown CMPIValueState!";
        break;
    }

    return retval;
}

void showProperty( CMPIData data, char *name )
{
	 char *valuestr;
  CMPIValueState state = data.state & ~CMPI_keyValue;
  if (state != CMPI_badValue)
    {
      if (CMIsArray(data) && state != CMPI_nullValue)
        {
	     CMPIArray *arr   = data.value.array;
	     CMPIType  eletyp = data.type & ~CMPI_ARRAY;
	     int j, n;
	     n = CMGetArrayCount(arr, NULL);
            for (j = 0; j < n; ++j)
            {
		CMPIData ele = CMGetArrayElementAt(arr, j, NULL);
	        if (state == CMPI_goodValue)
	          valuestr = value2Chars(eletyp, &ele.value);
	        else
		  valuestr = NULL;
	        printf("\t%s[%d]=%s<%s>\n", name, j, (state == CMPI_nullValue)?"NIL":valuestr, type2Chars(ele.type));
		if (valuestr) free (valuestr);
	     }
        }
        else
        {
	  if (state == CMPI_goodValue)
	    valuestr = value2Chars(data.type, &data.value);
	  else
	    valuestr = NULL;
    
	  printf("\t%s=%s<%s>\n", name, (state == CMPI_nullValue)?"NIL":valuestr, type2Chars(data.type));
	  if (valuestr) free (valuestr);
	}         
    }
    else
    {
        printf("\t%s=%d(%s)\n", name, data.state, CMPIState_str(data.state));
    }
}

void showInstance( CMPIInstance *instance )
{
   CMPIObjectPath * objectpath = instance->ft->getObjectPath(instance, NULL);
   CMPIString * objectpathname = objectpath->ft->toString(objectpath, NULL);
   CMPIString * namespace = objectpath->ft->getNameSpace(objectpath, NULL);
   CMPIString * classname = objectpath->ft->getClassName(objectpath, NULL);
   int numkeys = objectpath->ft->getKeyCount(objectpath, NULL);
   int numproperties = instance->ft->getPropertyCount(instance, NULL);
   int i;

    if (objectpathname && objectpathname->hdl)
    {
        printf("objectpath=%s\n", (char *)objectpathname->hdl);
    }

    if (namespace && namespace->hdl)
    {
        printf("namespace=%s\n", (char *)namespace->hdl);
    }

    if (classname && classname->hdl)
    {
        printf("classname=%s\n", (char *)classname->hdl);
    }

    if (numkeys)
    {
      printf("keys:\n");
        for (i=0; i<numkeys; i++)
        {
         CMPIString * keyname;
            CMPIData data = objectpath->ft->getKeyAt(objectpath, i,
                                                     &keyname, NULL);
         char *ptr=NULL;
            printf("\t%s=%s\n", (char *)keyname->hdl,
                   (ptr=value2Chars(data.type, &data.value)));
         free(ptr);
	 if (keyname) CMRelease(keyname);
      }
   }
   else
    {
      printf("No keys!\n");
    }

    if (numproperties)
    {
      printf("properties:\n");
        for (i=0; i<numproperties; i++)
        {
         CMPIString * propertyname;
            CMPIData data = instance->ft->getPropertyAt(instance, i,
                                                        &propertyname, NULL);
         showProperty( data, (char *)propertyname->hdl );
	 CMRelease(propertyname);
      }
   }
   else
    {
      printf("No properties!\n");
    }

   if (classname) CMRelease(classname);
   if (namespace) CMRelease(namespace);
   if (objectpathname) CMRelease(objectpathname);
   if (objectpath) CMRelease(objectpath);
}


void showClass( CMPIConstClass * class )
{
   CMPIString * classname = class->ft->getClassName(class, NULL);
   int numqualifiers = class->ft->getQualifierCount(class, NULL);
   int numproperties = class->ft->getPropertyCount(class, NULL);
   int i;
   char *cv;

    if (classname && classname->hdl)
    {
        printf("classname=%s\n", (char *)classname->hdl);
    }

    if (numqualifiers)
    {
      int j;
      printf("%d qualifiers:\n", numqualifiers);
      for (j = 0; j < numqualifiers; ++j) {
	CMPIString *qualifier_name;
	CMPIData data = class->ft->getQualifierAt(class, j, &qualifier_name, NULL);
	printf("[%s]", CMGetCharPtr(qualifier_name));
      }
    }
    else
    {
      printf("No qualifiers\n");
    }
  
    if (numproperties)
    {
      printf("%d properties:\n", numproperties);
        for (i=0; i<numproperties; i++)
        {
	  CMPIString * propertyname;
	  CMPIData data = class->ft->getPropertyAt(class, i,
						   &propertyname, NULL);
	  if (propertyname) {
#if 1
	    int property_qualifier_count = class->ft->getPropertyQualifierCount(class, CMGetCharPtr(propertyname), NULL);
	    int j;
	    for (j = 0; j < property_qualifier_count; ++j) {
	       CMPIString *property_qualifier_name;
	       CMPIData data = class->ft->getPropertyQualifierAt(class, CMGetCharPtr(propertyname), j, &property_qualifier_name, NULL);
	       printf("[%s]", CMGetCharPtr(property_qualifier_name));
	    }
#else
	    CMPIData data = class->ft->getPropertyQualifier(class,CMGetCharPtr(propertyname),"KEY",NULL);
	    if (data.state != CMPI_nullValue && data.value.boolean) {
	      printf ("[KEY]");
	    }
	    printf("%2d",property_qualifier_count);
#endif
	  }

	  if (data.state==0)
	    cv=value2Chars(data.type, &data.value);
	  else
	    cv=NULL;
	  printf("\t%s=%s<%s>\n", (char *)propertyname->hdl,cv?cv:"NIL", type2Chars(data.type) );
	  if(cv) free(cv);	    
	  if (propertyname) {
	    CMRelease(propertyname);
	  }
	}
    }
    
    if (classname) CMRelease(classname);
}

