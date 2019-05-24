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

void getgraph() {
    try{
        SQLite:: Database db("StatsCollector.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        string q = "select Time, UL, DL, Throughput from stats where 1";
        
        SQLite::Statement query(db,q);

        Json::Value response;

        while(query.executeStep()) {
    		Json::Value row;
            row["Time"] = (int) query.getColumn(0);
            row["UL"] = (const char *) query.getColumn(1);
            row["DL"] = (const char *) query.getColumn(2);
            row["Throughput"] = (const char *) query.getColumn(3);
            response.append(row);
        }
        ofstream o("stats_out.json");
        o << response << endl;


    }
    catch (const SQLite::Exception & ex) {
        cout << "ERROR:" << ex.what();
    }
}

int main() {
    getgraph();
    return 0;
}
