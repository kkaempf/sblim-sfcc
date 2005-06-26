
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

#include "cmci.h"
#include "utilStringBuffer.h"
#include "utilList.h"
#include "native.h"

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cimXmlParser.h"

typedef struct _CMCIConnectionFT {
    char *(*genRequest)(ClientEnc *cle, char *op, CMPIObjectPath *cop, int cls, int keys);
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

static const char *headers[] = {
    "Content-type: application/xml; charset=\"utf-8\"",
    "Accept:",
    "Expect:",
    "CIMProtocolVersion: 1.0",
    "CIMOperation: MethodCall"
};
#define NUM_HEADERS ((sizeof(headers))/(sizeof(headers[0])))

/* --------------------------------------------------------------------------*/

static char *getErrorMessage(CURLcode err)
{
    static char error[128];
#if LIBCURL_VERSION_NUM >= 0x071200
    return curl_easy_strerror(rv);
#else
    sprintf(error,"CURL error: %d",err);
    return error;
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

static size_t writeCb(void *ptr, size_t size, size_t nmemb, void *stream)
{
    UtilStringBuffer *sb=(UtilStringBuffer*)stream;
    unsigned int length = size * nmemb;
    sb->ft->appendBlock(sb, ptr, length);
    return length;
}

/* --------------------------------------------------------------------------*/

static char *addPayload(CMCIConnection *con, UtilStringBuffer *pl)
{
//    con->mBody = pl;
//    if (dumpXml)
//        cerr << "To server: " << pl << endl;

    CURLcode rv;

    rv = curl_easy_setopt(con->mHandle, CURLOPT_POSTFIELDS, pl->ft->getCharPtr(pl));
    if (rv) return getErrorMessage(rv);
    rv = curl_easy_setopt(con->mHandle, CURLOPT_POSTFIELDSIZE, pl->ft->getSize(pl));
    if (rv) return getErrorMessage(rv);
    else return NULL;
}

/* --------------------------------------------------------------------------*/

static char* genRequest(ClientEnc *cle, char *op, CMPIObjectPath *cop, int cls, int keys)
{
   CMCIConnection *con=cle->connection;
   CMCIClientData *cld=&cle->data;
   CURLcode rv;
   UtilStringBuffer *sb=newStringBuffer(1024);
   char userPwd[256];
   char method[256]="CIMMethod: ";
 
   if (!con->mHandle) return "Unable to initialize curl interface.";
   
//    if (!supportsSSL() && url.scheme == "https")
//        throw HttpException("this curl library does not support https urls.");

   con->mResponse->ft->reset(con->mResponse);
  
   con->mUri->ft->reset(con->mUri);
   con->mUri->ft->append6Chars(con->mUri,cld->scheme,"://",cld->hostName,":",cld->port,"/cimom");

   // HACK - fix this to instead parse cop to obtain namespace components and put them in CIMObject 
   sb->ft->appendChars(sb, "CIMObject: root%2Fcimv2");
   
   /* Initialize curl with the url */
   rv = curl_easy_setopt(con->mHandle, CURLOPT_URL, con->mUri->ft->getCharPtr(con->mUri));

   /* Disable progress output */
   rv = curl_easy_setopt(con->mHandle, CURLOPT_NOPROGRESS, 1);

   /* This will be a HTTP post */
   rv = curl_easy_setopt(con->mHandle, CURLOPT_POST, 1);

  /* Disable SSL verification */
   rv = curl_easy_setopt(con->mHandle, CURLOPT_SSL_VERIFYHOST, 0);
   rv = curl_easy_setopt(con->mHandle, CURLOPT_SSL_VERIFYPEER, 0);

   /* Set username and password */
   if (cld->user) {
      strcpy(userPwd,cld->user);
      if (cld->pwd) {
         strcat(userPwd,":");
         rv = curl_easy_setopt(con->mHandle, CURLOPT_USERPWD, userPwd);
      }  
   }
   
   // Initialize default headers
   con->ft->initializeHeaders(con);

   // Add CIMMethod header
   strcat(method,op);
   con->mHeaders = curl_slist_append(con->mHeaders, method);

   // Add CIMObject header
   con->mHeaders = curl_slist_append(con->mHeaders, sb->ft->getCharPtr(sb));

   // Set all of the headers for the request
   rv = curl_easy_setopt(con->mHandle, CURLOPT_HTTPHEADER, con->mHeaders);

   // Set up the callbacks to store the response
   rv = curl_easy_setopt(con->mHandle, CURLOPT_WRITEFUNCTION, writeCb);

   // Use CURLOPT_FILE instead of CURLOPT_WRITEDATA - more portable
   rv = curl_easy_setopt(con->mHandle, CURLOPT_FILE, con->mResponse);
   
   // Fail if we receive an error (HTTP response code >= 300)
   rv = curl_easy_setopt(con->mHandle, CURLOPT_FAILONERROR, 1);

   // Turn this on to enable debugging
   rv = curl_easy_setopt(con->mHandle, CURLOPT_VERBOSE, 1);
   
   return NULL;
}

/* --------------------------------------------------------------------------*/

char *getResponse(CMCIConnection *con, CMPIObjectPath *cop)
{
    CURLcode rv;

    rv = curl_easy_perform(con->mHandle);

    if (rv) {
        long responseCode = -1;
        char *error;
        // Use CURLINFO_HTTP_CODE instead of CURLINFO_RESPONSE_CODE
        // (more portable to older versions of curl)
        curl_easy_getinfo(con->mHandle, CURLINFO_HTTP_CODE, &responseCode);
        if (responseCode == 401) {
            error = (con->mUserPass->ft->getSize(con->mUserPass) > 0) ? "Invalid username/password." :
                                               "Username/password required.";
        }
        else error = getErrorMessage(rv);
        return error;
    }

    if (con->mResponse->ft->getSize(con->mResponse) == 0)
        return "No data received from server.";
   
    return NULL;
}

/* --------------------------------------------------------------------------*/

static void initializeHeaders(CMCIConnection *con)
{
     unsigned int i;
   
    if (con->mHeaders) {
        curl_slist_free_all(con->mHeaders);
        con->mHeaders = NULL;
    }
    for (i = 0; i < NUM_HEADERS; i++)
        con->mHeaders = curl_slist_append(con->mHeaders, headers[i]);
}

CMCIConnectionFT conFt={
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
   c->mResponse = newStringBuffer(2048);
  
   return c;   
}

/* --------------------------------------------------------------------------*/

extern UtilList *getNameSpaceComponents(CMPIObjectPath * cop);
extern char *keytype2Chars(CMPIType type);
extern void pathToXml(UtilStringBuffer *sb, CMPIObjectPath *cop);
extern char *value2Chars(CMPIType type, CMPIValue * value);

/*--------------------------------------------------------------------------*/

static void emitlocal(UtilStringBuffer *sb, int f)
{
   if (f)
      sb->ft->appendChars(sb,"<IPARAMVALUE NAME=\"LocalOnly\"><VALUE>TRUE</VALUE></IPARAMVALUE>\n");
   else
      sb->ft->appendChars(sb,"<IPARAMVALUE NAME=\"LocalOnly\"><VALUE>FALSE</VALUE></IPARAMVALUE>\n");
}     

/* --------------------------------------------------------------------------*/

static void emitqual(UtilStringBuffer *sb, int f)
{
   if (f)
      sb->ft->appendChars(sb,"<IPARAMVALUE NAME=\"IncludeQualifiers\"><VALUE>TRUE</VALUE></IPARAMVALUE>\n");
   else
      sb->ft->appendChars(sb,"<IPARAMVALUE NAME=\"IncludeQualifiers\"><VALUE>FALSE</VALUE></IPARAMVALUE>\n");
}

/* --------------------------------------------------------------------------*/

static void emitorigin(UtilStringBuffer *sb, int f)
{
   if (f)
      sb->ft->appendChars(sb,"<IPARAMVALUE NAME=\"IncludeClassOrigin\"><VALUE>TRUE</VALUE></IPARAMVALUE>\n");
   else
      sb->ft->appendChars(sb,"<IPARAMVALUE NAME=\"IncludeClassOrigin\"><VALUE>FALSE</VALUE></IPARAMVALUE>\n");
}     

/* --------------------------------------------------------------------------*/

static void emitdeep(UtilStringBuffer *sb, int f)
{
   if (f)
      sb->ft->appendChars(sb,"<IPARAMVALUE NAME=\"DeepInheritance\"><VALUE>TRUE</VALUE></IPARAMVALUE>\n");
   else
      sb->ft->appendChars(sb,"<IPARAMVALUE NAME=\"DeepInheritance\"><VALUE>FALSE</VALUE></IPARAMVALUE>\n");
}

/* --------------------------------------------------------------------------*/

static void addXmlProperty(UtilStringBuffer *sb, char *name, char *value)
{
   sb->ft->append3Chars(sb, "<PROPERTY NAME=\"", name, "\" TYPE=\"string\">");
   sb->ft->append3Chars(sb, "<VALUE>", value, "</VALUE>");
   sb->ft->appendChars(sb, "</PROPERTY>\n");
}

/* --------------------------------------------------------------------------*/

static void addXmlNamespace(UtilStringBuffer *sb, UtilList *nsc)
{
   char *nsp;

   sb->ft->appendChars(sb,"<LOCALNAMESPACEPATH>");
   for (nsp = nsc->ft->getFirst(nsc); nsp; nsp = nsc->ft->getNext(nsc)) {
      sb->ft->append3Chars(sb, "<NAMESPACE NAME=\"", nsp, "\"></NAMESPACE>");
   }
   sb->ft->appendChars(sb, "</LOCALNAMESPACEPATH>\n");
}

/* --------------------------------------------------------------------------*/

static inline void addXmlHeader(UtilStringBuffer *sb)
{
   static const char xmlHeader[]={
      "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n"
      "<CIM CIMVERSION=\"2.0\" DTDVERSION=\"2.0\">\n"
      "<MESSAGE ID=\"4711\" PROTOCOLVERSION=\"1.0\">"
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

static inline void addXmlClassnameParam(UtilStringBuffer *sb,
					CMPIObjectPath *cop)
{
   CMPIString *cn = cop->ft->getClassName(cop, NULL);
   if (cn && cn->hdl && *((char*)cn->hdl)) { 
      sb->ft->append3Chars(sb,
	      "<IPARAMVALUE NAME=\"ClassName\"><CLASSNAME NAME=\"",
	      (char*)cn->hdl, "\"/></IPARAMVALUE>\n");
   }
}

/* --------------------------------------------------------------------------*/

static void addXmlPropertyListParam(UtilStringBuffer *sb, char** properties)
{
   if (properties) {
      sb->ft->appendChars(sb, "<IPARAMVALUE NAME=\"PropertyList\"><VALUE.ARRAY>");
      while (*properties) {
         sb->ft->append3Chars(sb, "<VALUE>", *properties, "</VALUE>");
         properties++;
      }   
      sb->ft->appendChars(sb, "</VALUE.ARRAY></IPARAMVALUE>\n");
   }
}

/* --------------------------------------------------------------------------*/
/* --------------------------------------------------------------------------*/

/* Finished & working */
static CMPIEnumeration * enumInstanceNames(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	CMPIStatus * rc)
{
   ClientEnc * cl = (ClientEnc*)mb;
   CMCIConnection * con = cl->connection;
   UtilStringBuffer * sb = newStringBuffer(2048);
   char * error;
   ResponseHdr rh;

   con->ft->genRequest(cl, "EnumerateInstanceNames", cop, 0, 0);

   /* Construct the CIM-XML request */
   addXmlHeader(sb);
   sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"EnumerateInstanceNames\">");

   addXmlNamespace(sb, getNameSpaceComponents(cop));
   addXmlClassnameParam(sb, cop);

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

//   fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
   con->ft->addPayload(con,sb);
  
   if ((error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      return NULL;
   }

//   fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
   rh = scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse), cop);

   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      return NULL;
   }
  
   if (rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) == CMPI_ref) {
      CMPIEnumeration *enm = newCMPIEnumeration(rh.rvArray,NULL);
      CMSetStatus(rc,CMPI_RC_OK);
      return enm;
   }  
  
   CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,"Unexpected return value");
   return NULL;
}

/* --------------------------------------------------------------------------*/

/* Finished & working */
static CMPIInstance * getInstance(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	CMPIFlags flags,
	char ** properties,
	CMPIStatus * rc)
{
   ClientEnc *cl=(ClientEnc*)mb;
   CMCIConnection *con=cl->connection;
   UtilStringBuffer *sb=newStringBuffer(2048);
   char *error;
   ResponseHdr rh;
   CMPIString *cn = cop->ft->getClassName(cop,NULL);

   con->ft->genRequest(cl,"GetInstance",cop,0,0);

   /* Construct the CIM-XML request */
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
   addXmlHeader(sb);
   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"GetInstance\">");

   addXmlNamespace(sb, getNameSpaceComponents(cop));

   emitlocal(sb,flags & CMPI_FLAG_LocalOnly);
   emitorigin(sb,flags & CMPI_FLAG_IncludeClassOrigin);
   emitqual(sb,flags & CMPI_FLAG_IncludeQualifiers);

   addXmlPropertyListParam(sb, properties);

   sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"InstanceName\">\n"
          "<INSTANCENAME CLASSNAME=\"",(char*)cn->hdl,"\">\n");
   pathToXml(sb, cop);
   sb->ft->appendChars(sb,"</INSTANCENAME>\n</IPARAMVALUE>\n");
   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

//   fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
   con->ft->addPayload(con,sb);
  
   if ((error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      return NULL;
   }

//   fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
   rh = scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse), cop);

   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      return NULL;
   }
  
   if (rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) == CMPI_instance) {
      CMSetStatus(rc,CMPI_RC_OK);
      return rh.rvArray->ft->getElementAt(rh.rvArray, 0, NULL).value.inst;
   }  
  
   CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,"Unexpected return value");
   return NULL;
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
   ResponseHdr	     rh;
   CMPIString *cn = cop->ft->getClassName(cop,NULL);
   int i, numproperties = inst->ft->getPropertyCount(inst, NULL);
   CMPIString * propertyname;
   CMPIData propertydata;
  
   con->ft->genRequest(cl, "CreateInstance", cop, 0, 0);

   /* Construct the CIM-XML request */
   addXmlHeader(sb);

   sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"CreateInstance\">");

   addXmlNamespace(sb, getNameSpaceComponents(cop));

   sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"NewInstance\">\n"
          "<INSTANCE CLASSNAME=\"",(char*)cn->hdl,"\">\n");

   /* Add all the instance properties */
   for (i=0; i<numproperties; i++) {
      propertydata = inst->ft->getPropertyAt(inst, i, &propertyname, NULL);
      addXmlProperty(sb, propertyname->hdl, value2Chars(propertydata.type,&(propertydata.value)));  
   }
    
   sb->ft->appendChars(sb,"</INSTANCE>\n</IPARAMVALUE>\n");
   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

//   fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
   con->ft->addPayload(con,sb);
  
   if ((error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      return NULL;
   }

//   fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
   rh = scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse), cop);
   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      return NULL;
   }

   if (rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) == CMPI_ref) {
      CMSetStatus(rc,CMPI_RC_OK);
      return rh.rvArray->ft->getElementAt(rh.rvArray, 0, NULL).value.ref;
   }
  
   CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,"Unexpected return value");
   return NULL;
}

/* --------------------------------------------------------------------------*/

/* Finished & working */
static CMPIStatus setInstance(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	CMPIInstance * inst,
	CMPIFlags flags,
	char ** properties)
{
   ClientEnc        *cl  = (ClientEnc*)mb;
   CMCIConnection   *con = cl->connection;
   UtilStringBuffer *sb  = newStringBuffer(2048);
   char             *error;
   ResponseHdr       rh;
   CMPIString *cn = cop->ft->getClassName(cop,NULL);
   int i, numproperties = inst->ft->getPropertyCount(inst, NULL);
   CMPIString * propertyname;
   CMPIData propertydata;
   CMPIStatus rc = {CMPI_RC_OK, NULL};                                                                                                                  
   con->ft->genRequest(cl,"ModifyInstance",cop,0,0);

   /* Construct the CIM-XML request */
   addXmlHeader(sb);
   sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"ModifyInstance\">");

   addXmlNamespace(sb, getNameSpaceComponents(cop));

   /* NOTE - IncludeQualifiers flag is now deprecated */
   emitqual(sb,flags & CMPI_FLAG_IncludeQualifiers);

   /* Add the property filter list */
   addXmlPropertyListParam(sb, properties);

   sb->ft->appendChars(sb, "<IPARAMVALUE NAME=\"ModifiedInstance\">\n");
   sb->ft->appendChars(sb, "<VALUE.NAMEDINSTANCE>\n");

   /* Add the objectpath */
   sb->ft->append3Chars(sb,"<INSTANCENAME CLASSNAME=\"",(char*)cn->hdl,"\">\n");
   pathToXml(sb, cop);
   sb->ft->appendChars(sb,"</INSTANCENAME>\n");

   /* Add the instance */
   sb->ft->append3Chars(sb,"<INSTANCE CLASSNAME=\"",(char*)cn->hdl,"\">\n");
   for (i=0; i<numproperties; i++) {
      propertydata = inst->ft->getPropertyAt(inst, i, &propertyname, NULL);
      addXmlProperty(sb, propertyname->hdl, value2Chars(propertydata.type,&(propertydata.value)));
   }
   sb->ft->appendChars(sb,"</INSTANCE>\n");

   sb->ft->appendChars(sb,"</VALUE.NAMEDINSTANCE>\n");
   sb->ft->appendChars(sb,"</IPARAMVALUE>\n");
   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

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

//   fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
   con->ft->addPayload(con,sb);

   if ((error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(&rc,CMPI_RC_ERR_FAILED,error);
      return rc;
   }

//   fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
   rh = scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse), cop);
   if (rh.errCode != 0) {
      CMSetStatusWithChars(&rc, rh.errCode, rh.description);
      return rc;
   }

   return rc;
}

/* --------------------------------------------------------------------------*/

/* Finished & working */
static CMPIStatus deleteInstance(
	CMCIClient * mb,
	CMPIObjectPath * cop)
{
   ClientEnc *cl=(ClientEnc*)mb;
   CMCIConnection *con=cl->connection;
   UtilStringBuffer *sb=newStringBuffer(2048);
   char *error;
   ResponseHdr rh;
   CMPIString *cn = cop->ft->getClassName(cop,NULL);
   CMPIStatus rc = {CMPI_RC_OK, NULL};                                                                                                                   
   con->ft->genRequest(cl,"DeleteInstance",cop,0,0);
                                                                                                                  
   /* Construct the CIM-XML request */
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

   addXmlHeader(sb);
   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"DeleteInstance\">");
                                                                                                                  
   addXmlNamespace(sb, getNameSpaceComponents(cop));
                                                                                                                  
   sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"InstanceName\">\n"
          "<INSTANCENAME CLASSNAME=\"",(char*)cn->hdl,"\">\n");
   pathToXml(sb, cop);
   sb->ft->appendChars(sb,"</INSTANCENAME>\n");

   sb->ft->appendChars(sb,"</IPARAMVALUE>\n");
   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

//   fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
   con->ft->addPayload(con,sb);
                                                                                                                  
   if ((error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(&rc,CMPI_RC_ERR_FAILED,error);
      return rc;
   }
                                                                                                                  
//   fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
   rh = scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse), cop);
   if (rh.errCode != 0) {
      CMSetStatusWithChars(&rc, rh.errCode, rh.description);
      return rc;
   }
                                                                                                                  
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

   con->ft->genRequest(cl, "ExecQuery", cop, 0, 0);

   addXmlHeader(sb);

   sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"ExecQuery\">");

   addXmlNamespace(sb, getNameSpaceComponents(cop));

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

//   fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
   con->ft->addPayload(con,sb);
                                                                                                                  
   if ((error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      return NULL;
   }
//   fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
                                                                                                                  
   rh = scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse), cop);
                                                                                                                  
   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      return NULL;
   }
                                                                                                                  
   int rv;
   if ((rv=rh.rvArray->ft->getSimpleType(rh.rvArray,NULL)) == CMPI_instance) {
      CMPIEnumeration *enm = newCMPIEnumeration(rh.rvArray,NULL);
      CMSetStatus(rc, CMPI_RC_OK);
      return enm;
   }
//   fprintf(stderr,"rv: %x %d\n",rv,rv);                                                                                                             
   CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,"Unexpected return value");
   return NULL;
}

/* --------------------------------------------------------------------------*/

/* Finished but not working - scanCimXmlResponse fails parsing results */
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

    con->ft->genRequest(cl, "EnumerateInstances", cop, 0, 0);

    addXmlHeader(sb);

    sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"EnumerateInstances\">");
    addXmlNamespace(sb, getNameSpaceComponents(cop));

    addXmlClassnameParam(sb, cop);

    emitdeep(sb,flags & CMPI_FLAG_DeepInheritance);
    emitlocal(sb,flags & CMPI_FLAG_LocalOnly);
    emitqual(sb,flags & CMPI_FLAG_IncludeQualifiers);
    emitorigin(sb,flags & CMPI_FLAG_IncludeClassOrigin);
   
    addXmlPropertyListParam(sb, properties);

    sb->ft->appendChars(sb,"</IMETHODCALL>\n");
    addXmlFooter(sb);

//  fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
    con->ft->addPayload(con,sb);
  
    if ((error = con->ft->getResponse(con, cop))) {
        CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
        return NULL;
    }
//  fprintf(stderr,"{{%s}}\n",con->mResponse->ft->getCharPtr(con->mResponse));

    rh = scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse), cop);

    if (rh.errCode != 0) {
        CMSetStatusWithChars(rc, rh.errCode, rh.description);
        return NULL;
    }

    if (rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) == CMPI_instance) {
        CMPIEnumeration *enm = newCMPIEnumeration(rh.rvArray,NULL);
        CMSetStatus(rc, CMPI_RC_OK);
        return enm;
    }  

   CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,"Unexpected return value");
    return NULL;
}

/* --------------------------------------------------------------------------*/

/* finished but not working - scanCimXmlResponse fails parsing XML results */ 
static CMPIEnumeration * associators(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	const char * assocClass,
	const char * resultClass,
	const char * role,
	const char * resultRole,
	CMPIFlags flags,
	char ** properties, 
	CMPIStatus * rc)
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
   ClientEnc *cl=(ClientEnc*)mb;
   CMCIConnection *con=cl->connection;
   UtilStringBuffer *sb=newStringBuffer(2048);
   UtilList *nsc;
   CMPIString *cn;
   char *ns,*error;
   
   cn=cop->ft->getClassName(cop,NULL);
   con->ft->genRequest(cl,"Associators",cop,0,0);
   addXmlHeader(sb);
   
   nsc=getNameSpaceComponents(cop);
   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"Associators\">\n"
          "<LOCALNAMESPACEPATH>");
   for (ns=nsc->ft->getFirst(nsc); ns; ns=nsc->ft->getNext(nsc))
      sb->ft->append3Chars(sb,"<NAMESPACE NAME=\"",ns,"\"></NAMESPACE>");
   sb->ft->appendChars(sb,"</LOCALNAMESPACEPATH>\n");
   
   sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"ObjectName\">\n"
          "<INSTANCENAME CLASSNAME=\"",(char*)cn->hdl,"\">\n");
   pathToXml(sb, cop);
   sb->ft->appendChars(sb,"</INSTANCENAME>\n</IPARAMVALUE>\n");

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
   if (properties) {
      sb->ft->appendChars(sb,"<IPARAMVALUE NAME=\"PropertyList\"><VALUE.ARRAY>");
      while (*properties) {
          sb->ft->append3Chars(sb,"<VALUE>",*properties,"</VALUE>");
          properties++;
      }    
      sb->ft->appendChars(sb,"</VALUE.ARRAY></IPARAMVALUE>\n");
   }

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

//   fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
   con->ft->addPayload(con,sb);
   
   if ((error = con->ft->getResponse(con,cop))) {
      CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED, error);
      return NULL;
   }

//   fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
   ResponseHdr rh=scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse),cop);

   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      return NULL;
   }
   
   if (rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) == CMPI_instance) {
      CMPIEnumeration *enm = newCMPIEnumeration(rh.rvArray, NULL);
      CMSetStatus(rc, CMPI_RC_OK);
      return enm;
   }   
   
   CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,"Unexpected return value");
   return NULL;
}

/* --------------------------------------------------------------------------*/

/* finished & working */
static CMPIEnumeration * associatorNames(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	const char * assocClass,
	const char * resultClass,
	const char * role,
	const char * resultRole,
	CMPIStatus * rc)
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
   ClientEnc *cl=(ClientEnc*)mb;
   CMCIConnection *con=cl->connection;
   UtilStringBuffer *sb=newStringBuffer(2048);
   UtilList *nsc;
   CMPIString *cn;
   char *ns,*error;
   
   cn=cop->ft->getClassName(cop,NULL);
   con->ft->genRequest(cl,"AssociatorNames",cop,0,0);
   addXmlHeader(sb);
   
   nsc=getNameSpaceComponents(cop);
   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"AssociatorNames\">\n"
          "<LOCALNAMESPACEPATH>");
   for (ns=nsc->ft->getFirst(nsc); ns; ns=nsc->ft->getNext(nsc))
      sb->ft->append3Chars(sb,"<NAMESPACE NAME=\"",ns,"\"></NAMESPACE>");
   sb->ft->appendChars(sb,"</LOCALNAMESPACEPATH>\n");
   
   sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"ObjectName\">\n"
          "<INSTANCENAME CLASSNAME=\"",(char*)cn->hdl,"\">\n");
   pathToXml(sb, cop);
   sb->ft->appendChars(sb,"</INSTANCENAME>\n</IPARAMVALUE>\n");

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
   
//   fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
   con->ft->addPayload(con,sb);
   
   if ((error=con->ft->getResponse(con,cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      return NULL;
   }

//   fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
   ResponseHdr rh=scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse),cop);

   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      return NULL;
   }
   
   if (rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) == CMPI_ref) {
      CMPIEnumeration *enm = newCMPIEnumeration(rh.rvArray, NULL);
      CMSetStatus(rc, CMPI_RC_OK);
      return enm;
   }   
   
   CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,"Unexpected return value");
   return NULL;
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
   ClientEnc *cl=(ClientEnc*)mb;
   CMCIConnection *con=cl->connection;
   UtilStringBuffer *sb=newStringBuffer(2048);
   UtilList *nsc;
   CMPIString *cn;
   char *ns,*error;
   
   cn=cop->ft->getClassName(cop,NULL);
   con->ft->genRequest(cl,"References",cop,0,0);
   addXmlHeader(sb);
   
   nsc=getNameSpaceComponents(cop);
   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"References\">\n"
          "<LOCALNAMESPACEPATH>");
   for (ns=nsc->ft->getFirst(nsc); ns; ns=nsc->ft->getNext(nsc))
      sb->ft->append3Chars(sb,"<NAMESPACE NAME=\"",ns,"\"></NAMESPACE>");
   sb->ft->appendChars(sb,"</LOCALNAMESPACEPATH>\n");
   
   sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"ObjectName\">\n"
          "<INSTANCENAME CLASSNAME=\"",(char*)cn->hdl,"\">\n");
   pathToXml(sb, cop);
   sb->ft->appendChars(sb,"</INSTANCENAME>\n</IPARAMVALUE>\n");

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
   if (properties) {
      sb->ft->appendChars(sb,"<IPARAMVALUE NAME=\"PropertyList\"><VALUE.ARRAY>");
      while (*properties) {
          sb->ft->append3Chars(sb,"<VALUE>",*properties,"</VALUE>");
          properties++;
      }    
      sb->ft->appendChars(sb,"</VALUE.ARRAY></IPARAMVALUE>\n");
   }

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

//   fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
   con->ft->addPayload(con,sb);
   
   if ((error=con->ft->getResponse(con,cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      return NULL;
   }
//   fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
   
   ResponseHdr rh=scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse),cop);

   if (rh.errCode!=0) {
      CMSetStatusWithChars(rc,rh.errCode,rh.description);
      return NULL;
   }
   
   if (rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) == CMPI_instance) {
      CMPIEnumeration *enm = newCMPIEnumeration(rh.rvArray,NULL);
      CMSetStatus(rc,CMPI_RC_OK);
      return enm;
   }   
   
   CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,"Unexpected return value");
   return NULL;
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
   ClientEnc *cl=(ClientEnc*)mb;
   CMCIConnection *con=cl->connection;
   UtilStringBuffer *sb=newStringBuffer(2048);
   UtilList *nsc;
   CMPIString *cn;
   char *ns,*error;
   
   cn=cop->ft->getClassName(cop,NULL);
   con->ft->genRequest(cl,"ReferenceNames",cop,0,0);
   addXmlHeader(sb);
   
   nsc=getNameSpaceComponents(cop);
   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"ReferenceNames\">\n"
          "<LOCALNAMESPACEPATH>");
   for (ns=nsc->ft->getFirst(nsc); ns; ns=nsc->ft->getNext(nsc))
      sb->ft->append3Chars(sb,"<NAMESPACE NAME=\"",ns,"\"></NAMESPACE>");
   sb->ft->appendChars(sb,"</LOCALNAMESPACEPATH>\n");
   
   sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"ObjectName\">\n"
          "<INSTANCENAME CLASSNAME=\"",(char*)cn->hdl,"\">\n");
   pathToXml(sb, cop);
   sb->ft->appendChars(sb,"</INSTANCENAME>\n</IPARAMVALUE>\n");

   if (resultClass!=NULL)
      sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"ResultClass\"><CLASSNAME NAME=\"", resultClass,
          "\"/></IPARAMVALUE>\n");
   if (role) sb->ft->append3Chars(sb, "<IPARAMVALUE NAME=\"Role\"><VALUE>", role,
         "</VALUE></IPARAMVALUE>\n");
   
   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);
   
//   fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
   con->ft->addPayload(con,sb);
   
   if ((error=con->ft->getResponse(con,cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      return NULL;
   }
//   fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
   
   ResponseHdr rh=scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse),cop);

   if (rh.errCode!=0) {
      CMSetStatusWithChars(rc,rh.errCode,rh.description);
      return NULL;
   }
   
   if (rh.rvArray->ft->getSimpleType(rh.rvArray,NULL)==CMPI_ref) {
      CMPIEnumeration *enm=newCMPIEnumeration(rh.rvArray,NULL);
      CMSetStatus(rc,CMPI_RC_OK);
      return enm;
   }   
   
   CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,"Unexpected return value");
   return NULL;
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
   ClientEnc         *cl = (ClientEnc*)mb;
   CMCIConnection   *con = cl->connection;
   UtilStringBuffer *sb=newStringBuffer(2048);
   char      *error;
   ResponseHdr       rh;
   CMPIString *cn = cop->ft->getClassName(cop,NULL);
   CMPIData          retval;
   int i, numinargs = 0;
   int numoutargs = 0;

   if (in) numinargs = in->ft->getArgCount(in, NULL);
   if (out) numoutargs = out->ft->getArgCount(in, NULL);

   con->ft->genRequest(cl, "InvokeMethod", cop, 0, 0);

   addXmlHeader(sb);

   /* Add the extrinsic method name */
   sb->ft->append3Chars(sb,"<METHODCALL NAME=\"", method, "\">");

   sb->ft->appendChars(sb,"<LOCALINSTANCEPATH>");

   addXmlNamespace(sb, getNameSpaceComponents(cop));

   /* Add the objectpath */
   sb->ft->append3Chars(sb,"<INSTANCENAME CLASSNAME=\"",(char*)cn->hdl,"\">\n");
   pathToXml(sb, cop);
   sb->ft->appendChars(sb,"</INSTANCENAME>\n");

   sb->ft->appendChars(sb,"</LOCALINSTANCEPATH>");

   /* Add the input parameters */
   for (i=0; i<numinargs; i++) {
      CMPIString * argname;
      CMPIData argdata = in->ft->getArgAt(in, i, &argname, NULL);
      sb->ft->append3Chars(sb,"<PARAMVALUE NAME=\"", argname->hdl, "\">\n");
      sb->ft->append3Chars(sb,"<VALUE>", value2Chars(argdata.type,&(argdata.value)), "</VALUE>\n");
      sb->ft->appendChars(sb,"</PARAMVALUE>\n");
   }
   
   sb->ft->appendChars(sb,"</METHODCALL>\n");
   addXmlFooter(sb);

//  fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
   con->ft->addPayload(con,sb);

   if ((error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      retval.state = CMPI_notFound | CMPI_nullValue;
      return retval;
   }

//   fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
   rh = scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse),cop);
   
   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      retval.state = CMPI_notFound | CMPI_nullValue;
      return retval;
   }

   /* TODO: process output args */                                                                                                                  
   /* TODO: check this */
   if ((rh.rvArray->ft->getSimpleType(rh.rvArray, NULL) == CMPI_INTEGER) ||
       (rh.rvArray->ft->getSimpleType(rh.rvArray, NULL) == CMPI_ENC) ||
       (rh.rvArray->ft->getSimpleType(rh.rvArray, NULL) == CMPI_REAL) ||
       (rh.rvArray->ft->getSimpleType(rh.rvArray, NULL) == CMPI_SIMPLE) ||
       (rh.rvArray->ft->getSimpleType(rh.rvArray, NULL) == CMPI_ARRAY) ||
       (rh.rvArray->ft->getSimpleType(rh.rvArray, NULL) == CMPI_ENCA)) {
      CMSetStatus(rc, CMPI_RC_OK);
      return rh.rvArray->ft->getElementAt(rh.rvArray, 0, NULL);
   }

   CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED, "Unexpected return value");
   retval.state = CMPI_badValue;
   return retval;
}

/* --------------------------------------------------------------------------*/

/* finished (and working?) - need to test against property provider */
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
   ClientEnc         *cl = (ClientEnc*)mb;
   CMCIConnection   *con = cl->connection;
   UtilStringBuffer *sb=newStringBuffer(2048);
   char      *error;
   ResponseHdr       rh;
   CMPIString *cn = cop->ft->getClassName(cop,NULL);
   CMPIStatus rc = {CMPI_RC_OK, NULL};                                                                                                                   
   con->ft->genRequest(cl, "SetProperty", cop, 0, 0);
                                                                                                                  
   addXmlHeader(sb);
   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"SetProperty\">");
                                                                                                                  
   addXmlNamespace(sb, getNameSpaceComponents(cop));

   /* Add the property */
   sb->ft->append3Chars(sb,
        "<IPARAMVALUE NAME=\"PropertyName\">\n<VALUE>",
        name, "</VALUE>\n</IPARAMVALUE>");

   /* Add the new value */
   sb->ft->append3Chars(sb,
        "<IPARAMVALUE NAME=\"NewValue\">\n<VALUE>",
        value2Chars(type,value), "</VALUE>\n</IPARAMVALUE>");

   /* Add the objectpath */
   sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"InstanceName\">\n"
          "<INSTANCENAME CLASSNAME=\"",(char*)cn->hdl,"\">\n");
   pathToXml(sb, cop);
   sb->ft->appendChars(sb,"</INSTANCENAME>\n</IPARAMVALUE>\n");

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

//  fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
   con->ft->addPayload(con,sb);
                                                                                                                  
   if ((error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(&rc,CMPI_RC_ERR_FAILED,error);
      return rc;
   }
                                                                                                                  
//  fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
   rh = scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse),cop);
                                                                                                                  
   if (rh.errCode != 0) {
      CMSetStatusWithChars(&rc, rh.errCode, rh.description);
      return rc;
   }

   return rc;
}

/* --------------------------------------------------------------------------*/

/* finished (and working?) - need to test against property provider */
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
   ClientEnc	     *cl = (ClientEnc*)mb;
   CMCIConnection   *con = cl->connection;
   UtilStringBuffer *sb=newStringBuffer(2048);
   char	     *error;
   ResponseHdr	     rh;
   CMPIString *cn = cop->ft->getClassName(cop,NULL);
   CMPIData	     retval;

   con->ft->genRequest(cl, "GetProperty", cop, 0, 0);

   addXmlHeader(sb);
   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"GetProperty\">");

   addXmlNamespace(sb, getNameSpaceComponents(cop));

   /* Add the property */
   sb->ft->append3Chars(sb,
        "<IPARAMVALUE NAME=\"PropertyName\">\n<VALUE>",
        name, "</VALUE>\n</IPARAMVALUE>");

   /* Add the objectpath */
   sb->ft->append3Chars(sb,"<IPARAMVALUE NAME=\"InstanceName\">\n"
          "<INSTANCENAME CLASSNAME=\"",(char*)cn->hdl,"\">\n");
   pathToXml(sb, cop);
   sb->ft->appendChars(sb,"</INSTANCENAME>\n</IPARAMVALUE>\n");

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

//  fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
   con->ft->addPayload(con,sb);
  
   if ((error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      retval.state = CMPI_notFound | CMPI_nullValue;
      return retval;
   }

//  fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
   rh = scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse),cop);

   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      retval.state = CMPI_notFound | CMPI_nullValue;
      return retval;
   }

   /* TODO: check this */
   if ((rh.rvArray->ft->getSimpleType(rh.rvArray, NULL) == CMPI_INTEGER) ||
       (rh.rvArray->ft->getSimpleType(rh.rvArray, NULL) == CMPI_ENC) ||
       (rh.rvArray->ft->getSimpleType(rh.rvArray, NULL) == CMPI_REAL) ||
       (rh.rvArray->ft->getSimpleType(rh.rvArray, NULL) == CMPI_SIMPLE) ||
       (rh.rvArray->ft->getSimpleType(rh.rvArray, NULL) == CMPI_ARRAY) ||
       (rh.rvArray->ft->getSimpleType(rh.rvArray, NULL) == CMPI_ENCA)) {
      CMSetStatus(rc, CMPI_RC_OK);
      return rh.rvArray->ft->getElementAt(rh.rvArray, 0, NULL);
   }  

   CMSetStatusWithChars(rc, CMPI_RC_ERR_FAILED, "Unexpected return value");
   retval.state = CMPI_badValue;
   return retval;
}

/* --------------------------------------------------------------------------*/

/* finished but not working - scanCimXmlResponse fails */
static CMPIConstClass * getClass(
	CMCIClient * mb,
	CMPIObjectPath * cop,
	CMPIFlags flags,
	char ** properties,
	CMPIStatus * rc)
{
   ClientEnc *cl=(ClientEnc*)mb;
   CMCIConnection *con=cl->connection;
   UtilStringBuffer *sb=newStringBuffer(2048);
   char *error;
  
   con->ft->genRequest(cl,"GetClass",cop,0,0);

   addXmlHeader(sb);
  
   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"GetClass\">");

   addXmlNamespace(sb, getNameSpaceComponents(cop));

   emitlocal(sb,flags & CMPI_FLAG_LocalOnly);
   emitorigin(sb,flags & CMPI_FLAG_IncludeClassOrigin);
   emitqual(sb,flags & CMPI_FLAG_IncludeQualifiers);

   addXmlPropertyListParam(sb, properties);

   addXmlClassnameParam(sb, cop);

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

//   fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
   con->ft->addPayload(con,sb);
  
   if ((error=con->ft->getResponse(con,cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      return NULL;
   }
//   fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
  
   ResponseHdr rh=scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse),cop);

   if (rh.errCode!=0) {
      CMSetStatusWithChars(rc,rh.errCode,rh.description);
      return NULL;
   }
  
   if (rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) == CMPI_class) {
      CMSetStatus(rc,CMPI_RC_OK);
      return rh.rvArray->ft->getElementAt(rh.rvArray, 0, NULL).value.cls;
   }  
  
   CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,"Unexpected return value");
   return NULL;
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

   con->ft->genRequest(cl,"EnumerateClassNames",cop,0,0);

   /* Construct the CIM-XML request */
   addXmlHeader(sb);
   sb->ft->appendChars(sb,"<IMETHODCALL NAME=\"EnumerateClassNames\">");
   
   addXmlNamespace(sb, getNameSpaceComponents(cop));
   emitdeep(sb,flags & CMPI_FLAG_DeepInheritance);
   addXmlClassnameParam(sb, cop);

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);
//   fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));

   con->ft->addPayload(con,sb);
  
   if ((error = con->ft->getResponse(con,cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      return NULL;
   }
//   fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
  
   rh=scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse),cop);

   if (rh.errCode!=0) {
      CMSetStatusWithChars(rc,rh.errCode,rh.description);
      return NULL;
   }
  
   if (rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) == CMPI_ref) {
      CMPIEnumeration *enm = newCMPIEnumeration(rh.rvArray,NULL);
      CMSetStatus(rc,CMPI_RC_OK);
      return enm;
   }  
  
   CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,"Unexpected return value");
   return NULL;
}

/* --------------------------------------------------------------------------*/

/* finished (and working?) */
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

   con->ft->genRequest(cl, "EnumerateClasses", cop, 0, 0);

   /* Construct the CIM-XML request */
   addXmlHeader(sb);
   sb->ft->appendChars(sb, "<IMETHODCALL NAME=\"EnumerateClasses\">");
   
   addXmlNamespace(sb, getNameSpaceComponents(cop));
   emitdeep(sb,flags & CMPI_FLAG_DeepInheritance);
   emitlocal(sb,flags & CMPI_FLAG_LocalOnly);
   emitqual(sb,flags & CMPI_FLAG_IncludeQualifiers);
   emitorigin(sb,flags & CMPI_FLAG_IncludeClassOrigin);
   addXmlClassnameParam(sb, cop);

   sb->ft->appendChars(sb,"</IMETHODCALL>\n");
   addXmlFooter(sb);

//  fprintf(stderr,"%s\n",sb->ft->getCharPtr(sb));
   con->ft->addPayload(con,sb);
  
   if ((error = con->ft->getResponse(con, cop))) {
      CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,error);
      return NULL;
   }

//  fprintf(stderr,"%s\n",con->mResponse->ft->getCharPtr(con->mResponse));
   rh = scanCimXmlResponse(con->mResponse->ft->getCharPtr(con->mResponse), cop);

   if (rh.errCode != 0) {
      CMSetStatusWithChars(rc, rh.errCode, rh.description);
      return NULL;
   }

   if (rh.rvArray->ft->getSimpleType(rh.rvArray,NULL) == CMPI_class) {
      CMPIEnumeration *enm = newCMPIEnumeration(rh.rvArray,NULL);
      CMSetStatus(rc, CMPI_RC_OK);
      return enm;
   }  

   CMSetStatusWithChars(rc,CMPI_RC_ERR_FAILED,"Unexpected return value");
   return NULL;
}

/* --------------------------------------------------------------------------*/

static CMCIClientFT clientFt = {
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

CMCIClient *cmciConnect(const char *hn, const char *port,
                        const char *user, const char *pwd, CMPIStatus *rc)
{
   ClientEnc *cc=(ClientEnc*)calloc(1,sizeof(ClientEnc));
                                                                                                                  
   cc->enc.hdl=&cc->data;
   cc->enc.ft=&clientFt;
                                                                                                                  
   cc->data.hostName=  hn ?   strdup(hn)   : "localhost";
   cc->data.port=      port ? strdup(port) : "5988";
   cc->data.user=      user ? strdup(user) : NULL;
   cc->data.pwd=       pwd ?  strdup(pwd)  : NULL;
   cc->data.scheme=    "http";
                                                                                                                  
   cc->connection=initConnection(&cc->data);
                                                                                                                  
   return (CMCIClient*)cc;
}
