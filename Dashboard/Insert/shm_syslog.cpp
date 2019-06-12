
#include <uplink_shared.h>      //Uplink SHM
#include <downlink_shared.h>    //Downlink SHM
#include <cfm.h>                //Config SHM, Stats SHM
#include <cacApi.h>             //CAC SHM
#include <shmAmuWacInterface.h>
#include <pep_kmod.h>
#include <ipre.h>
#include <mpe.h>
#include <sys_utils.h>
#include "tempmon.h"
#include "statecode.h"
#include "utilsLib.h"
#include "uldefs.h"
#include "pep_kmod.h"
#include "diag/sys_statecode.h"
#include <gtp_db_apps.h>
#include "ris.h"
#include "mai.h"

#include <ctime>

#include <string>
#include <vector>
//#include <sstream>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
//#include <map>
#include <boost/date_time/posix_time/posix_time.hpp>
//#include <tracelogger.h>

using namespace std;

#include <uplink_shm_syslog.generated>
#include <downlink_shm_syslog.generated>
#include <cfm_shm_syslog.generated>
#include <stats_shm_syslog.generated>
#include <cac_shm_syslog.generated>
//#include <wac_shm_syslog.generated>
//#include <pep_shm_syslog.generated>

struct connection_data {
    int sessionid;
    std::string name;
};

struct custom_config : public websocketpp::config::asio {
    // pull default settings from our core config
    typedef websocketpp::config::asio core;
    
    typedef core::concurrency_type concurrency_type;
    typedef core::request_type request_type;
    typedef core::response_type response_type;
    typedef core::message_type message_type;
    typedef core::con_msg_manager_type con_msg_manager_type;
    typedef core::endpoint_msg_manager_type endpoint_msg_manager_type;
    typedef core::alog_type alog_type;
    typedef core::elog_type elog_type;
    typedef core::rng_type rng_type;
    typedef core::transport_type transport_type;
    typedef core::endpoint_base endpoint_base;
    
    // Set a custom connection_base class
    typedef connection_data connection_base;
};
//typedef websocketpp::server<websocketpp::config::asio> server;
typedef websocketpp::server<custom_config> server;
typedef server::connection_ptr connection_ptr;
typedef server::timer_ptr timer_ptr;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
int num_connection = 0;
typedef server::message_ptr message_ptr;

typedef struct {
    vector<string> labels;
    vector<short> paths;
    vector<vector<string> > vars;
    string rows;
    string style;
    unsigned int iter_ms;
    vector<vector<string> > keys;
    websocketpp::connection_hdl hdl;
}connection_stats_t;
/*
typedef struct {
    connection_stats_t  *conn_stats[10];
}connections_t;
 */
connection_stats_t conn_stats[10];
//connections_t * connections = NULL;
void on_timer(server* s, websocketpp::connection_hdl hdl,websocketpp::lib::error_code const & ec);  

extern void ris_get_all_reset_info (unsigned int *resetCount, unsigned int *lastResetReason, char *lastResetTime,
                                    unsigned int *unx, unsigned int *snmp, unsigned int *lui, unsigned int *swcut,
                                    unsigned int *reinst, unsigned int *watchgod, unsigned int *swfail,
                                    unsigned int *lowmem, unsigned int *uplinkerr, unsigned int *powercycle,
                                    unsigned int *unknwn);

extern void tsk_get_stats(Tsk_stats_t *stats);
void pe_get_bb_switch_stats(bb_switch_coln_stats_t *bb_switch);
void shm_pe_get_pep_stats(pep_stats_displ_t *pep_stats);
static void scStatsCollect(statecode_coln_stats_t * stats);
static void collect_current_statecode(char * str, char * statecodeDurationErrCnt);
int sendWACEssStatsReq();
int fetchWACEssStats(int reqID, wac_coln_stats_t *essStats);

static cfm_external_parms_t *parms =NULL;
static global_stm_t *stats_parms = NULL;

static kernel_stats_t *p_kpep_stats=NULL;
static sm_info_t *sm_info_p = NULL;

cfm_coln_parms_t coln_parms;

statecodeDur_t statecodeDur_arr[_STATE_CODE_MAX_INDEX];

static struct stateCodeMonitor_t * _sysStateCodeMonitor = NULL;

static struct sat_gtp_stats_t gtpstats;
#define BGPD_SUMMARY_STATUS_LEN             50

bool reuse_flag=TRUE;

char * cachingAppState[] = {

    "Pairing Request Wait",
    "Linking Request Wait",
    "Linking Request Missed",
    "Heartbeat Request Wait",
    "Heartbeat Request Missed",
    "Linked With Heartbeat",
    "Unknown"
};


struct data_string {
    char *ptr;
    size_t len;
};


static void scStatsCollect(statecode_coln_stats_t * stats)
{
    /* Clear out previous hour */
    memset(stats,0,sizeof(statecode_coln_stats_t));

    _sysStateCodeMonitor = (struct stateCodeMonitor_t *)&stats_parms->sysStateCode.stateCodeMonitor;
    
    int i;
    for (i=0; i<_STATE_CODE_MAX_INDEX; i++)
    {
        if (_sysStateCodeMonitor->secondsSpent_periodic[i])
        {
            //std::cout<<"Getting sys_statecode";
            struct tm *startTimeStruct;
            struct tm *endTimeStruct;
            unsigned char process, function, info, severity;

            /* get state code */
            STATE_CODE_FIELDS_GET(i, process, function, info, severity);
            sprintf(stats->code[i], "%d.%d.%d", process, function, info);

            if (_sysStateCodeMonitor->startTimestamp_periodic[i])
            {
                startTimeStruct = localtime((time_t *) &_sysStateCodeMonitor->startTimestamp_periodic[i]);
                strftime(stats->startTimestamp[i],BUF_SIZE,"%m/%d/%Y %H:%M",startTimeStruct);
            }

            if (_sysStateCodeMonitor->endTimestamp_periodic[i])
            {
                endTimeStruct = localtime((time_t *) &_sysStateCodeMonitor->endTimestamp_periodic[i]);
                strftime(stats->endTimestamp[i],BUF_SIZE,"%m/%d/%Y %H:%M",endTimeStruct);
            }

            sprintf(stats->secondsSpent[i], "%d", _sysStateCodeMonitor->secondsSpent_periodic[i]);

            /* Clear out local periodic buffers */
            _sysStateCodeMonitor->secondsSpent_periodic[i]   = 0;
            _sysStateCodeMonitor->startTimestamp_periodic[i] = 0;
            _sysStateCodeMonitor->endTimestamp_periodic[i]   = 0;
        }
    }
}


/***********************************************************************
 *
 * FUNCTION NAME: initStatecodeDurArray
 *
 * DESCRIPTION: initialize statecode duration array used when collect statecode in limited stats
 *
 ***********************************************************************/
static void initStatecodeDurArray()
{
    int i;

    memset(statecodeDur_arr, 0, sizeof(statecodeDur_t)*_STATE_CODE_MAX_INDEX);
    for (i = 0; i < _STATE_CODE_MAX_INDEX; i++)
    {
        statecodeDur_arr[i].statecodeIndex = i;
    }
}



/*************************************************************************
FUNCTION NAME   : shm_pe_get_pep_stats

DESCRIPTION     : This funtion is called by CIM to display the PEP statistics
                  on the Console. (From the Main Menu -> (c)->
                  Satellite Interface Stats Menu -> (g) -> Display PEP Statistics.
                  to display the PEP statistics)

PARAMETERS      : pep_stats - pointer to the structure of type pep_stats_displ_t.

RETURNED VALUES : None.

NOTES           : If additional fields are to be displayed, modify
                  pep_stats_displ_t structure definition.

*************************************************************************/
void shm_pe_get_pep_stats(pep_stats_displ_t *pep_stats)
{

    kernel_cfg_t * p_config = NULL;
    kernel_stats_t * p_stats  = NULL;

    byte index;
    ecb_t *ecb_ptr;
    bcb_t *bcb_ptr;

    if (!p_config )
        p_config = (kernel_cfg_t *)memmap_kpep_cfg_mseg();

    if (!p_stats)
        p_stats  = (kernel_stats_t *)memmap_kpep_stats_mseg();

    memset(pep_stats, 0, sizeof(pep_stats_displ_t));

    /* set only the required parameters, if needed other parms can be
       added later */

    /* TCP Spoofing flag */
    pep_stats->tcp_spoofing = p_config->pep_parms.pe_parms.tcp_spoofing_enable;

    /* TCP pkts eligible for spoofing (PE)*/
    pep_stats->rx_tcp[IPV4] = p_stats->mpe_stats.ctr.pep_rx_tcp[IPV4];
    pep_stats->rx_tcp[IPV6] = p_stats->mpe_stats.ctr.pep_rx_tcp[IPV6];

    /* TCP pkts forwarded unspoofed (PE) */
    pep_stats->rx_tcp_unspoofed[IPV4] = p_stats->mpe_stats.ctr.tcp_pkts_unspoofed[IPV4];
    pep_stats->rx_tcp_unspoofed[IPV6] = p_stats->mpe_stats.ctr.tcp_pkts_unspoofed[IPV6];

    /* TCP pkts spoofed by TSK (PE) */
    pep_stats->rx_tcp_spoofed[IPV4] = p_stats->mpe_stats.ctr.tcp_pkts_spoofed[IPV4];
    pep_stats->rx_tcp_spoofed[IPV6] = p_stats->mpe_stats.ctr.tcp_pkts_spoofed[IPV6];

    /* total PBP pkts received (PE) */
    pep_stats->rx_pbp = p_stats->mpe_stats.ctr.rx_pbp;

    /* total PBP pkts with bad checksum received */
    pep_stats->rx_pbp_bad_checksum = p_stats->mpe_stats.ctr.rx_pbp_bad_checksum;

    /* total PBP pkts transmitted (PBPK)*/
    pep_stats->tx_pbp = p_stats->pe_pbpk_stats.ctr.tx_pkt;

    /* total TCP pkts with bad checksum received (PE)*/
    pep_stats->rx_tcp_bad_checksum = p_stats->mpe_stats.ctr.rx_tcp_bad_checksum;

    /* total SYNs TSK decides not to spoof (PE)*/
    pep_stats->tcp_syn_unspoofed[IPV4] = p_stats->mpe_stats.ctr.tcp_syn_unspoofed[IPV4];
    pep_stats->tcp_syn_unspoofed[IPV6] = p_stats->mpe_stats.ctr.tcp_syn_unspoofed[IPV6];

    /* reset reason for last backbone reset (PBPK)*/
    strncpy(pep_stats->last_rst_reason,
            p_stats->pe_pbpk_stats.reset_reason, PBPK_RESET_REASON_SIZE);

    /* stats per BCB/ECB */
    for(index = 0; index < p_config->pep_parms.pe_parms.max_num_bb; index++)
    {

        ecb_ptr = &p_stats->ecb_table[index];

        if ((ecb_ptr != NULL_PTR) && ecb_ptr->bb_connection_state != BACKBONE_STATUS_UNDEFINED)
        {
            bcb_ptr =&p_stats->bcb_table[index];
            if(bcb_ptr != NULL_PTR)
            {
                pep_stats->bb_parms[index].pbp_open_ts = ecb_ptr->bb_open_ts;
                /* get the stats from BCB */
                pep_stats->bb_parms[index].pbp_state = bcb_ptr->state;
                pep_stats->bb_parms[index].tx_pkt = bcb_ptr->stats.ctr.tx_pkt;
                pep_stats->bb_parms[index].rx_pkt = bcb_ptr->stats.ctr.rx_pkt;
                pep_stats->bb_parms[index].tx_bytes = bcb_ptr->stats.ctr.tx_bytes;
                pep_stats->bb_parms[index].rx_bytes = bcb_ptr->stats.ctr.rx_bytes;
                pep_stats->bb_parms[index].tx_sack = bcb_ptr->stats.ctr.tx_sack;
                pep_stats->bb_parms[index].rx_sack = bcb_ptr->stats.ctr.rx_sack;
                pep_stats->bb_parms[index].tx_rst = bcb_ptr->stats.ctr.tx_rst;
                pep_stats->bb_parms[index].rx_rst = bcb_ptr->stats.ctr.rx_rst;
                pep_stats->bb_parms[index].snd_win = bcb_ptr->snd_max;
                pep_stats->bb_parms[index].rcv_win =  bcb_ptr->rcv_max;
                pep_stats->bb_parms[index].zero_rcv_win =  bcb_ptr->stats.ctr.zero_rcv_win;
                memcpy(pep_stats->bb_parms[index].last_rx_op_str,
                       bcb_ptr->bcb_debug.last_rx_status_str,
                       PBPK_STATUS_STR_SIZE);
                memcpy(pep_stats->bb_parms[index].last_tx_op_str,
                       bcb_ptr->bcb_debug.last_tx_status_str,
                       PBPK_STATUS_STR_SIZE);
                pep_stats->bb_parms[index].rpb_poll_tx_sack = bcb_ptr->stats.ctr.rbp_poll_sack_tx;
                pep_stats->bb_parms[index].rpb_poll_rx_sack = bcb_ptr->stats.ctr.rbp_poll_sack_rx;
            }

            /* stats from ECB */
            pep_stats->bb_parms[index].max_w2l = ecb_ptr->w2l_space_limit;
            pep_stats->bb_parms[index].curr_w2l = ecb_ptr->w2l_space_in_use;
            pep_stats->bb_parms[index].curr_l2w = ecb_ptr->l2w_space_in_use;
            pep_stats->bb_parms[index].max_l2w = ecb_ptr->l2w_space_limit;

            /* calculate the CCB limit since it is not stored in ECB */
            pep_stats->bb_parms[index].max_ccb = p_config->pep_parms.pe_parms.cos_param[index].ccb_limit;
            pep_stats->bb_parms[index].curr_ccb[IPV4] = p_stats->tcb_table[index].cur_ccb_count[IPV4];
            pep_stats->bb_parms[index].curr_ccb[IPV6] = p_stats->tcb_table[index].cur_ccb_count[IPV6];
         }
    }

    munmap_kpep_cfg_mseg(p_config);
    munmap_kpep_stats_mseg(p_stats);
}
/*************************************************************************
 * FUNCTION NAME   : pe_get_bb_switch_stats
 *
 * DESCRIPTION     : This funtion is called to get the BB Switch statistics
 *
 * PARAMETERS      : bb_switch - pointer to the structure of type bb_switch_coln_stats_t.
 *
 * RETURNED VALUES : None.
 *
 * *************************************************************************/
void pe_get_bb_switch_stats(bb_switch_coln_stats_t *bb_switch)
{
    
    static kernel_stats_t *p_stats = NULL;
    memset(bb_switch, 0, sizeof(bb_switch_coln_stats_t));
    if (!p_stats)
        p_stats  = (kernel_stats_t *)memmap_kpep_stats_mseg();
    if (!p_stats)
        return;
    bb_switch->wan_bb_move_num=p_stats->bb_switch.wan_bb_move_num;
    bb_switch->wan_bb_byte_tx_before=p_stats->bb_switch.wan_bb_byte_tx_before;
    bb_switch->wan_bb_byte_tx_after=p_stats->bb_switch.wan_bb_byte_tx_after;
    bb_switch->wan_bb_byte_rx_before=p_stats->bb_switch.wan_bb_byte_rx_before;
    bb_switch->wan_bb_byte_rx_after=p_stats->bb_switch.wan_bb_byte_rx_after;
    return;
}

/***********************************************************************
 * Function :    collect_tcp_opt_rule_hits
 *
 * Return Value:  None
 *
 * Description:  Collects the number of hits for each TCP option rule and resets the counters to 0
 *
 * Parm:        TSK stats and PEP collection stats
 **********************************************************************/
static void collect_tcp_opt_rule_hits(Tsk_stats_t *tsk_stats, pep_coln_stats_t *pep_coln_stats)
{
    dword rule_hits_rest = 0;
    int rule_idx;

    for (rule_idx = 14; rule_idx < TSK_MAX_TCP_OPTION_ENTRIES; rule_idx++)
        rule_hits_rest += tsk_stats->num_tcp_opt_rule_hits[rule_idx];
    
    pep_coln_stats->tskTcpOptHitsRule1 = tsk_stats->num_tcp_opt_rule_hits[0];
    pep_coln_stats->tskTcpOptHitsRule2 = tsk_stats->num_tcp_opt_rule_hits[1];
    pep_coln_stats->tskTcpOptHitsRule3 = tsk_stats->num_tcp_opt_rule_hits[2];
    pep_coln_stats->tskTcpOptHitsRule4 = tsk_stats->num_tcp_opt_rule_hits[3];
    pep_coln_stats->tskTcpOptHitsRule5 = tsk_stats->num_tcp_opt_rule_hits[4];
    pep_coln_stats->tskTcpOptHitsRule6 = tsk_stats->num_tcp_opt_rule_hits[5];
    pep_coln_stats->tskTcpOptHitsRule7 = tsk_stats->num_tcp_opt_rule_hits[6];
    pep_coln_stats->tskTcpOptHitsRule8 = tsk_stats->num_tcp_opt_rule_hits[7];
    pep_coln_stats->tskTcpOptHitsRule9 = tsk_stats->num_tcp_opt_rule_hits[8];
    pep_coln_stats->tskTcpOptHitsRule10 = tsk_stats->num_tcp_opt_rule_hits[9];
    pep_coln_stats->tskTcpOptHitsRule11 = tsk_stats->num_tcp_opt_rule_hits[10];
    pep_coln_stats->tskTcpOptHitsRule12 = tsk_stats->num_tcp_opt_rule_hits[11];
    pep_coln_stats->tskTcpOptHitsRule13 = tsk_stats->num_tcp_opt_rule_hits[12];
    pep_coln_stats->tskTcpOptHitsRule14 = tsk_stats->num_tcp_opt_rule_hits[13];    
    pep_coln_stats->tskTcpOptHitsRuleRest = rule_hits_rest; // catch all bucket
}

/**
******************************************************************************************
* \skipline Function: _cmpStartTime
*
* \brief This is a compare function used by collect_current_statecode to perform qsort.
*        It sorts the integer array that contains the interested statecode index.
********************************************************************************************/
static int _cmpStartTime(const void * a, const void * b)
{
    statecodeDur_t * statecodeStrct1 = (statecodeDur_t *) a;
    statecodeDur_t * statecodeStrct2 = (statecodeDur_t *) b;

    if(statecodeStrct1->newStatecodeDur < statecodeStrct2->newStatecodeDur)
        return 1;
    else
        return -1;
}

/***********************************************************************
 * Function :    collect_current_statecode
 *
 * Return Value:  None
 *
 * Description:  collect the current statecode list, sort them in an order that their duration
 *               is from big to small, and pack as many statecode-duration pairs as possible into
 *               char * str, which is a EX_STR_SIZE long buffer
 * Parm:        char * str
 **********************************************************************/
static void collect_current_statecode(char * str, char * statecodeDurationErrCnt)
{
    /*Get state code string*/;
    unsigned char process, function, info, severity;
    int i, errCnt = 0; //errCnt is calculated each time when collecting statecode
    unsigned int duration = 0, max_duration=0;
    char stateCode[EX_STR_SIZE] = {0};
    char evtStr[100] = {0};

    memset(str, 0, EX_STR_SIZE);
    memset(statecodeDurationErrCnt, 0, EX_STR_SIZE);
    
    if (parms->stats_frm_parms.rpt_parms.stat_collection_short_timer)
        max_duration = coln_parms.short_interval_mins * 60;
    else
        max_duration = coln_parms.long_interval_mins * 60;
    
    /*fill up the statcode duration array and sort it with qsort by their newDuration value from big to small*/
    for (i = 0; i < _STATE_CODE_MAX_INDEX; i++)
    {
        duration = stats_parms->sysStateCode.stateCodeMonitor.secondsSpent[statecodeDur_arr[i].statecodeIndex];
        statecodeDur_arr[i].newStatecodeDur = duration - statecodeDur_arr[i].oldStatecodeDur;
        if(statecodeDur_arr[i].newStatecodeDur > max_duration)
        {
            errCnt ++;
            //snprintf(evtStr, sizeof(evtStr), "ERROR statecodeIndex == %d | cur duration == %u | newDuration == %u | oldDuration == %u | errCnt == %d",
            //      i, duration, statecodeDur_arr[i].newStatecodeDur, statecodeDur_arr[i].oldStatecodeDur, errCnt);
            //evtLibPost(EVT_INDEX_TERM_STATS_COLL_ERR, 0, evtStr);
            statecodeDur_arr[i].newStatecodeDur = 0;
            //TRACE_LOG(diagLogId, LOG_ERR, "%s: %s", __FUNCTION__, evtStr);
        }
        statecodeDur_arr[i].oldStatecodeDur = duration;
    }
    /*sort statecode */
    qsort(statecodeDur_arr, _STATE_CODE_MAX_INDEX, sizeof(statecodeDur_t), _cmpStartTime);

    for(i = 0; i < _STATE_CODE_MAX_INDEX; i++)
    {
        /*Only cares for statecode has duration, stop when reach the first 0 duration statecode*/
        if(!statecodeDur_arr[i].newStatecodeDur)
            break;

        memset(stateCode, 0, EX_STR_SIZE);
        STATE_CODE_FIELDS_GET( statecodeDur_arr[i].statecodeIndex, process, function, info, severity);
        //TRACE_LOG(diagLogId, LOG_DEBUG, "%s: i == %d, statecode index == %d, new duration == %u", __FUNCTION__, i, statecodeDur_arr[i].statecodeIndex, statecodeDur_arr[i].newStatecodeDur);
        snprintf(stateCode, EX_STR_SIZE, "%d.%d.%d-%u", process, function, info, statecodeDur_arr[i].newStatecodeDur);
        //TRACE_LOG(diagLogId, LOG_DEBUG, "%s: stateCode == %s", __FUNCTION__, stateCode);
        //TRACE_LOG(diagLogId, LOG_DEBUG, "%s: (strlen(str) + strlen(stateCode)) == %d", __FUNCTION__, (strlen(str) + strlen(stateCode)));
        if((strlen(str) + strlen(stateCode)) <= EX_STR_SIZE)
        {
            strcpy((str + strlen(str)), stateCode);
            if((strlen(str) + 1) < EX_STR_SIZE)
            {
                strcpy((str + strlen(str)), " ");
                //TRACE_LOG(diagLogId, LOG_DEBUG, "%s: str == %s strlen(str) == %d", __FUNCTION__, str, strlen(str));
            }
            else
                break;
        }
        else
            break;
    }
    snprintf(statecodeDurationErrCnt, EX_STR_SIZE, "%d", errCnt); 
    //TRACE_LOG(diagLogId, LOG_INFO, "%s: Exit. stateCodeStr == %s", __FUNCTION__, str);
}

static void get_top_statecode(char * str, int index)
{
    unsigned char process, function, info, severity;
    
    memset(str, 0, EX_STR_SIZE);
	std::cout << "get_top_statecode :" << index << statecodeDur_arr[index].newStatecodeDur << std::endl;
    if(statecodeDur_arr[index].newStatecodeDur)
    {
        STATE_CODE_FIELDS_GET( statecodeDur_arr[index].statecodeIndex, process, function, info, severity);
        snprintf(str, EX_STR_SIZE, "%d.%d.%d", process, function, info);
    }
    else if (index == 0)
    {
            snprintf(str, EX_STR_SIZE, "%d.%d.%d", 0,0,0);
    }
    
}

static void get_top_statecode_duration(char * str, int index)
{
    memset(str, 0, EX_STR_SIZE);
    if(!statecodeDur_arr[index].newStatecodeDur && index == 0)
    {
        if (parms->stats_frm_parms.rpt_parms.stat_collection_short_timer)
            snprintf(str, EX_STR_SIZE, "%u", coln_parms.short_interval_mins * 60);
        else
            snprintf(str, EX_STR_SIZE, "%u", coln_parms.long_interval_mins * 60);
    }
    else if (statecodeDur_arr[index].newStatecodeDur)
        snprintf(str, EX_STR_SIZE, "%u", statecodeDur_arr[index].newStatecodeDur);
}


/***********************************************************************
 * Function :    sendWACEssStatsReq
 *
 * Return Value:
 *
 * Description:
***********************************************************************/
int sendWACEssStatsReq()
{
    mqd_t mqFd;
    essential_stats_header_s wacMsg;

    mqFd = mq_open(WAC_ESTAT_MESSAGE_QUEUE_NAME, O_RDWR | O_NONBLOCK);

    if (mqFd == (mqd_t) - 1)
    {
        std::cout << "open WAC queue failed :" <<WAC_ESTAT_MESSAGE_QUEUE_NAME << std::endl;
        //TRACE_LOG(diagLogId, LOG_ERR, "open WAC queue %s failed", WAC_ESTAT_MESSAGE_QUEUE_NAME);
        return ERROR;
    }

    /* initialize random seed: */
    srand ( time(NULL) );

    wacMsg.requestID = (rand() % MAX_REQ_ID) + 1;

    if (mq_send(mqFd, (char *)&wacMsg, sizeof(essential_stats_header_s), 0) != 0)
        std::cout << "No more space left on WAC queue :" <<WAC_ESTAT_MESSAGE_QUEUE_NAME << std::endl;

    mq_close(mqFd);
    return (wacMsg.requestID);
}



/***********************************************************************
 * Function :    fetchWACEssStats
 *
 * Return Value:
 *
 * Description:   Gathers   stats.
***********************************************************************/
int fetchWACEssStats(int reqID, wac_coln_stats_t *colnStats)
{
  char i=0;
  int shmFd = -1;
  size_t shmSize = sizeof(shmProviderData_t);
  bool wacProviderShmMmapped = false;
  shmProviderData_t *shmProviderData;

  // static counter to keep track of WAC crash count during last polling period
  static int wac_previous_crash_count = 0;

  /* Zero out our target data so we don't possibly report bad values */
  memset(colnStats, 0, sizeof(wac_coln_stats_t));

  // open WAC provider shared memory with read only permissions
  shmFd = shm_open(WAC_PROVIDER_SHM_NAME, O_RDONLY, 0666);
  if(shmFd != -1)
  {
      // map to VM
      void *addr = mmap(NULL, shmSize, PROT_READ, MAP_SHARED, shmFd, 0);
      if(MAP_FAILED != addr)
      {
          // map shared memory to a stats struct
          shmProviderData = (shmProviderData_t *)addr;
          wacProviderShmMmapped = true;
      }
      else
      {
          std::cout << "WAC provider shm mmap error :" <<strerror(errno) << std::endl;
          return ERROR;
      }
  }
  else
  {
      std::cout << "WAC provider shm open error :" <<strerror(errno) << std::endl;
      return ERROR;
  }

  /* Try to see if WAC has placed stats in the shm */
if (shmProviderData->estats.header.requestID == reqID)
{
    memcpy(colnStats, &(shmProviderData->estats.data), sizeof(wac_coln_stats_t));
}
else 
    std::cout << "WAC providerdata shm req ID is not the same" << std::endl;

  if (wacProviderShmMmapped)
  {
    munmap(shmProviderData, sizeof(shmProviderData_t));
    // do not need shmFd any more, can close it
    close(shmFd);
  }

  // fetch the wac crash count from stats_parms
  // The stats_parms pointer points to the global stats shared memory. The wac crash count is getting
  // fetch from amu process stats. Although this stat is coming from a different source, it has been
  // made a part of the wac essential stat.

  // With every reporting interval, we would only report the number of times the WAC process has
  // crashed since last polling.

  int wac_current_crash_count = stats_parms->amu_proc_stats[WEB15_APPL].numRestarts;
  int wac_delta_crash_count = wac_current_crash_count - wac_previous_crash_count;

  // update the static variable

  wac_previous_crash_count = wac_current_crash_count;

  // report the stat in the data structure

  colnStats->wacStWacCrashCount = (unsigned int)(wac_delta_crash_count > 0 ? wac_delta_crash_count : 0);

  return OK;
}


/* Collect All current stats that are not accessible via API
 *
 * */
static void collect_current_stats(int transportColFlag)
{
    //coln_stats_t *p_curr=(coln_stats_t*)malloc(sizeof(coln_stats_t));
    coln_stats_t p_curr;
    pep_stats_displ_t pep_stats;
    dev_info_t dev_stats_lan1;
    dev_info_t dev_stats_wan;
    assocStatus_t    *pAssocStatus = &stats_parms->assocStatus;
    assocIPGWPool_t  *pPool = &pAssocStatus->ipgwPool;
    struct tm  *ts;
    int reqID=0;
    temp_mon_t tempMonBuf;
    int i, len, bgpColumn= 0;
    Tcb_t *p_tcb;
    char *savePtr, *token, *str2;
    char tmpStr[400],tmpStr1[100];
    char buf[6000],command[200];
    char floatingAddressSubnet[30];
    char vrState[20];
    char ipaddrstr[INET_ADDRSTRLEN];

    
    char timeStr[sizeof(p_curr.mgmt.cachingAppLastMsgRecvTime)] = {0};
#ifdef THEBE_PLATFORM    
    GET_LOCAL_TIME3(timeStr, &(stats_parms->mai_stats.lastMsgRcvTime));
#endif

    if (!p_kpep_stats)
        p_kpep_stats  = (kernel_stats_t *)memmap_kpep_stats_mseg();
    if(!sm_info_p)
        sm_info_p = (sm_info_t*)memmap_sm_info_mseg();
    

    memcpy(&coln_parms, &(parms->stats_frm_parms.coln_parms), sizeof(cfm_coln_parms_t));
    
    /* get system reboot info */
    ris_get_all_reset_info(&p_curr.mgmt.genStRebootCount, &p_curr.mgmt.genStLastRebootReason,p_curr.mgmt.genStLastRebootTime,
                      &p_curr.mgmt.rbtStUnexpectedReboots,&p_curr.mgmt.rbtStSNMPReboots, &p_curr.mgmt.rbtStLUIReboots,
                      &p_curr.mgmt.rbtStSWCutReboots, &p_curr.mgmt.rbtStReinstReboots, &p_curr.mgmt.rbtStWatchDogReboots,
                      &p_curr.mgmt.rbtStSWFailReboots, &p_curr.mgmt.rbtStLowMemReboots, &p_curr.mgmt.rbtStUplinkErrReboots,
                      &p_curr.mgmt.rbtStPowerCycleReboots,&p_curr.mgmt.rbtStUnknwnReboots);
    
    /* Send msg to WAC server to retrieve WAC stats */
    if(transportColFlag)
    {
        reqID = sendWACEssStatsReq();
    }

    shm_pe_get_pep_stats(&pep_stats);
    

    if(!(parms->pep_parms.pe_parms.tcp_spoofing_enable)) {
        //std::cout<<"TCP spoofing not enabled"<<std::endl;
    }
    else
    {
        pe_get_bb_switch_stats(&p_curr.bb_switch); // Get BB switch stats.
        
        p_curr.pep.pbpStBBNum = parms->pep_parms.pe_parms.max_num_bb;
        p_curr.pep.pbpStCOS1PktsTx = pep_stats.bb_parms[0].tx_pkt;
        p_curr.pep.pbpStCOS1PktsRx = pep_stats.bb_parms[0].rx_pkt;
        p_curr.pep.pbpStCOS1SACKsTx = pep_stats.bb_parms[0].tx_sack;
        p_curr.pep.pbpStCOS1SACKsRx = pep_stats.bb_parms[0].rx_sack;

        p_curr.pep.pbpStCOS2PktsTx = pep_stats.bb_parms[1].tx_pkt;
        p_curr.pep.pbpStCOS2PktsRx = pep_stats.bb_parms[1].rx_pkt;
        p_curr.pep.pbpStCOS2SACKsTx = pep_stats.bb_parms[1].tx_sack;
        p_curr.pep.pbpStCOS2SACKsRx = pep_stats.bb_parms[1].rx_sack;

        p_curr.pep.pbpStCOS3PktsTx = pep_stats.bb_parms[2].tx_pkt;
        p_curr.pep.pbpStCOS3PktsRx = pep_stats.bb_parms[2].rx_pkt;
        p_curr.pep.pbpStCOS3SACKsTx = pep_stats.bb_parms[2].tx_sack;
        p_curr.pep.pbpStCOS3SACKsRx = pep_stats.bb_parms[2].rx_sack;

        p_curr.pep.pbpStCOS4PktsTx = pep_stats.bb_parms[3].tx_pkt;
        p_curr.pep.pbpStCOS4PktsRx = pep_stats.bb_parms[3].rx_pkt;
        p_curr.pep.pbpStCOS4SACKsTx = pep_stats.bb_parms[3].tx_sack;
        p_curr.pep.pbpStCOS4SACKsRx = pep_stats.bb_parms[3].rx_sack;

        p_curr.pep.pbpStCOS5PktsTx = pep_stats.bb_parms[4].tx_pkt;
        p_curr.pep.pbpStCOS5PktsRx = pep_stats.bb_parms[4].rx_pkt;
        p_curr.pep.pbpStCOS5SACKsTx = pep_stats.bb_parms[4].tx_sack;
        p_curr.pep.pbpStCOS5SACKsRx = pep_stats.bb_parms[4].rx_sack;

        p_curr.pep.pbpStCOS6PktsTx = pep_stats.bb_parms[5].tx_pkt;
        p_curr.pep.pbpStCOS6PktsRx = pep_stats.bb_parms[5].rx_pkt;
        p_curr.pep.pbpStCOS6SACKsTx = pep_stats.bb_parms[5].tx_sack;
        p_curr.pep.pbpStCOS6SACKsRx = pep_stats.bb_parms[5].rx_sack;

        p_curr.pep.pbpStCOS7PktsTx = pep_stats.bb_parms[6].tx_pkt;
        p_curr.pep.pbpStCOS7PktsRx = pep_stats.bb_parms[6].rx_pkt;
        p_curr.pep.pbpStCOS7SACKsTx = pep_stats.bb_parms[6].tx_sack;
        p_curr.pep.pbpStCOS7SACKsRx = pep_stats.bb_parms[6].rx_sack;

        p_curr.pep.pbpStCOS8PktsTx = pep_stats.bb_parms[7].tx_pkt;
        p_curr.pep.pbpStCOS8PktsRx = pep_stats.bb_parms[7].rx_pkt;
        p_curr.pep.pbpStCOS8SACKsTx = pep_stats.bb_parms[7].tx_sack;
        p_curr.pep.pbpStCOS8SACKsRx = pep_stats.bb_parms[7].rx_sack;

        p_curr.pep.pbpStCOS1RetranTx = p_kpep_stats->ecb_table[0].retry_count;
        p_curr.pep.pbpStCOS1RTT = p_kpep_stats->bcb_table[0].last_pkt_rtt;
        p_curr.pep.pbpStCOS2RetranTx = p_kpep_stats->ecb_table[1].retry_count;
        p_curr.pep.pbpStCOS2RTT = p_kpep_stats->bcb_table[1].last_pkt_rtt;
        p_curr.pep.pbpStCOS3RetranTx = p_kpep_stats->ecb_table[2].retry_count;
        p_curr.pep.pbpStCOS3RTT = p_kpep_stats->bcb_table[2].last_pkt_rtt;
        p_curr.pep.pbpStCOS4RetranTx = p_kpep_stats->ecb_table[3].retry_count;
        p_curr.pep.pbpStCOS4RTT = p_kpep_stats->bcb_table[3].last_pkt_rtt;
        p_curr.pep.pbpStCOS5RetranTx = p_kpep_stats->ecb_table[4].retry_count;
        p_curr.pep.pbpStCOS5RTT = p_kpep_stats->bcb_table[4].last_pkt_rtt;
        p_curr.pep.pbpStCOS6RetranTx = p_kpep_stats->ecb_table[5].retry_count;
        p_curr.pep.pbpStCOS6RTT = p_kpep_stats->bcb_table[5].last_pkt_rtt;
        p_curr.pep.pbpStCOS7RetranTx = p_kpep_stats->ecb_table[6].retry_count;
        p_curr.pep.pbpStCOS7RTT = p_kpep_stats->bcb_table[6].last_pkt_rtt;
        p_curr.pep.pbpStCOS8RetranTx = p_kpep_stats->ecb_table[7].retry_count;
        p_curr.pep.pbpStCOS8RTT = p_kpep_stats->bcb_table[7].last_pkt_rtt;
        
        p_curr.pep.tskTcpOptNumBlocked = p_kpep_stats->pe_tsk_stats.tcp_conn_stats.conn_blocked;
        p_curr.pep.tskTcpOptNumUnspoofed = p_kpep_stats->pe_tsk_stats.tcp_conn_stats.conn_unspoofed;
        p_curr.pep.tskTcpOptNum3WHSDsbld = p_kpep_stats->pe_tsk_stats.tcp_conn_stats.twhspoof_disabled;
        p_curr.pep.tskTcpOptNumSupressed = p_kpep_stats->pe_tsk_stats.tcp_conn_stats.option_supressed;
        
        collect_tcp_opt_rule_hits(&p_kpep_stats->pe_tsk_stats, &p_curr.pep);

        /* Smart Spoofing Stats. */
        
        if(transportColFlag)
        {
            p_curr.pep.ccb_terminal_avg = (sm_info_p->sm_stats.total_sample_l)?sm_info_p->sm_stats.ccb_terminal_l/sm_info_p->sm_stats.total_sample_l:0;
            if(sm_info_p->sm_stats.ccb_terminal_min_l == (unsigned int)-1)
                p_curr.pep.ccb_terminal_min = 0;
            else
                p_curr.pep.ccb_terminal_min = sm_info_p->sm_stats.ccb_terminal_min_l;
            p_curr.pep.ccb_terminal_max = sm_info_p->sm_stats.ccb_terminal_max_l;
            p_curr.pep.ccb_ipgw_avg = (sm_info_p->sm_stats.total_sample_l)?sm_info_p->sm_stats.ccb_ipgw_l/sm_info_p->sm_stats.total_sample_l:0;
            if(sm_info_p->sm_stats.ccb_ipgw_min_l == (unsigned int)-1)
                p_curr.pep.ccb_ipgw_min = 0;
            else
                p_curr.pep.ccb_ipgw_min = sm_info_p->sm_stats.ccb_ipgw_min_l;
            p_curr.pep.ccb_ipgw_max = sm_info_p->sm_stats.ccb_ipgw_max_l;
            p_curr.pep.ccb_overall_avg = (sm_info_p->sm_stats.total_sample_l)?sm_info_p->sm_stats.ccb_overall_l/sm_info_p->sm_stats.total_sample_l:0;
            if(sm_info_p->sm_stats.ccb_overall_min_l == (unsigned int)-1)
                p_curr.pep.ccb_overall_min = 0;
            else
                p_curr.pep.ccb_overall_min = sm_info_p->sm_stats.ccb_overall_min_l;
            p_curr.pep.ccb_overall_max = sm_info_p->sm_stats.ccb_overall_max_l;
            p_curr.pep.remote_host_cnt_avg = (sm_info_p->sm_stats.total_sample_l)?sm_info_p->sm_stats.s_remote_host_cnt_l/sm_info_p->sm_stats.total_sample_l:0;
            if(sm_info_p->sm_stats.s_remote_host_cnt_min_l == (unsigned int)-1)
                p_curr.pep.remote_host_cnt_min = 0;
            else
                p_curr.pep.remote_host_cnt_min = sm_info_p->sm_stats.s_remote_host_cnt_min_l;
            p_curr.pep.remote_host_cnt_max = sm_info_p->sm_stats.s_remote_host_cnt_max_l;
            p_curr.pep.local_host_cnt_avg = (sm_info_p->sm_stats.total_sample_l)?sm_info_p->sm_stats.s_local_host_cnt_l/sm_info_p->sm_stats.total_sample_l:0;
            if(sm_info_p->sm_stats.s_local_host_cnt_min_l == (unsigned int)-1)
                p_curr.pep.local_host_cnt_min = 0;
            else
                p_curr.pep.local_host_cnt_min = sm_info_p->sm_stats.s_local_host_cnt_min_l;
            p_curr.pep.local_host_cnt_max = sm_info_p->sm_stats.s_local_host_cnt_max_l;
            p_curr.pep.remote_bucket_cnt_avg = (sm_info_p->sm_stats.total_sample_l)?sm_info_p->sm_stats.s_remote_bucket_cnt_l/sm_info_p->sm_stats.total_sample_l:0;
            if(sm_info_p->sm_stats.s_remote_bucket_cnt_min_l == (unsigned int)-1)
                p_curr.pep.remote_bucket_cnt_min = 0;
            else
                p_curr.pep.remote_bucket_cnt_min = sm_info_p->sm_stats.s_remote_bucket_cnt_min_l;
            p_curr.pep.remote_bucket_cnt_max = sm_info_p->sm_stats.s_remote_bucket_cnt_max_l;
            p_curr.pep.local_bucket_cnt_avg = (sm_info_p->sm_stats.total_sample_l)?sm_info_p->sm_stats.s_local_bucket_cnt_l/sm_info_p->sm_stats.total_sample_l:0;
            if(sm_info_p->sm_stats.s_local_bucket_cnt_min_l == (unsigned int)-1)
                p_curr.pep.local_bucket_cnt_min = 0;
            else
                p_curr.pep.local_bucket_cnt_min = sm_info_p->sm_stats.s_local_bucket_cnt_min_l;
            p_curr.pep.local_bucket_cnt_max = sm_info_p->sm_stats.s_local_bucket_cnt_max_l;
            //MrIpcMsg_t ipcMsg ;
            //ipcMsg.ipcHdr.dstId   = TMU_APPL_ID;
            //ipcMsg.ipcHdr.version = 1;
            //ipcMsg.ipcHdr.hdrLen  = sizeof(MrIpcHdr_t);
            //ipcMsg.ipcHdr.ifId    = IF_TYPE_INTERNAL;
            //ipcMsg.ipcHdr.msgType = DIAG_CLEAR_SM_L_STATS_MSGID;
            //ipcMsg.ipcHdr.dataLen = 0;
            //ipcMsg.ipcHdr.srcId   = 0;
            //if (mrSend(&ipcMsg) != 0)
            //{
            //    TRACE_LOG(diagLogId, LOG_ERR,"Unable to send Smart Spoofing long interval stats clear msg to TMU\n");
            //}
        }
        else
        {
            p_curr.pep.ccb_terminal_avg = (sm_info_p->sm_stats.total_sample)?sm_info_p->sm_stats.ccb_terminal/sm_info_p->sm_stats.total_sample:0;
            if(sm_info_p->sm_stats.ccb_terminal_min == (unsigned int)-1)
                p_curr.pep.ccb_terminal_min = 0;
            else
                p_curr.pep.ccb_terminal_min = sm_info_p->sm_stats.ccb_terminal_min;
            p_curr.pep.ccb_terminal_max = sm_info_p->sm_stats.ccb_terminal_max;
            p_curr.pep.ccb_ipgw_avg = (sm_info_p->sm_stats.total_sample)?sm_info_p->sm_stats.ccb_ipgw/sm_info_p->sm_stats.total_sample:0;
            if(sm_info_p->sm_stats.ccb_ipgw_min == (unsigned int)-1)
                p_curr.pep.ccb_ipgw_min = 0;
            else
                p_curr.pep.ccb_ipgw_min = sm_info_p->sm_stats.ccb_ipgw_min;
            p_curr.pep.ccb_ipgw_max = sm_info_p->sm_stats.ccb_ipgw_max;
            p_curr.pep.ccb_overall_avg = (sm_info_p->sm_stats.total_sample)?sm_info_p->sm_stats.ccb_overall/sm_info_p->sm_stats.total_sample:0;
            if(sm_info_p->sm_stats.ccb_overall_min == (unsigned int)-1)
                p_curr.pep.ccb_overall_min = 0;
            else
                p_curr.pep.ccb_overall_min = sm_info_p->sm_stats.ccb_overall_min;
            p_curr.pep.ccb_overall_max = sm_info_p->sm_stats.ccb_overall_max;
            p_curr.pep.remote_host_cnt_avg = (sm_info_p->sm_stats.total_sample)?sm_info_p->sm_stats.s_remote_host_cnt/sm_info_p->sm_stats.total_sample:0;
            if(sm_info_p->sm_stats.s_remote_host_cnt_min == (unsigned int)-1)
                p_curr.pep.remote_host_cnt_min = 0;
            else
                p_curr.pep.remote_host_cnt_min = sm_info_p->sm_stats.s_remote_host_cnt_min;
            p_curr.pep.remote_host_cnt_max = sm_info_p->sm_stats.s_remote_host_cnt_max;
            p_curr.pep.local_host_cnt_avg = (sm_info_p->sm_stats.total_sample)?sm_info_p->sm_stats.s_local_host_cnt/sm_info_p->sm_stats.total_sample:0;
            if(sm_info_p->sm_stats.s_local_host_cnt_min == (unsigned int)-1)
                p_curr.pep.local_host_cnt_min = 0;
            else
                p_curr.pep.local_host_cnt_min = sm_info_p->sm_stats.s_local_host_cnt_min;
            p_curr.pep.local_host_cnt_max = sm_info_p->sm_stats.s_local_host_cnt_max;
            p_curr.pep.remote_bucket_cnt_avg = (sm_info_p->sm_stats.total_sample)?sm_info_p->sm_stats.s_remote_bucket_cnt/sm_info_p->sm_stats.total_sample:0;
            if(sm_info_p->sm_stats.s_remote_bucket_cnt_min == (unsigned int)-1)
                p_curr.pep.remote_bucket_cnt_min = 0;
            else
                p_curr.pep.remote_bucket_cnt_min = sm_info_p->sm_stats.s_remote_bucket_cnt_min;
            p_curr.pep.remote_bucket_cnt_max = sm_info_p->sm_stats.s_remote_bucket_cnt_max;
            p_curr.pep.local_bucket_cnt_avg = (sm_info_p->sm_stats.total_sample)?sm_info_p->sm_stats.s_local_bucket_cnt/sm_info_p->sm_stats.total_sample:0;
            if(sm_info_p->sm_stats.s_local_bucket_cnt_min == (unsigned int)-1)
                p_curr.pep.local_bucket_cnt_min = 0;
            else
                p_curr.pep.local_bucket_cnt_min = sm_info_p->sm_stats.s_local_bucket_cnt_min;
            p_curr.pep.local_bucket_cnt_max = sm_info_p->sm_stats.s_local_bucket_cnt_max;
            //MrIpcMsg_t ipcMsg ;
            //ipcMsg.ipcHdr.dstId   = TMU_APPL_ID;
            //ipcMsg.ipcHdr.version = 1;
            //ipcMsg.ipcHdr.hdrLen  = sizeof(MrIpcHdr_t);
            //ipcMsg.ipcHdr.ifId    = IF_TYPE_INTERNAL;
            //ipcMsg.ipcHdr.msgType = DIAG_CLEAR_SM_S_STATS_MSGID;
            //ipcMsg.ipcHdr.dataLen = 0;
            //ipcMsg.ipcHdr.srcId   = 0;
            //if (mrSend(&ipcMsg) != 0)
            //{
            //    TRACE_LOG(diagLogId, LOG_ERR,"Unable to send Smart Spoofing short interval stats clear msg to TMU\n");
            //}
        }
    }
    if ( get_dev_stats("eth0", &dev_stats_lan1) != ERROR)
    {
        p_curr.intf.intfStLan1FramesTx = dev_stats_lan1.tx_pkt;
        p_curr.intf.intfStLan1BytesTx  = dev_stats_lan1.tx_bytes;
        p_curr.intf.intfStLan1ErrorTx  = dev_stats_lan1.tx_error;
        p_curr.intf.intfStLan1FramesRx = dev_stats_lan1.rx_pkt;
        p_curr.intf.intfStLan1ErrorRx = dev_stats_lan1.rx_error;
        p_curr.intf.intfStLan1BytesRx = dev_stats_lan1.rx_bytes;
    }

    if ( (parms->terr_parms.terr_lan_inroute) || (parms->terr_parms.terr_lan_outroute) )
    {
        if ( get_dev_stats("eth1", &dev_stats_wan) != ERROR)
        {
            p_curr.intf.intfStWanFramesTx = dev_stats_wan.tx_pkt;
            p_curr.intf.intfStWanBytesTx  = dev_stats_wan.tx_bytes;
            p_curr.intf.intfStWanErrorTx  = dev_stats_wan.tx_error;
            p_curr.intf.intfStWanFramesRx = dev_stats_wan.rx_pkt;
            p_curr.intf.intfStWanErrorRx  = dev_stats_wan.rx_error;
            p_curr.intf.intfStWanBytesRx  = dev_stats_wan.rx_bytes;
        }
    }
    else
    {
        if ( get_dev_stats("sat", &dev_stats_wan) != ERROR)
        {
            p_curr.intf.intfStWanFramesTx = dev_stats_wan.tx_pkt;
            p_curr.intf.intfStWanBytesTx  = dev_stats_wan.tx_bytes;
            p_curr.intf.intfStWanErrorTx  = dev_stats_wan.tx_error;
            p_curr.intf.intfStWanFramesRx = dev_stats_wan.rx_pkt;
            p_curr.intf.intfStWanErrorRx  = dev_stats_wan.rx_error;
            p_curr.intf.intfStWanBytesRx  = dev_stats_wan.rx_bytes;
        }
    }

    /* LanTCP stats should be non-accumulated. */
    p_curr.intf.intfStLan1TCPBytesRx = p_kpep_stats->pe_tsk_stats.num_tcp_bytes_rxed[IPV4] +
                                        p_kpep_stats->pe_tsk_stats.num_tcp_bytes_rxed[IPV6];
    p_curr.intf.intfStLan1TCPBytesTx = p_kpep_stats->pe_tsk_stats.num_tcp_bytes_txed[IPV4] +
                                        p_kpep_stats->pe_tsk_stats.num_tcp_bytes_txed[IPV6];
    p_curr.intf.intfStLan1TCPSACKsRx = p_kpep_stats->pe_tsk_stats.num_sack_rxed[IPV4] +
                                        p_kpep_stats->pe_tsk_stats.num_sack_rxed[IPV6];
    p_curr.intf.intfStLan1TCPSACKsTx = p_kpep_stats->pe_tsk_stats.num_sack_txed[IPV4] +
                                        p_kpep_stats->pe_tsk_stats.num_sack_txed[IPV6];
    p_curr.intf.intfStLan1TCPBytesRetran = p_kpep_stats->pe_tsk_stats.num_tcp_bytes_retxed[IPV4] +
                                            p_kpep_stats->pe_tsk_stats.num_tcp_bytes_retxed[IPV6];

    if (pPool->lastKnownAssocTime != 0)
    {
        ts = localtime(&pPool->lastKnownAssocTime);
        strftime(p_curr.assoc.last_assoc_timestamp, sizeof(p_curr.assoc.last_assoc_timestamp), "%m/%d/%Y %H:%M:%S", ts);
        p_curr.assoc.last_assoc_timestamp[BUF_SIZE-1]=0;   // remove \n
    }

    if (strlen(pPool->lastKnownIPGWId))
    {
        memset(p_curr.assoc.last_assoc_ipgw, '\0', BUF_SIZE);
        memcpy(p_curr.assoc.last_assoc_ipgw, pPool->lastKnownIPGWId, MAX_IPGW_ID_LEN-1);
    }
    p_curr.assoc.ipgw_reassoc_count = stats_parms->assocStats.assocRespCtn;
    p_curr.assoc.cro_move_ni = stats_parms->outroute_move_stats.cro_move_ni;
    p_curr.assoc.cro_move_na = stats_parms->outroute_move_stats.cro_move_na;
    p_curr.assoc.cro_move_nd = stats_parms->outroute_move_stats.cro_move_nd;
    p_curr.assoc.cro_move_total = stats_parms->outroute_move_stats.cro_move_total;
    p_curr.assoc.ipgw_move_na = stats_parms->outroute_move_stats.ipgw_move_na;
    p_curr.assoc.cro_move = stats_parms->outroute_move_stats.cro_move;
    p_curr.assoc.ipgw_move = stats_parms->outroute_move_stats.ipgw_move;
    p_curr.assoc.cro_ipgw_move = stats_parms->outroute_move_stats.cro_ipgw_move;
    p_curr.assoc.outroute_move_ni = stats_parms->outroute_move_stats.outroute_move_ni;
    p_curr.assoc.outroute_move_na = stats_parms->outroute_move_stats.outroute_move_na;
    p_curr.assoc.outroute_move_nd = stats_parms->outroute_move_stats.outroute_move_nd;
    p_curr.assoc.outroute_move_ni_fail = stats_parms->outroute_move_stats.outroute_move_ni_fail;
    p_curr.assoc.outroute_move_na_fail = stats_parms->outroute_move_stats.outroute_move_na_fail;
    p_curr.assoc.outroute_move_nd_fail = stats_parms->outroute_move_stats.outroute_move_nd_fail;
    p_curr.assoc.outroute_move_ni_fail_omi_nack = stats_parms->outroute_move_stats.outroute_move_ni_fail_omi_nack;
    p_curr.assoc.outroute_move_ni_fail_omc_nack = stats_parms->outroute_move_stats.outroute_move_ni_fail_omc_nack;
    p_curr.assoc.outroute_move_ni_fail_voip = stats_parms->outroute_move_stats.outroute_move_ni_fail_voip;
    p_curr.assoc.outroute_move_ni_fail_mgmt = stats_parms->outroute_move_stats.outroute_move_ni_fail_mgmt;
    p_curr.assoc.outroute_move_ni_fail_usr = stats_parms->outroute_move_stats.outroute_move_ni_fail_usr;
    p_curr.assoc.outroute_move_ni_fail_max_omi = stats_parms->outroute_move_stats.outroute_move_ni_fail_max_omi;
    p_curr.assoc.outroute_move_ni_fail_max_omc = stats_parms->outroute_move_stats.outroute_move_ni_fail_max_omc;
    p_curr.assoc.outroute_move_ni_fail_timer_expired = stats_parms->outroute_move_stats.outroute_move_ni_fail_timer_expired;
    p_curr.assoc.outroute_move_ni_fail_protocol_error = stats_parms->outroute_move_stats.outroute_move_ni_fail_protocol_error;
    p_curr.assoc.outroute_move_ni_fail_wan_error = stats_parms->outroute_move_stats.outroute_move_ni_fail_wan_error;
    p_curr.assoc.outroute_move_na_fail_omi_nack = stats_parms->outroute_move_stats.outroute_move_na_fail_omi_nack;
    p_curr.assoc.outroute_move_na_fail_omc_nack = stats_parms->outroute_move_stats.outroute_move_na_fail_omc_nack;
    p_curr.assoc.outroute_move_na_fail_voip = stats_parms->outroute_move_stats.outroute_move_na_fail_voip;
    p_curr.assoc.outroute_move_na_fail_mgmt = stats_parms->outroute_move_stats.outroute_move_na_fail_mgmt;
    p_curr.assoc.outroute_move_na_fail_usr = stats_parms->outroute_move_stats.outroute_move_na_fail_usr;
    p_curr.assoc.outroute_move_na_fail_max_omi = stats_parms->outroute_move_stats.outroute_move_na_fail_max_omi;
    p_curr.assoc.outroute_move_na_fail_max_omc = stats_parms->outroute_move_stats.outroute_move_na_fail_max_omc;
    p_curr.assoc.outroute_move_na_fail_timer_expired = stats_parms->outroute_move_stats.outroute_move_na_fail_timer_expired;
    p_curr.assoc.outroute_move_na_fail_protocol_error = stats_parms->outroute_move_stats.outroute_move_na_fail_protocol_error;
    p_curr.assoc.outroute_move_na_fail_wan_error = stats_parms->outroute_move_stats.outroute_move_na_fail_wan_error;
    p_curr.assoc.outroute_move_nd_fail_omi_nack = stats_parms->outroute_move_stats.outroute_move_nd_fail_omi_nack;
    p_curr.assoc.outroute_move_nd_fail_omc_nack = stats_parms->outroute_move_stats.outroute_move_nd_fail_omc_nack;
    p_curr.assoc.outroute_move_nd_fail_voip = stats_parms->outroute_move_stats.outroute_move_nd_fail_voip;
    p_curr.assoc.outroute_move_nd_fail_mgmt = stats_parms->outroute_move_stats.outroute_move_nd_fail_mgmt;
    p_curr.assoc.outroute_move_nd_fail_usr = stats_parms->outroute_move_stats.outroute_move_nd_fail_usr;
    p_curr.assoc.outroute_move_nd_fail_max_omi = stats_parms->outroute_move_stats.outroute_move_nd_fail_max_omi;
    p_curr.assoc.outroute_move_nd_fail_max_omc = stats_parms->outroute_move_stats.outroute_move_nd_fail_max_omc;
    p_curr.assoc.outroute_move_nd_fail_timer_expired = stats_parms->outroute_move_stats.outroute_move_nd_fail_timer_expired;
    p_curr.assoc.outroute_move_nd_fail_protocol_error = stats_parms->outroute_move_stats.outroute_move_nd_fail_protocol_error;
    p_curr.assoc.outroute_move_nd_fail_wan_error = stats_parms->outroute_move_stats.outroute_move_nd_fail_wan_error;
    p_curr.assoc.omt_expired = stats_parms->outroute_move_stats.omt_expired;

    /* collect these only if the flag is on */
    if(transportColFlag)
    {
        fetchWACEssStats(reqID, &p_curr.wac);
        getSysGenStats(&p_curr.mgmt, &coln_parms, DO_NOT_GET_RESET_INFO);
#if !defined(IMX_PLATFORM) && !defined(_THEBE_GEN_0)
        dlLimitStatsCollect(&p_curr.outroute);
        ulLimitedStatsCollect(&p_curr.inroute);
        tempMonStatsGet(&tempMonBuf);
#endif

        p_curr.mgmt.genStMaxTemp = tempMonBuf.max;
        p_curr.mgmt.genStAvgTemp = tempMonBuf.avg;
        p_curr.mgmt.genStMinTemp = tempMonBuf.min;

        /* Get statecode hourly stats */
        scStatsCollect(&p_curr.statecode);
    }
    /*collect current statecode list*/
    collect_current_statecode(p_curr.mgmt.genStateCodeList, p_curr.mgmt.genStateCodeDurationErrCnt);
    get_top_statecode(p_curr.mgmt.genTop1StateCode, 0);    // top 1 statecode
    get_top_statecode(p_curr.mgmt.genTop2StateCode, 1);    // top 2 statecode
    get_top_statecode(p_curr.mgmt.genTop3StateCode, 2);    // top 3 statecode
    
    get_top_statecode_duration(p_curr.mgmt.genTop1StateCodeDuration, 0); // top 1 statecode duration
    get_top_statecode_duration(p_curr.mgmt.genTop2StateCodeDuration, 1); // top 2 statecode duration
    get_top_statecode_duration(p_curr.mgmt.genTop3StateCodeDuration, 2); // top 3 statecode duration
    /* collect the no connect times */
    p_curr.mgmt.genSysNoConnTimeSecs = stats_parms->sysStateCode.stateCodeMonitor.totalSysNoConnSeconds;
    p_curr.mgmt.genUsrNoConnTimeSecs = stats_parms->sysStateCode.stateCodeMonitor.totalUserNoConnSeconds;

    // Collect multicast stats
    p_curr.mgmt.mcastPktsRxV4 = p_kpep_stats->sat_overall_mcast_v4_pkt_rx;
    p_curr.mgmt.mcastPktsRxV6 = p_kpep_stats->sat_overall_mcast_v6_pkt_rx;
    p_curr.mgmt.mcastPktsFwdLan1Static = p_kpep_stats->sat_overall_mcast_pkts_tx[LAN_PORT_1][MCAST_STATIC_MODE];
    p_curr.mgmt.mcastPktsFwdLan1Active = p_kpep_stats->sat_overall_mcast_pkts_tx[LAN_PORT_1][MCAST_ACTIVE_MODE];
    p_curr.mgmt.mcastPktsFwdLan2Static = p_kpep_stats->sat_overall_mcast_pkts_tx[LAN_PORT_2][MCAST_STATIC_MODE];
    p_curr.mgmt.mcastPktsFwdLan2Active = p_kpep_stats->sat_overall_mcast_pkts_tx[LAN_PORT_2][MCAST_ACTIVE_MODE];
    
    //for (i=0; i < parms->pep_parms.pe_parms.max_num_bb; i++)  
    ///{
    //    TRACE_LOG(diagLogId, LOG_INFO, "collect_current_stats: after enterprise stats 22 %p", &p_kpep_stats->tcb_table[i]);
        //if(p_kpep_stats != NULL)
    //  {
    //      TRACE_LOG(diagLogId, LOG_INFO, "collect_current_stats: after enterprise stats 222 %u", p_kpep_stats->pe_tsk_stats.cur_num_of_gre_tcp_spoofed[IPV4]);
            
            p_curr.mgmt.curnumgretcpspoofv4=p_kpep_stats->pe_tsk_stats.cur_num_of_gre_tcp_spoofed[IPV4];
            p_curr.mgmt.curnumgretcpspoofv6=p_kpep_stats->pe_tsk_stats.cur_num_of_gre_tcp_spoofed[IPV6];
    //  }
    //}

    if(p_kpep_stats)
    {
        //TRACE_LOG(diagLogId, LOG_INFO, "collect_current_stats: unmap p_kpep_stats");
        munmap_kpep_stats_mseg(p_kpep_stats);
        p_kpep_stats = NULL;
    }

   // Collect DNS Stats
    p_curr.mgmt.dnsStQRecv = stats_parms->dns_stats.udp_queries_rcvd_a + stats_parms->dns_stats.tcp_queries_rcvd_a +
                              stats_parms->dns_stats.udp_queries_rcvd_aaaa + stats_parms->dns_stats.tcp_queries_rcvd_aaaa;
    p_curr.mgmt.dnsStFQRecv = 0; // Not applicable for the new DNS proxy
    p_curr.mgmt.dnsStQForwarded = stats_parms->dns_stats.queries_snd_a + stats_parms->dns_stats.queries_snd_aaaa;
    p_curr.mgmt.dnsStFQForwarded = stats_parms->dns_stats.forged_queries_snd_a + stats_parms->dns_stats.forged_queries_snd_aaaa;
    p_curr.mgmt.dnsStQRRecv = stats_parms->dns_stats.responses_rcvd_a + stats_parms->dns_stats.responses_rcvd_aaaa;
    p_curr.mgmt.dnsStFQRRecv = stats_parms->dns_stats.forged_responses_rcvd_a + stats_parms->dns_stats.forged_responses_rcvd_aaaa;
    p_curr.mgmt.dnsStCacheRefresh = stats_parms->dns_stats.refresh_queries_snd_a + stats_parms->dns_stats.refresh_queries_snd_aaaa;
    p_curr.mgmt.dnsStFCacheRefresh = 0; // Not applicable for the new DNS proxy
    p_curr.mgmt.dnsStCacheHits = stats_parms->dns_stats.local_answer_a + stats_parms->dns_stats.local_answer_aaaa;
    p_curr.mgmt.dnsStFCacheHits = 0; // Not applicable for the new DNS proxy
    p_curr.mgmt.dnsStTDNSQRecv = stats_parms->dns_stats.tdns_queries_rcvd_a + stats_parms->dns_stats.tdns_queries_rcvd_aaaa;
    p_curr.mgmt.dnsStTDNSQForwarded = stats_parms->dns_stats.tdns_queries_snd_a + stats_parms->dns_stats.tdns_queries_snd_aaaa;
    p_curr.mgmt.dnsStTDNSQRRecv = stats_parms->dns_stats.tdns_responses_rcvd_a + stats_parms->dns_stats.tdns_responses_rcvd_aaaa;
    p_curr.mgmt.dnsStTDNSCacheHits = stats_parms->dns_stats.tdns_local_answer_a + stats_parms->dns_stats.tdns_local_answer_aaaa;
    p_curr.mgmt.dnsStTDNSCacheRefresh = 0;  // Not applicable for the new DNS proxy
    p_curr.mgmt.dhcpRelayEnabledV4 = parms->dhcp_parms.relay_agent_enable;
    p_curr.mgmt.dhcpRelayEnabledV6 = parms->dhcpr_v6_parms.relay_agent_enable;
    p_curr.mgmt.dhcpRelayClPktRelayedV4 = stats_parms->relay_stats.client_packets_relayed;
    p_curr.mgmt.dhcpRelaySerPktRelayedV4 = stats_parms->relay_stats.server_packets_relayed;
    p_curr.mgmt.dhcpRelayClPktErrorV4 = stats_parms->relay_stats.client_packet_errors;
    p_curr.mgmt.dhcpRelaySerPktErrorV4 = stats_parms->relay_stats.server_packet_errors;
    p_curr.mgmt.dhcpRelaybadCirIDV4 = stats_parms->relay_stats.bad_circuit_id;
    p_curr.mgmt.dhcpRelayMissCirIDV4 = stats_parms->relay_stats.missing_circuit_id;
    p_curr.mgmt.dhcpRelaybadIntfIDV4 = stats_parms->relay_stats.bad_interface_id;
    p_curr.mgmt.dhcpRelayMissIntfIDV4 = stats_parms->relay_stats.missing_interface_id;
    p_curr.mgmt.dhcpRelaybogusgiaddrDropV4 = stats_parms->relay_stats.bogus_giaddr_drops;
    p_curr.mgmt.dhcpRelayCorruptAgentDropV4 = stats_parms->relay_stats.corrupt_agent_drops;
    p_curr.mgmt.dhcpRelayAgentOptErrV4 = stats_parms->relay_stats.agent_option_errors;
    p_curr.mgmt.dhcpRelayBootRqstRcvdV4 = stats_parms->relay_stats.boot_request_rcvd;
    p_curr.mgmt.dhcpRelayBootRplyRcvdV4 = stats_parms->relay_stats.boot_reply_rcvd;
    p_curr.mgmt.dhcpRelayStripOptErrV4 = stats_parms->relay_stats.strip_option_error;
    p_curr.mgmt.dhcpRelayMissAgtOptV4 = stats_parms->relay_stats.missing_agent_option;
    p_curr.mgmt.dhcpRelayBogusAgtDropV4 = stats_parms->relay_stats.bogus_agent_drops;  
    p_curr.mgmt.dhcpRelayClPktRelayedV6 = stats_parms->relay_stats_v6.client_packets_relayed;
    p_curr.mgmt.dhcpRelaySerPktRelayedV6 = stats_parms->relay_stats_v6.server_packets_relayed;
    p_curr.mgmt.dhcpRelayClPktErrorV6 = stats_parms->relay_stats_v6.client_packet_errors;
    p_curr.mgmt.dhcpRelaySerPktErrorV6 = stats_parms->relay_stats_v6.server_packet_errors;
    p_curr.mgmt.dhcpRelaybadCirIDV6 = stats_parms->relay_stats_v6.bad_circuit_id;
    p_curr.mgmt.dhcpRelayMissCirIDV6 = stats_parms->relay_stats_v6.missing_circuit_id;
    p_curr.mgmt.dhcpRelaybadIntfIDV6= stats_parms->relay_stats_v6.bad_interface_id;
    p_curr.mgmt.dhcpRelayMissIntfIDV6 = stats_parms->relay_stats_v6.missing_interface_id;
    p_curr.mgmt.dhcpRelaybogusgiaddrDropV6 = stats_parms->relay_stats_v6.bogus_giaddr_drops;
    p_curr.mgmt.dhcpRelayCorruptAgentDropV6 = stats_parms->relay_stats_v6.corrupt_agent_drops;
    p_curr.mgmt.dhcpRelayAgentOptErrV6 = stats_parms->relay_stats_v6.agent_option_errors;
    p_curr.mgmt.dhcpRelayBootRqstRcvdV6 = stats_parms->relay_stats_v6.boot_request_rcvd;
    p_curr.mgmt.dhcpRelayBootRplyRcvdV6 = stats_parms->relay_stats_v6.boot_reply_rcvd;
    p_curr.mgmt.dhcpRelayStripOptErrV6 = stats_parms->relay_stats_v6.strip_option_error;
    p_curr.mgmt.dhcpRelayMissAgtOptV6 = stats_parms->relay_stats_v6.missing_agent_option;
    p_curr.mgmt.dhcpRelayBogusAgtDropV6 = stats_parms->relay_stats_v6.bogus_agent_drops;
    p_curr.mgmt.greSpoofEnabled=parms->pep_parms.pe_parms.gre_spoof;

    p_curr.mgmt.bgpdstatus = parms->bgpd_parms.bgpd_status;

    
    for(i = 0; i < BGP_MAX_PEER_COUNT; i++)
    {
        len = strlen(stats_parms->bgpStatus[i].peerStatus);
        if(len < BGPD_SUMMARY_STATUS_LEN)
            break;
        strcpy(tmpStr, stats_parms->bgpStatus[i].peerStatus);
        //BGP Status string has the format "Peer IP, State, UP Time, AS #, Up/Down, NextHop IP 
        for(str2 = tmpStr; bgpColumn < 6; str2 = NULL, bgpColumn++)
        {
            token = strtok_r(str2, ", ", &savePtr);
            if (token == NULL)
                break;
            switch(bgpColumn)
            {
                case 1:
                    if(atoi(token)||(!strcmp(token, "0")))
                    {
                         p_curr.mgmt.bgpdpeers++;
                    }
                    else
                    {
                        //TRACE_LOG(diagLogId, LOG_DEBUG, "Peer State Down\n");
                    }
            }
                    
        }        
    }
    p_curr.mgmt.gtpnumTunnelsCreated = gtpstats.numTunnelsCreated;
    p_curr.mgmt.gtpnumTunnelsDeleted = gtpstats.numTunnelsDeleted;
    p_curr.mgmt.gtpnumCurTunnelCnt = gtpstats.numCurrentTunnelCnt;
    p_curr.mgmt.gtpStNumVoiceCalls = gtpstats.gtpStNumVoiceCalls;
    p_curr.mgmt.gtpNumPktsHeaderStripped = gtpstats.gtpNumPktsHeaderStripped;
    p_curr.mgmt.gtpNumUDPPktHeaderRestore = gtpstats.gtpNumUDPPktHeaderRestore;
    p_curr.mgmt.gtpNumSpoofedTCPPktHeaderRestore = gtpstats.gtpNumSpoofedTCPPktHeaderRestore;
    p_curr.mgmt.gtpStRcvBytesConvDSCP = gtpstats.gtpStRcvBytesConvDSCP;
    p_curr.mgmt.gtpStRcvBytesStrmDSCP = gtpstats.gtpStRcvBytesStrmDSCP;
    p_curr.mgmt.gtpStRcvBytesIntrDSCP = gtpstats.gtpStRcvBytesIntrDSCP;
    p_curr.mgmt.gtpStRcvBytesBkgdDSCP = gtpstats.gtpStRcvBytesBkgdDSCP;
    p_curr.mgmt.gtpTxBytesConvDSCP = gtpstats.gtpTxBytesConvDSCP;
    p_curr.mgmt.gtpTxBytesStrmDSCP = gtpstats.gtpTxBytesStrmDSCP;
    p_curr.mgmt.gtpTxBytesIntrDSCP = gtpstats.gtpTxBytesIntrDSCP;
    p_curr.mgmt.gtpTxBytesBkgdDSCP = gtpstats.gtpTxBytesBkgdDSCP;
    p_curr.mgmt.vrrpEnabled=parms->vrrpCfgParms.vrrpCfMode;
    
    //memset(buf, 0, 6000);
    //memset(command,0,200);
    //memset(floatingAddressSubnet,0,30);
    memset(vrState,0,20);
    //memset(ipaddrstr, 0, INET_ADDRSTRLEN);
    /*if (parms->vrrpCfgParms.vrrpCfInstance[0].vrConfigurationValid)
    {
        inet_ntop(AF_INET, &(parms->vrrpCfgParms.vrrpCfInstance[0].vrFloatingIPv4Addr), ipaddrstr, INET_ADDRSTRLEN);
        sprintf(floatingAddressSubnet,"%s",ipaddrstr);
    
        sprintf(command,"rm /tmp/vrInterfaceStatus; /sbin/ip addr show dev %s > /tmp/vrInterfaceStatus",(parms->vrrpCfgParms.vrrpCfInstance[0].vrInterfaceDevice));
        system(command);
    
        if(OK == read_proc_file("/tmp/vrInterfaceStatus", buf, sizeof(buf)))
        {
            if (strstr(buf,floatingAddressSubnet) != NULL)
                sprintf(vrState,"MASTER");
            else
                sprintf(vrState,"BACKUP");
        }
    
    }
    else
    {
        sprintf(vrState,"UNKNOWN");
    }
    */
    sprintf(p_curr.mgmt.vrrpInstanceStatus,"%s",vrState);
    
#ifdef THEBE_PLATFORM 
    memcpy(p_curr.mgmt.cachingAppDevId, parms->caching_app_parms.pair_request.cachingAppDeviceId, sizeof(p_curr.mgmt.cachingAppDevId));
    memcpy(p_curr.mgmt.cachingAppState, cachingAppState[stats_parms->mai_stats.cachingAppState], sizeof(p_curr.mgmt.cachingAppState));
    memcpy(p_curr.mgmt.cachingAppSWVersion, parms->caching_app_parms.pair_request.cachingAppDeviceSW, sizeof(p_curr.mgmt.cachingAppSWVersion));
    memcpy(p_curr.mgmt.cachingAppHWVersion, parms->caching_app_parms.pair_request.cachingAppDeviceHW, sizeof(p_curr.mgmt.cachingAppHWVersion));
    //memcpy(p_curr.mgmt.cachingAppLastMsgRecvTime, timeStr, sizeof(p_curr.mgmt.cachingAppLastMsgRecvTime));
    p_curr.mgmt.cachingAppUnknowMsgCnt = stats_parms->mai_stats.cachingAppMsgStatsTbl[CACHING_APP_MSG_UNKNOWN].msgCnt;
#else
    memcpy(p_curr.mgmt.cachingAppDevId, "N/A", sizeof(p_curr.mgmt.cachingAppDevId));
    memcpy(p_curr.mgmt.cachingAppState, "N/A", sizeof(p_curr.mgmt.cachingAppState));
    memcpy(p_curr.mgmt.cachingAppSWVersion, "N/A", sizeof(p_curr.mgmt.cachingAppSWVersion));
    memcpy(p_curr.mgmt.cachingAppHWVersion, "N/A", sizeof(p_curr.mgmt.cachingAppHWVersion));
    memcpy(p_curr.mgmt.cachingAppLastMsgRecvTime, "N/A", sizeof(p_curr.mgmt.cachingAppLastMsgRecvTime));
    p_curr.mgmt.cachingAppUnknowMsgCnt = 0;
#endif     
    //TRACE_LOG(diagLogId, LOG_DEBUG, "collect_current_stats: Exit");
    

	memcpy(&stats_parms->system_stats.curr.outroute,&p_curr.outroute,sizeof(outroute_coln_stats_t));
    memcpy(&stats_parms->system_stats.curr.inroute,&p_curr.inroute,sizeof(inroute_coln_stats_t));
    memcpy(&stats_parms->system_stats.curr.wac,&p_curr.wac,sizeof(wac_coln_stats_t));
    memcpy(&stats_parms->system_stats.curr.mgmt,&p_curr.mgmt,sizeof(mgmt_coln_stats_t));
    memcpy(&stats_parms->system_stats.curr.pep,&p_curr.pep,sizeof(pep_coln_stats_t));
    memcpy(&stats_parms->system_stats.curr.intf,&p_curr.intf,sizeof(intf_coln_stats_t));
    memcpy(&stats_parms->system_stats.curr.assoc,&p_curr.assoc,sizeof(assoc_coln_stats_t));
    memcpy(&stats_parms->system_stats.curr.statecode,&p_curr.statecode,sizeof(statecode_coln_stats_t));
    memcpy(&stats_parms->system_stats.curr.bb_switch,&p_curr.bb_switch,sizeof(bb_switch_coln_stats_t));


}


void init_string(struct data_string *s) {
    s->len = 0;
    s->ptr = (char *)malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct data_string *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = (char *)realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}

size_t writeCallback(char* buf, size_t size, size_t nmemb, char* up)
{ //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

    for (int c = 0; c<size*nmemb; c++)
    {
        //data.push_back(buf[c]);
        up[c] = buf[c];
    }
    up[size*nmemb] = '\0';

    return size*nmemb; //tell curl how many bytes we handled
}

#define DATA_LEN    2000
#define CAC         1
#define UPLINK      2
#define DOWNLINK    3
#define CFM         4
#define STATS       5
//#define WAC         6
//#define PEP         7
#define SKIP       -1

inline short _translate_path(string path)
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
    /*else if (path == "wac")
        return WAC;
    else if (path == "pep")
        return PEP;*/
    else  
        return SKIP;
}

//int _parse_conf(string &rows, string &style, unsigned int iter_ms, vector<string> &labels, vector<short> &paths, vector<vector<string> > &vars, string message)
int _parse_conf(int idx, string message)
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

string _get_headers(string &rows, string &style, vector<string> &labels, vector<short> &paths, vector<vector<string> > &vars, vector<vector<string> > &keys_for_vars)
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
    //printf("get headers: %s\n", headers.c_str());
    return headers.c_str();
}

string _get_data(string &rows, vector<string> &labels, vector<short> &paths, vector<vector<string> > &keys)
{
    //collect all current stats
    collect_current_stats(1);
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
        data_str = "";
        for(int i = 0; i < data.size(); i++)
        {
            for(int y = 0; y < data[i].size(); y++)
            { 
                data_str += keys[i][y] + ":";
                data_str += data[i][y] + ((i == data.size()-1 && y == data[i].size()-1) ? "" : ",");
            }
        }
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
    //printf("get data: %s\n", data_str.c_str());
    return data_str.c_str();
}

void set_timer(server* s, websocketpp::connection_hdl hdl) 
{
    connection_ptr con = s->get_con_from_hdl(hdl);
    //std::cout << "set_timer called with hdl: " << con->sessionid << std::endl;
    timer_ptr m_timer;
    m_timer = con->set_timer(conn_stats[con->sessionid].iter_ms, bind(&on_timer,s,hdl,::_1));
}

void on_timer(server* s, websocketpp::connection_hdl hdl, websocketpp::lib::error_code const & ec) 
{
    //websocketpp::lib::error_code ec;
    connection_ptr con = s->get_con_from_hdl(hdl);
    //std::cout << "on_timer called with hdl: " << con->sessionid << std::endl;
    string data;
    string output;
    
    data = _get_data(conn_stats[con->sessionid].rows, conn_stats[con->sessionid].labels, conn_stats[con->sessionid].paths, conn_stats[con->sessionid].keys);
    output="data:";
    output.append(data);
    //std::cout << "Message sent : " <<  data;
    
    s->send(conn_stats[con->sessionid].hdl,output,websocketpp::frame::opcode::text);
    //s->get_alog().write(websocketpp::log::alevel::app, "Sent Message: "+output);
    
    set_timer(s, hdl);  
}

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) 
{
    int err = 0;
    connection_ptr con = s->get_con_from_hdl(hdl);
    // check for a special command to instruct the server to stop listening so
    // it can be cleanly exited.
    if (msg->get_payload() == "stop-listening") 
    {
        s->stop_listening();
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
        headers = _get_headers(conn_stats[con->sessionid].rows, conn_stats[con->sessionid].style, conn_stats[con->sessionid].labels, conn_stats[con->sessionid].paths, conn_stats[con->sessionid].vars, conn_stats[con->sessionid].keys);
        
        //std::cout << "Headers: " <<  headers << std::endl;
        output="headers:";
        conn_stats[con->sessionid].hdl = hdl;
        //connections[num_connection] = hdl;
        output.append(headers);
        set_timer(s, hdl);
        //tfwStartTimer(con->sessionid, TFW_GET_SEC_TICKS(conn_stats[con->sessionid].iter_ms), TFW_ONESHOT, DIAG_SERV_APPL_ID);
        s->send(hdl,output,websocketpp::frame::opcode::text,ec);
		if (ec)
        {
            std::cout << "Echo failed because: " << ec.message() << std::endl;
        }
		
		data = _get_data(conn_stats[con->sessionid].rows, conn_stats[con->sessionid].labels, conn_stats[con->sessionid].paths, conn_stats[con->sessionid].keys);
		output="data:";
		output.append(data);
		//std::cout << "Message sent : " <<  data;
		
		s->send(conn_stats[con->sessionid].hdl,output,websocketpp::frame::opcode::text);
        //s->get_alog().write(websocketpp::log::alevel::app, "Sent Message: "+output);
        if (ec)
        {
            std::cout << "Echo failed because: " << ec.message() << std::endl;
        }
        //s->interrupt(hdl);
    }
}


void on_open(server* s, websocketpp::connection_hdl hdl) 
{
    s->get_alog().write(websocketpp::log::alevel::app, "Connection opened");
    connection_ptr con = s->get_con_from_hdl(hdl);
    con->sessionid = num_connection++;
    //send relavent statistics here...
}


void on_close(server* s, websocketpp::connection_hdl hdl) {
    s->get_alog().write(websocketpp::log::alevel::app, "Connection Closed");
}

int main()
{
    int ret;

    /* attach to global cfm shared memory */
    std::cout<<"Attach global cfm shm"<<std::endl;
    parms =(cfm_external_parms_t*) shm_lib_attach(SHM_CONFIG_SHARED_MEM_KEY, SHM_SIZE);
    if(!parms) 
    {
        perror("Config SHM attach");
        return ERROR;
    }
   
    /* attach to global stats shared memory */
    std::cout<<"Attach global stats shm"<<std::endl;
    stats_parms =(global_stm_t *) shm_lib_attach(SHM_STATS_SHARED_MEM_KEY, SHM_STATS_SIZE);
    if (!stats_parms) 
    {
        perror("Stats SHM attach");
        return ERROR;
    }
 
    initialize_cacSHM_t_shm();
    initialize_uplinkSHM_t_shm();
    initialize_downlinkSHM_t_shm();
    initialize_cfm_external_parms_s_shm();
    initialize_global_stm_t_shm();
    //initialize_kernel_stats_t_shm(); 
    initStatecodeDurArray();
    //if ((ret=initialize_kernel_stats_t_shm() )<0 )
    //{
    //	 std::cout <<"failed to initialize kernel shm data " << std::endl;
    //}
    int i = 0;

    server echo_server;
	
    try 
    {   
        // Set logging settings
        echo_server.set_access_channels(websocketpp::log::alevel::none);//all
        echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);
	
        // Initialize Asio
        echo_server.init_asio();
	
        // Register our message handler
	    echo_server.set_open_handler(bind(&on_open,&echo_server,::_1));
        echo_server.set_message_handler(bind(&on_message,&echo_server,::_1,::_2));
        echo_server.set_close_handler(bind(&on_close,&echo_server,::_1));
        // Listen on port 9002
        //echo_server.listen(boost::asio::ip::address::from_string("192.168.0.1"),9002);
        echo_server.set_reuse_addr(reuse_flag);
	    echo_server.listen(boost::asio::ip::address::from_string("127.0.0.1"),9002);
        // Start the server accept loop
        echo_server.start_accept();
	
        // Start the ASIO io_service run loop
        echo_server.run();
		
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
    
    //close_kernel_stats_t_shm();
    // if(ret==0)
    // 	close_kernel_stats_t_shm();

    return 0;
}
