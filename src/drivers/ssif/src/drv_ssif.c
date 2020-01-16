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
* File Name   : drv_ssif.c
* $Rev: 392 $
* $Date:: 2013-08-02 19:56:33 +0900#$
* Description  : This module is sample of SSIF driver.
******************************************************************************/

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_intc.h"            /* INTC Driver header */
#include "reg_bit_access.h"
#include "drv_ssif.h"
#include "drv_ssif_user.h"
#include "sample_audio_dma.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define NON_REGIST                          (0xFFFFFFFFu)

/* INT Prohibition */
#define MASK_INT_LV                         (0)

/* INT ID (SSI) */
#define SSI_ID_SSII0                        (uint16_t)(172)
#define SSI_ID_SSII1                        (uint16_t)(175)
#define SSI_ID_SSII2                        (uint16_t)(178)
#define SSI_ID_SSII3                        (uint16_t)(180)
#define SSI_ID_SSII4                        (uint16_t)(183)
#define SSI_ID_SSII5                        (uint16_t)(185)

/* Register Setting Value */
#define SSI_CLEAR_VALUE                     (0)
#define SSI_SET_VALUE                       (1)
#define SSI_WSET_VALUE                      (3)

#define SSI_TX_ENABLE                       (0x00000002u)
#define SSI_RX_ENABLE                       (0x00000001u)
enum ssi_ssicr_bit_inf
{
   SSI_SSICR_TUIEN_BIT
 , SSI_SSICR_ROIEN_BIT
 , SSI_SSICR_TEN_BIT
 , SSI_SSICR_REN_BIT
 , SSI_SSICR_TENREN_BIT
 , SSI_SSICR_NUM_MAX
};

#define SSI_SSICR_TUIEN_INIT_VALUE          (0x00000000uL)
#define SSI_SSICR_TOIEN_INIT_VALUE          (0x00000000uL)
#define SSI_SSICR_RUIEN_INIT_VALUE          (0x00000000uL)
#define SSI_SSICR_ROIEN_INIT_VALUE          (0x00000000uL)
#define SSI_SSICR_IIEN_INIT_VALUE           (0x00000000uL)
#define SSI_SSICR_MUEN_INIT_VALUE           (0x00000000uL)
#define SSI_SSICR_TEN_INIT_VALUE            (0x00000000uL)
#define SSI_SSICR_REN_INIT_VALUE            (0x00000000uL)
#define SSI_SSICR_BASE_INIT_VALUE           (SSI_SSICR_TUIEN_INIT_VALUE | \
                                             SSI_SSICR_TOIEN_INIT_VALUE | \
                                             SSI_SSICR_RUIEN_INIT_VALUE | \
                                             SSI_SSICR_ROIEN_INIT_VALUE | \
                                             SSI_SSICR_IIEN_INIT_VALUE  | \
                                             SSI_SSICR_MUEN_INIT_VALUE  | \
                                             SSI_SSICR_TEN_INIT_VALUE   | \
                                             SSI_SSICR_REN_INIT_VALUE)
/*  [29]    TUIEN   : B'0    : Disables an underflow interrupt              */
/*  [28]    TOIEN   : B'0    : Disables an overflow interrupt               */
/*  [27]    RUIEN   : B'0    : Disables an underflow interrupt              */
/*  [26]    ROIEN   : B'0    : Receive overflow interrupt enable            */
/*  [25]    IIEN    : B'0    : Disables an idle mode interrupt              */
/*  [3]     MUEN    : B'0    : This module is not muted                     */
/*  [1]     TEN     : B'0    : Disables the transmit operation              */
/*  [0]     REN     : B'0    : Disables the receive operation               */

enum ssi_ssifcr_bit_inf
{
   SSI_SSIFCR_TIE_BIT
 , SSI_SSIFCR_RIE_BIT
 , SSI_SSIFCR_TFRST_BIT
 , SSI_SSIFCR_RFRST_BIT
 , SSI_SSIFCR_NUM_MAX
};
#define SSI_SSIFCR_TIE_INIT_VALUE           (0x00000000uL)
#define SSI_SSIFCR_RIE_INIT_VALUE           (0x00000000uL)
#define SSI_SSIFCR_TFRST_INIT_VALUE         (0x00000002uL)
#define SSI_SSIFCR_RFRST_INIT_VALUE         (0x00000001uL)
#define SSI_SSIFCR_BASE_INIT_VALUE          (SSI_SSIFCR_TIE_INIT_VALUE   | \
                                             SSI_SSIFCR_RIE_INIT_VALUE   | \
                                             SSI_SSIFCR_TFRST_INIT_VALUE | \
                                             SSI_SSIFCR_RFRST_INIT_VALUE)
/*  [3]   TIE   : B'0  : Transmit data empty interrupt (TXI) request is disabled    */
/*  [2]   RIE   : B'0  : Receive data full interrupt (RXI) request is disabled      */
/*  [1]   TFRST : B'1  : Reset is enabled                                           */
/*  [0]   RFRST : B'1  : Reset is enabled                                           */

/* SSI TDM Mode Register Setting */
#define SSI_SSITDMR_RXDMUTE_INIT_VALUE      (0x00000000uL)
#define SSI_SSITDMR_TDM_INIT_VALUE          (0x00000000uL)
#define SSI_SSITDMR_BASE_INIT_VALUE         (SSI_SSITDMR_RXDMUTE_INIT_VALUE | \
                                             SSI_SSITDMR_TDM_INIT_VALUE)
/*  [17] RXDMUTE  : B'0 : Rx Direct Data MUTE : Rx Direct Data                      */
/*   [0] TDM      : B'0 : TDM Mode : Disables TDM mode                              */

/* SSISR Register */
enum ssi_ssisr_bit_inf
{
   SSI_SSISR_TUIRQ_BIT
 , SSI_SSISR_ROIRQ_BIT
 , SSI_SSISR_IIRQ_BIT
 , SSI_SSISR_NUM_MAX
};
#define SSI_IDLE                            (0x02000000u)


/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* ====  Prototype function ==== */
/* SSI ERR function */
static void SSI_SSI0_INT_Err(uint32_t int_sense);
static void SSI_SSI1_INT_Err(uint32_t int_sense);
static void SSI_SSI2_INT_Err(uint32_t int_sense);
static void SSI_SSI3_INT_Err(uint32_t int_sense);
static void SSI_SSI4_INT_Err(uint32_t int_sense);
static void SSI_SSI5_INT_Err(uint32_t int_sense);
/* SSI local function */
static int32_t SsiErrInt(uint32_t ssi_channel);

typedef struct
{
    uint32_t    txdma_channel;      /* DMA registration situation */
    uint32_t    rxdma_channel;      /* DMA registration situation */
} ssi_regist_info_t;

/* ==== SSI Register Table ==== */
typedef struct{
    volatile uint32_t *ssicr;       /* control register(SSICR) */
    volatile uint32_t *ssifcr;      /* FIFO control register(SSIFCR) */
    volatile uint32_t *ssisr;       /* status register(SSISR) */
    volatile uint32_t *ssifsr;      /* FIFO status register(SSIFSR) */
    volatile uint32_t *ssiftdr;     /* Tx FIFO data register(SSIFTDR) */
    volatile uint32_t *ssifrdr;     /* Rx FIFO data register(SSIFRDR) */
    volatile uint32_t *ssitdmr;     /* TDM mode register(SSITDMR) */
} ssif_reg_t;

static const ssif_reg_t ssif[SSI_CHANNEL_MAX]={
    {   /* ch0 */
        &SSIF0.SSICR,     /* control register(SSICR) */
        &SSIF0.SSIFCR,    /* FIFO control register(SSIFCR) */
        &SSIF0.SSISR,     /* status register(SSISR) */
        &SSIF0.SSIFSR,    /* FIFO status register(SSIFSR) */
        &SSIF0.SSIFTDR,   /* Tx FIFO data register(SSIFTDR) */
        &SSIF0.SSIFRDR,   /* Rx FIFO data register(SSIFRDR) */
        &SSIF0.SSITDMR    /* TDM mode register(SSITDMR) */
    },
    {   /* ch1 */
        &SSIF1.SSICR,     /* control register(SSICR) */
        &SSIF1.SSIFCR,    /* FIFO control register(SSIFCR) */
        &SSIF1.SSISR,     /* status register(SSISR) */
        &SSIF1.SSIFSR,    /* FIFO status register(SSIFSR) */
        &SSIF1.SSIFTDR,   /* Tx FIFO data register(SSIFTDR) */
        &SSIF1.SSIFRDR,   /* Rx FIFO data register(SSIFRDR) */
        &SSIF1.SSITDMR    /* TDM mode register(SSITDMR) */
    },
    {   /* ch2 */
        &SSIF2.SSICR,     /* control register(SSICR) */
        &SSIF2.SSIFCR,    /* FIFO control register(SSIFCR) */
        &SSIF2.SSISR,     /* status register(SSISR) */
        &SSIF2.SSIFSR,    /* FIFO status register(SSIFSR) */
        &SSIF2.SSIFTDR,   /* Tx FIFO data register(SSIFTDR) */
        &SSIF2.SSIFRDR,   /* Rx FIFO data register(SSIFRDR) */
        &SSIF2.SSITDMR    /* TDM mode register(SSITDMR) */
    },
    {   /* ch3 */
        &SSIF3.SSICR,     /* control register(SSICR) */
        &SSIF3.SSIFCR,    /* FIFO control register(SSIFCR) */
        &SSIF3.SSISR,     /* status register(SSISR) */
        &SSIF3.SSIFSR,    /* FIFO status register(SSIFSR) */
        &SSIF3.SSIFTDR,   /* Tx FIFO data register(SSIFTDR) */
        &SSIF3.SSIFRDR,   /* Rx FIFO data register(SSIFRDR) */
        &SSIF3.SSITDMR    /* TDM mode register(SSITDMR) */
    },
    {   /* ch4 */
        &SSIF4.SSICR,     /* control register(SSICR) */
        &SSIF4.SSIFCR,    /* FIFO control register(SSIFCR) */
        &SSIF4.SSISR,     /* status register(SSISR) */
        &SSIF4.SSIFSR,    /* FIFO status register(SSIFSR) */
        &SSIF4.SSIFTDR,   /* Tx FIFO data register(SSIFTDR) */
        &SSIF4.SSIFRDR,   /* Rx FIFO data register(SSIFRDR) */
        &SSIF4.SSITDMR    /* TDM mode register(SSITDMR) */
    },
    {   /* ch5 */
        &SSIF5.SSICR,     /* control register(SSICR) */
        &SSIF5.SSIFCR,    /* FIFO control register(SSIFCR) */
        &SSIF5.SSISR,     /* status register(SSISR) */
        &SSIF5.SSIFSR,    /* FIFO status register(SSIFSR) */
        &SSIF5.SSIFTDR,   /* Tx FIFO data register(SSIFTDR) */
        &SSIF5.SSIFRDR,   /* Rx FIFO data register(SSIFRDR) */
        &SSIF5.SSITDMR    /* TDM mode register(SSITDMR) */
    }
};

/* SSI INT ID Table */
static const uint16_t ssi_int_id[SSI_CHANNEL_MAX] = 
{
   SSI_ID_SSII0
 , SSI_ID_SSII1
 , SSI_ID_SSII2
 , SSI_ID_SSII3
 , SSI_ID_SSII4
 , SSI_ID_SSII5
};

/* SSICR Register Table */
static const uint32_t ssi_ssicr_reg[SSI_CHANNEL_MAX] = 
{
    SSI_SSICR_BASE_INIT_VALUE | SSI_SSICR0_USER_INIT_VALUE
 ,  SSI_SSICR_BASE_INIT_VALUE | SSI_SSICR1_USER_INIT_VALUE
 ,  SSI_SSICR_BASE_INIT_VALUE | SSI_SSICR2_USER_INIT_VALUE
 ,  SSI_SSICR_BASE_INIT_VALUE | SSI_SSICR3_USER_INIT_VALUE
 ,  SSI_SSICR_BASE_INIT_VALUE | SSI_SSICR4_USER_INIT_VALUE
 ,  SSI_SSICR_BASE_INIT_VALUE | SSI_SSICR5_USER_INIT_VALUE
};

/* SSIFCR Register Table */
static const uint32_t ssi_ssifcr_reg[SSI_CHANNEL_MAX] = 
{
    SSI_SSIFCR_BASE_INIT_VALUE | SSI_SSIFCR0_USER_INIT_VALUE
 ,  SSI_SSIFCR_BASE_INIT_VALUE | SSI_SSIFCR1_USER_INIT_VALUE
 ,  SSI_SSIFCR_BASE_INIT_VALUE | SSI_SSIFCR2_USER_INIT_VALUE
 ,  SSI_SSIFCR_BASE_INIT_VALUE | SSI_SSIFCR3_USER_INIT_VALUE
 ,  SSI_SSIFCR_BASE_INIT_VALUE | SSI_SSIFCR4_USER_INIT_VALUE
 ,  SSI_SSIFCR_BASE_INIT_VALUE | SSI_SSIFCR5_USER_INIT_VALUE
};

/* SSITDMR Register Table */
static const uint32_t ssi_ssitdmr_reg[SSI_CHANNEL_MAX] = 
{
    SSI_SSITDMR_BASE_INIT_VALUE | SSI_SSITDMR0_USER_INIT_VALUE
 ,  SSI_SSITDMR_BASE_INIT_VALUE | SSI_SSITDMR1_USER_INIT_VALUE
 ,  SSI_SSITDMR_BASE_INIT_VALUE | SSI_SSITDMR2_USER_INIT_VALUE
 ,  SSI_SSITDMR_BASE_INIT_VALUE | SSI_SSITDMR3_USER_INIT_VALUE
 ,  SSI_SSITDMR_BASE_INIT_VALUE | SSI_SSITDMR4_USER_INIT_VALUE
 ,  SSI_SSITDMR_BASE_INIT_VALUE | SSI_SSITDMR5_USER_INIT_VALUE
};

/* SWRSTCR1 Register Bit Table */
static const uint8_reg_bit_inf_t ssi_swrstcr1_bit_tbl[SSI_CHANNEL_MAX] = 
{
   { 0x40   ,   6   }   /* SSI_SWRSTCR1_SRST16_BIT */
 , { 0x20   ,   5   }   /* SSI_SWRSTCR1_SRST15_BIT */
 , { 0x10   ,   4   }   /* SSI_SWRSTCR1_SRST14_BIT */
 , { 0x08   ,   3   }   /* SSI_SWRSTCR1_SRST13_BIT */
 , { 0x04   ,   2   }   /* SSI_SWRSTCR1_SRST12_BIT */
 , { 0x02   ,   1   }   /* SSI_SWRSTCR1_SRST11_BIT */
};

/* SSIFCR Register Bit Table */
static const uint32_reg_bit_inf_t ssi_ssifcr_bit_tbl[SSI_SSIFCR_NUM_MAX] = 
{
   { 0x00000008u   ,    3   }   /* SSI_SSIFCR_TIE_BIT   */
 , { 0x00000004u   ,    2   }   /* SSI_SSIFCR_RIE_BIT   */
 , { 0x00000002u   ,    1   }   /* SSI_SSIFCR_TFRST_BIT */
 , { 0x00000001u   ,    0   }   /* SSI_SSIFCR_RFRST_BIT */
};

/* SSISR Register Bit Table */
static const uint32_reg_bit_inf_t ssi_ssisr_bit_tbl[SSI_SSISR_NUM_MAX] = 
{
   { 0x20000000u    ,   29  }   /* SSI_SSISR_TUIRQ_BIT   */
 , { 0x04000000u    ,   26  }   /* SSI_SSISR_ROIRQ_BIT   */
 , { 0x02000000u    ,   26  }   /* SSI_SSISR_IIRQ_BIT    */
};

/* SSICR Register Bit Table */
static const uint32_reg_bit_inf_t ssi_ssicr_bit_tbl[SSI_SSICR_NUM_MAX] = 
{
   { 0x20000000u    ,   29  }   /* SSI_SSICR_TUIEN_BIT  */
 , { 0x04000000u    ,   26  }   /* SSI_SSICR_ROIEN_BIT  */
 , { 0x00000002u    ,   1   }   /* SSI_SSICR_TEN_BIT    */
 , { 0x00000001u    ,   0   }   /* SSI_SSICR_REN_BIT    */
 , { 0x00000003u    ,   0   }   /* SSI_SSICR_TENREN_BIT */
};

/* SSIF Regist Information */
static ssi_regist_info_t ssi_regist_info[SSI_CHANNEL_MAX];


/*******************************************************************************
* Function Name: R_SSI_RamInit
* Description  : Initialization of the internal management variable
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_SSI_RamInit(void)
{

    uint32_t counter;
    uint8_t mask;

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* Initialize Management Information */
    for (counter = 0; counter < SSI_CHANNEL_MAX; counter++)
    {
        ssi_regist_info[counter].txdma_channel = NON_REGIST;
        ssi_regist_info[counter].rxdma_channel = NON_REGIST;
    }

    /* Initialize DMAC Management Infomation */
    DMA_DmaInit();

    (void)R_INTC_SetMaskLevel(mask);
}

/*******************************************************************************
* Function Name: R_SSI_SsiInit
* Description  : Initialization SSI modules
* Arguments    : ssi_channel -
*                    SSI channel number
* Return Value : DEVDRV_SUCCESS -
*                    Operation successful.
*                DEVDRV_ERROR -
*                    Operation error.
*******************************************************************************/
int32_t R_SSI_SsiInit(const uint32_t ssi_channel)
{
    int32_t ret;
    uint8_t mask;

    /* SSI ERR INT Func Table */
    static void (* ssi_int_err[SSI_CHANNEL_MAX])(uint32_t int_sense) =
    {
       &SSI_SSI0_INT_Err
     , &SSI_SSI1_INT_Err
     , &SSI_SSI2_INT_Err
     , &SSI_SSI3_INT_Err
     , &SSI_SSI4_INT_Err
     , &SSI_SSI5_INT_Err
    };

    /* Pin Setting Function Table */
    static void (*SSI_UserdefSsiPinSet[SSI_CHANNEL_MAX])( void ) =
    {
       &SSI_UserdefSsi0PinSet
     , &SSI_UserdefSsi1PinSet
     , &SSI_UserdefSsi2PinSet
     , &SSI_UserdefSsi3PinSet
     , &SSI_UserdefSsi4PinSet
     , &SSI_UserdefSsi5PinSet
    };

    /* STBCR11 Register Bit Table */
    static const uint8_reg_bit_inf_t ssi_stbcr11_bit_tbl[SSI_CHANNEL_MAX] = 
    {
       { 0x20   ,   5   }   /* SSI_STBCR11_MSTP115_BIT */
     , { 0x10   ,   4   }   /* SSI_STBCR11_MSTP114_BIT */
     , { 0x08   ,   3   }   /* SSI_STBCR11_MSTP113_BIT */
     , { 0x04   ,   2   }   /* SSI_STBCR11_MSTP112_BIT */
     , { 0x02   ,   1   }   /* SSI_STBCR11_MSTP111_BIT */
     , { 0x01   ,   0   }   /* SSI_STBCR11_MSTP110_BIT */
    };

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* ==== Argument Check ==== */
    if (ssi_channel >= SSI_CHANNEL_MAX)
    {
        /* ---- Argument NG ---- */
        ret = DEVDRV_ERROR;
    }
    else
    {
        /* ==== SSI Initialize ==== */
        /* ---- SSI Clock supply ---- */
        COMMON_Set8BitMaskLShift(&CPG.STBCR11,
                                 &ssi_stbcr11_bit_tbl[ssi_channel],
                                 SSI_CLEAR_VALUE);
        CPG.STBCR11;    /* Dummy read */

        /* ---- SSI Software reset ---- */
        COMMON_Set8BitMaskLShift(&CPG.SWRSTCR1,
                                 &ssi_swrstcr1_bit_tbl[ssi_channel],
                                 SSI_SET_VALUE);
        CPG.SWRSTCR1;   /* Dummy read */
        COMMON_Set8BitMaskLShift(&CPG.SWRSTCR1,
                                 &ssi_swrstcr1_bit_tbl[ssi_channel],
                                 SSI_CLEAR_VALUE);
        CPG.SWRSTCR1;   /* Dummy read */

        /* ---- SSI Pin Setting ---- */
        SSI_UserdefSsiPinSet[ssi_channel]();

        /* ---- SSI TDM Mode Register Setting ---- */
        *ssif[ssi_channel].ssitdmr = ssi_ssitdmr_reg[ssi_channel];

        /* ---- SSI Control Register Setting ---- */
        *ssif[ssi_channel].ssicr = ssi_ssicr_reg[ssi_channel];

        /* ---- SSI FIFO Control Register Setting ---- */
        *ssif[ssi_channel].ssifcr = ssi_ssifcr_reg[ssi_channel];

        /* ---- Set Interrupt Function ---- */
        ret = R_INTC_RegistIntFunc(ssi_int_id[ssi_channel], ssi_int_err[ssi_channel]);

        /* ---- Set Interrupt Priority ---- */
        if (DEVDRV_SUCCESS == ret)
        {
            ret = R_INTC_SetPriority(ssi_int_id[ssi_channel], SSI_ERR_INT_LV);
        }
    }


    (void)R_INTC_SetMaskLevel(mask);

    return ret;
}

/*******************************************************************************
* Function Name: R_SSI_DmaInit
* Description  : Initialization DMAC modules
* Arguments    : ssi_channel -
*                    SSI channel number
*                dma_channel -
*                    DMAC channel number
*                direct -
*                    Transmission and reception direction
*                func -
*                    DMAC transfer end interrupt function
* Return Value : DEVDRV_SUCCESS -
*                    Operation successful.
*                DEVDRV_ERROR -
*                    Operation error.
*******************************************************************************/
int32_t R_SSI_DmaInit(const uint32_t ssi_channel,
                      const uint32_t dma_channel,
                      const uint32_t direct,
                      const fp_type func)
{
    uint32_t counter;
    int32_t ret;
    dma_regist_info_t regist_inf;
    uint8_t mask;

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* ==== Argument Check ==== */
    if (ssi_channel >= SSI_CHANNEL_MAX)
    {
        ret = DEVDRV_ERROR;
    }
    else
    {
        /* ==== Stop DMA ==== */
        ret = DMA_DmaStop(dma_channel);

        /* ==== Setup DMA ==== */
        if (DEVDRV_SUCCESS == ret)
        {
            /* ---- Set DMA Parameter ---- */
            if (SSI_TX == direct)
            {
                regist_inf.direct = DMA_AUDIO_TX;
            }
            else
            {
                regist_inf.direct = DMA_AUDIO_RX;
            }
            regist_inf.module_name = DMA_SSIF;
            regist_inf.module_channel = ssi_channel;
            regist_inf.transfer_continue = true;
            regist_inf.user_trnend_func = func;

            /* ---- Setup DMA  ---- */
            ret = DMA_DmaSetup(dma_channel, &regist_inf);
        }

        /* ==== DMA Initialize ==== */
        if (DEVDRV_SUCCESS == ret)
        {
            if (SSI_TX == direct)
            {
                /* ---- DMA Initialize (Tx) ---- */
                ret = DMA_DmaTxInit(dma_channel, DMA_TRANSFER_SIZE_32);
            }
            else
            {
                /* ---- DMA Initialize (Rx) ---- */
                ret = DMA_DmaRxInit(dma_channel, DMA_TRANSFER_SIZE_32);
            }
        }

        /* ==== DMA Channel Info Register ==== */
        if (DEVDRV_SUCCESS == ret)
        {
            for (counter = 0; counter < SSI_CHANNEL_MAX; counter++)
            {
                if (ssi_regist_info[counter].txdma_channel == dma_channel)
                {
                    ret = DEVDRV_ERROR;
                }
                if (ssi_regist_info[counter].rxdma_channel == dma_channel)
                {
                    ret = DEVDRV_ERROR;
                }
            }
            if (DEVDRV_SUCCESS == ret)
            {
                if (SSI_TX == direct)
                {
                    ssi_regist_info[ssi_channel].txdma_channel = dma_channel;
                }
                else
                {
                    ssi_regist_info[ssi_channel].rxdma_channel = dma_channel;
                }
            }
        }
    }

    (void)R_INTC_SetMaskLevel(mask);

    return ret;
}

/*******************************************************************************
* Function Name: R_SSI_DmaStart
* Description  : In the case of transmission setting, transfer data to an SSI module
*                after the start of the DMA module.
*                In the case of reception setting, I receive data from an SSI module
*                after the start of the DMA module.
* Arguments    : ssi_channel -
*                    SSI channel number
*                direct -
*                    Transmission and reception direction
* Return Value : DEVDRV_SUCCESS -
*                    Operation successful.
*                DEVDRV_ERROR -
*                    Operation error.
*                DEVDRV_QUEUE_EMPTY -
*                    Queue empty.
*******************************************************************************/
int32_t R_SSI_DmaStart(const uint32_t ssi_channel, const uint32_t direct)
{
    uint32_t txdma_ch;
    uint32_t rxdma_ch;
    int32_t ret;
    uint8_t mask;

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    txdma_ch = NON_REGIST;
    rxdma_ch = NON_REGIST;

    /* ==== Argument Check ==== */
    ret = DEVDRV_SUCCESS;
    if (ssi_channel >= SSI_CHANNEL_MAX)
    {
        /* ---- Argument NG ---- */
        ret = DEVDRV_ERROR;
    }
    else
    {
        /* ==== SET Source Address Register(Tx) ==== */
        if (SSI_TX == (direct & SSI_TX))
        {
            txdma_ch = ssi_regist_info[ssi_channel].txdma_channel;
            if (NON_REGIST == txdma_ch)
            {
                ret = DEVDRV_ERROR;
            }
            else
            {
                /* ---- Next0 Source Address Register Setting ---- */
                if (DMA_TxRegSet(txdma_ch, N0_SIDE) == DEVDRV_ERROR)
                {
                    ret = DEVDRV_ERROR;
                }
                /* ---- Next1 Source Address Register Setting ---- */
                if (DMA_TxRegSet(txdma_ch, N1_SIDE) == DEVDRV_ERROR)
                {
                    ret = DEVDRV_ERROR;
                }
            }
        }

        /* ==== SET Destination Address Register(Rx) ==== */
        if (DEVDRV_SUCCESS == ret)
        {
            if (SSI_RX == (direct & SSI_RX))
            {
                rxdma_ch = ssi_regist_info[ssi_channel].rxdma_channel;
                if (NON_REGIST == rxdma_ch)
                {
                    ret = DEVDRV_ERROR;
                }
                else
                {
                    /* ---- Next0 Destination Address Register Setting ---- */
                    ret = DMA_RxRegSet(rxdma_ch, N0_SIDE);
                    if (DEVDRV_SUCCESS != ret)
                    {
                        ret = DEVDRV_ERROR;
                    }
                    /* ---- Next1 Destination Address Register Setting ---- */
                    if (DEVDRV_SUCCESS == ret)
                    {
                        ret = DMA_RxRegSet(rxdma_ch, N1_SIDE);
                    }
                }
            }
        }

        /* ==== START DMA ==== */
        if (DEVDRV_SUCCESS == ret)
        {
            if (SSI_TX == direct)
            {
                ret = DMA_DmaStart(txdma_ch);
            }
            else if (SSI_RX == direct)
            {
                ret = DMA_DmaStart(rxdma_ch);
            }
            else if (SSI_FULLDUPLEX == direct)
            {
                ret = DMA_DmaStart(txdma_ch);
                if (DEVDRV_SUCCESS == ret)
                {
                    ret = DMA_DmaStart(rxdma_ch);
                }
            }
            else
            {
                ret = DEVDRV_ERROR;
            }
        }
    }

    (void)R_INTC_SetMaskLevel(mask);

    return ret;
}

/*******************************************************************************
* Function Name: R_SSI_SsiStart
* Description  : Start disposal of SSI modules
* Arguments    : ssi_channel -
*                    SSI channel number
*                direct -
*                    Transmission and reception direction
* Return Value : DEVDRV_SUCCESS -
*                    Operation successful.
*                DEVDRV_ERROR -
*                    Operation error.
*******************************************************************************/
int32_t R_SSI_SsiStart(const uint32_t ssi_channel, const uint32_t direct)
{
    uint8_t mask;
    int32_t ret;

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* ==== Argument Check ==== */
    ret = DEVDRV_SUCCESS;
    if ((ssi_channel >= SSI_CHANNEL_MAX) || (0u != (direct & ~SSI_FULLDUPLEX)))
    {
        /* ---- Argument NG ---- */
        ret = DEVDRV_ERROR;
    }
    else
    {
        /* ==== Set SSI(Tx) ==== */
        if (SSI_TX == (direct & SSI_TX))
        {
            /* ---- SSI Tx FIFO Buf reset release ---- */
            COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssifcr,
                                      &(ssi_ssifcr_bit_tbl[SSI_SSIFCR_TFRST_BIT]),
                                      SSI_CLEAR_VALUE);

            /* ---- SSI Tx Under Int status clear ---- */
            COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssisr,
                                      &ssi_ssisr_bit_tbl[SSI_SSISR_TUIRQ_BIT],
                                      SSI_CLEAR_VALUE);

            /* ---- SSI Tx Under Int enable ---- */
            COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssicr,
                                      &ssi_ssicr_bit_tbl[SSI_SSICR_TUIEN_BIT],
                                      SSI_SET_VALUE);

            /* ---- SSI Tx Empty Int enable ---- */
            COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssifcr,
                                      &ssi_ssifcr_bit_tbl[SSI_SSIFCR_TIE_BIT],
                                      SSI_SET_VALUE);

            /* ---- SSI Tx Under Int enable ---- */
            ret = R_INTC_Enable(ssi_int_id[ssi_channel]);
         }

        /* ==== Set SSI(Rx) ==== */
        if (DEVDRV_SUCCESS == ret)
        {
            if (SSI_RX == (direct & SSI_RX))
            {
                /* ---- SSI Rx FIFO Buf reset release ---- */
                COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssifcr,
                                          &ssi_ssifcr_bit_tbl[SSI_SSIFCR_RFRST_BIT],
                                          SSI_CLEAR_VALUE);

                /* ---- SSI Rx Over Int Status clear ---- */
                COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssisr,
                                          &ssi_ssisr_bit_tbl[SSI_SSISR_ROIRQ_BIT],
                                          SSI_CLEAR_VALUE);

                /* ---- SSI Rx Over Int enable ---- */
                COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssicr,
                                          &ssi_ssicr_bit_tbl[SSI_SSICR_ROIEN_BIT],
                                          SSI_SET_VALUE);

                /* ---- SSI Rx Full Int enable ---- */
                COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssifcr,
                                          &ssi_ssifcr_bit_tbl[SSI_SSIFCR_RIE_BIT],
                                          SSI_SET_VALUE);

                /* ---- SSI Rx Over Int enable ---- */
                ret = R_INTC_Enable(ssi_int_id[ssi_channel]);
            }
        }

        /* ==== SSI Tx and Rx enable ==== */
        if (DEVDRV_SUCCESS == ret)
        {
            if (SSI_TX == direct)
            {
                /* ---- SSI Tx enable ---- */
                COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssicr,
                                          &ssi_ssicr_bit_tbl[SSI_SSICR_TEN_BIT],
                                          SSI_SET_VALUE);
            }
            else if (SSI_RX == direct)
            {
                /* ---- SSI Rx enable ---- */
                COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssicr,
                                          &ssi_ssicr_bit_tbl[SSI_SSICR_REN_BIT],
                                          SSI_SET_VALUE);
            }
            else if (SSI_FULLDUPLEX == direct)
            {
                /* ---- SSI Tx and Rx enable ---- */
                COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssicr,
                                          &ssi_ssicr_bit_tbl[SSI_SSICR_TENREN_BIT],
                                          SSI_WSET_VALUE);
            }
            else
            {
                ret = DEVDRV_ERROR;
            }
        }
    }

    (void)R_INTC_SetMaskLevel(mask);

    return ret;

}

/*******************************************************************************
* Function Name: R_SSI_SsiDmaStop
* Description  : In the case of transmission designation, transfer silence data to an SSI module.
*                In the case of reception designation, I stop the data reception processing from an SSI module.
* Arguments    : ssi_channel -
*                    SSI channel number
* Return Value : DEVDRV_SUCCESS -
*                    Operation successful.
*                DEVDRV_ERROR -
*                    Operation error.
*******************************************************************************/
int32_t R_SSI_SsiDmaStop(const uint32_t ssi_channel)
{
    uint32_t txdma_ch;
    uint32_t rxdma_ch;
    int32_t ret;
    uint8_t mask;
    bool_t mute;

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* ==== Argument Check ==== */
    ret = DEVDRV_SUCCESS;
    if (ssi_channel >= SSI_CHANNEL_MAX)
    {
        /* ---- Argument NG ---- */
        ret = DEVDRV_ERROR;
    }
    else
    {
       /* ==== Get DMA Channel ==== */
        txdma_ch = ssi_regist_info[ssi_channel].txdma_channel;
        rxdma_ch = ssi_regist_info[ssi_channel].rxdma_channel;
        /* ==== Stop Full Duplex ====*/
        if ((txdma_ch != NON_REGIST) && (rxdma_ch !=NON_REGIST))
        {
            /* ---- SSI Stop ---- */
            ret = R_SSI_SsiStop(ssi_channel);
            if (DEVDRV_SUCCESS == ret)
            {
                /* ---- DMA(Tx) Stop ---- */
                ret = DMA_DmaStop(txdma_ch);
            }
            if (DEVDRV_SUCCESS == ret)
            {
                /* ---- DMA(Rx) Stop ---- */
                ret = DMA_DmaStop(rxdma_ch);
            }
            if (DEVDRV_SUCCESS == ret)
            {
                /* ---- Queue(Tx) Initialize ---- */
                ret = DMA_QueueInit(txdma_ch);
            }
            if (DEVDRV_SUCCESS == ret)
            {
                /* ---- Queue(Rx) Initialize ---- */
                ret = DMA_QueueInit(rxdma_ch);
            }
        }
        else if((txdma_ch != NON_REGIST) && (NON_REGIST == rxdma_ch))
        {
            /* ==== Stop Tx(Output Mute Data) ====*/
            /* ---- Mute Check ---- */
            ret = DMA_CheckDmaMute(txdma_ch, &mute);
            if (DEVDRV_SUCCESS == ret)
            {
                if (true != mute)
                {
                    /* ---- Stop DMA ---- */
                    ret = DMA_DmaStop(txdma_ch);
                    if (DEVDRV_SUCCESS == ret)
                    {
                        /* ---- Queue(Tx) Initialize ---- */
                        ret = DMA_QueueInit(txdma_ch);
                    }
                    if (DEVDRV_SUCCESS == ret)
                    {
                        /* ---- Next0 Source Address Register Mute Data Setting ---- */
                        ret = DMA_TxRegSet(txdma_ch, N0_SIDE);
                    }
                    if (DEVDRV_ERROR != ret)
                    {
                        /* ---- Next1 Source Address Register Mute Data Setting ---- */
                        ret = DMA_TxRegSet(txdma_ch, N1_SIDE);
                    }
                    if (DEVDRV_ERROR != ret)
                    {
                        /* ---- Start Dma ---- */
                        ret = DMA_DmaStart(txdma_ch);
                    }
                }
            }
        }
        else if((rxdma_ch != NON_REGIST) && (NON_REGIST == txdma_ch))
        {
            /* ==== Stop Rx ====*/
            /* ---- SSI Stop ---- */
            ret = R_SSI_SsiStop(ssi_channel);
            if (DEVDRV_SUCCESS == ret)
            {
                /* ---- Stop DMA ---- */
                ret = DMA_DmaStop(rxdma_ch);
            }
            if (DEVDRV_SUCCESS == ret)
            {
                /* ---- Queue(Rx) Initialize ---- */
                ret = DMA_QueueInit(rxdma_ch);
            }
        }
        else
        {
            ret = DEVDRV_ERROR;
        }
    }

    (void)R_INTC_SetMaskLevel(mask);

    return ret;
}

/*******************************************************************************
* Function Name: R_SSI_SsiStop
* Description  : Stop SSI modules
* Arguments    : ssi_channel -
*                    SSI channel number
* Return Value : none
*******************************************************************************/
int32_t R_SSI_SsiStop(const uint32_t ssi_channel)
{
    uint32_t txstatus;
    uint32_t rxstatus;
    uint32_t iirq_status;
    uint32_t counter;
    int32_t ret;
    uint8_t mask;

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* ==== Argument Check ==== */
    if (ssi_channel >= SSI_CHANNEL_MAX)
    {
        /* ---- Argument NG ---- */
        ret = DEVDRV_ERROR;
    }
    else
    {
        /* ==== Disable SSI INT ==== */
        ret = R_INTC_Disable(ssi_int_id[ssi_channel]);

        /* ==== Stop SSI ==== */
        if (DEVDRV_SUCCESS == ret)
        {
            /* ---- Get Tx enable bit ---- */
            txstatus = COMMON_Get32BitMask(*ssif[ssi_channel].ssicr,
                                           ssi_ssicr_bit_tbl[SSI_SSICR_TEN_BIT].bit_mask);
            /* ---- Get Rx enable bit ---- */
            rxstatus = COMMON_Get32BitMask(*ssif[ssi_channel].ssicr,
                                           ssi_ssicr_bit_tbl[SSI_SSICR_REN_BIT].bit_mask);

            if ( SSI_RX_ENABLE == rxstatus )
            {
                if ( SSI_TX_ENABLE == txstatus )
                {
                    /* ---- Fullduplex Stop ---- */
                    COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssicr,
                                              &ssi_ssicr_bit_tbl[SSI_SSICR_TENREN_BIT],
                                              SSI_CLEAR_VALUE);
                }
                else
                {
                    /* ---- Rx Stop ---- */
                    COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssicr,
                                              &ssi_ssicr_bit_tbl[SSI_SSICR_REN_BIT],
                                              SSI_CLEAR_VALUE);
                }
            }
            else
            {
                if ( SSI_TX_ENABLE == txstatus )
                {
                    /* ---- Tx Stop ---- */
                    COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssicr,
                                              &ssi_ssicr_bit_tbl[SSI_SSICR_TEN_BIT],
                                              SSI_CLEAR_VALUE);
                }
                else
                {
                  /* Do Nothing */
                }
            }

            if(SSI_TX_ENABLE == txstatus)
            {
                /* ---- SSI Tx Under Int disable ---- */
                COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssicr,
                                          &ssi_ssicr_bit_tbl[SSI_SSICR_TUIEN_BIT],
                                          SSI_CLEAR_VALUE);
                /* ---- SSI Tx Empty Int disable ---- */
                COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssifcr,
                                          &ssi_ssifcr_bit_tbl[SSI_SSIFCR_TIE_BIT],
                                          SSI_CLEAR_VALUE);
            }

            if(SSI_RX_ENABLE == rxstatus)
            {
                 /* ---- SSI Rx Over Int disable ---- */
                 COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssicr,
                                           &ssi_ssicr_bit_tbl[SSI_SSICR_ROIEN_BIT],
                                           SSI_CLEAR_VALUE);
                 /* ---- SSI Rx Full Int disable ---- */
                 COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssifcr,
                                           &ssi_ssifcr_bit_tbl[SSI_SSIFCR_RIE_BIT],
                                           SSI_CLEAR_VALUE);
            }

            /* ---- Wait IDLE ---- */
            for (counter = 0; counter < SSI_IDLE_WAIT; counter++)
            {
                /* ---- Get IDLE bit ---- */
                iirq_status = COMMON_Get32BitMask(*ssif[ssi_channel].ssisr,
                                                  ssi_ssisr_bit_tbl[SSI_SSISR_IIRQ_BIT].bit_mask);
                /* ---- wait IDLE ---- */
                if (SSI_IDLE == iirq_status)
                {
                    break;
                }
            }

            /* ---- SSI software reset ---- */
            COMMON_Set8BitMaskLShift(&CPG.SWRSTCR1,
                                     &ssi_swrstcr1_bit_tbl[ssi_channel],
                                     SSI_SET_VALUE);
            CPG.SWRSTCR1;   /* Dummy read */
            COMMON_Set8BitMaskLShift(&CPG.SWRSTCR1,
                                     &ssi_swrstcr1_bit_tbl[ssi_channel],
                                     SSI_CLEAR_VALUE);
            CPG.SWRSTCR1;   /* Dummy read */

            /* ---- SSI TDM Mode register setting ---- */
            *ssif[ssi_channel].ssitdmr = ssi_ssitdmr_reg[ssi_channel];

            /* ---- SSI Control register setting ---- */
            *ssif[ssi_channel].ssicr = ssi_ssicr_reg[ssi_channel];

            /* ---- SSI FIFO Control register setting ---- */
            *ssif[ssi_channel].ssifcr = ssi_ssifcr_reg[ssi_channel];
        }
    }

    (void)R_INTC_SetMaskLevel(mask);

    return ret;
}

/*******************************************************************************
* Function Name: R_SSI_SsiDmaAdsSet
* Description  : register a transmission and reception buffer with a cue.
* Arguments    : ssi_channel -
*                    SSI channel number
*                direct -
*                    Transmission and reception direction
*                buf -
*                    Transmission and reception buffer address
*                buf_size -
*                    Transmission and reception buffer size
* Return Value : DEVDRV_SUCCESS -
*                    Operation successful.
*                DEVDRV_ERROR -
*                    Operation error.
*******************************************************************************/
int32_t R_SSI_SsiDmaAdsSet( const uint32_t ssi_channel,
                            const uint32_t direct,
                            volatile const void * const buf,
                            const uint32_t buf_size )
{
    uint32_t status;
    uint32_t dma_channel;
    int32_t ret;
    uint8_t mask;
    bool_t init;

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* ==== Argument Check ==== */
    ret = DEVDRV_SUCCESS;
    if (ssi_channel >= SSI_CHANNEL_MAX)
    {
        /* ---- Argument NG ---- */
        ret = DEVDRV_ERROR;
    }
    else
    {
        /* ==== Check SSI Tx and Rx Enable Bit ==== */
        init = true;
        if (SSI_TX == direct)
        {
            /* Tx */
            dma_channel = ssi_regist_info[ssi_channel].txdma_channel;
            status = COMMON_Get32BitMask(*ssif[ssi_channel].ssicr,
                                         ssi_ssicr_bit_tbl[SSI_SSICR_TEN_BIT].bit_mask);
            if ( SSI_TX_ENABLE == status )
            {
                init = false;
            }
        }
        else
        {
            /* Rx */
            dma_channel = ssi_regist_info[ssi_channel].rxdma_channel;
            status = COMMON_Get32BitMask(*ssif[ssi_channel].ssicr,
                                         ssi_ssicr_bit_tbl[SSI_SSICR_REN_BIT].bit_mask);
            if ( SSI_RX_ENABLE == status )
            {
                init = false;
            }
        }

        if (NON_REGIST == dma_channel)
        {
            ret = DEVDRV_ERROR;
        }

        /* ==== User Buffer Set ==== */
        if (DEVDRV_SUCCESS == ret)
        {
            ret = DMA_DmaAdsSet(dma_channel, buf, buf_size, init);
        }
    }

    (void)R_INTC_SetMaskLevel(mask);

    return ret;
}

/**************************** interrupt it from here ***************************/

/*******************************************************************************
* Function Name: SsiErrInt
* Description  : SSI ERR interrupt
* Arguments    : ssi_channel -
*                    SSI channel number
* Return Value : DEVDRV_SUCCESS -
*                    Operation successful.
*                DEVDRV_ERROR -
*                    Operation error.
*******************************************************************************/
static int32_t SsiErrInt(const uint32_t ssi_channel)
{
    uint32_t txdma_ch;
    uint32_t rxdma_ch;
    uint32_t txstatus;
    uint32_t rxstatus;
    int32_t ret;

    ret = DEVDRV_SUCCESS;

    /* ==== Get Tx enable bit ==== */
    txstatus = COMMON_Get32BitMask(*ssif[ssi_channel].ssicr,
                                   ssi_ssicr_bit_tbl[SSI_SSICR_TEN_BIT].bit_mask);

    /* ==== Get Rx enable bit ==== */
    rxstatus = COMMON_Get32BitMask(*ssif[ssi_channel].ssicr,
                                   ssi_ssicr_bit_tbl[SSI_SSICR_REN_BIT].bit_mask);

    /* ==== Tx & Rx Stop ==== */
    COMMON_Set32BitMaskLShift(ssif[ssi_channel].ssicr,
                              &ssi_ssicr_bit_tbl[SSI_SSICR_TENREN_BIT],
                               SSI_CLEAR_VALUE);

    /* ==== DMA Channel Get ==== */
    txdma_ch = ssi_regist_info[ssi_channel].txdma_channel;
    rxdma_ch = ssi_regist_info[ssi_channel].rxdma_channel;

    /* ==== DMA Stop and DMA Initialize ==== */
    if (txdma_ch != NON_REGIST)
    {
        /* ---- DMA(Tx) Stop ---- */
        ret = DMA_DmaStop(txdma_ch);
        if (DEVDRV_SUCCESS == ret)
        {
            /* ---- DMA Initialize (Tx) ---- */
            ret = DMA_DmaTxInit(txdma_ch, DMA_TRANSFER_SIZE_32);
        }
        if (DEVDRV_SUCCESS == ret)
        {
            /* ---- Next0 Source Address Register Setting ---- */
            ret = DMA_TxRegSet(txdma_ch, N0_SIDE);
        }
        if (DEVDRV_ERROR != ret)
        {
            /* ---- Next1 Source Address Register Setting ---- */
            ret = DMA_TxRegSet(txdma_ch, N1_SIDE);
        }
    }

    if (DEVDRV_ERROR != ret)
    {
        if (rxdma_ch !=NON_REGIST)
        {
            /* ---- DMA(Rx) Stop ---- */
            ret = DMA_DmaStop(rxdma_ch);
            if (DEVDRV_SUCCESS == ret)
            {
                /* ---- DMA Initialize (Rx) ---- */
                ret = DMA_DmaRxInit(rxdma_ch, DMA_TRANSFER_SIZE_32);
            }
            if (DEVDRV_SUCCESS == ret)
            {
                /* ---- Next0 Destination Address Register Setting ---- */
                ret = DMA_RxRegSet(rxdma_ch, N0_SIDE);
            }
            if (DEVDRV_ERROR != ret)
            {
                /* ---- Next1 Destination Address Register Setting ---- */
                ret = DMA_RxRegSet(rxdma_ch, N1_SIDE);
            }
        }
    }

    /* ==== SSI Software reset ==== */
    COMMON_Set8BitMaskLShift(&CPG.SWRSTCR1,
                             &ssi_swrstcr1_bit_tbl[ssi_channel],
                             SSI_SET_VALUE);
    CPG.SWRSTCR1;   /* Dummy read */
    COMMON_Set8BitMaskLShift(&CPG.SWRSTCR1,
                             &ssi_swrstcr1_bit_tbl[ssi_channel],
                             SSI_CLEAR_VALUE);
    CPG.SWRSTCR1;   /* Dummy read */

    /* ==== SSI Control register setting ==== */
    *ssif[ssi_channel].ssicr = ssi_ssicr_reg[ssi_channel];

    /* ==== SSI FIFO Control register setting ==== */
    *ssif[ssi_channel].ssifcr = ssi_ssifcr_reg[ssi_channel];

    /* ==== SSI Status register clear ==== */
    *ssif[ssi_channel].ssisr = 0;
    *ssif[ssi_channel].ssisr;   /* Dummy read */

    if (DEVDRV_ERROR != ret)
    {
        if ( SSI_RX_ENABLE == rxstatus )
        {
            if ( SSI_TX_ENABLE == txstatus )
            {
                /* ---- Full Duplex Start ---- */
                ret = DMA_DmaStart(txdma_ch);
                if (DEVDRV_SUCCESS == ret)
                {
                    ret = DMA_DmaStart(rxdma_ch);
                }
                if (DEVDRV_SUCCESS == ret)
                {
                    ret = R_SSI_SsiStart(ssi_channel, (SSI_RX | SSI_TX));
                }
            }
            else
            {
                /* ---- Rx Start ---- */
                ret = DMA_DmaStart(rxdma_ch);
                if (DEVDRV_SUCCESS == ret)
                {
                    ret = R_SSI_SsiStart(ssi_channel, SSI_RX);
                }
            }
        }
        else
        {
            if ( SSI_TX_ENABLE == txstatus )
            {
                /* ---- Tx Start ---- */
                ret = DMA_DmaStart(txdma_ch);
                if (DEVDRV_SUCCESS == ret)
                {
                    ret = R_SSI_SsiStart(ssi_channel, SSI_TX);
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
* Function Name: SSI_SSI0_INT_Err
* Description  : SSI (channel 0) error interrupt (Tx Under, Rx Over)
* Arguments    : int_sense -
*                    The interrupt detection
* Return Value : none
*******************************************************************************/
static void SSI_SSI0_INT_Err(uint32_t int_sense)
{
    int32_t ret;

    ret = SsiErrInt(SSI_CHANNEL0);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: SSI_SSI1_INT_Err
* Description  : SSI (channel 1) error interrupt (Tx Under, Rx Over)
* Arguments    : int_sense -
*                    The interrupt detection
* Return Value : none
*******************************************************************************/
static void SSI_SSI1_INT_Err(uint32_t int_sense)
{
    int32_t ret;

    ret = SsiErrInt(SSI_CHANNEL1);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: SSI_SSI2_INT_Err
* Description  : SSI (channel 2) error interrupt (Tx Under, Rx Over)
* Arguments    : int_sense -
*                    The interrupt detection
* Return Value : none
*******************************************************************************/
static void SSI_SSI2_INT_Err(uint32_t int_sense)
{
    int32_t ret;

    ret = SsiErrInt(SSI_CHANNEL2);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: SSI_SSI3_INT_Err
* Description  : SSI (channel 3) error interrupt (Tx Under, Rx Over)
* Arguments    : int_sense -
*                    The interrupt detection
* Return Value : none
*******************************************************************************/
static void SSI_SSI3_INT_Err(uint32_t int_sense)
{
    int32_t ret;

    ret = SsiErrInt(SSI_CHANNEL3);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: SSI_SSI4_INT_Err
* Description  : SSI (channel 4) error interrupt (Tx Under, Rx Over)
* Arguments    : int_sense -
*                    The interrupt detection
* Return Value : none
*******************************************************************************/
static void SSI_SSI4_INT_Err(uint32_t int_sense)
{
    int32_t ret;

    ret = SsiErrInt(SSI_CHANNEL4);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/*******************************************************************************
* Function Name: SSI_SSI5_INT_Err
* Description  : SSI (channel 5) error interrupt (Tx Under, Rx Over)
* Arguments    : int_sense -
*                    The interrupt detection
* Return Value : none
*******************************************************************************/
static void SSI_SSI5_INT_Err(uint32_t int_sense)
{
    int32_t ret;

    ret = SsiErrInt(SSI_CHANNEL5);
    if (DEVDRV_ERROR == ret)
    {
        /* ERR */
    }
}

/* END of File */
