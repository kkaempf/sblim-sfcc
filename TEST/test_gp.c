/*
 * test_gp.c
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
 *  Test for getProperty() library API. Note that this test case
 *  requires that the CIM schema 2.10 final and the test instance MOF
 *  be installed in root/iicmv1. For more information see library README.
 */
#include <lib/cmci.h>
#include <lib/native.h>
#include <unistd.h>

#include "show.h"

int main( int argc, char * argv[] )
{
    CMCIClient *cc;
    CMPIObjectPath * objectpath;
    CMPIStatus status;
    CMPIData   data;

    /* Setup a conncetion to the CIMOM */   
    cc = cmciConnect("localhost", NULL, "5988", "root", "password", NULL);
   
    /* Test getProperty() */
    printf("\n----------------------------------------------------------\n");
    printf("Testing getProperty() ...\n");
    objectpath = newCMPIObjectPath("root/iicmv1", "CIM_PhysicalPackage", NULL);
    CMAddKey(objectpath, "CreationClassName", "CIM_PhysicalPackage", CMPI_chars);
    CMAddKey(objectpath, "Tag", "IBM Asset Tag:0000001", CMPI_chars);

    data = cc->ft->getProperty(cc, objectpath, "ElementName", &status);
    /* Print the results */
    printf( "getProperty() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
    if (!status.rc) 
    {
        printf("result(s):\n");
        printf("ElementName=%s\n", (char*)(data.value.string)->hdl);
    }

    data = cc->ft->getProperty(cc, objectpath, "Width", &status);
    /* Print the results */
    printf( "getProperty() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
    if (!status.rc) 
    {
        printf("result(s):\n");
        printf("ElementName=%s\n", (char*)(data.value.string)->hdl);
    }

    if (objectpath) CMRelease(objectpath);
  
    return 0;
}

