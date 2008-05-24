#pragma once

#include "expressions/expression.h"

namespace LuaCC
{
    class IntegerExpression : public Expression
    {
        public:
            explicit IntegerExpression(const int val)
                : mValue(val)
                {}
        
            virtual llvm::Value*  GenerateCode();
        
        private:
            int mValue;
    };
}
