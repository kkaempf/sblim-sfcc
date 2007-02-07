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

static char *value2CharsUri(CMPIType type, CMPIValue * value, int uri)
{
   char str[2048], *p;
   unsigned int size;
   CMPIString *cStr;

   str[0]=0;
   if (type & CMPI_ARRAY) {
   }
   else if (type & CMPI_ENC) {

      switch (type) {
      case CMPI_instance:
         break;

      case CMPI_ref:
	if (value->ref) {
	  cStr=value->ref->ft->toString(value->ref,NULL);
	  p = strdup((char *) cStr->hdl);
	  CMRelease(cStr);
	} else {
	  p = strdup("NULL");
	}
 
	return p;

      case CMPI_args:
         break;

      case CMPI_filter:
         break;

      case CMPI_string:
      case CMPI_numericString:
      case CMPI_booleanString:
      case CMPI_dateTimeString:
      case CMPI_classNameString:
         return strdup(value->string ? (char*)value->string->hdl : "NULL");

      case CMPI_dateTime:
         if (value->dateTime) {
         cStr=CMGetStringFormat(value->dateTime,NULL);
         p = strdup((char *) cStr->hdl);
         CMRelease(cStr);
         } else
             p = strdup("NULL");
	 return p;
      }

   }
   else if (type & CMPI_SIMPLE) {

      switch (type) {
      case CMPI_boolean:
         return strdup(value->boolean ? "true" : "false");

      case CMPI_char16:
         break;
      }

   }
   else if (type & CMPI_INTEGER) {

      switch (type) {
      case CMPI_uint8:
         sprintf(str, "%u", value->uint8);
         return strdup(str);
      case CMPI_sint8:
         sprintf(str, "%d", value->sint8);
         return strdup(str);
      case CMPI_uint16:
         sprintf(str, "%u", value->uint16);
         return strdup(str);
      case CMPI_sint16:
         sprintf(str, "%d", value->sint16);
         return strdup(str);
      case CMPI_uint32:
         sprintf(str, "%lu", value->uint32);
         return strdup(str);
      case CMPI_sint32:
         sprintf(str, "%ld", value->sint32);
         return strdup(str);
      case CMPI_uint64:
         sprintf(str, "%llu", value->uint64);
         return strdup(str);
      case CMPI_sint64:
         sprintf(str, "%lld", value->sint64);
         return strdup(str);
      }

   }
   else if (type & CMPI_REAL) {

      switch (type) {
      case CMPI_real32:
         sprintf(str, "%g", value->real32);
         return strdup(str);
      case CMPI_real64:
         sprintf(str, "%g", value->real64);
         return strdup(str);
      }

   }
   return strdup(str);
}

char *value2Chars(CMPIType type, CMPIValue * value)
{
   return value2CharsUri(type,value,0);
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
    if (state == CMPI_goodValue)
    {
        if (CMIsArray(data))
        {
	     CMPIArray *arr   = data.value.array;
	     CMPIType  eletyp = data.type & ~CMPI_ARRAY;
	     int j, n;
	     n = CMGetArrayCount(arr, NULL);
            for (j = 0; j < n; ++j)
            {
		CMPIData ele = CMGetArrayElementAt(arr, j, NULL);
		valuestr = value2Chars(eletyp, &ele.value);
	        printf("\t%s[%d]=%s\n", name, j, valuestr);
		free (valuestr);
	     }
        }
        else
        {
            if (state == CMPI_goodValue)
            {
	     valuestr = value2Chars(data.type, &data.value);
	     printf("\t%s=%s\n", name, valuestr);
	     free (valuestr);
	 }
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
   int numproperties = class->ft->getPropertyCount(class, NULL);
   int i;
   char *cv;

    if (classname && classname->hdl)
    {
        printf("classname=%s\n", (char *)classname->hdl);
    }

    if (numproperties)
    {
      printf("properties:\n");
        for (i=0; i<numproperties; i++)
        {
         CMPIString * propertyname;
            CMPIData data = class->ft->getPropertyAt(class, i,
                                                     &propertyname, NULL);
            if (data.state==0)
            {
                printf("\t%s=%s\n", (char *)propertyname->hdl,
                       cv=value2Chars(data.type, &data.value));
	    if(cv) free(cv);	    
	 }
         else printf("\t%s=NIL\n", (char *)propertyname->hdl);
	 if (propertyname) CMRelease(propertyname);
      }
   }

   if (classname) CMRelease(classname);
}

