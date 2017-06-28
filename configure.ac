#
#
#	ffuzzy++ : C++ implementation of fast fuzzy hashing
#
#	configure.ac
#	Configuration script
#
#	Copyright (C) 2017 Tsukasa OI.
#
#
#	This program is free software; you can redistribute it and/or modify it
#	under the terms of the GNU General Public License as published by the
#	Free Software Foundation, either version 3 of the License, or (at your
#	option) any later version.
#
#	This program is distributed in the hope that it will be useful, but
#	WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	Under Section 7 of GPL version 3, you are granted additional
#	permissions described in the Autoconf Configure Script Exception,
#	version 3.0, as published by the Free Software Foundation.
#
#	You should have received a copy of the GNU General Public License
#	and a copy of the Autoconf Configure Script Exception along with
#	this program; see the files COPYING.GPLv3 and COPYING.GPLv3Autoconf
#	respectively.  If not, see <http://www.gnu.org/licenses/>.
#
#
AC_PREREQ([2.65])
AC_INIT([ffuzzypp], [4.0.1], [floss_ssdeep@irq.a4lg.com])
AC_CONFIG_SRCDIR([ffuzzy_declarations.cpp])
AC_CONFIG_AUX_DIR([ext])
AM_INIT_AUTOMAKE([foreign dist-xz])

dnl --disable-static-lib
AC_ARG_ENABLE([static-lib],
[AS_HELP_STRING([--disable-static-lib],[disable compiling static library libffuzzypp.a])],,
[enable_static_lib=yes])
AM_CONDITIONAL([ENABLE_STATIC_LIB],[test "x$enable_static_lib" != xno])

dnl --disable-debug
AC_ARG_ENABLE([debug],
[AS_HELP_STRING([--disable-debug],[disable debugging support on examples and tests])],,
[enable_debug=yes])
if test "x$enable_debug" != xno
then
AC_DEFINE([FFUZZYPP_DEBUG], [1], [enable testing this implementation])
fi

dnl --disable-position-array
AC_ARG_ENABLE([position-array],
[AS_HELP_STRING([--disable-position-array],[disable using bit-parallel algorithms on examples and tests])],,
[enable_position_array=yes])
if test "x$enable_position_array" = xno
then
AC_DEFINE([FFUZZYPP_DISABLE_POSITION_ARRAY], [1], [disable bit-parallel algorithms])
fi

dnl --enable-examples
AC_ARG_ENABLE([examples],
[AS_HELP_STRING([--enable-examples],[enable building examples (will not be installed)])],,
[enable_examples=no])
AM_CONDITIONAL([ENABLE_EXAMPLES],[test "x$enable_examples" != xno])

dnl --enable-tests
AC_ARG_ENABLE([tests],
[AS_HELP_STRING([--enable-tests],[enable runtime tests])],,
[enable_tests=no])
AM_CONDITIONAL([ENABLE_TESTS],[test "x$enable_tests" != xno])

dnl --disable-compatibility-tests
AC_ARG_ENABLE([compatibility-tests],
[AS_HELP_STRING([--disable-compatibility-tests],[disable compatibility tests])],,
[enable_compatibility_tests=yes])
AM_CONDITIONAL([ENABLE_COMPATIBILITY_TESTS],[test "x$enable_compatibility_tests" != xno])

dnl C++11 compilers and required libraries (when testing)
AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11
AC_PROG_CXXCPP
if test "x$enable_static_lib" != xno
then
AC_PROG_RANLIB
fi
if test "x$enable_tests" != xno
then
AC_CHECK_HEADER([gtest/gtest.h],,[AC_MSG_ERROR([gtest/gtest.h from Google Test is required to build tests.])])
AC_CHECK_LIB([gtest],[main],[:],[AC_MSG_ERROR([library gtest not found.])])
AC_CHECK_LIB([gtest_main],[main],[:],[AC_MSG_ERROR([library gtest_main not found.])])
fi

AC_OUTPUT([Makefile examples/Makefile tests/Makefile])
