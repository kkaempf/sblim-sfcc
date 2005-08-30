/*!
  \file value.c
  \brief Native CMPIValue helper module.

  This module provides means to clone and release CMPIValues.

  (C) Copyright IBM Corp. 2003
 
  THIS FILE IS PROVIDED UNDER THE TERMS OF THE COMMON PUBLIC LICENSE 
  ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE 
  CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 
  You can obtain a current copy of the Common Public License from
  http://oss.software.ibm.com/developerworks/opensource/license-cpl.html

  \author Frank Scheffler
  $Revision: 1.4 $
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmcidt.h"
#include "cmcift.h"
//#include "tool.h"
#include "native.h"
#include "cimXmlParser.h"
#include "utilStringBuffer.h"

 
char *keytype2Chars(CMPIType type);
CMPIType guessType(char *val);
char *value2Chars(CMPIType type, CMPIValue * value);
extern char *pathToChars(CMPIObjectPath * cop, CMPIStatus * rc, char *str,
								 int uri);


void native_release_CMPIValue ( CMPIType type, CMPIValue * val )
{
        if (val) switch ( type ) {

	case CMPI_instance:
		if ( val->inst ) CMRelease ( val->inst );
		break;

	case CMPI_ref:
		if ( val->ref ) CMRelease ( val->ref );
		break;

	case CMPI_args:
		if ( val->args ) CMRelease ( val->args );
		break;

	case CMPI_enumeration:
		if ( val->Enum ) CMRelease ( val->Enum );
		break;

	case CMPI_string:
		if ( val->string ) CMRelease ( val->string );
		break;

	case CMPI_chars:
		if ( val->chars ) free ( val->chars );
		break;

	case CMPI_dateTime:
		if ( val->dateTime ) CMRelease ( val->dateTime );
		break;

	default:
		if ( type & CMPI_ARRAY && val->array) {
			CMRelease ( val->array );
		}
	}
}


CMPIValue native_clone_CMPIValue ( CMPIType type,
				   CMPIValue * val,
				   CMPIStatus * rc )
{
	CMPIValue v;

	if ( type & CMPI_ENC ) {

		switch ( type ) {

		case CMPI_instance:
			v.inst = CMClone ( val->inst, rc );
			break;

		case CMPI_ref:
			v.ref = CMClone ( val->ref, rc );
			break;

		case CMPI_args:
			v.args = CMClone ( val->args, rc );
			break;

		case CMPI_enumeration:
			v.Enum = CMClone ( val->Enum, rc );
			break;

		case CMPI_string:
			v.string = CMClone ( val->string, rc );
			break;

		case CMPI_chars:
			v.chars = strdup ( val->chars );
			CMSetStatus ( rc, CMPI_RC_OK );
			break;

		case CMPI_dateTime:
			v.dateTime = CMClone ( val->dateTime, rc );
			break;
		}

	} else if ( type & CMPI_ARRAY ) {

			v.array = CMClone ( val->array, rc );
	} else {
		
		v = *val;
		CMSetStatus ( rc, CMPI_RC_OK );
	}

	return v;
}

static char *value2CharsUri(CMPIType type, CMPIValue * value, int uri)
{
   char str[256], *p;
   unsigned int size;
   CMPIString *cStr;

   if (type & CMPI_ARRAY) {

   }
   else if (type & CMPI_ENC) {

      switch (type) {
      case CMPI_instance:
         break;

      case CMPI_ref: {
            char r[2048];
            p = pathToChars(value->ref, NULL, r, uri);
            return p;
         }
         break;

      case CMPI_args:
         break;

      case CMPI_filter:
         break;

      case CMPI_string:
      case CMPI_numericString:
      case CMPI_booleanString:
      case CMPI_dateTimeString:
      case CMPI_classNameString:
         size = strlen((char *) value->string->hdl);
         p = malloc(size + 8);
         sprintf(p, "%s", (char *) value->string->hdl);
         return p;

      case CMPI_dateTime:
         cStr=CMGetStringFormat(value->dateTime,NULL);
         return strdup((char *) cStr->hdl);
         CMRelease(cStr);
         break;
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

char *keytype2Chars(CMPIType type)
{
   if (type == CMPI_boolean) return "boolean";
   if (type & (CMPI_INTEGER | CMPI_REAL)) return "numeric";
   return "string";
}

CMPIValue *getKeyValueTypePtr(char *type, char *value, XtokValueReference *ref,
                              CMPIValue * val, CMPIType * typ)
{
   if (type) {
      if (strcasecmp(type, "string") == 0);
      else if (strcasecmp(type, "boolean") == 0) {
         *typ = CMPI_boolean;
         if (strcasecmp(type, "true") == 0)
            val->boolean = 1;
         else
            val->boolean = 0;
         return val;
      }
      else if (strcasecmp(type, "numeric") == 0) {
         if (value[0] == '+' || value[0] == '-') {
            *typ = CMPI_sint64;
            sscanf(value, "%lld", &val->uint64);
         }
         else {
            sscanf(value, "%llu", &val->sint64);
            *typ = CMPI_uint64;
         }
         return val;
      }
      else if (strcasecmp(type, "ref") == 0) {
         CMPIObjectPath *op;
         char *hn="",*ns="",*cn;
         CMPIType type;
         CMPIValue v, *valp;
         int i,m;
         XtokInstanceName *in; 
         
         switch(ref->type) {
         case typeValRef_InstancePath: 
            in=&ref->instancePath.instanceName;
            hn=ref->instancePath.path.host.host;
            ns=ref->instancePath.path.nameSpacePath;
            break;   
         case typeValRef_InstanceName: 
            in=&ref->instanceName;
            break;   
         default:
//            mlogf(M_ERROR,M_SHOW,"%s(%d): unexpected reference type %d %x\n", __FILE__, __LINE__, 
            printf("%s(%d): unexpected reference type %d %x\n", __FILE__, __LINE__, 
               (int) type, (int) type);
            abort();   
         }

         cn=in->className;
         op=newCMPIObjectPath(ns,cn,NULL);
         CMSetHostname(op,hn);

         for (i = 0, m = in->bindings.next; i < m; i++) {
            valp = getKeyValueTypePtr(
               in->bindings.keyBindings[i].type,
               in->bindings.keyBindings[i].value,
               &in->bindings.keyBindings[i].ref,
               &v, &type);
            CMAddKey(op,in->bindings.keyBindings[i].name,valp,type);
         }
         *typ = CMPI_ref;
         val->ref=op;
         return val;
      }
   }
   
   *typ = CMPI_chars;
   return (CMPIValue *) value;
}

CMPIType guessType(char *val)
{
   /* TODO: Currently doesn't guess right for real values (3.175e+00) */
   if (((*val=='-' || *val=='+') && strlen(val)>1) || isdigit(*val)) {
      char *c;
      for (c=val+1; ; c++) {
         if (*c==0) {
            if (!isdigit(*val)) return CMPI_sint64;
            return CMPI_uint64;
         }
         if (!isdigit(*c)) break;
      }
   }
   else if (strcasecmp(val,"true") == 0 || strcasecmp(val,"false") == 0)
     return CMPI_boolean;
   return CMPI_string;
}

CMPIValue str2CMPIValue(CMPIType type, char *val, XtokValueReference *ref)
{
   CMPIValue value,*valp;
 //  char *val=p->value;
   CMPIType t;

   if (type==0) {
      type=guessType(val);
   }
   
   switch (type) {
   case CMPI_char16:
      value.char16 = *val;
      break;
   case CMPI_chars:
      value.char16 = *val;
      break;
   case CMPI_string:
      value.string = native_new_CMPIString(val, NULL);
      break;
   case CMPI_sint64:
      sscanf(val, "%lld", &value.sint64);
      break;
   case CMPI_uint64:
      sscanf(val, "%llu", &value.uint64);
      break;
   case CMPI_sint32:
      sscanf(val, "%ld", &value.sint32);
      break;
   case CMPI_uint32:
      sscanf(val, "%lu", &value.uint32);
      break;
   case CMPI_sint16:
      sscanf(val, "%hd", &value.sint16);
      break;
   case CMPI_uint16:
      sscanf(val, "%hu", &value.uint16);
      break;
   case CMPI_uint8:
      sscanf(val, "%lu", &value.uint32);
      value.uint8 = value.uint32;
      break;
   case CMPI_sint8:
      sscanf(val, "%ld", &value.sint32);
      value.sint8 = value.sint32;
      break;
   case CMPI_boolean:
      value.boolean = strcasecmp(val, "false");
      break;
   case CMPI_real32:
      sscanf(val, "%f", &value.real32);
      break;
   case CMPI_real64:
      sscanf(val, "%lf", &value.real64);
      break;
   case CMPI_dateTime:
      value.dateTime = native_new_CMPIDateTime_fromChars(val, NULL);
      break;
   case CMPI_ref:
      valp=getKeyValueTypePtr("ref", NULL, ref, &value, &t);
      break;
  default:
 //     mlogf(M_ERROR,M_SHOW,"%s(%d): invalid value %d-%s\n", __FILE__, __LINE__, (int) type, val);
      printf("%s(%d): invalid value %d-%s\n", __FILE__, __LINE__, (int) type, val);
      abort();
   }
   return value;
}
/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
