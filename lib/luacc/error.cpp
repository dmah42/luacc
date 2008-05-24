#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "core/error.h"

namespace LuaCC
{
    Error::Level Error::msLevel = Error::LEVEL_INFO;
    size_t       Error::msErrorCount = 0;
    size_t       Error::msWarningCount = 0;
    
    Error::Error(const Error::Level level)
    {
        Report(level, "Unknown error");
    }
    
    Error::Error(const Error::Level level, const char* pMess, ...)
    {
        va_list args;
        va_start(args, pMess);
        
        Report(level, pMess, args);
        
        va_end(args);
    }
    
    Error::Error(const Error::Level level, const unsigned int line,
                 const char* pFile)
    {
        char buffer[512] = "";
        sprintf(buffer, "%s (%d): Unknown error", pFile, line);
        
        Report(level, buffer);
    }
    
    Error::Error(const Error::Level level, const unsigned int line, 
                 const char* pFile, const char* pMess, ...)
    {
        char varBuff[512] = "";
        
        va_list args;
        va_start(args, pMess);
        
        vsprintf(varBuff, pMess, args);
        va_end(args);
        
        char buffer[512] = "";
        sprintf(buffer, "%s (%d): %s", pFile, line, varBuff);

        Report(level, buffer);
    }
   
    Error::Error(const Error::Level level, const unsigned int line,
                 const std::string& rFile)
    {
        char buffer[512] = "";
        sprintf(buffer, "%s (%d): Unknown error", rFile.c_str(), line);
        
        Report(level, buffer);
    }
    
    Error::Error(const Error::Level level, const unsigned int line, 
                 const std::string& rFile, const char* pMess, ...)
    {
        char varBuff[512] = "";
        
        va_list args;
        va_start(args, pMess);
        
        vsprintf(varBuff, pMess, args);
        va_end(args);
        
        char buffer[512] = "";
        sprintf(buffer, "%s (%d): %s", rFile.c_str(), line, varBuff);
        
        Report(level, buffer);
    }    
    
    void Error::Report(const Error::Level level, const char* pMess, ...)
    {
        char buff[1024] = "";
        
        va_list args;
        va_start(args, pMess);
        vsprintf(buff, pMess, args);
        
        if( level > msLevel )
        {
            switch(level)
            {
                case LEVEL_INFO:
                    fprintf(stdout, "INFO: %s\n", buff);
                    break;
                    
                case LEVEL_WARNING:
                    fprintf(stderr, "WARNING: %s\n", buff);
                    ++msWarningCount;
                    break;
                    
                case LEVEL_ERROR:
                    fprintf(stderr, "ERROR: %s\n", buff);
                    ++msErrorCount;
                    break;
                    
                case LEVEL_ABORT:
                    fprintf(stderr, "ABORT: %s\n", buff);
                    assert(false);
                    break;
            }
        }
        
        va_end(args);
    }
    
    void Error::SummaryReport()
    {
        fprintf(stdout, "\n%u errors, %u warnings\n", (unsigned) msErrorCount, 
                (unsigned) msWarningCount);
    }
}
