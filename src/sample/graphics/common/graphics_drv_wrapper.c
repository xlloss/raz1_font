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
* @file         graphics_drv_wrapper.c
* @version      0.11
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* @brief        Graphics driver wrapper function
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    <string.h>

#include    "r_typedefs.h"

#include    "r_vdc5.h"

#include    "iodefine.h"
#include    "graphics_drv_wrapper.h"
#include    "lcd_panel.h"

#include    "graphics_debug.h"


/******************************************************************************
Macro definitions
******************************************************************************/
#define STP91_BIT1  (0x02u)
#define STP91_BIT0  (0x01u)

#define VSYNC_1_2_FH_TIMING  (858u) /* Vsync signal 1/2fH phase timing */
#define VSYNC_1_4_FH_TIMING  (429u) /* Vsync signal 1/4fH phase timing */


#define BUFFER_NUM (2u)


/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
    vdc5_panel_clksel_t     panel_icksel;   /* Panel clock select */
    vdc5_panel_clk_dcdr_t   panel_dcdr;     /* Panel clock frequency division ratio */
    uint16_t                res_fv;         /* Free-running Vsync period setting */
    uint16_t                res_fh;         /* Hsync period setting */
    vdc5_period_rect_t      res_f;          /* Full-screen enable signal */
    uint16_t                tcon_half;      /* TCON reference timing, 1/2fH timing */
    uint16_t                tcon_offset;    /* TCON reference timing, offset Hsync signal timing */
    vdc5_lcd_outformat_t    out_format;     /* Output format select */
    vdc5_edge_t             outcnt_lcd_edge;/* Output phase control of LCD_DATA23 to LCD_DATA0 pin */
} lcd_dependent_param_t;


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static void init_func(const uint32_t user_num);
static void quit_func(const uint32_t user_num);

/*! Conversion table from video-signal writing format into read signal format */
static const vdc5_gr_format_t gr_format_conv[VDC5_RES_MD_NUM] =
{
    VDC5_GR_FORMAT_YCBCR422,            /* YCbCr422 */
    VDC5_GR_FORMAT_RGB565,              /* RGB565 */
    VDC5_GR_FORMAT_RGB888,              /* RGB888 */
    VDC5_GR_FORMAT_YCBCR444             /* YCbCr444 */
};
/*! Swap mode */
static const vdc5_wr_rd_swa_t gr_rdswa_tbl[VDC5_GR_FORMAT_NUM] =
{
    VDC5_WR_RD_WRSWA_32_16BIT,      /* RGB565 */
    VDC5_WR_RD_WRSWA_32BIT,         /* RGB888 */
    VDC5_WR_RD_WRSWA_32_16BIT,      /* ARGB1555 */
    VDC5_WR_RD_WRSWA_32_16BIT,      /* ARGB4444 */
    VDC5_WR_RD_WRSWA_32BIT,         /* ARGB8888 */
    VDC5_WR_RD_WRSWA_32_16_8BIT,    /* CLUT8 */
    VDC5_WR_RD_WRSWA_32_16_8BIT,    /* CLUT4 */
    VDC5_WR_RD_WRSWA_32_16_8BIT,    /* CLUT1 */
    VDC5_WR_RD_WRSWA_32BIT,         /* YCbCr422 */
    VDC5_WR_RD_WRSWA_32BIT,         /* YCbCr444 */
    VDC5_WR_RD_WRSWA_32_16BIT,      /* RGBA5551 */
    VDC5_WR_RD_WRSWA_32BIT          /* RGBA8888 */
};
static const lcd_dependent_param_t lcd_dependent_param[VDC5_CHANNEL_NUM] =
{
    {
        LCD_CH0_PANEL_CLK,                  /* Panel clock select */
        LCD_CH0_PANEL_CLK_DIV,              /* Panel clock frequency division ratio */
        (uint16_t)LCD_CH0_SIG_FV,           /* Free-running Vsync period setting */
        (uint16_t)LCD_CH0_SIG_FH,           /* Hsync period setting */
        {   /* Full-screen enable signal */
            (uint16_t)LCD_CH0_DISP_VS,
            (uint16_t)LCD_CH0_DISP_VW,
            (uint16_t)LCD_CH0_DISP_HS,
            (uint16_t)LCD_CH0_DISP_HW
        },
        (uint16_t)LCD_CH0_TCON_HALF,        /* TCON reference timing, 1/2fH timing */
        (uint16_t)LCD_CH0_TCON_OFFSET,      /* TCON reference timing, offset Hsync signal timing */
        LCD_CH0_OUT_FORMAT,                 /* Output format select */
        LCD_CH0_OUT_EDGE                    /* Output phase control of LCD_DATA23 to LCD_DATA0 pin */
    },
    {
        LCD_CH1_PANEL_CLK,                  /* Panel clock select */
        LCD_CH1_PANEL_CLK_DIV,              /* Panel clock frequency division ratio */
        (uint16_t)LCD_CH1_SIG_FV,           /* Free-running Vsync period setting */
        (uint16_t)LCD_CH1_SIG_FH,           /* Hsync period setting */
        {   /* Full-screen enable signal */
            (uint16_t)LCD_CH1_DISP_VS,
            (uint16_t)LCD_CH1_DISP_VW,
            (uint16_t)LCD_CH1_DISP_HS,
            (uint16_t)LCD_CH1_DISP_HW
        },
        (uint16_t)LCD_CH1_TCON_HALF,        /* TCON reference timing, 1/2fH timing */
        (uint16_t)LCD_CH1_TCON_OFFSET,      /* TCON reference timing, offset Hsync signal timing */
        LCD_CH1_OUT_FORMAT,                 /* Output format select */
        LCD_CH1_OUT_EDGE                    /* Output phase control of LCD_DATA23 to LCD_DATA0 pin */
    }
};


/**************************************************************************//**
 * @brief       User-defined function within R_VDC5_Initialize
 * @param[in]   user_num
 * @retval      None
******************************************************************************/
static void init_func (const uint32_t user_num)
{
    uint32_t        reg_data;
    vdc5_channel_t  channel;

    reg_data = (uint32_t)CPG.STBCR9;
    if ((vdc5_channel_t)user_num == VDC5_CHANNEL_0)
    {
        /* Standby control register 9 (STBCR9)
            b1      ------0-;  MSTP91 : 0 : Video display controller channel 0 & LVDS enable */
        reg_data &= (uint32_t)~STP91_BIT1;

        channel = VDC5_CHANNEL_0;
    }
    else
    {
        /* Standby control register 9 (STBCR9)
            b1      ------0-;  MSTP91 : 0 : Video display controller channel 0 & LVDS enable
            b0      -------0;  MSTP90 : 0 : Video display controller channel 1 enable */
        reg_data &= (uint32_t)~(STP91_BIT1|STP91_BIT0);

        channel = VDC5_CHANNEL_1;
    }
    CPG.STBCR9 = (uint8_t)reg_data;

    GRAPHICS_SetLcdPanel(channel);
}   /* End of function init_func() */

/**************************************************************************//**
 * @brief       User-defined function within R_VDC5_Terminate
 * @param[in]   user_num
 * @retval      None
******************************************************************************/
static void quit_func (const uint32_t user_num)
{
    uint32_t reg_data;

    reg_data = (uint32_t)CPG.STBCR9;
    if ((vdc5_channel_t)user_num == VDC5_CHANNEL_0)
    {   /* If LVDS should remain enabled, the following code should be removed. */
        reg_data |= (uint32_t)STP91_BIT1;
    }
    else
    {
        reg_data |= (uint32_t)STP91_BIT0;
    }
    CPG.STBCR9 = (uint8_t)reg_data;
}   /* End of function quitt_func() */

/**************************************************************************//**
 * @brief       Initialize graphics driver
 * @param[in]   ch
 * @param[in]   vd_in_0
 * @param[in]   vd_in_1
 * @retval      VDC5 driver error code
******************************************************************************/
vdc5_error_t GRPDRV_Init (const vdc5_channel_t ch, const vdc5_onoff_t vd_in_0, const vdc5_onoff_t vd_in_1)
{
    vdc5_error_t                    error;
    vdc5_init_t                     init;
    vdc5_input_t                    input;
    vdc5_sync_ctrl_t                sync_ctrl;
    vdc5_output_t                   output;
    const lcd_dependent_param_t   * param;

#if 0				//When it is enabled. CVBS In+ LVDS1280+OPenVG will be===> [Error]:eglCreateWindowSurface
    param = &lcd_dependent_param[ch];

    /* Initialization parameter */
    init.panel_icksel   = param->panel_icksel;      /* Panel clock select */
    init.panel_dcdr     = param->panel_dcdr;        /* Panel clock frequency division ratio */
    init.lvds           = GRAPHICS_GetLvdsParam(ch);/* LVDS parameter */
    /* Initialize */
    error = R_VDC5_Initialize(ch, &init, &init_func, (uint32_t)ch);
    if (error != VDC5_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
#endif

		error = VDC5_OK;
		
    if (error == VDC5_OK)
    {
        /* Input parameter */
        input.inp_sel   = VDC5_INPUT_SEL_VDEC;           /* Input select */
        input.inp_fh50  = (uint16_t)VSYNC_1_2_FH_TIMING; /* Vsync signal 1/2fH phase timing */
        input.inp_fh25  = (uint16_t)VSYNC_1_4_FH_TIMING; /* Vsync signal 1/4fH phase timing */
        input.dly       = NULL;                          /* Sync signal delay adjustment */
        input.ext_sig   = NULL;                          /* External input signal */
        if (vd_in_0 != VDC5_OFF)
        {   /* Video input 0ch */
            error = R_VDC5_VideoInput(VDC5_CHANNEL_0, &input);
            if (error != VDC5_OK)
            {
                GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
            }
        }
    }
    if (error == VDC5_OK)
    {
        if (vd_in_1 != VDC5_OFF)
        {   /* Video input 0ch */
            error = R_VDC5_VideoInput(VDC5_CHANNEL_1, &input);
            if (error != VDC5_OK)
            {
                GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
            }
        }
    }

    if (error == VDC5_OK)
    {
        /* Sync signal control */
        sync_ctrl.res_vs_sel    = VDC5_ON;          /* Vsync signal output select (free-running Vsync on/off control) */
            /* Sync signal output and full-screen enable signal select */
        sync_ctrl.res_vs_in_sel = VDC5_RES_VS_IN_SEL_SC0;
        sync_ctrl.res_fv        = param->res_fv;    /* Free-running Vsync period setting */
        sync_ctrl.res_fh        = param->res_fh;    /* Hsync period setting */
        sync_ctrl.res_vsdly     = (uint16_t)0u;     /* Vsync signal delay control */
            /* Full-screen enable control */
        sync_ctrl.res_f.vs      = param->res_f.vs;
        sync_ctrl.res_f.vw      = param->res_f.vw;
        sync_ctrl.res_f.hs      = param->res_f.hs;
        sync_ctrl.res_f.hw      = param->res_f.hw;
        sync_ctrl.vsync_cpmpe   = NULL;             /* Vsync signal compensation */
        /* Sync control */
        error = R_VDC5_SyncControl(ch, &sync_ctrl);
        if (error != VDC5_OK)
        {
            GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
        }
    }

    if (error == VDC5_OK)
    {
        /* Output parameter */
        output.tcon_half        = param->tcon_half;         /* TCON reference timing, 1/2fH timing */
        output.tcon_offset      = param->tcon_offset;       /* TCON reference timing, offset Hsync signal timing */
            /* LCD TCON timing setting */
        GRAPHICS_SetLcdTconSettings(ch, output.outctrl);
        output.outcnt_lcd_edge  = param->outcnt_lcd_edge;   /* Output phase control of LCD_DATA23 to LCD_DATA0 pin */
        output.out_endian_on    = VDC5_OFF;                 /* Bit endian change on/off control */
        output.out_swap_on      = VDC5_OFF;                 /* B/R signal swap on/off control */
        output.out_format       = param->out_format;        /* Output format select */
        output.out_frq_sel      = VDC5_LCD_PARALLEL_CLKFRQ_1;   /* Clock frequency control */
        output.out_dir_sel      = VDC5_LCD_SERIAL_SCAN_FORWARD; /* Scan direction select */
        output.out_phase        = VDC5_LCD_SERIAL_CLKPHASE_0;   /* Clock phase adjustment */
        output.bg_color         = (uint32_t)0x00000000u;    /* Background color in 24-bit RGB color format */
        /* Display output */
        error = R_VDC5_DisplayOutput(ch, &output);
        if (error != VDC5_OK)
        {
            GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
        }
    }
    return error;
}   /* End of function GRPDRV_Init() */

/**************************************************************************//**
 * @brief       Terminate graphics driver
 * @param[in]   ch
 * @retval      VDC5 driver error code
******************************************************************************/
vdc5_error_t GRPDRV_Term (const vdc5_channel_t ch)
{
    vdc5_error_t error;

    /* Terminate */
    error = R_VDC5_Terminate(ch, &quit_func, (uint32_t)ch);
    if (error != VDC5_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
    return error;
}   /* End of function GRPDRV_Term() */

/**************************************************************************//**
 * @brief       Create graphics surface
 * @param[in]   ch
 * @param[in]   layer_id
 * @param[in]   framebuff
 * @param[in]   fb_stride
 * @param[in]   gr_format
 * @param[in]   period_rect
 * @retval      VDC5 driver error code
******************************************************************************/
vdc5_error_t GRPDRV_GraphicsCreateSurface (
    const vdc5_channel_t                ch,
    const vdc5_layer_id_t               layer_id,
    void                        * const framebuff,
    const uint32_t                      fb_stride,
    const vdc5_gr_format_t              gr_format,
    const vdc5_period_rect_t    * const period_rect)
{
    vdc5_error_t    error;
    vdc5_read_t     read;

    /* Read data parameter */
    read.gr_ln_off_dir  = VDC5_GR_LN_OFF_DIR_INC;   /* Line offset address direction of the frame buffer */
    read.gr_flm_sel     = VDC5_GR_FLM_SEL_FLM_NUM;  /* Selects a frame buffer address setting signal */
    read.gr_imr_flm_inv = VDC5_OFF;                 /* Frame buffer number for distortion correction */
    read.gr_bst_md      = VDC5_BST_MD_32BYTE;       /* Frame buffer burst transfer mode */
    read.gr_base        = framebuff;                /* Frame buffer base address */
    read.gr_ln_off      = fb_stride;                /* Frame buffer line offset address */
    read.width_read_fb  = NULL;                     /* Width of the image read from frame buffer */
    read.adj_sel        = VDC5_OFF;                 /* Measures to decrease the influence
                                                       by folding pixels/lines (on/off) */
    read.gr_format      = gr_format;                /* Format of the frame buffer read signal */
    read.gr_ycc_swap    = VDC5_GR_YCCSWAP_CRY0CBY1;		//by Generallai  org is VDC5_GR_YCCSWAP_CBY0CRY1; /* Controls swapping of data read from buffer
                                                      // in the YCbCr422 format */
    read.gr_rdswa       = gr_rdswa_tbl[gr_format];  /* Frame buffer swap setting */
        /* Display area */
    read.gr_grc.vs      = period_rect->vs;
    read.gr_grc.vw      = period_rect->vw;
    read.gr_grc.hs      = period_rect->hs;
    read.gr_grc.hw      = period_rect->hw;
    /* Read data control */
    error = R_VDC5_ReadDataControl(ch, layer_id, &read);
    if (error != VDC5_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
    return error;
}   /* End of function GRPDRV_GraphicsCreateSurface() */

/**************************************************************************//**
 * @brief       Create video surface
 * @param[in]   ch
 * @param[in]   layer_id
 * @param[in]   framebuff
 * @param[in]   fb_stride
 * @param[in]   res_md
 * @param[in]   res
 * @param[in]   period_rect
 * @retval      VDC5 driver error code
******************************************************************************/
vdc5_error_t GRPDRV_VideoCreateSurface (
    const vdc5_channel_t                ch,
    const vdc5_layer_id_t               layer_id,
    void                        * const framebuff,
    const uint32_t                      fb_stride,
    const vdc5_res_md_t                 res_md,
    const vdc5_period_rect_t    * const res,
    const vdc5_period_rect_t    * const period_rect)    
{
    vdc5_error_t            error;
    vdc5_write_t            write;
    vdc5_sclingdown_rot_t * scldw_rot;
    vdc5_read_t             read;
    vdc5_width_read_fb_t    w_read_fb;
    vdc5_gr_format_t        gr_format_tmp;

    gr_format_tmp = gr_format_conv[res_md];
    /* Scaling-down and rotation parameter */
    scldw_rot = &write.sclingdown_rot;
 
    /* Image area to be captured */
 
    scldw_rot->res.vs   = (uint16_t)((uint32_t)res->vs - 1u);
    scldw_rot->res.vw   = res->vw;
    scldw_rot->res.hs   = res->hs;
    scldw_rot->res.hw   = res->hw;
 
    read.gr_grc.vs      = period_rect->vs;
    read.gr_grc.vw      = period_rect->vw;
    read.gr_grc.hs      = period_rect->hs;
    read.gr_grc.hw      = period_rect->hw;
 
    if ((uint32_t)res->vw <= (uint32_t)read.gr_grc.vw)
    {
        w_read_fb.in_vw = res->vw;
    }
    else
    {
        w_read_fb.in_vw = read.gr_grc.vw;
    }
 
    if ((uint32_t)res->hw <= (uint32_t)read.gr_grc.hw)
    {
        w_read_fb.in_hw = res->hw;
    }
    else
    {
        w_read_fb.in_hw = read.gr_grc.hw;
    }

        w_read_fb.in_vw = read.gr_grc.vw;
        w_read_fb.in_hw = read.gr_grc.hw;
 
        
    /* Write data parameter */
    scldw_rot->res_pfil_sel = VDC5_ON;              /* Prefilter mode select for brightness signals (on/off) */
    
    scldw_rot->res_out_vw   = w_read_fb.in_vw;			/*w_read_fb.in_vw;      Number of valid lines in vertical direction
                                                     		output by scaling-down control block */
    scldw_rot->res_out_hw   = w_read_fb.in_hw;			/*w_read_fb.in_hw;      Number of valid horizontal pixels
                                                       	output by scaling-down control block */
    scldw_rot->adj_sel      = VDC5_ON;              /* Measures to decrease the influence
                                                       by lack of last-input line (on/off) */
    scldw_rot->res_ds_wr_md = VDC5_WR_MD_NORMAL;		// VDC5_WR_MD_ROT_90DEG;			//VDC5_WR_MD_NORMAL;    /* Frame buffer writing mode */
    
    
    write.res_wrswa     = VDC5_WR_RD_WRSWA_NON;     /* Frame buffer swap setting */
    write.res_md        = res_md;                   /* Frame buffer video-signal writing format */
    write.res_bst_md    = VDC5_BST_MD_32BYTE;       /* Transfer burst length for frame buffer */
    write.res_inter     = VDC5_RES_INTER_INTERLACE; /* Field operating mode select */
    write.res_fs_rate   = VDC5_RES_FS_RATE_PER1;    /* Writing rate */
    write.res_fld_sel   = VDC5_RES_FLD_SEL_TOP;     /* Write field select */
    write.res_dth_on    = VDC5_ON;                  /* Dither correction on/off */
    write.base          = framebuff;                /* Frame buffer base address */
    write.ln_off        = fb_stride;                /* Frame buffer line offset address [byte] */
    write.flm_num       = (uint32_t)(BUFFER_NUM - 1u);/* Number of frames of buffer (res_flm_num + 1) */
                                                    /* Frame buffer frame offset address */
    write.flm_off       = fb_stride * (uint32_t)scldw_rot->res_out_vw;
    write.btm_base      = NULL;                     /* Frame buffer base address for bottom */
    
    
    /* Write data control */
    error = R_VDC5_WriteDataControl(ch, layer_id, &write);
    if (error != VDC5_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
    else
    {
        /* Read data parameter */
        read.gr_ln_off_dir  = VDC5_GR_LN_OFF_DIR_INC;   /* Line offset address direction of the frame buffer */
        read.gr_flm_sel     = VDC5_GR_FLM_SEL_POINTER_BUFF; /* Selects a frame buffer address setting signal */
        read.gr_imr_flm_inv = VDC5_OFF;                 /* Frame buffer number for distortion correction */
        read.gr_bst_md      = VDC5_BST_MD_32BYTE;       /* Frame buffer burst transfer mode */
        read.gr_base        = framebuff;                /* Frame buffer base address */
        read.gr_ln_off      = fb_stride;                /* Frame buffer line offset address */
        read.width_read_fb  = &w_read_fb;               /* Width of the image read from frame buffer */
        read.adj_sel        = VDC5_ON;                  /* Measures to decrease the influence
                                                           by folding pixels/lines (ON/OFF) */
        read.gr_format      = gr_format_tmp;            /* Format of the frame buffer read signal */
        read.gr_ycc_swap    = VDC5_GR_YCCSWAP_CRY0CBY1;			//by Generallai  org is VDC5_GR_YCCSWAP_CBY0CRY1; /* Controls swapping of data read from buffer
                                                           //in the YCbCr422 format */
        read.gr_rdswa       = VDC5_WR_RD_WRSWA_NON;
        /* Read data control */
        error = R_VDC5_ReadDataControl(
                    ch,
                    (layer_id == VDC5_LAYER_ID_0_WR) ? VDC5_LAYER_ID_0_RD : VDC5_LAYER_ID_1_RD,
                    &read);
        if (error != VDC5_OK)
        {
            GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
        }
    }
    return error;
}   /* End of function GRPDRV_VideoCreateSurface() */

/**************************************************************************//**
 * @brief       Destroy all surfaces
 * @param[in]   ch
 * @retval      VDC5 driver error code
******************************************************************************/
vdc5_error_t GRPDRV_DestroySurfaces (const vdc5_channel_t ch)
{
    vdc5_error_t error;

    /* Release data control */
    error = R_VDC5_ReleaseDataControl(ch, VDC5_LAYER_ID_ALL);
    if (error != VDC5_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
    return error;
}   /* End of function GRPDRV_DestroySurfaces() */

/**************************************************************************//**
 * @brief       Start all surfaces
 * @param[in]   ch
 * @param[in]   gr_disp_sel     : Graphics display mode
 * @retval      VDC5 driver error code
******************************************************************************/
vdc5_error_t GRPDRV_StartSurfaces (const vdc5_channel_t ch, const vdc5_gr_disp_sel_t * const gr_disp_sel)
{
    vdc5_error_t    error;
    vdc5_start_t    start;

    start.gr_disp_sel = gr_disp_sel;
    /* Start process */
    error = R_VDC5_StartProcess(ch, VDC5_LAYER_ID_ALL, &start);
    if (error != VDC5_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
        printf("R_VDC5_StartProcess--fail\n");
    }
    printf("R_VDC5_StartProcess finished\n");
    return error;
}   /* End of function GRPDRV_StartSurfaces() */

/**************************************************************************//**
 * @brief       Stop all surfaces
 * @param[in]   ch
 * @retval      VDC5 driver error code
******************************************************************************/
vdc5_error_t GRPDRV_StopSurfaces (const vdc5_channel_t ch)
{
    vdc5_error_t error;

    /* Stop process */
    error = R_VDC5_StopProcess(ch, VDC5_LAYER_ID_ALL);
    if (error != VDC5_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
    return error;
}   /* End of function GRPDRV_StopSurfaces() */

