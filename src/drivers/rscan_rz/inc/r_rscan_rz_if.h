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
* File Name    : r_rscan_rz_if.h
* Description  : This module provides the interface file to be included by the user when using the CAN driver module.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           22.03.2015 1.00    Initial Release
*           21.03.2016 1.01    Fixed FIFO bug.
*           04.04.2016 1.02    Fixed index bug.
***********************************************************************************************************************/

#ifndef CAN_INTERFACE_HEADER_FILE
#define CAN_INTERFACE_HEADER_FILE

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "iodefine.h"


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Version Number of API */
#define CAN_VERSION_MAJOR           (1)
#define CAN_VERSION_MINOR           (02)


/* Channel numbers */
#define CAN_CH0                     (0)
#define CAN_CH1                     (1)
#define CAN_CH2                     (2)
#define CAN_CH3                     (3)
#define CAN_CH4                     (4)
#define CAN_ANY_CHAN                (5)     // used for non-channel specific arguments

/* R_CAN_GetStatus() Masks */

/* CAN_STAT_CH_TXMBX_SENT, CAN_STAT_CH_TXMBX_ABORTED */
#define CAN_MASK_TXMBX_0            (0x0001)
#define CAN_MASK_TXMBX_1            (0x0002)
#define CAN_MASK_TXMBX_2            (0x0004)
#define CAN_MASK_TXMBX_3            (0x0008)
#define CAN_MASK_TXMBX_4            (0x0010)
#define CAN_MASK_TXMBX_5            (0x0020)
#define CAN_MASK_TXMBX_6            (0x0040)
#define CAN_MASK_TXMBX_7            (0x0080)
#define CAN_MASK_TXMBX_8            (0x0100)
#define CAN_MASK_TXMBX_9            (0x0200)
#define CAN_MASK_TXMBX_10           (0x0400)
#define CAN_MASK_TXMBX_11           (0x0800)
#define CAN_MASK_TXMBX_12           (0x1000)
#define CAN_MASK_TXMBX_13           (0x2000)
#define CAN_MASK_TXMBX_14           (0x4000)
#define CAN_MASK_TXMBX_15           (0x8000)

/* CAN_STAT_RXMBX_FULL */
#define CAN_MASK_RXMBX_0            (0x0001)
#define CAN_MASK_RXMBX_1            (0x0002)
#define CAN_MASK_RXMBX_2            (0x0004)
#define CAN_MASK_RXMBX_3            (0x0008)
#define CAN_MASK_RXMBX_4            (0x0010)
#define CAN_MASK_RXMBX_5            (0x0020)
#define CAN_MASK_RXMBX_6            (0x0040)
#define CAN_MASK_RXMBX_7            (0x0080)
#define CAN_MASK_RXMBX_8            (0x0100)
#define CAN_MASK_RXMBX_9            (0x0200)
#define CAN_MASK_RXMBX_10           (0x0400)
#define CAN_MASK_RXMBX_11           (0x0800)
#define CAN_MASK_RXMBX_12           (0x1000)
#define CAN_MASK_RXMBX_13           (0x2000)
#define CAN_MASK_RXMBX_14           (0x4000)
#define CAN_MASK_RXMBX_15           (0x8000)

/* CAN_STAT_FIFO_EMPTY, CAN_STAT_FIFO_THRESHOLD, CAN_STAT_FIFO_OVFL */
#define CAN_MASK_RXFIFO_0           (0x00000001)
#define CAN_MASK_RXFIFO_1           (0x00000002)
#define CAN_MASK_RXFIFO_2           (0x00000004)
#define CAN_MASK_RXFIFO_3           (0x00000008)
#define CAN_MASK_RXFIFO_4           (0x00000010)
#define CAN_MASK_RXFIFO_5           (0x00000020)
#define CAN_MASK_RXFIFO_6           (0x00000040)
#define CAN_MASK_RXFIFO_7           (0x00000080)
#define CAN_MASK_CH0_TXFIFO_0       (0x00000100)
#define CAN_MASK_CH0_TXFIFO_1       (0x00000200)
#define CAN_MASK_CH0_GWFIFO         (0x00000400)
#define CAN_MASK_CH1_TXFIFO_0       (0x00000800)
#define CAN_MASK_CH1_TXFIFO_1       (0x00001000)
#define CAN_MASK_CH1_GWFIFO         (0x00002000)
#define CAN_MASK_CH2_TXFIFO_0       (0x00004000)
#define CAN_MASK_CH2_TXFIFO_1       (0x00008000)
#define CAN_MASK_CH2_GWFIFO         (0x00010000)
#define CAN_MASK_CH3_TXFIFO_0       (0x00020000)
#define CAN_MASK_CH3_TXFIFO_1       (0x00040000)
#define CAN_MASK_CH3_GWFIFO         (0x00080000)
#define CAN_MASK_CH4_TXFIFO_0       (0x00100000)
#define CAN_MASK_CH4_TXFIFO_1       (0x00200000)
#define CAN_MASK_CH4_GWFIFO         (0x00400000)
#define CAN_MASK_CH0_HIST_FIFO      (0x00800000)
#define CAN_MASK_CH1_HIST_FIFO      (0x01000000)
#define CAN_MASK_CH2_HIST_FIFO      (0x02000000)
#define CAN_MASK_CH3_HIST_FIFO      (0x04000000)
#define CAN_MASK_CH4_HIST_FIFO      (0x08000000)

/* CAN_STAT_GLOBAL_ERR */
#define CAN_MASK_ERR_DLC            (0x0001)
#define CAN_MASK_ERR_GW_RX_OVFL     (0x0002)
#define CAN_MASK_ERR_HIST_OVFL      (0x0004)
#define CAN_MASK_ERR_FIFO_OVFL      (0x0006)

/* CAN_STAT_CH_ERROR */
#define CAN_MASK_ERR_PROTOCOL       (0x0001)
#define CAN_MASK_ERR_WARNING        (0x0002)
#define CAN_MASK_ERR_PASSIVE        (0x0004)
#define CAN_MASK_ERR_BUS_OFF_ENTRY  (0x0008)
#define CAN_MASK_ERR_BUS_OFF_EXIT   (0x0010)
#define CAN_MASK_ERR_OVERLOAD       (0x0020)
#define CAN_MASK_ERR_DOMINANT_LOCK  (0x0040)
#define CAN_MASK_ERR_ARB_LOST       (0x0080)
#define CAN_MASK_ERR_STUFF          (0x0100)
#define CAN_MASK_ERR_FORM           (0x0200)
#define CAN_MASK_ERR_ACK            (0x0400)
#define CAN_MASK_ERR_CRC            (0x0800)
#define CAN_MASK_ERR_RECESSIVE_BIT  (0x1000)
#define CAN_MASK_ERR_DOMINANT_BIT   (0x2000)
#define CAN_MASK_ERR_ACK_DELIMITER  (0x4000)


#define TRUE        (1)
#define FALSE       (0)
#define CAN_UNUSED  (0)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/


/* Mailbox and FIFO box IDs */

#define CAN_FLG_TXMBX           (0x80000000)
#define CAN_FLG_RXMBX           (0x40000000)
#define CAN_FLG_FIFO            (0x20000000)

typedef enum e_can_box
{
    CAN_BOX_NONE              = 0,

    CAN_BOX_CH0_TXMBX_0       = (CAN_FLG_TXMBX | 0),
    CAN_BOX_CH0_TXMBX_1       = (CAN_FLG_TXMBX | 1),
    CAN_BOX_CH0_TXMBX_2       = (CAN_FLG_TXMBX | 2),
    CAN_BOX_CH0_TXMBX_3       = (CAN_FLG_TXMBX | 3),
    CAN_BOX_CH0_TXMBX_4       = (CAN_FLG_TXMBX | 4),
    CAN_BOX_CH0_TXMBX_5       = (CAN_FLG_TXMBX | 5),
    CAN_BOX_CH0_TXMBX_6       = (CAN_FLG_TXMBX | 6),
    CAN_BOX_CH0_TXMBX_7       = (CAN_FLG_TXMBX | 7),
    CAN_BOX_CH0_TXMBX_8       = (CAN_FLG_TXMBX | 8),
    CAN_BOX_CH0_TXMBX_9       = (CAN_FLG_TXMBX | 9),
    CAN_BOX_CH0_TXMBX_10      = (CAN_FLG_TXMBX | 10),
    CAN_BOX_CH0_TXMBX_11      = (CAN_FLG_TXMBX | 11),
    CAN_BOX_CH0_TXMBX_12      = (CAN_FLG_TXMBX | 12),
    CAN_BOX_CH0_TXMBX_13      = (CAN_FLG_TXMBX | 13),
    CAN_BOX_CH0_TXMBX_14      = (CAN_FLG_TXMBX | 14),
    CAN_BOX_CH0_TXMBX_15      = (CAN_FLG_TXMBX | 15),

    CAN_BOX_CH1_TXMBX_0       = (CAN_FLG_TXMBX | 16),
    CAN_BOX_CH1_TXMBX_1       = (CAN_FLG_TXMBX | 17),
    CAN_BOX_CH1_TXMBX_2       = (CAN_FLG_TXMBX | 18),
    CAN_BOX_CH1_TXMBX_3       = (CAN_FLG_TXMBX | 19),
    CAN_BOX_CH1_TXMBX_4       = (CAN_FLG_TXMBX | 20),
    CAN_BOX_CH1_TXMBX_5       = (CAN_FLG_TXMBX | 21),
    CAN_BOX_CH1_TXMBX_6       = (CAN_FLG_TXMBX | 22),
    CAN_BOX_CH1_TXMBX_7       = (CAN_FLG_TXMBX | 23),
    CAN_BOX_CH1_TXMBX_8       = (CAN_FLG_TXMBX | 24),
    CAN_BOX_CH1_TXMBX_9       = (CAN_FLG_TXMBX | 25),
    CAN_BOX_CH1_TXMBX_10      = (CAN_FLG_TXMBX | 26),
    CAN_BOX_CH1_TXMBX_11      = (CAN_FLG_TXMBX | 27),
    CAN_BOX_CH1_TXMBX_12      = (CAN_FLG_TXMBX | 28),
    CAN_BOX_CH1_TXMBX_13      = (CAN_FLG_TXMBX | 29),
    CAN_BOX_CH1_TXMBX_14      = (CAN_FLG_TXMBX | 30),
    CAN_BOX_CH1_TXMBX_15      = (CAN_FLG_TXMBX | 31),

    CAN_BOX_CH2_TXMBX_0       = (CAN_FLG_TXMBX | 32),
    CAN_BOX_CH2_TXMBX_1       = (CAN_FLG_TXMBX | 33),
    CAN_BOX_CH2_TXMBX_2       = (CAN_FLG_TXMBX | 34),
    CAN_BOX_CH2_TXMBX_3       = (CAN_FLG_TXMBX | 35),
    CAN_BOX_CH2_TXMBX_4       = (CAN_FLG_TXMBX | 36),
    CAN_BOX_CH2_TXMBX_5       = (CAN_FLG_TXMBX | 37),
    CAN_BOX_CH2_TXMBX_6       = (CAN_FLG_TXMBX | 38),
    CAN_BOX_CH2_TXMBX_7       = (CAN_FLG_TXMBX | 39),
    CAN_BOX_CH2_TXMBX_8       = (CAN_FLG_TXMBX | 40),
    CAN_BOX_CH2_TXMBX_9       = (CAN_FLG_TXMBX | 41),
    CAN_BOX_CH2_TXMBX_10      = (CAN_FLG_TXMBX | 42),
    CAN_BOX_CH2_TXMBX_11      = (CAN_FLG_TXMBX | 43),
    CAN_BOX_CH2_TXMBX_12      = (CAN_FLG_TXMBX | 44),
    CAN_BOX_CH2_TXMBX_13      = (CAN_FLG_TXMBX | 45),
    CAN_BOX_CH2_TXMBX_14      = (CAN_FLG_TXMBX | 46),
    CAN_BOX_CH2_TXMBX_15      = (CAN_FLG_TXMBX | 47),

    CAN_BOX_CH3_TXMBX_0       = (CAN_FLG_TXMBX | 48),
    CAN_BOX_CH3_TXMBX_1       = (CAN_FLG_TXMBX | 49),
    CAN_BOX_CH3_TXMBX_2       = (CAN_FLG_TXMBX | 50),
    CAN_BOX_CH3_TXMBX_3       = (CAN_FLG_TXMBX | 51),
    CAN_BOX_CH3_TXMBX_4       = (CAN_FLG_TXMBX | 52),
    CAN_BOX_CH3_TXMBX_5       = (CAN_FLG_TXMBX | 53),
    CAN_BOX_CH3_TXMBX_6       = (CAN_FLG_TXMBX | 54),
    CAN_BOX_CH3_TXMBX_7       = (CAN_FLG_TXMBX | 55),
    CAN_BOX_CH3_TXMBX_8       = (CAN_FLG_TXMBX | 56),
    CAN_BOX_CH3_TXMBX_9       = (CAN_FLG_TXMBX | 57),
    CAN_BOX_CH3_TXMBX_10      = (CAN_FLG_TXMBX | 58),
    CAN_BOX_CH3_TXMBX_11      = (CAN_FLG_TXMBX | 59),
    CAN_BOX_CH3_TXMBX_12      = (CAN_FLG_TXMBX | 60),
    CAN_BOX_CH3_TXMBX_13      = (CAN_FLG_TXMBX | 61),
    CAN_BOX_CH3_TXMBX_14      = (CAN_FLG_TXMBX | 62),
    CAN_BOX_CH3_TXMBX_15      = (CAN_FLG_TXMBX | 63),

    CAN_BOX_CH4_TXMBX_0       = (CAN_FLG_TXMBX | 64),
    CAN_BOX_CH4_TXMBX_1       = (CAN_FLG_TXMBX | 65),
    CAN_BOX_CH4_TXMBX_2       = (CAN_FLG_TXMBX | 66),
    CAN_BOX_CH4_TXMBX_3       = (CAN_FLG_TXMBX | 67),
    CAN_BOX_CH4_TXMBX_4       = (CAN_FLG_TXMBX | 68),
    CAN_BOX_CH4_TXMBX_5       = (CAN_FLG_TXMBX | 69),
    CAN_BOX_CH4_TXMBX_6       = (CAN_FLG_TXMBX | 70),
    CAN_BOX_CH4_TXMBX_7       = (CAN_FLG_TXMBX | 71),
    CAN_BOX_CH4_TXMBX_8       = (CAN_FLG_TXMBX | 72),
    CAN_BOX_CH4_TXMBX_9       = (CAN_FLG_TXMBX | 73),
    CAN_BOX_CH4_TXMBX_10      = (CAN_FLG_TXMBX | 74),
    CAN_BOX_CH4_TXMBX_11      = (CAN_FLG_TXMBX | 75),
    CAN_BOX_CH4_TXMBX_12      = (CAN_FLG_TXMBX | 76),
    CAN_BOX_CH4_TXMBX_13      = (CAN_FLG_TXMBX | 77),
    CAN_BOX_CH4_TXMBX_14      = (CAN_FLG_TXMBX | 78),
    CAN_BOX_CH4_TXMBX_15      = (CAN_FLG_TXMBX | 79),

    CAN_BOX_RXMBX_0           = (CAN_FLG_RXMBX | 0),
    CAN_BOX_RXMBX_1           = (CAN_FLG_RXMBX | 1),
    CAN_BOX_RXMBX_2           = (CAN_FLG_RXMBX | 2),
    CAN_BOX_RXMBX_3           = (CAN_FLG_RXMBX | 3),
    CAN_BOX_RXMBX_4           = (CAN_FLG_RXMBX | 4),
    CAN_BOX_RXMBX_5           = (CAN_FLG_RXMBX | 5),
    CAN_BOX_RXMBX_6           = (CAN_FLG_RXMBX | 6),
    CAN_BOX_RXMBX_7           = (CAN_FLG_RXMBX | 7),
    CAN_BOX_RXMBX_8           = (CAN_FLG_RXMBX | 8),
    CAN_BOX_RXMBX_9           = (CAN_FLG_RXMBX | 9),
    CAN_BOX_RXMBX_10          = (CAN_FLG_RXMBX | 10),
    CAN_BOX_RXMBX_11          = (CAN_FLG_RXMBX | 11),
    CAN_BOX_RXMBX_12          = (CAN_FLG_RXMBX | 12),
    CAN_BOX_RXMBX_13          = (CAN_FLG_RXMBX | 13),
    CAN_BOX_RXMBX_14          = (CAN_FLG_RXMBX | 14),
    CAN_BOX_RXMBX_15          = (CAN_FLG_RXMBX | 15),

    CAN_BOX_RXFIFO_0          = (CAN_FLG_FIFO | CAN_MASK_RXFIFO_0),
    CAN_BOX_RXFIFO_1          = (CAN_FLG_FIFO | CAN_MASK_RXFIFO_1),
    CAN_BOX_RXFIFO_2          = (CAN_FLG_FIFO | CAN_MASK_RXFIFO_2),
    CAN_BOX_RXFIFO_3          = (CAN_FLG_FIFO | CAN_MASK_RXFIFO_3),
    CAN_BOX_RXFIFO_4          = (CAN_FLG_FIFO | CAN_MASK_RXFIFO_4),
    CAN_BOX_RXFIFO_5          = (CAN_FLG_FIFO | CAN_MASK_RXFIFO_5),
    CAN_BOX_RXFIFO_6          = (CAN_FLG_FIFO | CAN_MASK_RXFIFO_6),
    CAN_BOX_RXFIFO_7          = (CAN_FLG_FIFO | CAN_MASK_RXFIFO_7),

    CAN_BOX_CH0_TXFIFO_0      = (CAN_FLG_FIFO | CAN_MASK_CH0_TXFIFO_0),
    CAN_BOX_CH0_TXFIFO_1      = (CAN_FLG_FIFO | CAN_MASK_CH0_TXFIFO_1),
    CAN_BOX_CH0_GWFIFO        = (CAN_FLG_FIFO | CAN_MASK_CH0_GWFIFO),
    CAN_BOX_CH1_TXFIFO_0      = (CAN_FLG_FIFO | CAN_MASK_CH1_TXFIFO_0),
    CAN_BOX_CH1_TXFIFO_1      = (CAN_FLG_FIFO | CAN_MASK_CH1_TXFIFO_1),
    CAN_BOX_CH1_GWFIFO        = (CAN_FLG_FIFO | CAN_MASK_CH1_GWFIFO),
    CAN_BOX_CH2_TXFIFO_0      = (CAN_FLG_FIFO | CAN_MASK_CH2_TXFIFO_0),
    CAN_BOX_CH2_TXFIFO_1      = (CAN_FLG_FIFO | CAN_MASK_CH2_TXFIFO_1),
    CAN_BOX_CH2_GWFIFO        = (CAN_FLG_FIFO | CAN_MASK_CH2_GWFIFO),
    CAN_BOX_CH3_TXFIFO_0      = (CAN_FLG_FIFO | CAN_MASK_CH3_TXFIFO_0),
    CAN_BOX_CH3_TXFIFO_1      = (CAN_FLG_FIFO | CAN_MASK_CH3_TXFIFO_1),
    CAN_BOX_CH3_GWFIFO        = (CAN_FLG_FIFO | CAN_MASK_CH3_GWFIFO),
    CAN_BOX_CH4_TXFIFO_0      = (CAN_FLG_FIFO | CAN_MASK_CH4_TXFIFO_0),
    CAN_BOX_CH4_TXFIFO_1      = (CAN_FLG_FIFO | CAN_MASK_CH4_TXFIFO_1),
    CAN_BOX_CH4_GWFIFO        = (CAN_FLG_FIFO | CAN_MASK_CH4_GWFIFO),

    CAN_BOX_CH0_HIST_FIFO     = (CAN_FLG_FIFO | CAN_MASK_CH0_HIST_FIFO),
    CAN_BOX_CH1_HIST_FIFO     = (CAN_FLG_FIFO | CAN_MASK_CH1_HIST_FIFO),
    CAN_BOX_CH2_HIST_FIFO     = (CAN_FLG_FIFO | CAN_MASK_CH2_HIST_FIFO),
    CAN_BOX_CH3_HIST_FIFO     = (CAN_FLG_FIFO | CAN_MASK_CH3_HIST_FIFO),
    CAN_BOX_CH4_HIST_FIFO     = (CAN_FLG_FIFO | CAN_MASK_CH4_HIST_FIFO)
} can_box_t;


/* Callback Function Events */

typedef enum e_can_cb_evt           // callback function events
{
    // Main Callback Events
    CAN_EVT_RXFIFO_THRESHOLD,       // RX FIFO threshold
    CAN_EVT_GLOBAL_ERR,             // RX, GW, or History FIFO overflow, or DLC error

    // Channel Callback Events
    CAN_EVT_TRANSMIT,               // mbx tx complete or aborted, tx or history FIFO threshold
    CAN_EVT_GWFIFO_RX_THRESHOLD,    // GW FIFO rx threshold
    CAN_EVT_CHANNEL_ERR,
} can_cb_evt_t;


/* API Error Codes */

typedef enum e_can_err          // CAN API error codes
{
    CAN_SUCCESS=0,
    CAN_ERR_OPENED,             // Call to Open already made
    CAN_ERR_NOT_OPENED,         // Call to Open not yet made
    CAN_ERR_INIT_DONE,          // Call to InitChan already made for specified channel
    CAN_ERR_CH_NO_INIT,         // Channel not initialized
    CAN_ERR_INVALID_ARG,        // Invalid argument passed to function
    CAN_ERR_MISSING_CALLBACK,   // Callback function not provided and interrupts requested
    CAN_ERR_MAX_ONE_GWFIFO,     // Can only configure one GWFIFO
    CAN_ERR_MAX_RULES,          // Max configured rules already present
                                // (as specified in r_can_rz_config.h)
    CAN_ERR_BOX_FULL,           // Transmit mailbox or FIFO is full
    CAN_ERR_BOX_EMPTY,          // Receive mailbox or FIFO is full
    CAN_ERR_ILLEGAL_MODE        // Not in proper mode for request
} can_err_t;


/* R_CAN_Open() */

typedef enum e_can_timestamp_src
{
    CAN_TIMESTAMP_SRC_CH0_BIT_CLK = 0,
    CAN_TIMESTAMP_SRC_CH1_BIT_CLK = 1,
    CAN_TIMESTAMP_SRC_CH2_BIT_CLK = 2,
    CAN_TIMESTAMP_SRC_CH3_BIT_CLK = 3,
    CAN_TIMESTAMP_SRC_CH4_BIT_CLK = 4,
    CAN_TIMESTAMP_SRC_HALF_PCLK   = 5,
    CAN_TIMESTAMP_SRC_END_ENUM
} can_timestamp_src_t;

typedef enum e_can_timestamp_div
{
    CAN_TIMESTAMP_DIV_1     = 0,
    CAN_TIMESTAMP_DIV_2     = 1,
    CAN_TIMESTAMP_DIV_4     = 2,
    CAN_TIMESTAMP_DIV_8     = 3,
    CAN_TIMESTAMP_DIV_16    = 4,
    CAN_TIMESTAMP_DIV_32    = 5,
    CAN_TIMESTAMP_DIV_64    = 6,
    CAN_TIMESTAMP_DIV_128   = 7,
    CAN_TIMESTAMP_DIV_256   = 8,
    CAN_TIMESTAMP_DIV_512   = 9,
    CAN_TIMESTAMP_DIV_1024  = 10,
    CAN_TIMESTAMP_DIV_2048  = 11,
    CAN_TIMESTAMP_DIV_4096  = 12,
    CAN_TIMESTAMP_DIV_8192  = 13,
    CAN_TIMESTAMP_DIV_16384 = 14,
    CAN_TIMESTAMP_DIV_32768 = 15,
    CAN_TIMESTAMP_DIV_END_ENUM
} can_timestamp_div_t;

typedef struct st_can_cfg
{
    can_timestamp_src_t     timestamp_src;
    can_timestamp_div_t     timestamp_div;
} can_cfg_t;


/* R_CAN_InitChan() */

typedef struct st_can_bitrate
{
    uint16_t    prescaler;  // 1-1024
    uint8_t     tseg1;      // 4-16
    uint8_t     tseg2;      // 2-8
    uint8_t     sjw;        // 1-4
} can_bitrate_t;

/* Sample settings for 125kbps with 13.33MHz XTAL (1/2 pclk = 33.33MHz; .2% baud error) */
#define CAN_RSK_13MHZXTAL_125KBPS_PRESCALER     38
#define CAN_RSK_13MHZXTAL_125KBPS_TSEG1         4       // minimum
#define CAN_RSK_13MHZXTAL_125KBPS_TSEG2         2       // minimum
#define CAN_RSK_13MHZXTAL_125KBPS_SJW           1


/* Sample settings for 125kbps with 13.33MHz XTAL (1/2 pclk = 33.33MHz; .2% baud error) */
#define CAN_RSK_13MHZXTAL_500KBPS_PRESCALER     6
#define CAN_RSK_13MHZXTAL_500KBPS_TSEG1         8       // minimum
#define CAN_RSK_13MHZXTAL_500KBPS_TSEG2         2       // minimum
#define CAN_RSK_13MHZXTAL_500KBPS_SJW           1


/* Sample settings for 500kbps with 12.00MHz XTAL (1/2 pclk = 30MHz; 0% baud error) */
#define CAN_RSK_12MHZXTAL_500KBPS_PRESCALER     4
#define CAN_RSK_12MHZXTAL_500KBPS_TSEG1         11      // TSEG1 + TSEG2 + SJW = 15
#define CAN_RSK_12MHZXTAL_500KBPS_TSEG2         3
#define CAN_RSK_12MHZXTAL_500KBPS_SJW           1


/* R_CAN_ConfigFIFO() */

typedef enum e_can_fifo_threshold       /* NOTE: History FIFO can only have a threshold of 1 or 12 */
{
    CAN_FIFO_THRESHOLD_2    = 0,        // 1/8 of 16
    CAN_FIFO_THRESHOLD_4    = 1,        // 2/8 of 16
    CAN_FIFO_THRESHOLD_6    = 2,        // 3/8 of 16
    CAN_FIFO_THRESHOLD_8    = 3,        // 4/8 of 16
    CAN_FIFO_THRESHOLD_10   = 4,        // 5/8 of 16
    CAN_FIFO_THRESHOLD_12   = 5,        // 6/8 of 16
    CAN_FIFO_THRESHOLD_14   = 6,        // 7/8 of 16
    CAN_FIFO_THRESHOLD_FULL = 7,        // 8/8 of 16
    CAN_FIFO_THRESHOLD_1    = 8,        // every message
    CAN_FIFO_THRESHOLD_END_ENUM
} can_fifo_threshold_t;


/* R_CAN_AddRxRule() */

typedef struct st_can_filter
{
    bool_t      check_ide;
    uint8_t     ide;
    bool_t      check_rtr;
    uint8_t     rtr;
    uint32_t    id;
    uint32_t    id_mask;
    uint8_t     min_dlc;
    uint16_t    label;          // 12-bit label
} can_filter_t;


/* R_CAN_SendMsg() */

typedef struct st_can_txmsg
{
    uint8_t     ide;
    uint8_t     rtr;
    uint32_t    id;
    uint8_t     dlc;
    uint8_t     data[8];
    bool_t      one_shot;       // no retries on error; txmbx only
    bool_t      log_history;    // true if want to log
    uint8_t     label;          // 8-bit label for History FIFO
} can_txmsg_t;


/* R_CAN_GetMsg() */

typedef struct st_can_rxmsg
{
    uint8_t     ide;
    uint8_t     rtr;
    uint32_t    id;
    uint8_t     dlc;
    uint8_t     data[8];
    uint16_t    label;          // 12-bit label from receive rule
    uint16_t    timestamp;
} can_rxmsg_t;


/* R_CAN_GetHistoryEntry() */

typedef struct st_can_history
{
    can_box_t   box_id;         // box which sent message
    uint8_t     label;          // associated 8-bit label
} can_history_t;


/* R_CAN_GetStatusMask() (masks defined at top of file) */

typedef enum e_can_stat
{
    CAN_STAT_FIFO_EMPTY,
    CAN_STAT_FIFO_THRESHOLD,
    CAN_STAT_FIFO_OVFL,         // bits reset after reading
    CAN_STAT_RXMBX_FULL,
    CAN_STAT_GLOBAL_ERR,        // DLC error bit is reset after reading
    CAN_STAT_CH_TXMBX_SENT,     // bits reset after reading
    CAN_STAT_CH_TXMBX_ABORTED,  // bits reset after reading
    CAN_STAT_CH_ERROR,          // bits reset after reading
    CAN_STAT_END_ENUM
} can_stat_t;


/* R_CAN_GetCountErr() */

typedef enum e_can_count
{
    CAN_COUNT_RX_ERR,
    CAN_COUNT_TX_ERR,
    CAN_COUNT_END_ENUM
} can_count_t;


/* R_CAN_Control() */

typedef enum e_can_cmd
{
    CAN_CMD_ABORT_TX,                       // argument: transmit mailbox id
    CAN_CMD_RESET_TIMESTAMP,
    CAN_CMD_SET_MODE_COMM,                  // start normal bus communications
    CAN_CMD_SET_MODE_TST_STANDARD,
    CAN_CMD_SET_MODE_TST_LISTEN,
    CAN_CMD_SET_MODE_TST_EXT_LOOPBACK,
    CAN_CMD_SET_MODE_TST_INT_LOOPBACK,
    CAN_CMD_SET_MODE_TST_INTERCHANNEL,
    CAN_CMD_END_ENUM
} can_cmd_t;



/*****************************************************************************
Public functions
******************************************************************************/
can_err_t R_CAN_Open(can_cfg_t  *p_cfg,
                     void       (* const p_callback)(can_cb_evt_t   event,
                                                     void           *p_args));
can_err_t R_CAN_InitChan(uint8_t        chan,
                         can_bitrate_t  *p_baud,
                         void           (* const p_chcallback)(uint8_t      chan,
                                                               can_cb_evt_t event,
                                                               void         *p_args));
can_err_t R_CAN_ConfigFIFO(can_box_t            fifo_id,
                           can_fifo_threshold_t threshold,
                           can_box_t            txmbx_id);

can_err_t R_CAN_AddRxRule(uint8_t       chan,
                          can_filter_t  *p_filter,
                          can_box_t     dst_box);

can_err_t R_CAN_Control(can_cmd_t   cmd,
                        uint32_t    arg1);

can_err_t R_CAN_SendMsg(can_box_t   box_id,
                        can_txmsg_t *p_txmsg);

can_err_t R_CAN_GetMsg(can_box_t    box_id,
                       can_rxmsg_t  *p_rxmsg);

can_err_t R_CAN_GetHistoryEntry(can_box_t       box_id,
                                can_history_t   *p_entry);

uint32_t  R_CAN_GetStatusMask(can_stat_t    stat_type,
                              uint8_t       chan,
                              can_err_t     *p_err);

uint32_t  R_CAN_GetCountFIFO(can_box_t  box_id,
                             can_err_t  *p_err);

uint32_t  R_CAN_GetCountErr(can_count_t  count_type,
                            uint8_t      chan,
                            can_err_t    *p_err);

void      R_CAN_Close(void);
uint32_t  R_CAN_GetVersion(void);


#endif // CAN_INTERFACE_HEADER_FILE
