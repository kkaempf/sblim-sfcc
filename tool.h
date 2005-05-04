/*!
  \file tool.h
  \brief General tooling facility header file.

  (C) Copyright IBM Corp. 2003
 
  THIS FILE IS PROVIDED UNDER THE TERMS OF THE COMMON PUBLIC LICENSE 
  ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE 
  CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 
  You can obtain a current copy of the Common Public License from
  http://oss.software.ibm.com/developerworks/opensource/license-cpl.html

  \author Frank Scheffler
  $Revision: 1.1 $
*/

#ifndef _SFCC_TOOL_H
#define _SFCC_TOOL_H

#include "cmcimacs.h"
#include "cmcidt.h"
#include "cmcift.h"
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define tool_mm_alloc(opt,size) calloc(1,size)
#define tool_mm_realloc(obj,size) realloc(obj,size) 
#define tool_mm_add(obj)
#define memAddEncObj(type, ul, size, state) *state=0,memcpy(malloc(size),ul,size)

#define TOOL_MM_NO_ADD 0
#define TOOL_MM_ADD    1

#endif
