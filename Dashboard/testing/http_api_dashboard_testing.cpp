/*
Created by Andrew Durkiewicz
Date: May 30, 2019
*/


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
static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char *)data);

    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}

SQLite:: Database db("/tmp/StatsCollector.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
string q = "Select * from live where Time between datetime('now','localtime','-10 days') and datetime('now', 'localtime');";    

Json::Value _initJson(string stat, string s){
    Json::Value proto_response;
    proto_response["Data"] = Json::arrayValue; //Where the Values go
    proto_response["Stat"] = stat; //What statistic is being measured
    proto_response["Unit"] = s; //Unit for y-axis
    return proto_response;
}
sqlite3 *db_csv;
char *zErrMsg = 0;
void SQL_CMD(sqlite3* db2, string command)
{

    sqlite3_exec(db2, command.c_str(), callback, NULL, NULL);
}
void getRx()
{
    SQLite::Statement query(db,q);
    Json::Value response = _initJson("Rx", "Mbps");
    while(query.executeStep())
    {
        Json::Value row;
        row["Time"] = (const char *)  query.getColumn("Time");
        row["V4"] = (const char *) query.getColumn("RxV4");
        row["V6"] = (const char *) query.getColumn("RxV6");

        response["Data"].append(row);
    }
        ofstream o("Rx.json");
        o << response << endl;
}
void getTx()
{
    SQLite::Statement query(db,q);
    Json::Value response = _initJson("Tx", "Mbps");
    while(query.executeStep())
    {
        Json::Value row;
        row["Time"] = (const char *)  query.getColumn("Time");
        row["V4"] = (const char *) query.getColumn("TxV4");
        row["V6"] = (const char *) query.getColumn("TxV6");
        response["Data"].append(row);
    }
        ofstream o("Tx.json");
        o << response << endl;

}

void getMx() //ManTRx
{
    SQLite::Statement query(db,q);
    Json::Value response = _initJson("Mx", "Kbps");
    while(query.executeStep())
    {
        Json::Value row;
        row["Time"] = (const char *)  query.getColumn("Time");
        row["Tx"] = (const char *) query.getColumn("MTx");
        row["Rx"] = (const char *) query.getColumn("MRx");
        response["Data"].append(row);
    }
        ofstream o("Mx.json");
        o << response << endl;
}
void getData()
{
    int rc = sqlite3_open("/tmp/StatsCollector.db", &db_csv);
    if(rc)
    {
        SQL_CMD(db_csv, ".output /fl0/thisismyoutputakjkj.csv");
    }
    SQL_CMD(db_csv, "select * from live where 1;");

    std::cout << "done";
}
int main()
{

    getData();
    return 0;
}
/*void initDashboardRoutes(Router & router)
{
    router.get("/UL", getUL);
    router.get("/DL", getDL);
    router.get("/Throughput", getThroughput);
}*/
