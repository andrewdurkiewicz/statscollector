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
string q = "select * from stats where Time";    

Json::Value _initJson(string stat, string unit){
    Json::Value proto_response;
    //proto_response["Data"] = Json::arrayValue; //Where the Values go
    proto_response["A"] = Json::StaticString(stat.c_str()); //What statistic is being measured
    proto_response["B"] = Json::StaticString(unit.c_str()); //Unit for y-axis
    return proto_response;
}

void getUL()
{
	SQLite::Statement query(db,q);
    Json::Value response = _initJson("UL", "Kb/s");
    while(query.executeStep())
    {
        Json::Value row;
        row["A"] = Json::StaticString(query.getColumn("Time").getString().c_str());
        row["B"] = Json::StaticString(query.getColumn("UL").getString().c_str());
        response["C"].append(row);
    }
        ofstream o("UL.json");
        o << response << endl;
}
void getDL()
{
    SQLite::Statement query(db,q);
    Json::Value response = _initJson("DL", "Kb/s");
    while(query.executeStep())
    {
        Json::Value row;
        row["A"] = Json::StaticString(query.getColumn("Time").getString().c_str());
        row["B"] = Json::StaticString(query.getColumn("DL").getString().c_str());
        response["C"].append(row);
    }
        ofstream o("DL.json");
        o << response << endl;

}

void getThroughput()
{
    SQLite::Statement query(db,q);
    Json::Value response = _initJson("Throughput", "Kb/s");
    while(query.executeStep())
    {
        Json::Value row;
        row["A"] = Json::StaticString(query.getColumn("Time").getString().c_str());
        row["B"] = Json::StaticString(query.getColumn("Throughput").getString().c_str());
        response["C"].append(row);
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