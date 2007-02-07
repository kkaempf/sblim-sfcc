#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT(sblim-sfcc-cpp, 1.0.2, <sblim-sfcc-cpp@lists.sourceforge.net>)
AC_CONFIG_SRCDIR([CimClient.cpp])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.
# FIXME: Replace `main' with a function in `-ldl':
AC_CHECK_LIB([dl], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string.h strings.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_CHECK_FUNCS([strcasecmp])

AC_CONFIG_FILES([Makefile sblim-sfcc-cpp.spec])
AC_OUTPUT
