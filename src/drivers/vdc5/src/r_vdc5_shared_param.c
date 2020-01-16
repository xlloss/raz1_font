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
* @file         r_vdc5_shared_param.c
* @version      0.11
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* @brief        VDC5 driver shared parameters
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    <string.h>

#include    "r_vdc5.h"
#include    "r_vdc5_user.h"
#include    "r_vdc5_shared_param.h"


/******************************************************************************
Macro definitions
******************************************************************************/
/*! Two to the power of eleven */
#define     TWO_POWER_ELEVEN                (2048)

#define     VDC5_SPARA_DIV_2                (2)
#define     VDC5_SPARA_DIV_256              (256) 

/* Color Conversion Value */
#define     VDC5_SPARA_COLOR_CONV_RANGE     (255)
#define     VDC5_SPARA_COLOR_CONV_OFFSET    (128)

/* Color mask data */
#define     VDC5_SPARA_COLOR_8BIT_MASK      (0x000000FFu)

/* shift value */
#define     VDC5_SPARA_SHIFT_16             (16u)
#define     VDC5_SPARA_SHIFT_8              (8u)

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
    vdc5_panel_clksel_t     panel_icksel;   /*!< Panel clock select */
    vdc5_onoff_t            lvds_ref;       /*!< Reference to the LVDS PLL clock */
    vdc5_color_space_t      color_sp_in;    /*!< Color space */
    vdc5_input_sel_t        inp_sel;        /*!< Input select */
    vdc5_res_vs_in_sel_t    res_vs_in_sel;  /*!< Sync signal output and full-screen enable signal select */
    vdc5_onoff_t            cascade;        /*!< Cascade */
    vdc5_onoff_t            und_sel;        /*!< Selection of lower-layer plane in scaler */
    vdc5_onoff_t            en_gr0_scaler;  /*!< Scaler in graphics 0 is enabled or not */
    uint32_t                bg_color_rgb;   /*!< Background color in 24-bit RGB color format */
    uint32_t                bg_color_crycb; /*!< Background color in 24-bit CrYCb format */
} vdc5_shared_param_common_t;

typedef struct
{
    vdc5_wr_md_t            res_ds_wr_md;   /*!< Frame buffer writing mode */
    vdc5_res_inter_t        res_inter;      /*!< Field operating mode select */
    vdc5_color_space_t      color_sp_fb_wr; /*!< Color space of the frame buffer writing format */
    void                  * btm_base;       /*!< Frame buffer base address for bottom */
    vdc5_width_read_fb_t    width_wr_fb;    /*!< Width of the image output from scaling-down control block */
} vdc5_shared_param_scaling_t;

typedef struct
{
    vdc5_gr_flm_sel_t       gr_flm_sel;         /*!< Selects a frame buffer address setting signal */
    vdc5_gr_format_t        gr_format;          /*!< Graphics format of the frame buffer read signal */
    vdc5_color_space_t      color_sp_fb_rd;     /*!< Color space of the frame buffer reading format */
    vdc5_onoff_t            adj_sel;            /*!< Measures to decrease the influence
                                                     by folding pixels/lines (ON/OFF) */
    vdc5_period_rect_t      gr_grc;             /*!< Display area */
    vdc5_width_read_fb_t    width_read_fb;      /*!< Width of the image read from frame buffer */
} vdc5_shared_param_graphics_t;

typedef struct
{
    vdc5_resource_state_t   rsrc_panel_clock;
    vdc5_resource_state_t   rsrc_vidoe_input;
    vdc5_resource_state_t   rsrc_vsync_signal;
    vdc5_resource_state_t   rsrc_lcd_panel;
} vdc5_shared_param_resource_t;

/*! Read/write process state */
typedef enum
{
    VDC5_RW_PROC_STATE_DISABLE   = 0,
    VDC5_RW_PROC_STATE_ENABLE    = 1
} vdc5_rw_proc_state_t;


/******************************************************************************
Private global variables and functions
******************************************************************************/
static vdc5_resource_state_t * GetResourceStatePointer(const vdc5_channel_t ch, const vdc5_resource_type_t rsrc_type);
static uint32_t SumProduct(
    const int32_t   red,
    const int32_t   green,
    const int32_t   blue,
    int32_t         coeff_r,
    int32_t         coeff_g,
    int32_t         coeff_b,
    const int32_t   offset);

static vdc5_shared_param_common_t   param_common[VDC5_CHANNEL_NUM];
static vdc5_shared_param_scaling_t  param_scaling[VDC5_CHANNEL_NUM][VDC5_SC_TYPE_NUM];
static vdc5_shared_param_graphics_t param_graphics[VDC5_CHANNEL_NUM][VDC5_GR_TYPE_NUM];
/*! A channel in VDC5 whose data is to be output through the LVDS */
static vdc5_channel_t               lvds_vdc_sel_ch;

static vdc5_resource_state_t        layer_resource[VDC5_CHANNEL_NUM][VDC5_LAYER_ID_NUM];
static vdc5_rw_proc_state_t         rw_proc_state[VDC5_CHANNEL_NUM][VDC5_LAYER_ID_NUM];

static vdc5_onoff_t video_input_flag[VDC5_CHANNEL_NUM] = {VDC5_OFF, VDC5_OFF};


/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmInit (const vdc5_channel_t ch)
{
    int32_t                         layer_id_num;
    vdc5_shared_param_common_t    * shared_param_common;
    vdc5_shared_param_scaling_t   * shared_param_scaling;
    vdc5_scaling_type_t             scaling_id;
    vdc5_shared_param_graphics_t  * shared_param_graphics;
    vdc5_graphics_type_t            graphics_id;

    for (layer_id_num = 0; layer_id_num < VDC5_LAYER_ID_NUM; layer_id_num++)
    {
        layer_resource[ch][layer_id_num] = VDC5_RESOURCE_ST_INVALID;
        rw_proc_state[ch][layer_id_num]  = VDC5_RW_PROC_STATE_DISABLE;
    }

    shared_param_common = &param_common[ch];

    shared_param_common->panel_icksel   = VDC5_PANEL_ICKSEL_PERI;   /* Panel clock select */
    shared_param_common->lvds_ref       = VDC5_OFF;                 /* Reference to the LVDS PLL clock */
    shared_param_common->res_vs_in_sel  = VDC5_RES_VS_IN_SEL_SC0;   /* Sync signal output and
                                                                       full-screen enable signal select */
    shared_param_common->cascade        = VDC5_ON;                  /* Cascade */
    shared_param_common->und_sel        = VDC5_OFF;                 /* Selection of lower-layer plane in scaler */
    shared_param_common->en_gr0_scaler  = VDC5_OFF;                 /* Scaler in graphics 0 is enabled or not */
    shared_param_common->bg_color_rgb   = (uint32_t)0u;             /* Background color in 24-bit RGB color format */
    shared_param_common->bg_color_crycb = (uint32_t)0u;             /* Background color in 24-bit CrYCb format */

    if (video_input_flag[ch] == VDC5_OFF)
    {
        shared_param_common->color_sp_in    = VDC5_COLOR_SPACE_GBR; /* Color space */
        shared_param_common->inp_sel        = VDC5_INPUT_SEL_VDEC;  /* Input select */
    }

    for (scaling_id = VDC5_SC_TYPE_SC0; scaling_id < VDC5_SC_TYPE_NUM; scaling_id++)
    {
        shared_param_scaling = &param_scaling[ch][scaling_id];

        shared_param_scaling->res_ds_wr_md      = VDC5_WR_MD_NORMAL;            /* Frame buffer writing mode */
        shared_param_scaling->res_inter         = VDC5_RES_INTER_PROGRESSIVE;   /* Field operating mode select */
        /* Color space of the frame buffer writing format */
        shared_param_scaling->color_sp_fb_wr    = VDC5_COLOR_SPACE_GBR;
        /* Frame buffer base address for bottom */
        shared_param_scaling->btm_base          = NULL;
        /* Width of the image output from scaling-down control block */
        shared_param_scaling->width_wr_fb.in_vw = (uint16_t)0;
        shared_param_scaling->width_wr_fb.in_hw = (uint16_t)0;
    }

    for (graphics_id = VDC5_GR_TYPE_GR0; graphics_id < VDC5_GR_TYPE_NUM; graphics_id++)
    {
        shared_param_graphics = &param_graphics[ch][graphics_id];

        /* Selects a frame buffer address setting signal */
        shared_param_graphics->gr_flm_sel = VDC5_GR_FLM_SEL_FLM_NUM;
        /* Graphics format of the frame buffer read signal */
        shared_param_graphics->gr_format = VDC5_GR_FORMAT_RGB565;
        /* Color space of the frame buffer reading format */
        shared_param_graphics->color_sp_fb_rd = VDC5_COLOR_SPACE_GBR;
        /* Measures to decrease the influence by folding pixels/lines (ON/OFF) */
        shared_param_graphics->adj_sel = VDC5_OFF;
        /* Display area */
        shared_param_graphics->gr_grc.vs = (uint16_t)0;
        shared_param_graphics->gr_grc.vw = (uint16_t)0;
        shared_param_graphics->gr_grc.hs = (uint16_t)0;
        shared_param_graphics->gr_grc.hw = (uint16_t)0;
        /* Width of the image read from frame buffer */
        shared_param_graphics->width_read_fb.in_vw = (uint16_t)0;
        shared_param_graphics->width_read_fb.in_hw = (uint16_t)0;
    }
    /* A channel in VDC5 whose data is to be output through the LVDS */
    lvds_vdc_sel_ch = VDC5_CHANNEL_NUM;     /* Initialized with invalid channel */
}   /* End of function VDC5_ShrdPrmInit() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   param
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetInitParam (const vdc5_channel_t ch, const vdc5_init_t * const param)
{
    vdc5_shared_param_common_t    * shared_param_common;

    shared_param_common = &param_common[ch];

    /* Panel clock select */
    shared_param_common->panel_icksel = param->panel_icksel;
    /* Reference to the LVDS PLL clock */
    if ((param->panel_icksel == VDC5_PANEL_ICKSEL_LVDS) || (param->panel_icksel == VDC5_PANEL_ICKSEL_LVDS_DIV7))
    {
        shared_param_common->lvds_ref = VDC5_ON;
    }
    if (param->lvds != NULL)
    {   /* A channel in VDC5 whose data is to be output through the LVDS */
        lvds_vdc_sel_ch = param->lvds->lvds_vdc_sel;
    }
}   /* End of function VDC5_ShrdPrmSetInitParam() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetTerminate (const vdc5_channel_t ch)
{
    vdc5_shared_param_common_t    * shared_param_common;

    shared_param_common = &param_common[ch];

    /* Reference to the LVDS PLL clock */
    shared_param_common->lvds_ref = VDC5_OFF;
}   /* End of function VDC5_ShrdPrmSetTerminate() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   param
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetInputParam (const vdc5_channel_t ch, const vdc5_input_t * const param)
{
    vdc5_shared_param_common_t    * shared_param_common;

    video_input_flag[ch] = VDC5_ON;

    shared_param_common = &param_common[ch];

    /* Color space */
    if (param->inp_sel == VDC5_INPUT_SEL_VDEC)
    {   /* Video decoder output signals */
        shared_param_common->color_sp_in = VDC5_COLOR_SPACE_YCBCR;
    }
    else
    {   /* Signals supplied via the external input pins */
        if ((param->ext_sig->inp_format == VDC5_EXTIN_FORMAT_RGB888) ||
            (param->ext_sig->inp_format == VDC5_EXTIN_FORMAT_RGB666) ||
            (param->ext_sig->inp_format == VDC5_EXTIN_FORMAT_RGB565))
        {
            shared_param_common->color_sp_in = VDC5_COLOR_SPACE_GBR;
        }
        else
        {
            shared_param_common->color_sp_in = VDC5_COLOR_SPACE_YCBCR;
        }
    }
    /* Input select */
    shared_param_common->inp_sel = param->inp_sel;
}   /* End of function VDC5_ShrdPrmSetInputParam() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   param
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetSyncParam (const vdc5_channel_t ch, const vdc5_sync_ctrl_t * const param)
{
    vdc5_shared_param_common_t    * shared_param_common;

    shared_param_common = &param_common[ch];

    shared_param_common->res_vs_in_sel = param->res_vs_in_sel;
}   /* End of function VDC5_ShrdPrmSetSyncParam() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   param
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetOutputParam (const vdc5_channel_t ch, const vdc5_output_t * const param)
{
    vdc5_shared_param_common_t    * shared_param_common;
    int32_t                         red;
    int32_t                         green;
    int32_t                         blue;
    uint32_t                        y_val;
    uint32_t                        cb_val;
    uint32_t                        cr_val;

    shared_param_common = &param_common[ch];

    /* Background color in 24-bit RGB color format */
    shared_param_common->bg_color_rgb = param->bg_color;

    red     = (int32_t)((param->bg_color >> VDC5_SPARA_SHIFT_16) & (uint32_t)VDC5_SPARA_COLOR_8BIT_MASK);
    green   = (int32_t)((param->bg_color >> VDC5_SPARA_SHIFT_8)  & (uint32_t)VDC5_SPARA_COLOR_8BIT_MASK);
    blue    = (int32_t)( param->bg_color                         & (uint32_t)VDC5_SPARA_COLOR_8BIT_MASK);
    /* Y */
    y_val = SumProduct(
        red,
        green,
        blue,
        (int32_t)VDC5_COLORCONV_Y_R,
        (int32_t)VDC5_COLORCONV_Y_G,
        (int32_t)VDC5_COLORCONV_Y_B,
        0);
    /* Cb */
    cb_val = SumProduct(
        red,
        green,
        blue,
        (int32_t)VDC5_COLORCONV_CB_R,
        (int32_t)VDC5_COLORCONV_CB_G,
        (int32_t)VDC5_COLORCONV_CB_B,
        (int32_t)VDC5_SPARA_COLOR_CONV_OFFSET);
    /* Cr */
    cr_val = SumProduct(
        red,
        green,
        blue,
        (int32_t)VDC5_COLORCONV_CR_R,
        (int32_t)VDC5_COLORCONV_CR_G,
        (int32_t)VDC5_COLORCONV_CR_B,
        (int32_t)VDC5_SPARA_COLOR_CONV_OFFSET);
    /* Background color in 24-bit CrYCb format */
    shared_param_common->bg_color_crycb = (uint32_t)((cr_val << VDC5_SPARA_SHIFT_16) | (y_val << VDC5_SPARA_SHIFT_8) | cb_val);
}   /* End of function VDC5_ShrdPrmSetOutputParam() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   scaling_id
 * @param[in]   param
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetWriteParam (
    const vdc5_channel_t        ch,
    const vdc5_scaling_type_t   scaling_id,
    const vdc5_write_t  * const param)
{
    vdc5_shared_param_scaling_t   * shared_param_scaling;

    shared_param_scaling = &param_scaling[ch][scaling_id];

    if (scaling_id == VDC5_SC_TYPE_OIR)
    {
        /* Frame buffer writing mode */
        shared_param_scaling->res_ds_wr_md = VDC5_WR_MD_NORMAL;
        /* Field operating mode select */
        shared_param_scaling->res_inter = VDC5_RES_INTER_PROGRESSIVE;
    }
    else
    {
        /* Frame buffer writing mode */
        shared_param_scaling->res_ds_wr_md = param->sclingdown_rot.res_ds_wr_md;
        /* Field operating mode select */
        shared_param_scaling->res_inter = param->res_inter;
    }

    /* Color space of the frame buffer */
    if ((param->res_md == VDC5_RES_MD_YCBCR422) || (param->res_md == VDC5_RES_MD_YCBCR444))
    {
        shared_param_scaling->color_sp_fb_wr = VDC5_COLOR_SPACE_YCBCR;
    }
    else
    {
        shared_param_scaling->color_sp_fb_wr = VDC5_COLOR_SPACE_GBR;
    }

    /* Frame buffer base address for bottom */
    shared_param_scaling->btm_base = param->btm_base;
    /* Width of the image output from scaling-down control block */
    if (scaling_id == VDC5_SC_TYPE_OIR)
    {   /* OIR */
        shared_param_scaling->width_wr_fb.in_vw = param->sclingdown_rot.res.vw;
        shared_param_scaling->width_wr_fb.in_hw = param->sclingdown_rot.res.hw;
    }
    else
    {
        shared_param_scaling->width_wr_fb.in_vw = param->sclingdown_rot.res_out_vw;
        shared_param_scaling->width_wr_fb.in_hw = param->sclingdown_rot.res_out_hw;
    }
}   /* End of function VDC5_ShrdPrmSetWriteParam() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   scaling_id
 * @param[in]   param
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetChgWriteParam (
    const vdc5_channel_t            ch,
    const vdc5_scaling_type_t       scaling_id,
    const vdc5_write_chg_t  * const param)
{
    vdc5_shared_param_scaling_t   * shared_param_scaling;

    shared_param_scaling = &param_scaling[ch][scaling_id];

    if (scaling_id != VDC5_SC_TYPE_OIR)
    {
        /* Frame buffer writing mode */
        shared_param_scaling->res_ds_wr_md = param->sclingdown_rot.res_ds_wr_md;
    }

    /* Width of the image output from scaling-down control block */
    if (scaling_id == VDC5_SC_TYPE_OIR)
    {   /* OIR */
        shared_param_scaling->width_wr_fb.in_vw = param->sclingdown_rot.res.vw;
        shared_param_scaling->width_wr_fb.in_hw = param->sclingdown_rot.res.hw;
    }
    else
    {
        shared_param_scaling->width_wr_fb.in_vw = param->sclingdown_rot.res_out_vw;
        shared_param_scaling->width_wr_fb.in_hw = param->sclingdown_rot.res_out_hw;
    }
}   /* End of function VDC5_ShrdPrmSetChgWriteParam() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   graphics_id
 * @param[in]   param
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetReadParam (
    const vdc5_channel_t        ch,
    const vdc5_graphics_type_t  graphics_id,
    const vdc5_read_t   * const param)
{
    vdc5_shared_param_graphics_t  * shared_param_graphics;

    shared_param_graphics = &param_graphics[ch][graphics_id];

    /* Selects a frame buffer address setting signal */
    shared_param_graphics->gr_flm_sel = param->gr_flm_sel;

    /* Graphics format of the frame buffer read signal */
    shared_param_graphics->gr_format = param->gr_format;
 
    /* Color space of the frame buffer */
    if ((param->gr_format == VDC5_GR_FORMAT_YCBCR422) || (param->gr_format == VDC5_GR_FORMAT_YCBCR444))
    {
        shared_param_graphics->color_sp_fb_rd = VDC5_COLOR_SPACE_YCBCR;
    }
    else
    {
        shared_param_graphics->color_sp_fb_rd = VDC5_COLOR_SPACE_GBR;
    }

    /* Measures to decrease the influence by folding pixels/lines (ON/OFF) */
    shared_param_graphics->adj_sel = param->adj_sel;

    /* Display area */
    shared_param_graphics->gr_grc = param->gr_grc;
    /* Width of the image read from frame buffer */
    if (param->width_read_fb != NULL)
    {
        shared_param_graphics->width_read_fb.in_vw = param->width_read_fb->in_vw;
        shared_param_graphics->width_read_fb.in_hw = param->width_read_fb->in_hw;
    }
    else
    {   /* If not specified, ... */
        if ((param->gr_flm_sel == VDC5_GR_FLM_SEL_FLM_NUM) || (graphics_id == VDC5_GR_TYPE_OIR))
        {   /* The width of the image read is assumed that it is the same as the width of the display area. */
            shared_param_graphics->width_read_fb.in_vw = param->gr_grc.vw;
            shared_param_graphics->width_read_fb.in_hw = param->gr_grc.hw;
        }
        else
        {   /* The width of the image output from scaling-down control block is used for the width of the image read
               when a frame buffer address setting signal links to the video image signals. */
            if (graphics_id == VDC5_GR_TYPE_GR0)
            {       /* Scaler 0 */
                shared_param_graphics->width_read_fb = param_scaling[ch][VDC5_SC_TYPE_SC0].width_wr_fb;
            }
            else    /* Scaler 1 */
            {
                shared_param_graphics->width_read_fb = param_scaling[ch][VDC5_SC_TYPE_SC1].width_wr_fb;
            }
        }
    }
}   /* End of function VDC5_ShrdPrmSetReadParam() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   graphics_id
 * @param[in]   param
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetChgReadParam (
    const vdc5_channel_t            ch,
    const vdc5_graphics_type_t      graphics_id,
    const vdc5_read_chg_t   * const param)
{
    vdc5_shared_param_graphics_t  * shared_param_graphics;

    shared_param_graphics = &param_graphics[ch][graphics_id];

    /* Display area */
    if (param->gr_grc != NULL)
    {
        shared_param_graphics->gr_grc = *(param->gr_grc);
    }

    /* Width of the image read from frame buffer */
    if (param->width_read_fb != NULL)
    {
        shared_param_graphics->width_read_fb.in_vw = param->width_read_fb->in_vw;
        shared_param_graphics->width_read_fb.in_hw = param->width_read_fb->in_hw;
    }
}   /* End of function VDC5_ShrdPrmSetChgReadParam() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   cascade
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetCascade (const vdc5_channel_t ch, const vdc5_onoff_t cascade)
{
    param_common[ch].cascade = cascade;
}   /* End of function VDC5_ShrdPrmSetCascade() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   und_sel
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetUndSel (const vdc5_channel_t ch, const vdc5_onoff_t und_sel)
{
    param_common[ch].und_sel = und_sel;
}   /* End of function VDC5_ShrdPrmSetUndSel() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   en_gr0_scaler
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetGr0Scaler (const vdc5_channel_t ch, const vdc5_onoff_t en_gr0_scaler)
{
    /* Scaler in graphics 0 is enabled or not
       VDC5_ON: Graphics 0 is used for the input video image display or the enlarged graphics display.
       VDC5_OFF: Graphics 0 is not used, or graphics 0 is used for the normal graphics display.
     */
    param_common[ch].en_gr0_scaler = en_gr0_scaler;
}   /* End of function VDC5_ShrdPrmSetGr0Scaler() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @retval      Panel clock select
 *****************************************************************************/
vdc5_panel_clksel_t VDC5_ShrdPrmGetPanelClkSel (const vdc5_channel_t ch)
{
    return param_common[ch].panel_icksel;
}   /* End of function VDC5_ShrdPrmGetPanelClkSel() */

/**************************************************************************//**
 * @brief       
 * @param[in]   void
 * @retval      Reference to the LVDS PLL clock
 *****************************************************************************/
vdc5_onoff_t VDC5_ShrdPrmGetLvdsClkRef (void)
{
    vdc5_onoff_t onoff;

    if ((param_common[VDC5_CHANNEL_0].lvds_ref == VDC5_OFF) && (param_common[VDC5_CHANNEL_1].lvds_ref == VDC5_OFF))
    {
        onoff = VDC5_OFF;
    }
    else
    {
        onoff = VDC5_ON;
    }
    return onoff;
}   /* End of function VDC5_ShrdPrmGetLvdsClkRef() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @retval      Color space
 *****************************************************************************/
vdc5_color_space_t VDC5_ShrdPrmGetColorSpace (const vdc5_channel_t ch)
{
    return param_common[ch].color_sp_in;
}   /* End of function VDC5_ShrdPrmGetColorSpace() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @retval      Input select
 *****************************************************************************/
vdc5_input_sel_t VDC5_ShrdPrmGetInputSelect (const vdc5_channel_t ch)
{
    return param_common[ch].inp_sel;
}   /* End of function VDC5_ShrdPrmGetInputSelect() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @retval      SC_RES_VS_IN_SEL
 *****************************************************************************/
vdc5_res_vs_in_sel_t VDC5_ShrdPrmGetVsInSel (const vdc5_channel_t ch)
{
    return param_common[ch].res_vs_in_sel;
}   /* End of function VDC5_ShrdPrmGetVsInSel() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @retval      Cascade ON/OFF
 *****************************************************************************/
vdc5_onoff_t VDC5_ShrdPrmGetCascade (const vdc5_channel_t ch)
{
    return param_common[ch].cascade;
}   /* End of function VDC5_ShrdPrmGetCascade() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @retval      Selection of lower-layer plane in scaler, ON/OFF
 *****************************************************************************/
vdc5_onoff_t VDC5_ShrdPrmGetUndSel (const vdc5_channel_t ch)
{
    return param_common[ch].und_sel;
}   /* End of function VDC5_ShrdPrmGetUndSel() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @retval      Scaler in graphics 0 is enabled or not
 *****************************************************************************/
vdc5_onoff_t VDC5_ShrdPrmGetGr0Scaler (const vdc5_channel_t ch)
{
    return param_common[ch].en_gr0_scaler;
}   /* End of function VDC5_ShrdPrmGetGr0Scaler() */

/**************************************************************************//**
 * @brief       Get background color
 * @param[in]   ch
 * @param[in]   color_space
 * @retval      Background color in 24-bit RGB color format or CrYCb format
 *****************************************************************************/
uint32_t VDC5_ShrdPrmGetBgColor (const vdc5_channel_t ch, const vdc5_color_space_t color_space)
{
    return (color_space == VDC5_COLOR_SPACE_GBR) ? param_common[ch].bg_color_rgb : param_common[ch].bg_color_crycb;
}   /* End of function VDC5_ShrdPrmGetBgColor() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   scaling_id
 * @retval      Frame buffer writing mode
 *****************************************************************************/
vdc5_wr_md_t VDC5_ShrdPrmGetWritingMode (const vdc5_channel_t ch, const vdc5_scaling_type_t scaling_id)
{
    return param_scaling[ch][scaling_id].res_ds_wr_md;
}   /* End of function VDC5_ShrdPrmGetWritingMode() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   scaling_id
 * @retval      Field operating mode select
 *****************************************************************************/
vdc5_res_inter_t VDC5_ShrdPrmGetInterlace (const vdc5_channel_t ch, const vdc5_scaling_type_t scaling_id)
{
    return param_scaling[ch][scaling_id].res_inter;
}   /* End of function VDC5_ShrdPrmGetInterlace() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   scaling_id
 * @retval      Color space of the frame buffer writing format
 *****************************************************************************/
vdc5_color_space_t VDC5_ShrdPrmGetColorSpaceFbWr (const vdc5_channel_t ch, const vdc5_scaling_type_t scaling_id)
{
    return param_scaling[ch][scaling_id].color_sp_fb_wr;
}   /* End of function VDC5_ShrdPrmGetColorSpaceFbWr() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   scaling_id
 * @retval      Frame buffer base address for bottom
 *****************************************************************************/
void * VDC5_ShrdPrmGetFrBuffBtm (const vdc5_channel_t ch, const vdc5_scaling_type_t scaling_id)
{
    return param_scaling[ch][scaling_id].btm_base;
}   /* End of function VDC5_ShrdPrmGetFrBuffBtm() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   graphics_id
 * @retval      Selects a frame buffer address setting signal
 *****************************************************************************/
vdc5_gr_flm_sel_t VDC5_ShrdPrmGetSelFbAddrSig (const vdc5_channel_t ch, const vdc5_graphics_type_t graphics_id)
{
    return param_graphics[ch][graphics_id].gr_flm_sel;
}   /* End of function VDC5_ShrdPrmGetSelFbAddrSig() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   graphics_id
 * @retval      vdc5_gr_format_t
 *****************************************************************************/
vdc5_gr_format_t VDC5_ShrdPrmGetGraphicsFormat (const vdc5_channel_t ch, const vdc5_graphics_type_t graphics_id)
{
    return param_graphics[ch][graphics_id].gr_format;
}   /* End of function VDC5_ShrdPrmGetGraphicsFormat() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   graphics_id
 * @retval      Color space of the frame buffer reading format
 *****************************************************************************/
vdc5_color_space_t VDC5_ShrdPrmGetColorSpaceFbRd (const vdc5_channel_t ch, const vdc5_graphics_type_t graphics_id)
{
    return param_graphics[ch][graphics_id].color_sp_fb_rd;
}   /* End of function VDC5_ShrdPrmGetColorSpaceFbRd() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   graphics_id
 * @retval      Measures to decrease the influence by folding pixels/lines (ON/OFF)
 *****************************************************************************/
vdc5_onoff_t VDC5_ShrdPrmGetMeasureFolding (const vdc5_channel_t ch, const vdc5_graphics_type_t graphics_id)
{
    return param_graphics[ch][graphics_id].adj_sel;
}   /* End of function VDC5_ShrdPrmGetMeasureFolding() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   graphics_id
 * @retval      Display area
 *****************************************************************************/
vdc5_period_rect_t * VDC5_ShrdPrmGetDisplayArea (const vdc5_channel_t ch, const vdc5_graphics_type_t graphics_id)
{
    return &param_graphics[ch][graphics_id].gr_grc;
}   /* End of function VDC5_ShrdPrmGetDisplayArea() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   graphics_id
 * @retval      Width of the image read from frame buffer
 *****************************************************************************/
vdc5_width_read_fb_t * VDC5_ShrdPrmGetFrBuffWidth_Rd (
    const vdc5_channel_t        ch,
    const vdc5_graphics_type_t  graphics_id)
{
    return &param_graphics[ch][graphics_id].width_read_fb;
}   /* End of function VDC5_ShrdPrmGetFrBuffWidth_Rd() */

/**************************************************************************//**
 * @brief       
 * @param[in]   void
 * @retval      A channel in VDC5 whose data is to be output through the LVDS
 *****************************************************************************/
vdc5_channel_t VDC5_ShrdPrmGetLvdsCh (void)
{
    /* This value is valid only when LVDS PLL clock resource (VDC5_RESOURCE_LVDS_CLK) is valid. */
    return lvds_vdc_sel_ch;
}   /* End of function VDC5_ShrdPrmGetLvdsCh() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   rsrc_type
 * @param[in]   rsrc_state
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetResource (
    const vdc5_channel_t        ch,
    const vdc5_resource_type_t  rsrc_type,
    const vdc5_resource_state_t rsrc_state)
{
    vdc5_resource_state_t * resource_state;

    resource_state = GetResourceStatePointer(ch, rsrc_type);
    if (resource_state != NULL)
    {
        *resource_state = rsrc_state;
    }
}   /* End of function VDC5_ShrdPrmSetResource() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   layer_id
 * @param[in]   rsrc_state
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetLayerResource (
    const vdc5_channel_t        ch,
    const vdc5_layer_id_t       layer_id,
    const vdc5_resource_state_t rsrc_state)
{
    uint32_t layer_id_num;

    if (layer_id != VDC5_LAYER_ID_ALL)
    {
        layer_resource[ch][layer_id] = rsrc_state;
    }
    else
    {
        for (layer_id_num = 0; layer_id_num < (uint32_t)VDC5_LAYER_ID_NUM; layer_id_num++)
        {
            if (rw_proc_state[ch][layer_id_num] == VDC5_RW_PROC_STATE_DISABLE)
            {
                layer_resource[ch][layer_id_num] = rsrc_state;
            }
        }
    }
}   /* End of function VDC5_ShrdPrmSetLayerResource() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   rsrc_type
 * @retval      Resource state
 *****************************************************************************/
vdc5_resource_state_t VDC5_ShrdPrmGetResource (const vdc5_channel_t ch, const vdc5_resource_type_t rsrc_type)
{
    vdc5_resource_state_t * resource_state;
    vdc5_resource_state_t   rsrc_state;

    rsrc_state = VDC5_RESOURCE_ST_INVALID;

    resource_state = GetResourceStatePointer(ch, rsrc_type);
    if (resource_state != NULL)
    {
        rsrc_state = *resource_state;
    }
    return rsrc_state;
}   /* End of function VDC5_ShrdPrmGetResource() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   layer_id
 * @retval      Resource state
 *****************************************************************************/
vdc5_resource_state_t VDC5_ShrdPrmGetLayerResource (const vdc5_channel_t ch, const vdc5_layer_id_t layer_id)
{
    return layer_resource[ch][layer_id];
}   /* End of function VDC5_ShrdPrmGetLayerResource() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   layer_id
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetRwProcEnable (const vdc5_channel_t ch, const vdc5_layer_id_t layer_id)
{
    if (layer_id != VDC5_LAYER_ID_ALL)
    {
        if (layer_resource[ch][layer_id] != VDC5_RESOURCE_ST_INVALID)
        {
            rw_proc_state[ch][layer_id] = VDC5_RW_PROC_STATE_ENABLE;
        }
    }
}   /* End of function VDC5_ShrdPrmSetRwProcEnable() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   layer_id
 * @retval      None
 *****************************************************************************/
void VDC5_ShrdPrmSetRwProcDisable (const vdc5_channel_t ch, const vdc5_layer_id_t layer_id)
{
    if (layer_id != VDC5_LAYER_ID_ALL)
    {
        if (layer_resource[ch][layer_id] != VDC5_RESOURCE_ST_INVALID)
        {
            rw_proc_state[ch][layer_id] = VDC5_RW_PROC_STATE_DISABLE;
        }
    }
}   /* End of function VDC5_ShrdPrmSetRwProcDisable() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   layer_id
 * @retval      Resource state
 *****************************************************************************/
vdc5_resource_state_t VDC5_ShrdPrmGetRwProcReady (const vdc5_channel_t ch, const vdc5_layer_id_t layer_id)
{
    vdc5_resource_state_t state;

    if ((layer_resource[ch][layer_id] != VDC5_RESOURCE_ST_INVALID) &&
        (rw_proc_state[ch][layer_id] == VDC5_RW_PROC_STATE_DISABLE))
    {
        state = VDC5_RESOURCE_ST_VALID;
    }
    else
    {
        state = VDC5_RESOURCE_ST_INVALID;
    }
    return state;
}   /* End of function VDC5_ShrdPrmGetRwProcReady() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   layer_id
 * @retval      Resource state
 *****************************************************************************/
vdc5_resource_state_t VDC5_ShrdPrmGetRwProcEnabled (const vdc5_channel_t ch, const vdc5_layer_id_t layer_id)
{
    vdc5_resource_state_t state;

    if ((layer_resource[ch][layer_id] != VDC5_RESOURCE_ST_INVALID) &&
        (rw_proc_state[ch][layer_id] != VDC5_RW_PROC_STATE_DISABLE))
    {
        state = VDC5_RESOURCE_ST_VALID;
    }
    else
    {
        state = VDC5_RESOURCE_ST_INVALID;
    }
    return state;
}   /* End of function VDC5_ShrdPrmGetRwProcEnabled() */

/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @retval      Resource state
 *****************************************************************************/
vdc5_resource_state_t VDC5_ShrdPrmGetOirRwProcEnabled (const vdc5_channel_t ch)
{
    vdc5_resource_state_t state;

    state = VDC5_RESOURCE_ST_INVALID;

    if ((layer_resource[ch][VDC5_LAYER_ID_OIR_WR] != VDC5_RESOURCE_ST_INVALID) &&
        (rw_proc_state[ch][VDC5_LAYER_ID_OIR_WR] != VDC5_RW_PROC_STATE_DISABLE))
    {
        state = VDC5_RESOURCE_ST_VALID;
    }
    if ((layer_resource[ch][VDC5_LAYER_ID_OIR_RD] != VDC5_RESOURCE_ST_INVALID) &&
        (rw_proc_state[ch][VDC5_LAYER_ID_OIR_RD] != VDC5_RW_PROC_STATE_DISABLE))
    {
        state = VDC5_RESOURCE_ST_VALID;
    }
    return state;
}   /* End of function VDC5_ShrdPrmGetOirRwProcEnabled() */

/******************************************************************************
Local Functions
******************************************************************************/
/**************************************************************************//**
 * @brief       
 * @param[in]   ch
 * @param[in]   rsrc_type
 * @param[in]   rsrc_state
 * @retval      Pointer to the resource state
 *****************************************************************************/
static vdc5_resource_state_t * GetResourceStatePointer (const vdc5_channel_t ch, const vdc5_resource_type_t rsrc_type)
{
    vdc5_resource_state_t             * resource_state;
    static vdc5_shared_param_resource_t param_resource[VDC5_CHANNEL_NUM] =
    {
        {VDC5_RESOURCE_ST_INVALID, VDC5_RESOURCE_ST_INVALID, VDC5_RESOURCE_ST_INVALID, VDC5_RESOURCE_ST_INVALID},
        {VDC5_RESOURCE_ST_INVALID, VDC5_RESOURCE_ST_INVALID, VDC5_RESOURCE_ST_INVALID, VDC5_RESOURCE_ST_INVALID}
    };
    static vdc5_resource_state_t        lvds_resource = VDC5_RESOURCE_ST_INVALID;

    switch (rsrc_type)
    {
        case VDC5_RESOURCE_PANEL_CLK:
            resource_state = &param_resource[ch].rsrc_panel_clock;
        break;
        case VDC5_RESOURCE_VIDEO_IN:
            resource_state = &param_resource[ch].rsrc_vidoe_input;
        break;
        case VDC5_RESOURCE_VSYNC:
            resource_state = &param_resource[ch].rsrc_vsync_signal;
        break;
        case VDC5_RESOURCE_LCD_PANEL:
            resource_state = &param_resource[ch].rsrc_lcd_panel;
        break;
        case VDC5_RESOURCE_LVDS_CLK:
            resource_state = &lvds_resource;
        break;

        default:
            resource_state = NULL;
        break;
    }
    return resource_state;
}   /* End of function GetResourceStatePointer() */

/**************************************************************************//**
 * @brief       Product-sum operation
 * @param[in]   red
 * @param[in]   green
 * @param[in]   blue
 * @param[in]   coeff_r
 * @param[in]   coeff_g
 * @param[in]   coeff_b
 * @param[in]   offset
 * @retval      Answer
 *****************************************************************************/
static uint32_t SumProduct (
    const int32_t   red,
    const int32_t   green,
    const int32_t   blue,
    int32_t         coeff_r,
    int32_t         coeff_g,
    int32_t         coeff_b,
    const int32_t   offset)
{
    int32_t color_val;

    /* Coefficient values are represented in 11-bit two's complement integer. */
    if (coeff_r >= (TWO_POWER_ELEVEN / VDC5_SPARA_DIV_2))
    {
        coeff_r -= (int32_t)TWO_POWER_ELEVEN;
    }
    if (coeff_g >= (TWO_POWER_ELEVEN / VDC5_SPARA_DIV_2))
    {
        coeff_g -= (int32_t)TWO_POWER_ELEVEN;
    }
    if (coeff_b >= (TWO_POWER_ELEVEN / VDC5_SPARA_DIV_2))
    {
        coeff_b -= (int32_t)TWO_POWER_ELEVEN;
    }

    color_val  = (red * coeff_r) + (green * coeff_g) + (blue * coeff_b);
    color_val /= (int32_t)VDC5_SPARA_DIV_256;
    color_val += offset;
    if (color_val < 0)
    {
        color_val = 0;
    }
    else if (color_val > (int32_t)VDC5_SPARA_COLOR_CONV_RANGE)
    {
        color_val = (int32_t)VDC5_SPARA_COLOR_CONV_RANGE;
    }
    else
    {
    }
    return (uint32_t)color_val;
}   /* End of function SumProduct() */

