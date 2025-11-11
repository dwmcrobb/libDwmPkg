AC_INIT([libDwmPkg],[m4_esyscmd_s(./getvers.sh -v)],[dwmcrobb@me.com],[libDwmPkg],[http://www.mcplex.net])
AC_PREFIX_DEFAULT(/usr/local)

AC_PROG_CC
AC_PROG_CXX
AC_CANONICAL_TARGET

LT_INIT([])

dnl  ###  Read some generally useful macros.
builtin(include, ./dwm.m4)  dnl
builtin(include, ax_check_compile_flag.m4) dnl

dnl  Set variables for building shared library
DWM_SET_CXX_SHARED_FLAGS

dnl  Set variables for pthreads
DWM_SET_PTHREADFLAGS

OSLIBS=""
LIBTOOL="libtool"

case $host_os in
  freebsd1[[01234]]*)
    CXXFLAGS="-O2"
    LDFLAGS=""
    ;;
  linux*)
    CXXFLAGS="-O2"
    DEBARCH=`dpkg --print-architecture`
    AC_SUBST(DEBARCH)
    ;;
  darwin*)
    CC="clang++"
    CXX="clang++"
    CXXFLAGS="${CXXFLAGS} -stdlib=libc++"
    LDFLAGS="${LDFLAGS} -g -O2 -stdlib=libc++"
    LIBTOOL="glibtool"
    ;;
esac
AC_SUBST(LIBTOOL)

echo "host_os ${host_os}"

DWM_REQUIRES_DWMPKG(dwmgmk,0.0.0)

dnl  Check for C++20
DWM_CHECK_CPLUSPLUS_20

AC_SUBST(PC_REQ_PKGS)

PKG_EXTINCS=`pkg-config --cflags-only-I ${PC_PKGS}`
PKG_EXTLIBS=`pkg-config --libs ${PC_PKGS}`

EXTINCS=`dwmgmk_uniqleft ${PKG_EXTINCS} ${EXTINCS}`

AC_SUBST(EXTINCS)
AC_SUBST(EXTLIBS)
AC_SUBST(LDFLAGS)
AC_SUBST(OSLIBS)
AC_SUBST(PKG_EXTINCS)
AC_SUBST(PKG_EXTLIBS)

libPrefix="${prefix}"
if [[ -z "${libPrefix}" ]] || [[ "${libPrefix}" = "NONE" ]]; then
  libPrefix=/usr/local
fi

echo libPrefix ${libPrefix}

PKG_CFLAGS=`dwmgmk_uniqleft ${PTHREADCXXFLAGS} -I${libPrefix}/include/libDwmPkg ${EXTINCS}`
AC_SUBST(PKG_CFLAGS)

DWM_PREREQ_PATH(htmlman,share/htmlman,[HTML manpage path])

DWM_SET_PKGVARS(libDwmPkg,[libDwmPkg-0.0.0])
DWM_GET_TAG([libDwmPkg])

AC_CHECK_PROG([MANDOC], [mandoc], [mandoc], [])

AC_SUBST(CXXFLAGS)

DWM_CHECK_STD_FORMAT

BUILD_DOCS=""
AC_ARG_ENABLE([docs],[AS_HELP_STRING([--enable-docs],[build documentation])],
	      [BUILD_DOCS="yes"], [])
AC_SUBST(BUILD_DOCS)

DWM_PKG_STATUS=`./getvers.sh -S`
AC_SUBST(DWM_PKG_STATUS)

AC_CONFIG_FILES([Makefile.vars packaging/libDwmPkg.pc
                 packaging/fbsd_manifest packaging/debcontrol doc/Doxyfile
		 classes/include/DwmPkgInfo.hh])
AC_OUTPUT
