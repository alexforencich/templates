AC_PREREQ(2.57)

AC_INIT([template], [0.1.0])

AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_INSTALL

case "${host}" in
        i[[3456789]]86-mingw32*) WIN32="yes" ;;
        *cygwin*) WIN32="yes" ;;
        *) WIN32="no" ;;
esac
AM_CONDITIONAL([WIN32], test "$WIN32" = "yes")

if test "$WIN32" = "yes"; then
        dnl what to do when building for windows
        
        % ...
fi

PKG_CHECK_MODULES([DEPS], [gtkmm-2.4 >= 2.24.0])

AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

