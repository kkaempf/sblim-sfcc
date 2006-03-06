
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
 * CMCI function tables.
 *
*/

#ifndef _CIMCFT_H_
#define _CIMCFT_H_

#include "cimcdt.h"

#ifdef __cplusplus
extern "C" {
#endif


 
   //---------------------------------------------------
   //--
   //	_cimcConstClass Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to ConstClass
       support sevices.
   */

#ifdef mmmmmmmmmm

typedef struct _cimcConstClassFT {

       /** Function table version
       */
     int ftVersion;

       /** The ConstClass object will not be used any further and may be freed by
           cimc run time system.
	 @param ccls ConstClass this pointer.
	 @return Service return status.
      */
     cimcStatus (*release)
              (cimcConstClass* ccls);

       /** Create an independent copy of this CpnstClass object. The resulting
           object must be released explicitly.
	 @param ccls ConstClass this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied ConstClass object.
      */
     cimcConstClass* (*clone)
              (cimcConstClass* ccls, cimcStatus* rc);

       /** Gets the classname.
	 @param ccls ConstClass this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return classname.
      */     
     cimcString *(*getClassName) 
              ( cimcConstClass * ccls, cimcStatus * rc );
                
       /** Gets a named property value.
	 @param ccls ConstClass this pointer.
	 @param name Property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
     cimcData (*getProperty)
              (cimcConstClass* ccls, const char *name, cimcStatus* rc);

       /** Gets a Property value defined by its index.
	 @param ccls ConstClass this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
     cimcData (*getPropertyAt)
              (cimcConstClass* ccls, unsigned int index, cimcString** name,
	       cimcStatus* rc);

      /** Gets the number of properties contained in this Instance.
	 @param ccls ConstClass this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
     unsigned int (*getPropertyCount)
              (cimcConstClass* ccls, cimcStatus* rc);

              
     cimcData (*getQualifier)
              (cimcConstClass * ccls,const char *name, cimcStatus* rc);
     cimcData (*getQualifierAt)
              (cimcConstClass * ccls,unsigned int index, cimcString** name,
	       cimcStatus* rc);
     unsigned int (*getQualifierCount)
              (cimcConstClass * ccls,cimcStatus* rc);
              
              
     cimcData (*getPropertyQualifier)
              (cimcConstClass * ccls,const char *pname, const char *qname, cimcStatus* rc);
     cimcData (*getPropertyQualifierAt)
              (cimcConstClass * ccls,const char *pname, unsigned int index, cimcString** name,
	       cimcStatus* rc);
     unsigned int (*getPropertyQualifierCount)
              (cimcConstClass * ccls,const char *pname, cimcStatus* rc);
              
} cimcConstClassFT;

#endif

typedef struct _cimcConstClassFT { 
   int version;
   cimcStatus(*release) (cimcConstClass * cc);
   cimcConstClass *(*clone) (cimcConstClass * cc, cimcStatus * rc); 
   cimcString *(*getClassName)(cimcConstClass* cc, cimcStatus * rc);
   cimcString *(*getSuperClassName)(cimcConstClass* cc, cimcStatus * rc);
   
   cimcData (*getProperty) (cimcConstClass * cc, const char *prop, cimcStatus * rc);
   cimcData (*getPropertyAt) (cimcConstClass * cc, cimcCount i,
                             cimcString ** name,  cimcStatus * rc);
   cimcCount (*getPropertyCount) (cimcConstClass * cc, cimcStatus * rc);
   
   cimcData (*getQualifier) (cimcConstClass * cc, const char *qual, cimcStatus * rc);
   cimcData (*getQualifierAt) (cimcConstClass * cc, cimcCount i,
                              cimcString ** name, cimcStatus * rc);
   cimcCount (*getQualifierCount) (cimcConstClass * cc, cimcStatus * rc);
   
   cimcData (*getPropQualifier) (cimcConstClass * cc, const char *prop,
                                   const char *qual, cimcStatus * rc);
   cimcData (*getPropQualifierAt) (cimcConstClass * cc, const char *prop,
                                   cimcCount i, cimcString ** name,
                                   cimcStatus * rc);
   cimcCount (*getPropQualifierCount) (cimcConstClass * cc, const char * prop, cimcStatus * rc);
   
   cimcArray *(*getKeyList) (cimcConstClass * cc);
   char *(*toString) (cimcConstClass * cc);     
   void (*relocate) (cimcConstClass * cc);
   const char *(*getCharClassName) (cimcConstClass * br);
   const char *(*getCharSuperClassName) (cimcConstClass * br);
   cimcBoolean(*isAssociation) (cimcConstClass * cc);
   cimcBoolean(*isAbstract) (cimcConstClass * cc);
   cimcBoolean(*isIndication) (cimcConstClass * cc);
   cimcData(*getPropQualAt) (cimcConstClass * cc, cimcCount p, cimcCount i, cimcString ** name,
                                   cimcStatus * rc);
} cimcConstClassFT;

struct _cimcConstClass {
   void *hdl;
   cimcConstClassFT *ft;
};


  //---------------------------------------------------
   //--
   //	_cimcInstance Encapsulated object
   //--
   //---------------------------------------------------


   /** This structure represents the Encapsulated Instance object.
   */
   struct _cimcInstance {

       /** Opaque pointer to MB specific implementation data.
       */
     void *hdl;

       /** Pointer to the Instance Function Table.
       */
     cimcInstanceFT* ft;
   };



   //---------------------------------------------------
   //--
   //	_cimcInstance Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to Instance
       support sevices.
   */
   struct _cimcInstanceFT {

       /** Function table version
       */
     int ftVersion;

       /** The Instance object will not be used any further and may be freed by
           cimc run time system.
	 @param inst Instance this pointer.
	 @return Service return status.
      */
     cimcStatus (*release)
              (cimcInstance* inst);

       /** Create an independent copy of this Instance object. The resulting
           object must be released explicitly.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Instance object.
      */
     cimcInstance* (*clone)
              (cimcInstance* inst, cimcStatus* rc);

       /** Gets a named property value.
	 @param inst Instance this pointer.
	 @param name Property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
     cimcData (*getProperty)
              (cimcInstance* inst, const char *name, cimcStatus* rc);

       /** Gets a Property value defined by its index.
	 @param inst Instance this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
     cimcData (*getPropertyAt)
              (cimcInstance* inst, unsigned int index, cimcString** name,
	       cimcStatus* rc);

      /** Gets the number of properties contained in this Instance.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
     unsigned int (*getPropertyCount)
              (cimcInstance* inst, cimcStatus* rc);

      /** Adds/replaces a named Property.
	 @param inst Instance this pointer.
         @param name Entry name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
     cimcStatus (*setProperty)
              (cimcInstance* inst, const char *name,
               cimcValue* value, cimcType type);

      /** Generates an ObjectPath out of the nameSpace, classname and
	  key propeties of this Instance.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
         @return the generated ObjectPath.
      */
     cimcObjectPath* (*getObjectPath)
              (cimcInstance* inst, cimcStatus* rc);

      /** Directs cimc to ignore any setProperty operations for this
	  instance for any properties not in this list.
	 @param inst Instance this pointer.
	 @param propertyList If not NULL, the members of the array define one
	     or more Property names to be accepted by setProperty operations.
	 @param keys Array of key property names of this instance. This array
	     must be specified.
	 @return Service return status.
      */
     cimcStatus (*setPropertyFilter)
              (cimcInstance* inst, char **propertyList, char **keys);
              
              
     cimcData (*getQualifier)
              (cimcInstance* inst, const char *name, cimcStatus* rc);
     cimcData (*getQualifierAt)
              (cimcInstance* inst, unsigned int index, cimcString** name,
	       cimcStatus* rc);
     unsigned int (*getQualifierCount)
              (cimcInstance* inst, cimcStatus* rc);
              
              
     cimcData (*getPropertyQualifier)
              (cimcInstance* inst, const char *pname, const char *qname, cimcStatus* rc);
     cimcData (*getPropertyQualifierAt)
              (cimcInstance* inst, const char *pname, unsigned int index, cimcString** name,
	       cimcStatus* rc);
     unsigned int (*getPropertyQualifierCount)
              (cimcInstance* inst, const char *pname, cimcStatus* rc);
              
   };




   //---------------------------------------------------
   //--
   //	_cimcObjectPath Encapsulated object
   //--
   //---------------------------------------------------


   /** This structure represents the Encapsulated Instance object.
   */
   struct _cimcObjectPath {

       /** Opaque pointer to MB specific implementation data.
       */
     void *hdl;

       /** Pointer to the ObjectPath Function Table.
       */
     cimcObjectPathFT* ft;
   };



   //---------------------------------------------------
   //--
   //	_cimcObjectPath Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to ObjectPath
       support sevices.
   */
   struct _cimcObjectPathFT {

       /** Function table version
       */
     int ftVersion;

       /** The ObjectPath object will not be used any further and may be freed by
           cimc run time system.
	 @param op ObjectPath this pointer.
	 @return Service return status.
      */
     cimcStatus (*release)
              (cimcObjectPath* op);

       /** Create an independent copy of this ObjectPath object. The resulting
           object must be released explicitly.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied ObjectPath object.
      */
     cimcObjectPath* (*clone)
              (cimcObjectPath* op, cimcStatus* rc);

       /** Set/replace the nameSpace component.
	 @param op ObjectPath this pointer.
	 @param ns The nameSpace string
	 @return Service return status.
      */
     cimcStatus (*setNameSpace)
              (cimcObjectPath* op, const char *ns);

       /** Get the nameSpace component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The nameSpace component.
      */
     cimcString* (*getNameSpace)
              (cimcObjectPath* op, cimcStatus* rc);

       /** Set/replace the hostname component.
	 @param op ObjectPath this pointer.
	 @param hn The hostname string
	 @return Service return status.
      */
     cimcStatus (*setHostname)
              (cimcObjectPath* op, const char *hn);

       /** Get the hostname component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The hostname component.
      */
     cimcString* (*getHostname)
              (cimcObjectPath* op, cimcStatus* rc);

       /** Set/replace the classname component.
	 @param op ObjectPath this pointer.
	 @param cn The hostname string
	 @return Service return status.
      */
     cimcStatus (*setClassName)
              (cimcObjectPath* op, const char *cn);

       /** Get the classname component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The classname component.
      */
     cimcString* (*getClassName)
              (cimcObjectPath* op, cimcStatus* rc);

      /** Adds/replaces a named key property.
	 @param op ObjectPath this pointer.
         @param name Key property name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
     cimcStatus (*addKey)
              (cimcObjectPath* op, const char *name,
               cimcValue* value, cimcType type);

       /** Gets a named key property value.
	 @param op ObjectPath this pointer.
	 @param name Key property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Entry value.
      */
     cimcData (*getKey)
              (cimcObjectPath* op, const char *name, cimcStatus* rc);

       /** Gets a key property value defined by its index.
	 @param op ObjectPath this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Data value.
      */
     cimcData (*getKeyAt)
              (cimcObjectPath* op, unsigned int index, cimcString** name,
	       cimcStatus* rc);

      /** Gets the number of key properties contained in this ObjectPath.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
     unsigned int (*getKeyCount)
              (cimcObjectPath* op, cimcStatus* rc);

      /** Set/replace nameSpace and classname components from &lt;src&gt;.
	 @param op ObjectPath this pointer.
	 @param src Source input.
	 @return Service return status.
      */
     cimcStatus (*setNameSpaceFromObjectPath)
              (cimcObjectPath* op, cimcObjectPath* src);

      /** Set/replace hostname, nameSpace and classname components from &lt;src&gt;.
	 @param op ObjectPath this pointer.
	 @param src Source input.
	 @return Service return status.
      */
     cimcStatus (*setHostAndNameSpaceFromObjectPath)
              (cimcObjectPath* op,
               cimcObjectPath* src);
	


		// optional qualifier support


       /** Get class qualifier value.
	 @param op ObjectPath this pointer.
	 @param qName Qualifier name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Qualifier value.
      */
     cimcData (*getClassQualifier)
              (cimcObjectPath* op,
               const char *qName,
               cimcStatus *rc);

       /** Get property qualifier value.
	 @param op ObjectPath this pointer.
	 @param pName Property name.
	 @param qName Qualifier name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Qualifier value.
      */
     cimcData (*getPropertyQualifier)
              (cimcObjectPath* op,
               const char *pName,
               const char *qName,
               cimcStatus *rc);

       /** Get method qualifier value.
	 @param op ObjectPath this pointer.
	 @param mName Method name.
	 @param qName Qualifier name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Qualifier value.
      */
     cimcData (*getMethodQualifier)
              (cimcObjectPath* op,
               const char *methodName,
               const char *qName,
               cimcStatus *rc);

       /** Get method parameter quailifier value.
	 @param op ObjectPath this pointer.
	 @param mName Method name.
	 @param pName Parameter name.
	 @param qName Qualifier name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Qualifier value.
      */
     cimcData (*getParameterQualifier)
              (cimcObjectPath* op,
               const char *mName,
               const char *pName,
               const char *qName,
               cimcStatus *rc);
	
      /** Generates a well formed string representation of this ObjectPath
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return String representation.
      */
     cimcString *(*toString)
              (cimcObjectPath* op, cimcStatus *rc);

   };







   //---------------------------------------------------
   //--
   //	_cimcArgs Encapsulated object
   //--
   //---------------------------------------------------


   /** This structure represents the Encapsulated Args object.
   */
   struct _cimcArgs {

       /** Opaque pointer to MB specific implementation data.
       */
     void *hdl;

       /** Pointer to the Args Function Table.
       */
     cimcArgsFT* ft;
   };



   //---------------------------------------------------
   //--
   //	_cimcArgsFT Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to Args
       support sevices.
   */
   struct _cimcArgsFT{

       /** Function table version
       */
     int ftVersion;

       /** The Args object will not be used any further and may be freed by
           cimc run time system.
	 @param as Args this pointer.
	 @return Service return status.
      */
     cimcStatus (*release)
              (cimcArgs* as);

       /** Create an independent copy of this Args object. The resulting
           object must be released explicitly.
	 @param as Args this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Args object.
      */
     cimcArgs* (*clone)
              (cimcArgs* as, cimcStatus* rc);

      /** Adds/replaces a named argument.
	 @param as Args this pointer.
         @param name Argument name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
     cimcStatus (*addArg)
              (cimcArgs* as, const char *name ,cimcValue* value,
               cimcType type);

       /** Gets a named argument value.
	 @param as Args this pointer.
	 @param name Argument name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Argument value.
      */
     cimcData (*getArg)
              (cimcArgs* as, const char *name, cimcStatus* rc);

       /** Gets a Argument value defined by its index.
	 @param as Args this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned argument name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Argument value.
      */
     cimcData (*getArgAt)
              (cimcArgs* as, unsigned int index, cimcString** name,
	       cimcStatus* rc);

      /** Gets the number of arguments contained in this Args.
	 @param as Args this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
     unsigned int (*getArgCount)
              (cimcArgs* as, cimcStatus* rc);
   };




   //---------------------------------------------------
   //--
   //	_cimcString Encapsulated object
   //--
   //---------------------------------------------------


   /** This structure represents the Encapsulated String object.
   */
   struct _cimcString {

       /** Opaque pointer to MB specific implementation data.
       */
     void *hdl;

       /** Pointer to the String Function Table.
       */
     cimcStringFT* ft;
   };



   //---------------------------------------------------
   //--
   //	_cimcStringFT Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to String
       support sevices.
   */
   struct _cimcStringFT {

       /** Function table version
       */
     int ftVersion;

       /** The String object will not be used any further and may be freed by
           cimc run time system.
	 @param st String this pointer.
	 @return Service return status.
       */
     cimcStatus (*release)
             (cimcString* st);

       /** Create an independent copy of this String object. The resulting
           object must be released explicitly.
	 @param st String this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied String object.
      */
     cimcString* (*clone)
             (cimcString* st, cimcStatus* rc);

       /** Get a pointer to a C char *representation of this String.
	 @param st String this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to char *representation.
      */
     char *(*getCharPtr)
             (cimcString* st, cimcStatus* rc);
   };




   //---------------------------------------------------
   //--
   //	_cimcArray Encapsulated object
   //--
   //---------------------------------------------------


   /** This structure represents the Encapsulated Array object.
   */
   struct _cimcArray {

       /** Opaque pointer to MB specific implementation data.
       */
     void *hdl;

       /** Pointer to the Array Function Table.
       */
     cimcArrayFT* ft;
   };



   //---------------------------------------------------
   //--
   //	_cimcArrayFT Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to Array
       support sevices.
   */
   struct _cimcArrayFT {

       /** Function table version
       */
     int ftVersion;

       /** The Array object will not be used any further and may be freed by
           cimc run time system.
	 @param ar Array this pointer.
	 @return Service return status.
       */
     cimcStatus (*release)
             (cimcArray* ar);

       /** Create an independent copy of this Array object. The resulting
           object must be released explicitly.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Array object.
      */
     cimcArray* (*clone)
             (cimcArray* ar, cimcStatus* rc);

      /** Gets the number of elements contained in this Array.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of elements.
      */
     cimcCount (*getSize)
             (cimcArray* ar, cimcStatus* rc);

      /** Gets the element type.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of elements.
      */
     cimcType (*getSimpleType)
             (cimcArray* ar, cimcStatus* rc);

       /** Gets an element value defined by its index.
	 @param ar Array this pointer.
	 @param index Position in the internal Data array.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Element value.
      */
     cimcData (*getElementAt)
             (cimcArray* ar, cimcCount index, cimcStatus* rc);

       /** Sets an element value defined by its index.
	 @param ar Array this pointer.
	 @param index Position in the internal Data array.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
     cimcStatus (*setElementAt)
             (cimcArray* ar, cimcCount index, cimcValue* value, cimcType type);
   };





   //---------------------------------------------------
   //--
   //	_cimcEnumeration Encapsulated object
   //--
   //---------------------------------------------------


   /** This structure represents the Encapsulated Enumeration object.
   */
   struct _cimcEnumeration {

       /** Opaque pointer to MB specific implementation data.
       */
     void *hdl;

       /** Pointer to the Enumeration Function Table.
       */
     cimcEnumerationFT* ft;
   };



   //---------------------------------------------------
   //--
   //	_cimcEnumerationFT Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to Enumeration
       support sevices.
   */
   struct _cimcEnumerationFT {

       /** Function table version
       */
     int ftVersion;

       /** The Enumeration object will not be used any further and may be freed by
           cimc run time system.
	 @param en Enumeration this pointer.
	 @return Service return status.
       */
     cimcStatus (*release)
             (cimcEnumeration* en);

       /** Create an independent copy of this Enumeration object. The resulting
           object must be released explicitly.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Enumeration object.
      */
     cimcEnumeration* (*clone)
             (cimcEnumeration* en, cimcStatus* rc);

       /** Get the next element of this Enumeration.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Element value.
      */
     cimcData (*getNext)
             (cimcEnumeration* en, cimcStatus* rc);

       /** Test for any elements left in this Enumeration.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return True or false.
      */
     cimcBoolean (*hasNext)
             (cimcEnumeration* en, cimcStatus* rc);

       /** Convert this Enumeration into an Array.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The Array.
      */
     cimcArray* (*toArray)
             (cimcEnumeration* en, cimcStatus* rc);
  };





   //---------------------------------------------------
   //--
   //	_cimcDateTime Encapsulated object
   //--
   //---------------------------------------------------


   /** This structure represents the DateTime object.
   */
  struct _cimcDateTime {

       /** Opaque pointer to MB specific implementation data.
       */
     void *hdl;

       /** Pointer to the DateTime Function Table.
       */
     cimcDateTimeFT *ft;
   };



   //---------------------------------------------------
   //--
   //	_cimcDateTimeFT Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to DateTime
       support sevices.
   */
   struct _cimcDateTimeFT {

       /** Function table version
       */
     int ftVersion;

       /** The DateTime object will not be used any further and may be freed by
           cimc run time system.
	 @param dt DateTime this pointer.
	 @return Service return status.
       */
     cimcStatus (*release)
             (cimcDateTime* dt);

       /** Create an independent copy of this DateTime object. The resulting
           object must be released explicitly.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied DateTime object.
      */
     cimcDateTime* (*clone)
             (cimcDateTime* dt, cimcStatus* rc);

       /** Get DateTime setting in binary format (in microsecods
	       starting since 00:00:00 GMT, Jan 1,1970).
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return DateTime in binary.
      */
     cimcUint64 (*getBinaryFormat)
             (cimcDateTime* dt, cimcStatus* rc);

       /** Get DateTime setting in UTC string format.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return DateTime as UTC string.
      */
     cimcString* (*getStringFormat)
             (cimcDateTime* dt, cimcStatus* rc);

       /** Tests whether DateTime is an interval value.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return True if interval value.
      */
     cimcBoolean (*isInterval)
              (cimcDateTime* dt, cimcStatus* rc);
  };


#ifdef __cplusplus
 };
#endif

#endif // _CMCIFT_H_
