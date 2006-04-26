
/*
 * cmpic.h
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
 * cimc Client function tables.
 *
*/

#ifndef _cimcC_H_
#define _cimcC_H_

#include "cimcdt.h"
#include "cimcft.h"

#ifdef __cplusplus
extern "C" {
#endif



   //---------------------------------------------------
   //--
   //	_cimcClientFt Function Table
   //--
   //---------------------------------------------------


struct _cimcClient;
typedef struct _cimcClient cimcClient;
   
struct _ClientEnc;
typedef struct _ClientEnc ClientEnc;

typedef cimcStatus (*CimcRelease)(cimcClient* cl);
typedef cimcClient *(*CimcClone) ( cimcClient * cl, cimcStatus * st );
typedef cimcConstClass* (*CimcGetClass)(cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, char** properties, cimcStatus* rc);
typedef cimcEnumeration* (*CimcEnumClassNames)(cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, cimcStatus* rc);
typedef cimcEnumeration* (*CimcEnumClasses)(cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, cimcStatus* rc);
typedef cimcInstance* (*CimcGetInstance)(cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, char** properties, cimcStatus* rc);
typedef cimcObjectPath* (*CimcCreateInstance)(cimcClient* cl,
                 cimcObjectPath* op, cimcInstance* inst, cimcStatus* rc);
typedef cimcStatus (*CimcSetInstance)(cimcClient* cl,
                 cimcObjectPath* op, cimcInstance* inst, cimcFlags flags, char ** properties);
typedef cimcStatus (*CimcDeleteInstance)(cimcClient* cl,
                 cimcObjectPath* op);
typedef cimcEnumeration* (*CimcExecQuery)(cimcClient* cl,
                 cimcObjectPath* op, const char *query, const char *lang, cimcStatus* rc);
typedef cimcEnumeration* (*CimcEnumInstanceNames)(cimcClient* cl,
                 cimcObjectPath* op, cimcStatus* rc);
typedef cimcEnumeration* (*CimcEnumInstances)(cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, char** properties, cimcStatus* rc);
typedef cimcEnumeration* (*CimcAssociators)(cimcClient* cl,
                 cimcObjectPath* op, const char *assocClass, const char *resultClass,
		 const char *role, const char *resultRole, cimcFlags flags, 
                 char** properties, cimcStatus* rc);
typedef cimcEnumeration* (*CimcAssociatorNames)(cimcClient* cl, 
                 cimcObjectPath* op, const char *assocClass, const char *resultClass,
		 const char *role, const char *resultRole, cimcStatus* rc);
typedef cimcEnumeration* (*CimcReferences)(cimcClient* cl, cimcObjectPath* op, const char *resultClass ,
                 const char *role, cimcFlags flags, char** properties, cimcStatus* rc);
typedef cimcEnumeration* (*CimcReferenceNames)(cimcClient* cl,
                 cimcObjectPath* op, const char *resultClass ,const char *role, cimcStatus* rc);
typedef cimcData (*CimcInvokeMethod)(cimcClient* cl, cimcObjectPath* op,const char *method,
		 cimcArgs* in, cimcArgs* out, cimcStatus* rc);
typedef cimcStatus (*CimcSetProperty)(cimcClient* cl,
                 cimcObjectPath* op, const char *name , cimcValue* value, cimcType type);
typedef cimcData (*CimcGetProperty)(cimcClient *cl, 
                 cimcObjectPath *op, const char *name, cimcStatus *rc);

     

typedef struct _cimcClientFT {

       /** Function table version
       */
     int ftVersion;

      /** The Client object will not be used any further and may be freed by
           cimc run time system.
	 @param cl Client this pointer.
	 @return Service return status.
      */
     cimcStatus (*release)
              (cimcClient* cl);
   
      /** The clone operation is not supported. 
           Inserted here to conform with other cimc function table layouts.
	 @param cl Client this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Always NULL.
      */
     cimcClient *(*clone) ( cimcClient * cl, cimcStatus * st );
     
     
      /** Get Class using &lt;op&gt; as reference. Class structure can be
         controled using the flags parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param flags Any combination of the following flags are supported: 
	    CIMC_FLAG_LocalOnly, CIMC_FLAG_IncludeQualifiers and CIMC_FLAG_IncludeClassOrigin.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The Class.
     */
     cimcConstClass* (*getClass)
                (cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, char** properties, cimcStatus* rc);


       /** Enumerate Class Names (and subclass names) in the nameSpace defined by &lt;op&gt;. Inheritance scope can be controled using the
	        flags parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace component.
	 @param flags The following flag is supported: CIMC_FLAG_DeepInheritance.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of ObjectPathes.
     */
     
     cimcEnumeration* (*enumClassNames)
                (cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, cimcStatus* rc);

     /** Enumerate Classes (and subclasses) in the nameSpace  defined by &lt;op&gt;.
         Class structure and inheritance scope can be controled using the &lt;flags&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param flags Any combination of the following flags are supported: CIMC_FLAG_LocalOnly, 
	     CIMC_FLAG_DeepInheritance, CIMC_FLAG_IncludeQualifiers and CIMC_FLAG_IncludeClassOrigin.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of Instances.
     */
     
     cimcEnumeration* (*enumClasses)
                (cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, cimcStatus* rc);

      /** Get Instance using &lt;op&gt; as reference. Instance structure can be
         controled using the &lt;flags&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param flags Any combination of the following flags are supported: 
	    CIMC_FLAG_LocalOnly, CIMC_FLAG_IncludeQualifiers and CIMC_FLAG_IncludeClassOrigin.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The Instance.
     */
     cimcInstance* (*getInstance)
                (cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, char** properties, cimcStatus* rc);

      /** Create Instance from &lt;inst&gt; using &lt;op&gt; as reference.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param inst Complete instance.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The assigned instance reference.
     */
     cimcObjectPath* (*createInstance)
                (cimcClient* cl,
                 cimcObjectPath* op, cimcInstance* inst, cimcStatus* rc);

      /** Replace an existing Instance from &lt;inst&gt; using &lt;op&gt; as reference.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param inst Complete instance.
	 @param flags The following flag is supported: CIMC_FLAG_IncludeQualifiers.
	 @param properties If not NULL, the members of the array define one or more Property
	     names, only those properties will be updated. Else, all properties will be updated. 
	 @return Service return status.
     */
     cimcStatus (*setInstance)
                (cimcClient* cl,
                 cimcObjectPath* op, cimcInstance* inst, cimcFlags flags, char ** properties);

      /** Delete an existing Instance using &lt;op&gt; as reference.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @return Service return status.
     */
     cimcStatus (*deleteInstance)
                (cimcClient* cl,
                 cimcObjectPath* op);

      /** Query the enumeration of instances of the class (and subclasses) defined
         by &lt;op&gt; using &lt;query&gt; expression.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param query Query expression
	 @param lang Query Language
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Resulting eumeration of Instances.
     */
     cimcEnumeration* (*execQuery)
                (cimcClient* cl,
                 cimcObjectPath* op, const char *query, const char *lang, cimcStatus* rc);

       /** Enumerate Instance Names of the class (and subclasses) defined by &lt;op&gt;.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of ObjectPathes.
     */
     cimcEnumeration* (*enumInstanceNames)
                (cimcClient* cl,
                 cimcObjectPath* op, cimcStatus* rc);

     /** Enumerate Instances of the class (and subclasses) defined by &lt;op&gt;.
         Instance structure and inheritance scope can be controled using the
	      &lt;flags&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param flags Any combination of the following flags are supported: CIMC_FLAG_LocalOnly, 
	     CIMC_FLAG_DeepInheritance, CIMC_FLAG_IncludeQualifiers and CIMC_FLAG_IncludeClassOrigin.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of Instances.
     */
     cimcEnumeration* (*enumInstances)
                (cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, char** properties, cimcStatus* rc);

      /** Enumerate instances associated with the Instance defined by the &lt;op&gt;.
	 @param cl Client this pointer.
	 @param op Source ObjectPath containing nameSpace, classname and key components.
	 @param assocClass If not NULL, MUST be a valid Association Class name.
	    It acts as a filter on the returned set of Objects by mandating that
	    each returned Object MUST be associated to the source Object via an
	    Instance of this Class or one of its subclasses.
	 @param resultClass If not NULL, MUST be a valid Class name.
	    It acts as a filter on the returned set of Objects by mandating that
	    each returned Object MUST be either an Instance of this Class (or one
	    of its subclasses).
	 @param role If not NULL, MUST be a valid Property name.
	    It acts as a filter on the returned set of Objects by mandating
	    that each returned Object MUST be associated to the source Object
	    via an Association in which the source Object plays the specified role
	    (i.e. the name of the Property in the Association Class that refers
	    to the source Object MUST match the value of this parameter).
	 @param resultRole If not NULL, MUST be a valid Property name.
	    It acts as a filter on the returned set of Objects by mandating
	    that each returned Object MUST be associated to the source Object
	    via an Association in which the returned Object plays the specified role
	    (i.e. the name of the Property in the Association Class that refers to
	    the returned Object MUST match the value of this parameter).
	 @param flags Any combination of the following flags are supported: 
	    CIMC_FLAG_IncludeQualifiers and CIMC_FLAG_IncludeClassOrigin.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of Instances.
     */
     cimcEnumeration* (*associators)
                (cimcClient* cl,
                 cimcObjectPath* op, const char *assocClass, const char *resultClass,
		 const char *role, const char *resultRole, cimcFlags flags, 
                 char** properties, cimcStatus* rc);

      /** Enumerate ObjectPaths associated with the Instance defined by &lt;op&gt;.
	 @param cl Client this pointer.
	 @param op Source ObjectPath containing nameSpace, classname and key components.
	 @param assocClass If not NULL, MUST be a valid Association Class name.
	    It acts as a filter on the returned set of Objects by mandating that
	    each returned Object MUST be associated to the source Object via an
	    Instance of this Class or one of its subclasses.
	 @param resultClass If not NULL, MUST be a valid Class name.
	    It acts as a filter on the returned set of Objects by mandating that
	    each returned Object MUST be either an Instance of this Class (or one
	    of its subclasses).
	 @param role If not NULL, MUST be a valid Property name.
	    It acts as a filter on the returned set of Objects by mandating
	    that each returned Object MUST be associated to the source Object
	    via an Association in which the source Object plays the specified role
	    (i.e. the name of the Property in the Association Class that refers
	    to the source Object MUST match the value of this parameter).
	 @param resultRole If not NULL, MUST be a valid Property name.
	    It acts as a filter on the returned set of Objects by mandating
	    that each returned Object MUST be associated to the source Object
	    via an Association in which the returned Object plays the specified role
	    (i.e. the name of the Property in the Association Class that refers to
	    the returned Object MUST match the value of this parameter).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of ObjectPaths.
     */
     cimcEnumeration* (*associatorNames)
                (cimcClient* cl,
                 cimcObjectPath* op, const char *assocClass, const char *resultClass,
		 const char *role, const char *resultRole, cimcStatus* rc);

       /** Enumerates the association instances that refer to the instance defined by
           &lt;op&gt;.
	 @param cl Client this pointer.
	 @param op Source ObjectPath containing nameSpace, classname and key components.
	 @param resultClass If not NULL, MUST be a valid Class name.
	    It acts as a filter on the returned set of Objects by mandating that
	    each returned Object MUST be either an Instance of this Class (or one
	    of its subclasses).
	 @param role If not NULL, MUST be a valid Property name.
	    It acts as a filter on the returned set of Objects by mandating
	    that each returned Object MUST be associated to the source Object
	    via an Association in which the source Object plays the specified role
	    (i.e. the name of the Property in the Association Class that refers
	    to the source Object MUST match the value of this parameter).
	 @param flags Any combination of the following flags are supported: 
	    CIMC_FLAG_IncludeQualifiers and CIMC_FLAG_IncludeClassOrigin.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of ObjectPaths.
     */
     cimcEnumeration* (*references)
                (cimcClient* cl,
                 cimcObjectPath* op, const char *resultClass ,const char *role ,
		 cimcFlags flags, char** properties, cimcStatus* rc);

       /** Enumerates the association ObjectPaths that refer to the instance defined by
           &lt;op&gt;.
	 @param cl Client this pointer.
	 @param op Source ObjectPath containing nameSpace, classname and key components.
	 @param resultClass If not NULL, MUST be a valid Class name.
	    It acts as a filter on the returned set of Objects by mandating that
	    each returned Object MUST be either an Instance of this Class (or one
	    of its subclasses).
	 @param role If not NULL, MUST be a valid Property name.
	    It acts as a filter on the returned set of Objects by mandating
	    that each returned Object MUST be associated to the source Object
	    via an Association in which the source Object plays the specified role
	    (i.e. the name of the Property in the Association Class that refers
	    to the source Object MUST match the value of this parameter).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of ObjectPaths.
       */
     cimcEnumeration* (*referenceNames)
                (cimcClient* cl,
                 cimcObjectPath* op, const char *resultClass ,const char *role,
                 cimcStatus* rc);

       /** Invoke a named, extrinsic method of an Instance
         defined by the &lt;op&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param method Method name
	 @param in Input parameters.
	 @param out Output parameters.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Method return value.
      */
     cimcData (*invokeMethod)
                (cimcClient* cl,
                 cimcObjectPath* op,const char *method,
		 cimcArgs* in, cimcArgs* out, cimcStatus* rc);

       /** Set the named property value of an Instance defined by the &lt;op&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param name Property name
	 @param value Value.
	 @param type Value type.
	 @return Service return status.
      */
     cimcStatus (*setProperty)
                (cimcClient* cl,
                 cimcObjectPath* op, const char *name , cimcValue* value,
                 cimcType type);

       /** Get the named property value of an Instance defined by the &lt;op&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param name Property name
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
     cimcData (*getProperty)
                (cimcClient *cl, 
                 cimcObjectPath *op, const char *name, cimcStatus *rc);


} cimcClientFT;

struct _cimcClient {
   void *hdl;
   cimcClientFT *ft;
};

   
struct _cimcEnv;
typedef struct _cimcEnv cimcEnv;

typedef struct _cimcEnvFT {
   char *env;
   void* (*release)(cimcEnv *ce);
   cimcClient* (*connect)
               (cimcEnv *ce, const char *hn, const char *scheme, const char *port, 
                const char *user, const char *pwd,cimcStatus *rc);
   cimcClient* (*connect2)
               (cimcEnv *ce, const char *hn, const char *scheme, const char *port, 
                const char *user, const char *pwd, 
                int verifyMode, const char * trustStore,
                const char * certFile, const char * keyFile,
                cimcStatus *rc);
   cimcInstance* (*newInstance)
               (cimcEnv *ce, const cimcObjectPath* op, cimcStatus* rc);
   cimcObjectPath* (*newObjectPath)
               (cimcEnv *ce, const char *ns, const char *cn, cimcStatus* rc);
   cimcArgs* (*newArgs)
               (cimcEnv *ce, cimcStatus* rc);
   cimcString* (*newString)
               (cimcEnv *ce, const char *data, cimcStatus* rc);
   cimcArray* (*newArray)
               (cimcEnv *ce, cimcCount max, cimcType type, cimcStatus* rc);
   cimcDateTime* (*newDateTime)
               (cimcEnv *ce, cimcStatus* rc);
   cimcDateTime* (*newDateTimeFromBinary)
               (cimcEnv *ce, cimcUint64 binTime, cimcBoolean interval, cimcStatus* rc);
   cimcDateTime* (*newDateTimeFromChars)
               (cimcEnv *ce, const char *utcTime, cimcStatus* rc);
} cimcEnvFT;

struct _cimcEnv {
   void *hdl;
   cimcEnvFT *ft;
};


cimcEnv* NewCimcEnv(const char *id, unsigned int options, int *rc, char **msg);
void ReleaseCimcEnv(cimcEnv* env);

#include "cimcfc.h"

#ifdef __cplusplus
 };
#endif

#endif // _cimcC_H_
