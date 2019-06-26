
#ifndef ECPP_APPLICATION_H
#define ECPP_APPLICATION_H

#include <Router.h>
#include <iostream>
#include <vector>
#include <string>

namespace Express
{

class Application
{
    public:
        Application();
        Response cgi(std::string cgi_extension=".cgi");
        void use(std::string route_str, Router & router);
        void get(std::string route_str, Callback cb);
        void post(std::string route_str, Callback cb);
        void processRequest(Request & req, Response & res);
        void setStdin(std::streambuf * sbuf);
        void setAccessAll(bool enable, bool routers_only=true);
        bool setAccess(std::string route, bool enable);
        bool setAccess(std::string route, const std::string & method, bool enable);
        void setMaxUploadSize(int bytes);
        void setHeader(std::string name, std::string value);
        void removeHeader(std::string name);
    
    private:
        Router root;
        std::streambuf * stdinbuf;
        int max_upload_size;
        std::map<std::string, std::string> default_headers;

        void parseBody(Request & req);
        std::string getRequestPath(std::string & cgi_extension);
};

}

#endif