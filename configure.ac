AC_INIT([installwatch], [0.8.0rc2], [ratler@lunar-linux.org])
LT_INIT
AM_INIT_AUTOMAKE([dist-xz])
AC_ENABLE_SHARED
AC_DISABLE_STATIC
AC_PROG_LIBTOOL(libtool)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADERS([config.h])
AH_TOP([#ifndef __CONFIG_H])
AH_TOP([#define __CONFIG_H])
AH_BOTTOM([#endif /*__CONFIG_H*/])
AC_PROG_CC
AC_PROG_EGREP
AC_PROG_SED
AC_PROG_AWK
AC_PROG_MKDIR_P
AC_PROG_INSTALL
AC_CHECK_TOOL(LDD, ldd)
AC_CHECK_TOOL(CHMOD, chmod)
AC_CHECK_HEADERS([sys/param.h \
                  fcntl.h \
                  stdarg.h \
                  stdio.h \
                  dlfcn.h \
                  syslog.h \
                  errno.h \
                  libgen.h \
                  inttypes.h \
                  time.h \
                  utime.h \
                  dirent.h])

local_cflags="$CFLAGS"

dnl Check what libc.so.X version we have and if it is supported
AC_MSG_CHECKING([for libc soname number])
AC_LINK_IFELSE([AC_LANG_SOURCE([[
  int main(void) { return 0; }
]])], [
  LIBC_SONAME=`$LDD ./conftest$EXEEXT | $EGREP libc\.so | $AWK '{print $1}'`
],[])
AC_MSG_RESULT([$LIBC_SONAME])

case "$LIBC_SONAME" in
     libc.so.5)
       AC_DEFINE(BROKEN_RTLD_NEXT, 1, [Broken rtld])
       AC_DEFINE(LIBC_FILE, "libc.so.5", [The libc filename])
       ;;
     libc.so.6)
       dnl Determine glibc version, if not possible assume 2.1
       AC_CHECK_HEADER([features.h])
       AC_MSG_CHECKING([for glibc version])
       glibc_version_major=2
       glibc_version_minor=1
       if test "x$ac_cv_header_features_h" = "xyes"; then
         rm -f conftest.$ac_ext
         cat <<_EOF >conftest.$ac_ext
#include <features.h>
#if defined(__GNU_LIBRARY__) && defined(__GLIBC__) && defined(__GLIBC_MINOR__)
glibc version: __GLIBC__ __GLIBC_MINOR__
#endif
_EOF
         glibc_version_number="`$CPP -P conftest.$ac_ext | $SED -n 's/^glibc version: //p' | $SED 's/ /./g'`"
         glibc_version_major=`echo "$glibc_version_number" | $AWK -F. '{print $1}'`
         glibc_version_minor=`echo "$glibc_version_number" | $AWK -F. '{print $2}'`
       fi
       AC_MSG_RESULT([$glibc_version_major.$glibc_version_minor])
       AC_DEFINE_UNQUOTED([GLIBC_MAJOR], [$glibc_version_major],
       [GNU C Library major version number used on the target, or 2.])
       AC_DEFINE_UNQUOTED([GLIBC_MINOR], [$glibc_version_minor],
       [GNU C Library minor version number used on the target, or 1.])
       ;;
     *) AC_MSG_ERROR("Unknown libc found.") ;;
esac

dnl Tests needs to validate compatible pointer types so set the proper -Werror CFLAGS
CFLAGS="-Werror=incompatible-pointer-types"
AC_MSG_CHECKING(wether truncate() argument type is size_t)
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM(
    [[#include <unistd.h>]],
    [[int (*f)(const char *, size_t) = truncate;]])
  ],[
    AC_MSG_RESULT(yes)
    AC_DEFINE(HAVE_SIZE_T_TRUNCATE, 1, [Test])
  ],[
  AC_MSG_RESULT(no)
  ])


AC_MSG_CHECKING(wether readlinkat() type is ssize_t)
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM(
    [[#include <unistd.h>]],
    [[ssize_t (*f)(int, const char *__restrict, char *__restrict, size_t) = readlinkat;]])
  ],[
    AC_MSG_RESULT(yes)
    AC_DEFINE(HAVE_SSIZE_T_READLINKAT, 1, [Test])
  ],[
    AC_MSG_RESULT(no)
  ])
CFLAGS=""

dnl Enable debug build
AC_ARG_ENABLE([debug],
[AS_HELP_STRING([--enable-debug],
[Switch on debugging mode])],
[],
[enable_debug=no])

AS_IF([test "x$enable_debug" != "xno"],
      [CFLAGS=$local_cflags" -g"
      AC_DEFINE(DEBUG, 1, [Enable debugging])
      ])

dnl Enable "access" syscall logging
AC_ARG_ENABLE([access-syscall],
  [AS_HELP_STRING([--enable-access-syscall],
  [Log access syscalls [default=yes]])],
  [],
  [enable_access_syscall=yes])
if test "x$enable_access_syscall" = "xyes"; then
  AC_DEFINE(ENABLE_ACCESS_SYSCALL, 1, [Log access syscalls])
fi

dnl Enable "acl" syscalls (setxattr, removexattr) logging
AC_ARG_ENABLE([acl-syscall],
  [AS_HELP_STRING([--enable-acl-syscall],
  [Log acl (setxattr, removexattr) syscalls [default=yes]])],
  [],
  [enable_acl_syscall=yes])
if test "x$enable_acl_syscall" = "xyes"; then
   AC_DEFINE(ENABLE_ACL_SYSCALL, 1, [Log acl syscalls])
fi

AC_OUTPUT
