#pragma once

namespace llvm
{
    class Value;
}

namespace LuaCC
{
    class Expression
    {
        public:
            virtual                 ~Expression()   {}
            virtual llvm::Value*    GenerateCode() = 0;
    };
}
