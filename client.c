#include "cmci.h"

int main(int argc, char *argv[])
{
   CMCIClient *cc;
   CMPIObjectPath *cop;
   CMPIInstance *ci;

   cc=cmciConnect("localhost","root/cimv2",NULL,NULL,NULL);
   cop=newCMPIObjectPath("root/cimv2","class",NULL);
   CMAddKey(cop,"key1","keyvalue",CMPI_chars);

   ci=cc->ft->getInstance(cc,cop,NULL,NULL);

   return 0;
}

