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
* File Name    : sample_audio_dma.h
* $Rev: 370 $
* $Date:: 2013-08-01 11:14:12 +0900#$
* Description  : 
******************************************************************************/

#ifndef SAMPLE_AUDIO_DMA_H
#define SAMPLE_AUDIO_DMA_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== DMA Channel ==== */
#define DMA_CHANNEL0            (0u)
#define DMA_CHANNEL1            (1u)
#define DMA_CHANNEL2            (2u)
#define DMA_CHANNEL3            (3u)
#define DMA_CHANNEL4            (4u)
#define DMA_CHANNEL5            (5u)
#define DMA_CHANNEL6            (6u)
#define DMA_CHANNEL7            (7u)
#define DMA_CHANNEL8            (8u)
#define DMA_CHANNEL9            (9u)
#define DMA_CHANNEL10           (10u)
#define DMA_CHANNEL11           (11u)
#define DMA_CHANNEL12           (12u)
#define DMA_CHANNEL13           (13u)
#define DMA_CHANNEL14           (14u)
#define DMA_CHANNEL15           (15u)

#define N0_SIDE                 (0u)
#define N1_SIDE                 (1u)

#define DMA_TRANSFER_SIZE_16    (0x00000001u)
#define DMA_TRANSFER_SIZE_32    (0x00000002u)

/* Return value */
#define DEVDRV_QUEUE_EMPTY      (-10)
#define DEVDRV_QUEUE_FULL       (-11)

#define DMA_SSIF                (0u)            /* SSIF */
#define DMA_SCUX                (1u)            /* SCUX */
#define DMA_NON_REGIST          (2u)            /* a module is Non registration. */

#define DMA_AUDIO_TX            (0u)            /* Memory to audio peripheral module[SSIF or SCUX] */
#define DMA_AUDIO_RX            (1u)            /* Audio peripheral module[SSIF or SCUX] to Memory */
#define DMA_NONE                (2u)            /* No select direct */

/* DMA STOP WAIT TIME */
/* This definition becomes the definition at waiting time DMA stop */
/* Please adjust time for waiting by user                          */
#define DMA_STOP_WAIT           (200u)

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef void (*fp_type)(void *vpBuff);

typedef struct dma_regist_info
{
    uint32_t    direct;                         /* Transfer direction */
    uint32_t    module_name;                    /* Name of transmit peripheral module */
    uint32_t    module_channel;                 /* Channel number of the DMA */
    bool_t      transfer_continue;              /* Transfer continuation */
    fp_type     user_trnend_func;               /* Callback interrupt function */
} dma_regist_info_t;

/******************************************************************************
Exported global variables
******************************************************************************/
void DMA_DmaInit (void);
int32_t DMA_DmaSetup (const uint32_t dma_channel, const dma_regist_info_t * const regist_info);
int32_t DMA_DmaAdsSet ( const uint32_t dma_channel,
                        volatile const void * const buf,
                        const uint32_t buf_size,
                        const bool_t init );
int32_t DMA_QueueInit (const uint32_t dma_channel);
int32_t DMA_TxRegPause (const uint32_t dma_channel, const bool_t pause_req);
int32_t DMA_TxRegSet (const uint32_t dma_channel, const uint32_t set_side);
int32_t DMA_RxRegSet (const uint32_t dma_channel, const uint32_t set_side);
int32_t DMA_CheckDmaMute (const uint32_t dma_channel, bool_t * const mute);
int32_t DMA_DmaStart (const uint32_t dma_channel);
int32_t DMA_DmaStop (const uint32_t dma_channel);
int32_t DMA_DmaTxInit (const uint32_t dma_channel, const uint32_t dma_transfer_size);
int32_t DMA_DmaRxInit (const uint32_t dma_channel, const uint32_t dma_transfer_size);

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SAMPLE_AUDIO_DMA_H */

/* End of File */
