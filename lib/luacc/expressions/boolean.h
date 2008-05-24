#pragma once

#include "expressions/expression.h"

namespace LuaCC
{
    class BooleanExpression : public Expression
    {
        public:
            explicit BooleanExpression(const bool val)
                : mValue(val)
                {}
        
            virtual llvm::Value*  GenerateCode();
        
        private:
            bool mValue;
    };
}
