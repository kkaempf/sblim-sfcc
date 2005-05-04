   

/*
 * utilList.h
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
 *
*/

#ifndef UTILList_H 
#define UTILList_H 
 
   
   struct _Util_List_FT;
   typedef struct _Util_List_FT Util_List_FT;

   struct _UtilList {
      void *hdl;
      Util_List_FT *ft;
      int  mem_state;
   };
   typedef struct _UtilList UtilList;

   struct _Util_List_FT {
      int version;
      void (*release)
       (UtilList * ul);
      UtilList *(*clone)
       (UtilList * ul);
      void (*clear)
       (UtilList * ul);
      unsigned long (*size)
       (UtilList * ul);
      int (*isEmpty)
       (UtilList * ul);
      int (*contains)
       (UtilList * ul, const void *elm);
      void (*append)
       (UtilList * ul, const void *elm);
      void (*prepend)
       (UtilList * ul, const void *elm);
      void (*add)
       (UtilList * ul, const void *elm);
      void *(*getFirst)
       (UtilList * ul);
      void *(*getLast)
       (UtilList * ul);
      void *(*getNext)
       (UtilList * ul);
      void *(*getPrevious)
       (UtilList * ul);
      void *(*getCurrent)
       (UtilList * ul);
      void *(*removeFirst)
       (UtilList * ul);
      void *(*removeLast)
       (UtilList * ul);
      void *(*removeCurrent)
       (UtilList * ul);
      void *(*removeThis)
       (UtilList * ul, void *elm);
   };

      UtilList *newList();
      
#endif
