
#ifndef ECPP_TOOLS_H
#define ECPP_TOOLS_H

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <Regex.h>
#include <json/json.h>

namespace Express
{

enum query_string_codes
{
    OK,
    EMPTY,
    BAD_RATIO ,
    NOT_BALANCED,
    DUPLICATE_KEY
};

std::string expressToRegex(Regex & re, std::string expr, std::vector<std::string> & params, bool mount_point);
std::string toJSONPretty(Json::Value & container);
std::string toJSONCompact(Json::Value & container);
std::string getEnv(std::string name);
void getStdin(std::streambuf * sbuf, std::string & body);
void clearStdin(std::streambuf * sbuf);
std::string getURITrimmed(std::string uri, std::string ext);
bool isValidAmpersandEqualsRatio(std::string request);
int parseHTTPQuery(std::map<std::string, std::string> &query_map, std::string query_str);
std::string getErrorMessage(int code);
std::string getQueryErrorMessage(int code);
bool parseMultipartFormData(std::map<std::string, std::string> & form_map, std::map<std::string, std::string> & file_map, std::string content_type, std::string & body);
}

#endif