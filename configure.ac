# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)
AC_INIT(sinsy, 0.92, http://sinsy.sourceforge.net/)
AC_CONFIG_AUX_DIR([config])
AC_COPYRIGHT(Copyright 2009-2015 Nagoya Institute of Technology)
AM_INIT_AUTOMAKE(subdir-objects)

# Checks for C compiler
AC_PROG_CXX
AC_SUBST([CXXFLAGS], ["-std=c++11"])
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_RANLIB
AN_MAKEVAR([AR], [AC_PROG_AR])
AN_PROGRAM([ar], [AC_PROG_AR])
AC_DEFUN([AC_PROG_AR], [AC_CHECK_TOOL(AR, ar, :)])
AC_PROG_AR

# Checks for libraries.
AC_CHECK_LIB([m], [log])


# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])


# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_CHECK_SIZEOF([char])
AC_CHECK_SIZEOF([unsigned char])
AC_CHECK_SIZEOF([short])
AC_CHECK_SIZEOF([unsigned short])
AC_CHECK_SIZEOF([int])
AC_CHECK_SIZEOF([unsigned int])
AC_CHECK_SIZEOF([long])
AC_CHECK_SIZEOF([unsigned long])
AC_CHECK_SIZEOF([long long])
AC_CHECK_SIZEOF([unsigned long long])

# Checks for library functions.
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([sqrt strchr strrchr strstr])

AC_CANONICAL_HOST
AC_C_BIGENDIAN

AC_CONFIG_FILES([Makefile lib/Makefile bin/Makefile dic/Makefile])

AC_OUTPUT
