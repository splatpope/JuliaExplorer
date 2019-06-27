#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#define DEBUG

#include <iostream>
#include <string>

#include "GLError.h"

namespace util
{
inline void EmitLine(std::string s)
{
    #ifndef DEBUG
    return;
    #endif
    std::cout << s << std::endl;
}

//Turn var into a string (maybe) and append it to a string describing it
template<class t>
    inline void DebugLog(std::string s, t var)
    { 
        EmitLine(s + std::to_string( static_cast<int>(var))); 
    }
}

#endif