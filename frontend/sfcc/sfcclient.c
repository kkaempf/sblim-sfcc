
/*
 * sfcclient.c
 *
 * © Copyright IBM Corp. 2007
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
 * Sfcc Client function tables.
 *
 */

#include "cimc.h"
#include "cmci.h"
#include "native.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* 
 * sfcc legacy entry points
 * this is required while sfcc clients haven't
 * switched to the new canonical entry points.
 */

static struct _ConnectionControl {
  pthread_mutex_t  ccMux;
  unsigned         ccCount;
  CIMCEnv         *ccEnv;
  CMPIStatus     (*ccRelease)(CMCIClient *cc);
} ConnectionControl = {
  PTHREAD_MUTEX_INITIALIZER, 0, NULL, NULL
};

static char *DefaultClient = "XML";

static CMPIStatus cmciRelease(CMCIClient* cc)
{
  CMPIStatus rc = {CMPI_RC_OK, NULL};
  pthread_mutex_lock(&ConnectionControl.ccMux);
  cc->ft->release = ConnectionControl.ccRelease;
  cc->ft->release(cc);
  if (ConnectionControl.ccCount > 0) {
    if (--ConnectionControl.ccCount == 0) {
      ReleaseCIMCEnv(ConnectionControl.ccEnv);
    }
  }
  pthread_mutex_lock(&ConnectionControl.ccMux);
  return rc;
}

CMCIClient *cmciConnect(const char *hn, const char *scheme, const char *port,
                        const char *user, const char *pwd, CMPIStatus *rc)
{
  return cmciConnect2(hn,scheme,port,user,pwd,0,NULL,NULL,NULL,rc);
}

CMCIClient *cmciConnect2(const char *hn, const char *scheme, const char *port,
			 const char *user, const char *pwd, 
			 int verifyMode, const char * trustStore,
			 const char * certFile, const char * keyFile,
			 CMPIStatus *rc)
{  
  CMCIClient     *cc;
  char           *msg;
  int             retc;
  char           *client;

  pthread_mutex_lock(&ConnectionControl.ccMux);
  if (ConnectionControl.ccCount == 0) {
    client = getenv("SFCC_CLIENT");
    if (client == NULL) {
      client = DefaultClient;
    }
    ConnectionControl.ccEnv = NewCIMCEnv(client,0,&retc, &msg);
  }
  ConnectionControl.ccCount += 1;
  if (retc && rc) {
    rc->rc=CMPI_RC_ERR_FAILED;
    rc->msg=NULL;
    cc=NULL;
  } else {
    cc = (CMCIClient*)ConnectionControl.ccEnv->ft->connect2(ConnectionControl.ccEnv,hn,scheme,port,user,pwd,verifyMode,trustStore,certFile,keyFile,(CIMCStatus*)rc);
    if (cc) {
      ConnectionControl.ccRelease = cc->ft->release;
      cc->ft->release=cmciRelease;
    }
  }
  pthread_mutex_unlock(&ConnectionControl.ccMux);
  return cc;
}

CMPIObjectPath *newCMPIObjectPath( const char * namespace, 
				   const char * classname,
				   CMPIStatus * rc )
{
  return (CMPIObjectPath*)ConnectionControl.ccEnv->ft->
    newObjectPath(ConnectionControl.ccEnv,namespace,classname,
		  (CIMCStatus*)rc);
}

CMPIInstance *native_new_CMPIInstance( CMPIObjectPath *op,
					CMPIStatus * rc )
{
  return (CMPIInstance*)ConnectionControl.ccEnv->ft->
    newInstance(ConnectionControl.ccEnv,(CIMCObjectPath *)op,
		(CIMCStatus*)rc);
}

CMPIString *native_new_CMPIString( const char *ptr,
				   CMPIStatus * rc )
{
  return (CMPIString*)ConnectionControl.ccEnv->ft->
    newString(ConnectionControl.ccEnv, ptr, (CIMCStatus*)rc);
}

CMPIArgs *native_new_CMPIArgs( CMPIStatus * rc )
{
  return (CMPIArgs*)ConnectionControl.ccEnv->ft->
    newArgs(ConnectionControl.ccEnv,(CIMCStatus*)rc);
}

CMPIArray * native_new_CMPIArray ( CMPICount size,
				   CMPIType type,
				   CMPIStatus * rc)
{
  return (CMPIArray*)ConnectionControl.ccEnv->ft->
    newArray(ConnectionControl.ccEnv, size, type,(CIMCStatus*)rc);
}

CMPIDateTime * native_new_CMPIDateTime ( CMPIStatus * rc)
{
  return (CMPIDateTime*)ConnectionControl.ccEnv->ft->
    newDateTime(ConnectionControl.ccEnv,(CIMCStatus*)rc);
}

CMPIDateTime * native_new_CMPIDateTime_fromBinary ( CMPIUint64 date,
						    CMPIBoolean interval,
						    CMPIStatus * rc)
{
  return (CMPIDateTime*)ConnectionControl.ccEnv->ft->
    newDateTimeFromBinary(ConnectionControl.ccEnv, date, interval,
			  (CIMCStatus*)rc);
}

CMPIDateTime * native_new_CMPIDateTime_fromChars ( const char * date,
						   CMPIStatus * rc)
{
  return (CMPIDateTime*)ConnectionControl.ccEnv->ft->
    newDateTimeFromChars(ConnectionControl.ccEnv, date,
			 (CIMCStatus*)rc);
}

CMPIEnumeration * native_new_CMPIEnumeration ( CMPIArray * arr, 
					       CMPIStatus * rc)
{
  fprintf (stderr, "*** native_new_CMPIEnumeration not implemented *** ");
  return NULL;
}

CMPIConstClass * native_new_CMPIConstClass ( char *cn,
					     CMPIStatus * rc)
{
  fprintf (stderr, "*** native_new_CMPIConstClass not implemented *** ");
  return NULL;
}

void native_release_CMPIValue ( CMPIType type, CMPIValue * val )
{
  fprintf (stderr, "*** native_release_CMPIValue not implemented *** ");
}

void * newList ()
{
  fprintf (stderr, "*** newList not implemented *** ");
  return NULL;
}

void * newStringBuffer (int s)
{
  fprintf (stderr, "*** newStringBuffer not implemented *** ");
  return NULL;
}

void * value2Chars (CMPIType type, CMPIValue *value)
{
  fprintf (stderr, "*** value2Chars not implemented *** ");
  return NULL;
}
