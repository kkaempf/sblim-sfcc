/*
 * test_ein.c
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
 *  Test for enumInstanceNames() library API. Note that this test case
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
    CMPIConstClass	* class;
    CMPIEnumeration	* enumeration;
    CMPIStatus		status;

    /* Setup a conncetion to the CIMOM */   
    cc = cmciConnect("localhost", NULL, "5988", "root", "password", NULL);
   
    /* Test enumInstanceNames() */
    printf("\n----------------------------------------------------------\n");
    printf("Testing enumInstanceNames() ...\n");
    objectpath = newCMPIObjectPath("root/iicmv1", "CIM_PhysicalPackage", NULL);

    enumeration = cc->ft->enumInstanceNames(cc, objectpath, &status);

    /* Print the results */
    printf( "enumInstanceNames() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
    if (!status.rc) {
        printf("result(s):\n");
        while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showObjectPath(data.value.ref);
        }
    }

    if (enumeration) CMRelease(enumeration);
    if (objectpath) CMRelease(objectpath);
 
    return 0;
}
