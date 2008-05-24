/*
 *  nil.cpp
 *  luacc
 *
 *  Created by Dominic Hamon on 3/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "expressions/nil.h"

#include "llvm/Constants.h"
#include "llvm/DerivedTypes.h"

namespace LuaCC
{
    llvm::Value* NilExpression::GenerateCode()
    {
        return llvm::ConstantInt::getNullValue(llvm::IntegerType::Int32Ty);
    }
}
