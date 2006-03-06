
/*
 * cimcft2.h
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
 * CMCI function tables.
 *
*/

#ifndef _CIMCFT2_H_
#define _CIMCFT2_H_

#include "cimcdt.h"


struct _cimcProperty;
typedef struct _cimcProperty cimcProperty;

typedef struct _cimcPropertyFT {
   int ftVersion;
   cimcStatus    (*release)          (cimcProperty* prop);
   cimcProperty* (*clone)            (cimcProperty* prop, cimcStatus* rc);
   cimcData      (*getValue)         (cimcProperty* prop, cimcStatus* rc);
   cimcStatus    (*setValue)         (cimcProperty* prop, cimcValue *value, cimcType); 
   void          (*clearValue)       (cimcProperty* prop); 
   cimcData      (*getQualifier)     (cimcProperty* prop, const char* name, cimcStatus* rc);
   cimcData      (*getQualifierAt)   (cimcProperty* prop, cimcCount at, cimcString **name, cimcStatus* rc);
   cimcCount     (*getQualifierCount)(cimcProperty* prop);
   cimcStatus    (*addQualifier)     (cimcProperty* prop, const char* name, cimcValue *value, cimcType);
   cimcStatus    (*removeQualifier)  (cimcProperty* prop, const char* name);
} cimcPropertyFT;

struct _cimcProperty {
   void *hdl;
   cimcPropertyFT *ft;
};

struct _cimcPropertyList;
typedef struct _cimcPropertyList cimcPropertyList;

typedef struct _cimcPropertyListFT {
   int ftVersion;
   cimcStatus        (*release)         (cimcPropertyList* plst);
   cimcPropertyList* (*clone)           (cimcPropertyList* plst, cimcStatus* rc);
   cimcProperty*     (*getProperty)     (cimcPropertyList* plst, const char* name, cimcStatus* rc);
   cimcProperty*     (*getPropertyAt)   (cimcProperty* plst, cimcCount at, cimcString **name, cimcStatus* rc); 
   cimcCount         (*getPropertyCount)(cimcProperty* plst);
   cimcStatus        (*addProperty)     (cimcProperty* plst, const char* name, cimcProperty *prop);
   cimcStatus        (*removeProperty)  (cimcProperty* plst, const char* name);
} cimcPropertyListFT;

struct _cimcPropertyList {
   void *hdl;
   cimcPropertyListFT *ft;
};


struct _cimcParameter;
typedef struct _cimcParameter cimcParameter;

typedef struct _cimcParameterFT {
   int ftVersion;
   cimcStatus    (*release)          (cimcParameter* parm);
   cimcParameter* (*clone)           (cimcParameter* parm, cimcStatus* rc);
   cimcType      (*getType)          (cimcParameter* parm, cimcStatus* rc);
   cimcStatus    (*setType)          (cimcParameter* parm, cimcType); 
   cimcData      (*getQualifier)     (cimcParameter* parm, const char* name, cimcStatus* rc);
   cimcData      (*getQualifierAt)   (cimcParameter* parm, cimcCount at, cimcString **name, cimcStatus* rc);
   cimcCount     (*getQualifierCount)(cimcParameter* parm);
   cimcStatus    (*addQualifier)     (cimcParameter* parm, const char* name, cimcValue *value, cimcType);
   cimcStatus    (*removeQualifier)  (cimcParameter* parm, const char* name);
} cimcParameterFT;

struct _cimcParameter {
   void *hdl;
   cimcParameterFT *ft;
};

struct _cimcParameterList;
typedef struct _cimcParameterList cimcParameterList;

typedef struct _cimcParameterListFT {
   int ftVersion;
   cimcStatus         (*release)          (cimcParameterList* plst);
   cimcParameterList* (*clone)            (cimcParameterList* plst, cimcStatus* rc);
   cimcParameter*     (*getParameter)     (cimcParameterList* plst, const char* name, cimcStatus* rc);
   cimcParameter*     (*getParameterAt)   (cimcParameter* plst, cimcCount at, cimcString **name, cimcStatus* rc); 
   cimcCount          (*getParameterCount)(cimcParameter* plst);
   cimcStatus         (*addParameter)     (cimcParameter* plst, const char* name, cimcParameter *parm);
   cimcStatus         (*removeParameter)  (cimcParameter* plst, const char* name);
} cimcParameterListFT;

struct _cimcParameterList {
   void *hdl;
   cimcParameterListFT *ft;
};


struct _cimcMethod;
typedef struct _cimcMethod cimcMethod;

typedef struct _cimcMethodFT {
   int ftVersion;
   cimcStatus    (*release)          (cimcMethod* meth);
   cimcMethod*   (*clone)            (cimcMethod* meth, cimcStatus* rc);
   cimcType      (*getRetType)       (cimcMethod* meth, cimcStatus* rc);
   cimcStatus    (*setRetType)       (cimcMethod* meth, cimcType); 
   cimcData      (*getQualifier)     (cimcMethod* meth, const char* name, cimcStatus* rc);
   cimcData      (*getQualifierAt)   (cimcMethod* meth, cimcCount at, cimcString **name, cimcStatus* rc);
   cimcCount     (*getQualifierCount)(cimcMethod* meth);
   cimcStatus    (*addQualifier)     (cimcMethod* meth, const char* name, cimcValue *value, cimcType);
   cimcStatus    (*removeQualifier)  (cimcMethod* meth, const char* name);
   cimcParameterList* (*getParamList)(cimcMethod* meth, cimcStatus *rc);
   cimcStatus    (*addParameterList) (cimcMethod* meth, cimcParameterList* list);
} cimcMethodFT;

struct _cimcMethod {
   void *hdl;
   cimcMethodFT *ft;
};

struct _cimcMethodList;
typedef struct _cimcMethodList cimcMethodList;

typedef struct _cimcMethodListFT {
   int ftVersion;
   cimcStatus        (*release)       (cimcMethodList* mlst);
   cimcMethodList*   (*clone)         (cimcMethodList* mlst, cimcStatus* rc);
   cimcMethod*       (*getMethod)     (cimcMethodList* mlst, const char* name, cimcStatus* rc);
   cimcMethod*       (*getMethodAt)   (cimcMethod* mlst, cimcCount at, cimcString **name, cimcStatus* rc); 
   cimcCount         (*getMethodCount)(cimcMethod* mlst);
   cimcStatus        (*addMethod)     (cimcMethod* mlst, const char* name, cimcMethod *meth);
   cimcStatus        (*removeMethod)  (cimcMethod* mlst, const char* name);
} cimcMethodListFT;

struct _cimcMethodList {
   void *hdl;
   cimcMethodListFT *ft;
};


#endif
