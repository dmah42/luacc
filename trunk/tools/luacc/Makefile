##===- projects/sample/tools/sample/Makefile ---------------*- Makefile -*-===##

#
# Indicate where we are relative to the top of the source tree.
#
LEVEL=../..

#
# Give the name of the tool.
#
TOOLNAME=LuaTest

#
# List libraries that we'll need
# We use LIBS because sample is a dynamic library.
#
USEDLIBS = luacc.a

#
# Link components
#
LINK_COMPONENTS = support interpreter jit codegen core system target transformutils

#
# Include Makefile.common so we know what to do.
#
include $(LEVEL)/Makefile.common

