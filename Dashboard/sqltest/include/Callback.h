
#ifndef ECPP_CALLBACK_H
#define ECPP_CALLBACK_H

#include <Request.h>
#include <Response.h>

namespace Express
{

class Callback
{
    public:
        Callback();
        Callback(void (*callback)(Request &, Response &));
        void call(Request &, Response &);

    private:
        void (*cb)(Request &, Response &);
};

}

#endif