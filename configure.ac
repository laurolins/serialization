#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([serialize], [0.0.1], [lauro.lins@gmail.com])
AC_CONFIG_SRCDIR([src/Serializer.cpp])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign])
LT_INIT

dnl # Checks for programs.
dnl case $host_os in
dnl     darwin*)
dnl         if test -z "$CXX"
dnl         then
dnl             CXX="clang++ -stdlib=libc++ -mmacosx-version-min=10.7"
dnl         fi
dnl     ;;
dnl esac

AC_PROG_CXX

AC_LANG([C++])

AX_CXX_COMPILE_STDCXX_11(noext)

# Checks for header files.
AC_CHECK_HEADERS([stdint.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 include/serialization/Makefile
                 src/Makefile
                 examples/Makefile])
AC_OUTPUT
