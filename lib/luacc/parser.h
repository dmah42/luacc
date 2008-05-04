#pragma once

#include <map>

#include "lexer.h"

#include "llvm/Support/IRBuilder.h"

namespace llvm
{
    class AllocaInst;
    class Constant;
    class GlobalVariable;
    class Module;
    class Value;
}

namespace LuaCC
{
    class Expression;
    
    typedef std::map<std::string, llvm::AllocaInst*>        LocalVars;
    typedef LocalVars::iterator                             LocalVarsIter;
    typedef LocalVars::const_iterator                       LocalVarsConstIter;
    
    typedef std::map<std::string, llvm::GlobalVariable*>    GlobalVars;
    typedef GlobalVars::iterator                            GlobalVarsIter;
    typedef GlobalVars::const_iterator                      GlobalVarsConstIter;
    
    class Parser
    {
        public:
        
            static Parser&  Instance()  { return mInstance; }
        
            void            Execute(FILE* stream);
            void            Execute(const std::string& rFilename);

            llvm::IRBuilder*   GetBuilder()		{ return mpBuilder; }
        
            llvm::Value*       GetValueFromVariable(const std::string& rName);
        
        private:
        
            void                ExecutePrivate();
		
            Expression*         ParseRvalue();
            Expression*         ParsePrimary();
            Expression*         ParseChain();
            Expression*         ParseAssign();
        
            void                CreateLocalVariable(const std::string& rName, 
                                                    llvm::Value* pRHS);
            void                CreateGlobalVariable(const std::string& rName, 
                                                     llvm::Constant* pRHS,
                                                     const bool isInternal);
        
            static Parser       mInstance;

            Lexer*              mpLexer;        
        
            GlobalVars          mGlobalVars;
            LocalVars           mLocalVars;
        
            llvm::BasicBlock*   mpBasicBlock;
            llvm::Module*       mpModule;
            llvm::IRBuilder*    mpBuilder;
    };
}
