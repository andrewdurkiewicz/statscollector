/*
Created by Andrew Durkiewicz
Date: May 30, 2019
*/


#include <http_api_dashboard_testing.h>
#include <fstream>
#include <json/json.h>
#include <sqlite3.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>
using namespace std;
//using namespace Express;
//export LD_LIBRARY_PATH=.
//To Run:
//  g++ http_api_dashboard_testing.cpp -I. libSQLiteCpp.so libsqlite3.so -lpthread -ldl libSQLiteCpp.so -libjsoncpp.so ljsoncpp /home/user/jsoncpp/build-shared/libjsoncpp.so -o http_api_testing_executable && ./http_api_testing_executable 


SQLite:: Database db("StatsCollector.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
string q = "Select * from stats where Time between date('now','-10 days') and date('now');";    

Json::Value _initJson(string stat, string s){
    Json::Value proto_response;
    proto_response["Data"] = Json::arrayValue; //Where the Values go
    proto_response["Stat"] = stat; //What statistic is being measured
    proto_response["Unit"] = s; //Unit for y-axis
    return proto_response;
}

void getUL()
{
    SQLite::Statement query(db,q);
    Json::Value response = _initJson("UL", "Kbps");
    while(query.executeStep())
    {
        Json::Value row;
        row["Time"] = (const char *)  query.getColumn("Time");
        row["Value"] = (const char *) query.getColumn("UL");
        response["Data"].append(row);
    }
        ofstream o("UL.json");
        o << response << endl;
}
void getDL()
{
    SQLite::Statement query(db,q);
    Json::Value response = _initJson("DL", "Kbps");
    while(query.executeStep())
    {
        Json::Value row;
        row["Time"] = (const char *)  query.getColumn("Time");
        row["Value"] = (const char *) query.getColumn("DL");
        response["Data"].append(row);
    }
        ofstream o("DL.json");
        o << response << endl;

}

void getThroughput()
{
    SQLite::Statement query(db,q);
    Json::Value response = _initJson("Throughput", "Kbps");
    while(query.executeStep())
    {
        Json::Value row;
        row["Time"] = (const char *)  query.getColumn("Time");
        row["Value"] = (const char *) query.getColumn("Throughput");
        response["Data"].append(row);
    }
        ofstream o("Throughput.json");
        o << response << endl;
}
int main()
{
    getUL();
    getDL();
    getThroughput();
    return 0;
}
/*void initDashboardRoutes(Router & router)
{
    router.get("/UL", getUL);
    router.get("/DL", getDL);
    router.get("/Throughput", getThroughput);
}*/
