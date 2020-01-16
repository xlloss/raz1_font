/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2012 - 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : dma_register.c
* $Rev: 370 $
* $Date:: 2013-08-01 11:14:12 +0900#$
* Description  : Sample DMA driver for audio sample software.
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "dma_register.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

/* ==== Addresses of the DMA registers for each channel ==== */
const dma_reg_t g_dma_reg_tbl[DMA_CHANNEL_MAX] = 
{
    {   /* ch0 */
        &DMAC.N0SA_0,       /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_0,       /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_0,       /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_0,       /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_0,       /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_0,       /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_0,     /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_0,     /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_0,      /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_0,     /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_0,      /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_0_7, /* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_0_7,    /* DMA Control Register(DCTRL) */
        &DMAC.DMARS0        /* DMA Expansion Resource Selector(DMARS) */
    },
    {   /* ch1 */
        &DMAC.N0SA_1,       /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_1,       /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_1,       /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_1,       /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_1,       /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_1,       /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_1,     /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_1,     /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_1,      /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_1,     /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_1,      /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_0_7, /* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_0_7,    /* DMA Control Register(DCTRL) */
        &DMAC.DMARS0        /* DMA Expansion Resource Selector(DMARS) */
    },
    {   /* ch2 */
        &DMAC.N0SA_2,       /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_2,       /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_2,       /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_2,       /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_2,       /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_2,       /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_2,     /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_2,     /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_2,      /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_2,     /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_2,      /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_0_7, /* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_0_7,    /* DMA Control Register(DCTRL) */
        &DMAC.DMARS1        /* DMA Expansion Resource Selector(DMARS) */
    },
    {   /* ch3 */
        &DMAC.N0SA_3,       /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_3,       /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_3,       /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_3,       /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_3,       /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_3,       /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_3,     /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_3,     /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_3,      /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_3,     /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_3,      /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_0_7, /* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_0_7,    /* DMA Control Register(DCTRL) */
        &DMAC.DMARS1        /* DMA Expansion Resource Selector(DMARS) */
    },
    {   /* ch4 */
        &DMAC.N0SA_4,       /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_4,       /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_4,       /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_4,       /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_4,       /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_4,       /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_4,     /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_4,     /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_4,      /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_4,     /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_4,      /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_0_7, /* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_0_7,    /* DMA Control Register(DCTRL) */
        &DMAC.DMARS2        /* DMA Expansion Resource Selector(DMARS) */
    },
    {   /* ch5 */
        &DMAC.N0SA_5,       /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_5,       /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_5,       /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_5,       /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_5,       /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_5,       /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_5,     /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_5,     /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_5,      /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_5,     /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_5,      /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_0_7, /* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_0_7,    /* DMA Control Register(DCTRL) */
        &DMAC.DMARS2        /* DMA Expansion Resource Selector(DMARS) */
    },
    {   /* ch6 */
        &DMAC.N0SA_6,       /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_6,       /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_6,       /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_6,       /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_6,       /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_6,       /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_6,     /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_6,     /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_6,      /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_6,     /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_6,      /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_0_7, /* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_0_7,    /* DMA Control Register(DCTRL) */
        &DMAC.DMARS3        /* DMA Expansion Resource Selector(DMARS) */
    },
    {   /* ch7 */
        &DMAC.N0SA_7,       /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_7,       /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_7,       /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_7,       /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_7,       /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_7,       /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_7,     /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_7,     /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_7,      /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_7,     /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_7,      /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_0_7, /* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_0_7,    /* DMA Control Register(DCTRL) */
        &DMAC.DMARS3        /* DMA Expansion Resource Selector(DMARS) */
    },
    {   /* ch8 */
        &DMAC.N0SA_8,       /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_8,       /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_8,       /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_8,       /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_8,       /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_8,       /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_8,     /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_8,     /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_8,      /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_8,     /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_8,      /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_8_15,/* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_8_15,   /* DMA Control Register(DCTRL) */
        &DMAC.DMARS4        /* DMA Expansion Resource Selector(DMARS) */
    },
    {   /* ch9 */
        &DMAC.N0SA_9,       /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_9,       /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_9,       /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_9,       /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_9,       /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_9,       /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_9,     /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_9,     /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_9,      /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_9,     /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_9,      /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_8_15,/* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_8_15,   /* DMA Control Register(DCTRL) */
        &DMAC.DMARS4        /* DMA Expansion Resource Selector(DMARS) */
    },
    {   /* ch10 */
        &DMAC.N0SA_10,      /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_10,      /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_10,      /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_10,      /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_10,      /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_10,      /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_10,    /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_10,    /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_10,     /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_10,    /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_10,     /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_8_15,/* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_8_15,   /* DMA Control Register(DCTRL) */
        &DMAC.DMARS5        /* DMA Expansion Resource Selector(DMARS) */
    },
    {   /* ch11 */
        &DMAC.N0SA_11,      /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_11,      /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_11,      /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_11,      /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_11,      /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_11,      /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_11,    /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_11,    /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_11,     /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_11,    /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_11,     /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_8_15,/* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_8_15,   /* DMA Control Register(DCTRL) */
        &DMAC.DMARS5        /* DMA Expansion Resource Selector(DMARS) */
    },
    {   /* ch12 */
        &DMAC.N0SA_12,      /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_12,      /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_12,      /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_12,      /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_12,      /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_12,      /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_12,    /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_12,    /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_12,     /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_12,    /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_12,     /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_8_15,/* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_8_15,   /* DMA Control Register(DCTRL) */
        &DMAC.DMARS6        /* DMA Expansion Resource Selector(DMARS) */
    },
    {   /* ch13 */
        &DMAC.N0SA_13,      /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_13,      /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_13,      /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_13,      /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_13,      /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_13,      /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_13,    /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_13,    /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_13,     /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_13,    /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_13,     /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_8_15,/* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_8_15,   /* DMA Control Register(DCTRL) */
        &DMAC.DMARS6        /* DMA Expansion Resource Selector(DMARS) */
    },
    {   /* ch14 */
        &DMAC.N0SA_14,      /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_14,      /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_14,      /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_14,      /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_14,      /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_14,      /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_14,    /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_14,    /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_14,     /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_14,    /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_14,     /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_8_15,/* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_8_15,   /* DMA Control Register(DCTRL) */
        &DMAC.DMARS7        /* DMA Expansion Resource Selector(DMARS) */
    },
    {   /* ch15 */
        &DMAC.N0SA_15,      /* Next0 Source Address Register(N0SA) */
        &DMAC.N0DA_15,      /* Next0 Destination Address Register(N0DA) */
        &DMAC.N0TB_15,      /* Next0 Transaction Byte Register(N0TB) */
        &DMAC.N1SA_15,      /* Next1 Source Address Register(N1SA) */
        &DMAC.N1DA_15,      /* Next1 Destination Address Register(N1DA) */
        &DMAC.N1TB_15,      /* Next1 Transaction Byte Register(N1TB) */
        &DMAC.CHSTAT_15,    /* Channel Status Register(CHSTAT) */
        &DMAC.CHCTRL_15,    /* Channel Control Register(CHCTRL) */
        &DMAC.CHCFG_15,     /* Channel Configuration Register(CHCFG) */
        &DMAC.CHITVL_15,    /* Channel Interval Register(chitvl) */
        &DMAC.CHEXT_15,     /* Channel Extension Register(CHEXT) */
        &DMAC.DSTAT_EN_8_15,/* DMA Status EN Register(DSTAT_EN) */
        &DMAC.DCTRL_8_15,   /* DMA Control Register(DCTRL) */
        &DMAC.DMARS7        /* DMA Expansion Resource Selector(DMARS) */
    }
};


/* ==== Mask data and Position of Register bits ==== */


/* End of File */
