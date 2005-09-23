/*
 * test_gc.c
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
 *  Test for getClass() library API. Note that this test case requires 
 *  that the CIM schema 2.10 final and the test instance MOF be installed
 *  in root/iicmv1. For more information see library README.
 */
#include <cmci.h>
#include <native.h>
#include <unistd.h>

#include "show.h"

int main()
{
    CMCIClient *cc;
    CMPIObjectPath * objectpath;
    CMPIConstClass * class;
    CMPIStatus status;
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

    /* Test getClass() */
    printf("\n----------------------------------------------------------\n");
    printf("Testing getClass() ...\n");
    objectpath = newCMPIObjectPath("root/iicmv1", "CIM_AdminDomain", NULL);
    class = cc->ft->getClass(cc, objectpath, 0, NULL, &status);

    /* Print the results */
    printf( "getClass() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);

    if (!status.rc) {
        printf("result:\n");
        showClass(class);
    }

    if (class) CMRelease(class);
    if (objectpath) CMRelease(objectpath);
    
    return 0;
}
