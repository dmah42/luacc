#include <stdio.h>

/* LLVM Header File
#include "llvm/Support/DataTypes.h"
*/

#include "parser.h"

#include "luacc/luacc.h"

namespace LuaCC
{
    LuaCC LuaCC::mInstance;
    
    void LuaCC::Execute(FILE* stream)
    {
        Parser::Instance().Execute(stream);
    }
	
    void LuaCC::Execute(const std::string& rFilename)
    {
        Parser::Instance().Execute(rFilename);
    }
}
