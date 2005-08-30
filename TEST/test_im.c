/*
 * test_im.c
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
 *  Test for invokeMethod() library API. Note that this test case
 *  requires that the CIM schema 2.10 final and the test instance MOF
 *  be installed in root/iicmv1. For more information see library README.
 */
#include <lib/cmci.h>
#include <lib/native.h>
#include <unistd.h>

#include "show.h"

int main( int argc, char * argv[] )
{
    CMCIClient		* cc;
    CMPIObjectPath	* objectpath;
    CMPIStatus		status;
    CMPIArgs		* argsin; 
    CMPIValue		value;

    /* Setup a conncetion to the CIMOM */   
    cc = cmciConnect("localhost", NULL, "5988", "root", "password", NULL);
   
    printf("\n----------------------------------------------------------\n");
    printf("Testing invokeMethod() ...\n");

    objectpath = newCMPIObjectPath("root/cimv2", "CWS_Authorization", NULL);
    CMAddKey(objectpath, "Username", "bestorga", CMPI_chars);
    CMAddKey(objectpath, "Classname", "CIM_OperatingSystem", CMPI_chars);

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
    }

    if (objectpath) CMRelease(objectpath);
  
    return 0;
}

