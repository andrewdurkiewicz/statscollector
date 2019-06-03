/*
Created by Andrew Durkiewicz
Date: May 30, 2019
*/


#include <http_api_dashboard.h>
#include <fstream>
#include <json/json.h>
#include <sqlite3.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Express;
//export LD_LIBRARY_PATH=.
//To Run:
//  g++ http_api_dashboard.cpp -I. libSQLiteCpp.so libsqlite3.so -lpthread -ldl libSQLiteCpp.so -libjsoncpp.so ljsoncpp /home/user/jsoncpp/build-shared/libjsoncpp.so -o http_api_executable && ./http_api_executable 


SQLite:: Database db("/fl0/StatsCollector.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
string q = "select * from stats where Time";    

Json::Value _initJson(string stat){
    Json::Value proto_response;
    Json::Value proto_statistic;
    Json::Value proto_unit;
    proto_statistic["Stat"] = stat;
    proto_unit["Unit"] = "Kb/s"; //Rate
    proto_response["Data"] = Json::arrayValue;

    return proto_response;
}

void getUL(Request & req, Response & res)
{
    SQLite::Statement query(db,q);
    Json::Value response = _initJson("UL");
    while(query.executeStep())
    {
        Json::Value row;
        row["Time"] = (int) query.getColumn("Time");
        row["UL"] = (const char *) query.getColumn("UL");
        response["Data"].append(row);
    }
        /*ofstream o("UL.json");
        o << response << endl;*/
    res.send(response);
}
void getDL(Request & req, Response & res)
{
    SQLite::Statement query(db,q);
    Json::Value response = _initJson("DL");
    while(query.executeStep())
    {
        Json::Value row;
        row["Time"] = (int) query.getColumn("Time");
        row["DL"] = (const char *) query.getColumn("DL");
        response["Data"].append(row);
    }
        /*ofstream o("DL.json");
        o << response << endl;*/
    res.send(response);

}

void getThroughput(Request & req, Response & res)
{
    SQLite::Statement query(db,q);
    Json::Value response = _initJson("Throughput");
    while(query.executeStep())
    {
        Json::Value row;
        row["Time"] = (int) query.getColumn("Time");
        row["Throughput"] = (const char *) query.getColumn("Throughput");
        response["Data"].append(row);
    }
        /*ofstream o("Throughput.json");
        o << response << endl;*/
    res.send(response);
}

void initDashboardRoutes(Router & router)
{
    router.get("/UL", getUL);
    router.get("/DL", getDL);
    router.get("/Throughput", getThroughput);
}
