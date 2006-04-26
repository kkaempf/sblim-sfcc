
#ifndef Sfcc_CimStatus_h
#define Sfcc_CimStatus_h

#include <cimcClient.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


class CimStatus {
   int rc;
   char *msg;
  public: 
   CimStatus() {
      rc=0;
      msg=NULL;
   }
   CimStatus(int prc, char *pmsg) {
      rc=prc;
      msg=pmsg;
   }
   CimStatus(cimcStatus &st) {
      rc=st.rc;
      msg=(char*)st.msg->hdl;
   }
   operator char*() { return msg; }
   operator int() { return rc; };
};

#endif
