/*!
  \file objectpath.c
  \brief Native CMPIObjectPath implementation.

  This is the native CMPIObjectPath implementation as used for remote
  providers. It reflects the well-defined interface of a regular
  CMPIObjectPath, however, it works independently from the management broker.
  
  It is part of a native broker implementation that simulates CMPI data
  types rather than interacting with the entities in a full-grown CIMOM.

  (C) Copyright IBM Corp. 2003
  (C) Copyright Intel Corp. 2005
 
  THIS FILE IS PROVIDED UNDER THE TERMS OF THE COMMON PUBLIC LICENSE 
  ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE 
  CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 
  You can obtain a current copy of the Common Public License from
  http://oss.software.ibm.com/developerworks/opensource/license-cpl.html

  \author Frank Scheffler
  $Revision: 1.6 $
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmcidt.h"
#include "cmcift.h"
#include "cmcimacs.h"
#include "native.h"
#include "utilStringBuffer.h"

#ifdef DMALLOC
#include "dmalloc.h"
#endif

extern char *pathToChars(CMPIObjectPath * cop, CMPIStatus * rc, char *str,
								 int uri);
void pathToXml(UtilStringBuffer *sb, CMPIObjectPath *cop);
UtilList *getNameSpaceComponents(CMPIObjectPath * cop);

extern char *keytype2Chars(CMPIType type);
extern char *value2Chars(CMPIType type, CMPIValue * value);


struct native_cop {
	CMPIObjectPath cop;
	char * nameSpace;
	char * classname;
	struct native_property * keys;
};


static struct native_cop * __new_empty_cop ( const char *,
					     const char *,
					     CMPIStatus * );

/****************************************************************************/


static CMPIStatus __oft_release ( CMPIObjectPath * cop )
{
	struct native_cop * o = (struct native_cop *) cop;

	if ( o ) {
 
		if (o->classname) free ( o->classname );
		if (o->nameSpace) free ( o->nameSpace );
 		propertyFT.release ( o->keys );

		free ( o );
 
 		CMReturn ( CMPI_RC_OK );
	}

	CMReturn ( CMPI_RC_ERR_FAILED );
}


static CMPIObjectPath * __oft_clone ( CMPIObjectPath * cop, CMPIStatus * rc )
{
	CMPIStatus tmp;
	struct native_cop * o   = (struct native_cop *) cop;
	struct native_cop * new = __new_empty_cop ( o->nameSpace,
						    o->classname,
						    &tmp );

	if ( tmp.rc == CMPI_RC_OK ) {
		new->keys = propertyFT.clone ( o->keys, rc );

	} else if ( rc ) CMSetStatus ( rc, tmp.rc );

	return (CMPIObjectPath *) new;
}



static CMPIStatus __oft_setNameSpace ( CMPIObjectPath * cop,
				       const char * nameSpace )
{
	struct native_cop * o = (struct native_cop *) cop;

	char * ns = ( nameSpace )? strdup ( nameSpace ): NULL;
  
	if ( o ) {
		free ( o->nameSpace );
	}

	o->nameSpace = ns;
	CMReturn ( CMPI_RC_OK );
}


static CMPIString * __oft_getNameSpace ( CMPIObjectPath * cop,
					 CMPIStatus * rc )
{
	struct native_cop * o = (struct native_cop *) cop;

	return native_new_CMPIString ( o->nameSpace, rc );
}


static CMPIStatus __oft_setHostName ( CMPIObjectPath * cop, const char * hn )
{
	CMReturn ( CMPI_RC_ERR_NOT_SUPPORTED );
}


static CMPIString * __oft_getHostName ( CMPIObjectPath * cop,
					CMPIStatus * rc )
{
	if ( rc ) CMSetStatus ( rc, CMPI_RC_ERR_NOT_SUPPORTED );
	return NULL;
}


static CMPIStatus __oft_setClassName ( CMPIObjectPath * cop,
				       const char * classname )
{
	struct native_cop * o = (struct native_cop *) cop;

	char * cn = ( classname )? strdup ( classname ): NULL;
  
	if ( o ) {
		free ( o->classname );
	}

	o->classname = cn;
	CMReturn ( CMPI_RC_OK );
}


static CMPIString * __oft_getClassName ( CMPIObjectPath * cop,
					 CMPIStatus * rc )
{
	struct native_cop * o = (struct native_cop *) cop;

	return native_new_CMPIString ( o->classname, rc );
}


static CMPIStatus __oft_addKey ( CMPIObjectPath * cop,
				 const char * name,
				 CMPIValue * value,
				 CMPIType type )
{
	struct native_cop * o = (struct native_cop *) cop;

	CMReturn ( ( propertyFT.addProperty ( &o->keys,
					      name,
					      type,
					      CMPI_keyValue,
					      value ) )?
		   CMPI_RC_ERR_ALREADY_EXISTS: 
		   CMPI_RC_OK );
}


static CMPIData __oft_getKey ( CMPIObjectPath * cop,
			       const char * name,
			       CMPIStatus * rc )
{
	struct native_cop * o = (struct native_cop *) cop;

	return propertyFT.getDataProperty ( o->keys, name, rc );
}


static CMPIData __oft_getKeyAt ( CMPIObjectPath * cop, 
				 unsigned int index,
				 CMPIString ** name,
				 CMPIStatus * rc )
{
	struct native_cop * o = (struct native_cop *) cop;

	return propertyFT.getDataPropertyAt ( o->keys, index, name, rc );
}


static unsigned int __oft_getKeyCount ( CMPIObjectPath * cop, CMPIStatus * rc )
{
	struct native_cop * o = (struct native_cop *) cop;
  
	return propertyFT.getPropertyCount ( o->keys, rc );
}


static CMPIStatus __oft_setNameSpaceFromObjectPath ( CMPIObjectPath * cop,
						     CMPIObjectPath * src )
{
	struct native_cop * s = (struct native_cop *) src;
	return __oft_setNameSpace ( cop, s->nameSpace );
}

static CMPIString *__oft_toString(CMPIObjectPath * cop, CMPIStatus * rc);

static struct native_cop * __new_empty_cop ( const char * nameSpace,
					     const char * classname,
					     CMPIStatus * rc )
{
	static CMPIObjectPathFT const oft = { 
		NATIVE_FT_VERSION,
		__oft_release,
		__oft_clone,
		__oft_setNameSpace,
		__oft_getNameSpace,
		__oft_setHostName,
		__oft_getHostName,
		__oft_setClassName,
		__oft_getClassName,
		__oft_addKey,
		__oft_getKey,
		__oft_getKeyAt,
		__oft_getKeyCount,
		__oft_setNameSpaceFromObjectPath,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		__oft_toString
	};
        
	static CMPIObjectPath const o = {
		"CMPIObjectPath",
		&oft
	};

	struct native_cop * cop =
	      (struct native_cop *) calloc ( 1, sizeof ( struct native_cop ) );

	cop->cop       = o;
	cop->classname = ( classname )? strdup ( classname ): NULL;
	cop->nameSpace = ( nameSpace )? strdup ( nameSpace ): NULL;

	if ( rc ) CMSetStatus ( rc, CMPI_RC_OK );
	return cop;
}


CMPIObjectPath * newCMPIObjectPath ( const char * nameSpace, 
					     const char * classname,
					     CMPIStatus * rc )
{
	return (CMPIObjectPath *) __new_empty_cop ( nameSpace,
						    classname,
						    rc );
}

void sameReleaseCMPIString( CMPIString *str )
{
    // If we are not freeing a NULL pointer ...
    if( str )
    {
        // Free dynamically allocated memory.
        CMRelease( str );
    }
}

int sameCompareCMPIString( CMPIString *str1, CMPIString *str2 )
{
    int result=1;

    if( !str1 || !str2 ||
        ( strcmp( CMGetCharsPtr( str1, NULL ),
                  CMGetCharsPtr( str2, NULL ) ) != 0 ) )
    {
        result=0;
    }

    /* Free up dynamically allocated memory if needed. */
    sameReleaseCMPIString( str1 );
    sameReleaseCMPIString( str2 );

    return result;
}

int sameCMPIObjectPath (const CMPIObjectPath *cop1, const CMPIObjectPath *cop2)
{
   CMPIString *keyname;
   CMPIData   data1, data2;
   CMPIStatus status;
   struct native_cop *ncop1 = (struct native_cop *)cop1;
   struct native_cop *ncop2 = (struct native_cop *)cop2;
   unsigned int i, m;
   char *cv1, *cv2;

   /* Check if name spaces are the same */
   if (strcmp(ncop1->nameSpace, ncop2->nameSpace) != 0)
      return 0;

   /* Check if classnames are the same */
   if (strcmp(ncop1->classname, ncop2->classname) != 0)
      return 0;

   /* Check if the key count is the same */
   m = propertyFT.getPropertyCount(ncop1->keys, NULL);
   if (m != propertyFT.getPropertyCount(ncop2->keys, NULL)) 
      return 0;

   /* Check on each key */
   for (i = 0; i < m; i++) {
      data1 = propertyFT.getDataPropertyAt(ncop1->keys, i, &keyname, &status );

      /* check if key exists in both */
      if (status.rc != CMPI_RC_OK)
      {
        // Free up dynamically allocated memory
        sameReleaseCMPIString( keyname );
        return 0;
      }

      // This in effect verifies the keynames are the same. We are searching
      // for the keyname of cop1 in cop2.  
      data2 = propertyFT.getDataProperty(ncop2->keys,
					 CMGetCharsPtr( keyname, NULL ),
					 &status );

      // Free up dynamically allocated memory
      sameReleaseCMPIString( keyname );

      /* check if key exists in both */
      if (status.rc != CMPI_RC_OK)
	  return 0;

      /* Check if the values are the same */
      if( data1.type  != data2.type       ||
          data1.state != data2.state  ){
	      return 0;
      } else {
	      cv1 = value2Chars(data1.type, &data1.value);
	      cv2 = value2Chars(data2.type, &data2.value);
	      if (strcmp(cv1,cv2) == 0) {
		      if (cv1) free(cv1);
		      if (cv2) free(cv2);
	      } else {
		      if (cv1) free(cv1);
		      if (cv2) free(cv2);
		      return 0;
	      }
      }
   }

   return 1;
}

char *pathToChars(CMPIObjectPath * cop, CMPIStatus * rc, char *str, int uri)
{
//            "//atp:9999/root/cimv25:TennisPlayer.first="Patrick",last="Rafter";

   CMPIString *ns;
   CMPIString *cn;
   CMPIString *name;
   CMPIData data;
   unsigned int i, m, s;
   char *v;
   char *colon = (uri) ? "%3A" : ":";
   
   *str = 0;

   ns = cop->ft->getNameSpace(cop, rc);
   cn = cop->ft->getClassName(cop, rc);
   
   if (ns && ns->hdl && *(char*)ns->hdl) {
      if (!uri)
	  strcpy(str,(char*)ns->hdl);
      else {
         char *cns=(char*)ns->hdl;
         for (s=i=0, m=strlen(cns); i<m; i++,s++) {
            if (cns[i]=='/') {
               str[s++]='%';
               str[s++]='2';
               str[s]='F';
            }
            else
	       str[s]=cns[i];    
         } 
         str[s]=0;   
      }
      CMRelease(ns);
      strcat(str,colon);
   }   
   
   strcat(str, (char *) cn->hdl);
   CMRelease(cn);
   
   for (i = 0, m = cop->ft->getKeyCount(cop, rc); i < m; i++) {
      data = cop->ft->getKeyAt(cop, i, &name, rc);
      strcat(str, i ? "," : ".");
      strcat(str, (char *) name->hdl);
      strcat(str, (uri) ? "%3D" : "=");
      v = value2Chars(data.type, &data.value);
      if (data.type & (CMPI_INTEGER | CMPI_REAL))
         strcat(str, v);
      else {
	 strcat(str, "\"");
         strcat(str, v);
	 strcat(str, "\"");
      }
      free(v);
      CMRelease(name);
   };
 
   return str;
}

static CMPIString *__oft_toString(CMPIObjectPath * cop, CMPIStatus * rc)
{
   char str[4096] = { 0 };
   pathToChars(cop, rc, str,0);
   return native_new_CMPIString(str, rc);
}

const char *getNameSpaceChars(CMPIObjectPath * cop)
{
	struct native_cop * o = (struct native_cop *) cop;
   return o->nameSpace;
}

UtilList *getNameSpaceComponents(CMPIObjectPath * cop)
{
   UtilList *ul=newList();
   CMPIString *nss=__oft_getNameSpace(cop, NULL);
   int s=0,i,m;
   char nsc[256],*ns;
   
   if (nss && nss->hdl) {
      ns=(char*)nss->hdl;
      if (ns) for (s=i=0, m=strlen(ns); i<m; i++,s++) {
         if (ns[i]=='/') {
            nsc[s]=0;
            ul->ft->append(ul,strdup(nsc));
            s=-1;
         }
         else nsc[s]=ns[i];    
      } 
      nsc[s]=0;
   }   
   
   if (s) ul->ft->append(ul,strdup(nsc));
   CMRelease(nss);
   return ul;
}

void pathToXml(UtilStringBuffer *sb, CMPIObjectPath *cop)
{
   int i,s,m,state=0;
   CMPIData data;
   CMPIString *name;
   char *cv;

   for (i=0,s=__oft_getKeyCount(cop,NULL); i<s; i++) {
      data=__oft_getKeyAt(cop,i,&name,NULL);
      switch (state) {
      case 0:
         sb->ft->append3Chars(sb,"<KEYBINDING NAME=\"",(char*)name->hdl,"\">");
         if (data.type==CMPI_ref) {
            CMPIObjectPath *ref=data.value.ref;           
            sb->ft->appendChars(sb, "<VALUE.REFERENCE><INSTANCEPATH>");
            
            CMPIString *hn=__oft_getHostName(ref, NULL);;
            sb->ft->append3Chars(sb,"<NAMESPACEPATH><HOST>",(char*)hn->hdl,"</HOST>");
            CMRelease(hn);
            
            sb->ft->appendChars(sb, "<VALUE.REFERENCE><INSTANCEPATH><LOCALNAMESPACEPATH>");
            CMPIString *nss=__oft_getNameSpace(ref, NULL);
            if (nss && nss->hdl) {
               char nsc[256];
               char *ns=(char*)nss->hdl;
               if (ns) for (s=i=0, m=strlen(ns); i<m; i++,s++) {
                  if (ns[i]=='/') {
                     nsc[s]=0;
                     sb->ft->append3Chars(sb,"<NAMESPACE NAME=\"",nsc,"\"></NAMESPACE>");
                     s=0;
                  }
                  else nsc[s]=ns[i];    
              } 
              nsc[s]=0;
              if (s) sb->ft->append3Chars(sb,"<NAMESPACE NAME=\"",nsc,"\"></NAMESPACE>");
              CMRelease(nss);
            }   
            
            sb->ft->appendChars(sb,"</LOCALNAMESPACEPATH></NAMESPACEPATH>");
            
            CMPIString *cn=__oft_getClassName(ref, NULL);
            sb->ft->append3Chars(sb,"<INSTANCENAME CLASSNAME=\"",(char*)cn->hdl,"\">");
            state=1;
            continue;
         }
         else {
            sb->ft->append5Chars(sb,"<KEYVALUE VALUETYPE=\"",keytype2Chars(data.type),"\">",
                cv=value2Chars(data.type,&data.value),"</KEYVALUE>");
	    if (cv) free (cv);
            state=0;
         }
         break; 
      case 1:
         if (data.type==CMPI_ref) {
            sb->ft->appendChars(sb,"</INSTANCENAME></INSTANCEPATH></VALUE.REFERENCE>");
            i--;
            state=0;
         }
         else {
            sb->ft->append3Chars(sb,"<KEYBINDING NAME=\"",(char*)name->hdl,"\">");
            sb->ft->append5Chars(sb,"<KEYVALUE VALUETYPE=\"",keytype2Chars(data.type),"\">",
                cv=value2Chars(data.type,&data.value),"</KEYVALUE>");
	    if (cv) free (cv);
         }
         break;
      }
      sb->ft->appendChars(sb,"</KEYBINDING>\n");
      if (name) CMRelease(name);
   }

   if (state==1)
      sb->ft->appendChars(sb,"</INSTANCENAME></INSTANCEPATH></VALUE.REFERENCE></KEYBINDING>");
}

/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
