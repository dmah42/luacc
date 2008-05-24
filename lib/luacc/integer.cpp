/*
 *  integer.cpp
 *  luacc
 *
 *  Created by Dominic Hamon on 3/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "expressions/integer.h"

#include "llvm/Constants.h"
#include "llvm/DerivedTypes.h"

namespace LuaCC
{
    llvm::Value* IntegerExpression::GenerateCode()
    {        
        // 32-bit signed int with value mValue
        return llvm::ConstantInt::get(llvm::Type::Int32Ty, mValue, true);
    }
}
