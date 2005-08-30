/*
 * test_xq.c
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
 *  Test for executeQuery() library API. Note that this test case
 *  requires that the CIM schema 2.10 final and the test instance MOF
 *  be installed in root/iicmv1. For more information see library README.
 */
#include <cmci.h>
#include <native.h>
#include <unistd.h>

#include "show.h"

static char * _HOSTNAME = "bestorga.ibm.com";

int main( int argc, char * argv[] )
{
    CMCIClient *cc;
    CMPIObjectPath * objectpath;
    CMPIEnumeration * enumeration;
    CMPIStatus status;
    char hostName[512];

    /* Setup a conncetion to the CIMOM */   
    cc = cmciConnect("localhost", NULL, "5988", "clp", NULL, NULL);
   
    gethostname(hostName,511);
    _HOSTNAME=strdup(hostName);

    /* Test execQuery() */
    printf("\n----------------------------------------------------------\n");
    printf("Testing execQuery() ...\n");
    objectpath = newCMPIObjectPath("root/cimv2", "Linux_ComputerSystem", NULL);
    char * query = "select * from Linux_ComputerSystem where PrimaryOwnerName=\"root\"";
    enumeration = cc->ft->execQuery(cc, objectpath, query, "WQL", &status);

    /* Print the results */
    printf( "execQuery() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);

    if (!status.rc) 
    {
        printf("result(s):\n");
        while (enumeration->ft->hasNext(enumeration, NULL)) 
        {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showInstance(data.value.inst);
        }
    }

    if (enumeration) CMRelease(enumeration);
    if (objectpath) CMRelease(objectpath);
  
    return 0;
}

