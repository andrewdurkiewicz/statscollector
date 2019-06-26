/*
Created by Andrew Durkiewicz
Date: May 30, 2019
***/


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



void print_map(std::map<std::string, std::string> myMap, string filename, string extra)
{
	string output = extra;
	for(std::map<string,string>::iterator it = myMap.begin(); it!=myMap.end(); ++it)
	{
	    output = output + it->first +  " " + it->second + "\n";
	}
        ofstream o(filename.c_str());
	o << output << endl;
}

bool findState(vector<string> & conditions, Request & req, Response & res)
{
    if (req.query.count("State"))
    {
        conditions.push_back(req.query["State"]);
    }
	else
	{
    	res.status(400);
		return false;
	} 	 
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

void getRx(Request & req, Response & res)
{
	/*string hi = "test.txt";
	ofstream o(hi.c_str());
    print_map(req.query, "query.txt","This is the query\n");
    print_map(req.params, "params.txt","This is the params\n");
    print_map(req.form, "form.txt","This is the form\n");
    print_map(req.file, "file.txt","This is the file\n");*/
    vector<string> conditions;
    string q;
	
	if(findState(conditions,req,res))
	{
	
		if(req.query["State"] == "Live")
		{
			q = "Select * from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-4 hours','-1 seconds');";
		}
		else if(req.query["State"] == "Day")
		{
			q = "Select * from day where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-4 hours','-1 day');";

		}
		else if(req.query["State"] == "Max")
		{
			q = "Select * from max where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-4 hours','-15 days');";

		}
		else if(req.query["State"] == "Hour")
		{
			q = "Select * from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours');";
		}
		else
		{
			res.status(400);
		}
    	SQLite::Database db("/fl0/StatsCollector.db", SQLite::OPEN_READONLY);
	    SQLite::Statement query(db,q);
	    //cout << req.request_uri << endl;
	    Json::Value response = _initJson("Rx", "Mbps");
		if(req.query["State"] == "Hour" || req.query["State"] == "Live")
		{
			while(query.executeStep())
			{
				Json::Value row;
				row["Time"] = (const char *)  query.getColumn("Time");
				row["V4"] = (const char *) query.getColumn("RxV4");
				row["V6"] = (const char *) query.getColumn("RxV6");
				response["Data"].append(row);
			}
		}
		else if(req.query["State"] == "Max" || req.query["State"] == "Day")
		{
			while(query.executeStep())
			{
				Json::Value row;
				row["Time"] = (const char *)  query.getColumn("Time");
				row["V4"] = (const char *) query.getColumn("RxV4");
				row["V4_MAX"] = (const char *) query.getColumn("RxV4_MAX");
				row["V4_MIN"] = (const char *) query.getColumn("RxV4_MIN");
				row["V6"] = (const char *) query.getColumn("RxV6");
				row["V6_MAX"] = (const char *) query.getColumn("RxV6_MAX");
				row["V6_MIN"] = (const char *) query.getColumn("RxV6_MIN");
				response["Data"].append(row);
			}
		}
		else
		{
			//shouldn't reach here
		} 	 	    
	    res.send(response);
	}	


}
void getTx(Request & req, Response & res)
{

    vector<string> conditions;
    string q;
    
	if(findState(conditions,req,res))
	{
	    if(req.query["State"] == "Live")
		{
		    q = "Select * from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-4 hours','-1 seconds');";
	    }
	    else if(req.query["State"] == "Day")
		{
		    q = "Select * from day where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-4 hours','-1 day');";

        }
	    else if(req.query["State"] == "Max")
		{
		    q = "Select * from max where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-4 hours','-15 days');";

        }
		else if(req.query["State"] == "Hour")
		{
			q = "Select * from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours');";
		}
        else
		{
            res.status(400);
        }
    	SQLite::Database db("/fl0/StatsCollector.db", SQLite::OPEN_READONLY);
	    SQLite::Statement query(db,q);
	    //cout << req.request_uri << endl;
	    Json::Value response = _initJson("Tx", "Mbps");
			if(req.query["State"] == "Hour" || req.query["State"] == "Live")
			{
				while(query.executeStep())
				{
					Json::Value row;
					row["Time"] = (const char *)  query.getColumn("Time");
					row["V4"] = (const char *) query.getColumn("TxV4");
					row["V6"] = (const char *) query.getColumn("TxV6");
					response["Data"].append(row);
				}
			}
			else if( req.query["State"] == "Max" || req.query["State"] == "Day")
			{
			  while(query.executeStep())
			  {
		          Json::Value row;
		          row["Time"] = (const char *)  query.getColumn("Time");
		          row["V4"] = (const char *) query.getColumn("TxV4");
		          row["V4_MAX"] = (const char *) query.getColumn("TxV4_MAX");
		          row["V4_MIN"] = (const char *) query.getColumn("TxV4_MIN");
		          row["V6"] = (const char *) query.getColumn("TxV6");
		          row["V6_MAX"] = (const char *) query.getColumn("TxV6_MAX");
		          row["V6_MIN"] = (const char *) query.getColumn("TxV6_MIN");
		          response["Data"].append(row);
			  }
			}
			else
			{
				//shouldn't reach here
			} 	


	    res.send(response);
	}

}

void getMx(Request & req, Response & res)
{

    vector<string> conditions;
    string q;

	if(findState(conditions,req,res))
	{
	    if(req.query["State"] == "Live")
		{
		    q = "Select * from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-4 hours','-1 seconds');";
	    }
	    else if(req.query["State"] == "Day")
		{
		    q = "Select * from day where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-4 hours','-1 day');";

        }
	    else if(req.query["State"] == "Max")
		{
		    q = "Select * from max where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-4 hours','-15 days');";

        }
		else if(req.query["State"] == "Hour")
		{
			q = "Select * from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours');";
		}
        else{
            res.status(400);
        }
    	SQLite::Database db("/fl0/StatsCollector.db", SQLite::OPEN_READONLY);
	    SQLite::Statement query(db,q);
	    //cout << req.request_uri << endl;
        Json::Value response = _initJson("Mx", "Kbps");
		if(req.query["State"] == "Hour" || req.query["State"] == "Live")
			{
		    while(query.executeStep())
			{
				Json::Value row;
				row["Time"] = (const char *)  query.getColumn("Time");
				row["Tx"] = (const char *) query.getColumn("MTx");
				row["Rx"] = (const char *) query.getColumn("MRx");
				response["Data"].append(row);
	    	}
			}
			else if( req.query["State"] == "Max" || req.query["State"] == "Day")
			{
			  while(query.executeStep())
			  {
		          Json::Value row;
		          row["Time"] = (const char *)  query.getColumn("Time");
		          row["Tx"] = (const char *) query.getColumn("MTx");
		          row["Tx_MAX"] = (const char *) query.getColumn("MTx_MAX");
		          row["Tx_Min"] = (const char *) query.getColumn("MTx_MIN");
		          row["Rx"] = (const char *) query.getColumn("MRx");
		          row["Rx_MAX"] = (const char *) query.getColumn("MRx_MAX");
		          row["Rx_Min"] = (const char *) query.getColumn("MRx_MIN");
		          response["Data"].append(row);
			  }
			}
			else
			{
				//shouldn't reach here
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


