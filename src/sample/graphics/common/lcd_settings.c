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
* @file         lcd_settings.c
* @version      0.11
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* @brief        VDC5 driver LCD panel settings sample
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    <string.h>

#include    "r_typedefs.h"

#include    "r_vdc5.h"

#include    "iodefine.h"
#include    "lcd_panel.h"


/******************************************************************************
Macro definitions
******************************************************************************/

#if (LCD_VDC5_CH0_PANEL == LCD_CH0_PANEL_ANALOG_RGB)

#define LVDSPLL_TST (16u)      /* LVDSPLL_TST (LVDS PLL internal parameters) */
#define NFD         (384u)    /* NFD */
#define NRD         (5u-1u)   /* NRD */
 
#elif (LCD_VDC5_CH0_PANEL == LCD_CH0_LVDS_1280x480)				//LVDS 1280x480

#define LVDSPLL_TST (16u)     /* LVDSPLL_TST (LVDS PLL internal parameters) */
#define NFD         (384u)    /* NFD */
#define NRD         (5u-1u)   /* NRD */

#else

#define LVDSPLL_TST (16u)      /* LVDSPLL_TST (LVDS PLL internal parameters) */
#define NFD         (384u)    /* NFD */
#define NRD         (5u-1u)   /* NRD */

#endif




/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

/**************************************************************************//**
 * @brief       
 * @param[in]   channel
 * @retval      None
******************************************************************************/
void GRAPHICS_SetLcdPanel (const vdc5_channel_t channel)
{
    if (channel == VDC5_CHANNEL_0)
    {
        GRAPHICS_SetLcdPanel_Ch0();
    }
    else
    {
        GRAPHICS_SetLcdPanel_Ch1();
    }
}   /* End of function GRAPHICS_SetLcdPanel() */

/**************************************************************************//**
 * @brief       
 * @param[in]   channel
 * @param[out]  outctrl
 * @retval      None
******************************************************************************/
void GRAPHICS_SetLcdTconSettings (const vdc5_channel_t channel, const vdc5_lcd_tcon_timing_t * * const outctrl)
{
    /* LCD TCON timing setting */
    if (channel == VDC5_CHANNEL_0)
    {
        GRAPHICS_SetLcdTconSettings_Ch0(outctrl);
    }
    else
    {
        GRAPHICS_SetLcdTconSettings_Ch1(outctrl);
    }
}   /* End of function GRAPHICS_SetLcdTconSettings() */

/**************************************************************************//**
 * @brief       Retrieve LVDS parameters
 * @param[in]   channel
 * @retval      vdc5_lvds_t *
******************************************************************************/
vdc5_lvds_t * GRAPHICS_GetLvdsParam (const vdc5_channel_t channel)
{
    static vdc5_lvds_t  lvds_param;
    vdc5_lvds_t       * p_lvds;

    p_lvds = (vdc5_lvds_t *)&lvds_param;
    /*  LVDS Clock
        ex)
        Input clock to the LVDS: P1, 66.67 MHz
        Output clock from the LVDS: 40.0 MHz
        NIDIV (Divider 1): 1/4
          FIN  = 66.67 / 4
               = 16.67 MHz
        NFD: 384
        NRD: 5
          FVCO = 16.67 * 384 / 5
               = 1280.0 MHz
        NOD: 8
          FOUT = 1280.0 / 8
               = 160.0 MHz
        NODIV (Divider 2): 1/4
          Output clock from the LVDS
               = 160.0 / 4
               = 40.0 MHz

        NIDIV   NODIV   NFD     NRD     NOD
        4       4       384     5       8       40.0 MHz, SVGA Signal 800 x 600 (60 Hz), LVDSPLL_TST = 8, 12, or 20
        4       4       312     5       4       65.0 MHz, XGA Signal 1024 x 768 (60 Hz), LVDSPLL_TST = 8 or 12
        4       4       290     6       8       25.175 MHz, VGA Signal 640 x 480 (60 Hz), LVDSPLL_TST = 8
        4       2        	?			?				?       83.46 MHz, XGA Signal 1280 x 800 (60 Hz), LVDSPLL_TST = 8
    
    */
    if (channel == VDC5_CHANNEL_0)
    {
#if	(LCD_VDC5_CH0_PANEL==1)
    /* 40.0 MHz (SVGA) */
        p_lvds->lvds_in_clk_sel  = VDC5_LVDS_INCLK_SEL_PERI; /* The clock input to frequency divider 1 */
        p_lvds->lvds_idiv_set    = VDC5_LVDS_NDIV_4;         /* NIDIV */
        p_lvds->lvdspll_tst      = (uint16_t)LVDSPLL_TST;    /* LVDSPLL_TST (LVDS PLL internal parameters) */
        p_lvds->lvds_odiv_set    = VDC5_LVDS_NDIV_4;         /* NODIV */
        p_lvds->lvds_vdc_sel     = channel;                  /* The channel in VDC5 */
        p_lvds->lvdspll_fd       = (uint16_t)NFD;            /* NFD */
        p_lvds->lvdspll_rd       = (uint16_t)NRD;            /* NRD */
        p_lvds->lvdspll_od       = VDC5_LVDS_PLL_NOD_8;      /* NOD */
        
 
#elif ( LCD_VDC5_CH0_PANEL==LCD_CH0_LVDS_1280x480)

            p_lvds->lvds_in_clk_sel  = VDC5_LVDS_INCLK_SEL_PERI; /* The clock input to frequency divider 1 */
            p_lvds->lvds_idiv_set    = VDC5_LVDS_NDIV_4;         /* NIDIV */
            p_lvds->lvdspll_tst      = (uint16_t)LVDSPLL_TST;    /* LVDSPLL_TST (LVDS PLL internal parameters) */
            p_lvds->lvds_odiv_set    = VDC5_LVDS_NDIV_4;         /* NODIV */
            p_lvds->lvds_vdc_sel     = channel;                  /* The channel in VDC5 */
            p_lvds->lvdspll_fd       = (uint16_t)86;            /* NFD */
            p_lvds->lvdspll_rd       = (uint16_t)0;            /* LVDSPLL_RD= NRD-1 */
            p_lvds->lvdspll_od       = VDC5_LVDS_PLL_NOD_4;      /* NOD */
#else

        p_lvds = NULL;
#endif
    }
    else
    {
#if     (LCD_VDC5_CH1_PANEL==1)
    /* 40.0 MHz (SVGA) */
        p_lvds->lvds_in_clk_sel  = VDC5_LVDS_INCLK_SEL_PERI; /* The clock input to frequency divider 1 */
        p_lvds->lvds_idiv_set    = VDC5_LVDS_NDIV_4;         /* NIDIV */
        p_lvds->lvdspll_tst      = (uint16_t)LVDSPLL_TST;    /* LVDSPLL_TST (LVDS PLL internal parameters) */
        p_lvds->lvds_odiv_set    = VDC5_LVDS_NDIV_4;         /* NODIV */
        p_lvds->lvds_vdc_sel     = channel;                  /* The channel in VDC5 */
        p_lvds->lvdspll_fd       = (uint16_t)NFD;            /* NFD */
        p_lvds->lvdspll_rd       = (uint16_t)NRD;            /* NRD */
        p_lvds->lvdspll_od       = VDC5_LVDS_PLL_NOD_8;      /* NOD */
#else
        p_lvds = NULL;
#endif
    }
    return p_lvds;
}   /* End of function GRAPHICS_GetLvdsParam() */

