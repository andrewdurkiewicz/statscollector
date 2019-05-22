#ifndef ECPP_ROUTER_H
#define ECPP_ROUTER_H

#include "Route.h"
#include "Regex.h"
#include <vector>
#include <string>

namespace Express
{

class Router
{
    public:
        Router();
        Router(const Router& other);  //Copy constructor
        Router& operator=(const Router & other);
        ~Router();

        void use(std::string route_str, Router & router);
        void get(std::string route_str, Callback cb);
        void post(std::string route_str, Callback cb);

        Route * findRoute(std::string uri, const std::string & method, Request & req);
        bool setAccess(std::string uri, const std::string & method, bool enabled);
        bool setAccessAll(bool enabled, bool routers_only);

    private:
        std::vector<Route> routes;
        Regex express_re;
};

}

#endif