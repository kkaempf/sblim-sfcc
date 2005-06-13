
#include "cmci.h"

int main(int argc, char *argv[])
{
   CMCIClient *cc;
   CMPIObjectPath *cop;
   CMPIInstance *ci;
   CMPIEnumeration *enm;
   CMPIStatus rc;
   CMPIConstClass *cls;
   CMPIData data;
   
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
   CMRelease(enm);
   CMRelease(cop);
   
   cop=newCMPIObjectPath("root/cimv2","Linux_ComputerSystem",NULL);
   cls=cc->ft->getClass(cc,cop,0,NULL,&rc);
   printf("--- getClass rc: %d - %s\n",rc.rc, rc.msg ? (char*)rc.msg->hdl : NULL);
   data=cls->ft->getProperty(cls,"InstallDate",&rc);
   printf("--- getProperty rc: %d - %s %x\n",rc.rc, rc.msg ? (char*)rc.msg->hdl : NULL,data.state);
   data=cls->ft->getProperty(cls,"EnabledDefault",&rc);
   printf("--- getProperty rc: %d - %s %x %d\n",rc.rc, rc.msg ? (char*)rc.msg->hdl : NULL,data.state,data.value.uint16);
  
   CMAddKey(cop,"CreationClassName","Linux_ComputerSystem",CMPI_chars);  
   
   enm=cc->ft->enumInstanceNames(cc,cop,&rc);
   printf("--- enumInstanceNames rc: %d - %s\n",rc.rc, rc.msg ? (char*)rc.msg->hdl : NULL);
   
   if (argc==1) CMAddKey(cop,"Name","localhost.localdomain",CMPI_chars);
   else CMAddKey(cop,"Name","dyn-9-152-143-58.boeblingen.de.ibm.com",CMPI_chars);
   
   ci=cc->ft->getInstance(cc,cop,0,NULL,&rc);   
   printf("--- getInstance rc: %d - %s\n",rc.rc, rc.msg ? (char*)rc.msg->hdl : NULL);
   if (rc.rc) printf("You may need to modify the hostname in line 41 or 42\n");
   
   return 0;
   
   
   
}
