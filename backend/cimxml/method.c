/*!
  \file method.c
  \brief Native method implementation.

  This module implements a native method, which is not public to any
  provider programmer. It is used to implement various other data types
  natively, such as instances, object-paths and args.

  It provides means to maintain linked lists of named methods including
  functionality to add, remove, clone and release them.

  (C) Copyright IBM Corp. 2013

  THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
  ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
  CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.

  You can obtain a current copy of the Eclipse Public License from
  http://www.opensource.org/licenses/eclipse-1.0.php

  \author Frank Scheffler
  $Revision: 1.3 $
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmcidt.h"
#include "cmcift.h"
#include "cmcimacs.h"
#include "native.h"

#ifdef DMALLOC
#include "dmalloc.h"
#endif

//! Storage container for commonly needed data within native CMPI data types.
/*!
  This structure is used to build linked lists of data containers as needed
  for various native data types.
*/
// struct native_method {}  // defined in frontend/sfcc/native.h

/****************************************************************************/

static CMPIData __convert2CMPIData(struct native_method * meth,
    CMPIString ** methname) {

  CMPIData result = { 0, CMPI_nullValue, { 0 } };

  if (meth != NULL) {
    result.type = meth->type;
    result.state = meth->state;
    result.value = meth->value;

    if (methname) {
      *methname = native_new_CMPIString(meth->name, NULL);
    }
  }
  return result;
}


/**
 * returns non-zero if already existent
 */
static int __addMethod(struct native_method ** meth, const char * name,
    CMPIType type, CMPIValueState state,
    CMPIValue * value) {

  CMPIStatus rc;

  if (*meth == NULL) {
    struct native_method * tmp = *meth = (struct native_method *) calloc(1,
        sizeof(struct native_method));

    tmp->name = strdup(name);
    tmp->type = type;
    tmp->state = state;

    if (type != CMPI_null && state != CMPI_nullValue) {
      if (type == CMPI_chars) {
        tmp->type = CMPI_string;
        tmp->value.string = native_new_CMPIString((char *) value, &rc);
      } else
        tmp->value = native_clone_CMPIValue(type, value, &rc);
    } else {
      tmp->state = CMPI_nullValue;
      tmp->value.uint64 = 0;
    }
    return 0;
  }
  return (strcasecmp((*meth)->name, name) == 0
      || __addMethod(&((*meth)->next), name, type, state, value));
}


/**
 * returns -1 if non-existent
 */
static int __setMethod(struct native_method * meth, const char * name,
    CMPIType type, CMPIValue * value) {

  CMPIStatus rc;

  if (meth == NULL)
    return -1;

  if (strcasecmp(meth->name, name) == 0) {

    if (!(meth->state & CMPI_nullValue))
      native_release_CMPIValue(meth->type, &meth->value);

    meth->type = type;
    if (type == CMPI_chars) {
      meth->type = CMPI_string;
      meth->value.string = native_new_CMPIString((char *) value, &rc);
    } else {
      if (type != CMPI_null)
        meth->value = native_clone_CMPIValue(type, value, &rc);
      else
        meth->state = CMPI_nullValue;
    }
    return 0;
  }
  return __setMethod(meth->next, name, type, value);
}


static struct native_method * __getMethod(struct native_method * meth,
    const char * name) {

  if (!meth || !name) {
    return NULL;
  }

  return
      (strcasecmp(meth->name, name) == 0) ? meth : __getMethod(meth->next, name);
}


static CMPIData __getDataMethod(struct native_method * meth, const char * name,
    CMPIStatus * rc) {

  struct native_method * m = __getMethod(meth, name);

  CMSetStatus( rc, ( m ) ? CMPI_RC_OK : CMPI_RC_ERR_FAILED);

  return __convert2CMPIData(m, NULL);
}


static struct native_method * __getMethodAt(struct native_method * meth,
    unsigned int pos) {

  if (!meth)
    return NULL;

  return (pos == 0) ? meth : __getMethodAt(meth->next, --pos);
}


static CMPIData __getDataMethodAt(struct native_method * meth, unsigned int pos,
    CMPIString ** methname, CMPIStatus * rc) {

  struct native_method * m = __getMethodAt(meth, pos);

  CMSetStatus( rc, ( m ) ? CMPI_RC_OK : CMPI_RC_ERR_METHOD_NOT_FOUND);

  return __convert2CMPIData(m, methname);
}


static CMPICount __getMethodCount(struct native_method * meth, CMPIStatus * rc) {
  CMPICount c = 0;

  CMSetStatus(rc, CMPI_RC_OK);

  while (meth != NULL) {
    c++;
    meth = meth->next;
  }
  return c;
}


static void __release(struct native_method * meth) {

  struct native_method *next;

  for (; meth; meth = next) {
    free(meth->name);
    if(meth->state != CMPI_nullValue)
      native_release_CMPIValue (meth->type, &meth->value);
    parameterFT.release(meth->parameters);
    qualifierFT.release(meth->qualifiers);
    next = meth->next;
    free(meth);
  }
}


static struct native_method * __clone(struct native_method * meth,
    CMPIStatus * rc) {

  struct native_method * result;
  CMPIStatus tmp;

  if (meth == NULL) {
    CMSetStatus( rc, CMPI_RC_OK);
    return NULL;
  }

  result = (struct native_method *) calloc(1, sizeof(struct native_method));

  result->name = strdup(meth->name);
  result->type = meth->type;
  result->state = meth->state;
  result->value = native_clone_CMPIValue(meth->type, &meth->value, &tmp);

  if (tmp.rc != CMPI_RC_OK)
    result->state = CMPI_nullValue;

  result->parameters = parameterFT.clone ( meth->parameters, rc );
  result->qualifiers = qualifierFT.clone ( meth->qualifiers, rc );

  result->next = __clone(meth->next, rc);
  return result;
}


/**
 * Global function table to access native_method helper functions.
 */
struct native_methodFT const methodFT = {
    NATIVE_FT_VERSION,
    __release,
    __clone,
    __getMethod,
    __addMethod,
    __setMethod,
    __getDataMethod,
    __getDataMethodAt,
    __getMethodCount
};



/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
