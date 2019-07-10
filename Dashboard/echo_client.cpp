/*
 * Copyright (c) 2016, Peter Thorson. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the WebSocket++ Project nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL PETER THORSON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "websocketpp/config/asio_no_tls_client.hpp"
#include "websocketpp/client.hpp"
#include <iostream> 
#include <sqlite3.h> 
#include <string> 
#include <stdio.h>
#include <unistd.h> 
#include <math.h>   
#include <cmath> 
#include <time.h>
#include <stdlib.h> 
#include <iostream>
#include <fstream>
#include <time.h>
#include "json/json.h"
#include <unistd.h>
#include <sstream>

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
clock_t  prev_time, current_time;
double delta_time;
double prev,curr;

struct timeval  tv1;

typedef struct intf_vals
{
    mutable std::string check;
    mutable std::string label;
    mutable std::string API_Callback;
    mutable std::string unit;
    mutable unsigned long long prev;
    mutable unsigned long long curr;
    mutable unsigned long long delta;
    mutable float thru;
}intf_val_t;

//std::vector<intf_val_t> intf_arr;
// intf_val_t LANRxv4;
// intf_val_t LANRxv6;
// intf_val_t LANTxv4;
// intf_val_t LANTxv6;

// intf_val_t WANRxv4;
// intf_val_t WANRxv6;
// intf_val_t WANTxv4;
// intf_val_t WANTxv6;

// intf_val_t MgmtRx;
// intf_val_t MgmtTx;
std::vector<intf_val_t> nodes;

typedef websocketpp::config::asio_client::message_type::ptr message_ptr;
std::string input_file ="";
std::string header_str="headers:";
std::string data_str="data:";
std::string SQL_live = "CREATE TABLE 'live' ( 'Time' TEXT NOT NULL";
std::string SQL_day = "CREATE TABLE 'day' ( 'Time' TEXT NOT NULL";
std::string SQL_max = "CREATE TABLE 'max' ( 'Time' TEXT NOT NULL";
std::string SQL_input_live = "INSERT INTO live (Time";
std::string SQL_input_day = "INSERT INTO day (Time";
std::string SQL_input_max = "INSERT INTO max (Time";
std::string SQL_dayQuery;
std::string SQL_maxQuery;

/*Initialize SQLitecpp parameters*/
void SQL_CMD(std::string comand);
static int callback(void* data, int argc, char** argv, char** azColName);
sqlite3 *db;
char *zErrMsg = 0;

/*SQLite table parameters*/
int day_counter = 0;
int max_counter = 0;
std::string my_toString (float number){
    std::cout<<"My_toString" << std::endl;
    std::ostringstream buff;
    buff<<number;
    return buff.str();   
}
unsigned long long convert(std::string val)
{
    if(val != "A Default Value if not exists")
    {
            std::stringstream stream(val);
            unsigned long long result;
            stream >> result;
            return result;
    }
    return 0;
}

bool entered_flag = false;
// bool connectDB() {
//     if (false == isOpenDB && sqlite3_open(DB, &dbfile) == SQLITE_OK) {
//         isOpenDB = true;
//     }
//     return isOpenDB;
// }
/*----------------------------------------------------------------------
Function: callback(void* data, int argc, char** argv, char** azColName) 
Data Change Notification Callbacks  
Purpose: Registers a callback function with the database connection identified by the first argument 
            to be invoked whenever a row is updated, inserted or deleted in a rowid table.
            This function is essential for the functionality of SQL_CMD(std::string command)
------------------------------------------------------------------------*/
static int callback(void* data, int argc, char** argv, char** azColName) 
{ 
    int i; 
    fprintf(stderr, "%s: ", (const char*)data); 
  
    for (i = 0; i < argc; i++) 
    { 
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL"); 
    } 
  
    printf("\n"); 
    return 0; 
} 
/*----------------------------------------------------------------------
Function: SQL_CMD(std::string command)
Purpose: Executes common sqlite3 commands with the command as an input string 'command'
------------------------------------------------------------------------*/
void SQL_CMD(std::string command)
{
   sqlite3_exec(db, command.c_str(), callback, NULL, NULL);
}

// This message handler will be invoked once for each incoming message. It
// prints the message and then sends a copy of the message back to the server.
void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) 
{

    std::string results = msg->get_payload();
    std::string parsed_data;
    size_t found_header = results.find("headers:");
    size_t found_data = results.find("data:");   


    if (found_data!=std::string::npos)
    {
        std::cout<<"174" << std::endl;
        parsed_data = results.substr(found_data+data_str.length(), results.length());
    }
    else
    {
        std::cout<<"179" << std::endl;
        return;
    }


    Json::Value pdata;
    Json::StyledWriter styledWriter;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(parsed_data, pdata);
    std::string RxV4, RxV6, TxV4, TxV6, RMx, TMx, WRxV4, WRxV6, WTxV4, WTxV6;
    if (parsingSuccessful)
    {
        for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
        {
            std::cout<<"194" << std::endl;
            (*i).check = pdata.get((*i).API_Callback,"A Default Value if not exists" ).asString();
        }
        if(nodes[0].check != "A Default Value if not exists")
        {
            int rc = sqlite3_open("/fl0/StatsCollector.db", &db);
            if (rc == 0)
            {
                /**checks for open database. If rc == 0, the database is generated**/
                SQL_CMD(SQL_live);
                SQL_CMD(SQL_day);
                SQL_CMD(SQL_max);
            }

            SQL_CMD("ATTACH DATABASE '/fl0/StatsCollector' as 'statscollector';");

            if(entered_flag == false)
            {
                for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
                {
                    std::cout<<"216" << std::endl;
                    (*i).curr=convert((*i).check);
                    (*i).prev=(*i).curr;
                }
                entered_flag = true;
            }
            else
            {
                gettimeofday(&tv1, NULL);
                curr=(double) (tv1.tv_usec)/1000000 + (double) (tv1.tv_sec);
                delta_time = (double)(curr - prev);
                for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
                {
                    std::cout<<"231" << std::endl;
                    (*i).curr=convert((*i).check); 
                    if((*i).unit == "Mbps")
                    {
                        (*i).thru=8*((*i).curr-(*i).prev)/(1024.0*1024.0 * delta_time);
                    }
                    else
                    {
                        (*i).thru=8*((*i).curr-(*i).prev)/(1024.0 * delta_time);
                    }
                }
                char buffer[200];
                std::cout << "buffer: " << buffer << std::endl;
                std::string SQL_thru;
                for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
                {
                    std::cout<<"251" << std::endl;
                    SQL_thru = SQL_thru + "," + my_toString((*i).thru);
                }

                SQL_thru = SQL_thru + ");";
                std::string time_prefix = "strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours')";
                sprintf(buffer,
                    "%s VALUES (%s%s",
                    SQL_input_live.c_str(),
                    time_prefix.c_str(), 
                    SQL_thru.c_str());
                // std::ofstream out("/fl0/SQL_thru.txt");
                // out << SQL_dayQuery + "\n\n" + SQL_maxQuery;
                // out.close();
                for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
                {
                    std::cout<<"268" << std::endl;
                    (*i).prev = (*i).curr; 
                }       

                prev=curr;


                SQL_CMD(buffer);
                day_counter++;
                max_counter++;
                SQL_CMD("Delete from live where Time < strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hour');");
                if(day_counter == 300)
                {
                /**Averages last 5 minutes from live into table day**/

                    SQL_CMD(SQL_dayQuery);
                    SQL_CMD("Delete from day where Time < strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-1 day');");
                    day_counter = 0;
                }
                if(max_counter == 3600)
                {
                /**Averages last hour from live into table max**/
                    SQL_CMD(SQL_maxQuery);
                    max_counter = 0;
                }
                rc = sqlite3_close_v2(db);    
            }
        }
    }
}
void on_open(client* c, websocketpp::connection_hdl hdl) 
{

    websocketpp::lib::error_code ec;
    std::ifstream file_input(input_file.c_str());
	Json::Reader reader;
	Json::Value root;
	reader.parse(file_input, root);
	std::string output;
	    
        /*The first keys denote the format of data collection that the client needs to request. 
         * The time row is the frequency of statistics that the server needs to send in milliseconds. */
	output.append("rows="+root["rows"].asString()+":");
	output.append("style="+root["style"].asString()+":");
	output.append("time="+root["time"].asString()+":");
	
	Json::Value stats = root["stats"];
	
	for ( int index = 0; index < stats.size(); ++index ) 
        {
                std::cout<<"322" << std::endl;

	    /* The parent struct key denotes the region of shared memory the statistics needs to be collected from.
             * Root_struct: this is the root structure of the data point in the stats memory. In this case, it is mgmt structure
             * Name: name is the variable name of data point
             *      "stats": [
             *          {
             *                "root_struct": "mgmt",
             *                "name": "genStUpTimeMins"
             *          } ]
             *          will translate to 
             *          stats,system_stats_curr_mgmt_genStUpTimeMins             
             *          ":" is used as a delimiter for the server .... for now (#TODO:change to json format string)
             */        
            output.append("stats,");
	    output.append("system_stats_curr_"+stats[index]["parent_struct"].asString()+"_"+stats[index]["root_struct"].asString()+"_");
	    output.append(stats[index]["name"].asString()+":");
	}
	
    c->send(hdl,output,websocketpp::frame::opcode::text,ec);
    gettimeofday(&tv1, NULL);
    prev=(double) (tv1.tv_usec)/1000000 + (double) (tv1.tv_sec);
    //std::cout << "Message sent : " <<  output << std::endl;
    if (ec)
    {
        std::cout << "Echo failed because: " << ec.message() << std::endl;
    }	
}


int main(int argc, char* argv[]) {
    // Create a client endpoint
    
    Json::Value root;
	Json::Reader reader;
	std::ifstream file("/fl0/stats_available.json");
    bool out = reader.parse(file, root, true);
    if(!out)
	{
        std::cout<<reader.getFormattedErrorMessages();
	}
    int i = 0;
    while(root[i] != Json::Value::null)
    {
        intf_val_t tmp = intf_val_t();
        tmp.API_Callback = root[i].get("API_Callback", "A Default Value if not exists").asString();
        tmp.label = root[i].get("Label", "A Default Value if not exists").asString();
        tmp.unit = root[i].get("Unit","A Default Value if not exists").asString();
        nodes.push_back(tmp);
        i++;
    }
    for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
    {
            std::cout<<"375" << std::endl;

        SQL_live = SQL_live + ", '" + (*i).label + "' REAL";
        SQL_day = SQL_day + ", '" + (*i).label + "' REAL";
        SQL_max = SQL_max + ", '" + (*i).label + "' REAL";
        SQL_input_live = SQL_input_live + ", " + (*i).label;
        SQL_input_day = SQL_input_day + ", " + (*i).label;
        SQL_input_max = SQL_input_max + ", " + (*i).label;
    }

    SQL_live = SQL_live + " );";
    SQL_day = SQL_day + " );";
    SQL_max = SQL_max + " );";
    SQL_input_live = SQL_input_live + ")";
    SQL_input_day = SQL_input_day + ")";
    SQL_input_max = SQL_input_max + ")";
    SQL_dayQuery = SQL_input_day + " VALUES(datetime('now', 'localtime')";
    SQL_maxQuery= SQL_input_max + " VALUES(datetime('now', 'localtime')";
    for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
    {
            std::cout<<"395" << std::endl;

        SQL_dayQuery = SQL_dayQuery + ",(select avg(" + (*i).label + ") from"
        " live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes'))";
        SQL_maxQuery = SQL_maxQuery + ",(select avg(" + (*i).label + ") from"
        " live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours'))";
    }
    SQL_dayQuery = SQL_dayQuery + ";";
    SQL_maxQuery = SQL_maxQuery + ";";
    
    client c;

    std::string uri = "ws://127.0.0.1:9002";

    input_file = argv[1];  

    
    try {
        // Set logging to be pretty verbose (everything except message payloads)
        c.set_access_channels(websocketpp::log::alevel::all);
        c.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize ASIO
        c.init_asio();

        // Register our message handler
        c.set_message_handler(bind(&on_message,&c,::_1,::_2));
        c.set_open_handler(bind(&on_open,&c,::_1));
        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(uri, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return 0;
        }

        // Note that connect here only requests a connection. No network messages are
        // exchanged until the event loop starts running in the next line.
        c.connect(con);

        // Start the ASIO io_service run loop
        // this will cause a single connection to be made to the server. c.run()
        // will exit when this connection is closed.
        c.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}
