/*
 *  boolean.cpp
 *  luacc
 *
 *  Created by Dominic Hamon on 3/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "expressions/boolean.h"
#include "llvm/Constants.h"

namespace LuaCC
{
    llvm::Value* BooleanExpression::GenerateCode()
    {
        return (mValue) ? llvm::ConstantInt::getTrue() : llvm::ConstantInt::getFalse();
    }
}
