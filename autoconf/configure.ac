dnl **************************************************************************
dnl * Initialize
dnl **************************************************************************
AC_INIT([[[LUACC]]],[[[0.10]]],[bugs@runtime.nu])

dnl Identify where LLVM source tree is
LLVM_SRC_ROOT="../../"
LLVM_OBJ_ROOT="../../"
dnl Tell autoconf that the auxilliary files are actually located in
dnl the LLVM autoconf directory, not here.
AC_CONFIG_AUX_DIR($LLVM_SRC_ROOT/autoconf)

dnl Tell autoconf that this is an LLVM project being configured
dnl This provides the --with-llvmsrc and --with-llvmobj options
LLVM_CONFIG_PROJECT($LLVM_SRC_ROOT,$LLVM_OBJ_ROOT)

dnl Verify that the source directory is valid
AC_CONFIG_SRCDIR(["Makefile.common.in"])

dnl Configure a common Makefile
AC_CONFIG_FILES(Makefile.common)

dnl Configure project makefiles
dnl List every Makefile that exists within your source tree
AC_CONFIG_MAKEFILE(Makefile)
AC_CONFIG_MAKEFILE(lib/Makefile)
AC_CONFIG_MAKEFILE(lib/luacc/Makefile)
AC_CONFIG_MAKEFILE(tools/Makefile)
AC_CONFIG_MAKEFILE(tools/luacc/Makefile)

dnl **************************************************************************
dnl * Determine which system we are building on
dnl **************************************************************************

dnl **************************************************************************
dnl * Check for programs.
dnl **************************************************************************

dnl **************************************************************************
dnl * Check for libraries.
dnl **************************************************************************

dnl **************************************************************************
dnl * Checks for header files.
dnl **************************************************************************

dnl **************************************************************************
dnl * Checks for typedefs, structures, and compiler characteristics.
dnl **************************************************************************

dnl **************************************************************************
dnl * Checks for library functions.
dnl **************************************************************************

dnl **************************************************************************
dnl * Enable various compile-time options
dnl **************************************************************************

dnl **************************************************************************
dnl * Set the location of various third-party software packages
dnl **************************************************************************

dnl **************************************************************************
dnl * Create the output files
dnl **************************************************************************

dnl This must be last
AC_OUTPUT
