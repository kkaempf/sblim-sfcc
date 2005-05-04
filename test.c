
#include "cmci.h"

int main(int argc, char *argv[])
{
   CMCIClient *cc;
   CMPIObjectPath *cop;
   CMPIInstance *ci;
   CMPIEnumeration *enm;
   CMPIStatus rc;
   
   cc=cmciConnect("localhost",NULL,NULL,NULL,NULL);
   cop=newCMPIObjectPath("root/cimv2","Linux_ComputerSystem",NULL);
   CMAddKey(cop,"CreationClassName","Linux_ComputerSystem",CMPI_chars);
   
   enm=cc->ft->enumInstanceNames(cc,cop,&rc);
   printf("--- enumInstanceNames rc: %d - %s\n",rc.rc, rc.msg ? (char*)rc.msg->hdl : NULL);
   
   CMAddKey(cop,"Name","localhost.localdomain",CMPI_chars);
//   CMAddKey(cop,"Name","dyn-9-152-141-116.boeblingen.de.ibm.com" ,CMPI_chars); 
   
   ci=cc->ft->getInstance(cc,cop,NULL,&rc);   
   printf("--- getInstance rc: %d - %s\n",rc.rc, rc.msg ? (char*)rc.msg->hdl : NULL);
   
   return 0;
   
   
   
}
