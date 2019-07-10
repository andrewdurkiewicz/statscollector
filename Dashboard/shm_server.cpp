/**
*\file
*\brief Contains member function definitions of the Server Stats class
*/

/******************************************************************************************
 *
 *  FILE NAME: shm_server.cpp
 *
 *  DESCRIPTION:    Contains the function definitions for creating or attaching to 
 *                   shared memory segment, and processing/organizing the data that 
 *                   is retrieved from shared memory
 *
 *--------------------------------------------------------------------------------------------------
 *   Copyright (c) 2018 Hughes Network Systems, LLC
 *   Hughes Proprietary
*******************************************************************************************/

#include "shm_server.hpp"
#include <sys/time.h>
/*  Generated Files from GenSyslog.py
 */
#include <uplink_shm_syslog.generated>
#include <downlink_shm_syslog.generated>
#include <cfm_shm_syslog.generated>
#include <stats_shm_syslog.generated>
#include <cac_shm_syslog.generated>

/*  Namespaces
 */
using namespace std;
using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

double prev,curr,curr2,delta_time;
struct timeval tv1;

bool reuse_flag=TRUE;
int long_interval_count=0;

/************************************************************************
 *
 * FUNCTION: Stats()
 *
 * \brief  Stats class Constructor.
 *                      Initializes the Stats class object
 *
 * \return - 
 *
 ************************************************************************/
Stats::Stats() : num_connection(0) , m_worker(ios) , t(ios,boost::posix_time::milliseconds(1000))
{
    /* Set logging settings */
    min_timer = 1000; // 1sec default time 
    shmserver.set_error_channels(websocketpp::log::alevel::all);
    shmserver.set_access_channels(websocketpp::log::alevel::app);
    
     /* Initialize Asio */
     shmserver.init_asio(&ios);
    
    //shmserver.clear_access_channels(websocketpp::log::alevel::none);
}

Stats::~Stats()
{
   //shmserver.stop_perpetual();
   //m_thread->join();
}


void Stats::tick(const boost::system::error_code &e)
{
    //std::cout<<"timer !"<<std::endl;
    collect_current_stats(1);
    t.expires_at(t.expires_at() + boost::posix_time::milliseconds(min_timer));

    t.async_wait(boost::bind(&Stats::tick,this,boost::asio::placeholders::error));

}
/************************************************************************
 *
 * FUNCTION: init
 *
 * \brief  Member function of Stats class. 
 *			Initializes the Stats class object
 *          
 * \return - True if init successful , otherwise false    
 *
 ************************************************************************/
bool Stats::init()
{
	int port = 9002;
	
	try 
    { 
                
                		
      	/* Register our message handler */
	shmserver.set_open_handler(bind(&Stats::on_open,this,::_1));
        shmserver.set_fail_handler(bind(&Stats::on_fail,this,::_1));
	shmserver.set_message_handler(bind(&Stats::on_message,this,::_1,::_2));
	shmserver.set_close_handler(bind(&Stats::on_close,this,::_1));


	    shmserver.set_reuse_addr(reuse_flag);
		shmserver.listen(boost::asio::ip::address::from_string("127.0.0.1"),port);

		/* Start the server accept loop */
		shmserver.start_accept();

		/* Start the ASIO io_service run loop */
	    //shmserver.run();

    } catch (websocketpp::exception const & e) 
    {
        std::cout << e.what() << std::endl;
        return false;
    } catch (...) 
    {
        std::cout << "other exception" << std::endl;
        return false;
    }

    return true;

}

/************************************************************************
 *
 * FUNCTION: run
 *
 * \brief  Member function of Stats class. 
 *           Starts the asio event loop
 *          
 * \return - True if  successful , otherwise false    
 *
 ************************************************************************/
bool Stats::run()
{
    try 
    {
        t.async_wait(boost::bind(&Stats::tick,this,boost::asio::placeholders::error));
        shmserver.run();
    } 
    catch(websocketpp::exception const &e) 
    {
       	std::cout <<"client run catch"<< e.what() << std::endl;
        if(shmserver.stopped())
        {
            std::cout<<"restart ios \n";
            try { 
            shmserver.reset();
            }
            catch(websocketpp::exception const &e)
            {
		std::cout <<"could not restart ios"<<e.what() << std::endl;
		return false;
            }
        }
    	return false;
        // Websocket exception. Get message via e.what().
    }

    return true;
}

/************************************************************************
 *
 * FUNCTION: stop
 *
 * \brief  Member function of Stats class. 
 *           Stops the asio event loop
 *          
 * \return - void
 *
 ************************************************************************/
void Stats::stop() 
{
    // Stopping the Websocket listener and closing outstanding connections.
    std::cout<<"stop callback \n";
    websocketpp::lib::error_code ec;
    shmserver.stop_listening(ec);
    if (ec) 
    {
        // Failed to stop listening. Log reason using ec.message().
        return;
    }
      
    // Stop the endpoint.
    shmserver.stop();
}


/************************************************************************
 *
 * FUNCTION: on_close
 *
 * \brief  Member function of Stats class. 
 *           Message Handler Callback function called when connection is
 *           closed.
 *          
 * \return - void   
 *
 ************************************************************************/
void Stats::on_close(websocketpp::connection_hdl hdl) {
    
    shmserver.get_alog().write(websocketpp::log::alevel::app, "Connection Closed");
    shmserver.pause_reading(hdl);
    //shmserver.close(hdl,websocketpp::close::status::going_away,"");
}


/************************************************************************
 *
 * FUNCTION: on_fail
 *
 * \brief  Member function of Stats class.
 *           Message Handler Callback function called when connection is
 *           closed.
 *
 * \return - void
 *
 ************************************************************************/
void Stats::on_fail(websocketpp::connection_hdl hdl) {
    shmserver.get_alog().write(websocketpp::log::alevel::app, "On Fail Handler");
}



/************************************************************************
 *
 * FUNCTION: on_open
 *
 * \brief  Member function of Stats class. 
 *            Message Handler Callback function called when connection is
 *           is opened.
 *          
 * \return - void
 *
 ************************************************************************/
void Stats::on_open(websocketpp::connection_hdl hdl) 
{
    shmserver.get_alog().write(websocketpp::log::alevel::app, "Connection opened");
    connection_ptr con = shmserver.get_con_from_hdl(hdl);
    con->sessionid = num_connection++;

}


/************************************************************************
 *
 * FUNCTION: on_message
 *
 * \brief  Member function of Stats class. 
 *         Message Handler Callback function called when connection 
 *         event receives a message
 *          
 * \return - void   
 *
 ************************************************************************/
void Stats::on_message(websocketpp::connection_hdl hdl, message_ptr msg) 
{
    int err = 0;
    connection_ptr con = shmserver.get_con_from_hdl(hdl);
    // check for a special command to instruct the server to stop listening so
    // it can be cleanly exited.
    if (msg->get_payload() == "stop-listening") 
    {
        shmserver.stop_listening();
        return;
    }
    
    websocketpp::lib::error_code ec; 
    string data;
    string output;
    string headers;
    int ret;
    int i = 0;
    
    if((err = _parse_conf(con->sessionid, msg->get_payload())))
    {
        std::cout << "Invalid Data: " << std::endl;
    }
    else
    {
        std::cout << "Connection number: " <<  con->sessionid << std::endl;
        std::cout << "periodicity: " <<  conn_stats[con->sessionid].iter_ms << std::endl;

        if(min_timer >conn_stats[con->sessionid].iter_ms)
        {
            //update min timer collection
            min_timer=conn_stats[con->sessionid].iter_ms;
        }

        headers = _get_headers(conn_stats[con->sessionid].rows, conn_stats[con->sessionid].style, conn_stats[con->sessionid].labels, conn_stats[con->sessionid].paths, conn_stats[con->sessionid].vars, conn_stats[con->sessionid].keys);
        
        //std::cout << "Headers: " <<  headers << std::endl;
        output="headers:";
        conn_stats[con->sessionid].hdl = hdl;
        //connections[num_connection] = hdl;
        output.append(headers);
        set_timer(hdl);
        gettimeofday(&tv1, NULL);
        prev=(double) (tv1.tv_usec)/1000000 + (double) (tv1.tv_sec);
        //tfwStartTimer(con->sessionid, TFW_GET_SEC_TICKS(conn_stats[con->sessionid].iter_ms), TFW_ONESHOT, DIAG_SERV_APPL_ID);
        if(con->get_state() != websocketpp::session::state::open)
        {
            //connection closed
            return;
        }
        else
        {
            try {
            shmserver.send(hdl,output,websocketpp::frame::opcode::text,ec);
            }
            catch(...)
            {
            std::cout << "send failed: on_message closing connection" << std::endl;
                shmserver.pause_reading(hdl);
                shmserver.close(hdl,websocketpp::close::status::going_away,"");
            }
        }
        
 
        //s->get_alog().write(websocketpp::log::alevel::app, "Sent Message: "+output);
        if (ec)
        {
            std::cout << "Echo failed because: " << ec.message() << std::endl;
        }
        //s->interrupt(hdl);
    }
}


/************************************************************************
 *
 * FUNCTION: on_timer
 *
 * \brief  Member function of Stats class. 
 *         Message Handler Callback function called when connection 
 *         event receives a timer expiry
 *          
 * \return - Void  
 *
 ************************************************************************/
void Stats::on_timer(websocketpp::connection_hdl hdl, websocketpp::lib::error_code const & ec) 
{
    //websocketpp::lib::error_code ec;
    connection_ptr con = shmserver.get_con_from_hdl(hdl);
    //std::cout << "on_timer called with hdl: " << con->sessionid << std::endl;
    string data;
    string output;
        
    data = _get_data(conn_stats[con->sessionid].rows, conn_stats[con->sessionid].labels, conn_stats[con->sessionid].paths, conn_stats[con->sessionid].keys);
    output="data:";
    output.append(data);
    //std::cout << "Message sent : " <<  data;
    if(con->get_state() != websocketpp::session::state::open)
    {
        //connection closed
        return;
    }
    else
    {
        try { 
            shmserver.send(conn_stats[con->sessionid].hdl,output,websocketpp::frame::opcode::text);
        }
        catch(...)
        {
           std::cout << "Send failed on_timer: .. closing connection"<< std::endl;
            shmserver.pause_reading(hdl);
           shmserver.close(hdl,websocketpp::close::status::going_away,"");
        }
    }
    
    //s->get_alog().write(websocketpp::log::alevel::app, "Sent Message: "+output);
    gettimeofday(&tv1, NULL);
    curr=(double) (tv1.tv_usec)/1000000 + (double) (tv1.tv_sec);
    delta_time = (double)(curr - prev);
    std::cout<<"Total time = "<<delta_time<<" seconds\n";
    prev=curr;
    set_timer(hdl);  
}


/************************************************************************
 *
 * FUNCTION: set_timer
 *
 * \brief  Member function of Stats class. 
 *         Message Handler Callback function called to reset timer event 
 *      
 *          
 * \return - Void  
 *
 ************************************************************************/
void Stats::set_timer(websocketpp::connection_hdl hdl) 
{
    connection_ptr con = shmserver.get_con_from_hdl(hdl);
    //std::cout << "set_timer called with hdl: " << con->sessionid << std::endl;
    timer_ptr m_timer;
    m_timer = con->set_timer(conn_stats[con->sessionid].iter_ms, bind(&Stats::on_timer,this,hdl,::_1));
}



/************************************************************************
 *
 * FUNCTION: _get_data
 *
 * \brief  Member function of Stats class. 
 *         This method fetches Data from SHM to sends the dat to client 
 *      
 *          
 * \return - string containing data   
 *
 ************************************************************************/
string Stats::_get_data(string &rows, vector<string> &labels, vector<short> &paths, vector<vector<string> > &keys)
{
    
    short correlate = (rows == "correlated") ? 1 : 0;
    vector<vector<string> > data;

    for(int i = 0; i < paths.size(); i++)
    {
        vector<string> temp_data;

        switch(paths[i]) {
            case UPLINK:
                temp_data = get_uplinkSHM_t_data(keys[i]);
                break;
            case DOWNLINK:
                temp_data = get_downlinkSHM_t_data(keys[i]);
                break;
            case CFM:
                temp_data = get_cfm_external_parms_s_data(keys[i]);
                break;
            case CAC:
                temp_data = get_cacSHM_t_data(keys[i]);
                break;
            case STATS:
                temp_data = get_global_stm_t_data(keys[i]);
                break;
            default:
                break;
        }
        data.push_back(temp_data);
        //std::cout << "_get_data() function " << endl;
        //for (std::vector<string>::const_iterator i = temp_data.begin(); i != temp_data.end(); ++i)
        //   std::cout << *i << ' ' <<std::endl;
            
    }

    string data_str = "";
    if(correlate)
    {
        #if 0
        data_str = "";
        for(int i = 0; i < data.size(); i++)
        {
            for(int y = 0; y < data[i].size(); y++)
            {
                data_str += keys[i][y] + ":";
                data_str += data[i][y] + ((i == data.size()-1 && y == data[i].size()-1) ? "" : ",");
            }
        }
        #endif
        #if 1
        data_str = "{";
        for(int i = 0; i < data.size(); i++)
        {
            for(int y = 0; y < data[i].size(); y++)
            {
                data_str += "\"" + keys[i][y] + "\""; 
                data_str += ":";
                data_str += "\"" + data[i][y] + "\"" + ((i == data.size()-1 && y == data[i].size()-1) ? "" : ",");
            }
        }
        data_str+="}";
        #endif
    }
    else
    {
        for(int i = 0; i < data.size(); i++)
        {
            data_str += labels[i] + ':';
            for(int y = 0; y < data[i].size(); y++)
            {
                data_str += data[i][y] + ((y < data[i].size()-1) ? "," : "");
            }
        }
    }
    long_interval_count++;
    printf("get data: %s\n", data_str.c_str());
    return data_str.c_str();
}


/************************************************************************
 *
 * FUNCTION: _get_headers
 *
 * \brief  Member function of Stats class. 
 *         This method fetches the Header Keys from SHM to retrieve data from SHM 
 *      
 *          
 * \return - string containing data headers and its type
 *
 ************************************************************************/
string Stats::_get_headers(string &rows, string &style, vector<string> &labels, vector<short> &paths, vector<vector<string> > &vars, vector<vector<string> > &keys_for_vars)
{
    short leaf = (style == "leaf") ? 1 : 0;
    short correlate = (rows == "correlated") ? 1 : 0;

    //std::cout << "_get_headers path size: " << paths.size() << endl;
    vector<vector<string> > keytype_for_vars;

    for(int i = 0; i < paths.size(); i++)
    {
        vector<string> temp_keys;
        vector<string> key_types;
        //std::cout << "_get_headers pathi: " << paths[i] << endl;
        switch(paths[i]) {
            case UPLINK:
                temp_keys = get_uplinkSHM_t_keys(vars[i]);
                key_types = get_uplinkSHM_t_keytypes(temp_keys);
                break;
            case DOWNLINK:
                temp_keys = get_downlinkSHM_t_keys(vars[i]);
                key_types = get_downlinkSHM_t_keytypes(temp_keys);
                break;
            case CFM:
                temp_keys = get_cfm_external_parms_s_keys(vars[i]);
                key_types = get_cfm_external_parms_s_keytypes(temp_keys);
                break;
            case CAC:
                temp_keys = get_cacSHM_t_keys(vars[i]);
                key_types = get_cacSHM_t_keytypes(temp_keys);
                break;
            case STATS:
                temp_keys = get_global_stm_t_keys(vars[i]);
                key_types = get_global_stm_t_keytypes(temp_keys);
                break;
            default:
                break;
        }
    //std::cout << "_get_headers:::::::::::::::::::::::::::::: " << endl;
        //for (std::vector<string>::const_iterator i = temp_keys.begin(); i != temp_keys.end(); ++i)
    //    std::cout << *i << ' ' <<std::endl;
        keys_for_vars.push_back(temp_keys);
        keytype_for_vars.push_back(key_types);

        /*for (std::vector<string>::const_iterator i = key_types.begin(); i != key_types.end(); ++i)
        {
            std::cout <<"key types :"<< *i << ' ' <<std::endl;
        }*/
    }
    
    vector<vector<string> > keys_for_header = keys_for_vars;

    if(leaf)
    {
    //std::cout << "_get_headers key header size: " << keys_for_header.size() << endl;
        for(int i = 0; i < keys_for_header.size(); i++)
        {
            for(int y = 0; y < keys_for_header[i].size(); y++)
            {
        //std::cout << "_get_headers key header: " << keys_for_header[i][y] << endl;
                size_t pos = keys_for_header[i][y].find_last_of('_');
        //std::cout << "_get_headers key header pos: " << pos << endl;
                keys_for_header[i][y] = keys_for_header[i][y].substr(pos+1);
        //std::cout << "_get_headers key header: " << keys_for_header[i][y] << endl;
            }
        }
    }
    
    string headers = "";
    
    if(correlate)
    {
    //std::cout << "_get_headers key header size correlate: " << keys_for_header.size() << endl;
        #if 0
        for(int i = 0; i < keys_for_header.size(); i++)
        {
        //std::cout << "_get_headers key header size i correlate: " << keys_for_header[i].size() << endl;
            for(int y = 0; y < keys_for_header[i].size(); y++)
            {
                headers += keytype_for_vars[i][y] + ":";
                headers += keys_for_header[i][y] + ((i == keys_for_header.size()-1 && y == keys_for_header[i].size()-1) ? "" : ",");
                //std::cout << "_get_headers: " << headers << endl;
                //std::cout << "key type for : "<< keys_for_header[i][y]<<" is : "<<keytype_for_vars[i][y]<<endl;
            }
        }
        #endif
        #if 1
        headers = "{";
        for(int i = 0; i < keys_for_header.size(); i++)
        {
            for(int y = 0; y < keys_for_header[i].size(); y++)
            {
                headers += "\"" + keys_for_header[i][y] + "\""; 
                headers += ":";
                headers += "\"" + keytype_for_vars[i][y] + "\"" + ((i == keys_for_header.size()-1 && y == keys_for_header[i].size()-1) ? "" : ",");
            }
        }
        headers+="}";
        #endif
    }
    else
    {
        for(int i = 0; i < keys_for_header.size(); i++)
        {
            headers += labels[i] + ':';
            for(int y = 0; y < keys_for_header[i].size(); y++)
            {
                headers += keytype_for_vars[i][y] + ":";
                headers += keys_for_header[i][y] + ((y < keys_for_header[i].size()-1) ? "," : "");
            }
        }
    }
    printf("get headers: %s\n", headers.c_str());
    return headers.c_str();
}


/************************************************************************
 *
 * FUNCTION: _parse_conf
 *
 * \brief  Member function of Stats class. 
 *         This method parses configuration received from client  
 *      
 *          
 * \return - -1 if parsing failed , 0 if success
 *
 ************************************************************************/
int Stats::_parse_conf(int idx, string message)
{
    if(message == "")
        return -1;
    
    int correlated = 0;

    string line;
    string delimiter = ":";
    size_t pos = 0;
    
    // Keep printing tokens while one of the 
    // delimiters present in str[]. 
    
    while ( (pos = message.find(delimiter)) != string::npos ) 
    {
        line = message.substr(0, pos);
        //std::cout << "parse config line: " << line << endl;
        int first = line.find('#');
        if(!first)
        {
            message.erase(0, pos + delimiter.length()); 
            continue;
        }

        size_t equal_sign = line.find("=");

        if(equal_sign == string::npos)
        {
            string label, path;
            size_t pos;

            if(!correlated)
            {
                label = line.substr(0, (pos=line.find(",")));
                conn_stats[idx].labels.push_back(label);
                line.erase(0, pos+1);
            }

            path = line.substr(0, (pos=line.find(",")));
            
            short trans = _translate_path(path);
            conn_stats[idx].paths.push_back(trans);

            if(pos == string::npos)
            {
                continue;
            }
            else
            {
                line.erase(0, pos+1);
            }


            vector<string> temp_vars;
            while(!line.empty())
            {
                string var = line.substr(0, (pos=line.find(",")));
                temp_vars.push_back(var);

                if(pos == string::npos)
                {
                    break;
                }
                else
                {
                    line.erase(0, pos+1);
                }
            }
            conn_stats[idx].vars.push_back(temp_vars);
        }
        else
        {
            string key, value;

            key = line.substr(0, equal_sign);
            value = line.substr(equal_sign+1, line.length());

            if(key == "rows")
            {
                conn_stats[idx].rows = value;
                if(value == "correlated")
                    correlated = 1;
                //std::cout<<"correlated!\n";
            }
            else if (key == "style")
            {
                conn_stats[idx].style = value;
                //std::cout <<"style!="<<style<<std::endl;
            }
            else if (key == "time")
            {
                stringstream convert(value);
                convert >> conn_stats[idx].iter_ms;
                //std::cout<<"time="<<value<<std::endl;
            }
        }
        message.erase(0, pos + delimiter.length()); 
    }

    return 0;
}


/************************************************************************
 *
 * FUNCTION: _translate_path
 *
 * \brief  Member function of Stats class. 
 *         This method translate SHM region for the data point  
 *      
 *          
 * \return - enum value for SHM region
 *
 ************************************************************************/
inline short Stats::_translate_path(string path)
{
    if(path == "uplink") 
        return UPLINK;
    else if (path == "downlink")
        return DOWNLINK;
    else if (path == "cfm")
        return CFM;
    else if (path == "cac")
        return CAC;
    else if (path == "stats")
    return STATS;
    else  
        return SKIP;
}
