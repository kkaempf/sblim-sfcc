/*
 * test_im4.c
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
 *  Test for invokeMethod() library API. Tests int32_t data type handling.
 *  Note: This test requires that the cmpi_methodtest provider be installed 
 *        and registered on your CIMOM.
 */
#include <cmci.h>
#include <native.h>
#include <unistd.h>
#include <stdlib.h>
#include "show.h"

int main()
{
    CMCIClient		* cc;
    CMPIObjectPath	* objectpath;
    CMPIStatus		status;
    CMPIArgs		* args; 
    char 		*cim_host, *cim_host_passwd, *cim_host_userid;
    CMPIData		retval;
    CMPIValue		arg;

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

    printf("\n----------------------------------------------------------\n");
    printf("Testing invokeMethod() ...\n");

    objectpath = newCMPIObjectPath("root/cimv2", "TST_MethodProperties", NULL);
    CMAddKey(objectpath, "CreationClassName", "TST_MethodProperties", CMPI_chars);
    CMAddKey(objectpath, "Id", "Instance #1", CMPI_chars);

/*---------------------------------------------------------------------*/

    printf("+++T1:passing IN int32_t argument\n");

    args = newCMPIArgs(NULL);
    arg.sint32 = 65535;
    args->ft->addArg(args, "Property_int32", &arg, CMPI_sint32);

    retval = cc->ft->invokeMethod(
        cc, objectpath, "Method_sint32_in", args, NULL, &status);

    /* Print the results */
    printf( "invokeMethod() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);

    if (args) CMRelease(args);
    if (status.msg) CMRelease(status.msg);

    if (!status.rc) {
        char *cv = value2Chars(retval.type,&(retval.value));
        printf("result(s):\n\treturn value:%s\n", cv);
        if (cv != NULL) free(cv);
    }
    else
        goto done;

/*---------------------------------------------------------------------*/

    printf("+++T2:returning int32_t value\n");

    retval = cc->ft->invokeMethod(
        cc, objectpath, "Method_sint32", NULL, NULL, &status);

    /* Print the results */
    printf( "invokeMethod() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);

    if (!status.rc) {
        char *cv = value2Chars(retval.type,&(retval.value));
        printf("result(s):\n\treturn value:%s\n", cv);
        if (cv != NULL) free(cv);
    }

    if (status.msg) CMRelease(status.msg);

/*---------------------------------------------------------------------*/

    printf("+++T3:passing OUT int32_t value\n");

    args = newCMPIArgs(NULL);

    retval = cc->ft->invokeMethod(
        cc, objectpath, "Method_sint32_out", NULL, args, &status);

    /* Print the results */
    printf( "invokeMethod() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);

    if (args) {
        char     *cv;
        CMPIData data = CMGetArg(args, "Arg_int32", NULL);
        if (!CMIsNullValue(data))
        {
            cv = value2Chars(data.type,&(data.value));
            printf("\n result(s): OUT Parm:Arg_int32 type: %d value:%s\n",
                                                            data.type, cv);
            if (cv != NULL) free(cv);
            CMRelease(args);
        }
    }

    if (args) CMRelease(args);
    if (status.msg) CMRelease(status.msg);

    if (!status.rc) {
        char *cv = value2Chars(retval.type,&(retval.value));
        printf("result(s):\n\treturn value:%s\n", cv);
        if (cv != NULL) free(cv);
    }
    else
        goto done;

/*---------------------------------------------------------------------*/

done:
    if (objectpath) CMRelease(objectpath);
    if (cc) CMRelease(cc);
  
    return 0;
}

