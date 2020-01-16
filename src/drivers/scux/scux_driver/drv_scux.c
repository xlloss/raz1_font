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
/*******************************************************************************
* File Name   : drv_scux.c
* $Rev: 729 $
* $Date:: 2014-06-11 17:44:40 +0900#$
* Description  : This module is sample of SCUX driver.
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "devdrv_intc.h"
#include "drv_scux_register.h"
#include "drv_scux_user.h"
#include "drv_scux.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/* Initial value of SRCRSELn_CIM and MIXRSEL_CIM Register */
#define RSEL_INIT_VALUE     (0x76543210u)

/* Value of SEVMRp_2SRC0_m Register */
#define SEVMR_VALUE         (0xc0004000u)

/* Initial value of SRCCRp_2SRC0_m Register */
#define SRCCR_INIT_VALUE    (0x00010110u)
/* Bit16, Bit8, and Bit4 should be set to 1 */

/* Initial value of UEVMR_FFU0_n Register */
#define UEVMR_VALUE         (0xE0000000u)

/* Initial value of DEVMR_FFD0_n Register */
#define DEVMR_VALUE         (0xF0000000u)

/* Initial value of VEVMR_DVU0_n Register */
#define VEVMR_VALUE         (0xFF008000u)

/* Value of MIXBR_MIX0_0 Register */
#define MIXBR_VALUE         (0u)

/* Value of IFSCRp_2SRC0_m Register */
#define IFSCR_VALUE         (1u)

/* Output sampling rate(fixed value) */
#define FSO     (uint64_t)(0x0400000)

/* Number of FS */
#define NUM_OUTPUT_FS       (7u)
#define NUM_INPUT_FS        (12u)

/* No IFSVR Register Value */
#define NO_IFSVR            (0u)

/* INT Prohibition */
#define MASK_INT_LV         (0)

/* FFDPR Register Value */
#define FFDPR_NO_PASS_SELECT    (0u)
#define FFDPR_CIM_FFD_IPC_ASYNC (1u)
#define FFDPR_CIM_FFD_IPC_SYNC  (2u)

/* FFUPR Register Value */
#define FFUPR_NO_PASS_SELECT    (0u)
#define FFUPR_CIM_FFU_OPC_ASYNC (1u)
#define FFUPR_CIM_FFU_OPC_SYNC  (2u)

/* IPSLR Register Value */
#define IPSLR_SSIF_IPC_ASYNC    (1u)
#define IPSLR_FFD_IPC_ASYNC     (3u)
#define IPSLR_FFD_IPC_SYNC      (4u)

/* OPSLR Register Value */
#define OPSLR_ASYNC_OPC_DVU     (1u)
#define OPSLR_ASYNC_OPC_FFU     (3u)
#define OPSLR_SYNC_OPC_FFU      (4u)

/* SSIRSEL Register SOSEL bit */
#define SOSEL0_SRC0_SELECT      (0u)
#define SOSEL0_SRC1_SELECT      (1u)
#define SOSEL0_MIX_SELECT       (2u)
#define SOSEL3_SRC1_SELECT      (0u)
#define SOSEL3_SRC0_SELECT      (1u)
#define SOSEL3_MIX_SELECT       (2u)

/* SSIRSEL Register SISEL bit */
#define SISEL2_SSIF1_SELECT     (0u)
#define SISEL2_SSIF4_SELECT     (1u)
#define SISEL3_SSIF2_SELECT     (0u)
#define SISEL3_SSIF5_SELECT     (1u)

/* Decimal digits of min_fs_ratio */
#define MIN_FS_RATIO_DECIMAL_DIGITS     (24u)

/* BUFIN bit */
#define BUFIN_LOW_DELAY_OFF     (0u)
#define BUFIN_LOW_DELAY_ON      (5u)

/* INIT bit */
#define INIT_ON                 (1u)
#define INIT_OFF                (0u)

/* BOOT bit */
#define BOOT_ON                 (1u)
#define BOOT_OFF                (0u)

/* SWRST bit */
#define RESET_ON                (0u)
#define RESET_OFF               (1u)

/******************************************************************************
Typedef definitions
******************************************************************************/

typedef struct scux_pass_min_max
{
    uint32_t min;
    uint32_t max;
} scux_pass_min_max_t;

typedef struct scux_io
{
    scux_select_t   input;
    scux_select_t   output;
} scux_io_t;

typedef struct scux_pass_inf
{
    scux_io_t   io;
    bool_t      src_mode;
} scux_pass_inf_t;

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* ==== Prototype declaration ==== */
/* ---- Check Parameter Functions ---- */
static int32_t SetupCheckParam (    const uint32_t ch,
                                    const scux_setup_arg_t * const setup_arg,
                                    const scux_config_dvu_arg_t * const config_dvu_arg,
                                    const scux_config_mix_arg_t * const config_mix_arg,
                                    scux_pass_inf_t * const get_pass_inf);
static int32_t CheckParamNumScuxCh (const uint32_t ch);
static int32_t CheckParamNumAudioCh (   const uint32_t ch,
                                        const scux_setup_arg_t * const setup_arg,
                                        const scux_pass_inf_t * const get_pass_inf);
static int32_t CheckParamSyncMode ( const scux_setup_arg_t * const setup_arg,
                                    const scux_pass_inf_t * const get_pass_inf);
static int32_t CheckFifoRequestSize (   const uint32_t ch,
                                        const scux_select_t io_select,
                                        const uint32_t request_size,
                                        const scux_num_audio_ch_t audio_channel);
static int32_t CheckParamLowDelay (const scux_setup_arg_t * const setup_arg);
static int32_t CheckParamWaitTime (const scux_setup_arg_t * const setup_arg);
static int32_t CheckParamFs (const scux_setup_arg_t * const setup_arg);
static int32_t CheckParamMinFsRatio (const uint32_t min_fs_ratio);
static int32_t CheckParamAudioChExch (  const scux_setup_arg_t * const setup_arg,
                                        const bool_t use_ch_exch,
                                        const uint32_t * const exch);
static int32_t ConfigDvuCheckParam (const scux_config_dvu_arg_t * const config_dvu_arg);
static int32_t ConfigMixCheckParam (const scux_config_mix_arg_t * const config_mix_arg);

/* ---- Check Status Functions ---- */
static int32_t SetupCheckStatus (const scux_reg_address_t * const scux_reg);
static int32_t ClearInitCheckStatus (const scux_reg_address_t * const scux_reg);
static int32_t StartFifoCheckStatus (const scux_reg_address_t * const scux_reg);

/* ---- Register Setting Functions ---- */
static int32_t SetupPassInOut ( const scux_reg_address_t * const scux_reg,
                                const uint32_t ch,
                                const scux_setup_arg_t * const setup_arg);
static int32_t SetupPassSsifRoute (const uint32_t ch, const scux_setup_arg_t * const setup_arg);
static int32_t SetupPassSsifControl (const uint32_t ch, const scux_setup_arg_t * const setup_arg);
static int32_t SetupPassAudioExch (const scux_reg_address_t * const scux_reg, const scux_setup_arg_t * const setup_arg);
static int32_t SetupClock ( const scux_reg_address_t * const scux_reg,
                            const uint32_t ch,
                            const scux_setup_arg_t * const setup_arg);
static int32_t SetupFfd (   const scux_reg_address_t * const scux_reg,
                            uint32_t ch,
                            const scux_setup_arg_t * const setup_arg);
static int32_t SetupFfu (   const scux_reg_address_t * const scux_reg,
                            uint32_t ch,
                            const scux_setup_arg_t * const setup_arg);
static int32_t SetupSrc (const scux_reg_address_t * const scux_reg, const scux_setup_arg_t * const setup_arg);
static int32_t SetupDvu (   const scux_reg_address_t * const scux_reg,
                            const scux_setup_arg_t * const setup_arg,
                            const scux_config_dvu_arg_t * const config_dvu_arg);
static int32_t SetupMix (const scux_setup_arg_t * const setup_arg, const scux_config_mix_arg_t * const config_mix_arg);
static int32_t ClearInit (const scux_reg_address_t * const scux_reg, const uint32_t ch);
static int32_t SetInit (const scux_reg_address_t * const scux_reg, const uint32_t ch, const bool_t mix_stop);
static int32_t ConfigDvu (  const scux_reg_address_t * const scux_reg,
                            const scux_config_dvu_arg_t * const config_dvu_arg);
static int32_t ConfigMix (const scux_config_mix_arg_t * const config_mix_arg);
static uint32_t MultiplyIntAndFixedPoint(   const uint32_t integer,
                                            const uint32_t fixed_point_decimal,
                                            const uint32_t decimal_digits );

/* ==== Global variables ==== */
static scux_pass_inf_t pass_inf[NUM_SCUX_CH];
static scux_callback_t scux_callback_func;
static uint32_t ssictrl_reg_value;
static uint32_t futsel_reg_value[NUM_SCUX_CH];

/* Minimum and Max channels of scux_pass */
static const scux_pass_min_max_t scux_pass_min_max_tbl[NUM_SCUX_CH] =
{
        /*  Min             , Max */
        { SSIF0_SRC0_SSIF0, FFD_SRC0_MIX }  /* SCUX0 */
    ,   { SSIF3_SRC1_SSIF3, FFD_SRC1_MIX }  /* SCUX1 */
    ,   { SSIF1_SRC2_SSIF4, FFD_SRC2_MIX }  /* SCUX2 */
    ,   { SSIF2_SRC3_SSIF5, FFD_SRC3_MIX }  /* SCUX3 */
};

/* Input/Output of SCUX */
static const scux_io_t scux_io_tbl[NUM_SCUX_PASS] =
{
        /* input        , output */
    /* SCUX channel 0 */
        { SELECT_SSIF0  , SELECT_SSIF0  }   /* SSIF0_SRC0_SSIF0 */
    ,   { SELECT_SSIF0  , SELECT_SSIF3  }   /* SSIF0_SRC0_SSIF3 */
    ,   { SELECT_SSIF0  , SELECT_FIFO   }   /* SSIF0_SRC0_FFU   */
    ,   { SELECT_FIFO   , SELECT_SSIF0  }   /* FFD_SRC0_SSIF0   */
    ,   { SELECT_FIFO   , SELECT_SSIF3  }   /* FFD_SRC0_SSIF3   */
    ,   { SELECT_FIFO   , SELECT_FIFO   }   /* FFD_SRC0_FFU     */
    ,   { SELECT_SSIF0  , SELECT_MIX    }   /* SSIF0_SRC0_MIX   */
    ,   { SELECT_FIFO   , SELECT_MIX    }   /* FFD_SRC0_MIX     */
    /* SCUX channel 1 */
    ,   { SELECT_SSIF3  , SELECT_SSIF3  }   /* SSIF3_SRC1_SSIF3 */
    ,   { SELECT_SSIF3  , SELECT_SSIF0  }   /* SSIF3_SRC1_SSIF0 */
    ,   { SELECT_SSIF3  , SELECT_FIFO   }   /* SSIF3_SRC1_FFU   */
    ,   { SELECT_FIFO   , SELECT_SSIF3  }   /* FFD_SRC1_SSIF3   */
    ,   { SELECT_FIFO   , SELECT_SSIF0  }   /* FFD_SRC1_SSIF0   */
    ,   { SELECT_FIFO   , SELECT_FIFO   }   /* FFD_SRC1_FFU     */
    ,   { SELECT_SSIF3  , SELECT_MIX    }   /* SSIF3_SRC1_MIX   */
    ,   { SELECT_FIFO   , SELECT_MIX    }   /* FFD_SRC1_MIX     */
    /* SCUX channel 2 */
    ,   { SELECT_SSIF1  , SELECT_SSIF4  }   /* SSIF1_SRC2_SSIF4 */
    ,   { SELECT_SSIF4  , SELECT_SSIF1  }   /* SSIF4_SRC2_SSIF1 */
    ,   { SELECT_SSIF1  , SELECT_FIFO   }   /* SSIF1_SRC2_FFU   */
    ,   { SELECT_SSIF4  , SELECT_FIFO   }   /* SSIF4_SRC2_FFU   */
    ,   { SELECT_FIFO   , SELECT_SSIF1  }   /* FFD_SRC2_SSIF1   */
    ,   { SELECT_FIFO   , SELECT_SSIF4  }   /* FFD_SRC2_SSIF4   */
    ,   { SELECT_FIFO   , SELECT_FIFO   }   /* FFD_SRC2_FFU     */
    ,   { SELECT_SSIF1  , SELECT_MIX    }   /* SSIF1_SRC2_MIX   */
    ,   { SELECT_SSIF4  , SELECT_MIX    }   /* SSIF4_SRC2_MIX   */
    ,   { SELECT_FIFO   , SELECT_MIX    }   /* FFD_SRC2_MIX     */
    /* SCUX channel 3 */
    ,   { SELECT_SSIF2  , SELECT_SSIF5  }   /* SSIF2_SRC3_SSIF5 */
    ,   { SELECT_SSIF5  , SELECT_SSIF2  }   /* SSIF5_SRC3_SSIF2 */
    ,   { SELECT_SSIF2  , SELECT_FIFO   }   /* SSIF2_SRC3_FFU   */
    ,   { SELECT_SSIF5  , SELECT_FIFO   }   /* SSIF5_SRC3_FFU   */
    ,   { SELECT_FIFO   , SELECT_SSIF2  }   /* FFD_SRC3_SSIF2   */
    ,   { SELECT_FIFO   , SELECT_SSIF5  }   /* FFD_SRC3_SSIF5   */
    ,   { SELECT_FIFO   , SELECT_FIFO   }   /* FFD_SRC3_FFU     */
    ,   { SELECT_SSIF2  , SELECT_MIX    }   /* SSIF2_SRC3_MIX   */
    ,   { SELECT_SSIF5  , SELECT_MIX    }   /* SSIF5_SRC3_MIX   */
    ,   { SELECT_FIFO   , SELECT_MIX    }   /* FFD_SRC3_MIX     */
};


/******************************* API Functions                      *******************************/

/******************************************************************************
* Function Name: R_SCUX_RegistCallback
* Description  : Register Callback function
* Arguments    : scux_callback_arg -
*                    callback function
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Bad arguments.
******************************************************************************/
int32_t R_SCUX_RegistCallback (const scux_callback_t * const scux_callback_arg)
{
    int32_t ret = SCUX_OK;

    if (NULL == scux_callback_arg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        scux_callback_func = *scux_callback_arg;
    }

    return ret;
}

/******************************************************************************
* Function Name: R_SCUX_Reset
* Description  : Initialize the SCUX internal circuits by software reset.
* Arguments    : none
* Return Value : none
******************************************************************************/
void R_SCUX_Reset (void)
{
    uint8_t     mask;
    uint32_t    scux_ch_loop;
    uint32_t    scux_module_loop;

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* Run SCUX */
    COMMON_Set8BitMaskLShift(&CPG.STBCR8, &g_stbcr8_mstp81_bit_inf_tbl, 0);
    CPG.STBCR8; /* Dummy read */

    /* Reset SCUX */
    SCUX.SWRSR_CIM = RESET_ON;  /* Reset SCUX */
    SCUX.SWRSR_CIM = RESET_OFF; /* Operating state */

    /* Claer Callback */
    scux_callback_func.set_int_callback = 0;
    scux_callback_func.set_dmac_callback = 0;

    for (scux_ch_loop = SCUX_CH_0; scux_ch_loop < NUM_SCUX_CH; scux_ch_loop++)
    {
        /* Clear input/output setting value of SCUX */
        pass_inf[scux_ch_loop].io.input  = SELECT_NON;
        pass_inf[scux_ch_loop].io.output = SELECT_NON;
        pass_inf[scux_ch_loop].src_mode = ASYNC_SRC;

        /* Clear SCUX Interrupt Callback Function */
        for (scux_module_loop = SCUX_INT_ID_SRC; scux_module_loop < NUM_SCUX_INT_ID_MODULE; scux_module_loop++)
        {
            (void)R_SCUX_SetIntCallback(scux_ch_loop, scux_module_loop, 0);
        }

        /*  Clear FUTSET Register Value */
        futsel_reg_value[scux_ch_loop] = 0;
    }

    /* Clear SSICTRL Register Value */
    ssictrl_reg_value = 0;

    (void)R_INTC_SetMaskLevel(mask);
}


/******************************************************************************
* Function Name: R_SCUX_Setup
* Description  : The channel which SCUX specified is initialized.
*                Perform this function after the completion of a setting of SSIF.
* Arguments    : ch -
*                    SCUX channel
*                setup_arg -
*                    Setup parameter for SCUX
*                config_dvu_arg -
*                    Setting parameter for DVU
*                config_mix_arg -
*                    Setting parameter for MIX
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Bad arguments.
*                SCUX_E_BUSY -
*                    SCUX is busy.
*                Other value -
*                    Callback function error.
******************************************************************************/
int32_t R_SCUX_Setup (  const uint32_t ch,
                        const scux_setup_arg_t * const setup_arg,
                        const scux_config_dvu_arg_t * const config_dvu_arg,
                        const scux_config_mix_arg_t * const config_mix_arg)
{
    uint8_t mask;
    const scux_reg_address_t * scux_reg = &g_scux_reg_address_tbl[0];
    scux_pass_inf_t get_pass_inf;
    int32_t ret;

    if ((NULL == setup_arg) || (NULL == config_dvu_arg) || (NULL == config_mix_arg))
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        R_INTC_GetMaskLevel(&mask);
        (void)R_INTC_SetMaskLevel(MASK_INT_LV);

        /* ==== Check parameter ==== */
        ret = SetupCheckParam(ch, setup_arg, config_dvu_arg, config_mix_arg, &get_pass_inf);

        if (SCUX_OK == ret)
        {
            scux_reg = &g_scux_reg_address_tbl[ch];
        }

        /* ==== Check SCUX Status ==== */
        if (SCUX_OK == ret)
        {
            ret = SetupCheckStatus(scux_reg);
        }

        if (SCUX_OK == ret)
        {
            /* ==== Get SCUX global variables ==== */
            pass_inf[ch] = get_pass_inf;

            /* ==== Set pass parameter ==== */
            ret = SetupPassInOut(scux_reg, ch, setup_arg);

            if (SCUX_OK == ret)
            {
                ret = SetupPassSsifRoute(ch, setup_arg);
            }

            if (SCUX_OK == ret)
            {
                ret = SetupPassAudioExch(scux_reg, setup_arg);
            }

            if (SCUX_OK == ret)
            {
                ret = SetupPassSsifControl(ch, setup_arg);
            }

            /* ==== Set clock parameter ==== */
            if (SCUX_OK == ret)
            {
                ret = SetupClock(scux_reg, ch, setup_arg);
            }

            /* ==== Set FFU parameter ==== */
            if (SCUX_OK == ret)
            {
                if (SELECT_FIFO == pass_inf[ch].io.output)
                {
                    ret = SetupFfu(scux_reg, ch, setup_arg);
                }
            }

            /* ==== Set FFD parameter ==== */
            if (SCUX_OK == ret)
            {
                if (SELECT_FIFO == pass_inf[ch].io.input)
                {
                    ret = SetupFfd(scux_reg, ch, setup_arg);
                }
            }

            /* ==== Set SRC parameter ==== */
            if (SCUX_OK == ret)
            {
                ret = SetupSrc(scux_reg, setup_arg);
            }

            /* ==== Set DVU parameter ==== */
            if (SCUX_OK == ret)
            {
                if (SELECT_FIFO != pass_inf[ch].io.output)
                {
                    ret = SetupDvu(scux_reg, setup_arg, config_dvu_arg);
                }
            }

            /* ==== Set MIX parameter ==== */
            if (SCUX_OK == ret)
            {
                if (SELECT_NON != setup_arg->mix_output_select)
                {
                    ret = SetupMix(setup_arg, config_mix_arg);
                }
            }

            /* ==== Set interrupt ==== */
            if (SCUX_OK == ret)
            {
                if (0 != scux_callback_func.set_int_callback)
                {
                    ret = scux_callback_func.set_int_callback(ch);
                }
                else
                {
                    ret = SCUX_E_BUSY;
                }
            }
        }

        (void)R_INTC_SetMaskLevel(mask);
    }

    return ret;
}

/******************************************************************************
* Function Name: R_SCUX_ClearInit
* Description  : Clear initialization bit of SCUX
* Arguments    : ch -
*                    SCUX channel
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Bad arguments
*                SCUX_E_BUSY -
*                    SCUX is busy.
*                Other value -
*                    Callback function error.
******************************************************************************/
int32_t R_SCUX_ClearInit (const uint32_t ch)
{
    int32_t     ret;
    uint8_t     mask;
    const scux_reg_address_t * scux_reg = &g_scux_reg_address_tbl[0];

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* ==== Check parameter ==== */
    ret = CheckParamNumScuxCh(ch);

    if (SCUX_OK == ret)
    {
        scux_reg = &g_scux_reg_address_tbl[ch];
    }

    /* ==== Check SCUX Status ==== */
    if (SCUX_OK == ret)
    {
        ret = ClearInitCheckStatus(scux_reg);
    }

    /* ==== Clear initialization ==== */
    if (SCUX_OK == ret)
    {
        ret = ClearInit(scux_reg, ch);
    }

    (void)R_INTC_SetMaskLevel(mask);

    return ret;
}

/******************************************************************************
* Function Name: R_SCUX_SetInit
* Description  : Set initialization bit of SCUX
* Arguments    : ch -
*                    SCUX channel
*                mix_stop -
*                    Stop MIX module
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
int32_t R_SCUX_SetInit (const uint32_t ch, const bool_t mix_stop)
{
    int32_t     ret;
    uint8_t     mask;
    const scux_reg_address_t * scux_reg = &g_scux_reg_address_tbl[0];

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* ==== Check parameter ==== */
    ret = CheckParamNumScuxCh(ch);

    if (SCUX_OK == ret)
    {
        scux_reg = &g_scux_reg_address_tbl[ch];
    }

    /* ==== Set initialization ==== */
    if (SCUX_OK == ret)
    {
        ret = SetInit(scux_reg, ch, mix_stop);
    }

    (void)R_INTC_SetMaskLevel(mask);

    return ret;
}

/******************************************************************************
* Function Name: R_SCUX_StartFfu
* Description  : Start FFU transfer
* Arguments    : ch -
*                    SCUX channel
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Bad arguments
*                SCUX_E_BUSY -
*                    SCUX is busy.
******************************************************************************/
int32_t R_SCUX_StartFfu (const uint32_t ch)
{
    int32_t     ret;
    uint8_t     mask;
    uint32_t    scksel;
    const scux_reg_address_t * scux_reg = &g_scux_reg_address_tbl[0];

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* ==== Check parameter ==== */
    ret = CheckParamNumScuxCh(ch);

    if (SCUX_OK == ret)
    {
        scux_reg = &g_scux_reg_address_tbl[ch];
    }

    /* ==== Check SCUX Status ==== */
    if (SCUX_OK == ret)
    {
        ret = StartFifoCheckStatus(scux_reg);
    }

    if (SCUX_OK == ret)
    {
        if ((SELECT_FIFO == pass_inf[ch].io.output) && (ASYNC_SRC == pass_inf[ch].src_mode))
        {
            scksel = COMMON_Get32BitMask(futsel_reg_value[ch], g_scux_bit_inf_tbl[SCUX_FUTSEL_BIT_SCKSEL].bit_mask);
            if (SCKSEL_CLKP1_PHY_DIV2 >= scksel)
            {
                COMMON_Set32BitMaskLShift(&futsel_reg_value[ch], &g_scux_bit_inf_tbl[SCUX_FUTSEL_BIT_DIVEN], 1);
                *(scux_reg->futsel) = futsel_reg_value[ch];
            }
        }
        else
        {
            ret = SCUX_E_PAR;
        }
    }

    (void)R_INTC_SetMaskLevel(mask);

    return ret;
}

/******************************************************************************
* Function Name: R_SCUX_StartFfd
* Description  : Start FFD transfer
* Arguments    : ch -
*                    SCUX channel
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Bad arguments
*                SCUX_E_BUSY -
*                    SCUX is busy.
******************************************************************************/
int32_t R_SCUX_StartFfd (const uint32_t ch)
{
    int32_t     ret;
    uint8_t     mask;
    uint32_t    scksel;
    uint32_t    temp_reg;
    const scux_reg_address_t * scux_reg = &g_scux_reg_address_tbl[0];

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* ==== Check parameter ==== */
    ret = CheckParamNumScuxCh(ch);

    if (SCUX_OK == ret)
    {
        scux_reg = &g_scux_reg_address_tbl[ch];
    }

    /* ==== Check SCUX Status ==== */
    if (SCUX_OK == ret)
    {
        ret = StartFifoCheckStatus(scux_reg);
    }

    if (SCUX_OK == ret)
    {
        if ((SELECT_FIFO == pass_inf[ch].io.input) && (ASYNC_SRC == pass_inf[ch].src_mode))
        {
            temp_reg = *(scux_reg->fdtsel);
            scksel = COMMON_Get32BitMask(temp_reg, g_scux_bit_inf_tbl[SCUX_FDTSEL_BIT_SCKSEL].bit_mask);
            if (SCKSEL_CLKP1_PHY_DIV2 >= scksel)
            {
                COMMON_Set32BitMaskLShift(scux_reg->fdtsel, &g_scux_bit_inf_tbl[SCUX_FDTSEL_BIT_DIVEN], 1);
            }
        }
        else
        {
            ret = SCUX_E_PAR;
        }
    }

    (void)R_INTC_SetMaskLevel(mask);

    return ret;
}

/******************************************************************************
* Function Name: R_SCUX_StopFfu
* Description  : Stop FFU transfer
* Arguments    : ch -
*                    SCUX channel
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Bad arguments
*                SCUX_E_BUSY -
*                    SCUX is busy.
*                Other value -
*                    Callback function error.
******************************************************************************/
int32_t R_SCUX_StopFfu (const uint32_t ch)
{
    int32_t     ret;
    uint8_t     mask;
    uint32_t    scksel;
    const scux_reg_address_t * scux_reg = &g_scux_reg_address_tbl[0];

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* ==== Check parameter ==== */
    ret = CheckParamNumScuxCh(ch);

    if (SCUX_OK == ret)
    {
        scux_reg = &g_scux_reg_address_tbl[ch];
    }

    /* ==== Stop transmission ==== */
    if (SCUX_OK == ret)
    {
        if (SELECT_FIFO == pass_inf[ch].io.output)
        {
            if (ASYNC_SRC == pass_inf[ch].src_mode)
            {
                scksel = COMMON_Get32BitMask(futsel_reg_value[ch], g_scux_bit_inf_tbl[SCUX_FUTSEL_BIT_SCKSEL].bit_mask);
                if (SCKSEL_CLKP1_PHY_DIV2 >= scksel)
                {
                    COMMON_Set32BitMaskLShift(&futsel_reg_value[ch], &g_scux_bit_inf_tbl[SCUX_FUTSEL_BIT_DIVEN], 0);
                    *(scux_reg->futsel) = futsel_reg_value[ch];
                }
            }
            *(scux_reg->opcir) = INIT_ON;       /* Set INIT of OPC */
            
            /* ---- Stop DMA ---- */
            COMMON_Set32BitMaskLShift(&SCUX.DMACR_CIM, &g_dmacr_dmamdffu_bit_inf_tbl[ch], 0);
            if (0 != scux_callback_func.set_dmac_callback)
            {
                ret = scux_callback_func.set_dmac_callback(ch, FFU_DMA_STOP);
            }
            else
            {
                ret = SCUX_E_BUSY;
            }

            *(scux_reg->ffuir) = INIT_ON;       /* Set INIT of FFU */
        }
        else
        {
            ret = SCUX_E_PAR;
        }
    }

    (void)R_INTC_SetMaskLevel(mask);

    return ret;
}

/******************************************************************************
* Function Name: R_SCUX_StopFfd
* Description  : Stop FFD transfer
* Arguments    : ch -
*                    SCUX channel
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Bad arguments
*                SCUX_E_BUSY -
*                    SCUX is busy.
*                Other value -
*                    Callback function error.
******************************************************************************/
int32_t R_SCUX_StopFfd (const uint32_t ch)
{
    int32_t     ret;
    uint8_t     mask;
    uint32_t    scksel;
    uint32_t    temp_reg;
    const scux_reg_address_t * scux_reg = &g_scux_reg_address_tbl[0];

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* ==== Check parameter ==== */
    ret = CheckParamNumScuxCh(ch);

    if (SCUX_OK == ret)
    {
        scux_reg = &g_scux_reg_address_tbl[ch];
    }

    /* ==== Stop transmission ==== */
    if (SCUX_OK == ret)
    {
        if (SELECT_FIFO == pass_inf[ch].io.input)
        {
            if (ASYNC_SRC == pass_inf[ch].src_mode)
            {
                temp_reg = *(scux_reg->fdtsel);
                scksel = COMMON_Get32BitMask(temp_reg, g_scux_bit_inf_tbl[SCUX_FDTSEL_BIT_SCKSEL].bit_mask);
                if (SCKSEL_CLKP1_PHY_DIV2 >= scksel)
                {
                    COMMON_Set32BitMaskLShift(scux_reg->fdtsel, &g_scux_bit_inf_tbl[SCUX_FDTSEL_BIT_DIVEN], 0);
                }
            }
            *(scux_reg->ipcir) = INIT_ON;       /* Set INIT of IPC */
            *(scux_reg->ffdbr) = BOOT_OFF;      /* Halt the data request */

            /* ---- Stop DMA ---- */
            COMMON_Set32BitMaskLShift(&SCUX.DMACR_CIM, &g_dmacr_dmamdffd_bit_inf_tbl[ch], 0);
            if (0 != scux_callback_func.set_dmac_callback)
            {
                ret = scux_callback_func.set_dmac_callback(ch, FFD_DMA_STOP);
            }
            else
            {
                ret = SCUX_E_BUSY;
            }

            *(scux_reg->ffdir) = INIT_ON;       /* Set INIT of FFD */
        }
        else
        {
            ret = SCUX_E_PAR;
        }
    }

    (void)R_INTC_SetMaskLevel(mask);

    return ret;
}

/******************************************************************************
* Function Name: R_SCUX_StopDirectSsifTx
* Description  : Stop direct SSIF TX
* Arguments    : ch -
*                    SCUX channel
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Bad arguments
******************************************************************************/
int32_t R_SCUX_StopDirectSsifTx (const uint32_t ch)
{
    int32_t     ret;
    uint8_t     mask;

    const scux_reg_address_t * scux_reg = &g_scux_reg_address_tbl[0];

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* ==== Check parameter ==== */
    ret = CheckParamNumScuxCh(ch);

    if (SCUX_OK == ret)
    {
        scux_reg = &g_scux_reg_address_tbl[ch];
    }

    /* ==== Stop TX ==== */
    if (SCUX_OK == ret)
    {
        if (SELECT_FIFO != pass_inf[ch].io.output)
        {
            *(scux_reg->opcir) = INIT_ON;   /* Set INIT of OPC */
        }
        else
        {
            ret = SCUX_E_PAR;
        }
    }

    (void)R_INTC_SetMaskLevel(mask);

    return ret;
}

/******************************************************************************
* Function Name: R_SCUX_StopDirectSsifRx
* Description  : Stop direct SSIF RX
* Arguments    : ch -
*                    SCUX channel
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Bad arguments
******************************************************************************/
int32_t R_SCUX_StopDirectSsifRx (const uint32_t ch)
{
    int32_t     ret;
    uint8_t     mask;

    const scux_reg_address_t * scux_reg = &g_scux_reg_address_tbl[0];

    R_INTC_GetMaskLevel(&mask);
    (void)R_INTC_SetMaskLevel(MASK_INT_LV);

    /* ==== Check parameter ==== */
    ret = CheckParamNumScuxCh(ch);

    if (SCUX_OK == ret)
    {
        scux_reg = &g_scux_reg_address_tbl[ch];
    }

    /* ==== Stop transmission ==== */
    if (SCUX_OK == ret)
    {
        if (SELECT_FIFO != pass_inf[ch].io.input)
        {
            *(scux_reg->ipcir) = INIT_ON;   /* Set INIT of IPC */
        }
        else
        {
            ret = SCUX_E_PAR;
        }
    }

    (void)R_INTC_SetMaskLevel(mask);

    return ret;
}

/******************************************************************************
* Function Name: R_SCUX_ConfigDvu
* Description  : Change DVU parameters
* Arguments    : ch -
*                    SCUX channel
*                config_dvu_arg -
*                    Setting parameter for DVU
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Bad arguments
******************************************************************************/
int32_t R_SCUX_ConfigDvu (const uint32_t ch, const scux_config_dvu_arg_t * const config_dvu_arg)
{
    int32_t ret;
    const scux_reg_address_t * scux_reg = &g_scux_reg_address_tbl[0];

    if (NULL == config_dvu_arg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* ==== Check parameter ==== */
        ret = CheckParamNumScuxCh(ch);

        if (SCUX_OK == ret)
        {
            scux_reg = &g_scux_reg_address_tbl[ch];
        }

        if (SCUX_OK == ret)
        {
            ret = ConfigDvuCheckParam(config_dvu_arg);
        }

        if (SCUX_OK == ret)
        {
            ret = ConfigDvu(scux_reg, config_dvu_arg);
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: R_SCUX_ConfigMix
* Description  : Change MIX parameters
* Arguments    : config_mix_arg -
*                    Setting parameter for MIX
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Bad arguments
******************************************************************************/
int32_t R_SCUX_ConfigMix (const scux_config_mix_arg_t * const config_mix_arg)
{
    int32_t ret;

    if (NULL == config_mix_arg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* ==== Check parameter ==== */
        ret = ConfigMixCheckParam(config_mix_arg);

        if (SCUX_OK == ret)
        {
            ret = ConfigMix(config_mix_arg);
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: R_SCUX_ConfigEnable
* Description  : Enable Change DVU and MIX parameters
* Arguments    : config_enable_arg -
*                    Enable or Disable Setting of DVU and MIX
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Bad arguments
******************************************************************************/
int32_t R_SCUX_ConfigEnable (const scux_config_enable_arg_t * const config_enable_arg)
{
    uint32_t scux_ch_loop;
    int32_t ret = SCUX_OK;

    if (NULL == config_enable_arg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        for (scux_ch_loop = SCUX_CH_0; scux_ch_loop < NUM_SCUX_CH; scux_ch_loop++)
        {
            if (CONFIG_NO_CHANGE != config_enable_arg->dvu[scux_ch_loop])
            {
                /* Set DVUER_DVU0_n register - DVUEN bit */
                *g_scux_reg_address_tbl[scux_ch_loop].dvuer = config_enable_arg->dvu[scux_ch_loop];
            }
        }

        if (CONFIG_NO_CHANGE != config_enable_arg->mix)
        {
            /* Set MDBER_MIX0_0 register - DVUEN bit */
            SCUX.MDBER_MIX0_0 = config_enable_arg->mix;
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: R_SCUX_GetVolRampSts
* Description  : Get Volume Ramp Status
* Arguments    : ch -
*                    SCUX channel
*                *ramp_sts -
*                    Volume Ramp Status
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Bad arguments
******************************************************************************/
int32_t R_SCUX_GetVolRampSts (const uint32_t ch, scux_vol_ramp_sts_t * const ramp_sts)
{
    int32_t     ret;
    uint32_t    tmp_reg;
    const scux_reg_address_t * scux_reg = &g_scux_reg_address_tbl[0];

    if (NULL == ramp_sts)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* ==== Check parameter ==== */
        ret = CheckParamNumScuxCh(ch);

        if (SCUX_OK == ret)
        {
            scux_reg = &g_scux_reg_address_tbl[ch];
        }

        /* ==== Get Volume Ramp Status ==== */
        if (SCUX_OK == ret)
        {
            tmp_reg     = *(scux_reg->dvusr);
            *ramp_sts   = (scux_vol_ramp_sts_t)COMMON_Get32BitMask( tmp_reg,
                                                                    g_scux_bit_inf_tbl[SCUX_DVUSR_BIT_VRSTS].bit_mask);
        }
    }

    return ret;
}

/******************************* Check Parameter Functions          *******************************/

/******************************************************************************
* Function Name: SetupCheckParam
* Description  : Checking parameter for R_SCUX_Setup function
* Arguments    : ch -
*                    SCUX channel
*                setup_arg -
*                    Setup parameter for SCUX
*                config_dvu_arg -
*                    Setting parameter for DVU
*                config_mix_arg -
*                    Setting parameter for MIX
*                get_pass_inf -
*                    Get Input and Output informations of SCUX.
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Bad arguments
******************************************************************************/
static int32_t SetupCheckParam (    const uint32_t ch,
                                    const scux_setup_arg_t * const setup_arg,
                                    const scux_config_dvu_arg_t * const config_dvu_arg,
                                    const scux_config_mix_arg_t * const config_mix_arg,
                                    scux_pass_inf_t * const get_pass_inf)
{
    int32_t ret;

    if ((NULL == setup_arg) || (NULL == config_dvu_arg) || (NULL == config_mix_arg) || (NULL == get_pass_inf))
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* ==== Check SCUX channel number ==== */
        ret = CheckParamNumScuxCh(ch);

        /* ==== Check pass select ==== */
        if (SCUX_OK == ret)
        {
            if ((setup_arg->pass_select < scux_pass_min_max_tbl[ch].min) ||
                (setup_arg->pass_select > scux_pass_min_max_tbl[ch].max))
            {
                ret = SCUX_E_PAR;
            }
        }

        /* ==== Get parameter for SCUX driver control ==== */
        if (SCUX_OK == ret)
        {
            get_pass_inf->io = scux_io_tbl[setup_arg->pass_select];
            get_pass_inf->src_mode = setup_arg->src_mode;
        }

        /* ==== Check audio channel number ==== */
        if (SCUX_OK == ret)
        {
            ret = CheckParamNumAudioCh(ch, setup_arg, get_pass_inf);
        }

        /* ==== Check Combination of sync_mode, input/output, and SRC Bypass Mode ==== */
        if (SCUX_OK == ret)
        {
            ret = CheckParamSyncMode(setup_arg, get_pass_inf);
        }

        /* ==== Check FIFO Request Size ==== */
        if (SCUX_OK == ret)
        {
            /* For FFD */
            ret = CheckFifoRequestSize( ch,
                                        get_pass_inf->io.input,
                                        setup_arg->ffd_request_size,
                                        setup_arg->audio_channel);
        }

        if (SCUX_OK == ret)
        {
            /* For FFU */
            ret = CheckFifoRequestSize( ch,
                                        get_pass_inf->io.output,
                                        setup_arg->ffu_request_size,
                                        setup_arg->audio_channel);
        }

        /* ==== Check Combination of Low Delay Mode and channel number of audio data ==== */
        if (SCUX_OK == ret)
        {
            ret = CheckParamLowDelay(setup_arg);
        }

        /* ==== Check Wait Time ==== */
        if (SCUX_OK == ret)
        {
            ret = CheckParamWaitTime(setup_arg);
        }

        /* ==== Check fs ==== */
        if (SCUX_OK == ret)
        {
            ret = CheckParamFs(setup_arg);
        }

        /* ==== Check minimum FS ratio ==== */
        if (SCUX_OK == ret)
        {
            ret = CheckParamMinFsRatio(setup_arg->min_fs_ratio);
        }

        /* ==== Check Audio Channel Exchange ==== */
        if (SCUX_OK == ret)
        {
            ret = CheckParamAudioChExch(    setup_arg,
                                            setup_arg->src_use_channel_exch,
                                            setup_arg->src_channel_exch);
        }

        /* ==== Check DVU parameter ==== */
        if (false == setup_arg->dvu_bypass_mode)
        {
            if (SCUX_OK == ret)
            {
                ret = ConfigDvuCheckParam(config_dvu_arg);
            }
        }

        /* ==== Check MIX parameter ==== */
        if (SELECT_MIX == get_pass_inf[ch].io.output)
        {
            if (SCUX_OK == ret)
            {
                if ((SELECT_SSIF0 != setup_arg->mix_output_select) &&
                    (SELECT_SSIF3 != setup_arg->mix_output_select))
                {
                    ret = SCUX_E_PAR;
                }
            }

            if (SCUX_OK == ret)
            {
                ret = CheckParamAudioChExch(    setup_arg,
                                                setup_arg->mix_use_channel_exch,
                                                setup_arg->mix_channel_exch);
            }

            if (SCUX_OK == ret)
            {
                ret = ConfigMixCheckParam(config_mix_arg);
            }
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: CheckParamNumScuxCh
* Description  : Checking Number of SCUX Channel.
* Arguments    : ch -
*                    SCUX channel
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t CheckParamNumScuxCh (const uint32_t ch)
{
    int32_t ret = SCUX_OK;

    if (NUM_SCUX_CH <= ch)
    {
        ret = SCUX_E_PAR;
    }

    return ret;
}

/******************************************************************************
* Function Name: CheckParamNumAudioCh
* Description  : Checking Number of Audio Channel.
* Arguments    : ch -
*                    SCUX channel
*                setup_arg -
*                    Setup parameter for SCUX
*                get_pass_inf -
*                    Input and Output informations of SCUX.
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t CheckParamNumAudioCh (   const uint32_t ch,
                                        const scux_setup_arg_t * const setup_arg,
                                        const scux_pass_inf_t * const get_pass_inf)
{
    int32_t ret = SCUX_OK;

    if ((NULL == setup_arg) || (NULL == get_pass_inf))
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* ==== Check audio channel number ==== */
        if (((SCUX_CH_2 == ch) || (SCUX_CH_3 == ch)) &&
            ((SELECT_FIFO == get_pass_inf->io.input) || (SELECT_FIFO == get_pass_inf->io.output)))
        {
            if (NUM_AUDIO_CH_2 < setup_arg->audio_channel)
            {
                ret = SCUX_E_PAR;
            }
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: CheckParamSyncMode
* Description  : Checking Combination of src_mode and input/output and SRC Bypass Mode.
* Arguments    : setup_arg -
*                    Setup parameter for SCUX
*                get_pass_inf -
*                    Input and Output informations of SCUX.
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t CheckParamSyncMode ( const scux_setup_arg_t * const setup_arg,
                                    const scux_pass_inf_t * const get_pass_inf)
{
    int32_t ret = SCUX_OK;

    if ((NULL == setup_arg) || (NULL == get_pass_inf))
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        if (SYNC_SRC == get_pass_inf->src_mode)
        {
            if ((SELECT_FIFO != get_pass_inf->io.input) || (SELECT_FIFO != get_pass_inf->io.output))
            {
                /* Input other than FIFO in Sync Mode */
                ret = SCUX_E_PAR;
            }

            if (true == setup_arg->src_bypass_mode)
            {
                /* Bypass mode in Sync Mode */
                ret = SCUX_E_PAR;
            }
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: CheckFifoRequestSize
* Description  : Checking Parameter Request Size
* Arguments    : ch -
*                    SCUX channel
*                io_select -
*                    Target of Input or Output
*                request_size -
*                    FIFO request size
*                audio_channel -
*                    Number of Audio Channel
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t CheckFifoRequestSize (   const uint32_t ch,
                                        const scux_select_t io_select,
                                        const uint32_t request_size,
                                        const scux_num_audio_ch_t audio_channel)
{
    int32_t ret = SCUX_OK;

    if (SELECT_FIFO == io_select)
    {
        if (REQ_SIZE_1 < request_size)
        {
            ret = SCUX_E_PAR;
        }
        else
        {
            if ((SCUX_CH_2 == ch) || (SCUX_CH_3 == ch))
            {
                if (    (REQ_SIZE_64 > request_size) ||
                        ((REQ_SIZE_64 == request_size) && (NUM_AUDIO_CH_6 == audio_channel)))
                {
                    ret = SCUX_E_PAR;
                }
            }
            else {
                if ((REQ_SIZE_256 == request_size) && (NUM_AUDIO_CH_6 == audio_channel))
                {
                    ret = SCUX_E_PAR;
                }
            }
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: CheckParamLowDelay
* Description  : Checking Parameter Low Delay Mode.
* Arguments    : setup_arg -
*                    Setup parameter for SCUX
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t CheckParamLowDelay (const scux_setup_arg_t * const setup_arg)
{
    int32_t ret = SCUX_OK;

    if (NULL == setup_arg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        if (LOW_DELAY_NON != setup_arg->low_delay)
        {
            if ((NUM_AUDIO_CH_1 != setup_arg->audio_channel) &&
                (NUM_AUDIO_CH_2 != setup_arg->audio_channel))
            {
                ret = SCUX_E_PAR;
            }
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: CheckParamWaitTime
* Description  : Checking Parameter Wait Time.
* Arguments    : setup_arg -
*                    Setup parameter for SCUX
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t CheckParamWaitTime (const scux_setup_arg_t * const setup_arg)
{
    int32_t ret = SCUX_OK;

    if (NULL == setup_arg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        if ((SCUX_NO_WAIT > setup_arg->wait_time) || (SCUX_MAX_WAIT < setup_arg->wait_time))
        {
            ret = SCUX_E_PAR;
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: CheckParamFs
* Description  : Checking parameter for FS.
* Arguments    : setup_arg -
*                    Setup parameter for SCUX
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t CheckParamFs (const scux_setup_arg_t * const setup_arg)
{
    int32_t ret = SCUX_OK;

    if (NULL == setup_arg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        if ((setup_arg->output_fs != FS_8000)  && (setup_arg->output_fs != FS_16000) &&
            (setup_arg->output_fs != FS_24000) && (setup_arg->output_fs != FS_32000) &&
            (setup_arg->output_fs != FS_44100) && (setup_arg->output_fs != FS_48000) &&
            (setup_arg->output_fs != FS_96000))
        {
            ret = SCUX_E_PAR;
        }
        else
        {
            switch (setup_arg->audio_channel)
            {
                /* These 2 cases are intentionally combined. */
                case NUM_AUDIO_CH_1:
                case NUM_AUDIO_CH_2:
                        if (LOW_DELAY_NON == setup_arg->low_delay)
                        {
                            /* Normal */
                            if ((FS_8000 == setup_arg->output_fs) && (FS_64000 < setup_arg->input_fs))
                            {
                                ret = SCUX_E_PAR;
                            }
                        }
                        else if (LOW_DELAY_1 == setup_arg->low_delay)
                        {
                            /* Low Delay 1 */
                            if (((FS_8000  == setup_arg->output_fs) && (FS_16000 < setup_arg->input_fs)) ||
                                ((FS_16000 == setup_arg->output_fs) && (FS_32000 < setup_arg->input_fs)) ||
                                ((FS_24000 == setup_arg->output_fs) && (FS_48000 < setup_arg->input_fs)) ||
                                ((FS_32000 == setup_arg->output_fs) && (FS_64000 < setup_arg->input_fs)) ||
                                ((FS_44100 == setup_arg->output_fs) && (FS_88200 < setup_arg->input_fs)))
                            {
                                ret = SCUX_E_PAR;
                            }
                        }
                        else
                        {
                            /* Low Delay 2 */
                            if (((FS_8000  == setup_arg->output_fs) && (FS_8000  < setup_arg->input_fs)) ||
                                ((FS_16000 == setup_arg->output_fs) && (FS_16000 < setup_arg->input_fs)) ||
                                ((FS_24000 == setup_arg->output_fs) && (FS_24000 < setup_arg->input_fs)) ||
                                ((FS_32000 == setup_arg->output_fs) && (FS_32000 < setup_arg->input_fs)) ||
                                ((FS_44100 == setup_arg->output_fs) && (FS_44100 < setup_arg->input_fs)) ||
                                ((FS_48000 == setup_arg->output_fs) && (FS_48000 < setup_arg->input_fs)))
                            {
                                ret = SCUX_E_PAR;
                            }
                        }
                break;

                case NUM_AUDIO_CH_4:
                        if (LOW_DELAY_NON == setup_arg->low_delay)
                        {
                            /* Normal */
                            if (((FS_8000  == setup_arg->output_fs) && (FS_32000 < setup_arg->input_fs)) ||
                                ((FS_16000 == setup_arg->output_fs) && (FS_64000 < setup_arg->input_fs)))
                            {
                                ret = SCUX_E_PAR;
                            }
                        }
                        else
                        {
                                ret = SCUX_E_PAR;
                        }
                break;

                case NUM_AUDIO_CH_6:
                        if (LOW_DELAY_NON == setup_arg->low_delay)
                        {
                            /* Normal */
                            if ((FS_64000 < setup_arg->input_fs) || (FS_48000 < setup_arg->output_fs))
                            {
                                ret = SCUX_E_PAR;
                            }
                            if (((FS_8000  == setup_arg->output_fs) && (FS_16000 < setup_arg->input_fs)) ||
                                ((FS_16000 == setup_arg->output_fs) && (FS_32000 < setup_arg->input_fs)))
                            {
                                ret = SCUX_E_PAR;
                            }
                        }
                        else
                        {
                                ret = SCUX_E_PAR;
                        }
                break;

                case NUM_AUDIO_CH_8:
                        if (LOW_DELAY_NON == setup_arg->low_delay)
                        {
                            /* Normal */
                            if ((FS_48000 < setup_arg->input_fs) || (FS_48000 < setup_arg->output_fs))
                            {
                                ret = SCUX_E_PAR;
                            }
                            if (((FS_8000  == setup_arg->output_fs) && (FS_16000 < setup_arg->input_fs)) ||
                                ((FS_16000 == setup_arg->output_fs) && (FS_32000 < setup_arg->input_fs)))
                            {
                                ret = SCUX_E_PAR;
                            }
                        }
                        else
                        {
                                ret = SCUX_E_PAR;
                        }
                break;

                default:
                        ret = SCUX_E_PAR;
                break;
            }
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: CheckParamMinFsRatio
* Description  : Checking parameter for minimum FS ratio.
* Arguments    : min_fs_ratio -
*                    Minimum FS ratio
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t CheckParamMinFsRatio (const uint32_t min_fs_ratio)
{
    int32_t ret = SCUX_OK;

    if((SCUX_MIN_FS_RATIO_90 > min_fs_ratio) || (SCUX_MIN_FS_RATIO_98 < min_fs_ratio))
    {
        ret = SCUX_E_PAR;
    }

    return ret;
}

/******************************************************************************
* Function Name: CheckParamAudioChExch
* Description  : Checking Audio Channel Exchange.
* Arguments    : setup_arg -
*                    Setup parameter for SCUX
*                use_ch_exch -
*                    Use of SRC channel exchange
*                exch -
*                    Setting of SRC input channel for output channel
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t CheckParamAudioChExch (  const scux_setup_arg_t * const setup_arg,
                                        const bool_t use_ch_exch,
                                        const uint32_t * const exch)
{
    int32_t     ret = SCUX_OK;
    uint32_t    audio_ch_loop;

    if ((NULL == setup_arg) || (NULL == exch))
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        if (true == use_ch_exch)
        {
            for (audio_ch_loop = AUDIO_CH_0; audio_ch_loop < (uint32_t)setup_arg->audio_channel; audio_ch_loop++)
            {
                if ((uint32_t)setup_arg->audio_channel <= exch[audio_ch_loop])
                {
                    /* Over channel */
                    ret = SCUX_E_PAR;
                    break;
                }
            }
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: ConfigDvuCheckParam
* Description  : Checking parameter for config_dvu_arg
*                config_dvu_arg -
*                    Setting parameter for DVU
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t ConfigDvuCheckParam (const scux_config_dvu_arg_t * const config_dvu_arg)
{
    int32_t     ret = SCUX_OK;
    uint32_t    audio_ch_loop;

    if (NULL == config_dvu_arg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        if (SCUX_MIN_RAMP_GAIN < config_dvu_arg->ramp_gain)
        {
            ret = SCUX_E_PAR;
        }

        if (SCUX_MAX_RAMP_WAIT < config_dvu_arg->ramp_wait_time)
        {
            ret = SCUX_E_PAR;
        }

        for (audio_ch_loop = AUDIO_CH_0; audio_ch_loop < NUM_AUDIO_CH; audio_ch_loop++)
        {
            if (SCUX_MAX_VOL_VALUE < config_dvu_arg->vol_value[audio_ch_loop])
            {
                ret = SCUX_E_PAR;
                break;
            }
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: ConfigMixCheckParam
* Description  : Checking parameter for config_mix_arg
*                config_mix_arg -
*                    Setting parameter for MIX
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t ConfigMixCheckParam (const scux_config_mix_arg_t * const config_mix_arg)
{
    int32_t     ret = SCUX_OK;
    uint32_t    scux_ch_loop;

    if (NULL == config_mix_arg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        for (scux_ch_loop = SCUX_CH_0; scux_ch_loop < NUM_SCUX_CH; scux_ch_loop++)
        {
            if (SCUX_MIN_MIX_GAIN < config_mix_arg->mix_gain[scux_ch_loop])
            {
                ret = SCUX_E_PAR;
                break;
            }
        }
    }

    return ret;
}


/******************************* Check Status Functions          *******************************/

/******************************************************************************
* Function Name: SetupCheckStatus
* Description  : Checking status for Setup function
* Arguments    : scux_reg -
*                    SCUX register address of target SCUX channel
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Parameter Error.
*                SCUX_E_BUSY -
*                    SCUX is busy.
******************************************************************************/
static int32_t SetupCheckStatus (const scux_reg_address_t * const scux_reg)
{
    int32_t ret = SCUX_OK;

    if (NULL == scux_reg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        if (INIT_OFF == *(scux_reg->ipcir))
        {
            ret = SCUX_E_BUSY;
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: ClearInitCheckStatus
* Description  : Checking status for ClearInit function
* Arguments    : scux_reg -
*                    SCUX register address of target SCUX channel
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Parameter Error.
*                SCUX_E_BUSY -
*                    SCUX is busy.
******************************************************************************/
static int32_t ClearInitCheckStatus (const scux_reg_address_t * const scux_reg)
{
    int32_t ret = SCUX_OK;

    if (NULL == scux_reg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        if (INIT_OFF == *(scux_reg->ipcir))
        {
            ret = SCUX_E_BUSY;
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: StartFifoCheckStatus
* Description  : Checking status for StartFifo function
* Arguments    : scux_reg -
*                    SCUX register address of target SCUX channel
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Parameter Error.
*                SCUX_E_BUSY -
*                    SCUX is busy.
******************************************************************************/
static int32_t StartFifoCheckStatus (const scux_reg_address_t * const scux_reg)
{
    int32_t ret = SCUX_OK;

    if (NULL == scux_reg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        if (INIT_ON == *(scux_reg->ipcir))
        {
            ret = SCUX_E_BUSY;
        }
    }

    return ret;
}

/******************************* Register Setting Functions         *******************************/

/******************************************************************************
* Function Name: SetupPassInOut
* Description  : Set registers related to Pass
* Arguments    : scux_reg -
*                    SCUX register address of target SCUX channel
*                ch -
*                    SCUX channel
*                setup_arg -
*                    Setup parameter for SCUX
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t SetupPassInOut ( const scux_reg_address_t * const scux_reg,
                                const uint32_t ch,
                                const scux_setup_arg_t * const setup_arg)
{
    int32_t     ret = SCUX_OK;
    uint32_t    ffdpr_value;
    uint32_t    ffupr_value;
    uint32_t    ipslr_value;
    uint32_t    opslr_value;

    if ((NULL == scux_reg) || (NULL == setup_arg))
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        if (SELECT_FIFO == pass_inf[ch].io.input)
        {
            if (ASYNC_SRC == setup_arg->src_mode)
            {
                ffdpr_value = FFDPR_CIM_FFD_IPC_ASYNC;
                ipslr_value = IPSLR_FFD_IPC_ASYNC;
            }
            else
            {
                ffdpr_value = FFDPR_CIM_FFD_IPC_SYNC;
                ipslr_value = IPSLR_FFD_IPC_SYNC;
            }
        }
        else
        {
            ffdpr_value = FFDPR_NO_PASS_SELECT;
            ipslr_value = IPSLR_SSIF_IPC_ASYNC;
        }

        if (SELECT_FIFO == pass_inf[ch].io.output)
        {
            if (ASYNC_SRC == setup_arg->src_mode)
            {
                ffupr_value = FFUPR_CIM_FFU_OPC_ASYNC;
                opslr_value = OPSLR_ASYNC_OPC_FFU;
            }
            else
            {
                ffupr_value = FFUPR_CIM_FFU_OPC_SYNC;
                opslr_value = OPSLR_SYNC_OPC_FFU;
            }
        }
        else
        {
            ffupr_value = FFUPR_NO_PASS_SELECT;
            opslr_value = OPSLR_ASYNC_OPC_DVU;
        }

        /* Configure SCUX
        FFDPR_FFD0_n - FFD0_n FIFO Download Pass Register(n=0,1,2,3)
        b1:b0 PASS - Data Pass Select - Set parameter
        Other Reserved - The write value should always be 0 */
        *(scux_reg->ffdpr) = ffdpr_value;

        /* Configure SCUX
        FFUPR_FFU0_n - FFU0_n FIFO Upload Pass Register(n=0,1,2,3)
        b1:b0 PASS - Data Pass Select - Set parameter
        Other Reserved - The write value should always be 0 */
        *(scux_reg->ffupr) = ffupr_value;

        /* Configure SCUX
        IPSLR_IPC0_n - IPC0_n Pass Select Register(n=0,1,2,3)
        b1:b0 IPC_PASS_SEL - Pass selection of IPC - Set parameter
        Other Reserved - The write value should always be 0 */
        *(scux_reg->ipslr) = ipslr_value;

        /* Configure SCUX
        OPSLR_OPC0_n - OPC0_n Pass Select Register(n=0,1,2,3)
        b1:b0 OPC_PASS_SEL - Pass selection of OPC - Set parameter
        Other Reserved - The write value should always be 0 */
        *(scux_reg->opslr) = opslr_value;
    }

    return ret;
}

/******************************************************************************
* Function Name: SetupPassSsifRoute
* Description  : Set SSIRSEL_CIM register
* Arguments    : ch -
*                    SCUX channel
*                setup_arg -
*                    Setup parameter for SCUX
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t SetupPassSsifRoute (const uint32_t ch, const scux_setup_arg_t * const setup_arg)
{
    int32_t     ret = SCUX_OK;
    uint32_t    temp_reg;

    if (NULL == setup_arg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* Configure SCUX
        SSIRSEL_CIM - SSI Route Select Register
        b31:b30 SISEL3 - SRC3 SSIF Input Select - Set parameter
        b29:b28 SISEL2 - SRC2 SSIF Input Select - Set parameter
        b27:b26 SISEL1 - SRC1 SSIF Input Select - Input from SSIF3
        b25:b24 SISEL0 - SRC0 SSIF Input Select - input from SSIF0
        b21:b20 SOSEL5 - SSIF5 Output Select - Output from SRC3 (DVU0_3)
        b17:b16 SOSEL4 - SSIF4 Output Select - Output from SRC2 (DVU0_2)
        b13:b12 SOSEL3 - SSIF3 Output Select - Set parameter
        b9:b8   SOSEL2 - SSIF2 Output Select - Output from SRC3 (DVU0_3)
        b5:b4   SOSEL1 - SSIF1 Output Select - Output from SRC2 (DVU0_2)
        b1:b0   SOSEL0 - SSIF0 Output Select - Set parameter
        Other Reserved - The write value should always be 0 */
        temp_reg = SCUX.SSIRSEL_CIM;
        switch (ch)
        {
            case SCUX_CH_3:
                /* ---- SSIF Input select ---- */
                if (SELECT_SSIF2 == pass_inf[ch].io.input)
                {
                    /* SRC3:Input from SSIF2 */
                    COMMON_Set32BitMaskLShift(  &temp_reg,
                                                &g_scux_bit_inf_tbl[SCUX_SSIRSEL_BIT_SISEL3],
                                                SISEL3_SSIF2_SELECT);
                }
                else if (SELECT_SSIF5 == pass_inf[ch].io.input)
                {
                    /* SRC3:Input from SSIF5 */
                    COMMON_Set32BitMaskLShift(  &temp_reg,
                                                &g_scux_bit_inf_tbl[SCUX_SSIRSEL_BIT_SISEL3],
                                                SISEL3_SSIF5_SELECT);
                }
                else
                {
                    /* Do Nothing */
                }
            break;

            case SCUX_CH_2:
                /* ---- SSIF Input select ---- */
                if (SELECT_SSIF1 == pass_inf[ch].io.input)
                {
                    /* SRC2:Input from SSIF1 */
                    COMMON_Set32BitMaskLShift(  &temp_reg,
                                                &g_scux_bit_inf_tbl[SCUX_SSIRSEL_BIT_SISEL2],
                                                SISEL2_SSIF1_SELECT);
                }
                else if (SELECT_SSIF4 == pass_inf[ch].io.input)
                {
                    /* SRC2:Input from SSIF4 */
                    COMMON_Set32BitMaskLShift(  &temp_reg,
                                                &g_scux_bit_inf_tbl[SCUX_SSIRSEL_BIT_SISEL2],
                                                SISEL2_SSIF4_SELECT);
                }
                else
                {
                    /* Do Nothing */
                }
            break;

            case SCUX_CH_1:
                /* ---- SSIF Output select ---- */
                if (SELECT_SSIF3 == pass_inf[ch].io.output)
                {
                    /* SRC1:Output to SSIF3 */
                    COMMON_Set32BitMaskLShift(  &temp_reg,
                                                &g_scux_bit_inf_tbl[SCUX_SSIRSEL_BIT_SOSEL3],
                                                SOSEL3_SRC1_SELECT);
                }
                else if (SELECT_SSIF0 == pass_inf[ch].io.output)
                {
                    /* SRC1:Output to SSIF0 */
                    COMMON_Set32BitMaskLShift(  &temp_reg,
                                                &g_scux_bit_inf_tbl[SCUX_SSIRSEL_BIT_SOSEL0],
                                                SOSEL0_SRC1_SELECT);
                }
                else
                {
                    /* Do Nothing */
                }
            break;

            case SCUX_CH_0:
                /* ---- SSIF Output select ---- */
                if (SELECT_SSIF3 == pass_inf[ch].io.output)
                {
                    /* SRC0:Output to SSIF3 */
                    COMMON_Set32BitMaskLShift(  &temp_reg,
                                                &g_scux_bit_inf_tbl[SCUX_SSIRSEL_BIT_SOSEL3],
                                                SOSEL3_SRC0_SELECT);
                }
                else if (SELECT_SSIF0 == pass_inf[ch].io.output)
                {
                    /* SRC0:Output to SSIF0 */
                    COMMON_Set32BitMaskLShift(  &temp_reg,
                                                &g_scux_bit_inf_tbl[SCUX_SSIRSEL_BIT_SOSEL0],
                                                SOSEL0_SRC0_SELECT);
                }
                else
                {
                    /* Do Nothing */
                }
            break;

            default:
                /* Do Nothing */
            break;
        }

        if (SELECT_SSIF0 == setup_arg->mix_output_select)
        {
            /* MIX:Output to SSIF0 */
            COMMON_Set32BitMaskLShift(&temp_reg, &g_scux_bit_inf_tbl[SCUX_SSIRSEL_BIT_SOSEL0], SOSEL0_MIX_SELECT);
        }
        else if (SELECT_SSIF3 == setup_arg->mix_output_select)
        {
            /* MIX:Output to SSIF3 */
            COMMON_Set32BitMaskLShift(&temp_reg, &g_scux_bit_inf_tbl[SCUX_SSIRSEL_BIT_SOSEL3], SOSEL3_MIX_SELECT);
        }
        else
        {
            /* Do Nothing */
        }
        SCUX.SSIRSEL_CIM = temp_reg;
    }

    return ret;
}

/******************************************************************************
* Function Name: SetupPassSsifControl
* Description  : Set SSICTRL_CIM register
* Arguments    : ch -
*                    SCUX channel
*                setup_arg -
*                    Setup parameter for SCUX
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t SetupPassSsifControl (const uint32_t ch, const scux_setup_arg_t * const setup_arg)
{
    int32_t     ret = SCUX_OK;

    if (NULL == setup_arg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* Configure SCUX
        SSICTRL_CIM - SSI Control Register
        b30:28 SSI3TX,SSI4TX,SSI5TX - SSIF3,4,5 Direct Transmission - Set parameter
        b26:24 SSI3RX,SSI4RX,SSI5RX - SSIF3,4,5 Direct Reception - Set parameter
        b17    SSI345TEN - SSIF345 Transmission Enable - Transmission disabled
        b16    SSI345REN - SSIF345 Reception Enable - Reception disabled
        b14:12 SSI0TX,SSI1TX,SSI2TX - SSIF0,1,2 Direct Transmission - Set parameter
        b11:9  SSI0RX,SSI1RX,SSI2RX - SSIF0,1,2 Direct Reception - Set parameter
        b1     SSI012TEN - SSIF012 Transmission Enable - Transmission disabled
        b0     SSI012REN - SSIF012 Reception Enable - Reception disabled
        Other Reserved - The write value should always be 0 */
        if (SELECT_SSIF5 >= pass_inf[ch].io.input)
        {
            COMMON_Set32BitMaskLShift(&ssictrl_reg_value, &g_ssictrl_ssirx_bit_inf_tbl[pass_inf[ch].io.input], 1);
        }

        if (SELECT_SSIF5 >= pass_inf[ch].io.output)
        {
            COMMON_Set32BitMaskLShift(&ssictrl_reg_value, &g_ssictrl_ssitx_bit_inf_tbl[pass_inf[ch].io.output], 1);
        }

        if (SELECT_NON != setup_arg->mix_output_select)
        {
            COMMON_Set32BitMaskLShift(  &ssictrl_reg_value,
                                        &g_ssictrl_ssitx_bit_inf_tbl[setup_arg->mix_output_select],
                                        1);
        }
        SCUX.SSICTRL_CIM = ssictrl_reg_value;
        /* SSICTRL_CIM Register is write-only register. Therefore, I store the value to ssictrl_reg_value. */
    }

    return ret;
}

/******************************************************************************
* Function Name: SetupPassAudioExch
* Description  : Set registers related to xchange of the audio channels
* Arguments    : scux_reg -
*                    SCUX register address of target SCUX channel
*                setup_arg -
*                    Setup parameter for SCUX
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t SetupPassAudioExch ( const scux_reg_address_t * const scux_reg,
                                    const scux_setup_arg_t * const setup_arg)
{
    int32_t ret = SCUX_OK;
    uint32_t    temp_reg;
    uint32_t    audio_ch_loop;

    if ((NULL == scux_reg) || (NULL == setup_arg))
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* Configure SCUX
        SRCRSELn_CIM - SRCn Route Select Register(n=0,1,2,3)
        b30:28 PLACE7 - Place 7 - Set parameter
        b26:24 PLACE6 - Place 6 - Set parameter
        b26:24 PLACE5 - Place 5 - Set parameter
        b26:24 PLACE4 - Place 4 - Set parameter
        b26:24 PLACE3 - Place 3 - Set parameter
        b26:24 PLACE2 - Place 2 - Set parameter
        b26:24 PLACE1 - Place 1 - Set parameter
        b26:24 PLACE0 - Place 0 - Set parameter
        Other Reserved - The write value should always be 0 */
        temp_reg = RSEL_INIT_VALUE;
        if (true == setup_arg->src_use_channel_exch)
        {
            for (audio_ch_loop = AUDIO_CH_0; audio_ch_loop < (uint32_t)setup_arg->audio_channel; audio_ch_loop++)
            {
                COMMON_Set32BitMaskLShift(  &temp_reg,
                                    &g_rsel_place_bit_inf_tbl[audio_ch_loop],
                                    setup_arg->src_channel_exch[audio_ch_loop]);
            }
        }
        *(scux_reg->srcrsel) = temp_reg;

        /* Configure SCUX
        MIXRSEL_CIM - MIX Route Select Register
        b30:28 PLACE7 - Place 7 - Set parameter
        b26:24 PLACE6 - Place 6 - Set parameter
        b26:24 PLACE5 - Place 5 - Set parameter
        b26:24 PLACE4 - Place 4 - Set parameter
        b26:24 PLACE3 - Place 3 - Set parameter
        b26:24 PLACE2 - Place 2 - Set parameter
        b26:24 PLACE1 - Place 1 - Set parameter
        b26:24 PLACE0 - Place 0 - Set parameter
        Other Reserved - The write value should always be 0 */
        temp_reg = RSEL_INIT_VALUE;
        if (true == setup_arg->mix_use_channel_exch)
        {
            for (audio_ch_loop = AUDIO_CH_0; audio_ch_loop < (uint32_t)setup_arg->audio_channel; audio_ch_loop++)
            {
                COMMON_Set32BitMaskLShift(  &temp_reg,
                                    &g_rsel_place_bit_inf_tbl[audio_ch_loop],
                                    setup_arg->mix_channel_exch[audio_ch_loop]);
            }
        }
        SCUX.MIXRSEL_CIM = temp_reg;
    }

    return ret;
}

/******************************************************************************
* Function Name: SetupClock
* Description  : Set registers related to Clock
* Arguments    : scux_reg -
*                    SCUX register address of target SCUX channel
*                ch -
*                    SCUX channel
*                setup_arg -
*                    Setup parameter for SCUX
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t SetupClock ( const scux_reg_address_t * const scux_reg,
                            const uint32_t ch,
                            const scux_setup_arg_t * const setup_arg)
{
    int32_t ret = SCUX_OK;

    /* Value of FDTSELn_CIM(n=0,1,2,3) */
    static const uint32_t fdtsel_value_tbl[NUM_SCUX_CH] =
    {
            FDTSEL_CH_0_VALUE
        ,   FDTSEL_CH_1_VALUE
        ,   FDTSEL_CH_2_VALUE
        ,   FDTSEL_CH_3_VALUE
    };

    /* Value of FUTSELn_CIM(n=0,1,2,3) */
    static const uint32_t futsel_value_tbl[NUM_SCUX_CH] =
    {
            FUTSEL_CH_0_VALUE
        ,   FUTSEL_CH_1_VALUE
        ,   FUTSEL_CH_2_VALUE
        ,   FUTSEL_CH_3_VALUE
    };

    if ((NULL == scux_reg) || (NULL == setup_arg))
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* Configure SCUX
        FDTSELn_CIM - FFD0_n Timing Select Register(n=0,1,2,3)
        b26:16 SCKDIV - Division Ratio - Set value of the definition
        b8     DIVEN - Division Enable - Division disabled
        b3:0   SCKSEL - Clock Select - Set value of the definition
        Other Reserved - The write value should always be 0 */
        if (SELECT_FIFO == pass_inf[ch].io.input)
        {
            *(scux_reg->fdtsel) = fdtsel_value_tbl[ch];
        }

        if (SELECT_FIFO == pass_inf[ch].io.output)
        {
            /* Configure SCUX
            FUTSELn_CIM - FFU0_n Timing Select Register(n=0,1,2,3)
            b26:16 SCKDIV - Division Ratio - Set value of the definition
            b8     DIVEN - Division Enable - Division disabled
            b3:0   SCKSEL - Clock Select - Set value of the definition
            Other Reserved - The write value should always be 0 */
            futsel_reg_value[ch] = futsel_value_tbl[ch];
            *(scux_reg->futsel) = futsel_reg_value[ch];
        }
        else if (SELECT_MIX == pass_inf[ch].io.output)
        {
            /* Configure SCUX
            FUTSELn_CIM - FFU0_n Timing Select Register(n=0,1,2,3)
            b26:16 SCKDIV - Division Ratio - No division
            b8     DIVEN - Division Enable - Division disabled
            b3:0   SCKSEL - Clock Select - SSIF0 WS or SSIF3 WS
            Other Reserved - The write value should always be 0 */
            if (SELECT_SSIF0 == setup_arg->mix_output_select)
            {
                *(scux_reg->futsel) = SCKSEL_SSIF0_WS;
            }
            else
            {
                *(scux_reg->futsel) = SCKSEL_SSIF3_WS;
            }
        }
        else
        {
            /* Do Nothing */
        }

        /* Configure SCUX
        SSIPMD_CIM - SSI Pin Mode Register
        b21    SSI5CKS - SSIF5 Clock Select
        b20    SSI4CKS - SSIF4 Clock Select
        b19    SSI3CKS - SSIF3 Clock Select
        b18    SSI2CKS - SSIF2 Clock Select
        b17    SSI1CKS - SSIF1 Clock Select
        b16    SSI0CKS - SSIF0 Clock Select
        b15:14 SSI3PMD - SSIF3 Pin Mode
        b12    SSI345EN - SSIF345 Enable - Six channels mode disabled
        b11:10 SSI4PMD - SSIF4 Pin Mode
        b9:8   SSI5PMD - SSIF5 Pin Mode
        b4     SSI012EN - SSIF012 Enable - Six channels mode disabled
        b3:2   SSI1PMD - SSIF1 Pin Mode
        b1:0   SSI0PMD - SSIF2 Pin Mode
        Other Reserved - The write value should always be 0 */
        SCUX.SSIPMD_CIM = SSIPMD_VALUE;
    }

    return ret;
}

/******************************************************************************
* Function Name: SetupFfd
* Description  : Set registers related to FFD
* Arguments    : scux_reg -
*                    SCUX register address of target SCUX channel
*                ch -
*                    SCUX channel
*                setup_arg -
*                    Setup parameter for SCUX
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t SetupFfd (   const scux_reg_address_t * const scux_reg,
                            const uint32_t ch,
                            const scux_setup_arg_t * const setup_arg)
{
    int32_t     ret = SCUX_OK;

    if ((NULL == scux_reg) || (NULL == setup_arg))
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* Configure SCUX
        FDAIR_FFD0_n - FFD0_n FIFO Download Audio Information Register(n=0,1,2,3)
        b3:b0 CHNUM - Channel Number - Set parameter
        Other Reserved - The write value should always be 0 */
        *(scux_reg->fdair) = setup_arg->audio_channel;

        /* Configure SCUX
        DRQSR_FFD0_n - FFD0_n FIFO Download Request Size Register(n=0,1,2,3)
        b3:b0 SIZE - Request size data - Set parameter
        Other Reserved - The write value should always be 0 */
        if ((SCUX_CH_2 == ch) || (SCUX_CH_3 == ch))
        {
            *(scux_reg->drqsr) = setup_arg->ffd_request_size - REQ_SIZE_64;
            /* In the case of SCUX Channel 2 or 3, a value of 64 data is 0.
               Therefore, I adjust the standard value by subtracting a value of 64 data. */
        }
        else
        {
            *(scux_reg->drqsr) = setup_arg->ffd_request_size;
        }

        /* Configure SCUX
        DEVMR_FFD0_n - FFD0_n FIFO Download Event Mask Register(n=0,1,2,3)
        b31   DEVMUF - FFD Underflow Mask - Interrupt enabled
        b30   DEVMOF - FFD Overflow Mask - Interrupt enabled
        b29   DEVMOL - FFD Overlap Mask - Interrupt enabled
        b28   DEVMIUF - FFD Initialization Underflow Mask - Interrupt enabled
        b15   DEVMRQ - FFD Request Packet Mask - Interrupt disabled
        Other Reserved - The write value should always be 0 */
        *(scux_reg->devmr) = DEVMR_VALUE;
    }

    return ret;
}


/******************************************************************************
* Function Name: SetupFfu
* Description  : Set registers related to FFU
* Arguments    : scux_reg -
*                    SCUX register address of target SCUX channel
*                ch -
*                    SCUX channel
*                setup_arg -
*                    Setup parameter for SCUX
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t SetupFfu (   const scux_reg_address_t * const scux_reg,
                            const uint32_t ch,
                            const scux_setup_arg_t * const setup_arg)
{
    int32_t ret = SCUX_OK;

    if ((NULL == scux_reg) || (NULL == setup_arg))
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* Configure SCUX
        FUAIR_FFU0_n - FFU0_n FIFO Upload Audio Information Register(n=0,1,2,3)
        b3:b0 CHNUM - Channel Number - Set parameter
        Other Reserved - The write value should always be 0 */
        *(scux_reg->fuair) = setup_arg->audio_channel;

        /* Configure SCUX
        URQSR_FFU0_n - FFU0_n FIFO Upload Request Size Register(n=0,1,2,3)
        b3:b0 SIZE - Request size data - Set parameter
        Other Reserved - The write value should always be 0 */
        if ((SCUX_CH_2 == ch) || (SCUX_CH_3 == ch))
        {
            *(scux_reg->urqsr) = setup_arg->ffu_request_size - REQ_SIZE_64;
            /* In the case of SCUX Channel 2 or 3, a value of 64 data is 0.
               Therefore, I adjust the standard value by subtracting a value of 64 data. */
        }
        else {
            *(scux_reg->urqsr) = setup_arg->ffu_request_size;
        }

        /* Configure SCUX
        UEVMR_FFU0_n - FFU0_n FIFO Upload Event Mask Register(n=0,1,2,3)
        b31   DEVMUF - FFU Underflow Mask - Interrupt enabled
        b30   DEVMOF - FFU Overflow Mask - Interrupt enabled
        b29   DEVMOL - FFU Overlap Mask - Interrupt enabled
        b15   DEVMRQ - FFU Request Packet Mask - Interrupt disabled
        Other Reserved - The write value should always be 0 */
        *(scux_reg->uevmr) = UEVMR_VALUE;
    }

    return ret;
}

/******************************************************************************
* Function Name: SetupSrc
* Description  : Set registers related to SRC
* Arguments    : scux_reg -
*                    SCUX register address of target SCUX channel
*                setup_arg -
*                    Setup parameter for SCUX
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t SetupSrc (const scux_reg_address_t * const scux_reg, const scux_setup_arg_t * const setup_arg)
{
    int32_t     ret = SCUX_OK;
    uint32_t    temp_reg;
    uint32_t    output_fs_loop;

    /* Output FS ID translation table for IFSVRp_2SRC0_m Register value table */
    static const scux_fs_t output_fs_id_tbl[NUM_OUTPUT_FS] =
    {
        FS_8000, FS_16000, FS_24000, FS_32000, FS_44100, FS_48000, FS_96000
    };

    /* IFSVRp_2SRC0_m Register value table */
    static const uint32_t ifsvr_value_tbl[NUM_INPUT_FS][NUM_OUTPUT_FS] =
    {
            /*                   Output fs
              8KHz       , 16KHz      , 24KHz      ,   32KHz    ,
              44.1KHz    , 48KHz      , 96KHz */
            { 0x0400000u, 0x0200000u, 0x0155555u, 0x0100000u,       /* Input fs     */
              0x00b9c27u, 0x00aaaaau, 0x0055555u }                  /* 8KHz         */
        ,   { 0x0583333u, 0x02c1999u, 0x01d6666u, 0x0160cccu,
              0x0100000u, 0x00eb333u, 0x0075999u }                  /* 11.025KHz    */
        ,   { 0x0600000u, 0x0300000u, 0x0200000u, 0x0180000u,
              0x0116a3bu, 0x0100000u, 0x0080000u }                  /* 12KHz        */
        ,   { 0x0800000u, 0x0400000u, 0x02aaaaau, 0x0200000u,
              0x017384eu, 0x0155555u, 0x00aaaaau }                  /* 16KHz        */
        ,   { 0x0b06666u, 0x0583333u, 0x03accccu, 0x02c1999u,
              0x0200000u, 0x01d6666u, 0x00eb333u }                  /* 22.05KHz     */
        ,   { 0x0c00000u, 0x0600000u, 0x0400000u, 0x0300000u,
              0x022d476u, 0x0200000u, 0x0100000u }                  /* 24KHz        */
        ,   { 0x1000000u, 0x0800000u, 0x0555555u, 0x0400000u,
              0x02e709du, 0x02aaaaau, 0x0155555u }                  /* 32KHz        */
        ,   { 0x160ccccu, 0x0b06666u, 0x0759999u, 0x0583333u,
              0x0400000u, 0x03accccu, 0x01d6666u }                  /* 44.1KHz      */
        ,   { 0x1800000u, 0x0c00000u, 0x0800000u, 0x0600000u,
              0x045a8ecu, 0x0400000u, 0x0200000u }                  /* 48KHz        */
        ,   { 0x2000000u, 0x1000000u, 0x0aaaaaau, 0x0800000u,
              0x05ce13bu, 0x0555555u, 0x02aaaaau }                  /* 64KHz        */
        ,   { NO_IFSVR,   0x160ccccu, 0x0eb3333u, 0x0b06666u,
              0x0800000u, 0x0759999u, 0x03accccu }                  /* 88.2KHz      */
        ,   { NO_IFSVR,   0x1800000u, 0x1000000u, 0x0c00000u,
              0x08b51d9u, 0x0800000u, 0x0400000u }                  /* 96KHz        */
    };

    if ((NULL == scux_reg) || (NULL == setup_arg))
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* Configure SCUX
        SRCBRp_2SRC0_m - 2SRC0_m Bypass Register p(m=0,1 p=0,1)
        b0    BYPASS - Bypass Mode - Set parameter
        Other Reserved - The write value should always be 0 */
        *(scux_reg->srcbr) = (uint32_t)setup_arg->src_bypass_mode;

        if (false == setup_arg->src_bypass_mode)
        {
            /* Configure SCUX
            SADIRp_2SRC0_m - 2SRC0_m Audio Information Register p(m=0,1 p=0,1)
            b20:b16 OTBL - Bit Length of Output Audio Data - Set parameter
            b3:b0   CHNUM - Channel Number - Set parameter
            Other   Reserved - The write value should always be 0 */
            temp_reg = setup_arg->audio_channel;
            COMMON_Set32BitMaskLShift(  &temp_reg,
                                        &g_scux_bit_inf_tbl[SCUX_SADIR_BIT_OTBL],
                                        (uint32_t)setup_arg->bit_length);
            *(scux_reg->sadir) = temp_reg;

            /* Configure SCUX
            IFSCRp_2SRC0_m - 2SRC0_m IFS Control Register p(m=0,1 p=0,1)
            b0    INTIFSEN - INTIFS value setting enable - Initial value enabled
            Other Reserved - The write value should always be 0 */
            *(scux_reg->ifscr) = IFSCR_VALUE;

            /* Configure SCUX
            IFSVRp_2SRC0_m - 2SRC0_m IFS Value Setting Register p(m=0,1 p=0,1)
            b27:b0 INTIFS - Initial value of FSI - Get ifsver_value table
            Other  Reserved - The write value should always be 0 */
            for (output_fs_loop = 0; output_fs_loop < NUM_OUTPUT_FS; output_fs_loop++)
            {
                if (output_fs_id_tbl[output_fs_loop] == setup_arg->output_fs)
                {
                    *(scux_reg->ifsvr) = ifsvr_value_tbl[setup_arg->input_fs][output_fs_loop];
                    break;
                }
            }

            /* Configure SCUX
            SRCCRp_2SRC0_m - 2SRC0_m Control Register p(m=0,1 p=0,1)
            b20     WATMD - Wait Time Control of SRC - Set parameter
            b12     BUFMD - Low Delay Control by Buffer Size of SRC - Set parameter
            b0      SRCMD - Select SRC Mode of SRCI - Set parameter
            b16,8,4 Reserved - These bits should be set to 1
            Other   Reserved - The write value should always be 0 */
            temp_reg = SRCCR_INIT_VALUE;
            COMMON_Set32BitMaskLShift(  &temp_reg,
                                        &g_scux_bit_inf_tbl[SCUX_SRCCR_BIT_SRCMD],
                                        (uint32_t)setup_arg->src_mode);
            if (LOW_DELAY_NON == setup_arg->low_delay)
            {
                COMMON_Set32BitMaskLShift(&temp_reg, &g_scux_bit_inf_tbl[SCUX_SRCCR_BIT_BUFMD], 0);
            }
            else
            {
                COMMON_Set32BitMaskLShift(&temp_reg, &g_scux_bit_inf_tbl[SCUX_SRCCR_BIT_BUFMD], 1);
            }
            if (SCUX_NO_WAIT == setup_arg->wait_time)
            {
                COMMON_Set32BitMaskLShift(&temp_reg, &g_scux_bit_inf_tbl[SCUX_SRCCR_BIT_WATMD], 0);
            }
            else
            {
                COMMON_Set32BitMaskLShift(&temp_reg, &g_scux_bit_inf_tbl[SCUX_SRCCR_BIT_WATMD], 1);
            }
            *(scux_reg->srccr) = temp_reg;

            /* Configure SCUX
            MNFSRp_2SRC0_m - 2SRC0_m Minimum FS Setting Register p(m=0,1 p=0,1)
            b27:b0 MINFS - Set the minimum FS ratio - Set parameter
            Other  Reserved - The write value should always be 0 */
            temp_reg = *(scux_reg->ifsvr);
            temp_reg = MultiplyIntAndFixedPoint(    temp_reg,
                                                    setup_arg->min_fs_ratio,
                                                    MIN_FS_RATIO_DECIMAL_DIGITS);
            *(scux_reg->mnfsr) = temp_reg;

            /* Configure SCUX
            BFSSRp_2SRC0_m - 2SRC0_m Buffer Size Setting Register p(m=0,1 p=0,1)
            b25:b16 BUFDATA - Set the buffer size of 1 channel in DATA RAM - Set parameter
            b3:b0   BUFIN - Set the buffer size of 1 channel in IJEC RAM - Sets 0x5 for low delay mode
            Other   Reserved - The write value should always be 0 */
            if (LOW_DELAY_NON == setup_arg->low_delay)
            {
                temp_reg = BUFIN_LOW_DELAY_OFF;
            }
            else
            {
                temp_reg = BUFIN_LOW_DELAY_ON;  /* Set BUFIN bit */
                COMMON_Set32BitMaskLShift(  &temp_reg,
                                            &g_scux_bit_inf_tbl[SCUX_BFSSR_BIT_BUFDATA],
                                            (uint32_t)setup_arg->low_delay);
            }
            *(scux_reg->bfssr) = temp_reg;

            /* Configure SCUX
            WATSRp_2SRC0_m - 2SRC0_m Wait Time Setting Register p(m=0,1 p=0,1)
            b23:b0 WTIME - Wait Time - Set parameter
            Other  Reserved - The write value should always be 0 */
            if (SCUX_NO_WAIT != setup_arg->wait_time)
            {
                *(scux_reg->watsr) = (uint32_t)setup_arg->wait_time;
            }

            /* Configure SCUX
            SEVMRp_2SRC0_m - 2SRC0_m Event Mask Register p(m=0,1 p=0,1)
            b31 EVMUF - Sampling Rate Converter Underflow Mask - Interrupt enabled
            b30 EVMOF - Sampling Rate Converter Overflow Mask - Interrupt enabled
            b14 EVMWAIT - Sampling Rate Converter Wait Time Mask - Interrupt enabled
            Other  Reserved - The write value should always be 0 */
            *(scux_reg->sevmr) = SEVMR_VALUE;
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: SetupDvu
* Description  : Set registers related to DVU
* Arguments    : scux_reg -
*                    SCUX register address of target SCUX channel
*                setup_arg -
*                    Setup parameter for SCUX
*                config_dvu_arg -
*                    Setting parameter for DVU
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t SetupDvu (   const scux_reg_address_t * const scux_reg,
                            const scux_setup_arg_t * const setup_arg,
                            const scux_config_dvu_arg_t * const config_dvu_arg)
{
    int32_t     ret = SCUX_OK;
    uint32_t    temp_reg;

    if ((NULL == scux_reg) || (NULL == setup_arg) || (NULL == config_dvu_arg))
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* Configure SCUX
        DVUBR_DVU0_n - DVU0_n Bypass Register(n=0,1,2,3)
        b0    BYPASS - Bypass Mode - Set parameter
        Other Reserved - The write value should always be 0 */
        *(scux_reg->dvubr) = (uint32_t)setup_arg->dvu_bypass_mode;

        if (false == setup_arg->dvu_bypass_mode)
        {
            /* Configure SCUX
            VADIR_DVU0_n - DVU0_n Audio Information Register(n=0,1,2,3)
            b20:b16 OTBL - Bit Length of Output Audio Data - Set parameter
            b3:b0   CHNUM - Channel Number - Set parameter
            Other   Reserved - The write value should always be 0 */
            temp_reg = setup_arg->audio_channel;
            COMMON_Set32BitMaskLShift(  &temp_reg,
                                        &g_scux_bit_inf_tbl[SCUX_VADIR_BIT_OTBL],
                                        (uint32_t)setup_arg->bit_length);
            *(scux_reg->vadir) = temp_reg;

            /* Configure SCUX
            DVUCR_DVU0_n - DVU0_n Control Register(n=0,1,2,3)
            b8    VVMD - Select Digital Volume Value Mode - Set parameter
            b4    VRMD - Select Volume Ramp Mode - Set parameter
            b0    ZCMD - Select Zero Cross Mute Mode - Set parameter
            Other Reserved - The write value should always be 0 */
            temp_reg = (uint32_t)setup_arg->dvu_zc_mute_mode;
            if (0u != (uint32_t)setup_arg->dvu_ramp_mode)
            {
                COMMON_Set32BitMaskLShift(&temp_reg, &g_scux_bit_inf_tbl[SCUX_DVUCR_BIT_VRMD], 1);
            }
            else
            {
                /* Do Nothing */
            }
            COMMON_Set32BitMaskLShift(  &temp_reg,
                                        &g_scux_bit_inf_tbl[SCUX_DVUCR_BIT_VVMD],
                                        (uint32_t)setup_arg->dvu_vol_mode);
            *(scux_reg->dvucr) = temp_reg;

            /* Configure SCUX
            VRCTR_DVU0_n - DVU0_n Volume Ramp Control Register(n=0,1,2,3)
            b7:b0 VREN7:VREN0 - Volume Ramp Enable for Channel 7 to 0 - Set parameter
            Other Reserved - The write value should always be 0 */
            *(scux_reg->vrctr) = (uint32_t)setup_arg->dvu_ramp_mode;

            ret = ConfigDvu(scux_reg, config_dvu_arg);

            /* Configure SCUX
            VEVMR_DVU0_n - DVU0_n Event Mask Register(n=0,1,2,3)
            b31:b24 VEVMZCM7:VEVMZCM0 - Channel 7 to 0 Zero Cross Mute Status Change Mask - Interrupt enabled
            b15     VEVMVR - Volume Ramp Status Change - Interrupt enabled
            Other Reserved - The write value should always be 0 */
            *(scux_reg->vevmr) = VEVMR_VALUE;
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: SetupMix
* Description  : Set registers related to MIX
* Arguments    : scux_reg -
*                    SCUX register address of target SCUX channel
*                setup_arg -
*                    Setup parameter for SCUX
*                config_mix_arg -
*                    Setting parameter for MIX
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t SetupMix (const scux_setup_arg_t * const setup_arg, const scux_config_mix_arg_t * const config_mix_arg)
{
    int32_t     ret;
    uint32_t    temp_reg;

    if ((NULL == setup_arg) || (NULL == config_mix_arg))
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* Configure SCUX
        MIXBR_MIX0_0 - MIX0_0 Bypass Register
        b17:b16 BPSYS - Select Bypass System - Initial Value
        b0      BYPASS - Bypass Mode - MIX function is used
        Other Reserved - The write value should always be 0 */
        SCUX.MIXBR_MIX0_0 = MIXBR_VALUE;

        /* Configure SCUX
        MADIR_MIX0_0 - MIX0_0 Audio Information Register
        b3:b0 CHNUM - Channel Number - Set parameter
        Other Reserved - The write value should always be 0 */
        SCUX.MADIR_MIX0_0 = setup_arg->audio_channel;

        /* Configure SCUX
        MIXMR_MIX0_0 - MIX0_0 Mode Register
        b0    MIXMODE - MIX mode - Set parameter
        Other Reserved - The write value should always be 0 */
        SCUX.MIXMR_MIX0_0 = setup_arg->mix_mode;

        /* Configure SCUX
        MVPDR_MIX0_0 - MIX0_0 Volume Period Register
        b11:b8 MXPDUP - MIX Period for Volume Up - Set parameter
        b3:b0  MXPDDW - MIX Period for Volume Down - Set parameter
        Other  Reserved - The write value should always be 0 */
        temp_reg = setup_arg->mix_vol_down;
        COMMON_Set32BitMaskLShift(  &temp_reg,
                                    &g_scux_bit_inf_tbl[SCUX_MVPDR_BIT_MXPDUP],
                                    (uint32_t)setup_arg->mix_vol_up);
        SCUX.MVPDR_MIX0_0 = temp_reg;

        ret = ConfigMix(config_mix_arg);
    }

    return ret;
}

/******************************************************************************
* Function Name: ClearInit
* Description  : Clear initialization of SCUX
* Arguments    : scux_reg -
*                    SCUX register address of target SCUX channel
*                ch -
*                    SCUX channel
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
*                SCUX_E_BUSY -
*                    SCUX is busy.
*                Other value -
*                    Callback function error.
******************************************************************************/
static int32_t ClearInit (const scux_reg_address_t * const scux_reg, const uint32_t ch)
{
    int32_t ret = SCUX_OK;

    if (NULL == scux_reg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        if (SELECT_FIFO == pass_inf[ch].io.input)
        {
            /* FFD is used */
            /* ==== Start DMAC ==== */
            if (0 != scux_callback_func.set_dmac_callback)
            {
                ret = scux_callback_func.set_dmac_callback(ch, FFD_DMA_START);
            }
            else
            {
                ret = SCUX_E_BUSY;
            }
            COMMON_Set32BitMaskLShift(&SCUX.DMACR_CIM, &g_dmacr_dmamdffd_bit_inf_tbl[ch], 1);

            /* ==== Clear initialization of FFD ==== */
            *(scux_reg->ffdir) = INIT_OFF;      /* Clear INIT of FFD */
            *(scux_reg->ffdbr) = BOOT_ON;       /* Boot the data request */
        }

        if (SELECT_FIFO == pass_inf[ch].io.output)
        {
            /* FFU is used */
            /* ==== Start DMAC ==== */
            if (0 != scux_callback_func.set_dmac_callback)
            {
                ret = scux_callback_func.set_dmac_callback(ch, FFU_DMA_START);
            }
            else
            {
                ret = SCUX_E_BUSY;
            }
            COMMON_Set32BitMaskLShift(&SCUX.DMACR_CIM, &g_dmacr_dmamdffu_bit_inf_tbl[ch], 1);

            /* ==== Clear initialization of FFU ==== */
            *(scux_reg->ffuir) = INIT_OFF;      /* Clear INIT of FFU */
        }
        else {
            /* DVU is used */
            /* ==== Clear initialization of DVU ==== */
            *(scux_reg->dvuir) = INIT_OFF;      /* Clear INIT of DVU */

            if (SELECT_MIX == pass_inf[ch].io.output)
            {
                /* MIX is used */
                /* ==== Clear initialization of MIX ==== */
                SCUX.MIXIR_MIX0_0 = INIT_OFF;       /* Clear INIT of MIX */
            }
        }

        if (ASYNC_SRC == pass_inf[ch].src_mode)
        {
            /* Async SRC is used */
            /* ==== Clear initialization of SRC ==== */
            *(scux_reg->srcir)  = INIT_OFF;     /* Clear INIT of SRC */
            *(scux_reg->srcirr) = INIT_OFF;     /* Clear INIT of COEF-ROMIF */

            /* ==== Clear initialization of IPC/OPC ==== */
            *(scux_reg->ipcir) = INIT_OFF;      /* Clear INIT of IPC */
            *(scux_reg->opcir) = INIT_OFF;      /* Clear INIT of OPC */
        }
        else {
            /* Sync SRC is used */
            /* ==== Clear initialization of IPC/OPC ==== */
            *(scux_reg->ipcir) = INIT_OFF;      /* Clear INIT of IPC */
            *(scux_reg->opcir) = INIT_OFF;      /* Clear INIT of OPC */

            /* ==== Clear initialization of SRC ==== */
            *(scux_reg->srcir)  = INIT_OFF;     /* Clear INIT of SRC */
            *(scux_reg->srcirr) = INIT_OFF;     /* Clear INIT of COEF-ROMIF */
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: SetInit
* Description  : Set initialization of SCUX
* Arguments    : scux_reg -
*                    SCUX register address of target SCUX channel
*                ch -
*                    SCUX channel
*                mix_stop -
*                    Stop MIX module
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t SetInit (const scux_reg_address_t * const scux_reg, const uint32_t ch, const bool_t mix_stop)
{
    int32_t ret = SCUX_OK;

    if (NULL == scux_reg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        if (SELECT_FIFO != pass_inf[ch].io.output)
        {
            /* DVU is used */
            /* ==== Set initialization of DVU ==== */
            *(scux_reg->dvuir) = INIT_ON;   /* Set INIT of DVU */

            if (SELECT_MIX == pass_inf[ch].io.output)
            {
                if (true == mix_stop)
                {
                    /* MIX is used */
                    /* ==== Set initialization of MIX ==== */
                    SCUX.MIXIR_MIX0_0 = INIT_ON;    /* Set INIT of MIX */
                }
            }
        }

        /* ==== Set initialization of SRC ==== */
        *(scux_reg->srcir)  = INIT_ON;  /* Set INIT of SRC */
        *(scux_reg->srcirr) = INIT_ON;  /* Set INIT of COEF-ROMIF */
    }

    return ret;
}

/******************************************************************************
* Function Name: ConfigDvu
* Description  : Change the DVU parameters
* Arguments    : scux_reg -
*                    SCUX register address of target SCUX channel
*                config_dvu_arg -
*                    Setting parameter for DVU
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t ConfigDvu (const scux_reg_address_t * const scux_reg, const scux_config_dvu_arg_t * const config_dvu_arg)
{
    int32_t ret = SCUX_OK;

    if ((NULL == scux_reg) || (NULL == config_dvu_arg))
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* Set ZCMCR register - ZCEN bit */
        COMMON_Set32BitMaskLShift(scux_reg->zcmcr, &g_scux_bit_inf_tbl[SCUX_ZCMCR_BIT_ZCEN],
                        (uint32_t)config_dvu_arg->zc_mute);
        /* Set VRPDR register - VRPDUP bit */
        COMMON_Set32BitMaskLShift(scux_reg->vrpdr, &g_scux_bit_inf_tbl[SCUX_VRPDR_BIT_VRPDUP],
                        (uint32_t)config_dvu_arg->ramp_vol_up);
        /* Set VRPDR register - VRPDDW bit */
        COMMON_Set32BitMaskLShift(scux_reg->vrpdr, &g_scux_bit_inf_tbl[SCUX_VRPDR_BIT_VRPDDW],
                        (uint32_t)config_dvu_arg->ramp_vol_down);
        /* Set VRDBR register - VRDB bit */
        COMMON_Set32BitMaskLShift(scux_reg->vrdbr, &g_scux_bit_inf_tbl[SCUX_VRDBR_BIT_VRDB],
                        config_dvu_arg->ramp_gain);
        /* Set VRWTR register - VRWT bit */
        COMMON_Set32BitMaskLShift(scux_reg->vrwtr, &g_scux_bit_inf_tbl[SCUX_VRWTR_BIT_VRWT],
                        config_dvu_arg->ramp_wait_time);
        /* Set VOL0R register - VOLVAL0 bit */
        COMMON_Set32BitMaskLShift(scux_reg->vol0r, &g_scux_bit_inf_tbl[SCUX_VOLR_BIT_VOLVAL],
                        config_dvu_arg->vol_value[AUDIO_CH_0]);
        /* Set VOL1R register - VOLVAL1 bit */
        COMMON_Set32BitMaskLShift(scux_reg->vol1r, &g_scux_bit_inf_tbl[SCUX_VOLR_BIT_VOLVAL],
                        config_dvu_arg->vol_value[AUDIO_CH_1]);
        /* Set VOL2R register - VOLVAL2 bit */
        COMMON_Set32BitMaskLShift(scux_reg->vol2r, &g_scux_bit_inf_tbl[SCUX_VOLR_BIT_VOLVAL],
                        config_dvu_arg->vol_value[AUDIO_CH_2]);
        /* Set VOL3R register - VOLVAL3 bit */
        COMMON_Set32BitMaskLShift(scux_reg->vol3r, &g_scux_bit_inf_tbl[SCUX_VOLR_BIT_VOLVAL],
                        config_dvu_arg->vol_value[AUDIO_CH_3]);
        /* Set VOL4R register - VOLVAL4 bit */
        COMMON_Set32BitMaskLShift(scux_reg->vol4r, &g_scux_bit_inf_tbl[SCUX_VOLR_BIT_VOLVAL],
                        config_dvu_arg->vol_value[AUDIO_CH_4]);
        /* Set VOL5R register - VOLVAL5 bit */
        COMMON_Set32BitMaskLShift(scux_reg->vol5r, &g_scux_bit_inf_tbl[SCUX_VOLR_BIT_VOLVAL],
                        config_dvu_arg->vol_value[AUDIO_CH_5]);
        /* Set VOL6R register - VOLVAL6 bit */
        COMMON_Set32BitMaskLShift(scux_reg->vol6r, &g_scux_bit_inf_tbl[SCUX_VOLR_BIT_VOLVAL],
                        config_dvu_arg->vol_value[AUDIO_CH_6]);
        /* Set VOL7R register - VOLVAL7 bit */
        COMMON_Set32BitMaskLShift(scux_reg->vol7r, &g_scux_bit_inf_tbl[SCUX_VOLR_BIT_VOLVAL],
                        config_dvu_arg->vol_value[AUDIO_CH_7]);
    }

    return ret;
}

/******************************************************************************
* Function Name: ConfigMix
* Description  : Change the MIX parameters
* Arguments    : config_mix_arg -
*                    Setting parameter for MIX
* Return Value : SCUX_OK -
*                    Parameter OK.
*                SCUX_E_PAR -
*                    Parameter Error.
******************************************************************************/
static int32_t ConfigMix (const scux_config_mix_arg_t * const config_mix_arg)
{
    int32_t ret = SCUX_OK;

    if (NULL == config_mix_arg)
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        /* Set MDBAR register - MIXDBA bit */
        COMMON_Set32BitMaskLShift(&SCUX.MDBAR_MIX0_0, &g_scux_bit_inf_tbl[SCUX_MDBR_BIT_MIXDB],
                        config_mix_arg->mix_gain[SCUX_CH_0]);
        /* Set MDBBR register - MIXDBB bit */
        COMMON_Set32BitMaskLShift(&SCUX.MDBBR_MIX0_0, &g_scux_bit_inf_tbl[SCUX_MDBR_BIT_MIXDB],
                        config_mix_arg->mix_gain[SCUX_CH_1]);
        /* Set MDBCR register - MIXDBC bit */
        COMMON_Set32BitMaskLShift(&SCUX.MDBCR_MIX0_0, &g_scux_bit_inf_tbl[SCUX_MDBR_BIT_MIXDB],
                        config_mix_arg->mix_gain[SCUX_CH_2]);
        /* Set MDBDR register - MIXDBD bit */
        COMMON_Set32BitMaskLShift(&SCUX.MDBDR_MIX0_0, &g_scux_bit_inf_tbl[SCUX_MDBR_BIT_MIXDB],
                        config_mix_arg->mix_gain[SCUX_CH_3]);
    }

    return ret;
}

/******************************************************************************
* Function Name: MultiplyIntAndFixedPoint
* Description  : Multiply 32bit integer and 32bit fixed-point_decimal.
*                The operation result converts it into an integer and returns.
* Arguments    : integer -
*                    Integer data
*                fixed_point_decimal -
*                    Fixed-point decimal
*                decimal_digits -
*                    Decimal digits of fixed point decival value
* Return Value : Result of multiplication(integer).
******************************************************************************/
static uint32_t MultiplyIntAndFixedPoint(   const uint32_t integer,
                                            const uint32_t fixed_point_decimal,
                                            const uint32_t decimal_digits )
{
    uint64_t temp_uint64;

    temp_uint64 = (uint64_t)integer;
    temp_uint64 *= fixed_point_decimal;
    temp_uint64 >>= decimal_digits;     /* Convert a decimal into an integer. */

    return (uint32_t)temp_uint64;
}

/* End of File */
