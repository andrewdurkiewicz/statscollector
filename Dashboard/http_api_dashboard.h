/*
Created by Andrew Durkiewicz
Date: May 30, 2019
*/

#include <Router.h>
#ifndef HTTP_API_DASHBOARD_H
#define HTTP_API_DASHBOARD_H


void initDashboardRoutes(Express::Router & router);
void getUL(Request & req, Response & res);
void getDL(Request & req, Response & res);
void getThroughput(Request & req, Response & res);
Json::Value _initJson(string stat, string unit);


#endif
