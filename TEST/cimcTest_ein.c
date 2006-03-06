/*
 * test_ein.c
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
 *  Test for enumInstanceNames() library API. Note that this test case
 *  requires that the CIM schema 2.10 final and the test instance MOF
 *  be installed in root/iicmv1. For more information see library README.
 */
#include <cimcClient.h>
#include <cimcmacs.h>
#include <unistd.h>
#include <stdlib.h>

#include "cimcShow.h"

int main()
{
    cimcEnv		* ce;
    int rc;
    char *msg;
    cimcClient		* cc;
    cimcObjectPath	* objectpath;
    cimcEnumeration	* enumeration;
    cimcStatus		status;
    char		*cim_host, *cim_host_passwd, *cim_host_userid;

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
	
    cc = ce->connect(ce, cim_host, NULL, "5988",
			       cim_host_userid, cim_host_passwd, &status);
			       
	 if (cc==NULL)
	 {
       printf( "connect() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
       return -1;
	 }

    /* Test enumInstanceNames() */
    printf("\n----------------------------------------------------------\n");
    printf("Testing enumInstanceNames() ...\n");
    objectpath = ce->newObjectPath("root/cimv2", "CIM_process", NULL);

    enumeration = cc->ft->enumInstanceNames(cc, objectpath, &status);

    /* Print the results */
    printf( "enumInstanceNames() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
    if (!status.rc) {
        printf("result(s):\n");
        while (enumeration->ft->hasNext(enumeration, NULL)) {
            cimcData data = enumeration->ft->getNext(enumeration, NULL);
            showObjectPath(data.value.ref);
        }
    }

    if (enumeration) CMRelease(enumeration);
    if (objectpath) CMRelease(objectpath);
    if (cc) CMRelease(cc);
    if (status.msg) CMRelease(status.msg);
 
    return 0;
}
