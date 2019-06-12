/************************************************************
 *
 *  FILE NAME:    terminalCommon.H
 *
 *  NOTE: This file is used by user space application and PEP kernel module.
 *        The section of code used exclusively by kernel space application is
 *        enclosed with compiler directive __KERNEL__.
 *
 *  DATE     NAME      REFERENCE        REASON
 *  ----     ----      ---------        ------
 * 02/03/10  N. Rathore                 Removed warnings
 * 03/03/10  P. Malhotra                Added code for PEP
 * 12/04/10  N. Rathore                 Added application ID's
 *                                      to support sysmon
 * 27/04/10  S. Uchil                   Add Dlseq test application ID
 * 28/04/10  R Pathak                   Router advertisement for ipv6
 * 07/05/10  V. Chan                    Add demod process to application list.
 * 18/05/10  M. Baer                    Add uplink application
 * 01/06/10  P. Malhotra                Port SDL
 * 11/06/10  Z. Fan                     Port FLL
 * 28/06/10  S.Uchil                    Rename demod to downlink
 * 19/07/10  R.Pathak                   Add CAC module
 * 04/08/10  K.Deepak                   Added vip changes
 * 22/09/10  K. Deepak                  Incorporated review comments
 * 01/10/10  V Ganesan                  Association Implementation : Phase I
 * 10/20/10  V Ganesan                  Association Phase II: PEP init.
 * 10/27/10  S.Kaur                     Add Diagnostics module
 * 11/09/10  S.Kaur                     Add Web authorization module.
 * 11/17/10  V Ganesan                  Added MAX_APPL_ID for run time loglevel
 *                                      implementation
 * 12/02/10  S.Uchil                    Send modcod adaptation message
 * 12/22/10  D.Sehrawat                 Added IPERF Launcher
 * 03/07/11  V.Ganesan                  PEP in kernel changes.
 * 17/03/11  Atul Gupta                 SPR:907 removed PEP_LINUX directive
 * 18/04/11  B. Wang                    Add sysinfo application related
 * 22/05/11  D. Panwar                  corrected MSTYPE_OFFSET_SYSINFO value
 * 31/05/11  A Gupta                    Added Message IDs for SBC process and registration thread.
 * 01/06/11  B. Wang                    Alocate message for mgmt
 * 10/11/11  V.Chan    S-01292          DAPT can convey stats and commands to the SBC
 * 10/19/11  Atul Gupta                 Event logging porting
 * 10/20/11  V Ganesan                  Application ID should be unique.
 * 04/02/13  M Sharma        		Handling of SNMP Traps
 *
 *     DATE         NAME           REFERENCE        REASON
 *  ----------  -------------   --------------    ----------------------
 *  12-02-2011  zfan            SPR 3290          Implement ODU information parser and API based on new spec.
 *  12-22-2011  V Ganesan                         Application ID/NAME Macro
 *  02-15-2012  O'Neil                            Add _get_appl_num
 *  06-15-2012  D Panwar                          New datatypes for unified macro implementation (CFM splitting)
 *  09-06-2012  Atul Gupta                        DNS Wrapper added
 *  10-17-2012  B. Wang                           Add cbrproxy application related
 *  03-20-2013  zfan                              Merge Callisto and Ganymede downlink_shared.c/h into one file
 *  04-02-2013  ksamavedam                        ROHC Implementation
 *  11-25-2013  ksamavedam                        Added size check macro
 *  01-27-2016  hvargas                           ER6.2 Multicast Development.
 *  03-08-2016  apugaonk                          Merge ER6.1 Gateway Diversity to ER6.2 J2WW Code.
 *  10-04-2017  M.Liu                             Merge DNSProxy feature
 *  05-01-2018  Z. Xu           SPR 22883         Changed value of NO_OF_MSGS_ASSOC to 55.
 *  06-02-2017  ahosalli                          CBOS Development TDM Inroute Changes, added new process TDMControl
 *  05-28-2017  lgagne                            Added PTP definitions
 *  
 * Copyright 2009-2017, Hughes Network Systems, LLC
 * Hughes Proprietary
 *
 ***********************************************************/
#ifndef __TERMINAL_COMMON_H__
#define __TERMINAL_COMMON_H__

#ifndef __KERNEL__

#include <signal.h>
#include <mcheck.h>
#include <stdlib.h>
#ifndef CXXTEST_FLAG
#define LOCAL static
#else
#define LOCAL
#endif /*CXXTEST_FLAG*/

#include "semLib.h"

#include <sys/types.h>
#include <sys/resource.h>
#else
#include <linux/types.h>
#define LOCAL static
#endif

#define MAX_LINE_LEN 255

#define MAX_NUM_VAL 20

#define DWORD UINT32
#define DWORD_DECR DWORD

#ifndef OK
#define OK  0
#endif

#ifndef ERROR
#define ERROR -1
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef STATUS
typedef int STATUS;
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (!FALSE)
#endif

#ifndef	NO_APPL
#define NO_APPL		0
#endif

#ifndef SEND_TO_APPL
#define SEND_TO_APPL	1
#endif

#ifndef COPY_TO_APPL
#define COPY_TO_APPL    2
#endif

#ifndef ULONG
#define ULONG              unsigned long
#endif
#ifndef FUNCPTR
typedef void (*FUNCPTR)(void);
#endif
\


/*************************************************************************
**  MACROS Converting int (32-bit) into string for convenience
    _theStr is the predeifined storage, with size >= 13 bytes
*************************************************************************/
#define INT_TO_STR_WITH_COMMA_DIVIDER(val,_theStr) \
    {\
        if(val/1000000) sprintf(_theStr, "%d,%03d,%03d", val/1000000,abs((val%1000000)/1000),abs(val%1000));\
        else if(val/1000) sprintf(_theStr, "%d,%03d", val/1000, abs(val%1000));\
        else sprintf(_theStr, "%03d", val);\
    }

/*************************************************************************
**  MACROS Converting int (32-bit) into string for convenience
    _theStr is the predeifined storage, with size >= 13 bytes
*************************************************************************/
#define INT_TO_STR_WITH_COMMA_DIVIDER(val,_theStr) \
    {\
        if(val/1000000) sprintf(_theStr, "%d,%03d,%03d", val/1000000,abs((val%1000000)/1000),abs(val%1000));\
        else if(val/1000) sprintf(_theStr, "%d,%03d", val/1000, abs(val%1000));\
        else sprintf(_theStr, "%03d", val);\
    }

/*************************************************************************
**  MACROS Converting timestamp into string for convenience
*************************************************************************/

#define TIMESTAMP_STRING_LEN  (20)

/*e.g., Sep 5 13:24:17 2013 being stored in __timeStr (size of 20 chars including terminator)*/
#define GET_LOCAL_TIME(__timeStr) {\
    time_t __tim=time(NULL); char *__s=ctime(&__tim); int __len=strlen(__s);\
    if(__timeStr != NULL && sizeof (__timeStr) >= __len)\
        memcpy(__timeStr, (__s + 4), strlen(__s)); __timeStr[strlen(__s)-1]=0;\
    }

/* YYYY/MM/DD HH:MM:SS - e.g., 2013/09/05 13:57:06  being stored in __timeStr
  __timeStr is a pointer to char array (i.e., a string) with size >= 20
  I will not (be able to) check the array __timeStr size; you need to make sure its size >= 20.
 __oldTimePtr is the time (time_t) you want me to convert into a string to store it in __timeStr
 if __oldTimePtr is NULL, I will read the current locatime for you and convert it into string */
#define GET_LOCAL_TIME2(__timeStr, __oldTimePtr) \
    {\
    struct tm * __timeinfo;\
    time_t __tim;\
    if(!strcmp(#__oldTimePtr, "NULL"))\
    {\
        __tim=time(NULL);\
        __timeinfo = localtime(&__tim);\
    }\
    else \
        __timeinfo = localtime(__oldTimePtr);\
                 strftime (__timeStr, TIMESTAMP_STRING_LEN, "%Y/%m/%d %H:%M:%S", __timeinfo);\
    }

/*  MM/DD/YY HH:MM:SS - e.g., 09/05/2013 13:57:06  being stored in __timeStr */
#define GET_LOCAL_TIME3(__timeStr, __oldTimePtr) \
    {\
        struct tm * __timeinfo;\
        time_t __tim;\
        if(!strcmp(#__oldTimePtr, "NULL"))\
        {\
            __tim=time(NULL);\
            __timeinfo = localtime(&__tim);\
        }\
        else \
            __timeinfo = localtime(__oldTimePtr);\
        strftime (__timeStr, TIMESTAMP_STRING_LEN, "%m/%d/%Y %H:%M:%S", __timeinfo);\
    }

/* MM/DD HH:MM:SS - e.g., 09/05 13:57:06  being stored in __timeStr
  __timeStr is a pointer to char array (i.e., a string) with size >= 20
  I will not (be able to) check the array __timeStr size; you need to make sure its size >= 20.
 __oldTimePtr is the time (time_t) you want me to convert into a string to store it in __timeStr
 if __oldTimePtr is NULL, I will read the current locatime for you and convert it into string */
#define GET_LOCAL_TIME4(__timeStr, __oldTimePtr) \
    {\
        struct tm * __timeinfo;\
        time_t __tim;\
        if(!strcmp(#__oldTimePtr, "NULL"))\
        {\
            __tim=time(NULL);\
            __timeinfo = localtime(&__tim);\
        }\
        else \
            __timeinfo = localtime(__oldTimePtr);\
        strftime (__timeStr, TIMESTAMP_STRING_LEN, "%m/%d %H:%M:%S", __timeinfo);\
    }

/*************************************************************************/
#ifndef SOCK_INVALID
#define SOCK_INVALID  (-1)
#endif

#define MACRO_QUOTE(name) #name
#define MACRO_STR(macro) MACRO_QUOTE(macro)

#define MODULE_CREATION_TIME(x)  char * creationTime = MACRO_STR(x)

/* To make sure that all the modules use MODULE_CREATION_TIME */
extern  char * creationTime ;

#ifndef boolean
typedef unsigned char       boolean;
#endif

/*
 *  BYTE, WORD, DWORD and ADDRESS type definitions
 */
typedef unsigned char       byte;
typedef unsigned short int  word;
typedef unsigned long       dword;
typedef long long           qword;

typedef dword               ip_addr_t;

typedef short int       sword;
typedef long            sdword;

/*************************************************************************
**  these datatypes to be used for those variables, whoes bit width needs
**  to be the same irrespective of machine architecture.
*************************************************************************/
typedef int8_t     INT8_T;
typedef u_int8_t   UINT8_T;
typedef int16_t    INT16_T;
typedef u_int16_t  UINT16_T;
typedef int32_t    INT32_T;
typedef u_int32_t  UINT32_T;
typedef int64_t    INT64_T;
typedef u_int64_t  UINT64_T;

/* Yet another set */
typedef int8_t     INT8;
typedef u_int8_t   UINT8;
typedef int16_t    INT16;
typedef u_int16_t  UINT16;
typedef int32_t    INT32;
typedef u_int32_t  UINT32;
typedef int64_t    INT64;
typedef u_int64_t  UINT64;


/*************************************************************************/

typedef dword               IP_ADDR;
typedef dword               IPV4_ADDR;
typedef struct in6_addr     IP6_ADDR;

#define NULL_PTR                   0L

#define SIZEOF_WORD                sizeof(word)
#define SIZEOF_BYTE                sizeof(byte)
#define SIZEOF_DWORD               sizeof(dword)
#define SIZEOF_QWORD               sizeof(qword)
#define SIZEOF_WORD_DECR           SIZEOF_WORD
#define SIZEOF_BYTE_DECR           SIZEOF_BYTE
#define SIZEOF_DWORD_DECR          SIZEOF_DWORD
#define SIZEOF_IP_ADDR             sizeof(ip_addr_t)
#define SIZEOF_IP6_ADDR            (16)
#define IP6_ADDR_STR_LEN           (INET6_ADDRSTRLEN)
#define MAX_HOSTNAME_IPADDR_LEN    (3*INET6_ADDRSTRLEN)
#define SIZEOF_FLOAT               sizeof(double)
#define SIZEOF_SFLOAT              sizeof(float)

//#define IP6_ADDR_MAX_STR_LEN       (40)
#define GET_UNIQ_NAME( name , uniqueName)       {\
   sprintf(uniqueName,"%s",name);}

#define CONCAT_STR(a, b) { a b }

#ifndef __KERNEL__
static __inline__ void disable_coredumps()
{
    struct rlimit rlim = {1, 1}; /* when /proc/sys/kernel/core_pattern is setup to pipe cores to a
                                    program (as ours is), then RLIMIT_CORE can be set to 1 to tell
                                    the kernel to disable core dumps for the process. */
    setrlimit(RLIMIT_CORE, &rlim);
}
#endif

#ifndef __KERNEL__
#define MTRACE_ENV "/tmp/mtrace_%s.txt"

/*Mtrace signal handler functions*/
#define MTRACE_HDLR_FUNC() \
  void mtrace_sig_hdlr(int x){\
        static int flag =0;\
        if(flag ==0){\
                char mt_env[100] ;\
                sprintf(mt_env, MTRACE_ENV,_procName) ;\
                setenv("MALLOC_TRACE",mt_env,1);\
                mtrace();\
                flag =1;}\
        else{\
                muntrace();\
                flag=0;\
        }\
}\

#define INSTALL_MTRACE_SIG_HDLR() signal(31,mtrace_sig_hdlr)
#endif

/*
 * Note we need the 2 concats below because arguments to ##
 * are not expanded, so we need to expand __LINE__ with one indirection
 * before doing the actual concatenation.
*/
#define STRUCT_SIZE_ASSERT_CONCAT_(a, b) a##b
#define STRUCT_SIZE_ASSERT_CONCAT(a, b) STRUCT_SIZE_ASSERT_CONCAT_(a, b)

/*
 * This macro has no runtime side effects as it just defines an enum whose name depends
 * on the current line, and whose value will give a divide by zero error at compile time if
 * the assertion is false. If you try to SIZE_CHECK() a non constant expression by mistake,
 * this will also give an error.
 * Using this macro you can create a compile time check at any scope
 *
*/
#define STRUCT_SIZE_CHECK(e,m) enum { STRUCT_SIZE_ASSERT_CONCAT(m, __LINE__) = 1/(!!(e)) }






/*
 * Note we need the 2 concats below because arguments to ##
 * are not expanded, so we need to expand __LINE__ with one indirection
 * before doing the actual concatenation.
*/
#define STRUCT_SIZE_ASSERT_CONCAT_(a, b) a##b
#define STRUCT_SIZE_ASSERT_CONCAT(a, b) STRUCT_SIZE_ASSERT_CONCAT_(a, b)

/*
 * This macro has no runtime side effects as it just defines an enum whose name depends
 * on the current line, and whose value will give a divide by zero error at compile time if
 * the assertion is false. If you try to SIZE_CHECK() a non constant expression by mistake,
 * this will also give an error.
 * Using this macro you can create a compile time check at any scope
 *
*/
#define STRUCT_SIZE_CHECK(e,m) enum { STRUCT_SIZE_ASSERT_CONCAT(m, __LINE__) = 1/(!!(e)) }




/* APPL ID and NAME MACRO:
** The following macros make sure that the Application ID
** and the name are defined in one place so that they are never out of
** sync.
*/
#define DEF_APPL_LIST                         \
     DEF_APPL_ID_R_NAME(SHM,=1)               \
     DEF_APPL_ID_R_NAME(MR,)                  \
     DEF_APPL_ID_R_NAME(TFW,)                 \
     DEF_APPL_ID_R_NAME(LFW,)                 \
     DEF_APPL_ID_R_NAME(LED,)                 \
     DEF_APPL_ID_R_NAME(CFM,)                 \
     DEF_APPL_ID_R_NAME(AMU,)                 \
     DEF_APPL_ID_R_NAME(TMU,)                 \
     DEF_APPL_ID_R_NAME(DLSEQ,)               \
     DEF_APPL_ID_R_NAME(DLTEST,)              \
     DEF_APPL_ID_R_NAME(DOWNLINK,)            \
     DEF_APPL_ID_R_NAME(UPLINK,)              \
     DEF_APPL_ID_R_NAME(SDLMU,)               \
     DEF_APPL_ID_R_NAME(FLL,)                 \
     DEF_APPL_ID_R_NAME(CAC,)                 \
     DEF_APPL_ID_R_NAME(VMU,)                 \
     DEF_APPL_ID_R_NAME(ASSOC,)               \
     DEF_APPL_ID_R_NAME(DIAG,)                \
     DEF_APPL_ID_R_NAME(WEB,)                 \
     DEF_APPL_ID_R_NAME(IPERF_LAUNCHER,)      \
     DEF_APPL_ID_R_NAME(SYSINFO,)             \
     DEF_APPL_ID_R_NAME(SBC,)                 \
     DEF_APPL_ID_R_NAME(REGISTRATION,)        \
     DEF_APPL_ID_R_NAME(SBC_CGI,)             \
     DEF_APPL_ID_R_NAME(DAPT,)                \
     DEF_APPL_ID_R_NAME(EVTMGR,)              \
     DEF_APPL_ID_R_NAME(DDNSC,)               \
     DEF_APPL_ID_R_NAME(VPT_MGR,)             \
     DEF_APPL_ID_R_NAME(ODU,)                 \
     DEF_APPL_ID_R_NAME(BIST,)                \
     DEF_APPL_ID_R_NAME(VSAT_TEST,)           \
     DEF_APPL_ID_R_NAME(WIT,)                 \
     DEF_APPL_ID_R_NAME(FWLOAD,)              \
     DEF_APPL_ID_R_NAME(SIP,)                 \
     DEF_APPL_ID_R_NAME(CBRPROXY,)            \
     DEF_APPL_ID_R_NAME(AGENTX,)              \
     DEF_APPL_ID_R_NAME(DISEQC,)              \
     DEF_APPL_ID_R_NAME(ODUID,)               \
     DEF_APPL_ID_R_NAME(DISCLT0,)             \
     DEF_APPL_ID_R_NAME(DISCLT1,)             \
     DEF_APPL_ID_R_NAME(DISCLT2,)             \
     DEF_APPL_ID_R_NAME(DISCLT3,)             \
     DEF_APPL_ID_R_NAME(DISCLT4,)             \
     DEF_APPL_ID_R_NAME(DISEQCWRITE2,)        \
     DEF_APPL_ID_R_NAME(DISEQCREAD2,)         \
     DEF_APPL_ID_R_NAME(DAPT2,)               \
     DEF_APPL_ID_R_NAME(DOWNLINK2,)           \
     DEF_APPL_ID_R_NAME(SPDTSTSRVR,)          \
     DEF_APPL_ID_R_NAME(UPLINK2,)             \
     DEF_APPL_ID_R_NAME(TVOA,)                \
     DEF_APPL_ID_R_NAME(TVM,)                 \
     DEF_APPL_ID_R_NAME(UPLINK_UPP_READ,)     \
     DEF_APPL_ID_R_NAME(TRANSPORTABLE,)       \
     DEF_APPL_ID_R_NAME(PCAP,)                \
     DEF_APPL_ID_R_NAME(TERMINAL_API,)        \
     DEF_APPL_ID_R_NAME(LUI_API,)             \
     DEF_APPL_ID_R_NAME(BOARD_TEST_MAIN,)     \
     DEF_APPL_ID_R_NAME(VSATTESTUDP,)         \
     DEF_APPL_ID_R_NAME(TOM,)                 \
     DEF_APPL_ID_R_NAME(WIFI_MGR,)            \
     DEF_APPL_ID_R_NAME(WIFI_MGR_FACT,)       \
     DEF_APPL_ID_R_NAME(DMD_HW_API_CLIENT,)   \
     DEF_APPL_ID_R_NAME(DMD_HW_API_SRV,)      \
     DEF_APPL_ID_R_NAME(IGMP,)                \
     DEF_APPL_ID_R_NAME(MLD,)                 \
     DEF_APPL_ID_R_NAME(SHMON,)               \
     DEF_APPL_ID_R_NAME(RTM,)                 \
     DEF_APPL_ID_R_NAME(PEP_STATS_MOD,)       \
     DEF_APPL_ID_R_NAME(IPFD,)                \
     DEF_APPL_ID_R_NAME(JUP_AERO_BEAM_SELECT,)\
     DEF_APPL_ID_R_NAME(NPU_JUP_MSG,)		  \
     DEF_APPL_ID_R_NAME(AERO_AMIP,)		      \
     DEF_APPL_ID_R_NAME(BEAM_SWITCH,)		  \
     DEF_APPL_ID_R_NAME(AERO_UDP_TRACE,)	  \
     DEF_APPL_ID_R_NAME(MAI_MOD,)             \
     DEF_APPL_ID_R_NAME(DNSPROXY,)            \
     DEF_APPL_ID_R_NAME(TDM_UPLINK,)          \
     DEF_APPL_ID_R_NAME(LOAD_CBM_FPGA,)       \
     DEF_APPL_ID_R_NAME(PTPAPP,)              \
     DEF_APPL_ID_R_NAME(UPLINK_ODU_CAL,)      \
     DEF_APPL_ID_R_NAME(UPLINK_ODU_READ,)     \
     DEF_APPL_ID_R_NAME(BIST2,)               \
     DEF_APPL_ID_R_NAME(AERO_NMEA_GPS,)       \
	 DEF_APPL_ID_R_NAME(AERO_ROAMING,)		  \

#define DEF_APPL_ID_R_NAME(NAME,VAL) NAME##_APPL_ID VAL,
typedef enum
{
    DEF_APPL_LIST
    MAX_APPL_ID
} appl_id_e;
#undef DEF_APPL_ID_R_NAME

#ifndef __KERNEL__
#define DEF_APPL_ID_R_NAME(NAME,VAL) #NAME,
static const char *appl_name[] = {"BCAST",DEF_APPL_LIST "INVALID"};
#undef DEF_APPL_ID_R_NAME

static __inline__ const char *_get_appl_name(int appl_id)
{
    if ((appl_id >= 0) && (appl_id < MAX_APPL_ID))
        return(appl_name[appl_id]);
    else
        return(appl_name[MAX_APPL_ID]);
}

/*Inverse of the above -- or -1*/
static __inline__ int _get_appl_num(char *name)
{
    int i;

    for (i=0; i<MAX_APPL_ID; i++)
        if (strcmp(name, appl_name[i]) == 0)
            return i;
    return -1;
}
#endif

#ifndef __KERNEL__
#define SECONDS_PER_DAY     86400
#define SECONDS_PER_HOUR    3600
#define SECONDS_PER_MINUTE  60

/* Should be moved to terminalcommon.h */

struct timeDuration_t {
    unsigned int days;
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;
};

/***************************************************************************
 * \skipline FUNCTION NAME:  _secondsToTime
 *
 * \brief   Converts frame count into time (d:h:m:s)
 *
 * @param (unsigned int) seconds - Number of seconds
 * @param (struct timeDuration_t *) timeDuration - Holds time info
 *
 * \return void
****************************************************************************/
static __inline__ void _secondsToTime(unsigned int seconds, struct timeDuration_t * timeDuration)
{
    if (timeDuration && seconds)
    {
        unsigned int remainder = 0;

        memset(timeDuration, 0, sizeof(struct timeDuration_t));

        timeDuration->days = seconds / SECONDS_PER_DAY;
        remainder = seconds % SECONDS_PER_DAY;

        timeDuration->hours = remainder / SECONDS_PER_HOUR;
        remainder = remainder % SECONDS_PER_HOUR;

        timeDuration->minutes = remainder / SECONDS_PER_MINUTE;
        remainder = remainder % SECONDS_PER_MINUTE;

        timeDuration->seconds = remainder;
    }
}
#endif

/* Interface types for message routing */

#define IF_TYPE_INTERNAL        (0)  /*  Internal messages */

/* Total number of messages in each appliaction */

#define NO_OF_MSGS_TFW          (20)
#define NO_OF_MSGS_LFW          (20)
#define NO_OF_MSGS_LED          (20)
#define NO_OF_MSGS_TEST         (20)
#define NO_OF_MSGS_TEST2        (20)
#define NO_OF_MSGS_TEST3        (20)
#define NO_OF_MSGS_SHM          (20)
#define NO_OF_MSGS_CCPM         (20)
#define NO_OF_MSGS_CPI          (1)
#define NO_OF_MSGS_CFM          (20)
#define NO_OF_MSGS_DLSEQ        (20)
#define NO_OF_MSGS_SDL          (20)
#define NO_OF_MSGS_CAC          (20)
#define NO_OF_MSGS_ASSOC        (57)
#define NO_OF_MSGS_UPLINK       (20)
#define NO_OF_MSGS_DOWNLINK     (20)
#define NO_OF_MSGS_SYSINFO      (40)
#define NO_OF_MSGS_DIAG         (10)
#define NO_OF_MSGS_SBC          (40)
#define NO_OF_MSGS_REGISTRATION (10)
#define NO_OF_MSGS_EVTMGR       (40)
#define NO_OF_MSGS_VPT          (10)
#define NO_OF_MSGS_DDNSC        (20)
#define NO_OF_MSGS_AMU          (20)
#define NO_OF_MSGS_CBRPROXY     (10)
#define NO_OF_MSGS_AGENTX       (5)
#define NO_OF_MSGS_DISEQC       (5)
#define NO_OF_MSGS_MGC          (2)
#define NO_OF_MSGS_DNSPROXY     (10)
#define NO_OF_MSGS_TOM          (8)
#define NO_OF_MSGS_WIFI_MGR     (20)
#define NO_OF_MSGS_WIFI_MGR_FACT (20)
#define NO_OF_MSGS_SHMON        (10)
#define NO_OF_MSGS_PEP_STATS    (20)
#define NO_OF_MSGS_NPU_JUP_MSG  (20)
#define NO_OF_MSGS_AERO_BEAM    (10)
#define NO_OF_MSGS_AERO_AMIP    (10)
#define NO_OF_MSGS_MAI          (20)
#define NO_OF_MSGS_IGMP         (5)
#define NO_OF_MSGS_PTP          (10)
#define NO_OF_MSGS_AERO_STATS   (10)
#define NO_OF_MSGS_IPF          (5)

/* Message Type Offsets */

#define MSTYPE_OFFSET_TFW               (0)
#define MSTYPE_OFFSET_LFW               (MSTYPE_OFFSET_TFW + NO_OF_MSGS_TFW)
#define MSTYPE_OFFSET_TEST              (MSTYPE_OFFSET_LFW + NO_OF_MSGS_LFW)
#define MSTYPE_OFFSET_TEST2             (MSTYPE_OFFSET_TEST + NO_OF_MSGS_TEST)
#define MSTYPE_OFFSET_TEST3             (MSTYPE_OFFSET_TEST2 + NO_OF_MSGS_TEST2)
#define MSTYPE_OFFSET_SHM               (MSTYPE_OFFSET_TEST3 + NO_OF_MSGS_TEST3)
#define MSTYPE_OFFSET_CCPM              (MSTYPE_OFFSET_SHM + NO_OF_MSGS_SHM)
#define MSTYPE_OFFSET_LED               (MSTYPE_OFFSET_CCPM + NO_OF_MSGS_CCPM)
#define MSTYPE_OFFSET_CFM               (MSTYPE_OFFSET_LED + NO_OF_MSGS_LED)
#define MSTYPE_OFFSET_DLSEQ             (MSTYPE_OFFSET_CFM + NO_OF_MSGS_CFM)
#define MSTYPE_OFFSET_SDL               (MSTYPE_OFFSET_DLSEQ + NO_OF_MSGS_DLSEQ)
#define MSTYPE_OFFSET_CAC               (MSTYPE_OFFSET_SDL + NO_OF_MSGS_SDL)
#define MSTYPE_OFFSET_ASSOC             (MSTYPE_OFFSET_CAC + NO_OF_MSGS_CAC)
#define MSTYPE_OFFSET_UPLINK            (MSTYPE_OFFSET_ASSOC + NO_OF_MSGS_ASSOC)
#define MSTYPE_OFFSET_DOWNLINK          (MSTYPE_OFFSET_UPLINK + NO_OF_MSGS_UPLINK)
#define MSTYPE_OFFSET_SYSINFO           (MSTYPE_OFFSET_DOWNLINK + NO_OF_MSGS_DOWNLINK)
#define MSTYPE_OFFSET_DIAG              (MSTYPE_OFFSET_SYSINFO + NO_OF_MSGS_SYSINFO)
#define MSTYPE_OFFSET_SBC               (MSTYPE_OFFSET_DIAG + NO_OF_MSGS_DIAG)
#define MSTYPE_OFFSET_REGISTRATION      (MSTYPE_OFFSET_SBC + NO_OF_MSGS_SBC)
#define MSTYPE_OFFSET_EVTMGR            (MSTYPE_OFFSET_REGISTRATION + NO_OF_MSGS_REGISTRATION)
#define MSTYPE_OFFSET_VPT               (MSTYPE_OFFSET_EVTMGR + NO_OF_MSGS_EVTMGR)
#define MSTYPE_OFFSET_DDNSC             (MSTYPE_OFFSET_VPT + NO_OF_MSGS_VPT)
#define MSTYPE_OFFSET_AMU               (MSTYPE_OFFSET_DDNSC + NO_OF_MSGS_DDNSC)
#define MSTYPE_OFFSET_CBRPROXY          (MSTYPE_OFFSET_AMU + NO_OF_MSGS_AMU)
#define MSTYPE_OFFSET_AGENTX    	    (MSTYPE_OFFSET_CBRPROXY + NO_OF_MSGS_CBRPROXY)
#define MSTYPE_OFFSET_DISEQC            (MSTYPE_OFFSET_AGENTX + NO_OF_MSGS_AGENTX)
#define MSTYPE_OFFSET_MGC               (MSTYPE_OFFSET_DISEQC + NO_OF_MSGS_DISEQC)
#define MSTYPE_OFFSET_DNSPROXY          (MSTYPE_OFFSET_MGC + NO_OF_MSGS_MGC)
#define MSTYPE_OFFSET_TOM               (MSTYPE_OFFSET_DNSPROXY + NO_OF_MSGS_DNSPROXY)
#define MSTYPE_OFFSET_WIFI_MGR          (MSTYPE_OFFSET_TOM + NO_OF_MSGS_TOM)
#define MSTYPE_OFFSET_WIFI_MGR_FACT     (MSTYPE_OFFSET_WIFI_MGR + NO_OF_MSGS_WIFI_MGR)
#define MSTYPE_OFFSET_SHMON             (MSTYPE_OFFSET_WIFI_MGR_FACT + NO_OF_MSGS_WIFI_MGR_FACT)
#define MSTYPE_OFFSET_PEP_STATS         (MSTYPE_OFFSET_SHMON + NO_OF_MSGS_SHMON)
#define MSTYPE_OFFSET_MAI               (MSTYPE_OFFSET_PEP_STATS + NO_OF_MSGS_PEP_STATS)
#define MSTYPE_OFFSET_IGMP              (MSTYPE_OFFSET_MAI + NO_OF_MSGS_MAI)
#define MSTYPE_OFFSET_PTP               (MSTYPE_OFFSET_IGMP + NO_OF_MSGS_IGMP)
#define MSTYPE_OFFSET_NPU_JUP_MSG       (MSTYPE_OFFSET_PTP + NO_OF_MSGS_PTP)
#define MSTYPE_OFFSET_AERO_BEAM_SWITCH  (MSTYPE_OFFSET_NPU_JUP_MSG + NO_OF_MSGS_NPU_JUP_MSG)
#define MSTYPE_OFFSET_AERO_AMIP         (MSTYPE_OFFSET_AERO_BEAM_SWITCH + NO_OF_MSGS_AERO_BEAM)
#define MSTYPE_OFFSET_AERO_UDP_STATS    (MSTYPE_OFFSET_AERO_AMIP + NO_OF_MSGS_AERO_AMIP)
#define MSTYPE_OFFSET_AERO_IPF          (MSTYPE_OFFSET_AERO_UDP_STATS + MSTYPE_OFFSET_AERO_UDP_STATS)

#ifndef BYTE
typedef unsigned char  BYTE;
#endif
#define BYTE_DECR BYTE


#ifndef SBYTE
typedef char  SBYTE;
#endif

#define BYTE_DECR BYTE


#ifndef WORD
typedef unsigned short WORD;
#endif
#define WORD_DECR WORD

#ifndef __KERNEL__
#ifndef PFN
typedef void           (*PFN)();
#endif

#ifndef TM_DESC
typedef byte TM_DESC;
#endif

#ifndef SEM_DESC
typedef SEM_ID SEM_DESC;
#endif
#endif

/* Get maximum of two signed values. */
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

/* Get minimum of two signed values. */
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif


/* Added for PEP compilation, moved from ipre.h */

#define CHECK_DF_BIT 0x4000   /* DF bit */
#define CHECK_M_BIT  0x2000   /* M(ore) bit */
#define FRAG_OFF     0x1fff   /* actual frag offset */

/* SNMP query type defines */
#define GET_EXACT           0xA0
#define GET_NEXT            0xA1


typedef struct
{
#ifdef LITTLE_ENDIAN

#ifdef NT
    BYTE    hlen:4,
            ver:4;
    BYTE    tos;
    WORD    total_len;
#else
    unsigned int  hlen      : 4,
                  ver       : 4,
                  tos       : 8,
                  total_len : 16;
#endif /* NT */

#else /* Big endian */
    unsigned int  ver       : 4,
                  hlen      : 4,
                  tos       : 8,
                  total_len : 16;
#endif /* LITTLE_ENDIAN */
    WORD    id;
    WORD    frag_off;
    BYTE    ttl;
    BYTE    prot;
    WORD    chksum;
    IP_ADDR src_ip;
    IP_ADDR dest_ip;
    BYTE    data[2];
} IP, *PIP;


typedef enum _ipv6_addr_type
{
    IPV6_LAN_ADDR_TYPE=1,
    IPV6_WAN_ADDR_TYPE,
    IPV6_MGMT_ADDR_TYPE,
    IPV6_MAX_ADDR_TYPE
} ipv6_addr_type_t;

// Although it belongs in sysinfo.h, it is defined here
// to avoid cyclic dependency and to prevent kernel module compilation
// problem
#define SRVC_CODE_POINT_ADDR_LEN     20


#ifdef CALLISTO_PLATFORM
#define TERMINAL_MODEL "HT1100"
#else
#define TERMINAL_MODEL "HT1000"
#endif


#endif  /* __TERMINAL_COMMON_H__  */
