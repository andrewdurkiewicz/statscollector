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


/**
 * @brief Get the Dash Config object
 * 
 * @param req ExpressCpp Request
 * @param res ExpressCpp Response
 */
void getDashConfig(Express::Request & req, Express::Response & res)
{
	Json::Value root;
	Json::Reader reader;
	std::ifstream file("/vsat/apps/stats_config.json");
	bool isparsed = reader.parse(file, root, true);
	if(!isparsed)
	{
		Json::Value error_response;		
		TRACE_LOG(LOG_ERR, LOG_ERR, "JSON Failed To Parse");
		error_response["Error"] = reader.getFormattedErrorMessages(); //What statistic is being measured
		res.status(434);
		res.send(error_response);
	}
	Json::Value Parameters = root["parameters"];
	res.send(Parameters);


}

/*
 * @brief This parses the URL query string into a vector string.
 * 
 * @param conditions String-type vector to hold query parameters from URL query string
 * @param req ExpressCpp Request
 * @param res ExpressCpp Response
 * @return true if conditions were successfully parsed into conditions vector
 * @return false if conditions were unsuccessfully parsed into conditions vector. This includes but are not limited to,
 * 				No state parameter, no stat1 parameter
 */
bool findState(vector<std::string> & conditions, Express::Request & req, Express::Response & res)
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

/**
 * @brief Query data and return it as a json.
 * 
 * @param req ExpressCpp Request
 * @param res ExpressCpp Response
 */
void getData(Express::Request & req, Express::Response & res)
{
	Json::Value response;
	vector<std::string> conditions;
	std::string q;
	if(findState(conditions,req,res))
	{
	    if(req.query["State"] == "Live")
		{
		    q = "Select * from live where Time > dateTime('now','-2 seconds');";
	    }
	    else if(req.query["State"] == "Day")
		{
		    q = "Select * from day where Time > dateTime('now','-1 day');";

        }
	    else if(req.query["State"] == "Max")
		{
		    q = "Select * from max where Time > dateTime('now','-14 days');";

        }
		else if(req.query["State"] == "Hour")
		{
			q = "Select * from live where Time > dateTime('now','-1 hours');";
		}
        else{
            res.status(400);
        }	
	}
	    try
		{
			SQLite:: Database db("/tmp/StatsCollector.db", SQLite::OPEN_READONLY );
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




/**
 * @brief 
 * 
 * @param router 
 */
void initDashboardRoutes(Express::Router & router)
{
	router.get("/getConfig",getDashConfig);
	router.get("/getData",getData);
}
