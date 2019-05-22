#include <http_api_dashboard.h>
#include <fstream>
#include <json.hpp>
#include <sqlite3.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <iomanip>
using nlohmann::json;
using namespace std;
//export LD_LIBRARY_PATH=.
// g++ -std=c++11 http_api_dashboard.cpp -I. libSQLiteCpp.so libsqlite3.so -lpthread -ldl -o http_api_executable && ./http_api_executable 
void getgraph() {
    try{
        SQLite:: Database db("StatsCollector.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        string q = "select Time, UL, DL, Throughput from stats where 1";
        
        SQLite::Statement query(db,q);
        json file = json::array();
        while(query.executeStep()) {
            json row;
            row["Time"] = (int) query.getColumn(0);
            row["UL"] = (const char *) query.getColumn(1);
            row["DL"] = (const char *) query.getColumn(2);
            row["Throughput"] = (const char *) query.getColumn(3);
            file.push_back(row);
        }
        ofstream o("stats_out.json");
		o << setw(4) << file << endl;
    }
    catch (const SQLite::Exception & ex) {
        cout << "ERROR:" << ex.what();
    }
}

int main() {
    getgraph();
    return 0;
}