/*
Created by Andrew Durkiewicz
Date: May 30, 2019
*/
#include <jsoncpp/json/json.h>
using namespace std;
//#include <Router.h>
#ifndef HTTP_API_DASHBOARD_H
#define HTTP_API_DASHBOARD_H


//void initDashboardRoutes(Express::Router & router);
void getRx();
void getTx();
void getMx();
Json::Value _initJson(string stat, string s);


#endif
