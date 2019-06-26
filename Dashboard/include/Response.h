
#ifndef ECPP_RESPONSE_H
#define ECPP_RESPONSE_H

#include <map>
#include <string>
#include "json/json.h"

namespace Express
{

class Response
{
    public:
        Response();
        Response & send(std::string response);
        Response & send(Json::Value & response, bool pretty_json=false);
        Response & status(int status);

        void setHeader(std::string name, std::string value);
        void removeHeader(std::string name);
        void setHeaders(std::map<std::string, std::string> & h);
        std::string getHeaders(bool status=true);

        int http_status;
        std::string response;

    private:
        std::map<std::string, std::string> headers;
        void setResponse(std::string response);
};

}

#endif