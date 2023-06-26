/*
 * $id$
 *
 *  © Copyright IBM Corp. 2013
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:       Sven Schuetz <sven@de.ibm.com> (created file)
 *               all content by Adrian Schuur
 *
 * Description:
 *
 * CIM XML parser helper functions
 *
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "parserUtil.h"
#include "cimXmlParser.h"
#include "sfcUtil/utilft.h"

extern CMPIConstClass * native_new_CMPIConstClass ( char  *cn, CMPIStatus * rc );
extern int addClassProperty( CMPIConstClass * ccls, char * name,
                 CMPIValue * value, CMPIType type,
                 CMPIValueState state);
extern int addInstPropertyQualifier( CMPIInstance* ci, char * pname,
                     char *qname, CMPIValue * value,
                     CMPIType type);
extern int addClassPropertyQualifier( CMPIConstClass* cc, char * pname,
                      char *qname, CMPIValue * value,
                      CMPIType type);
extern int addClassQualifier( CMPIConstClass* cc, char * name,
                      CMPIValue * value,
                      CMPIType type);
extern int addClassMethod( CMPIConstClass* cc, char * mname,
                      CMPIValue * value, CMPIType type,
                      CMPIValueState state);
extern int addClassMethodQualifier( CMPIConstClass* cc, char * mname,
                      char *qname, CMPIValue * value,
                      CMPIType type);
extern int addClassMethodParameter( CMPIConstClass* cc, char * mname,
                      char *pname,
                      CMPIType type);
extern char *XmlToAsciiStr(char *XmlStr);

#if DEBUG
extern int do_debug;
#endif

int isBoolean(CMPIData data)
{
   if (data.type == CMPI_chars) {
      if (strcasecmp(data.value.chars,"true") == 0) return 0xffff;
      if (strcasecmp(data.value.chars,"false") == 0) return 0;
   }
   return -1;
}

void createPath(CMPIObjectPath **op, XtokInstanceName *p)
{
   CMPIValue val,*valp;
   CMPIType type;
   XtokKeyBinding *b;

   *op = newCMPIObjectPath(NULL, p->className, NULL);
   for (b = p->bindings.first; b; b = b->next) {
      valp = getKeyValueTypePtr(b->type,
                              b->val.keyValue.value,
                              &b->val.ref,
                              &val, &type);
      CMAddKey(*op, b->name, valp, type);
      if (type == CMPI_ref) {
    CMRelease(valp->ref);
      }
   }
}

void setInstProperties(CMPIInstance *ci, XtokProperties *ps)
{
   XtokProperty *np = NULL,*p = ps ? ps->first : NULL;
   CMPIValue val;
   CMPIStatus status;
   CMPIType   type = CMPI_null;
   XtokQualifier *nq = NULL,*q;
   XtokQualifiers *qs;
   int setq;

   while (p) {
      setq=1;
      switch (p->propType) {
      case typeProperty_Value:
         type = p->valueType;
         if (p->val.value.data.value != NULL && p->val.null==0) {
            if (type == CMPI_string || type == CMPI_chars) {
                char *charsStr = XmlToAsciiStr(p->val.value.data.value);
                val = str2CMPIValue(type, charsStr, NULL);
                free (charsStr);
            }
            else
                val = str2CMPIValue(type, p->val.value.data.value, NULL);
            CMSetProperty(ci, p->name, &val, type);
        native_release_CMPIValue(type, &val);
         }
         else {
            CMSetProperty(ci, p->name, NULL, type);
            setq = 0;
         }
         break;
      case typeProperty_Reference: 
         val=str2CMPIValue(CMPI_ref, NULL, &p->val.ref);
         CMSetProperty(ci, p->name, &val.ref, CMPI_ref);
     CMRelease(val.ref);
         break;
      case typeProperty_Array:
         type = p->valueType;
         if (p->val.array.next > 0) {
            CMPIArray *arr = newCMPIArray(0, type, &status);
            if (p->val.array.max > 0) {
            int i;
               for (i = 0; i < p->val.array.next; ++i)
               {
                   char *valStr = p->val.array.values[i];
                   if (type == CMPI_string || type == CMPI_chars) {
                       char *charsStr = XmlToAsciiStr(valStr);
                       val = str2CMPIValue(type, charsStr, NULL);
                       free (charsStr);
                   }
                   else
                       val = str2CMPIValue(type, valStr, NULL);
                   CMSetArrayElementAt(arr, i, &val, type);
               native_release_CMPIValue(type, &val);
               }
            }
            val.array = arr;
            CMSetProperty(ci, p->name, &val, type | CMPI_ARRAY);
            CMRelease(arr);         /* cloned in property */
         }
         else {
            CMSetProperty(ci, p->name, NULL, p->valueType | CMPI_ARRAY);
            setq = 0;
         }
         break;
      case typeProperty_Unknown:
         break;
      }

      if (setq) {
         qs=&p->val.qualifiers;
         q=qs ? qs->first : NULL;  
         while (q) {
            if (q->type & CMPI_ARRAY) {
               CMPIArray *arr = NULL;
               arr = newCMPIArray(0, type, NULL);
               type  = q->type & ~CMPI_ARRAY;
               int i;
               if (q->data.array.max) {
                   for (i = 0; i < q->data.array.next; ++i) {
                  val = str2CMPIValue(type, q->data.array.values[i], NULL);
                  CMSetArrayElementAt(arr, i, &val, type);
                  native_release_CMPIValue(type,&val);
               }
               }
               (void)addInstPropertyQualifier(ci, p->name, q->name,
                         (CMPIValue *)&arr, q->type); 
               native_release_CMPIValue(q->type,(CMPIValue*)&arr);
            }
            else {
               val = str2CMPIValue(q->type, q->data.value.data.value, NULL);
               (void)addInstPropertyQualifier(ci, p->name, q->name, &val, q->type);
               native_release_CMPIValue(q->type,&val);
            }   
            nq = q->next; 
            q = nq;
         }
      }

      np = p->next;
      p = np;

   }

   if (ps)
      ps->first = ps->last =  NULL;
}

void setInstQualifiers(CMPIInstance *ci, XtokQualifiers *qs)
{
   XtokQualifier *nq = NULL,*q = qs ? qs->first : NULL;
   CMPIValue val;
   
   while (q) {
      if (q->type & CMPI_ARRAY) {
               CMPIType type=q->type&~CMPI_ARRAY;
               CMPIArray *arr = newCMPIArray(0, type, NULL);
               int i;
          if (q->data.array.max) {
              for (i = 0; i < q->data.array.next; ++i) {
                  val = str2CMPIValue(type, q->data.array.values[i], NULL);
                  CMSetArrayElementAt(arr, i, &val, type);
                  native_release_CMPIValue(type,&val);
               }
               (void)addInstQualifier(ci, q->name, (CMPIValue*)&arr, q->type);
               native_release_CMPIValue(q->type,(CMPIValue*)&arr);
      }
      }
      else {
         val = str2CMPIValue(q->type, q->data.value.data.value, NULL);
         (void)addInstQualifier(ci, q->name, &val, q->type);
         native_release_CMPIValue( q->type,&val);
      }
      nq = q->next;
      q = nq;
   }
   if (qs) {
      qs->first = qs->last = NULL;
   }
}

void setClassMethods(CMPIConstClass *cls, XtokMethods *ms)
{
   XtokMethod *nm = NULL,*m = ms ? ms->first : NULL;
   CMPIValue      val;
   CMPIArray      *arr = NULL;
   XtokQualifier  *nq,*q;
   XtokQualifiers *qs;
   XtokParam      *np,*p;
   XtokParams     *ps;

   val.uint64=0l;
   while (m) {
      addClassMethod(cls, m->name, &val, m->type, CMPI_nullValue);

      qs=&m->qualifiers;
      q=qs ? qs->first : NULL;
      while (q) {
         if (q->type & CMPI_ARRAY) {
            CMPIType type=q->type&~CMPI_ARRAY;
            arr = newCMPIArray(0, type, NULL);
            int i;
            if (q->data.array.max) {
               for (i = 0; i < q->data.array.next; ++i) {
                  val = str2CMPIValue(type, q->data.array.values[i], NULL);
                  CMSetArrayElementAt(arr, i, &val, type);
                  native_release_CMPIValue(type,&val);
               }
            }
            val.array = arr;
            (void)addClassMethodQualifier(cls, m->name, q->name, &val, q->type);
            native_release_CMPIValue(q->type,(CMPIValue*)&arr);
         }
         else {
            val = str2CMPIValue(q->type, q->data.value.data.value, NULL);
            (void)addClassMethodQualifier(cls, m->name, q->name, &val, q->type);
            native_release_CMPIValue(q->type,&val);
         }
         nq = q->next; 
         q = nq;
      }

      ps=&m->params;
      p=ps ? ps->first : NULL;
      while (p) {
         (void)addClassMethodParameter(cls, m->name, p->name, p->type);
         np = p->next; 
         p = np;
      }

      nm = m->next;
      m = nm;
   }
   if (ms) ms->first = ms->last = NULL;
}

void setClassProperties(CMPIConstClass *cls, XtokProperties *ps)
{
   XtokProperty *np = NULL,*p = ps ? ps->first : NULL;
   CMPIValue val;
   CMPIArray       *arr = NULL;
   XtokQualifier   *nq,*q;
   XtokQualifiers *qs;

      val.uint64=0l;
   while (p) {
      switch (p->propType) {
      case typeProperty_Value:
         addClassProperty(cls, p->name, &val, p->valueType, CMPI_nullValue);
         break;
      case typeProperty_Reference:
         addClassProperty(cls, p->name, &val, CMPI_ref, CMPI_nullValue);
     break;
      case typeProperty_Array:
         val.array = arr;
     addClassProperty(cls, p->name, &val,
                               p->valueType | CMPI_ARRAY, CMPI_nullValue);
         break;
      case typeProperty_Unknown:
         break;
      }

      qs=&p->val.qualifiers;
      q=qs ? qs->first : NULL;  
      while (q) {
         if (q->type & CMPI_ARRAY) {
            CMPIType type=q->type&~CMPI_ARRAY;
            arr = newCMPIArray(0, type, NULL);
            int i;
            if (q->data.array.max) {
                for (i = 0; i < q->data.array.next; ++i) {
               val = str2CMPIValue(type, q->data.array.values[i], NULL);
               CMSetArrayElementAt(arr, i, &val, type);
               native_release_CMPIValue(type,&val);
            }
            }
            val.array = arr;
            (void)addClassPropertyQualifier(cls, p->name, q->name, &val, q->type); 
            native_release_CMPIValue(q->type,(CMPIValue*)&arr);
         }
         else {
            val = str2CMPIValue(q->type, q->data.value.data.value, NULL);
            (void)addClassPropertyQualifier(cls, p->name, q->name, &val, q->type);
            native_release_CMPIValue(q->type,&val);
         }   
         nq = q->next; 
         q = nq;
      }

      np = p->next;
      p = np;
   }
   if (ps) ps->first = ps->last = NULL;
}

void setClassQualifiers(CMPIConstClass *cls, XtokQualifiers *qs)
{
   XtokQualifier *nq = NULL,*q = qs ? qs->first : NULL;
   CMPIValue val;
   
   while (q) {
      if (q->type & CMPI_ARRAY) {
               CMPIType type=q->type&~CMPI_ARRAY;
               CMPIArray *arr = newCMPIArray(0, type, NULL);
               int i;
          if (q->data.array.max > 0) {
              for (i = 0; i < q->data.array.next; ++i) {
                   char *valStr = q->data.array.values[i];
                   if (type == CMPI_string || type == CMPI_chars)
                   {
                       char *charsStr = XmlToAsciiStr(valStr);
                       val = str2CMPIValue(type, charsStr, NULL);
                       free (charsStr);
                   }
                   else
                       val = str2CMPIValue(type, valStr, NULL);
                  CMSetArrayElementAt(arr, i, &val, type);
                  native_release_CMPIValue(type,&val);
               }
               (void)addClassQualifier(cls, q->name, (CMPIValue*)&arr, q->type);
               native_release_CMPIValue(q->type,(CMPIValue*)&arr);
      }
      }
      else {
          char *valStr = q->data.value.data.value;
          if (q->type == CMPI_string || q->type == CMPI_chars)
          {
              char *charsStr = XmlToAsciiStr(valStr);
              val = str2CMPIValue(q->type, charsStr, NULL);
              free (charsStr);
          }
          else
              val = str2CMPIValue(q->type, valStr, NULL);
         (void)addClassQualifier(cls, q->name, &val, q->type);
         native_release_CMPIValue( q->type,&val);
      }
      nq = q->next;
      q = nq;
   }
   if (qs) {
      qs->first = qs->last = NULL;
   }
}


void addProperty(ParserControl *parm, XtokProperties *ps, XtokProperty *p)
{
   XtokProperty *np;
   np = (XtokProperty*)PARSER_MALLOC(sizeof(XtokProperty));
   memcpy(np, p, sizeof(XtokProperty));
   np->next = NULL;
   if (ps->last) ps->last->next = np;
   else ps->first = np;
   ps->last = np;
}

void addParamValue(ParserControl *parm, XtokParamValues *vs, XtokParamValue *v)
{
   XtokParamValue *nv;
   nv = (XtokParamValue*)PARSER_MALLOC(sizeof(XtokParamValue));
   memcpy(nv, v, sizeof(XtokParamValue));
   nv->next = NULL;
   if (vs->last)
      vs->last->next = nv;
   else
      vs->first = nv;
   vs->last = nv;
}

void addKeyBinding(ParserControl *parm, XtokKeyBindings *ks, XtokKeyBinding *k)
{
   XtokKeyBinding *nk;
   nk = (XtokKeyBinding*)PARSER_MALLOC(sizeof(XtokKeyBinding));
   memcpy(nk, k, sizeof(XtokKeyBinding));
   nk->next = NULL;
   if (ks->last) ks->last->next = nk;
   else ks->first = nk;
   ks->last = nk;
}

void addQualifier(ParserControl *parm, XtokQualifiers *qs, XtokQualifier *q)
{
   XtokQualifier *nq;
   nq = (XtokQualifier*)PARSER_MALLOC(sizeof(XtokQualifier));
   memcpy(nq, q, sizeof(XtokQualifier));
   nq->next = NULL;
   if (qs->last) qs->last->next = nq;
   else qs->first = nq;
   qs->last = nq;
}

void addMethod(ParserControl *parm, XtokMethods *ms, XtokMethod *m)
{
   XtokMethod *nm;
   nm = (XtokMethod*)PARSER_MALLOC(sizeof(XtokMethod));
   memcpy(nm, m, sizeof(XtokMethod));
   nm->next = NULL;
   if (ms->last)
      ms->last->next = nm;
   else
      ms->first = nm;
   ms->last = nm;
}

void addParam(ParserControl *parm, XtokParams *ps, XtokParam *p)
{
   XtokParam *np;
   np = (XtokParam*)PARSER_MALLOC(sizeof(XtokParam));
   memcpy(np, p, sizeof(XtokParam));
   np->next = NULL;
   if (ps->last)
      ps->last->next = np;
   else
      ps->first = np;
   ps->last = np;
}

void setError(ParserControl *parm, XtokErrorResp *e)
{
#if DEBUG
   if (do_debug)
       fprintf(stderr, "error:: code:%s description:%s\n", 
                                 e->code, e->description);
#endif
   parm->respHdr.errCode = atoi(e->code);
   parm->respHdr.description = XmlToAsciiStr(e->description);
}

void setReturnArgs(ParserControl *parm, XtokParamValues *ps)
{
    CMPIValue value;
    XtokParamValue *outParam=NULL;
    CMPIArgs *args = NULL;
   
    /* Process OUT parameters */
    outParam = ps->first;

    if (outParam) { 
        args = newCMPIArgs(NULL);

        while (outParam) {
            value = str2CMPIValue(outParam->type, outParam->data.value.data.value, &outParam->data.valueRef);

            /* Add it to the args list */
            args->ft->addArg ( args, outParam->name, &value, outParam->type);
            native_release_CMPIValue(outParam->type,&value);
            outParam = outParam->next;
        }
        parm->respHdr.outArgs = args;
        /* Note : Freeing of list will be done by
        * parser_heap_term() routine.
        */
    }
}
