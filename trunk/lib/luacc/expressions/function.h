#pragma once

#include "expressions/expression.h"

namespace LuaCC
{
    class PrototypeExpression;

    class FunctionExpression : public Expression
    {
        public:
            explicit FunctionExpression(PrototypeExpression* pProto, Expression* pBody)
                :   mpProto(pProto),
                    mpBody(pBody)
                {}

        private:
            PrototypeExpression*        mpProto;
            Expression*                 mpBody;
    }
}

