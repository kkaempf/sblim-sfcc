
/*
 * cmpic.h
 *
 * (C) Copyright IBM Corp. 2005
 * (C) Copyright Intel Corp. 2005
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

#include "cmci.h"
#include "utilStringBuffer.h"
#include "utilList.h"
#include "native.h"

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cimXmlParser.h"

#ifdef DEBUG
#undef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

#ifdef DMALLOC
#include "dmalloc.h"
#endif

void list2StringBuffer(UtilStringBuffer *sb, UtilList *ul, char *sep);
char *getResponse(CMCIConnection *con, CMPIObjectPath *cop);
CMCIConnection *initConnection(CMCIClientData *cld);

extern UtilList *getNameSpaceComponents(CMPIObjectPath * cop);
extern void pathToXml(UtilStringBuffer *sb, CMPIObjectPath *cop);
extern char *value2Chars(CMPIType type, CMPIValue * value);
extern char *pathToChars(CMPIObjectPath * cop, CMPIStatus * rc, char *str,
								 int uri);

#if DEBUG
int do_debug = 0;	/* simple enable debug messages flag */

static void set_debug()
{
    static int firsttime = 1;
    char *dbg;
    if (firsttime == 1) {
	firsttime--;
        do_debug = ((dbg = getenv("CMPISFCC_DEBUG")) != NULL &&
		    strcasecmp(dbg, "true") == 0);
    }
}
#endif


typedef const struct _CMCIConnectionFT {
    CMPIStatus (*release) (CMCIConnection *);
    char *(*genRequest)(ClientEnc *cle, const char *op, CMPIObjectPath *cop,
						  int classWithKeys);
    char *(*addPayload)(CMCIConnection *, UtilStringBuffer *pl);
    char *(*getResponse)(CMCIConnection *con, CMPIObjectPath *cop);
    void (*initializeHeaders)(CMCIConnection *con);
    void (*reset)(CMCIConnection *);
} CMCIConnectionFT;

struct _ClientEnc {
   CMCIClient enc;
   CMCIClientData data;
   CMCIConnection *connection;
};

struct _CMCIConnection {
    CMCIConnectionFT *ft;        // The handle to the curl object
    CURL *mHandle;               // The list of headers sent with each request
    struct curl_slist *mHeaders; // The body of the request
    UtilStringBuffer *mBody;     // The uri of the request
    UtilStringBuffer *mUri;      // The username/password used in authentication
    UtilStringBuffer *mUserPass; // Used to store the HTTP response
    UtilStringBuffer *mResponse;
};

/* --------------------------------------------------------------------------*/

static char *getErrorMessage(CURLcode err)
{
#if LIBCURL_VERSION_NUM >= 0x071200
    return curl_easy_strerror(err);
#else
    char error[64];
    sprintf(error,"CURL error: %d", err);
    return strdup(error);
#endif
}

/* --------------------------------------------------------------------------*/

void list2StringBuffer(UtilStringBuffer *sb, UtilList *ul, char *sep)
{
   void *e;
   for (e=ul->ft->getFirst(ul); e; e=ul->ft->getNext(ul)) {
      sb->ft->appendChars(sb,(char*)e);
      sb->ft->appendChars(sb,sep);
   }
}

/* --------------------------------------------------------------------------*/

static inline size_t writeCb(void *ptr, size_t size,
					size_t nmemb, void *stream)
{
    UtilStringBuffer *sb=(UtilStringBuffer*)stream;
    unsigned int length = size * nmemb;
    sb->ft->appendBlock(sb, ptr, length);
    return length;
}



/* --------------------------------------------------------------------------*/

static CMPIStatus releaseConnection(CMCIConnection *con)
{
  CMPIStatus rc = {CMPI_RC_OK,NULL};
  if (con->mHeaders) {
    curl_slist_free_all(con->mHeaders);
    con->mHeaders = NULL;
  }
  curl_easy_cleanup(con->mHandle);
  if (con->mBody) CMRelease(con->mBody);
  if (con->mUri) CMRelease(con->mUri);
  if (con->mUserPass) CMRelease(con->mUserPass);
  if (con->mResponse) CMRelease(con->mResponse);

  free(con);
  return rc;
}

/* --------------------------------------------------------------------------*/

static char *addPayload(CMCIConnection *con, UtilStringBuffer *pl)
{
//    con->mBody = pl;
//    if (dumpXml)
//        cerr << "To server: " << pl << endl;

    CURLcode rv;

    rv = curl_easy_setopt(con->mHandle, CURLOPT_POSTFIELDS,
					pl->ft->getCharPtr(pl));
    if (rv) return getErrorMessage(rv);
    rv = curl_easy_setopt(con->mHandle, CURLOPT_POSTFIELDSIZE,
					pl->ft->getSize(pl));
    if (rv) return getErrorMessage(rv);
    return NULL;
}

/* --------------------------------------------------------------------------*/

static char* genRequest(ClientEnc *cle, const char *op,
			CMPIObjectPath *cop, int classWithKeys)
{
   CMCIConnection   *con = cle->connection;
   CMCIClientData   *cld = &cle->data;
   UtilList 	    *nsc;
   char		    method[256]    = "CIMMethod: ";
   char		    CimObject[512] = "CIMObject: ";
   char		    *nsp;

   if (!con->mHandle) return "Unable to initialize curl interface.";

//    if (!supportsSSL() && url.scheme == "https")
//        throw HttpException("this curl library does not support https urls.");

   con->mResponse->ft->reset(con->mResponse);

   con->mUri->ft->reset(con->mUri);
   con->mUri->ft->append6Chars(con->mUri, cld->scheme, "://", cld->hostName,
						  ":", cld->port, "/cimom");

   /* Initialize curl with the url */
   curl_easy_setopt(con->mHandle, CURLOPT_URL,
				  con->mUri->ft->getCharPtr(con->mUri));

   /* Disable progress output */
   curl_easy_setopt(con->mHandle, CURLOPT_NOPROGRESS, 1);

   /* This will be a HTTP post */
   curl_easy_setopt(con->mHandle, CURLOPT_POST, 1);

   /* Disable SSL verification */
   curl_easy_setopt(con->mHandle, CURLOPT_SSL_VERIFYHOST, 0);
   curl_easy_setopt(con->mHandle, CURLOPT_SSL_VERIFYPEER, 0);

   /* Setup authentication */
   curl_easy_setopt(con->mHandle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

   /* Set username and password */
   if (cld->user != NULL) {
      UtilStringBuffer *UserPass = con->mUserPass;

      UserPass->ft->reset(UserPass);
      UserPass->ft->appendChars(UserPass, cld->user);
      if (cld->pwd)
	 UserPass->ft->append3Chars(UserPass, ":", cld->pwd, NULL);
      /* Setup "<userid>:<password>" */
      curl_easy_setopt(con->mHandle, CURLOPT_USERPWD,
				     UserPass->ft->getCharPtr(UserPass));
   }

   // Initialize default headers
   con->ft->initializeHeaders(con);

   // Add CIMMethod header
   strcat(method, op);
   con->mHeaders = curl_slist_append(con->mHeaders, method);

   // Add CIMObject header with cop's namespace, class, keys
   if (classWithKeys)
       pathToChars(cop, NULL, &CimObject[11], 1);
   else {
       nsc = getNameSpaceComponents(cop);
       nsp = nsc->ft->getFirst(nsc);
       while (nsp != NULL) {
	   strcat(CimObject, nsp);
	   free(nsp); /* VM: freeing strdup'ed memory - should be part of release */
           if ((nsp = nsc->ft->getNext(nsc)) != NULL)
	       strcat(CimObject, "%2F");
       }
       CMRelease(nsc);
   }
   con->mHeaders = curl_slist_append(con->mHeaders, CimObject);

   // Set all of the headers for the request
   curl_easy_setopt(con->mHandle, CURLOPT_HTTPHEADER, con->mHeaders);

   // Set up the callbacks to store the response
   curl_easy_setopt(con->mHandle, CURLOPT_WRITEFUNCTION, writeCb);

   // Use CURLOPT_FILE instead of CURLOPT_WRITEDATA - more portable
   curl_easy_setopt(con->mHandle, CURLOPT_FILE, con->mResponse);

   // Fail if we receive an error (HTTP response code >= 300)
   curl_easy_setopt(con->mHandle, CURLOPT_FAILONERROR, 1);

   // Turn this on to enable debugging
#if DEBUG
   curl_easy_setopt(con->mHandle, CURLOPT_VERBOSE, do_debug);
#endif

   return NULL;
}

/* --------------------------------------------------------------------------*/

char *getResponse(CMCIConnection *con, CMPIObjectPath *cop)
{
    CURLcode rv;

    rv = curl_easy_perform(con->mHandle);

    if (rv) {
        long responseCode = -1;
        // Use CURLINFO_HTTP_CODE instead of CURLINFO_RESPONSE_CODE
        // (more portable to older versions of curl)
        curl_easy_getinfo(con->mHandle, CURLINFO_HTTP_CODE, &responseCode);
        return (responseCode == 401) ? strdup("Invalid username/password") :
				       getErrorMessage(rv);
    }

    if (con->mResponse->ft->getSize(con->mResponse) == 0)
        return strdup("No data received from server");

    return NULL;
}

/* --------------------------------------------------------------------------*/

static void initializeHeaders(CMCIConnection *con)
{
    static const char *headers[] = {
	"Content-type: application/xml; charset=\"utf-8\"",
	"Accept:",
	"Expect:",
	"CIMProtocolVersion: 1.0",
	"CIMOperation: MethodCall",
	NULL
    };
    unsigned int i;

    if (con->mHeaders) {
        curl_slist_free_all(con->mHeaders);
        con->mHeaders = NULL;
    }
    for (i = 0; headers[i]!= NULL; i++)
        con->mHeaders = curl_slist_append(con->mHeaders, headers[i]);
}

static CMCIConnectionFT conFt={
  releaseConnection,
  genRequest,
  addPayload,
  getResponse,
  initializeHeaders
};

/* --------------------------------------------------------------------------*/

CMCIConnection *initConnection(CMCIClientData *cld)
{
   CMCIConnection *c=(CMCIConnection*)calloc(1,sizeof(CMCIConnection));
   c->ft=&conFt;
   c->mHandle = curl_easy_init();
   c->mHeaders = NULL;
   c->mBody = newStringBuffer(256);
   c->mUri = newStringBuffer(256);
   c->mUserPass = newStringBuffer(64);
   c->mResponse = newStringBuffer(2048);

   return c;
}

/*--------------------------------------------------------------------------*/
static inline void emitlocal(UtilStringBuffer *sb, int f)
{
   sb->ft->append3Chars(sb, "<IPARAMVALUE NAME=\"LocalOnly\"><VALUE>",
			    f != 0? "TRUE" : "FALSE",
			    "</VALUE></IPARAMVALUE>\n");
}

/* --------------------------------------------------------------------------*/

static inline void emitqual(UtilStringBuffer *sb, int f)
{
   sb->ft->append3Chars(sb, "<IPARAMVALUE NAME=\"IncludeQualifiers\"><VALUE>",
			    f != 0 ? "TRUE" : "FALSE",
			    "</VALUE></IPARAMVALUE>\n");
}

/* --------------------------------------------------------------------------*/

static inline void emitorigin(UtilStringBuffer *sb, int f)
{
   sb->ft->append3Chars(sb, "<IPARAMVALUE NAME=\"IncludeClassOrigin\"><VALUE>",
			    f != 0 ? "TRUE" : "FALSE",
			    "</VALUE></IPARAMVALUE>\n");
}

/* --------------------------------------------------------------------------*/

static inline void emitdeep(UtilStringBuffer *sb, int f)
{
   sb->ft->append3Chars(sb, "<IPARAMVALUE NAME=\"DeepInheritance\"><VALUE>",
			    f != 0 ? "TRUE" : "FALSE",
			    "</VALUE></IPARAMVALUE>\n");
}

/* --------------------------------------------------------------------------*/

static inline void addXmlProperty(UtilStringBuffer *sb,
				  char *name, char *value)
{
   sb->ft->append6Chars(sb, "<PROPERTY NAME=\"", name, "\" TYPE=\"string\">",
   			    "<VALUE>", value, "</VALUE></PROPERTY>\n");
}

/* --------------------------------------------------------------------------*/

static void addXmlNamespace(UtilStringBuffer *sb, CMPIObjectPath *cop)
{
   UtilList *nsc = getNameSpaceComponents(cop);
   char *nsp;

   sb->ft->appendChars(sb,"<LOCALNAMESPACEPATH>");
   for (nsp = nsc->ft->getFirst(nsc); nsp; nsp = nsc->ft->getNext(nsc)) {
      sb->ft->append3Chars(sb, "<NAMESPACE NAME=\"", nsp, "\"></NAMESPACE>");
      free(nsp); /* VM: freeing strdup'ed memory - should be part of release */
   }
   sb->ft->appendChars(sb, "</LOCALNAMESPACEPATH>\n");
   CMRelease(nsc);
}

/* --------------------------------------------------------------------------*/

static void addXmlObjectName(UtilStringBuffer *sb, CMPIObjectPath *cop,
					      const char *ValueTagName)
{
   CMPIString *cn = cop->ft->getClassName(cop, NULL);
   sb->ft->append5Chars(sb, "<IPARAMVALUE NAME=\"",
			    ValueTagName,
			    "\">\n<INSTANCENAME CLASSNAME=\"",
			    (char *)cn->hdl, "\">\n");
   pathToXml(sb, cop);
   sb->ft->appendChars(sb,"</INSTANCENAME>\n</IPARAMVALUE>\n");
   CMRelease(cn);
}

/* --------------------------------------------------------------------------*/

static void addXmlClassnameParam(UtilStringBuffer *sb, CMPIObjectPath *cop)
{
   CMPIString *cn = cop->ft->getClassName(cop, NULL);
   if (cn->hdl && *((char*)cn->hdl)) {
      sb->ft->append3Chars(sb,
	      "<IPARAMVALUE NAME=\"ClassName\"><CLASSNAME NAME=\"",
	      (char*)cn->hdl, "\"/></IPARAMVALUE>\n");
   }
   CMRelease(cn);
}

/* --------------------------------------------------------------------------*/

static inline void addXmlHeader(UtilStringBuffer *sb)
{
   static const char xmlHeader[]={
      "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n"
      "<CIM CIMVERSION=\"2.0\" DTDVERSION=\"2.0\">\n"
      "<MESSAGE ID=\"4711\" PROTOCOLVERSION=\"1.0\">\n"
      "<SIMPLEREQ>\n"
   };

   sb->ft->appendChars(sb, xmlHeader);
}

/* --------------------------------------------------------------------------*/

static inline void addXmlFooter(UtilStringBuffer *sb)
{
   static const char xmlFooter[]={
      "</SIMPLEREQ>\n"
      "</MESSAGE>\n"
      "</CIM>\n"
   };

   sb->ft->appendChars(sb, xmlFooter);
}

/* --------------------------------------------------------------------------*/

static void addXmlPropertyListParam(UtilStringBuffer *sb, char** properties)
{
   sb->ft->appendChars(sb, "<IPARAMVALUE NAME=\"PropertyList\"><VALUE.ARRAY>");
   while (*properties) {
       sb->ft->append3Chars(sb, "<VALUE>", *properties, "</VALUE>");
       properties++;
   }
   sb->ft->appendChars(sb, "</VALUE.ARRAY></IPARAMVALUE>\n");
}

/* --------------------------------------------------------------------------*/
/* --------------------------------------------------------------------------*/

static CMCIClient * cloneClient ( CMCIClient * cl, CMPIStatus * st )
{
  CMPIStatus rc;
  CMSetStatusWithChars(&rc, CMPI_RC_ERR_NOT_SUPPORTED, "Clone function not supported");
  if (st) *st=rc;
  return NULL;
}  

static CMPIStatus releaseClient(CMCIClient * mb)
{
  CMPIStatus rc={CMPI_RC_OK,NULL};
  ClientEnc		* cl = (ClientEnc*)mb;
  
  if (cl->data.hostName) {
    free(cl->data.hostName);
  }
  if (cl->data.user) {
    free(cl->data.user);
  }
  if (cl->data.pwd) {
    free(cl->data.pwd);
  }
  if (cl->data.scheme) {
    free(cl->data.scheme);
  }
  if (cl->data.port) {
    free(cl->data.port);
  }
 
  if (cl->connection) CMRelease(cl->connection);

  free(cl);
  return rc;
}

/* Finished & working */
static CMPIEnumeration * enumInstanceNames(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	CMPIStatus * rc)
{
   ClientEnc		* cl = (ClientEnc*)mb;
   CMCIConnection	* con = cl->connection;
   UtilStringBuffer	* sb = newStringBuffer(2048);
   char			* error;
   ResponseHdr		rh;

#if DEBUG
   set_debug();
#endif

   con->ft->genRequest(cl, "EnumerateInstanceNames", cop, 0);

   /* Construct the CIM-XML request */
   addXmlHeader(sb);
   sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"EnumerateInstanceNames\">");

   addXmlNamespace(sb, cop);
   addXmlClassnameParam(sb, cop);

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

   error = con->ft->addPayload(con, sb);

   if (error || (error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED, error);
      free(error);
      CMRelease(sb);
      return NULL;
   }

   CMRelease(sb);

   rh = scanCimXmlResponse(CMGetCharPtr(con->mResponse), cop);

   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      free(rh.description);
      CMRelease(rh.rvArray);
      return NULL;
   }

#if DEBUG
#ifdef CHECK_RETURN_TYPES
   if (CMGetArrayCount(rh.rvArray,NULL)==0 ||
       rh.rvArray->ft->getSimpleType(rh.rvArray, NULL) != CMPI_ref) {
	/* Should not happen in production builds */
	CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED,
				 strdup("Unexpected return value"));
	return NULL;
   }
#else
   if (do_debug)
       printf ("\treturn enumeration array type %d expected CMPI_ref %d\n",
	        rh.rvArray->ft->getSimpleType(rh.rvArray, NULL), CMPI_ref);
#endif
#endif

   CMSetStatus(rc,CMPI_RC_OK);
   return newCMPIEnumeration(rh.rvArray, NULL);
}

/* --------------------------------------------------------------------------*/

/* Finished & working */
static CMPIInstance * getInstance(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	CMPIFlags flags,
	char ** properties,
	CMPIStatus * rc)
/*
<?xml version="1.0" encoding="utf-8"?>
<CIM CIMVERSION="2.0" DTDVERSION="2.0">
  <MESSAGE ID="4711" PROTOCOLVERSION="1.0">
    <SIMPLEREQ>
      <IMETHODCALL NAME="GetInstance">
        <LOCALNAMESPACEPATH>
          <NAMESPACE NAME="root"/>
          <NAMESPACE NAME="cimv2"/>
        </LOCALNAMESPACEPATH>
        <IPARAMVALUE NAME="LocalOnly">
          <VALUE>FALSE</VALUE>
        </IPARAMVALUE>
        <IPARAMVALUE NAME="IncludeClassOrigin">
          <VALUE>FALSE</VALUE>
        </IPARAMVALUE>
        <IPARAMVALUE NAME="IncludeQualifiers">
          <VALUE>FALSE</VALUE>
        </IPARAMVALUE>
        <IPARAMVALUE NAME="InstanceName">
          <INSTANCENAME CLASSNAME="Linux_ComputerSystem">
            <KEYBINDING NAME="CreationClassName">
              <KEYVALUE VALUETYPE="string">Linux_ComputerSystem</KEYVALUE>
            </KEYBINDING>
            <KEYBINDING NAME="Name">
              <KEYVALUE VALUETYPE="string">bestorga.ibm.com</KEYVALUE>
            </KEYBINDING>
          </INSTANCENAME>
        </IPARAMVALUE>
      </IMETHODCALL>
    </SIMPLEREQ>
  </MESSAGE>
</CIM>
*/
{
   CMPIInstance *cci;
   ClientEnc *cl=(ClientEnc*)mb;
   CMCIConnection *con=cl->connection;
   UtilStringBuffer *sb=newStringBuffer(2048);
   char *error;
   ResponseHdr rh;

#if DEBUG
   set_debug();
#endif

   con->ft->genRequest(cl, "GetInstance", cop, 0);

   addXmlHeader(sb);
   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"GetInstance\">");

   addXmlNamespace(sb, cop);

   emitlocal(sb,flags & CMPI_FLAG_LocalOnly);
   emitorigin(sb,flags & CMPI_FLAG_IncludeClassOrigin);
   emitqual(sb,flags & CMPI_FLAG_IncludeQualifiers);

   if (properties != NULL)
	addXmlPropertyListParam(sb, properties);

   /* Add the instance classname */
   addXmlObjectName(sb, cop, "InstanceName");

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");

   addXmlFooter(sb);

   error = con->ft->addPayload(con, sb);

   if (error || (error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      free(error);
      CMRelease(sb);
      return NULL;
   }

   CMRelease(sb);

   rh = scanCimXmlResponse(CMGetCharPtr(con->mResponse), cop);

   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      free(rh.description);
      CMRelease(rh.rvArray);
      return NULL;
   }

#if DEBUG
#ifdef CHECK_RETURN_TYPES
    if (CMGetArrayCount(rh.rvArray,NULL)==0 ||
	rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) != CMPI_instance) {
	/* Should not happen in production builds */
	CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED,
				 strdup("Unexpected return value"));
	return NULL;
    }
#else
   if (do_debug)
       printf ("\treturn type %d expected CMPI_instance %d\n",
	    rh.rvArray->ft->getSimpleType(rh.rvArray, NULL), CMPI_instance);
#endif
#endif

   CMSetStatus(rc,CMPI_RC_OK);
   cci = rh.rvArray->ft->getElementAt(rh.rvArray, 0, NULL).value.inst;
   cci = CMClone(cci,NULL);
   CMRelease(rh.rvArray);
   return cci;
}

/* --------------------------------------------------------------------------*/

/* Finished & working */
static CMPIObjectPath * createInstance(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	CMPIInstance * inst,
	CMPIStatus * rc)
{
   ClientEnc        *cl  = (ClientEnc*)mb;
   CMCIConnection   *con = cl->connection;
   UtilStringBuffer *sb  = newStringBuffer(2048);
   char             *error;
   ResponseHdr	    rh;
   int		    i, numproperties = inst->ft->getPropertyCount(inst, NULL);
   CMPIString	    *classname, *propertyname;
   CMPIData	    propertydata;
   char             *cv;

#if DEBUG
   set_debug();
#endif

   con->ft->genRequest(cl, "CreateInstance", cop, 0);

   /* Construct the CIM-XML request */
   addXmlHeader(sb);

   sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"CreateInstance\">");

   addXmlNamespace(sb, cop);

   classname = cop->ft->getClassName(cop, NULL);
   sb->ft->append3Chars(sb, "<IPARAMVALUE NAME=\"NewInstance\">\n"
			    "<INSTANCE CLASSNAME=\"",
			    (char *)classname->hdl, "\">\n");
   CMRelease(classname);

   /* Add all the instance properties */
   for (i=0; i<numproperties; i++) {
      propertydata = inst->ft->getPropertyAt(inst, i, &propertyname, NULL);
      addXmlProperty(sb, propertyname->hdl, cv=value2Chars(propertydata.type,&(propertydata.value)));
      if (cv) free(cv);
      if (propertyname) CMRelease(propertyname);
   }

   sb->ft->appendChars(sb,"</INSTANCE>\n</IPARAMVALUE>\n");
   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

   error = con->ft->addPayload(con,sb);

   if (error || (error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      free(error);
      CMRelease(sb);
      return NULL;
   }

   CMRelease(sb);

   rh = scanCimXmlResponse(CMGetCharPtr(con->mResponse), cop);
   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      free(rh.description);
      CMRelease(rh.rvArray);
      return NULL;
   }

#if DEBUG
#ifdef CHECK_RETURN_TYPES
    if (CMGetArrayCount(rh.rvArray,NULL)==0 ||
	rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) != CMPI_ref) {
	/* Should not happen in production builds */
	CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED,
				 strdup("Unexpected return value"));
	return NULL;
    }
#else
   if (do_debug)
       printf ("\treturn type %d expected CMPI_ref %d\n",
	    rh.rvArray->ft->getSimpleType(rh.rvArray, NULL), CMPI_ref);
#endif
#endif

   CMSetStatus(rc,CMPI_RC_OK);
   return rh.rvArray->ft->getElementAt(rh.rvArray, 0, NULL).value.ref;
}

/* --------------------------------------------------------------------------*/

/* Finished & working */
static CMPIStatus setInstance(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	CMPIInstance * inst,
	CMPIFlags flags,
	char ** properties)
/*
<?xml version="1.0" encoding="utf-8"?>
<CIM CIMVERSION="2.0" DTDVERSION="2.0">
  <MESSAGE ID="4711" PROTOCOLVERSION="1.0">
    <SIMPLEREQ>
      <IMETHODCALL NAME="ModifyInstance">
        <LOCALNAMESPACEPATH>
          <NAMESPACE NAME="root"/>
          <NAMESPACE NAME="cimv2"/>
        </LOCALNAMESPACEPATH>
        <IPARAMVALUE NAME="ModifiedInstance">
          <VALUE.NAMEDINSTANCE>
            <INSTANCENAME CLASSNAME="CWS_Authorization">
              <KEYBINDING NAME="Username">
                <KEYVALUE VALUETYPE="string">foo</KEYVALUE>
              </KEYBINDING>
              <KEYBINDING NAME="Classname">
                <KEYVALUE VALUETYPE="string">bar</KEYVALUE>
              </KEYBINDING>
            </INSTANCENAME>
            <INSTANCE CLASSNAME="CWS_Authorization">
              <PROPERTY NAME="Username" TYPE="string">
                <VALUE>foo</VALUE>
              </PROPERTY>
              <PROPERTY NAME="Classname" TYPE="string">
                <VALUE>bar</VALUE>
              </PROPERTY>
              <PROPERTY NAME="Enumerate" TYPE="boolean">
                <VALUE>FALSE</VALUE>
              </PROPERTY>
              <PROPERTY NAME="Get" TYPE="boolean">
                <VALUE>FALSE</VALUE>
              </PROPERTY>
              <PROPERTY NAME="Set" TYPE="boolean">
                <VALUE>FALSE</VALUE>
              </PROPERTY>
              <PROPERTY NAME="Create" TYPE="boolean">
                <VALUE>FALSE</VALUE>
              </PROPERTY>
              <PROPERTY NAME="Delete" TYPE="boolean">
                <VALUE>FALSE</VALUE>
              </PROPERTY>
              <PROPERTY NAME="Query" TYPE="boolean">
                <VALUE>true</VALUE>
              </PROPERTY>
            </INSTANCE>
          </VALUE.NAMEDINSTANCE>
        </IPARAMVALUE>
      </IMETHODCALL>
    </SIMPLEREQ>
  </MESSAGE>
</CIM>
*/
{
   ClientEnc        *cl  = (ClientEnc*)mb;
   CMCIConnection   *con = cl->connection;
   UtilStringBuffer *sb  = newStringBuffer(2048);
   char             *error;
   ResponseHdr      rh;
   int		    i, numproperties = inst->ft->getPropertyCount(inst, NULL);
   CMPIString	    * propertyname;
   CMPIString	    * cn;
   CMPIData	    propertydata;
   CMPIStatus	    rc;
   char             *cv;

#if DEBUG
   set_debug();
#endif

   con->ft->genRequest(cl, "ModifyInstance", cop, 0);

   /* Construct the CIM-XML request */
   addXmlHeader(sb);
   sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"ModifyInstance\">");

   addXmlNamespace(sb, cop);

   /* NOTE - IncludeQualifiers flag is now deprecated */
   emitqual(sb,flags & CMPI_FLAG_IncludeQualifiers);

   /* Add the property filter list */
   if (properties != NULL)
	addXmlPropertyListParam(sb, properties);

   sb->ft->appendChars(sb, "<IPARAMVALUE NAME=\"ModifiedInstance\">\n");
   sb->ft->appendChars(sb, "<VALUE.NAMEDINSTANCE>\n");

   /* Add the objectpath */
   cn = cop->ft->getClassName(cop,NULL);
   sb->ft->append3Chars(sb, "<INSTANCENAME CLASSNAME=\"",
			    (char *)cn->hdl, "\">\n");
   pathToXml(sb, cop);
   sb->ft->appendChars(sb,"</INSTANCENAME>\n");

   /* Add the instance */
   sb->ft->append3Chars(sb,"<INSTANCE CLASSNAME=\"", (char*)cn->hdl, "\">\n");
   CMRelease(cn);

   for (i = 0; i < numproperties; i++) {
      propertydata = inst->ft->getPropertyAt(inst, i, &propertyname, NULL);
      addXmlProperty(sb, propertyname->hdl,
			 cv=value2Chars(propertydata.type,&(propertydata.value)));
      if(cv) free(cv);
      if(propertyname) CMRelease(propertyname);
   }
   sb->ft->appendChars(sb,"</INSTANCE>\n");

   sb->ft->appendChars(sb,"</VALUE.NAMEDINSTANCE>\n");
   sb->ft->appendChars(sb,"</IPARAMVALUE>\n");
   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

   error = con->ft->addPayload(con,sb);

   if (error || (error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(&rc,CMPI_RC_ERR_FAILED,error);
      free(error);
      CMRelease(sb);
      return rc;
   }

   CMRelease(sb);

   rh = scanCimXmlResponse(CMGetCharPtr(con->mResponse), cop);
   if (rh.errCode != 0) {
      CMSetStatusWithChars(&rc, rh.errCode, rh.description);
      free(rh.description);
      CMRelease(rh.rvArray);
      return rc;
   }

   CMSetStatus(&rc, CMPI_RC_OK);
   return rc;
}

/* --------------------------------------------------------------------------*/

/* Finished & working */
static CMPIStatus deleteInstance(
	CMCIClient * mb,
	CMPIObjectPath * cop)
/*
<?xml version="1.0" encoding="utf-8"?>
<CIM CIMVERSION="2.0" DTDVERSION="2.0">
  <MESSAGE ID="4711" PROTOCOLVERSION="1.0">
    <SIMPLEREQ>
      <IMETHODCALL NAME="DeleteInstance">
        <LOCALNAMESPACEPATH>
          <NAMESPACE NAME="root"/>
          <NAMESPACE NAME="cimv2"/>
        </LOCALNAMESPACEPATH>
        <IPARAMVALUE NAME="InstanceName">
          <INSTANCENAME CLASSNAME="CWS_Authorization">
            <KEYBINDING NAME="Username">
              <KEYVALUE VALUETYPE="string">bestorga</KEYVALUE>
            </KEYBINDING>
            <KEYBINDING NAME="Classname">
              <KEYVALUE VALUETYPE="string">foobar</KEYVALUE>
            </KEYBINDING>
          </INSTANCENAME>
        </IPARAMVALUE>
      </IMETHODCALL>
    </SIMPLEREQ>
  </MESSAGE>
</CIM>
*/
{
   ClientEnc		*cl = (ClientEnc*)mb;
   CMCIConnection	*con = cl->connection;
   UtilStringBuffer	*sb = newStringBuffer(2048);
   char			*error;
   ResponseHdr		rh;
   CMPIString		*classname;
   CMPIStatus		rc;

#if DEBUG
   set_debug();
#endif

   con->ft->genRequest(cl, "DeleteInstance", cop, 0);

   addXmlHeader(sb);
   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"DeleteInstance\">");

   addXmlNamespace(sb, cop);

   /* Add instance stuff */
   classname = cop->ft->getClassName(cop, NULL);
   sb->ft->append3Chars(sb, "<IPARAMVALUE NAME=\"InstanceName\">\n"
			    "<INSTANCENAME CLASSNAME=\"",
			    (char*)classname->hdl,"\">\n");
   CMRelease(classname);

   pathToXml(sb, cop);

   sb->ft->appendChars(sb,"</INSTANCENAME>\n");
   sb->ft->appendChars(sb,"</IPARAMVALUE>\n");
   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

   error = con->ft->addPayload(con,sb);

   if (error || (error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(&rc,CMPI_RC_ERR_FAILED,error);
      free(error);
      CMRelease(sb);
      return rc;
   }

   CMRelease(sb);

   rh = scanCimXmlResponse(CMGetCharPtr(con->mResponse), cop);
   if (rh.errCode != 0) {
      CMSetStatusWithChars(&rc, rh.errCode, rh.description);
      free(rh.description);
      CMRelease(rh.rvArray);
      return rc;
   }

   CMSetStatus(&rc, CMPI_RC_OK);
   return rc;
}

/* --------------------------------------------------------------------------*/

/* finish but not working - scanCimXmlResponse fails parsing results */
static CMPIEnumeration * execQuery(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	const char * query,
	const char * lang,
	CMPIStatus * rc)
/*
<?xml version="1.0" encoding="utf-8"?>
<CIM CIMVERSION="2.0" DTDVERSION="2.0">
  <MESSAGE ID="4711" PROTOCOLVERSION="1.0">
    <SIMPLEREQ>
      <IMETHODCALL NAME="ExecQuery">
        <LOCALNAMESPACEPATH>
          <NAMESPACE NAME="root"/>
          <NAMESPACE NAME="cimv2"/>
        </LOCALNAMESPACEPATH>
        <IPARAMVALUE NAME="QueryLanguage">
          <VALUE>WQL</VALUE>
        </IPARAMVALUE>
        <IPARAMVALUE NAME="Query">
          <VALUE>select * from Linux_ComputerSystem where PrimaryOwnerName="root"</VALUE>
        </IPARAMVALUE>
      </IMETHODCALL>
    </SIMPLEREQ>
  </MESSAGE>
</CIM>
*/
{
   ClientEnc        *cl  = (ClientEnc *)mb;
   CMCIConnection   *con = cl->connection;
   UtilStringBuffer *sb  = newStringBuffer(2048);
   char             *error;
   ResponseHdr      rh;

#if DEBUG
   set_debug();
#endif

   con->ft->genRequest(cl, "ExecQuery", cop, 0);

   addXmlHeader(sb);

   sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"ExecQuery\">");

   addXmlNamespace(sb, cop);

   /* Add the query language */
   sb->ft->append3Chars(sb,
        "<IPARAMVALUE NAME=\"QueryLanguage\">\n<VALUE>",
        lang, "</VALUE>\n</IPARAMVALUE>");

   /* Add the query */
   sb->ft->append3Chars(sb,
        "<IPARAMVALUE NAME=\"Query\">\n<VALUE>",
        query, "</VALUE>\n</IPARAMVALUE>");

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

   error = con->ft->addPayload(con,sb);

   if (error || (error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      free(error);
      CMRelease(sb);
      return NULL;
   }

   CMRelease(sb);

   rh = scanCimXmlResponse(CMGetCharPtr(con->mResponse), cop);
   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      free(rh.description);
      CMRelease(rh.rvArray);
      return NULL;
   }

#if DEBUG
#ifdef CHECK_RETURN_TYPES
    if (CMGetArrayCount(rh.rvArray,NULL)==0 ||
	rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) != CMPI_instance) {
	/* Should not happen in production builds */
	CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED,
				 strdup("Unexpected return value"));
	return NULL;
    }
#else
   if (do_debug)
       printf ("\treturn enumeration array type %d expected CMPI_instance %d\n",
	    rh.rvArray->ft->getSimpleType(rh.rvArray, NULL), CMPI_instance);
#endif
#endif

   CMSetStatus(rc, CMPI_RC_OK);
   return newCMPIEnumeration(rh.rvArray,NULL);
}

/* --------------------------------------------------------------------------*/

/* Finished and working */
static CMPIEnumeration * enumInstances(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	CMPIFlags flags,
	char ** properties,
	CMPIStatus * rc)
{
    ClientEnc	     *cl  = (ClientEnc *)mb;
    CMCIConnection   *con = cl->connection;
    UtilStringBuffer *sb  = newStringBuffer(2048);
    char             *error;
    ResponseHdr	     rh;

#if DEBUG
    set_debug();
#endif

    con->ft->genRequest(cl, "EnumerateInstances", cop, 0);

    addXmlHeader(sb);

    sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"EnumerateInstances\">");
    addXmlNamespace(sb, cop);

    addXmlClassnameParam(sb, cop);

    emitdeep(sb,flags & CMPI_FLAG_DeepInheritance);
    emitlocal(sb,flags & CMPI_FLAG_LocalOnly);
    emitqual(sb,flags & CMPI_FLAG_IncludeQualifiers);
    emitorigin(sb,flags & CMPI_FLAG_IncludeClassOrigin);

    if (properties != NULL)
	addXmlPropertyListParam(sb, properties);

    sb->ft->appendChars(sb,"</IMETHODCALL>\n");
    addXmlFooter(sb);

    error = con->ft->addPayload(con,sb);

    if (error || (error = con->ft->getResponse(con, cop))) {
        CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
	free(error);
        return NULL;
    }

    CMRelease(sb);

    rh = scanCimXmlResponse(CMGetCharPtr(con->mResponse), cop);

    if (rh.errCode != 0) {
        CMSetStatusWithChars(rc, rh.errCode, rh.description);
	free(rh.description);
	CMRelease(rh.rvArray);
        return NULL;
    }

#if DEBUG
#ifdef CHECK_RETURN_VALUES
    if (CMGetArrayCount(rh.rvArray,NULL)==0 ||
	rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) != CMPI_instance) {
	/* Should not happen in production builds */
	CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED,
				 strdup("Unexpected return value"));
	return NULL;
    }
#else
   if (do_debug)
       printf ("\treturn enumeration array type %d expected CMPI_instance %d\n",
	    rh.rvArray->ft->getSimpleType(rh.rvArray, NULL), CMPI_instance);
#endif
#endif

    CMSetStatus(rc, CMPI_RC_OK);
    return newCMPIEnumeration(rh.rvArray,NULL);
}

/* --------------------------------------------------------------------------*/

/* finished and working */
static CMPIEnumeration * associators(
	CMCIClient	* mb,
	CMPIObjectPath	* cop,
	const char	* assocClass,
	const char	* resultClass,
	const char	* role,
	const char	* resultRole,
	CMPIFlags	flags,
	char		** properties,
	CMPIStatus	* rc)
/*
<?xml version="1.0" encoding="utf-8"?>
<CIM CIMVERSION="2.0" DTDVERSION="2.0">
  <MESSAGE ID="4711" PROTOCOLVERSION="1.0">
    <SIMPLEREQ>
      <IMETHODCALL NAME="Associators">
        <LOCALNAMESPACEPATH>
          <NAMESPACE NAME="root"/>
          <NAMESPACE NAME="cimv2"/>
        </LOCALNAMESPACEPATH>
        <IPARAMVALUE NAME="ObjectName">
          <INSTANCENAME CLASSNAME="Linux_ComputerSystem">
            <KEYBINDING NAME="CreationClassName">
              <KEYVALUE VALUETYPE="string">Linux_ComputerSystem</KEYVALUE>
            </KEYBINDING>
            <KEYBINDING NAME="Name">
              <KEYVALUE VALUETYPE="string">bestorga.ibm.com</KEYVALUE>
            </KEYBINDING>
          </INSTANCENAME>
        </IPARAMVALUE>
        <IPARAMVALUE NAME="AssocClass">
          <CLASSNAME NAME="Linux_RunningOS"/>
        </IPARAMVALUE>
        <IPARAMVALUE NAME="IncludeQualifiers">
          <VALUE>FALSE</VALUE>
        </IPARAMVALUE>
        <IPARAMVALUE NAME="IncludeClassOrigin">
          <VALUE>TRUE</VALUE>
        </IPARAMVALUE>
      </IMETHODCALL>
    </SIMPLEREQ>
  </MESSAGE>
</CIM>
*/
{
   ClientEnc		*cl = (ClientEnc*)mb;
   CMCIConnection	*con = cl->connection;
   UtilStringBuffer	*sb = newStringBuffer(2048);
   char			*error;

#if DEBUG
   set_debug();
#endif

   con->ft->genRequest(cl, "Associators", cop, 0);
   addXmlHeader(sb);

   sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"Associators\">");

   addXmlNamespace(sb, cop);

   /* TODO: Need to switch from ObjName to InstanceName? */
   addXmlObjectName(sb, cop, "ObjectName");

   /* Add optional parameters */
   if (assocClass!=NULL)
      sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"AssocClass\"><CLASSNAME NAME=\"", assocClass,
		  "\"/></IPARAMVALUE>\n");

   if (resultClass!=NULL)
      sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"ResultClass\"><CLASSNAME NAME=\"", resultClass,
		  "\"/></IPARAMVALUE>\n");

   if (role)
      sb->ft->append3Chars(sb, "<IPARAMVALUE NAME=\"Role\"><VALUE>", role,
		 "</VALUE></IPARAMVALUE>\n");

   if (resultRole)
      sb->ft->append3Chars(sb, "<IPARAMVALUE NAME=\"ResultRole\"><VALUE>", resultRole,
		 "</VALUE></IPARAMVALUE>\n");

   /* Add optional flags */
   emitorigin(sb,flags & CMPI_FLAG_IncludeClassOrigin);
   emitqual(sb,flags & CMPI_FLAG_IncludeQualifiers);

   /* Add property list filter */
   if (properties != NULL)
	addXmlPropertyListParam(sb, properties);

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

   error = con->ft->addPayload(con, sb);

   if (error || (error = con->ft->getResponse(con,cop))) {
      CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED, error);
      free(error);
      CMRelease(sb);
      return NULL;
   }

   CMRelease(sb);

   ResponseHdr rh=scanCimXmlResponse(CMGetCharPtr(con->mResponse),cop);

   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      free(rh.description);
      CMRelease(rh.rvArray);
      return NULL;
   }

#if DEBUG
#ifdef CHECK_RETURN_VALUES
    if (CMGetArrayCount(rh.rvArray,NULL)==0 ||
	rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) != CMPI_instance) {
	/* Should not happen in production builds */
	CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED,
				 strdup("Unexpected return value"));
	return NULL;
    }
#else
   if (do_debug)
       printf ("\treturn enumeration array type %d expected CMPI_instance %d\n",
	    rh.rvArray->ft->getSimpleType(rh.rvArray, NULL), CMPI_instance);
#endif
#endif

   CMSetStatus(rc, CMPI_RC_OK);
   return newCMPIEnumeration(rh.rvArray, NULL);
}

/* --------------------------------------------------------------------------*/

/* finished & working */
static CMPIEnumeration * associatorNames(
	CMCIClient	* mb,
	CMPIObjectPath	* cop,
	const char	* assocClass,
	const char	* resultClass,
	const char	* role,
	const char	* resultRole,
	CMPIStatus	* rc)
/*
<?xml version="1.0" encoding="utf-8"?>
<CIM CIMVERSION="2.0" DTDVERSION="2.0">
  <MESSAGE ID="4711" PROTOCOLVERSION="1.0">
    <SIMPLEREQ>
      <IMETHODCALL NAME="AssociatorNames">
	<LOCALNAMESPACEPATH>
	  <NAMESPACE NAME="root"/>
	  <NAMESPACE NAME="cimv2"/>
	</LOCALNAMESPACEPATH>
	<IPARAMVALUE NAME="ObjectName">
	  <INSTANCENAME CLASSNAME="Linux_ComputerSystem">
	    <KEYBINDING NAME="CreationClassName">
	      <KEYVALUE VALUETYPE="string">Linux_ComputerSystem</KEYVALUE>
	    </KEYBINDING>
	    <KEYBINDING NAME="Name">
	      <KEYVALUE VALUETYPE="string">bestorga.ibm.com</KEYVALUE>
	    </KEYBINDING>
	  </INSTANCENAME>
	</IPARAMVALUE>
	<IPARAMVALUE NAME="AssocClass">
	  <CLASSNAME NAME="Linux_RunningOS"/>
	</IPARAMVALUE>
      </IMETHODCALL>
    </SIMPLEREQ>
  </MESSAGE>
</CIM>
*/
{
   ClientEnc		*cl = (ClientEnc*)mb;
   CMCIConnection	*con = cl->connection;
   UtilStringBuffer	*sb = newStringBuffer(2048);
   char			*error;

#if DEBUG
   set_debug();
#endif

   con->ft->genRequest(cl, "AssociatorNames", cop, 0);
   addXmlHeader(sb);

   sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"AssociatorNames\">");

   addXmlNamespace(sb, cop);

   addXmlObjectName(sb, cop, "ObjectName");

   /* Add optional parameters */
   if (assocClass!=NULL)
      sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"AssocClass\"><CLASSNAME NAME=\"", assocClass,
          "\"/></IPARAMVALUE>\n");

   if (resultClass!=NULL)
      sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"ResultClass\"><CLASSNAME NAME=\"", resultClass,
          "\"/></IPARAMVALUE>\n");

   if (role)
      sb->ft->append3Chars(sb, "<IPARAMVALUE NAME=\"Role\"><VALUE>", role,
         "</VALUE></IPARAMVALUE>\n");

   if (resultRole)
      sb->ft->append3Chars(sb, "<IPARAMVALUE NAME=\"ResultRole\"><VALUE>", resultRole,
         "</VALUE></IPARAMVALUE>\n");

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

   error = con->ft->addPayload(con,sb);

   if (error || (error=con->ft->getResponse(con,cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      free(error);
      CMRelease(sb);
      return NULL;
   }

   CMRelease(sb);

   ResponseHdr rh=scanCimXmlResponse(CMGetCharPtr(con->mResponse),cop);

   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      free(rh.description);
      CMRelease(rh.rvArray);
      return NULL;
   }

#if DEBUG
#ifdef CHECK_RETURN_TYPES
    if (CMGetArrayCount(rh.rvArray,NULL)==0 ||
	rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) != CMPI_ref) {
	/* Should not happen in production builds */
	CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED,
				 strdup("Unexpected return value"));
	return NULL;
    }
#else
   if (do_debug)
      printf ("\treturn enumeration array type %d expected CMPI_ref %d\n",
	    rh.rvArray->ft->getSimpleType(rh.rvArray, NULL), CMPI_ref);
#endif
#endif

   CMSetStatus(rc, CMPI_RC_OK);
   return newCMPIEnumeration(rh.rvArray, NULL);
}

/* --------------------------------------------------------------------------*/

/* finished (and working?) test against Pegasus / bug in sfcb prevents rin working */
static CMPIEnumeration * references(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	const char * resultClass,
	const char * role ,
	CMPIFlags flags,
	char ** properties,
	CMPIStatus * rc)
{
   ClientEnc		*cl = (ClientEnc*)mb;
   CMCIConnection	*con = cl->connection;
   UtilStringBuffer	*sb = newStringBuffer(2048);
   char			*error;

#if DEBUG
   set_debug();
#endif

   con->ft->genRequest(cl, "References", cop, 0);
   addXmlHeader(sb);

   sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"References\">");

   addXmlNamespace(sb, cop);

   addXmlObjectName(sb, cop, "ObjectName");

   /* Add optional parameters */
   if (resultClass)
      sb->ft->append3Chars(sb, "<IPARAMVALUE NAME=\"ResultClass\"><VALUE>", resultClass,
         "</VALUE></IPARAMVALUE>\n");

   if (role)
      sb->ft->append3Chars(sb, "<IPARAMVALUE NAME=\"Role\"><VALUE>", role,
         "</VALUE></IPARAMVALUE>\n");

   /* Add optional flags */
   emitorigin(sb,flags & CMPI_FLAG_IncludeClassOrigin);
   emitqual(sb,flags & CMPI_FLAG_IncludeQualifiers);

   /* Add property list filter */
   if (properties != NULL)
	addXmlPropertyListParam(sb, properties);

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

   error = con->ft->addPayload(con, sb);

   if (error || (error=con->ft->getResponse(con,cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      free(error);
      return NULL;
   }

   CMRelease(sb);

   ResponseHdr rh=scanCimXmlResponse(CMGetCharPtr(con->mResponse),cop);

   if (rh.errCode!=0) {
      CMSetStatusWithChars(rc,rh.errCode,rh.description);
      free(rh.description);
      CMRelease(rh.rvArray);
      return NULL;
   }

#if DEBUG
#ifdef CHECK_RETURN_TYPES
    if (CMGetArrayCount(rh.rvArray,NULL)==0 ||
	rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) != CMPI_instance) {
	/* Should not happen in production builds */
	CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED,
				 strdup("Unexpected return value"));
	return NULL;
    }
#else
   if (do_debug)
       printf ("\treturn enumeration array type %d expected CMPI_instance %d\n",
	    rh.rvArray->ft->getSimpleType(rh.rvArray, NULL), CMPI_instance);
#endif
#endif

   CMSetStatus(rc,CMPI_RC_OK);
   return newCMPIEnumeration(rh.rvArray,NULL);
}

/* --------------------------------------------------------------------------*/

/* finished (and working?) test against Pegasus / bug in sfcb prevents rin working */
static CMPIEnumeration * referenceNames(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	const char * resultClass,
	const char * role,
	CMPIStatus * rc)
{
   ClientEnc		*cl = (ClientEnc*)mb;
   CMCIConnection	*con = cl->connection;
   UtilStringBuffer	*sb = newStringBuffer(2048);
   char			*error;

#if DEBUG
   set_debug();
#endif

   con->ft->genRequest(cl, "ReferenceNames", cop, 0);
   addXmlHeader(sb);

   sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"ReferenceNames\">");

   addXmlNamespace(sb, cop);

   addXmlObjectName(sb, cop, "ObjectName");

   if (resultClass!=NULL)
      sb->ft->append3Chars(sb,
	      "<IPARAMVALUE NAME=\"ResultClass\"><CLASSNAME NAME=\"",
	      resultClass, "\"/></IPARAMVALUE>\n");
   if (role)
      sb->ft->append3Chars(sb, "<IPARAMVALUE NAME=\"Role\"><VALUE>",
			       role, "</VALUE></IPARAMVALUE>\n");

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

   error = con->ft->addPayload(con, sb);

   if (error || (error=con->ft->getResponse(con,cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      free(error);
      CMRelease(sb);
      return NULL;
   }

   CMRelease(sb);

   ResponseHdr rh=scanCimXmlResponse(CMGetCharPtr(con->mResponse),cop);

   if (rh.errCode!=0) {
      CMSetStatusWithChars(rc,rh.errCode,rh.description);
      free(rh.description);
      CMRelease(rh.rvArray);
      return NULL;
   }

#if DEBUG
#ifdef CHECK_RETURN_TYPES
    if (CMGetArrayCount(rh.rvArray,NULL)==0 ||
	rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) != CMPI_ref) {
	/* Should not happen in production builds */
	CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED,
				 strdup("Unexpected return value"));
	return NULL;
    }
#else
   if (do_debug)
       printf ("\treturn enumeration array type %d expected CMPI_ref %d\n",
	    rh.rvArray->ft->getSimpleType(rh.rvArray, NULL), CMPI_ref);
#endif
#endif

   CMSetStatus(rc,CMPI_RC_OK);
   return newCMPIEnumeration(rh.rvArray,NULL);
}

/* --------------------------------------------------------------------------*/

/* almost finished (need to add output arg support) but scanCimXmlResponse segv's parsing results */
static CMPIData invokeMethod(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	const char * method,
	CMPIArgs * in,
	CMPIArgs * out,
	CMPIStatus * rc)
/*
<?xml version="1.0" encoding="utf-8"?>
<CIM CIMVERSION="2.0" DTDVERSION="2.0">
  <MESSAGE ID="4711" PROTOCOLVERSION="1.0">
    <SIMPLEREQ>
      <METHODCALL NAME="IsAuthorized">
        <LOCALINSTANCEPATH>
          <LOCALNAMESPACEPATH>
            <NAMESPACE NAME="root"/>
            <NAMESPACE NAME="cimv2"/>
          </LOCALNAMESPACEPATH>
          <INSTANCENAME CLASSNAME="CWS_Authorization">
            <KEYBINDING NAME="Username">
              <KEYVALUE VALUETYPE="string">schuur</KEYVALUE>
            </KEYBINDING>
            <KEYBINDING NAME="Classname">
              <KEYVALUE VALUETYPE="string">CIM_ComputerSystem</KEYVALUE>
            </KEYBINDING>
          </INSTANCENAME>
        </LOCALINSTANCEPATH>
        <PARAMVALUE NAME="operation">
          <VALUE>Query</VALUE>
        </PARAMVALUE>
      </METHODCALL>
    </SIMPLEREQ>
  </MESSAGE>
</CIM>
*/
{
   ClientEnc		*cl = (ClientEnc*)mb;
   CMCIConnection	*con = cl->connection;
   UtilStringBuffer	*sb=newStringBuffer(2048);
   char			*error;
   ResponseHdr		rh;
   CMPIString		*cn;
   CMPIData		retval;
   int			i, numinargs = 0, numoutargs = 0;
   char                 *cv;

#if DEBUG
   set_debug();
#endif

   if (in)
      numinargs = in->ft->getArgCount(in, NULL);
   if (out)
      numoutargs = out->ft->getArgCount(out, NULL);

   con->ft->genRequest(cl, (const char *)method, cop, 1);

   addXmlHeader(sb);

   /* Add the extrinsic method name */
   sb->ft->append3Chars(sb,"<METHODCALL NAME=\"", method, "\">");

   sb->ft->appendChars(sb,"<LOCALINSTANCEPATH>");

   addXmlNamespace(sb, cop);

   /* Add the objectpath */
   cn = cop->ft->getClassName(cop,NULL);
   sb->ft->append3Chars(sb, "<INSTANCENAME CLASSNAME=\"",
					  (char*)cn->hdl,"\">\n");
   pathToXml(sb, cop);
   sb->ft->appendChars(sb,"</INSTANCENAME>\n");
   CMRelease(cn);

   sb->ft->appendChars(sb,"</LOCALINSTANCEPATH>");

   /* Add the input parameters */
   for (i = 0; i < numinargs; i++) {
      CMPIString * argname;
      CMPIData argdata = in->ft->getArgAt(in, i, &argname, NULL);
      sb->ft->append3Chars(sb,"<PARAMVALUE NAME=\"", argname->hdl, "\">\n");
      sb->ft->append3Chars(sb,"<VALUE>", cv=value2Chars(argdata.type,&(argdata.value)), "</VALUE>\n");
      sb->ft->appendChars(sb,"</PARAMVALUE>\n");
      if(cv) free(cv);
      if(argname) CMRelease(argname);
   }

   sb->ft->appendChars(sb,"</METHODCALL>\n");
   addXmlFooter(sb);

   error = con->ft->addPayload(con,sb);

   if (error || (error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      free(error);
      retval.state = CMPI_notFound | CMPI_nullValue;
      CMRelease(sb);
      return retval;
   }

   CMRelease(sb);

   rh = scanCimXmlResponse(CMGetCharPtr(con->mResponse),cop);

   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      free(rh.description);
      CMRelease(rh.rvArray);
      retval.state = CMPI_notFound | CMPI_nullValue;
      return retval;
   }

   /* TODO: process output args */
#if DEBUG
   if (do_debug && numoutargs > 0)
	printf("TODO: invokeMethod() with output args\n");
#endif

   /* Set good status and return */
#if DEBUG
   if (do_debug)
       printf ("\treturn type %d\n",
		 rh.rvArray->ft->getSimpleType(rh.rvArray, NULL));
#endif

   CMSetStatus(rc, CMPI_RC_OK);
   retval=rh.rvArray->ft->getElementAt(rh.rvArray, 0, NULL);
   retval.value=native_clone_CMPIValue(rh.rvArray->ft->getSimpleType(rh.rvArray, NULL),&retval.value,NULL);
   CMRelease(rh.rvArray);
   return retval;
}

/* --------------------------------------------------------------------------*/

/* finished and working */
static CMPIStatus setProperty(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	const char * name,
	CMPIValue * value,
	CMPIType type)
/*
<?xml version="1.0" encoding="utf-8"?>
<CIM CIMVERSION="2.0" DTDVERSION="2.0">
  <MESSAGE ID="4711" PROTOCOLVERSION="1.0">
    <SIMPLEREQ>
      <IMETHODCALL NAME="SetProperty">
        <LOCALNAMESPACEPATH>
          <NAMESPACE NAME="root"/>
          <NAMESPACE NAME="cimv2"/>
        </LOCALNAMESPACEPATH>
        <IPARAMVALUE NAME="PropertyName">
          <VALUE>CurrentTimeZone</VALUE>
        </IPARAMVALUE>
        <IPARAMVALUE NAME="NewValue">
          <VALUE>123</VALUE>
        </IPARAMVALUE>
        <IPARAMVALUE NAME="InstanceName">
          <INSTANCENAME CLASSNAME="Linux_OperatingSystem">
            <KEYBINDING NAME="CSCreationClassName">
              <KEYVALUE VALUETYPE="string">Linux_ComputerSystem</KEYVALUE>
            </KEYBINDING>
            <KEYBINDING NAME="CSName">
              <KEYVALUE VALUETYPE="string">bestorga.ibm.com</KEYVALUE>
            </KEYBINDING>
            <KEYBINDING NAME="CreationClassName">
              <KEYVALUE VALUETYPE="string">Linux_OperatingSystem</KEYVALUE>
            </KEYBINDING>
            <KEYBINDING NAME="Name">
              <KEYVALUE VALUETYPE="string">bestorga.ibm.com</KEYVALUE>
            </KEYBINDING>
          </INSTANCENAME>
        </IPARAMVALUE>
      </IMETHODCALL>
    </SIMPLEREQ>
  </MESSAGE>
</CIM>
*/
{
   ClientEnc        *cl = (ClientEnc*)mb;
   CMCIConnection   *con = cl->connection;
   UtilStringBuffer *sb=newStringBuffer(2048);
   char		    *error;
   ResponseHdr      rh;
   CMPIString	    *cn;
   CMPIStatus	    rc = {CMPI_RC_OK, NULL};
   char             *cv;

#if DEBUG
   set_debug();
#endif

   con->ft->genRequest(cl, "SetProperty", cop, 0);

   addXmlHeader(sb);
   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"SetProperty\">");

   addXmlNamespace(sb, cop);

   /* Add the property */
   sb->ft->append3Chars(sb,
        "<IPARAMVALUE NAME=\"PropertyName\">\n<VALUE>",
        name, "</VALUE>\n</IPARAMVALUE>");

   /* Add the new value */
   sb->ft->append3Chars(sb,
        "<IPARAMVALUE NAME=\"NewValue\">\n<VALUE>",
        cv=value2Chars(type,value), "</VALUE>\n</IPARAMVALUE>");
   if(cv) free(cv);

   /* Add the objectpath */
   cn = cop->ft->getClassName(cop,NULL);
   sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"InstanceName\">\n"
          "<INSTANCENAME CLASSNAME=\"",(char*)cn->hdl,"\">\n");
   pathToXml(sb, cop);
   sb->ft->appendChars(sb,"</INSTANCENAME>\n</IPARAMVALUE>\n");
   CMRelease(cn);

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

   error = con->ft->addPayload(con,sb);

   if (error || (error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(&rc,CMPI_RC_ERR_FAILED,error);
      free(error);
      CMRelease(sb);
      return rc;
   }

   CMRelease(sb);

   rh = scanCimXmlResponse(CMGetCharPtr(con->mResponse),cop);

   if (rh.errCode != 0) {
      CMSetStatusWithChars(&rc, rh.errCode, rh.description);
      free(rh.description);
   }
   
   CMRelease(rh.rvArray);
   return rc;
}

/* --------------------------------------------------------------------------*/

/* finished and working */
static CMPIData getProperty(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	const char * name,
	CMPIStatus * rc)
/*
<?xml version="1.0" encoding="utf-8"?>
<CIM CIMVERSION="2.0" DTDVERSION="2.0">
  <MESSAGE ID="4711" PROTOCOLVERSION="1.0">
    <SIMPLEREQ>
      <IMETHODCALL NAME="GetProperty">
        <LOCALNAMESPACEPATH>
          <NAMESPACE NAME="root"/>
          <NAMESPACE NAME="cimv2"/>
        </LOCALNAMESPACEPATH>
        <IPARAMVALUE NAME="PropertyName">
          <VALUE>CurrentTimeZone</VALUE>
        </IPARAMVALUE>
        <IPARAMVALUE NAME="InstanceName">
          <INSTANCENAME CLASSNAME="Linux_OperatingSystem">
            <KEYBINDING NAME="CSCreationClassName">
              <KEYVALUE VALUETYPE="string">Linux_ComputerSystem</KEYVALUE>
            </KEYBINDING>
            <KEYBINDING NAME="CSName">
              <KEYVALUE VALUETYPE="string">bestorga.ibm.com</KEYVALUE>
            </KEYBINDING>
            <KEYBINDING NAME="CreationClassName">
              <KEYVALUE VALUETYPE="string">Linux_OperatingSystem</KEYVALUE>
            </KEYBINDING>
            <KEYBINDING NAME="Name">
              <KEYVALUE VALUETYPE="string">bestorga.ibm.com</KEYVALUE>
            </KEYBINDING>
          </INSTANCENAME>
        </IPARAMVALUE>
      </IMETHODCALL>
    </SIMPLEREQ>
  </MESSAGE>
</CIM>
*/
{
   ClientEnc		*cl = (ClientEnc*)mb;
   CMCIConnection	*con = cl->connection;
   UtilStringBuffer	*sb=newStringBuffer(2048);
   char			*error;
   ResponseHdr		rh;
   CMPIString		*cn;
   CMPIData		retval;

#if DEBUG
   set_debug();
#endif

   con->ft->genRequest(cl, "GetProperty", cop, 0);

   addXmlHeader(sb);
   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"GetProperty\">");

   addXmlNamespace(sb, cop);

   /* Add the property */
   sb->ft->append3Chars(sb,
        "<IPARAMVALUE NAME=\"PropertyName\">\n<VALUE>",
        name, "</VALUE>\n</IPARAMVALUE>");

   /* Add the objectpath */
   cn = cop->ft->getClassName(cop,NULL);
   sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"InstanceName\">\n"
          "<INSTANCENAME CLASSNAME=\"",(char*)cn->hdl,"\">\n");
   pathToXml(sb, cop);
   sb->ft->appendChars(sb,"</INSTANCENAME>\n</IPARAMVALUE>\n");
   CMRelease(cn);

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

   error = con->ft->addPayload(con,sb);

   if (error || (error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      free(error);
      retval.state = CMPI_notFound | CMPI_nullValue;
      CMRelease(sb);
      return retval;
   }

   CMRelease(sb);

   rh = scanCimXmlResponse(CMGetCharPtr(con->mResponse),cop);

   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      free(rh.description);
      CMRelease(rh.rvArray);
      retval.state = CMPI_notFound | CMPI_nullValue;
      return retval;
   }

   /* Set good return status and return */
#if DEBUG
   if (do_debug)
       printf ("\treturn type %d\n",
	    rh.rvArray->ft->getSimpleType(rh.rvArray, NULL));
#endif

   CMSetStatus(rc, CMPI_RC_OK);
   retval=rh.rvArray->ft->getElementAt(rh.rvArray, 0, NULL);
   retval.value=native_clone_CMPIValue(rh.rvArray->ft->getSimpleType(rh.rvArray, NULL),&retval.value,NULL);
   CMRelease(rh.rvArray);
   return retval;
}

/* --------------------------------------------------------------------------*/

/* finished and working */
static CMPIConstClass * getClass(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	CMPIFlags flags,
	char ** properties,
	CMPIStatus * rc)
{
   CMPIConstClass *ccl, *ccc;
   ClientEnc *cl=(ClientEnc*)mb;
   CMCIConnection *con=cl->connection;
   UtilStringBuffer *sb=newStringBuffer(2048);
   char *error;

#if DEBUG
   set_debug();
#endif

   con->ft->genRequest(cl, "GetClass", cop, 0);

   addXmlHeader(sb);

   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"GetClass\">");

   addXmlNamespace(sb, cop);

   emitlocal(sb,flags & CMPI_FLAG_LocalOnly);
   emitorigin(sb,flags & CMPI_FLAG_IncludeClassOrigin);
   emitqual(sb,flags & CMPI_FLAG_IncludeQualifiers);

   if (properties != NULL)
	addXmlPropertyListParam(sb, properties);

   addXmlClassnameParam(sb, cop);

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

   error = con->ft->addPayload(con,sb);

   if (error || (error=con->ft->getResponse(con,cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      free(error);
      CMRelease(sb);
      return NULL;
   }

   CMRelease(sb);

   ResponseHdr rh=scanCimXmlResponse(CMGetCharPtr(con->mResponse),cop);

   if (rh.errCode!=0) {
      CMSetStatusWithChars(rc,rh.errCode,rh.description);
      free(rh.description);
      CMRelease(rh.rvArray);
      return NULL;
   }

#if DEBUG
#ifdef CHECK_RETURN_TYPES
    if (CMGetArrayCount(rh.rvArray,NULL)==0 ||
	rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) != CMPI_class) {
	/* Should not happen in production builds */
	CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED,
				 strdup("Unexpected return value"));
	return NULL;
    }
#else
   if (do_debug)
       printf ("\treturn enumeration array type %d expected CMPI_class %d\n",
	    rh.rvArray->ft->getSimpleType(rh.rvArray, NULL), CMPI_class);
#endif
#endif

   CMSetStatus(rc, CMPI_RC_OK);
   ccl = rh.rvArray->ft->getElementAt(rh.rvArray, 0, NULL).value.cls;
   ccc = CMClone(ccl,NULL);
   CMRelease(rh.rvArray);
   return ccc;
}

/* --------------------------------------------------------------------------*/

/* finished & working */
static CMPIEnumeration* enumClassNames(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	CMPIFlags flags,
	CMPIStatus * rc)
{
   ClientEnc *cl=(ClientEnc*)mb;
   CMCIConnection *con=cl->connection;
   UtilStringBuffer *sb=newStringBuffer(2048);
   char *error;
   ResponseHdr       rh;

#if DEBUG
   set_debug();
#endif

   con->ft->genRequest(cl, "EnumerateClassNames", cop, 0);

   /* Construct the CIM-XML request */
   addXmlHeader(sb);
   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"EnumerateClassNames\">");

   addXmlNamespace(sb, cop);
   emitdeep(sb,flags & CMPI_FLAG_DeepInheritance);
   addXmlClassnameParam(sb, cop);

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

   error = con->ft->addPayload(con,sb);

   if (error || (error = con->ft->getResponse(con,cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      free(error);
      CMRelease(sb);
      return NULL;
   }

   CMRelease(sb);

   rh=scanCimXmlResponse(CMGetCharPtr(con->mResponse),cop);

   if (rh.errCode!=0) {
      CMSetStatusWithChars(rc,rh.errCode,rh.description);
      free(rh.description);
      CMRelease(rh.rvArray);
      return NULL;
   }

#if DEBUG
#ifdef CHECK_RETURN_TYPES
    if (CMGetArrayCount(rh.rvArray,NULL)==0 ||
	rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) != CMPI_ref) {
	/* Should not happen in production builds */
	CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED,
				 strdup("Unexpected return value"));
	return NULL;
    }
#else
   if (do_debug)
       printf ("\treturn enumeration array type %d expected CMPI_ref %d\n",
	    rh.rvArray->ft->getSimpleType(rh.rvArray, NULL), CMPI_ref);
#endif
#endif

   CMSetStatus(rc,CMPI_RC_OK);
   return newCMPIEnumeration(rh.rvArray,NULL);
}

/* --------------------------------------------------------------------------*/

/* finished and working */
static CMPIEnumeration * enumClasses(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	CMPIFlags flags,
	CMPIStatus * rc)
{
   ClientEnc	     *cl  = (ClientEnc *)mb;
   CMCIConnection   *con = cl->connection;
   UtilStringBuffer *sb  = newStringBuffer(2048);
   char             *error;
   ResponseHdr	     rh;

#if DEBUG
   set_debug();
#endif

   con->ft->genRequest(cl, "EnumerateClasses", cop, 0);

   /* Construct the CIM-XML request */
   addXmlHeader(sb);
   sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"EnumerateClasses\">");

   addXmlNamespace(sb, cop);
   emitdeep(sb,flags & CMPI_FLAG_DeepInheritance);
   emitlocal(sb,flags & CMPI_FLAG_LocalOnly);
   emitqual(sb,flags & CMPI_FLAG_IncludeQualifiers);
   emitorigin(sb,flags & CMPI_FLAG_IncludeClassOrigin);
   addXmlClassnameParam(sb, cop);

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

   error = con->ft->addPayload(con,sb);

   if (error || (error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      free(error);
      CMRelease(sb);
      return NULL;
   }

   CMRelease(sb);

   rh = scanCimXmlResponse(CMGetCharPtr(con->mResponse), cop);

   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      free(rh.description);
      CMRelease(rh.rvArray);
      return NULL;
   }

#if DEBUG
#ifdef CHECK_RETURN_TYPES
    if (CMGetArrayCount(rh.rvArray,NULL)==0 ||
	rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) != CMPI_class) {
	/* Should not happen in production builds */
	CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED,
				 strdup("Unexpected return value"));
	return NULL;
    }
#else
   if (do_debug)
       printf ("\treturn enumeration array type %d expected CMPI_class %d\n",
	    rh.rvArray->ft->getSimpleType(rh.rvArray, NULL), CMPI_class);
#endif
#endif

   CMSetStatus(rc, CMPI_RC_OK);
   return newCMPIEnumeration(rh.rvArray,NULL);
}

/* --------------------------------------------------------------------------*/

static CMCIClientFT clientFt = {
   NATIVE_FT_VERSION,
   releaseClient,
   cloneClient,
   getClass,
   enumClassNames,
   enumClasses,
   getInstance,
   createInstance,
   setInstance,
   deleteInstance,
   execQuery,
   enumInstanceNames,
   enumInstances,
   associators,
   associatorNames,
   references,
   referenceNames,
   invokeMethod,
   setProperty,
   getProperty
};

/* --------------------------------------------------------------------------*/

CMCIClient *cmciConnect(const char *hn, const char *scheme, const char *port,
                        const char *user, const char *pwd, CMPIStatus *rc)
{
   ClientEnc *cc = (ClientEnc*)calloc(1, sizeof(ClientEnc));

   cc->enc.hdl		= &cc->data;
   cc->enc.ft		= &clientFt;

   cc->data.hostName	= hn ? strdup(hn) : strdup("localhost");
   cc->data.user	= user ? strdup(user) : NULL;
   cc->data.pwd		= pwd ? strdup(pwd) : NULL;
   cc->data.scheme	= scheme ? strdup(scheme) : strdup("http");
   if (port != NULL)
      cc->data.port = strdup(port);
   else
      cc->data.port = strcmp(cc->data.scheme, "https") == 0 ? 
	strdup("5989") : strdup("5988");

   cc->connection=initConnection(&cc->data);

   return (CMCIClient*)cc;
}
