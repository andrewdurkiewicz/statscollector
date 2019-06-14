/*
Created by Andrew Durkiewicz
Date: May 30, 2019
****/


#include <http_api_dashboard.h>
#include <fstream>
#include <json/json.h>
#include <sqlite3.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <iomanip>
#include <map> 

using namespace std;
using namespace Express;
//export LD_LIBRARY_PATH=.
//To Run:
//  g++ http_api_dashboard.cpp -I. libSQLiteCpp.so libsqlite3.so -lpthread -ldl libSQLiteCpp.so -libjsoncpp.so ljsoncpp /home/user/jsoncpp/build-shared/libjsoncpp.so -o http_api_executable && ./http_api_executable 



bool findState(vector<string> & conditions, Request & req, Response & res){
    if (req.query.count("State"))
    {
        conditions.push_back(req.query["State"]);
    }
    else{

    	res.status(400);
	return false;
	
    }
    return true;
}
Json::Value _initJson(string stat, string s){
    Json::Value proto_response;
    proto_response["Data"] = Json::arrayValue; //Where the Values go
    proto_response["Stat"] = stat; //What statistic is being measured
    proto_response["Unit"] = s; //Unit for y-axis
    return proto_response;
}

void getRx(Request & req, Response & res)
{
    SQLite:: Database db("/fl0/StatsCollector.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    vector<string> conditions;
    string q;

	if(findState(conditions,req,res)){
	    if(req.query["State"] == "Live"){
		    q = "Select * from stats where Time > dateTime('now','localtime','-4 hours','-2 seconds');";
	    }
	    else if(req.query["State"] == "Day"){
		    q = "Select * from stats where Time > dateTime('now','localtime','-4 hours','-1 day');";

        }
	    else if(req.query["State"] == "Max"){
		    q = "Select * from stats where Time > dateTime('now','localtime','-4 hours','-15 days');";

        }
        else{
            res.status(400);
        }

	    SQLite::Statement query(db,q);
	    //cout << req.request_uri << endl;
	    Json::Value response = _initJson("Rx", "Mbps");
	    while(query.executeStep()){
		Json::Value row;
            row["Time"] = (const char *)  query.getColumn("Time");
            row["V4"] = (const char *) query.getColumn("RxV4");
            row["V6"] = (const char *) query.getColumn("RxV6");
            response["Data"].append(row);
	    }

	    res.send(response);
	}
}
void getTx(Request & req, Response & res)
{

    SQLite:: Database db("/fl0/StatsCollector.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    vector<string> conditions;
    string q;
    
	if(findState(conditions,req,res)){
	    if(req.query["State"] == "Live"){
		    q = "Select * from stats where Time > dateTime('now','localtime','-4 hours','-2 seconds');";
	    }
	    else if(req.query["State"] == "Day"){
		    q = "Select * from stats where Time > dateTime('now','localtime','-4 hours',' -1 days');";

        }
	    else if(req.query["State"] == "Max"){
		    q = "Select * from stats where Time > dateTime('now','localtime','-4 hours','-15 days');";

        }
        else{
            res.status(400);
        }

	    SQLite::Statement query(db,q);
	    Json::Value response = _initJson("Tx", "Mbps");
	    while(query.executeStep()){
            Json::Value row;
            row["Time"] = (const char *)  query.getColumn("Time");
            row["V4"] = (const char *) query.getColumn("TxV4");
            row["V6"] = (const char *) query.getColumn("TxV6");
            response["Data"].append(row);
	    }

	    res.send(response);
	}
}

void getMx(Request & req, Response & res)
{
    SQLite:: Database db("/fl0/StatsCollector.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    vector<string> conditions;
    string q;

	if(findState(conditions,req,res)){
	    if(req.query["State"] == "Live"){
		    q = "Select * from stats where Time > dateTime('now','localtime','-4 hours','-2 seconds');";
	    }
	    else if(req.query["State"] == "Day"){
		    q = "Select * from stats where Time > dateTime('now','localtime','-4 hours',' -1 days');";

        }
	    else if(req.query["State"] == "Max"){
		    q = "Select * from stats where Time > dateTime('now','localtime','-4 hours','-15 days');";

        }
        else{
            res.status(400);
        }

	    SQLite::Statement query(db,q);
	    //cout << req.request_uri << endl;
        Json::Value response = _initJson("Mx", "Kbps");
	    while(query.executeStep()){
            Json::Value row;
            row["Time"] = (const char *)  query.getColumn("Time");
            row["Tx"] = (const char *) query.getColumn("MTx");
            row["Rx"] = (const char *) query.getColumn("MRx");
            response["Data"].append(row);
	    }

	    res.send(response);
	}
}

void initDashboardRoutes(Router & router)
{
    router.get("/Rx", getRx);
    router.get("/Tx", getTx);
    router.get("/Mx", getMx);
}

