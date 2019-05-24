#include <http_api_dashboard.h>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <sqlite3.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <iomanip>

using namespace std;
//export LD_LIBRARY_PATH=.
//To Run:
//  g++ http_api_dashboard.cpp -I. libSQLiteCpp.so libsqlite3.so -lpthread -ldl libsqlite3.so libSQLiteCpp.so  -ljsoncpp /home/user/jsoncpp/build-shared/libjsoncpp.so -o http_api_executable && ./http_api_executable 


Json::Value response;
SQLite:: Database db("StatsCollector.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
string q = "select * from stats where Time";    
void getUL(), getDL(), getThroughput();




int main() {
    getUL();
    getDL();

    getThroughput();
    return 0;
}


void getUL(){
    SQLite::Statement query(db,q);

    while(query.executeStep()){
        Json::Value row;
        row["Time"] = (int) query.getColumn("Time");
        row["UL"] = (const char *) query.getColumn("UL");
        response.append(row);
    }
        ofstream o("UL.json");
        o << response << endl;
        cout << response;
}

void getDL(){
        SQLite::Statement query(db,q);

    while(query.executeStep()){
        Json::Value row;
        row["Time"] = (int) query.getColumn("Time");
        row["DL"] = (const char *) query.getColumn("DL");
        response.append(row);
    }
        ofstream o("DL.json");
        o << response << endl;
        cout << response;
}

void getThroughput(){
        SQLite::Statement query(db,q);

    while(query.executeStep()){
        Json::Value row;
        row["Time"] = (int) query.getColumn("Time");
        row["Throughput"] = (const char *) query.getColumn("DL");
        response.append(row);
    }
        ofstream o("Throughput.json");
        o << response << endl;
        cout << response;
}
