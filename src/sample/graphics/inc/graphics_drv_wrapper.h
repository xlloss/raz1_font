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
* @file         graphics_drv_wrapper.h
* @version      0.11
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* @brief        Graphics driver wrapper function header
******************************************************************************/

#ifndef GRAPHICS_DRV_WRAPPER_H
#define GRAPHICS_DRV_WRAPPER_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    <stdlib.h>

#include    "r_typedefs.h"

#include    "r_vdc5.h"


/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
vdc5_error_t GRPDRV_Init(const vdc5_channel_t ch, const vdc5_onoff_t vd_in_0, const vdc5_onoff_t vd_in_1);
vdc5_error_t GRPDRV_Term(const vdc5_channel_t ch);
vdc5_error_t GRPDRV_GraphicsCreateSurface(
    const vdc5_channel_t                ch,
    const vdc5_layer_id_t               layer_id,
    void                        * const framebuff,
    const uint32_t                      fb_stride,
    const vdc5_gr_format_t              gr_format,
    const vdc5_period_rect_t    * const period_rect);
    
vdc5_error_t GRPDRV_VideoCreateSurface(
    const vdc5_channel_t                ch,
    const vdc5_layer_id_t               layer_id,
    void                        * const framebuff,
    const uint32_t                      fb_stride,
    const vdc5_res_md_t                 res_md,
    const vdc5_period_rect_t    * const res,
    const vdc5_period_rect_t    * const period_rect);
    
    
    
vdc5_error_t GRPDRV_DestroySurfaces(const vdc5_channel_t ch);
vdc5_error_t GRPDRV_StartSurfaces(const vdc5_channel_t ch, const vdc5_gr_disp_sel_t * const gr_disp_sel);
vdc5_error_t GRPDRV_StopSurfaces(const vdc5_channel_t ch);


#endif  /* GRAPHICS_DRV_WRAPPER_H */
