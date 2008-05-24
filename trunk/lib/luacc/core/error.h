#pragma once

#include <string>

namespace LuaCC
{
    class Error
    {
        public:
            enum Level
            {
                LEVEL_INFO,
                LEVEL_WARNING,
                LEVEL_ERROR,
                LEVEL_ABORT
            };
        
            Error(const Level level);
            Error(const Level level, const char* pMess, ...);
            Error(const Level level, const unsigned int line, const char* pFile);
            Error(const Level level, const unsigned int line, const char* pFile, 
                  const char* pMess, ...);
            Error(const Level level, const unsigned int line, 
                  const std::string& rFile);
            Error(const Level level, const unsigned int line, 
                  const std::string& rFile, const char* pMess, ...);
        
            static void SetOutputLevel(const Level level)
            {
                msLevel = level;
            }
        
            static void SummaryReport();
        
        private:
            void    Report(const Error::Level level, const char* pMess, ...);

            static Level    msLevel;
        
            static size_t   msErrorCount;
            static size_t   msWarningCount;
    };
}
