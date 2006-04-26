
/*
 * cimcft.h
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
 * CMCI function calls.
 *
*/

#ifndef _CIMCFC_H_
#define _CIMCFC_H_

#include "cimcdt.h"

#ifdef __cplusplus
extern "C" {
#endif



#ifdef mmmmmmmmmm

       /** The ConstClass object will not be used any further and may be freed by
           cimc run time system.
         @param ccls ConstClass this pointer.
         @return Service return status.
       */
cimcStatus cimcClsRelease (cimcConstClass* ccls);

       /** Create an independent copy of this CpnstClass object. The resulting
           object must be released explicitly.
	 @param ccls ConstClass this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied ConstClass object.
      */
cimcConstClass cimsClsClone(cimcConstClass* ccls, cimcStatus* rc);

       /** Gets the classname.
	 @param ccls ConstClass this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return classname.
      */     
cimcString* cimcClsGetClassName(cimcConstClass * ccls, cimcStatus * rc );
                
       /** Gets a named property value.
	 @param ccls ConstClass this pointer.
	 @param name Property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
cimcData cimcClsGetProperty(cimcConstClass* ccls, const char *name, cimcStatus* rc);

       /** Gets a Property value defined by its index.
	 @param ccls ConstClass this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
cimcData cimcClsGetPropertyAt(cimcConstClass* ccls, unsigned int index, cimcString** name,
	       cimcStatus* rc);

      /** Gets the number of properties contained in this Instance.
	 @param ccls ConstClass this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
unsigned int cimcClsGetPropertyCount(cimcConstClass* ccls, cimcStatus* rc);

              
cimcData cimcClsGetQualifier(cimcConstClass * ccls,const char *name, cimcStatus* rc);
cimcData cimcClsGetQualifierAt(cimcConstClass * ccls,unsigned int index, cimcString** name, cimcStatus* rc);
unsigned int cimcClsGetQualifierCount(cimcConstClass * ccls,cimcStatus* rc);
              
cimcData cimcClsGetPropertyQualifier(cimcConstClass * ccls,const char *pname, const char *qname, 
          cimcStatus* rc);
cimcData cimcClsGetPropertyQualifierAt(cimcConstClass * ccls,const char *pname, unsigned int index,
          cimcString** name, cimcStatus* rc);
unsigned int cimcClsGetPropertyQualifierCount(cimcConstClass * ccls,const char *pname, cimcStatus* rc);

#else 

cimcStatus cimcClsRelease(cimcConstClass * cc);
cimcConstClass *cimcClsClone(cimcConstClass * cc, cimcStatus * rc); 
cimcString *cimcClsGetClassName(cimcConstClass* cc, cimcStatus * rc);
cimcString *cimcClsGetSuperClassName(cimcConstClass* cc, cimcStatus * rc);
 
cimcData cimcClsGetProperty(cimcConstClass * cc, const char *prop, cimcStatus * rc);
cimcData cimcClsGetPropertyAt(cimcConstClass * cc, cimcCount i,
                          cimcString ** name,  cimcStatus * rc);
cimcCount cimcClsGetPropertyCount(cimcConstClass * cc, cimcStatus * rc);
 
cimcData cimcClsGetQualifier(cimcConstClass * cc, const char *qual, cimcStatus * rc);
cimcData cimcClsGetQualifierAt(cimcConstClass * cc, cimcCount i,
                              cimcString ** name, cimcStatus * rc);
cimcCount cimcClsGetQualifierCount(cimcConstClass * cc, cimcStatus * rc);
 
cimcData getPropQualifier(cimcConstClass * cc, const char *prop,
                               const char *qual, cimcStatus * rc);
cimcData cimcClsGetPropQualifierAt(cimcConstClass * cc, const char *prop,
               cimcCount i, cimcString ** name,cimcStatus * rc);
cimcCount cimcClsGetPropQualifierCount(cimcConstClass * cc, const char * prop, cimcStatus * rc);
 
cimcArray *cimcClsGetKeyList(cimcConstClass * cc);
char *cimcClsToString(cimcConstClass * cc);     
void cimcClsRelocate(cimcConstClass * cc);
const char *cimcClsGetCharClassName(cimcConstClass * br);
const char *cimcClsGetCharSuperClassName(cimcConstClass * br);
cimcBoolean cimcClsIsAssociation(cimcConstClass * cc);
cimcBoolean cimcClsIsAbstract(cimcConstClass * cc);
cimcBoolean cimcClsIsIndication(cimcConstClass * cc);
cimcData cimcClsGetPropQualAt(cimcConstClass * cc, cimcCount p, cimcCount i, cimcString ** name,
           cimcStatus * rc);
#endif



   //---------------------------------------------------
   //--
   //	cimcInstance Function Calls
   //--
   //---------------------------------------------------



       /** The Instance object will not be used any further and may be freed by
           cimc run time system.
	 @param inst Instance this pointer.
	 @return Service return status.
      */
cimcStatus cimcInstRelease(cimcInstance* inst);

       /** Create an independent copy of this Instance object. The resulting
           object must be released explicitly.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Instance object.
      */
cimcInstance* cimcInstClone(cimcInstance* inst, cimcStatus* rc);

       /** Gets a named property value.
	 @param inst Instance this pointer.
	 @param name Property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
cimcData cimcInstGetProperty(cimcInstance* inst, const char *name, cimcStatus* rc);

       /** Gets a Property value defined by its index.
	 @param inst Instance this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
cimcData cimcInstGetPropertyAt(cimcInstance* inst, unsigned int index, cimcString** name,
	       cimcStatus* rc);

      /** Gets the number of properties contained in this Instance.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
unsigned int cimcInstGetPropertyCount(cimcInstance* inst, cimcStatus* rc);

      /** Adds/replaces a named Property.
	 @param inst Instance this pointer.
         @param name Entry name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
cimcStatus cimcInstSetProperty(cimcInstance* inst, const char *name,
               void* value, cimcType type);

      /** Generates an ObjectPath out of the nameSpace, classname and
	  key propeties of this Instance.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
         @return the generated ObjectPath.
      */
cimcObjectPath* cimcInstGetObjectPath(cimcInstance* inst, cimcStatus* rc);

      /** Directs cimc to ignore any setProperty operations for this
	  instance for any properties not in this list.
	 @param inst Instance this pointer.
	 @param propertyList If not NULL, the members of the array define one
	     or more Property names to be accepted by setProperty operations.
	 @param keys Array of key property names of this instance. This array
	     must be specified.
	 @return Service return status.
      */
cimcStatus cimcInstSetPropertyFilter(cimcInstance* inst, char **propertyList, char **keys);
              
cimcData cimcInstGetQualifier(cimcInstance* inst, const char *name, cimcStatus* rc);
cimcData cimcInstGetQualifierAt(cimcInstance* inst, unsigned int index, cimcString** name,
	       cimcStatus* rc);
unsigned int cimcInstGetQualifierCount(cimcInstance* inst, cimcStatus* rc);
              
cimcData cimcInstGetPropertyQualifier(cimcInstance* inst, const char *pname, const char *qname, 
          cimcStatus* rc);
cimcData cimcInstGetPropertyQualifierAt(cimcInstance* inst, const char *pname, unsigned int index, 
          cimcString** name, cimcStatus* rc);
unsigned int cimcInstGetPropertyQualifierCount(cimcInstance* inst, const char *pname, cimcStatus* rc);




   //---------------------------------------------------
   //--
   //	cimcObjectPath Function Calls
   //--
   //---------------------------------------------------


       /** The ObjectPath object will not be used any further and may be freed by
           cimc run time system.
	 @param op ObjectPath this pointer.
	 @return Service return status.
      */
cimcStatus cimcOpRelease(cimcObjectPath* op);

       /** Create an independent copy of this ObjectPath object. The resulting
           object must be released explicitly.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied ObjectPath object.
      */
cimcObjectPath* cimcOpClone(cimcObjectPath* op, cimcStatus* rc);

       /** Set/replace the nameSpace component.
	 @param op ObjectPath this pointer.
	 @param ns The nameSpace string
	 @return Service return status.
      */
cimcStatus cimcOpSetNameSpace(cimcObjectPath* op, const char *ns);

       /** Get the nameSpace component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The nameSpace component.
      */
cimcString* cimcOpGetNameSpace(cimcObjectPath* op, cimcStatus* rc);

       /** Set/replace the hostname component.
	 @param op ObjectPath this pointer.
	 @param hn The hostname string
	 @return Service return status.
      */
cimcStatus cimcOpSetHostname(cimcObjectPath* op, const char *hn);

       /** Get the hostname component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The hostname component.
      */
cimcString* cimcOpGetHostname(cimcObjectPath* op, cimcStatus* rc);

       /** Set/replace the classname component.
	 @param op ObjectPath this pointer.
	 @param cn The hostname string
	 @return Service return status.
      */
cimcStatus cimcOpSetClassName(cimcObjectPath* op, const char *cn);

       /** Get the classname component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The classname component.
      */
cimcString* cimcOpGetClassName(cimcObjectPath* op, cimcStatus* rc);

      /** Adds/replaces a named key property.
	 @param op ObjectPath this pointer.
         @param name Key property name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
cimcStatus cimcOpAddKey(cimcObjectPath* op, const char *name, void* value, cimcType type);

       /** Gets a named key property value.
	 @param op ObjectPath this pointer.
	 @param name Key property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Entry value.
      */
cimcData cimcOpGetKey(cimcObjectPath* op, const char *name, cimcStatus* rc);

       /** Gets a key property value defined by its index.
	 @param op ObjectPath this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Data value.
      */
cimcData cimcOpGetKeyAt(cimcObjectPath* op, unsigned int index, cimcString** name,
	       cimcStatus* rc);

      /** Gets the number of key properties contained in this ObjectPath.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
unsigned int cimcOpGetKeyCount(cimcObjectPath* op, cimcStatus* rc);

      /** Set/replace nameSpace and classname components from &lt;src&gt;.
	 @param op ObjectPath this pointer.
	 @param src Source input.
	 @return Service return status.
      */
cimcStatus cimcOpSetNameSpaceFromObjectPath(cimcObjectPath* op, cimcObjectPath* src);

      /** Set/replace hostname, nameSpace and classname components from &lt;src&gt;.
	 @param op ObjectPath this pointer.
	 @param src Source input.
	 @return Service return status.
      */
cimcStatus cimcOpSetHostAndNameSpaceFromObjectPath(cimcObjectPath* op, cimcObjectPath* src);
	
      /** Generates a well formed string representation of this ObjectPath
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return String representation.
      */
cimcString *cimcOpToString(cimcObjectPath* op, cimcStatus *rc);





   //---------------------------------------------------
   //--
   //	cimcArgs Function Calls
   //--
   //---------------------------------------------------


       /** The Args object will not be used any further and may be freed by
           cimc run time system.
	 @param as Args this pointer.
	 @return Service return status.
      */
cimcStatus cimcArgsRelease(cimcArgs* as);

       /** Create an independent copy of this Args object. The resulting
           object must be released explicitly.
	 @param as Args this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Args object.
      */
cimcArgs* cimcArgsClone(cimcArgs* as, cimcStatus* rc);

      /** Adds/replaces a named argument.
	 @param as Args this pointer.
         @param name Argument name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
cimcStatus cimcArgsAddArg(cimcArgs* as, const char *name, void* value, cimcType type);

       /** Gets a named argument value.
	 @param as Args this pointer.
	 @param name Argument name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Argument value.
      */
cimcData cimcArgsGetArg(cimcArgs* as, const char *name, cimcStatus* rc);

       /** Gets a Argument value defined by its index.
	 @param as Args this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned argument name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Argument value.
      */
cimcData cimcArgsGetArgAt(cimcArgs* as, unsigned int index, cimcString** name,
	       cimcStatus* rc);

      /** Gets the number of arguments contained in this Args.
	 @param as Args this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
unsigned int cimcArgsGetArgCount(cimcArgs* as, cimcStatus* rc);




   //---------------------------------------------------
   //--
   //	cimcString Function Calls
   //--
   //---------------------------------------------------


       /** The String object will not be used any further and may be freed by
           cimc run time system.
	 @param st String this pointer.
	 @return Service return status.
       */
cimcStatus cimcStrRelease(cimcString* st);

       /** Create an independent copy of this String object. The resulting
           object must be released explicitly.
	 @param st String this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied String object.
      */
cimcString* cimcStrClone(cimcString* st, cimcStatus* rc);

       /** Get a pointer to a C char *representation of this String.
	 @param st String this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to char *representation.
      */
char *cimcStrGetCharPtr(cimcString* st, cimcStatus* rc);





   //---------------------------------------------------
   //--
   //	cimcArray Function Calls
   //--
   //---------------------------------------------------



       /** The Array object will not be used any further and may be freed by
           cimc run time system.
	 @param ar Array this pointer.
	 @return Service return status.
       */
cimcStatus cimcArrRelease(cimcArray* ar);

       /** Create an independent copy of this Array object. The resulting
           object must be released explicitly.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Array object.
      */
cimcArray* cimcArrClone(cimcArray* ar, cimcStatus* rc);

      /** Gets the number of elements contained in this Array.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of elements.
      */
cimcCount cimcArrGetSize(cimcArray* ar, cimcStatus* rc);

      /** Gets the element type.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of elements.
      */
cimcType cimcArrGetSimpleType(cimcArray* ar, cimcStatus* rc);

       /** Gets an element value defined by its index.
	 @param ar Array this pointer.
	 @param index Position in the internal Data array.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Element value.
      */
cimcData cimcArrGetElementAt(cimcArray* ar, cimcCount index, cimcStatus* rc);

       /** Sets an element value defined by its index.
	 @param ar Array this pointer.
	 @param index Position in the internal Data array.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
cimcStatus cimcArrSetElementAt(cimcArray* ar, cimcCount index, void* value, cimcType type);





   //---------------------------------------------------
   //--
   //	cimcEnumeration Function Calls
   //--
   //---------------------------------------------------



       /** The Enumeration object will not be used any further and may be freed by
           cimc run time system.
	 @param en Enumeration this pointer.
	 @return Service return status.
       */
cimcStatus cimcEnmRelease(cimcEnumeration* en);

       /** Create an independent copy of this Enumeration object. The resulting
           object must be released explicitly.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Enumeration object.
      */
cimcEnumeration* cimcEnmClone(cimcEnumeration* en, cimcStatus* rc);

       /** Get the next element of this Enumeration.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Element value.
      */
cimcData cimcEnmGetNext(cimcEnumeration* en, cimcStatus* rc);

       /** Test for any elements left in this Enumeration.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return True or false.
      */
cimcBoolean cimcEnmHasNext(cimcEnumeration* en, cimcStatus* rc);

       /** Convert this Enumeration into an Array.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The Array.
      */
cimcArray* cimcEnmToArray(cimcEnumeration* en, cimcStatus* rc);





   //---------------------------------------------------
   //--
   //	cimcDateTime Function Calls
   //--
   //---------------------------------------------------



       /** The DateTime object will not be used any further and may be freed by
           cimc run time system.
	 @param dt DateTime this pointer.
	 @return Service return status.
       */
cimcStatus cimcDtRelease(cimcDateTime* dt);

       /** Create an independent copy of this DateTime object. The resulting
           object must be released explicitly.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied DateTime object.
      */
cimcDateTime* cimcDtClone(cimcDateTime* dt, cimcStatus* rc);

       /** Get DateTime setting in binary format (in microsecods
	       starting since 00:00:00 GMT, Jan 1,1970).
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return DateTime in binary.
      */
cimcUint64 cimcDtGetBinaryFormat(cimcDateTime* dt, cimcStatus* rc);

       /** Get DateTime setting in UTC string format.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return DateTime as UTC string.
      */
cimcString* cimcDtGetStringFormat(cimcDateTime* dt, cimcStatus* rc);

       /** Tests whether DateTime is an interval value.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return True if interval value.
      */
cimcBoolean cimcDtIsInterval(cimcDateTime* dt, cimcStatus* rc);





   //---------------------------------------------------
   //--
   //	cimcClient Function Calls
   //--
   //---------------------------------------------------



      /** The Client object will not be used any further and may be freed by
           cimc run time system.
	 @param cl Client this pointer.
	 @return Service return status.
      */
cimcStatus cimcClntRelease(cimcClient* cl);
   
      /** The clone operation is not supported. 
           Inserted here to conform with other cimc function table layouts.
	 @param cl Client this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Always NULL.
      */
cimcClient *cimcClntClone(cimcClient * cl, cimcStatus * st ); 


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
cimcConstClass* cimcClntGetClass(cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, char** properties, cimcStatus* rc);


       /** Enumerate Class Names (and subclass names) in the nameSpace defined by &lt;op&gt;. Inheritance scope can be controled using the
	        flags parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace component.
	 @param flags The following flag is supported: CIMC_FLAG_DeepInheritance.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of ObjectPathes.
     */
     
cimcEnumeration* cimcClntEnumClassNames(cimcClient* cl,
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
     
cimcEnumeration* cimcClntEnumClasses(cimcClient* cl,
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

cimcInstance* cimcClntGetInstance(cimcClient* cl,
                 cimcObjectPath* op, cimcFlags flags, char** properties, cimcStatus* rc);

      /** Create Instance from &lt;inst&gt; using &lt;op&gt; as reference.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param inst Complete instance.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The assigned instance reference.
     */
     
cimcObjectPath* cimcClntCreateInstance(cimcClient* cl,
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

cimcStatus cimcClntSetInstance(cimcClient* cl,
                 cimcObjectPath* op, cimcInstance* inst, cimcFlags flags, char ** properties);

      /** Delete an existing Instance using &lt;op&gt; as reference.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @return Service return status.
     */

cimcStatus cimcClntDeleteInstance(cimcClient* cl,
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

cimcEnumeration* cimcClntExecQuery(cimcClient* cl,
                 cimcObjectPath* op, const char *query, const char *lang, cimcStatus* rc);

       /** Enumerate Instance Names of the class (and subclasses) defined by &lt;op&gt;.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of ObjectPathes.
     */

cimcEnumeration* cimcClntEnumInstanceNames(cimcClient* cl,
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

cimcEnumeration* cimcClntEnumInstances(cimcClient* cl,
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

cimcEnumeration* cimcClntAssociators(cimcClient* cl,
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

cimcEnumeration* cimcClntAssociatorNames(cimcClient* cl,
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

cimcEnumeration* cimcClntReferences(cimcClient* cl,
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

cimcEnumeration* cimcClntReferenceNames(cimcClient* cl,
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

cimcData cimcClntInvokeMethod(cimcClient* cl,
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

cimcStatus cimcClntSetProperty(cimcClient* cl,
                 cimcObjectPath* op, const char *name, void* value,
                 cimcType type);

       /** Get the named property value of an Instance defined by the &lt;op&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param name Property name
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */

cimcData cimcClntGetProperty(cimcClient *cl, 
                 cimcObjectPath *op, const char *name, cimcStatus *rc);



   //---------------------------------------------------
   //--
   //	 cimcEnv Function Calls
   //--
   //---------------------------------------------------



cimcClient* cimcEnvConnect(cimcEnv *ce, const char *hn, const char *scheme, const char *port,
                const char *user, const char *pwd,cimcStatus *rc);
                
cimcClient* cimcEnvConnect2(cimcEnv *ce, const char *hn, const char *scheme, const char *port,
                const char *user, const char *pwd, 
                int verifyMode, const char * trustStore,
                const char * certFile, const char * keyFile,
                cimcStatus *rc);

cimcInstance* cimcEnvNewInstance(cimcEnv *ce, const cimcObjectPath* op, cimcStatus* rc);
cimcObjectPath* cimcEnvNewObjectPath(cimcEnv *ce, const char *ns, const char *cn, cimcStatus* rc);
cimcArgs* cimcEnvNewArgs(cimcEnv *ce, cimcStatus* rc);
cimcString* cimcEnvNewString(cimcEnv *ce, const char *data, cimcStatus* rc);
cimcArray* cimcEnvNewArray(cimcEnv *ce, cimcCount max, cimcType type, cimcStatus* rc);
cimcDateTime* cimcEnvNewDateTime(cimcEnv *ce, cimcStatus* rc);
cimcDateTime* cimcEnvNewDateTimeFromBinary(cimcEnv *ce, cimcUint64 binTime, cimcBoolean interval, 
           cimcStatus* rc);
cimcDateTime* cimcEnvNewDateTimeFromChars(cimcEnv *ce, const char *utcTime, cimcStatus* rc);


   //---------------------------------------------------
   //--
   //	 cimcNewEnv - Mother of all functions
   //--
   //---------------------------------------------------


cimcEnv* cimcNewEnv(const char *id, unsigned int options, int *rc, char **msg);


#ifdef __cplusplus
 };
#endif

#endif // _CMCIFC_H_
