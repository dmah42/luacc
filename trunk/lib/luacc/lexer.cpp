#include <cassert>
#include <stdlib.h>

#include "core/assert.h"
#include "core/error.h"
#include "lexer.h"

using namespace LuaCC;

Lexer::Lexer(const std::string& rFilename)
:	mFilename(rFilename),
	mLine(1)
{
    AssertMess(mFilename.length() > 0, "Invalid filename");
	
    mpStream = fopen(mFilename.c_str(), "r");

	AssertMess(mpStream, "Unable to open file");
	
    InitialiseTokenNames();
}

Lexer::Lexer(FILE* pStream)
:	mpStream(pStream),
	mFilename(""),
	mLine(1)
{
    Assert(mpStream);
	
    InitialiseTokenNames();
}

#if ENABLE_OPTIMIZED == 0

    bool        Lexer::mInitialisedTokenNames = false;
    const char* Lexer::mpTokenName[NUM_OPERATORS - NULL_TOKEN];

    void Lexer::InitialiseTokenNames()
    {
        if( !mInitialisedTokenNames)
        {
            #undef DEF_TOKEN
            #define DEF_TOKEN(x)                    \
                case TOKEN_##x:                     \
                    mpTokenName[i] = "TOKEN_" #x;   \
                    break;                          \
        
            const int numOps = NUM_OPERATORS - NULL_TOKEN;
            for( int i = 0; i < numOps; ++i )
            {
                const Token tok = (Token) (i + NULL_TOKEN);
                
                switch(tok)
                {
                    case NULL_TOKEN:
                        mpTokenName[i] = "NULL_TOKEN";
                        break;
                        
                    #include "tokens.h"
                        
                    default:
                        AssertMess(false, "Unknown token");
                        break;
                }
            }
            
            mInitialisedTokenNames = true;
        }
    }

#endif

std::string Lexer::GetWord() const
{
    std::string word = "";
    
    char lastChar = fgetc(mpStream);
    while( lastChar == ' ' || lastChar == '\t' )
    {
        // skip over spaces and tabs
        lastChar = fgetc(mpStream);
    }
    
    if( lastChar == EOF || lastChar == '\n' )
    {
        word = lastChar;
    }
    else 
    {
        
        #define UpdateWord()    word += lastChar;           \
                                lastChar = fgetc(mpStream);
        
        if( isalpha(lastChar) )
        {
            UpdateWord();
                
            while(isalnum(lastChar) || lastChar == '_')
            {
                UpdateWord();
            }
        }
        else if(isdigit(lastChar) || lastChar == '.')
        {
            UpdateWord();
                
            while(isdigit(lastChar) || lastChar == '.')
            {
                UpdateWord();
            }
        }
        else
        {
            while(!isalnum(lastChar) && 
                    lastChar != ' ' &&
                    lastChar != EOF )
            {
                UpdateWord();
            }
        }
            
        #undef UpdateWord
        
        ungetc(lastChar, mpStream);
    }
    
    return word;
}    

void Lexer::GetNextToken()
{
    AssertMess(mpStream != NULL, "Invalid stream in Lexer");

    const Token token = InterpretToken(GetWord(), true);

//    if( token == TOKEN_BLOCKCOMMENT )
//    {
//        // skip to end of block comment
//        std::string word = "";
//
//        do
//        {
//            word = GetWord();
//        } 
//        while(word != "]]");
//    }
//    else if( token == TOKEN_COMMENT )
//    {
//        // skip to end of line
//        char lastChar = getc(mpStream);
//        while(lastChar != '\n' && lastChar != '\r' && lastChar != EOF)
//        {
//            lastChar = getc(mpStream);
//        }
//    }
//    
    mToken = token;
}

Lexer::Token Lexer::PeekNextToken()
{
    AssertMess(mpStream != NULL, "Invalid stream in Lexer");
  
    const std::string word = GetWord();
      
    const Token token = InterpretToken(word, false);
  
    const int len = word.length();
    
    for( int i = len - 1; i >= 0; --i )
    {
        ungetc(word[i], mpStream);
    }
    
    return token;
}

Lexer::Token Lexer::InterpretToken(const std::string& identifier, bool update)
{
    Token token = NULL_TOKEN;

//    if( identifier[0] == '-' && identifier[1] == '-' )
//    {
//        if( identifier[2] == '[' && identifier[3] == ']' )
//        {
//            token = TOKEN_BLOCKCOMMENT;
//        }
//        else
//        {
//            token = TOKEN_COMMENT;
//        }
//    }
//    // commands/identifiers
//    else 
    if( isalpha(identifier[0]) )
    {
        if(identifier == "function")
        {
            token = TOKEN_FUNCTION;
        }
        else if( identifier == "local" )
        {
            token = TOKEN_LOCAL;
        }
        else if( identifier == "return" )
        {
            token = TOKEN_RETURN;
        }
        else if( identifier == "end" )
        {
            token = TOKEN_END;
        }
        else if( identifier == "nil" )
        {
            token = TOKEN_NIL;
        }
        else if( identifier == "false" )
        {   
            token = TOKEN_BOOLEAN;
            if( update )
            {
                mValue.b = false;
            }
        }
        else if( identifier == "true" )
        {
            token = TOKEN_BOOLEAN;
            if( update )
            {
                mValue.b = true;
            }
        }
        else
        {
            token = TOKEN_IDENTIFIER;
            if( update )
            {
                strncpy(mValue.id, identifier.c_str(), 127);
            }
        }
    }
    // numbers
    else if( isdigit(identifier[0]) || identifier[0] == '.' )
    {
        std::string::size_type loc = identifier.find(".", 0);
        
        if(loc == std::string::npos)
        {
            token = TOKEN_INTEGER;
            if( update )
            {
                mValue.i = strtol(identifier.c_str(), NULL, 0);
            }
        }
        else
        {
            if(identifier.find(".", loc + 1) != std::string::npos)
            {
                Error(Error::LEVEL_ERROR, GetLine(), GetFile(),
                      "Malformed floating point value '%s'", 
                      identifier.c_str());
            }

            token = TOKEN_FLOAT;
            if( update )
            {
                mValue.f = strtof(identifier.c_str(), NULL);
            }
        }
    }
    // end of file
    else if( identifier[0] == EOF )
    {
        token = TOKEN_EOF;
    }
    else if( identifier[0] == '\n' )
    {
        token = TOKEN_EOL;
        if( update )
        {
            ++mLine;
        }
    }
    else if( identifier[0] == '(' )
    {
        Assert(identifier.length() == 1);
        token = TOKEN_LPAREN;
    }
    else if( identifier[0] == ')' )
    {
        Assert(identifier.length() == 1);
        token = TOKEN_RPAREN;
    }
    else if( identifier[0] == '=' && identifier.length() == 1 )
    {
        token = TOKEN_EQUALS;
    }
    // any other token is assumed operator
    else
    {
        token = TOKEN_OPERATOR;
        if( update )
        {
            strncpy(mValue.id, identifier.c_str(), 127);
        }
    }

    return token;
}
