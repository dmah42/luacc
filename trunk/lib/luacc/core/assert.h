#pragma once

#include "core/error.h"

#if ENABLE_OPTIMIZED == 0
    #define Assert(b)   if(!(b))                                              \
                        {                                                     \
                            Error err(Error::LEVEL_ABORT, __LINE__, __FILE__);\
                        }

    #define AssertMess(b, mess) if(!(b))                                    \
                                {                                           \
                                    Error err(Error::LEVEL_ABORT, __LINE__, \
                                              __FILE__, mess);              \
                                }
#else
    #define Assert      do{ (void) 0; } while(false)
    #define AssertMess  do{ (void) 0; } while(false)
#endif
