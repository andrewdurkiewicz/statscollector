#include <http_api_dashboard.h>

#include <json.hpp>
#include <sqlite3.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>

using nlohmann::json;
using namespace std;

void getgraph() {
    try{
        SQLite:: Database db("StatsCollector.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        string q = "select Time, UL, DL, Throughput from stats where 1";
        
        SQLite::Statement query(db,q);
        json j = json::array();
        while(query.executeStep()) {
            json row;
            row["time"] = (int) query.getColumn(0);
            row["time2"] = (const char *) query.getColumn(1);
            row["time3"] = (const char *) query.getColumn(2);
            row["time4"] = (const char *) query.getColumn(3);
            j.push_back(row);
        }

        cout << j << endl;
    }
    catch (const SQLite::Exception & ex) {
        cout << "ERROR:" << ex.what();
    }
}

int main() {
    getgraph();
    return 0;
}