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

Json::Value _initJson(string stat, string unit){
    Json::Value proto_response;
    proto_response["Data"] = Json::arrayValue; //Where the Values go
    proto_response["Stat"] = stat; //What statistic is being measured
    proto_response["Unit"] = unit; //Unit for y-axis
    return proto_response;
    return proto_response;
}

void getUL(Request & req, Response & res)
{
	SQLite::Statement query(db,q);
    Json::Value response = _initJson("UL", "Kb/s");
    while(query.executeStep())
    {
        Json::Value row;
        row["Time"] = (int) query.getColumn("Time");
        row["Value"] = (const char *) query.getColumn("UL");
        response["Data"].append(row);
    }
        /*ofstream o("UL.json");
        o << response << endl;*/
    res.send(response);
}
void getDL(Request & req, Response & res)
{
    SQLite::Statement query(db,q);
    Json::Value response = _initJson("DL", "Kb/s");
    while(query.executeStep())
    {
        Json::Value row;
        row["Time"] = (int) query.getColumn("Time");
        row["Value"] = (const char *) query.getColumn("DL");
        response["Data"].append(row);
    }
        /*ofstream o("DL.json");
        o << response << endl;*/
    res.send(response);

}

void getThroughput(Request & req, Response & res)
{
    SQLite::Statement query(db,q);
    Json::Value response = _initJson("Throughput", "Kb/s");
    while(query.executeStep())
    {
        Json::Value row;
        row["Time"] = (int) query.getColumn("Time");
        row["Value"] = (const char *) query.getColumn("Throughput");
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
