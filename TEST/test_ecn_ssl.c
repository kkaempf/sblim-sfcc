/*
 * test_ecn.c
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
 *  Test for enumClassNames() library API. Note that this test case
 *  requires that the CIM schema 2.10 final and the test instance MOF
 *  be installed in root/iicmv1. For more information see library README.
 */
#include <cmci.h>
#include <native.h>
#include <unistd.h>

#include "show.h"

int main( int argc, char * argv[] )
{
    CMCIClient *cc;
    CMPIObjectPath * objectpath;
    CMPIEnumeration * enumeration;
    CMPIStatus status;
    char 	*cim_host, *cim_host_passwd, *cim_host_userid,
                *cim_host_cert, *cim_client_cert, *cim_client_key;

    /* Setup a conncetion to the CIMOM */
    cim_host = getenv("CIM_HOST");
    if (cim_host == NULL)
	cim_host = "localhost";
    cim_host_userid = getenv("CIM_HOST_USERID");
    if (cim_host_userid == NULL)
	cim_host_userid = "root";
    cim_host_passwd = getenv("CIM_HOST_PASSWD");
    if (cim_host_passwd == NULL)
	cim_host_passwd = "password";
    cim_host_cert = getenv("CIM_HOST_CERT");
    cim_client_cert = getenv("CIM_CLIENT_CERT");
    cim_client_key = getenv("CIM_CLIENT_KEY");
    cc = cmciConnect2(cim_host, "https", "5989",
		      cim_host_userid, cim_host_passwd, 
		      CMCI_VERIFY_PEER, cim_host_cert, 
		      cim_client_cert, cim_client_key,
		      NULL);
   
    /* Test enumClassNames() */
    printf("\n----------------------------------------------------------\n");
    printf("Testing enumClassNames() ...\n");   
    objectpath = newCMPIObjectPath("root/iicmv1", NULL, NULL);
    enumeration = cc->ft->enumClassNames(cc, objectpath, 0, &status);

    /* Print the results */
    printf( "enumClassNames() rc=%d, msg=%s\n", status.rc, 
            (status.msg)? (char *)status.msg->hdl : NULL);

    if (!status.rc) {
        printf("result(s):\n");
        while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showObjectPath(data.value.ref);
        }
    }

    if (enumeration) CMRelease(enumeration);
    if (objectpath) CMRelease(objectpath);
    if (cc) CMRelease(cc);
    if (status.msg) CMRelease(status.msg);
    
    return 0;
}
