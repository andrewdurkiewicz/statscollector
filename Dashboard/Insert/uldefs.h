/** \file
 * \brief Contains all uplink specific definitions and enumerations.
 */

/***********************************************************************
 *
 *  FILE NAME:    uldefs.h
 *
 *  DESCRIPTION: This file should only contain definitions and enumerations
 *               used by any of the uplink modules. The module's structures and
 *               function prototypes will remain in the module's own header files.
 *               This allows for ANYONE to use these definitions and enumerations
 *               without having to pick up all the extra structures and function
 *               prototypes as well.
 *
 *     DATE         NAME           REFERENCE        REASON
 *  ----------  -------------   --------------    ----------------------
 *  05-18-2011  vchan           S-01144           Created
 *  05-24-2011  suchil          S-01112           Process ICAP Messages
 *  05-26-2011  suchil          S-01096,97,99     Ranging Adaptation Messages
 *  06-01-2011  vchan                             Merged Aloha and aperture types into one.
 *  05-31-2011  suchil                            Change IBE indicator bits to macros
 *  06-03-2011  suchil          S-01183           Process IPFP Feedback for ACK/NAKs
 *  06-07-2011  suchil          S-01182           Ranging trigger - user initiated
 *  06-14-2011  vchan                             Updated SFNP processing to mimic EXACTLY HN calculations
 *  06-15-2011  suchil          S-01185           Send updates and results to IGM during ranging process
 *  06-17-2011  vchan           S-01132           Baseline initial timing and save into ultiming.txt
 *  06-20-2001  vchan           S-01176           Implement concept of timing validation
 *  06-23-2001  vchan                             Timing: Temporarily relax SFNP interval sanity check until GW fix
 *  06-24-2011  suchil          S-01186           Power adjustments based on avg feedback
 *  06-24-2011  vchan           S-01199           Common Air Interface message units update for timing
 *  06-24-2011  vchan           S-01199           Timing poll adaptation message handling updated to match latest ICD
 *  06-27-2011  vchan           S-01199           Updated ADAP_BASE_HEADER_LEN declaration
 *  06-28-2011  vchan           S-01217           Timing drift analysis (with enhanced SFNP error tracking)
 *  06-30-2011  vchan           S-01199           Common Air Interface message units update for timing (use of dekananoseconds) cont.
 *  07-06-2011  suchil          S-01025           Ranging trigger intial install
 *  07-08-2011  vchan           S-01201           TRO and CLT tracking and adjustment part 1, setup variables and webpages
 *  07-08-2011  vchan           S-01201           TRO and CLT tracking and adjustment part 1, added EST history tracking
 *  07-11-2011  vchan           S-01201           TRO and CLT tracking and adjustment part 2, updated SFNP format and added reference history tracking
 *  07-12-2011  vchan           S-01201           TRO and CLT tracking and adjustment part 3, added reference update and TRO update capabilites and usrcmd timing overrides.
 *  07-13-2011  vchan           S-01201           TRO and CLT tracking and adjustment part 3, support basic EST and CLT TRO calculations.
 *  07-13-2011  suchil          S-01025           Rate module controls ranging
 *  07-15-2011  vchan           S-01235           Add polling response history keeping
 *  07-16-2011  suchil                            Ranging State Machine
 *  07-23-2011  suchil          S-01204           Ranging Rate Selection
 *  07-24-2011  mbaer           S-01129           Enumerations for PDU CRC configuration
 *  07-25-2011  vchan           S-01235           Handle race condition of CLT and SFNP EST updates.
 *  07-29-2011  suchil          S-01206           Single Rate Ranging
 *  08-04-2011  suchil          S-01273           Record power feedbacks
 *  08-05-2011  suchil          SPR 1793          Adaptation messages get stuck in the sent state
 *  08-05-2011  vchan                             Added webpage to track large adjustments (> 5us) and added frame number to SFNP error traces
 *  08-05-2011  suchil          SPR 1916          Ranging done only after receiving a power results ack
 *  08-09-2011  vchan           SPR 1976          Add missed SFNP count to webpage and misc cleanup
 *  08-05-2011  suchil          SPR 1965          Failed ranging of most robust rate not handled properly
 *  08-19-2011  mbaer           S-01129           Move PDU CRC config enumeration to uplinkcommon.h
 *  08-21-2011  vchan           S-01178           Initial timing calculations based on satellite and terminal location
 *  08-26-2011  vchan           SPR 2262          Update system delay and gateway change detection handling
 *  08-29-2011  vchan           SPR 2264          Correct handling of multiple TSA.
 *  08-31-2011  vchan           SPR 2324          Reset timing parameters for every bootstrap Aloha.
 *  09-12-2011  vchan           SPR 2367          Shift timing for bootstrap aloha transmission
 *  09-16-2011  mbaer           S-01289           Burst debug over LAN
 *  08-16-2011  suchil                            CLPC Feature
 *  09-19-2011  suchil          SPR-2575          Default ULFE Bin Size
 *  09-23-2011  suchil          SPR-1966          Estimate new rates added to the Trajectory table
 *  09-26-2011  suchil          SPR-1948          Terminate ranging process after timeout expiry
 *  10-04-2011  suchil          SPR-2672          Final ranged power setting is somehow set to 0
 *  10-07-2011  suchil                            Increase power if terminal backs off too much
 *  10-11-2011  mbaer           S-01305           Terminal capabilities (Dedicated Bandwidth) for ContextInfo
 *  10-12-2011  vchan           SPR 2758          Terminal stuck in transmitting Small Aloha despite Bootstrap Aloha needed
 *  10-12-2011  drivers         S-01284           Add inroute limited stats
 *  10-21-2011  vchan           S-01306           Terminal misses mid frame interrupts when CLPC is enabled
 *  10-21-2011  vchan                             Increase CLPC memory dump size
 *  10-28-2011  suchil                            Change symcod macro
 *  10-31-2011  mbaer           S-01175           Group module defines
 *  11-01-2011  vchan           SPR 2916          Additional group selection check before performing bootstrap Aloha shift.Additional group selection check before performing bootstrap Aloha shift.
 *  11-08-2011  suchil                            AIS Feature
 *  11-11-2011  suchil                            Terminal barred feature
 *  11-11-2011  ksamavedam                        Changede hemisphere definitions to match sbc defs
 *  11-21-2011  suchil                            Change ODU max power setting to 60000
 *  11-23-2011  vchan           SPR 3174          CLT timer for applying CLT adjustments should be changed from 30 to 150 seconds.
 *  11-28-2011  mbaer                             Refine rate available enum
 *  11-28-2011  suchil                            Reset AIS and CLPC after ranging
 *  11-29-2011  vchan           SPR 2917          "Reset All Timing" link should clear out bootstrap Aloha shift.
 *  12-05-2011  suchil          SPR 3060          Terminate ranging when ranging metric is zero
 *  12-05-2011  vchan           SPR 3064          Too big timing offset when switch rate from 512k to 2048k
                                SPR 3065          Incorrect BitRate shift when reboot terminal and when switch rate
 *  01-09-2012  vchan                             Implement timing adjustment debouncing to prevent jitter.
 *  01-09-2012  suchil          S-01329           Read max power setting from oduinfo.txt
 *  01-12-2012  vchan           SPR 3668          In STEADY state network the terminal should re-establish reference when using bootstrap Aloha
 *  01-12-2012  suchil          SPR-3606          Need to revamp decimation for power control algo
 *  01-17-2012  mbaer           S-01345           Additional limited stats that were missing
 *  02-02-2012  vchan           SPR 3904          Better log handling when bootstrap shift results in shift of 0
 *  02-07-2012  vchan           SPR 3929          Add timing adjustment logging to a file with ability to disable
 *  02-09-2012  suchil          SPR 3893          Terminal bootstraps to the lowest rate after a groups timeout.
 *  02-22-2012  vchan           SPR 4078          Do not continuously perform SFNP timing sync with NOC using HN algorithm
 *  02-28-2012  vchan           S-1372            Update frame interrupt marker history to track anomalies
 *  03-05-2012  vchan           SPR 4175          Implement mechanism to initiate clear timing if bootstrap Aloha fails.
 *  03-07-2012  vchan           SPR 4079          Investigate SFNP timing sync in OTA environment with delay
 *  03-12-2011  vchan           S-01385           Terminal Backlog Reporting Handling
 *  03-14-2012  vchan           SPR 4314          Terminal doesn't handle SFNP HSH change during initial bootstrap.
 *  03-21-2012  suchil          S-01375           Aloha ingress algorithm
 *  03-28-2012  mbaer           SPR 4484          Ensure that Context Info is sent to current inroute group
 *  04-06-2012  vchan                             Change timeout of switching from small to bootstrap Aloha
 *  04-26-2012  mbaer           SPR 4688          Group Selection and Metric type enums
 *  05-15-2012  vchan           SPR 4819          Update reference information and bootstrap Aloha handling
 *  05-17-2012  vchan           S-01355           Gateway Test Terminal transmission with no delay
 *  07-24-2012  BWang                             Add clpc feedback file archive related
 *  09-25-2012  mbaer                             Cavalier Sync threshold is configurable
 *  09-28-2012  vchan                             Add Gershwin/Cavalier sync trace
 *  09-28-2012  mbaer                             Defines for Tx LED 'winking'
 *  07-30-2012  vchan           S-01451           Code cleanup
 *  08-15-2012  vchan           S-01477           Port over ultiming module and related webpages to Callisto
 *  09-10-2012  jsanchez                          Added Regular Aperture Burst type
 *  10-24-2012  mbaer                             Move Burst Debug over LAN defines here
 *  10-31-2012  S. Uchil        S-01534           Set unique word on every group change
 *  11-14-2012  vchan                             Increase MAX_BURSTS_IN_FRAME from 32 to 256
 *  11-14-2012  vchan                             Change MAX_BURSTS_IN_FRAME back to 32 for now.
 *  11-16-2012  mbaer                             Change MAX_ADAPTATION_SIZE to 96 bytes
 *  11-16-2012  vchan                             Moved sendTROAdjustConfig back to ulTfixRun with frame delay protection
 *  12-06-2012  vchan           S-01482           Implement timing/burst module sync for making frame number and timing adjustments
 *  12-18-2012  vchan                             Add definition for TXHA_FRM_SYNC_BIT
 *  12-13-2012  vchan           SPR 6300          Track Bootstrap Aloha ack and nak counts based on network timing state
 *  01-17-2013  suchil                            Change RANGE_NO_FEEDBACK enum
 *  01-22-2013  drivers         S-01555           CTTT: Add Force Symcod command
 *  01-28-2013  drivers         S-01563           Enhance CTTT command to bar terminal transmission
 *  01-17-2013  suchil                            Change RANGE_NO_FEEDBACK enum
 *  01-23-2013  vchan                             Update and refactor FIT and symcod enum to include modulation type 8PSK and 16PSK.
 *  01-23-2013  mbaer                             Increase MAX_BURSTS_IN_FRAME for Callisto
 *  01-31-2013  vchan           SPR 6612          Implement flywheel for frame number adjustments
 *  02-19-2013  mbaer                             Macrofy Aloha type enumeration
 *  02-14-2013  vchan                             Add checks for skipped SFNPs
 *  02-20-2013  lqin                              Increased MAX_BURSTS_IN_FRAME from 300 to 341 for Callisto
 *  03-05-2013  vchan                             Frame number handling updated, removed SWP/UPP sync, add more stats
 *  03-05-2013  vchan           SPR 6339          Use Aloha ack to sync frame number instead of SFNP to avoid SFNP processing delay issues.
 *  03-11-2013  vchan                             Frame number synchronization between timing, aloha, and burst ctrl modules.
 *  04-02-2013  ksamavedam                        ROHC Implementation
 *  04-18-2013  vchan           SPR 7059          Terminal stuck at 12.7.2 (Bootstrap Aloha Transmission is Unsuccessful) after reboot
 *  05-14-2013  vchan                             Remove symcod related definitions and include SymCodDefs.h
 *  02-09-2016  ehumphrey                         Aero Updates
 *
 *  Copyright 2012-2013, Hughes Network Systems, LLC
 *  Hughes Proprietary
 *
 **************************************************************************/

#ifndef _ULDEFS_H
#define _ULDEFS_H

#include "SFNPCommonDefinitions.h"
#include "ICAPCommonDefinitions.h"
#include "IGDPCommonDefinitions.h"
#include "ISDPCommonDefinitions.h"
#include "SymCodDefs.h"
#include "oduinfo.h"

/*************************************************************/
/*************************************************************/
/*                  UPLINK GENERAL BEGIN                     */
/*************************************************************/
/*************************************************************/

enum testTerminal_e
{
    TERMINAL_NOT_UNDER_TEST = 0,    /* not a test terminal  */
    TERMINAL_UNDER_TEST     = 1,    /* a general test terminal in the field */
    TERMINAL_TVOA           = 2,    /* a TVOA test terminal */
    TERMINAL_GWHM           = 3,    /* a Gateway Health Monitor terminal */
    TERMINAL_AERO_RACK_HM   = 4,    /* Aero Rack mounted Health Monitor */
    TERMINAL_TEST_TYPE_MAX
};

enum terminalBarredStatus_e
{
    TERM_NOT_BARRED = 0,
    TERM_IS_BARRED  = 1
};

enum terminalBarredReason_e
{
    BARRED_REASON_NOT_BARRED  = 0,
    BARRED_REASON_SYS_INFO    = 1,
    BARRED_REASON_TEST_CMD    = 2,
    BARRED_REASON_POINTING_IN_PROGRESS = 4,
    BARRED_REASON_NO_A429_GPS = 8,
    BARRED_REASON_SPREADING_REQUIRED = 16,
    BARRED_REASON_AMIP_TRANSMIT_DISABLED = 32,
    BARRED_REASON_NO_ANTENNA_CONNECTION = 64,
    BARRED_REASON_LOCAL_COMMANDED = 128,
    BARRED_REASON_OUT_OF_RANGE_GAIN_TABLE_VALUES = 256
};

enum uplinkTimer_e
{
    TX_LED_WINK               = 1,
    COLLECT_STATS             = 2,
    ONE_MINUTE_TASKS          = 3,
    TWO_MINUTE_TASKS          = 4,
    ODU_TASKS                 = 5,
    AERO_PERIODIC             = 6
};

#define UPLINK_STARTUP_HYSTERESIS (12 * TX_FRAMES_PER_SEC)  /* How long to wait on startup before monitoring state codes */
#define TERMINAL_BARRED_FILE "/fl0/termBarred.txt"
#define TX_LED_WINK_INTERVAL_MS 500
#define COLLECT_STATS_INTERVAL_MS 5000
#define AERO_PERIODIC_INTERVAL_MS 1000
#define UPDATE_RECENT_STATS_INTERVAL_MS 60000
#define ONE_MINUTE_TIMER_MS 60000
#define TWO_MINUTE_TIMER_MS 120000
#define ONE_SECOND_TIMER_MS 1000
#define COLLECTION_INTERVAL (UPDATE_RECENT_STATS_INTERVAL_MS / COLLECT_STATS_INTERVAL_MS)

#define TFIX_TRO_ADJUST_CONFIG_DELAY_FRAMES 32          /* number of frames to wait after TRO adjust config command is sent */

#define UNRECOVERABLE_STATE_CODE_CRITICAL_FRAMES_THRESHOLD 220      /* number of frames in unrecoverable CRITICAL state code before rebooting */
#define UNRECOVERABLE_STATE_CODE_MAJOR_FRAMES_THRESHOLD 700800000   /* number of frames in unrecoverable MAJOR state code before rebooting (1 year!) */
#define STREAM_ERROR_STATE_CODE_THRESHOLD 50                        /* stream error rate, in tenths of percent, for indicating high stream error state code */
#define SCMA_ERROR_STATE_CODE_THRESHOLD 50                        /* scma error rate, in tenths of percent, for indicating high stream error state code */
#define STREAM_ERROR_STATE_CODE_MIN_BURSTS 2000                     /* number of stream bursts that must be sent before indicating high stream error rate state code */
#define SCMA_ERROR_STATE_CODE_MIN_BURSTS   2000                     /* number of SCMA bursts that must be sent before indicating high stream error rate state code */
#define DEFAULT_DYN_MC_MODCOD_TIMER 10                              /* default interval (minutes) that the MODCOD adaptation is sent when a terminal is configured with multicast groups */
#define DYN_MC_MODCOD_DISABLED 0                                    /* Dynamic Multicast Modcod is disabled in the default configuration*/


/*Uplink messages*/
#define MODCOD_ACK_RESPONSE_MSGID            (MSTYPE_OFFSET_UPLINK + 0)
#define UPLINK_GROUP_CHANGE_MSGID            (MSTYPE_OFFSET_UPLINK + 1)
#define UPLINK_RATE_CHANGE_MSGID             (MSTYPE_OFFSET_UPLINK + 2)
#define UPLINK_CONF_ARRAY_WRITE_MSGID        (MSTYPE_OFFSET_UPLINK + 3)


/*************************************************************/
/*************************************************************/
/*                  UPLINK GENERAL END                      */
/*************************************************************/
/*************************************************************/

 /**************************************************************/
/**************************************************************/
/*                    TIMING MODULE BEGIN                     */
/**************************************************************/
/**************************************************************/
#define TIMING_TXT_FILE "/fl0/ultiming.txt"
#define TIMING_TXT_VERSION  5

#define DEFAULT_ALOHA_FRAME_CORRECTION_THRESHOLD 8
#define TIMING_TERMINAL_DELAY       111900  /* Known terminal processing delay as burst make its way through the terminal, 10s of ns */
#define CAV_SYNC_LOSS_THRESH_MS     90          /* In milliseconds */
#define CAV_SYNC_LOSS_COUNT_THRESH  3           /* How many Cavalier clock mis-syncs before declaring error */
#define MS_TO_CAV_CLOCKS            100000      /* How many Cavalier clock ticks are in one millisecond */

#define SUPERFRAME_COUNT 8

#define LAB_MAX_TIMING_DELAY_10NS   100000      /* the maximum delay that is still considered no-delay lab environment is 1 ms */

#define SF_TIME_THRESHOLD_10NS      500         /* 5 ms in units of 10ns */
#define SUPER_FRAME_TIME_10NS       36000000    /* super frame duration in 10ns */
#define SUPER_FRAME_TIME_100NS      3600000     /* super frame duration in 100ns */
#define SUPER_FRAME_TIME_MS         360         /* super frame duration in ms */
#define SUPER_FRAME_TIME_100NS      3600000     /* super frame duration in 100ns */
#define FRAME_TIME_10NS             4500000     /* frame duration in 10ns */
#define FRAME_TIME_100NS            450000      /* frame duration in 100ns */
#define FRAME_TIME_US               45000       /* frame duration in us */
#define FRAME_TIME_MS               45          /* frame duration in ms */

#define TIMING_100NS_TO_10NS        10          /* to track which variables we converted from units of 100NS to 10NS */
#define TIMING_US_TO_10NS           100         /* to track which variables we converted from units of US to 10NS */
#define TIMING_MS_TO_10NS           100000      /* to track which variables we converted from units of MS to 10NS */
#define TIMING_SEC_TO_10NS          100000000   /* to track which variables we converted from units of SEC to 10NS */

#define TRO_CHANGE_HISTORY_THRESHOLD_10NS   100 /* Don't bother saving unless a TRO component changed by more than 1 us */

#define CLT_TIMER_IN_FRAMES     150*TX_FRAMES_PER_SEC   /* frames to stay with CLT adjustment before transitioning to SFNP Estimates in race conditions */
#define CLT_EST_DIFF_MAX        100     /* The value that the CLT and EST can differ (1 us) in units of 10ns */

#define TIMING_BOOTSTRAP_SHIFT_ERROR_INDICATOR      2   /* 20 ns, something insignificant to show that there is a problem doing a normal bootstrap Aloha shift */
#define TIMING_BOOTSTRAP_SHIFT_OF_ZERO_INDICATOR    1   /* 10 ns, something insignificant to show that the bootstrap shift was calculated to be 0 */

#define LARGE_TIMING_ADJ_1US_THRESHOLD_10NS  100    /* any timing adjustments greater than 2 us  */
#define LARGE_TIMING_ADJ_2US_THRESHOLD_10NS  200    /* any timing adjustments greater than 2 us  */
#define LARGE_TIMING_ADJ_5US_THRESHOLD_10NS  500    /* any timing adjustments greater than 5 us  */

#define FINE_ADJUSTMENT_THRESHOLD_10NS       960    /* threshold to do a fine adjustment without having to stop bursting */

#define GATEWAY_CHANGE_REFERENCE_THRESHOLD     5    /* number of seconds difference in the reference timestamp that is allowable without declaring a full gateway change */

#define TIMING_DEFAULT_SMALL_ALOHA_TMO      30
#define TIMING_DEFAULT_FRAME_NUMBER_ADJUSTMENT_FLYWHEEL_COUNT   1

/* TXHA timing-related registers */
#define TXHA_FRM_SYNC_BIT           (1)         /* (0x00000001) */
#define TXHA_TRO_BIT                (1<<6)      /* (0x00000040) */

#ifdef THEBE_PLATFORM
/* For Acadia: GershTimestamp is defined the same as register: TXHA_TIMESTAMP_HOLD_R: bit[0:26] timestamp at 245.76 MHz; bit[27:31] frame counter
   For Sahara: GershTimestamp is bit[0:25]; bit[27:31] unused and always 0 */
#define TXHA_TIMESTAMP_MASK         (0x7FFFFFF) /* bit[0:26] */
#define TXHA_CLOCK_RATE             245760000   /* Acadia clock rate: 245.76 MHz clock ticks */
#else
#define TXHA_TIMESTAMP_MASK         (0x3FFFFFF) /* bit[0:25] */
#define TXHA_CLOCK_RATE             122880000   /* Sahara clock rate: 122.88 MHz clock ticks */
#endif




/* Calculate the number of clock ticks in a superframe (360 ms). This is max value that a clock tick value is allowed.
   Note that we had to convert from units of milli-seconds to seconds for the latter part of the equation.
   The values should be 0x2a30000 for the 122.88 MHz Sahara clock and 0x5460000 for the 245.76 MHz Acadia clock */
#define TXHA_MAX_TRO_CLOCK_TICS     (TXHA_CLOCK_RATE / 1000 * SUPER_FRAME_TIME_MS)


/* Number of entries to save into our TRO history. It should cover the max delay time
   the gateway waits before sending a CLT response plus the maximum round trip propagation
   delay.  The max wait time at the gateway is typical 1 sec and the max rtt is
   0.54 secs according to the Timing Sync SSD (H41835). So we need enough frames to
   cover 1.54 seconds which will be 35 frames. We'll round up to 64. */
#define TRO_HISTORY_SIZE    64

#define SFNP_HISTORY_SIZE          8    /* 5 seconds worth of SFNP history */

#define TU_RANGE_RATE_BAD       0x80    /*  aka TUF2_BAD. Ranging Info for current rate is bad and requires new ranging info. */
#define TU_RANGE_RATE_BADALL    0x40    /*  aka TUF2_BADALL. Ranging Info for all rates are bad and requires new range info. */

#define TUINTDELTA_10NS           80    /*  Max Clock drift on Gateway Timing Units, units of 10 ns. */

#define SFNP_TMO_ONE        (2 * 8 + 2)     /*  Allowable Frame times w/o receiving any SFNP (2 superframes) before timeout */
#define SFNP_TMO_TWO        (4 * 8 + 2)     /*  Allowable Frame times w/o receiving two SFNPs in a row (4 superframes) before timeout */
#define SFNP_TMO_THREE      (6 * 8 + 2)     /*  Allowable Frame times w/o receiving three SFNPs in a row (6 superframes) before timeout */

#define REFERENCE_RESET     1
#define REFERENCE_NO_RESET  0

#define TIMING_NETWORK_STATE \
    TIMING_ENUM_D(TIMING_NETWORK_STEADY_STATE, "Steady State") \
    TIMING_ENUM_D(TIMING_NETWORK_INITIAL_BOOTSTRAP, "Initial Bootstrap") \
    TIMING_ENUM_D(TIMING_NETWORK_UNUSED, "Unused") \
    TIMING_ENUM_D(TIMING_NETWORK_POLLING_OUTAGE, "Polling Outage") \
    TIMING_ENUM_D(TIMING_NETWORK_STATE_MAX, "Network State Max (Unused)") \

enum ulTimingNetworkState_e {

    #define TIMING_ENUM_D(enumName, comment) enumName,
    TIMING_NETWORK_STATE
    #undef TIMING_ENUM_D

    NUM_TIMING_NETWORK_STATES,
};

enum ulTimingNetworkActionForNominalHSHChange {
    UPDATE_NOMINAL_HSH_NO_RESET,
    RESET_ALL_TIMING_WITH_REFERENCE_RESET,
    RESET_ALL_TIMING_WITH_REFERENCE_NOT_RESET,
};

enum ulTimingNetworkSectionForNominalHSHChange {
    DEFAULT_NOMINAL_HSH_CHANGE,   /* No Gateway Diversity switchover or RFT Diversity Transition */
    GW_DIVERSITY_SWITCHOVER,
    RFT_DIVERSITY_TRANSITION,
};


#define TIMING_BOOTSTRAP_ALOHA_SHIFT_MODE \
    TIMING_ENUM_D(TIMING_BOOTSTRAP_ALOHA_SHIFT_ALWAYS, "Shift Always") \
    TIMING_ENUM_D(TIMING_BOOTSTRAP_ALOHA_SHIFT_ONCE, "Shift Once") \
    TIMING_ENUM_D(TIMING_BOOTSTRAP_ALOHA_SHIFT_DISABLE, "Shift Disabled") \
    /* end of list */

#define TIMING_ENUM_D(enumName, comment) enumName,

enum ulTimingBootStrapAlohaShiftMode_e {
    TIMING_BOOTSTRAP_ALOHA_SHIFT_MODE
};

#undef TIMING_ENUM_D


#define TIMING_STATUS \
    TIMING_ENUM_D(TIMING_STATUS_GOOD, "Good") \
    TIMING_ENUM_D(TIMING_STATUS_NO_SFNP_RECEIVED, "No SFNPs Received") \
    TIMING_ENUM_D(TIMING_STATUS_BAD_SFNP_RECEIVED, "Bad SFNPs Received") \
    TIMING_ENUM_D(TIMING_STATUS_TIMING_ADJ_NEEDED, "No Tx Until Timing Adjustment Made") \
    TIMING_ENUM_D(TIMING_STATUS_NO_TSA_SELECTED, "No TSA Selected") \
    TIMING_ENUM_D(TIMING_STATUS_RFT_DIVERSITY_TRANSITION_IN_PROGRESS, "RFT Diversity Transition in progress") \
    /* end of list */

#define TIMING_ENUM_D(enumName, comment) enumName,

enum ulTimingStatus_e {
    TIMING_STATUS_UNDEFINED = 0,

    TIMING_STATUS

    TIMING_STATUS_MAX_ENUM
};

#undef TIMING_ENUM_D

#define TIMING_ERROR_REASONS \
    TIMING_ENUM_D(TIMING_ERR_SFNP_DECODE, "SFNP Decode Error") \
    TIMING_ENUM_D(TIMING_ERR_NO_RSR, "No Calculated RSR Available") \
    TIMING_ENUM_D(TIMING_ERR_SELECTED_TSA_INVALID, "Selected TSA Invalid") \
    TIMING_ENUM_D(TIMING_ERR_GERSHWIN_CAV_OUT_OF_SYNC, "Gershwin and Cavalier Clocks Not in Sync") \
    TIMING_ENUM_D(TIMING_ERR_INVALID_SYMRATE, "Invalid Symbol Rate") \
    TIMING_ENUM_D(TIMING_ERR_CAV_CLOCK_ERR, "Error Tracking Cavalier Clock") \
    TIMING_ENUM_D(TIMING_ERR_TIME_ADJ_CALC, "Calc Adjustment Not Reasonable") \
    TIMING_ENUM_D(TIMING_ERR_TIME_ADJ_DELTA_FRAME, "Calc Delta Between Tx Not Reasonable") \
    TIMING_ENUM_D(TIMING_ERR_INTERVAL_SANITY, "SFNP Interval Not 360 ms +/- 5 us") \
    TIMING_ENUM_D(TIMING_ERR_GW_ID_CHANGE_SFNP, "Gateway ID Changed") \
    TIMING_ENUM_D(TIMING_ERR_GW_REF_TS_CHANGE_SFNP, "Gateway Reference TimeStamp Changed") \
    TIMING_ENUM_D(TIMING_ERR_TMO1_LESS_4, "SFNP Arrived Too Soon") \
    TIMING_ENUM_D(TIMING_ERR_NONCONSECUTIVE_SFNP, "Non-consecutive SFNP") \
    TIMING_ENUM_D(TIMING_ERR_FRAME_MISMATCH_SFNP, "Frame Number of SFNP Not Synced") \
    TIMING_ENUM_D(TIMING_ERR_INTERVAL_DEVIATION, "Consecutive SFNP Interval Values Off") \
    TIMING_ENUM_D(TIMING_ERR_GWDELAY_BOUNDS, "Gateway Network Delay is Zero") \
    TIMING_ENUM_D(TIMING_ERR_SATDELAY_TOO_LARGE, "Satellite Delay Too Large") \
    TIMING_ENUM_D(TIMING_ERR_NETDELAY_TOO_LARGE, "GW Network Delay Too Large") \
    TIMING_ENUM_D(TIMING_ERR_STO_TOO_BIG, "Space Timing Offset Value Too Large") \
    TIMING_ENUM_D(TIMING_ERR_NO_FRAME_PULSE_CONTROL_INFO, "No Frame Pulse Control Information Available") \
    TIMING_ENUM_D(TIMING_ERR_QUEUE_DELAY_TOO_LARGE, "The Queue Delay Is Too Large") \

    /* end of list */

#define TIMING_ENUM_D(enumName, comment) enumName,

enum  timingErrorReason_e {
    TIMING_ERR_UNDEFINED = 0,

    TIMING_ERROR_REASONS

    TIMING_ERR_LOSS_MAX_ENUM
};

#undef TIMING_ENUM_D

#define TIMING_BURST_SYNC_STATE \
    TIMING_ENUM_D(0, TIMING_BURST_SYNC_IDLE,                    "(0) Idle") \
    TIMING_ENUM_D(1, TIMING_BURST_SYNC_ADJUSTMENTS_APPLIED,     "(1) Adjustments Applied") \
    TIMING_ENUM_D(2, TIMING_BURST_SYNC_SKIPPING_FIRST_FRAME,    "(2) Skipping 1st frame") \
    TIMING_ENUM_D(3, TIMING_BURST_SYNC_SKIPPING_SECOND_FRAME,   "(3) Skipping 2nd frame") \

#define TIMING_ENUM_D(enumVal, enumName, comment) enumName = enumVal,

enum ulTimingBurstSyncState_e {
    TIMING_BURST_SYNC_STATE
};

#undef TIMING_ENUM_D

#define TIMING_BURST_SYNC_UPDATE_SOURCE \
TIMING_ENUM_D(0, TIMING_BURST_SYNC_UPDATE_TIMING_INIT,          "Timing Module Init") \
TIMING_ENUM_D(1, TIMING_BURST_SYNC_FRAME_FNUM_CHANGE,           "*** SFNP (fnum) ***") \
TIMING_ENUM_D(2, TIMING_BURST_SYNC_FRAME_LARGE_TIMING,          "*** SFNP (timing) ***") \
TIMING_ENUM_D(3, TIMING_BURST_SYNC_UPDATE_MIDFRAME,             "Midframe Processing") \
TIMING_ENUM_D(4, TIMING_BURST_SYNC_UPDATE_FRAME,                "Frame Processing") \

#define TIMING_ENUM_D(enumVal, enumName, comment) enumName = enumVal,

enum ulTimingBurstSyncUpdateSource_e {
    TIMING_BURST_SYNC_UPDATE_SOURCE
};

#undef TIMING_ENUM_D

#define TIMING_VALIDATION_HISTORY 150

#define TIMING_VALIDATION_STATES \
    TIMING_VALIDATION_STATE_D(0,  ULT_NOT_VALID, "Timing Not Validated") \
    TIMING_VALIDATION_STATE_D(1,  ULT_VALIDATION_IN_PROGRESS, "Timing Validation in Progress") \
    TIMING_VALIDATION_STATE_D(2,  ULT_VALIDATED, "Timing Validated") \

#define TIMING_VALIDATION_STATE_D(enumVal, enumName, comment) enumName = enumVal,

typedef enum {
    TIMING_VALIDATION_STATES
} validationState_e;

#undef TIMING_VALIDATION_STATE_D

#define TIMING_VALIDATION_REASONS \
    TIMING_VALIDATION_REASON_D(0,   INVALID_INITIAL_STARTUP,                    "Initial Startup") \
    TIMING_VALIDATION_REASON_D(1,   INVALID_ENTER_INITIAL_BOOTSTRAP,            "Enter Initial Bootstrap") \
    TIMING_VALIDATION_REASON_D(2,   INVALID_BOOTSTRAP_ALOHA_IDLE_COUNT_EXCEEDED,"Bootstrap Aloha - Idle Timeout") \
    TIMING_VALIDATION_REASON_D(3,   INVALID_SMALL_ALOHA_ATTEMPT_EXCEEDED,       "Small Aloha - Nak Timeout") \
    TIMING_VALIDATION_REASON_D(4,   INVALID_BOOTSTRAP_ALOHA_ATTEMPT_EXCEEDED,   "Bootstrap Aloha - Nak Timeout") \
    TIMING_VALIDATION_REASON_D(5,   INVALID_FORCE_RESET_ALL_TIMING_FROM_WEBPAGE,"Forced Timing Reset From Webpage") \
    TIMING_VALIDATION_REASON_D(6,   INVALID_GW_ID_CHANGE,                       "Gateway ID Change") \
    TIMING_VALIDATION_REASON_D(7,   INVALID_GW_REF_CHANGE,                      "Gateway Reference Change") \
    TIMING_VALIDATION_REASON_D(8,   INVALID_VSAT_LOCATION_CHANGE,               "VSAT Location Change") \
    TIMING_VALIDATION_REASON_D(9,   INVALID_TIMING_CHANGED_TO_DELAY,            "System Changed to Delay") \
    TIMING_VALIDATION_REASON_D(10,  INVALID_TIMING_CHANGED_TO_NO_DELAY,         "System Changed to No Delay") \
    TIMING_VALIDATION_REASON_D(11,  INVALID_NOMINAL_HSH_CHANGED,                "Nominal HSH Changed") \
    TIMING_VALIDATION_REASON_D(12,  VALID_LARGE_ALOHA_ACK,                      "Large Aloha Ack") \
    TIMING_VALIDATION_REASON_D(13,  VALID_SMALL_ALOHA_ACK,                      "Small Aloha Ack") \
    TIMING_VALIDATION_REASON_D(14,  VALID_STREAM_ACK,                           "Stream Ack") \
    TIMING_VALIDATION_REASON_D(15,  VALID_TFP_ACK,                              "Timing Feedback ICAP") \
    TIMING_VALIDATION_REASON_D(16,  VALID_STARTUP_WITH_SAVED_REFERENCE,         "Startup with Saved Reference") \
    TIMING_VALIDATION_REASON_D(17,  EXCESSIVE_CLT_FEEDBACKS,                    "Too many CLTs were received in steady state.") \
    TIMING_VALIDATION_REASON_D(18,  INVALID_RADIO_CHANGE,                       "Radio change") \
    TIMING_VALIDATION_REASON_D(19,  AERO_TERM_WAS_PREV_BARRED,                  "Aero: Terminal previously barred") \
    TIMING_VALIDATION_REASON_D(20,  INVALID_FORCE_RESET_ALL_TIMING_SNMP_COMMAND,"Forced Timing Reset via SNMP") \

#define TIMING_VALIDATION_REASON_D(enumVal, enumName, comment) enumName = enumVal,

typedef enum {
    TIMING_VALIDATION_REASONS
} validationReason_e;   /* covers both valid and invalid reasons */

#undef TIMING_VALIDATION_REASON_D

#define TIMING_ADJUSTMENT_REASONS \
  TIMING_ADJUSTMENT_REASON_D(0, FRM_ADJ_UNDEFINED, "UNDEFINED") \
  TIMING_ADJUSTMENT_REASON_D(1, FRM_ADJ_UL_SWITCH_RATE, "UL_SWITCH_RATE") \
  TIMING_ADJUSTMENT_REASON_D(2, FRM_ADJ_SELECT_IG, "SELECT IG") \
  TIMING_ADJUSTMENT_REASON_D(3, FRM_ADJ_NEW_RATE_SET, "NEW RATE SET") \
  TIMING_ADJUSTMENT_REASON_D(4, FRM_ADJ_PARSE_SFNP_NO_TIME_SYNC, "SFNP NO TIME SYNC") \
  TIMING_ADJUSTMENT_REASON_D(5, FRM_ADJ_PARSE_SFNP_ADJUST, "SFNP ADJUST") \
  TIMING_ADJUSTMENT_REASON_D(6, FRM_ADJ_INIT_SFNP_ADJUST, "INITIAL SFNP ADJUST") \
  TIMING_ADJUSTMENT_REASON_D(7, FRM_ADJ_BITRATE_CHANGE_ADJUST, "BITRATE CHANGE") \
  TIMING_ADJUSTMENT_REASON_D(8, FRM_ADJ_TRO_CLT, "TRO CLT") \
  TIMING_ADJUSTMENT_REASON_D(9, FRM_ADJ_TRO_EST, "TRO EST") \
  TIMING_ADJUSTMENT_REASON_D(10, FRM_ADJ_PROCESS_RANGING_ACK, "RANGING ACK") \
  TIMING_ADJUSTMENT_REASON_D(11, FRM_ADJ_PROCESS_LARGE_ALOHA_ACK, "LARGE ALOHA ACK") \
  TIMING_ADJUSTMENT_REASON_D(12, FRM_ADJ_PROCESS_SMALL_ALOHA_ACK, "SMALL ALOHA ACK") \
  TIMING_ADJUSTMENT_REASON_D(13, FRM_ADJ_HANDLE_SMALL_APER_FEEDBACK_ICAP, "SMALL APERTURE FEEDBACK ICAP") \
  TIMING_ADJUSTMENT_REASON_D(14, FRM_ADJ_TIMING_FEEDBACK_ICAP, "TIMING FEEDBACK ICAP") \
  TIMING_ADJUSTMENT_REASON_D(15, FRM_ADJ_START_BOOTSTRAP_ALOHA, "SHIFT FOR BOOTSTRAP ALOHA") \
  TIMING_ADJUSTMENT_REASON_D(16, FRM_ADJ_STOP_BOOTSTRAP_ALOHA, "SHIFT AFTER BOOTSTRAP ALOHA ACK") \
  TIMING_ADJUSTMENT_REASON_D(17, FRM_ADJ_MANUAL_TIME_SHIFT, "MANUAL TIMING SHIFT") \
  TIMING_ADJUSTMENT_REASON_D(18, FRM_ADJ_NETWORK_TO_STEADY, "NETWORK SWITCH TO STEADY") \
  TIMING_ADJUSTMENT_REASON_D(19, FRM_ADJ_CLEAR_CLT, "CLEAR CLT FOR BOOTSTRAP ALOHA") \

#define TIMING_ADJUSTMENT_REASON_D(enumVal, enumName, comment) enumName = enumVal,

typedef enum {
    TIMING_ADJUSTMENT_REASONS
} frmAdjReason_e;

#undef TIMING_ADJUSTMENT_REASON_D

#define FRAME_NUMBER_ADJUSTMENT_REASONS \
TIMING_ENUM_D(0, FRAME_NUMBER_NO_PENDING_ADJUSTMENT,        "NO Pending Adjustment") \
TIMING_ENUM_D(1, FRAME_NUMBER_ADJUSTMENT_SFNP,              "SFNP Triggered") \
TIMING_ENUM_D(2, FRAME_NUMBER_ADJUSTMENT_UNKNOWN_ALOHA,     "Unknown Aloha Ack") \
TIMING_ENUM_D(3, FRAME_NUMBER_ADJUSTMENT_SMALL_ALOHA,       "Small Aloha Ack") \
TIMING_ENUM_D(4, FRAME_NUMBER_ADJUSTMENT_BOOTSTRAP_ALOHA,   "Bootstrap Aloha Ack") \
TIMING_ENUM_D(5, FRAME_NUMBER_ADJUSTMENT_BAP,               "BAP") \
TIMING_ENUM_D(6, FRAME_NUMBER_ADJUSTMENT_MANUAL,            "Manual Shift (usrcmd)") \
TIMING_ENUM_D(7, FRAME_NUMBER_ADJUSTMENT_ALOHA_ACK_ERROR,   "SFNP Triggered by Aloha Ack Error") \

#define TIMING_ENUM_D(enumVal, enumName, comment) enumName = enumVal,
enum frameNumberAdj_e {
    FRAME_NUMBER_ADJUSTMENT_REASONS
};
#undef TIMING_ENUM_D

/*  Frame Pulse Control State */
#define FPC_HISTORY_SIZE    4

#define FRAME_PULSE_CONTROL_STATES \
    FRAME_PULSE_CONTROL_D(FPC_UNINITIALIZED,            "Uninitialized") \
    FRAME_PULSE_CONTROL_D(FPC_FUTURE_FRAME_WIDTH_SET,   "Frame Width Set") \
    FRAME_PULSE_CONTROL_D(FPC_FRAME_TIMESTAMP_SET,      "Timestamp Set") \

#define FRAME_PULSE_CONTROL_D(enumName, comment) enumName,

enum fpcStateStatus_e {
    FRAME_PULSE_CONTROL_STATES
};

#undef FRAME_PULSE_CONTROL_D

#define TIMING_SF_MARKER_ADJUST_STATUS \
    TIMING_ENUM_D(0, SF_MARKER_ADJUST_NO_ERR,       "No Error") \
    TIMING_ENUM_D(1, SF_MARKER_NOT_INITIALIZED,     "Not initialized to 180ms") \
    TIMING_ENUM_D(2, SF_MARKER_EXCEED_MIN_BOUND,    "EXCEEDS min bound") \
    TIMING_ENUM_D(3, SF_MARKER_EXCEED_MAX_BOUND,    "EXCEEDS max bound") \
    TIMING_ENUM_D(4, SF_MARKER_ADJ_EXCEED_180MS,    "Adj EXCEEDS 180ms") \

#define TIMING_ENUM_D(enumVal, enumName, comment) enumName = enumVal,

enum ulTimingSFMarkerAdjStatus_e {
    TIMING_SF_MARKER_ADJUST_STATUS
};

#undef TIMING_ENUM_D

#define NORTH_HEMISPHERE 0
#define SOUTH_HEMISPHERE 1
#define EAST_HEMISPHERE 0
#define WEST_HEMISPHERE 1

/**************************************************************/
/**************************************************************/
/*                    TIMING MODULE ENDS                      */
/**************************************************************/
/**************************************************************/

/**************************************************************/
/**************************************************************/
/*                    INROUTE GROUP MODULE START              */
/**************************************************************/
/**************************************************************/

#define GROUP_RESELECT_FRAMES (5 * TX_FRAMES_PER_MIN)

#define GROUP_SELECT_METRIC_TYPE \
GROUP_SELECT_METRIC_TYPE_D(0, GROUP_SELECT_METRIC_STREAM, "Stream") \
GROUP_SELECT_METRIC_TYPE_D(1, GROUP_SELECT_METRIC_RANGING, "Ranging") \
GROUP_SELECT_METRIC_TYPE_D(2, GROUP_SELECT_METRIC_BOOTSTRAP_ALOHA, "Bootstrap Aloha") \
GROUP_SELECT_METRIC_TYPE_D(3, GROUP_SELECT_METRIC_ALOHA, "Aloha") \


enum groupSelectMetricType_e {
    GROUP_SELECT_METRIC_INVALID_TYPE = -1,
    #define GROUP_SELECT_METRIC_TYPE_D(enumVal, enumName, comment) enumName = enumVal,
    GROUP_SELECT_METRIC_TYPE
#undef GROUP_SELECT_METRIC_TYPE_D
    GROUP_SELECT_NUM_METRIC_TYPES,
};

#define _IS_GROUP_SELECT_METRIC_TYPE_VALID(metricType)  (metricType >= 0 && metricType < GROUP_SELECT_NUM_METRIC_TYPES)

#define GROUP_SELECT_REASONS \
GROUP_SELECT_REASONS_D(0, GROUP_SELECT_REASON_FORCE, "Force") \
GROUP_SELECT_REASONS_D(1, GROUP_SELECT_REASON_EXTERN_REQUEST, "External Request") \
GROUP_SELECT_REASONS_D(2, GROUP_SELECT_REASON_STATUS_NOT_SELECTED, "Group Status = Not Selected") \
GROUP_SELECT_REASONS_D(3, GROUP_SELECT_REASON_IGID_0, "IGID = 0") \
GROUP_SELECT_REASONS_D(4, GROUP_SELECT_REASON_PERIODIC, "Periodic") \
GROUP_SELECT_REASONS_D(5, GROUP_SELECT_REASON_CLEAR_GROUP, "Clear Group") \
GROUP_SELECT_REASONS_D(6, GROUP_SELECT_REASON_RATE_CHANGE, "Rate Change") \
GROUP_SELECT_REASONS_D(7, GROUP_SELECT_REASON_RANGING_REQUEST, "Ranging Request") \
GROUP_SELECT_REASONS_D(8, GROUP_SELECT_REASON_INROUTE_SET, "Inroute Set") \
GROUP_SELECT_REASONS_D(9, GROUP_SELECT_REASON_PERIODIC_DED, "Switched to Dedicated") \
GROUP_SELECT_REASONS_D(10, GROUP_SELECT_REASON_GROUP_UNAVAILABLE, "Group Unavailable Previously") \

#define GROUP_SELECT_REASONS_D(enumVal, enumName, comment) enumName,

enum groupSelectReason_e {
    GROUP_SELECT_REASONS
};


#undef GROUP_SELECT_REASONS_D

#define INROUTE_SET_SELECT_REASONS \
INROUTE_SET_SELECT_REASONS_D(0, INROUTE_SET_SELECT_REASON_FORCE,    "Force") \
INROUTE_SET_SELECT_REASONS_D(1, INROUTE_SET_SELECT_REASON_PERIODIC,     "Periodic") \
INROUTE_SET_SELECT_REASONS_D(2, INROUTE_SET_SELECT_REASON_RANGING_SCMA, "Ranging SCMA") \
INROUTE_SET_SELECT_REASONS_D(3, INROUTE_SET_SELECT_REASON_INROUTE_SET_UNAVAILABLE, "Set Unavailable Previously") \
INROUTE_SET_SELECT_REASONS_D(4, INROUTE_SET_SELECT_REASON_DESIRED_GROUP,           "Has Desired Group") \
INROUTE_SET_SELECT_REASONS_D(5, INROUTE_SET_SELECT_REASON_DESIRED_SYMCOD,          "Has Desired Symcod") \

#define INROUTE_SET_SELECT_REASONS_D(enumVal, enumName, comment) enumName,

enum inrouteSetSelectReason_e {
    INROUTE_SET_SELECT_REASONS
};

#undef INROUTE_SET_SELECT_REASONS_D



/**************************************************************/
/**************************************************************/
/*                    INROUTE GROUP MODULE ENDS               */
/**************************************************************/
/**************************************************************/


/**************************************************************/
/**************************************************************/
/*                    BURST CONTROL START                     */
/**************************************************************/
/**************************************************************/
#define IPFP_TABLE_SIZE     64
#define RANGE_NO_FEEDBACK   -127

#define BURST_CONTROL_APERTURE_TYPES \
BURST_CONTROL_APERTURE_TYPE_D(0,  INVALID_APERTURE, "INVALID") \
BURST_CONTROL_APERTURE_TYPE_D(1,  SMALL_APERTURE, "SMALL") \
BURST_CONTROL_APERTURE_TYPE_D(2,  LARGE_APERTURE, "LARGE") \
BURST_CONTROL_APERTURE_TYPE_D(3,  REGULAR_APERTURE, "REGULAR")


typedef enum {
    MIN_APERTURE_INVALID = 0,
#define BURST_CONTROL_APERTURE_TYPE_D(enumVal, enumName, comment) enumName = enumVal,
    BURST_CONTROL_APERTURE_TYPES
#undef BURST_CONTROL_APERTURE_TYPE_D
    MAX_APERTURE_INVALID,
} apertureType_e;

#define IS_VALID_APERTURE(x)   (x<MAX_APERTURE_INVALID && x>MIN_APERTURE_INVALID)

enum ipfpBurstType_e{
    IPFP_ALOHA_BURST  = 0,
    IPFP_STREAM_BURST = 1,
    IPFP_NACKED_BURST
};

/* Burst Debug Over LAN */
#define BURST_BAD_BAP_STATE_THRESHOLD   22

#define ENUM_D(enumName, comment) enumName,

#define BURST_DBG_OVER_LAN_CFG \
ENUM_D(BURST_DBG_DISABLED, "Disabled") \
ENUM_D(BURST_DBG_ENABLED_ALL_BURSTS, "Enabled - All Bursts") \
ENUM_D(BURST_DBG_ENABLED_ALOHA_BURSTS, "Enabled - Aloha Bursts") \
ENUM_D(BURST_DBG_ENABLED_NAK_BURSTS, "Enabled - NAK'd Bursts") \

enum burstDbgCfg_e {
    BURST_DBG_OVER_LAN_CFG
};

#define BURST_DBG_OVER_LAN_LOG_STAGE_CFG \
ENUM_D(BURST_DBG_LOG_ACK, "Burst Debug Logged During ACK") \
ENUM_D(BURST_DBG_LOG_SCHED, "Burst Debug Logged During Scheduling") \

enum burstDbgLogStageCfg_e {
    BURST_DBG_OVER_LAN_LOG_STAGE_CFG
};

#define BURST_DBG_OVER_LAN_OUTPUT_CFG \
ENUM_D(BURST_DBG_OUTPUT_LAN, "Burst Debug Sent to LAN")\
ENUM_D(BURST_DBG_OUTPUT_PCAP, "Burst Debug Sent to PCAP File")\

enum burstDbgOutputCfg_e {
    BURST_DBG_OVER_LAN_OUTPUT_CFG
};

#undef ENUM_D


#define CONTEXT_INFO_REASONS \
CI_REASON_ENUM_D(CI_DATA_ROUTER, "Data Router") \
CI_REASON_ENUM_D(CI_MGMT_ROUTER, "Mgmt Router") \
CI_REASON_ENUM_D(CI_IG_CHANGE, "IGID Change") \
CI_REASON_ENUM_D(CI_IS_CHANGE, "ISID Change") \
CI_REASON_ENUM_D(CI_UPLOAD_THROUGHPUT, "Upload Rate") \
CI_REASON_ENUM_D(CI_CAPABILITIES_CHANGE, "Capabilities") \
CI_REASON_ENUM_D(CI_ALOHA_INDICATED, "Aloha ACK Requested") \
CI_REASON_ENUM_D(CI_SGBA_INDICATED, "SGBA ACK Requested") \
CI_REASON_ENUM_D(CI_FORCE, "Forced") \
CI_REASON_ENUM_D(CI_VNO_CHANGE, "VNO Capable Change")\
CI_REASON_ENUM_D(CI_UPLINK_SPI_CHANGE, "Uplink SPI") \
CI_REASON_ENUM_D(CI_SAI_CHANGED, "New SAI") \
CI_REASON_ENUM_D(CI_RED_IPGW_CHANGE, "ASSOC IPGW RED CHANGE") \

/* end of list */

#define CI_REASON_ENUM_D(enumName, comment) enumName,

enum contextInfoReason_e {
    CONTEXT_INFO_REASONS_UNDEFINED = 0,

    CONTEXT_INFO_REASONS

    CONTEXT_INFO_REASONS_MAX_ENUM
};

#undef CI_REASON_ENUM_D

#define BAP_OFFSET_MIN  (-16)
#define BAP_OFFSET_MAX  (16)
#define BAP_OFFSETS (BAP_OFFSET_MAX - BAP_OFFSET_MIN + 1)

/* BurstCtrl Histograms */
#define ULBURSTCTRL_FRAMEFILL_HISTO_STEP_SIZE       1000
#define ULBURSTCTRL_FRAMEFILL_HISTO_NUMBER_STEPS    30
#define ULBURSTCTRL_BURSTFILL_HISTO_STEP_SIZE       20
#define ULBURSTCTRL_BURSTFILL_HISTO_NUMBER_STEPS    60
#define ULBURSTCTRL_IBEFILL_HISTO_STEP_SIZE         20
#define ULBURSTCTRL_IBEFILL_HISTO_NUMBER_STEPS      60
#define ULBURSTCTRL_TXCTRLFILL_HISTO_STEP_SIZE      20
#define ULBURSTCTRL_TXCTRLFILL_HISTO_NUMBER_STEPS   60
#define ULBURSTCTRL_MAX_FUTURE_FRAMES               10   /* TODO: this will prevent TVOA from working */

/**************************************************************/
/**************************************************************/
/*                    BURST CONTROL END                       */
/**************************************************************/
/**************************************************************/

/**************************************************************/
/**************************************************************/
/*                    IBE MODULE START                       */
/**************************************************************/
/**************************************************************/

#define BASE_BURST_HEADER_LEN 1      /*Base burst header length is 1 byte*/
#define MAC_PROTOCOL_VERSION  1      /*Jupiter inroute encapsulation protocol*/

#define IBE_HEADER_BACKLOG_BIT    0x08  /*Backlog bit position in the IBE header*/
#define IBE_HEADER_ADAPTATION_BIT 0x04  /*Adaptation bit position in the IBE header*/
#define IBE_HEADER_ENCRYPTION_BIT 0x02  /*Encryption bit position in the IBE header*/
#define IBE_HEADER_BACKLOG_FORMAT_BIT 0x01  /*Backlog format bit position in the IBE header*/

#define BACKLOG_MAX_SEND_INTERVAL     4

#define ADAPTATION_MSG_LIST \
    /* ADAPTATION_TYPE_D(enumVal,       enumName,             order, comment) */\
    ADAPTATION_TYPE_D(     1,       CONTEXT_INFORMATION,        1,   "Context Information") \
    ADAPTATION_TYPE_D(     2,       MODCOD_REQUEST,             7,   "Modcod Request") \
    ADAPTATION_TYPE_D(     3,       RANGING_REQUEST,            4,   "Ranging Request") \
    ADAPTATION_TYPE_D(     4,       RANGING_STATUS_UPDATE,      8,   "Ranging Status Update") \
    ADAPTATION_TYPE_D(     5,       RANGING_POWER_RESULTS,      9,   "Ranging Power Results") \
    ADAPTATION_TYPE_D(     6,       RANGING_TIMING_RESULTS,    10,   "Ranging Timing Results") \
    ADAPTATION_TYPE_D(     7,       TIMING_POLL_RESPONSE,      11,   "Timing Poll Response") \
    ADAPTATION_TYPE_D(     8,       SPECIFIC_INROUTE_REQ,       3,   "Specific Inroute Request") \
    ADAPTATION_TYPE_D(     9,       VNO_STATUS_UPDATE,         12,   "VNO Status Update") \
    ADAPTATION_TYPE_D(     10,      TRANSMIT_POWER,             6,   "Transmit Power") \
    ADAPTATION_TYPE_D(     11,      MOBILITY_STATUS,            2,   "Mobility Status") \
    ADAPTATION_TYPE_D(     12,      MODCOD_REQUEST_MULTICAST,  13,   "Modcod Request With Multicast") \
    ADAPTATION_TYPE_D(     13,      ENHANCED_RANGING_REQUEST,   5,   "Enhanced Ranging Request") \

typedef enum {
    #define ADAPTATION_TYPE_D(enumVal, enumName, order, comment) enumName = enumVal,

    INVALID_ADAPTATION = 0,
    ADAPTATION_MSG_LIST
    ADAP_MSG_MAX

    #undef ADAPTATION_TYPE_D
} adapType_e;   /* covers both valid and invalid  */

#define ADAP_MSG_TYPE_HIGHEST_PRECEDENCE  (CONTEXT_INFORMATION)
#define IS_VALID_ADAPTATION_MSG_TYPE(msgType) (msgType > INVALID_ADAPTATION && msgType < ADAP_MSG_MAX)

typedef enum {
    INVALID_ADAPTATION_ORDER = 0,

    /* e.g., CONTEXT_INFORMATION_ORDER = 1, MODCOD_REQUEST_ORDER = 7, ... */
    #define ADAPTATION_TYPE_D(enumVal, enumName, order, comment) enumName##_ORDER = order,
    ADAPTATION_MSG_LIST
    #undef ADAPTATION_TYPE_D
}adapMsgOrder_e;   /* covers both valid and invalid  */

#define ADAP_MSG_HIGHEST_PRECEDENCE_ORDER  (CONTEXT_INFORMATION_ORDER)
#define ADAP_MSG_LOWEST_PRECEDENCE_ORDER   (MODCOD_REQUEST_MULTICAST_ORDER)
#define ADAP_MSG_MAX_ORDER                 (ADAP_MSG_LOWEST_PRECEDENCE_ORDER + 1)
#define ADAP_MSG_ORDER                     (ADAP_MSG_MAX_ORDER)
#define IS_VALID_ADAPTATION_MSG_ORDER(msgOrder) (msgOrder > INVALID_ADAPTATION_ORDER && msgOrder < ADAP_MSG_MAX_ORDER)
#define ADAP_CONTEXT_INFO_RE_TX_REASON_BITMASK (1<<31)  /* bit 31 indicating re-tx*/

typedef enum{
    CONTEXT_INFO = 0,   /*Context Information change causes Modcod Request Adaptation sending*/
    SNR_CHANGE,         /*Es/No change causes Modcod Request Adaptation sending*/
    PERIODIC_SEND,      /*Periodically send Modcod Request Adaptation when DMM is enable and Multicast group is configured*/
    MC_CFG_CHANGE,       /*Multicast configuration change causes Modcod Request Adaptation sending*/
    MCOD_ADAP_TYPE_MAX,
    INVALID_TYPE = MCOD_ADAP_TYPE_MAX,
}modcodAdapReason_e;


/*The state that the adaptation message could be in*/
typedef enum{
    ADAP_INV = -1,      /*Invalid adaptation state*/
    ADAP_IDLE = 0,
    ADAP_NEEDED,        /*Adaptation needs to be sent*/
    ADAP_UNSENT,        /*Adaptation was needed but unable to send*/
    ADAP_SENT,          /*Adaptation sent */
    ADAP_ACK,           /*Acknowledgement received for the adaptation*/
    ADAP_NACK,          /*Adaptation msg was not ack'd by DNCC*/
    ADAP_CANCEL         /*Adaptation msg was canceled. Did not receive an ACK/NAK*/
}adapState_e;

/*The encryption status */
typedef enum{
    ENCRYPTION_INV      = -1,      /*Invalid encryption value */
    ENCRYPTION_DISABLED = 0,
    ENCRYPTION_ENABLED  = 1
}ibeEncryptStatus_e;

#ifdef ULIBE_STRINGS
char *adapTypeStr[ADAP_MSG_MAX] =
{
    (char *) "INVALID",

    #define ADAPTATION_TYPE_D(enumVal, enumName, order, comment) comment,
    ADAPTATION_MSG_LIST
    #undef ADAPTATION_TYPE_D
};

char *adapStateStr[] =
{
    (char *) "IDLE",
    (char *) "NEEDED",
    (char *) "UNSENT",
    (char *) "SENT",
    (char *) "ACK'D",
    (char *) "NACK'D",
    (char *) "TIMEOUT"
};

char *modcodAdapReasonStr[MCOD_ADAP_TYPE_MAX] =
{
    (char *) "CONTEXT INFO CHANGE",
    (char *) "SNR CHANGE",
    (char *) "MULTICAST MODCOD PERIODIC SENDING",
    (char *) "MULTICAST CONFIGURATION CHANGE"
};
#endif

/**************************************************************/
/**************************************************************/
/*                    IBE MODULE END                          */
/**************************************************************/
/**************************************************************/

/**************************************************************/
/**************************************************************/
/*                    ALOHA MODULE START                     */
/**************************************************************/
/**************************************************************/

typedef enum {
    /*** IDLE and IN_PROGRESS are the desired 'good' states ***/
    ALOHA_IDLE                  = 0,  /* No Aloha activity */
    ALOHA_IN_PROGRESS           = 1,  /* Attempting Aloha, still less than Aloha retries */
    ALOHA_TRY_DIFF_IG           = 2,  /* Error Cond: Trying a different inroute group */
    ALOHA_TRY_INGRESS           = 3,  /* Error Cond: Trying lower coding and/or symbol rate */
    ALOHA_TRY_LARGE_APERTURE    = 4,  /* Error Cond: Trying to use large aperture Aloha */
    ALOHA_CANCEL                = 5,  /* Aloha has been canceled */
    ALOHA_ACKD                  = 6,  /* Aloha has been ACKnowledged */
    ALOHA_MAX_STATE
} alohaState_e;

/**************************************************************/
/**************************************************************/
/*                    ALOHA MODULE ENDS                       */
/**************************************************************/
/**************************************************************/

/**************************************************************/
/**************************************************************/
/*                    RATE MODULE STARTS                      */
/**************************************************************/
/**************************************************************/
#define UPLINK_TRAJECTORY_TABLE_CSV_FILE_PATH "/var/run"
#define UPLINK_TRAJECTORY_TABLE_CSV_FILE "uplink_trajectory_table.csv"

#define AIS_MAX_TRAJECTORY_ENTRY (NUM_SYMCODS) /* same as number of valid symcods */
#define TDM_ACM_MAX_TRAJECTORY_ENTRY 98
#define MAX_HYSTERESIS_Q_ENTRY   35
#define MAX_HYSTERESIS_Q_TYPES    2     /* indicates if we are tracking for the current symcod or the max (see aisHysteresisType_e) */

#define ACTION_MORE_ROBUST_RATE 0
#define ACTION_LESS_ROBUST_RATE 1
#define NO_ACTION               2
#define ACTION_ALOHA_INGRESS    3

#define AIS_STREAM_ACK  0
#define AIS_STREAM_NAK  1

#define AIS_ALOHA_TIMEOUT       15
#define MAX_IC_GROUPS           8

#define AIS_VERSION_NUMBER      2

/*Enumeration provies the reason for rate selection */
#define RATE_SELECT \
    RATE_SELECT_ENUM_D(RATE_SELECT_TFIX,                    "TFIX RATE")                /* 1 */\
    RATE_SELECT_ENUM_D(RATE_SELECT_SIR,                     "SIR RATE")                 /* 2 */\
    RATE_SELECT_ENUM_D(RATE_SELECT_FORCED_RATE,             "FORCED RATE")              /* 3 */\
    RATE_SELECT_ENUM_D(RATE_SELECT_RANGING,                 "RANGING RATE")             /* 4 */\
    RATE_SELECT_ENUM_D(RATE_SELECT_AIS,                     "AIS RATE")                 /* 5 */\
    RATE_SELECT_ENUM_D(RATE_SELECT_ROBUST,                  "ROBUST RATE")              /* 6 */\
    RATE_SELECT_ENUM_D(RATE_SELECT_FORCED_GROUP,            "FORCED GROUP")             /* 7 */\
    RATE_SELECT_ENUM_D(RATE_SELECT_BOOTSTRAP_ALOHA_NEEDED,  "BOOTSTRAP ALOHA NEEDED")   /* 8 */\
    RATE_SELECT_ENUM_D(RATE_SELECT_ERROR_RECOVERY,          "RECOVER FROM ANY ERROR")   /* 9 */\

#define RATE_SELECT_ENUM_D(enumName, comment) enumName,

typedef enum {
    ATTEN_QPSK      = 0,             //In 10th of dB
    ATTEN_OQPSK     = 0,
    ATTEN_8PSK      = 0,
    ATTEN_16APSK    = 15,
    ATTEN_CPM       = 0
}defaultRadioMaxPowerBackoff_e;

enum rateSelectMode_e{
    INVALID_SELECT_MODE       = 0,
    RATE_SELECT
    RATE_SELECT_MAX
};
#undef RATE_SELECT_ENUM_D

typedef enum {
    NO_RATE_CHANGE      = 0,
    RATE_CHANGED        = 1,
}rateChange_e;

typedef enum {
    RATE_AVAIL                     = 0,

    RATE_UNAVAIL                   = 1,
    RATE_UNAVAIL_FAIL_RANGING      = 2,
    RATE_UNAVAIL_NOSUCCESS_RANGING = 3,
    RATE_UNAVAIL_NO_GROUP          = 4,
}rateAvail_e;

typedef enum {
    FORCE_RATE_DISABLED = 0,
    FORCE_RATE_ENABLED  = 1,
}forceRate_e;

typedef enum {
    FORCE_SYMCOD_DISABLED = 0,
    FORCE_SYMCOD_ENABLED  = 1,
}forceSymcod_e;

typedef enum {
    AIS_NOT_ENABLED = 0,
    AIS_ENABLED     = 1,
}aisRate_e;

typedef enum {
    INROUTE_COMP_DISABLED = 0,
    INROUTE_COMP_ENABLED  = 1,
}inrouteComp_e;

typedef enum {
    CURR_AIS_SYMCOD      = 0,
    MAX_AIS_SYMCOD       = 1,
}aisHysteresisType_e;


/*Enumeration provies the reason why the terminal is ranging*/
#define AIS_TYPE \
    AIS_TYPE_ENUM_D(AIS_TYPE_ALOHA_INGRESS,         "Aloha Ingress")      /* 1 */\
    AIS_TYPE_ENUM_D(AIS_TYPE_PC_INGRESS,            "Stream Ingress")         /* 2 */\
    AIS_TYPE_ENUM_D(AIS_TYPE_EGRESS,                "Stream Egress")             /* 3 */\

#define AIS_TYPE_ENUM_D(enumName, comment) enumName,

enum aisType_e{
    AIS_TYPE_NONE       = 0,
    AIS_TYPE
    AIS_TYPE_MAX
};
#undef AIS_TYPE_ENUM_D

#define AIS_CLPC_FILTER_RESET_REASONS \
    ENUM_D(AIS_CLPC_FILTER_RESET_NONE,      "Unknown reason for AIS/CLPC Filter Reset") \
    ENUM_D(AIS_CLPC_FILTER_RESET_LUI,       "AIS/CLPC Filter Reset through LUI") \
    ENUM_D(AIS_CLPC_FILTER_RESET_RANGING,   "AIS/CLPC Filter Reset due to Ranging") \
    ENUM_D(AIS_CLPC_FILTER_RESET_ALOHA_NAK, "AIS/CLPC Filter Reset due to excessive Aloha NAKs") \
    ENUM_D(AIS_CLPC_FILTER_RESET_STF_CONFIDENCE, "AIS/CLPC Filter Reset due to STF no confidence") \

#define ENUM_D(enumName, comment) enumName,

enum aisClpcFilterResetReason_e {
    AIS_CLPC_FILTER_RESET_REASONS
};

#undef ENUM_D

/**************************************************************/
/**************************************************************/
/*                    RATE MODULE ENDS                       */
/**************************************************************/
/**************************************************************/

/**************************************************************/
/**************************************************************/
/*                   RANGING MODULE START                     */
/**************************************************************/
/**************************************************************/


/*  Constants  */
#define RANGING_TXT_FILE               "/fl0/ranging.txt"
#define POWER_FEEDBACK_FILE            "/fl0/rangingFeedback.csv"
#define POWER_FEEDBACK_LOG_INFO        "/fl0/rangingFeedbackLogInfo"
#define POWER_FEEDBACK_BUFFER_FILE     "/fl0/rangingFeedback.csv.buf"
#define RANGING_DATA_FILE              "/fl0/ranging.dat"
#define ESNOFILE                       "/fl0/esnoRangingAnalysis.txt"
#define FEEDBACK_FILE_SIZE              1000000      /* Max file size for csv file */
#ifdef THEBE_PLATFORM
#define MAX_RANGE_PROCESS_HISTORY       60      /*Number of ranging process records that will be saved*/
#else
#define MAX_RANGE_PROCESS_HISTORY       12      /*Number of ranging process records that will be saved*/
#endif
#define MAX_RANGE_AVG_PERIOD            64     /*Max num of frames the terminal should average over
                                                before making power adjustments*/
#define MAX_RANGE_SESSIONS              20     /*Number of session attempts to reach the target*/
#define MAX_NO_STREAM_ACK_INTVL         25     /*percent of bursts in a session that can be nack'd*/
#define TARGET_THRESHOLD_VARIANCE_10TH_DB     2  /* 0.2 dB */
#define RANGING_TIMEOUT_BUFFER          20
#define RANGE_ALOHA_TMO_FACTOR          2
#define DEFAULT_RANGING_BACKOFF         220
#define DEFAULT_MAX_RANGING_BACKOFF     1920000
#define DEFAULT_RANGING_HOLES_CHECK     5
#define MOBILE_RANGING_HOLES_CHECK      3  /* Minutes */

// Constants for logs
#define RANG_CSV_DIR_ON_RDISK           "/fl0"                      /* Directory where csv file resides */
#define RANG_ZIP_DIR_ON_RDISK           "/fl0/logs"                 /* Directory where zip files reside*/
#define MAX_RANG_FEEDBACK_ZIP_FILE_NUM  10                          /* Max num of zip files to be archived */
#define MAX_RANG_LOG_NAME_SIZE          50                          /* Max size of file name for logs */
#define MAX_RANG_LOG_PATH_SIZE          100                         /* Max size of path for log files */
#define LAST_RANG_ZIP_FILE_ID           "/fl0/logs/lastRangingFid"  /*Path to store last file id of zipfiles */

enum threshold_type_e{
    MINIMUM_THRESHOLD = 0,                          /* Interested in Minimum threshold */
    TARGET_THRESHOLD                                /* Interested in Target threshold */
};

/*Enumeration:  reason why the terminal is ranging*/
#define RANGE_REASONS \
    RANGE_REASON_ENUM_D(INITIAL_INSTALL,             "INITIAL INSTALL")             /* 1 */\
    RANGE_REASON_ENUM_D(ESTIMATION_ADJUST,           "ESTIMATION_ADJUSTMENT")       /* 2 */\
    RANGE_REASON_ENUM_D(BASELINE_POWER_SET_NEW_RATE, "BASELINE RATE SET")           /* 3 */\
    RANGE_REASON_ENUM_D(TRANSMIT_RADIO_CHANGE,       "TX RADIO CHANGE")             /* 4 */\
    RANGE_REASON_ENUM_D(NOC_INITIATED,               "NOC INITIATED")               /* 5 */\
    RANGE_REASON_ENUM_D(TERMINAL_INITIATED,          "TERMINAL INITIATED")          /* 6 */\
    RANGE_REASON_ENUM_D(STF_NO_CONFIDENCE,           "STF NO CONFIDENCE")           /* 7 */\
    RANGE_REASON_ENUM_D(OUTROUTE_PARM_CHANGE,        "OUTROUTE PARAMETER CHANGE")   /* 8 */\
    RANGE_REASON_ENUM_D(SPECIFIC_INROUTE_TEST,       "SPECIFIC INROUTE TEST")       /* 9 */\
    RANGE_REASON_ENUM_D(AUTO_RERANGE,                "AUTO RE-RANGE")               /* 10 */\
    RANGE_REASON_ENUM_D(IC_TABLE_VERSION_CHANGE,     "IC TABLE VERSION CHANGE")     /* 11 */\
    RANGE_REASON_ENUM_D(RANGING_HOLES_FIX,           "RANGING HOLES FIX")           /* 12 */\
    RANGE_REASON_ENUM_D(FAILED_UNSUC_AUTO_RERANGING, "FAILED UNSUC AUTO_RERANGING") /* 13 */\
    RANGE_REASON_ENUM_D(POWER_CURVE_FIX,             "POWER CURVE FIX")             /* 14 */\
    RANGE_REASON_ENUM_D(INROUTE_COMP_RANGE,          "INROUTE_COMP_RANGE")          /* 15 */\
    RANGE_REASON_ENUM_D(AERO_BEAM_SWITCH,            "AERO BEAM SWITCH")            /* 16 */\
    RANGE_REASON_ENUM_D(AERO_HOLES_FIX,              "AERO_HOLES_FIX")              /* 17 */\

    /* Note, ranging reason range[1:15] according to CAI ICD (H45529) for ranging request adaptation message
       the ranging reason is a 4-bit field, so at most 15;  <=0 and >= 16 is invalid, and the ranging request will be ignored by gateway */

#define RANGE_REASON_ENUM_D(enumName, comment) enumName,

enum rangeReason_e{
    INVALID_RANGE_REASON       = 0,
    RANGE_REASONS
    RANGE_REASON_MAX
};
#undef RANGE_REASON_ENUM_D

#define AUTO_RERANGE_REASONS \
    AUTO_RERANGE_REASON_ENUM_D(NO_AUTO_RERANGING,     "NO AUTO RE RANGING")         /* 0 */\
    AUTO_RERANGE_REASON_ENUM_D(STF_CHANGE,             "STF CHANGE")                /* 1 */\
    AUTO_RERANGE_REASON_ENUM_D(OUTROUTE_CHANGE,        "OUTROUTE CHANGE")           /* 2 */\
    AUTO_RERANGE_REASON_ENUM_D(STF_OUTROUTE_CHANGE,    "STF AND OUTROUTE CHANGE")   /* 3 */\

#define AUTO_RERANGE_REASON_ENUM_D(enumName, comment) enumName,

enum autoReRangeReason_e{
    AUTO_RERANGE_REASONS
};
#undef AUTO_RERANGE_REASON_ENUM_D


/* Relationship:  rangeOutcome_e --> rangeRateStatus_e --> rateAvail_e -->  aisTrajectoryTbl_t:rateAvail
    RANGE_RESULT_D(rangeOutcome_e,                    Comment,                                  rangeRateStatus_e,      rateAvail_e,  aisTrajectoryTbl_t:rateAvail)
*/
#define RANGING_RESULT_TABLE \
    RANGE_RESULT_D(RANGE_NOT_TRIED,                   "NOT TRIED - RANGE NOT TRIED",            RANGE_RATE_NOT_TRIED,   RATE_UNAVAIL,                   0)\
    RANGE_RESULT_D(RANGE_SUCCESS_MET_TARGET_ESNO,     "SUCCESS - MET TARGET ESNO",              RANGE_RATE_SUCCESS,     RATE_AVAIL,                     1)\
    RANGE_RESULT_D(RANGE_SUCCESS_MET_MIN_ESNO,        "SUCCESS - MET MIN ESNO",                 RANGE_RATE_SUCCESS,     RATE_AVAIL,                     1)\
    RANGE_RESULT_D(RANGE_FAIL_UNABLE_MEET_MIN_ESNO,   "FAILED - UNABLE TO MEET MIN ESNO",       RANGE_RATE_FAILED,      RATE_UNAVAIL_FAIL_RANGING,      0)\
    RANGE_RESULT_D(RANGE_NO_SUCCESS_MANY_AVG_PERIODS, "NO SUCCESS - TOO MANY AVGING PERIODS",   RANGE_RATE_NOSUCCESS,   RATE_UNAVAIL_NOSUCCESS_RANGING, 0)\
    RANGE_RESULT_D(RANGE_NO_SUCCESS_REQ_RANGING_SESS, "NO SUCCESS - UNABLE REQ RANGE SESSIONS", RANGE_RATE_NOSUCCESS,   RATE_UNAVAIL_NOSUCCESS_RANGING, 0)\
    RANGE_RESULT_D(RANGE_NO_SUCCESS_TIMEOUT_EXPIRY,   "NO SUCCESS - TIMEOUT EXPIRY",            RANGE_RATE_NOSUCCESS,   RATE_UNAVAIL_NOSUCCESS_RANGING, 0)\
    RANGE_RESULT_D(RANGE_NO_SUCCESS_INVALID_GROUP,    "NO SUCCESS - INVALID GROUP",             RANGE_RATE_NOSUCCESS,   RATE_UNAVAIL_NOSUCCESS_RANGING, 0)\
    RANGE_RESULT_D(RANGE_NO_SUCCESS_INVALID_SESSION,  "NO SUCCESS - INVALID SESSION",           RANGE_RATE_SWERROR,     RATE_UNAVAIL,                   0)\

#if 0
    (1) make INVALID_RANGE_OUTCOME = RANGE_NOT_TRIED
    (2) should treat  RANGE_NO_SUCCESS_REQ_RANGING_SESS --> RATE_UNAVAIL_FAIL_RANGING
        so that no ranging hole fix is allowed, same as RANGE_FAIL_UNABLE_MEET_MIN_ESNO
    (3) RANGE_RATE_NOSUCCESS allows "hole" fixing, estimation from "nearest symcod"
    (4) RANGE_RATE_FAILED does not allow hole fixing, and does not allow retrying ranging, even it is the most robust rate;
        effectively this rate is disabled (until the next ranging event)
#endif

/*Enumeration provides final outcome for ranging activity*/
#define RANGE_OUTCOME \
    RANGE_OUTCOME_ENUM_D(RANGE_SUCCESS_MET_TARGET_ESNO,     "SUCCESS - MET TARGET ESNO")               /*1*/\
    RANGE_OUTCOME_ENUM_D(RANGE_SUCCESS_MET_MIN_ESNO,        "SUCCESS - MET MIN ESNO")                  /*2*/\
    RANGE_OUTCOME_ENUM_D(RANGE_FAIL_UNABLE_MEET_MIN_ESNO,   "FAILED - UNABLE TO MEET MIN ESNO")        /*3*/\
    RANGE_OUTCOME_ENUM_D(RANGE_NO_SUCCESS_MANY_AVG_PERIODS, "NO SUCCESS - TOO MANY AVGING PERIODS")    /*4*/\
    RANGE_OUTCOME_ENUM_D(RANGE_NO_SUCCESS_REQ_RANGING_SESS, "NO SUCCESS - UNABLE REQ RANGE SESSIONS")  /*5*/\
    RANGE_OUTCOME_ENUM_D(RANGE_NO_SUCCESS_TIMEOUT_EXPIRY,   "NO SUCCESS - TIMEOUT EXPIRY")             /*6*/\
    RANGE_OUTCOME_ENUM_D(RANGE_NO_SUCCESS_INVALID_GROUP,    "NO SUCCESS - INVALID GROUP")              /*7*/\
    RANGE_OUTCOME_ENUM_D(RANGE_NO_SUCCESS_INVALID_SESSION,  "NO SUCCESS - INVALID SESSION")            /*8*/\

#define RANGE_OUTCOME_ENUM_D(enumName, comment) enumName,

enum rangeOutcome_e{
    INVALID_RANGE_OUTCOME      = 0,
    RANGE_OUTCOME
    RANGE_OUTCOME_MAX
};
#undef RANGE_OUTCOME_ENUM_D

/*Enumeration provides final outcome for ranging activity*/
#define RANGE_STATE \
    RANGE_STATE_D(RANGE_IDLE,         "RANGE IDLE")             /*1*/\
    RANGE_STATE_D(RANGE_REQUEST,      "RANGE REQUEST")          /*2*/\
    RANGE_STATE_D(RANGE_COMPUTING,    "COMPUTING")              /*3*/\
    RANGE_STATE_D(RANGE_RESULTS,      "SENDING RESULTS")        /*4*/\
    RANGE_STATE_D(RANGE_DONE,         "DONE")                   /*5*/\

#define RANGE_STATE_D(enumName, comment) enumName,

enum rangeState_e{
    INVALID_RANGE_STATE      = 0,
    RANGE_STATE
    RANGE_STATE_MAX
};
#undef RANGE_STATE_D

#define TERMINAL_NOT_RANGING 0
#define TERMINAL_RANGING     1

enum rangeRateStatus_e{
    RANGE_RATE_FAILED    = -1,
    RANGE_RATE_NOT_TRIED = 0,
    RANGE_RATE_SUCCESS   = 1,
    RANGE_RATE_NOSUCCESS = 2,
    RANGE_RATE_SWERROR   = 3,
    RANGE_RATE_INVALID
};

enum estRates_e{
    EST_RATES_SYM = 0, /* on this symcod */
    EST_RATES_ALL = 1,
    EST_RATES_SYMBOL_MOD = 2, /* on all symcods matching this symbol rate/modulation type */
};

/* New from CAICD SLC/MAC H45592, version D.11 */
enum rangePwrResultsFormat_e{ /* power type that the terminal is reporting */
    PWR_SETTING = 0,
    PWR_ATTENUATION,
};

enum rangeMultAndCodingType_e{ /* multiplexing type and coding method that the ranging results are for */
    TDMA_LDPC = 0,
    SCMA_LDPC = 1,

    /* 2 - 15 Reserved */
};

/**************************************************************/
/**************************************************************/
/*                   RANGING MODULE END                       */
/**************************************************************/
/**************************************************************/


/**************************************************************/
/**************************************************************/
/*                   CLPC MODULE START                        */
/**************************************************************/
/**************************************************************/

enum calcType_e
{
    CALC_SHORT_TERM = 0,
    CALC_MED_TERM,
    CALC_LONG_TERM,
    CALC_MAX_TERM
};

typedef enum
{
    PDM_MODE = 0,   /* Radio varies transmit gain, controlled by
                     * Pulse Density Modulated 2.5 MHz "tone" on
                     * transmit uplink.  This is the usual method for
                     * Ka-band radios.
                     */
    PWM_MODE = 1,  /* Radio varies transmit gain, controlled by
                     * Pulse Width Modulated 10Mhz "tone" on transmit
                     * uplink.  This is the usual method for Ku-band
                     * radios.
                     */


    OTHER_POWER_MODE  /* not supported */
}powerMode_t;

#define PWM_MAX_POWER_SETTING 0x00           /* PWM max power value is 0x00 */
#define PWM_NO_POWER          0xFF           /* PWM no power (radio off) is 0xFF */

#define MAX_PDM_SETTING_VALUE 65535
#define PDM_MAX_POWER_SETTING 55704          /* Default PDM (for Ka) max power value is 85% of 65535*/
#define PDM_NO_POWER          0x00           /* PDM (for Ka) no power (radio off) is 0x00 */

#define MAX_ULFE_BIN 320

#define T_LNA    150
#define T_COSMIC 2.7      /*unit in K */
#define T_ATM    280
#define PRE_LNA_LOSS 0.3   /* unit in dB */

#define DL_FREQ_KU  12   /* Downlink frequency at Ku in GHz */
#define DL_FREQ_KA  20   /* Downlink frequency at Ka in GHz */
#define UL_FREQ_KU  14   /* Uplink frequency at Ku in GHz */
#define UL_FREQ_KA  27   /* Uplink frequency at Ka in GHz */

#define CLPC_FILTER_DIR_ON_RDISK "/tmp/clpc"
#define CLPC_ZIP_DIR_ON_RDISK    "/fl0/logs"
#define CLPC_FEEDBACK_FILE_1_IND 0
#define CLPC_FEEDBACK_FILE_2_IND 1
#define CLPC_FEEDBACK_INDEX 750
#define MAX_CLPC_FEEDBACK_ZIP_FILE_NUM 15
#define MAX_CLPC_FEEDBACK_UNZIP_FILE_NUM 2
#define MAX_CLPC_LOG_NAME_SIZE 550
#define MAX_CLPC_LOG_PATH_SIZE 600
#define LAST_CLPC_ZIP_FILE_ID "/fl0/logs/lastClpcFid"
#define CLPC_FILE_ENTRY_LIMIT (22*CLPC_FEEDBACK_INDEX)   /* max number of entries allowed in a file */
#define CLPC_FEEDBACK_DUMP_MEM_SIZE  100000              /* bytes of storage for the clcp feedback dump */

#define DL_GASEOUS_ABSORPTION_KU          2 /* upscaled by 10 */
#define DL_GASEOUS_ABSORPTION_KA          6 /* upscaled by 10 */
#define STF_LOWER_LIMIT                   (-50) /* upscaled by 10, in 1/10 dB */
#define STF_UPPER_LIMIT                   80 /* upscaled by 10, in 1/10 dB */
#define DEFAULT_CLEAR_SKY_THRESHOLD       50 /* upscaled by 100, in 1/100 dB  */
#define DEFAULT_CLEAR_SKY_THRESHOLD       50 /* upscaled by 100, in 1/100 dB  */
#define DEFAULT_RANGING_L_EXPEDITE_WINDOW 7 /* days  */
#define DEFAULT_RANGING_L_EXPEDITE_TIMER  60 /* minutes  */
#define DEFAULT_ULFE_BIN_SIZE             10 /* 0.1dB */
#define DEFAULT_OTRT_MED_DEC_RATE         22
#define DEFAULT_OTRT_LONG_DEC_RATE        1333 /*Frames in a minute*/
#define DEFAULT_SHORT_FILTER_TC           10 /*seconds*/
#define DEFAULT_MEDIUM_FILTER_TC          60 /*seconds*/
#define DEFAULT_LONG_FILTER_TC_UP         259200 /*seconds*/
#define DEFAULT_LONG_FILTER_TC_DOWN       604800 /*seconds*/
#define DEFAULT_BIAS_FILTER_TIME_CONSTANT 86400
#define DEFAULT_BIAS_THRESHOLD            1.0
#define DEFAULT_RERANGE_TRIGGER_INTERVAL  7
#define DEFAULT_OR_SQF_CONF_CALC_INTERVAL 1
#define DEFAULT_OR_SQF_CONF_THRESHOLD     1.0
#define DEFAULT_OR_CONFIDENCE_FACTOR      0.9
#define DEFAULT_IC_CONF_BIN_SIZE          10000
#define DEFAULT_IC_CONF_WIN_SIZE          9600000
#define DEFAULT_IC_CONF_METRIC_SCALAR     0.5
#define DEFAULT_IC_TABLE_IGDP_WEIGHT      0.0
#define DEFAULT_IC_LTF_FIRST_COEFF        0.08
#define DEFAULT_IC_LTF_SECOND_COEFF       0.000016
#define DEFAULT_SMOOTHING_PHR_FACTOR      0.05
#define DEFAULT_ATTEN_JITTER_RANGE        20 /*  10ths of dB*/

/* CLPC STF Confidence related BEGIN */

#define STF_NO_CONFIDENCE_ACTION \
    STF_NO_CONFIDENCE_ACTION_D(STF_NO_CONFIDENCE_MIN,        "INVALID-MIN",  0) \
    STF_NO_CONFIDENCE_ACTION_D(STF_NO_CONFIDENCE_NO_ACTION,  "NO ACTION",    1) \
    STF_NO_CONFIDENCE_ACTION_D(STF_NO_CONFIDENCE_CLEAR_STF,  "CLEAR STF",    2) \
    STF_NO_CONFIDENCE_ACTION_D(STF_NO_CONFIDENCE_RE_RANGE,   "RE RANGE",     3) \
    STF_NO_CONFIDENCE_ACTION_D(STF_NO_CONFIDENCE_MAX,        "INVALID-MAX",  4) \

#define STF_NO_CONFIDENCE_ACTION_D(enumName, comment, enumVal) enumName = enumVal,
enum STFResetAction_e{
    STF_NO_CONFIDENCE_ACTION
};
#undef STF_NO_CONFIDENCE_ACTION_D

typedef enum{
    STF_CONFIDENCE_BURST_TYPE_STREAM = 0,       /* Stream and SCMA Data */
    STF_CONFIDENCE_BURST_TYPE_SALOHA = 1,       /* Small Aloha for both TDMA and SCMA */
    STF_CONFIDENCE_BURST_TYPE_BALOHA = 2,       /* Bootstrap Aloha */
} STFConfidenceBurstType_t;

enum {
    STF_INCREASE_CONFIDENCE = 1,
    STF_DECREASE_CONFIDENCE = 0,
};

#define STF_FULL_CONFIDENCE                                 10000


#define DEFAULT_STF_CONFIDENCE_RESET_THRESHOLD_10THS_DB     30      /* 3.0 dB */
#define DEFAULT_STF_CONFIDENCE_NAK_WEIGHT_STREAM            3       /* 0.03%, expect about 400 of them before Small Aloha */
#define DEFAULT_STF_CONFIDENCE_NAK_WEIGHT_SALOHA            25      /* 0.25%, expect about 90 of them before Bootstrap Aloha */
#define DEFAULT_STF_CONFIDENCE_NAK_WEIGHT_BALOHA            500     /* 5.00%, expect about 13 of them before Bootstrap Aloha triggers reset */

/* CLPC STF Confidence related END */


#define DEFAULT_PERSYMCOD_RERANGING_INTERVAL 12  /* hours */

#define DEFAULT_STF_GAIN                     (8)
#define DEFAULT_ULFE_GAIN                    (5)
#define DEFAULT_INROUTE_FEEDBACK_DEC         (3)
#define DEFAULT_CLPC_TIME_DIFF_THRESH        (2)
#define DEFAULT_MIN_ULFE_ATTEN               (-3)
#define DEFAULT_MAX_ULFE_ATTEN               (25)
#define DEFAULT_CLPC_AVG_WINDOW              (2)
#define DEFAULT_AIS_QUEUE_SIZE               (3)
#define DEFAULT_PCE_TIME_CONST               (3600)

#define RANGE_PWM_POWER_MINIMUM_PASS  0xB2
#define RANGE_PDM_POWER_MINIMUM_PASS  0x24

#define PARMS_LOAD_FAIL     -1
#define PARMS_LOAD_SUCCESS   0
#define PARMS_LOAD_NO_AIS    1

#define ELEVATION_ANGLE_CONUS   20
#define SINR_INTERVAL   10        /* unite in mSec */

/* Bounds for the feedback */
#define MIN_TDMA_SINR_FEEDBACK_VAL       10  /* 1.0d dB is the min accepted Es/No  (*** Will probably need to change for SCMA ***) */
#define MAX_TDMA_SINR_FEEDBACK_VAL  200 /* 20.0d dB is the max accepted Es/No for 8PSK */
#define MIN_SCMA_SINR_FEEDBACK_VAL  -30  /* 10ths of dB*/
#define MAX_SCMA_SINR_FEEDBACK_VAL  127  /* 10ths of dB*/
#define INVALID_SCMA_FEEDBACK_VAL   (MIN_SCMA_SINR_FEEDBACK_VAL-10)  /* So that, it is definitely below the MIN_SCMA_SINR_FEEDBACK_VAL */

#define RETX_POWER_STEP_SIZE 1
#define RETX_POWER_MAX       20

#define AIS_DATA_FILE    "/fl0/ais.dat"
#define CLPC_ERR_FILE    "/fl0/clpchist.txt"
#define NO_ATTEN         0

#define POWER_SETTING_INPUT \
PS_INPUT_D(PS_LOCAL, "Local config") \
PS_INPUT_D(PS_ODU, "ODU Info") \
PS_INPUT_D(PS_DEFAULT, "Default") \
/* end of list */

#define PS_INPUT_D(enumName, comment) enumName,

enum ulPowerSettingInput_e {
    POWER_SETTING_INPUT
};

#undef PS_INPUT_D

#define AIS_SELECT_MODE \
AIS_SELECT_D(AIS_INVALID, "AIS Invalid") \
AIS_SELECT_D(AIS_NO_CLPC, "Highest Rate") \
AIS_SELECT_D(AIS_BOOTSTRAP, "Bootstrap") \
AIS_SELECT_D(AIS_ALGO, "AIS Algo") \
AIS_SELECT_D(AIS_ALGO_IC, "AIS Algo IC") \
AIS_SELECT_D(AIS_ALGO_IC_INROUTE_SET, "AIS Algo IC with Inroute Set enabled") \
AIS_SELECT_D(AIS_INROUTE_SET_ASSIGNED, "Inroute Set Assigned") \
AIS_SELECT_D(AIS_INROUTE_SET_PREC, "Inroute Set Precedence") \

/* end of list */

#define AIS_SELECT_D(enumName, comment) enumName,

enum aisSelectMode_e {
    AIS_SELECT_MODE
};

#undef AIS_SELECT_D


/**************************************************************/
/**************************************************************/
/*                   CLPC MODULE ENDS                         */
/**************************************************************/
/**************************************************************/


/**************************************************************/
/**************************************************************/
/*               UPLINK STATE CODE START                      */
/**************************************************************/
/**************************************************************/

/* Enumeration provies different state code monitors */
#define UL_STATE_CODE_MONITOR \
    UL_STATE_CODE_MONITOR_ENUM_D(UL_STATE_CODE_MONITOR_GLOBAL,        "Global Uplink State Code Monitor")         /* 0 */\
    UL_STATE_CODE_MONITOR_ENUM_D(UL_STATE_CODE_MONITOR_LIMITED_STATS, "Limited Stats Uplink State Code Monitor")  /* 1 */\

#define UL_STATE_CODE_MONITOR_ENUM_D(enumName, comment) enumName,

enum ulStateCodeMonitor_e{
    UL_STATE_CODE_MONITOR
    UL_STATE_CODE_MONITOR_MAX
};
#undef UL_STATE_CODE_MONITOR_ENUM_D

enum stateCodeImpact_e {
  IMPACT_NONE     = 0,
  IMPACT_MINOR    = 1,
  IMPACT_MODERATE = 2,
  IMPACT_MAJOR    = 3,
  IMPACT_CRITICAL = 4,

  IMPACT_MAX
};

/**************************************************************/
/**************************************************************/
/*               UPLINK STATE CODE END                        */
/**************************************************************/
/**************************************************************/


/**************************************************************/
/**************************************************************/
/*               UPLINK GENERAL SECTION BEGIN                 */
/**************************************************************/
/**************************************************************/

#define BURST_PADDING_BYTE  (0xA5)

#define ALOHA_BURST_HEADER_SAI_LEN  (5)
#define ALOHA_BURST_HEADER_ESN_LEN  (6)
#define BACKLOG_LEN                (3)
#define ENCRYPTION_LEN             (1)

/*** For Burst / Frame Debuging */
#define MAX_BURSTS_IN_FRAME_DEBUG   6       /* track at most this many bursts per frame */
#define MAX_PKT_IN_BURST_DEBUG      10      /* track at most this many pkts per burst */


/*** Burst Information ***/
#define MAX_BURST_LENGTH      1566    /* maximum burst length in bytes [See Jupiter CAICD H43044] */
#define MAX_IBE_BURST_HDR_LEN 12
#define MAX_ADAPTATION_MSGS   1
#define MAX_ADAPTATION_SIZE   140
#define IP_UDP_HEADER_SIZE    28
#define CDS_DNCC_HEADER_SIZE  40

/* ICD (H45592): Table - Base Section of Burst Header Feild Description:
The Burst Type field indicates the type of burst.  Valid burst types include:
    0 (00b)  [TDMA] Unallocated (aka Aloha)
    1 (01b)  [TDMA] Allocated (aka Stream)
    2 (10b)  SCMA Aloha
    3 (11b)  SCMA Data
*/
typedef enum
{
    IBE_BASE_BURST_TYPE_MIN_EXCLUSIVE = -1,

    IBE_BASE_BURST_TYPE_TDMA_ALOHA = 0,
    IBE_BASE_BURST_TYPE_TDMA_DATA,
    IBE_BASE_BURST_TYPE_SCMA_ALOHA,
    IBE_BASE_BURST_TYPE_SCMA_DATA,

    IBE_BASE_BURST_TYPE_MAX_EXCLUSIVE,

    IBE_BASE_BURST_TYPE_SCMA_ALLOC = IBE_BASE_BURST_TYPE_MAX_EXCLUSIVE, /* for future extension */

}ibeBaseHeaderBurstTypes_e;

/* this is SWP defined burst types, and mapped to IBE burst type */
#define UL_BURST_TYPE_LIST \
 _BURST_TYPE_D(TDMA_ALOHA,      "TDMA Aloha",   IBE_BASE_BURST_TYPE_TDMA_ALOHA)\
 _BURST_TYPE_D(STREAM,          "TDMA Stream",  IBE_BASE_BURST_TYPE_TDMA_DATA)\
 _BURST_TYPE_D(SCMA_ALOHA,      "SCMA Aloha",   IBE_BASE_BURST_TYPE_SCMA_ALOHA)\
 _BURST_TYPE_D(SCMA_DATA,       "SCMA Data",    IBE_BASE_BURST_TYPE_SCMA_DATA)\
 _BURST_TYPE_D(TFIX_TEST,       "TFIX TDMA",    IBE_BASE_BURST_TYPE_TDMA_DATA)\
 _BURST_TYPE_D(TFIX_SCMA,       "TFIX SCMA",    IBE_BASE_BURST_TYPE_SCMA_DATA)\
 _BURST_TYPE_D(SCMA_ALLOC,      "SCMA Alloc",   IBE_BASE_BURST_TYPE_SCMA_ALLOC)

#define SCMA_UNALLOC  SCMA_DATA

typedef enum {
    BURST_TYPE_NIN_EXCLUSIVE = -1,
    #define _BURST_TYPE_D(typeEnum, comment,dummy) typeEnum,
    UL_BURST_TYPE_LIST
    #undef _BURST_TYPE_D

    BURST_TYPE_MAX_EXCLUSIVE,
    BURST_TYPE_MAX = BURST_TYPE_MAX_EXCLUSIVE,
}burstTypes_e;

typedef enum{
    TDMA_TRANSMISSION,
    SCMA_TRANSMISSION,
}transmissionTypes_e;

#define IS_BURST_TYPE_VALID(burstType)        (burstType > BURST_TYPE_NIN_EXCLUSIVE && burstType < BURST_TYPE_MAX_EXCLUSIVE)
#define IS_BURST_TYPE_UNALLOCATED(burstType)  (burstType == TDMA_ALOHA || burstType == SCMA_ALOHA || burstType == SCMA_DATA || burstType == TFIX_SCMA)
#define IS_BURST_TYPE_ALLOCATED(burstType)    (burstType == STREAM || burstType == SCMA_ALLOC || burstType == TFIX_TEST)
#define IS_BURST_TYPE_TDMA(burstType)         (burstType == STREAM || burstType == TDMA_ALOHA || burstType == TFIX_TEST)
#define IS_BURST_TYPE_SCMA(burstType)         (burstType == SCMA_ALLOC || burstType == SCMA_DATA || burstType == SCMA_ALOHA || burstType == TFIX_SCMA)
#define IS_BURST_TYPE_TFIX(burstType)         (burstType == TFIX_TEST || burstType == TFIX_SCMA)
#define IS_BURST_TYPE_ALOHA(burstType)        (burstType == SCMA_ALOHA || burstType == TDMA_ALOHA)
#define IS_BURST_TYPE_DATA(burstType)         (burstType == STREAM || burstType == SCMA_DATA || burstType == SCMA_ALLOC || burstType == TFIX_TEST)
#define IS_SCMA_MUST_BE_TXED(burstType)       (burstType == SCMA_ALOHA || burstType == SCMA_ALLOC)

/* we will define a coded burst property to replace burst type */
/* e.g.,
 bit3: CW or burst, bit2 - tfix or normal, bit1: SCMA or TDMA, bit0: Allocated or Unallocated */
enum{
  BURST_TYPE_ALLOCATED0_UNALLOCATED1_BIT,
  BURST_TYPE_TDMA0_SCMA1_BIT,
  BURST_TYPE_TFIX_OFF0_ON1_BIT,
  BURST_TYPE_CW_OFF0_ON1_BIT,
};

typedef enum {
    ADDRESS_NONE               = 0, /* No address sent in the burst header */
    ADDRESS_SERIAL_NUMBER      = 1, /* Serial number sent in the burst header */
    ADDRESS_SYSTEM_ASSIGNED_ID = 2, /* System Assigned ID (SAI) send in the burst header */
    ADDRESS_RESERVED           = 3,

    ADDRESS_TYPE_MAX
} addrTypes_e;

#define ALOHA_TYPES \
_ENUM_D(NO_ALOHA,           0, "No Aloha")  \
_ENUM_D(INITIATING_ALOHA,   1, "Initiating")  \
_ENUM_D(DIVERSITY_ALOHA,    2, "Diversity")  \
_ENUM_D(RETX_ALOHA,         3, "Retransmit")  \
_ENUM_D(ALOHA_TYPE_MAX,     4, "ALOHA_TYPE_MAX")  \

#define _ENUM_D(enumName, enumVal, comment) enumName,
typedef enum {
    ALOHA_TYPES
} alohaTypes_e;
#undef _ENUM_D

/*Note - The following enum and string structure should be updated in parallel */
#define UL_FRAME_STATE_LIST \
    _FRAME_STATE_D(DISCARD_FRAME,        "DISCARD",          "Frame was not setup right")\
    _FRAME_STATE_D(FRAME_INIT,           "FRAME_INIT",       "Initial state of the frame with no data")\
    _FRAME_STATE_D(FRAME_VALID,          "FRAME_VALID",      "BAP Arrived for this frame")\
    _FRAME_STATE_D(ALLOC_DONE,           "ALLOC_DONE",       "Allocation done for this frame")\
    _FRAME_STATE_D(IBE_DONE,             "IBE_DONE",         "IBE portion filled for this frame")\
    _FRAME_STATE_D(FRAME_SENT,           "FRAME_SENT",       "Frame configured and sent to the UPP")\
    _FRAME_STATE_D(FRAME_ACK,            "FRAME_ACK",        "Frame has successful transmission acknowledged")\
    _FRAME_STATE_D(FRAME_TDMA_ACK,       "FRAME_TDMA_ACK",   "Frame has successful TDMA bursts acknowledged")\
    _FRAME_STATE_D(FRAME_SCMA_ACK,       "FRAME_SCMA_ACK",   "Frame has successful SCMA burst acknowledged")\
    _FRAME_STATE_D(FRAME_NAK,            "FRAME_NAK",        "Frame has successful transmission NOT acknowledged")\
    _FRAME_STATE_D(UPP_DISCARD_FRAME,    "NO_BURST_TO_SEND", "Frame was discarded by UPP due to error found")\
    _FRAME_STATE_D(FRAME_INVALID_STATE,  "FRAME_INVALID",    "Invalid frame state")\

typedef enum {
    #define _FRAME_STATE_D(enumName, keyWorStr, moreTip) enumName,
    UL_FRAME_STATE_LIST
    #undef _FRAME_STATE_D
    FRAME_STATE_MAX,
}frameState_e;

/* Burst State Enum */
#define BURST_STATE \
    BURSTCTRL_ENUM_D(BURST_STATE_NOT_SENT, "Not Sent")\
    BURSTCTRL_ENUM_D(BURST_STATE_SENT,     "Sent")\
    BURSTCTRL_ENUM_D(BURST_STATE_ACK,      "ACK")\
    BURSTCTRL_ENUM_D(BURST_STATE_NAK,      "NAK")\

enum burstState_e {
    #define BURSTCTRL_ENUM_D(enumName, comment) enumName,
    BURST_STATE
    #undef BURSTCTRL_ENUM_D
};

typedef enum {
    UL_OP_MODE_UNKNOWN = -1,
    UL_OP_MODE_NORMAL = 0,
    UL_OP_MODE_TX_TEST,
    UL_OP_MODE_TFIX,
    UL_OP_MODE_ALOHA,
    TFIX_BYPASS_ULP,
    TFIX_TPQ_COMBO,
    UL_OP_MODE_MAX
}operationMode_e;

/*** Frame Information ***/
#define MAX_FRAME_HISTORY    32

/* this allows SWP to manipulate FIT on cached memory, and then copy it into the reserved (and uncached) memory
   or copy the uncached memory into cached memory for UPP burst information */

/* the ulfit_reserved_t is used between SWP and UPP,
    which is the subset mirror of struct ulfit_t (as cached and part of the Uplink shared memory),
    but resides in SWP-UPP reserved memory (as Uncached);
    The MAX_RESERVED_FRAME_HISTORY <=  MAX_FRAME_HISTORY */
#define MAX_RESERVED_FRAME_HISTORY  (8)
#define UL_FIT_MEM_SIZE              0x00200000   /* 2Mb */

#define VALID_BURST_INDEX(index) (((index >= 0) && (index < MAX_BURSTS_IN_FRAME)) ? 1 : 0)

#define MAX_FRAME_NUMBER    0xffffffff   /* Maximum 32 bit frame number */
#define IPE_DATA_PADDING_THRESHOLD (1) /* if IPE available len < this to fill data, do data padding*/

#define FRAME_INDEX(frameNr) ((frameNr) % MAX_FRAME_HISTORY)


#define FRAME_FLAG_TFIX_BURST   (1<<0)    /* to indicate, this frame of burst is for TFIX
so that the IPE/data portion are using tfix pattern in stead of TPQ */

enum /* used by IBE flag */
{
    IBE_FLAG_CACHE_BURST            = 0x00000001,   /* Cache away this burst... to be used later for Diversity Aloha */
    IBE_FLAG_RESEND                 = 0x00000002,   /* Retransmit the last burst. Exact duplicate */
    IBE_FLAG_INITIATING_ALOHA_ACKED = 0x00000004,   /* The initiating aloha for this diversity aloha burst has been already acknowledged */

    /* to indicate which slot is this 2-slot SCMA aloha, when
       adaption is too long, and the whole IBE header crosses slots */
    IBE_FLAG_SCMA_ALOHA_1ST_SLOT_WHEN_IBE_HEADER_CROSSED = 0x000000010,  /* indicate it is the 1st slot of this SCMA aloha: slot 0 */
    IBE_FLAG_SCMA_ALOHA_2ND_SLOT_WHEN_IBE_HEADER_CROSSED = 0x000000020,  /* indicate it is the 2nd slot of this SCMA aloha: slot 1 */
};

/*outroute transport packets protocol type*/
typedef enum {
    PROTOCOL_IPV4,
    PROTOCOL_IPV6,
    PROTOCOL_INRT_CTRL,
    PROTOCOL_OUTRT_CTRL,
    PROTOCOL_ROHC,
    PROTOCOL_MAX
}xportProtoType_e;

/* PDU CRC configuration based on IGDP */
#define PDU_CRC_CONFIG \
PDU_CRC_CONFIG_ENUM_D(CRC_NOT_REQUIRED,         "PDU CRC Not Required")                     /* 0 */\
PDU_CRC_CONFIG_ENUM_D(CRC_ON_SEGMENTED_PKTS,    "PDU CRC Required on Segmented Packets")    /* 1 */\
PDU_CRC_CONFIG_ENUM_D(CRC_ALWAYS,               "PDU CRC Required on All Packets")          /* 2 */\

#define PDU_CRC_CONFIG_ENUM_D(enumName, comment) enumName,
/*note: sequencer compiler does not like enum directly. Have to use typdef*/
typedef enum pduCrcConfig_e {
    PDU_CRC_CONFIG
}pduCrcConfig_t;
#undef PDU_CRC_CONFIG_ENUM_D

/* TPQ definition */
#define PRIORITY_QUEUE \
    TPQ_ENUM_D(TPQ_0, "Conversational [P0]") \
    TPQ_ENUM_D(TPQ_1, "Interactive    [P1]") \
    TPQ_ENUM_D(TPQ_2, "Streaming      [P2]") \
    TPQ_ENUM_D(TPQ_3, "Bulk           [P3]") \
    TPQ_ENUM_D(TPQ_4, "Expedited      [P4]")

enum transmitPriorityQ_e {
    #define TPQ_ENUM_D(enumName, comment) enumName,
    PRIORITY_QUEUE
    #undef TPQ_ENUM_D
    MAX_NUM_PRI_QUEUES,
};

#define ULTPQ_PACKET_SIZE_HISTO_STEP_SIZE       10
#define ULTPQ_PACKET_SIZE_HISTO_NUMBER_STEPS    151     //record packet size (in bytes) from 0-1500 in steps of 10, so need 150 + 1 number of bins/steps
#define ULTPQ_PACKET_SIZE_HISTO_BASE_VALUE      0
#define MAX_PACKET_SIZE_HISTO                   151     //record packet size (in bytes) from 0-1500 in steps of 10, so index 150 contains max value
#define ULTPQ_ARRIVAL_RATE_HISTO_STEP_SIZE      1       //step size for arrival rate is in power of 2, but initialize to 1 -- computation will be done in histogram.c
#define ULTPQ_ARRIVAL_RATE_HISTO_NUMBER_STEPS   21      //record arrival rates in steps of powers of starting from 0 to 524288ms, so will need 20 + 1 bins/steps
#define ULTPQ_ARRIVAL_RATE_HISTO_BASE_VALUE     0
#define MAX_ARRIVAL_RATE_HISTO                  21      //record arrival rates in steps of powers of starting from 0 to 524288ms, so index 20 contains max value
#define TPQ_HISTO_EXPONENTIAL_VALUE             2       //power of 2
#define MAX_TIME_MILLISECONDS                   4294967

#define MAX_UPLOAD_THROUGHPUT_DEFAULT       100
#define MAX_QUEUE_LATENCY_DEFAULT           6000
#define MAX_CONV_QUEUE_LATENCY_DEFAULT      1500
#define MAX_BANDWIDTH_PERCENTAGE_DEFAULT    20

#define TERMINAL_MIN_UL_THROUGHPUT 0
#define TERMINAL_GUARANTEED_UL_THROUGHPUT 1
#define TERMINAL_MAX_UL_THROUGHPUT 2
#define TERMINAL_UL_THROUGHPUT_TYPES (TERMINAL_MAX_UL_THROUGHPUT+1)

/***********************************************************************
 * DATA DESCRIPTION: Uplink Transmit freq band                 *
 **********************************************************************/
/* Frequency Band defines */

/* On an HN based terminal, we will ONLY support Ka and Ku band.
 * On a Sahara based terminal, we will ONLY support Ka band
 */

typedef enum {
    FREQ_BAND_KA = 0,
    FREQ_BAND_KU,
    FREQ_BAND_MAX
} freqBand_e;

/* standard range */
#define MAX_TX_KA_FREQ_LIMIT            30000000000LL
#define MIN_TX_KA_FREQ_LIMIT            28300000000LL
#define MAX_TX_KU_FREQ_LIMIT            14500000000LL
#define MIN_TX_KU_FREQ_LIMIT            13700000000LL

/* if in Factory mode: stretched from standard range */
#define MAX_TX_KA_FREQ_LIMIT_FACT_MODE  40000000000LL
#define MIN_TX_KA_FREQ_LIMIT_FACT_MODE  27000000000LL
#define MAX_TX_KU_FREQ_LIMIT_FACT_MODE  15000000000LL
#define MIN_TX_KU_FREQ_LIMIT_FACT_MODE  13000000000LL

#define IS_TX_FREQ_KU_BAND(otaFreq)  (otaFreq >= MIN_TX_KU_FREQ_LIMIT && otaFreq <= MAX_TX_KU_FREQ_LIMIT)
#define IS_TX_FREQ_KA_BAND(otaFreq)  (otaFreq >= MIN_TX_KA_FREQ_LIMIT && otaFreq <= MAX_TX_KA_FREQ_LIMIT)

#define IS_TX_FREQ_KU_BAND_FACT_MODE(otaFreq)  (otaFreq >= MIN_TX_KU_FREQ_LIMIT_FACT_MODE && otaFreq <= MAX_TX_KU_FREQ_LIMIT_FACT_MODE)
#define IS_TX_FREQ_KA_BAND_FACT_MODE(otaFreq)  (otaFreq >= MIN_TX_KA_FREQ_LIMIT_FACT_MODE && otaFreq <= MAX_TX_KA_FREQ_LIMIT_FACT_MODE)

#define KU_BASE_FREQ_MHZ        14000 /* in MHz */
#define KA_BASE_FREQ_MHZ        29250 /* in MHz */

#define TX_BANDWIDTH_MHZ 500  /* We support a 500 MHz transmit bandwidth.  For example, from 14000 to 14500 (MHz) for Ku */

#define MAX_INROUTE_GROUPS      (ISDP_MAX_INROUTE_GROUPS)

/* define group types: SCMA or TDMA */
#define GROUP_TYPE \
    ENUM_INFO(0, TDMA_GROUP, "TDMA Group")\
    ENUM_INFO(1, SCMA_GROUP, "SCMA Group")

typedef enum {
    GROUP_TYPE_MIN = -1,

    #define ENUM_INFO(enumVal, enumName, comment) enumName = enumVal,
    GROUP_TYPE
    #undef ENUM_INFO
    GROUP_TYPE_MAX,
} groupType_e;

#define INVALID_GROUP_TYPE (GROUP_TYPE_MIN)
#define IS_VALID_UL_GROUP_TYPE(groupType) (groupType > GROUP_TYPE_MIN && groupType < GROUP_TYPE_MAX)

#define UW_SET_MIN 1
#define UW_SET_MAX 7

#define ITUR_RATE   0x07    /* Mask for Base Rate */
#define ITUR_LDPC   0x10    /* LDPC coding flag */

#define CHANNEL_FREQ_TO_MHZ 10000 /* Scaling factor between advertised channel freq (in 100s of Hz) to MHz */
#define CHANNEL_FREQ_TO_KHZ 10    /* Scaling factor between advertised channel freq (in 100s of Hz) to kHz */
#define FREQ_MHZ_TO_100S_OF_HZ 10000

#define BITS_IN_A_BYTE              8
#define OUTERCODE_LDPC_CRC_SIZE     2
#define SYMBOLS_PER_SLOT          120
#define QPSK_BITS_PER_SYMBOL        2
#define ENCODED_BYTES_PER_SLOT     30

/* Multimode defaults */
#define PERIODIC_MM_CAL_INTERVAL            20     /* Periodic multimode calibration interval (in minutes) */
#define PERIODIC_RADIO_TEMP_READ_INTERVAL   10     /* Periodic Radio Temperature  Read Interval (in seconds) */
#define RADIO_TEMP_CHANGE_IDLE              6      /* Min Change in Temperature for Radio Calibration (in Celsius), calibrate when IDLE */
#define RADIO_TEMP_CHANGE_IMMEDIATE         12     /* Max Change in Temperature for Radio Calibration (in Celsius), calibrate immediately */
#define CHECK_DAPT_INLINE_INTERVAL          10     /* Periodic DAPT INLINE CHECK  Read Interval (in seconds)*/


#define MM_CONFIG_TX_MODE \
    MM_CONFIG_TX_MODE_D(MM_CONFIG_TX_MODE_UNSPECIFIED,  "Unspecified",  0) \
    MM_CONFIG_TX_MODE_D(MM_CONFIG_TX_MODE_LINEAR,       "Linear",       1) \
    MM_CONFIG_TX_MODE_D(MM_CONFIG_TX_MODE_SATURATED,    "Saturated",    2) \

#define MM_CONFIG_TX_MODE_D(enumName, comment, enumVal) enumName = enumVal,
enum MultimodeConfigMode_e{
    MM_CONFIG_TX_MODE
};
#undef MM_CONFIG_TX_MODE_D

/* Cavalier Constants */
#define CAVALIER_CLOCK_MHZ          100
#define CAVALIER_CLOCK_TICKS_US     100
#define CAVALIER_CLOCK_TICKS_MS     100000

/* Frame Information Constants */
#define FRAMES_IN_SUPERFRAME 8
#define SUPERFRAME_TIME_MS   (FRAMES_IN_SUPERFRAME * FRAME_TIME_MS)
#define TX_FRAMES_PER_SEC    22
#define TX_FRAMES_PER_MIN    1333
#define TX_FRAMES_PER_HOUR   80000
#define TX_FRAMES_PER_DAY    1920000

/* inroute sets constants */
#define DEFAULT_POWERHR_TIMER               5
#define DEFAULT_POWERHR_DELTA_THRESHOLD     3
#define DEFAULT_POWERHR_SMOOTH_FACTOR       50

/* Handy Macros */
#define MIN_VAL(a,b)    ((a < b) ?  (a) : (b))
#define MAX_VAL(a,b)    ((a < b) ?  (b) : (a))
#define VALID_QID(qID)  (((qID >= 0) && (qID < MAX_NUM_PRI_QUEUES)) ? 1 : 0)
#define VALID_INROUTE(inrouteNum)   (((inrouteNum >= 0) && (inrouteNum < IGD_MAX_INROUTES)) ? 1 : 0)

typedef enum {
    VNO_CAPABLE_DISABLED = 0,
    VNO_CAPABLE_ENABLED
} vnoCapableTerminal_e;

typedef enum {
    BACKLOG_SCALAR_DISABLED = 0,
    BACKLOG_SCALAR_ENABLED
} backlogScalar_e;

/**************************************************************/
/**************************************************************/
/*               UPLINK GENERAL SECTION END                 */
/**************************************************************/
/**************************************************************/

/**************************************************************/
/*               SCMA START                                   */
/**************************************************************/

/* * Timing/Symbol layout of a SCMA burst * * *

    (9.6us Radio on)                          +
    128-Symbol-Aperture                       +
    256-Symbol-UW                             +
    (88-Symbol-use-data + 8-Symbol-pilot)[1]  +
    (88-Symbol-use-data + 8-Symbol-pilot)[2]  +
    ..........                                +
    (88-Symbol-Burst-data + 8-Symbol-pilot)[32] +
    64-symbol                                 +
    (9.6us Radio off)

Note:
    OTA Symbols (for burst data) = 32*88+64 = 2880
    i.e., 80 Bytes
* * * * * * * * * * * * *  * * * * * * * * * */
#define  NUM_SYMBOLS_PER_SLOT                 (120)      /* pre-defined */

/* * * * * * * * * * Color layout of a SCMA Frame * * * * * * * * * * *

   | C1 | C2 | C3 |...| C12 | C1 | C2 | C3 |...| C13 |18 unused slots|

Note:
  (1) each color set represents 1 burst with 30 slots
  (2) Each color has 32 UWs, and randomly chooses one
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define  SCMA_BURST_SYMBOL_2_BYTES(symbols)    (symbols*2/8)
#define  DISTANCE_SCMA_AND_TDMA_IN_US          (3000)     /* pre-defined 3 ms (=3000 us) btw TDMA and SCMA bursts */

#define  SCMA_SYMBOL_RATE_KSPS                 (2048)     /* pre-defined 2048Ksps */

/* FEC = 1/9 */
#define  SCMA_FEC_RATE_M_IN_M_OVER_N           (1)        /* pre-defined */
#define  SCMA_FEC_RATE_N_IN_M_OVER_N           (9)        /* pre-defined */

#define  NUM_SCMA_BURSTS_PER_FRAME             (25)       /* pre-defined */
#define  NUM_SCMA_COLORS_PER_FRAME             (13)       /* pre-defined */
#define  MAX_UW_SETS_PER_SCMA_COLOR            (32)       /* pre-defined */
#define  MAX_SCMA_COLORS                       (32)       /* pre-defined */
#define  MAX_NUM_SCMA_UW_SETS                  (1024)     /* 32 colors * 32 UWs*/

#define  MAX_NUM_SCMA_BURSTS_PER_GROUP         (20)       /* pre-defined */
#define  NUM_SYMBOLS_PER_SCMA_SLOT             (NUM_SYMBOLS_PER_SLOT)      /* pre-defined */
#define  NUM_SCMA_SLOTS_PER_FRAME              (768)      /* 45us*SymbolRate/120 = 45*2048/120 */
#define  TOTAL_NUM_SCMA_SYMBOLS_PER_FRAME      (92160)    /* 768*120 */

#define  NUM_ENCODED_OTA_DATA_BYTES_PER_SCMA_BURST      (720)      /* (2880*2)/8bits; OQPSk, 2 bits/symbol */
#define  NUM_OTA_DATA_BYTES_PER_SCMA_BURST     (80)       /* (2880/9(FEC)*2)/8bits; OQPSk, 2 bits/symbol */
#define  NUM_CRC_BYTES_PER_SCMA_BURST          (2)        /* pre-defined; at the end of the burst */
#define  NUM_BURST_DATA_BYTES_PER_SCMA_BURST   (78)       /* 78 = 80 OTA- 2 CRC */
#define  NUM_UW_SYMBOLS_PER_SCMA_BURST         (256)      /* pre-defined */
#define  NUM_SCMA_PILOT_SYMBOLS                (8)        /* pre-defined */
#define  NUM_SYMBOLS_SCMA_PILOT_INSERT         (96)       /* insert pilot 8 symbols for every 96 symbols: 88 + 8 = 96*/
#define  SCMA_RADIO_ON_OFF_TIME_IN_10TH_US     (96)       /* Radio on/off each with 9.6 us at start/end of the brust */
#define  TOTAL_NUM_SYMBOLS_PER_SCMA_BURST      (3539)     /*  */

#define  START_SLOT_OFFSET_SCMA_BURST          (18)        /* offset[burst#] = me + burst# * NUM_SLOTS_PER_SCMA_BURST*/
#define  NUM_SLOTS_PER_SCMA_BURST              (30)       /* pre-defined */
#define  NUM_SCMA_SLOTS_UNUSED_PER_FRAME       (18)       /* total-slots mod total-bursts: (768 mod 30)= 18 */
#define  NUM_RAND_APER_SYMBOLS_PER_SCMA_BURST  (128)      /* pre-defined */
#define  NUM_PILOT_SYMBOLS_PER_SCMA_BURST      (256)      /* 32*8 */
#define  NUM_30US_APER_SYMBOLS_PER_SCMA_BURST  (62)       /* derived from symbol rate: ceil(2048E3*30E-6) = 62 */
#define  NUM_BURST_DATA_SYMBOLS_PER_SCMA_BURST (2880)     /* 32*88 + 64: burst header + burst payload */
#define  NUM_OVERHEAD_SYMBOLS_PER_SCMA_BURST   (702)      /* Aperture + UW + pilots + 30 us aper= 128+256+32*8 + 62 */
#define  NUM_OVERHEAD_BYTES_ENCODED_PER_SCMA_BURST     (702*2/8)  /* Symbol to bytes */
#define  NUM_OVERHEAD_BYTES_PER_SCMA_BURST     (702*2/8/9)  /* Symbol to bytes */
#define  NUM_SYMBOLS_PER_SCMA_BURST            (3582)     /* 2880 + 702 */
#define  NUM_BYTES_ENCODED_PER_SCMA_BURST      (3582*2/8)  /* Symbol to bytes  */
#define  NUM_BYTES_PER_SCMA_BURST              (3582*2/8/9) /* Symbol to bytes  */

#ifdef THEBE_PLATFORM
/* noting, THEBE clock rate is doubled, and at the same time doubled number of common slots per frame; so the symbols per
   common-slot must be half of that for CALLISTO, since the symbol per frame is a constant for a given symbol rate */
#define SCMA_NUM_SYMBOLS_PER_COMMON_SLOT       (5)    /* 5 symbols per common slot for 2Msps SCMA */
#define SCMA_CLKS_PER_SYMBOL                   (120)  /* clock cycles per symbol for SCMA */
/*((double)(FRAME_TIME_US*100/SCMA_CLKS_PER_SYMBOL/TOTAL_NUM_SCMA_SYMBOLS_PER_FRAME)) */
#define CLK_CYCLE_TO_10NS_AT_2MSPS  (0.4069010415)
#else
#define SCMA_NUM_SYMBOLS_PER_COMMON_SLOT       (10)  /* 10 symbols per common slot for 2Msps SCMA */
#define SCMA_CLKS_PER_SYMBOL                   (60)  /* clock cycles per symbol for SCMA */
/*((double)(FRAME_TIME_US*100/SCMA_CLKS_PER_SYMBOL/TOTAL_NUM_SCMA_SYMBOLS_PER_FRAME)) */
#define CLK_CYCLE_TO_10NS_AT_2MSPS  (0.813802083)
#endif

/* frequency table entry allocation */
#define  TDMA_INROUTE_FREQ_TABLE_UPPER_INDEX   123          /* MAX TDMA index */
#define  SCMA_INROUTE_FREQ_TABLE_INDEX         124          /* For SCMA normal operation */
#define  SCMA_TEST_INROUTE_FREQ_TABLE_INDEX    125          /* reserved for SCMA test (TFIX or Force TX) purpose */
#define  TDMA_TEST_INROUTE_FREQ_TABLE_INDEX    126          /* reserved for TDMA test (TFIX or Force TX) purpose */
#define  CW_TEST_INROUTE_FREQ_TABLE_INDEX      127          /* reserved for CW test purpose */

/* UW size in bytes: 7 beams * (13 colors * 32 Uw entries) * 26 words * 4 bytes */
#define  NUM_WORDS_PER_SCMA_UW_ENTRY           (26)
#define  NUM_BEAMS_4_SCMA_UW_STORAGE           (7)    /* need to store UW for 7 outroute beams */

#define VALID_UWSetID(id) (id>=1 && id<=NUM_BEAMS_4_SCMA_UW_STORAGE)
#define VALID_UWIndex(id) (id>=0 && id<MAX_UW_SETS_PER_SCMA_COLOR)
#define SCMA_SLOT_INDEX_IS_VALID(slotIndex) (slotIndex>=0 && slotIndex < NUM_SCMA_BURSTS_PER_FRAME)

/* for configure TXHA: TX control Table */
#define  PAYLOAD_SIZE_SLOTS_PER_SCMA_BURST     (2880/120)   /* 24 = NUM_BURST_DATA_SYMBOLS_PER_SCMA_BURST/NUM_SYMBOLS_PER_SCMA_SLOT*/
#define  SCMA_TXHA_CB_SIZE_SLOTS               (24)
#define  SCMA_TXHA_N_SHORT_CB                  (1)
#define  SCMA_TXHA_N_LONG_CB                   (0)
#define  SCMA_START_SLOT_OFFSET BOARD_TYPE_J2WW_LINEAR_DEFAULT_VAL               (18) /* 1st color starts from slot 18 */

/* SCMA burst states: alloc, shcedule, transmit, ... */
typedef enum {
    UL_SCMA_NOT_ELIGIBLE = 0,  /* default */
    UL_SCMA_NOT_ELIGIBLE_TDMA,
    UL_SCMA_NOT_ELIGIBLE_GAP,
    UL_SCMA_ELIGIBLE,
    UL_SCMA_ELIGIBLE_BUT_NOT_SCHEDULED,
    UL_SCMA_SCHEDULED,
    UL_SCMA_TRANSMITTED,
    UL_SCMA_DISCARDED,
    UL_SCMA_ACKD,
    UL_SCMA_PARTIAL_ACKD,
    UL_SCMA_NAKD,
}ulScmaBurstState_e;

#define SCMA_TX_DISABLED_REASONS \
    SCMA_ENUM_D(0,  SCMA_TX_NOT_DISABLED,   "SCMA TX Not Disabled")\
    SCMA_ENUM_D(1,  SCMA_DISABLED_CFM,      "SCMA Disabled via CFM")\
    SCMA_ENUM_D(2,  INVALID_TRIGGER,        "Invalid Trigger (no SGDP/TFIX)")\
    SCMA_ENUM_D(3,  GROUP_UNAVAILABLE,      "Unavailable SCMA Groups")\
    SCMA_ENUM_D(4,  TDMA_RANGING,           "TDMA Ranging in progress")\
    SCMA_ENUM_D(5,  FAILED_SCMA_RANGING,    "SCMA Ranging not done or failed")\

#define SCMA_EXPEDITED_Q_DISABLED_REASONS \
    SCMA_ENUM_D(0,  EXPEDITED_Q_NOT_DISABLED,   "Expedited Queue Not Disabled")\
    SCMA_ENUM_D(1,  SCMA_TX_IS_DISABLED,        "SCMA Transmit Was Disabled")\
    SCMA_ENUM_D(2,  EXPEDITED_Q_DISABLED_CFM,   "Expedited Queue disabled via CFM")\
    SCMA_ENUM_D(3,  CHANNEL_CONDITION_ERR,      "Channel Condition Err (power)")\
    SCMA_ENUM_D(4,  HIGH_BURST_ERR_RATE,        "High Burst Error Rate (IGM)")\
    SCMA_ENUM_D(5,  SCMA_NAK_RATE_ERROR,        "High local SCMA Nack Rate")\


#define SCMA_ENUM_D(enumVal, enumName, comment) enumName = enumVal,

typedef enum {

    SCMA_TX_DISABLED_REASONS
    SCMA_TX_DISABLED_MAX
} ulScmaTxDisabledReasons_e;

typedef enum {

    SCMA_EXPEDITED_Q_DISABLED_REASONS
    SCMA_EXPEDITED_Q_DISABLED_MAX
} ulScmaExpeditedQDisabledReasons_e;

#undef SCMA_ENUM_D

/**************************************************************/
/*               SCMA END                                     */
/**************************************************************/

/**************************************************************/
/*               MAC/CAC/KEY  START                           */
/**************************************************************/

enum {CAC_KEYA_INSTANCE = 0, CAC_KEYB_INSTANCE, NUM_OF_KEY_INSTANCE};
#define CAC_KEYA_INSTANCE_AND_KEYB NUM_OF_KEY_INSTANCE

enum { KEY_LENGTH_DES_CBC_64 = 8, KEY_LENGTH_AES_CTR_256 = 32, };
enum { KEY_TYPE_DES_CBC_64 = 0x0, KEY_TYPE_AES_CTR_256 = 0xF, };

#define MAX_NUM_MAC          128
#define MAX_NUM_TCF_SLOTS    128
#define ESK_LEN              KEY_LENGTH_AES_CTR_256    /* bytes */
#define EEMK_LEN             KEY_LENGTH_AES_CTR_256
#define EERMK_LEN             KEY_LENGTH_AES_CTR_256

#define MAX_NUM_MAC_IN_MAC_TABLE (3*MAX_NUM_MAC)  /* account for maximum possibly totally different MAC addresses
                                                     for: KeyA-256, KeyB-256, Clear-256 */
#define MAX_NUM_OUTROUTE_ESK     (MAX_NUM_MAC)
#define MAX_NUM_INROUTE_GROUP    (MAX_INROUTE_GROUPS)
#define MAC_ADDRESS_LEN          6
#define MAX_NUM_INROUTE_ESK      (MAX_NUM_INROUTE_GROUP + 1)

/* Callisto needs a DPP/UPP Decrypt KeyTable to store clear session key
   SWP writes into it, and UPP/DPP reads from */
#define NUM_OF_SID  16

/**************************************************************/
/*               MAC/CAC/KEY  END                               */
/**************************************************************/

/**************************************************************/
/*               UPLINK MISC START                           */
/**************************************************************/
/* rolloff configuration */
#define TX_ROLLOFF_LIST \
    /* MOD_TYPE_D(md_name, md_comment, md_codeblock_size_in_slots, md_encoded_bytes_per_slot, md_bits_per_symbol) */\
    TX_ROLLOFF_D(TX_ROLLOFF_5,   "5", 5,) \
    TX_ROLLOFF_D(TX_ROLLOFF_10, "10", 10,) \
    TX_ROLLOFF_D(TX_ROLLOFF_15, "15", 15,) \
    TX_ROLLOFF_D(TX_ROLLOFF_20, "20", 20,) \
    TX_ROLLOFF_D(TX_ROLLOFF_25, "25", 25,) \
    TX_ROLLOFF_D(TX_ROLLOFF_30, "30", 30,) \
    TX_ROLLOFF_D(TX_ROLLOFF_45, "45", 45,) \
    TX_ROLLOFF_D(TX_ROLLOFF_55, "55", 55,) \

typedef enum {
    TX_ROLLOFF_MIN_INDEX = 0,

    ROLLOFF_DUMMY_1 = -1,  /* so that first rolloff index starts from 0 */
    #define TX_ROLLOFF_D(enumName, valueAsString, percentValue, dummy) enumName##_INDEX,
    TX_ROLLOFF_LIST
    #undef TX_ROLLOFF_D
    ROLLOFF_DUMMY,

    TX_ROLLOFF_MAX_INDEX = ROLLOFF_DUMMY - 1,
} TX_ROLL_OFF_Index_e;

#define VALID_TX_ROLLOFF_INDEX(rolloffIndex)  ((rolloffIndex >= TX_ROLLOFF_MIN_INDEX) && (rolloffIndex <= TX_ROLLOFF_MAX_INDEX))
#define J2WW_LINEAR_DEFAULT_TX_ROLLOFF      25
#define DEFAULT_TX_ROLLOFF                  45
#define DEFAULT_TX_ROLLOFF_TDMA0_OQPSK      45
#define DEFAULT_TX_ROLLOFF_TDMA0_8PSK       25
#define DEFAULT_TX_ROLLOFF_TDMA1            25
#define DEFAULT_TX_ROLLOFF_TDMA2            25

typedef enum {
    #define TX_ROLLOFF_D(enumName, valueAsString, percentValue, dummy) enumName##_PERCENT = percentValue,
    TX_ROLLOFF_LIST
    #undef TX_ROLLOFF_D
} TX_ROLL_OFF_Value_e;


#define INVSINCGAIN_REG            0x33700E64
#define AERO_CW_ICG_VAL            0x0000006D
#define AERO_MOD_ICG_VAL           0x000000D9

/**************************************************************/
/*                UPLINK MISC   END                               */
/**************************************************************/
#endif
