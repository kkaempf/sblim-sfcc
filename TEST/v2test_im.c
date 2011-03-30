/*
 * test_im1.c
 *
 * (C) Copyright IBM Corp. 2005
 * (C) Copyright Intel Corp. 2005
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
 * Testing method invocation over SfcbLocal using 
 * SFCB's cmpiTestMethodProvider
 */

#include <cimc.h>
#include <cmci.h>
#include <native.h>
#include <unistd.h>
#include <stdlib.h>

#include "show.h"

int main()
{
  CIMCEnv* ce;
  CIMCClient* client;
  char* msg; 
  CIMCStatus cstatus;
  int rc;

  CIMCObjectPath	* objectpath;
  CMPIStatus		status;
  CIMCArgs		* args; 
  char 		*cim_host, *cim_host_passwd, *cim_host_userid, *cim_host_port;
  CIMCData		retval;
  CIMCValue		arg;

    /* Setup a connection to the CIMOM */
    cim_host = getenv("CIM_HOST");
    if (cim_host == NULL) cim_host = "localhost";
    cim_host_userid = getenv("CIM_HOST_USERID");
    if (cim_host_userid == NULL) cim_host_userid = "root";
    cim_host_passwd = getenv("CIM_HOST_PASSWD");
    if (cim_host_passwd == NULL) cim_host_passwd = "password";
    cim_host_port = getenv("CIM_HOST_PORT");
    if (cim_host_port == NULL) cim_host_port = "5988";

    ce = NewCIMCEnv("SfcbLocal",0,&rc,&msg);
    //    ce = NewCIMCEnv("XML",0,&rc,&msg);
    client = ce->ft->connect(ce, cim_host , "http", cim_host_port, cim_host_userid, cim_host_passwd , &cstatus);

    objectpath = ce->ft->newObjectPath(ce, "root/cimv2", "Sample_Method", NULL);

    printf("+++creating args\n");

    args = ce->ft->newArgs(ce, NULL);
    CIMCValue a;
    const int ARR_SIZE = 1672;
    a.array = ce->ft->newArray(ce, ARR_SIZE, CMPI_uint8, NULL);

    arg.uint8 = 123;

    int i;
    for (i = 0; i < ARR_SIZE; i++) {
      a.array->ft->setElementAt(a.array, i, &arg, CMPI_uint8);
    }

    args->ft->addArg(args, "IntArray", &a, CMPI_uint8A);

    printf("+++calling invoke method\n");

    retval = client->ft->invokeMethod(client, objectpath, "CheckArrayNoType", args, NULL, &cstatus);

    /* Print the results */
    printf( "invokeMethod() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);

    //    if (args) CMRelease(args);
    printf("release status\n");
    if (status.msg) CMRelease(status.msg);

    if (!status.rc) {
      char *cv = value2Chars(retval.type,&(retval.value));
      printf("result(s):\n\treturn value: "%s"\n", cv);
    }

    if (objectpath) CMRelease(objectpath);
    if (ce) CMRelease(ce);
  
    return 0;
}

