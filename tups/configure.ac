#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([tupsbackend], [1.0], [ohmcoeco@gmail.com])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/compile.h])
AC_CONFIG_HEADERS([config.h])

#Automake
AM_INIT_AUTOMAKE([1.11 -Wall subdir-objects foreign])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AM_PROG_LEX
AC_LANG([C++])

#beanstalk client check
#AX_CXX_CHECK_LIB(beanstalk, [Beanstalk::Client], [], [
#  AC_MSG_ERROR([unable to find the client/beanstalk library])
#])

PKG_CHECK_MODULES([LIBBEANSTALK], [libbeanstalk = 1.3.0], [], [
  AC_MSG_ERROR([libbeanstalk-1.3.0 is required])
])

#Check for libodb
PKG_CHECK_MODULES([LIBODB], [libodb = 2.3.0], [], [
  AC_MSG_ERROR([libodb-2.3.0 is required])
])

PKG_CHECK_MODULES([LIBODBMYSQL], [libodb-mysql = 2.3.0], [], [
  AC_MSG_ERROR([libodb-mysql-2.3.0 is required])
])

PKG_CHECK_MODULES([LIBSANDBOX], [libsandbox >= 0.3.0], [], [
  AC_MSG_ERROR([at least libsandbox 0.3.0 is required])
])

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT16_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([dup2 getpagesize mkdir])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
