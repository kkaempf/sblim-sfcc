
/*
 * cmpimacs.h
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
 * cimc convenious macros.
 *
*/


#ifndef _CMCIMACS_H_
#define _CMCIMACS_H_

#ifndef DOC_ONLY
#include "cimcdt.h"
#include "cimcft.h"
#endif

#ifdef DOC_ONLY
#define CIMC_INLINE
#endif

#ifdef DOC_ONLY
  /** This macro builds a cimcStatus object with &lt;rc&gt; as return code and returns
      to the Broker.
      @param rc the cimc return code
      @return This macro contains a return statement and leaves the function.
   */
 noReturn CMReturn(cimcrc rc);
#else
#define CMReturn(rc) \
      { cimcStatus stat={(rc),NULL}; \
         return stat; }
#endif

#ifdef CIMC_INLINE
  /** Initializes status object with rc and NULL message.
      @param st Address of status object
      @param rcp cimc return code
  */
inline static  void CMSetStatus(cimcStatus* st, cimcrc rcp)
      { if (rc && st) { (st)->rc=(rcp); (st)->msg=NULL; }}
#else
#define CMSetStatus(st,rcp) \
      { if (st){(st)->rc=(rcp); (st)->msg=NULL; }}
#endif

#ifdef CIMC_INLINE
  /** Initializes status object with rc and message.
      @param mb Broker this pointer
      @param st Address of status object
      @param rcp cimc return code
      @param chars Message character string
  */
inline static   void CMSetStatusWithChars(cimcStatus* st, cimcrc rcp,
                                 char* chars)
      { if (st) {
          (st)->rc=(rcp);
          (st)->msg=newcimcString((chars),NULL); 
      } }
#else
#define CMSetStatusWithChars(st,rcp,chars) \
      { if (st) { (st)->rc=(rcp); \
        (st)->msg=newcimcString((chars),NULL); }}
#endif

#ifdef DOC_ONLY
  /** This macro builds a cimcStatus object with &lt;rc&gt; as return code and &lt;str&gt; as
      message and returns to the Broker.
//       @param rc the cimc return code
      @param str the message as String object
      @return This macro contains a return statement and leaves the function.
   */
 noReturn CMReturnWithString(cimcrc rc, cimcString *str);
#else
#define CMReturnWithString(rc,str) \
      { cimcStatus stat={(rc),(str)}; \
         return stat; }
#endif

#ifdef DOC_ONLY
  /** This macro builds a cimcStatus object with &lt;rc&gt; as return code and &lt;msg&gt; as
      message and returns to the Broker.
      @param mb Broker this pointer
      @param rc the cimc return code
      @param msg the message as character string
      @return This macro contains a return statement and leaves the function.
   */
 noReturn CMReturnWithChars(cimcrc rc, char *msg);
#else
#define CMReturnWithChars(rc,chars) \
      { cimcStatus stat={(rc),NULL}; \
         stat.msg=newcimcString((chars),NULL); \
         return stat; }
#endif



#ifndef DOC_ONLY
  #ifdef CIMC_PLATFORM_WIN32_IX86_MSVC
	#define  CIMC_EXTERN_C __declspec(dllexport)
  #else
	#ifdef __cplusplus
      #define  CIMC_EXTERN_C extern "C"
    #else
      #define CIMC_EXTERN_C
    #endif
  #endif
#endif

#ifdef CIMC_INLINE
  /** Tests for encapsulated NULL object.
      @param obj cimc Object pointer
  */
  inline static   cimcBoolean CMIsNullObject(void* obj)
       { return ((obj)==NULL || *((void**)(obj))==NULL); }
#else
  #define CMIsNullObject(o)           ((o)==NULL || *((void**)(o))==NULL)
#endif

#ifdef CIMC_INLINE
  /** Tests for nullValue data item.
      @param val Value object
  */
  inline static   cimcBoolean CMIsNullValue(cimcData val)
       { return ((val.state) & CIMC_nullValue); }
#else
  #define CMIsNullValue(v)                   ((v.state) & CIMC_nullValue)
#endif

#ifdef CIMC_INLINE
  /** Tests for keyValue data item.
      @param val Value object
  */
  inline static   cimcBoolean CMIsKeyValue(cimcData val)
       { return ((val.state) & CIMC_keyValue); }
#else
  #define CMIsKeyValue(v)                     ((v.state) & CIMC_keyValue)
#endif

#ifdef CIMC_INLINE
  /** Tests for keyValue data item.
      @param val Value object
  */
  inline static   cimcBoolean CMIsArray(cimcData val)
       { return ((val.type) & CIMC_ARRAY); }
#else
  #define CMIsArray(v)                            ((v.type) & CIMC_ARRAY)
#endif


    // Life-cycle macros

  #define CMClone(o,rc)                        ((o)->ft->clone((o),(rc)))
  #define CMRelease(o)                            ((o)->ft->release((o)))
  #define CMGetCharPtr(s)                                 ((char*)s->hdl)
  #define CMReleaseData(d) \
       if (((d)->type >= CIMC_instance  && (d)->type <= CIMC_dateTime) || \
           ((d)->type >= CIMC_instanceA && (d)->type <= CIMC_dateTimeA)) \
                                                    CMRelease((d)->value);

    // cimcInstance macros


#ifdef CIMC_INLINE
       /** Gets a named property value.
	 @param inst Instance this pointer.
	 @param name Property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
   inline static   cimcData CMGetProperty
              (cimcInstance* inst, char* name, cimcStatus* rc)
	{ return ((inst)->ft->getProperty((inst),(name),(rc))); }
#else
  #define CMGetProperty(i,n,rc)      ((i)->ft->getProperty((i),(n),(rc)))
#endif

#ifdef CIMC_INLINE
       /** Gets a Property value defined by its index.
	 @param inst Instance this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
   inline static   cimcData CMGetPropertyAt
              (cimcInstance* inst, unsigned int index, cimcString** name,
	       cimcStatus* rc)
	{ return ((inst)->ft->getPropertyAt((inst),(index),(name),(rc))); }
#else
 #define CMGetPropertyAt(i,num,s,rc) \
                  ((i)->ft->getPropertyAt((i),(num),(s),(rc)))
#endif

#ifdef CIMC_INLINE
      /** Adds/replaces a named Property.
	 @param inst Instance this pointer.
         @param name Entry name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
  inline static   cimcStatus CMSetProperty
              (cimcInstance* inst, char* name,
               void* value, cimcType type)
	{ return ((inst)->ft->setProperty((inst),(name),(cimcValue*)(value),(type))); }
#else
  #define CMSetProperty(i,n,v,t) \
                      ((i)->ft->setProperty((i),(n),(cimcValue*)(v),(t)))
#endif

#ifdef CIMC_INLINE
      /** Gets the number of properties contained in this Instance.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
     inline static   unsigned int CMGetPropertyCount
              (cimcInstance* inst, cimcStatus* rc)
	 { return ((inst)->ft->getPropertyCount((inst),(rc))); }
#else
  #define CMGetPropertyCount(i,rc)   ((i)->ft->getPropertyCount((i),(rc)))
#endif

#ifdef CIMC_INLINE
      /** Generates an ObjectPath out of the nameSpace, classname and
	  key propeties of this Instance.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
         @return the generated ObjectPath.
      */
     inline static   cimcObjectPath* CMGetObjectPath
              (cimcInstance* inst, cimcStatus* rc)
	{ return ((inst)->ft->getObjectPath((inst),(rc))); }
#else
  #define CMGetObjectPath(i,rc)        ((i)->ft->getObjectPath((i),(rc)))
#endif

#ifdef CIMC_INLINE
      /** Directs cimc to ignore any setProperty operations for this
	  instance for any properties not in this list.
	 @param inst Instance this pointer.
	 @param propertyList If not NULL, the members of the array define one
	     or more Property names to be accepted by setProperty operations.
	 @param keys array of key properties. This array must be specified.
	 @return Service return status.
      */
     inline static   cimcStatus CMSetPropertyFilter
              (cimcInstance* inst, char** propertyList, char **keys)
	{ return ((inst)->ft->setPropertyFilter((inst),(propertyList),(keys))); }
#else
  #define CMSetPropertyFilter(i,pl,k) ((i)->ft->setPropertyFilter((i),(pl),(k)))
#endif



   // cimcObjectPath macros


#ifdef CIMC_INLINE
       /** Set/replace the hostname component.
	 @param op ObjectPath this pointer.
	 @param hn The hostname string
	 @return Service return status.
      */
     inline static   cimcStatus CMSetHostname
              (cimcObjectPath* op, char* hn)
	{ return ((op)->ft->setHostname((op),(hn))); }
#else
  #define CMSetHostname(p,n)              ((p)->ft->setHostname((p),(n)))
#endif

#ifdef CIMC_INLINE
       /** Get the hostname component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The hostname component.
      */
   inline static   cimcString* CMGetHostname
              (cimcObjectPath* op,cimcStatus* rc)
	{ return ((op)->ft->getHostname((op),(rc))); }
#else
  #define CMGetHostname(p,rc)            ((p)->ft->getHostname((p),(rc)))
#endif

#ifdef CIMC_INLINE
       /** Set/replace the nameSpace component.
	 @param op ObjectPath this pointer.
	 @param ns The nameSpace string
	 @return Service return status.
      */
   inline static   cimcStatus CMSetNameSpace
              (cimcObjectPath* op,char* ns)
	 { return ((op)->ft->setNameSpace((op),(ns))); }
#else
  #define CMSetNameSpace(p,n)            ((p)->ft->setNameSpace((p),(n)))
#endif

#ifdef CIMC_INLINE
       /** Get the nameSpace component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The nameSpace component.
      */
   inline static   cimcString* CMGetNameSpace
              (cimcObjectPath* op, cimcStatus* rc)
	{ return ((op)->ft->getNameSpace((op),(rc))); }
#else
  #define CMGetNameSpace(p,rc)          ((p)->ft->getNameSpace((p),(rc)))
#endif

#ifdef CIMC_INLINE
       /** Set/replace the classname component.
	 @param op ObjectPath this pointer.
	 @param cn The hostname string
	 @return Service return status.
      */
   inline static   cimcStatus CMSetClassName
                 (cimcObjectPath* op, char* cn)
	{ return ((op)->ft->setClassName((op),(cn))); }
#else
  #define CMSetClassName(p,n)            ((p)->ft->setClassName((p),(n)))
#endif

#ifdef CIMC_INLINE
       /** Get the classname component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The classname component.
      */
   inline static   cimcString* CMGetClassName
              (cimcObjectPath* op,cimcStatus* rc)
	{ return ((op)->ft->getClassName((op),(rc))); }
#else
  #define CMGetClassName(p,rc)          ((p)->ft->getClassName((p),(rc)))
#endif

#ifdef CIMC_INLINE
      /** Adds/replaces a named key property.
	 @param op ObjectPath this pointer.
         @param name Key property name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
   inline static   cimcStatus CMAddKey
              (cimcObjectPath* op, char* name,
               void* value, cimcType type)
	{ return ((op)->ft->addKey((op),(name),(cimcValue*)(value),(type))); }
#else
  #define CMAddKey(p,n,v,t) \
                           ((p)->ft->addKey((p),(n),(cimcValue*)(v),(t)))
#endif

#ifdef CIMC_INLINE
       /** Gets a named key property value.
	 @param op ObjectPath this pointer.
	 @param name Key property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Entry value.
      */
   inline static   cimcData CMGetKey
              (cimcObjectPath* op, char* name, cimcStatus* rc)
	{ return ((op)->ft->getKey((op),(name),(rc))); }
#else
  #define CMGetKey(p,n,rc)                ((p)->ft->getKey((p),(n),(rc)))
#endif

#ifdef CIMC_INLINE
       /** Gets a key property value defined by its index.
	 @param op ObjectPath this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Data value.
      */
   inline static   cimcData CMGetKeyAt
              (cimcObjectPath* op,unsigned int index, cimcString** name,
	       cimcStatus* rc)
	{ return ((op)->ft->getKeyAt((op),(index),(name),(rc))); }
#else
  #define CMGetKeyAt(p,i,n,rc)          ((p)->ft->getKeyAt((p),(i),(n),(rc)))
#endif


#ifdef CIMC_INLINE
      /** Gets the number of key properties contained in this ObjectPath.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
   inline static   unsigned int CMGetKeyCount
              (cimcObjectPath* op, cimcStatus* rc)
	{ return ((op)->ft->getKeyCount((op),(rc))); }
#else
  #define CMGetKeyCount(p,rc)            ((p)->ft->getKeyCount((p),(rc)))
#endif

#ifdef CIMC_INLINE
      /** Set/replace nameSpace and classname components from &lt;src&gt;.
	 @param op ObjectPath this pointer.
	 @param src Source input.
	 @return Service return status.
      */
   inline static   cimcStatus CMSetNameSpaceFromObjectPath
              (cimcObjectPath* op, cimcObjectPath* src)
	{ return ((op)->ft->setNameSpaceFromObjectPath((op),(src))); }
#else
  #define CMSetNameSpaceFromObjectPath(p,s) \
                           ((p)->ft->setNameSpaceFromObjectPath((p),(s)))
#endif

#ifdef CIMC_INLINE
      /** Set/replace hostname, nameSpace and classname components from &lt;src&gt;.
	 @param op ObjectPath this pointer.
	 @param src Source input.
	 @return Service return status.
      */
   inline static   cimcStatus CMSetHostAndNameSpaceFromObjectPath
              (cimcObjectPath* op,
               cimcObjectPath* src)
	{ return ((op)->ft->setHostAndNameSpaceFromObjectPath((op),(src))); }
#else
  #define CMSetHostAndNameSpaceFromObjectPath(p,s) \
                     ((p)->ft->setHostAndNameSpaceFromObjectPath((p),(s)))
#endif

#if defined(CIMC_VER_86)
#ifdef CIMC_INLINE
      /** Set/replace hostname, nameSpace and classname components from &lt;src&gt;.
	 @param op ObjectPath this pointer.
	 @param src Source input.
	 @return Service return status.
      */
   inline static   cimcString* CMObjectPathToString
              (cimcObjectPath* op, cimcStatus* rc)
	{ return ((op)->ft->toString(op),(rc))); }
#else
  #define CMObjectPathToString(p,rc) \
                     ((p)->ft->toString((p),(rc)))
#endif
#endif

    // cimcArray macros


#ifdef CIMC_INLINE
      /** Gets the number of elements contained in this Array.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of elements.
      */
   inline static   cimcCount CMGetArrayCount
             (cimcArray* ar, cimcStatus* rc)
	{ return ((ar)->ft->getSize((ar),(rc))); }
#else
   #define CMGetArrayCount(a,rc)             ((a)->ft->getSize((a),(rc)))
#endif

#ifdef CIMC_INLINE
      /** Gets the element type.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of elements.
      */
   inline static   cimcType CMGetArrayType
             (cimcArray* ar, cimcStatus* rc)
	{ return ((ar)->ft->getSimpleType((ar),(rc))); }
#else
   #define CMGetArrayType(a,rc)        ((a)->ft->getSimpleType((a),(rc)))
#endif

#ifdef CIMC_INLINE
       /** Gets an element value defined by its index.
	 @param ar Array this pointer.
	 @param index Position in the internal Data array.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Element value.
      */
   inline static   cimcData CMGetArrayElementAt
             (cimcArray* ar, cimcCount index, cimcStatus* rc)
	{ return ((ar)->ft->getElementAt((ar),(index),(rc))); }
#else
   #define CMGetArrayElementAt(a,n,rc) \
                                    ((a)->ft->getElementAt((a),(n),(rc)))
#endif

#ifdef CIMC_INLINE
       /** Sets an element value defined by its index.
	 @param ar Array this pointer.
	 @param index Position in the internal Data array.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
   inline static   cimcStatus CMSetArrayElementAt
             (cimcArray* ar, cimcCount index, cimcValue* value, cimcType type)
	{ return ((ar)->ft->setElementAt((ar),(index),(value),(type))); }
#else
   #define CMSetArrayElementAt(a,n,v,t) \
                     ((a)->ft->setElementAt((a),(n),(cimcValue*)(v),(t)))
#endif



    // cimcARgs macros


#ifdef CIMC_INLINE
      /** Adds/replaces a named argument.
	 @param as Args this pointer.
         @param name Argument name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
   inline static   cimcStatus CMAddArg
              (cimcArgs* as, char* name ,void* value,
               cimcType type)
	{ return  ((as)->ft->addArg((as),(name),(cimcValue*)(value),(type))); }
#else
  #define CMAddArg(a,n,v,t) \
                           ((a)->ft->addArg((a),(n),(cimcValue*)(v),(t)))
#endif

#ifdef CIMC_INLINE
       /** Gets a named argument value.
	 @param as Args this pointer.
	 @param name Argument name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Argument value.
      */
  inline static   cimcData CMGetArg
              (cimcArgs* as, char* name, cimcStatus* rc)
	{ return  ((as)->ft->getArg((as),(name),(rc))); }
#else
  #define CMGetArg(a,n,rc)                ((a)->ft->getArg((a),(n),(rc)))
#endif

#ifdef CIMC_INLINE
       /** Gets a Argument value defined by its index.
	 @param as Args this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned argument name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Argument value.
      */
   inline static   cimcData CMGetArgAt
              (cimcArgs* as, unsigned int index, cimcString** name,
	       cimcStatus* rc)
	{ return  ((as)->ft->getArgAt((as),(index),(name),(rc))); }
#else
  #define CMGetArgAt(a,p,n,rc)       ((a)->ft->getArgAt((a),(p),(n),(rc)))
#endif

#ifdef CIMC_INLINE
      /** Gets the number of arguments contained in this Args.
	 @param as Args this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
   inline static   unsigned int CMGetArgCount
              (cimcArgs* as, cimcStatus* rc)
	{ return  ((as)->ft->getArgCount((as),(rc))); }
#else
  #define CMGetArgCount(a,rc)            ((a)->ft->getArgCount((a),(rc)))
#endif



    // cimcString Macros


#ifdef CIMC_INLINE
       /** Get a pointer to a C char* representation of this String.
	 @param st String this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to char* representation.
      */
   inline static   char* CMGetCharsPtr
             (cimcString* st, cimcStatus* rc)
	{ return  ((st)->ft->getCharPtr((st),(rc))); }
#else
  #define CMGetCharsPtr(st,rc)              ((st)->ft->getCharPtr((st),(rc)))
#endif



    // cimcDateTime macros


#ifdef CIMC_INLINE
       /** Get DateTime setting in UTC string format.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return DateTime as UTC string.
      */
   inline static   cimcString* CMGetStringFormat
             (cimcDateTime* dt, cimcStatus* rc)
	{ return ((dt)->ft->getStringFormat((dt),(rc))); }
#else
  #define CMGetStringFormat(d,rc)    ((d)->ft->getStringFormat((d),(rc)))
#endif

#ifdef CIMC_INLINE
       /** Get DateTime setting in binary format (in microsecods
	       starting since 00:00:00 GMT, Jan 1,1970).
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return DateTime in binary.
      */
   inline static   cimcUint64 CMGetBinaryFormat
             (cimcDateTime* dt, cimcStatus* rc)
	{ return ((dt)->ft->getBinaryFormat((dt),(rc))); }
#else
  #define CMGetBinaryFormat(d,rc)    ((d)->ft->getBinaryFormat((d),(rc)))
#endif

#ifdef CIMC_INLINE
       /** Tests whether DateTime is an interval value.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return True if interval value.
      */
   inline static   cimcBoolean CMIsInterval
              (cimcDateTime* dt, cimcStatus* rc)
	{ return ((dt)->ft->isInterval((dt),(rc))); }
#else
  #define CMIsInterval(d,rc)              ((d)->ft->isInterval((d),(rc)))
#endif


    // cimcEnumeration Macros


#ifdef CIMC_INLINE
       /** Get the next element of this Enumeration.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Element value.
      */
   inline static   cimcData CMGetNext
             (cimcEnumeration* en, cimcStatus* rc)
	{ return ((en)->ft->getNext((en),(rc))); }
#else
  #define CMGetNext(n,rc)                    ((n)->ft->getNext((n),(rc)))
#endif

#ifdef CIMC_INLINE
       /** Test for any elements left in this Enumeration.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return True or false.
      */
   inline static   cimcBoolean CMHasNext
             (cimcEnumeration* en, cimcStatus* rc)
	{ return ((en)->ft->hasNext((en),(rc))); }
#else
  #define CMHasNext(n,rc)                    ((n)->ft->hasNext((n),(rc)))
#endif

#ifdef CIMC_INLINE
       /** Convert this Enumeration into an Array.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The Array.
      */
   inline static   cimcArray* CMToArray
             (cimcEnumeration* en, cimcStatus* rc)
	{ return ((en)->ft->toArray((en),(rc))); }
#else
  #define CMToArray(n,rc)                   ((n)->ft->toArray((n),(rc)))
#endif



#endif // _CMCIMACS_H_
