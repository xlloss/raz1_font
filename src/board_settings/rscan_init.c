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
* File Name    : rscan_init.c
* Description  : Demo program for the CAN driver on the RSK+RZA1H board.
*
*                This program requires the connection of a CAN device (such as a sniffer) on channel 1 capable of
*                receiving and sending messages. This program spins in a loop sending then receiving one message
*                at a time. The messages received must have an ID of 0x60-0x6F and contain at least 4 bytes of data.
*                The baud rate is set to 125Kbps.
*
*                This program can run using either mailboxes without interrupts or FIFOs with interrupts. The
*                desired operation is configured by changing the value of USE_FIFOS to 0 (mailboxes) or 1 (FIFOs).
*
*                Be sure that your RSK board has 0-ohm resistors in the following locations:
*                R104 (not R105) and R206 (not R207)
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           16.01.2017 1.00    Initial Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <stdio.h>
#include "iodefine.h"
/* can driver */
#include "r_rscan_rz_if.h"
#include "r_rscan_rz_config.h"
/* intc driver */
#include "intc.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#define USE_FIFOS   0

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

static void can_init_ports(void);
static void RSCANCallback(can_cb_evt_t event, void *p_args);
static void RSCANChanCallback(uint8_t       chan,
                              can_cb_evt_t  event,
                              void          *p_args);

static uint32_t        g_rxcnt = 0, g_ovfl_cnt = 0;
static can_filter_t    g_filter;
static can_txmsg_t     g_txmsg;
static can_rxmsg_t     g_rxmsg;
static can_history_t   g_entry;


void RSCAN_Init(void)
{
    can_err_t       err;
    can_cfg_t       config;
    can_bitrate_t   baud;


    /* Configure CAN ports for channels 1 and 2 */
    can_init_ports();


    /* Open driver. Timestamp source is arbitrary. */
    config.timestamp_src = CAN_TIMESTAMP_SRC_CH1_BIT_CLK;
    config.timestamp_div = CAN_TIMESTAMP_DIV_1024;
    err = R_CAN_Open(&config, RSCANCallback);
    if (err != CAN_SUCCESS)
    {
        while(1) ;
    }

    /* Initialize channel 1 (includes mailboxes) */
    baud.prescaler = CAN_RSK_13MHZXTAL_500KBPS_PRESCALER;
    baud.tseg1 = CAN_RSK_13MHZXTAL_500KBPS_TSEG1;
    baud.tseg2 = CAN_RSK_13MHZXTAL_500KBPS_TSEG2;
    baud.sjw = CAN_RSK_13MHZXTAL_500KBPS_SJW;
    
    err = R_CAN_InitChan(CAN_CH1, &baud, RSCANChanCallback);
    if (err != CAN_SUCCESS)
    {
        while(1) ;
    }

    /*
     * Initialize a receive FIFO (optional)
     * Interrupt occurs only if CAN_CFG_INT_RXFIFO_THRESHOLD is set to 1 in r_can_rz_config.h.
     * Interrupt calls main callback function (from Open()) with CAN_EVT_RXFIFO_THRESHOLD.
     * No messages appear in FIFO unless a receive rule specifies this as a destination.
     */
    err = R_CAN_ConfigFIFO(CAN_BOX_RXFIFO_0,
                           CAN_FIFO_THRESHOLD_1,    // set int flag at every message received
                           0);                      // unused field for RXFIFO
    if (err != CAN_SUCCESS)
    {
        while(1) ;
    }

    /*
     * Initialize a transmit FIFO (optional)
     * Interrupt occurs only if CAN_CFG_INT_TXFIFO_THRESHOLD is set to 1 in r_can_rz_config.h.
     * Interrupt calls channel callback function (from InitChan()) with CAN_EVT_TRANSMIT.
     * Messages cannot be written to the linked transmit mailbox.
     */
    err = R_CAN_ConfigFIFO(CAN_BOX_CH1_TXFIFO_0,
                           CAN_FIFO_THRESHOLD_4,    // set int flag when 4 messages remain in FIFO
                           CAN_BOX_CH1_TXMBX_3);    // tx mailbox linked with FIFO
    if (err != CAN_SUCCESS)
    {
        while(1) ;
    }

    /*
     * Initialize a history FIFO (optional)
     * Interrupt occurs only if CAN_CFG_INT_HIST_FIFO_THRESHOLD is set to 1 in config.h.
     * Interrupt calls channel callback function (from InitChan()) with CAN_EVT_TRANSMIT.
     */
    err = R_CAN_ConfigFIFO(CAN_BOX_CH1_HIST_FIFO,
                           CAN_FIFO_THRESHOLD_12,   // set int flag when 12 entries are in FIFO
                           0);                      // unused field for HIST_FIFO
    if (err != CAN_SUCCESS)
    {
        while(1) ;
    }
#if 0		//org
    /* Initialize a receive rule filter */
    g_filter.check_ide = TRUE;    // check the IDE field in message
    g_filter.ide = 0;             // 0 = 11-bit ID
    g_filter.check_rtr = FALSE;   // do not check the RTR field in message
    g_filter.rtr = 0;             // (value does not matter here; not checking)
    g_filter.id = 0x060;          // message ID
    g_filter.id_mask = 0x7F0;     // messages with IDs of 0x060-0x06F are accepted
    g_filter.min_dlc = 4;         // message data must be at least four bytes long
    g_filter.label = 0x800;       // arbitrary label applied to messages of this type
#else			//for OBD2
    /* Initialize a receive rule filter */
    g_filter.check_ide = TRUE;    // check the IDE field in message
    g_filter.ide = 0;             // 0 = 11-bit ID
    g_filter.check_rtr = FALSE;   // do not check the RTR field in message
    g_filter.rtr = 0;             // (value does not matter here; not checking)
    g_filter.id = 0x7E0;          // message ID
    g_filter.id_mask = 0x7F0;     // messages with IDs of 0x7E0-0x7EF are accepted
    g_filter.min_dlc = 8;         // message data must be at least four bytes long
    g_filter.label = 0x800;       // arbitrary label applied to messages of this type
#endif

 
#if (USE_FIFOS == 1)
    /* Add rule to channel 1. Route filtered messages to receive FIFO 0 */
		err = R_CAN_AddRxRule(CAN_CH1, &g_filter, CAN_BOX_RXFIFO_0);
    if (err != CAN_SUCCESS)
    {
        while(1) ;
    }
#else
    /* Add rule to channel 1. Route filtered messages to receive mailbox 4. */
    err = R_CAN_AddRxRule(CAN_CH1, &g_filter, CAN_BOX_RXMBX_4);
    if (err != CAN_SUCCESS)
    {
        while(1) ;
    }
#endif
 
    /* Initialization complete. Go to communications mode */
    err = R_CAN_Control(CAN_CMD_SET_MODE_COMM, 0);
    if (err != CAN_SUCCESS)
    {
        while(1) ;
    }

    /* Setup a message to transmit */
#if 0
    g_txmsg.ide = 0;
    g_txmsg.rtr = 0;
    g_txmsg.id = 0x61;
    g_txmsg.dlc = 8;
    g_txmsg.data[0] = 1;
    g_txmsg.data[1] = 2;
    g_txmsg.data[2] = 3;
    g_txmsg.data[3] = 4;
    g_txmsg.data[4] = 5;
    g_txmsg.data[5] = 6;
    g_txmsg.data[6] = 7;
    g_txmsg.data[7] = 8;
    g_txmsg.one_shot = FALSE;       // do normal retries when sending
    g_txmsg.log_history = TRUE;     // log this message in history FIFO
    g_txmsg.label = 0x55;           // label stored in history FIFO

#if (USE_FIFOS == 1)
    /* USE FIFOS */
    /* SEND MESSAGE */
    err = R_CAN_SendMsg(CAN_BOX_CH1_TXFIFO_0, &g_txmsg);
    if (err != CAN_SUCCESS)
    {
        while(1) ;
    }
#else
    /* USE MAILBOXES */
    /* SEND MESSAGE */
    err = R_CAN_SendMsg(CAN_BOX_CH1_TXMBX_4, &g_txmsg);
	if (err != CAN_SUCCESS)
	{
		while(1) ;
	}
#endif

#endif

}


/******************************************************************************
* Function Name: can_init_ports
* Description  : Initializes RZ ports for CAN channels 1 and 2.
*
* NOTES: 0-ohm resistor locations:
*        channel 1: R104 (not R105) and R206 (not R207)
*        channel 2: R18 (not R4) and R174 (not R173)
*
* Arguments    : None
* Return Value : None
*******************************************************************************/
static void can_init_ports(void)
{
    /* P5_10 (see Table 54.21: 5th alternative mode, output function): CTX1# */

    GPIO.PIBC5   &= (~0x400u);  // Port Input Buffer Control (0 = disabled)
    GPIO.PBDC5   &= (~0x400u);  // Port Bidirection Control (0 = disabled)

    GPIO.PMC5    |= 0x400u;     // 5th alternative mode (see Table 54.6)
    GPIO.PFCAE5  |= 0x400u;     // 5th alternative mode (see Table 54.6)
    GPIO.PFCE5   &= (~0x400u);  // 5th alternative mode (see Table 54.6)
    GPIO.PFC5    &= (~0x400u);  // 5th alternative mode (see Table 54.6)
    GPIO.PIPC5   |= 0x400u;     // direction controlled by function (not PMn.PMnm)
    GPIO.PM5     &= (~0x400u);  // Port Mode (0 = output)

    /* P5_9 (5th alternative mode, input function): CRX1# */

    GPIO.PIBC5   &= (~0x200u);
    GPIO.PBDC5   &= (~0x200u);

    GPIO.PMC5    |= 0x200u;
    GPIO.PFCAE5  |= 0x200u;
    GPIO.PFCE5   &= (~0x200u);
    GPIO.PFC5    &= (~0x200u);
    GPIO.PIPC5   |= 0x200u;
    GPIO.PM5     |= 0x200u;

    /*
     * 0–ohm Resistors: CAN CTX2    Remove R173 Fit R174
     *                  CAN CRX2    Remove R4   Fit R18
     */

    /* P7_3 (5th alternative mode, output function): CTX2# */
    GPIO.PIBC7  &= (~0x8u);
    GPIO.PBDC7  &= (~0x8u);

    GPIO.PMC7   |= 0x8u;
    GPIO.PFCAE7 |= 0x8u;
    GPIO.PFCE7  &= (~0x8u);
    GPIO.PFC7   &= (~0x8u);
    GPIO.PIPC7  |= 0x8u;
    GPIO.PM7    &= (~0x8u);

    /* P7_2 (5th alternative mode, input function): CRX2# */
    GPIO.PIBC7  &= (~0x4u);
    GPIO.PBDC7  &= (~0x4u);

    GPIO.PMC7   |= 0x4u;
    GPIO.PFCAE7 |= 0x4u;
    GPIO.PFCE7  &= (~0x4u);
    GPIO.PFC7   &= (~0x4u);
    GPIO.PIPC7  |= 0x4u;
    GPIO.PM7    |= 0x4u;
}


/******************************************************************************
* Function Name: RSCANCallback
* Description  : This function is called from interrupt level whenever a message
*                is received in a receive FIFO, or a global error occurs. Global
*                errors include FIFO overflows or a DLC error detection.
* Arguments    : event-
*                   CAN_EVT_RXFIFO_THRESHOLD when threshold messages in an RXFIFO
*                   CAN_EVT_GLOBAL_ERR when a global error is detected
*                p_args-
*                   unused (future)
* Return Value : None
*******************************************************************************/
static void RSCANCallback(can_cb_evt_t event, void *p_args)
{
    uint32_t    mask;
    can_err_t   err;


    if (event == CAN_EVT_RXFIFO_THRESHOLD)
    {
        /*
         * Determine which RXFIFO(s) received a message
         * Only are using 1 RXFIFO, so really do not need to get or check mask
         */
        mask = R_CAN_GetStatusMask(CAN_STAT_FIFO_THRESHOLD, CAN_ANY_CHAN, &err);
        if (mask & CAN_MASK_RXFIFO_0)
        {
            R_CAN_GetMsg(CAN_BOX_RXFIFO_0, &g_rxmsg);
            g_rxcnt++;
        }
    }

    else if (event == CAN_EVT_GLOBAL_ERR)   // Should never happen with this demo
    {
        /* Get mask of which global error(s) occurred */
        mask = R_CAN_GetStatusMask(CAN_STAT_GLOBAL_ERR, CAN_ANY_CHAN, &err);

        if (mask & CAN_MASK_ERR_DLC)
        {
            /* handle DLC error */
        }

        if (mask & CAN_MASK_ERR_FIFO_OVFL)
        {
            mask = R_CAN_GetStatusMask(CAN_STAT_FIFO_OVFL, CAN_ANY_CHAN, &err);

            /* check the RXFIFOs, GWFIFO, and HIST_FIFOs in use */
            if (mask & CAN_MASK_RXFIFO_0)
            {
                g_ovfl_cnt++;

                /* drain FIFO */
                while (R_CAN_GetMsg(CAN_BOX_RXFIFO_0, &g_rxmsg) == CAN_SUCCESS)
                {
                    ;
                }
            }

            if (mask & CAN_MASK_CH1_HIST_FIFO)
            {
                g_ovfl_cnt++;

                /* drain FIFO */
                while (R_CAN_GetHistoryEntry(CAN_BOX_CH1_HIST_FIFO, &g_entry) == CAN_SUCCESS)
                {
                    ;
                }
            }
        }
    }
}


/******************************************************************************
* Function Name: RSCANChanCallback
* Description  : This function is called from interrupt level whenever a channel
*                transmit, channel error, or gateway receive threshold event
*                occurs
* Arguments    : chan-
*                    Channel in which the interrupt occurred on
*                event-
*                    CAN_EVT_TRANSMIT for mailbox transmit complete, abort
*                           complete, or TX/GW/HIST FIFO at threshold
*                    CAN_EVT_CHANNEL_ERR for channel error detection, which
*                           includes (among others) error passive state entry,
*                           bus off entry, and bus off recovery.
*                    CAN_EVT_GWFIFO_RX_THRESHOLD for GW FIFO at receive
*                           threshhold
* Return Value : None
*******************************************************************************/
static void RSCANChanCallback(uint8_t       chan,
                              can_cb_evt_t  event,
                              void          *p_args)
{
    uint32_t   mask;
    can_err_t  err;


    /* NOTE: THESE EVENTS WILL NOT OCCUR IN THIS DEMO, BUT ARE PROVIDED AS A TEMPLATE */

    if (event == CAN_EVT_TRANSMIT)
    {
        /* determine if mailbox transmit completes */
        mask = R_CAN_GetStatusMask(CAN_STAT_CH_TXMBX_SENT, chan, &err);
        if (mask != 0)
        {
            /* only using one mailbox, do not have to check which one */
            // do something; typically send next message
            err=0;
        }

        /* determine if mailbox transmit abort completed */
        mask = R_CAN_GetStatusMask(CAN_STAT_CH_TXMBX_ABORTED, chan, &err);
        if (mask != 0)
        {
            /* only using one mailbox, do not have to check which one */
            // do something
        }

        /* determine if TX, GW, or HIST FIFOs at threshhold */
        mask = R_CAN_GetStatusMask(CAN_STAT_FIFO_THRESHOLD, CAN_ANY_CHAN, &err);
        if (mask & CAN_MASK_CH1_TXFIFO_0)
        {
            // do something; typically load more messages into TXFIFO for transmit
        }

        if (mask & CAN_MASK_CH1_HIST_FIFO)
        {
            // do something; typically read messages from HIST_FIFO
        }
    }


    else if (event == CAN_EVT_CHANNEL_ERR)
    {
        /* get mask of which errors detected */
        mask = R_CAN_GetStatusMask(CAN_STAT_CH_ERROR, chan, &err);

        /* check only for specific errors */
        if (mask & CAN_MASK_ERR_PASSIVE)
        {
            /* handle error */
        }

        if (mask & CAN_MASK_ERR_BUS_OFF_ENTRY)
        {
            /* handle error */
        }

        if (mask & CAN_MASK_ERR_BUS_OFF_EXIT)
        {
            /* handle recovery */
        }
    }

    else if (event == CAN_EVT_GWFIFO_RX_THRESHOLD)
    {
        /* read gateway FIFO message if desired */
    }

}

