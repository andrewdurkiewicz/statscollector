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
   unsigned long long prev;
   unsigned long long curr;
   unsigned long long delta;
   float thru;
}intf_val_t;

//std::vector<intf_val_t> intf_arr;
intf_val_t LANRxv4;
intf_val_t LANRxv6;
intf_val_t LANTxv4;
intf_val_t LANTxv6;

intf_val_t WANRxv4;
intf_val_t WANRxv6;
intf_val_t WANTxv4;
intf_val_t WANTxv6;

intf_val_t MgmtRx;
intf_val_t MgmtTx;

typedef websocketpp::config::asio_client::message_type::ptr message_ptr;
std::string input_file ="";
std::string header_str="headers:";
std::string data_str="data:";


/*Initialize SQLitecpp parameters*/
void SQL_CMD(std::string comand);
static int callback(void* data, int argc, char** argv, char** azColName);
sqlite3 *db;
char *zErrMsg = 0;

/*SQLite table parameters*/
int day_counter = 0;
int max_counter = 0;

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
        parsed_data = results.substr(found_data+data_str.length(), results.length());
    }
    else
    {
	//something else
	return;
    }

     
    Json::Value pdata;
    Json::StyledWriter styledWriter;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(parsed_data, pdata);
    std::string RxV4, RxV6, TxV4, TxV6, RMx, TMx, WRxV4, WRxV6, WTxV4, WTxV6;
    if (parsingSuccessful)
    {
        /*Grabs the stats values from the parsed json data*/
        RxV4 =  pdata.get("system_stats_curr_intf_lan_v4_pkt_stats_rx_pkt_stats_tot_bytes", "A Default Value if not exists" ).asString();
        RxV6 = pdata.get("system_stats_curr_intf_lan_v6_pkt_stats_rx_pkt_stats_tot_bytes", "A Default Value if not exists" ).asString();
    	TxV4 = pdata.get("system_stats_curr_intf_lan_v4_pkt_stats_tx_pkt_stats_tot_bytes", "A Default Value if not exists" ).asString();
    	TxV6 = pdata.get("system_stats_curr_intf_lan_v6_pkt_stats_tx_pkt_stats_tot_bytes", "A Default Value if not exists" ).asString();
    	RMx = pdata.get("system_stats_curr_intf_v6_mgmt_pkt_stats_rx_pkt_stats_tot_bytes", "A Default Value if not exists" ).asString();
    	TMx = pdata.get("system_stats_curr_intf_v6_mgmt_pkt_stats_tx_pkt_stats_tot_bytes", "A Default Value if not exists" ).asString();

        WRxV4 =  pdata.get("system_stats_curr_intf_wan_v4_pkt_stats_rx_pkt_stats_tot_bytes", "A Default Value if not exists" ).asString();
        WRxV6 = pdata.get("system_stats_curr_intf_wan_v6_pkt_stats_rx_pkt_stats_tot_bytes", "A Default Value if not exists" ).asString();
        WTxV4 = pdata.get("system_stats_curr_intf_wan_v4_pkt_stats_tx_pkt_stats_tot_bytes", "A Default Value if not exists" ).asString();
        WTxV6 = pdata.get("system_stats_curr_intf_wan_v6_pkt_stats_tx_pkt_stats_tot_bytes", "A Default Value if not exists" ).asString();
    

        if(RxV4 != "A Default Value if not exists")
        {
            int rc = sqlite3_open("/fl0/StatsCollector.db", &db);
            if (rc == 0)
            {
                /**checks for open database. If rc == 0, the database is generated**/
                SQL_CMD("/fl0/StatsCollector.db");
        	SQL_CMD("CREATE TABLE 'live' ( 'Time' TEXT NOT NULL, 'RxV4' REAL,'RxV6' REAL,'TxV4' REAL,'TxV6' REAL,'MRx' REAL,'MTx' REAL);");
        	SQL_CMD("CREATE TABLE 'day' ( 'Time' TEXT NOT NULL, 'RxV4' REAL, 'RxV4_MAX' REAL, 'RxV4_MIN' REAL,"
            		" 'RxV6' REAL, 'RxV6_MAX' REAL, 'RxV6_MIN' REAL, 'TxV4' REAL, 'TxV4_MAX' REAL, 'TxV4_MIN' REAL,"
            		" 'TxV6' REAL, 'TxV6_MAX' REAL, 'TxV6_MIN' REAL, 'MRx' REAL, 'MRx_MAX' REAL, 'MRx_MIN' REAL,"
            		" 'MTx' REAL, 'MTx_MAX' REAL, 'MTx_MIN' REAL );");
		SQL_CMD("CREATE TABLE 'max' ( 'Time' TEXT NOT NULL, 'RxV4' REAL, 'RxV4_MAX' REAL, 'RxV4_MIN' REAL,"
            		" 'RxV6' REAL, 'RxV6_MAX' REAL, 'RxV6_MIN' REAL, 'TxV4' REAL, 'TxV4_MAX' REAL, 'TxV4_MIN' REAL,"
            		" 'TxV6' REAL, 'TxV6_MAX' REAL, 'TxV6_MIN' REAL, 'MRx' REAL, 'MRx_MAX' REAL, 'MRx_MIN' REAL,"
            		" 'MTx' REAL, 'MTx_MAX' REAL, 'MTx_MIN' REAL );");
            }

            SQL_CMD("ATTACH DATABASE '/fl0/StatsCollector' as 'statscollector';");
            
            if(entered_flag == false)
            {
                std::cout << "Got here";
                /*First iteration is always empty, this sets up the baseline for forward progression*/
                LANRxv4.curr=convert(RxV4);
                LANRxv6.curr=convert(RxV6);
                LANTxv4.curr=convert(TxV4);
                LANTxv6.curr=convert(TxV6);
                WANRxv4.curr=convert(WRxV4);
                WANRxv6.curr=convert(WRxV6);
                WANTxv4.curr=convert(WTxV4);
                WANTxv6.curr=convert(WTxV6);
                MgmtRx.curr=convert(RMx);
                MgmtTx.curr=convert(TMx);

                
                LANRxv4.prev = LANRxv4.curr;
                LANRxv6.prev = LANRxv6.curr;
                LANTxv4.prev = LANTxv4.curr;
                LANTxv6.prev = LANTxv6.curr;

                WANRxv4.prev = WANRxv4.curr;
                WANRxv6.prev = WANRxv6.curr;
                WANTxv4.prev = WANTxv4.curr;
                WANTxv6.prev = WANTxv6.curr;
                MgmtRx.prev = MgmtRx.curr;
                MgmtTx.prev = MgmtTx.curr;

                entered_flag = true;
            }
            else
            {
                gettimeofday(&tv1, NULL);
                curr=(double) (tv1.tv_usec)/1000000 + (double) (tv1.tv_sec);
                delta_time = (double)(curr - prev);
                printf ("Total time = %f seconds\n",delta_time);



                LANRxv4.curr=convert(RxV4);
                LANRxv4.thru = 8*(LANRxv4.curr - LANRxv4.prev)/ (1024.0*1024.0 * delta_time);


                LANRxv6.curr=convert(RxV6);
                LANRxv6.thru = 8*(LANRxv6.curr - LANRxv6.prev)/ (1024.0*1024.0 * delta_time);
                //2x on bottom. There is an issue where LANTxv4 is double counting packets, this is a temp fix
                
                LANTxv4.curr=convert(TxV4);
                LANTxv4.thru = 8*(LANTxv4.curr - LANTxv4.prev)/ (2*1024.0*1024.0 * delta_time);

                LANTxv6.curr=convert(TxV6);
                LANTxv6.thru = 8*(LANTxv6.curr - LANTxv6.prev)/ (1024.0*1024.0 * delta_time);

                WANRxv4.curr=convert(WRxV4);
                WANRxv4.thru = 8*(WANRxv4.curr - WANRxv4.prev)/ (1024.0*1024.0 * delta_time);

                WANRxv6.curr=convert(WRxV6);
                WANRxv6.thru = 8*(WANRxv6.curr - WANRxv6.prev)/ (1024.0*1024.0 * delta_time);

                WANTxv4.curr=convert(WTxV4);
                WANTxv4.thru = 8*(WANTxv4.curr - WANTxv4.prev)/ (1024.0*1024.0 * delta_time);

                WANTxv6.curr=convert(WTxV6);
                WANTxv6.thru = 8*(WANTxv6.curr - WANTxv6.prev)/ (1024.0*1024.0 * delta_time);

                MgmtRx.curr=convert(RMx);
                MgmtRx.thru = 8*(MgmtRx.curr - MgmtRx.prev)/ (1024.0 * delta_time);

                MgmtTx.curr=convert(TMx);
                MgmtTx.thru = 8*(MgmtTx.curr - MgmtTx.prev)/ (1024.0 * delta_time);


                char buffer[200];
		std::string time_prefix = "strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours')";
                sprintf(buffer,
                    "INSERT INTO live (Time,RxV4,RxV6,TxV4, TxV6, MRx, MTx) VALUES (%s,%-.2f,%-.2f,%-.2f,%-.2f,%-.2f,%-.2f);",
                    time_prefix.c_str(),            
					WANRxv4.thru,
                    WANRxv6.thru,
                    WANTxv4.thru,
                    WANTxv6.thru,
                    MgmtRx.thru,
                    MgmtTx.thru
                    );
            
                /**Store Previous values for next iteration. Used for delta calculation **/ 
                LANRxv4.prev = LANRxv4.curr;
                LANRxv6.prev = LANRxv6.curr;
                LANTxv4.prev = LANTxv4.curr;
                LANTxv6.prev = LANTxv6.curr;

                WANRxv4.prev = WANRxv4.curr;
                WANRxv6.prev = WANRxv6.curr;
                WANTxv4.prev = WANTxv4.curr;
                WANTxv6.prev = WANTxv6.curr;
                MgmtRx.prev = MgmtRx.curr;
                MgmtTx.prev = MgmtTx.curr;
                
                prev=curr;

                
                SQL_CMD(buffer);
                day_counter++;
                max_counter++;
                SQL_CMD("Delete from live where Time < strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hour');");
                if(day_counter == 300)
                {
                    /**Averages last 5 minutes from live into table day**/

                    
			SQL_CMD("INSERT INTO day (Time, RxV4, RxV4_MAX, RxV4_MIN, RxV6, RxV6_MAX, RxV6_MIN,TxV4, TxV4_MAX,"
			"TXV4_MIN, TxV6, TxV6_MAX, TXV6_MIN, MRx,MRx_MAX, MRx_MIN, MTx, MTx_MAX, MTx_MIN) VALUES" 
			"(datetime('now', 'localtime'),"  
			"(select avg(RxV4) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select max(RxV4) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select min(RxV4) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select avg(RxV6) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select max(RxV6) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select min(RxV6) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select avg(TxV4) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select max(TxV4) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select min(TxV4) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select avg(TxV6) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select max(TxV6) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select min(TxV6) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select avg(MRx) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select max(MRx) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select min(MRx) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select avg(MTx) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select max(MTx) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')),"
			"(select min(MTx) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-5 minutes')));");

			SQL_CMD("Delete from day where Time < strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime', '-4 hours','-1 day');");

                    day_counter = 0;
                }
                if(max_counter == 3600)
                {
                    /**Averages last hour from live into table max**/
			SQL_CMD(""
			"INSERT INTO max (Time, RxV4, RxV4_MAX, RxV4_MIN, RxV6, RxV6_MAX, RxV6_MIN,TxV4, TxV4_MAX,"
			"TXV4_MIN, TxV6, TxV6_MAX, TXV6_MIN, MRx,MRx_MAX, MRx_MIN, MTx, MTx_MAX, MTx_MIN) VALUES" 
			"(datetime('now', 'localtime'),"  
			"(select avg(RxV4) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select max(RxV4) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select min(RxV4) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select avg(RxV6) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select max(RxV6) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select min(RxV6) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select avg(TxV4) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select max(TxV4) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select min(TxV4) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select avg(TxV6) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select max(TxV6) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select min(TxV6) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select avg(MRx) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select max(MRx) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select min(MRx) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select avg(MTx) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select max(MTx) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')),"
			"(select min(MTx) from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-5 hours')));");
                    SQL_CMD("Delete from day where Time < strftime('%Y-%m-%d %H:%M:%f', 'NOW', 'localtime','-15 days');");


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