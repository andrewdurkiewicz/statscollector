
#ifndef ECPP_REQUEST_H
#define ECPP_REQUEST_H

#include <map>
#include <string>
#include <json/json.h>

namespace Express
{

class Request
{
    public:
        std::string request_uri;
        std::string request_uri_relative;
        std::string request_method;
        std::string content_type;
        std::string content_length;
        std::string body_raw;
        Json::Value body;
        std::map<std::string, std::string> params;
        std::map<std::string, std::string> query;
        std::map<std::string, std::string> form;
        std::map<std::string, std::string> file;
};

}

#endif