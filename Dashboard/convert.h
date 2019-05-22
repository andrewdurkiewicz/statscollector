#ifndef ECPP_CONVERT_H
#define ECPP_CONVERT_H

#include <ExpressRuntimeException.h>
#include <sstream>
#include <string>

namespace Express
{

template <class TYPE>
TYPE parse(std::string value)
{
    std::istringstream ss(value);
    TYPE x;
    ss >> x;
    
    if (ss.fail())
        throw ExpressRuntimeException(400, "Value parsing error: [" + value + "] could not be parsed to provided type");

    return x;
}

template <>
bool parse<bool>(std::string value)
{
    std::istringstream ss(value);
    bool x;
    ss >> std::boolalpha >> x;
    
    if (ss.fail())
        throw ExpressRuntimeException(400, "Value parsing error: expected [" + value + "] to be boolean (true/false)");

    return x;
}

}

#endif