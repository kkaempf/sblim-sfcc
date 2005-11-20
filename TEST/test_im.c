/*
 * test_im.c
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
 *  Test for invokeMethod() library API. Note that this test case
 *  requires that the CIM schema 2.10 final and the test instance MOF
 *  be installed in root/iicmv1. For more information see library README.
 */
#include <cmci.h>
#include <native.h>
#include <unistd.h>

#include "show.h"

int main( int argc, char * argv[] )
{
    CMCIClient		* cc;
    CMPIObjectPath	* objectpath;
    CMPIStatus		status;
    CMPIArgs		* argsin; 
    CMPIValue		value;
    char 	*cim_host, *cim_host_passwd, *cim_host_userid;

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
    cc = cmciConnect(cim_host, NULL, "5988",
			       cim_host_userid, cim_host_passwd, NULL);

    printf("\n----------------------------------------------------------\n");
    printf("Testing invokeMethod() ...\n");

    objectpath = newCMPIObjectPath("root/cimv2", "CWS_Authorization", NULL);
    CMAddKey(objectpath, "Username", "bestorga", CMPI_chars);
    CMAddKey(objectpath, "Classname", "IICM_OperatingSystem", CMPI_chars);

    argsin = newCMPIArgs(NULL);
    value.string = newCMPIString("Query", NULL);
    argsin->ft->addArg(argsin, "operation", &value, CMPI_string);

    CMPIData data = cc->ft->invokeMethod(
        cc, objectpath, "IsAuthorized", argsin, NULL, &status);

    /* Print the results */
    printf( "getProperty() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);

    if (!status.rc) {
        printf("result(s):\n");
        printf("PrimaryOwnerName=%s\n", (char*)(data.value.string)->hdl);
	native_release_CMPIValue(data.type,&data.value);
    }

    if (objectpath) CMRelease(objectpath);
    if (cc) CMRelease(cc);
    if (status.msg) CMRelease(status.msg);
    if (argsin) CMRelease(argsin);
    if (value.string) CMRelease(value.string);
  
    return 0;
}

