#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([clboss], [0.11A], [ZmnSCPxj@protonmail.com])
AC_CONFIG_AUX_DIR([auxdir])
AM_INIT_AUTOMAKE([subdir-objects tar-ustar])
AC_CONFIG_SRCDIR([main.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])
LT_INIT([disable-shared])

# Checks for programs.
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11
AC_LANG([C++])

AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for libraries.
AX_PTHREAD([:],[
	AC_MSG_ERROR([Need pthread.])
])
AX_LIB_EV
PKG_CHECK_MODULES([SQLITE3], [sqlite3 >= 3.0.0])
AX_LIB_CURL(,[:],[
	AC_MSG_ERROR([Need libcurl.])
])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
CLBOSS_CXXFLAGS=''
AC_SUBST([CLBOSS_CXXFLAGS])
# Determine if compiling ev.h succeeds with -Wno-noexcept-type
# On FreeBSD ev.h compilation fails without this flag.
AC_MSG_CHECKING([if compiler compiles ev.h with -Wno-noexcept-type])
saved_CXXFLAGS="${CXXFLAGS}"
CXXFLAGS="${CXXFLAGS} ${libev_CFLAGS} -Wno-noexcept-type"
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#include<ev.h>
]])], [ # then
    CLBOSS_CXXFLAGS="${CLBOSS_CXXFLAGS} -Wno-noexcept-type"
    AC_MSG_RESULT([yes])
], [ # else
    AC_MSG_RESULT([no])
])
CXXFLAGS="${saved_CXXFLAGS}"

# Determine if the compiler supports __attribute__((format (printf, 2, 3)))
AC_MSG_CHECKING([[if compiler supports __attribute__((format))]])
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
int fake_printf(void* dummy, char const* fmt, ...)
  __attribute__((format (printf, 2, 3)));
]])], [# then
    AC_DEFINE([HAVE_ATTRIBUTE_FORMAT], [1], [Define if understands gcc __attribute__((format))])
    AC_MSG_RESULT([yes])
], [ # else
    AC_MSG_RESULT([no])
])

# Checks for library functions.

AC_CONFIG_FILES([Makefile external/bitcoin-ripemd160/Makefile])
AC_CONFIG_SUBDIRS([external/libsodium
		   external/secp256k1
		  ])

AC_OUTPUT
