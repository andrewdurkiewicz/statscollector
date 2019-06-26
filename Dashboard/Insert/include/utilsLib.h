/***********************************************************************
 *  FILE NAME: utilsLib.h
 *
 *  DESCRIPTION: I provide general utility interface definition
 *
 **********************************************************************/

#ifndef _UTILS_H
#define _UTILS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "terminalCommon.h"
#include "uldefs.h"
#include "gpio.h"

#define ESN_CFG_FILE_PATH     "/fl0/esn.cfg"
#define SAI_CFG_FILE_PATH     "/fl0/config/sai.cfg"


/*********************************************************************
    Board specific based on strapping:
    Doc# H52986
    Jupiter IDU Board Strapping Document
    Current Version 03, February 3, 2016

strap   BOM Number        Configuration                             Notes
----- ------------     ------------------------------------   -----------------------
0x41  1504889-0005      Jupiter 2 Evaluation IDU
0x02  1505048-0005      HT2000 - Proto
0x43  1505048-0102      HT2000
0x44  1505812-0103      HT2000W
0xC5  1505471-0050      Jupiter 2  Aero Prototype
0xC6  1505471-xxxx      Jupiter 2  Aero Production
0x47  1505048-0005      HT2000 Prototype
0x48  1505608-0005      Jupiter 2 Enterprise  Proto
0x49  1505608-0018
0x4A  1505608-0118
0x4B  1505608-0119
0x4C  1505812-0102

*********************************************************************/

/* * * Important:  Please read the guidlines, before make any changes  * * */

/* (1) Each board type has its own (Hardware) Module Spec. defining its unique properties

   (2) Direct use of Board type enumeric name in .c/.cc files is STRICTLY PROHIBITED.
       E.g, you are not allowed to use enumeric constant name HT1100_PRODUCTION in any .c/.cc files.

       Instead, you should use various utility wrappers defined by inline functions or Macros for
       a concrete property or hardware module you need to deal with.

       E.g., use IS_SYNTH_TYPE_HMC() to configure the HMC syntheziers stuffed in these types of boards

   (3) While creating the macros or inline funcitons, youe MUST follow the naming convention,
       (always starting with "IS_BOARD_TYPE_"): IS_BOARD_TYPE_XXX..._XXX()

       e.g., for those boards requiring configuarion of the duty cycle via 27 MHZ, we name it as

       IS_BOARD_TYPE_27MHZ_DUTY_CYCLE()

   (4) Any violation of the guideline is treated as coding error and should be corrected;
      legacy code will be fixed with time.

    * * * To add a new board type * * *
    (1) add the new board type into the BOARD_TYPE_LIST and follow the same naming convention
    (2) Go through each utility Macro/inline function to decide if this board type should be included
    (3) Or if need to create a new MAcro/inline function for the new property from this new board following the
        naming convention.
*/

/* strapping value can be override with parm in /fl0/cimcfg.b: for example,
   iduBoardStrappingVal=0x1234 */
#define IDU_BOARDSTRAPPINGVAL_CFG_FILE_PATH     "/fl0/cimcfg.b"

 /* _BT_D(EnumName, boardStrapVal, boardClass, comment, ddrMemSizeInMB) */

#ifndef THEBE_PLATFORM  /* for Sahara ASIC */
#define BOARD_TYPE_LIST \
  _BT_D(IDU_UNKNOWN_TYPE,  0x00, HT1100, "Unknown IDU Type (treated as HT1100)",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(HT1100_PRODUCTION, 0x03, HT1100, "HT1100 Production",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(HT1100_PROTOTYPE, 0x0B, HT1100, "HT1100 Prototype",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
\
  _BT_D(KALE_SATURATED_PROTOTYPE, 0x05, KALE, "Kale Saturated Prototype",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(KALE_LINEAR_DUAL_IFL_PROTOTYPE, 0x06, KALE, "Kale Linear Dual IFL Prototype",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(KALE_LINEAR_SINGLE_IFL_PROTOTYPE, 0x07, KALE, "Kale Linear Single IFL Prototype",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(KALE_LINEAR_DUAL_IFL_PROTOTYPE_ENT, 0x08, KALE, "Kale Linear Dual IFL Prototype Enterprise",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
\
  _BT_D(KALE_SATURATED_PRODUCTION, 0x0F, KALE, "Kale Saturated Production",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(KALE_LINEAR_DUAL_IFL_PRODUCTION_ENT, 0x12, KALE, "Kale Linear Dual IFL Production Enterprise",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(KALE_SATURATED_PRODUCTION_ENT, 0x13, KALE, "Kale Saturated Production Enterprise",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
\
  _BT_D(KALE_RC_SATURATED_PROTOTYPE, 0x02, KALERC, "Kale RC Saturated Prototype",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE, 0x04, KALERC, "Kale RC Linear Dual IFL Prototype",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(KALE_RC_LINEAR_SINGLE_IFL_PROTOTYPE, 0x09, KALERC, "Kale RC Linear Single IFL Prototype",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_ENT, 0x0A, KALERC, "Kale RC Linear Dual IFL Prototype Enterprise",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
\
  _BT_D(KALE_RC_SATURATED_PRODUCTION, 0x0C, KALERC, "Kale RC Saturated Production",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT, 0x0D, KALERC, "Kale RC Linear Dual IFL Production Enterprise",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(KALE_RC_SATURATED_PRODUCTION_ENT, 0x0E, KALERC, "Kale RC Saturated Production Enterprise",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
\
  _BT_D(KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT_REDESIGNED, 0x10, KALERC, "Kale RC Linear Dual IFL Production Enterprise Redesigned",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(KALE_RC_SATURATED_PRODUCTION_ENT_REDESIGNED, 0x11, KALERC, "Kale RC Saturated Production Enterprise Redesigned",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_RACK_MOUNT, 0x14, KALERC, "Kale RC Linear Dual IFL Production Rack Mount",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
\
  _BT_D(KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_AERO, 0x16, KALERC, "Kale RC Linear Dual IFL Production AERO",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_AERO, 0x17, KALERC, "Kale RC Linear Dual IFL Protptype AERO",DDR_MEM_SIZE_2_GBIT, GPIO_LIST_DEFAULT)\

#else  /* for Acadia ASIC */

#define BOARD_TYPE_STRAPPING_MASK          (0x3F) /* least significant 6 bits for board type */
#define BOARD_DDR_MEM_SIZE_STRAPPING_MASK  (0xC0) /* most significant 2 bits for DDR memory size */

#define BOARD_TYPE_LIST \
  /* Note: for THEBE_PLATFORM, boardTypeStrapVal is least significant 6 bits; other 2 bits is for memory size;
    But, we don't rely on this, since it may change in the future */\
   /* _BT_D(boardTypeEnum,  boardTypeStrapVal, boardTypeClass, Comment) */\
  _BT_D(IDU_UNKNOWN_TYPE,              0x00, HT2000, "Unknown IDU Type (treated as HT2000)",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_DEFAULT)\
  _BT_D(HT2000_PROTOTYPE_3RD_NYQUIST,  0x02, HT2000, "HT2000 Prototype with 3rd Nyquist Support",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_1)\
\
  _BT_D(HT2000_PRODUCTION,             0x43, HT2000, "HT2000 Production",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_1)\
  _BT_D(HT2000_WIFI,                   0x44, HT2000, "HT2000 Production with builtin WIFI",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_2)\
  _BT_D(HT2000_PRODUCTION_SBD_REDESIGN,0x53, HT2000, "HT2000 Production with SBD support ",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_3)\
\
  _BT_D(HT2000_WIFI_A,                 0x4D, HT2000, "HT2000 Production with builtin WIFI A",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_2)\
  _BT_D(HT2000_WIFI_A_SBD_REDESIGN,    0x54, HT2000, "HT2000 Production with builtin WIFI A and SBD support",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_3)\
\
  _BT_D(HT2000_PROTOTYPE,              0x47, HT2000, "HT2000 Prototype",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_1)\
  _BT_D(HT2000_PRODUCTION_REDESIGN,    0x4C, HT2000, "HT2000 Production Redesigned",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_2)\
\
  _BT_D(JUP2_EVALUATION_IDU,           0x41, JUP2, "Jupiter-2 Evaluation",DDR_MEM_SIZE_8_GBIT, GPIO_LIST_0)\
\
  _BT_D(JUP2_MULTIMODE,                0x57, JUP2, "Jupiter-2 MultiMode",DDR_MEM_SIZE_16_GBIT, GPIO_LIST_9)\
  _BT_D(JUP2_MULTIMODE_B,              0x58, JUP2, "Jupiter-2 MultiMode B",DDR_MEM_SIZE_16_GBIT, GPIO_LIST_9)\
\
  _BT_D(JUP2_MULTIMODE_ENT_LINEAR_PHASE2,     0xD9, JUP2, "Jupiter-2 MultiMode Enterprise Linear Phase2",       DDR_MEM_SIZE_16_GBIT, GPIO_LIST_8)\
  _BT_D(JUP2_MULTIMODE_ENT_LINEAR_PHASE2_TDM, 0xDA, JUP2, "Jupiter-2 MultiMode Enterprise Linear Phase2 TDM",   DDR_MEM_SIZE_16_GBIT, GPIO_LIST_8)\
  _BT_D(JUP2_MULTIMODE_ENT_LINEAR_PHASE2_PTP, 0xDC, JUP2, "Jupiter-2 MultiMode Enterprise Linear Phase2 PTP",   DDR_MEM_SIZE_16_GBIT, GPIO_LIST_8)\
\
  _BT_D(JUP2_AERO_PROTOTYPE,           0xC5, JUP2, "Jupiter-2 Aero Prototype",DDR_MEM_SIZE_16_GBIT, GPIO_LIST_4)\
  _BT_D(JUP2_AERO_PRODUCTION,          0xC6, JUP2, "Jupiter-2 Aero Production",DDR_MEM_SIZE_16_GBIT, GPIO_LIST_4)\
\
  _BT_D(JUP2_ENT_LINEAR_PROTOTYPE,     0x48, JUP2, "Jupiter-2 Enterprise Linear Prototype",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_5)\
  _BT_D(JUP2_ENT_LINEAR_PRODUCTION,    0x49, JUP2, "Jupiter-2 Enterprise Linear Production",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_5)\
  _BT_D(JUP2_ENT_LINEAR_RACK_MOUNT,    0x4A, JUP2, "Jupiter-2 Enterprise Linear Rack Mount Production",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_5)\
  _BT_D(JUP2_ENT_SATURATED_RACK_MOUNT, 0x4B, JUP2, "Jupiter-2 Enterprise Saturated Rack Mount Production",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_6)\
\
  _BT_D(JUP2_ENT_LINEAR_PROTOTYPE_A,     0x4E, JUP2, "Jupiter-2 Enterprise Linear Prototype A",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_5)\
  _BT_D(JUP2_ENT_LINEAR_PRODUCTION_A,    0x4F, JUP2, "Jupiter-2 Enterprise Linear Production A",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_5)\
  _BT_D(JUP2_ENT_LINEAR_PRODUCTION_A_8GBIT, 0x8F, JUP2, "Jupiter-2 Enterprise Linear Production A (8GBIT)",DDR_MEM_SIZE_8_GBIT, GPIO_LIST_5)\
  _BT_D(JUP2_ENT_LINEAR_RACK_MOUNT_A_16GBIT,0xD1, JUP2, "Jupiter-2 Enterprise Linear Rack Mount Production A (16GBIT)",DDR_MEM_SIZE_16_GBIT, GPIO_LIST_5)\
  _BT_D(JUP2_ENT_LINEAR_RACK_MOUNT_A,    0x51, JUP2, "Jupiter-2 Enterprise Linear Rack Mount Production A",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_5)\
  _BT_D(JUP2_ENT_LINEAR_PHASE2_PROTOTYPE,   0xD5, JUP2, "Jupiter-2 Enterprise Linear Phase2 Prototype",DDR_MEM_SIZE_16_GBIT, GPIO_LIST_7)\
  _BT_D(JUP2_ENT_LINEAR_PHASE2_PRODUCTION,  0xD6, JUP2, "Jupiter-2 Enterprise Linear Phase2 Production TDM",DDR_MEM_SIZE_16_GBIT, GPIO_LIST_7)\
  _BT_D(JUP2_ENT_LINEAR_PHASE2_PTP,         0xDB, JUP2, "Jupiter-2 Enterprise Linear Phase2 PTP",DDR_MEM_SIZE_16_GBIT, GPIO_LIST_7)\
\
  _BT_D(JUP2_ENT_SATURATED_RACK_MOUNT_A, 0x52, JUP2, "Jupiter-2 Enterprise Saturated Rack Mount Production A",DDR_MEM_SIZE_4_GBIT, GPIO_LIST_6)\
  _BT_D(JUP2_ENT_SATURATED_RACK_MOUNT_A_16GBIT,0xD2, JUP2, "Jupiter-2 Enterprise Saturated Rack Mount Production A (16GBIT)",DDR_MEM_SIZE_16_GBIT, GPIO_LIST_6)\

#endif


/* IP radio not defined yet */
#define IS_BOARD_TYPE_IP_RADIO()  (FALSE)

typedef enum
{
    /* e.g., HT1100_PRODUCTION = 0x03 */
    #define  _BT_D(EnumName, boardStrapVal, boardClass, comment, ddrMemSizeInMB, gpioList) EnumName = boardStrapVal,
    BOARD_TYPE_LIST
    #undef _BT_D
} iduType_e;

enum
{
    /* e.g., HT1100_PRODUCTION = 0x03 */
    #define  _BT_D(EnumName, boardStrapVal, boardClass, comment, ddrMemSizeInMB, gpioList) EnumName##_INDEX,
    BOARD_TYPE_LIST
    #undef _BT_D
    IDU_BOARD_MAX_INDEX_EXCLUSIVE,
};

typedef enum
{
    HT1100_CLASS = 0,
    KALE_CLASS   = 1,
    KALERC_CLASS = 2,
    HT2000_CLASS = 3,
    JUP2_CLASS   = 4,
    UNKONWN_BOARD_CLASS,
} iduClass_e;


/* Jupiter-2 strapping value bit[6:7] is for memory size */
#define DDR_SIZE_LIST \
  /* _DS_D(ddrSizeEnum, ddrMemSizeIndicator, ddrMemSizeInMB, comment) */\
  _DS_D(DDR_MEM_SIZE_2_GBIT,   0x00, 256,  "DDR memory size of 2Gbit")\
  _DS_D(DDR_MEM_SIZE_4_GBIT,   0x40, 512,  "DDR memory size of 4Gbit")\
  _DS_D(DDR_MEM_SIZE_8_GBIT,   0x80, 1024, "DDR memory size of 8Gbit")\
  _DS_D(DDR_MEM_SIZE_16_GBIT,  0xC0, 2048, "DDR memory size of 16Gbit")\


typedef enum
{
    #define  _DS_D(ddrSizeEnum, ddrMemSizeIndicator, ddrMemSizeInMB, comment) ddrSizeEnum = ddrMemSizeInMB,
    DDR_SIZE_LIST
    #undef _DS_D
} ddrSize_e;

enum
{
    #define  _DS_D(ddrSizeEnum, ddrMemSizeIndicator, ddrMemSizeInMB, commen) ddrSizeEnum##_INDEX,
    DDR_SIZE_LIST
    #undef _DS_D
    DDR_SIZE_MAX_INDEX_EXCLUSIVE,
};

#define MAX_ATTEN_MULTIMODE_CONSUMER    310 /* 31.0 dB */
#define MAX_ATTEN_MULTIMODE_ENTERPRISE  620 /* 62.0 dB */
#define MAX_ATTEN_NON_MULTIMODE_LINEAR  470 /* 47.0 dB */

extern iduType_e getIduBoardStrappingVal();
extern int       oduMultiModeTXModeIsLinear();
extern int       blockTxhaInit();

/* strapping indetifier functions*/
static inline unsigned int getIduDdrMemSizeInMB()
{
    static unsigned int ddrMemSizeInMB = 0;  /* cache the value */
    unsigned int bType;

    if(ddrMemSizeInMB > 0)
         return ddrMemSizeInMB;

    bType = getIduBoardStrappingVal();

    #define _BT_D(EnumName, boardStrapVal, boardClass, comment, ddrMemSize, gpioList) \
        if(boardStrapVal == bType) {ddrMemSizeInMB = ddrMemSize; goto _RTN_HERE;}
    BOARD_TYPE_LIST
    #undef _BT_D

_RTN_HERE:
    return ddrMemSizeInMB;
}

/* zfan - I will be used inside BOARD_GPIO_SET(void) */
static inline int BOARD_TYPE_GPIO_LIST_GET()
{
    unsigned int bType = getIduBoardStrappingVal();

    if(bType)

    #define _BT_D(EnumName, boardStrapVal, boardClass, comment, ddrMemSize, gpioList) \
        if(boardStrapVal == bType) {return gpioList;}
    BOARD_TYPE_LIST
    #undef _BT_D

    return GPIO_LIST_INVALID;
}

static inline int IS_BOARD_TYPE_JUP2_MULTIMODE_ENT_LINEAR_PHASE2(unsigned int bType)
{
#ifdef THEBE_PLATFORM
    return (bType == JUP2_MULTIMODE_ENT_LINEAR_PHASE2 || bType == JUP2_MULTIMODE_ENT_LINEAR_PHASE2_PTP || bType == JUP2_MULTIMODE_ENT_LINEAR_PHASE2_TDM);
#else
    return FALSE;
#endif
}

/* The Below IS_BOARD_TYPE_XXXX_YYYY_..._ZZZ() is in general used for high-level
   feature conditional branch, or TXHA initialization code provided by HW/FW team,
   it is not recommended to be used for low-level driver */

static inline int IS_BOARD_TYPE_JUP2_IDU()
{
#ifdef THEBE_PLATFORM
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == HT2000_PROTOTYPE_3RD_NYQUIST) ||
            (bType == HT2000_PRODUCTION || bType == HT2000_PRODUCTION_SBD_REDESIGN) ||
            (bType == HT2000_WIFI || bType == HT2000_WIFI_A || bType == HT2000_WIFI_A_SBD_REDESIGN) ||
            (bType == HT2000_PROTOTYPE) ||
            (bType == HT2000_PRODUCTION_REDESIGN) ||
            (bType == JUP2_MULTIMODE || bType == JUP2_MULTIMODE_B) ||
            (IS_BOARD_TYPE_JUP2_MULTIMODE_ENT_LINEAR_PHASE2(bType)) ||
            (bType == JUP2_EVALUATION_IDU) ||
            (bType == JUP2_AERO_PROTOTYPE) ||
            (bType == JUP2_AERO_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PROTOTYPE || bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PTP) ||
            (bType == JUP2_ENT_LINEAR_PROTOTYPE || bType == JUP2_ENT_LINEAR_PROTOTYPE_A) ||
            (bType == JUP2_ENT_LINEAR_PRODUCTION || bType == JUP2_ENT_LINEAR_PRODUCTION_A || bType == JUP2_ENT_LINEAR_PRODUCTION_A_8GBIT) ||
            (bType == JUP2_ENT_LINEAR_RACK_MOUNT || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A_16GBIT) ||
            (bType == JUP2_ENT_SATURATED_RACK_MOUNT || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A_16GBIT));
#else
    return FALSE;
#endif
}
static inline int IS_BOARD_TYPE_HT2000()
{
#ifdef THEBE_PLATFORM
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == HT2000_PROTOTYPE_3RD_NYQUIST) ||
            (bType == HT2000_PRODUCTION || bType == HT2000_PRODUCTION_SBD_REDESIGN) ||
            (bType == HT2000_WIFI || bType == HT2000_WIFI_A || bType == HT2000_WIFI_A_SBD_REDESIGN) ||
            (bType == HT2000_PROTOTYPE) ||
            (bType == HT2000_PRODUCTION_REDESIGN));
#else
    return FALSE;
#endif
}


static inline int IS_BOARD_TYPE_HAS_PTP()
{
#ifdef THEBE_PLATFORM
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION) ||
             (bType == JUP2_ENT_LINEAR_PHASE2_PROTOTYPE)  ||
             (bType == JUP2_ENT_LINEAR_PHASE2_PTP) ||      
             (bType == JUP2_MULTIMODE_ENT_LINEAR_PHASE2_PTP));

#else
    return FALSE;
#endif
}

static inline int IS_BOARD_TYPE_SATURATED()
{
    unsigned int bType = getIduBoardStrappingVal();

#ifdef THEBE_PLATFORM
    return ((bType == HT2000_PROTOTYPE_3RD_NYQUIST) ||
            (bType == HT2000_PRODUCTION || bType == HT2000_PRODUCTION_SBD_REDESIGN) ||
            (bType == HT2000_WIFI || bType == HT2000_WIFI_A || bType == HT2000_WIFI_A_SBD_REDESIGN) ||
            (bType == HT2000_PROTOTYPE) ||
            (bType == HT2000_PRODUCTION_REDESIGN) ||
            ((bType == JUP2_MULTIMODE || bType == JUP2_MULTIMODE_B) && (!oduMultiModeTXModeIsLinear())) ||
            ((IS_BOARD_TYPE_JUP2_MULTIMODE_ENT_LINEAR_PHASE2(bType)) && (!oduMultiModeTXModeIsLinear())) ||
            (bType == JUP2_ENT_SATURATED_RACK_MOUNT || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A_16GBIT));
#else
    return ((bType == HT1100_PRODUCTION) ||
            (bType == HT1100_PROTOTYPE) ||
            (bType == KALE_SATURATED_PROTOTYPE) ||
            (bType == KALE_SATURATED_PRODUCTION) ||
            (bType == KALE_SATURATED_PRODUCTION_ENT) ||
            (bType == KALE_RC_SATURATED_PROTOTYPE) ||
            (bType == KALE_RC_SATURATED_PRODUCTION) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT_REDESIGNED));
#endif
}

static inline int IS_BOARD_TYPE_LINEAR()
{
    unsigned int bType = getIduBoardStrappingVal();

#ifdef THEBE_PLATFORM
    return ((bType == JUP2_AERO_PROTOTYPE) ||
            (bType == JUP2_AERO_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PROTOTYPE || bType == JUP2_ENT_LINEAR_PROTOTYPE_A) ||
            ((bType == JUP2_MULTIMODE || bType == JUP2_MULTIMODE_B) && oduMultiModeTXModeIsLinear()) ||
            ((IS_BOARD_TYPE_JUP2_MULTIMODE_ENT_LINEAR_PHASE2(bType)) && (oduMultiModeTXModeIsLinear())) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PROTOTYPE || bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PTP) ||
            (bType == JUP2_ENT_LINEAR_PRODUCTION || bType == JUP2_ENT_LINEAR_PRODUCTION_A || bType == JUP2_ENT_LINEAR_PRODUCTION_A_8GBIT) ||
            (bType == JUP2_ENT_LINEAR_RACK_MOUNT || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A_16GBIT));
#else
    return ((bType == KALE_LINEAR_DUAL_IFL_PROTOTYPE) ||
            (bType == KALE_LINEAR_SINGLE_IFL_PROTOTYPE) ||
            (bType == KALE_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||
            (bType == KALE_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_SINGLE_IFL_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_RACK_MOUNT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_AERO) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_AERO));
#endif
}

static inline int IS_BOARD_TYPE_J2WW_SATURATED()
{
#ifdef THEBE_PLATFORM

    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == JUP2_ENT_SATURATED_RACK_MOUNT || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A_16GBIT));
#else
    return FALSE;
#endif
}

static inline int IS_BOARD_TYPE_J2WW_LINEAR()
{
#ifdef THEBE_PLATFORM

    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == JUP2_ENT_LINEAR_PROTOTYPE || bType == JUP2_ENT_LINEAR_PROTOTYPE_A) ||
            (bType == JUP2_ENT_LINEAR_PRODUCTION || bType == JUP2_ENT_LINEAR_PRODUCTION_A || bType == JUP2_ENT_LINEAR_PRODUCTION_A_8GBIT) ||
            (bType == JUP2_ENT_LINEAR_RACK_MOUNT || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A_16GBIT) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PROTOTYPE || bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION)) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PTP) ||
            ((bType == JUP2_MULTIMODE || bType == JUP2_MULTIMODE_B) && oduMultiModeTXModeIsLinear()) ||
            ((IS_BOARD_TYPE_JUP2_MULTIMODE_ENT_LINEAR_PHASE2(bType)) && (oduMultiModeTXModeIsLinear()));
#else
    return FALSE;
#endif
}

static inline int IS_BOARD_TYPE_JUP2_DUAL_IFL_LINEAR_AERO()
{
#ifdef THEBE_PLATFORM
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == JUP2_AERO_PROTOTYPE) ||
            (bType == JUP2_AERO_PRODUCTION));
#else
    return FALSE;
#endif
}

static inline int IS_BOARD_TYPE_HT2000_PROTOTYPE_3RD_NYQUIST()
{
#ifdef THEBE_PLATFORM
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == HT2000_PROTOTYPE_3RD_NYQUIST));
#else
    return FALSE;
#endif
}

#define IS_BOARD_TYPE_3RD_NYQUIST() (IS_BOARD_TYPE_HT2000_PROTOTYPE_3RD_NYQUIST())

static inline int IS_BOARD_TYPE_DUAL_IFL_LINEAR_RACK_MOUNT()
{
    unsigned int bType = getIduBoardStrappingVal();

#ifdef THEBE_PLATFORM
    return ((bType == JUP2_ENT_LINEAR_RACK_MOUNT || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A_16GBIT) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PROTOTYPE || bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION));
#else
    return ((bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_RACK_MOUNT));
#endif
}

static inline int IS_BOARD_TYPE_RACK_MOUNT()
{
    unsigned int bType = getIduBoardStrappingVal();

#ifdef THEBE_PLATFORM
    return ((bType == JUP2_ENT_LINEAR_RACK_MOUNT || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A_16GBIT) ||
            (bType == JUP2_ENT_SATURATED_RACK_MOUNT || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A_16GBIT) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PROTOTYPE || bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PTP)); 
#else
    return ((bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_RACK_MOUNT));
#endif
}

static inline int IS_BOARD_TYPE_DUAL_IFL_LINEAR_AERO()
{
    unsigned int bType = getIduBoardStrappingVal();

#ifdef THEBE_PLATFORM
    return ((bType == JUP2_AERO_PROTOTYPE) ||
            (bType == JUP2_AERO_PRODUCTION));
#else
    return ((bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_AERO) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_AERO));
#endif
}

static inline int IS_BOARD_TYPE_DUAL_IFL_LINEAR()
{
    unsigned int bType = getIduBoardStrappingVal();

#ifdef THEBE_PLATFORM
    return ((bType == JUP2_AERO_PROTOTYPE) ||
            (bType == JUP2_AERO_PRODUCTION) ||
            ((bType == JUP2_MULTIMODE || bType == JUP2_MULTIMODE_B) && oduMultiModeTXModeIsLinear()) ||
            ((IS_BOARD_TYPE_JUP2_MULTIMODE_ENT_LINEAR_PHASE2(bType)) && (oduMultiModeTXModeIsLinear())) ||
            (bType == JUP2_ENT_LINEAR_PROTOTYPE || bType == JUP2_ENT_LINEAR_PROTOTYPE_A) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PROTOTYPE || bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PTP) ||
            (bType == JUP2_ENT_LINEAR_PRODUCTION || bType == JUP2_ENT_LINEAR_PRODUCTION_A || bType == JUP2_ENT_LINEAR_PRODUCTION_A_8GBIT) ||
            (bType == JUP2_ENT_LINEAR_RACK_MOUNT || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A_16GBIT));
#else
    return ((bType == KALE_LINEAR_DUAL_IFL_PROTOTYPE) ||
            (bType == KALE_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||
            (bType == KALE_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_RACK_MOUNT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_AERO) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_AERO));
#endif
}

static inline int IS_BOARD_TYPE_HAS_WIFI()
{
#ifdef THEBE_PLATFORM
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == HT2000_WIFI || bType == HT2000_WIFI_A || bType == HT2000_WIFI_A_SBD_REDESIGN));
#else
    return FALSE;
#endif
}

static inline int IS_BOARD_TYPE_HT1100()
{
#ifdef THEBE_PLATFORM
    return FALSE;
#else
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == HT1100_PRODUCTION) ||
            (bType == HT1100_PROTOTYPE));
#endif
}

static inline int IS_BOARD_TYPE_KALE()
{
#ifdef THEBE_PLATFORM
    return FALSE;
#else
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == KALE_SATURATED_PROTOTYPE) ||
            (bType == KALE_LINEAR_DUAL_IFL_PROTOTYPE) ||
            (bType == KALE_LINEAR_SINGLE_IFL_PROTOTYPE) ||
            (bType == KALE_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||

            (bType == KALE_SATURATED_PRODUCTION) ||
            (bType == KALE_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_SATURATED_PRODUCTION_ENT) ||

            (bType == KALE_RC_SATURATED_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_SINGLE_IFL_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||

            (bType == KALE_RC_SATURATED_PRODUCTION) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT) ||

            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_RACK_MOUNT) ||

            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_AERO) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_AERO));
#endif
}

static inline int IS_BOARD_TYPE_KALE_RC()
{
#ifdef THEBE_PLATFORM
    return FALSE;
#else
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == KALE_RC_SATURATED_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_SINGLE_IFL_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||

            (bType == KALE_RC_SATURATED_PRODUCTION) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT) ||

            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_RACK_MOUNT) ||

            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_AERO) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_AERO));
#endif
}

static inline int IS_BOARD_TYPE_KALE_SATURATED()
{
#ifdef THEBE_PLATFORM
    return FALSE;
#else
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == KALE_SATURATED_PROTOTYPE) ||
            (bType == KALE_SATURATED_PRODUCTION) ||
            (bType == KALE_SATURATED_PRODUCTION_ENT) ||
            (bType == KALE_RC_SATURATED_PROTOTYPE) ||
            (bType == KALE_RC_SATURATED_PRODUCTION) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT_REDESIGNED));
#endif
}

static inline int IS_BOARD_TYPE_SINGLE_IFL_LINEAR()
{
#ifdef THEBE_PLATFORM
    return FALSE;
#if 0
    return (((bType == JUP2_MULTIMODE || bType == JUP2_MULTIMODE_B) && oduMultiModeTXModeIsLinear()) ||
            ((IS_BOARD_TYPE_JUP2_MULTIMODE_ENT_LINEAR_PHASE2(bType)) && (oduMultiModeTXModeIsLinear())));
#endif
#else
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == KALE_LINEAR_SINGLE_IFL_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_SINGLE_IFL_PROTOTYPE));
#endif
}

static inline int IS_BOARD_TYPE_JUP1_DUAL_IFL_LINEAR_AERO()
{
#ifdef THEBE_PLATFORM
    return FALSE;
#else
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_AERO) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_AERO));
#endif
}

/* The above IS_BOARD_TYPE_XXXX_YYYY_..._ZZZ() is in general used for high-level
   feature conditional branch, or TXHA initialization code provided by HW/FW team,
   it is not recommended to be used for low-level driver */

/*************************************************************************
 *
 *   Board attributes/specifics that affect the coding logic:
 *    used to determine which HW driver function to use
 *
 *  It indicates a "concrete" case, specific to a (class of) HW module/chip that
 *  should be programmed
 *
 * Guidelines:
 *
 * (1) For each definition, the comment MUST include the original source:
 * such as datasheet/spec, or document# (if by Hughes)
 *
 * (2) for each new board strapping added, you have to walk through the below list
 * to decide if need to add this type into each attribute
 * or if need to create a new attribute
 *
 **************************************************************************/
/* used to distinguish external or internal pll */
static inline int IS_RX_EXT_PLL()
{
#ifdef THEBE_PLATFORM
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == HT2000_PRODUCTION || bType == HT2000_PRODUCTION_SBD_REDESIGN) ||
            (bType == HT2000_PROTOTYPE) ||
            (bType == JUP2_AERO_PROTOTYPE) ||
            (bType == JUP2_AERO_PRODUCTION) ||
            (bType == HT2000_WIFI || bType == HT2000_WIFI_A || bType == HT2000_WIFI_A_SBD_REDESIGN) ||
            (bType == JUP2_MULTIMODE || bType == JUP2_MULTIMODE_B) ||
            (IS_BOARD_TYPE_JUP2_MULTIMODE_ENT_LINEAR_PHASE2(bType)) ||
            (bType == JUP2_ENT_LINEAR_PROTOTYPE_A) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PROTOTYPE || bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PTP) || 
            (bType == JUP2_ENT_LINEAR_PRODUCTION_A || bType == JUP2_ENT_LINEAR_PRODUCTION_A_8GBIT) ||
            (bType == JUP2_ENT_LINEAR_RACK_MOUNT_A || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A_16GBIT) ||
            (bType == JUP2_ENT_SATURATED_RACK_MOUNT_A || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A_16GBIT));
#else
    return FALSE;
#endif
}

/*  Checks wich board are forbidden from using SBD since the FEC clocks is used by USB and cant't be modified*/
static inline int IS_SBD_DECODING_FORBIDDEN()
{
    return FALSE;
}

/* capable of intoute spreading */
static inline int IS_INROUTE_SPREADING_CAPABLE()
{
    return TRUE;
}

/* compatibility */
#define IS_RX_EXTERNAL_PLL() (IS_RX_EXT_PLL())

/* Use I2C to configure this external PLL;
   Otherwise, by default (other types of) External PLL is not configurable */
static inline int IS_RX_EXT_PLL_CONFIG_VIA_I2C()
{
#ifdef THEBE_PLATFORM
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == HT2000_PRODUCTION || bType == HT2000_PRODUCTION_SBD_REDESIGN) ||
            (bType == HT2000_PROTOTYPE) ||
            (bType == HT2000_WIFI || bType == HT2000_WIFI_A || bType == HT2000_WIFI_A_SBD_REDESIGN) ||
            (bType == JUP2_MULTIMODE || bType == JUP2_MULTIMODE_B) ||
            (IS_BOARD_TYPE_JUP2_MULTIMODE_ENT_LINEAR_PHASE2(bType)) ||
            (bType == JUP2_ENT_LINEAR_PROTOTYPE_A) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PROTOTYPE || bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PTP) ||
            (bType == JUP2_ENT_LINEAR_PRODUCTION_A || bType == JUP2_ENT_LINEAR_PRODUCTION_A_8GBIT) ||
            (bType == JUP2_ENT_LINEAR_RACK_MOUNT_A || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A_16GBIT) ||
            (bType == JUP2_ENT_SATURATED_RACK_MOUNT_A || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A_16GBIT));
#else
    return FALSE;
#endif
}

static inline int IS_LNB_CONTROLLER_ISL9492_PRESENT()
{
    unsigned int bType = getIduBoardStrappingVal();

#ifdef THEBE_PLATFORM
    return ((bType == JUP2_ENT_LINEAR_PROTOTYPE || bType == JUP2_ENT_LINEAR_PROTOTYPE_A) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PROTOTYPE || bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PTP) ||
            (bType == JUP2_ENT_LINEAR_PRODUCTION || bType == JUP2_ENT_LINEAR_PRODUCTION_A || bType == JUP2_ENT_LINEAR_PRODUCTION_A_8GBIT) ||
            (bType == JUP2_ENT_LINEAR_RACK_MOUNT || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A_16GBIT));
#else
    return ((bType == KALE_LINEAR_DUAL_IFL_PROTOTYPE) ||
            (bType == KALE_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||
            (bType == KALE_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_RACK_MOUNT));
#endif
}

/* external input of 10 MHZ */
static inline int IS_10MHZ_CIRCUIT_PRESENT()
{
    unsigned int bType = getIduBoardStrappingVal();

#ifdef THEBE_PLATFORM
    return ((bType == JUP2_AERO_PROTOTYPE) ||
            (bType == JUP2_AERO_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PROTOTYPE || bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_RACK_MOUNT || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A_16GBIT));
#else
    return ((bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_RACK_MOUNT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_AERO) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_AERO));
#endif
}

/* 27MHZ duty cycle control */
static inline int IS_BOARD_TYPE_27MHZ_DUTY_CYCLE()
{
#ifdef THEBE_PLATFORM
    return FALSE;
#else
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == KALE_SATURATED_PROTOTYPE) ||
            (bType == KALE_LINEAR_DUAL_IFL_PROTOTYPE) ||
            (bType == KALE_LINEAR_SINGLE_IFL_PROTOTYPE) ||
            (bType == KALE_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||

            (bType == KALE_SATURATED_PRODUCTION) ||
            (bType == KALE_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_SATURATED_PRODUCTION_ENT) ||

            (bType == KALE_RC_SATURATED_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_SINGLE_IFL_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||

            (bType == KALE_RC_SATURATED_PRODUCTION) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT) ||

            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_RACK_MOUNT));
#endif
}

/* For this kind of boards, only use minimum Attenuation value */
#define IS_BOARD_TYPE_USE_MIN_ATTEN_VALUE()    (IS_BOARD_TYPE_DUAL_IFL_LINEAR())

/* The outroute carrier needs to configure differently according to the symbol rate
    cutoff value */
static inline int IS_BOARD_TYPE_CARRIER_SYMBOL_RATE_CUTOFF_1()
{
#ifdef THEBE_PLATFORM
    return FALSE;
#else
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == KALE_RC_SATURATED_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_SINGLE_IFL_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||

            (bType == KALE_RC_SATURATED_PRODUCTION) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT) ||

            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_RACK_MOUNT) ||

            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_AERO) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_AERO));
#endif
}

/* MPU 8KB flash exists */
static inline int IS_MPU_8KB_FLASH()
{
#ifdef THEBE_PLATFORM
    return FALSE;
#else
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_RACK_MOUNT) ||

            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_AERO) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_AERO));
#endif
}

/************************************************
     * * * *     RX ADC/Tuners       * * * *
************************************************/

/* Special external RX ADC */
#define IS_RX_ADC_HMCAD8111()               (IS_BOARD_TYPE_KALE_RC())
static inline int IS_RX_ADC_ADC08DL500()
{
    return (IS_BOARD_TYPE_HT1100() ||
            IS_BOARD_TYPE_KALE());
}

/* Special external RX Tuner */
#define IS_RX_TUNER_STV6111()               (IS_BOARD_TYPE_KALE_RC())
#define IS_RX_TUNER_STV6111_25MHZ()         (IS_BOARD_TYPE_JUP1_DUAL_IFL_LINEAR_AERO())

#define IS_RX_TUNER_MAX2121_25MHZ()         (IS_BOARD_TYPE_JUP2_DUAL_IFL_LINEAR_AERO())
static inline int IS_RX_TUNER_MAX2121()
{
    unsigned int bType = getIduBoardStrappingVal();

#ifdef THEBE_PLATFORM
    return ((bType == HT2000_PROTOTYPE_3RD_NYQUIST) ||
            (bType == HT2000_PRODUCTION || bType == HT2000_PRODUCTION_SBD_REDESIGN) ||
            (bType == HT2000_WIFI || bType == HT2000_WIFI_A || bType == HT2000_WIFI_A_SBD_REDESIGN) ||
            (bType == HT2000_PROTOTYPE) ||
            (bType == HT2000_PRODUCTION_REDESIGN) ||
            (bType == JUP2_AERO_PROTOTYPE) ||
            (bType == JUP2_AERO_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PROTOTYPE || bType == JUP2_ENT_LINEAR_PROTOTYPE_A) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PROTOTYPE || bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PTP) ||
            (bType == JUP2_ENT_LINEAR_PRODUCTION || bType == JUP2_ENT_LINEAR_PRODUCTION_A || bType == JUP2_ENT_LINEAR_PRODUCTION_A_8GBIT) ||
            (bType == JUP2_ENT_LINEAR_RACK_MOUNT || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A_16GBIT) ||
            (bType == JUP2_ENT_SATURATED_RACK_MOUNT || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A_16GBIT));
#else
    return ((bType == HT1100_PRODUCTION) ||
            (bType == HT1100_PROTOTYPE) ||
            (bType == KALE_SATURATED_PROTOTYPE) ||
            (bType == KALE_LINEAR_DUAL_IFL_PROTOTYPE) ||
            (bType == KALE_LINEAR_SINGLE_IFL_PROTOTYPE) ||
            (bType == KALE_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||
            (bType == KALE_SATURATED_PRODUCTION) ||
            (bType == KALE_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_SATURATED_PRODUCTION_ENT));
#endif
}

/************************************************
     * * * *     TX synthesizers       * * * *
************************************************/

static inline int IS_SYNTH_TYPE_INTERNAL()
{
#ifdef THEBE_PLATFORM
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == HT2000_PROTOTYPE_3RD_NYQUIST));
#else
    return FALSE;
#endif
}

static inline int IS_SYNTH_TYPE_AK1599V()
{
#ifdef THEBE_PLATFORM
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == HT2000_PRODUCTION || bType == HT2000_PRODUCTION_SBD_REDESIGN) ||
            (bType == HT2000_WIFI || bType == HT2000_WIFI_A || bType == HT2000_WIFI_A_SBD_REDESIGN) ||
            (bType == HT2000_PROTOTYPE) ||
            (bType == HT2000_PRODUCTION_REDESIGN));
#else
    return FALSE;
#endif
}

#define IS_SYNTH_TYPE_AKM()    (IS_SYNTH_TYPE_AK1599V())

#define IS_SYNTH_TYPE_ADRF_AERO() IS_BOARD_TYPE_DUAL_IFL_LINEAR_AERO()

static inline int IS_SYNTH_TYPE_ADRF()
{
    unsigned int bType = getIduBoardStrappingVal();

#ifdef THEBE_PLATFORM
    return ((bType == JUP2_AERO_PROTOTYPE) ||
            (bType == JUP2_AERO_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PROTOTYPE || bType == JUP2_ENT_LINEAR_PROTOTYPE_A) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PROTOTYPE || bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PTP) ||
            (bType == JUP2_ENT_LINEAR_PRODUCTION || bType == JUP2_ENT_LINEAR_PRODUCTION_A || bType == JUP2_ENT_LINEAR_PRODUCTION_A_8GBIT) ||
            (bType == JUP2_ENT_LINEAR_RACK_MOUNT || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A_16GBIT) ||
            (bType == JUP2_ENT_SATURATED_RACK_MOUNT || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A_16GBIT));
#else
    return ((bType == KALE_SATURATED_PROTOTYPE) ||
            (bType == KALE_LINEAR_DUAL_IFL_PROTOTYPE) ||
            (bType == KALE_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||

            (bType == KALE_SATURATED_PRODUCTION) ||
            (bType == KALE_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_SATURATED_PRODUCTION_ENT) ||

            (bType == KALE_RC_SATURATED_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||

            (bType == KALE_RC_SATURATED_PRODUCTION) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT) ||

            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_RACK_MOUNT));
#endif
}

static inline int IS_SYNTH_TYPE_HMC()
{
#ifdef THEBE_PLATFORM
    return FALSE;
#else
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == HT1100_PRODUCTION) ||
            (bType == HT1100_PROTOTYPE) ||
            (bType == KALE_LINEAR_SINGLE_IFL_PROTOTYPE) ||
            (bType == KALE_RC_LINEAR_SINGLE_IFL_PROTOTYPE));
#endif
}

static inline int IS_SYNTH_TYPE_TRF3722()
{
#ifdef THEBE_PLATFORM
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == JUP2_MULTIMODE || bType == JUP2_MULTIMODE_B || (IS_BOARD_TYPE_JUP2_MULTIMODE_ENT_LINEAR_PHASE2(bType))) &&
            (!oduMultiModeTXModeIsLinear()));
#else
    return FALSE;
#endif
}

#define IS_SYNTH_TYPE_TRF()    (IS_SYNTH_TYPE_TRF3722())


static inline int IS_SYNTH_TYPE_LMX2571()
{
#ifdef THEBE_PLATFORM
    unsigned int bType = getIduBoardStrappingVal();

    return (((bType == JUP2_MULTIMODE || bType == JUP2_MULTIMODE_B) && oduMultiModeTXModeIsLinear()) ||
            ((IS_BOARD_TYPE_JUP2_MULTIMODE_ENT_LINEAR_PHASE2(bType)) && (oduMultiModeTXModeIsLinear())));
#else
    return FALSE;
#endif
}

#define IS_SYNTH_TYPE_LMX()    (IS_SYNTH_TYPE_LMX2571())


/************************************************
     * * * *    Lan Port/Switch       * * * *
************************************************/

static inline int IS_LAN_PORT_WIFI_ONLY()
{
#ifdef THEBE_PLATFORM
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == HT2000_WIFI || bType == HT2000_WIFI_A || bType == HT2000_WIFI_A_SBD_REDESIGN));
#else
    return FALSE;
#endif
}

static inline int IS_LAN_PORT_SWITCH_ONLY()
{
    unsigned int bType = getIduBoardStrappingVal();

#ifdef THEBE_PLATFORM
    return ((bType == JUP2_AERO_PROTOTYPE) ||
            (bType == JUP2_AERO_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PROTOTYPE || bType == JUP2_ENT_LINEAR_PROTOTYPE_A) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PROTOTYPE || bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION) ||
            (bType == JUP2_ENT_LINEAR_PRODUCTION || bType == JUP2_ENT_LINEAR_PRODUCTION_A || bType == JUP2_ENT_LINEAR_PRODUCTION_A_8GBIT) ||
            (bType == JUP2_ENT_LINEAR_RACK_MOUNT || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A || bType == JUP2_ENT_LINEAR_RACK_MOUNT_A_16GBIT) ||
            (bType == JUP2_ENT_SATURATED_RACK_MOUNT || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A || bType == JUP2_ENT_SATURATED_RACK_MOUNT_A_16GBIT));

#else
    return ((bType == KALE_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||
            (bType == KALE_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_SATURATED_PRODUCTION_ENT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_ENT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_SATURATED_PRODUCTION_ENT_REDESIGNED) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_RACK_MOUNT) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PRODUCTION_AERO) ||
            (bType == KALE_RC_LINEAR_DUAL_IFL_PROTOTYPE_AERO));
#endif
}

static inline int IS_LAN_PORT_PHY_ONLY()
{
    unsigned int bType = getIduBoardStrappingVal();

#ifdef THEBE_PLATFORM
    return ((bType == HT2000_PROTOTYPE_3RD_NYQUIST) ||
            (bType == HT2000_PRODUCTION || bType == HT2000_PRODUCTION_SBD_REDESIGN) ||
            (bType == JUP2_ENT_LINEAR_PHASE2_PTP) ||
            (bType == HT2000_PROTOTYPE) ||
            (bType == HT2000_PRODUCTION_REDESIGN) ||
            (bType == HT2000_WIFI) ||
            (bType == HT2000_WIFI_A || bType == HT2000_WIFI_A_SBD_REDESIGN));
#else
    return ((bType == HT1100_PRODUCTION) ||
            (bType == KALE_SATURATED_PRODUCTION) ||
            (bType == KALE_RC_SATURATED_PRODUCTION));
#endif
}

static inline int IS_BOARD_TYPE_MULTI_MODE()
{
    unsigned int bType = getIduBoardStrappingVal();

#ifdef THEBE_PLATFORM
    return (bType == JUP2_MULTIMODE || bType == JUP2_MULTIMODE_B || (IS_BOARD_TYPE_JUP2_MULTIMODE_ENT_LINEAR_PHASE2(bType)));
#else
    return (0);
#endif
}

/* This function return if the terminal should be treated as linear during installation... we don't want this for multimode boards */
static inline int IS_LINEAR_INSTALLATION()
{
    return (IS_BOARD_TYPE_DUAL_IFL_LINEAR() && !IS_BOARD_TYPE_MULTI_MODE());
}

static inline int IS_CBM_PRESENT()
{
#ifdef THEBE_PLATFORM
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION));
#else
    return FALSE;
#endif
}

static inline int IS_AD9545_PRESENT()
{
#ifdef THEBE_PLATFORM
    unsigned int bType = getIduBoardStrappingVal();

    return ((bType == JUP2_ENT_LINEAR_PHASE2_PRODUCTION) ||
			(bType == JUP2_MULTIMODE_ENT_LINEAR_PHASE2_PTP));
#else
    return FALSE;
#endif
}
/************************************************
   requirements per board specific
************************************************/

#define IS_BOARD_LINEAR_CALIBRATION_REQUIRED() (IS_BOARD_TYPE_LINEAR())

/************************************************
  general utility functions
************************************************/
static inline void boarTypeInfoGet(unsigned int * boardType, char * boardName, char * boardDescription, unsigned int * memSize)
{
    *boardType = (unsigned int) getIduBoardStrappingVal();

    switch(*boardType)
    {
        #define _BT_D(EnumName, boardStrapVal, boardClass, comment, ddrMemSize, gpioList) \
        case boardStrapVal:\
            strcpy(boardName, comment);\
            sprintf(boardDescription, "(%s), (TDM present:%s), (TX Mode:%s), (IFL:%s), (Built-in WIFI:%s), (RX_PLL:%s), (GPIO_list:%d)", #EnumName, \
                    (IS_CBM_PRESENT())?"Yes":"No",\
                    (IS_BOARD_TYPE_SATURATED())?"Saturated":"Linear",\
                    (IS_BOARD_TYPE_DUAL_IFL_LINEAR())?"Dual":IS_BOARD_TYPE_SINGLE_IFL_LINEAR()?"Single":"Unkown",\
                    (IS_BOARD_TYPE_HAS_WIFI())?"Yes":"No",\
                    (IS_RX_EXT_PLL())?"External":"Internal",\
                    BOARD_TYPE_GPIO_LIST_GET());\
            *memSize = ddrMemSize;\
            break;

            BOARD_TYPE_LIST
        #undef _BT_D

        default:
            *memSize = 0;
            sprintf(boardName, "Strapping value: 0x%x is not defined", *boardType);
            boardDescription[0] = 0;
            break;
    }

    return;
}

/***********************************************************************
 *
 * Function Name: utilsExt10MHzSelectViaGpio()
 *
 * \brief : I write the GPIO register to select external
 *          10MHz or on-board FLL.
 *
 * \return - OK or ERROR
 *
 **********************************************************************/

static inline int utilsExt10MHzSelectViaGpio(int enableExt10MHz)
{
    if(IS_10MHZ_CIRCUIT_PRESENT())
    {
        int boardGpioList = BOARD_TYPE_GPIO_LIST_GET();

#ifdef THEBE_PLATFORM
        if(boardGpioList == GPIO_LIST_J2AERO)
        {
            GPIO_UPP_10MHZ_SW_P9_Write((enableExt10MHz)?(GPIO_OUT_VAL_1):(GPIO_OUT_VAL_0));
        }
        else if(boardGpioList == GPIO_LIST_J2_ENT_PHASE1_LINEAR ||
                boardGpioList == GPIO_LIST_J2_ENT_PHASE2)
        {
            /* zfan -- how about: GPIO_LIST_J2_ENT_PHASE1_SATURATED  (PIN13) as in rcs_hw_dr.c which calls this function too;
                       but skipped due to the above condition check;
                       looks like UPP will set it */

            GPIO_UPP_10MHZ_SW_P10_Write((enableExt10MHz)?(GPIO_OUT_VAL_1):(GPIO_OUT_VAL_0));
        }
#endif

        return OK;
    }

    return ERROR;
}


// #define utilsExt10MHzSelect(a)  utilsExt10MHzSelectViaGpio(a)

/***********************************************************************
 *
 * Function Name: utilsTxOnOffSwitchViaGpio()
 *
 * \brief : I Turn ON/Off the TX by GPIO write
 * INPUT: on_off -  0 (OFF), otherwise (ON)
 *
 * \return - OK or ERROR
 *
 **********************************************************************/
static inline int utilsTxOnOffSwitchViaGpio(int on_off)
{
    GPIO_UPP_TX_ON_Write((on_off)?(GPIO_OUT_VAL_1):(GPIO_OUT_VAL_0));   /* PIN13 */
    return OK;
}

/***********************************************************************
*
* Function Name: llgDefaultGroupIdGet()
*
* \brief : I get the default LLG group ID
*
* \return - Default LLG group id in integer (1 for linear board, 0 for others)
*
**********************************************************************/
static inline int getDefaultLlgGroupId()
{
    if (IS_BOARD_TYPE_LINEAR())
        return 1;
    else
        return 0;
}

#define utilsTxOnOffSwitch(a)  utilsTxOnOffSwitchViaGpio(a)
#define utilsEnableLinear10MhzOut(a)  utilsTxOnOffSwitchViaGpio(a)

/***********************************************************************
 *
 * Function Name: utils10MHzStatusGetViaGpio()
 *
 * \brief : I return 10 Mhz status: 10MHz select, detection, and lock.
 *
 * \return - Ext 10 Mhz status flag:
 *           bit 0 - Enabled, 1 - detected, 2- locked
 *
 *
 **********************************************************************/
enum {
    EXT_10MHZ_ENABLED_BIT  = 0,
    EXT_10MHZ_DETECTED_BIT = 1,
    EXT_10MHZ_LOCKED_BIT   = 2,
};

static inline unsigned int utils10MHzStatusGetViaGpio()
{
    unsigned int status = 0;

    if(IS_10MHZ_CIRCUIT_PRESENT())
    {
        int boardGpioList = BOARD_TYPE_GPIO_LIST_GET();

#ifdef THEBE_PLATFORM  /* zfan - deal this later for Callsito */
        if(boardGpioList == GPIO_LIST_J2AERO)
        {
            status = GPIO_UPP_10MHZ_SW_P9_Read();
        }
        else if(boardGpioList == GPIO_LIST_J2_ENT_PHASE1_LINEAR ||
                boardGpioList == GPIO_LIST_J2_ENT_PHASE2)
        {
            status = GPIO_UPP_10MHZ_SW_P10_Read();
        }
        else
            return status;

        status += GPIO_UPP_10MHZ_DET_Read()<<EXT_10MHZ_DETECTED_BIT;

        if((status & (1<<EXT_10MHZ_ENABLED_BIT)) &&
           (status & (1<<EXT_10MHZ_DETECTED_BIT))) /* EXT 10 MHZ enablbed, and Detected */
            status += GPIO_UPP_10MHZ_LOCK_Read()<<EXT_10MHZ_LOCKED_BIT;
#endif
    }

    return status;
}

#define utils10MHzStatusGet()  (utils10MHzStatusGetViaGpio())
#define isExt10MHzLocked()     (utils10MHzStatusGet() & (1<<EXT_10MHZ_LOCKED_BIT))
#define isExt10MHzDetected()   (utils10MHzStatusGet() & (1<<EXT_10MHZ_DETECTED_BIT))
#define isExt10MHzEnabled()    (utils10MHzStatusGet() & (1<<EXT_10MHZ_ENABLED_BIT))

static inline int _gpio_name_pin_is_valid(char * gipoPinName, int gpioPinNum)
{
#ifdef THEBE_PLATFORM

    /* specific hanlder you provide */
    #define _GPIO_HANDLER(pinNum, asicPinName, gpioName, dir, comment) \
        {\
            if(pinNum == gpioPinNum && !strcmp(#gpioName, gipoPinName)) \
            {\
                return TRUE;\
            }\
        }

    // #include <gpioListGenericHandlerPerBoardType.h>

   #define _GPIO_LIST_PER_BASE(base) \
        {\
            base##_GPIO_LISTS \
        }

    #define _ALL_GPIO_LIST \
         _GPIO_LIST_PER_BASE(DPP) \
         _GPIO_LIST_PER_BASE(UPP) \
         _GPIO_LIST_PER_BASE(SWP) \


    switch(BOARD_TYPE_GPIO_LIST_GET())
    {
        case  GPIO_LIST_EVAL:
                #define _GL_D(pin, asicPinName, InitStats,  EVAL, HT2000, HT2000W, HT2000SBD,J2Aero, J2EntPh1Lin, J2EntPh1Sat, J2EntPh2, J2EntPh2_MM, HT2000_MM, dir, comment) \
                    _GPIO_HANDLER(pin, asicPinName, EVAL,  dir, comment)
                #define _GL_E   _GL_D
                #define _GL_F   _GL_D

                _ALL_GPIO_LIST
                #undef _GL_D
                #undef _GL_E
                #undef _GL_F

            break;

        case  GPIO_LIST_HT2000:
                #define _GL_D(pin, asicPinName, InitStats,  EVAL, HT2000, HT2000W, HT2000SBD,J2Aero, J2EntPh1Lin, J2EntPh1Sat, J2EntPh2, J2EntPh2_MM, HT2000_MM, dir, comment) \
                    _GPIO_HANDLER(pin, asicPinName, HT2000,  dir, comment)
                #define _GL_E   _GL_D
                #define _GL_F   _GL_D

                _ALL_GPIO_LIST
                #undef _GL_D
                #undef _GL_E
                #undef _GL_F

            break;

        case  GPIO_LIST_HT2000W:
                #define _GL_D(pin, asicPinName, InitStats,  EVAL, HT2000, HT2000W, HT2000SBD,J2Aero, J2EntPh1Lin, J2EntPh1Sat, J2EntPh2, J2EntPh2_MM, HT2000_MM, dir, comment) \
                    _GPIO_HANDLER(pin, asicPinName, HT2000W,  dir, comment)
                #define _GL_E   _GL_D
                #define _GL_F   _GL_D

                _ALL_GPIO_LIST
                #undef _GL_D
                #undef _GL_E
                #undef _GL_F

            break;

        case  GPIO_LIST_J2AERO:
                #define _GL_D(pin, asicPinName, InitStats,  EVAL, HT2000, HT2000W, HT2000SBD,J2Aero, J2EntPh1Lin, J2EntPh1Sat, J2EntPh2, J2EntPh2_MM, HT2000_MM, dir, comment) \
                    _GPIO_HANDLER(pin, asicPinName, J2Aero,  dir, comment)
                #define _GL_E   _GL_D
                #define _GL_F   _GL_D

                _ALL_GPIO_LIST
                #undef _GL_D
                #undef _GL_E
                #undef _GL_F
                break;

        case  GPIO_LIST_J2_ENT_PHASE1_LINEAR:
                #define _GL_D(pin, asicPinName, InitStats,  EVAL, HT2000, HT2000W, HT2000SBD,J2Aero, J2EntPh1Lin, J2EntPh1Sat, J2EntPh2, J2EntPh2_MM, HT2000_MM, dir, comment) \
                    _GPIO_HANDLER(pin, asicPinName, J2EntPh1Lin,  dir, comment)
                #define _GL_E   _GL_D
                #define _GL_F   _GL_D

                _ALL_GPIO_LIST
                #undef _GL_D
                #undef _GL_E
                #undef _GL_F

                break;

        case  GPIO_LIST_J2_ENT_PHASE1_SATURATED:
                #define _GL_D(pin, asicPinName, InitStats,  EVAL, HT2000, HT2000W, HT2000SBD,J2Aero, J2EntPh1Lin, J2EntPh1Sat, J2EntPh2, J2EntPh2_MM, HT2000_MM, dir, comment) \
                    _GPIO_HANDLER(pin, asicPinName, J2EntPh1Sat,  dir, comment)
                #define _GL_E   _GL_D
                #define _GL_F   _GL_D

                _ALL_GPIO_LIST
                #undef _GL_D
                #undef _GL_E
                #undef _GL_F

            break;

        case  GPIO_LIST_J2_ENT_PHASE2:
                #define _GL_D(pin, asicPinName, InitStats,  EVAL, HT2000, HT2000W, HT2000SBD,J2Aero, J2EntPh1Lin, J2EntPh1Sat, J2EntPh2, J2EntPh2_MM, HT2000_MM, dir, comment) \
                    _GPIO_HANDLER(pin, asicPinName, J2EntPh2,  dir, comment)
                #define _GL_E   _GL_D
                #define _GL_F   _GL_D

                _ALL_GPIO_LIST
                #undef _GL_D
                #undef _GL_E
                #undef _GL_F

            break;
        default:
            break;
    }

    return FALSE;

#undef _GPIO_HANDLER
#undef _GPIO_LIST_PER_BASE
#undef _ALL_GPIO_LIST

#endif

    /* all other platforms are unable to verify, so let it pass */
    return TRUE;
}


/* The maximum supported attenation differs between different types of linear boards:
    - Multimode consumer:   31.0 dB
    - Multimode enterprise: 62.0 dB
    - All other linear:     47.0 dB
 */
static inline int GET_LINEAR_BOARD_TYPE_MAX_ATTENUATION()
{
    int max_attenuation = MAX_ATTEN_MULTIMODE_ENTERPRISE; /* default to highest max attenuation, just in case */
    iduType_e bType = getIduBoardStrappingVal();

    if (IS_BOARD_TYPE_LINEAR())
    {
        if (IS_BOARD_TYPE_MULTI_MODE())
        {
            if (IS_BOARD_TYPE_JUP2_MULTIMODE_ENT_LINEAR_PHASE2(bType))
                max_attenuation = MAX_ATTEN_MULTIMODE_ENTERPRISE;
            else
                max_attenuation = MAX_ATTEN_MULTIMODE_CONSUMER;
        }
        else
        {
            max_attenuation = MAX_ATTEN_NON_MULTIMODE_LINEAR;
        }
    }

    return max_attenuation;
}


static inline void GPIO_VALIDATION_FAILED(const char * gpio_base, const char * gpio_name, int gpio_bit,  const char * callerFuncName, int calledAtLineNo) \
{\
    FILE * fp = fopen("/tmp/gpio_operation_error.log", "a");\
    if(fp)\
    {\
        fprintf(fp,\
                "[%s line# %d] - calling GPIO-name:%s "\
                "for GPIO-pin#:%d. GPIO either not defined or pin# mismatch "\
                "in GPIO_LIST_%d (/cm_data/jupsw/terminal/callisto/apps/gpio.h)\n",\
                callerFuncName, calledAtLineNo, gpio_name, gpio_bit, BOARD_TYPE_GPIO_LIST_GET());\
        fclose(fp);\
    }\
}

/* Read GPIO state, with GPIO verification for the board type:
    e.g., GPIO_UPP_TX_DIV_MC_Read_WITH_VALIDATION(void)

 return: 0 or 1 if read successfully; otherwise ERROR.
 So, the caller has to check all cases: 0, 1, and ERROR */
#define _GPIO_D(gpio_name, gpio_base, gpio_bit, direction) \
static inline int GPIO_##gpio_base##_##gpio_name##_Read_WITH_VALIDATION(const char * callerFuncName, int calledAtLineNo) \
{\
    if(_gpio_name_pin_is_valid(#gpio_name, gpio_bit) == TRUE)\
    {\
        if(gpio_base##_gpioBasePtr == NULL) \
            gpio_base##_gpioBasePtr = (gpio_base##_GPIO_REGS *)MAP_REG_BLK(gpio_base##_GPIO);\
        if(gpio_base##_gpioBasePtr) \
        {    /* return the GPIO value: 0 or 1, if read sucessfully */\
            return (((gpio_base##_gpioBasePtr->gpio_base##_GPIO_DIN_RO) & (1<<gpio_bit)) != 0);\
        }\
    }\
    else GPIO_VALIDATION_FAILED(#gpio_base, #gpio_name, gpio_bit, callerFuncName, calledAtLineNo);\
    return ERROR; /* anything bad, return ERROR; caller MUST check if read successfully */\
}
GPIO_FULL_LIST
#undef _GPIO_D

/* Write On/Off value to a GPIO, with GPIO verification for the board type:
   e.g., GPIO_UPP_TX_DIV_MC_Write_WITH_VALIDATION(GPIO_OUT_VAL_1 or GPIO_OUT_VAL_0) */
#define _GPIO_D(gpio_name, gpio_base, gpio_bit, direction) \
static inline int GPIO_##gpio_base##_##gpio_name##_Write_WITH_VALIDATION(int on_off, const char * callerFuncName, int calledAtLineNo) \
{\
    if(_gpio_name_pin_is_valid(#gpio_name, gpio_bit) == TRUE)\
    {\
        if(gpio_base##_gpioBasePtr == NULL) \
            gpio_base##_gpioBasePtr = (gpio_base##_GPIO_REGS *)MAP_REG_BLK(gpio_base##_GPIO);\
        if(gpio_base##_gpioBasePtr) \
        {\
            gpio_base##_gpioBasePtr->gpio_base##_GPIO_DIR_CNTL_RWE = (1<<(gpio_bit+16)) | (1<<gpio_bit); /* output mode for the bit */\
            on_off = (on_off != 0); /* so that either 0 (OFF) or 1 (ON)*/ \
            gpio_base##_gpioBasePtr->gpio_base##_GPIO_DOUT_RWE = (1<<(gpio_bit+16)) | (on_off<<gpio_bit); /* set on/off for the bit */\
            return OK;\
        }\
    }\
    else GPIO_VALIDATION_FAILED(#gpio_base, #gpio_name, gpio_bit, callerFuncName, calledAtLineNo); \
    return ERROR; \
}
GPIO_FULL_LIST
#undef _GPIO_D

/* Set desired direction for a GPIO with GPIO verification for the board type:
   e.g.,   GPIO_UPP_TX_DIV_MC_Direction_set_WITH_VALIDATION(GPIO_DIR_IN) */
#define _GPIO_D(gpio_name, gpio_base, gpio_bit, direction) \
static inline int GPIO_##gpio_base##_##gpio_name##_Direction_set_WITH_VALIDATION(int in_out, const char * callerFuncName, int calledAtLineNo)\
{\
    if(_gpio_name_pin_is_valid(#gpio_name, gpio_bit) == TRUE)\
    {\
        if(gpio_base##_gpioBasePtr == NULL) \
            gpio_base##_gpioBasePtr = (gpio_base##_GPIO_REGS *)MAP_REG_BLK(gpio_base##_GPIO);\
        if(gpio_base##_gpioBasePtr) \
        {\
            if(in_out)  in_out = 1; \
            gpio_base##_gpioBasePtr->gpio_base##_GPIO_DIR_CNTL_RWE = (1<<(gpio_bit+16)) | (in_out << gpio_bit);\
            return OK;\
        }\
    }\
    else GPIO_VALIDATION_FAILED(#gpio_base, #gpio_name, gpio_bit, callerFuncName, calledAtLineNo); \
    return ERROR;\
}
GPIO_FULL_LIST
#undef _GPIO_D

/* return a new value after swapping betweent two bits of a 32-bit unsigned int;
   swap bits: bit_m/bit_n, rang [0:31] */
static inline unsigned int  bitsSwap(unsigned int inputData, int bit_m, int bit_n)
{
    /* swap bits: bit_m and bit_n in inputData */
    unsigned int swappedData = inputData;

    /* zero out bit_m and bit_n */
    swappedData &= ~((1<<bit_m) | (1<<bit_n));

    /* swap the bit_m in data into bit_n in swappedData */
    swappedData  |= ((inputData & (1<<bit_m)) != 0) << bit_n;

    /* swap the bit_n in data into bit_m in swappedData */
    swappedData  |= ((inputData & (1<<bit_n)) != 0) << bit_m;

    return swappedData;
}

/************************************************
  functions may or may not be per board specific
*************************************************/

/* utility functions */
extern unsigned int getSaiNum();
extern unsigned int setSaiNum(unsigned int sai);
extern unsigned int getSerNum();
extern unsigned int regAddrLookup(char * regAddrStr);
extern char * regNameLookup(unsigned int regAddr);
extern int getNumActiveCores();
extern int getSanName(char * san, int bufferSize);
extern int getTerminalLatitude(char * latitude, int bufferSize);
extern int getTerminalLongitude(char * longitude, int bufferSize);
extern int getInitialRegistrationDate(char * initialRegistrationDate, int bufferSize);
extern int getLastRegistrationDate(char * lastRegistrationDate, int bufferSize);

extern unsigned int GetRand(unsigned int mod);
extern int GetRandRange(int low, int high);
extern int torben( unsigned int  m[], int n,
                            unsigned int  * maxVal,
                            unsigned int  * minVal,
                            unsigned int  * meanVal, int dbg);
void createProcess(char * bin, char * argv[], int * pid);
void killProcess(int pid, int signal);

extern int statecodeHysteresisInEffect(void);
extern void statecodeHysteresisDecrement(void);
extern void statecodeHysteresisSet(void);

extern int multiModeTXCfmMode(void);
extern int multiModeTXCfmModeSaturated(void);
extern int mobilityArchitectureCfmMode(void);

extern int utilIsTVOATerminal(void);
extern int utilIsGWHMTerminal(void);
extern int utilIsTestTerminal(void);
extern enum testTerminal_e utilTestTerminalTypeGet(void);
extern void utilTestTerminalTypeSet(enum testTerminal_e testTerminalType);

int gen_random(unsigned int min, unsigned int max);

#ifdef __cplusplus
}
#endif

#endif
