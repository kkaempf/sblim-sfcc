/*
 * test_sp.c
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
 *  Test for setProperty() library API. Note that this test case
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
    CMPIValue value;

    /* Setup a conncetion to the CIMOM */   
    cc = cmciConnect("localhost", NULL, "5988", "root", "password", NULL);
   
    /* Test setProperty() */
    printf("\n----------------------------------------------------------\n");
    printf("Testing setProperty() ...\n");
    objectpath = newCMPIObjectPath("root/iicmv1", "CIM_PhysicalPackage", NULL);
    CMAddKey(objectpath, "CreationClassName", "CIM_PhysicalPackage", CMPI_chars);
    CMAddKey(objectpath, "Tag", "IBM Asset Tag:0000002", CMPI_chars);

    value.string = newCMPIString("Sorta Fast Fan", NULL);
    status = cc->ft->setProperty( cc, objectpath, "Model", 
                                  &value, CMPI_chars);

    /* Print the results */
    printf( "setProperty() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);

    if (objectpath) CMRelease(objectpath);

    return 0;
}

