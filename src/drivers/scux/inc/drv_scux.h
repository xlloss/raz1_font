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
* Copyright (C) 2012 - 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File Name    : drv_scux.h
* $Rev: 729 $
* $Date:: 2014-06-11 17:44:40 +0900#$
* Description  : This module is header for sample of SCUX driver.
******************************************************************************/
/* Guards against multiple inclusion */
#ifndef DRV_SCUX_H
#define DRV_SCUX_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <string.h>
#include "r_typedefs.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/******************************************************************************
Macro definitions
******************************************************************************/

/* Return value for SCUX Driver functions*/
#define SCUX_OK     (0)
#define SCUX_E_PAR  (-1)
#define SCUX_E_BUSY (-2)

/* SCUX SRC wait time */
#define SCUX_MIN_WAIT   (0x00000000)
#define SCUX_MAX_WAIT   (0x00FFFFFF)
#define SCUX_NO_WAIT    (-1)

/* SCUX DVU ramp wait time */
#define SCUX_MIN_RAMP_WAIT  (0x00000000u)
#define SCUX_MAX_RAMP_WAIT  (0x00FFFFFFu)

/* SCUX DVU ramp gain level */
#define SCUX_MIN_RAMP_GAIN  (0x000003FFu)
#define SCUX_MAX_RAMP_GAIN  (0x00000000u)

/* SCUX DVU digital volume level */
#define SCUX_MAX_VOL_VALUE  (0x007FFFFFu)

/* SCUX MIX gain level */
#define SCUX_MIN_MIX_GAIN   (0x000003FFu)
#define SCUX_MAX_MIX_GAIN   (0x00000000u)

/* SCUX minimum FS ratio */
#define SCUX_MIN_FS_RATIO_90    (0x00E66666u)
#define SCUX_MIN_FS_RATIO_98    (0x00FAE147u)

enum scux_ch
{
        SCUX_CH_0
    ,   SCUX_CH_1
    ,   SCUX_CH_2
    ,   SCUX_CH_3
    /* Number of this enumeration type */
    ,   NUM_SCUX_CH
};

enum scux_int_module
{
        SCUX_INT_ID_SRC         /* Intrrupt for SRC module */
    ,   SCUX_INT_ID_FFD         /* Intrrupt for FFD module */
    ,   SCUX_INT_ID_FFU         /* Intrrupt for FFU module */
    ,   SCUX_INT_ID_DVU         /* Intrrupt for DVU module */
    /* Number of this enumeration type */
    ,   NUM_SCUX_INT_ID_MODULE
};

enum scux_pass
{
    /* SCUX channel 0 pass */
        SSIF0_SRC0_SSIF0    /* SSIF->SRC->SSIF No.1    */
    ,   SSIF0_SRC0_SSIF3    /* SSIF->SRC->SSIF No.2    */
    ,   SSIF0_SRC0_FFU      /* SSIF->SRC->FFU No.1     */
    ,   FFD_SRC0_SSIF0      /* FFD->SRC->SSIF No.1     */
    ,   FFD_SRC0_SSIF3      /* FFD->SRC->SSIF No.3     */
    ,   FFD_SRC0_FFU        /* FFD->SRC->FFU No.1,2    */
    ,   SSIF0_SRC0_MIX      /* SSIF->SRC->MIX No.1,2   */
    ,   FFD_SRC0_MIX        /* FFD->SRC->MIX No.1,3    */
    /* SCUX channel 1 pass */
    ,   SSIF3_SRC1_SSIF3    /* SSIF->SRC->SSIF No.4    */
    ,   SSIF3_SRC1_SSIF0    /* SSIF->SRC->SSIF No.5    */
    ,   SSIF3_SRC1_FFU      /* SSIF->SRC->FFU No.3     */
    ,   FFD_SRC1_SSIF3      /* FFD->SRC->SSIF No.5     */
    ,   FFD_SRC1_SSIF0      /* FFD->SRC->SSIF No.7     */
    ,   FFD_SRC1_FFU        /* FFD->SRC->FFU No.3,4    */
    ,   SSIF3_SRC1_MIX      /* SSIF->SRC->MIX No.4,5   */
    ,   FFD_SRC1_MIX        /* FFD->SRC->MIX No.5,7    */
    /* SCUX channel 2 pass */
    ,   SSIF1_SRC2_SSIF4    /* SSIF->SRC->SSIF No.7    */
    ,   SSIF4_SRC2_SSIF1    /* SSIF->SRC->SSIF No.8    */
    ,   SSIF1_SRC2_FFU      /* SSIF->SRC->FFU No.5     */
    ,   SSIF4_SRC2_FFU      /* SSIF->SRC->FFU No.6     */
    ,   FFD_SRC2_SSIF1      /* FFD->SRC->SSIF No.9     */
    ,   FFD_SRC2_SSIF4      /* FFD->SRC->SSIF No.10    */
    ,   FFD_SRC2_FFU        /* FFD->SRC->FFU No.5,6    */
    ,   SSIF1_SRC2_MIX      /* SSIF->SRC->MIX No.7,8   */
    ,   SSIF4_SRC2_MIX      /* SSIF->SRC->MIX No.10,12 */
    ,   FFD_SRC2_MIX        /* FFD->SRC->MIX No.9,11   */
    /* SCUX channel 3 pass */
    ,   SSIF2_SRC3_SSIF5    /* SSIF->SRC->SSIF No.9    */
    ,   SSIF5_SRC3_SSIF2    /* SSIF->SRC->SSIF No.10   */
    ,   SSIF2_SRC3_FFU      /* SSIF->SRC->FFU No.7     */
    ,   SSIF5_SRC3_FFU      /* SSIF->SRC->FFU No.8     */
    ,   FFD_SRC3_SSIF2      /* FFD->SRC->SSIF No.11    */
    ,   FFD_SRC3_SSIF5      /* FFD->SRC->SSIF No.12    */
    ,   FFD_SRC3_FFU        /* FFD->SRC->FFU No.7,8    */
    ,   SSIF2_SRC3_MIX      /* SSIF->SRC->MIX No.13,14 */
    ,   SSIF5_SRC3_MIX      /* FFD->SRC->MIX No.16,18  */
    ,   FFD_SRC3_MIX        /* FFD->SRC->MIX No.13,15  */
    /* Number of this enumeration type */
    ,   NUM_SCUX_PASS
};

enum scux_audio_ch
{
        AUDIO_CH_0
    ,   AUDIO_CH_1
    ,   AUDIO_CH_2
    ,   AUDIO_CH_3
    ,   AUDIO_CH_4
    ,   AUDIO_CH_5
    ,   AUDIO_CH_6
    ,   AUDIO_CH_7
    /* Number of this enumeration type */
    ,   NUM_AUDIO_CH
};

enum scux_fifo_req_size
{
        REQ_SIZE_256    /* 256data (FFU0_0 and FFU0_1 Only) */
    ,   REQ_SIZE_128    /* 128data (FFU0_0 and FFU0_1 Only) */
    ,   REQ_SIZE_64     /* 64data   */
    ,   REQ_SIZE_32     /* 32data   */
    ,   REQ_SIZE_16     /* 16data   */
    ,   REQ_SIZE_8      /* 8data    */
    ,   REQ_SIZE_4      /* 4data    */
    ,   REQ_SIZE_2      /* 2data    */
    ,   REQ_SIZE_1      /* 1data    */
};

/******************************************************************************
Typedef definitions
******************************************************************************/

typedef enum scux_req_dma
{
        FFD_DMA_START   /* Require a start of DMA for FFD */
    ,   FFD_DMA_STOP    /* Require a stop of DMA for FFD */
    ,   FFU_DMA_START   /* Require a start of DMA for FFU */
    ,   FFU_DMA_STOP    /* Require a stop of DMA for FFU */
} scux_req_dma_t;

/*********************** For Setup SCUX parameter ***********************/
typedef enum scux_num_audio_ch
{
        NUM_AUDIO_CH_1 = 1
    ,   NUM_AUDIO_CH_2 = 2
    ,   NUM_AUDIO_CH_4 = 4
    ,   NUM_AUDIO_CH_6 = 6
    ,   NUM_AUDIO_CH_8 = 8
} scux_num_audio_ch_t;

typedef enum scux_bit_length
{
        BIT_LENGTH_24 = 0
    ,   BIT_LENGTH_16 = 8
} scux_bit_length_t;

typedef enum scux_src_mode
{
        ASYNC_SRC
    ,   SYNC_SRC
} scux_src_mode_t;

typedef enum scux_select
{
        SELECT_SSIF0
    ,   SELECT_SSIF1
    ,   SELECT_SSIF2
    ,   SELECT_SSIF3
    ,   SELECT_SSIF4
    ,   SELECT_SSIF5
    ,   SELECT_MIX      /* only output      */
    ,   SELECT_FIFO     /* FIFO (FFD/FFU)   */
    ,   SELECT_NON      /* Initial value    */
} scux_select_t;

typedef enum scux_fs
{
        FS_8000     /* 8KHz                     */
    ,   FS_11025    /* 11.025KHz (Input Only)   */
    ,   FS_12000    /* 12KHz                    */
    ,   FS_16000    /* 16KHz                    */
    ,   FS_22050    /* 22.05KHz  (Input Only)   */
    ,   FS_24000    /* 24KHz                    */
    ,   FS_32000    /* 32KHz                    */
    ,   FS_44100    /* 44.1KHz                  */
    ,   FS_48000    /* 48KHz                    */
    ,   FS_64000    /* 64KHz     (Input Only)   */
    ,   FS_88200    /* 88.2KHz   (Input Only)   */
    ,   FS_96000    /* 96KHz                    */
} scux_fs_t;

typedef enum scux_low_delay
{
        LOW_DELAY_NON = 0x00    /* normal       */
    ,   LOW_DELAY_1   = 0x80    /* Low Delay 1  */
    ,   LOW_DELAY_2   = 0x40    /* Low Delay 2  */
} scux_low_delay_t;

typedef enum scux_mix_mode
{
        VOL_STEP_MIXER  /* volume step mixer */
    ,   VOL_RAMP_MIXER  /* volume ramp mixer */
} scux_mix_mode_t;

typedef enum scux_mix_vol_period
{
                            /*             vol_up , vol_down        */
        MIX_PERIOD_1        /*    1sample (  128dB,   -128dB/1step) */
    ,   MIX_PERIOD_2        /*    2sample (   64dB,    -64dB/1step) */
    ,   MIX_PERIOD_4        /*    4sample (   32dB,    -32dB/1step) */
    ,   MIX_PERIOD_8        /*    8sample (   16dB,    -16dB/1step) */
    ,   MIX_PERIOD_16       /*   16sample (    8dB,     -8dB/1step) */
    ,   MIX_PERIOD_32       /*   32sample (    4dB,     -4dB/1step) */
    ,   MIX_PERIOD_64       /*   64sample (    2dB,     -2dB/1step) */
    ,   MIX_PERIOD_128      /*  128sample (    1dB,     -1dB/1step) */
    ,   MIX_PERIOD_256      /*  256sample (  0.5dB,   -0.5dB/1step) */
    ,   MIX_PERIOD_512      /*  512sample ( 0.25dB,  -0.25dB/1step) */
    ,   MIX_PERIOD_1024     /* 1024sample (0.125dB, -0.125dB/1step) */
} scux_mix_vol_period_t;

typedef enum scux_ramp_period
{
                            /*                vol_up , vol_down             */
        DVU_PERIOD_1        /*       1sample (  128dB,   -128dB/1step)      */
    ,   DVU_PERIOD_2        /*       2sample (   64dB,    -64dB/1step)      */
    ,   DVU_PERIOD_4        /*       4sample (   32dB,    -32dB/1step)      */
    ,   DVU_PERIOD_8        /*       8sample (   16dB,    -16dB/1step)      */
    ,   DVU_PERIOD_16       /*      16sample (    8dB,     -8dB/1step)      */
    ,   DVU_PERIOD_32       /*      32sample (    4dB,     -4dB/1step)      */
    ,   DVU_PERIOD_64       /*      64sample (    2dB,     -2dB/1step)      */
    ,   DVU_PERIOD_128      /*     128sample (    1dB,     -1dB/1step)      */
    ,   DVU_PERIOD_256      /*     256sample (  0.5dB,   -0.5dB/1step)      */
    ,   DVU_PERIOD_512      /*     512sample ( 0.25dB,  -0.25dB/1step)      */
    ,   DVU_PERIOD_1024     /*    1024sample (0.125dB, -0.125dB/1step)      */
    ,   DVU_PERIOD_2048     /*    2048sample (0.125dB, -0.125dB/2step)      */
    ,   DVU_PERIOD_4096     /*    4096sample (0.125dB, -0.125dB/4step)      */
    ,   DVU_PERIOD_8192     /*    8192sample (0.125dB, -0.125dB/8step)      */
    ,   DVU_PERIOD_16384    /*   16384sample (0.125dB, -0.125dB/16step)     */
    ,   DVU_PERIOD_32768    /*   32768sample (0.125dB, -0.125dB/32step)     */
    ,   DVU_PERIOD_65536    /*   65536sample (0.125dB, -0.125dB/64step)     */
    ,   DVU_PERIOD_131072   /*  131072sample (0.125dB, -0.125dB/128step)    */
    ,   DVU_PERIOD_262144   /*  262144sample (0.125dB, -0.125dB/256step)    */
    ,   DVU_PERIOD_524288   /*  524288sample (0.125dB, -0.125dB/512step)    */
    ,   DVU_PERIOD_1048576  /* 1048576sample (0.125dB, -0.125dB/1024step)   */
    ,   DVU_PERIOD_2097152  /* 2097152sample (0.125dB, -0.125dB/2048step)   */
    ,   DVU_PERIOD_4194304  /* 4194304sample (0.125dB, -0.125dB/4096step)   */
    ,   DVU_PERIOD_8388608  /* 8388608sample (0.125dB, -0.125dB/8192step)   */
} scux_ramp_period_t;

typedef enum scux_config_enable
{
        CONFIG_DISABLE
    ,   CONFIG_ENABLE
    ,   CONFIG_NO_CHANGE
} scux_config_enable_t;

typedef enum scux_vol_ramp_sts
{
        MUTE_STATUS             /* Mute status */
    ,   VOL_RAMP_DOWN           /* Volume ramp down */
    ,   VOL_RAMP_UP             /* Volume ramp up */
    ,   STATE_GAIN_LEVEL        /* Level of Volume ramp is in the state of the value of VRDBR register */
    ,   VOL_MAINTENANCE         /* Volume of input data is maintained (Volume is 1-time) */
} scux_vol_ramp_sts_t;

typedef struct scux_callback
{
    int32_t (* set_int_callback)(uint32_t scux_ch);
                                    /* Request to interrupt setting */
    int32_t (* set_dmac_callback)(uint32_t scux_ch, scux_req_dma_t req_dma_start_stop);
                                    /* Request to dma setting */
} scux_callback_t;

typedef struct scux_setup_arg
{
    uint32_t                pass_select;            /* Select SCUX pass                     */
    scux_num_audio_ch_t     audio_channel;          /* channel number of audio data         */
    scux_bit_length_t       bit_length;             /* bit length of audio data             */
    scux_src_mode_t         src_mode;               /* src_mode                             */
    uint32_t                ffd_request_size;       /* request size of FFD                  */
    uint32_t                ffu_request_size;       /* request size of FFU                  */
    scux_fs_t               output_fs;              /* FS of output                         */
    scux_fs_t               input_fs;               /* FS of input                          */
    bool_t                  src_bypass_mode;        /* bypass mode of SRC
                                                        true:bypass, false:no bypass        */
    int32_t                 wait_time;              /* wait time of SRC                     */
    scux_low_delay_t        low_delay;              /* low delay mode of SRC                */
    uint32_t                min_fs_ratio;           /* minimum FS ratio                     */
    bool_t                  dvu_bypass_mode;        /* bypass mode of DVU
                                                        true:bypass, false:no bypass        */
    bool_t                  dvu_zc_mute_mode;       /* zero cross mute mode of DVU
                                                        true:use, false:sleep               */
    uint8_t                 dvu_ramp_mode;          /* volume ramp mode of DVU
                                                        channel 7 to 0 for bit7 to 0
                                                        1:enable, 0:disable                 */
    bool_t                  dvu_vol_mode;           /* digital volume value mode of DVU
                                                        true:use, false:sleep               */
    bool_t                  src_use_channel_exch;   /* use of SRC channel exchange
                                                        true:exchange, false:no exchange    */
    uint32_t                src_channel_exch[NUM_AUDIO_CH]; /* src_channel_exch[n]:setting of SRC
                                                               input channel for output channel n   */
    scux_select_t           mix_output_select;      /* output pass to MIX                   */
    scux_mix_mode_t         mix_mode;               /* mix mode                             */
    scux_mix_vol_period_t   mix_vol_up;             /* MIX Period for Volume Up             */
    scux_mix_vol_period_t   mix_vol_down;           /* MIX Period for Volume Down           */
    bool_t                  mix_use_channel_exch;   /* use of MIX channel exchange
                                                        true:exchange, false:no exchange    */
    uint32_t                mix_channel_exch[NUM_AUDIO_CH]; /* src_channel_exch[n]:setting of MIX
                                                               input channel for output channel n   */
} scux_setup_arg_t;

typedef struct scux_config_dvu_arg
{
    uint8_t             zc_mute;        /* setting zero cross mute function of
                                           channel 7 to 0 for bit7 to 0 */
    scux_ramp_period_t  ramp_vol_up;    /* volume ramp period for volume up                 */
    scux_ramp_period_t  ramp_vol_down;  /* volume ramp period for volume down               */
    uint32_t            ramp_gain;      /* decibel (gain level) of volume ramp              */
    uint32_t            ramp_wait_time; /* standby time to adjust the start timing of
                                            operating of the volume ramp function           */
    uint32_t            vol_value[NUM_AUDIO_CH];    /* vol_value[n]: Digital Volume Value for channel n */
} scux_config_dvu_arg_t;

typedef struct scux_config_mix_arg
{
    uint32_t    mix_gain[NUM_SCUX_CH];  /* mix_gain[n]:decibel (gain level) of system n(0-3:A-D) */
} scux_config_mix_arg_t;

typedef struct scux_config_enable_arg
{
    scux_config_enable_t dvu[NUM_SCUX_CH];  /* dvu[n]:Configuration setting enable of DVU(n=0-3)    */
    scux_config_enable_t mix;       /* Configuration setting enable of MIX                  */
} scux_config_enable_arg_t;


/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
int32_t R_SCUX_RegistCallback (const scux_callback_t * const scux_callback_arg);
void R_SCUX_Reset (void);
int32_t R_SCUX_Setup (  const uint32_t ch,
                        const scux_setup_arg_t * const setup_arg,
                        const scux_config_dvu_arg_t * const config_dvu_arg,
                        const scux_config_mix_arg_t * const config_mix_arg);
int32_t R_SCUX_ClearInit (const uint32_t ch);
int32_t R_SCUX_SetInit (const uint32_t ch, const bool_t mix_stop);
int32_t R_SCUX_StartFfu (const uint32_t ch);
int32_t R_SCUX_StartFfd (const uint32_t ch);
int32_t R_SCUX_StopFfu (const uint32_t ch);
int32_t R_SCUX_StopFfd (const uint32_t ch);
int32_t R_SCUX_StopDirectSsifTx (const uint32_t ch);
int32_t R_SCUX_StopDirectSsifRx (const uint32_t ch);
int32_t R_SCUX_ConfigDvu (const uint32_t ch, const scux_config_dvu_arg_t * const config_dvu_arg);
int32_t R_SCUX_ConfigMix (const scux_config_mix_arg_t * const config_mix_arg);
int32_t R_SCUX_ConfigEnable (const scux_config_enable_arg_t * const config_enable_arg);
int32_t R_SCUX_GetVolRampSts (const uint32_t ch, scux_vol_ramp_sts_t * const ramp_sts);

void (* R_SCUX_GetIntFunc (const uint32_t ch, const uint32_t module))(uint32_t int_sense);
int32_t R_SCUX_SetIntCallback ( const uint32_t ch,
                                const uint32_t module,
                                void (* const callback)(const uint32_t int_req));


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DRV_SCUX_H */

/* End of File */
