/*
 *  float.cpp
 *  luacc
 *
 *  Created by Dominic Hamon on 3/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parser.h"
#include "expressions/variable.h"

namespace LuaCC
{
    llvm::Value* VariableExpression::GenerateCode()
    {
        return Parser::Instance().GetValueFromVariable(mName);
    }
}
