#pragma once

#include <string>
#include <vector>
#include "expressions/expression.h"

namespace LuaCC
{
    class CallExpression : public Expression
    {
        public:
            explicit CallExpression(const std::string& name, std::vector<Expression*>& args)
                :   mName(name),
                    mArgs(args)
                {}

        private:
            std::string                 mName;
            std::vector<Expression*>    mArgs;
    }
}

