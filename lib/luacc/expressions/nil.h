#pragma once

#include "expressions/expression.h"

namespace LuaCC
{
    class NilExpression : public Expression
    {
        public:
            explicit NilExpression()
                {}

            virtual llvm::Value* GenerateCode();
        private:
    };
}
