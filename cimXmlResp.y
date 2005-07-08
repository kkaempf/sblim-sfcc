%{

/*
 * cimXmlResp.y
 *
 * (C) Copyright IBM Corp. 2005
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE COMMON PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Common Public License from
 * http://oss.software.ibm.com/developerworks/opensource/license-cpl.html
 *
 * Author:       Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * CIM XML grammar for sfcc.
 *
*/


/*
**==============================================================================
**
** Includes
**
**==============================================================================
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "cimXmlParser.h"


//
// Define the global parser state object:
//

#define YYPARSE_PARAM parm
#define YYLEX_PARAM parm
#define YYERROR_VERBOSE 1

#define PARM ((ParserControl*)parm)

extern int yyerror(char*);
extern int yylex (void *lvalp, ParserControl *parm);

extern CMPIConstClass * native_new_CMPIConstClass ( char  *cn, CMPIStatus * rc );
extern int addClassProperty( CMPIConstClass * ccls, char * name, CMPIValue * value, CMPIType type, 
				      CMPIValueState state);


int isBoolean(CMPIData data)
{
   if (data.type==CMPI_chars) {
      if (strcasecmp(data.value.chars,"true")==0) return 0xffff;
      if (strcasecmp(data.value.chars,"false")==0) return 0;
   }
   return -1;
}

static void createPath(CMPIObjectPath **op, XtokInstanceName *p)
{
   int i,m;
   CMPIValue val,*valp;
   CMPIType type;
   
   *op = newCMPIObjectPath(NULL, p->className, NULL);
   for (i = 0, m = p->bindings.next; i < m; i++) {
      valp = getKeyValueTypePtr(p->bindings.keyBindings[i].type,
                                p->bindings.keyBindings[i].value,
                                &p->bindings.keyBindings[i].ref,
                                &val, &type);
      CMAddKey(*op, p->bindings.keyBindings[i].name, valp, type);
   }
}

static void createClassPath(CMPIObjectPath **op, char *ns, char *cn)
{   
   *op = newCMPIObjectPath(NULL, cn, NULL);
}

static void setInstProperties(CMPIInstance *ci, XtokProperties *ps)
{
   XtokProperty *np=NULL,*p= ps ? ps->first : NULL;
   CMPIValue val;
   
   while (p) {
      switch (p->propType) {
      case typeProperty_Value: ;
         val=str2CMPIValue(p->valueType, p->val.value, NULL);
         ci->ft->setProperty(ci,p->name,&val,p->valueType);
         break;
      case typeProperty_Reference: ;
      case typeProperty_Array: ;
         break;
      }   
      np=p->next;
      free(p);
      p=np;
   } 
   if (ps) ps->first=ps->last=NULL;
}

static void setClassProperties(CMPIConstClass *cls, XtokProperties *ps)
{
   XtokProperty *np=NULL,*p= ps ? ps->first : NULL;
   CMPIValue val;
   CMPIValueState state;
   
   while (p) {
      switch (p->propType) {
      case typeProperty_Value: 
         if (p->val.null) state=CMPI_nullValue;
         else {
            state=0;
            val=str2CMPIValue(p->valueType, p->val.value, NULL);
         }   
         addClassProperty(cls,p->name,&val,p->valueType,state);
         break;
      case typeProperty_Reference: ;
      case typeProperty_Array: ;
         break;
      }   
      np=p->next;
      free(p);
      p=np;
   } 
   if (ps) ps->first=ps->last=NULL;
}

static void addProperty(XtokProperties *ps, XtokProperty *p)
{
   XtokProperty *np;
   np=(XtokProperty*)malloc(sizeof(XtokProperty));
   memcpy(np,p,sizeof(XtokProperty));
   np->next=NULL;
   if (ps->last) {
      ps->last->next=np;
   }
   else ps->first=np;
   ps->last=np;
}

static void addParamValue(XtokParamValues *vs, XtokParamValue *v)
{
   XtokParamValue *nv;
   nv=(XtokParamValue*)malloc(sizeof(XtokParamValue));
   memcpy(nv,v,sizeof(XtokParamValue));
   nv->next=NULL;
   if (vs->last) {
      vs->last->next=nv;
   }
   else vs->first=nv;
   vs->last=nv;
}

static void addQualifier(XtokQualifiers *qs, XtokQualifier *q)
{
   XtokQualifier *nq;
   nq=(XtokQualifier*)malloc(sizeof(XtokQualifier));
   memcpy(nq,q,sizeof(XtokQualifier));
   nq->next=NULL;
   if (qs->last) {
      qs->last->next=nq;
   }
   else qs->first=nq;
   qs->last=nq;
}

static void addMethod(XtokMethods *ms, XtokMethod *m)
{
   XtokMethod *nm;
   nm=(XtokMethod*)malloc(sizeof(XtokMethod));
   memcpy(nm,m,sizeof(XtokMethod));
   nm->next=NULL;
   if (ms->last) {
      ms->last->next=nm;
   }
   else ms->first=nm;
   ms->last=nm;
}

static void addParam(XtokParams *ps, XtokParam *p)
{
   XtokParam *np;
   np=(XtokParam*)malloc(sizeof(XtokParam));
   memcpy(np,p,sizeof(XtokParam));
   np->next=NULL;
   if (ps->last) {
      ps->last->next=np;
   }
   else ps->first=np;
   ps->last=np;
}

static void setError(void *parm, XtokErrorResp *e)
{
   int err=atoi(e->code);
   fprintf(stderr,"error:: %s %s\n",e->code,e->description);
   PARM->respHdr.errCode=err;
   PARM->respHdr.description=strdup(e->description);
}
   
%}

%pure_parser

/*
**==============================================================================
**
** Union used to pass tokens from Lexer to this Parser.
**
**==============================================================================
*/

%union
{
   int                           intValue;
   char                          boolValue;
   char*                         className;
   void*                         tokCim;

   XtokMessage                   xtokMessage;
   XtokErrorResp                 xtokErrorResp;
   
   XtokNameSpace                 xtokNameSpace;
   char*                         xtokLocalNameSpacePath;
   XtokNameSpacePath             xtokNameSpacePath;
   XtokHost                      xtokHost;
   XtokInstancePath              xtokInstancePath;
   XtokLocalInstancePath         xtokLocalInstancePath;
   XtokLocalClassPath            xtokLocalClassPath;

   XtokValue                     xtokValue;
   XtokValueArray                xtokValueArray;
   XtokValueReference            xtokValueReference;
   XtokObjectWithPath            xtokObjectWithPath;
   XtokInstanceName              xtokInstanceName;
   XtokKeyBinding                xtokKeyBinding;
   XtokKeyBindings               xtokKeyBindings;
   XtokKeyValue                  xtokKeyValue;

   XtokClass                     xtokClass;
   XtokInstance                  xtokInstance;
   XtokNamedInstance             xtokNamedInstance;

   XtokProperty                  xtokProperty;
   XtokPropertyData              xtokPropertyData;

   XtokMethod                    xtokMethod;
   XtokMethodData                xtokMethodData;
   XtokQualifier                 xtokQualifier;
   
   XtokParamValue                xtokParamValue;  
   XtokParam                     xtokParam;
};

%token <tokCim>                  XTOK_XML
%token <intValue>                ZTOK_XML
%type <tokCim>                   cimOperation


%token <tokCim>                  XTOK_CIM
%token <intValue>                ZTOK_CIM

%token <xtokMessage>             XTOK_MESSAGE
%token <intValue>                ZTOK_MESSAGE
%type  <xtokMessage>             message

%token <intValue>                XTOK_SIMPLERSP
%token <intValue>                ZTOK_SIMPLERSP

%token <intValue>                XTOK_IMETHODRESP
%token <intValue>                ZTOK_IMETHODRESP
%type  <xtokImethodResp>         iMethodResp 

%token <intValue>                XTOK_METHODRESP
%token <intValue>                ZTOK_METHODRESP
//%type  <xtokMethodResp>          methodResp

%token <intValue>                XTOK_ERROR
%token <intValue>                ZTOK_ERROR
%type  <xtokErrorResp>           errorResp

%token <intValue>                XTOK_IRETVALUE
%token <intValue>                ZTOK_IRETVALUE

%token <intValue>                XTOK_RETVALUE
%token <intValue>                ZTOK_RETVALUE
//%type  <xtokRetValue>            retValue

%token <xtokNameSpacePath>       XTOK_NAMESPACEPATH
%token <intValue>                ZTOK_NAMESPACEPATH
%type  <xtokNameSpacePath>       nameSpacePath

%token <xtokLocalNameSpacePath>  XTOK_LOCALNAMESPACEPATH
%token <intValue>                ZTOK_LOCALNAMESPACEPATH
%type  <xtokLocalNameSpacePath>  localNameSpacePath

%token <xtokNameSpace>           XTOK_NAMESPACE
%token <intValue>                ZTOK_NAMESPACE
%type  <xtokNameSpace>           namespaces

%token <intValue>                ZTOK_IPARAMVALUE

%token <xtokHost>                XTOK_HOST
%type  <xtokHost>                host
%token <intValue>                ZTOK_HOST

%token <xtokValue>               XTOK_VALUE
%type  <xtokValue>               value
%token <intValue>                ZTOK_VALUE

%token <xtokValueArray>          XTOK_VALUEARRAY
%type  <xtokValueArray>          valueArray
%token <intValue>                ZTOK_VALUEARRAY

%token <intValueReference>       XTOK_VALUEREFERENCE
%type  <xtokValueReference>      valueReference
%token <intValue>                ZTOK_VALUEREFERENCE

%token <className>               XTOK_CLASSNAME
%token <intValue>                ZTOK_CLASSNAME
%type  <className>               className

%token <xtokInstanceName>        XTOK_INSTANCENAME
%token <intValue>                ZTOK_INSTANCENAME
%type  <xtokInstanceName>        instanceName

%token <xtokKeyBinding>          XTOK_KEYBINDING
%token <intValue>                ZTOK_KEYBINDING
%type  <xtokKeyBinding>          keyBinding
%type  <xtokKeyBindings>         keyBindings

%token <xtokKeyValue>            XTOK_KEYVALUE
%token <intValue>                ZTOK_KEYVALUE

%type  <boolValue>               boolValue

%token <xtokNamedInstance>       XTOK_VALUENAMEDINSTANCE
%token <intValue>                ZTOK_VALUENAMEDINSTANCE
%type  <xtokNamedInstance>       namedInstance

%token <xtokQualifier>           XTOK_QUALIFIER
%type  <xtokQualifier>           qualifier
%token <intValue>                ZTOK_QUALIFIER

%token <xtokProperty>            XTOK_PROPERTY
%token <intValue>                ZTOK_PROPERTY
%token <xtokPropertyArray>       XTOK_PROPERTYARRAY
%token <intValue>                ZTOK_PROPERTYARRAY
%token <xtokProperty>            XTOK_PROPERTYREFERENCE
%token <intValue>                ZTOK_PROPERTYREFERENCE

%type  <xtokPropertyData>        propertyData
%type  <xtokProperty>            property

%token <xtokParam>               XTOK_PARAM
%type  <xtokParam>               parameter
%token <intValue>                ZTOK_PARAM
%token <xtokParam>               XTOK_PARAMARRAY
%token <intValue>                ZTOK_PARAMARRAY
%token <xtokParam>               XTOK_PARAMREF
%token <intValue>                ZTOK_PARAMREF
%token <xtokParam>               XTOK_PARAMREFARRAY
%token <intValue>                ZTOK_PARAMREFARRAY

%token <xtokMethod>              XTOK_METHOD
%type  <xtokMethod>              method
%token <intValue>                ZTOK_METHOD
%type  <xtokMethodData>          methodData

%token <xtokClass>               XTOK_CLASS
%token <intValue>                ZTOK_CLASS
%type  <xtokClass>               class
%type  <xtokClassData>           classData

%token <xtokInstance>            XTOK_INSTANCE
%token <intValue>                ZTOK_INSTANCE
%type  <xtokInstance>            instance
%type  <xtokInstanceData>        instanceData

%token <xtokObjectWithPath>      XTOK_VALUEOBJECTWITHPATH
%token <intValue>                ZTOK_VALUEOBJECTWITHPATH
%type  <xtokObjectWithPath>      objectWithPath

//%type  <xtokParamValue>          paramValue
%token <xtokParamValue>          XTOK_PARAMVALUE
%token <intValue>                ZTOK_PARAMVALUE

%type  <xtokInstancePath>        instancePath
%token <xtokInstancePath>        XTOK_INSTANCEPATH
%token <intValue>                ZTOK_INSTANCEPATH

%type  <xtokLocalInstancePath>   localInstancePath
%token <xtokLocalInstancePath>   XTOK_LOCALINSTANCEPATH
%token <intValue>                ZTOK_LOCALINSTANCEPATH

%type  <xtokLocalClassPath>      localClassPath
%token <xtokLocalClassPath>      XTOK_LOCALCLASSPATH
%token <intValue>                ZTOK_LOCALCLASSPATH

%%

/*
**==============================================================================
**
** The grammar itself.
**
**==============================================================================
*/

start
    : XTOK_XML ZTOK_XML cimOperation
    {
    }
;

cimOperation
    : XTOK_CIM message ZTOK_CIM
    {
    }
;

message
    : XTOK_MESSAGE simpleResp ZTOK_MESSAGE
    {
    }
;

simpleResp
    : /* XTOK_SIMPLERSP methodResp ZTOK_SIMPLERSP
    {
    }
    | */ XTOK_SIMPLERSP iMethodResp ZTOK_SIMPLERSP
    {
    }
;

iMethodResp
    : XTOK_IMETHODRESP errorResp ZTOK_IMETHODRESP
    {
    }
    | XTOK_IMETHODRESP iReturnValue ZTOK_IMETHODRESP
    {
    }
    | XTOK_IMETHODRESP ZTOK_IMETHODRESP
    {
    }
;

errorResp
    : XTOK_ERROR ZTOK_ERROR
    {
        setError(parm,&$$);
    }
;
    
iReturnValue
    : XTOK_IRETVALUE instanceNames ZTOK_IRETVALUE
    {
    }
    |  XTOK_IRETVALUE classNames ZTOK_IRETVALUE
    {
    }
    | /* XTOK_IRETVALUE values ZTOK_IRETVALUE
    {
    }
    | XTOK_IRETVALUE valueObjectsWithPath ZTOK_IRETVALUE
    {
    }
    | XTOK_IRETVALUE valueObjectsWithLocalPath ZTOK_IRETVALUE
    {
    }
    | XTOK_IRETVALUE valueObjects ZTOK_IRETVALUE
    {
    }
    | XTOK_IRETVALUE objectPathes ZTOK_IRETVALUE
    {
    }
    | XTOK_IRETVALUE valueArray ZTOK_IRETVALUE
    {
    }
    | XTOK_IRETVALUE valueRef ZTOK_IRETVALUE
    {
    } */
    | XTOK_IRETVALUE objectsWithPath ZTOK_IRETVALUE
    {
    }
    | XTOK_IRETVALUE classes ZTOK_IRETVALUE
    {
    } 
    | XTOK_IRETVALUE instances ZTOK_IRETVALUE
    {
    } 
    | XTOK_IRETVALUE namedInstances ZTOK_IRETVALUE
    {
    } 
;
 

 
classes
    : /* empty */
    | class
    {
       PARM->curClass=native_new_CMPIConstClass($1.className,NULL);
       setClassProperties(PARM->curClass,&PARM->properties);
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)&PARM->curClass,CMPI_class);
       PARM->curClass=NULL;
    }    
    | classes class
    {
       PARM->curClass=native_new_CMPIConstClass($2.className,NULL);
       setClassProperties(PARM->curClass,&PARM->properties);
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)&PARM->curClass,CMPI_class);
       PARM->curClass=NULL;
    }    
;

instances
    : /* empty */
    | instance
    {
       PARM->curInstance=native_new_CMPIInstance(NULL,NULL);
       setInstNsAndCn(PARM->curInstance,PARM->nameSpace,$1.className);
       setInstProperties(PARM->curInstance,&PARM->properties);
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)&PARM->curInstance,CMPI_instance);
       PARM->curInstance=NULL;
    }    
    | instances instance
    {
       PARM->curInstance=native_new_CMPIInstance(NULL,NULL);
       setInstNsAndCn(PARM->curInstance,PARM->nameSpace,$2.className);
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)&PARM->curInstance,CMPI_instance);
       PARM->curInstance=NULL;
    }    
;

instanceNames
    : /* empty */
    | instanceName
    {
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)&PARM->curPath,CMPI_ref);
       PARM->curPath=NULL;
    }    
    | instanceNames instanceName
    {
//       setInstNsAndCn(PARM->curInstance,PARM->nameSpace,$2.className);
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)&PARM->curPath,CMPI_ref);
       PARM->curPath=NULL;
    }    
;

objectsWithPath
    : /* empty */
    | objectWithPath
    {
       PARM->curInstance=native_new_CMPIInstance(NULL,NULL);
       setInstNsAndCn(PARM->curInstance,PARM->nameSpace,$1.instance.className);
       setInstProperties(PARM->curInstance,&PARM->properties);
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)&PARM->curInstance,CMPI_instance);
       PARM->curInstance=NULL;
    }    
    | objectsWithPath objectWithPath 
    {
       PARM->curInstance=native_new_CMPIInstance(NULL,NULL);
       setInstNsAndCn(PARM->curInstance,PARM->nameSpace,$2.instance.className);
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)&PARM->curInstance,CMPI_instance);
       PARM->curInstance=NULL;
    }    
;


namedInstances
    : /* empty */
    | namedInstance
    {
       PARM->curInstance=native_new_CMPIInstance(NULL,NULL);
       setInstNsAndCn(PARM->curInstance,PARM->nameSpace,$1.instance.className);
       setInstProperties(PARM->curInstance,&PARM->properties);
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)&PARM->curInstance,CMPI_instance);
       PARM->curInstance=NULL;
    }    
    | namedInstances namedInstance 
    {
       PARM->curInstance=native_new_CMPIInstance(NULL,NULL);
       setInstNsAndCn(PARM->curInstance,PARM->nameSpace,$2.instance.className);
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)&PARM->curInstance,CMPI_instance);
       PARM->curInstance=NULL;
    }    
;


/*
 *    valueNamedInstance
*/

namedInstance
    : XTOK_VALUENAMEDINSTANCE instanceName instance ZTOK_VALUENAMEDINSTANCE
    {
       $$.path=$2;
       $$.instance=$3;
    }
;


/*
 *    objectWithPath
*/

objectWithPath
    : XTOK_VALUEOBJECTWITHPATH instancePath instance ZTOK_VALUEOBJECTWITHPATH
    {
       $$.path=$2;
       $$.instance=$3;
    }
;



/*
 *    class
*/

class
    : XTOK_CLASS classData ZTOK_CLASS
    {
       if (PARM->Qs) 
          $$.qualifiers=PARM->qualifiers;
       else memset(&$$.qualifiers,0,sizeof($$.qualifiers));
       if (PARM->Ps) 
          $$.properties=PARM->properties;
       else memset(&$$.properties,0,sizeof($$.properties));
       if (PARM->Ms) 
          $$.methods=PARM->methods;
       else memset(&$$.methods,0,sizeof($$.methods));
    }
;

classData
    : /* empty */ {;}
    | classData qualifier
    {
       PARM->Qs++;
       addQualifier(&(PARM->qualifiers),&$2);
    }
    | classData property     {
       PARM->Ps++;
       addProperty(&(PARM->properties),&$2);
    }
    | classData method     {
        PARM->Ms++;
        addMethod(&(PARM->methods),&$2);
    }
;

method
    : XTOK_METHOD methodData ZTOK_METHOD
    {
       if (PARM->MQs) 
          $$.qualifiers=$2.qualifiers;
       else memset(&$$.qualifiers,0,sizeof($$.qualifiers));
       if (PARM->MPs) 
          $$.params=$2.params;
       else memset(&$$.params,0,sizeof($$.params));
       PARM->MQs=0; 
       PARM->MPs=0; 
       PARM->MPQs=0; 
    }   
;

methodData 
    : /* empty */ {;}
    | methodData qualifier
    {
       if (PARM->MQs==0) 
          memset(&$$.qualifiers,0,sizeof($$.qualifiers));
       PARM->MQs++;
       addQualifier(&($$.qualifiers),&$2);
    }      
    | methodData XTOK_PARAM parameter ZTOK_PARAM 
    {
       if (PARM->MPs==0) 
          memset(&$$.params,0,sizeof($$.params));
       PARM->MPs++;
       if (PARM->MPQs) 
          $2.qualifiers=$3.qualifiers;
       else memset(&$2.qualifiers,0,sizeof($2.qualifiers));
       addParam(&($$.params),&$2);
       PARM->MPQs=0; 
    }      
;  

parameter 
    : /* empty */ {;}
    | parameter qualifier
    {
       if (PARM->MPQs==0) 
          memset(&$$.qualifiers,0,sizeof($$.qualifiers));
       PARM->MPQs++; 
       addQualifier(&($$.qualifiers),&$2);
    }
;


/*
 *    instance
*/

instance
    : XTOK_INSTANCE instanceData ZTOK_INSTANCE
    {
       if (PARM->Qs) 
          $$.qualifiers=PARM->qualifiers;
       else memset(&$$.qualifiers,0,sizeof($$.qualifiers));
       if (PARM->Ps) 
          $$.properties=PARM->properties;
       else memset(&$$.properties,0,sizeof($$.properties)); 
    }
;

instanceData 
    : /* empty */ {;}
    | instanceData qualifier 
    {
       PARM->Qs++;
       addQualifier(&(PARM->qualifiers),&$2);
    }
    | instanceData property 
    {
       PARM->Ps++;
       addProperty(&(PARM->properties),&$2);
    }
;


/*
 *    property
*/

property
    : XTOK_PROPERTY propertyData ZTOK_PROPERTY
    {
       $$.val=$2;
    }  
    | XTOK_PROPERTYREFERENCE propertyData ZTOK_PROPERTYREFERENCE
    {
       $$.val=$2;
    }
    | XTOK_PROPERTYARRAY propertyData ZTOK_PROPERTYARRAY
    {
       $$.val=$2;
    }
;

propertyData 
    : /* empty */ {$$.null=1;}
    | propertyData qualifier
    {
       addQualifier(&(PARM->qualifiers),&$2);
    }
    | propertyData value
    {
    //   printf("--- value: %s\n",$1.value);
       $$.value=$2.value;
       $$.null=0;
    }
    | propertyData valueReference
    {
       $$.ref=$2;
       $$.null=0;
    }
    | propertyData XTOK_VALUEARRAY valueArray ZTOK_VALUEARRAY
    {
    //   printf("--- valueArray: \n");
       $$.array=$2;
       $$.null=0;
    }
;  

//propertyArray
//    : XTOK_PROPERTYARRAY  ZTOK_PROPERTYARRAY
//    {
  //  printf("--- propertyArray\n");
//    }
//;




/*
 *    value
*/


value
    : XTOK_VALUE ZTOK_VALUE
    {
       $$.value=$1.value;
    }
;

valueArray 
    : value
    {
       $$.next=1;
       $$.max=128;
       $$.values=(char**)malloc(sizeof(char*)*128);
       $$.values[0]=$1.value;
    }
    | valueArray value
    {
       if ($$.next>$$.max) {
          $$.max*=2;
          $$.values=(char**)realloc($$.values,$$.max);
       }
       $$.values[$$.next]=$2.value;
       $$.next++;
    }
;

valueReference
    : XTOK_VALUEREFERENCE instancePath ZTOK_VALUEREFERENCE
    {
       $$.instancePath=$2;
       $$.type=typeValRef_InstancePath;
    }
    | XTOK_VALUEREFERENCE instanceName ZTOK_VALUEREFERENCE
    {
       $$.instanceName=$2;
       $$.type=typeValRef_InstanceName;
    }
;

boolValue
    : XTOK_VALUE ZTOK_VALUE
    {
//       int b=isBoolean($1.val);
//       if (b>=0) $$=(b!=0);
    }
;

classNames
    : /* empty */
    | className
    {
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)&PARM->curPath,CMPI_ref);
       PARM->curPath=NULL;
    }    
    | classNames className
    {
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)&PARM->curPath,CMPI_ref);
       PARM->curPath=NULL;
    }    
;

className
    : XTOK_CLASSNAME ZTOK_CLASSNAME
    {
        createClassPath(&PARM->curPath, NULL, $$);
    }
;

/*
 *    qualifier
*/

qualifier
    : XTOK_QUALIFIER value ZTOK_QUALIFIER
    {
       $$.value=$2.value;
    }
    | XTOK_QUALIFIER XTOK_VALUEARRAY valueArray ZTOK_VALUEARRAY ZTOK_QUALIFIER
    {
//       $$.value=$2.value;
    }
;


/*
 *    localNameSpacePath 
*/



localNameSpacePath
    : XTOK_LOCALNAMESPACEPATH namespaces ZTOK_LOCALNAMESPACEPATH
    {
       $$=$2.cns;
    }
;

namespaces
    : XTOK_NAMESPACE ZTOK_NAMESPACE
    {
       $$.cns=strdup($1.ns);
    }
    | namespaces XTOK_NAMESPACE ZTOK_NAMESPACE
    {
       int l=strlen($1.cns)+strlen($2.ns)+2;
       $$.cns=(char*)malloc(l);
       strcpy($$.cns,$1.cns);
       strcat($$.cns,"/");
       strcat($$.cns,$2.ns);
       free($1.cns);
    }
;


nameSpacePath
    : XTOK_NAMESPACEPATH host localNameSpacePath ZTOK_NAMESPACEPATH
    {
       $$.host=$2;
       $$.nameSpacePath=$3;
    }
;

host
    : XTOK_HOST ZTOK_HOST
    {
    }
;

instancePath
    : XTOK_INSTANCEPATH nameSpacePath instanceName ZTOK_INSTANCEPATH
    {
       $$.path=$2;
       $$.instanceName=$3;
       $$.type=1;
    }
 /*
    | nameSpacePath instanceName
    {
    }
    | XTOK_CLASSPATH nameSpacePath className  ZTOK_CLASSPATH
    {
    }
    | XTOK_LOCALCLASSPATH localNameSpacePath className ZTOK_LOCALCLASSPATH
    {
    } */
;

localInstancePath
    : XTOK_LOCALINSTANCEPATH localNameSpacePath instanceName ZTOK_LOCALINSTANCEPATH
    {
       $$.path=$2;
       $$.instanceName=$3;
       $$.type=1;
    }
;

localClassPath
    : XTOK_LOCALCLASSPATH localNameSpacePath className ZTOK_LOCALCLASSPATH
    {
       $$.path=$2;
       $$.className=$3;
       $$.type=1;
    }
;



/*
 *    instanceName
*/


instanceName
    : XTOK_INSTANCENAME ZTOK_INSTANCENAME
    {
       $$.className=$1.className;
       $$.bindings.next=0;
       $$.bindings.keyBindings=NULL;
       PARM->curPath=NULL;
    }
    | XTOK_INSTANCENAME keyBindings ZTOK_INSTANCENAME
    {
       $$.className=$1.className;
       $$.bindings=$2;
       createPath(&(PARM->curPath), &$$);
    }
;

keyBindings
    : keyBinding
    {
       $$.next=1;
       $$.max=16;
       $$.keyBindings=(XtokKeyBinding*)malloc(sizeof(XtokKeyBinding)*16);
       $$.keyBindings[0].name=$1.name;
       $$.keyBindings[0].value=$1.value;
       $$.keyBindings[0].type=$1.type;
       $$.keyBindings[0].ref=$1.ref;
    }
    | keyBindings keyBinding
    {
       $$.keyBindings[$$.next].name=$2.name;
       $$.keyBindings[$$.next].value=$2.value;
       $$.keyBindings[$$.next].type=$2.type;
       $$.keyBindings[$$.next].ref=$2.ref;
       $$.next++;
    }
;

keyBinding
    : XTOK_KEYBINDING XTOK_KEYVALUE ZTOK_KEYVALUE ZTOK_KEYBINDING
    {
       $$.name=$1.name;
       $$.value=$2.value;
       $$.type=$2.valueType;
    }
    | XTOK_KEYBINDING valueReference ZTOK_KEYBINDING
    {
       $$.name=$1.name;
       $$.value=NULL;
       $$.type="ref";
       $$.ref=$2;
    }
;

%%
