#include "cmci.h"
#include "native.h"

static char * _HOSTNAME = "bestorga.ibm.com";

extern char *value2Chars(CMPIType type, CMPIValue * value);

static void showObjectPath( CMPIObjectPath * objectpath )
{
   CMPIString * namespace = objectpath->ft->getNameSpace(objectpath, NULL);
   CMPIString * classname = objectpath->ft->getClassName(objectpath, NULL);
   int numkeys = objectpath->ft->getKeyCount(objectpath, NULL);
   int i;

   if (namespace && namespace->hdl) printf("namespace=%s\n", (char *)namespace->hdl);
   if (classname && classname->hdl) printf("classname=%s\n", (char *)classname->hdl);
   if (numkeys) {
      printf("keys:\n");
      for (i=0; i<numkeys; i++) {
         CMPIString * keyname;
         CMPIData data = objectpath->ft->getKeyAt(objectpath, i, &keyname, NULL);
         printf("\t%s=%s\n", (char *)keyname->hdl, value2Chars(data.type, &data.value)); 
      }
   }

   if (classname) CMRelease(classname); 
   if (namespace) CMRelease(namespace);
}


static void showInstance( CMPIInstance *instance )
{
   CMPIObjectPath * objectpath = instance->ft->getObjectPath(instance, NULL);
   CMPIString * objectpathname = objectpath->ft->toString(objectpath, NULL);
   CMPIString * namespace = objectpath->ft->getNameSpace(objectpath, NULL);
   CMPIString * classname = objectpath->ft->getClassName(objectpath, NULL);
   int numproperties = instance->ft->getPropertyCount(instance, NULL);
   int i;

   if (objectpathname && objectpathname->hdl) printf("objectpath=%s\n", (char *)objectpathname->hdl);
   if (namespace && namespace->hdl) printf("namespace=%s\n", (char *)namespace->hdl);
   if (classname && classname->hdl) printf("classname=%s\n", (char *)classname->hdl);
   if (numproperties) {
      printf("properties:\n");
      for (i=0; i<numproperties; i++) {
         CMPIString * propertyname;
         CMPIData data = instance->ft->getPropertyAt(instance, i, &propertyname, NULL);
         printf("\t%s=%s\n", (char *)propertyname->hdl, value2Chars(data.type, &data.value));
      }
   }

   if (classname) CMRelease(classname);
   if (namespace) CMRelease(namespace);
   if (objectpathname) CMRelease(objectpathname);
   if (objectpath) CMRelease(objectpath);
}


static void showClass( CMPIConstClass * class )
{
   CMPIString * classname = class->ft->getClassName(class, NULL);
   int numproperties = class->ft->getPropertyCount(class, NULL);
   int i;

   if (classname && classname->hdl) printf("classname=%s\n", (char *)classname->hdl);
   if (numproperties) {
      printf("properties:\n");
      for (i=0; i<numproperties; i++) {
         CMPIString * propertyname;
         CMPIData data = class->ft->getPropertyAt(class, i, &propertyname, NULL);
         printf("\t%s=%s\n", (char *)propertyname->hdl, value2Chars(data.type, &data.value));
      }
   }

   if (classname) CMRelease(classname);
}


int main( int argc, char * argv[] )
{
   CMCIClient *cc;
   CMPIObjectPath * objectpath;
   CMPIInstance * instance;
   CMPIEnumeration * enumeration;
   CMPIConstClass * class;
   CMPIStatus status;

   /* Setup a conncetion to the CIMOM */   
   cc = cmciConnect("localhost",NULL,NULL,NULL,NULL);

   if (0) {
      /* Test enumClassNames() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing enumClassNames() ...\n");   
      objectpath = newCMPIObjectPath("root/cimv2", NULL, NULL);
      enumeration = cc->ft->enumClassNames(cc, objectpath, 0, &status);

      /* Print the results */
      printf("enumClassNames() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result(s):\n");
         while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showObjectPath(data.value.ref);
         }
      }
      if (enumeration) CMRelease(enumeration);
      if (objectpath) CMRelease(objectpath);
   }

   if (0) {
      /* Test enumClasses() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing enumClasses() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", NULL, NULL);
      enumeration = cc->ft->enumClasses(cc, objectpath, 0, &status);
                                                                                                                
      /* Print the results */
      printf("enumClasses() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result(s):\n");
         while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
//            How get class data out of data.value.???;
         }
      }
      if (enumeration) CMRelease(enumeration);
      if (objectpath) CMRelease(objectpath);
   }

   if (0) {
      /* Test getClass() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing getClass() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "Linux_ComputerSystem", NULL);
      class = cc->ft->getClass(cc, objectpath, 0, NULL, &status);

      /* Print the results */
      printf("getClass() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result:\n");
         showClass(class);
      }
      if (instance) CMRelease(instance);
      if (objectpath) CMRelease(objectpath);
   }

   if (0) {
      /* Test enumInstanceNames() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing enumInstanceNames() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "Linux_ComputerSystem", NULL);
      enumeration = cc->ft->enumInstanceNames(cc, objectpath, &status);

      /* Print the results */
      printf("enumInstanceNames() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result(s):\n");
         while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showObjectPath(data.value.ref);
         }
      }
      if (enumeration) CMRelease(enumeration);
      if (objectpath) CMRelease(objectpath);
   }

   if (0) {
      /* Test enumInstances() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing enumInstances() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "Linux_ComputerSystem", NULL);
      enumeration = cc->ft->enumInstances(cc, objectpath, 0, NULL, &status);

      /* Print the results */
      printf("enumInstances() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result(s):\n");
         while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showInstance(data.value.inst);
         }
      }
      if (enumeration) CMRelease(enumeration);
      if (objectpath) CMRelease(objectpath);
   }

   if (0) {
      /* Test getInstance() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing getInstance() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "Linux_ComputerSystem", NULL);
      CMAddKey(objectpath, "CreationClassName", "Linux_ComputerSystem", CMPI_chars);
      CMAddKey(objectpath, "Name", _HOSTNAME, CMPI_chars);
      instance = cc->ft->getInstance(cc, objectpath, 0, NULL, &status);   

      /* Print the results */
      printf("getInstance() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result:\n");
         showInstance(instance);
      }
      if (instance) CMRelease(instance);
      if (objectpath) CMRelease(objectpath);
   }

   if (1) {
      /* Test createInstance() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing createInstance() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "CWS_Authorization", NULL);
      instance = newCMPIInstance(objectpath, NULL);
      CMSetProperty(instance, "Username", "bestorga", CMPI_chars);
      CMSetProperty(instance, "Classname", "foobar", CMPI_chars);

      objectpath = cc->ft->createInstance(cc, objectpath, instance, &status);
                                                                                                                
      /* Print the results */
      printf("createInstance() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result:\n");
         showObjectPath(objectpath);
      }
      if (instance) CMRelease(instance);
      if (objectpath) CMRelease(objectpath);
   }

   if (1) {
      /* Test setInstance() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing setInstance() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "CWS_Authorization", NULL);
      CMAddKey(objectpath, "Username", "bestorga", CMPI_chars);
      CMAddKey(objectpath, "Classname", "foobar", CMPI_chars);
      instance = newCMPIInstance(objectpath, NULL);
      CMSetProperty(instance, "Username", "bestorga", CMPI_chars);
      CMSetProperty(instance, "Classname", "foobar", CMPI_chars);
      int yes = 1;
      CMSetProperty(instance, "Query", (CMPIValue *)&yes, CMPI_boolean);
                                                                                                                
      status = cc->ft->setInstance(cc, objectpath, instance, 0, NULL);
                                                                                                                
      /* Print the results */
      printf("setInstance() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (instance) CMRelease(instance);
      if (objectpath) CMRelease(objectpath);
   }

   if (1) {
      /* Test deleteInstance() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing deleteInstance() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "CWS_Authorization", NULL);
      CMAddKey(objectpath, "Username", "bestorga", CMPI_chars);
      CMAddKey(objectpath, "Classname", "foobar", CMPI_chars);

      status = cc->ft->deleteInstance(cc, objectpath);
                                                                                                                
      /* Print the results */
      printf("deleteInstance() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (instance) CMRelease(instance);
      if (objectpath) CMRelease(objectpath);
   }

   return 0;
}
