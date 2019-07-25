/**
 * @file echo_client.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-07-15
 * 
 * @copyright Copyright (c) 2019
 * 
 */

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

#define MAX 3600
#define DAY 300


typedef struct intf_vals
{
    mutable std::string check;
    mutable std::string label;
    mutable std::string API_Callback;
    mutable std::string unit;
    mutable unsigned long delta;
    mutable float thru;
} intf_val_t;

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

std::vector<intf_val_t> nodes;

std::string input_file;
std::string header_str = "headers:";
std::string data_str = "data:";
std::string SQL_live = "CREATE TABLE 'live' ( 'Time' TEXT NOT NULL";
std::string SQL_day = "CREATE TABLE 'day' ( 'Time' TEXT NOT NULL";
std::string SQL_max = "CREATE TABLE 'max' ( 'Time' TEXT NOT NULL";
std::string SQL_input_live = "INSERT INTO live (Time";
std::string SQL_input_day = "INSERT INTO day (Time";
std::string SQL_input_max = "INSERT INTO max (Time";
std::string SQL_dayQuery;
std::string SQL_maxQuery;
std::string time_prefix = "dateTime('NOW')";


void SQL_CMD(std::string command);
static int callback(void *data, int argc, char **argv, char **azColName);
sqlite3 *db;
char *zErrMsg = 0;

/**
 * @brief SQLite table parameters
 * 
 */
int day_counter, max_counter = 0;

/**
 * @brief 
 * 
 * @param val 
 * @return unsigned long 
 */
unsigned long convert(std::string val)
{
    if (val != "A Default Value if not exists")
    {
        std::stringstream stream(val);
        unsigned long result;
        stream >> result;
        return result;
    }
    return 0;
}

/**
 * @brief 
 * 
 * @param data 
 * @param argc 
 * @param argv 
 * @param azColName 
 * @return int 
 */
static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i; 
    fprintf(stderr, "%s: ", (const char *) data);

    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}
/*----------------------------------------------------------------------
Function: SQL_CMD(std::string command)
Purpose: 
------------------------------------------------------------------------*/
/* 
 * @brief Executes common sqlite3 commands with the command as an input string 'command'
 * 
 * @param command 
 */

void SQL_CMD(std::string command)
{
    sqlite3_exec(db, command.c_str(), callback, NULL, NULL);
}

/**
 * @brief This message handler will be invoked once for each incoming message. 
 * It prints the message and then sends a copy of the message back to the server.
 * 
 * @param c  Client config with asio transport and TLS disabled
 * @param hdl Client connection handler 
 * @param msg 
 */
void on_message(client *c, websocketpp::connection_hdl hdl, message_ptr msg)
{
    std::string results = msg->get_payload();
    std::string parsed_data;
    size_t found_header = results.find("headers:");
    size_t found_data = results.find("data:");

    if (found_data != std::string::npos)
    {
        parsed_data = results.substr(found_data + data_str.length(), results.length());
    }
    else
    {
        return;
    }

    Json::Value pdata;
    Json::StyledWriter styledWriter;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(parsed_data, pdata);
    std::cout << parsingSuccessful << std::endl << std::endl;
    if (parsingSuccessful)
    {
        for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
        {
            (*i).check = pdata.get((*i).API_Callback, "A Default Value if not exists").asString();
        }
        std::cout <<nodes[0].check;
        if (nodes[0].check != "A Default Value if not exists")
        {
            int rc = sqlite3_open("/tmp/StatsCollector.db", &db);
            std::cout << rc;
            if (rc == 0)
            {
                /**checks for open database. If rc == 0, the database is generated**/
                SQL_CMD(SQL_live);
                SQL_CMD(SQL_day);
                SQL_CMD(SQL_max);
            }

            SQL_CMD("ATTACH DATABASE '/tmp/StatsCollector' as 'statscollector';");


                for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
                {
                    (*i).thru = convert((*i).check);
                    if ((*i).unit == "Mbps")
                    {
                        (*i).thru = 8 * (*i).thru / (1024.0 * 1024.0);

                    }
                    else if ((*i).unit == "Kbps")
                    {

                        (*i).thru = 8 * (*i).thru /  1024.0;
                    }
                    else
                    {
                        std::cout << "Unit not recognized" << std::endl;
                    }
                }
                char buffer[800];
                char SQL_thru[800];
                int pass_number = 0;
                for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
                {
                    if (pass_number > 0)
                    {
                        sprintf(SQL_thru, "%s,%f", SQL_thru, (*i).thru);
                    }
                    else
                    {
                        sprintf(SQL_thru, ",%f", (*i).thru);
                    }
                    pass_number++;
                }

                sprintf(SQL_thru, "%s);", SQL_thru);
                sprintf(buffer,"%s VALUES (%s%s",SQL_input_live.c_str(),time_prefix.c_str(),SQL_thru);


                SQL_CMD(buffer);
                std::ofstream os("/fl0/outofbuffer.txt");
                os << buffer;
                day_counter++;
                max_counter++;
                SQL_CMD("Delete from live where Time < strftime('%Y-%m-%d %H:%M:%f', 'NOW','-1 hour');");

                if (day_counter == DAY)
                {
                    SQL_CMD(SQL_dayQuery);
                    SQL_CMD("Delete from day where Time < strftime('%Y-%m-%d %H:%M:%f', 'NOW','-1 day');");
                    day_counter = 0;
                }
                if (max_counter == MAX)
                {
                    SQL_CMD(SQL_maxQuery);
                    SQL_CMD("Delete from day where Time < strftime('%Y-%m-%d %H:%M:%f', 'NOW','-14 days');");
                    max_counter = 0;
                }
                rc = sqlite3_close_v2(db);
        }
    }
}


/**
 * @brief 
 * 
 * @param c 
 * @param hdl 
 */
void on_open(client *c, websocketpp::connection_hdl hdl)
{
    Json::Value configroot;
    Json::Reader configReader;
    std::ifstream file("/fl0/stats_available.json");
    bool out = configReader.parse(file, configroot, true);
    if (!out)
    {
        std::cout << configReader.getFormattedErrorMessages();
    }
    int i = 0;
    while (configroot[i] != Json::Value::null)
    {
        intf_val_t tmp = intf_val_t();
        tmp.API_Callback = configroot[i].get("API_Callback", "A Default Value if not exists").asString();
        tmp.label = configroot[i].get("Label", "A Default Value if not exists").asString();
        tmp.unit = configroot[i].get("Unit", "A Default Value if not exists").asString();
        nodes.push_back(tmp);
        i++;
    }
    for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
    {

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
    SQL_dayQuery = SQL_input_day + " VALUES(datetime('now')";
    SQL_maxQuery = SQL_input_max + " VALUES(datetime('now')";
    for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
    {

        SQL_dayQuery += ",(select avg(" + (*i).label + ") from"
                         " live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW','-5 minutes'))";
        SQL_maxQuery += ",(select avg(" + (*i).label + ") from"
                              " live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', '-1 hours'))";
    }
    SQL_dayQuery = SQL_dayQuery + ");";
    SQL_maxQuery = SQL_maxQuery + ");";

    websocketpp::lib::error_code ec;
    std::ifstream file_input(input_file.c_str());
    Json::Reader reader;
    Json::Value root;
    reader.parse(file_input, root);
    std::string output;

    /*The first keys denote the format of data collection that the client needs to request. 
         * The time row is the frequency of statistics that the server needs to send in milliseconds. */
    output.append("rows=" + root["rows"].asString() + ":");
    output.append("style=" + root["style"].asString() + ":");
    output.append("time=" + root["time"].asString() + ":");

    Json::Value stats = root["stats"];

    for (int index = 0; index < stats.size(); ++index)
    {
        output.append("stats,");
        output.append("system_stats_curr_" + stats[index]["parent_struct"].asString() + "_" + stats[index]["root_struct"].asString() + "_");
        output.append(stats[index]["name"].asString() + ":");
    }

    c->send(hdl, output, websocketpp::frame::opcode::text, ec);
    if (ec)
    {
        std::cout << "Echo failed because: " << ec.message() << std::endl;
    }
}


int main(int argc, char *argv[])
{

    client c;

    std::string uri = "ws://127.0.0.1:9002";

    input_file = argv[1];

    try
    {
        // Set logging to be pretty verbose (everything except message payloads)
        c.set_access_channels(websocketpp::log::alevel::all);
        c.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize ASIO
        c.init_asio();
        
        /**
         * @brief Register our message handler
         * 
         */
        c.set_message_handler(bind(&on_message, &c, ::_1, ::_2));
        c.set_open_handler(bind(&on_open, &c, ::_1));
        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(uri, ec);
        if (ec)
        {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return 0;
        }
    
        /**
         * @brief Note that connect here only requests a connection. 
         * No network messages areexchanged until the event loop starts running in the next line.
         * 
         */
        c.connect(con);

        /*
         * @brief start the ASIO io_service run loop
         * this will cause a single connection to be made to the server. c.run()
         * will exit when this connection is closed.
         */
        c.run();
    }
    catch (websocketpp::exception const &e)
    {
        std::cout << e.what() << std::endl;
    }
}
dashboard