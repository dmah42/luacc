#pragma once

#include <string>
#include <stdio.h>

namespace LuaCC
{
    class LuaCC
    {
        public:
            static LuaCC&   Instance()  { return mInstance; }
            
            void            Execute(FILE* stream);
            void            Execute(const std::string& rFilename);
        
        private:
            static LuaCC    mInstance;
    };
}
