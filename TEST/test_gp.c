/*
 * test_gp.c
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
 *  Test for getProperty() library API. Note that this test case
 *  requires that the CIM schema 2.10 final and the test instance MOF
 *  be installed in root/iicmv1. For more information see library README.
 */
#include <cmci.h>
#include <native.h>
#include <unistd.h>

#include "show.h"

int main()
{
    CMCIClient *cc;
    CMPIObjectPath * objectpath;
    CMPIStatus status;
    CMPIData   data;
    char 	*cim_host, *cim_host_passwd, *cim_host_userid;

    /* Setup a connection to the CIMOM */
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
	native_release_CMPIValue(data.type,&data.value);
    }

    data = cc->ft->getProperty(cc, objectpath, "Width", &status);
    /* Print the results */
    printf( "getProperty() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
    if (!status.rc) 
    {
        printf("result(s):\n");
        printf("Width=%s\n", (char*)(data.value.string)->hdl);
	native_release_CMPIValue(data.type,&data.value);
    }

    if (objectpath) CMRelease(objectpath);
    if (cc) CMRelease(cc);
    if (status.msg) CMRelease(status.msg);
  
    return 0;
}

