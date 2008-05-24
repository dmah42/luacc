/*
 *  binary.cpp
 *  luacc
 *
 *  Created by Dominic Hamon on 3/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parser.h"
#include "core/assert.h"
#include "expressions/binary.h"

#include "llvm/Support/IRBuilder.h"

using namespace llvm;

namespace LuaCC
{
    Value* BinaryExpression::GenerateCode()
    {        
        Value* pLHS = mpLHS->GenerateCode();
        Value* pRHS = mpRHS->GenerateCode();
        
        IRBuilder* pBuilder = Parser::Instance().GetBuilder();
        
        Value* pValue = NULL;
        
        switch(mOp)
        {
            case OP_MUL:
                pValue = pBuilder->CreateMul(pLHS, pRHS, "multmp");
                break;
            case OP_DIV:
                if( isa<ConstantInt>(pLHS) && isa<ConstantInt>(pRHS) )
                {
                    pValue = pBuilder->CreateSDiv(pLHS, pRHS, "sdivtmp");
                }
                else if( isa<ConstantFP>(pLHS) && isa<ConstantFP>(pRHS) )
                {
                    pValue = pBuilder->CreateFDiv(pLHS, pRHS, "fdivtmp");
                }
                else
                {
                    AssertMess(false, "Incompatible types in division");
                }
                break;
            case OP_ADD:
                pValue = pBuilder->CreateAdd(pLHS, pRHS, "addtmp");
                break;
            case OP_SUB:
                pValue = pBuilder->CreateSub(pLHS, pRHS, "subtmp");
                break;
        };
        
        AssertMess(pValue, "Unable to create Value for BinaryExpression");

        return pValue;
    }
}
