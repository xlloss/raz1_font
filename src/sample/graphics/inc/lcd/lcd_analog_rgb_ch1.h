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
* @file         lcd_analog_rgb_ch1.h
* @version      0.11
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* @brief        LCD panel for vdc5 channel 1 definition header
******************************************************************************/

#ifndef LCD_ANALOG_RGB_CH1_H
#define LCD_ANALOG_RGB_CH1_H

#ifndef LCD_PANEL_H
    #error  Do not include this file directly!
#else
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    <stdlib.h>

#include    "r_typedefs.h"

#include    "r_vdc5.h"

#include    "lcd_analog_rgb.h"


/******************************************************************************
Macro definitions
******************************************************************************/
/* Option board (part number: RTK7721000B00000BR)
    ADV7123 (Video DAC), U10
    Analog RGB D-sub15 (RGB888), J16 */
#define     LCD_CH1_S_HSYNC         (0u)                        /* Hsync start position */
#define     LCD_CH1_W_HSYNC         (LCD_SVGA_H_SYNC_WIDTH)     /* Hsync width */
#define     LCD_CH1_POL_HSYNC       (LCD_SVGA_H_POLARITY)       /* Polarity of Hsync pulse */
                                                                /* LCD display area size, horizontal start position */
#define     LCD_CH1_DISP_HS         (LCD_SVGA_H_SYNC_WIDTH + LCD_SVGA_H_BACK_PORCH)
#define     LCD_CH1_DISP_HW         (LCD_SVGA_H_VISIBLE_AREA)   /* LCD display area size, horizontal width */

#define     LCD_CH1_S_VSYNC         (0u)                        /* Vsync start position */
#define     LCD_CH1_W_VSYNC         (LCD_SVGA_V_SYNC_WIDTH)     /* Vsync width */
#define     LCD_CH1_POL_VSYNC       (LCD_SVGA_V_POLARITY)       /* Polarity of Vsync pulse */
                                                                /* LCD display area size, vertical start position */
#define     LCD_CH1_DISP_VS         (LCD_SVGA_V_SYNC_WIDTH + LCD_SVGA_V_BACK_PORCH)
#define     LCD_CH1_DISP_VW         (LCD_SVGA_V_VISIBLE_AREA)   /* LCD display area size, height (vertical width) */

#define     LCD_CH1_SIG_FV          (LCD_SVGA_V_TOTAL - 1u)     /* Free-running Vsync period */
#define     LCD_CH1_SIG_FH          (LCD_SVGA_H_TOTAL - 1u)     /* Hsync period */
    /* Pixel data is latched in the rising edge of pixel clock on ADV7123.
       Therefore, pixel data should be output from VDC5 at the falling edge of the clock. */
#define     LCD_CH1_OUT_EDGE        VDC5_EDGE_FALLING           /* Output phase control of LCD_DATA[23:0] signal */
#define     LCD_CH1_OUT_FORMAT      VDC5_LCD_OUTFORMAT_RGB888   /* LCD output format select */

#define     LCD_CH1_PANEL_CLK       VDC5_PANEL_ICKSEL_LVDS      /* Panel clock select */
#define     LCD_CH1_PANEL_CLK_DIV   VDC5_PANEL_CLKDIV_1_1       /* Panel clock frequency division ratio */

#define     LCD_CH1_TCON_HALF       (LCD_CH1_SIG_FH / 2u)       /* TCON reference timing, 1/2fH timing */
#define     LCD_CH1_TCON_OFFSET     (0u)                        /* TCON reference timing, offset Hsync signal timing */


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
void GRAPHICS_SetLcdPanel_Ch1(void);
void GRAPHICS_SetLcdTconSettings_Ch1(const vdc5_lcd_tcon_timing_t * * const outctrl);


#endif  /* LCD_PANEL_H not defined */
#endif  /* LCD_ANALOG_RGB_CH1_H */
