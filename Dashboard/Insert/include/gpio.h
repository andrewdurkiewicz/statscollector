/**************************************************************************
 *
 *  FILE NAME:    gpio.h
 *
 *  DESCRIPTION: GPIO definition and access macro/functions
 *
 * ICD: Document #
 *
 *     DATE,   NAME,     REFERENCE        DESCRIPTION
 *  ----------  -------------   --------------    ----------------------
 *  03-05-2013  zfan,      SPR 6848,   Original for user space GPIO access
 *
 **************************************************************************

The Thebe platform is based on:
"J2 IDU Variant Comparison_revised09212016_rev6.xlsx" provided by HW team (Kang Yuan)

Read-Me:
(1) there are 3 GPIO HW logics: SWP, DPP and UPP respectively.
    Each has its own registers for GPIO control:
    GPIO_DIR_CNTL_RWE - for GPIO Direction IN/OUT
    GPIO_DOUT_RWE - for Output Level (if Direction is OUT)
    In addtion, some GPIO pins can be shared with other HW logic as configured by register:
    SWP_PIN_SHARING_CNTL_RWE
(2) Preferably (or normally), SWP SW controls SWP GPIO, DDP FW controls DPP GPIO, and UPP FW controls UPP GPIO
    but there is exception (which will be specified explicitly in GPIO definitions).
(3) Currently, Acadia ASIC IDUs has 8 different set of GPIO definitions, for various board strapping options.
    Each strapping is using a specific set of the GPIO out of these GPIO sets, as defined in board type list (utilsLib.h)
(4) GPIO read/write must use the driver functions provided in this file. Directly read/write into gpio registers
    are STRICTLY PROHIBITED, and deemed as coding error, which should be fixed as soon as possible, except for
    the code directly ported or copied from HW/FW team such as acadia_txha_init.c
(5) At Advanced Web: "IDU/ODU" -> "IDU Info" displays the GPIO information pertaining to each specific IDU

Finally,
(6) Just include this file, then the following interface functions and constants are yours.
    (No initialization needed)

     * * * *  for access to multiple GPIO pins in one operation * * * *
int SWP_GPIO_Read_UsingBitmap(unsigned int bitmask);
int UPP_GPIO_Read_UsingBitmap(unsigned int bitmask);
int DPP_GPIO_Read_UsingBitmap(unsigned int bitmask);

void SWP_GPIO_Write_UsingBitmap(unsigned int enable_bitmask, unsigned int value);
void UPP_GPIO_Write_UsingBitmap(unsigned int enable_bitmask, unsigned int value);
void DPP_GPIO_Write_UsingBitmap(unsigned int enable_bitmask, unsigned int value);

void SWP_GPIO_Set_Dir_UsingBitmap(unsigned int enable_bitmask, unsigned int dir_value);
void UPP_GPIO_Set_Dir_UsingBitmap(unsigned int enable_bitmask, unsigned int dir_value);
void DPP_GPIO_Set_Dir_UsingBitmap(unsigned int enable_bitmask, unsigned int dir_value);

     * * * * For single GPIO PIN access in one operation: Read/Write/Set_Dir function per one bit  * * * *
*/

#if 0  /* more Read-Me: examples of GPIO API functions for single pin */

/* for legacy code already used */
int  GPIO_FCS_LNB_MOD_Read(void);
void GPIO_FCS_LNB_MOD_Write(int on_off);
void GPIO_FCS_LNB_MOD_Set_Dir(void);

/* for new code, preferred to use */
int  GPIO_DPP_FCS_LNB_MOD_Read(void);
void GPIO_DPP_FCS_LNB_MOD_Write(int on_off);
void GPIO_DPP_FCS_LNB_MOD_Set_Dir(void);                    /* set to default direction */
void GPIO_DPP_FCS_LNB_MOD_Direction_set(int GPIO_DIR_IN);   /* set to desired direction */

/* for legacy code already used */
int  GPIO_RCS_DAC_RESET_Read(void);
void GPIO_RCS_DAC_RESET_Write(int on_off);
void GPIO_RCS_DAC_RESET_Set_Dir(void);

/* for new code, preferred to use */
int  GPIO_DPP_RCS_DAC_RESET_Read(void);
void GPIO_DPP_RCS_DAC_RESET_Write(int on_off);
void GPIO_DPP_RCS_DAC_RESET_Set_Dir(void);
void GPIO_DPP_RCS_DAC_RESET_Direction_set(int GPIO_DIR_IN);

/* for legacy code already used */
int  GPIO_TX_MOD_OFF_Read(void);
void GPIO_TX_MOD_OFF_Write(int on_off);
void GPIO_TX_MOD_OFF_Set_Dir(void);

/* for new code, preferred to use */
int  GPIO_UPP_GPIO_TX_MOD_LOCK_Read(void);
void GPIO_UPP_GPIO_TX_MOD_LOCK_Write(int on_off);
void GPIO_UPP_TX_MOD_LOCK_Set_Dir(void);
void GPIO_UPP_TX_MOD_LOCK_Direction_set(int GPIO_DIR_IN);

/* for legacy code already used */
int  GPIO_TX_DIV_ON_Read(void);
void GPIO_TX_DIV_ON_Write(int on_off);
void GPIO_TX_DIV_ON_Set_Dir(void);
.
int  GPIO_TX_DIV_MC_Read(void);
void GPIO_TX_DIV_MC_Write(int on_off);
void GPIO_TX_DIV_MC_Set_Dir(void);

/* for new code, preferred to use */
int  GPIO_SWP_TX_DIV_MC_Read(void);
void GPIO_SWP_TX_DIV_MC_Write(int on_off);
void GPIO_SWP_TX_DIV_MC_Set_Dir(void);
void GPIO_SWP_TX_DIV_MC_Direction_set(int GPIO_DIR_IN);

/* for legacy code already used */
int  GPIO_TX_DIV_SW1_Read(void);
void GPIO_TX_DIV_SW1_Write(int on_off);
void GPIO_TX_DIV_SW1_Set_Dir(void);

/* for new code, preferred to use */
int  GPIO_SWP_TX_DIV_SW1_Read(void);
void GPIO_SWP_TX_DIV_SW1_Write(int on_off);
void GPIO_SWP_TX_DIV_SW1_Set_Dir(void);
void GPIO_SWP_TX_DIV_SW1_Direction_set(int GPIO_DIR_IN);

/* for legacy code already used */
int  GPIO_TX_DIV_SW2_Read(void);
void GPIO_TX_DIV_SW2_Write(int on_off);
void GPIO_TX_DIV_SW2_Set_Dir(void);

/* for new code, preferred to use */
int  GPIO_SWP_TX_DIV_SW2_Read(void);
void GPIO_SWP_TX_DIV_SW2_Write(int on_off);
void GPIO_SWP_TX_DIV_SW2_Set_Dir(void);
void GPIO_SWP_TX_DIV_SW2_Direction_set(int GPIO_DIR_IN);

/*
 The above is not a full list: by now, you should realized, for legacy code, the GPIO pin does not come with
 a base name (UPP, DPP, or SWP), but for the new preferred functions, you always pre-append the base name
 to the pin name. The pin name can be either found in the table or GPIO_FULL_LIST

*/

#endif

/* Final Read-Me:

    * * * You can only use enumeric constants defined below as input parms/values * * *
GPIO_DIR_IN = 0
GPIO_DIR_OUT = 1

GPIO_OUT_VAL_0 = 0
GPIO_OUT_VAL_1 = 1

    * * * they can be or'ed to make up a bitmask or value * * *
    * * * e.g., (UPP_GPIO_RCS_DAC_RESET_MASK | UPP_GPIO_TX_MOD_OFF_MASK) * * *
UPP_GPIO_FCS_LNB_MOD_MASK   = (1<<7)
UPP_GPIO_RCS_DAC_RESET_MASK = (1<<2)
UPP_GPIO_TX_MOD_OFF_MASK    = (1<<0)
UPP_GPIO_TX_MOD_LOCK_MASK   = (1<<1)
UPP_GPIO_TX_DIV_ON_MASK     = (1<<15)
UPP_GPIO_TX_DIV_MC_MASK     = (1<<13)

SWP_GPIO_TX_DIV_SW2_MASK    = (1<<8)
SWP_GPIO_TX_DIV_SW1_MASK    = (1<<9)

However, the above is not a full list of GPIO pin functions.
For a full list per IDU type, refer to tables: SWP_GPIO_LISTS/DPP_GPIO_LISTS/UPP_GPIO_LISTS
as defined below

If you get bored, just stop reading.
Above information should be enough!

*************************************************************************/

#ifndef _GPIO_H
#define _GPIO_H
#include "memmapping.h"
#include <hw-regs.h>

/* I am copied and then reformed from "J2 IDU Variant Comparison_revised03302016_rev5.xlsx: GPIO Comparison" */


/**********************************************************************************************************************************************************************************************************************************************************************************************************************************
copy and paste them in your code when need to define the list

#define  _GL_D(pinNum, asicPinMapped, initState, Eval, HT2000, HT2000W, HT2000SBD, J2Aero, J2EntPhase1Linear, J2EntPhase1Saturated, J2EntPhase2,J2 Ent Phase2 SIFL MM, HT2000_MM, direction, comment)
#define  _GL_E(pinNum, asicPinMapped, initState, Eval, HT2000, HT2000W, HT2000SBD, J2Aero, J2EntPhase1Linear, J2EntPhase1Saturated, J2EntPhase2,J2 Ent Phase2 SIFL MM, HT2000_MM, direction, comment)
#define  _GL_F(pinNum, asicPinMapped, initState, Eval, HT2000, HT2000W, HT2000SBD, J2Aero, J2EntPhase1Linear, J2EntPhase1Saturated, J2EntPhase2,J2 Ent Phase2 SIFL MM, HT2000_MM, direction, comment)

Initial state "z" means floating state
******************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
/*                                                                                     HT2000W / HT2004                                                                                       */
/*     GPIO   ASIC PIN MAPPING  INITIAL STATE   Eval USAGE        HT2000 Usage         HT2000 Redesign        HT2000 SBD            J2 Aero              J2 Ent Phase1 Linear    J2 Ent Phase1 Sat      J2 Ent Phase2,         J2 Ent Phase2 SIFL MM, HT2000 MM             DIRECTION,    PURPOSE */
#define DPP_GPIO_LISTS \
/******************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/\
    _GL_D(7,    FCS_GPIO_07,           Z,     WIFI_RESET_N,       WIFI_RESET_N,        WIFI_RESET_N,          WIFI_RESET_N,         NOT_USED,            WIFI_RESET_N,           WIFI_RESET_N,          WIFI_RESET_N,          WIFI_RESET_N,          WIFI_RESET_N,           OUT,       "Control for reset of wifi daughtercard")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(6,    FCS_GPIO_06,           Z,     FAN_PWR_CTRL_N,     FAN_PWR_CTRL_N,      NOT_USED,              NOT_USED,             FAN_PWR_CTRL_N,      NOT_USED,               NOT_USED,              NOT_USED,              NOT_USED,              NOT_USED,               OUT,       "Control Heat-Sink Fan ON/OFF")\
    _GL_E(6,    FCS_GPIO_06,           Z,     NOT_USED,           NOT_USED,            NOT_USED,              NOT_USED,             NOT_USED,            RS_485_IN_RX,           RS_485_IN_RX,          RS_485_IN_RX,          RS_485_IN_RX,          NOT_USED,               In,        "RX signal for ODU communication  on DB-15 connector (RM only)")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(5,    FCS_GPIO_05,           Z,     NOT_USED,           NOT_USED,            NOT_USED,              NOT_USED,             LOW_TEMP_SHUTDOWN,   RS_485_IN_TX,           RS_485_IN_TX,          RS_485_IN_TX,          RS_485_IN_TX,          NOT_USED,               OUT,       "LOW_TEMP_SHUTDOWN; RX signal for ODU communication  on DB-15 connector (RM only)")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(4,    FCS_GPIO_04,           Z,     NOT_USED,           NOT_USED,            NOT_USED,              NOT_USED,             NOT_USED,            NOT_USED,               NOT_USED,              FPGA_INIT,             FPGA_INIT,             NOT_USED,               OUT,       "FPGA Configuration")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(3,    FCS_GPIO_03,           Z,     FCS_UART_RXD,       NOT_USED,            NOT_USED,              NOT_USED,             FCS_UART_RXD,        FCS_UART_RXD,           FCS_UART_RXD,          NOT_USED,              NOT_USED,              NOT_USED,               IN,        "Receive signal for FCS UART Interface")\
    _GL_E(3,    FCS_GPIO_03,           Z,     WDT_DISABLE_INT,    NOT_USED,            NOT_USED,              NOT_USED,             NOT_USED,            WDT_DISABLE_INT,        WDT_DISABLE_INT,       NOT_USED,              NOT_USED,              NOT_USED,               IN,        "Disable watchdog timer")\
    _GL_F(3,    FCS_GPIO_03,           Z,     FCS_NEMA_GPS_RXD,   NOT_USED,            NOT_USED,              NOT_USED,             NOT_USED,            FCS_NEMA_GPS_RXD,       FCS_NEMA_GPS_RXD,      FCS_NEMA_GPS_RXD,      FCS_NEMA_GPS_RXD,      NOT_USED,               IN,        "GPS/Tracking antenna")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(2,    FCS_GPIO_02,           Z,     FCS_UART_TXD_INT,   NOT_USED,            NOT_USED,              NOT_USED,             FCS_UART_TXD_INT,    FCS_UART_TXD_INT,       FCS_UART_TXD_INT,      NOT_USED,              NOT_USED,              NOT_USED,               OUT,       "Transmit signal for FCS UART Interface")\
    _GL_E(2,    FCS_GPIO_02,           Z,     FCS_NEMA_GPS_TXD,   NOT_USED,            NOT_USED,              NOT_USED,             NOT_USED,            FCS_NEMA_GPS_TXD,       FCS_NEMA_GPS_TXD,      FCS_NEMA_GPS_TXD,      FCS_NEMA_GPS_TXD,      NOT_USED,               OUT,       "GPS/Tracking antenna")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(1,    FCS_GPIO_01,           Z,     NOT_USED,           NOT_USED,            NOT_USED,              NOT_USED,             NOT_USED,            OUTROUTE_LOCK,          OUTROUTE_LOCK,         OUTROUTE_LOCK,         OUTROUTE_LOCK,         NOT_USED,               OUT,       "Outroute Demod Lock indicator ")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(0,    FCS_GPIO_00,           Z,     NOT_USED,           NOT_USED,            NOT_USED,              NOT_USED,             NOT_USED,            ANTENNA_CTRL,           ANTENNA_CTRL,          ANTENNA_CTRL,          ANTENNA_CTRL,          NOT_USED,               IN,        "Antenna Mispoint-Disable TX")\

/******************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define UPP_GPIO_LISTS /*                     Eval USAGE,         HT2000 Usage,        HT2000 Redesign,       HT2000 SBD            J2 Aero,             J2 Ent Phase1 Linear    J2 Ent Phase1 Sat      J2 Ent Phase2,         J2 Ent Phase2 SIFL MM, HT2000 MM             DIRECTION,   PURPOSE */\
/******************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/\
    _GL_D(15,   RCS_GPIO_15,           Z,     TX_DIV_MC,          NOT_USED,            NOT_USED,              NOT_USED,             REF_CTRL_INT,        TX_DIV_MC,              TX_DIV_MC,             TX_DIV_MC,             LIN_SAT_SW,            LIN_SAT_SW,             OUT,       "TX_DIV_MC: Divider control (1/3); REF_CTRL_INT: Enable/Disable 50MHz REF output")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(14,   RCS_PWR_DISEQC,        0,     RCS_PWR_DISEQC,     RCS_PWR_DISEQC,      RCS_PWR_DISEQC,        RCS_PWR_DISEQC,       AC_PWR_FAIL,         RCS_PWR_DISEQC,         RCS_PWR_DISEQC,        RCS_PWR_DISEQC,        RCS_PWR_DISEQC,        RCS_PWR_DISEQC,         OUT,       "RCS_PWR_DISEQC: Outputs either 2.5MHz PDM for Saturated ODU or TX DiSEqc (2.5M); control switch is integrated within the ASIC")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(13,   RCS_GPIO_13,           Z,     TX_ON,              TX_ON,               TX_ON,                 TX_ON,                TX_AMP_EN_P13,       TX_ON,                  TX_ON,                 TX_ON,                 TX_ON,                 TX_ON,                  OUT,       "Enable/Disable TX Ref for Saturated; Enable/Disable 10MHz for Linear; TX-AMP-EN: Enable/Disables TX path amplifiers")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(12,   RCS_TEST_2,            0,     LNB_CNTRL_FAULT,    NOT_USED,            NOT_USED,              NOT_USED,             LNB_CNTRL_FAULT,     LNB_CNTRL_FAULT,        NOT_USED,              LNB_CNTRL_FAULT,       LNB_CNTRL_FAULT,       NOT_USED,               IN,        "LNB Fault indicator")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(11,   RCS_TEST_1,            0,     NOT_USED,           NOT_USED,            NOT_USED,              NOT_USED,             NOT_USED,            NOT_USED,               NOT_USED,              PHY_INT,               PHY_INT,               NOT_USED,               IN,        "Interrupt from the 1588 PHY - Used only on CBOS terminals ")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(10,   RCS_TEST_0,            0,     NOT_USED,           NOT_USED,            NOT_USED,              NOT_USED,             NOT_USED,            10MHZ_SW_P10,           NOT_USED,              10MHZ_SW_P10,          10MHZ_SW_P10,          NOT_USED,               OUT,       "10MHz_SW: switch between FLL or 10MHz input (mobility)")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(9,    RCS_GPIO_9,            Z,     2P5MHZ_PDM,         2P5MHZ_PWR_PDM,      2P5MHZ_PWR_PDM,        2P5MHZ_PWR_PDM,       10MHZ_SW_P9,         TX_BAND_SEL_P9,         NOT_USED,              TX_BAND_SEL_P9,        TX_BAND_SEL_P9,        NOT_USED,               OUT,       "2.5MHZ_PDM: tunable filter (six bands);  2.5MHZ-PWR-PDM: set to logic-0; TX-Band-Sel: switchable band filter (two bands); 10MHz-SW: switch b/w FLL or 10MHz input (Mobility)")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(8,    RCS_GPIO_8,            Z,     WIFI_AP_WAKE_N,     WIFI_AP_WAKE_N,      WIFI_AP_WAKE_N,        WIFI_AP_WAKE_N,       NOT_USED,            WIFI_AP_WAKE_N,         WIFI_AP_WAKE_N,        WIFI_AP_WAKE_N,        WIFI_AP_WAKE_N,        WIFI_AP_WAKE_N,         IN,        "Interrupt from WiFi card alerting to incoming traffic")\
    _GL_E(8,    RCS_GPIO_8,            Z,     SWP_SPI2_SS01,      NOT_USED,            NOT_USED,              WAKE_ON_LAN_N,        TX_MUTE_N,           WAKE_ON_LAN_N,          WAKE_ON_LAN_N,         WAKE_ON_LAN_N,         WAKE_ON_LAN_N,         NOT_USED,               IN,        "Interrupt from ethernet switch alerting to wake up from low power mode")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(7,    FCS_LNB_25,            0,     FCS_LNB_27MHZ,      FCS_LNB_27MHZ,       FCS_LNB_27MHZ,         FCS_LNB_27MHZ,        TX_BAND_SEL_P7,      FCS_LNB_27MHZ,          FCS_LNB_27MHZ,         FCS_LNB_27MHZ,         FCS_LNB_27MHZ,         FCS_LNB_27MHZ,          OUT,       "FCS-LNB-27: LNB LO Selection for Saturated; TX-Band-Sel: switchable band filter (two bands)")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(6,    RCS_GPIO_6,            Z,     SWP_SPI2_SS00,      NOT_USED,            NOT_USED,              NOT_USED,             EXT_10MHZ_LOCK,      EXT_10MHZ_LOCK,         NOT_USED,              EXT_10MHZ_LOCK,        EXT_10MHZ_LOCK,        NOT_USED,               IN,        "10MHz Input mobility PLL lock indicator")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(5,    RCS_GPIO_5,            Z,     SWP_SPI2_MISO,      RCS_UART_TXD,        RCS_UART_TXD,          RCS_UART_TXD,         NOT_USED,            RCS_UART_TXD,           RCS_UART_TXD,          RCS_UART_TXD,          RCS_UART_TXD,          RCS_UART_TXD,           OUT,       "UART TX to daughtercard")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(4,    RCS_GPIO_4,            Z,     SWP_SPI2_MOSI,      RCS_UART_RXD,        RCS_UART_RXD,          RCS_UART_RXD,         NOT_USED,            RCS_UART_RXD,           RCS_UART_RXD,          RCS_UART_RXD,          RCS_UART_RXD,          RCS_UART_RXD,           IN,        "UART RX to daughtercard")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(3,    RCS_GPIO_3,            Z,     SWP_SPI2_SCLK,      NOT_USED,            NOT_USED,              NOT_USED,             NOT_USED,            NOT_USED,               NOT_USED,              NOT_USED,              NOT_USED,              NOT_USED,               OUT,       "SPI Clock")\
    _GL_E(3,    RCS_GPIO_3,            Z,     NOT_USED,           NOT_USED,            NOT_USED,              NOT_USED,             10MHZ_DET,           10MHZ_DET,              NOT_USED,              10MHZ_DET,             10MHZ_DET,             NOT_USED,               IN,        "10MHZ_DET:10MHz input (mobility) detector indicator")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(2,    RCS_GPIO_2,            Z,     RCS_DAC_RESET,      NOT_USED,            NOT_USED,              NOT_USED,             RCS_DAC_RESET,       TX_AMP_EN_P2,           NOT_USED,              TX_AMP_EN_P2,          TX_AMP_EN_P2,          NOT_USED,               OUT,       "Used to reset the DAC Chip AD9743. Used in all board confi")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(1,    RCS_GPIO_1,            Z,     DAC_CLK_LOCK,       DAC_CLK_LOCK,        DAC_CLK_LOCK,          DAC_CLK_LOCK,         DAC_CLK_LOCK,        DAC_CLK_LOCK,           DAC_CLK_LOCK,          DAC_CLK_LOCK,          NOT_USED,              NOT_USED,               IN,        "External DAC clock/PLL Lock indicator")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(0,    RCS_GPIO_0,            Z,     NOT_USED,           NOT_USED,            NOT_USED,              NOT_USED,             NOT_USED,            NOT_USED,               NOT_USED,              NOT_USED,              TRF_MOD_LOCK,          TX_REF_LOCK,            IN,       "TX-REF-LOCK: LMX2571 PLL lock status; TRF-MOD-LOCK: TRF3722 PLL Lock Status")\


/******************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
#define SWP_GPIO_LISTS  /*                    Eval USAGE,         HT2000 Usage,        HT2000 Redesign,       HT2000 SBD            J2 Aero,             J2 Ent Phase1 Linear    J2 Ent Phase1 Sat      J2 Ent Phase2,         J2 Ent Phase2 SIFL MM, HT2000 MM             DIRECTION,     PURPOSE */\
/*********************************************************************************************************************************************************************************************************************************************************************************************************************************************************/\
    _GL_D(15,   SWP_GPIO_15,           Z,     SPI_SRDY,           NOT_USED,            MPU_RST_IN_N,          MPU_RST_IN_N,         NOT_USED,            MPU_RST_IN_N,           MPU_RST_IN_N,          MPU_RST_IN_N,          MPU_RST_IN_N,          MPU_RST_IN_N,           IN,         "Hard reset of MPU - used for programming; AC-PWR-FAIL: Logic low or high as an indicator to the MO")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(14,   SWP_GPIO_14,           Z,     SPI_MRDY,           NOT_USED,            NOT_USED,              NOT_USED,             NOT_USED,            SPI_MRDY,               NOT_USED,              FPGA_PROG,             FPGA_PROG,             NOT_USED,               OUT,        "FPGA Configuration")\
    _GL_E(14,   SWP_GPIO_14,           Z,     CORE_3_ON,          CORE_3_ON,           CORE_3_ON,             CORE_3_ON,            NOT_USED,            CORE_3_ON,              CORE_3_ON,             CORE_3_ON,             CORE_3_ON,             CORE_3_ON,              IN,         "Strapping input for Enabling Acadia - core-3 ")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(13,   SWP_GPIO_13,           Z,     USB_OVERCURRENT_N,  USB_OVERCURRENT_N,   USB_OVERCURRENT_N,     USB_OVERCURRENT_N,    MODMAN_SLOT,         USB_OVERCURRENT_N,      USB_OVERCURRENT_N,     USB_OVERCURRENT_N,     USB_OVERCURRENT_N,     USB_OVERCURRENT_N,      IN,         "Goes low to indicate that the USB device is consuming more than 500mA")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(12,   SWP_GPIO_12,           Z,     USB_PWR_CTRL_N,     USB_PWR_CTRL_N,      USB_PWR_CTRL_N,        USB_PWR_CTRL_N,       NOT_USED,            USB_PWR_CTRL_N,         USB_PWR_CTRL_N,        USB_PWR_CTRL_N,        USB_PWR_CTRL_N,        USB_PWR_CTRL_N,         OUT,        "Enable +5.0V-USB")\
    _GL_E(12,   SWP_GPIO_12,           Z,     CORE_2_ON,          CORE_2_ON,           CORE_2_ON,             CORE_2_ON,            NOT_USED,            CORE_2_ON,              CORE_2_ON,             CORE_2_ON,             CORE_2_ON,             CORE_2_ON,              IN,         "Strapping input for Enabling Acadia - core-2")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(11,   SWP_GPIO_11,           Z,     MPU_IPF_N,          MPU_IPF_N,           MPU_IPF_N,             WIFI_LED,             MPU_IPF_N,           MPU_IPF_N,              MPU_IPF_N,             WIFI_LED,              WIFI_LED,              MPU_IPF_N,              IN,         "Imminent power failure/front panel WiFi activity")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(10,   SWP_GPIO_10,           Z,     BOOT_SEL2,          BOOT_SEL2,           BOOT_SEL2,             BOOT_SEL2,            BOOT_SEL2,           BOOT_SEL2,              BOOT_SEL2,             BOOT_SEL2,             BOOT_SEL2,             BOOT_SEL2,              IN,         "Selects the device from which to boot from")\
    _GL_E(10,   SWP_GPIO_10,           Z,     TX_DIV_SW2,         NOT_USED,            NOT_USED,              NOT_USED,             NOT_USED,            TX_DIV_SW2,             TX_DIV_SW2,            TX_DIV_SW2,            NOT_USED,              NOT_USED,               OUT,        "Divider Control (3/3)")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(9,    SWP_GPIO_09,           Z,     BOOT_SEL1,          BOOT_SEL1,           BOOT_SEL1,             BOOT_SEL1,            BOOT_SEL1,           BOOT_SEL1,              BOOT_SEL1,             BOOT_SEL1,             BOOT_SEL1,             BOOT_SEL1,              IN,         "Selects the device from which to boot from")\
    _GL_E(9,    SWP_GPIO_09,           Z,     TX_DIV_SW1,         TX_DIV_SW1,          TX_DIV_SW1,            TX_DIV_SW1,           NOT_USED,            TX_DIV_SW1,             TX_DIV_SW1,            TX_DIV_SW1,            NOT_USED,              NOT_USED,               OUT,        "Divider Control (2/3)")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(8,    SWP_GPIO_08,           Z,     BOOT_SEL0,          BOOT_SEL0,           BOOT_SEL0,             BOOT_SEL0,            BOOT_SEL0,           BOOT_SEL0,              BOOT_SEL0,             BOOT_SEL0,             BOOT_SEL0,             BOOT_SEL0,              IN,         "Selects the device from which to boot from")\
    _GL_E(8,    SWP_GPIO_08,           Z,     WI_FI_DISABLE,      WI_FI_DISABLE,       WI_FI_DISABLE,         WI_FI_DISABLE,        NOT_USED,            WI_FI_DISABLE,          WI_FI_DISABLE,         WI_FI_DISABLE,         WI_FI_DISABLE,         WI_FI_DISABLE,          OUT,        "Disable Wi_Fi")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(7,    FCS_SPI_SS1,           0,     FCS_SPI_SS01,       NOT_USED,            NOT_USED,              NOT_USED,             NOT_USED,            FCS_SPI_SS01,           FCS_SPI_SS01,          FCS_SPI_SS01,          FCS_SPI_SS01,          NOT_USED,               OUT,        "FCS-SPI chip select 01")\
    _GL_E(7,    FCS_SPI_SS1,           0,     ADC_RESET_N,        NOT_USED,            NOT_USED,              NOT_USED,             ADC_RESET_N,         NOT_USED,               NOT_USED,              ADC_RESET_N,           NOT_USED,              NOT_USED,               OUT,        "External ADC reset")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(6,    SWP_GPIO_06,           Z,     RESCUE_N_INT,       RESCUE_N_INT,        RESCUE_N_INT,          RESCUE_N_INT,         RESCUE_N_INT,        RESCUE_N_INT,           RESCUE_N_INT,          RESCUE_N_INT,          RESCUE_N_INT,          RESCUE_N_INT,           IN,         "Rescue button input")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(5,    SWP_GPIO_05,           Z,     LAN_LED,            LAN_LED,             LAN_LED,               LAN_LED,              LAN_LED,             LAN_LED,                LAN_LED,               LAN_LED,               LAN_LED,               LAN_LED,                OUT,        "LAN link & activity LED indication")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(4,    SWP_GPIO_04,           Z,     TX_LED,             TX_LED,              TX_LED,                TX_LED,               TX_LED,              TX_LED,                 TX_LED,                TX_LED,                TX_LED,                TX_LED,                 OUT,        "Satellite TX LED control")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(3,    SWP_GPIO_03,           Z,     RX_LED,             RX_LED,              RX_LED,                RX_LED,               RX_LED,              RX_LED,                 RX_LED,                RX_LED,                RX_LED,                RX_LED,                 OUT,        "Satellite RX LED control")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(2,    SWP_GPIO_02,           Z,     SYS_LED,            SYS_LED,             SYS_LED,               SYS_LED,              SYS_LED,             SYS_LED,                SYS_LED,               SYS_LED,               SYS_LED,               SYS_LED,                OUT,        "System status LED control")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(1,    SWP_GPIO_01,           Z,     PWR_LED_BLUE,       PWR_LED_BLUE,        PWR_LED_BLUE,          PWR_LED_BLUE,         PWR_LED_BLUE,        PWR_LED_BLUE,           PWR_LED_BLUE,          PWR_LED_BLUE,          PWR_LED_BLUE,          PWR_LED_BLUE,           OUT,        "Power LED control")\
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/\
    _GL_D(0,    SWP_GPIO_00,           Z,     PWR_LED_RED,        PWR_LED_RED,         PWR_LED_RED,           PWR_LED_RED,          PWR_LED_RED,         PWR_LED_RED,            PWR_LED_RED,           PWR_LED_RED,           PWR_LED_RED,           PWR_LED_RED,            OUT,        "Indication of over_temp condition")\



/* board type and its GPIO initialization options */
#define BOARD_GPIO_LIST \
    _G_D(GPIO_LIST_0, GPIO_LIST_EVAL,                             "Evaluation")\
    _G_D(GPIO_LIST_1, GPIO_LIST_HT2000,                           "HT2000 Usage")\
    _G_D(GPIO_LIST_2, GPIO_LIST_HT2000W,                          "HT2000W/HT2000 Redesign/HT2004")\
    _G_D(GPIO_LIST_3, GPIO_LIST_HT2000SBD,                        "HT2000 SBD")\
    _G_D(GPIO_LIST_4, GPIO_LIST_J2AERO,                           "J2 Aero")\
    _G_D(GPIO_LIST_5, GPIO_LIST_J2_ENT_PHASE1_LINEAR,             "J2 Ent. Phase1 Linear")\
    _G_D(GPIO_LIST_6, GPIO_LIST_J2_ENT_PHASE1_SATURATED,          "J2 Ent. Phase1 Saturated")\
    _G_D(GPIO_LIST_7, GPIO_LIST_J2_ENT_PHASE2,                    "J2 Ent. Phase2")\
    _G_D(GPIO_LIST_8, GPIO_LIST_J2_MULTIMODE_ENT_LINEAR_PHASE2,   "J2 MULTIMODE Ent. Phase2 Linear")\
    _G_D(GPIO_LIST_9, GPIO_LIST_J2_MULTIMODE,                     "J2 MULTIMODE")\

enum {
    GPIO_LIST_DEFAULT = 0,
    GPIO_LIST_INVALID = -1,

    #define _G_D(gpioList, listName, comment) gpioList,  listName = gpioList,
    BOARD_GPIO_LIST
    #undef _G_D

    NUM_GPIO_LISTS,
};

#define GPIO_BASE_LIST \
    _GPIO_BASE_D(SWP)\
    _GPIO_BASE_D(UPP)\
    _GPIO_BASE_D(DPP)\

#define GPIO_FULL_LIST \
    GPIO_DPP_LIST \
    GPIO_UPP_LIST \
    GPIO_SWP_LIST \

#ifdef THEBE_PLATFORM
/* _GPIO_D(name_of_the_GPIO, register_block, gpio_pin#, direction)
    The denitions below must cover all cases and consistent with the table: BOARD_GPIO_LIST defined at above.
    An item with  "backward compatibility" comment indicates it is defined in the table with a different name,
    but this name is used in the legacy code */
#define GPIO_DPP_LIST /* A macro list to produce various entities: data structure, enum constants.. */\
    _GPIO_D(WIFI_RESET_N,         DPP,   7,  GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN28,             DPP,   7,  GPIO_DIR_OUT)  /* backward compatibility */\
\
    _GPIO_D(FAN_PWR_CTRL_N,       DPP,   6,  GPIO_DIR_OUT)\
    _GPIO_D(RS_485_IN_RX,         DPP,   6,  GPIO_DIR_IN)\
\
    _GPIO_D(LOW_TEMP_SHUTDOWN,    DPP,   5,  GPIO_DIR_IN)\
    _GPIO_D(RS_485_IN_TX,         DPP,   5,  GPIO_DIR_OUT)\
    _GPIO_D(LOW_TEMP_STDN,        DPP,   5,  GPIO_DIR_OUT)  /* backward compatibility */\
\
    _GPIO_D(FPGA_INIT,            DPP,   4,  GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN26,             DPP,   4,  GPIO_DIR_OUT) /* backward compatibility */\
\
    _GPIO_D(FCS_UART_RXD,         DPP,   3,  GPIO_DIR_IN)\
    _GPIO_D(WDT_DISABLE_INT,      DPP,   3,  GPIO_DIR_IN)\
    _GPIO_D(FCS_NEMA_GPS_RXD,     DPP,   3,  GPIO_DIR_IN)\
    _GPIO_D(J4_PIN16,             DPP,   3,  GPIO_DIR_IN) /* backward compatibility */\
\
    _GPIO_D(FCS_UART_TXD_INT,     DPP,   2,  GPIO_DIR_OUT)\
    _GPIO_D(FCS_NEMA_GPS_TXD,     DPP,   2,  GPIO_DIR_OUT)\
\
    _GPIO_D(BD_CONFIG_7_INT,      DPP,   1,  GPIO_DIR_OUT)\
    _GPIO_D(OUTROUTE_LOCK,        DPP,   1,  GPIO_DIR_OUT)\
\
    _GPIO_D(BD_CONFIG_6_INT,      DPP,   0,  GPIO_DIR_OUT)\
    _GPIO_D(ANTENNA_CTRL,         DPP,   0,  GPIO_DIR_IN)\

#define GPIO_UPP_LIST /* A macro list to produce various entities: data structure, enum constants... */\
    _GPIO_D(TX_DIV_MC,            UPP,   15, GPIO_DIR_OUT)\
    _GPIO_D(REF_CTRL_INT,         UPP,   15, GPIO_DIR_OUT)\
    _GPIO_D(TX_DIV_ON,            UPP,   15, GPIO_DIR_OUT)   /* backward compatibility */\
    _GPIO_D(LIN_SAT_SW,           UPP,   15, GPIO_DIR_OUT)\
\
    _GPIO_D(RCS_PWR_DISEQC,       UPP,   14, GPIO_DIR_OUT)\
    _GPIO_D(RCS_CNTRL_SEL,        UPP,   14, GPIO_DIR_OUT)   /* backward compatibility */\
    _GPIO_D(AC_PWR_FAIL,          UPP,   14, GPIO_DIR_IN)\
\
    _GPIO_D(TX_ON,                UPP,   13, GPIO_DIR_OUT)\
    _GPIO_D(TX_AMP_EN_P13,        UPP,   13, GPIO_DIR_OUT)\
    _GPIO_D(TX_AMP_EN,            UPP,   13, GPIO_DIR_OUT)  /* backward compatibility */\
\
    _GPIO_D(LNB_CNTRL_FAULT,      UPP,   12, GPIO_DIR_IN)\
\
    _GPIO_D(PHY_INT,              UPP,   11, GPIO_DIR_IN)\
    _GPIO_D(J4_PIN44,             UPP,   11, GPIO_DIR_OUT) /* backward compatibility */\
\
    _GPIO_D(10MHZ_SW_P10,         UPP,   10, GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN40,             UPP,   10, GPIO_DIR_OUT) /* backward compatibility */\
\
    _GPIO_D(2P5MHZ_PDM,           UPP,   9, GPIO_DIR_OUT)\
    _GPIO_D(2P5MHZ_PWR_PDM,       UPP,   9, GPIO_DIR_OUT)\
    _GPIO_D(10MHZ_SW_P9,          UPP,   9, GPIO_DIR_OUT)\
    _GPIO_D(TX_BAND_SEL_P9,       UPP,   9, GPIO_DIR_OUT)\
    _GPIO_D(10MHZ_SW,             UPP,   9, GPIO_DIR_OUT) /* backward compatibility */\
\
    _GPIO_D(WIFI_AP_WAKE_N,       UPP,   8, GPIO_DIR_IN)\
    _GPIO_D(TX_MUTE_N,            UPP,   8, GPIO_DIR_IN)\
    _GPIO_D(SWP_SPI2_SS01,        UPP,   8, GPIO_DIR_N_A)\
    _GPIO_D(WAKE_ON_LAN_N,        UPP,   8, GPIO_DIR_N_A)\
\
    _GPIO_D(FCS_LNB_27MHZ,        UPP,   7, GPIO_DIR_OUT)\
    _GPIO_D(TX_BAND_SEL_P7,       UPP,   7, GPIO_DIR_OUT)\
    _GPIO_D(TX_BAND_SEL,          UPP,   7, GPIO_DIR_OUT) /* backward compatibility */\
    _GPIO_D(FCS_LNB_MOD,          UPP,   7, GPIO_DIR_OUT) /* backward compatibility */\
\
    _GPIO_D(SWP_SPI2_SS00,        UPP,   6, GPIO_DIR_IN)\
    _GPIO_D(EXT_10MHZ_LOCK,       UPP,   6, GPIO_DIR_IN)\
    _GPIO_D(10MHZ_LOCK,           UPP,   6, GPIO_DIR_IN) /* backward compatibility */\
\
    _GPIO_D(SWP_SPI2_MISO,        UPP,   5,  GPIO_DIR_OUT)\
    _GPIO_D(RCS_UART_TXD,         UPP,   5,  GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN38,             UPP,   5,  GPIO_DIR_OUT) /* backward compatibility */\
\
    _GPIO_D(SWP_SPI2_MOSI,        UPP,   4,  GPIO_DIR_IN)\
    _GPIO_D(RCS_UART_RXD,         UPP,   4,  GPIO_DIR_IN)\
    _GPIO_D(J4_PIN34,             UPP,   4,  GPIO_DIR_OUT) /* backward compatibility */\
\
    _GPIO_D(SWP_SPI2_SCLK,        UPP,   3,  GPIO_DIR_OUT)\
    _GPIO_D(10MHZ_DET,            UPP,   3,  GPIO_DIR_IN)\
\
    _GPIO_D(RCS_DAC_RESET,        UPP,   2,  GPIO_DIR_OUT)\
    _GPIO_D(TX_AMP_EN_P2,         UPP,   2,  GPIO_DIR_OUT)\
\
    _GPIO_D(DAC_CLK_LOCK,         UPP,   1,  GPIO_DIR_IN)\
    _GPIO_D(TX_MOD_LOCK,          UPP,   1,  GPIO_DIR_IN) /* backward compatibility */\
\
    _GPIO_D(TX_REF_LOCK,          UPP,   0,  GPIO_DIR_IN)\
    _GPIO_D(TRF_MOD_LOCK,         UPP,   0,  GPIO_DIR_IN)\
    _GPIO_D(J4_PIN32,             UPP,   0,  GPIO_DIR_OUT) /* backward compatibility */\

#define GPIO_SWP_LIST /* A macro list to produce various entities: data structure, enum constants... */\
    _GPIO_D(SPI_SRDY,             SWP,   15, GPIO_DIR_IN)\
    _GPIO_D(MPU_RST_IN_N,         SWP,   15, GPIO_DIR_IN)\
\
    _GPIO_D(FPGA_PROG,            SWP,   14, GPIO_DIR_OUT)\
    _GPIO_D(SPI_MRDY,             SWP,   14, GPIO_DIR_OUT)\
    _GPIO_D(CORE_3_ON,            SWP,   14, GPIO_DIR_IN)\
\
    _GPIO_D(USB_OVERCURRENT_N,    SWP,   13, GPIO_DIR_IN)\
    _GPIO_D(MODMAN_SLOT,          SWP,   13, GPIO_DIR_IN)\
\
    _GPIO_D(USB_PWR_CTRL_N,       SWP,   12, GPIO_DIR_OUT)\
    _GPIO_D(CORE_2_ON,            SWP,   12, GPIO_DIR_IN)\
\
    _GPIO_D(MPU_IPF_N,            SWP,   11, GPIO_DIR_IN)\
    _GPIO_D(WIFI_LED,             SWP,   11, GPIO_DIR_IN)\
    _GPIO_D(IPF_N,                SWP,   11, GPIO_DIR_IN)    /* backward compatibility */\
\
    _GPIO_D(BOOT_SEL2,            SWP,   10, GPIO_DIR_IN)\
    _GPIO_D(TX_DIV_SW2,           SWP,   10, GPIO_DIR_OUT)\
\
    _GPIO_D(BOOT_SEL1,            SWP,   9,  GPIO_DIR_IN)\
    _GPIO_D(TX_DIV_SW1,           SWP,   9,  GPIO_DIR_OUT)\
\
    _GPIO_D(BOOT_SEL0,            SWP,   8,  GPIO_DIR_IN)\
    _GPIO_D(WI_FI_DISABLE,        SWP,   8,  GPIO_DIR_OUT)\
\
    _GPIO_D(FCS_SPI_SS01,         SWP,   7,  GPIO_DIR_OUT)\
    _GPIO_D(ADC_RESET_N,          SWP,   7,  GPIO_DIR_OUT)\
\
    _GPIO_D(RESCUE_N_INT,         SWP,   6,  GPIO_DIR_IN)\
    _GPIO_D(LAN_LED,              SWP,   5,  GPIO_DIR_OUT)\
    _GPIO_D(TX_LED,               SWP,   4,  GPIO_DIR_OUT)\
    _GPIO_D(RX_LED,               SWP,   3,  GPIO_DIR_OUT)\
    _GPIO_D(SYS_LED,              SWP,   2,  GPIO_DIR_OUT)\
    _GPIO_D(PWR_LED_BLUE,         SWP,   1,  GPIO_DIR_OUT)\
    _GPIO_D(PWR_LED_RED,          SWP,   0,  GPIO_DIR_OUT)\

#else  /* for Callisto */

#define GPIO_DPP_LIST /* A macro list to produce various entities: data structure, enum constants.. */\

#define GPIO_UPP_LIST /* A macro list to produce various entities: data structure, enum constants.. */\
    _GPIO_D(TX_MOD_OFF,    UPP, _GPIO_0,  GPIO_DIR_OUT)\
    _GPIO_D(TX_MOD_LOCK,   UPP, _GPIO_1,  GPIO_DIR_IN)\
    _GPIO_D(RCS_DAC_RESET, UPP, _GPIO_2,  GPIO_DIR_OUT)\
    _GPIO_D(FCS_LNB_MOD,   UPP, _GPIO_7,  GPIO_DIR_OUT)\
    _GPIO_D(TX_DIV_MC,     UPP, _GPIO_13, GPIO_DIR_OUT)\
    _GPIO_D(RCS_CNTRL_SEL, UPP, _GPIO_14, GPIO_DIR_OUT)\
    _GPIO_D(TX_DIV_ON,     UPP, _GPIO_15, GPIO_DIR_OUT)\
    _GPIO_D(TX_ON,         UPP, _GPIO_1,  GPIO_DIR_OUT)  /* zfan - not really defined; just for forward compatibility */\
\
    _GPIO_D(10MHZ_LOCK,    UPP, _GPIO_1,  GPIO_DIR_IN)\
    _GPIO_D(10MHZ_SW,      UPP, _GPIO_6,  GPIO_DIR_OUT)\
\
    _GPIO_D(TX_MUTE_N,     UPP, _GPIO_0,  GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN27,      UPP, _GPIO_3,  GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN29,      UPP, _GPIO_4,  GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN33,      UPP, _GPIO_5,  GPIO_DIR_OUT)\
    _GPIO_D(TX_BAND_SEL,   UPP, _GPIO_7,  GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN35,      UPP, _GPIO_9,  GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN39,      UPP, _GPIO_13, GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN41,      UPP, _GPIO_14, GPIO_DIR_OUT)

#define GPIO_SWP_LIST /* A macro list to produce various entities: data structure, enum constants.. */\
    _GPIO_D(TX_DIV_SW2,    SWP, _GPIO_8,  GPIO_DIR_OUT)\
    _GPIO_D(TX_DIV_SW1,    SWP, _GPIO_9,  GPIO_DIR_OUT)\
    _GPIO_D(10MHZ_DET,     SWP, _GPIO_11, GPIO_DIR_IN)\
    _GPIO_D(TX_AMP_EN,     SWP, _GPIO_12, GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN23,      SWP, _GPIO_13, GPIO_DIR_OUT)

#define GPIO_LIST /* A macro list to produce various entities: data structure, enum constants... */\
    /* KALE */\
    _GPIO_D(TX_MOD_OFF,    UPP, _GPIO_0,  GPIO_DIR_OUT)\
    _GPIO_D(TX_MOD_LOCK,   UPP, _GPIO_1,  GPIO_DIR_IN)\
    _GPIO_D(RCS_DAC_RESET, UPP, _GPIO_2,  GPIO_DIR_OUT)\
    _GPIO_D(FCS_LNB_MOD,   UPP, _GPIO_7,  GPIO_DIR_OUT)\
    _GPIO_D(TX_DIV_MC,     UPP, _GPIO_13, GPIO_DIR_OUT)\
    _GPIO_D(RCS_CNTRL_SEL, UPP, _GPIO_14, GPIO_DIR_OUT)\
    _GPIO_D(TX_DIV_ON,     UPP, _GPIO_15, GPIO_DIR_OUT)\
    _GPIO_D(TX_DIV_SW1,    SWP, _GPIO_9,  GPIO_DIR_OUT)\
    _GPIO_D(TX_DIV_SW2,    SWP, _GPIO_8,  GPIO_DIR_OUT)\
    \
    /* 10MHz - Rack Mount and AERO IDU */\
    _GPIO_D(10MHZ_LOCK,    UPP, _GPIO_1,  GPIO_DIR_IN)\
    _GPIO_D(10MHZ_SW,      UPP, _GPIO_6,  GPIO_DIR_OUT)\
    _GPIO_D(10MHZ_DET,     SWP, _GPIO_11, GPIO_DIR_IN)\
    \
    /* AERO IDU */\
    _GPIO_D(TX_MUTE_N,     UPP, _GPIO_0,  GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN27,      UPP, _GPIO_3,  GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN29,      UPP, _GPIO_4,  GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN33,      UPP, _GPIO_5,  GPIO_DIR_OUT)\
    _GPIO_D(TX_BAND_SEL,   UPP, _GPIO_7,  GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN35,      UPP, _GPIO_9,  GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN39,      UPP, _GPIO_13, GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN41,      UPP, _GPIO_14, GPIO_DIR_OUT)\
    _GPIO_D(TX_AMP_EN,     SWP, _GPIO_12, GPIO_DIR_OUT)\
    _GPIO_D(J4_PIN23,      SWP, _GPIO_13, GPIO_DIR_OUT)\

#endif

/* add new definition in the list following the convention; everything esle is taken care of "automatically" */

enum{ /* for internal use : pin # */
    _GPIO_0 = 0,
    _GPIO_1,
    _GPIO_2,
    _GPIO_3,
    _GPIO_4,
    _GPIO_5,
    _GPIO_6,
    _GPIO_7,
    _GPIO_8,
    _GPIO_9,
    _GPIO_10,
    _GPIO_11,
    _GPIO_12,
    _GPIO_13,
    _GPIO_14,
    _GPIO_15,
    NUM_GPIO,
};

/* used as input parms for individual GPIO access functions: GPIO direction, 0 (In), 1 (Out) 2 (Not Applicable) */
enum {
    GPIO_DIR_IN     = 0,
    GPIO_DIR_OUT    = 1,
    GPIO_DIR_N_A    = 2,
};

/* used as input parms for individual GPIO access functions: GPIO out values: 0 or 1 */
enum {
    GPIO_OUT_VAL_0 = 0,
    GPIO_OUT_VAL_1 = 1,
};

/* e.g., SWP_GPIO_RESCUE_N_INT_PIN6 = 6,  SWP_GPIO_RESCUE_N_INT = 6, for SWP GPIO pin6 */
enum {
    #define _GPIO_D(gpio_name, gpio_base, gpio_bit, direction) \
        gpio_base##_GPIO_##gpio_name = gpio_bit, gpio_base##_GPIO_##gpio_name##_PIN##gpio_bit = gpio_bit,
    GPIO_FULL_LIST
    #undef _GPIO_D
    NUM_GPIO_DEDINES,
};

/* to make compiler happy */
#define GPIO_NOT_USED_Read()          (0)
#define GPIO_NOT_USED_Wrtie(a)        ()
#define GPIO_NOT_USED_Set_Dir()       ()

enum { /* e.g.,     SWP_GPIO_TX_DIV_SW2_MASK = (1<<9)  SWP_GPIO_TX_DIV_SW2_PIN9_MASK = (1<<9) */
    #define _GPIO_D(gpio_name, gpio_base, gpio_bit, direction) \
        gpio_base##_GPIO_##gpio_name##_MASK = (1<<gpio_bit), gpio_base##_GPIO_##gpio_name##_PIN##gpio_bit##_MASK = (1<<gpio_bit),
    GPIO_FULL_LIST
    #undef _GPIO_D
};

#define _GPIO_BASE_D(gpio_base) static gpio_base##_GPIO_REGS * gpio_base##_gpioBasePtr = NULL;
GPIO_BASE_LIST
/*
 i.e.,
static UPP_GPIO_REGS * UPP_gpioBasePtr = NULL;
static UPP_GPIO_REGS * DPP_gpioBasePtr = NULL;
static SWP_GPIO_REGS * SWP_gpioBasePtr = NULL;
*/
#undef _GPIO_BASE_D

/* Read GPIO state
   user interface: e.g., GPIO_TX_DIV_MC_Read(void) */
#define _GPIO_D(gpio_name, gpio_base, gpio_bit, direction) \
    static inline int GPIO_##gpio_name##_Read(void) \
    {\
        int result = OK;\
        if(gpio_base##_gpioBasePtr == NULL) \
            gpio_base##_gpioBasePtr = (gpio_base##_GPIO_REGS *)MAP_REG_BLK(gpio_base##_GPIO);\
        if(gpio_base##_gpioBasePtr) \
        {\
            result = (((gpio_base##_gpioBasePtr->gpio_base##_GPIO_DIN_RO) & (1<<gpio_bit)) != 0);\
        }\
        return result;\
    }
GPIO_FULL_LIST
#undef _GPIO_D

/* Read GPIO state (with BASE name, preferred usage):
    GPIO_UPP_TX_DIV_MC_Read(void) is prefered to GPIO_TX_DIV_MC_Read(void)
   user interface: e.g., GPIO_UPP_TX_DIV_MC_Read(void) */
#define _GPIO_D(gpio_name, gpio_base, gpio_bit, direction) \
    static inline int GPIO_##gpio_base##_##gpio_name##_Read(void) \
    {\
        int result = OK;\
        if(gpio_base##_gpioBasePtr == NULL) \
            gpio_base##_gpioBasePtr = (gpio_base##_GPIO_REGS *)MAP_REG_BLK(gpio_base##_GPIO);\
        if(gpio_base##_gpioBasePtr) \
        {\
            result = (((gpio_base##_gpioBasePtr->gpio_base##_GPIO_DIN_RO) & (1<<gpio_bit)) != 0);\
        }\
        return result;\
    }
GPIO_FULL_LIST
#undef _GPIO_D

/* Write On/Off value to a GPIO
   user interface: e.g., GPIO_TX_DIV_MC_Write(GPIO_OUT_VAL_1 or GPIO_OUT_VAL_0) */
#define _GPIO_D(gpio_name, gpio_base, gpio_bit, direction) \
    static inline void GPIO_##gpio_name##_Write(int on_off) \
    {\
        if(gpio_base##_gpioBasePtr == NULL) \
            gpio_base##_gpioBasePtr = (gpio_base##_GPIO_REGS *)MAP_REG_BLK(gpio_base##_GPIO);\
        if(gpio_base##_gpioBasePtr) \
        {\
            gpio_base##_gpioBasePtr->gpio_base##_GPIO_DIR_CNTL_RWE = (1<<(gpio_bit+16)) | (1<<gpio_bit); /* output mode for the bit */\
            on_off = (on_off != 0); /* so that either 0 (OFF) or 1 (ON)*/ \
            gpio_base##_gpioBasePtr->gpio_base##_GPIO_DOUT_RWE = (1<<(gpio_bit+16)) | (on_off<<gpio_bit); /* set on/off for the bit */\
        }\
    }
GPIO_FULL_LIST
#undef _GPIO_D

/* Write On/Off value to a GPIO (with BASE name, preferred usage):
        GPIO_UPP_TX_DIV_MC_Write(...) is prefered to GPIO_TX_DIV_MC_Write(...)
   user interface: e.g., GPIO_UPP_TX_DIV_MC_Write(GPIO_OUT_VAL_1 or GPIO_OUT_VAL_0) */
#define _GPIO_D(gpio_name, gpio_base, gpio_bit, direction) \
static inline void GPIO_##gpio_base##_##gpio_name##_Write(int on_off) \
{\
    if(gpio_base##_gpioBasePtr == NULL) \
        gpio_base##_gpioBasePtr = (gpio_base##_GPIO_REGS *)MAP_REG_BLK(gpio_base##_GPIO);\
    if(gpio_base##_gpioBasePtr) \
    {\
        gpio_base##_gpioBasePtr->gpio_base##_GPIO_DIR_CNTL_RWE = (1<<(gpio_bit+16)) | (1<<gpio_bit); /* output mode for the bit */\
        on_off = (on_off != 0); /* so that either 0 (OFF) or 1 (ON)*/ \
        gpio_base##_gpioBasePtr->gpio_base##_GPIO_DOUT_RWE = (1<<(gpio_bit+16)) | (on_off<<gpio_bit); /* set on/off for the bit */\
    }\
}
GPIO_FULL_LIST
#undef _GPIO_D

/* Set default direction for a GPIO
   user interface: e.g., GPIO_TX_DIV_MC_Set_Dir() */
#define _GPIO_D(gpio_name, gpio_base, gpio_bit, direction) \
    static inline int GPIO_##gpio_name##_Set_Dir(void) \
    {\
        int result = OK;\
        if(gpio_base##_gpioBasePtr == NULL) \
            gpio_base##_gpioBasePtr = (gpio_base##_GPIO_REGS *)MAP_REG_BLK(gpio_base##_GPIO);\
        if(gpio_base##_gpioBasePtr) \
        {\
            gpio_base##_gpioBasePtr->gpio_base##_GPIO_DIR_CNTL_RWE = (1<<(gpio_bit+16)) | (direction<<gpio_bit);\
        }\
        return result;\
    }
GPIO_FULL_LIST
#undef _GPIO_D

/* Set default direction for a GPIO (with BASE name, preferred usage):
       GPIO_UPP_TX_DIV_MC_Set_Dir(...) is preferred to GPIO_TX_DIV_MC_Set_Dir(...)
   user interface: e.g.,  GPIO_UPP_TX_DIV_MC_Set_Dir() */
#define _GPIO_D(gpio_name, gpio_base, gpio_bit, direction) \
    static inline int GPIO_##gpio_base##_##gpio_name##_Set_Dir(void) \
    {\
        int result = OK;\
        if(gpio_base##_gpioBasePtr == NULL) \
            gpio_base##_gpioBasePtr = (gpio_base##_GPIO_REGS *)MAP_REG_BLK(gpio_base##_GPIO);\
        if(gpio_base##_gpioBasePtr) \
        {\
            gpio_base##_gpioBasePtr->gpio_base##_GPIO_DIR_CNTL_RWE = (1<<(gpio_bit+16)) | (direction<<gpio_bit);\
        }\
        return result;\
    }
GPIO_FULL_LIST
#undef _GPIO_D

/* Set desired direction for a GPIO (with BASE name, preferred usage):  GPIO_UPP_TX_DIV_MC_Direction_set(GPIO_DIR_IN)
                    is preferred to   GPIO_TX_DIV_MC_Direction_set(GPIO_DIR_IN)
user interface: e.g.,   GPIO_UPP_TX_DIV_MC_Direction_set(GPIO_DIR_IN) */
#define _GPIO_D(gpio_name, gpio_base, gpio_bit, direction) \
    static inline int GPIO_##gpio_base##_##gpio_name##_Direction_set(int in_out)\
    {\
        int result = OK;\
        if(gpio_base##_gpioBasePtr == NULL) \
            gpio_base##_gpioBasePtr = (gpio_base##_GPIO_REGS *)MAP_REG_BLK(gpio_base##_GPIO);\
        if(gpio_base##_gpioBasePtr) \
        {\
            if(in_out)  in_out = 1; \
            gpio_base##_gpioBasePtr->gpio_base##_GPIO_DIR_CNTL_RWE = (1<<(gpio_bit+16)) | (in_out << gpio_bit);\
        }\
        return result;\
    }
GPIO_FULL_LIST
#undef _GPIO_D

/* Set desired direction for a GPIO
   user interface: e.g., int GPIO_TX_DIV_MC_Direction_set(GPIO_DIR_IN) */
#define _GPIO_D(gpio_name, gpio_base, gpio_bit, direction) \
    static inline int GPIO_##gpio_name##_Direction_set(int in_out)\
    {\
        int result = OK;\
        if(gpio_base##_gpioBasePtr == NULL) \
            gpio_base##_gpioBasePtr = (gpio_base##_GPIO_REGS *)MAP_REG_BLK(gpio_base##_GPIO);\
        if(gpio_base##_gpioBasePtr) \
        {\
            if(in_out)  in_out = 1; \
            gpio_base##_gpioBasePtr->gpio_base##_GPIO_DIR_CNTL_RWE = (1<<(gpio_bit+16)) | (in_out << gpio_bit);\
        }\
        return result;\
    }
GPIO_FULL_LIST
#undef _GPIO_D

/* Below are used for accessing Multiple GPIO pins in one operation.
unsigned int SWP_GPIO_Read_UsingBitmap(unsigned int bitmask);
unsigned int UPP_GPIO_Read_UsingBitmap(unsigned int bitmask);
unsigned int DPP_GPIO_Read_UsingBitmap(unsigned int bitmask);

E.g.,
 bitmask = (SWP_GPIO_TX_DIV_SW2_MASK | SWP_GPIO_TX_DIV_SW1_MASK);
 SWP_GPIO_Read_UsingBitmap(bitmask)

  It reads GPIO SWP_GPIO_TX_DIV_SW2 and SWP_GPIO_TX_DIV_SW1 states, and returns
 ( state_of_SWP_GPIO_TX_DIV_SW2  << SWP_GPIO_TX_DIV_SW2)|(  state_of_SWP_GPIO_TX_DIV_SW1 << SWP_GPIO_TX_DIV_SW1)

 Note, all these pins have to be for the same base GPIO, and in this case it is SWP GPIO */

#define _GPIO_BASE_D(gpio_base) \
static inline int gpio_base##_GPIO_Read_UsingBitmap(unsigned int bitmask) \
{\
    int result = OK;\
    if(gpio_base##_gpioBasePtr == NULL) \
        gpio_base##_gpioBasePtr = (gpio_base##_GPIO_REGS *)MAP_REG_BLK(gpio_base##_GPIO);\
    \
    if(gpio_base##_gpioBasePtr) \
    {\
        result = (gpio_base##_gpioBasePtr->gpio_base##_GPIO_DIN_RO) & bitmask;\
    }\
    return result;\
}
GPIO_BASE_LIST
#undef _GPIO_BASE_D

/* user interface:
void  SWP_GPIO_Write_UsingBitmap(unsigned int enable_bitmask, unsigned int value);
void  UPP_GPIO_Write_UsingBitmap(unsigned int enable_bitmask, unsigned int value);
void  DPP_GPIO_Write_UsingBitmap(unsigned int enable_bitmask, unsigned int value);

      enable_bitmask =  (UPP_GPIO_TX_DIV_MC_MASK) |  UPP_GPIO_FCS_LNB_27MHZ_MASK);
      dir_value      =  ((1 << UPP_GPIO_TX_DIV_MC) | (0 << UPP_GPIO_FCS_LNB_27MHZ));
      UPP_GPIO_Write_UsingBitmap(enable_bitmask, dir_value);

 The example sets UPP_GPIO_TX_DIV_MC state as 1 and UPP_GPIO_FCS_LNB_27MHZ state as 0
 Note, all these pins have to be for the same base GPIO, and in this case it is UPP GPIO */

#define _GPIO_BASE_D(gpio_base) \
static inline void gpio_base##_GPIO_Write_UsingBitmap(unsigned int enable_bitmask, unsigned int state_value) \
{\
    if(gpio_base##_gpioBasePtr == NULL) \
        gpio_base##_gpioBasePtr = (gpio_base##_GPIO_REGS *)MAP_REG_BLK(gpio_base##_GPIO);\
    \
    if(gpio_base##_gpioBasePtr) /* note: only output mode GPIO-pins are valid for write */\
    {\
        gpio_base##_gpioBasePtr->gpio_base##_GPIO_DIR_CNTL_RWE = (enable_bitmask << 16) | enable_bitmask;       /* Set output mode for the bits */\
        gpio_base##_gpioBasePtr->gpio_base##_GPIO_DOUT_RWE = (enable_bitmask << 16) | (state_value & enable_bitmask); /* set on/off for the bits */\
    }\
}
GPIO_BASE_LIST
#undef _GPIO_BASE_D

/* user interface:
int SWP_GPIO_Set_Dir_UsingBitmap(unsigned int enable_bitmask, unsigned int dir_value)
int UPP_GPIO_Set_Dir_UsingBitmap(unsigned int enable_bitmask, unsigned int dir_value)
int DPP_GPIO_Set_Dir_UsingBitmap(unsigned int enable_bitmask, unsigned int dir_value)

e.g.,
      enable_bitmask =  (UPP_GPIO_TX_DIV_MC_MASK) | UPP_GPIO_10MHZ_DET_MASK) | UPP_GPIO_FCS_LNB_27MHZ_MASK);
      dir_value      =  ((GPIO_DIR_OUT << UPP_GPIO_TX_DIV_MC) | (GPIO_DIR_IN <<UPP_GPIO_10MHZ_DET) | (GPIO_DIR_OUT << UPP_GPIO_10MHZ_DET));
      UPP_GPIO_Set_Dir_UsingBitmap(enable_bitmask, dir_value);

The example sets  TX_DIV_MC and FCS_LNB_27MHZ as out, and 10MHZ_DET as in
Note, all these pins have to be for the same base GPIO, and in this case it is UPP GPIO */

#define _GPIO_BASE_D(gpio_base) \
static inline int gpio_base##_GPIO_Set_Dir_UsingBitmap(unsigned int enable_bitmask, unsigned int dir_value) \
{\
    int result = OK;\
    if(gpio_base##_gpioBasePtr == NULL) \
        gpio_base##_gpioBasePtr = (gpio_base##_GPIO_REGS *)MAP_REG_BLK(gpio_base##_GPIO);\
    \
    if(gpio_base##_gpioBasePtr) \
    {\
       gpio_base##_gpioBasePtr->gpio_base##_GPIO_DIR_CNTL_RWE = (enable_bitmask << 16) | (enable_bitmask & dir_value);\
    }\
    return result;\
}
GPIO_BASE_LIST
#undef _GPIO_BASE_D

#endif
