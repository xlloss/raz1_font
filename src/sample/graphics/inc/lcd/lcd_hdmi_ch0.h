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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         lcd_analog_rgb_ch0.h
* @version      0.10
* @brief        LCD panel for vdc5 channel 0 definition header 
******************************************************************************/

#ifndef LCD_HDMI_RGB_CH0_H
#define LCD_HDMI_RGB_CH0_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    <stdlib.h>
#include    <stdio.h>

#include    "r_typedefs.h"

#include    "r_vdc5.h"

#if 0 //SRSR
//SRSR#define     LCD_CH0_DISP_HS         (120u)                      /* LCD display area size, horizontal start position */
#define     LCD_CH0_DISP_HS         (210u)                      /* LCD display area size, horizontal start position */
#define     LCD_CH0_DISP_HW         (800u)                      /* LCD display area size, horizontal width */
//SRSR#define     LCD_DISP_S_HSYNC        (868u)
//SRSR#define     LCD_DISP_W_HSYNC        (106u)

//SRSR#define     LCD_CH0_DISP_VS         (29u)                       /* LCD display area size, vertical start position */
//SRSR#define     LCD_CH0_DISP_VW         (600u)                      /* LCD display area size, height (vertical width) */
//SRSR#define     LCD_CH0_DISP_VS         (23u)                       /* LCD display area size, vertical start position */
#define     LCD_CH0_DISP_VS         (22u)                       /* LCD display area size, vertical start position */
#define     LCD_CH0_DISP_VW         (480u)                      /* LCD display area size, height (vertical width) */

//SRSR#define     LCD_DISP_S_VSYNC        (0u)
//SRSR#define     LCD_DISP_W_VSYNC        (8u)

//SRSR#define     LCD_CH0_SIG_FV          (631u - 1u)                 /* Free-running Vsync period */
//SRSR#define     LCD_CH0_SIG_FH          (1000u - 1u)                /* Hsync period */
#define     LCD_CH0_SIG_FV          (525u - 1u)                 /* Free-running Vsync period */
#define     LCD_CH0_SIG_FH          (1056u - 1u)                /* Hsync period */


#else //SRSR


#define     LCD_CH0_DISP_HS         (120u)                      /* LCD display area size, horizontal start position */
#define     LCD_CH0_DISP_HW         (800u)                      /* LCD display area size, horizontal width */
#define     LCD_DISP_S_HSYNC        (868u)
#define     LCD_DISP_W_HSYNC        (106u)

#define     LCD_CH0_DISP_VS         (29u)                       /* LCD display area size, vertical start position */
//SRSR#define     LCD_CH0_DISP_VW         (600u)                      /* LCD display area size, height (vertical width) */
#define     LCD_CH0_DISP_VW         (480u)                      /* LCD display area size, height (vertical width) */

#define     LCD_DISP_S_VSYNC        (0u)
#define     LCD_DISP_W_VSYNC        (8u)

//SRSR#define     LCD_CH0_SIG_FV          (631u - 1u)                 /* Free-running Vsync period */
//SRSR#define     LCD_CH0_SIG_FH          (1000u - 1u)                /* Hsync period */
#define     LCD_CH0_SIG_FV          (525u - 1u)                 /* Free-running Vsync period */
#define     LCD_CH0_SIG_FH          (1056u - 1u)                /* Hsync period */
#endif //SRSR




#define     LCD_CH0_TCON_HALF       (LCD_CH0_SIG_FH / 2u)       /* TCON reference timing, 1/2fH timing */
#define     LCD_CH0_TCON_OFFSET     (0u)                        /* TCON reference timing, offset Hsync signal timing */

#define     LCD_CH0_PANEL_CLK       VDC5_PANEL_ICKSEL_PERI      /* Panel clock select */
#define     LCD_CH0_PANEL_CLK_DIV   VDC5_PANEL_CLKDIV_1_2       /* Panel clock frequency division ratio */
#define     LCD_CH0_OUT_EDGE        VDC5_EDGE_RISING            /* Output phase control of LCD_DATA[23:0] signal */
#define     LCD_CH0_OUT_FORMAT      VDC5_LCD_OUTFORMAT_RGB888   /* GenerallaiModify RSK2Genmai LCD output format select */					
//#define     LCD_CH0_OUT_FORMAT      VDC5_LCD_OUTFORMAT_RGB666						/* RGB666 is used for Genmai */			
/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
void GRAPHICS_SetLcdPanel_Ch0(void);
void GRAPHICS_SetLcdTconSettings_Ch0(const vdc5_lcd_tcon_timing_t * * outctrl);

#endif  /* LCD_HDMI_RGB_CH0_H */
