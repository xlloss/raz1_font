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
/******************************************************************************
* File Name    : dma_register.h
* $Rev: 529 $
* $Date:: 2013-10-15 19:19:45 +0900#$
* Description  : DMA Register
******************************************************************************/

#ifndef DMA_REGISTER_H
#define DMA_REGISTER_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "reg_bit_access.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/******************************************************************************
Macro definitions
******************************************************************************/
#define DMA_CHANNEL_MAX (16u)

/******************************************************************************
Typedef definitions
******************************************************************************/
/* ==== DMA register address list ==== */
typedef struct dma_reg
{
    volatile uint32_t *n0sa;        /* Next0 Source Address Register(N0SA) */
    volatile uint32_t *n0da;        /* Next0 Destination Address Register(N0DA) */
    volatile uint32_t *n0tb;        /* Next0 Transaction Byte Register(N0TB) */
    volatile uint32_t *n1sa;        /* Next1 Source Address Register(N1SA) */
    volatile uint32_t *n1da;        /* Next1 Destination Address Register(N1DA) */
    volatile uint32_t *n1tb;        /* Next1 Transaction Byte Register(N1TB) */
    volatile uint32_t *chstat;      /* Channel Status Register(CHSTAT) */
    volatile uint32_t *chctrl;      /* Channel Control Register(CHCTRL) */
    volatile uint32_t *chcfg;       /* Channel Configuration Register(CHCFG) */
    volatile uint32_t *chitvl;      /* Channel Interval Register(chitvl) */
    volatile uint32_t *chext;       /* Channel Extension Register(CHEXT) */
    volatile uint32_t *dstat_en;    /* DMA Status EN Register(DSTAT_EN) */
    volatile uint32_t *dctrl;       /* DMA Control Register(DCTRL) */
    volatile uint32_t *dmars;       /* DMA Expansion resource selector(DMARS) */
} dma_reg_t;

/******************************************************************************
Exported global variables
******************************************************************************/
extern const dma_reg_t g_dma_reg_tbl[DMA_CHANNEL_MAX];

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMA_REGISTER_H */

/* End of File */
