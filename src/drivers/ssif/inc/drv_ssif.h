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
* File Name    : drv_ssif.h
* $Rev: 474 $
* $Date:: 2013-10-03 17:13:02 +0900#$
* Description  : This module is header for sample of SSIF driver.
******************************************************************************/
#ifndef DRV_SSIF_H
#define DRV_SSIF_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "sample_audio_dma.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== SSI Channel ==== */
#define SSI_CHANNEL0        (0u)
#define SSI_CHANNEL1        (1u)
#define SSI_CHANNEL2        (2u)
#define SSI_CHANNEL3        (3u)
#define SSI_CHANNEL4        (4u)
#define SSI_CHANNEL5        (5u)
#define SSI_CHANNEL_MAX     (6u)

/* ==== SSI Transmission and reception ==== */
#define SSI_NONE        (0)
#define SSI_RX          (1u)
#define SSI_TX          (2u)
#define SSI_FULLDUPLEX  (SSI_RX | SSI_TX)

/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
/* ==== API Function ==== */
void R_SSI_RamInit(void);
int32_t R_SSI_SsiInit(const uint32_t ssi_channel);
int32_t R_SSI_DmaInit(const uint32_t ssi_channel,
                      const uint32_t dma_channel,
                      const uint32_t direct,
                      const fp_type func);
int32_t R_SSI_DmaStart(const uint32_t ssi_channel, const uint32_t direct);
int32_t R_SSI_SsiStart(const uint32_t ssi_channel, const uint32_t direct);
int32_t R_SSI_SsiDmaStop(const uint32_t ssi_channel);
int32_t R_SSI_SsiStop(const uint32_t ssi_channel);
int32_t R_SSI_SsiDmaAdsSet( const uint32_t ssi_channel,
                            const uint32_t direct,
                            volatile const void * const buf,
                            const uint32_t buf_size );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DRV_SSIF_H */

/* End of File */
