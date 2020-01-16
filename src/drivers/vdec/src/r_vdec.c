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
/**************************************************************************//**
* @file         r_vdec.c
* @version      0.06
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* @brief        VDEC driver API functions
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    "r_vdec.h"
#include    "r_vdec_user.h"
#include    "r_vdec_register.h"
#include    "r_vdec_check_parameter.h"


/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

/**************************************************************************//**
 * @brief       VDEC driver initialization
 *
 *              Description:<br>
 *              In this function, the operations are performed as below.
 *              - The specified user-defined function is executed.
 *              - The VDEC driver internal variables are initialized.
 * @param[in]   ch                      : Channel
 * @param[in]   vinsel                  : CVBS input pin
 * @param[in]   init_func               : Pointer to the user-defined function
 * @param[in]   user_num                : User-defined number
 * @retval      Error code
 *****************************************************************************/
vdec_error_t R_VDEC_Initialize (
    const vdec_channel_t    ch,
    const vdec_adc_vinsel_t vinsel,
    void           (* const init_func)(uint32_t),
    const uint32_t          user_num)
{
    vdec_error_t vdec_error;

    vdec_error = VDEC_OK;
#ifdef      R_VDEC_CHECK_PARAMETERS
    if ((ch != VDEC_CHANNEL_0) && (ch != VDEC_CHANNEL_1))
    {
        vdec_error = VDEC_ERR_CHANNEL;      /* Channel error */
    }
    if (vdec_error == VDEC_OK)
    {
        vdec_error = VDEC_InitializeCheckParam(vinsel);
    }
#endif      /* R_VDEC_CHECK_PARAMETERS */
    if (vdec_error == VDEC_OK)
    {
         /* User-defined function */
        if (init_func != 0)
        {
            init_func(user_num);
        }
        VDEC_Initialize(ch, vinsel);
    }
    return vdec_error;
}   /* End of function R_VDEC_Initialize() */

/**************************************************************************//**
 * @brief       VDEC driver termination
 *
 *              Description:<br>
 *              In this function, the operations are performed as below.
 *              - The specified user-defined function is executed.
 * @param[in]   ch                      : Channel
 * @param[in]   quit_func               : Pointer to the user-defined function
 * @param[in]   user_num                : User-defined number
 * @retval      Error code
 *****************************************************************************/
vdec_error_t R_VDEC_Terminate (const vdec_channel_t ch, void (* const quit_func)(uint32_t), const uint32_t user_num)
{
    vdec_error_t vdec_error;

    vdec_error = VDEC_OK;
#ifdef      R_VDEC_CHECK_PARAMETERS
    if ((ch != VDEC_CHANNEL_0) && (ch != VDEC_CHANNEL_1))
    {
        vdec_error = VDEC_ERR_CHANNEL;      /* Channel error */
    }
#endif      /* R_VDEC_CHECK_PARAMETERS */
    if (vdec_error == VDEC_OK)
    {
         /* User-defined function */
        if (quit_func != 0)
        {
            quit_func(user_num);
        }
    }
    return vdec_error;
}   /* End of function R_VDEC_Terminate() */

/**************************************************************************//**
 * @brief       Active image period
 * @param[in]   ch                      : Channel
 * @param[in]   param                   : Active image period parameters
 * @retval      Error code
 *****************************************************************************/
vdec_error_t R_VDEC_ActivePeriod (const vdec_channel_t ch, const vdec_active_period_t * const param)
{
    vdec_error_t vdec_error;

    vdec_error = VDEC_OK;
#ifdef      R_VDEC_CHECK_PARAMETERS
    if ((ch != VDEC_CHANNEL_0) && (ch != VDEC_CHANNEL_1))
    {
        vdec_error = VDEC_ERR_CHANNEL;      /* Channel error */
    }
    if (vdec_error == VDEC_OK)
    {
        vdec_error = VDEC_ActivePeriodCheckParam(param);
    }
#endif      /* R_VDEC_CHECK_PARAMETERS */
    if (vdec_error == VDEC_OK)
    {
        VDEC_ActivePeriod(ch, param);
    }
    return vdec_error;
}   /* End of function R_VDEC_ActivePeriod() */

/**************************************************************************//**
 * @brief       Sync separation
 * @param[in]   ch                      : Channel
 * @param[in]   param                   : Sync separation parameters
 * @retval      Error code
 *****************************************************************************/
vdec_error_t R_VDEC_SyncSeparation (const vdec_channel_t ch, const vdec_sync_separation_t * const param)
{
    vdec_error_t vdec_error;

    vdec_error = VDEC_OK;
#ifdef      R_VDEC_CHECK_PARAMETERS
    if ((ch != VDEC_CHANNEL_0) && (ch != VDEC_CHANNEL_1))
    {
        vdec_error = VDEC_ERR_CHANNEL;      /* Channel error */
    }
    if (vdec_error == VDEC_OK)
    {
        vdec_error = VDEC_SyncSeparationCheckParam(param);
    }
#endif      /* R_VDEC_CHECK_PARAMETERS */
    if (vdec_error == VDEC_OK)
    {
        VDEC_SyncSeparation(ch, param);
    }
    return vdec_error;
}   /* End of function R_VDEC_SyncSeparation() */

/**************************************************************************//**
 * @brief       Y/C separation
 * @param[in]   ch                      : Channel
 * @param[in]   param                   : Y/C separation parameters
 * @retval      Error code
 *****************************************************************************/
vdec_error_t R_VDEC_YcSeparation (const vdec_channel_t ch, const vdec_yc_separation_t * const param)
{
    vdec_error_t vdec_error;

    vdec_error = VDEC_OK;
#ifdef      R_VDEC_CHECK_PARAMETERS
    if ((ch != VDEC_CHANNEL_0) && (ch != VDEC_CHANNEL_1))
    {
        vdec_error = VDEC_ERR_CHANNEL;      /* Channel error */
    }
    if (vdec_error == VDEC_OK)
    {
        vdec_error = VDEC_YcSeparationCheckParam(param);
    }
#endif      /* R_VDEC_CHECK_PARAMETERS */
    if (vdec_error == VDEC_OK)
    {
        VDEC_YcSeparation(ch, param);
    }
    return vdec_error;
}   /* End of function R_VDEC_YcSeparation() */

/**************************************************************************//**
 * @brief       Chroma decoding
 * @param[in]   ch                      : Channel
 * @param[in]   param                   : Chroma decoding parameters
 * @retval      Error code
 *****************************************************************************/
vdec_error_t R_VDEC_ChromaDecoding (const vdec_channel_t ch, const vdec_chroma_decoding_t * const param)
{
    vdec_error_t vdec_error;

    vdec_error = VDEC_OK;
#ifdef      R_VDEC_CHECK_PARAMETERS
    if ((ch != VDEC_CHANNEL_0) && (ch != VDEC_CHANNEL_1))
    {
        vdec_error = VDEC_ERR_CHANNEL;      /* Channel error */
    }
    if (vdec_error == VDEC_OK)
    {
        vdec_error = VDEC_ChromaDecodingCheckParam(param);
    }
#endif      /* R_VDEC_CHECK_PARAMETERS */
    if (vdec_error == VDEC_OK)
    {
        VDEC_ChromaDecoding(ch, param);
    }
    return vdec_error;
}   /* End of function R_VDEC_ChromaDecoding() */

/**************************************************************************//**
 * @brief       Digital clamp
 * @param[in]   ch                      : Channel
 * @param[in]   param                   : Digital clamp parameters
 * @retval      Error code
 *****************************************************************************/
vdec_error_t R_VDEC_DigitalClamp (const vdec_channel_t ch, const vdec_degital_clamp_t * const param)
{
    vdec_error_t vdec_error;

    vdec_error = VDEC_OK;
#ifdef      R_VDEC_CHECK_PARAMETERS
    if ((ch != VDEC_CHANNEL_0) && (ch != VDEC_CHANNEL_1))
    {
        vdec_error = VDEC_ERR_CHANNEL;      /* Channel error */
    }
    if (vdec_error == VDEC_OK)
    {
        vdec_error = VDEC_DigitalClampCheckParam(param);
    }
#endif      /* R_VDEC_CHECK_PARAMETERS */
    if (vdec_error == VDEC_OK)
    {
        VDEC_DigitalClamp(ch, param);
    }
    return vdec_error;
}   /* End of function R_VDEC_DigitalClamp() */

/**************************************************************************//**
 * @brief       Output adjustment
 * @param[in]   ch                      : Channel
 * @param[in]   param                   : Output adjustment parameters
 * @retval      Error code
 *****************************************************************************/
vdec_error_t R_VDEC_Output (const vdec_channel_t ch, const vdec_output_t * const param)
{
    vdec_error_t vdec_error;

    vdec_error = VDEC_OK;
#ifdef      R_VDEC_CHECK_PARAMETERS
    if ((ch != VDEC_CHANNEL_0) && (ch != VDEC_CHANNEL_1))
    {
        vdec_error = VDEC_ERR_CHANNEL;      /* Channel error */
    }
    if (vdec_error == VDEC_OK)
    {
        vdec_error = VDEC_OutputCheckParam(param);
    }
#endif      /* R_VDEC_CHECK_PARAMETERS */
    if (vdec_error == VDEC_OK)
    {
        VDEC_Output(ch, param);
    }
    return vdec_error;
}   /* End of function R_VDEC_Output() */

/**************************************************************************//**
 * @brief       Query
 * @param[in]   ch                      : Channel
 * @param[out]  q_sync_sep              : Sync separation parameters
 * @param[out]  q_agc                   : Agc parameters
 * @param[out]  q_chroma_dec            : Chroma decoding parameters
 * @param[out]  q_digital_clamp         : Digital clamp parameters
 * @retval      Error code
 *****************************************************************************/
vdec_error_t R_VDEC_Query (
    const vdec_channel_t            ch,
    vdec_q_sync_sep_t       * const q_sync_sep,
    vdec_q_agc_t            * const q_agc,
    vdec_q_chroma_dec_t     * const q_chroma_dec,
    vdec_q_digital_clamp_t  * const q_digital_clamp)
{
    vdec_error_t vdec_error;

    vdec_error = VDEC_OK;
#ifdef      R_VDEC_CHECK_PARAMETERS
    if ((ch != VDEC_CHANNEL_0) && (ch != VDEC_CHANNEL_1))
    {
        vdec_error = VDEC_ERR_CHANNEL;      /* Channel error */
    }
#endif      /* R_VDEC_CHECK_PARAMETERS */
    if (vdec_error == VDEC_OK)
    {
        VDEC_Query(ch, q_sync_sep, q_agc, q_chroma_dec, q_digital_clamp);
    }
    return vdec_error;
}   /* End of function R_VDEC_Query() */

