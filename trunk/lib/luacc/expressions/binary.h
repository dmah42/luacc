#pragma once

#include "expressions/expression.h"

namespace LuaCC
{
    class BinaryExpression : public Expression
    {
        public:
            enum Operator
            {
                OP_MUL,
                OP_DIV,
                OP_ADD,
                OP_SUB
            };
        
            explicit BinaryExpression(const Operator op, Expression* pLHS, Expression* pRHS)
                :   mOp(op),
                    mpLHS(pLHS),
                    mpRHS(pRHS)
                {}
        
            virtual llvm::Value* GenerateCode();
        
        private:
            Operator    mOp;
            Expression* mpLHS;
            Expression* mpRHS;
    };
}
