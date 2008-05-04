#pragma once

#include <string>
#include "expressions/expression.h"

namespace LuaCC
{
    class VariableExpression : public Expression
    {
        public:
            explicit VariableExpression(const std::string& name)
                :   mName(name)
                {}
        
            virtual llvm::Value* GenerateCode();
        
        private:
            std::string mName;
    };
}
