/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rscan_rz_private.h
* Description  : Internal structures and definitions for the CAN driver.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           22.03.2015 1.00    Initial Release
***********************************************************************************************************************/

#ifndef CAN_PRIVATE_HEADER_FILE
#define CAN_PRIVATE_HEADER_FILE

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "iodefine.h"
#include "r_rscan_rz_config.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#if CAN_CFG_NUM_RULES_CH0 > 128
    #error "Error! Number of receive rules for channel 0 exceeds 128"
#endif
#if CAN_CFG_NUM_RULES_CH1 > 128
    #error "Error! Number of receive rules for channel 1 exceeds 128"
#endif
#if CAN_CFG_NUM_RULES_CH2 > 128
    #error "Error! Number of receive rules for channel 2 exceeds 128"
#endif
#if CAN_CFG_NUM_RULES_CH3 > 128
    #error "Error! Number of receive rules for channel 3 exceeds 128"
#endif
#if CAN_CFG_NUM_RULES_CH4 > 128
    #error "Error! Number of receive rules for channel 4 exceeds 128"
#endif

#if ((CAN_CFG_NUM_RULES_CH0 + CAN_CFG_NUM_RULES_CH1 + CAN_CFG_NUM_RULES_CH2 + CAN_CFG_NUM_RULES_CH3 + CAN_CFG_NUM_RULES_CH4) > 320)
    #error "Error! Total number of receive rules exceeds 320"
#endif


/*****************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
 * DRIVER CONTROL STRUCTURES
 ******************************************************************************/

typedef struct st_can_dcb       // driver control block
{
    bool_t      opened;
    bool_t      gw_init;        // true when a GWFIFO has been configured
    uint16_t    rule_count;     // number of total rules applied to module
    uint32_t    fifo_mask;      // mask of configured FIFOs
    void        (*p_callback)(can_cb_evt_t event, void *p_args);
} can_dcb_t;

typedef struct st_can_ccb       // channel control block
{
    bool_t      initialized;
    void        (*p_chcallback)(uint8_t       chan,
                                can_cb_evt_t  event,
                                void          *p_args);
} can_ccb_t;


/******************************************************************************
 * GLOBAL OPERATION DEFINITIONS
 ******************************************************************************/
/*
 * GCFG: Global Configuration Register
 */
typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    TPRI:1;     // tx buffer priority (0=ID, 1=buffer number)
        uint32_t    DCE:1;      // DLC check enable
        uint32_t    DRE:1;      // DLC replacement enable
        uint32_t    MME:1;      // mirror function enable
        uint32_t    DCS:1;      // CAN clock source (0=clkc, 1=clk_xincan)
        uint32_t    :3;         //  clkc=pclk/2 (RSK); clk_xincan connected to CAN_CLK
        uint32_t    TSP:4;      // timestamp clock source divisor (2**n; enter n)
        uint32_t    TSSS:1;     // timestamp source select (0=pclk/2; 1=bit-time clock)
        uint32_t    TSBTCS:3;   // timestamp clock source select (0-4 = ch0-ch4 bit-time clock)
        uint32_t    ITRCP:16;   // interval timer prescaler
    } BIT;
} can_gcfg_t;

/*
 * GTSC: Global Timestamp Counter Register      bits 0-15
 */

/*
 * GCTR: Global Control Register
 */
typedef enum e_can_global_mode
{
    CAN_GLOBAL_MODE_OPERATING = 0,
    CAN_GLOBAL_MODE_RESET     = 1,
    CAN_GLOBAL_MODE_TEST      = 2,
    CAN_GLOBAL_MODE_END_ENUM
} can_global_mode_t;

typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    GMDC:2;     // global mode select (global_mode_t)
        uint32_t    GSLPR:1;    // global stop mode
        uint32_t    :5;
        uint32_t    DEIE:1;     // DLC error interrupt enable
        uint32_t    MEIE:1;     // FIFO message lost interrupt enable
        uint32_t    THLEIE:1;   // Transmit History overflow interrupt enable
        uint32_t    :5;
        uint32_t    TSRST:1;    // reset timestamp counter
        uint32_t    :15;
    } BIT;
} can_gctr_t;

/*
 * GSTS: Global Status Register
 */
typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    GRSTSTS:1;  // in global reset mode
        uint32_t    GHLTSTS:1;  // in global test mode
        uint32_t    GSLPSTS:1;  // in global stop mode
        uint32_t    GRAMINIT:1; // CAN RAM init is ongoing
        uint32_t    :28;
    } BIT;
} can_gsts_t;

/*
 * GERFL: Global Error Flag Register
 */
typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    DEF:1;      // DLC error occurred
        uint32_t    MES:1;      // FIFO message lost occurred
        uint32_t    THLES:1;    // Transmit History overflow occurred
        uint32_t    :28;
    } BIT;
} can_gerfl_t;

/*
 * GTINTSTS0/1: Global Transmit Interrupt Status
 */
typedef union                   //
{
    uint8_t         byte;
    struct
    {
        uint8_t     tsif:1;     // transmit completed
        uint8_t     taif:1;     // transmit aborted
        uint8_t     tqif:1;     // transmit queue interrupt
        uint8_t     cftif:1;    // TX/RX FIFO interrupt
        uint8_t     thif:1;     // transmit history interrupt
        uint8_t     :3;
    } bit;
} can_gtintsts_ch_t;

typedef union
{
    uint32_t            sts[2];
    can_gtintsts_ch_t   ch_sts[8];  // only 0-4 valid
} txint_sts_t;


/******************************************************************************
 * OTHER STATUS and INTERRUPT ENABLE
 ******************************************************************************/
/*
 * RFISTS:  RX FIFO interrupt flags               bits 0-7
 * CFRISTS: TX/RX FIFO receive interrupt flags    bits 0-14
 * CFTISTS: TX/RX FIFO transmit interrupt flags   bits 0-14
 *
 * Use "y" access routines to check/set/clear bits 0-79
 * array_ptr = &RSCAN0TMIEC0;       // transmit buffer (non-FIFO) interrupt enable
 * array_ptr = &RSCAN0TMTRSTS0;     // transmit request
 * array_ptr = &RSCAN0TMTARSTS0;    // transmit abort request
 * array_ptr = &RSCAN0TMTCSTS0;     // transmit completed
 * array_ptr = &RSCAN0TMTASTS0;     // transmit aborted
 * array_ptr = &RSCAN0RMND0;        // new message in receive buffer
 */

typedef union
{
    uint32_t    y_array[3];
    uint16_t    chan[6];            // index is mailbox number / 4
} can_yreg_t;


/******************************************************************************
 * CHANNEL OPERATION DEFINITIONS
 ******************************************************************************/
/*
 * CmCFG: Channel configuration register (m=0-4)
 */
typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    BRP:10;     // fCAN prescaler (0-1023 = divide by 1-1024)
        uint32_t    :6;
        uint32_t    TSEG1:4;    // PROP_SEG + PHASE_SEG1 time (3-15 = 4-16 Tq)
        uint32_t    TSEG2:3;    // PHASE_SEG2 time (1-7 = 2-8 Tq)
        uint32_t    :1;
        uint32_t    SJW:2;      // resynchronization jump width (0-3 = 1-4 Tq)
        uint32_t    :6;
    } BIT;
} can_cmcfg_t;

/*
 * CmCTR: Channel control register (m=0-4)
 */
typedef enum e_can_chmode
{
    CAN_CHMODE_COMM = 0,
    CAN_CHMODE_RESET = 1,
    CAN_CHMODE_HALT = 2,
    CAN_CHMODE_END_ENUM
} can_chmode_t;

typedef enum e_bo_rmode
{
    CAN_BO_RECOVERY_ISO = 0,
    CAN_BO_RECOVERY_CH_HALT_AT_BO_ENTRY = 1,
    CAN_BO_RECOVERY_CH_HALT_AT_BO_END = 2,
    CAN_BO_RECOVERY_CH_HALT_DURING_BO = 3,
    CAN_BO_RECOVERY_END_ENUM
} can_bo_rmode_t;

typedef enum e_test_mode
{
    CAN_TEST_MODE_STD = 0,
    CAN_TEST_MODE_LISTEN = 1,
    CAN_TEST_MODE_EXT_LOOPBACK = 2,
    CAN_TEST_MODE_INT_LOOPBACK = 3,
    CAN_TEST_MODE_END_ENUM
} can_test_mode_t;

typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    CHMDC:2;    // Channel mode (chan_mode_t)
        uint32_t    CSLPR:1;    // Channel stop mode
        uint32_t    RTBO:1;     // Forcible return from Bus Off
        uint32_t    :4;
        uint32_t    BEIE:1;     // Bus Error interrupt enable
        uint32_t    EWIE:1;     // Error Warning interrupt enable
        uint32_t    EPIE:1;     // Error Passive interrupt enable
        uint32_t    BOEIE:1;    // Bus Off Entry interrupt enable
        uint32_t    BORIE:1;    // Bus Off Recovery interrupt enable
        uint32_t    OLIE:1;     // Overload Frame Transmit interrupt enable
        uint32_t    BLIE:1;     // Bus Lock interrupt enable
        uint32_t    ALIE:1;     // Arbitration Lost interrupt enable
        uint32_t    TAIE:1;     // Transmit Abort interrupt enable
        uint32_t    :4;
        uint32_t    BOM:2;      // Bus Off recovery mode (bom_rmode_t)
        uint32_t    ERRD:1;     // set error flags (0=all bits of first error only)
        uint32_t    CTME:1;     // Communication test mode enable
        uint32_t    CTMS:2;     // Communication test mode (test_mode_t)
        uint32_t    :5;
    } BIT;
} can_cmctr_t;

/*
 * CmSTS: Channel status register (m=0-4)
 */
typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    CRSTSTS:1;  // in channel reset mode
        uint32_t    CHLTSTS:1;  // in channel halt mode
        uint32_t    CSLPSTS:1;  // in channel stop mode
        uint32_t    EPSTS:1;    // in error passive state
        uint32_t    BOSTS:1;    // in bus off state
        uint32_t    TRMSTS:1;   // in transmission or bus off state
        uint32_t    RECSTS:1;   // in reception
        uint32_t    COMSTS:1;   // communication is ready
        uint32_t    :8;
        uint32_t    REC:8;      // rx error counter
        uint32_t    TEC:8;      // tx error counter
    } BIT;
} can_cmsts_t;

/*
 * CmERFL: Channel error flag register (m=0-4)
 */
typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    BEF:1;      // protocol error flag
        uint32_t    EWF:1;      // error warning flag
        uint32_t    EPF:1;      // error passive flag
        uint32_t    BOEF:1;     // bus off entry flag
        uint32_t    BORF:1;     // bus off recovery flag
        uint32_t    OVLF:1;     // overload flag
        uint32_t    BLF:1;      // dominant lock flag
        uint32_t    ALF:1;      // arbitration lost flag
        uint32_t    SERR:1;     // stuff error flag
        uint32_t    FERR:1;     // form error flag
        uint32_t    AERR:1;     // ack error flag
        uint32_t    CERR:1;     // crc error flag
        uint32_t    B1ERR:1;    // recessive bit error flag
        uint32_t    B0ERR:1;    // dominant bit error flag
        uint32_t    ADERR:1;    // ack delimiter error flag
        uint32_t    :1;
        uint32_t    CFCREG:15;  // crc calculation data
        uint32_t    :1;
    } BIT;
} can_cmerfl_t;

/*
 * array_ptr = (can_channel_t *)&RSCAN0C0CFG       array[5]
 */
typedef struct st_chan
{
    can_cmcfg_t     cmcfg;
    can_cmctr_t     cmctr;
    can_cmsts_t     cmsts;
    can_cmerfl_t    cmerfl;
} can_chan_t;


/******************************************************************************
 * MESSAGE STRUCTURE
 ******************************************************************************/

/* RMIDq: Receive buffer ID register (q=0-79)
 * RFIDx: RX FIFO access ID register (x=0-7)
 * CFIDk: TX/RX FIFO access ID register (k=0-14, 0-3 per channel)
 * TMIDp: TX buffer ID register (p=0-79, 16 per channel)
 */
typedef union                   //
{
    uint32_t        uint32;
    struct
    {
        uint32_t    id:29;      // ID
        uint32_t    thlen:1;    // TX ONLY; store in transmit history
        uint32_t    rtr:1;      // RTR
        uint32_t    ide:1;      // IDE
    } bit;
} can_msg_id_t;

/* RMPTRq: Receive buffer pointer register (q=0-79)
 * RFPTRx: RX FIFO access pointer register (x=0-7)
 * CFPTRk: TX/RX FIFO access pointer register (k=0-14, 3 per channel)
 * TMPTRp: TX buffer pointer register (p=0-79, 16 per channel)
 */
typedef union
{
    uint32_t        uint32;
    struct
    {
        uint32_t    ts:16;      // timestamp; on receive messages
        uint32_t    ptr:12;     // label; only 8 bits for transmit
        uint32_t    dlc:4;      // DLC
    } bit;
} can_msg_ptr_t;

/* RMDF0q: Receive buffer data field 0 register (q=0-79)
 * RFDF0x: RX FIFO access data field 0 register (x=0-7)
 * CFDF0k: TX/RX FIFO access data field 0 register (k=0-14, 3 per channel)
 * TMDF0p: TX buffer data field 0 register (p=0-79, 16 per channel)
 */
typedef union
{
    uint32_t        uint32;
    struct
    {
        uint32_t    db0:8;      // data byte 0
        uint32_t    db1:8;      // data byte 1
        uint32_t    db2:8;      // data byte 2
        uint32_t    db3:8;      // data byte 3
    } bit;
} can_msg_df0_t;

/* RMDF1q: Receive buffer data field 1 register (q=0-79)
 * RFDF1x: RX FIFO access data field 1 register (x=0-7)
 * CFDF1k: TX/RX FIFO access data field 1 register (k=0-14, 3 per channel)
 * TMDF1p: TX buffer data field 1 register (p=0-79, 16 per channel)
 */
typedef union
{
    uint32_t        uint32;
    struct
    {
        uint32_t    db4:8;      // data byte 4
        uint32_t    db5:8;      // data byte 5
        uint32_t    db6:8;      // data byte 6
        uint32_t    db7:8;      // data byte 7
    } bit;
} can_msg_df1_t;

/*  arrays starting at:
 *  p_msg = (can_msg_t *)&RSCAN0RMID0;      arr[80]
 *  p_msg = (can_msg_t *)&RSCAN0RFID0;      arr[8]
 *  p_msg = (can_msg_t *)&RSCAN0CFID0;      arr[3] * 5
 *  p_msg = (can_msg_t *)&RSCAN0TMID0;      arr[80]
 */
typedef struct st_msg
{
    can_msg_id_t    id;
    can_msg_ptr_t   ptr;
    can_msg_df0_t   df0;
    can_msg_df1_t   df1;
} can_msg_t;



/******************************************************************************
 * TRANSMIT BUFFER DEFINITIONS
 ******************************************************************************/
/*
 * TMCp: Transmit Buffer control register (p=0-79; 16*5)
 *   array[80] at &RSCAN0TMC0
 */
typedef union
{
    uint8_t         UINT8;
    struct
    {
        uint8_t     TMTR:1;     // transmit requested
        uint8_t     TMTAR:1;    // transmit abort request
        uint8_t     TMOM:1;     // one-shot transmission (vs retry)
        uint8_t     :5;
    } BIT;
} can_tmcp_t;

#define CAN_TMCP_REQ_PRESENT   (0x03)

/*
 * TMSTSp: Transmit Buffer status register (p=0-79; 16*5)
 *   array[80] at &RSCAN0TMSTS0
 */
typedef union
{
    uint8_t         UINT8;
    struct
    {
        uint8_t     TMSTS:1;    // transmit in progress
        uint8_t     TMTRF:2;    // transmit result (txbuf_res_t)
        uint8_t     TMTRM:1;    // transmit request is present
        uint8_t     TMTARM:1;   // transmit abort request is present
        uint8_t     :3;
    } BIT;
} can_tmstsp_t;


/******************************************************************************
 * COMMON FIFO DEFINTIONS
 ******************************************************************************/

typedef enum e_fifo_depth
{
    CAN_FIFO_DEPTH_0   = 0,
    CAN_FIFO_DEPTH_4   = 1,
    CAN_FIFO_DEPTH_8   = 2,
    CAN_FIFO_DEPTH_16  = 3,
    CAN_FIFO_DEPTH_32  = 4,
    CAN_FIFO_DEPTH_48  = 5,
    CAN_FIFO_DEPTH_64  = 6,
    CAN_FIFO_DEPTH_128 = 7,
    CAN_FIFO_DEPTH_END_ENUM
} can_fifo_depth_t;

/*
 * FESTS: FIFO empty status
 * FFSTS: FIFO full status
 * FMSTS: FIFO message lost status
 */
typedef union
{
    uint32_t        uint32;
    struct
    {
        /* bits 0-7 = RX FIFOs 0-7 */
        uint32_t    rxfifo_0:1;
        uint32_t    rxfifo_1:1;
        uint32_t    rxfifo_2:1;
        uint32_t    rxfifo_3:1;
        uint32_t    rxfifo_4:1;
        uint32_t    rxfifo_5:1;
        uint32_t    rxfifo_6:1;
        uint32_t    rxfifo_7:1;
        /* bits 8-22 = TX/RX FIFOs 0-14 */
        uint32_t    ch0_txfifo_0:1;
        uint32_t    ch0_txfifo_1:1;
        uint32_t    ch0_gwfifo:1;
        uint32_t    ch1_txfifo_0:1;
        uint32_t    ch1_txfifo_1:1;
        uint32_t    ch1_gwfifo:1;
        uint32_t    ch2_txfifo_0:1;
        uint32_t    ch2_txfifo_1:1;
        uint32_t    ch2_gwfifo:1;
        uint32_t    ch3_txfifo_0:1;
        uint32_t    ch3_txfifo_1:1;
        uint32_t    ch3_gwfifo:1;
        uint32_t    ch4_txfifo_0:1;
        uint32_t    ch4_txfifo_1:1;
        uint32_t    ch4_gwfifo:1;
        /* "unused" bits assigned to history FIFOs */
        uint32_t    ch0_hist:1;
        uint32_t    ch1_hist:1;
        uint32_t    ch2_hist:1;
        uint32_t    ch3_hist:1;
        uint32_t    ch4_hist:1;
        uint32_t    :4;
    } bit;
} can_fifo_mask_t;


/*
 * RFSTSx: RX FIFO status register (x=0-7)
 *   array[8] at &RSCAN0RFSTS0
 * CFSTSk: TX/RX FIFO status register (k=0-14; 3*5)
 *   array[15] at &RSCAN0CFSTS0
 * THLSTSm: Transmit History status register (m=0-4)
 *   array[5] at &RSCAN0THLSTS0
 */
typedef union
{
    uint32_t        uint32;
    struct
    {
        uint32_t    emp:1;      // FIFO empty
        uint32_t    fll:1;      // FIFO full
        uint32_t    mlt:1;      // message lost (TX History = overflow)
        uint32_t    IF:1;       // interrupt flag
        uint32_t    txif:1;     // transmit interrupt flag (TX/RX FIFO only)
        uint32_t    :3;
        uint32_t    mc:8;       // message count in FIFO (5 bits TX History)
        uint32_t    :16;
    } bit;
} can_fifo_stat_t;

#define CAN_FIFO_EMPTY_FLG      (0x01)
#define CAN_FIFO_FULL_FLG       (0x02)
#define CAN_FIFO_OVFL_FLG       (0x04)
#define CAN_FIFO_INT_FLG        (0x08)


/******************************************************************************
 * TX/RX FIFO DEFINITIONS (TX and GW FIFOs)
 ******************************************************************************/
/*
 * CFCCx: TX/RX FIFO configuration and control register (k=0-14; 3*5)
 *   array[15] at &RSCAN0CFCC0
 */
typedef enum e_fifo_mode
{
    CAN_FIFO_MODE_RECEIVE  = 0,
    CAN_FIFO_MODE_TRANSMIT = 1,
    CAN_FIFO_MODE_GATEWAY  = 2,
    CAN_FIFO_MODE_END_ENUM
} can_fifo_mode_t;

typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    CFE:1;      // FIFO enable
        uint32_t    CFRXIE:1;   // FIFO receive interrupt enable
        uint32_t    CFTXIE:1;   // FIFO transmit interrupt enable
        uint32_t    :5;
        uint32_t    CFDC:3;     // FIFO depth (fifo_depth_t)
        uint32_t    :1;
        uint32_t    CFIM:1;     // 1 = interrupt on each message
        uint32_t    CFIGCV:3;   // interrupt depth (fifo_int_t)
        uint32_t    CFM:2;      // FIFO mode (fifo_mode_t)
        uint32_t    CFITSS:1;   // interval timer source is ch bit time clk
                                //    (vs CFITR selection)
        uint32_t    CFITR:1;    // interval timer resolution pclk / GCFG.ITRCP*10
                                //    (vs pclk / GCFG.ITRCP)
        uint32_t    CFTML:4;    // linked transmit buffer number
        uint32_t    CFITT:8;    // transmission interval (0-0xFF)
    } BIT;
} can_cfcck_t;

/*
 * CFPCTRk: TX/RX FIFO pointer control register  (k=0-14; 3*5) write 0xFF
 *   array[15] at &RSCAN0CFPCTR0
 */


/******************************************************************************
 * TRANSMIT HISTORY BUFFER (FIFO) DEFINITIONS
 ******************************************************************************/
/*
 * THLCCSm: Transmit History configurations and control register (m=0-4)
 * array[NUM_CHANS] at &RSCAN0.THLCC0
 */
typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    THLE:1;     // enable history FIFO
        uint32_t    :7;
        uint32_t    THLIE:1;    // enable interrupt
        uint32_t    THLIM:1;    // interrupt at every entry (vs 12 entries)
        uint32_t    THLDTE:1;   // store tx bufs, txq, & tx/rx FIFOs msgs
                                //  (vs txq & tx/rx FIFOs msgs)
        uint32_t    :21;
    } BIT;
} can_thlccm_t;

/*
 * THLPCTRm: Transmit History pointer control register (m=0-4)
 *   array[NUM_CHANS] at &RSCAN0.THLPCTR0    write 0xFF
 */

/*
 * THLACCm: Transmit History access register (m=0-4)
 *   array[NUM_CHANS] at &RSCAN0.THLACC0
 */

typedef enum e_can_hist_src
{
    HIST_SRC_TX_BUF = 1,
    HIST_SRC_TXRX_FIFO = 2,
    HIST_SRC_TX_QUEUE = 4,
    HIST_SRC_END_ENUM
} can_hist_src_t;

typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    BT:3;       // buffer type (hist_src_t)
        uint32_t    BN:4;       // buffer number
        uint32_t    :1;
        uint32_t    TID:8;      // label
        uint32_t    :16;
    } BIT;
} can_thlaccm_t;                // history data


/******************************************************************************
 * RECEIVE FIFO DEFINITIONS
 ******************************************************************************/

#define NUM_RX_FIFOS    8
/*
 * RFCCx: Receive FIFO configuration and control register (x=0-7)
 *   array[8] at &RSCAN0RFCC0
 */
typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    RFE:1;      // FIFO enable
        uint32_t    RFIE:1;     // FIFO interrupt enable
        uint32_t    :6;
        uint32_t    RFDC:3;     // FIFO depth (fifo_depth_t)
        uint32_t    :1;
        uint32_t    RFIM:1;     // interrupt on each message
        uint32_t    RFIGCV:3;   // interrupt depth (fifo_int_t)
        uint32_t    :16;
    } BIT;
} can_rfccx_t;

/*
 * RFPCTRx: Receive FIFO pointer control register (x=0-7)
 *   array[8] at &RSCAN0RFPCTR0    write 0xFF
 */



/******************************************************************************
 * RECEIVE RULE DEFINITIONS
 ******************************************************************************/
/*
 * GAFLECTR: Receive Rule entry control register
 */
typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    AFLPN:5;    // page number (0-19)
        uint32_t    :3;
        uint32_t    AFLDAE:1;   // write enable
        uint32_t    :23;
    } BIT;
} can_gaflectr_t;

/*
 * GAFLCFG0/1: Receive Rule configuration registers 0/1
 * Specify number (0-128 valid) rules for each channel
 */
typedef union
{
    uint32_t        rnc[2];
    uint8_t         ch_rnc[8];      // WARNING!!! Funky channel order!
} can_gaflcfg_t;

/* channel indexes */
#define RNC_CH0     3
#define RNC_CH1     2
#define RNC_CH2     1
#define RNC_CH3     0
#define RNC_CH4     7


/*
 * GAFLIDj: Receive Rule ID register (j=0-15)
 */
typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    GAFLID:29;      // ID
        uint32_t    GAFLLB:1;       // apply when own transmit message is received
        uint32_t    GAFLRTR:1;      // RTR
        uint32_t    GAFLIDE:1;      // IDE
    } BIT;
} can_rule_id_t;

/*
 * GAFLMj: Receive Rule mask register (j=0-15)
 */
typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    GAFLIDM:29;     // ID mask
        uint32_t    :1;
        uint32_t    GAFLRTRM:1;     // RTR is compared mask
        uint32_t    GAFLIDEM:1;     // IDE is compared mask
    } BIT;
} can_rule_mask_t;

/*
 * GAFLP0j: Receive Rule pointer 0 register (j=0-15)
 */
typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    :8;
        uint32_t    GAFLRMDP:7;     // destination RX buffer number
        uint32_t    GAFLRMV:1;      // use rx buffer destination
        uint32_t    GAFLPTR:12;     // label
        uint32_t    GAFLDLC:4;      // DLC minimum length check (0-8)
    } BIT;
} can_rule_p0_t;

/*
 * GAFLP1j: Receive Rule pointer 1 register (j=0-15)
 */
typedef union
{
    uint32_t        UINT32;
    struct
    {
        uint32_t    GAFLFDP:23; // bits 0-7 RX FIFOs, 8-22 TX/RXFIFOs
        uint32_t    :9;
    } BIT;
} can_rule_p1_t;

/*
 *  array_ptr = (can_rxrule_t *)&RSCAN0GAFLID0;      array[16]
 */
typedef struct st_rxrule
{
    can_rule_id_t       gaflidj;    // id
    can_rule_mask_t     gaflmj;     // mask
    can_rule_p0_t       gaflp0j;    // pointer 0
    can_rule_p1_t       gaflp1j;    // pointer 1
} can_rxrule_t;


#endif // CAN_PRIVATE_HEADER_FILE
