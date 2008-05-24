/*
 *  float.cpp
 *  luacc
 *
 *  Created by Dominic Hamon on 3/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "expressions/float.h"

#include "llvm/Constants.h"

namespace LuaCC
{
    llvm::Value* FloatExpression::GenerateCode()
    {
        return llvm::ConstantFP::get(llvm::APFloat(mValue));
    }
}
