#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([chat-client/include/net/ChatClient.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_CHECK_FUNCS([inet_ntoa localtime_r memset socket strerror])

AC_CONFIG_FILES([cmake-build-debug/Makefile
                 cmake-build-debug/chat-client/Makefile
                 cmake-build-debug/chat-server/Makefile
                 cmake-build-debug/chat-shared/Makefile])
AC_OUTPUT
