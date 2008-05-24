#pragma once

#include <string>
#include "expressions/expression.h"

namespace LuaCC
{
    class StringExpression : public Expression
    {
        public:
            explicit StringExpression(const std::string& val)
                : mValue(val)
                {}

        private:
            std::string mValue;
    }
}
