#pragma once

#include <string>
#include <vector>
#include "expressions/expression.h"

namespace LuaCC
{
    class PrototypeExpression : public Expression
    {
        public:
            explicit PrototypeExpression(const std::string& name, std::vector<std::string>& args)
                :   mName(name),
                    mArgs(args)
                {}

        private:
            std::string                 mName;
            std::vector<std::string>    mArgs;
    }
}

