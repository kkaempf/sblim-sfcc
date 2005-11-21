#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(sblim-sfcc, 0.7, sblim-devel@lists.sourceforge.net)
AC_CONFIG_SRCDIR([args.c])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_YACC

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([malloc.h stddef.h stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM

# Checks for libraries
AC_CHECK_LIB(curl,curl_easy_init)

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([gettimeofday localtime_r memset strcasecmp strdup strncasecmp strstr tzset])

# AUTOUNIT
AM_CONDITIONAL(HAVE_AUTOUNIT_CPP,[test 0 = 1 ])

AC_CONFIG_FILES([Makefile TEST/Makefile])
AC_OUTPUT
