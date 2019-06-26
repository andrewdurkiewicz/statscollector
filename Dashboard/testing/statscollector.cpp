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
#include <math.h>   
#include <cmath> 
#include <time.h>
#include <stdlib.h> 
using namespace std;   
#define PI 3.14159265

//to run:  g++ -o statscollector statscollector.cpp -lsqlite3 && ./statscollector

/////////////////// NETWORKING CODE ///////////////////
float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

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
int day_counter = 0;
int max_counter = 0;
int main(int argc, char* argv[])
{
	srand(time(NULL));
	rand();

   /////////////////// NETWORKING CODE ///////////////////
    /*
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
    */
   /////////////////// NETWORKING CODE ///////////////////

   /*for(int i = 0; i<60000; i+=10)
   {*/

   while(true){
    
      int rc = sqlite3_open("StatsCollector.db", &db);
      if (rc == 0)
      {
        //checks if database exist, if not, create it and the table
        SQL_CMD("StatsCollector.db");
        SQL_CMD("CREATE TABLE 'live' ( 'Time' TEXT NOT NULL, 'RxV4' REAL,'RxV6' REAL,'TxV4' REAL,'TxV6' REAL,'MRx' REAL,'MTx' REAL);");
        SQL_CMD("CREATE TABLE 'day' ( 'Time' TEXT NOT NULL, 'RxV4' REAL, 'RxV4_MAX' REAL, 'RxV4_MIN' REAL,"
            " 'RxV6' REAL, 'RxV6_MAX' REAL, 'RxV6_MIN' REAL, 'TxV4' REAL, 'TxV4_MAX' REAL, 'TxV4_MIN' REAL,"
            " 'TxV6' REAL, 'TxV6_MAX' REAL, 'TxV6_MIN' REAL, 'MRX' REAL, 'MRX_MAX' REAL, 'MRX_MIN' REAL,"
            " 'MTx' REAL, 'MTx_MAX' REAL, 'MTx_MIN' REAL );");
        SQL_CMD("CREATE TABLE 'max' ( 'Time' TEXT NOT NULL, 'RxV4' REAL, 'RxV4_MAX' REAL, 'RxV4_MIN' REAL,"
            " 'RxV6' REAL, 'RxV6_MAX' REAL, 'RxV6_MIN' REAL, 'TxV4' REAL, 'TxV4_MAX' REAL, 'TxV4_MIN' REAL,"
            " 'TxV6' REAL, 'TxV6_MAX' REAL, 'TxV6_MIN' REAL, 'MRX' REAL, 'MRX_MAX' REAL, 'MRX_MIN' REAL,"
            " 'MTx' REAL, 'MTx_MAX' REAL, 'MTx_MIN' REAL );");


      }
      char buffer1[200];
        sprintf(buffer1,
        "INSERT INTO live (Time,RxV4,RxV6,TxV4, TxV6, MRx, MTx) VALUES (datetime('now', 'localtime'),%-.2f,%-.2f,%-.2f,%-.2f,%-.2f,%-.2f);",
                RandomFloat(2,10),
                RandomFloat(2,10),
                RandomFloat(.5,4),
                RandomFloat(.5,4),
                RandomFloat(0.1, 0.8),
                RandomFloat(0.1,0.8));
        
        SQL_CMD(buffer1);
        day_counter++;
        max_counter++;
        //SQL_CMD("Delete from live where Time < datetime('now','localtime','-1 hour');");
        if(day_counter == 1)
        {
            SQL_CMD("INSERT INTO day (Time, RxV4, RxV4_MAX, RxV4_MIN, RxV6, RxV6_MAX, RxV6_MIN,TxV4, TxV4_MAX,"
            "TXV4_MIN, TxV6, TxV6_MAX, TXV6_MIN, MRx,MRx_MAX, MRx_MIN, MTx, MTx_MAX, MTx_MIN) VALUES" 
            "(datetime('now', 'localtime'),"  
            "(select avg(RxV4) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select max(RxV4) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select min(RxV4) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select avg(RxV6) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select max(RxV6) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select min(RxV6) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select avg(TxV4) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select max(TxV4) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select min(TxV4) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select avg(TxV6) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select max(TxV6) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select min(TxV6) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select avg(MRx) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select max(MRx) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select min(MRx) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select avg(MTx) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select max(MTx) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select min(MTx) from live where Time > datetime('now','localtime','-10 seconds')));");
            

            //SQL_CMD("Delete from day where Time < datetime('now','localtime','-1 day');");

            day_counter = 0;
        }
        if(max_counter == 1)
        {
            SQL_CMD("INSERT INTO max (Time, RxV4, RxV4_MAX, RxV4_MIN, RxV6, RxV6_MAX, RxV6_MIN,TxV4, TxV4_MAX,"
            "TXV4_MIN, TxV6, TxV6_MAX, TXV6_MIN, MRx,MRx_MAX, MRx_MIN, MTx, MTx_MAX, MTx_MIN) VALUES" 
            "(datetime('now', 'localtime'),"  
            "(select avg(RxV4) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select max(RxV4) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select min(RxV4) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select avg(RxV6) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select max(RxV6) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select min(RxV6) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select avg(TxV4) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select max(TxV4) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select min(TxV4) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select avg(TxV6) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select max(TxV6) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select min(TxV6) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select avg(MRx) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select max(MRx) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select min(MRx) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select avg(MTx) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select max(MTx) from live where Time > datetime('now','localtime','-10 seconds')),"
            "(select min(MTx) from live where Time > datetime('now','localtime','-10 seconds')));");
           // SQL_CMD("Delete from day where Time < datetime('now','localtime','-15 days');");

            max_counter = 0;
        }

      //SQL_CMD(buffer);
      //SQL_CMD("SELECT * FROM stats WHERE 1;");
      sqlite3_close_v2(db);
      usleep(pow(10,6)); //sleep for 1 minute 

   }

}

void SQL_CMD(string command){

   sqlite3_exec(db, command.c_str(), callback, NULL, NULL);
}

