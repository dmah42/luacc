#include <set>

#include "core/assert.h"
#include "core/error.h"

#include "expressions/binary.h"
#include "expressions/boolean.h"
#include "expressions/integer.h"
#include "expressions/float.h"
#include "expressions/nil.h"
#include "expressions/variable.h"

#include "llvm/Analysis/Verifier.h"
#include "llvm/GlobalValue.h"
#include "llvm/GlobalVariable.h"
#include "llvm/Module.h"

#include "parser.h"

using namespace LuaCC;

Parser Parser::mInstance;

// integer  ::= {0-9} [{0-9}]*
// float	::= [integer] . [integer] 
// number   ::= integer | float
// primary  ::= 'nil' | 'true' | 'false' | number | '(' rvalue ')' | ident
Expression* Parser::ParsePrimary()
{    
    Expression* pExpr = NULL;
    
    switch(mpLexer->GetToken())
    {
        case Lexer::TOKEN_NIL:
            pExpr = new NilExpression();
            break;
            
        case Lexer::TOKEN_INTEGER:
            pExpr = new IntegerExpression(mpLexer->GetValue().i);
            break;
            
        case Lexer::TOKEN_FLOAT:
            pExpr = new FloatExpression(mpLexer->GetValue().f);
            break;
            
        case Lexer::TOKEN_BOOLEAN:
            pExpr = new BooleanExpression(mpLexer->GetValue().b);
            break;
            
        case Lexer::TOKEN_EOL:
            break;
            
        case Lexer::TOKEN_LPAREN:
            mpLexer->GetNextToken();
            
            pExpr = ParseRvalue();
            
            Assert(mpLexer->GetToken() == Lexer::TOKEN_RPAREN);
            break;
            
        case Lexer::TOKEN_IDENTIFIER:
            pExpr = new VariableExpression(mpLexer->GetValue().id);
            break;
            
        default:
            Error(Error::LEVEL_ERROR, mpLexer->GetLine(), mpLexer->GetFile(),
                  "Unexpected primary %d ['%s']", mpLexer->GetToken(), 
                  mpLexer->GetTokenName());
            break;
    }
    
    mpLexer->GetNextToken();
    
    return pExpr;
}

// chain    ::= primary [{ '*' | '/' } primary]
Expression* Parser::ParseChain()
{
    Expression* pChain = ParsePrimary();
    
    bool validChain = true;
    while( validChain && mpLexer->GetToken() == Lexer::TOKEN_OPERATOR )
    {
        BinaryExpression::Operator op;
        validChain = false;
        
        if( strcmp(mpLexer->GetValue().id, "*") == 0 )
        {
            op = BinaryExpression::OP_MUL;
            validChain = true;
        }
        else if( strcmp(mpLexer->GetValue().id, "/") == 0 )
        {
            op = BinaryExpression::OP_DIV;
            validChain = true;
        }
        
        if( validChain )
        {
            // get RHS
            mpLexer->GetNextToken();
            
            Expression* pLHS = pChain;
            Expression* pRHS = ParsePrimary();
            
            pChain = new BinaryExpression(op, pLHS, pRHS);
        }
    }
    
    return pChain;
}

// rvalue   ::= chain [{ '+' | '-' } chain]
Expression* Parser::ParseRvalue()
{    
    Expression* pRvalue = ParseChain();

    bool validRvalue = true;
    
    while( validRvalue && mpLexer->GetToken() == Lexer::TOKEN_OPERATOR )
    {
        BinaryExpression::Operator op;
        validRvalue = false;
        
        if( strcmp(mpLexer->GetValue().id, "+") == 0 )
        {
            op = BinaryExpression::OP_ADD;
            validRvalue = true;
        }
        else if( strcmp(mpLexer->GetValue().id, "-") == 0 )
        {
            op = BinaryExpression::OP_SUB;
            validRvalue = true;
        }
        
        if( validRvalue )
        {
            // get RHS
            mpLexer->GetNextToken();
            
            Expression* pLHS = pRvalue;
            Expression* pRHS = ParseChain();
            
            pRvalue = new BinaryExpression(op, pLHS, pRHS);
        }
    }
    
    return pRvalue;
}

llvm::Value* Parser::GetValueFromVariable(const std::string& rName)
{
    AssertMess(rName.length() > 0, "Invalid variable name");
	
    llvm::Value* pValue = NULL;
	
    // local first
    const LocalVarsConstIter localVarIter = mLocalVars.find(rName);
	
    if( localVarIter != mLocalVars.end() )
    {
        // create the load for the local variable
        pValue = mpBuilder->CreateLoad(localVarIter->second);
    }
    else
    {
        const GlobalVarsConstIter globalVarIter = mGlobalVars.find(rName);
		
        if( globalVarIter == mGlobalVars.end() )
        {
            Error(Error::LEVEL_ERROR, mpLexer->GetLine(), mpLexer->GetFile(), 
                  "Undefined variable '%s' access", rName.c_str());
        }
		else
        {
            const llvm::GlobalVariable* pGlobalVar = globalVarIter->second;
        
            Assert(pGlobalVar->hasInitializer());
        
            pValue = pGlobalVar->getInitializer();
        }
    }
	
    return pValue;
}

void Parser::CreateGlobalVariable(const std::string& rName, 
                                  llvm::Constant* pRHS, const bool isInternal)
{
    AssertMess(rName.length() > 0, "Invalid variable name");
    
    GlobalVarsIter varIter = mGlobalVars.find(rName);
    
    llvm::GlobalVariable* pGlobal = NULL;
    
    if( varIter == mGlobalVars.end() )
    {	
        const llvm::GlobalValue::LinkageTypes linkType = isInternal ? 
                                                         llvm::GlobalValue::InternalLinkage :
                                                         llvm::GlobalValue::WeakLinkage;
        
        // create a global variable for this variable
        pGlobal = new llvm::GlobalVariable(llvm::Type::Int32Ty, false, linkType, 
                                           pRHS, rName, mpModule);
        
        mGlobalVars[rName] = pGlobal;
    }
    else
    {
        // pull from symbol table
        pGlobal = varIter->second;
        
        // create the store into the global pointer
        pGlobal->setInitializer(pRHS);
    }
}

void Parser::CreateLocalVariable(const std::string& rName, llvm::Value* pRHS)
{
    AssertMess(rName.length() > 0, "Invalid variable name");
    AssertMess(pRHS, "Invalid initializer");
    
    // check if the variable is a global already and warn if so
    // TODO: check local vars lists
    const GlobalVarsConstIter globalVarIter = mGlobalVars.find(rName);
	
    if( globalVarIter != mGlobalVars.end() )
    {
        Error(Error::LEVEL_WARNING, mpLexer->GetLine(), mpLexer->GetFile(),
              "Masking global variable '%s'", rName.c_str());
    }
	
    llvm::AllocaInst* pAlloca = NULL;
    
    LocalVarsIter varIter = mLocalVars.find(rName);
    
    if( varIter != mLocalVars.end() )
    {
        // pull from the symbol table
        pAlloca = varIter->second;
    }
    else
    {
        // Create an alloca for this variable.
        pAlloca = mpBuilder->CreateAlloca(llvm::Type::Int32Ty, 0, rName.c_str());
        
        // add alloca to local variable symbol table.
        mLocalVars[rName] = pAlloca;
    }
                                           
    // Store the value into the alloca.
    mpBuilder->CreateStore(pRHS, pAlloca);
}

// assign   ::= ['local'] ident ['=' [ident '=']* rvalue]*
// TODO: variant type
// TODO: create local variables if we're inside a function and definition 
//       includes 'local'
Expression* Parser::ParseAssign()
{
    std::set<std::string> idNameSet;
    
    bool isInternal = false;
    
    if( mpLexer->GetToken() == Lexer::TOKEN_LOCAL )
    {
        const Lexer::Token& rNextToken = mpLexer->PeekNextToken();
            
        if( rNextToken != Lexer::TOKEN_IDENTIFIER )
        {
            Error(Error::LEVEL_ERROR, mpLexer->GetLine(), mpLexer->GetFile(),
                    "Expected identifier after 'local', got '%s'", 
                    Lexer::GetTokenName(rNextToken));
        }
            
        isInternal = true;
        
        // eat the 'local'
        mpLexer->GetNextToken();
    }
    
    if( mpLexer->GetToken() == Lexer::TOKEN_IDENTIFIER )
    {
        std::string idName = mpLexer->GetValue().id;
        
        idNameSet.insert(idName);
        
        mpLexer->GetNextToken();
        
        if( mpLexer->GetToken() == Lexer::TOKEN_EQUALS )
        {
            // eat the assignment
            mpLexer->GetNextToken();
        }
    }
    
    while( mpLexer->GetToken() == Lexer::TOKEN_IDENTIFIER &&
           mpLexer->PeekNextToken() == Lexer::TOKEN_EQUALS )
    {
        std::string idName = mpLexer->GetValue().id;
        idNameSet.insert(idName);

        if( isInternal )
        {
            // local first
            const LocalVarsConstIter localVarIter = mLocalVars.find(idName);
            
            if( localVarIter != mLocalVars.end() )
            {
                Error(Error::LEVEL_ERROR, mpLexer->GetLine(), mpLexer->GetFile(),
                      "Cannot redeclare variable '%s'", idName.c_str());
            }
            else
            {
                // try globals
                GlobalVarsConstIter globalVarIter = mGlobalVars.find(idName);
                
                if( globalVarIter != mGlobalVars.end() )
                {
                    Error(Error::LEVEL_ERROR, mpLexer->GetLine(), mpLexer->GetFile(),
                          "Cannot redeclare variable '%s'", idName.c_str());
                }
            }
        }
        
        // eat the identifier
        mpLexer->GetNextToken();

        Assert(mpLexer->GetToken() == Lexer::TOKEN_EQUALS);
        
        // eat the equals
        mpLexer->GetNextToken();
    }
    
    Expression* pRHS = ParseRvalue();
    
    if( idNameSet.size() > 0 )
    {
        llvm::Constant* pRHSconst = NULL;
        
        if(pRHS)
        {
            llvm::Value* pRHSvalue = pRHS->GenerateCode();
        
            pRHSconst = llvm::dyn_cast<llvm::Constant>(pRHSvalue);
        
            if( !pRHSconst )
            {
                Error(Error::LEVEL_ERROR, mpLexer->GetLine(), mpLexer->GetFile(),
                      "Attempt to assign non-const value to '%s'", 
                      idNameSet.begin()->c_str());
            }
        }
        else
        {
            // initialise variables to nil by default
            NilExpression* pNil = new NilExpression();
            
            llvm::Value* pNilValue = pNil->GenerateCode();
            
            pRHSconst = llvm::dyn_cast<llvm::Constant>(pNilValue);
            
            Assert(pRHSconst);
        }

        const std::set<std::string>::const_iterator end = idNameSet.end();
        for(std::set<std::string>::iterator iter = idNameSet.begin();
            iter != end; ++iter)
        {
            CreateGlobalVariable(*iter, pRHSconst, isInternal);
        }
        
        mpLexer->GetNextToken();
        
        // clear the expression now we've handled it
        pRHS = NULL;
    }
    
    return pRHS;
}

//void Parser::InitialiseGlobalFunc()
//{
//    // set up the anonymous global function
//    std::vector<const llvm::Type*> args(0, llvm::Type::VoidTy);    
//    llvm::FunctionType* pGlobalFuncType = llvm::FunctionType::get(llvm::Type::VoidTy, args, false);
//    
//    mpGlobalFunc = new llvm::Function(pGlobalFuncType, llvm::Function::ExternalLinkage, "", mpModule);    
//}

void Parser::Execute(FILE* stream)
{
    Assert(stream);
 
    mpLexer = new Lexer(stream);
            
    ExecutePrivate();
}

void Parser::Execute(const std::string& rFilename)
{
    Assert(rFilename.length() > 0);
	
    mpLexer = new Lexer(rFilename);
	
    ExecutePrivate();
}

void Parser::ExecutePrivate()
{	
    mGlobalVars.clear();
	
    mpModule = new llvm::Module("luacc module");
    
    mpBasicBlock = llvm::BasicBlock::Create("entry", NULL);
	
    mpBuilder = new llvm::IRBuilder(mpBasicBlock);

    // prime the pump
    mpLexer->GetNextToken();

    while(mpLexer->GetToken() != Lexer::TOKEN_EOF)
    {
        Expression* pExpr = ParseAssign();
        
        if( pExpr != NULL )
        {
            pExpr->GenerateCode();
        }
    }

    // close the global function
//    mpBuilder->CreateRetVoid();
    
    // Validate the generated code, checking for consistency.
//    llvm::verifyFunction(*mpGlobalFunc);
    
    Error(Error::LEVEL_INFO, "+++ Dumping module +++");
    
    mpModule->dump();
    
    Error::SummaryReport();
    
    delete mpBuilder;
	
    delete mpModule;
}    
