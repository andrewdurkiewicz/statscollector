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
#include <fstream>

#include "json/json.h"
#include <unistd.h>

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

int period;
std::string input_file ="";
std::string header_str="headers:";
std::string data_str="data:";
// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;



// This message handler will be invoked once for each incoming message. It
// prints the message and then sends a copy of the message back to the server.
void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) 
{
    std::string results = msg->get_payload();
    std::string parsed_data;

    size_t found_header = results.find("headers:");
    std::cout << "on msg TEST\n" << results << std::endl;
    size_t found_data = results.find("data:");   
    
    if (found_header!=std::string::npos)
    {
	parsed_data = results.substr(found_header+header_str.length(), results.length());
    }
    else if (found_data!=std::string::npos)
    {
        parsed_data = results.substr(found_header+data_str.length(), results.length());
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
    if (parsingSuccessful)
    {
        std::cout<< styledWriter.write(pdata) << std::endl;
    }

    //for(Json::ValueIterator itr = parsed_data.begin() ; itr != parsed_data.end() ; itr++)
    //{
	//printf("%s\n",itr.key().asString().c_str());
	//printf("\n");
	    //printf("%s\n",itr.key().asString().c_str());
    //}
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
    std::cout << "Message sent : " <<  output << std::endl;
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
