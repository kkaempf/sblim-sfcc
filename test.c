
#include "cmci.h"

int main(int argc, char *argv[])
{
   CMCIClient *cc;
   CMPIObjectPath *cop;
   CMPIInstance *ci;
   CMPIEnumeration *enm;
   CMPIStatus rc;
   
   cc=cmciConnect("localhost",NULL,NULL,NULL,NULL);
   cop=newCMPIObjectPath("root/cimv2",NULL,NULL);
   
   enm=cc->ft->enumClassNames(cc,cop,0,&rc);
   printf("--- enumClassNames rc: %d - %s\n",rc.rc, rc.msg ? (char*)rc.msg->hdl : NULL);
   while (enm->ft->hasNext(enm,NULL)) {
      CMPIData d=enm->ft->getNext(enm,NULL);
      CMPIString *cn=d.value.ref->ft->getClassName(d.value.ref,NULL);
      printf(" -- className: %s\n",(char*)cn->hdl);
      CMRelease(cn);
   }
   
   cop=newCMPIObjectPath("root/cimv2","Linux_ComputerSystem",NULL);
   CMAddKey(cop,"CreationClassName","Linux_ComputerSystem",CMPI_chars);  
   
   enm=cc->ft->enumInstanceNames(cc,cop,&rc);
   printf("--- enumInstanceNames rc: %d - %s\n",rc.rc, rc.msg ? (char*)rc.msg->hdl : NULL);
   
   CMAddKey(cop,"Name","localhost.localdomain",CMPI_chars);
   
   ci=cc->ft->getInstance(cc,cop,0,NULL,&rc);   
   printf("--- getInstance rc: %d - %s\n",rc.rc, rc.msg ? (char*)rc.msg->hdl : NULL);
   
   return 0;
   
   
   
}
