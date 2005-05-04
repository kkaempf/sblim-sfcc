/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     XTOK_XML = 258,
     ZTOK_XML = 259,
     XTOK_CIM = 260,
     ZTOK_CIM = 261,
     XTOK_MESSAGE = 262,
     ZTOK_MESSAGE = 263,
     XTOK_SIMPLERSP = 264,
     ZTOK_SIMPLERSP = 265,
     XTOK_IMETHODRESP = 266,
     ZTOK_IMETHODRESP = 267,
     XTOK_METHODRESP = 268,
     ZTOK_METHODRESP = 269,
     XTOK_ERROR = 270,
     ZTOK_ERROR = 271,
     XTOK_IRETVALUE = 272,
     ZTOK_IRETVALUE = 273,
     XTOK_RETVALUE = 274,
     ZTOK_RETVALUE = 275,
     XTOK_NAMESPACEPATH = 276,
     ZTOK_NAMESPACEPATH = 277,
     XTOK_LOCALNAMESPACEPATH = 278,
     ZTOK_LOCALNAMESPACEPATH = 279,
     XTOK_NAMESPACE = 280,
     ZTOK_NAMESPACE = 281,
     ZTOK_IPARAMVALUE = 282,
     XTOK_HOST = 283,
     ZTOK_HOST = 284,
     XTOK_VALUE = 285,
     ZTOK_VALUE = 286,
     XTOK_VALUEARRAY = 287,
     ZTOK_VALUEARRAY = 288,
     XTOK_VALUEREFERENCE = 289,
     ZTOK_VALUEREFERENCE = 290,
     XTOK_CLASSNAME = 291,
     ZTOK_CLASSNAME = 292,
     XTOK_INSTANCENAME = 293,
     ZTOK_INSTANCENAME = 294,
     XTOK_KEYBINDING = 295,
     ZTOK_KEYBINDING = 296,
     XTOK_KEYVALUE = 297,
     ZTOK_KEYVALUE = 298,
     XTOK_VALUENAMEDINSTANCE = 299,
     ZTOK_VALUENAMEDINSTANCE = 300,
     XTOK_QUALIFIER = 301,
     ZTOK_QUALIFIER = 302,
     XTOK_PROPERTY = 303,
     ZTOK_PROPERTY = 304,
     XTOK_PROPERTYARRAY = 305,
     ZTOK_PROPERTYARRAY = 306,
     XTOK_PROPERTYREFERENCE = 307,
     ZTOK_PROPERTYREFERENCE = 308,
     XTOK_PARAM = 309,
     ZTOK_PARAM = 310,
     XTOK_PARAMARRAY = 311,
     ZTOK_PARAMARRAY = 312,
     XTOK_PARAMREF = 313,
     ZTOK_PARAMREF = 314,
     XTOK_PARAMREFARRAY = 315,
     ZTOK_PARAMREFARRAY = 316,
     XTOK_METHOD = 317,
     ZTOK_METHOD = 318,
     XTOK_CLASS = 319,
     ZTOK_CLASS = 320,
     XTOK_INSTANCE = 321,
     ZTOK_INSTANCE = 322,
     XTOK_PARAMVALUE = 323,
     ZTOK_PARAMVALUE = 324,
     XTOK_INSTANCEPATH = 325,
     ZTOK_INSTANCEPATH = 326,
     XTOK_LOCALINSTANCEPATH = 327,
     ZTOK_LOCALINSTANCEPATH = 328,
     XTOK_LOCALCLASSPATH = 329,
     ZTOK_LOCALCLASSPATH = 330
   };
#endif
#define XTOK_XML 258
#define ZTOK_XML 259
#define XTOK_CIM 260
#define ZTOK_CIM 261
#define XTOK_MESSAGE 262
#define ZTOK_MESSAGE 263
#define XTOK_SIMPLERSP 264
#define ZTOK_SIMPLERSP 265
#define XTOK_IMETHODRESP 266
#define ZTOK_IMETHODRESP 267
#define XTOK_METHODRESP 268
#define ZTOK_METHODRESP 269
#define XTOK_ERROR 270
#define ZTOK_ERROR 271
#define XTOK_IRETVALUE 272
#define ZTOK_IRETVALUE 273
#define XTOK_RETVALUE 274
#define ZTOK_RETVALUE 275
#define XTOK_NAMESPACEPATH 276
#define ZTOK_NAMESPACEPATH 277
#define XTOK_LOCALNAMESPACEPATH 278
#define ZTOK_LOCALNAMESPACEPATH 279
#define XTOK_NAMESPACE 280
#define ZTOK_NAMESPACE 281
#define ZTOK_IPARAMVALUE 282
#define XTOK_HOST 283
#define ZTOK_HOST 284
#define XTOK_VALUE 285
#define ZTOK_VALUE 286
#define XTOK_VALUEARRAY 287
#define ZTOK_VALUEARRAY 288
#define XTOK_VALUEREFERENCE 289
#define ZTOK_VALUEREFERENCE 290
#define XTOK_CLASSNAME 291
#define ZTOK_CLASSNAME 292
#define XTOK_INSTANCENAME 293
#define ZTOK_INSTANCENAME 294
#define XTOK_KEYBINDING 295
#define ZTOK_KEYBINDING 296
#define XTOK_KEYVALUE 297
#define ZTOK_KEYVALUE 298
#define XTOK_VALUENAMEDINSTANCE 299
#define ZTOK_VALUENAMEDINSTANCE 300
#define XTOK_QUALIFIER 301
#define ZTOK_QUALIFIER 302
#define XTOK_PROPERTY 303
#define ZTOK_PROPERTY 304
#define XTOK_PROPERTYARRAY 305
#define ZTOK_PROPERTYARRAY 306
#define XTOK_PROPERTYREFERENCE 307
#define ZTOK_PROPERTYREFERENCE 308
#define XTOK_PARAM 309
#define ZTOK_PARAM 310
#define XTOK_PARAMARRAY 311
#define ZTOK_PARAMARRAY 312
#define XTOK_PARAMREF 313
#define ZTOK_PARAMREF 314
#define XTOK_PARAMREFARRAY 315
#define ZTOK_PARAMREFARRAY 316
#define XTOK_METHOD 317
#define ZTOK_METHOD 318
#define XTOK_CLASS 319
#define ZTOK_CLASS 320
#define XTOK_INSTANCE 321
#define ZTOK_INSTANCE 322
#define XTOK_PARAMVALUE 323
#define ZTOK_PARAMVALUE 324
#define XTOK_INSTANCEPATH 325
#define ZTOK_INSTANCEPATH 326
#define XTOK_LOCALINSTANCEPATH 327
#define ZTOK_LOCALINSTANCEPATH 328
#define XTOK_LOCALCLASSPATH 329
#define ZTOK_LOCALCLASSPATH 330




/* Copy the first part of user declarations.  */
#line 1 "cimXmlResp.y"


/*
 * cimXmlOps.y
 *
 * (C) Copyright IBM Corp. 2005
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE COMMON PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Common Public License from
 * http://oss.software.ibm.com/developerworks/opensource/license-cpl.html
 *
 * Author:       Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * CIM XML grammar for sfcb.
 *
*/


/*
**==============================================================================
**
** Includes
**
**==============================================================================
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "cimXmlParser.h"


//
// Define the global parser state object:
//

#define YYPARSE_PARAM parm
#define YYLEX_PARAM parm
#define YYERROR_VERBOSE 1

#define PARM ((ParserControl*)parm)

extern int yyerror(char*);
extern int yylex (void *lvalp, ParserControl *parm);


int isBoolean(CMPIData data)
{
   if (data.type==CMPI_chars) {
      if (strcasecmp(data.value.chars,"true")==0) return 0xffff;
      if (strcasecmp(data.value.chars,"false")==0) return 0;
   }
   return -1;
}

static void addInstProperty(CMPIInstance **ci, XtokProperty *p)
{
   CMPIValue val;
   
   if (*ci==NULL) *ci=newCMPIInstance(NULL,NULL);
   switch (p->propType) {
   case typeProperty_Value: ;
      val=str2CMPIValue(p->valueType, p->val.value, NULL);
      (*ci)->ft->setProperty(*ci,p->name,&val,p->valueType);
      break;
   case typeProperty_Reference: ;
   case typeProperty_Array: ;
      break;
   }
}

static void createPath(CMPIObjectPath **op, XtokInstanceName *p)
{
   int i,m;
   CMPIValue val,*valp;
   CMPIType type;
   
   *op = newCMPIObjectPath(NULL, p->className, NULL);
   for (i = 0, m = p->bindings.next; i < m; i++) {
      valp = getKeyValueTypePtr(p->bindings.keyBindings[i].type,
                                p->bindings.keyBindings[i].value,
                                &p->bindings.keyBindings[i].ref,
                                &val, &type);
      CMAddKey(*op, p->bindings.keyBindings[i].name, valp, type);
   }
   CMPIString *path=(*op)->ft->toString(*op,NULL);
   printf("parh: %s\n",(char*)path->hdl);
}

static void setError(void *parm, XtokErrorResp *e)
{
   int err=atoi(e->code);
   fprintf(stderr,"error:: %s %s\n",e->code,e->description);
   PARM->respHdr.errCode=err;
   PARM->respHdr.description=strdup(e->description);
}
   


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 116 "cimXmlResp.y"
typedef union YYSTYPE {
   int                           intValue;
   char                          boolValue;
   char*                         className;
   void*                         tokCim;

   XtokMessage                   xtokMessage;
   XtokErrorResp                 xtokErrorResp;
   
   XtokNameSpace                 xtokNameSpace;
   char*                         xtokLocalNameSpacePath;
   XtokNameSpacePath             xtokNameSpacePath;
   XtokHost                      xtokHost;
   XtokInstancePath              xtokInstancePath;
   XtokLocalInstancePath         xtokLocalInstancePath;
   XtokLocalClassPath            xtokLocalClassPath;

   XtokValue                     xtokValue;
   XtokValueArray                xtokValueArray;
   XtokValueReference            xtokValueReference;

   XtokInstanceName              xtokInstanceName;
   XtokKeyBinding                xtokKeyBinding;
   XtokKeyBindings               xtokKeyBindings;
   XtokKeyValue                  xtokKeyValue;

   XtokClass                     xtokClass;
   XtokInstance                  xtokInstance;
   XtokNamedInstance             xtokNamedInstance;

   XtokProperty                  xtokProperty;
   XtokPropertyData              xtokPropertyData;

   XtokMethod                    xtokMethod;
   XtokMethodData                xtokMethodData;
   XtokQualifier                 xtokQualifier;
   
   XtokParamValue                xtokParamValue;  
   XtokParam                     xtokParam;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 370 "cimXmlResp.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 382 "cimXmlResp.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   85

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  76
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  26
/* YYNRULES -- Number of rules. */
#define YYNRULES  48
/* YYNRULES -- Number of states. */
#define YYNSTATES  95

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   330

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     7,    11,    15,    19,    23,    27,    30,
      33,    37,    38,    42,    43,    45,    48,    49,    51,    54,
      58,    59,    62,    65,    69,    72,    78,    81,    84,    86,
      88,    91,    93,    96,   100,   104,   108,   114,   118,   121,
     125,   130,   133,   138,   141,   145,   147,   150,   155
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      77,     0,    -1,     3,     4,    78,    -1,     5,    79,     6,
      -1,     7,    80,     8,    -1,     9,    81,    10,    -1,    11,
      82,    12,    -1,    11,    83,    12,    -1,    11,    12,    -1,
      15,    16,    -1,    17,    85,    18,    -1,    -1,    17,    84,
      18,    -1,    -1,    86,    -1,    84,    86,    -1,    -1,    99,
      -1,    85,    99,    -1,    66,    87,    67,    -1,    -1,    87,
      93,    -1,    87,    88,    -1,    48,    89,    49,    -1,    48,
      49,    -1,    50,    32,    91,    33,    51,    -1,    50,    51,
      -1,    89,    93,    -1,    90,    -1,    92,    -1,    30,    31,
      -1,    90,    -1,    91,    90,    -1,    34,    98,    35,    -1,
      34,    99,    35,    -1,    46,    90,    47,    -1,    46,    32,
      91,    33,    47,    -1,    23,    95,    24,    -1,    25,    26,
      -1,    95,    25,    26,    -1,    21,    97,    94,    22,    -1,
      28,    29,    -1,    70,    96,    99,    71,    -1,    38,    39,
      -1,    38,   100,    39,    -1,   101,    -1,   100,   101,    -1,
      40,    42,    43,    41,    -1,    40,    92,    41,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   309,   309,   315,   321,   330,   336,   339,   342,   348,
     355,   358,   388,   406,   408,   414,   422,   424,   429,   500,
     506,   507,   510,   522,   526,   530,   534,   541,   544,   548,
     561,   568,   575,   583,   588,   614,   618,   632,   639,   643,
     656,   664,   670,   713,   720,   729,   739,   750,   756
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "XTOK_XML", "ZTOK_XML", "XTOK_CIM", 
  "ZTOK_CIM", "XTOK_MESSAGE", "ZTOK_MESSAGE", "XTOK_SIMPLERSP", 
  "ZTOK_SIMPLERSP", "XTOK_IMETHODRESP", "ZTOK_IMETHODRESP", 
  "XTOK_METHODRESP", "ZTOK_METHODRESP", "XTOK_ERROR", "ZTOK_ERROR", 
  "XTOK_IRETVALUE", "ZTOK_IRETVALUE", "XTOK_RETVALUE", "ZTOK_RETVALUE", 
  "XTOK_NAMESPACEPATH", "ZTOK_NAMESPACEPATH", "XTOK_LOCALNAMESPACEPATH", 
  "ZTOK_LOCALNAMESPACEPATH", "XTOK_NAMESPACE", "ZTOK_NAMESPACE", 
  "ZTOK_IPARAMVALUE", "XTOK_HOST", "ZTOK_HOST", "XTOK_VALUE", 
  "ZTOK_VALUE", "XTOK_VALUEARRAY", "ZTOK_VALUEARRAY", 
  "XTOK_VALUEREFERENCE", "ZTOK_VALUEREFERENCE", "XTOK_CLASSNAME", 
  "ZTOK_CLASSNAME", "XTOK_INSTANCENAME", "ZTOK_INSTANCENAME", 
  "XTOK_KEYBINDING", "ZTOK_KEYBINDING", "XTOK_KEYVALUE", "ZTOK_KEYVALUE", 
  "XTOK_VALUENAMEDINSTANCE", "ZTOK_VALUENAMEDINSTANCE", "XTOK_QUALIFIER", 
  "ZTOK_QUALIFIER", "XTOK_PROPERTY", "ZTOK_PROPERTY", 
  "XTOK_PROPERTYARRAY", "ZTOK_PROPERTYARRAY", "XTOK_PROPERTYREFERENCE", 
  "ZTOK_PROPERTYREFERENCE", "XTOK_PARAM", "ZTOK_PARAM", "XTOK_PARAMARRAY", 
  "ZTOK_PARAMARRAY", "XTOK_PARAMREF", "ZTOK_PARAMREF", 
  "XTOK_PARAMREFARRAY", "ZTOK_PARAMREFARRAY", "XTOK_METHOD", 
  "ZTOK_METHOD", "XTOK_CLASS", "ZTOK_CLASS", "XTOK_INSTANCE", 
  "ZTOK_INSTANCE", "XTOK_PARAMVALUE", "ZTOK_PARAMVALUE", 
  "XTOK_INSTANCEPATH", "ZTOK_INSTANCEPATH", "XTOK_LOCALINSTANCEPATH", 
  "ZTOK_LOCALINSTANCEPATH", "XTOK_LOCALCLASSPATH", "ZTOK_LOCALCLASSPATH", 
  "$accept", "start", "cimOperation", "message", "simpleResp", 
  "iMethodResp", "errorResp", "iReturnValue", "instances", 
  "instanceNames", "instance", "instanceData", "property", "propertyData", 
  "value", "valueArray", "valueReference", "qualifier", 
  "localNameSpacePath", "namespaces", "nameSpacePath", "host", 
  "instancePath", "instanceName", "keyBindings", "keyBinding", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    76,    77,    78,    79,    80,    81,    81,    81,    82,
      83,    83,    83,    84,    84,    84,    85,    85,    85,    86,
      87,    87,    87,    88,    88,    88,    88,    89,    89,    89,
      90,    91,    91,    92,    92,    93,    93,    94,    95,    95,
      96,    97,    98,    99,    99,   100,   100,   101,   101
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     3,     3,     3,     3,     3,     3,     2,     2,
       3,     0,     3,     0,     1,     2,     0,     1,     2,     3,
       0,     2,     2,     3,     2,     5,     2,     2,     1,     1,
       2,     1,     2,     3,     3,     3,     5,     3,     2,     3,
       4,     2,     4,     2,     3,     1,     2,     4,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,     0,     1,     0,     2,     0,     0,     0,
       0,     3,     0,     0,     4,     8,     0,    13,     0,     0,
       5,     9,     0,    20,     0,     0,    14,    17,     6,     7,
      43,     0,     0,    45,     0,    12,    15,    10,    18,     0,
       0,     0,    44,    46,     0,     0,     0,    19,    22,    21,
       0,     0,     0,     0,    48,     0,     0,     0,    24,     0,
      28,    29,     0,    26,     0,     0,    33,    34,    47,    30,
      31,     0,    35,    23,    27,     0,     0,     0,     0,     0,
      32,     0,    41,     0,     0,    42,    36,    25,     0,     0,
      40,    38,    37,     0,    39
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     2,     6,     8,    10,    13,    18,    19,    24,    25,
      26,    34,    48,    59,    70,    71,    41,    49,    84,    89,
      65,    77,    51,    27,    32,    33
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -41
static const yysigned_char yypact[] =
{
      14,    22,    38,    35,   -41,    39,   -41,    40,    41,    42,
      43,   -41,     1,    44,   -41,   -41,    34,   -30,    45,    46,
     -41,   -41,    -7,   -41,   -18,   -15,   -41,   -41,   -41,   -41,
     -41,   -22,     3,   -41,   -39,   -41,   -41,   -41,   -41,   -36,
       9,    15,   -41,   -41,     7,   -20,   -26,   -41,   -41,   -41,
      47,    24,    25,    21,   -41,    30,    33,     8,   -41,   -27,
     -41,   -41,    33,   -41,    36,    27,   -41,   -41,   -41,   -41,
     -41,    -9,   -41,   -41,   -41,    -3,    37,    48,    -4,    23,
     -41,    18,   -41,    49,    50,   -41,   -41,   -41,    51,    20,
     -41,   -41,   -41,    52,   -41
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,
      55,   -41,   -41,   -41,   -40,    11,    31,    16,   -41,   -41,
     -41,   -41,   -41,   -24,   -41,    53
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
      35,    38,    22,    37,    57,    60,    62,    44,    22,    45,
      55,    46,    39,    15,    39,    52,    16,     1,    17,    44,
      40,    55,    73,    22,    79,    63,     3,    55,    47,    58,
      81,    80,    30,    31,    50,    80,    23,    55,     4,    56,
       5,    78,    42,    31,    92,    93,     7,    11,    23,     9,
      21,    14,    53,    12,    20,    72,    54,    28,    29,    66,
      67,    69,    68,    55,    76,    22,    82,    85,    64,    87,
      86,    83,    90,    75,    88,    74,    61,    91,    94,    36,
       0,     0,     0,     0,     0,    43
};

static const yysigned_char yycheck[] =
{
      18,    25,    38,    18,    44,    45,    32,    46,    38,    48,
      30,    50,    34,    12,    34,    39,    15,     3,    17,    46,
      42,    30,    49,    38,    33,    51,     4,    30,    67,    49,
      33,    71,    39,    40,    70,    75,    66,    30,     0,    32,
       5,    65,    39,    40,    24,    25,     7,     6,    66,     9,
      16,     8,    43,    11,    10,    47,    41,    12,    12,    35,
      35,    31,    41,    30,    28,    38,    29,    71,    21,    51,
      47,    23,    22,    62,    25,    59,    45,    26,    26,    24,
      -1,    -1,    -1,    -1,    -1,    32
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,    77,     4,     0,     5,    78,     7,    79,     9,
      80,     6,    11,    81,     8,    12,    15,    17,    82,    83,
      10,    16,    38,    66,    84,    85,    86,    99,    12,    12,
      39,    40,   100,   101,    87,    18,    86,    18,    99,    34,
      42,    92,    39,   101,    46,    48,    50,    67,    88,    93,
      70,    98,    99,    43,    41,    30,    32,    90,    49,    89,
      90,    92,    32,    51,    21,    96,    35,    35,    41,    31,
      90,    91,    47,    49,    93,    91,    28,    97,    99,    33,
      90,    33,    29,    23,    94,    71,    47,    51,    25,    95,
      22,    26,    24,    25,    26
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  /* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 310 "cimXmlResp.y"
    {
    }
    break;

  case 3:
#line 316 "cimXmlResp.y"
    {
    }
    break;

  case 4:
#line 322 "cimXmlResp.y"
    {
    }
    break;

  case 5:
#line 331 "cimXmlResp.y"
    {
    }
    break;

  case 6:
#line 337 "cimXmlResp.y"
    {
    }
    break;

  case 7:
#line 340 "cimXmlResp.y"
    {
    }
    break;

  case 8:
#line 343 "cimXmlResp.y"
    {
    }
    break;

  case 9:
#line 349 "cimXmlResp.y"
    {
        setError(parm,&yyval.xtokErrorResp);
    }
    break;

  case 10:
#line 356 "cimXmlResp.y"
    {
    }
    break;

  case 12:
#line 389 "cimXmlResp.y"
    {
    }
    break;

  case 14:
#line 409 "cimXmlResp.y"
    {
       setInstNsAndCn(PARM->curInstance,yyvsp[0].xtokInstance.className,PARM->nameSpace);
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)PARM->curInstance,CMPI_instance);
       PARM->curInstance=NULL;
    }
    break;

  case 15:
#line 415 "cimXmlResp.y"
    {
       setInstNsAndCn(PARM->curInstance,yyvsp[0].xtokInstance.className,PARM->nameSpace);
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)PARM->curInstance,CMPI_instance);
       PARM->curInstance=NULL;
    }
    break;

  case 17:
#line 425 "cimXmlResp.y"
    {
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)PARM->curPath,CMPI_ref);
       PARM->curPath=NULL;
    }
    break;

  case 18:
#line 430 "cimXmlResp.y"
    {
       setInstNsAndCn(PARM->curInstance,yyvsp[0].xtokInstanceName.className,PARM->nameSpace);
       simpleArrayAdd(PARM->respHdr.rvArray,(CMPIValue*)PARM->curPath,CMPI_ref);
       PARM->curPath=NULL;
    }
    break;

  case 19:
#line 501 "cimXmlResp.y"
    {
    }
    break;

  case 20:
#line 506 "cimXmlResp.y"
    {;}
    break;

  case 21:
#line 508 "cimXmlResp.y"
    {
    }
    break;

  case 22:
#line 511 "cimXmlResp.y"
    {
       addInstProperty(&(PARM->curInstance),&yyvsp[0].xtokProperty);
    }
    break;

  case 23:
#line 523 "cimXmlResp.y"
    {
       yyval.xtokProperty.val=yyvsp[-1].xtokPropertyData;
    }
    break;

  case 24:
#line 527 "cimXmlResp.y"
    {
       yyval.xtokProperty.val.null=1;
    }
    break;

  case 25:
#line 531 "cimXmlResp.y"
    {
       yyval.xtokProperty.val.array=yyvsp[-3].xtokValueArray;
    }
    break;

  case 26:
#line 535 "cimXmlResp.y"
    {
       yyval.xtokProperty.val.null=1;
    }
    break;

  case 27:
#line 542 "cimXmlResp.y"
    {
    }
    break;

  case 28:
#line 545 "cimXmlResp.y"
    {
       yyval.xtokPropertyData.value=yyvsp[0].xtokValue.value;
    }
    break;

  case 29:
#line 549 "cimXmlResp.y"
    {
       yyval.xtokPropertyData.ref=yyvsp[0].xtokValueReference;
    }
    break;

  case 30:
#line 562 "cimXmlResp.y"
    {
       yyval.xtokValue.value=yyvsp[-1].xtokValue.value;
    }
    break;

  case 31:
#line 569 "cimXmlResp.y"
    {
       yyval.xtokValueArray.next=1;
       yyval.xtokValueArray.max=64;
       yyval.xtokValueArray.values=(char**)malloc(sizeof(char*)*64);
       yyval.xtokValueArray.values[0]=yyvsp[0].xtokValue.value;
    }
    break;

  case 32:
#line 576 "cimXmlResp.y"
    {
       yyval.xtokValueArray.values[yyval.xtokValueArray.next]=yyvsp[0].xtokValue.value;
       yyval.xtokValueArray.next++;
    }
    break;

  case 33:
#line 584 "cimXmlResp.y"
    {
       yyval.xtokValueReference.instancePath=yyvsp[-1].xtokInstancePath;
       yyval.xtokValueReference.type=typeValRef_InstancePath;
    }
    break;

  case 34:
#line 589 "cimXmlResp.y"
    {
       yyval.xtokValueReference.instanceName=yyvsp[-1].xtokInstanceName;
       yyval.xtokValueReference.type=typeValRef_InstanceName;
    }
    break;

  case 35:
#line 615 "cimXmlResp.y"
    {
       yyval.xtokQualifier.value=yyvsp[-1].xtokValue.value;
    }
    break;

  case 36:
#line 619 "cimXmlResp.y"
    {
//       $$.value=$2.value;
    }
    break;

  case 37:
#line 633 "cimXmlResp.y"
    {
       yyval.xtokLocalNameSpacePath=yyvsp[-1].xtokNameSpace.cns;
    }
    break;

  case 38:
#line 640 "cimXmlResp.y"
    {
       yyval.xtokNameSpace.cns=strdup(yyvsp[-1].xtokNameSpace.ns);
    }
    break;

  case 39:
#line 644 "cimXmlResp.y"
    {
       int l=strlen(yyvsp[-2].xtokNameSpace.cns)+strlen(yyvsp[-1].xtokNameSpace.ns)+2;
       yyval.xtokNameSpace.cns=(char*)malloc(l);
       strcpy(yyval.xtokNameSpace.cns,yyvsp[-2].xtokNameSpace.cns);
       strcat(yyval.xtokNameSpace.cns,"/");
       strcat(yyval.xtokNameSpace.cns,yyvsp[-1].xtokNameSpace.ns);
       free(yyvsp[-2].xtokNameSpace.cns);
    }
    break;

  case 40:
#line 657 "cimXmlResp.y"
    {
       yyval.xtokNameSpacePath.host=yyvsp[-2].xtokHost;
       yyval.xtokNameSpacePath.nameSpacePath=yyvsp[-1].xtokLocalNameSpacePath;
    }
    break;

  case 41:
#line 665 "cimXmlResp.y"
    {
    }
    break;

  case 42:
#line 671 "cimXmlResp.y"
    {
       yyval.xtokInstancePath.path=yyvsp[-2].xtokNameSpacePath;
       yyval.xtokInstancePath.instanceName=yyvsp[-1].xtokInstanceName;
       yyval.xtokInstancePath.type=1;
    }
    break;

  case 43:
#line 714 "cimXmlResp.y"
    {
       yyval.xtokInstanceName.className=yyvsp[-1].xtokInstanceName.className;
       yyval.xtokInstanceName.bindings.next=0;
       yyval.xtokInstanceName.bindings.keyBindings=NULL;
       PARM->curPath=NULL;
    }
    break;

  case 44:
#line 721 "cimXmlResp.y"
    {
       yyval.xtokInstanceName.className=yyvsp[-2].xtokInstanceName.className;
       yyval.xtokInstanceName.bindings=yyvsp[-1].xtokKeyBindings;
       createPath(&(PARM->curPath), &yyval.xtokInstanceName);
    }
    break;

  case 45:
#line 730 "cimXmlResp.y"
    {
       yyval.xtokKeyBindings.next=1;
       yyval.xtokKeyBindings.max=16;
       yyval.xtokKeyBindings.keyBindings=(XtokKeyBinding*)malloc(sizeof(XtokKeyBinding)*16);
       yyval.xtokKeyBindings.keyBindings[0].name=yyvsp[0].xtokKeyBinding.name;
       yyval.xtokKeyBindings.keyBindings[0].value=yyvsp[0].xtokKeyBinding.value;
       yyval.xtokKeyBindings.keyBindings[0].type=yyvsp[0].xtokKeyBinding.type;
       yyval.xtokKeyBindings.keyBindings[0].ref=yyvsp[0].xtokKeyBinding.ref;
    }
    break;

  case 46:
#line 740 "cimXmlResp.y"
    {
       yyval.xtokKeyBindings.keyBindings[yyval.xtokKeyBindings.next].name=yyvsp[0].xtokKeyBinding.name;
       yyval.xtokKeyBindings.keyBindings[yyval.xtokKeyBindings.next].value=yyvsp[0].xtokKeyBinding.value;
       yyval.xtokKeyBindings.keyBindings[yyval.xtokKeyBindings.next].type=yyvsp[0].xtokKeyBinding.type;
       yyval.xtokKeyBindings.keyBindings[yyval.xtokKeyBindings.next].ref=yyvsp[0].xtokKeyBinding.ref;
       yyval.xtokKeyBindings.next++;
    }
    break;

  case 47:
#line 751 "cimXmlResp.y"
    {
       yyval.xtokKeyBinding.name=yyvsp[-3].xtokKeyBinding.name;
       yyval.xtokKeyBinding.value=yyvsp[-2].xtokKeyValue.value;
       yyval.xtokKeyBinding.type=yyvsp[-2].xtokKeyValue.valueType;
    }
    break;

  case 48:
#line 757 "cimXmlResp.y"
    {
       yyval.xtokKeyBinding.name=yyvsp[-2].xtokKeyBinding.name;
       yyval.xtokKeyBinding.value=NULL;
       yyval.xtokKeyBinding.type="ref";
       yyval.xtokKeyBinding.ref=yyvsp[-1].xtokValueReference;
    }
    break;


    }

/* Line 999 of yacc.c.  */
#line 1696 "cimXmlResp.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 765 "cimXmlResp.y"


