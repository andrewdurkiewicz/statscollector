
#ifndef ECPP_ROUTE_H
#define ECPP_ROUTE_H

#include "Callback.h"
#include "Regex.h"
#include <vector>
#include <string>

namespace Express
{

class Router; //Forward Declaration
class Route
{
    public:
        Route();
        Route(const Route & other);
        Route& operator=(const Route& other);
        ~Route();

        Route(std::string route_str, std::string regex_str, std::string http_method, Router & router_obj);
        Route(std::string route_str, std::string regex_str, std::string http_method, Callback & cb);

        Match match(std::string uri);
        bool matches(std::string uri);
        std::string mount(std::string uri);
        
        Router * getRouter();

        std::string getMethod();
        bool hasCallback();
        bool hasRouter();
        bool isEnabled();
        void setEnabled(bool b);
        bool handle(Request & req, Response & res);
        void setParamNames(std::vector<std::string> & names);

    private:
        Regex re;

        std::string route_raw;
        std::string regex_raw;
        std::string method;
        std::vector<std::string> param_names;

        Callback callback;
        Router * router;

        bool containsCallback;
        bool containsRouter;
        bool enabled;
};

}

#endif