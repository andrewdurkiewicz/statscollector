/**
 * @file echo_client.cpp
 * @author Andrew Durkiewicz
 * @brief This program serves as a stats client. It creates a SQLite database and stores stats from the server into the database
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
#include <mqueue.h>
#include <tfw.h>
#include <sysmonitor.h>
#include <pthread.h>
#include "json/json.h"
#include <unistd.h>
#include <sstream>
#include "terminalCommon.h"

#define MAX 3600
#define DAY 300
static int _shmIgmpIndex;
static char *_procName;
int igmpLogId = LOG_ERR;
static mqd_t _mqdesSelf;

typedef struct intf_vals
{
    mutable std::string value;
    mutable std::string label;
    mutable std::string API_Callback;
    mutable std::string unit;
    mutable float thru;
} intf_val_t;

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

std::vector<intf_val_t> nodes;
Json::Value configroot;

std::string input_file;
std::string header_str = "headers:";
std::string data_str = "data:";
std::string SQL_Create_Live = "CREATE TABLE 'live' ( 'Time' TEXT NOT NULL";
std::string SQL_Create_Day = "CREATE TABLE 'day' ( 'Time' TEXT NOT NULL";
std::string SQL_Create_Max = "CREATE TABLE 'max' ( 'Time' TEXT NOT NULL";
std::string SQL_Insert_Live = "INSERT INTO live (Time";
std::string SQL_Insert_Day = "INSERT INTO day (Time";
std::string SQL_Insert_Max = "INSERT INTO max (Time";
std::string SQL_Query_Day;
std::string SQL_Query_Max;
std::string time_prefix = "dateTime('NOW')";

void SQL_CMD(std::string command);
static int callback(void *data, int argc, char **argv, char **azColName);
sqlite3 *db;
char *zErrMsg = 0;
int day_counter, max_counter = 0;

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
 * @brief This callback provides a way to obtain results from SELECT statements. It has the following declaration
 * 
 * @param data Data provided in the 4th argument of sqlite3_exec()
 * @param argc The number of columns in row
 * @param argv An array of strings representing fields in the row
 * @param azColName An array of strings representing column names
 */
static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char *)data);

    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}

/*---------------------------------------------------------------------
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
 * @param hdl A handle to uniquely identify a connection.
 * @param msg Represents a buffer for a single WebSocket message.
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
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(parsed_data, pdata);

    if (parsingSuccessful)
    {
        for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
        {
            (*i).value = pdata[(*i).API_Callback].asString();
        }
        std::cout << nodes[0].value;
        if (nodes[0].value != "")
        {
            int rc = sqlite3_open("/tmp/StatsCollector.db", &db);
            std::cout << rc;
            if (rc == 0)
            {
                /**checks for open database. If rc == 0, the database is generated**/
                SQL_CMD(SQL_Create_Live);
                SQL_CMD(SQL_Create_Day);
                SQL_CMD(SQL_Create_Max);
            }

            SQL_CMD("ATTACH DATABASE '/tmp/StatsCollector' as 'statscollector';");

            for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
            {
                (*i).thru = convert((*i).value);
                if ((*i).unit == "Mbps")
                {
                    (*i).thru = 8 * (*i).thru / (1024.0 * 1024.0);
                }
                else if ((*i).unit == "Kbps")
                {

                    (*i).thru = 8 * (*i).thru / 1024.0;
                }
                else
                {
                    std::cout << "Unit not recognized" << std::endl;
                }
            }
            std::string buffer;
            std::string SQL_thru;
            for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
            {
                SQL_thru += "," + boost::to_string((*i).thru);
            }

            SQL_thru = SQL_thru + ");";
            buffer = SQL_Insert_Live + " VALUES (" + time_prefix + SQL_thru;

            SQL_CMD(buffer);
            day_counter++;
            max_counter++;
            SQL_CMD("Delete from live where Time < strftime('%Y-%m-%d %H:%M:%f', 'NOW','-1 hour');");

            if (day_counter == DAY)
            {
                SQL_CMD(SQL_Query_Day);
                SQL_CMD("Delete from day where Time < strftime('%Y-%m-%d %H:%M:%f', 'NOW','-1 day');");
                day_counter = 0;
            }
            if (max_counter == MAX)
            {
                SQL_CMD(SQL_Query_Max);
                SQL_CMD("Delete from max where Time < strftime('%Y-%m-%d %H:%M:%f', 'NOW','-14 days');");
                max_counter = 0;
            }
            rc = sqlite3_close_v2(db);
        }
    }
}

/**
 * @brief On connection opening
 * 
 * @param c Client config with asio transport and TLS disabled
 * @param hdl handler A handle to uniquely identify a connection.
 */
void on_open(client *c, websocketpp::connection_hdl hdl)
{
    Json::Reader configReader;
    std::ifstream file("/vsat/apps/stats_config.json");
    bool out = configReader.parse(file, configroot, true);
    if (!out)
    {
        std::cout << configReader.getFormattedErrorMessages();
    }
    Json::Value Parameters = configroot["parameters"];
    for (int index = 0; index < Parameters.size(); ++index)
    {
        intf_val_t tmp = intf_val_t();
        tmp.API_Callback = Parameters[index]["API_Callback"].asString();
        tmp.label = Parameters[index]["Label"].asString();
        tmp.unit = Parameters[index]["Unit"].asString();
        nodes.push_back(tmp);
    }
    for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
    {

        SQL_Create_Live += ", '" + (*i).label + "' REAL";
        SQL_Create_Day += ", '" + (*i).label + "' REAL";
        SQL_Create_Max += ", '" + (*i).label + "' REAL";
        SQL_Insert_Live += ", " + (*i).label;
        SQL_Insert_Day += ", " + (*i).label;
        SQL_Insert_Max +=  ", " + (*i).label;
    }

    SQL_Create_Live += " );";
    SQL_Create_Day  += " );";
    SQL_Create_Max  += " );";
    SQL_Insert_Live +=  ")";
    SQL_Insert_Day  +=  ")";
    SQL_Insert_Max  +=  ")";
    SQL_Query_Day = SQL_Insert_Day + " VALUES(datetime('now')";
    SQL_Query_Max = SQL_Insert_Max + " VALUES(datetime('now')";
    for (std::vector<intf_val_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
    {

        SQL_Query_Day += ",(select avg(" + (*i).label + ") from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW','-5 minutes'))";
        SQL_Query_Max += ",(select avg(" + (*i).label + ") from live where Time > strftime('%Y-%m-%d %H:%M:%f', 'NOW', '-1 hours'))";
    }
    SQL_Query_Day += ");";
    SQL_Query_Max += ");";

    websocketpp::lib::error_code ec;
    std::string output;

    /**
     * @brief   The first key denote the format of data collection that the client needs to request. 
                The time row is the frequency of statistics that the server needs to send in milliseconds.
     * 
     */ 

    output.append("rows=" + configroot["rows"].asString() + ":");
    output.append("style=" + configroot["style"].asString() + ":");
    output.append("time=" + configroot["time"].asString() + ":");

    Json::Value stats = configroot["stats"];

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
static void _igmpProcessHeartBeat()
{
    setShmConfig(_shmIgmpIndex, SHM_GOOD);
}

static int _initMq(void)
{
    struct mq_attr attributeMq;
    char qname[8];
    int ret = 0;
    int status;

    MR_GET_QNAME(qname, WSC_APPL_ID);
    ret = mq_unlink(qname);

    fflush(stdout);
    attributeMq.mq_maxmsg = MR_MAX_MSGS;
    attributeMq.mq_msgsize = MR_MAX_MTU;
    _mqdesSelf = mq_open(qname, O_CREAT | O_RDONLY, 0777, &attributeMq);

    if (_mqdesSelf == (mqd_t)-1)
    {
        return ERROR;
    }

    status = mrSubscribe(WSC_APPL_ID, MR_INTERNAL, SHM_HEART_BEAT_MSGID);
    if (status != MR_RET_OK)
    {
        return ERROR;
    }

    status = mrSubscribe(WSC_APPL_ID, MR_INTERNAL, CFM_CONF_UPDATE_MSGID);
    if (status != MR_RET_OK)
    {
         return ERROR;
    }

    return OK;
}

static void _messageCheck(MrIpcMsg_t *rxMsg)
{
    switch (rxMsg->ipcHdr.msgType)
    {
        case SHM_HEART_BEAT_MSGID:
            _igmpProcessHeartBeat();
            break;
        default:
            break;
    }
    return;
}

void * hb_thread(void *args)
{

    MrIpcMsg_t *rxMsg = (MrIpcMsg_t *)malloc(MR_MAX_MTU);
    if (ERROR == _initMq())
    {
        free(rxMsg);
        return NULL;
    }
    int fd_max = 0;
    fd_set read_fds;

    while (TRUE)
    {
        FD_ZERO(&read_fds);
        bzero(rxMsg, MR_MAX_MTU);
        fd_max = _mqdesSelf;
        FD_SET(_mqdesSelf, &read_fds);


        if (select(fd_max + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            perror("select error..possibly rcvd a signal..ignoring");
            continue;
        }

        if (errno == EINTR)
            continue;


        if (FD_ISSET(_mqdesSelf, &read_fds))
        {
            if ((mq_receive(_mqdesSelf, (char *)rxMsg, MR_MAX_MTU, NULL) <= 0))
            {

            }
            else
            {
                _messageCheck(rxMsg);
            }
        }

     
    }
}

int main(int argc, char *argv[])
{

    client c;

    std::string uri = "ws://127.0.0.1:9002";

    input_file = "/vsat/apps/stats_config.json";
    if (argv[1] != NULL)
    {
        _shmIgmpIndex = atoi(argv[1]);
        _procName = argv[0];
        updateShmConfig();
    }
    else
    {
        exit(1);
    }
    pthread_t th;
    pthread_create(&th, NULL, hb_thread, NULL);


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