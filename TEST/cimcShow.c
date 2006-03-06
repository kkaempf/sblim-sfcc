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

#include "cimcShow.h"
#include "cimcmacs.h"
#include <stdio.h>
#include <string.h>

static char *value2Chars(cimcType type, cimcValue * value);

static char *pathToChars(cimcObjectPath * cop, cimcStatus * rc, char *str, int uri)
{
//            "//atp:9999/root/cimv25:TennisPlayer.first="Patrick",last="Rafter";

   cimcString *ns;
   cimcString *cn;
   cimcString *name;
   cimcData data;
   unsigned int i, m, s;
   char *v;
   char *colon = (uri) ? "%3A" : ":";
   
   *str = 0;

   ns = cop->ft->getNameSpace(cop, rc);
   cn = cop->ft->getClassName(cop, rc);
   
   if (ns && ns->hdl && *(char*)ns->hdl) {
      if (!uri)
	  strcpy(str,(char*)ns->hdl);
      else {
         char *cns=(char*)ns->hdl;
         for (s=i=0, m=strlen(cns); i<m; i++,s++) {
            if (cns[i]=='/') {
               str[s++]='%';
               str[s++]='2';
               str[s]='F';
            }
            else
	       str[s]=cns[i];    
         } 
         str[s]=0;   
      }
      strcat(str,colon);
   }   
   
   if (ns) CMRelease(ns);
   strcat(str, (char *) cn->hdl);
   CMRelease(cn);
   
   for (i = 0, m = cop->ft->getKeyCount(cop, rc); i < m; i++) {
      data = cop->ft->getKeyAt(cop, i, &name, rc);
      strcat(str, i ? "," : ".");
      strcat(str, (char *) name->hdl);
      strcat(str, (uri) ? "%3D" : "=");
      v = value2Chars(data.type, &data.value);
      if (data.type & (CIMC_INTEGER | CIMC_REAL))
         strcat(str, v);
      else {
	 strcat(str, "\"");
         strcat(str, v);
	 strcat(str, "\"");
      }
      free(v);
      CMRelease(name);
   };
 
   return str;
}

static char *value2CharsUri(cimcType type, cimcValue * value, int uri)
{
   char str[2048], *p;
   unsigned int size;
   cimcString *cStr;

   str[0]=0;
   if (type & CIMC_ARRAY) {
   }
   else if (type & CIMC_ENC) {

      switch (type) {
      case CIMC_instance:
         break;

      case CIMC_ref:
         return strdup(pathToChars(value->ref, NULL, str, uri));

      case CIMC_args:
         break;

      case CIMC_filter:
         break;

      case CIMC_string:
      case CIMC_numericString:
      case CIMC_booleanString:
      case CIMC_dateTimeString:
      case CIMC_classNameString:
         if (value->string->hdl) return strdup((char*)value->string->hdl);
         return strdup("NULL");

      case CIMC_dateTime:
         cStr=CMGetStringFormat(value->dateTime,NULL);
         p = strdup((char *) cStr->hdl);
         CMRelease(cStr);
	 return p;
      }

   }
   else if (type & CIMC_SIMPLE) {

      switch (type) {
      case CIMC_boolean:
         return strdup(value->boolean ? "true" : "false");

      case CIMC_char16:
         break;
      }

   }
   else if (type & CIMC_INTEGER) {

      switch (type) {
      case CIMC_uint8:
         sprintf(str, "%u", value->uint8);
         return strdup(str);
      case CIMC_sint8:
         sprintf(str, "%d", value->sint8);
         return strdup(str);
      case CIMC_uint16:
         sprintf(str, "%u", value->uint16);
         return strdup(str);
      case CIMC_sint16:
         sprintf(str, "%d", value->sint16);
         return strdup(str);
      case CIMC_uint32:
         sprintf(str, "%lu", value->uint32);
         return strdup(str);
      case CIMC_sint32:
         sprintf(str, "%ld", value->sint32);
         return strdup(str);
      case CIMC_uint64:
         sprintf(str, "%llu", value->uint64);
         return strdup(str);
      case CIMC_sint64:
         sprintf(str, "%lld", value->sint64);
         return strdup(str);
      }

   }
   else if (type & CIMC_REAL) {

      switch (type) {
      case CIMC_real32:
         sprintf(str, "%g", value->real32);
         return strdup(str);
      case CIMC_real64:
         sprintf(str, "%g", value->real64);
         return strdup(str);
      }

   }
   return strdup(str);
}

static char *value2Chars(cimcType type, cimcValue * value)
{
   return value2CharsUri(type,value,0);
}

void showObjectPath( cimcObjectPath * objectpath )
{
   cimcString * namespace = objectpath->ft->getNameSpace(objectpath, NULL);
   cimcString * classname = objectpath->ft->getClassName(objectpath, NULL);
   int numkeys = objectpath->ft->getKeyCount(objectpath, NULL);
   int i;
   char *cv;

   if (namespace && namespace->hdl) printf("namespace=%s\n", (char *)namespace->hdl);
   if (classname && classname->hdl) printf("classname=%s\n", (char *)classname->hdl);
   if (numkeys) {
      printf("keys:");
      for (i=0; i<numkeys; i++) {
         cimcString * keyname;
         cimcData data = objectpath->ft->getKeyAt(objectpath, i, &keyname, NULL);
         printf("\t%s=%s\n", (char *)keyname->hdl, cv=value2Chars(data.type, &data.value)); 
	 if(cv) free(cv);
	 if(keyname) CMRelease(keyname);
      }
   }

   if (classname) CMRelease(classname); 
   if (namespace) CMRelease(namespace);
}

void showProperty( cimcData data, char *name )
{
	 char *valuestr;
	 if (CMIsArray(data)) {
	     cimcArray *arr   = data.value.array;
	     cimcType  eletyp = data.type & ~CIMC_ARRAY;
	     int j, n;
	     n = CMGetArrayCount(arr, NULL);
	     for (j = 0; j < n; ++j) { 
		cimcData ele = CMGetArrayElementAt(arr, j, NULL);
		valuestr = value2Chars(eletyp, &ele.value);
	        printf("\t%s[%d]=%s\n", name, j, valuestr);
		free (valuestr);
	     }
	 } else {
	     valuestr = value2Chars(data.type, &data.value);
	     printf("\t%s=%s\n", name, valuestr);
	     free (valuestr);
	 }
}         

void showInstance( cimcInstance *instance )
{
   cimcObjectPath * objectpath = instance->ft->getObjectPath(instance, NULL);
   cimcString * objectpathname = objectpath->ft->toString(objectpath, NULL);
   cimcString * namespace = objectpath->ft->getNameSpace(objectpath, NULL);
   cimcString * classname = objectpath->ft->getClassName(objectpath, NULL);
   int numkeys = objectpath->ft->getKeyCount(objectpath, NULL);
   int numproperties = instance->ft->getPropertyCount(instance, NULL);
   int i;

   if (objectpathname && objectpathname->hdl) printf("objectpath=%s\n", (char *)objectpathname->hdl);
   if (namespace && namespace->hdl) printf("namespace=%s\n", (char *)namespace->hdl);
   if (classname && classname->hdl) printf("classname=%s\n", (char *)classname->hdl);
   if (numkeys) {
      printf("keys:\n");
      for (i=0; i<numkeys; i++) {
         cimcString * keyname;
         cimcData data = objectpath->ft->getKeyAt(objectpath, i, &keyname, NULL);
         char *ptr=NULL;
         printf("\t%s=%s\n", (char *)keyname->hdl, (ptr=value2Chars(data.type, &data.value))); 
         free(ptr);
	 if (keyname) CMRelease(keyname);
      }
   }
   else
      printf("No keys!\n");
   if (numproperties) {
      printf("properties:\n");
      for (i=0; i<numproperties; i++) {
         cimcString * propertyname;
         cimcData data = instance->ft->getPropertyAt(instance, i, &propertyname, NULL);
         showProperty( data, (char *)propertyname->hdl );
	 CMRelease(propertyname);
      }
   }
   else
      printf("No properties!\n");

   if (classname) CMRelease(classname);
   if (namespace) CMRelease(namespace);
   if (objectpathname) CMRelease(objectpathname);
   if (objectpath) CMRelease(objectpath);
}


void showClass( cimcConstClass * class )
{
   cimcString * classname = class->ft->getClassName(class, NULL);
   int numproperties = class->ft->getPropertyCount(class, NULL);
   int i;
   char *cv;

   if (classname && classname->hdl) printf("classname=%s\n", (char *)classname->hdl);
   if (numproperties) {
      printf("properties:\n");
      for (i=0; i<numproperties; i++) {
         cimcString * propertyname;
         cimcData data = class->ft->getPropertyAt(class, i, &propertyname, NULL);
         if (data.state==0) {
            printf("\t%s=%s\n", (char *)propertyname->hdl, cv=value2Chars(data.type, &data.value));
	    if(cv) free(cv);	    
	 }
         else printf("\t%s=NIL\n", (char *)propertyname->hdl);
	 if (propertyname) CMRelease(propertyname);
      }
   }

   if (classname) CMRelease(classname);
}

