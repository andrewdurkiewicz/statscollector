/*
Created by Andrew Durkiewicz
Date: May 30, 2019
**/


#include <http_api_dashboard.h>
#include <fstream>
#include <json/json.h>
#include <sqlite3.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <iomanip>
#include <map> 
#include <ctime>
#include <tracelogger.h>

using namespace std;
using namespace Express;
//export LD_LIBRARY_PATH=.
//To Run:
//  g++ http_api_dashboard.cpp -I. libSQLiteCpp.so libsqlite3.so -lpthread -ldl libSQLiteCpp.so -libjsoncpp.so ljsoncpp /home/user/jsoncpp/build-shared/libjsoncpp.so -o http_api_executable && ./http_api_executable 


void getDashConfig(Request & req, Response & res)
{
	Json::Value root;
	Json::Reader reader;
	std::ifstream file("/fl0/stats_available.json");
    	bool isparsed = reader.parse(file, root, true);
	if(!isparsed)
	{
		Json::Value error_response;		
		TRACE_LOG(LOG_ERR, LOG_ERR, "JSON Failed To Parse");
		error_response["Error"] = reader.getFormattedErrorMessages(); //What statistic is being measured
		res.status(434);
		res.send(error_response);
	}
	res.send(root);


}

bool findState(vector<string> & conditions, Request & req, Response & res)
{
	char buff[10];
	int begin = 1;
	sprintf(buff, "Stat%i",begin);


    if(!req.query.count("Stat1") || !req.query.count("State"))
	{
    	res.status(400);
		return false;
	
    }

	do
	{
		conditions.push_back(req.query[buff]);
		memset( buff, '\0', sizeof(char)*10 );
		begin++;
		sprintf(buff, "Stat%i",begin);

	} while (req.query.count(buff));
	

    return true;
}
Json::Value _initJson(string stat, string s)
{
    Json::Value proto_response;
    proto_response["Data"] = Json::arrayValue; //Where the Values go
    proto_response["Stat"] = stat; //What statistic is being measured
    proto_response["Unit"] = s; //Unit for y-axis
    return proto_response;
}

void getData(Request & req, Response & res)
{
	Json::Value response;
	vector<std::string> conditions;
    string q;
	if(findState(conditions,req,res))
	{
	    if(req.query["State"] == "Live")
		{
		    q = "Select * from live where Time > dateTime('now','localtime','-4 hours','-2 seconds');";
	    }
	    else if(req.query["State"] == "Day")
		{
		    q = "Select * from day where Time > dateTime('now','localtime','-4 hours','-1 day');";

        }
	    else if(req.query["State"] == "Max")
		{
		    q = "Select * from max where Time > dateTime('now','localtime','-4 hours','-15 days');";

        }
		else if(req.query["State"] == "Hour")
		{
			q = "Select * from live where Time > dateTime('now','localtime','-5 hours');";
		}
        else{
            res.status(400);
        }	
	}
	    try
		{
			SQLite:: Database db("/fl0/StatsCollector.db", SQLite::OPEN_READONLY );
	    	SQLite::Statement query(db,q);
			while(query.executeStep())
			{
				Json::Value row;
				row["Time"] = (const char *)  query.getColumn("Time");
				for(int i = 0; i<conditions.size(); i++)
				{
					row[conditions[i].c_str()] =  (const char *) query.getColumn(conditions[i].c_str());

				}
				response["Data"].append(row);
			}
			res.send(response);

		}
		catch(std::exception& e)
		{
			Json::Value response;
			char buffer[200];
			sprintf(buffer, "Rx Request: SQLITE DB ERROR-> %s\n",e.what());
    		TRACE_LOG(LOG_ERR, LOG_ERR, "%s",buffer);
			res.status(206);
			res.send(response);
		}

}


void getRx(Request & req, Response & res)
{
    vector<string> conditions;
    string q;

	if(findState(conditions,req,res))
	{
	    if(req.query["State"] == "Live")
		{
		    q = "Select * from live where Time > dateTime('now','localtime','-4 hours','-2 seconds');";
	    }
	    else if(req.query["State"] == "Day")
		{
		    q = "Select * from day where Time > dateTime('now','localtime','-4 hours','-1 day');";

        }
	    else if(req.query["State"] == "Max")
		{
		    q = "Select * from max where Time > dateTime('now','localtime','-4 hours','-15 days');";

        }
		else if(req.query["State"] == "Hour")
		{
			q = "Select * from live where Time > dateTime('now','localtime','-5 hours');";
		}
        else{
            res.status(400);
        }

	    try
		{
			Json::Value response = _initJson("Rx", "Mbps");
			SQLite:: Database db("/fl0/StatsCollector.db", SQLite::OPEN_READONLY );
	    	SQLite::Statement query(db,q);
			while(query.executeStep())
			{
			
				Json::Value row;
				row["Time"] = (const char *)  query.getColumn("Time");
				row["V4"] = (const char *) query.getColumn("RxV4");
				row["V6"] = (const char *) query.getColumn("RxV6");
				response["Data"].append(row);
			}
			res.send(response);

		}
		catch(std::exception& e)
		{
			Json::Value response;
			char buffer[200];
			sprintf(buffer, "Rx Request: SQLITE DB ERROR-> %s\n",e.what());
    		TRACE_LOG(LOG_ERR, LOG_ERR, "%s",buffer);
			res.status(206);
			res.send(response);
		}

	}
}
void getTx(Request & req, Response & res)
{
    string q;
	vector<string> conditions;

	if(findState(conditions,req,res)){
	    if(req.query["State"] == "Live"){
		    q = "Select * from live where Time > dateTime('now','localtime','-4 hours','-2 seconds');";
	    }
	    else if(req.query["State"] == "Day"){
		    q = "Select * from day where Time > dateTime('now','localtime','-4 hours','-1 day');";

        }
	    else if(req.query["State"] == "Max"){
		    q = "Select * from max where Time > dateTime('now','localtime','-4 hours','-15 days');";

        }
		else if(req.query["State"] == "Hour"){
			q = "Select * from live where Time > dateTime('now','localtime','-5 hours');";
		}
        else{
            res.status(400);
        }

	    try
		{
			SQLite:: Database db("/fl0/StatsCollector.db", SQLite::OPEN_READONLY );
			Json::Value response = _initJson("Tx", "Mbps");
			SQLite::Statement query(db,q);
			//cout << req.request_uri << endl;
			while(query.executeStep()){
				Json::Value row;
				row["Time"] = (const char *)  query.getColumn("Time");
				row["V4"] = (const char *) query.getColumn("TxV4");
				row["V6"] = (const char *) query.getColumn("TxV6");
				response["Data"].append(row);
	    	}
	    	res.send(response);


		}
		catch(std::exception& e)
		{
			Json::Value response;
			char buffer[200];
			sprintf(buffer, "Tx Request: SQLITE DB ERROR-> %s\n",e.what());
    		TRACE_LOG(LOG_ERR, LOG_ERR, "%s",buffer);
			res.status(206);
			res.send(response);
		}

	}
}

void getMx(Request & req, Response & res)
{
    vector<string> conditions;
    string q;

	if(findState(conditions,req,res)){
	    if(req.query["State"] == "Live"){
		    q = "Select * from live where Time > dateTime('now','localtime','-4 hours','-2 seconds');";
	    }
	    else if(req.query["State"] == "Day"){
		    q = "Select * from day where Time > dateTime('now','localtime','-4 hours','-1 day');";

        }
	    else if(req.query["State"] == "Max"){
		    q = "Select * from max where Time > dateTime('now','localtime','-4 hours','-15 days');";

        }
		else if(req.query["State"] == "Hour"){
			q = "Select * from live where Time > dateTime('now','localtime','-5 hours');";
		}
        else{
            res.status(400);
        }

		try
		{
    		SQLite:: Database db("/fl0/StatsCollector.db", SQLite::OPEN_READONLY );

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
		catch(std::exception& e)
		{
			Json::Value response;
			char buffer[200];
			sprintf(buffer, "Management Request: SQLITE DB ERROR-> %s\n",e.what());
    		TRACE_LOG(LOG_ERR, LOG_ERR, "%s",buffer);
			res.status(206);
			res.send(response);
		}
		
	}

}

void initDashboardRoutes(Router & router)
{
	router.get("/getConfig",getDashConfig);
	router.get("/getData",getData);
	router.get("/Rx", getRx);
    router.get("/Tx", getTx);
    router.get("/Mx", getMx);
}

