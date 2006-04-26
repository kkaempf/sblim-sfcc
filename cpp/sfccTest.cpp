

#include "CimClient.h"

int main(int args, char *argv[])
{
   CimClient *cc = NULL;
   CimStatus rc;

   try {
      cc = new CimClient("SfcbLocal");
      cc->connect();
   }
   catch (CimStatus &st) {
      printf("Failed(%d): %s\n",(int)st,(char*)st);
      exit(1);
   }

   CimObjectPath  op = cc->makeObjectPath("root/cimv2", NULL);
   CimObjectPathEnumeration enop = cc->enumerateClassNames(op, 
       CIMC_FLAG_DeepInheritance|CIMC_FLAG_IncludeQualifiers);

   while (enop->hasNext()) {
      CimObjectPath p=enop->getNext();
      fprintf(stderr,"%s\n",(char*)*p->getClassName());
   }

   CimClassEnumeration enc = cc->enumerateClasses(op, 
       CIMC_FLAG_DeepInheritance|CIMC_FLAG_IncludeQualifiers);

   fprintf(stderr,"        ok\n");
   while (enc->hasNext()) {
      CimClass c=enc->getNext();
      fprintf(stderr,"%s\n",(char*)*c->getClassName());
      CimIterator it(c->getPropertyIterator());
      while (it->hasNext()) {
         CimString *name;
         CimData p=it->getNext(&name);
         fprintf(stderr,"\tP: %s\n",(char*)(**name));
         delete name;
         ;
      } 
   }
   
   delete cc;
   
   return 0;
}

