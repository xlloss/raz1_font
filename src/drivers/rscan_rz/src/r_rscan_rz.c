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
* File Name    : r_rscan_rz.c
* Description  : Primary source code for RZ RSCAN driver.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           22.03.2015 1.00    Initial Release
*           21.03.2016 1.01    Fixed FIFO bug on line 1126 (changed == 1 to != 0)
*           04.04.2016 1.02    Fixed index bug (no return value on line 908)
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <string.h>         // memset()

#include "iodefine.h"
#include "intc.h"
/* Interface file for this package. */
#include "r_rscan_rz_if.h"
/* Configuration file for this package. */
#include "r_rscan_rz_config.h"
/* Private header file for this package. */
#include "r_rscan_rz_private.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define CAN_BOX_TYPE_MASK           (0xF0000000)
#define CAN_BOX_BUFNUM_MASK         (0x000001FF)    // tx and rx buffer number

#define CAN_BOX_RXFIFO_MASK         (0x000000FF)
#define CAN_BOX_TXFIFO_MASK         (0x0036DB00)
#define CAN_BOX_GWFIFO_MASK         (0x00492400)
#define CAN_BOX_HIST_FIFO_MASK      (0x0F800000)
#define CAN_BOX_TXFIFO_0_MASK       (0x00124900)
#define CAN_BOX_TXFIFO_1_MASK       (0x00249200)
#define CAN_BOX_TX_GW_FIFO_MASK     (CAN_BOX_TXFIFO_MASK | CAN_BOX_GWFIFO_MASK)

#define CAN_BOX_CH_FIFO_MASK        (CAN_BOX_HIST_FIFO_MASK | CAN_BOX_TX_GW_FIFO_MASK)  // chan assigned FIFOs
#define CAN_BOX_CH0_MASK            (0x00800700)    // history, TX, and GW FIFO mask for chan 0
#define CAN_BOX_CH1_MASK            (0x01003800)
#define CAN_BOX_CH2_MASK            (0x0201C000)
#define CAN_BOX_CH3_MASK            (0x040E0000)
#define CAN_BOX_CH4_MASK            (0x08700000)

#define CAN_NUM_RXFIFOS             (8)
#define CAN_NUM_TX_GW_FIFOS         (15)
#define CAN_NUM_CHANS               (5)
#define CAN_MAX_CH_RULES            (128)
#define CAN_MAX_RULES               (320)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/* driver control block */
static can_dcb_t    g_dcb = {FALSE, FALSE, 0, 0, NULL};

/* channel control block */
static can_ccb_t    g_ccb[5] = {{FALSE, NULL},
                                {FALSE, NULL},
                                {FALSE, NULL},
                                {FALSE, NULL},
                                {FALSE, NULL}};

/* tx mailbox assigned to TX and GW FIFOs */
static uint16_t     g_fifo_txmbx[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


/* channel registers */
static volatile can_chan_t       *g_ch_regs = (can_chan_t *)&RSCAN0C0CFG;

/* local functions */
static uint32_t can_root_of_2(uint32_t num);
static uint32_t can_get_fifo_ch(can_box_t fifo_id);
static uint32_t can_rule_rnc_index(uint32_t chan);
static uint32_t can_rule_ch_start(uint32_t chan);
static void     can_enable_ints(void);
static void     can_enable_rxfifos(void);
static void     can_enable_chan_fifos(uint32_t chan);
/* interrupts */
static void can_global_err_isr(uint32_t int_sense);
static void can_rxfifo_isr(uint32_t int_sense);
static void can_ch0_error_isr(uint32_t int_sense);
static void can_ch0_gwfifo_rx_isr(uint32_t int_sense);
static void can_ch0_tx_isr(uint32_t int_sense);
static void can_ch1_error_isr(uint32_t int_sense);
static void can_ch1_gwfifo_rx_isr(uint32_t int_sense);
static void can_ch1_tx_isr(uint32_t int_sense);
static void can_ch2_error_isr(uint32_t int_sense);
static void can_ch2_gwfifo_rx_isr(uint32_t int_sense);
static void can_ch2_tx_isr(uint32_t int_sense);
static void can_ch3_error_isr(uint32_t int_sense);
static void can_ch3_gwfifo_rx_isr(uint32_t int_sense);
static void can_ch3_tx_isr(uint32_t int_sense);
static void can_ch4_error_isr(uint32_t int_sense);
static void can_ch4_gwfifo_rx_isr(uint32_t int_sense);
static void can_ch4_tx_isr(uint32_t int_sense);

/******************************************************************************
* Function Name: R_CAN_Open
* Description  : This function applies clock to the CAN peripheral, sets the
*                mode to RESET, configures the timestamp and all receive
*                mailboxes.
*
* Arguments    : p_cfg-
*                    Pointer to configuration structure (see below)
*                p_callback-
*                    Optional pointer to function called from RXIF or global
*                    error interrupts.
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_OPENED-
*                    Call to Open already made
*                CAN_ERR_INVALID_ARG-
*                    An element of the p_cfg structure contains an invalid value.
*                CAN_ERR_MISSING_CALLBACK-
*                    A callback function was not provided and a main callback
*                    interrupt is enabled in config.h
*******************************************************************************/
can_err_t R_CAN_Open(can_cfg_t  *p_cfg,
                     void       (* const p_callback)(can_cb_evt_t   event,
                                                     void           *p_args))
{
can_err_t   err = CAN_SUCCESS;
volatile can_gsts_t  *p_gsts = (can_gsts_t *)&RSCAN0GSTS;
volatile can_gctr_t  *p_gctr = (can_gctr_t *)&RSCAN0GCTR;
volatile can_gcfg_t  *p_gcfg = (can_gcfg_t *)&RSCAN0GCFG;

#if CAN_CFG_PARAM_CHECKING_ENABLE
    if ((p_cfg == NULL)
     || (p_cfg->timestamp_src >= CAN_TIMESTAMP_SRC_END_ENUM)
     || (p_cfg->timestamp_div >= CAN_TIMESTAMP_DIV_END_ENUM))
    {
        return CAN_ERR_INVALID_ARG;
    }

    if ((p_callback == NULL) && ((CAN_CFG_INT_RXFIFO_THRESHOLD != 0)
                              || (CAN_CFG_INT_DLC_ERR != 0)
                              || (CAN_CFG_INT_RXFIFO_OVFL != 0)
                              || (CAN_CFG_INT_HIST_FIFO_OVFL != 0)))
    {
        return CAN_ERR_MISSING_CALLBACK;
    }

    if (g_dcb.opened == TRUE)
    {
        return CAN_ERR_OPENED;
    }
#endif
    g_dcb.opened = TRUE;

    /* Apply clock to the CAN interface (MSTP32) */
    CPG.STBCR3 &= 0xFB;

    /* Wait for CAN RAM initialization to completed */
    while (p_gsts->BIT.GRAMINIT != 0)
    {
        __asm__("nop");
    }

    /* Apply clock to RSCAN */
    p_gctr->BIT.GSLPR = 0;

    /* Enter global reset mode */
    p_gctr->BIT.GMDC = CAN_GLOBAL_MODE_RESET;


    /* Set main clock and timestamp source */

    p_gcfg->BIT.DCS = CAN_CFG_CLOCK_SOURCE;

    if (p_cfg->timestamp_src == CAN_TIMESTAMP_SRC_HALF_PCLK)
    {
        p_gcfg->BIT.TSSS = 0;   // use 1/2 pclk
    }
    else // channel bit clock
    {
        p_gcfg->BIT.TSSS = 1;   // use channel bit clock
        p_gcfg->BIT.TSBTCS = p_cfg->timestamp_src;
    }
    p_gcfg->BIT.TSP = p_cfg->timestamp_div;
    p_gctr->BIT.TSRST = 1;      // reset timestamp counter


    /* Setup rest of global configuration (0/1) */
    p_gcfg->BIT.TPRI = 1;       // transmit priority mailbox number based, not msg ID
    p_gcfg->BIT.DCE = 1;        // enable receive rule check DLC
    p_gcfg->BIT.DRE = 0;        // DLC replacement disabled
    p_gcfg->BIT.MME = 0;        // mirror function disabled
    // interval timer prescaler (p_gcfg->BIT.ITRCP) not used

    /* Setup 16 receive mailboxes */
    RSCAN0RMNB = 16;

    /* Configure global interrupts */
    p_gctr->BIT.DEIE = CAN_CFG_INT_DLC_ERR;
    p_gctr->BIT.MEIE = CAN_CFG_INT_RXFIFO_OVFL;
    p_gctr->BIT.THLEIE = CAN_CFG_INT_HIST_FIFO_OVFL;

    /* Save callback function pointer (may be NULL) */
    g_dcb.p_callback = p_callback;


    /* Register all interrupt handlers, but enable later as needed */

    R_INTC_RegistIntFunc(INTC_ID_INTRCANGERR, can_global_err_isr);
    R_INTC_SetPriority(INTC_ID_INTRCANGERR, CAN_CFG_INT_PRIORITY);

    R_INTC_RegistIntFunc(INTC_ID_INTRCANGRECC, can_rxfifo_isr);
    R_INTC_SetPriority(INTC_ID_INTRCANGRECC, CAN_CFG_INT_PRIORITY);

    R_INTC_RegistIntFunc(INTC_ID_INTRCAN0ERR, can_ch0_error_isr);
    R_INTC_SetPriority(INTC_ID_INTRCAN0ERR, CAN_CFG_INT_PRIORITY);
    R_INTC_RegistIntFunc(INTC_ID_INTRCAN1ERR, can_ch1_error_isr);
    R_INTC_SetPriority(INTC_ID_INTRCAN1ERR, CAN_CFG_INT_PRIORITY);
    R_INTC_RegistIntFunc(INTC_ID_INTRCAN2ERR, can_ch2_error_isr);
    R_INTC_SetPriority(INTC_ID_INTRCAN2ERR, CAN_CFG_INT_PRIORITY);
    R_INTC_RegistIntFunc(INTC_ID_INTRCAN3ERR, can_ch3_error_isr);
    R_INTC_SetPriority(INTC_ID_INTRCAN3ERR, CAN_CFG_INT_PRIORITY);
    R_INTC_RegistIntFunc(INTC_ID_INTRCAN4ERR, can_ch4_error_isr);
    R_INTC_SetPriority(INTC_ID_INTRCAN4ERR, CAN_CFG_INT_PRIORITY);

    R_INTC_RegistIntFunc(INTC_ID_INTRCAN0REC, can_ch0_gwfifo_rx_isr);
    R_INTC_SetPriority(INTC_ID_INTRCAN0REC, CAN_CFG_INT_PRIORITY);
    R_INTC_RegistIntFunc(INTC_ID_INTRCAN1REC, can_ch1_gwfifo_rx_isr);
    R_INTC_SetPriority(INTC_ID_INTRCAN1REC, CAN_CFG_INT_PRIORITY);
    R_INTC_RegistIntFunc(INTC_ID_INTRCAN2REC, can_ch2_gwfifo_rx_isr);
    R_INTC_SetPriority(INTC_ID_INTRCAN2REC, CAN_CFG_INT_PRIORITY);
    R_INTC_RegistIntFunc(INTC_ID_INTRCAN3REC, can_ch3_gwfifo_rx_isr);
    R_INTC_SetPriority(INTC_ID_INTRCAN3REC, CAN_CFG_INT_PRIORITY);
    R_INTC_RegistIntFunc(INTC_ID_INTRCAN4REC, can_ch4_gwfifo_rx_isr);
    R_INTC_SetPriority(INTC_ID_INTRCAN4REC, CAN_CFG_INT_PRIORITY);

    R_INTC_RegistIntFunc(INTC_ID_INTRCAN0TRX, can_ch0_tx_isr);
    R_INTC_SetPriority(INTC_ID_INTRCAN0TRX, CAN_CFG_INT_PRIORITY);
    R_INTC_RegistIntFunc(INTC_ID_INTRCAN1TRX, can_ch1_tx_isr);
    R_INTC_SetPriority(INTC_ID_INTRCAN1TRX, CAN_CFG_INT_PRIORITY);
    R_INTC_RegistIntFunc(INTC_ID_INTRCAN2TRX, can_ch2_tx_isr);
    R_INTC_SetPriority(INTC_ID_INTRCAN2TRX, CAN_CFG_INT_PRIORITY);
    R_INTC_RegistIntFunc(INTC_ID_INTRCAN3TRX, can_ch3_tx_isr);
    R_INTC_SetPriority(INTC_ID_INTRCAN3TRX, CAN_CFG_INT_PRIORITY);
    R_INTC_RegistIntFunc(INTC_ID_INTRCAN4TRX, can_ch4_tx_isr);
    R_INTC_SetPriority(INTC_ID_INTRCAN4TRX, CAN_CFG_INT_PRIORITY);

    return err;
}

/******************************************************************************
* Function Name: R_CAN_InitChan
* Description  : This function applies clock to the channel, puts the channel
*                in Reset mode, and sets the bit rate registers.
*
* Arguments    : chan -
*                    Channel to initialize (0-4)
*                p_baud-
*                    Pointer to bit rate structure
*                p_chcallback-
*                    Optional pointer to function called for transmit,
*                    gateway receive, and channel error interrupts
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_ILLEGAL_MODE-
*                    Not in global reset mode (results from Open())
*                CAN_ERR_INVALID_ARG-
*                    An invalid argument was provided
*                CAN_ERR_MISSING_CALLBACK-
*                    A callback function was not provided and a channel
*                    interrupt is enabled in config.h
*******************************************************************************/
can_err_t R_CAN_InitChan(uint8_t        chan,
                         can_bitrate_t  *p_baud,
                         void          (* const p_chcallback)(uint8_t      chan,
                                                              can_cb_evt_t event,
                                                              void         *p_args))
{
can_err_t   err         = CAN_SUCCESS;
can_yreg_t  *tmiec_regs = (can_yreg_t *)&RSCAN0TMIEC0;
can_gctr_t  *p_gctr     = (can_gctr_t *)&RSCAN0GCTR;

#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((p_gctr->BIT.GSLPR != 0)
     || (p_gctr->BIT.GMDC != CAN_GLOBAL_MODE_RESET))
    {
        return CAN_ERR_ILLEGAL_MODE;
    }

    if ((chan > 4)
     || (p_baud == NULL)
     || (p_baud->prescaler < 1) || (p_baud->prescaler > 1024)
     || (p_baud->tseg1 < 4) || (p_baud->tseg1 > 16)
     || (p_baud->tseg2 < 2) || (p_baud->tseg2 > 8)
     || (p_baud->sjw < 1) || (p_baud->sjw > 4))
    {
        return CAN_ERR_INVALID_ARG;
    }

    if ((p_chcallback == NULL) && ((CAN_CFG_INT_TXFIFO_THRESHOLD != 0)
                                || (CAN_CFG_INT_GWFIFO_RX_THRESHOLD != 0)
                                || (CAN_CFG_INT_GWFIFO_TX_THRESHOLD != 0)
                                || (CAN_CFG_INT_HIST_FIFO_THRESHOLD != 0)
                                || (CAN_CFG_INT_MBX_TX_COMPLETE != 0)
                                || (CAN_CFG_INT_MBX_TX_ABORTED != 0)
                                || (CAN_CFG_INT_BUS_ERROR != 0)
                                || (CAN_CFG_INT_ERR_WARNING != 0)
                                || (CAN_CFG_INT_ERR_PASSIVE != 0)
                                || (CAN_CFG_INT_BUS_OFF_ENTRY != 0)
                                || (CAN_CFG_INT_BUS_OFF_RECOVERY != 0)
                                || (CAN_CFG_INT_OVERLOAD_FRAME_TX != 0)
                                || (CAN_CFG_INT_BUS_LOCK != 0)
                                || (CAN_CFG_INT_ARB_LOST != 0)))
    {
        return CAN_ERR_MISSING_CALLBACK;
    }

    if (g_ccb[chan].initialized == TRUE)
    {
        return CAN_ERR_INIT_DONE;
    }
#endif
    g_ccb[chan].initialized = TRUE;


    /* Apply clock to channel */
    g_ch_regs[chan].cmctr.BIT.CSLPR = 0;

    /* Enter channel reset mode */
    g_ch_regs[chan].cmctr.BIT.CHMDC = CAN_CHMODE_RESET;     // also resets all transmit buffers

    /* Set bit rate */
    g_ch_regs[chan].cmcfg.BIT.BRP = p_baud->prescaler - 1;
    g_ch_regs[chan].cmcfg.BIT.TSEG1 = p_baud->tseg1 - 1;
    g_ch_regs[chan].cmcfg.BIT.TSEG2 = p_baud->tseg2 - 1;
    g_ch_regs[chan].cmcfg.BIT.SJW = p_baud->sjw - 1;

    /* Configure channel interrupts */
    g_ch_regs[chan].cmctr.BIT.BEIE = CAN_CFG_INT_BUS_ERROR;
    g_ch_regs[chan].cmctr.BIT.EWIE = CAN_CFG_INT_ERR_WARNING;
    g_ch_regs[chan].cmctr.BIT.EPIE = CAN_CFG_INT_ERR_PASSIVE;
    g_ch_regs[chan].cmctr.BIT.BOEIE = CAN_CFG_INT_BUS_OFF_ENTRY;
    g_ch_regs[chan].cmctr.BIT.BORIE = CAN_CFG_INT_BUS_OFF_RECOVERY;
    g_ch_regs[chan].cmctr.BIT.OLIE = CAN_CFG_INT_OVERLOAD_FRAME_TX;
    g_ch_regs[chan].cmctr.BIT.BLIE = CAN_CFG_INT_BUS_LOCK;
    g_ch_regs[chan].cmctr.BIT.ALIE = CAN_CFG_INT_ARB_LOST;
    g_ch_regs[chan].cmctr.BIT.TAIE = CAN_CFG_INT_MBX_TX_ABORTED;

    /* Configure transmit complete interrupts for all 16 transmit mailboxes for channel */
    tmiec_regs->chan[chan] = (CAN_CFG_INT_MBX_TX_COMPLETE == 0) ? 0x0000 : 0xFFFF;

    /* Save channel callback function (may be NULL) */
    g_ccb[chan].p_chcallback = p_chcallback;

    /* Configure other channel settings */
    g_ch_regs[chan].cmctr.BIT.BOM = CAN_BO_RECOVERY_ISO;    // bus off recovery mode
    g_ch_regs[chan].cmctr.BIT.ERRD = 1;                     // error flags are accumulative (0/1)


    return err;
}

/******************************************************************************
* Function Name: R_CAN_ConfigFIFO
* Description  : This function configures the FIFO specified by the box id
*                passed in. It does not become enabled until the channel
*                mode has changed.
*                RFCCx.RFE - enable in global operating or global test mode
*                CFCCk.CFE - enable in chan comm or chan halt mode
*                THLCCm.THLE - enable in chan comm or chan halt mode
*
* Arguments    : fifo_id -
*                    Box ID for FIFO to configure.
*                threshold -
*                    Number of entries in FIFO to set interrupt flag
*                txmbx_id -
*                    Associated tx mailbox for TX and GW FIFOs.
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_ILLEGAL_MODE-
*                    Not in global reset mode (results from Open())
*                CAN_ERR_CH_NO_INIT-
*                    Channel not initialized, and non-RX FIFO is requested
*                CAN_ERR_INVALID_ARG-
*                    An argument contains an invalid value.
*                CAN_ERR_MAX_ONE_GWFIFO-
*                    One gateway FIFO has already been configured.
*******************************************************************************/
can_err_t R_CAN_ConfigFIFO(can_box_t            fifo_id,
                           can_fifo_threshold_t threshold,
                           can_box_t            txmbx_id)
{
uint32_t        chan = CAN_NUM_CHANS;
uint32_t        fifo_num,i;
can_err_t       err = CAN_SUCCESS;
can_cfcck_t     *cfcck_regs = (can_cfcck_t *)&RSCAN0CFCC0;
can_rfccx_t     *rfccx_regs = (can_rfccx_t *)&RSCAN0RFCC0;
can_thlccm_t    *thlccm_regs = (can_thlccm_t *)&RSCAN0THLCC0;
can_gctr_t      *p_gctr = (can_gctr_t *)&RSCAN0GCTR;
can_yreg_t      *tmiec_regs = (can_yreg_t *)&RSCAN0TMIEC0;


    /* Get channel number if FIFO is assigned to a channel (non-RXFIFO) */
    if (((fifo_id & CAN_BOX_TYPE_MASK) == CAN_FLG_FIFO)
     && ((fifo_id & CAN_BOX_CH_FIFO_MASK) != 0))
    {
        chan = can_get_fifo_ch(fifo_id);
    }

#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((p_gctr->BIT.GSLPR != 0)
     || (p_gctr->BIT.GMDC != CAN_GLOBAL_MODE_RESET))
    {
        return CAN_ERR_ILLEGAL_MODE;
    }

    if ((chan < CAN_NUM_CHANS)
     && (g_ch_regs[chan].cmctr.BIT.CHMDC != CAN_CHMODE_RESET))
    {
        return CAN_ERR_ILLEGAL_MODE;
    }

    /* Verify box id is for FIFO and threshold is valid */
    if (((fifo_id & CAN_BOX_TYPE_MASK) != CAN_FLG_FIFO)
     || (threshold >= CAN_FIFO_THRESHOLD_END_ENUM))
    {
        return CAN_ERR_INVALID_ARG;
    }

    /* Check history FIFO threshold is 1 or 12 */
    if (((fifo_id & CAN_BOX_HIST_FIFO_MASK) != 0)
     && (threshold != CAN_FIFO_THRESHOLD_1) && (threshold != CAN_FIFO_THRESHOLD_12))
    {
        return CAN_ERR_INVALID_ARG;
    }

    /* Check txmbx argument only for TX and GW FIFOs */
    if ((fifo_id & CAN_BOX_TX_GW_FIFO_MASK) != 0)
    {
        /* verify a txmbx box id was passed */
        if ((txmbx_id & CAN_FLG_TXMBX) == 0)
        {
            return CAN_ERR_INVALID_ARG;
        }

        /* verify it belongs to the proper channel */
        if (((txmbx_id & CAN_BOX_BUFNUM_MASK) / 16) != chan)
        {
            return CAN_ERR_INVALID_ARG;
        }

        /* verify the mailbox is not already used */
        for (i=0; i < 15; i++)
        {
            if (g_fifo_txmbx[i] == txmbx_id)
            {
                return CAN_ERR_INVALID_ARG;
            }
        }

        /* if gateway, verify one gateway has not already been configured */
        if (((fifo_id & CAN_BOX_GWFIFO_MASK) != 0)
         &&  (g_dcb.gw_init == TRUE))
        {
            return CAN_ERR_MAX_ONE_GWFIFO;
        }
    }

    /* Verify channel is initialized (when not an RXFIFO) */
    if ((chan < CAN_NUM_CHANS)
     && (g_ccb[chan].initialized == FALSE))
    {
        return CAN_ERR_CH_NO_INIT;
    }
#endif


    if (fifo_id & CAN_BOX_RXFIFO_MASK)
    {
        fifo_num = can_root_of_2(fifo_id);

        /* set FIFO depth, threshold, and threshold int */

        rfccx_regs[fifo_num].BIT.RFDC = CAN_FIFO_DEPTH_16;
        rfccx_regs[fifo_num].BIT.RFIE = CAN_CFG_INT_RXFIFO_THRESHOLD;
        if (threshold == CAN_FIFO_THRESHOLD_1)
        {
            rfccx_regs[fifo_num].BIT.RFIM = 1;      // set int flag at every message
        }
        else
        {
            rfccx_regs[fifo_num].BIT.RFIM = 0;      // set int flag at threshold
            rfccx_regs[fifo_num].BIT.RFIGCV = threshold;
        }

        /* enable RXFIFO only in global operating mode or global test mode */
    }

    else if (fifo_id & CAN_BOX_TX_GW_FIFO_MASK)
    {
        fifo_num = can_root_of_2(fifo_id >> 8);

        /* set FIFO type, depth, threshold, and threshold int */

        cfcck_regs[fifo_num].BIT.CFDC = CAN_FIFO_DEPTH_16;

        if (fifo_id & CAN_BOX_TXFIFO_MASK)
        {
            cfcck_regs[fifo_num].BIT.CFM = CAN_FIFO_MODE_TRANSMIT;
            cfcck_regs[fifo_num].BIT.CFITT = 0;     // interval timer off
            cfcck_regs[fifo_num].BIT.CFTXIE = CAN_CFG_INT_TXFIFO_THRESHOLD;
            cfcck_regs[fifo_num].BIT.CFRXIE = 0;
        }
        else
        {
            cfcck_regs[fifo_num].BIT.CFM = CAN_FIFO_MODE_GATEWAY;
            cfcck_regs[fifo_num].BIT.CFITT = 0;     // interval timer off
            cfcck_regs[fifo_num].BIT.CFTXIE = CAN_CFG_INT_GWFIFO_TX_THRESHOLD;
            cfcck_regs[fifo_num].BIT.CFRXIE = CAN_CFG_INT_GWFIFO_RX_THRESHOLD;
            g_dcb.gw_init = TRUE;
        }

        if (threshold == CAN_FIFO_THRESHOLD_1)
        {
            cfcck_regs[fifo_num].BIT.CFIM = 1;      // set int flag at every message
        }
        else
        {
            cfcck_regs[fifo_num].BIT.CFIM = 0;      // set int flag at threshold
            cfcck_regs[fifo_num].BIT.CFIGCV = threshold;
        }

        /* link associated transmit mailbox and save it */
        cfcck_regs[fifo_num].BIT.CFTML = (txmbx_id & CAN_BOX_BUFNUM_MASK) % 16;
        g_fifo_txmbx[fifo_num] = txmbx_id;

        /* disable the interrupt associated with the mailbox (may or may not be set) */
        i = txmbx_id & CAN_BOX_BUFNUM_MASK;         // get buffer number
        i %= 16;                                    // get mailbox/bit number for channel
        tmiec_regs->chan[chan] &= ~(1 << i);        // reset bit
    }

    else // history FIFO
    {
        /* log all transmit sources */
        thlccm_regs[chan].BIT.THLDTE = 1;
        /* set threshold */
        thlccm_regs[chan].BIT.THLIM = (threshold == CAN_FIFO_THRESHOLD_1) ? 1:0;
        /* set interrupt enable */
        thlccm_regs[chan].BIT.THLIE = CAN_CFG_INT_HIST_FIFO_THRESHOLD;
    }

    /* Show FIFO as being used */
    g_dcb.fifo_mask |= fifo_id;

    return err;
}

/******************************************************************************
* Function Name: can_enable_ints
* Description  : This function enables all interrupts that are configured.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void can_enable_ints(void)
{

    if ((CAN_CFG_INT_DLC_ERR || CAN_CFG_INT_RXFIFO_OVFL || CAN_CFG_INT_HIST_FIFO_OVFL) != 0)
    {
        R_INTC_Enable(INTC_ID_INTRCANGERR);
    }

    if (CAN_CFG_INT_RXFIFO_THRESHOLD == 1)
    {
        R_INTC_Enable(INTC_ID_INTRCANGRECC);
    }

    if ((CAN_CFG_INT_BUS_ERROR
     ||  CAN_CFG_INT_ERR_WARNING
     ||  CAN_CFG_INT_ERR_PASSIVE
     ||  CAN_CFG_INT_BUS_OFF_ENTRY
     ||  CAN_CFG_INT_BUS_OFF_RECOVERY
     ||  CAN_CFG_INT_OVERLOAD_FRAME_TX
     ||  CAN_CFG_INT_BUS_LOCK
     ||  CAN_CFG_INT_ARB_LOST) != 0)
    {
        if (g_ccb[0].p_chcallback != NULL)
        {
            R_INTC_Enable(INTC_ID_INTRCAN0ERR);
        }
        if (g_ccb[1].p_chcallback != NULL)
        {
            R_INTC_Enable(INTC_ID_INTRCAN1ERR);
        }
        if (g_ccb[2].p_chcallback != NULL)
        {
            R_INTC_Enable(INTC_ID_INTRCAN2ERR);
        }
        if (g_ccb[3].p_chcallback != NULL)
        {
            R_INTC_Enable(INTC_ID_INTRCAN3ERR);
        }
        if (g_ccb[4].p_chcallback != NULL)
        {
            R_INTC_Enable(INTC_ID_INTRCAN4ERR);
        }
    }

    if (CAN_CFG_INT_GWFIFO_RX_THRESHOLD != 0)
    {
        if (g_ccb[0].p_chcallback != NULL)
        {
            R_INTC_Enable(INTC_ID_INTRCAN0REC);
        }
        if (g_ccb[1].p_chcallback != NULL)
        {
            R_INTC_Enable(INTC_ID_INTRCAN1REC);
        }
        if (g_ccb[2].p_chcallback != NULL)
        {
            R_INTC_Enable(INTC_ID_INTRCAN2REC);
        }
        if (g_ccb[3].p_chcallback != NULL)
        {
            R_INTC_Enable(INTC_ID_INTRCAN3REC);
        }
        if (g_ccb[4].p_chcallback != NULL)
        {
            R_INTC_Enable(INTC_ID_INTRCAN4REC);
        }
    }

    if ((CAN_CFG_INT_MBX_TX_COMPLETE
     ||  CAN_CFG_INT_MBX_TX_ABORTED
     ||  CAN_CFG_INT_TXFIFO_THRESHOLD
     ||  CAN_CFG_INT_GWFIFO_TX_THRESHOLD
     ||  CAN_CFG_INT_HIST_FIFO_THRESHOLD) != 0)
    {
        if (g_ccb[0].p_chcallback != NULL)
        {
            R_INTC_Enable(INTC_ID_INTRCAN0TRX);
        }
        if (g_ccb[1].p_chcallback != NULL)
        {
            R_INTC_Enable(INTC_ID_INTRCAN1TRX);
        }
        if (g_ccb[2].p_chcallback != NULL)
        {
            R_INTC_Enable(INTC_ID_INTRCAN2TRX);
        }
        if (g_ccb[3].p_chcallback != NULL)
        {
            R_INTC_Enable(INTC_ID_INTRCAN3TRX);
        }
        if (g_ccb[4].p_chcallback != NULL)
        {
            R_INTC_Enable(INTC_ID_INTRCAN4TRX);
        }
    }

    return;
}

/******************************************************************************
* Function Name: can_root_of_2
* Description  : This function returns the bit number of the lowest order
*                "1" bit (should only be 1 bit set).
*
* Arguments    : num-
*                    value to inspect
* Return Value : bit number of low order "1" bit (32 if value is 0)
*******************************************************************************/
static uint32_t can_root_of_2(uint32_t num)
{
uint32_t    i;

    for (i=0; i < 32; i++)
    {
        if ((num & 0x01) != 0)
        {
            return i;
        }
        else
        {
            num >>= 1;
        }
    }

    return i;
}

/******************************************************************************
* Function Name: can_get_fifo_chan
* Description  : This function returns the channel number associated with the
*                history, transmit, or gateway FIFO passed in.
*
* Arguments    : fifo_id
*                    TX, GW, or HIST FIFO id
* Return Value : channel FIFO belongs to
*******************************************************************************/
static uint32_t can_get_fifo_ch(can_box_t   fifo_id)
{

    if ((fifo_id & CAN_BOX_CH0_MASK) != 0)
        return 0;

    if ((fifo_id & CAN_BOX_CH1_MASK) != 0)
        return 1;

    if ((fifo_id & CAN_BOX_CH2_MASK) != 0)
        return 2;

    if ((fifo_id & CAN_BOX_CH3_MASK) != 0)
        return 3;

    return 4;
}

/******************************************************************************
* Function Name: R_CAN_AddRxRule
* Description  : This function
*
* Arguments    : p_cfg-
*                    Pointer to configuration structure (see below)
*                p_callback-
*                    Optional pointer to function called from RXIF or global
*                    error interrupts.
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_ILLEGAL_MODE
*                    Not in global reset mode (results from Open())
*                CAN_ERR_INVALID_ARG-
*                    An element of the p_cfg structure contains an invalid value.
*                CAN_ERR_MISSING_CALLBACK-
*                    A callback function was not provided and a main callback
*                    interrupt is enabled in config.h
*******************************************************************************/
can_err_t R_CAN_AddRxRule(uint8_t       chan,
                          can_filter_t  *p_filter,
                          can_box_t     dst_box)
{
uint32_t        i;
can_err_t       err = CAN_SUCCESS;
can_gaflectr_t  *p_gaflectr = (can_gaflectr_t *)&RSCAN0GAFLECTR;
can_gaflcfg_t   *gaflcfg_regs = (can_gaflcfg_t *)&RSCAN0GAFLCFG0;
can_rxrule_t    *rule_arr = (can_rxrule_t *)&RSCAN0GAFLID0;
can_gctr_t      *p_gctr = (can_gctr_t *)&RSCAN0GCTR;


    /* Must clear here. Not documented as such. */
    if (g_dcb.rule_count == 0)
    {
        /* clear rule counts */
        RSCAN0GAFLCFG0 = 0;
        RSCAN0GAFLCFG1 = 0;
    }

#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((p_gctr->BIT.GSLPR != 0)
     || (p_gctr->BIT.GMDC != CAN_GLOBAL_MODE_RESET))
    {
        return CAN_ERR_ILLEGAL_MODE;
    }

    if ((chan >= CAN_NUM_CHANS)
     || (p_filter == NULL)
     || (p_filter->check_ide > 1)
     || (p_filter->ide > 1)
     || (p_filter->check_rtr > 1)
     || (p_filter->rtr > 1)
     || (p_filter->min_dlc > 8))
    {
        return CAN_ERR_INVALID_ARG;
    }

    /* verify RXMBX or RXFIFO for dst_box */
    if (((dst_box & CAN_FLG_RXMBX) == 0)
      && (((dst_box & CAN_FLG_FIFO) == 0) || ((dst_box & CAN_BOX_RXFIFO_MASK) == 0)))
    {
        return CAN_ERR_INVALID_ARG;
    }

    /* if RXIFO, verify configured */
    if ((dst_box & CAN_FLG_FIFO)
     && ((g_dcb.fifo_mask & dst_box) != dst_box))
    {
        return CAN_ERR_INVALID_ARG;
    }

    /* verify not already at number rules configured for channel */
    if (((chan == 0) && (gaflcfg_regs->ch_rnc[RNC_CH0] == CAN_CFG_NUM_RULES_CH0))
     || ((chan == 1) && (gaflcfg_regs->ch_rnc[RNC_CH1] == CAN_CFG_NUM_RULES_CH1))
     || ((chan == 2) && (gaflcfg_regs->ch_rnc[RNC_CH2] == CAN_CFG_NUM_RULES_CH2))
     || ((chan == 3) && (gaflcfg_regs->ch_rnc[RNC_CH3] == CAN_CFG_NUM_RULES_CH3))
     || ((chan == 4) && (gaflcfg_regs->ch_rnc[RNC_CH4] == CAN_CFG_NUM_RULES_CH4)))
    {
        return CAN_ERR_MAX_RULES;
    }
#endif

    /* enable receive rule writing */
    p_gaflectr->BIT.AFLDAE = 1;

    /* Set receive rule page:
     * Start of rules for this channel + number rules entered so far for channel
     *   divided by 16 rules per page (20 pages total)
     */
    i = can_rule_ch_start(chan) + gaflcfg_regs->ch_rnc[can_rule_rnc_index(chan)];
    p_gaflectr->BIT.AFLPN = i / 16;


    /* load rule info */

    i = i % 16;                             // offset in current page
    rule_arr[i].gaflidj.BIT.GAFLIDE = p_filter->ide;
    rule_arr[i].gaflidj.BIT.GAFLRTR = p_filter->rtr;
    rule_arr[i].gaflidj.BIT.GAFLID = p_filter->id;
    rule_arr[i].gaflidj.BIT.GAFLLB = 0;     // no mirrored messages

    rule_arr[i].gaflmj.BIT.GAFLIDEM = p_filter->check_ide;
    rule_arr[i].gaflmj.BIT.GAFLRTRM = p_filter->check_rtr;
    rule_arr[i].gaflmj.BIT.GAFLIDM = p_filter->id_mask;

    rule_arr[i].gaflp0j.BIT.GAFLDLC = p_filter->min_dlc;
    rule_arr[i].gaflp0j.BIT.GAFLPTR = p_filter->label;

    if ((dst_box & CAN_FLG_RXMBX) != 0)
    {
        rule_arr[i].gaflp0j.BIT.GAFLRMV = 1;     // RXMBX
        rule_arr[i].gaflp0j.BIT.GAFLRMDP = dst_box & CAN_BOX_BUFNUM_MASK;
    }
    else // RXFIFO
    {
        rule_arr[i].gaflp0j.BIT.GAFLRMV = 0;     // RXFIFO
        rule_arr[i].gaflp1j.BIT.GAFLFDP = dst_box & CAN_BOX_RXFIFO_MASK;
    }


    /* increment the number of rules in use */
    i = can_rule_rnc_index(chan);
    gaflcfg_regs->ch_rnc[i]++;
    g_dcb.rule_count++;

    /* disable receive rule writing */
    p_gaflectr->BIT.AFLDAE = 0;

    return err;
}


/******************************************************************************
* Function Name: can_rule_rnc_index
* Description  : The rule count for each channel is spread over two registers,
*                and the channel order is reverse of the byte order significance.
*                This function returns the channel index of a byte array in
*                union with these registers.
* Arguments    : chan-
*                    channel number
* Return Value : number of rules prior to current channel
*******************************************************************************/
static uint32_t can_rule_rnc_index(uint32_t chan)
{
uint32_t    index;

    if (chan == 0)
    {
        index = RNC_CH0;
    }
    else if (chan == 1)
    {
        index = RNC_CH1;
    }
    else if (chan == 2)
    {
        index = RNC_CH2;
    }
    else if (chan == 3)
    {
        index = RNC_CH3;
    }
    else
    {
        index = RNC_CH4;
    }

    return(index);
}


/******************************************************************************
* Function Name: can_rule_ch_start
* Description  : Of the 320 rules possible, this function returns the "index"
*                of where the rules start for a particular channel.
* Arguments    : chan-
*                    channel number
* Return Value : number of rules prior to current channel
*******************************************************************************/
static uint32_t can_rule_ch_start(uint32_t chan)
{
uint32_t base = 0;

    if (chan == 1)
    {
        base = CAN_CFG_NUM_RULES_CH0;
    }
    else if (chan == 2)
    {
        base = CAN_CFG_NUM_RULES_CH0 + CAN_CFG_NUM_RULES_CH1;
    }
    else if (chan == 3)
    {
        base = CAN_CFG_NUM_RULES_CH0 + CAN_CFG_NUM_RULES_CH1 + CAN_CFG_NUM_RULES_CH2;
    }
    else if (chan == 4)
    {
        base = CAN_CFG_NUM_RULES_CH0 + CAN_CFG_NUM_RULES_CH1 + CAN_CFG_NUM_RULES_CH2 + CAN_CFG_NUM_RULES_CH3;
    }

    return base;
}


/******************************************************************************
* Function Name: R_CAN_Control
* Description  : This function
*
* Arguments    : cmd-
*                    Command to execute
*                arg1-
*                    for CAN_CMD_ABORT_TX, this is a transmit mailbox id
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_NOT_OPENED-
*                    Call to Open not made
*                CAN_ERR_INVALID_ARG-
*                    An invalid value passed for one of the parameters.
*******************************************************************************/
can_err_t R_CAN_Control(can_cmd_t   cmd,
                        uint32_t    arg1)
{
uint32_t    i;
can_err_t   err = CAN_SUCCESS;
can_tmcp_t  *tmcp_regs = (can_tmcp_t *)&RSCAN0TMC0;
can_gctr_t  *p_gctr = (can_gctr_t *)&RSCAN0GCTR;
can_gsts_t  *p_gsts = (can_gsts_t *)&RSCAN0GSTS;


#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    if (g_dcb.opened == FALSE)
    {
        return CAN_ERR_NOT_OPENED;
    }

    if ((cmd >= CAN_CMD_END_ENUM)
     || ((cmd == CAN_CMD_ABORT_TX) && ((arg1 & CAN_FLG_TXMBX) == 0)))   // arg1 must be txmbx
    {
        return CAN_ERR_INVALID_ARG;
    }
#endif

    switch (cmd)
    {
    case CAN_CMD_ABORT_TX:
        /* get the transmit buffer number (in communications mode) */
        arg1 &= CAN_BOX_BUFNUM_MASK;
        /* request abort */
        tmcp_regs[arg1].BIT.TMTAR = 1;
        break;

    case CAN_CMD_RESET_TIMESTAMP:
        p_gctr->BIT.TSRST = 1;
        break;

    case CAN_CMD_SET_MODE_COMM:
        /* set global operating mode */
        p_gctr->BIT.GMDC = CAN_GLOBAL_MODE_OPERATING;

        /* enable configured RXFIFOs */
        can_enable_rxfifos();

        /* enable interrupts */
        can_enable_ints();

        /* put initialized channels into communications mode */
        for (i=0; i < CAN_NUM_CHANS; i++)
        {
            if (g_ccb[i].initialized == TRUE)
            {
                g_ch_regs[i].cmctr.BIT.CHMDC = CAN_CHMODE_COMM;
                while ((g_ch_regs[i].cmsts.UINT32 & 0x07) != 0)
                {
                    __asm__("nop");
                }

                /* enable channel FIFOs */
                can_enable_chan_fifos(i);

                while (g_ch_regs[i].cmsts.BIT.COMSTS == 0)
                {
                    __asm__("nop");
                }
            }
        }

        break;

    default:
        /* set global test mode */
        p_gctr->BIT.GMDC = CAN_GLOBAL_MODE_TEST;
        while ((p_gsts->BIT.GHLTSTS) != 1)
        {
            __asm__("nop");
        }

        /* enable configured RXFIFOs */
        can_enable_rxfifos();

        /* enable interrupts */
        can_enable_ints();

        /* put initialized channels into appropriate test mode */
        for (i=0; i < CAN_NUM_CHANS; i++)
        {
            if (g_ccb[i].initialized == TRUE)
            {
                /* set to channel halt mode */
                g_ch_regs[i].cmctr.BIT.CHMDC = CAN_CHMODE_HALT;
                while (g_ch_regs[i].cmsts.BIT.CHLTSTS != 1)
                    ;

                /* enable channel FIFOs */
                can_enable_chan_fifos(i);

                /* enable test mode */
                g_ch_regs[i].cmctr.BIT.CTME = 1;

                /* set test mode type */
                if (cmd == CAN_CMD_SET_MODE_TST_STANDARD)
                {
                    g_ch_regs[i].cmctr.BIT.CTMS = CAN_TEST_MODE_STD;
                }
                else if (cmd == CAN_CMD_SET_MODE_TST_LISTEN)
                {
                    g_ch_regs[i].cmctr.BIT.CTMS = CAN_TEST_MODE_LISTEN;
                }
                else if (cmd == CAN_CMD_SET_MODE_TST_INT_LOOPBACK)
                {
                    g_ch_regs[i].cmctr.BIT.CTMS = CAN_TEST_MODE_INT_LOOPBACK;
                }
                else if (cmd == CAN_CMD_SET_MODE_TST_EXT_LOOPBACK)
                {
                    g_ch_regs[i].cmctr.BIT.CTMS = CAN_TEST_MODE_EXT_LOOPBACK;
                }
                else // cmd == CAN_CMD_SET_MODE_TST_INTERCHANNEL
                {
                    // TODO: only need execute once
                    RSCAN0GTSTCFG = 0x1F;       // all channels
                    RSCAN0GTSTCTR = 0x01;       // enable comm between channels
                }

                /* set global operating mode */
                p_gctr->BIT.GMDC = CAN_GLOBAL_MODE_OPERATING;
                while (RSCAN0GSTS != 0)
                {
                    __asm__("nop");
                }

                /* go into channel communication mode */
                g_ch_regs[i].cmctr.BIT.CHMDC = CAN_CHMODE_COMM;
                while ((g_ch_regs[i].cmsts.UINT32 & 0x07) != 0)
                {
                    __asm__("nop");
                }

                while (g_ch_regs[i].cmsts.BIT.COMSTS == 0)
                {
                    __asm__("nop");
                }
            } // chan initialized
        } // for every chan
        break;
    }

    return err;
}


/******************************************************************************
* Function Name: can_enable_rxfifos
* Description  : This function enables configured RXFIFOs.
*                SYSTEM NUST BE IN GLOBAL OPERATING OR GLOBAL TEST MODE
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void can_enable_rxfifos(void)
{
uint32_t    i;
can_rfccx_t *rfccx_regs = (can_rfccx_t *)&RSCAN0RFCC0;


    /* enable configured RXFIFOs */
    for (i=0; i < CAN_NUM_RXFIFOS; i++)
    {
        if ((g_dcb.fifo_mask & (1<<i)) != 0)
        {
            rfccx_regs[i].BIT.RFE = 1;
        }
    }
}


/******************************************************************************
* Function Name: can_enable_chan_fifos
* Description  : This function enables configured channel-specific FIFOs.
*                SYSTEM NUST BE IN CHAN COMM OR CHAN HALT MODE
* Arguments    : chan-
*                   Channel number (0-4)
* Return Value : none
*******************************************************************************/
static void can_enable_chan_fifos(uint32_t chan)
{
uint32_t     mask,base,i;
can_cfcck_t  *cfcck_regs = (can_cfcck_t *)&RSCAN0CFCC0;
can_thlccm_t *thlccm_regs = (can_thlccm_t *)&RSCAN0THLCC0;


    /* enable TX and GW FIFOs */
    mask = g_dcb.fifo_mask >> 8;
    for (i=0, base=chan*3; i < 3; i++)
    {
        if ((mask & (1 << (base+i))) != 0)    // base+i is CF FIFO number (0-14)
        {
            cfcck_regs[base+i].BIT.CFE = 1;
        }
    }

    /* enable HIST FIFO */
    if ((g_dcb.fifo_mask & (1 << (23+chan))) != 0)
    {
        thlccm_regs[chan].BIT.THLE = 1;
    }
}


/******************************************************************************
* Function Name: R_CAN_SendMsg
* Description  : This function checks to see if the transmit mailbox or FIFO
*                is already full. If not, the appropriate registers are loaded
*                with the message and marked as ready to send. If the message
*                is for a transmit mailbox, this function will block until
*                transmission is complete. For TXFIFOs and interrupt driven
*                transmit mailboxes, this function does not wait for transmit
*                complete.
* Arguments    : box_id-
*                    TXMBX or TXFIFO
*                p_txmsg-
*                    Pointer to structure containing message info to send
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_BOX_FULL
*                    Cannot send at this time
*                CAN_ERR_INVALID_ARG-
*                    An invalid value passed for one of the parameters.
*                CAN_ERR_ILLEGAL_MODE-
*                    Channel not in proper mode
*******************************************************************************/
can_err_t R_CAN_SendMsg(can_box_t   box_id,
                        can_txmsg_t *p_txmsg)
{
uint32_t            chan,i;
can_err_t           err = CAN_SUCCESS;
can_tmstsp_t        *tmstsp_regs = (can_tmstsp_t *)&RSCAN0TMSTS0;
can_tmcp_t          *tmcp_regs = (can_tmcp_t *)&RSCAN0TMC0;
can_fifo_stat_t     *cfstsk_regs = (can_fifo_stat_t *)&RSCAN0CFSTS0;
volatile uint32_t   *cfpctrk_regs = &RSCAN0CFPCTR0;
can_msg_t           *msg_regs;


    if (box_id & CAN_FLG_TXMBX)
    {
        /* get pointer to base of message registers and index for this TXMBX */
        msg_regs = (can_msg_t *)&RSCAN0TMID0;
        i = box_id & CAN_BOX_BUFNUM_MASK;       // buffer number
    }
    else if (((box_id & CAN_FLG_FIFO) != 0) && ((box_id & CAN_BOX_TXFIFO_MASK) != 0))
    {
        /* get pointer to base of message registers and index for this TXFIFO */
        msg_regs = (can_msg_t *)&RSCAN0CFID0;
        i = can_root_of_2(box_id >> 8);         // TXRXFIFO number
    }
#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    else
    {
        return CAN_ERR_INVALID_ARG;             // not a transmit box
    }

    /* get channel number for box_id */
    if (box_id & CAN_FLG_TXMBX)
    {
        chan = i / 16;      // tx buffer number / 16
    }
    else
    {
        chan = i / 3;       // txrxfifo number / 3
    }

    /* check message pointer provided */
    if (p_txmsg == NULL)
    {
        return CAN_ERR_INVALID_ARG;
    }

    /* check if in communication mode */
    if (g_ch_regs[chan].cmsts.BIT.COMSTS == 0)
    {
        return CAN_ERR_ILLEGAL_MODE;
    }
#endif

    if (box_id & CAN_FLG_TXMBX)
    {
        /* return if transmit or abort request already outstanding */
        if ((tmcp_regs[i].UINT8 & CAN_TMCP_REQ_PRESENT) != 0)
        {
            return CAN_ERR_BOX_FULL;
        }
    }
    else // TXFIFO
    {
        /* return if fifo is full */
        if (cfstsk_regs[i].bit.fll == 1)
        {
            return CAN_ERR_BOX_FULL;
        }
    }


    /* load message into registers */

    msg_regs[i].id.bit.ide = p_txmsg->ide;
    msg_regs[i].id.bit.rtr = p_txmsg->rtr;
    msg_regs[i].id.bit.id = p_txmsg->id;
    msg_regs[i].id.bit.thlen = p_txmsg->log_history;
    msg_regs[i].ptr.bit.ptr = p_txmsg->label;
    msg_regs[i].ptr.bit.dlc = p_txmsg->dlc;
    msg_regs[i].df0.bit.db0 = p_txmsg->data[0];
    msg_regs[i].df0.bit.db1 = p_txmsg->data[1];
    msg_regs[i].df0.bit.db2 = p_txmsg->data[2];
    msg_regs[i].df0.bit.db3 = p_txmsg->data[3];
    msg_regs[i].df1.bit.db4 = p_txmsg->data[4];
    msg_regs[i].df1.bit.db5 = p_txmsg->data[5];
    msg_regs[i].df1.bit.db6 = p_txmsg->data[6];
    msg_regs[i].df1.bit.db7 = p_txmsg->data[7];

    /* initiate transmit request */

    if (box_id & CAN_FLG_TXMBX)
    {
        tmstsp_regs[i].BIT.TMTRF = 0;               // clear last result
        tmcp_regs[i].BIT.TMOM = p_txmsg->one_shot;  // set one-shot option
        tmcp_regs[i].BIT.TMTR = 1;                  // request transmit

        /* wait for transmit complete if not using interrupts */
        if (CAN_CFG_INT_MBX_TX_COMPLETE == 0)
        {
            while (tmstsp_regs[i].BIT.TMTRF == 0)
            {
                __asm__("nop");
            }
        }
    }
    else // TXFIFO
    {
        cfpctrk_regs[i] = 0xFF;   // increment TXFIFO index
    }

    return err;
}

/******************************************************************************
* Function Name: R_CAN_GetMsg
* Description  : This function checks to see if the receive mailbox or FIFO
*                contains a message. If it does, the p_rxmsg buffer is loaded
*                with the message info.
*
* Arguments    : box_id-
*                    RXMBX or RXFIFO
*                p_rxmsg-
*                    Pointer to structure to hold message info
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_BOX_EMPTY
*                    No message currently waiting in receive mailbox or FIFO
*                CAN_ERR_INVALID_ARG-
*                    An invalid value passed for one of the parameters.
*                CAN_ERR_ILLEGAL_MODE-
*                    System not in proper mode
*******************************************************************************/
can_err_t R_CAN_GetMsg(can_box_t   box_id,
                       can_rxmsg_t *p_rxmsg)
{
uint32_t            i;
can_err_t           err = CAN_SUCCESS;
can_fifo_stat_t     *rfstsx_regs = (can_fifo_stat_t *)&RSCAN0RFSTS0;
volatile uint32_t   *rfpctrx_regs = &RSCAN0RFPCTR0;
can_msg_t           *msg_regs;

    if (box_id & CAN_FLG_RXMBX)
    {
        /* get pointer to base of message registers and index for this RXMBX */
        msg_regs = (can_msg_t *)&RSCAN0RMID0;
        i = box_id & CAN_BOX_BUFNUM_MASK;        // buffer number
//  			printf("Receive get buffer num %x\n",i);
    }
    else if (((box_id & CAN_FLG_FIFO) != 0) && ((box_id & CAN_BOX_RXFIFO_MASK) != 0))
    {
        /* get pointer to base of message registers and index for this RXFIFO */
        msg_regs = (can_msg_t *)&RSCAN0RFID0;
        i = can_root_of_2(box_id);              // RXFIFO number
//				printf("Receive get RXFIFO num\n");
    }
#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    else
    {
//				printf("No receive box\n");
        return CAN_ERR_INVALID_ARG;             // not a receive box
    }
    if (p_rxmsg == NULL)
    {
//				printf("CAN_ERR_INVALID_ARG--\n");
        return CAN_ERR_INVALID_ARG;
    }
#endif
//======================================================================

    if (box_id & CAN_FLG_RXMBX)
    {
        /* return if receive mailbox is empty */
        if ((RSCAN0RMND0 & (1 << i)) == 0)      // receive buffer 0-15 */
        {
//				printf("CAN_ERR_BOX_EMPTY-1--\n");
            return CAN_ERR_BOX_EMPTY;
        }
    }
    else // RXFIFO
    {
        /* return if FIFO is empty */
        if (rfstsx_regs[i].bit.emp == 1)
        {
//        	printf("CAN_ERR_BOX_EMPTY-2--\n");
        	
            return CAN_ERR_BOX_EMPTY;
        }
    }
    
//=============================================================   
//		printf("R_CAN_GetMsg SUCCESS\n");
    
    /* load message from registers */
    p_rxmsg->ide = msg_regs[i].id.bit.ide;
    p_rxmsg->rtr = msg_regs[i].id.bit.rtr;
    p_rxmsg->id = msg_regs[i].id.bit.id;
    p_rxmsg->label = msg_regs[i].ptr.bit.ptr;
    p_rxmsg->dlc = msg_regs[i].ptr.bit.dlc;
    p_rxmsg->timestamp = msg_regs[i].ptr.bit.ts;
    p_rxmsg->data[0] = msg_regs[i].df0.bit.db0;
    p_rxmsg->data[1] = msg_regs[i].df0.bit.db1;
    p_rxmsg->data[2] = msg_regs[i].df0.bit.db2;
    p_rxmsg->data[3] = msg_regs[i].df0.bit.db3;
    p_rxmsg->data[4] = msg_regs[i].df1.bit.db4;
    p_rxmsg->data[5] = msg_regs[i].df1.bit.db5;
    p_rxmsg->data[6] = msg_regs[i].df1.bit.db6;
    p_rxmsg->data[7] = msg_regs[i].df1.bit.db7;

    /* clear message ready */

    if (box_id & CAN_FLG_RXMBX)
    {
        RSCAN0RMND0 &= ~(1 << i);
    }
    else // RXFIFO
    {
        rfpctrx_regs[i] = 0xFF;         // increment RXFIFO index
    }
    
    return CAN_SUCCESS;
}

/******************************************************************************
* Function Name: R_CAN_GetHistoryEntry
* Description  : This function determines if there is an entry in the History
*                FIFO, and if so, determines the box_id of the transmit source
*                and loads the label associated with the message sent.
*
* Arguments    : box_id-
*                    History FIFO
*                p_entry-
*                    Pointer to structure to hold message info
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_BOX_EMPTY
*                    No entries in FIFO
*                CAN_ERR_INVALID_ARG-
*                    An invalid value passed for one of the parameters.
*                CAN_ERR_ILLEGAL_MODE-
*                    Channel not in proper mode
*******************************************************************************/
can_err_t R_CAN_GetHistoryEntry(can_box_t       box_id,
                                can_history_t   *p_entry)
{
uint32_t            i,j,txbuf_num;
can_err_t           err = CAN_SUCCESS;
can_fifo_stat_t     *thlstsm_regs = (can_fifo_stat_t *)&RSCAN0THLSTS0;
volatile uint32_t   *thlpctrm_regs = &RSCAN0THLPCTR0;
can_thlaccm_t       *thlaccm_regs = (can_thlaccm_t *)&RSCAN0THLACC0;


    if (((box_id & CAN_FLG_FIFO) != 0) && ((box_id & CAN_BOX_HIST_FIFO_MASK) != 0))
    {
        i = can_get_fifo_ch(box_id);            // HIST FIFO number (channel number)
    }
#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    else
    {
        return CAN_ERR_INVALID_ARG;             // not a History FIFO box
    }

    if (p_entry == NULL)
    {
        return CAN_ERR_INVALID_ARG;
    }

    /* check mode */
    // GCTR GMDC
    {
        //return CAN_ERR_ILLEGAL_MODE;
    }
#endif

    /* return if FIFO is empty */
    if (thlstsm_regs[i].bit.emp == 1)
    {
        return CAN_ERR_BOX_EMPTY;
    }


    /* get tx buffer number of log entry */
    txbuf_num = thlaccm_regs[i].BIT.BN;
    txbuf_num += i*16;

    /* get box associated with tx buffer and create box_id */
    if (thlaccm_regs[i].BIT.BT == HIST_SRC_TX_BUF)
    {
        p_entry->box_id = CAN_FLG_TXMBX | txbuf_num;
    }
    else if (thlaccm_regs[i].BIT.BT == HIST_SRC_TXRX_FIFO)
    {
        for (j=0; j < 15; j++)
        {
            if ((g_fifo_txmbx[j] & CAN_BOX_BUFNUM_MASK) == txbuf_num)
                break;
        }
        p_entry->box_id = CAN_FLG_FIFO | ((1<<j) << 8);
    }

    /* load message label */
    p_entry->label = thlaccm_regs[i].BIT.TID;

    /* increment HIST FIFO index */
    thlpctrm_regs[i] = 0xFF;

    return err;
}


/******************************************************************************
* Function Name: R_CAN_GetStatusMask
* Description  : This function returns various status masks based upon the
*                the type requested.
* Arguments    : type-
*                    Which type of status to return
*                chan-
*                    Optional channel number. Needed for CAN_STAT_CH_xxx types.
*                p_err-
*                    Pointer to hold error code
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_INVALID_ARG-
*                    An invalid value passed for one of the parameters.
*******************************************************************************/
uint32_t  R_CAN_GetStatusMask(can_stat_t    type,
                              uint8_t       chan,
                              can_err_t     *p_err)
{
uint32_t        i;
can_yreg_t      *yregs;
can_fifo_mask_t stat;
can_tmstsp_t    *tmsts_regs = (can_tmstsp_t *)&RSCAN0TMSTS0;
can_fifo_stat_t *thlstsm_regs = (can_fifo_stat_t *)&RSCAN0THLSTS0;
can_fifo_stat_t *rfstsx_regs = (can_fifo_stat_t *)&RSCAN0RFSTS0;
can_fifo_stat_t *cfstsk_regs = (can_fifo_stat_t *)&RSCAN0CFSTS0;


#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((type >= CAN_STAT_END_ENUM)
     || (p_err == NULL))
    {
        *p_err = CAN_ERR_INVALID_ARG;
        return 0;
    }

    if ((chan >= CAN_NUM_CHANS)
     && ((type == CAN_STAT_CH_TXMBX_SENT) || (type == CAN_STAT_CH_TXMBX_ABORTED) || (type == CAN_STAT_CH_ERROR)))
    {
        *p_err = CAN_ERR_INVALID_ARG;
        return 0;
    }
#endif

    switch (type)
    {
    case CAN_STAT_FIFO_EMPTY:
        /* RX, TX, and GW FIFOs */
        stat.uint32 = RSCAN0FESTS;
        /* HIST FIFOs */
        stat.bit.ch0_hist = (RSCAN0THLSTS0 & CAN_FIFO_EMPTY_FLG) ? 1:0;
        stat.bit.ch1_hist = (RSCAN0THLSTS1 & CAN_FIFO_EMPTY_FLG) ? 1:0;
        stat.bit.ch2_hist = (RSCAN0THLSTS2 & CAN_FIFO_EMPTY_FLG) ? 1:0;
        stat.bit.ch3_hist = (RSCAN0THLSTS3 & CAN_FIFO_EMPTY_FLG) ? 1:0;
        stat.bit.ch4_hist = (RSCAN0THLSTS4 & CAN_FIFO_EMPTY_FLG) ? 1:0;

        break;

    case CAN_STAT_FIFO_THRESHOLD:
        /* RXFIFOs */
        stat.uint32 = RSCAN0RFISTS;
        /* TX and GW FIFOs */
        stat.uint32 |= RSCAN0CFTISTS << 8;
        /* HIST FIFOs */
        stat.bit.ch0_hist = (RSCAN0THLSTS0 & CAN_FIFO_INT_FLG) ? 1:0;
        stat.bit.ch1_hist = (RSCAN0THLSTS1 & CAN_FIFO_INT_FLG) ? 1:0;
        stat.bit.ch2_hist = (RSCAN0THLSTS2 & CAN_FIFO_INT_FLG) ? 1:0;
        stat.bit.ch3_hist = (RSCAN0THLSTS3 & CAN_FIFO_INT_FLG) ? 1:0;
        stat.bit.ch4_hist = (RSCAN0THLSTS4 & CAN_FIFO_INT_FLG) ? 1:0;

        /* clear RXFIFO threshold (interrupt) flags */
        if (stat.uint32 & CAN_BOX_RXFIFO_MASK)
        {
            for (i=0; i < CAN_NUM_RXFIFOS; i++)
            {
                rfstsx_regs[i].bit.IF = 0;
            }
        }

        /* clear TX and GW FIFO threshold (interrupt) flags */
        if (stat.uint32 & CAN_BOX_TX_GW_FIFO_MASK)
        {
            for (i=0; i < CAN_NUM_TX_GW_FIFOS; i++)
            {
                cfstsk_regs[i].uint32 &= 0xFFFFFFEF;    // WARNING! Must do 32 bit write!
            }
        }

        /* clear HIST FIFO threshold (interrupt) flags */
        if (stat.uint32 & CAN_BOX_HIST_FIFO_MASK)
        {
            for (i=0; i < CAN_NUM_CHANS; i++)
            {
                thlstsm_regs[i].bit.IF = 0;
            }
        }
        break;


    case CAN_STAT_FIFO_OVFL:
        /* RX, TX, GW FIFOs */
        stat.uint32 = RSCAN0FMSTS;
        /* HIST FIFOs */
        stat.bit.ch0_hist = RSCAN0THLSTS0 & CAN_FIFO_OVFL_FLG;
        stat.bit.ch1_hist = RSCAN0THLSTS1 & CAN_FIFO_OVFL_FLG;
        stat.bit.ch2_hist = RSCAN0THLSTS2 & CAN_FIFO_OVFL_FLG;
        stat.bit.ch3_hist = RSCAN0THLSTS3 & CAN_FIFO_OVFL_FLG;
        stat.bit.ch4_hist = RSCAN0THLSTS4 & CAN_FIFO_OVFL_FLG;

        /* clear RXFIFO overflow flags */
        for (i=0; i < CAN_NUM_RXFIFOS; i++)
        {
            rfstsx_regs[i].bit.mlt = 0;
        }

        /* clear GWFIFO overflow flags */
        RSCAN0CFSTS2  &= ~CAN_FIFO_OVFL_FLG;
        RSCAN0CFSTS5  &= ~CAN_FIFO_OVFL_FLG;
        RSCAN0CFSTS8  &= ~CAN_FIFO_OVFL_FLG;
        RSCAN0CFSTS11 &= ~CAN_FIFO_OVFL_FLG;
        RSCAN0CFSTS14 &= ~CAN_FIFO_OVFL_FLG;

        /* clear HIST FIFO overflow flags */
        for (i=0; i < CAN_NUM_CHANS; i++)
        {
            thlstsm_regs[i].bit.mlt = 0;
        }
        break;


    case CAN_STAT_RXMBX_FULL:
        stat.uint32 = RSCAN0RMND0;
        break;

    case CAN_STAT_GLOBAL_ERR:
        stat.uint32 = RSCAN0GERFL;
        RSCAN0GERFL &= ~CAN_MASK_ERR_DLC;
        break;

    case CAN_STAT_CH_TXMBX_SENT:
        yregs = (can_yreg_t *)&RSCAN0TMTCSTS0;
        stat.uint32 = yregs->chan[chan];
        /* reset bits */
        if (stat.uint32 != 0)
        {
            for (i=0; i < 16; i++)
            {
                tmsts_regs[chan*16 + i].BIT.TMTRF = 0;
            }
        }
        break;

    case CAN_STAT_CH_TXMBX_ABORTED:
        yregs = (can_yreg_t *)&RSCAN0TMTASTS0;
        stat.uint32 = (uint32_t)yregs->chan[chan];

        /* reset bits */
        if (stat.uint32 != 0)
        {
            for (i=0; i < 16; i++)
            {
                tmsts_regs[chan*16 + i].BIT.TMTRF = 0;
            }
        }
        break;

    case CAN_STAT_CH_ERROR:
        stat.uint32 = g_ch_regs[chan].cmerfl.UINT32;
        /* reset bits */
        g_ch_regs[chan].cmerfl.UINT32 = 0;
        break;
    }

    *p_err = CAN_SUCCESS;
    return stat.uint32;
}


/******************************************************************************
* Function Name: R_CAN_GetCountFIFO
* Description  : This function returns the number of entries used in a FIFO.
* Arguments    : box_id-
*                    FIFO box id
*                p_err-
*                    Pointer to hold error code
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_INVALID_ARG-
*                    An invalid value passed for one of the parameters.
*******************************************************************************/
uint32_t R_CAN_GetCountFIFO(can_box_t  box_id,
                            can_err_t  *p_err)
{
uint32_t            fifo_num;
can_fifo_stat_t     *rfstsx_regs = (can_fifo_stat_t *)&RSCAN0RFSTS0;
can_fifo_stat_t     *cfstsk_regs = (can_fifo_stat_t *)&RSCAN0CFSTS0;
can_fifo_stat_t     *thlstsm_regs = (can_fifo_stat_t *)&RSCAN0THLSTS0;

#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    if (((box_id & CAN_BOX_TYPE_MASK) != CAN_FLG_FIFO)
     || (p_err == NULL))
    {
        *p_err = CAN_ERR_INVALID_ARG;
        return 0;
    }
#endif

    if ((box_id & CAN_BOX_RXFIFO_MASK) != 0)
    {
        fifo_num = can_root_of_2(box_id);
        return rfstsx_regs[fifo_num].bit.mc;
    }
    else if ((box_id & CAN_BOX_TX_GW_FIFO_MASK) != 0)
    {
        fifo_num = can_root_of_2(box_id >> 8);
        return cfstsk_regs[fifo_num].bit.mc;
    }
    else // HIST FIFOs
    {
        fifo_num = can_root_of_2(box_id >> 23);
        return thlstsm_regs[fifo_num].bit.mc;
    }
}


/******************************************************************************
* Function Name: R_CAN_GetCountErr
* Description  : This function returns the number transmit or receive errors
*                on a channel.
* Arguments    : type-
*                    transmit or receive
*                chan-
*                    channel (0-4)
*                p_err-
*                    pointer to hold error code
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_INVALID_ARG-
*                    An invalid value passed for one of the parameters.
*******************************************************************************/
uint32_t R_CAN_GetCountErr(can_count_t  type,
                           uint8_t      chan,
                           can_err_t    *p_err)
{
#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((type >= CAN_COUNT_END_ENUM)
     || (chan >= CAN_NUM_CHANS)
     || (p_err == NULL))
    {
        return CAN_ERR_INVALID_ARG;
    }
#endif

    *p_err = CAN_SUCCESS;

    if (type == CAN_COUNT_RX_ERR)
    {
        return g_ch_regs[chan].cmsts.BIT.REC;
    }
    else // tx err
    {
        return g_ch_regs[chan].cmsts.BIT.TEC;
    }
}


/******************************************************************************
* Function Name: R_CAN_Close
* Description  : This function
* Arguments    : None
* Return Value : None
*******************************************************************************/
void R_CAN_Close(void)
{
can_gctr_t      *p_gctr = (can_gctr_t *)&RSCAN0GCTR;
can_gaflcfg_t   *gaflcfg_regs = (can_gaflcfg_t *)&RSCAN0GAFLCFG0;

    /* enter global reset mode */
    p_gctr->BIT.GMDC = CAN_GLOBAL_MODE_RESET;

    /*
     * NOTE: If you manually clear rule counts anywhere in this function, they
     * will go to zero, but then magically go back to their previous value later.
     * Must clear in AddRxRule().
     */

    /* disable interrupts */
    R_INTC_Disable(INTC_ID_INTRCANGERR);
    R_INTC_Disable(INTC_ID_INTRCANGRECC);
    R_INTC_Disable(INTC_ID_INTRCAN0ERR);
    R_INTC_Disable(INTC_ID_INTRCAN1ERR);
    R_INTC_Disable(INTC_ID_INTRCAN2ERR);
    R_INTC_Disable(INTC_ID_INTRCAN3ERR);
    R_INTC_Disable(INTC_ID_INTRCAN4ERR);
    R_INTC_Disable(INTC_ID_INTRCAN0REC);
    R_INTC_Disable(INTC_ID_INTRCAN1REC);
    R_INTC_Disable(INTC_ID_INTRCAN2REC);
    R_INTC_Disable(INTC_ID_INTRCAN3REC);
    R_INTC_Disable(INTC_ID_INTRCAN4REC);
    R_INTC_Disable(INTC_ID_INTRCAN0TRX);
    R_INTC_Disable(INTC_ID_INTRCAN1TRX);
    R_INTC_Disable(INTC_ID_INTRCAN2TRX);
    R_INTC_Disable(INTC_ID_INTRCAN3TRX);
    R_INTC_Disable(INTC_ID_INTRCAN4TRX);

    /* global stop mode */
    p_gctr->BIT.GSLPR = 1;
    while (p_gctr->BIT.GSLPR != 1)
        ;

    /* clear internal structures*/
    memset(&g_dcb, 0, sizeof(g_dcb));
    memset(&g_ccb, 0, sizeof(g_ccb));
    memset(&g_fifo_txmbx, 0, sizeof(g_fifo_txmbx));

    /* Remove clock from the CAN interface (MSTP32) */
    CPG.STBCR3 |= 0x04;

}


/*****************************************************************************
* Function Name: R_CAN_GetVersion
* Description  : Returns the version of this module. The version number is
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
******************************************************************************/
#pragma inline(R_CAN_GetVersion)
uint32_t  R_CAN_GetVersion(void)
{
uint32_t const version = (CAN_VERSION_MAJOR << 16) | CAN_VERSION_MINOR;

    return version;
}


/******************************************************************************
* Function Name: can_global_err_isr
* Description  : This function is executed when the INTCANGERR interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_global_err_isr(uint32_t int_sense)
{
    uint32_t    i;
    can_fifo_stat_t *thlstsm_regs = (can_fifo_stat_t *)&RSCAN0THLSTS0;
    can_fifo_stat_t *rfstsx_regs = (can_fifo_stat_t *)&RSCAN0RFSTS0;


    R_INTC_Disable(INTC_ID_INTRCANGERR);

    g_dcb.p_callback(CAN_EVT_GLOBAL_ERR, NULL);

    /* reset interrupt flags */

    /* clear HIST FIFO overflow flags (for THLES error) */
    for (i=0; i < CAN_NUM_CHANS; i++)
    {
        thlstsm_regs[i].bit.mlt = 0;
    }

    /* clear RXFIFO overflow flags (for MES error) */
    for (i=0; i < CAN_NUM_RXFIFOS; i++)
    {
        rfstsx_regs[i].bit.mlt = 0;
    }

    /* clear GWFIFO receive overflow flags (for MES error) */
    RSCAN0CFSTS2  &= ~CAN_FIFO_OVFL_FLG;
    RSCAN0CFSTS5  &= ~CAN_FIFO_OVFL_FLG;
    RSCAN0CFSTS8  &= ~CAN_FIFO_OVFL_FLG;
    RSCAN0CFSTS11 &= ~CAN_FIFO_OVFL_FLG;
    RSCAN0CFSTS14 &= ~CAN_FIFO_OVFL_FLG;

    /* clear dlc error flag (for DES error) */
    RSCAN0GERFL = 0;

    R_INTC_Enable(INTC_ID_INTRCANGERR);
}

/******************************************************************************
* Function Name: can_rxfifo_isr
* Description  : This function is executed when the INTCANGRECC interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_rxfifo_isr(uint32_t int_sense)
{
    uint32_t        i;
    can_fifo_stat_t *rfstsx_regs = (can_fifo_stat_t *)&RSCAN0RFSTS0;

    R_INTC_Disable(INTC_ID_INTRCANGRECC);

    g_dcb.p_callback(CAN_EVT_RXFIFO_THRESHOLD, NULL);

    /* reset interrupt flags */
    for (i=0; i < CAN_NUM_RXFIFOS; i++)
    {
        rfstsx_regs[i].bit.IF = 0;          // do as 32-bit write?
    }

    R_INTC_Enable(INTC_ID_INTRCANGRECC);
}

/******************************************************************************
* Function Name: can_ch0_error_isr
* Description  : This function is executed when the INTCAN0ERR interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_ch0_error_isr(uint32_t int_sense)
{
    R_INTC_Disable(INTC_ID_INTRCAN0ERR);

    g_ccb[0].p_chcallback(CAN_CH0, CAN_EVT_CHANNEL_ERR, NULL);

    /* reset interrupt flags */
    RSCAN0C0ERFL = 0;

    R_INTC_Enable(INTC_ID_INTRCAN0ERR);
}

/******************************************************************************
* Function Name: can_ch0_gwfifo_rx_isr
* Description  : This function is executed when the INTCAN0REC interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_ch0_gwfifo_rx_isr(uint32_t int_sense)
{
    R_INTC_Disable(INTC_ID_INTRCAN0REC);

    g_ccb[0].p_chcallback(CAN_CH0, CAN_EVT_GWFIFO_RX_THRESHOLD, NULL);

    /* reset interrupt flag */
    RSCAN0CFSTS2 &= 0xFFFFFFF7;

    R_INTC_Enable(INTC_ID_INTRCAN0REC);
}

/******************************************************************************
* Function Name: can_ch0_tx_isr
* Description  : This function is executed when the INTCAN0TRX interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_ch0_tx_isr(uint32_t int_sense)
{
    uint32_t        i;
    can_tmstsp_t    *tmsts_regs = (can_tmstsp_t *)&RSCAN0TMSTS0;


    R_INTC_Disable(INTC_ID_INTRCAN0TRX);

    g_ccb[0].p_chcallback(CAN_CH0, CAN_EVT_TRANSMIT, NULL);

    /* reset mailbox transmit complete or abort flags */
    for (i=0; i < 16; i++)
    {
        tmsts_regs[i].BIT.TMTRF = 0;
    }

    /* reset TX and GW FIFO threshold flags */
    RSCAN0CFSTS0 &= 0xFFFFFFEF;    // WARNING! Must do 32 bit write!
    RSCAN0CFSTS1 &= 0xFFFFFFEF;    // WARNING! Must do 32 bit write!
    RSCAN0CFSTS2 &= 0xFFFFFFEF;    // WARNING! Must do 32 bit write!

    /* reset HIST FIFO threshold flag */
    RSCAN0THLSTS0 &= 0xFFFFFFEF;

    R_INTC_Enable(INTC_ID_INTRCAN0TRX);
}

/******************************************************************************
* Function Name: can_ch1_error_isr
* Description  : This function is executed when the INTCAN1ERR interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_ch1_error_isr(uint32_t int_sense)
{
    R_INTC_Disable(INTC_ID_INTRCAN1ERR);

    g_ccb[1].p_chcallback(CAN_CH1, CAN_EVT_CHANNEL_ERR, NULL);

    /* reset interrupt flags */
    RSCAN0C1ERFL = 0;

    R_INTC_Enable(INTC_ID_INTRCAN1ERR);
}

/******************************************************************************
* Function Name: can_ch1_gwfifo_rx_isr
* Description  : This function is executed when the INTCAN1REC interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_ch1_gwfifo_rx_isr(uint32_t int_sense)
{
    R_INTC_Disable(INTC_ID_INTRCAN1REC);

    g_ccb[1].p_chcallback(CAN_CH1, CAN_EVT_GWFIFO_RX_THRESHOLD, NULL);

    /* reset interrupt flag */
    RSCAN0CFSTS5 &= 0xFFFFFFF7;

    R_INTC_Enable(INTC_ID_INTRCAN1REC);
}

/******************************************************************************
* Function Name: can_ch1_tx_isr
* Description  : This function is executed when the INTCAN1TRX interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_ch1_tx_isr(uint32_t int_sense)
{
    uint32_t        i;
    can_tmstsp_t    *tmsts_regs = (can_tmstsp_t *)&RSCAN0TMSTS16;


    R_INTC_Disable(INTC_ID_INTRCAN1TRX);

    g_ccb[1].p_chcallback(CAN_CH1, CAN_EVT_TRANSMIT, NULL);

    /* reset mailbox transmit complete or abort flags */
    for (i=0; i < 16; i++)
    {
        tmsts_regs[i].BIT.TMTRF = 0;
    }

    /* reset TX and GW FIFO threshold flags */
    RSCAN0CFSTS3 &= 0xFFFFFFEF;    // WARNING! Must do 32 bit write!
    RSCAN0CFSTS4 &= 0xFFFFFFEF;    // WARNING! Must do 32 bit write!
    RSCAN0CFSTS5 &= 0xFFFFFFEF;    // WARNING! Must do 32 bit write!

    /* reset HIST FIFO threshold flag */
    RSCAN0THLSTS1 &= 0xFFFFFFEF;

    R_INTC_Enable(INTC_ID_INTRCAN1TRX);
}

/******************************************************************************
* Function Name: can_ch2_error_isr
* Description  : This function is executed when the INTCAN2ERR interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_ch2_error_isr(uint32_t int_sense)
{
    R_INTC_Disable(INTC_ID_INTRCAN2ERR);

    g_ccb[2].p_chcallback(CAN_CH2, CAN_EVT_CHANNEL_ERR, NULL);

    /* reset interrupt flags */
    RSCAN0C2ERFL = 0;

    R_INTC_Enable(INTC_ID_INTRCAN2ERR);
}

/******************************************************************************
* Function Name: can_ch2_gwfifo_rx_isr
* Description  : This function is executed when the INTCAN2REC interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_ch2_gwfifo_rx_isr(uint32_t int_sense)
{
    R_INTC_Disable(INTC_ID_INTRCAN2REC);

    g_ccb[2].p_chcallback(CAN_CH2, CAN_EVT_GWFIFO_RX_THRESHOLD, NULL);

    /* reset interrupt flag */
    RSCAN0CFSTS8 &= 0xFFFFFFF7;

    R_INTC_Enable(INTC_ID_INTRCAN2REC);
}

/******************************************************************************
* Function Name: can_ch2_tx_isr
* Description  : This function is executed when the INTCAN2TRX interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_ch2_tx_isr(uint32_t int_sense)
{
    uint32_t        i;
    can_tmstsp_t    *tmsts_regs = (can_tmstsp_t *)&RSCAN0TMSTS32;


    R_INTC_Disable(INTC_ID_INTRCAN2TRX);

    g_ccb[2].p_chcallback(CAN_CH2, CAN_EVT_TRANSMIT, NULL);

    /* reset mailbox transmit complete or abort flags */
    for (i=0; i < 16; i++)
    {
        tmsts_regs[i].BIT.TMTRF = 0;
    }

    /* reset TX and GW FIFO threshold flags */
    RSCAN0CFSTS6 &= 0xFFFFFFEF;    // WARNING! Must do 32 bit write!
    RSCAN0CFSTS7 &= 0xFFFFFFEF;    // WARNING! Must do 32 bit write!
    RSCAN0CFSTS8 &= 0xFFFFFFEF;    // WARNING! Must do 32 bit write!

    /* reset HIST FIFO threshold flag */
    RSCAN0THLSTS2 &= 0xFFFFFFEF;

    R_INTC_Enable(INTC_ID_INTRCAN2TRX);
}

/******************************************************************************
* Function Name: can_ch3_error_isr
* Description  : This function is executed when the INTCAN3ERR interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_ch3_error_isr(uint32_t int_sense)
{
    R_INTC_Disable(INTC_ID_INTRCAN3ERR);

    g_ccb[3].p_chcallback(CAN_CH3, CAN_EVT_CHANNEL_ERR, NULL);

    /* reset interrupt flags */
    RSCAN0C3ERFL = 0;

    R_INTC_Enable(INTC_ID_INTRCAN3ERR);
}

/******************************************************************************
* Function Name: can_ch3_gwfifo_rx_isr
* Description  : This function is executed when the INTCAN3REC interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_ch3_gwfifo_rx_isr(uint32_t int_sense)
{
    R_INTC_Disable(INTC_ID_INTRCAN3REC);

    g_ccb[3].p_chcallback(CAN_CH3, CAN_EVT_GWFIFO_RX_THRESHOLD, NULL);

    /* reset interrupt flag */
    RSCAN0CFSTS11 &= 0xFFFFFFF7;

R_INTC_Enable(INTC_ID_INTRCAN3REC);
}

/******************************************************************************
* Function Name: can_ch3_tx_isr
* Description  : This function is executed when the INTCAN3TRX interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_ch3_tx_isr(uint32_t int_sense)
{
    uint32_t        i;
    can_tmstsp_t    *tmsts_regs = (can_tmstsp_t *)&RSCAN0TMSTS48;

    R_INTC_Disable(INTC_ID_INTRCAN3TRX);

    g_ccb[3].p_chcallback(CAN_CH3, CAN_EVT_TRANSMIT, NULL);

    /* reset mailbox transmit complete or abort flags */
    for (i=0; i < 16; i++)
    {
        tmsts_regs[i].BIT.TMTRF = 0;
    }

    /* reset TX and GW FIFO threshold flags */
    RSCAN0CFSTS9 &= 0xFFFFFFEF;    // WARNING! Must do 32 bit write!
    RSCAN0CFSTS10 &= 0xFFFFFFEF;   // WARNING! Must do 32 bit write!
    RSCAN0CFSTS11 &= 0xFFFFFFEF;   // WARNING! Must do 32 bit write!

    /* reset HIST FIFO threshold flag */
    RSCAN0THLSTS3 &= 0xFFFFFFEF;

    R_INTC_Enable(INTC_ID_INTRCAN3TRX);
}

/******************************************************************************
* Function Name: can_ch4_error_isr
* Description  : This function is executed when the INTCAN4ERR interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_ch4_error_isr(uint32_t int_sense)
{
    R_INTC_Disable(INTC_ID_INTRCAN4ERR);

    g_ccb[4].p_chcallback(CAN_CH4, CAN_EVT_CHANNEL_ERR, NULL);

    /* reset interrupt flags */
    RSCAN0C4ERFL = 0;

    R_INTC_Enable(INTC_ID_INTRCAN4ERR);
}

/******************************************************************************
* Function Name: can_ch4_gwfifo_rx_isr
* Description  : This function is executed when the INTCAN4REC interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_ch4_gwfifo_rx_isr(uint32_t int_sense)
{
    R_INTC_Disable(INTC_ID_INTRCAN4REC);

    g_ccb[4].p_chcallback(CAN_CH4, CAN_EVT_GWFIFO_RX_THRESHOLD, NULL);

    /* reset interrupt flag */
    RSCAN0CFSTS14 &= 0xFFFFFFF7;

    R_INTC_Enable(INTC_ID_INTRCAN4REC);
}

/******************************************************************************
* Function Name: can_ch4_tx_isr
* Description  : This function is executed when the INTCAN4TRX interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void can_ch4_tx_isr(uint32_t int_sense)
{
    uint32_t        i;
    can_tmstsp_t    *tmsts_regs = (can_tmstsp_t *)&RSCAN0TMSTS64;

    R_INTC_Disable(INTC_ID_INTRCAN4TRX);

    g_ccb[4].p_chcallback(CAN_CH4, CAN_EVT_TRANSMIT, NULL);

    /* reset mailbox transmit complete or abort flags */
    for (i=0; i < 16; i++)
    {
        tmsts_regs[i].BIT.TMTRF = 0;
    }

    /* reset TX and GW FIFO threshold flags */
    RSCAN0CFSTS12 &= 0xFFFFFFEF;    // WARNING! Must do 32 bit write!
    RSCAN0CFSTS13 &= 0xFFFFFFEF;    // WARNING! Must do 32 bit write!
    RSCAN0CFSTS14 &= 0xFFFFFFEF;    // WARNING! Must do 32 bit write!

    /* reset HIST FIFO threshold flag */
    RSCAN0THLSTS4 &= 0xFFFFFFEF;

    R_INTC_Enable(INTC_ID_INTRCAN4TRX);
}
