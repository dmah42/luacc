#pragma once

#include "expressions/expression.h"


namespace LuaCC
{
    class FloatExpression : public Expression
    {
        public:
            explicit FloatExpression(const float val)
                : mValue(val)
                {}
        
            virtual llvm::Value*  GenerateCode();
        
        private:
            float mValue;
    };
}
