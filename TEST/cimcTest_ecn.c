/*
 * test_ecn.c
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
 *  Test for enumClassNames() library API. Note that this test case
 *  requires that the CIM schema 2.10 final and the test instance MOF
 *  be installed in root/iicmv1. For more information see library README.
 */
#include <cimcClient.h>
#include <cimcmacs.h>
#include <unistd.h>

#include "cimcShow.h"

int main( int argc, char * argv[] )
{
    cimcEnv		* ce;
    int rc;
    char *msg;
    cimcClient *cc;
    cimcObjectPath * objectpath;
    cimcEnumeration * enumeration;
    cimcStatus status;
    char *cim_host, *cim_host_passwd, *cim_host_userid;

    ce=NewCimcEnv("SfcbLocal",0,&rc,&msg);
    if (ce==NULL) {
       fprintf(stderr,"--- Failed to load cimc environment\n"
                      "    rc: %d - %s\n",rc,msg);
    }
    
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
	
    cc = cimcEnvConnect(ce, cim_host, NULL, "5988",
			       cim_host_userid, cim_host_passwd, &status);
			       
	 if (cc==NULL)
	 {
       printf( "connect() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
       return -1;
	 }

    /* Test enumClassNames() */
    printf("\n----------------------------------------------------------\n");
    printf("Testing enumClassNames() ...\n");   
    objectpath = cimcEnvNewObjectPath(ce,"root/cimv2", NULL, NULL);
    
    enumeration = cimcClntEnumClassNames(cc, objectpath, 0, &status);

    /* Print the results */
    printf( "enumClassNames() rc=%d, msg=%s\n", status.rc, 
            (status.msg)? (char *)status.msg->hdl : NULL);

    if (!status.rc) {
        printf("result(s):\n");
        while (cimcEnmHasNext(enumeration, NULL)) {
            cimcData data = cimcEnmGetNext(enumeration, NULL);
            showObjectPath(data.value.ref);
        }
    }

    if (enumeration) cimcEnmRelease(enumeration);
    if (objectpath) cimcOpRelease(objectpath);
    if (cc) cimcClntRelease(cc);
    if (status.msg) cimcStrRelease(status.msg);
    
    return 0;
}
