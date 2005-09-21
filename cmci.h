
/*
 * cmpic.h
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
 * Author:        Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * CMPI Client function tables.
 *
*/

#ifndef _CMPIC_H_
#define _CMPIC_H_

struct _CMPIConstClass;
typedef struct _CMPIConstClass CMPIConstClass;
   
#include "cmcidt.h"
#include "cmcift.h"
#include "cmcimacs.h"
#include "utilList.h"

#ifdef __cplusplus
extern "C" {
#endif



   //---------------------------------------------------
   //--
   //	_CMCIClientFt Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers to client CIMOM services.
   */
#define native_new_CMPIObjectPath   newCMPIObjectPath

CMPIObjectPath * newCMPIObjectPath ( const char * my_nameSpace, 
					     const char * classname,
					     CMPIStatus * rc );

struct _CMCIClient;
typedef struct _CMCIClient CMCIClient;
   
struct _CMCIConnection;
typedef struct _CMCIConnection CMCIConnection;
   
struct _ClientEnc;
typedef struct _ClientEnc ClientEnc;

typedef struct _CMCIClientFT {
   
      /** Get Class using &lt;op&gt; as reference. Class structure can be
         controled using the flags parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param flags Any combination of the following flags are supported: 
	    CMPI_FLAG_LocalOnly, CMPI_FLAG_IncludeQualifiers and CMPI_FLAG_IncludeClassOrigin.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The Class.
     */
     CMPIConstClass* (*getClass)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIFlags flags, char** properties, CMPIStatus* rc);


       /** Enumerate Class Names (and subclass names) in the nameSpace defined by &lt;op&gt;. Inheritance scope can be controled using the
	        flags parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace component.
	 @param flags The following flag is supported: CMPI_FLAG_DeepInheritance.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of ObjectPathes.
     */
     
     CMPIEnumeration* (*enumClassNames)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIFlags flags, CMPIStatus* rc);

     /** Enumerate Classes (and subclasses) in the nameSpace  defined by &lt;op&gt;.
         Class structure and inheritance scope can be controled using the &lt;flags&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param flags Any combination of the following flags are supported: CMPI_FLAG_LocalOnly, 
	     CMPI_FLAG_DeepInheritance, CMPI_FLAG_IncludeQualifiers and CMPI_FLAG_IncludeClassOrigin.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of Instances.
     */
     
     CMPIEnumeration* (*enumClasses)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIFlags flags, CMPIStatus* rc);

      /** Get Instance using &lt;op&gt; as reference. Instance structure can be
         controled using the &lt;flags&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param flags Any combination of the following flags are supported: 
	    CMPI_FLAG_LocalOnly, CMPI_FLAG_IncludeQualifiers and CMPI_FLAG_IncludeClassOrigin.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The Instance.
     */
     CMPIInstance* (*getInstance)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIFlags flags, char** properties, CMPIStatus* rc);

      /** Create Instance from &lt;inst&gt; using &lt;op&gt; as reference.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param inst Complete instance.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The assigned instance reference.
     */
     CMPIObjectPath* (*createInstance)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIInstance* inst, CMPIStatus* rc);

      /** Replace an existing Instance from &lt;inst&gt; using &lt;op&gt; as reference.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param inst Complete instance.
	 @param flags The following flag is supported: CMPI_FLAG_IncludeQualifiers.
	 @param properties If not NULL, the members of the array define one or more Property
	     names, only those properties will be updated. Else, all properties will be updated. 
	 @return Service return status.
     */
     CMPIStatus (*setInstance)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIInstance* inst, CMPIFlags flags, char ** properties);

      /** Delete an existing Instance using &lt;op&gt; as reference.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @return Service return status.
     */
     CMPIStatus (*deleteInstance)
                (CMCIClient* cl,
                 CMPIObjectPath* op);

      /** Query the enumeration of instances of the class (and subclasses) defined
         by &lt;op&gt; using &lt;query&gt; expression.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param query Query expression
	 @param lang Query Language
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Resulting eumeration of Instances.
     */
     CMPIEnumeration* (*execQuery)
                (CMCIClient* cl,
                 CMPIObjectPath* op, const char *query, const char *lang, CMPIStatus* rc);

       /** Enumerate Instance Names of the class (and subclasses) defined by &lt;op&gt;.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of ObjectPathes.
     */
     CMPIEnumeration* (*enumInstanceNames)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIStatus* rc);

     /** Enumerate Instances of the class (and subclasses) defined by &lt;op&gt;.
         Instance structure and inheritance scope can be controled using the
	      &lt;flags&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param flags Any combination of the following flags are supported: CMPI_FLAG_LocalOnly, 
	     CMPI_FLAG_DeepInheritance, CMPI_FLAG_IncludeQualifiers and CMPI_FLAG_IncludeClassOrigin.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of Instances.
     */
     CMPIEnumeration* (*enumInstances)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIFlags flags, char** properties, CMPIStatus* rc);

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
	    CMPI_FLAG_IncludeQualifiers and CMPI_FLAG_IncludeClassOrigin.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of Instances.
     */
     CMPIEnumeration* (*associators)
                (CMCIClient* cl,
                 CMPIObjectPath* op, const char *assocClass, const char *resultClass,
		 const char *role, const char *resultRole, CMPIFlags flags, 
                 char** properties, CMPIStatus* rc);

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
     CMPIEnumeration* (*associatorNames)
                (CMCIClient* cl,
                 CMPIObjectPath* op, const char *assocClass, const char *resultClass,
		 const char *role, const char *resultRole, CMPIStatus* rc);

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
	    CMPI_FLAG_IncludeQualifiers and CMPI_FLAG_IncludeClassOrigin.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of ObjectPaths.
     */
     CMPIEnumeration* (*references)
                (CMCIClient* cl,
                 CMPIObjectPath* op, const char *resultClass ,const char *role ,
		 CMPIFlags flags, char** properties, CMPIStatus* rc);

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
     CMPIEnumeration* (*referenceNames)
                (CMCIClient* cl,
                 CMPIObjectPath* op, const char *resultClass ,const char *role,
                 CMPIStatus* rc);

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
     CMPIData (*invokeMethod)
                (CMCIClient* cl,
                 CMPIObjectPath* op,const char *method,
		 CMPIArgs* in, CMPIArgs* out, CMPIStatus* rc);

       /** Set the named property value of an Instance defined by the &lt;op&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param name Property name
	 @param value Value.
	 @param type Value type.
	 @return Service return status.
      */
     CMPIStatus (*setProperty)
                (CMCIClient* cl,
                 CMPIObjectPath* op, const char *name , CMPIValue* value,
                 CMPIType type);

       /** Get the named property value of an Instance defined by the &lt;op&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param name Property name
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
     CMPIData (*getProperty)
                (CMCIClient *cl, 
                 CMPIObjectPath *op, const char *name, CMPIStatus *rc);

} CMCIClientFT;


   //---------------------------------------------------
   //--
   //	_CMPIConstClass Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to Instance
       support sevices.
   */
typedef struct _CMPIConstClassFT {

       /** Function table version
       */
     int ftVersion;

       /** The ConstClass object will not be used any further and may be freed by
           CMPI run time system.
	 @param ccls ConstClass this pointer.
	 @return Service return status.
      */
     CMPIStatus (*release)
              (CMPIConstClass* ccls);

       /** Create an independent copy of this CpnstClass object. The resulting
           object must be released explicitly.
	 @param ccls ConstClass this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied ConstClass object.
      */
     CMPIConstClass* (*clone)
              (CMPIConstClass* ccls, CMPIStatus* rc);

       /** Gets the classname.
	 @param ccls ConstClass this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return classname.
      */     
     CMPIString *(*getClassName) 
              ( CMPIConstClass * ccls, CMPIStatus * rc );
                
       /** Gets a named property value.
	 @param ccls ConstClass this pointer.
	 @param name Property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
     CMPIData (*getProperty)
              (CMPIConstClass* ccls, const char *name, CMPIStatus* rc);

       /** Gets a Property value defined by its index.
	 @param ccls ConstClass this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
     CMPIData (*getPropertyAt)
              (CMPIConstClass* ccls, unsigned int index, CMPIString** name,
	       CMPIStatus* rc);

      /** Gets the number of properties contained in this Instance.
	 @param ccls ConstClass this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
     unsigned int (*getPropertyCount)
              (CMPIConstClass* ccls, CMPIStatus* rc);

} CMPIConstClassFT;


struct _CMPIConstClass {
   void *hdl;
   CMPIConstClassFT *ft;
};





typedef struct clientData {
   char *hostName;
   char *port;
   char *user;
   char *pwd;
   char *scheme;
   int  status;
} CMCIClientData;

struct _CMCIClient {
   void *hdl;
   CMCIClientFT *ft;
};   

CMCIClient *cmciConnect(const char *hn, const char *scheme, const char *port, 
                        const char *user, const char *pwd, CMPIStatus *rc);   

int sameCMPIObjectPath ( const CMPIObjectPath *cop1, const CMPIObjectPath *cop2);
   
#ifdef __cplusplus
 };
#endif

#endif // _CMPIC_H_
