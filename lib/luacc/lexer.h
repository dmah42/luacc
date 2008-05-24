#pragma once

#include <cassert>
#include <stdio.h>
#include <string>

namespace LuaCC
{
    class Lexer
    {
        public:
        
            #undef DEF_TOKEN
            #define DEF_TOKEN(x)    TOKEN_##x,
        
            enum Token
            {
                NULL_TOKEN = -100,
                
                #include "tokens.h"
                
                NUM_OPERATORS
            };
        
            union Value
            {
                char    id[128];
                float   f;
                int     i;
                bool    b;
            };

            Lexer(FILE* pStream);
            Lexer(const std::string& rFilename);

            void                GetNextToken();
            Token               PeekNextToken();
		
            Token               GetToken() const	{ return mToken; }
            const Value&        GetValue() const	{ return mValue; }    
            unsigned int        GetLine() const		{ return mLine; }
            const std::string&	GetFile() const		{ return mFilename; }
		
            #if ENABLE_OPTIMIZED == 0
                static const char*  GetTokenName(const Token token)  
                { 
                    return mpTokenName[token - NULL_TOKEN]; 
                }
                
                const char*         GetTokenName() const             
                { 
                    return Lexer::GetTokenName(mToken); 
                }
            #else
                #define InitializeTokenNames()  do{ void(0) } while(0)
                #define GetTokenName(x)         ""
            #endif

        private:

            std::string GetWord() const;
            Token       InterpretToken(const std::string& identifier, 
                                       bool update);

            #if ENABLE_OPTIMIZED == 0
                static void         InitialiseTokenNames();
        
                static const char*  mpTokenName[NUM_OPERATORS - NULL_TOKEN];
                static bool         mInitialisedTokenNames;
            #endif
        
            FILE*         mpStream;
            Token         mToken;
            Value         mValue;
            std::string		mFilename;
            unsigned int	mLine;
    };
}
