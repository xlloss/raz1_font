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
* File Name   : sample_audio_dma.c
* $Rev: 533 $
* $Date:: 2013-10-15 19:40:20 +0900#$
* Description  : Sample DMA driver for audio sample software.
*                 - audio peripheral module[SSIF or SCUX] to RAM
*                 - RAM to audio peripheral module[SSIF or SCUX]
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "dev_drv.h"
#include "devdrv_intc.h"
#include "dma_register.h"
#include "sample_audio_dma.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define ON                  (1u)
#define OFF                 (0u)
#define DMA_FIX_PRIO_MODE   (0u)
#define NON_REGIST          (0xFFFFFFFFu)
#define MUTE_BUF_SIZE       (880u)      /* Mute buffer(5ms) */
#define QUEUE_NUM           (2u)        /* QUEUE NUM */

#define DMA_NUM_SSIF_CH     (6u)
#define DMA_NUM_SCUX_CH     (4u)
#define DMA_NUM_MODULE      (2u)

/* DMA set point        */
/* DMA CHCFG register   */

#define DMA_CHCFG_TX_BASE_INIT_VALUE    (0x61222268u)
/*  [31]    DMS   : B'0    : register mode                                  */
/*  [30]    REN   : B'1    : After completion of transfer,transfer Continue */
/*  [29]    RSW   : B'1    : Invert the RSEL after transferring             */
/*  [28]    RSEL  : B'0    : Run side Next0 Register                        */
/*  [27]    SBE   : B'0    : Transfer abort without the sweep of buffer     */
/*  [26]    -     : B'0    :                                                */
/*  [25]    -     : B'0    :                                                */
/*  [24]    DEM   : B'1    : Masks the DMA transfer end interrupt                   */
/*  [23]    -     : B'0    :                                                */
/*  [22]    TM    : B'0    : Single transfer mode                           */
/*  [21]    DAD   : B'1    : Destination address is fixed                   */
/*  [20]    SAD   : B'0    : Source address increment                       */
/*  [19:16] DDS   : B'0010 : Destination Data Size:32-bit                           */
/*  [15:12] SDS   : B'0010 : Source Data Size:32-bit                                */
/*  [11]    -     : B'0    :                                                */
/*  [10:8]  AM    : B'010  : Bus cycle mode                                         */
/*  [7]     -     : B'0    :                                                */
/*  [6]     LVL   : B'1    : Detects based on the level                             */
/*  [5]     HIEN  : B'1    : Detects a request when the signal is at the High level */
/*  [4]     LOEN  : B'0    : initial value                                          */
/*  [3]     REQD  : B'1    : Destination; DMAACK is to become active when written   */

/* DMA CHCFG Register */
#define DMA_CHCFG_RX_BASE_INIT_VALUE    (0x61122260u)
/*  [31]    DMS   : B'0    : register mode                                  */
/*  [30]    REN   : B'1    : After completion of transfer,transfer Continue */
/*  [29]    RSW   : B'1    : Invert the RSEL after transferring             */
/*  [28]    RSEL  : B'0    : Run side Next0 Register                        */
/*  [27]    SBE   : B'0    : Transfer abort without the sweep of buffer     */
/*  [26]    -     : B'0    :                                                */
/*  [25]    -     : B'0    :                                                */
/*  [24]    DEM   : B'1    : Masks the DMA transfer end interrupt                   */
/*  [23]    -     : B'0    :                                                */
/*  [22]    TM    : B'0    : Single transfer mode                           */
/*  [21]    DAD   : B'0    : Destination address increment                  */
/*  [20]    SAD   : B'1    : Source address is fixed                        */
/*  [19:16] DDS   : B'0010 : Destination Data Size:32-bit                           */
/*  [15:12] SDS   : B'0010 : Source Data Size:32-bit                                */
/*  [11]    -     : B'0    :                                                */
/*  [10:8]  AM    : B'010  : Bus cycle mode                                         */
/*  [7]     -     : B'0    :                                                */
/*  [6]     LVL   : B'1    : Detects based on the level                             */
/*  [5]     HIEN  : B'1    : Detects a request when the signal is at the High level */
/*  [4]     LOEN  : B'0    : initial value                                          */
/*  [3]     REQD  : B'0    : Source; DMAACK is to become active when read           */

#define DMA_CHCFG_DDS_BIT_MASK              (0x000F0000u)
#define DMA_CHCFG_SDS_BIT_MASK              (0x0000F000u)
#define DMA_CHCFG_DDS_BIT_POSI              (16u)
#define DMA_CHCFG_SDS_BIT_POSI              (12u)


/* CHSTAT Register */
#define DMA_CHSTAT_SUS_BIT                  (0x00000008u)
#define DMA_CHSTAT_TACT_BIT                 (0x00000004u)

/* CHCTRL Register */
#define DMA_CHCTRL_SWRST_BIT                (0x00000008u)
#define DMA_CHCTRL_SETEN_BIT                (0x00000001u)
#define DMA_CHCTRL_SETSUS_BIT               (0x00000100u)
#define DMA_CHCTRL_CLREN_BIT                (0x00000002u)
#define DMA_CHCTRL_CLREND_BIT               (0x00000020u)
#define DMA_CHCTRL_CLRTC_BIT                (0x00000040u)

/* CHCFG Register */
#define DMA_CHCFG_REN_BIT                   (0x40000000u)
#define DMA_CHCFG_RSEL_BIT                  (0x10000000u)
#define DMA_CHCFG_DEM_BIT                   (0x01000000u)
#define DMA_CHCFG_RSW_BIT                   (0x20000000u)

/* DMA Transfer End INT LVL */
#define DMA_TRNEND_INT_LV                   (5u)


/******************************************************************************
Typedef definitions
******************************************************************************/
/* typedef variables */
/* queue information */
typedef struct queue_inf{
    uint32_t    txside;                 /* N0_SIDE of N1_SIDE */
    uint32_t    n0_side;
    uint32_t    n1_side;
    uint32_t    write_num;
    uint32_t    rpoint;
    uint32_t    wpoint;
    uint32_t    ads[QUEUE_NUM];
    uint32_t    size[QUEUE_NUM];
    uint32_t    last_ads;
    uint32_t    last_size;
    bool_t      tx_pause_flg;
} queue_inf_t;


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static int32_t QueueSet (uint32_t dma_channel, volatile const uint32_t * const buf, uint32_t buf_size);
static int32_t DmaInt (uint32_t dma_channel);
static void DMA0_INT_TrnEnd (uint32_t int_sense);
static void DMA1_INT_TrnEnd (uint32_t int_sense);
static void DMA2_INT_TrnEnd (uint32_t int_sense);
static void DMA3_INT_TrnEnd (uint32_t int_sense);
static void DMA4_INT_TrnEnd (uint32_t int_sense);
static void DMA5_INT_TrnEnd (uint32_t int_sense);
static void DMA6_INT_TrnEnd (uint32_t int_sense);
static void DMA7_INT_TrnEnd (uint32_t int_sense);
static void DMA8_INT_TrnEnd (uint32_t int_sense);
static void DMA9_INT_TrnEnd (uint32_t int_sense);
static void DMA10_INT_TrnEnd (uint32_t int_sense);
static void DMA11_INT_TrnEnd (uint32_t int_sense);
static void DMA12_INT_TrnEnd (uint32_t int_sense);
static void DMA13_INT_TrnEnd (uint32_t int_sense);
static void DMA14_INT_TrnEnd (uint32_t int_sense);
static void DMA15_INT_TrnEnd (uint32_t int_sense);


/* DMA Expansion resource selector Tx table */
/* SSIF */
static const uint16_t dma_ssif_dmars_reg[DMA_NUM_SSIF_CH][DMA_NONE] = 
{
        /* Tx   ,   Rx */
        { 0x00E1,   0x00E2 }        /* SSIF channel 0 */
    ,   { 0x00E5,   0x00E6 }        /* SSIF channel 1 */
    ,   { 0x00EB,   0x00EB }        /* SSIF channel 2 */
    ,   { 0x00ED,   0x00EE }        /* SSIF channel 3 */
    ,   { 0x00F3,   0x00F3 }        /* SSIF channel 4 */
    ,   { 0x00F5,   0x00F6 }        /* SSIF channel 5 */
};

/* SCUX */
static const uint16_t dma_scux_dmars_reg[DMA_NUM_SCUX_CH][DMA_NONE] =
{
        /* Tx   ,   Rx */
        { 0x0101,   0x0102 }        /* SCUX channel 0 */
    ,   { 0x0105,   0x0106 }        /* SCUX channel 1 */
    ,   { 0x0109,   0x010A }        /* SCUX channel 2 */
    ,   { 0x010D,   0x010E }        /* SCUX channel 3 */
};

/* SSIF */
static const volatile uint32_t * const dma_ssif_da_sa_reg[DMA_NUM_SSIF_CH][DMA_NONE] =
{
        /* Tx           , Rx */
        { &SSIF0.SSIFTDR, &SSIF0.SSIFRDR }      /* SSIF channel 0 */
    ,   { &SSIF1.SSIFTDR, &SSIF1.SSIFRDR }      /* SSIF channel 1 */
    ,   { &SSIF2.SSIFTDR, &SSIF2.SSIFRDR }      /* SSIF channel 2 */
    ,   { &SSIF3.SSIFTDR, &SSIF3.SSIFRDR }      /* SSIF channel 3 */
    ,   { &SSIF4.SSIFTDR, &SSIF4.SSIFRDR }      /* SSIF channel 4 */
    ,   { &SSIF5.SSIFTDR, &SSIF5.SSIFRDR }      /* SSIF channel 5 */
};

/* SCUX */
static const volatile uint32_t * const dma_scux_da_sa_reg[DMA_NUM_SCUX_CH][DMA_NONE] =
{
        /* Tx                    ,  Rx */
        { &SCUX.DMATD0_CIM.UINT32,  &SCUX.DMATU0_CIM.UINT32 }       /* SCUX channel 0 */
    ,   { &SCUX.DMATD1_CIM.UINT32,  &SCUX.DMATU1_CIM.UINT32 }       /* SCUX channel 1 */
    ,   { &SCUX.DMATD2_CIM.UINT32,  &SCUX.DMATU2_CIM.UINT32 }       /* SCUX channel 2 */
    ,   { &SCUX.DMATD3_CIM.UINT32,  &SCUX.DMATU3_CIM.UINT32 }       /* SCUX channel 3 */
};

/* DMA INT ID table */
static const uint16_t dma_int_id[DMA_CHANNEL_MAX] =
{
    INTC_ID_DMAINT0,
    INTC_ID_DMAINT1,
    INTC_ID_DMAINT2,
    INTC_ID_DMAINT3,
    INTC_ID_DMAINT4,
    INTC_ID_DMAINT5,
    INTC_ID_DMAINT6,
    INTC_ID_DMAINT7,
    INTC_ID_DMAINT8,
    INTC_ID_DMAINT9,
    INTC_ID_DMAINT10,
    INTC_ID_DMAINT11,
    INTC_ID_DMAINT12,
    INTC_ID_DMAINT13,
    INTC_ID_DMAINT14,
    INTC_ID_DMAINT15
};

static dma_regist_info_t dma_regist[DMA_CHANNEL_MAX];
static queue_inf_t queue[DMA_CHANNEL_MAX];
static uint32_t mute_buf[MUTE_BUF_SIZE / sizeof(uint32_t)];


/*******************************************************************************
* Function Name: DMA_DmaInit
* Description  : Initialize Queue for Mute Buffer
* Arguments    : none
* Return Value : none
*******************************************************************************/
void DMA_DmaInit (void)
{
    uint32_t counter;
    int32_t ret;

    /* ==== Initialize Queue Information ==== */
    for (counter = 0; counter < DMA_CHANNEL_MAX; counter++)
    {
        dma_regist[counter].module_name = DMA_NON_REGIST;
        ret = DMA_QueueInit(counter);
        if (DEVDRV_ERROR == ret)
        {
            break;
        }
    }

    if (DEVDRV_SUCCESS == ret)
    {
        /* ==== Clear Mute Buffer ===== */
        for (counter = 0; counter < (MUTE_BUF_SIZE/sizeof(uint32_t)); counter++)
        {
            mute_buf[counter] = 0;
        }
    }
}

/*******************************************************************************
* Function Name: DMA_DmaSetup
* Description  : Setup the DMAC
* Arguments    : - dma_cannel
*                    Channel number of the DMA
*                - regist_info
*                    Registration the DMAC Infomation
* Return Value : - DEVDRV_SUCCESS
*                    DMA Set Up Success 
*                - DEVDRV_ERROR
*                    DMA Set Up Error
*******************************************************************************/
int32_t DMA_DmaSetup (const uint32_t dma_channel, const dma_regist_info_t * const regist_info)
{
    int32_t ret;

    /* DMA INT Function Table */
    static void (* dma_int_trnend[DMA_CHANNEL_MAX])(uint32_t int_sense) =
    {
       &DMA0_INT_TrnEnd
     , &DMA1_INT_TrnEnd
     , &DMA2_INT_TrnEnd
     , &DMA3_INT_TrnEnd
     , &DMA4_INT_TrnEnd
     , &DMA5_INT_TrnEnd
     , &DMA6_INT_TrnEnd
     , &DMA7_INT_TrnEnd
     , &DMA8_INT_TrnEnd
     , &DMA9_INT_TrnEnd
     , &DMA10_INT_TrnEnd
     , &DMA11_INT_TrnEnd
     , &DMA12_INT_TrnEnd
     , &DMA13_INT_TrnEnd
     , &DMA14_INT_TrnEnd
     , &DMA15_INT_TrnEnd
    };

    /* ==== Module Channel Number Table ==== */
    static const uint32_t num_module_channel[DMA_NUM_MODULE] =
    {
        DMA_NUM_SSIF_CH,    /* The Number of SSIF channels */
        DMA_NUM_SCUX_CH     /* The Number of SCUX channels */
    };

    /* ==== Argument Check ==== */
    ret = DEVDRV_SUCCESS;
    if (dma_channel >= DMA_CHANNEL_MAX)
    {
        /* ---- DMA channel number is out of a range ----  */
        ret = DEVDRV_ERROR;
    }
    else
    {
        if (DMA_NON_REGIST != dma_regist[dma_channel].module_name)
        {
            /* ---- Module name has been already registered ---- */
            ret = DEVDRV_ERROR;
        }

        if (regist_info == NULL)
        {
            ret = DEVDRV_ERROR;
        }
        else
        {
            if (num_module_channel[regist_info->module_name] <= regist_info->module_channel)
            {
                /* ---- Module channel number is out of a range ---- */
                ret = DEVDRV_ERROR;
            }

            /* ==== DMA Information Register ==== */
            if (DEVDRV_SUCCESS == ret)
            {
                dma_regist[dma_channel] = *regist_info;

                /* ---- DMA interrupt setting ---- */
                ret = R_INTC_RegistIntFunc(dma_int_id[dma_channel], dma_int_trnend[dma_channel]);
                if (DEVDRV_SUCCESS == ret)
                {
                    ret = R_INTC_SetPriority(dma_int_id[dma_channel], DMA_TRNEND_INT_LV);
                }
            }
        }
    }

    return ret;

}

/*******************************************************************************
* Function Name: DMA_DmaAdsSet
* Description  : Sign up to transe and receive queue buffer
* Arguments    : - dma_channel
*                    DMA channel number
*                - *buf
*                    User Buffer top address
*                - buf_size
*                    User Buffer size
*                - init
*                    Initialize Audio Buffer setting
*                      true :Initialize
*                      false:Not initialize
* Return Value : - DEVDRV_SUCCESS
*                    User Buffer Regist Success 
*                - DEVDRV_ERROR
*                    User Buffer Regist Error
*                - DEVDRV_QUEUE_FULL
*                    Queue full
*******************************************************************************/
int32_t DMA_DmaAdsSet ( const uint32_t dma_channel,
                        volatile const void * const buf,
                        const uint32_t buf_size,
                        const bool_t init )
{
    dma_regist_info_t *dma_info;
    int32_t ret;
    
    /* ==== Argument Check ==== */
    ret = DEVDRV_SUCCESS;
    if (dma_channel >= DMA_CHANNEL_MAX)
    {
        ret = DEVDRV_ERROR;
    }
    else
    {
        if (DMA_NON_REGIST == dma_regist[dma_channel].module_name)
        {
            ret = DEVDRV_ERROR;
        }

        /* ==== DMA Information Register ==== */
        dma_info = &dma_regist[dma_channel];

        if (DEVDRV_SUCCESS == ret)
        {
            /* ==== DMA(Tx) Setting ==== */
            if (DMA_AUDIO_TX == dma_info->direct)
            {
                /* ---- DMA Enable ---- */
                if (false == init)
                {
                    if (ON == queue[dma_channel].n0_side)
                    {
                        if (ON == queue[dma_channel].n1_side)
                        {
                            /* Next0 and Next1 set */
                            ret = QueueSet(dma_channel, buf, buf_size);
                        }
                        else
                        {
                            /* Next0 set, Next1 non-set */
                            ret = QueueSet(dma_channel, buf, buf_size);
                            if (DEVDRV_SUCCESS == ret)
                            {
                                ret = DMA_TxRegSet(dma_channel, N1_SIDE);
                            }
                        }
                    }
                    else
                    {
                        if (ON == queue[dma_channel].n1_side)
                        {
                            /* Next0 non-set, Next1 set */
                            ret = QueueSet(dma_channel, buf, buf_size);
                            if (DEVDRV_SUCCESS == ret)
                            {
                                ret = DMA_TxRegSet(dma_channel, N0_SIDE);
                            }
                        }
                        else
                        {
                            /* Next0 and Next1 non-set */
                            ret = DMA_DmaStop(dma_channel);
                            if (DEVDRV_SUCCESS == ret)
                            {
                                ret = DMA_QueueInit(dma_channel);
                            }
                            if (DEVDRV_SUCCESS == ret)
                            {
                                ret = QueueSet(dma_channel, buf, buf_size);
                            }
                            if (DEVDRV_SUCCESS == ret)
                            {
                                ret = DMA_TxRegSet(dma_channel, N0_SIDE);
                            }
                            if (DEVDRV_SUCCESS == ret)
                            {
                                ret = DMA_TxRegSet(dma_channel, N1_SIDE);
                            }
                            if (ret != DEVDRV_ERROR)
                            {
                                ret = DMA_DmaStart(dma_channel);
                            }
                        }
                    }
                }
                else
                {
                    /* ---- DMA Disable ---- */
                    ret = QueueSet(dma_channel, buf, buf_size);
                }
            }
            /* ==== DMA(Rx) Setting ==== */
            else
            {
                /* ---- DMA Enable ---- */
                if (false == init)
                {
                    if (ON == queue[dma_channel].n0_side)
                    {
                        if (ON == queue[dma_channel].n1_side)
                        {
                            /* Next0 and Next1 set */
                            ret = QueueSet(dma_channel, buf, buf_size);
                        }
                        else
                        {
                            /* Next0 set, Next1 non-set */
                            ret = QueueSet(dma_channel, buf, buf_size);
                            if (DEVDRV_SUCCESS == ret)
                            {
                                ret = DMA_RxRegSet(dma_channel, N1_SIDE);
                            }
                        }
                    }
                    else
                    {
                        if (ON == queue[dma_channel].n1_side)
                        {
                            /* Next0 non-set, Next1 set */
                            ret = QueueSet(dma_channel, buf, buf_size);
                            if (DEVDRV_SUCCESS == ret)
                            {
                                ret = DMA_RxRegSet(dma_channel, N0_SIDE);
                            }
                        }
                        else
                        {
                            /* Next0 and Next1 non-set */
                            ret = DMA_DmaStop(dma_channel);
                            if (DEVDRV_SUCCESS == ret)
                            {
                                ret = DMA_QueueInit(dma_channel);
                            }
                            if (DEVDRV_SUCCESS == ret)
                            {
                                ret = QueueSet(dma_channel, buf, buf_size);
                            }
                            if (DEVDRV_SUCCESS == ret)
                            {
                                ret = DMA_RxRegSet(dma_channel, N0_SIDE);
                            }
                            if (DEVDRV_SUCCESS == ret)
                            {
                                ret = DMA_RxRegSet(dma_channel, N1_SIDE);
                            }
                            if (ret != DEVDRV_ERROR)
                            {
                                ret = DMA_DmaStart(dma_channel);
                            }
                        }
                    }
                }
                else
                {
                    /* ---- DMA Disable ---- */
                    ret = QueueSet(dma_channel, buf, buf_size);
                }
            }
        }
    }

    return ret;

}

/*******************************************************************************
* Function Name: DMA_QueueInit
* Description  : Initialization of the Queue
* Arguments    : - dma_channel
*                    DMA channel number
* Return Value : - DEVDRV_SUCCESS
*                    Queue Init Success 
*                - DEVDRV_ERROR
*                    Queue Init Error
*******************************************************************************/
int32_t DMA_QueueInit (const uint32_t dma_channel)
{
    int32_t ret;

    /* ==== Argument Check ==== */
    ret = DEVDRV_SUCCESS;
    if (dma_channel >= DMA_CHANNEL_MAX)
    {
        /* ---- DMA channel is out of a range ---- */
        ret = DEVDRV_ERROR;
    }
    else
    {
        queue[dma_channel].write_num = 0;
        queue[dma_channel].txside = N0_SIDE;
        queue[dma_channel].n0_side = OFF;
        queue[dma_channel].n1_side = OFF;
        queue[dma_channel].rpoint = 0;
        queue[dma_channel].wpoint = 0;
        queue[dma_channel].last_ads = 0;
        queue[dma_channel].last_size = 0;
        queue[dma_channel].tx_pause_flg = false;
    }

    return ret;

}

/*******************************************************************************
* Function Name: QueueSet
* Description  : User buffer register to the queue
* Arguments    : - dma_channel
*                    DMA channel number
*                - buf
*                    User Buffer top Address
*                - buf_size
*                    User Buffer size
* Return Value : - DEVDRV_SUCCESS
*                    Queue Regist Success 
*                - DEVDRV_QUEUE_FULL
*                    Queue full
*******************************************************************************/
static int32_t QueueSet ( const uint32_t dma_channel,
                          volatile const uint32_t * const buf,
                          const uint32_t buf_size )
{
    int32_t ret;

    ret = DEVDRV_SUCCESS;
    if(queue[dma_channel].write_num < QUEUE_NUM)
    {
        queue[dma_channel].ads[queue[dma_channel].wpoint] = (uint32_t)buf;
        queue[dma_channel].size[queue[dma_channel].wpoint] = buf_size;
        queue[dma_channel].wpoint++;
        if (queue[dma_channel].wpoint >= QUEUE_NUM)
        {
            queue[dma_channel].wpoint = 0;
        }
        queue[dma_channel].write_num++;
    }
    else
    {
        ret = DEVDRV_QUEUE_FULL;
    }

    return ret;

}

/*******************************************************************************
* Function Name: DMA_DmaStart
* Description  : Start DMA module
* Arguments    : - dma_channel
*                    DMA channel number
* Return Value : - DEVDRV_SUCCESS
*                    DMA Start Success 
*                - DEVDRV_ERROR
*                    DMA Start Error
*******************************************************************************/
int32_t DMA_DmaStart (const uint32_t dma_channel)
{
    int32_t ret;

    /* ==== Argument Check ==== */
    ret = DEVDRV_SUCCESS;
    if (dma_channel >= DMA_CHANNEL_MAX)
    {
        ret = DEVDRV_ERROR;
    }
    else
    {
        if (DMA_NON_REGIST == dma_regist[dma_channel].module_name)
        {
            ret = DEVDRV_ERROR;
        }

        /* ==== DMA Enable ==== */
        if (DEVDRV_SUCCESS == ret)
        {
            /* ---- Status Clear ---- */
            *g_dma_reg_tbl[dma_channel].chctrl |= DMA_CHCTRL_SWRST_BIT;

            /* ---- Enable DMA Transfer End Interrupt ---- */
            ret = R_INTC_Enable(dma_int_id[dma_channel]);

            if (DEVDRV_SUCCESS == ret)
            {
                /* ---- Release DMA Interrupt Mask ---- */
                *g_dma_reg_tbl[dma_channel].chcfg &= ~DMA_CHCFG_DEM_BIT;

                /* ---- Enable DMA Transfer ---- */
                *g_dma_reg_tbl[dma_channel].chctrl |= DMA_CHCTRL_SETEN_BIT;
            }
        }
    }

    return ret;

}

/*******************************************************************************
* Function Name: DMA_DmaStop
* Description  : Stop DMA module
* Arguments    : - dma_channel
*                    DMA channel number
* Return Value : - DEVDRV_SUCCESS
*                    DMA Stop Success 
*                - DEVDRV_ERROR
*                    DMA Stop Error
*******************************************************************************/
int32_t DMA_DmaStop (const uint32_t dma_channel)
{
    uint32_t counter;
    int32_t ret;

    /* DMA DSTAT_EN Register table */
    static const uint32_t dma_dstat_reg[DMA_CHANNEL_MAX] = 
    {
        0x00000001u,
        0x00000002u,
        0x00000004u,
        0x00000008u,
        0x00000010u,
        0x00000020u,
        0x00000040u,
        0x00000080u,
        0x00000001u,
        0x00000002u,
        0x00000004u,
        0x00000008u,
        0x00000010u,
        0x00000020u,
        0x00000040u,
        0x00000080u
    };

    /* ==== Argument Check ==== */
    if (dma_channel >= DMA_CHANNEL_MAX)
    {
        ret = DEVDRV_ERROR;
    }
    else
    {
        /* ==== DMA Stop ==== */
        /* ---- Disable DMA End Interrupt ---- */
        ret = R_INTC_Disable(dma_int_id[dma_channel]);
        if (DEVDRV_SUCCESS == ret)
        {
            /* ---- Set Suspend ---- */
            *g_dma_reg_tbl[dma_channel].chctrl |= DMA_CHCTRL_SETSUS_BIT;
            for ( counter = 0;
                 (counter < DMA_STOP_WAIT) &&
                 (0u != (*g_dma_reg_tbl[dma_channel].dstat_en & dma_dstat_reg[dma_channel]));
                  counter++)
            {
                /* Check Status */
                if ( DMA_CHSTAT_SUS_BIT == (*g_dma_reg_tbl[dma_channel].chstat & DMA_CHSTAT_SUS_BIT) )
                {
                    break;
                }
            }

            if ( (*g_dma_reg_tbl[dma_channel].dstat_en & dma_dstat_reg[dma_channel]) != 0u )
            {
                /* ---- Set DMA Stop ---- */
                *g_dma_reg_tbl[dma_channel].chctrl |= DMA_CHCTRL_CLREN_BIT;
                for (counter = 0; counter < DMA_STOP_WAIT; counter++)
                {
                    /* Check Status */
                    if ( 0u == (*g_dma_reg_tbl[dma_channel].chstat & DMA_CHSTAT_TACT_BIT) )
                    {
                        break;
                    }
                }
            }

            /* ---- Software reset ---- */
            *g_dma_reg_tbl[dma_channel].chctrl |= DMA_CHCTRL_SWRST_BIT;
            /* Queue Information clear */
            queue[dma_channel].txside = N0_SIDE;
            queue[dma_channel].n0_side = OFF;
            queue[dma_channel].n1_side = OFF;
        }
    }

    return ret;

}


/*******************************************************************************
* Function Name: DMA_DmaTxInit
* Description  : DMA initialization module in the transmission behavior
* Arguments    : - dma_channel
*                    DMA channel number
*                - dma_transfer_size
*                    Transfer size
* Return Value : - DEVDRV_SUCCESS
*                    DMA(Tx) Init Success
*                - DEVDRV_ERROR
*                    DMA(Tx) Init Error
*******************************************************************************/
int32_t DMA_DmaTxInit (const uint32_t dma_channel, const uint32_t dma_transfer_size)
{
    uint32_t da;
    uint32_t tmp_dmars;
    uint32_t tmp_chcfg;
    int32_t ret;
    dma_regist_info_t *dma_info;

    dma_info = &dma_regist[dma_channel];

    /* ==== Argument Check ==== */
    ret = DEVDRV_SUCCESS;
    if (dma_channel >= DMA_CHANNEL_MAX)
    {
        ret = DEVDRV_ERROR;
    }
    else
    {
        if (DMA_NON_REGIST == dma_regist[dma_channel].module_name)
        {
            ret = DEVDRV_ERROR;
        }

        if (DEVDRV_SUCCESS == ret)
        {
            /* ==== DMA Initialize ==== */
            if (DMA_SSIF == dma_info->module_name)
            {
                /* ---- SSIF Module ---- */
                tmp_dmars = dma_ssif_dmars_reg[dma_info->module_channel][dma_info->direct];
                da = (uint32_t)dma_ssif_da_sa_reg[dma_info->module_channel][dma_info->direct];
            }
            else
            {
                /* ---- SCUX Module ---- */
                tmp_dmars = dma_scux_dmars_reg[dma_info->module_channel][dma_info->direct];
                da = (uint32_t)dma_scux_da_sa_reg[dma_info->module_channel][dma_info->direct];
            }

            /* ---- DMA Control Register Setting ---- */
            *g_dma_reg_tbl[dma_channel].dctrl = DMA_FIX_PRIO_MODE;

            /* ---- DMA Next0 Address Setting ---- */
            *g_dma_reg_tbl[dma_channel].n0sa = (uint32_t)&mute_buf;
            *g_dma_reg_tbl[dma_channel].n0da = da;
            *g_dma_reg_tbl[dma_channel].n0tb = sizeof(mute_buf);

            /* ---- DMA Next1 Address Setting ---- */
            *g_dma_reg_tbl[dma_channel].n1sa = (uint32_t)&mute_buf;
            *g_dma_reg_tbl[dma_channel].n1da = da;
            *g_dma_reg_tbl[dma_channel].n1tb = sizeof(mute_buf);

            /* ----- Transmission Side Setting ---- */
            tmp_chcfg = DMA_CHCFG_TX_BASE_INIT_VALUE & ~DMA_CHCFG_DDS_BIT_MASK;
            tmp_chcfg |= (dma_transfer_size << DMA_CHCFG_DDS_BIT_POSI);
            tmp_chcfg &= ~DMA_CHCFG_SDS_BIT_MASK;
            tmp_chcfg |= (dma_transfer_size << DMA_CHCFG_SDS_BIT_POSI);
            tmp_chcfg |= (dma_channel % 8u);
            if (true == dma_info->transfer_continue)
            {
                tmp_chcfg |= DMA_CHCFG_REN_BIT;
            }
            else
            {
                tmp_chcfg &= ~DMA_CHCFG_REN_BIT;
            }
            *g_dma_reg_tbl[dma_channel].chcfg = tmp_chcfg;

            /* ---- DMA Expansion Resource Selector Setting ---- */
            if ( (dma_channel % 2u) == 0u )
            {
                *g_dma_reg_tbl[dma_channel].dmars |= tmp_dmars;
            }
            else
            {
                *g_dma_reg_tbl[dma_channel].dmars |= tmp_dmars << 16;
            }

            /* ---- DMA Channel Interval Register Setting ---- */
            *g_dma_reg_tbl[dma_channel].chitvl = 0;

            /* ---- DMA Channel Extension Register Setting ---- */
            *g_dma_reg_tbl[dma_channel].chext = 0;

            /* ---- Software Reset ---- */
            *g_dma_reg_tbl[dma_channel].chctrl |= DMA_CHCTRL_SWRST_BIT;
        }
    }

    return ret;

}

/*******************************************************************************
* Function Name: DMA_DmaRxInit
* Description  : DMA initialization module at the receiving operation
* Arguments    : - dma_channel
*                    DMA channel number
*                - dma_transfer_size
*                    Transfer size
* Return Value : - DEVDRV_SUCCESS
*                    DMA(Rx) Init Success
*                - DEVDRV_ERROR
*                    DMA(Rx) Init Error
*******************************************************************************/
int32_t DMA_DmaRxInit (const uint32_t dma_channel, const uint32_t dma_transfer_size)
{
    uint32_t sa;
    uint32_t tmp_dmars;
    uint32_t tmp_chcfg;
    int32_t ret;
    dma_regist_info_t *dma_info;

    dma_info = &dma_regist[dma_channel];

    /* ==== Argument Check ==== */
    ret = DEVDRV_SUCCESS;
    if (dma_channel >= DMA_CHANNEL_MAX)
    {
        ret = DEVDRV_ERROR;
    }
    else
    {
        if (DMA_NON_REGIST == dma_regist[dma_channel].module_name)
        {
            ret = DEVDRV_ERROR;
        }

        if (DEVDRV_SUCCESS == ret)
        {
            /* ==== DMA Initialize ==== */
            if (DMA_SSIF == dma_info->module_name)
            {
                /* ---- SSIF Module ---- */
                tmp_dmars = dma_ssif_dmars_reg[dma_info->module_channel][dma_info->direct];
                sa = (uint32_t)dma_ssif_da_sa_reg[dma_info->module_channel][dma_info->direct];
            }
            else
            {
                /* ---- SCUX Module ---- */
                tmp_dmars = dma_scux_dmars_reg[dma_info->module_channel][dma_info->direct];
                sa = (uint32_t)dma_scux_da_sa_reg[dma_info->module_channel][dma_info->direct];
            }

            /* ---- DMA Control Register Setting ---- */
            *g_dma_reg_tbl[dma_channel].dctrl = DMA_FIX_PRIO_MODE;

            /* ---- DMA Next0 Address Setting ---- */
            *g_dma_reg_tbl[dma_channel].n0sa = sa;

            /* ---- DMA Next1 Address Setting ---- */
            *g_dma_reg_tbl[dma_channel].n1sa = sa;

            /* ---- Transmission Side Setting ---- */
            tmp_chcfg = DMA_CHCFG_RX_BASE_INIT_VALUE & ~DMA_CHCFG_DDS_BIT_MASK;
            tmp_chcfg |= (dma_transfer_size << DMA_CHCFG_DDS_BIT_POSI);
            tmp_chcfg &= ~DMA_CHCFG_SDS_BIT_MASK;
            tmp_chcfg |= (dma_transfer_size << DMA_CHCFG_SDS_BIT_POSI);
            tmp_chcfg |= (dma_channel % 8u);
            if (true == dma_info->transfer_continue)
            {
                tmp_chcfg |= DMA_CHCFG_REN_BIT;
            }
            else
            {
                tmp_chcfg &= ~DMA_CHCFG_REN_BIT;
            }
            *g_dma_reg_tbl[dma_channel].chcfg = tmp_chcfg;

            /* ---- DMA Expansion Resource Selector Setting ---- */
            if ( (dma_channel % 2u) == 0u )
            {
                *g_dma_reg_tbl[dma_channel].dmars |= tmp_dmars;
            }
            else
            {
                *g_dma_reg_tbl[dma_channel].dmars |= tmp_dmars << 16;
            }

            /* ---- DMA Channel Interval Register Setting ---- */
            *g_dma_reg_tbl[dma_channel].chitvl = 0;

            /* ---- DMA Channel Extension Register Setting ---- */
            *g_dma_reg_tbl[dma_channel].chext = 0;

            /* ---- Software Reset ---- */
            *g_dma_reg_tbl[dma_channel].chctrl |= DMA_CHCTRL_SWRST_BIT;
        }
    }

    return ret;

}

/*******************************************************************************
* Function Name: DMA_TxRegPause
* Description  : Pause Transmission Queue to Next Register.
* Arguments    : - dma_channel
*                    DMA channel number
*                - pause_req
*                    true :Pause ON
*                    flase:Pause OFF
* Return Value : - DEVDRV_SUCCESS
*                    Pause Request Regist Success 
*                - DEVDRV_ERROR
*                    Pause Request Regist Error
*******************************************************************************/
int32_t DMA_TxRegPause (const uint32_t dma_channel, const bool_t pause_req)
{
    int32_t ret = DEVDRV_SUCCESS;

    /* ==== Argument Check ==== */
    if (dma_channel >= DMA_CHANNEL_MAX)
    {
        ret = DEVDRV_ERROR;
    }
    else if (DMA_NON_REGIST == dma_regist[dma_channel].module_name)
    {
        ret = DEVDRV_ERROR;
    }
    /* ==== Set Pause Request ==== */
    else {
        queue[dma_channel].tx_pause_flg = pause_req;
    }

    return ret;
}


/*******************************************************************************
* Function Name: DMA_TxRegSet
* Description  : Buffer register to the NEXT register from the transmit queue
* Arguments    : - dma_channel
*                    DMA channel number
*                - set_side
*                    DMA Next Register Number
* Return Value : - DEVDRV_SUCCESS
*                    DMA Source Address Register set Success
*                - DEVDRV_ERROR
*                    Argument Error
*                - DEVDRV_QUEUE_EMPTY
*                    DMA Source Address Register set Error
*******************************************************************************/
int32_t DMA_TxRegSet (const uint32_t dma_channel, const uint32_t set_side)
{
    int32_t ret;

    /* ==== Argument Check ==== */
    ret = DEVDRV_SUCCESS;
    if (dma_channel >= DMA_CHANNEL_MAX)
    {
        ret = DEVDRV_ERROR;
    }
    else
    {
        if (DMA_NON_REGIST == dma_regist[dma_channel].module_name)
        {
            ret = DEVDRV_ERROR;
        }

        if (DEVDRV_SUCCESS == ret)
        {
            if(N0_SIDE == set_side)
            {
                /* ==== Next0 Source Address Register Setting ==== */
                if((queue[dma_channel].write_num != 0u) && (false == queue[dma_channel].tx_pause_flg))
                {
                    *g_dma_reg_tbl[dma_channel].n0sa = queue[dma_channel].ads[queue[dma_channel].rpoint];
                    *g_dma_reg_tbl[dma_channel].n0tb = queue[dma_channel].size[queue[dma_channel].rpoint];
                    queue[dma_channel].rpoint++;
                    if (queue[dma_channel].rpoint >= QUEUE_NUM) {
                        queue[dma_channel].rpoint = 0u;
                    }
                    queue[dma_channel].write_num--;
                    queue[dma_channel].n0_side = ON;
                }
                else
                {
                    *g_dma_reg_tbl[dma_channel].n0sa = (uint32_t)&mute_buf;
                    *g_dma_reg_tbl[dma_channel].n0tb = sizeof(mute_buf);
                    ret = DEVDRV_QUEUE_EMPTY;
                }
            }
            else if(N1_SIDE == set_side)
            {
                /* ==== Next1 Source Address Register Setting ==== */
                if((queue[dma_channel].write_num != 0u) && (false == queue[dma_channel].tx_pause_flg))
                {
                    *g_dma_reg_tbl[dma_channel].n1sa = queue[dma_channel].ads[queue[dma_channel].rpoint];
                    *g_dma_reg_tbl[dma_channel].n1tb = queue[dma_channel].size[queue[dma_channel].rpoint];
                    queue[dma_channel].rpoint++;
                    if (queue[dma_channel].rpoint >= QUEUE_NUM) {
                        queue[dma_channel].rpoint = 0u;
                    }
                    queue[dma_channel].write_num--;
                    queue[dma_channel].n1_side = ON;
                }
                else
                {
                    *g_dma_reg_tbl[dma_channel].n1sa = (uint32_t)&mute_buf;
                    *g_dma_reg_tbl[dma_channel].n1tb = sizeof(mute_buf);
                    ret = DEVDRV_QUEUE_EMPTY;
                }
            }
            else
            {
                /* Do Nothing */
            }
        }
    }

    return ret;

}

/*******************************************************************************
* Function Name: DMA_RxRegSet
* Description  : Buffer register to the NEXT register from the receive queue
* Arguments    : - dma_channel
*                    DMA channel number
*                - set_side
*                    DMA Next Register Number
* Return Value : - DEVDRV_SUCCESS
*                    DMA Destination Address Register set Success 
*                - DEVDRV_ERROR
*                    Argument Err
*                - DEVDRV_QUEUE_EMPTY
*                    DMA Destination Address Register set Error
*******************************************************************************/
int32_t DMA_RxRegSet (const uint32_t dma_channel, const uint32_t set_side)
{
    int32_t ret;

    /* ==== Argument Check ==== */
    ret = DEVDRV_SUCCESS;
    if (dma_channel >= DMA_CHANNEL_MAX)
    {
        ret = DEVDRV_ERROR;
    }
    else
    {
        if (DMA_NON_REGIST == dma_regist[dma_channel].module_name)
        {
            ret = DEVDRV_ERROR;
        }

        if (DEVDRV_SUCCESS == ret)
        {
            if(N0_SIDE == set_side)
            {
                /* ==== Next0 Destination Address Register Setting ==== */
                if(0u != queue[dma_channel].write_num)
                {
                    queue[dma_channel].last_ads = queue[dma_channel].ads[queue[dma_channel].rpoint];
                    queue[dma_channel].last_size = queue[dma_channel].size[queue[dma_channel].rpoint];
                    *g_dma_reg_tbl[dma_channel].n0da = queue[dma_channel].ads[queue[dma_channel].rpoint];
                    *g_dma_reg_tbl[dma_channel].n0tb = queue[dma_channel].size[queue[dma_channel].rpoint];
                    queue[dma_channel].rpoint++;
                    if (queue[dma_channel].rpoint >= QUEUE_NUM) {
                        queue[dma_channel].rpoint = 0u;
                    }
                    queue[dma_channel].write_num--;
                    queue[dma_channel].n0_side = ON;
                }
                else
                {
                    if (0u != queue[dma_channel].last_ads)
                    {
                        *(g_dma_reg_tbl[dma_channel].n0da) = queue[dma_channel].last_ads;
                        *(g_dma_reg_tbl[dma_channel].n0tb) = queue[dma_channel].last_size;
                    }
                    ret = DEVDRV_QUEUE_EMPTY;
                }
            }
            else if(N1_SIDE == set_side)
            {
                /* ==== Next1 Destination Address Register Setting ==== */
                if(queue[dma_channel].write_num != 0u)
                {
                    queue[dma_channel].last_ads = queue[dma_channel].ads[queue[dma_channel].rpoint];
                    queue[dma_channel].last_size = queue[dma_channel].size[queue[dma_channel].rpoint];
                    *g_dma_reg_tbl[dma_channel].n1da = queue[dma_channel].ads[queue[dma_channel].rpoint];
                    *g_dma_reg_tbl[dma_channel].n1tb = queue[dma_channel].size[queue[dma_channel].rpoint];
                    queue[dma_channel].rpoint++;
                    if (queue[dma_channel].rpoint >= QUEUE_NUM) {
                        queue[dma_channel].rpoint = 0;
                    }
                    queue[dma_channel].write_num--;
                    queue[dma_channel].n1_side = ON;
                }
                else
                {
                    if (0u != queue[dma_channel].last_ads)
                    {
                        *(g_dma_reg_tbl[dma_channel].n1da) = queue[dma_channel].last_ads;
                        *(g_dma_reg_tbl[dma_channel].n1tb) = queue[dma_channel].last_size;
                    }
                    ret = DEVDRV_QUEUE_EMPTY;
                }
            }
            else
            {
                /* Do Nothing */
            }
        }
    }

    return ret;

}

/*******************************************************************************
* Function Name: DMA_CheckDmaMute
* Description  : Check output data from the DMA
* Arguments    : - dma_channel
*                    DMA channel number
*                - mute
*                    Mute status
*                      true :Mute
*                      false:No Mute
* Return Value : - DEVDRV_SUCCESS
*                    Completion of a check of output data
*                - DEVDRV_ERROR
*                    Argument Error
*******************************************************************************/
int32_t DMA_CheckDmaMute (const uint32_t dma_channel, bool_t * const mute)
{
    int32_t ret;

    /* ==== Argument Check ==== */
    ret = DEVDRV_SUCCESS;
    if (dma_channel >= DMA_CHANNEL_MAX)
    {
        ret = DEVDRV_ERROR;
    }
    else
    {
        if (DMA_NON_REGIST == dma_regist[dma_channel].module_name)
        {
            ret = DEVDRV_ERROR;
        }

        if (NULL == mute)
        {
            ret = DEVDRV_ERROR;
        }
        else
        {
            if (DEVDRV_SUCCESS == ret)
            {
                if ( (*g_dma_reg_tbl[dma_channel].n0sa == (uint32_t)&mute_buf) &&
                     (*g_dma_reg_tbl[dma_channel].n1sa == (uint32_t)&mute_buf) )
                {
                    *mute = true;
                }
                else
                {
                    *mute = false;
                }
            }
        }
    }

    return ret;

}

/*******************************************************************************
* Function Name: DmaInt
* Description  : DMA transfer end interrupt
*                DMA Source,Destination Address updata
* Arguments    : - dma_channel
*                    DMA channel number
* Return Value : none
*******************************************************************************/
static int32_t DmaInt (const uint32_t dma_channel)
{
    void *   pbuf;
    int32_t ret;
    dma_regist_info_t *dma_info;

    dma_info = &dma_regist[dma_channel];
    pbuf = 0;

    ret = DEVDRV_SUCCESS;
    if (DMA_AUDIO_TX == dma_info->direct)
    {
        if (N0_SIDE == queue[dma_channel].txside)
        {
            /* N0 side transfer completion -> set data in an N0 side */
            if ( ((uint32_t)*g_dma_reg_tbl[dma_channel].n0sa != (uint32_t)&mute_buf) )
            {
                pbuf = (void *)*g_dma_reg_tbl[dma_channel].n0sa;
            }
            queue[dma_channel].n0_side = OFF;
            ret = DMA_TxRegSet(dma_channel, N0_SIDE);
            queue[dma_channel].txside = N1_SIDE;
        }
        else
        {
            /* N1 side transfer completion -> set data in an N1 side */
            if ( ((uint32_t)*g_dma_reg_tbl[dma_channel].n1sa != (uint32_t)&mute_buf) )
            {
                pbuf = (void *)*g_dma_reg_tbl[dma_channel].n1sa;
            }
            queue[dma_channel].n1_side = OFF;
            ret = DMA_TxRegSet(dma_channel, N1_SIDE);
            queue[dma_channel].txside = N0_SIDE;
        }
    }
    else if (DMA_AUDIO_RX == dma_info->direct)
    {
        if (N0_SIDE == queue[dma_channel].txside)
        {
            /* N0 side transfer completion -> set data in an N0 side */
            queue[dma_channel].n0_side = OFF;
            pbuf = (void *)*g_dma_reg_tbl[dma_channel].n0da;
            ret = DMA_RxRegSet(dma_channel, N0_SIDE);
            queue[dma_channel].txside = N1_SIDE;
        }
        else
        {
            queue[dma_channel].n1_side = OFF;
            pbuf = (void *)*g_dma_reg_tbl[dma_channel].n1da;
            ret = DMA_RxRegSet(dma_channel, N1_SIDE);
            queue[dma_channel].txside = N0_SIDE;
        }
    }
    else
    {
        /* ERR */
    }

    /* User Function Call */
    if ((dma_info->user_trnend_func != NULL) && (pbuf != 0))
    {
        dma_info->user_trnend_func(pbuf);
    }

    /* Set Consecutive Transfer */
    if (true == dma_info->transfer_continue)
    {
        *g_dma_reg_tbl[dma_channel].chcfg |= DMA_CHCFG_REN_BIT;
    }

    /* Set Register Select Switch */
    *g_dma_reg_tbl[dma_channel].chcfg |= DMA_CHCFG_RSW_BIT;

    /* Clear Transfer End Bit Status */
    *g_dma_reg_tbl[dma_channel].chctrl |= (DMA_CHCTRL_CLREND_BIT | DMA_CHCTRL_CLRTC_BIT);
    *g_dma_reg_tbl[dma_channel].chstat; /* Dummy read */

    /* Permit Transfer Int */
    *g_dma_reg_tbl[dma_channel].chcfg &= ~DMA_CHCFG_DEM_BIT;

    return ret;

}

/*******************************************************************************
* Function Name: DMA0_INT_TrnEnd
* Description  : DMA channel 0 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA0_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL0);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: DMA1_INT_TrnEnd
* Description  : DMA channel 1 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA1_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL1);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: DMA2_INT_TrnEnd
* Description  : DMA channel 2 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA2_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL2);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: DMA3_INT_TrnEnd
* Description  : DMA channel 3 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA3_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL3);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: DMA4_INT_TrnEnd
* Description  : DMA channel 4 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA4_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL4);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: DMA5_INT_TrnEnd
* Description  : DMA channel 5 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA5_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL5);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: DMA6_INT_TrnEnd
* Description  : DMA channel 6 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA6_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL6);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: DMA7_INT_TrnEnd
* Description  : DMA channel 7 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA7_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL7);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: DMA8_INT_TrnEnd
* Description  : DMA channel 8 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA8_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL8);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: DMA9_INT_TrnEnd
* Description  : DMA channel 9 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA9_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL9);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: DMA10_INT_TrnEnd
* Description  : DMA channel 10 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA10_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL10);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: DMA11_INT_TrnEnd
* Description  : DMA channel 11 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA11_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL11);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: DMA12_INT_TrnEnd
* Description  : DMA channel 12 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA12_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL12);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: DMA13_INT_TrnEnd
* Description  : DMA channel 13 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA13_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL13);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: DMA14_INT_TrnEnd
* Description  : DMA channel 14 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA14_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL14);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: DMA15_INT_TrnEnd
* Description  : DMA channel 15 transfer end interrupt
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void DMA15_INT_TrnEnd (uint32_t int_sense)
{
    int32_t ret;

    ret = DmaInt(DMA_CHANNEL15);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}


/* End of File */
