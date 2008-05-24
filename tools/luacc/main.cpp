#include <cassert>
#include <stdio.h>

#include "luacc/luacc.h"

int main (int argc, char ** argv)
{
    assert(argc <= 2 && "invalid args");
    
    if( argc == 2 )
    {
        printf("Parsing file '%s'\n", argv[1]);
        
        LuaCC::LuaCC::Instance().Execute(argv[1]);
    }
    else
    {
        printf("Using stdin\n");
    
        while(true)
        {
            fprintf(stderr, "%%> ");
        
            LuaCC::LuaCC::Instance().Execute(stdin);
        }
    }

    return 0;
}

