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
* @file         r_vdc5.c
* @version      0.11
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* @brief        VDC5 driver API functions
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    "r_vdc5.h"
#include    "r_vdc5_user.h"
#include    "r_vdc5_register.h"
#include    "r_vdc5_shared_param.h"
#include    "r_vdc5_check_parameter.h"


/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static vdc5_scaling_type_t ConvertRwProcId2SclId(const vdc5_layer_id_t layer_id);
static vdc5_graphics_type_t ConvertRwProcId2GrId(const vdc5_layer_id_t layer_id);


/**************************************************************************//**
 * @brief       VDC5 driver initialization
 *
 *              Description:<br>
 *              In this function, the operations are performed as below.
 *              - The specified user-defined function is executed.
 *              - The VDC5 driver internal variables are initialized.
 *              - The operation of the panel clock is enabled.
 * @param[in]   ch                      : Channel
 * @param[in]   param                   : Initialization parameter
 * @param[in]   init_func               : Pointer to the user-defined function
 * @param[in]   user_num                : User-defined number
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_Initialize (
    const vdc5_channel_t        ch,
    const vdc5_init_t   * const param,
    void               (* const init_func)(uint32_t),
    const uint32_t              user_num)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_InitializeCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */

    printf("VDC5 ch %d initialized\n",ch);

    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmInit(ch);
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetResource(ch, VDC5_RESOURCE_LVDS_CLK);
        if (rsrc_state != VDC5_RESOURCE_ST_INVALID)
        {   /* The LVDS PLL clock has been already set. */
            if (param->lvds != NULL)
            {
                ret = VDC5_ERR_RESOURCE_LVDS_CLK;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetInitParam(ch, param);

        /* Callback function */
        if (init_func != 0)
        {
            init_func(user_num);
        }

        VDC5_Initialize(ch, param);

        /* Disable all VDC5 interrupts */
        VDC5_Int_Disable(ch);

        VDC5_ShrdPrmSetResource(ch, VDC5_RESOURCE_PANEL_CLK, VDC5_RESOURCE_ST_VALID);
        if (param->lvds != NULL)
        {
            VDC5_ShrdPrmSetResource(ch, VDC5_RESOURCE_LVDS_CLK, VDC5_RESOURCE_ST_VALID);
        }
    }
    return ret;
}   /* End of function R_VDC5_Initialize() */

/**************************************************************************//**
 * @brief       VDC5 driver termination
 *
 * @param[in]   ch                      : Channel
 * @param[in]   quit_func               : Pointer to the user-defined function
 * @param[in]   user_num                : User-defined number
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_Terminate (const vdc5_channel_t ch, void (* const quit_func)(uint32_t), const uint32_t user_num)
{
    vdc5_error_t    ret;
    vdc5_onoff_t    lvds_ref;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_TerminateCheckPrm(ch);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetTerminate(ch);

        /* Disable all VDC5 interrupts */
        VDC5_Int_Disable(ch);

        VDC5_Terminate(ch);

        VDC5_ShrdPrmSetResource(ch, VDC5_RESOURCE_PANEL_CLK, VDC5_RESOURCE_ST_INVALID);

        lvds_ref = VDC5_ShrdPrmGetLvdsClkRef();
        if (lvds_ref == VDC5_OFF)       /* LVDS PLL clock is not referred. */
        {
            VDC5_ShrdPrmSetResource(ch, VDC5_RESOURCE_LVDS_CLK, VDC5_RESOURCE_ST_INVALID);
        }

        /* Callback function */
        if (quit_func != 0)
        {
            quit_func(user_num);
        }
    }
    return ret;
}   /* End of function R_VDC5_Terminate() */

/**************************************************************************//**
 * @brief       VDC5 driver input
 *
 * @param[in]   ch                      : Channel
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_VideoInput (const vdc5_channel_t ch, const vdc5_input_t * const param)
{
    vdc5_error_t ret;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_VideoInputCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetInputParam(ch, param);

        /* Setting VDC5 registers */
        VDC5_VideoInput(ch, param);

        VDC5_ShrdPrmSetResource(ch, VDC5_RESOURCE_VIDEO_IN, VDC5_RESOURCE_ST_VALID);
    }
    return ret;
}   /* End of function R_VDC5_VideoInput() */

/**************************************************************************//**
 * @brief       VDC5 driver sync control
 *
 * @param[in]   ch                      : Channel
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_SyncControl (const vdc5_channel_t ch, const vdc5_sync_ctrl_t * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_SyncControlCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetResource(ch, VDC5_RESOURCE_PANEL_CLK);
        if (rsrc_state != VDC5_RESOURCE_ST_VALID)
        {
            ret = VDC5_ERR_RESOURCE_CLK;
        }
    }
    if (ret == VDC5_OK)
    {
        if (param->res_vs_sel == VDC5_OFF)
        {   /* External input Vsync signal is selected. */
            if (param->res_vs_in_sel == VDC5_RES_VS_IN_SEL_SC0)
            {   /* Scaler 0 */
                rsrc_state = VDC5_ShrdPrmGetResource(ch, VDC5_RESOURCE_VIDEO_IN);
            }
            else
            {   /* Scaler 1 */
                rsrc_state = VDC5_ShrdPrmGetResource(
                                ((ch == VDC5_CHANNEL_0) ? VDC5_CHANNEL_1 : VDC5_CHANNEL_0),
                                VDC5_RESOURCE_VIDEO_IN);
            }
            if (rsrc_state != VDC5_RESOURCE_ST_VALID)
            {
                ret = VDC5_ERR_RESOURCE_INPUT;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetSyncParam(ch, param);

        /* Setting VDC5 registers */
        VDC5_SyncControl(ch, param);

        VDC5_ShrdPrmSetResource(ch, VDC5_RESOURCE_VSYNC, VDC5_RESOURCE_ST_VALID);
    }
    return ret;
}   /* End of function R_VDC5_SyncControl() */

/**************************************************************************//**
 * @brief       VDC5 driver output
 *
 * @param[in]   ch                      : Channel
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_DisplayOutput (const vdc5_channel_t ch, const vdc5_output_t * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_DisplayOutputCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetResource(ch, VDC5_RESOURCE_PANEL_CLK);
        if (rsrc_state != VDC5_RESOURCE_ST_VALID)
        {
            ret = VDC5_ERR_RESOURCE_CLK;
        }
        else
        {
            rsrc_state = VDC5_ShrdPrmGetResource(ch, VDC5_RESOURCE_VSYNC);
            if (rsrc_state != VDC5_RESOURCE_ST_VALID)
            {
                ret = VDC5_ERR_RESOURCE_VSYNC;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetOutputParam(ch, param);

        /* Setting VDC5 registers */
        VDC5_DisplayOutput(ch, param);

        VDC5_ShrdPrmSetResource(ch, VDC5_RESOURCE_LCD_PANEL, VDC5_RESOURCE_ST_VALID);
    }
    return ret;
}   /* End of function R_VDC5_DisplayOutput() */

/**************************************************************************//**
 * @brief       VDC5 driver set interrupt callback function
 *
 * @param[in]   ch                      : Channel
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_CallbackISR (const vdc5_channel_t ch, const vdc5_int_t * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_CallbackISRCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetResource(ch, VDC5_RESOURCE_PANEL_CLK);
        if (rsrc_state != VDC5_RESOURCE_ST_VALID)
        {
            ret = VDC5_ERR_RESOURCE_CLK;
        }
        else
        {
            rsrc_state = VDC5_ShrdPrmGetResource(ch, VDC5_RESOURCE_VSYNC);
            if (rsrc_state != VDC5_RESOURCE_ST_VALID)
            {
                ret = VDC5_ERR_RESOURCE_VSYNC;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_Int_SetInterrupt(ch, param);
    }
    return ret;
}   /* End of function R_VDC5_CallbackISR() */

/**************************************************************************//**
 * @brief       VDC5 driver set writing data control
 *
 * @param[in]   ch                      : Channel
 * @param[in]   scaling_id
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_WriteDataControl (
    const vdc5_channel_t        ch,
    const vdc5_layer_id_t       layer_id,
    const vdc5_write_t  * const param)
{
    vdc5_error_t            ret;
    vdc5_scaling_type_t     scaling_id;
    vdc5_resource_state_t   rsrc_state;

    scaling_id = ConvertRwProcId2SclId(layer_id);

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_WriteDataControlCheckPrm(ch, layer_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetLayerResource(ch, layer_id);
        if (rsrc_state != VDC5_RESOURCE_ST_INVALID)
        {   /* The specified layer is already used. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
        else
        {
            if (layer_id == VDC5_LAYER_ID_0_WR)
            {   /* Layer 0 */
                rsrc_state = VDC5_ShrdPrmGetResource(ch, VDC5_RESOURCE_VIDEO_IN);
            }
            else if (layer_id == VDC5_LAYER_ID_1_WR)
            {   /* Layer 1 */
                rsrc_state = VDC5_ShrdPrmGetResource(
                                ((ch == VDC5_CHANNEL_0) ? VDC5_CHANNEL_1 : VDC5_CHANNEL_0),
                                VDC5_RESOURCE_VIDEO_IN);
            }
            else
            {   /* Layer OIR */
                rsrc_state = VDC5_RESOURCE_ST_VALID;
            }
            if (rsrc_state != VDC5_RESOURCE_ST_VALID)
            {
                ret = VDC5_ERR_RESOURCE_INPUT;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetWriteParam(ch, scaling_id, param);

        /* Setting VDC5 registers */
        VDC5_WriteDataControl(ch, scaling_id, param);

        VDC5_ShrdPrmSetLayerResource(ch, layer_id, VDC5_RESOURCE_ST_VALID);
    }
    return ret;
}   /* End of function R_VDC5_WriteDataControl() */

/**************************************************************************//**
 * @brief       VDC5 driver change writing data control
 *
 * @param[in]   ch                      : Channel
 * @param[in]   scaling_id
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_ChangeWriteProcess (
    const vdc5_channel_t            ch,
    const vdc5_layer_id_t           layer_id,
    const vdc5_write_chg_t   * const param)
{
    vdc5_error_t            ret;
    vdc5_scaling_type_t     scaling_id;
    vdc5_resource_state_t   rsrc_state;

    scaling_id = ConvertRwProcId2SclId(layer_id);

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ChangeWriteProcessCheckPrm(ch, layer_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetRwProcEnabled(ch, layer_id);
        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {   /* The specified layer is invalid. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
    }
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetChgWriteParam(ch, scaling_id, param);

        /* Setting VDC5 registers */
        VDC5_ChangeWriteProcess(ch, scaling_id, param);
    }
    return ret;
}   /* End of function R_VDC5_ChangeWriteProcess() */

/**************************************************************************//**
 * @brief       VDC5 driver set reading data control
 *
 * @param[in]   ch                      : Channel
 * @param[in]   layer_id
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_ReadDataControl (
    const vdc5_channel_t        ch,
    const vdc5_layer_id_t       layer_id,
    const vdc5_read_t   * const param)
{
    vdc5_error_t            ret;
    vdc5_graphics_type_t    graphics_id;
    vdc5_resource_state_t   rsrc_state;

    graphics_id = ConvertRwProcId2GrId(layer_id);

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ReadDataControlCheckPrm(ch, layer_id, graphics_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetLayerResource(ch, layer_id);
        if (rsrc_state != VDC5_RESOURCE_ST_INVALID)
        {   /* The specified layer is already used. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
    }
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetReadParam(ch, graphics_id, param);

        /* Setting VDC5 registers */
        VDC5_ReadDataControl(ch, graphics_id, param);

        VDC5_ShrdPrmSetLayerResource(ch, layer_id, VDC5_RESOURCE_ST_VALID);
    }
    return ret;
}   /* End of function R_VDC5_ReadDataControl() */

/**************************************************************************//**
 * @brief       VDC5 driver change reading data control
 *
 * @param[in]   ch                      : Channel
 * @param[in]   layer_id
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_ChangeReadProcess (
    const vdc5_channel_t            ch,
    const vdc5_layer_id_t           layer_id,
    const vdc5_read_chg_t   * const param)
{
    vdc5_error_t            ret;
    vdc5_graphics_type_t    graphics_id;
    vdc5_resource_state_t   rsrc_state;

    graphics_id = ConvertRwProcId2GrId(layer_id);

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ChangeReadProcessCheckPrm(ch, layer_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetRwProcEnabled(ch, layer_id);
        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {   /* The specified layer is invalid. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
    }
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetChgReadParam(ch, graphics_id, param);

        /* Setting VDC5 registers */
        VDC5_ChangeReadProcess(ch, graphics_id, param);
    }
    return ret;
}   /* End of function R_VDC5_ChangeReadProcess() */

/**************************************************************************//**
 * @brief       VDC5 driver start process
 *
 * @param[in]   ch                      : Channel
 * @param[in]   layer_id
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_StartProcess (
    const vdc5_channel_t        ch,
    const vdc5_layer_id_t       layer_id,
    const vdc5_start_t  * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_StartProcessCheckPrm(ch, layer_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        if (layer_id != VDC5_LAYER_ID_ALL)
        {
            rsrc_state = VDC5_ShrdPrmGetRwProcReady(ch, layer_id);
            if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
            {   /* The specified layer is invalid. */
                ret = VDC5_ERR_RESOURCE_LAYER;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_StartProcess(ch, layer_id, param);
    }
    return ret;
}   /* End of function R_VDC5_StartProcess() */

/**************************************************************************//**
 * @brief       VDC5 driver stop process
 *
 * @param[in]   ch                      : Channel
 * @param[in]   layer_id
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_StopProcess (const vdc5_channel_t ch, const vdc5_layer_id_t layer_id)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_StopProcessCheckPrm(ch, layer_id);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        if (layer_id != VDC5_LAYER_ID_ALL)
        {
            rsrc_state = VDC5_ShrdPrmGetRwProcEnabled(ch, layer_id);
            if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
            {   /* The specified layer is invalid. */
                ret = VDC5_ERR_RESOURCE_LAYER;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_StopProcess(ch, layer_id);
    }
    return ret;
}   /* End of function R_VDC5_StopSurface() */

/**************************************************************************//**
 * @brief       
 *
 * @param[in]   ch                      : Channel
 * @param[in]   layer_id
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_ReleaseDataControl (const vdc5_channel_t ch, const vdc5_layer_id_t layer_id)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ReleaseDataControlCheckPrm(ch, layer_id);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        if (layer_id != VDC5_LAYER_ID_ALL)
        {
            rsrc_state = VDC5_ShrdPrmGetRwProcReady(ch, layer_id);
            if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
            {   /* The specified layer is invalid. */
                ret = VDC5_ERR_RESOURCE_LAYER;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_ReleaseDataControl(ch, layer_id);

        VDC5_ShrdPrmSetLayerResource(ch, layer_id, VDC5_RESOURCE_ST_INVALID);
    }
    return ret;
}   /* End of function R_VDC5_ReleaseDataControl() */

/**************************************************************************//**
 * @brief       VDC5 driver noise reduction
 *
 * @param[in]   ch                      : Channel
 * @param[in]   nr1d_on                 : Noise reduction ON/OFF control
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_VideoNoiseReduction (
    const vdc5_channel_t                    ch,
    const vdc5_onoff_t                      nr1d_on,
    const vdc5_noise_reduction_t    * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_VideoNoiseReductCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetResource(ch, VDC5_RESOURCE_VIDEO_IN);
        if (rsrc_state != VDC5_RESOURCE_ST_VALID)
        {
            ret = VDC5_ERR_RESOURCE_INPUT;
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_VideoNoiseReduction(ch, nr1d_on, param);
    }
    return ret;
}   /* End of function R_VDC5_VideoNoiseReduction() */

/**************************************************************************//**
 * @brief       VDC5 driver color matrix
 *
 * @param[in]   ch                      : Channel
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_ImageColorMatrix (const vdc5_channel_t ch, const vdc5_color_matrix_t * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ImageColorMatrixCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        if (param->module == VDC5_COLORMTX_IMGCNT)
        {
            rsrc_state = VDC5_ShrdPrmGetLayerResource(ch, VDC5_LAYER_ID_0_WR);
            if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
            {
                rsrc_state = VDC5_ShrdPrmGetLayerResource(
                                    ((ch == VDC5_CHANNEL_0) ? VDC5_CHANNEL_1 : VDC5_CHANNEL_0),
                                    VDC5_LAYER_ID_1_WR);
            }
        }
        else if (param->module == VDC5_COLORMTX_ADJ_0)
        {
            rsrc_state = VDC5_ShrdPrmGetLayerResource(ch, VDC5_LAYER_ID_0_RD);
        }
        else
        {
            rsrc_state = VDC5_ShrdPrmGetLayerResource(ch, VDC5_LAYER_ID_1_RD);
        }

        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_ImageColorMatrix(ch, param);
    }
    return ret;
}   /* End of function R_VDC5_ImageColorMatrix() */

/**************************************************************************//**
 * @brief       VDC5 driver image enhancement
 *
 * @param[in]   ch                      : Channel
 * @param[in]   imgimprv_id
 * @param[in]   shp_h_on
 * @param[in]   sharp_param
 * @param[in]   lti_h_on
 * @param[in]   lti_param
 * @param[in]   enh_area
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_ImageEnhancement (
    const vdc5_channel_t                ch,
    const vdc5_imgimprv_id_t            imgimprv_id,
    const vdc5_onoff_t                  shp_h_on,
    const vdc5_enhance_sharp_t  * const sharp_param,
    const vdc5_onoff_t                  lti_h_on,
    const vdc5_enhance_lti_t    * const lti_param,
    const vdc5_period_rect_t    * const enh_area)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ImageEnhancementCheckPrm(ch, imgimprv_id, sharp_param, enh_area);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetLayerResource(
                            ch,
                            ((imgimprv_id == VDC5_IMG_IMPRV_0) ? VDC5_LAYER_ID_0_RD : VDC5_LAYER_ID_1_RD));
        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {   /* The layer is invalid. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_ImageEnhancement(ch, imgimprv_id, shp_h_on, sharp_param, lti_h_on, lti_param, enh_area);
    }
    return ret;
}   /* End of function R_VDC5_ImageEnhancement() */

/**************************************************************************//**
 * @brief       VDC5 driver black stretch
 *
 * @param[in]   ch                      : Channel
 * @param[in]   imgimprv_id
 * @param[in]   bkstr_on                : Black stretch ON/OFF control
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_ImageBlackStretch (
    const vdc5_channel_t        ch,
    const vdc5_imgimprv_id_t    imgimprv_id,
    const vdc5_onoff_t          bkstr_on,
    const vdc5_black_t  * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ImageBlackStretchCheckPrm(ch, imgimprv_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetLayerResource(
                            ch,
                            ((imgimprv_id == VDC5_IMG_IMPRV_0) ? VDC5_LAYER_ID_0_RD : VDC5_LAYER_ID_1_RD));
        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {   /* The layer is invalid. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_ImageBlackStretch(ch, imgimprv_id, bkstr_on, param);
    }
    return ret;
}   /* End of function R_VDC5_ImageBlackStretch() */

/**************************************************************************//**
 * @brief       VDC5 driver alpha blending
 *
 * @param[in]   ch                      : Channel
 * @param[in]   layer_id
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_AlphaBlending (
    const vdc5_channel_t                ch,
    const vdc5_layer_id_t               layer_id,
    const vdc5_alpha_blending_t * const param)
{
    vdc5_error_t            ret;
    vdc5_graphics_type_t    graphics_id;
    vdc5_resource_state_t   rsrc_state;

    graphics_id = ConvertRwProcId2GrId(layer_id);

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_AlphaBlendingCheckPrm(ch, layer_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetLayerResource(ch, layer_id);
        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {   /* The specified layer is invalid. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_AlphaBlending(ch, graphics_id, param);
    }
    return ret;
}   /* End of function R_VDC5_AlphaBlending() */

/**************************************************************************//**
 * @brief       VDC5 driver alpha blending in a rectangular area
 *
 * @param[in]   ch                      : Channel
 * @param[in]   layer_id
 * @param[in]   gr_arc_on               : Alpha blending in a rectangular area ON/OFF
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_AlphaBlendingRect (
    const vdc5_channel_t                        ch,
    const vdc5_layer_id_t                       layer_id,
    const vdc5_onoff_t                          gr_arc_on,
    const vdc5_alpha_blending_rect_t    * const param)
{
    vdc5_error_t            ret;
    vdc5_graphics_type_t    graphics_id;
    vdc5_color_space_t      color_space;
    vdc5_resource_state_t   rsrc_state;

    graphics_id = ConvertRwProcId2GrId(layer_id);

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_AlphaBlendingRectCheckPrm(ch, layer_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        if (layer_id != VDC5_LAYER_ID_VIN_RD)
        {
            /* Condition checks */
            color_space = VDC5_ShrdPrmGetColorSpaceFbRd(ch, graphics_id);
            if (color_space == VDC5_COLOR_SPACE_YCBCR)
            {   /* Alpha blending cannot be used
                   when the format of the signal read from the frame buffer is YCbCr422 or YCbCr444. */
                ret = VDC5_ERR_IF_CONDITION;
            }
            else
            {
                rsrc_state = VDC5_ShrdPrmGetLayerResource(ch, layer_id);
                if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
                {   /* The specified layer is invalid. */
                    ret = VDC5_ERR_RESOURCE_LAYER;
                }
            }
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_AlphaBlendingRect(ch, graphics_id, gr_arc_on, param);
    }
    return ret;
}   /* End of function R_VDC5_AlphaBlendingRect() */

/**************************************************************************//**
 * @brief       VDC5 driver chroma-key
 *
 * @param[in]   ch                      : Channel
 * @param[in]   layer_id
 * @param[in]   gr_ck_on                : Chroma-key processing ON/OFF
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_Chromakey (
    const vdc5_channel_t            ch,
    const vdc5_layer_id_t           layer_id,
    const vdc5_onoff_t              gr_ck_on,
    const vdc5_chromakey_t  * const param)
{
    vdc5_error_t            ret;
    vdc5_graphics_type_t    graphics_id;
    vdc5_color_space_t      color_space;
    vdc5_resource_state_t   rsrc_state;

    graphics_id = ConvertRwProcId2GrId(layer_id);

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ChromakeyCheckPrm(ch, layer_id, graphics_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Condition checks */
        color_space = VDC5_ShrdPrmGetColorSpaceFbRd(ch, graphics_id);
        if (color_space == VDC5_COLOR_SPACE_YCBCR)
        {   /* Chroma-key processing cannot be used
               when the format of the signal read from the frame buffer is YCbCr422 or YCbCr444. */
            ret = VDC5_ERR_IF_CONDITION;
        }
        else
        {
            /* Resource checks */
            rsrc_state = VDC5_ShrdPrmGetLayerResource(ch, layer_id);
            if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
            {   /* The specified layer is invalid. */
                ret = VDC5_ERR_RESOURCE_LAYER;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_Chromakey(ch, graphics_id, gr_ck_on, param);
    }
    return ret;
}   /* End of function R_VDC5_Chromakey() */

/**************************************************************************//**
 * @brief       VDC5 driver set color lookup table
 *
 * @param[in]   ch                      : Channel
 * @param[in]   layer_id
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_CLUT (const vdc5_channel_t ch, const vdc5_layer_id_t layer_id, const vdc5_clut_t * const param)
{
    vdc5_error_t            ret;
    vdc5_graphics_type_t    graphics_id;
    vdc5_resource_state_t   rsrc_state;

    graphics_id = ConvertRwProcId2GrId(layer_id);

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_CLUTCheckPrm(ch, layer_id, graphics_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetLayerResource(ch, layer_id);
        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {   /* The specified layer is invalid. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_CLUT(ch, graphics_id, param);
    }
    return ret;
}   /* End of function R_VDC5_SetCLUT() */

/**************************************************************************//**
 * @brief       VDC5 driver display calibration
 *
 * @param[in]   ch                      : Channel
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_DisplayCalibration (const vdc5_channel_t ch, const vdc5_disp_calibration_t * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_DisplayCalibrationCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetResource(ch, VDC5_RESOURCE_LCD_PANEL);
        if (rsrc_state != VDC5_RESOURCE_ST_VALID)
        {
            ret = VDC5_ERR_RESOURCE_OUTPUT;
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_DisplayCalibration(ch, param);
    }
    return ret;
}   /* End of function R_VDC5_DisplayCalibration() */

/**************************************************************************//**
 * @brief       VDC5 driver gamma correction
 *
 * @param[in]   ch                      : Channel
 * @param[in]   gam_on                  : Gamma correction ON/OFF control
 * @param[in]   param
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_GammaCorrection (
    const vdc5_channel_t                    ch,
    const vdc5_onoff_t                      gam_on,
    const vdc5_gamma_correction_t   * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_GammaCorrectionCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetResource(ch, VDC5_RESOURCE_LCD_PANEL);
        if (rsrc_state != VDC5_RESOURCE_ST_VALID)
        {
            ret = VDC5_ERR_RESOURCE_OUTPUT;
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_GammaCorrection(ch, gam_on, param);
    }
    return ret;
}   /* End of function R_VDC5_GammaCorrection() */

/******************************************************************************
Local Functions
******************************************************************************/
/**************************************************************************//**
 * @brief       Convert read/write process ID into scaling type ID
 *
 * @param[in]   layer_id              : Channel
 * @retval      Scaling type ID
 *****************************************************************************/
static vdc5_scaling_type_t ConvertRwProcId2SclId (const vdc5_layer_id_t layer_id)
{
    vdc5_scaling_type_t scaling_id;

    switch (layer_id)
    {
        case VDC5_LAYER_ID_0_WR:                /* Layer 0, write process */
            scaling_id = VDC5_SC_TYPE_SC0;      /* Scaler 0 */
        break;
        case VDC5_LAYER_ID_1_WR:                /* Layer 1, write process */
            scaling_id = VDC5_SC_TYPE_SC1;      /* Scaler 1 */
        break;
        case VDC5_LAYER_ID_OIR_WR:              /* Layer OIR, write process */
            scaling_id = VDC5_SC_TYPE_OIR;      /* OIR */
        break;
        default:                                /* Error */
            scaling_id = VDC5_SC_TYPE_NUM;
        break;
    }
    return scaling_id;
}   /* End of function ConvertRwProcId2SclId() */

/**************************************************************************//**
 * @brief       Convert read/write process ID into graphics type ID
 *
 * @param[in]   layer_id              : Channel
 * @retval      Graphics type ID
 *****************************************************************************/
static vdc5_graphics_type_t ConvertRwProcId2GrId (const vdc5_layer_id_t layer_id)
{
    vdc5_graphics_type_t graphics_id;

    switch (layer_id)
    {
        case VDC5_LAYER_ID_0_RD:                /* Layer 0, read process */
            graphics_id = VDC5_GR_TYPE_GR0;     /* Graphics 0 */
        break;
        case VDC5_LAYER_ID_1_RD:                /* Layer 1, read process */
            graphics_id = VDC5_GR_TYPE_GR1;     /* Graphics 1 */
        break;
        case VDC5_LAYER_ID_2_RD:                /* Layer 2, read process */
            graphics_id = VDC5_GR_TYPE_GR2;     /* Graphics 2 */
        break;
        case VDC5_LAYER_ID_3_RD:                /* Layer 3, read process */
            graphics_id = VDC5_GR_TYPE_GR3;     /* Graphics 3 */
        break;
        case VDC5_LAYER_ID_VIN_RD:              /* Layer VIN, read process */
            graphics_id = VDC5_GR_TYPE_VIN;     /* VIN */
        break;
        case VDC5_LAYER_ID_OIR_RD:              /* Layer OIR, read process */
            graphics_id = VDC5_GR_TYPE_OIR;     /* OIR */
        break;
        default:                                /* Error */
            graphics_id = VDC5_GR_TYPE_NUM;
        break;
    }
    return graphics_id;
}   /* End of function ConvertRwProcId2GrId() */

