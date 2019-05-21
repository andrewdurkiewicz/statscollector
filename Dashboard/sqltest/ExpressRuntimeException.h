/**
 *  Class declaration for a runtime_error derived class
 *  It allows you to throw an exception with additinal information such as:
 *  HTTP Error Code
 * 
 *  Author: Bryan Gillespie
 *          Javier Sanchez
 *          
 *  Created: October 2014
 *
 *  COPYRIGHT 2014-2015 Hughes Network Systems, LLC
 */

#ifndef EXPRESS_RUNTIME_EXCEPTION_H
#define EXPRESS_RUNTIME_EXCEPTION_H

#include <stdexcept>

namespace Express
{

class ExpressRuntimeException : public std::runtime_error
{
    private:
        int status;
    public:
        /**
         * Ex. throw ExpressRuntimeException(400, "Bad input")
         */
        ExpressRuntimeException(int http_status, std::string msg);

        /**
         * Returns the HTTP status that was provided to constructor
         * @return
         */
        int getStatus();
};

}

#endif