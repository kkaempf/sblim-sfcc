/*!
  \file parameter.c
  \brief Native parameter implementation.

  This module implements a native parameter, which is not public to any
  provider programmer. It is used to implement various other data types
  natively, such as instances, object-paths and args.

  It provides means to maintain linked lists of named parameters including
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
// struct native_parameter {}  // defined in frontend/sfcc/native.h

/****************************************************************************/

static CMPIData __convert2CMPIData(struct native_parameter * param,
    CMPIString ** paramname) {

  CMPIData result = { 0, CMPI_nullValue, { 0 } };

  if (param != NULL) {
    result.type = param->type;
    result.state = param->state;
    result.value = param->value;

    if (paramname) {
      *paramname = native_new_CMPIString(param->name, NULL);
    }
  }
  return result;
}


/**
 * returns non-zero if already existent
 */
static int __addParameter(struct native_parameter ** param, const char * name,
    CMPIType type) {

  CMPIStatus rc;

  if (*param == NULL) {
    struct native_parameter * tmp = *param = (struct native_parameter *) calloc(1,
        sizeof(struct native_parameter));

    tmp->name = strdup(name);
    tmp->type = type;

    return 0;
  }
  return (strcasecmp((*param)->name, name) == 0
      || __addParameter(&((*param)->next), name, type));
}


/**
 * returns -1 if non-existent
 */
static int __setParameter(struct native_parameter * param, const char * name,
    CMPIType type) {

  CMPIStatus rc;

  if (param == NULL)
    return -1;

  if (strcasecmp(param->name, name) == 0) {

    if (!(param->state & CMPI_nullValue))
      native_release_CMPIValue(param->type, &param->value);

    param->type = type;

    return 0;
  }
  return __setParameter(param->next, name, type);
}


static struct native_parameter * __getParameter(struct native_parameter * param,
    const char * name) {

  if (!param || !name) {
    return NULL;
  }

  return
      (strcasecmp(param->name, name) == 0) ? param : __getParameter(param->next, name);
}


static CMPIData __getDataParameter(struct native_parameter * param, const char * name,
    CMPIStatus * rc) {

  struct native_parameter * p = __getParameter(param, name);

  CMSetStatus( rc, ( p ) ? CMPI_RC_OK : CMPI_RC_ERR_FAILED);

  return __convert2CMPIData(p, NULL);
}


static struct native_parameter * __getParameterAt(struct native_parameter * param,
    unsigned int pos) {

  if (!param)
    return NULL;

  return (pos == 0) ? param : __getParameterAt(param->next, --pos);
}


static CMPIData __getDataParameterAt(struct native_parameter * param, unsigned int pos,
    CMPIString ** paramname, CMPIStatus * rc) {

  struct native_parameter * p = __getParameterAt(param, pos);

  CMSetStatus( rc, ( p ) ? CMPI_RC_OK : CMPI_RC_ERR_NOT_FOUND);

  return __convert2CMPIData(p, paramname);
}


static CMPICount __getParameterCount(struct native_parameter * param, CMPIStatus * rc) {
  CMPICount c = 0;

  CMSetStatus(rc, CMPI_RC_OK);

  while (param != NULL) {
    c++;
    param = param->next;
  }
  return c;
}


static void __release(struct native_parameter * param) {

  struct native_parameter *next;

  for (; param; param = next) {
    free(param->name);
    if(param->state != CMPI_nullValue)
      native_release_CMPIValue(param->type, &param->value);
    next = param->next;
    free(param);
  }
}


static struct native_parameter * __clone(struct native_parameter * param,
    CMPIStatus * rc) {

  struct native_parameter * result;
  CMPIStatus tmp;

  if (param == NULL) {
    CMSetStatus( rc, CMPI_RC_OK);
    return NULL;
  }

  result = (struct native_parameter *) calloc(1, sizeof(struct native_parameter));

  result->name = strdup(param->name);
  result->type = param->type;
  result->state = param->state;
  result->value = native_clone_CMPIValue(param->type, &param->value, &tmp);

  if (tmp.rc != CMPI_RC_OK)
    result->state = CMPI_nullValue;

  result->next = __clone(param->next, rc);
  return result;
}


/**
 * Global function table to access native_parameter helper functions.
 */
struct native_parameterFT const parameterFT = {
    NATIVE_FT_VERSION,
    __release,
    __clone,
    __getParameter,   // added this... need it, or just use the getDataParameterX funcs...?
    __addParameter,
    __setParameter,
    __getDataParameter,
    __getDataParameterAt,
    __getParameterCount
};



/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
