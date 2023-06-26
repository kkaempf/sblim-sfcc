#include <cimc.h>
#include <cmci.h>
#include <native.h>
#include <unistd.h>
#include <stdlib.h>
#include "cmcimacs.h"
#include "show.h"

/*
 * comment out this define to use v2 http XML interface
 */
#define v2local

int main()
{
CIMCEnv *ce;
char *msg = NULL;
int rc;

CIMCStatus status;
CMPIObjectPath *op = NULL ;
CIMCEnumeration *enm = NULL;
CIMCClient *client = NULL;
char 	*cim_host, *cim_host_passwd, *cim_host_userid, *cim_host_port;

    /*
     * Setup a connection to the CIMOM by checking environment 
     * if not found we default those values
     */
    cim_host = getenv("CIM_HOST");
    if (cim_host == NULL)
       cim_host = "localhost";
    cim_host_userid = getenv("CIM_HOST_USERID");
    if (cim_host_userid == NULL)
       cim_host_userid = "root";
    cim_host_passwd = getenv("CIM_HOST_PASSWD");
    if (cim_host_passwd == NULL)
       cim_host_passwd = "password";
    cim_host_port = getenv("CIM_HOST_PORT");
    if (cim_host_port == NULL)
	     cim_host_port = "5988";
	     
    printf(" Testing enumerateInstanceNames \n") ;
    
#ifdef v2local
    printf(" using SfcbLocal interface : host = %s userid = %s\n",
                          cim_host,cim_host_userid) ;
    ce = NewCIMCEnv("SfcbLocal",0,&rc,&msg);
#else
    printf(" using XML HTTP interface : host = %s userid = %s port=%s\n",
                          cim_host,cim_host_userid,cim_host_port) ;
    ce = NewCIMCEnv("XML",0,&rc,&msg);
#endif

    if(ce == NULL) {
      printf(" local connect failed call to NewCIMCEnv message = [%s] \n",msg) ;
      return 1;
    }
    printf("do connect \n") ;
    client = ce->ft->connect(ce, cim_host , "http", cim_host_port, cim_host_userid, cim_host_passwd , &status);
    if (client == NULL) {
	printf("Coudn't connect to the server. Check if sfcb is running.\n");
	return 1;
    }
    
    printf("do newObjectPath \n") ;
    op = (CMPIObjectPath *)ce->ft->newObjectPath(ce, "root/cimv2", "Linux_ComputerSystem" , &status);     
    if(op == NULL) 
    {
       printf(" failed the call to newObjectPath \n") ;	
    }
        
    printf("\ndo execQuery (select spelling error) \n") ;
    char * query = "selet * from Linux_ComputerSystem where PrimaryOwnerName=\"root\"";
    enm = client->ft->execQuery(client, (CIMCObjectPath *) op , query, "CQL", &status);
    if (status.rc) {
        printf("Syntax error successfully caught by execQuery status.rc = %d\n",status.rc) ;
    } else {
        // Though it won't get here since the execQuery aborted before this fix.
        printf("Syntax error NOT caught by execQuery status.rc = %d\n",status.rc) ;
    }

    printf("\ndo execQuery (missing * error) \n") ;
    query = "select from Linux_ComputerSystem where PrimaryOwnerName=\"root\"";
    enm = client->ft->execQuery(client, (CIMCObjectPath *) op , query, "CQL", &status);
    if (status.rc) {
        printf("Syntax error successfully caught by execQuery status.rc = %d\n",status.rc) ;
    } else {
        // Though it won't get here since the execQuery aborted before this fix.
        printf("Syntax error NOT caught by execQuery status.rc = %d\n",status.rc) ;
    }

    printf("\ndo execQuery (from spelling error) \n") ;
    query = "select * fom Linux_ComputerSystem where PrimaryOwnerName=\"root\"";
    enm = client->ft->execQuery(client, (CIMCObjectPath *) op , query, "CQL", &status);
    if (status.rc) {
        printf("Syntax error successfully caught by execQuery status.rc = %d\n",status.rc) ;
    } else {
        // Though it won't get here since the execQuery aborted before this fix.
        printf("Syntax error NOT caught by execQuery status.rc = %d\n",status.rc) ;
    }
    
    if(enm) enm->ft->release(enm);
    if(op) op->ft->release(op);
    if(client) client->ft->release(client);
    if(ce) ReleaseCIMCEnv(ce);
    if(status.msg) CMRelease(status.msg);	
    	
    return 0;
}

/* */
