/*
Created by Andrew Durkiewicz
Date: May 30, 2019
*/


#include <http_api_dashboard_testing.h>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <sqlite3.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <iomanip>

using namespace std;
//using namespace Express;
//export LD_LIBRARY_PATH=.
//To Run:
//  g++ http_api_dashboard_testing.cpp -I. libSQLiteCpp.so libsqlite3.so -lpthread -ldl libSQLiteCpp.so -libjsoncpp.so ljsoncpp /home/user/jsoncpp/build-shared/libjsoncpp.so -o http_api_testing_executable && ./http_api_testing_executable 


SQLite:: Database db("StatsCollector.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
string q = "select * from stats where Time";    

Json::Value _initJson(string stat){
    Json::Value proto_resname;
    proto_resname["Stat"] = stat;
    Json::Value proto_res;
    proto_res["Data"] = Json::arrayValue;
    return proto_res;
}

void getUL()
{
	SQLite::Statement query(db,q);
    Json::Value response = _initJson("UL");
    while(query.executeStep())
    {
        Json::Value row;
        row["Time"] = (int) query.getColumn("Time");
        row["UL"] = (const char *) query.getColumn("UL");
        response["UL"].append(row);
    }
        ofstream o("UL.json");
        o << response << endl;
}
void getDL()
{
    SQLite::Statement query(db,q);
    Json::Value response = _initJson("DL");
    while(query.executeStep())
    {
        Json::Value row;
        row["Time"] = (int) query.getColumn("Time");
        row["DL"] = (const char *) query.getColumn("DL");
        response["DL"].append(row);
    }
        ofstream o("DL.json");
        o << response << endl;

}

void getThroughput()
{
    SQLite::Statement query(db,q);
    Json::Value response = _initJson("Throughput");
    while(query.executeStep())
    {
        Json::Value row;
        row["Time"] = (int) query.getColumn("Time");
        row["Throughput"] = (const char *) query.getColumn("Throughput");
        response["Throughput"].append(row);
    }
        ofstream o("Throughput.json");
        o << response << endl;
}
int main()
{
    //getUL();
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
