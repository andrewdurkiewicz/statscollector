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

#include <iostream> 
#include <sqlite3.h> 
#include <string> 
#include <stdio.h>
#include <unistd.h>  
#include <cmath>    

// ALL INCLUDES AND DECLARATIONS AFTER THIS POINT PRETAIN TO NETWORKING ///////////////////
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>
#include <fstream>

#include <unistd.h>
#include <stdio.h>
#include "json/json.h"
#include <tickLib.h>
#include "ts100mhz.h"
#include "utilsLib.h"


#include <ctime>

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

std::string filename;

using namespace std; 
//to run:  g++ -o statscollector statscollector.cpp -lsqlite3 && ./statscollector

typedef websocketpp::config::asio_client::message_type::ptr message_ptr;
FILE *output_file;
std::string input_file;
std::string output_filename;
std::string request;

#define BILLION  1000000000.0;
#define STATS_FILE_HDR_PFX "Device_ID,ESN,Collection_Start_Time,Collection_End_Time"
time_t coln_start_time;
time_t coln_end_time;

typedef struct stats_data_s{
    std::string name;      
	std::string header;       //header of the column in output csv file
    std::string datatype;
    std::string curr;
	std::string prev;
	std::string val;       //current value or delta value
}stats_data_t;

std::vector<stats_data_t> conf_data;
unsigned int handle_alignment = 0;

// ALL INCLUDES AND DECLARATIONS BEFORE THIS POINT PRETAIN TO NETWORKING ///////////////////

/////////////////// NETWORKING CODE ///////////////////
void on_open(client* c, websocketpp::connection_hdl hdl) 
{	
	websocketpp::lib::error_code ec;
    std::ifstream file_input(input_file.c_str());
	Json::Reader reader;
	Json::Value root;
	stats_data_t temp_dat;
	reader.parse(file_input, root);
	std::string output;
	    
	output.append("rows="+root["rows"].asString()+":");
	output.append("style="+root["style"].asString()+":");
	output.append("time="+root["time"].asString()+":");
	
	Json::Value stats = root["stats"];
	
	for ( int index = 0; index < stats.size(); ++index ) {
		output.append(root["parent_struct"].asString()+",");
		output.append("system_stats_curr_"+stats[index]["root_struct"].asString()+"_");
		output.append(stats[index]["name"].asString()+":");
		if(stats[index]["header"] == "")
		{
			temp_dat.header = stats[index]["name"].asString()+"_"+stats[index]["field"].asString();
		}
		else
		{
		    temp_dat.header = stats[index]["header"].asString();
		}
		temp_dat.val = stats[index]["field"].asString();
		temp_dat.name = "system_stats_curr_"+stats[index]["root_struct"].asString()+"_"+stats[index]["name"].asString();
		temp_dat.datatype = "";
		temp_dat.curr = "";
		temp_dat.prev = "";
		conf_data.push_back(temp_dat);
	}
	
    c->send(hdl,output,websocketpp::frame::opcode::text,ec);
    std::cout << "Message sent : " <<  output << std::endl;
    if (ec)
    {
        std::cout << "Echo failed because: " << ec.message() << std::endl;
    }
	
	output_file = fopen(output_filename.c_str(),"w");
    if (output_file == NULL)
    {
        std::cout << "file open failed" << std::endl;
    }
	coln_start_time = time(NULL);
   
}
/////////////////// NETWORKING CODE ///////////////////

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

//CREATE TABLE 'stats' ( 'Time' INTEGER NOT NULL, 'UL' REAL, 'DL' REAL, 'Throughput' REAL);
  
void SQL_CMD(string comand);
sqlite3 *db;
char *zErrMsg = 0;

int main(int argc, char* argv[])
{

   /////////////////// NETWORKING CODE ///////////////////

       // Create a client endpoint
    client c;

    std::string uri = "ws://127.0.0.1:9002";    
    input_file = argv[1];
    output_filename = argv[2];
    
    try {
        // Set logging to be pretty verbose (everything except message payloads)
        c.set_access_channels(websocketpp::log::alevel::none); //all
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

   /////////////////// NETWORKING CODE ///////////////////
   for(int i = 0; i<10800; i++)
   {
      cout << i << endl;
      int rc = sqlite3_open("StatsCollector.db", &db);
      if (rc == 0)
      {
        //checks if database exist, if not, create it and the table
        SQL_CMD("StatsCollector.db");
        SQL_CMD("CREATE TABLE 'stats' ( 'Time' INTEGER NOT NULL, 'UL' REAL, 'DL' REAL, 'Throughput' REAL);");

      }
      SQL_CMD("ATTACH DATABASE 'StatsCollector' as 'statscollector';");

      char buffer[200];
      sprintf(buffer,"INSERT INTO stats (Time,UL,DL,Throughput) VALUES (%i,%-.2f,%-.2f,%-.2f);",i, i*2.2, i*4.3, i*1.5);
      SQL_CMD(buffer);
      //SQL_CMD("SELECT * FROM stats WHERE 1;");
      sqlite3_close(db);

      //usleep(60*pow(10,6)); //sleep for 1 minute 

   }

}

void SQL_CMD(string command){

   sqlite3_exec(db, command.c_str(), callback, NULL, NULL);
}

