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
* @file         lcd_lcd_kit_b01_ch0.c
* @version      0.11
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* @brief        LCD panel for vdc5 channel 0 function
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    <string.h>

#include    "r_typedefs.h"

#include    "r_vdc5.h"

#include    "iodefine.h"
#include    "lcd_panel.h"

#if     (LCD_VDC5_CH0_PANEL==2)

/******************************************************************************
Macro definitions
******************************************************************************/
/* Port 11 */
#define LCD_PORT11_5TH (0x90FFu)
/* Port 10 */
#define LCD_PORT10_5TH (0xFFC0u)

#define FH_1_2_CYCLE   (2u)

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
 * @param[in]   void
 * @retval      None
******************************************************************************/
void GRAPHICS_SetLcdPanel_Ch0 (void)
{
    volatile uint32_t   dummy_read;
    uint32_t            reg_data;

    /*  LCD-KIT-B01 (RGB666)
                        : LCD0_CLK              ... P11_15, 5th alternative function
            DE          : LCD0_TCON2            ... P11_12, 5th alternative function
        LCD0_DATA
            R[5:0]      : LCD0_DATA[17:12]      ... P10_6 ~ P10_11, 5th alternative function
            G[5:0]      : LCD0_DATA[11:8]       ... P10_12 ~ P10_15, 5th alternative function
                        : LCD0_DATA[7:6]        ... P11_0 ~ P11_1, 5th alternative function
            B[5:0]      : LCD0_DATA[5:0]        ... P11_2 ~ P11_7, 5th alternative function
    */
    /* Port 11 */
    reg_data        = (uint32_t)GPIO.PMC11 & (uint32_t)~LCD_PORT11_5TH;
    GPIO.PMC11      = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PMC11;
    dummy_read      = reg_data;
    /* PFCAE11, PFCE11, PFC11 ... 5th alternative function
       PIPC11, PMC11
       b15      : P11_15
       b12      : P11_12
       b7:b0    : P11_7 ~ P11_0 */
    reg_data        = (uint32_t)GPIO.PFCAE11 | (uint32_t)LCD_PORT11_5TH;
    GPIO.PFCAE11    = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PFCE11 & (uint32_t)~LCD_PORT11_5TH;
    GPIO.PFCE11     = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PFC11 & (uint32_t)~LCD_PORT11_5TH;
    GPIO.PFC11      = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PIPC11 | (uint32_t)LCD_PORT11_5TH;
    GPIO.PIPC11     = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PMC11 | (uint32_t)LCD_PORT11_5TH;
    GPIO.PMC11      = (uint16_t)reg_data;

    /* Port 10 */
    reg_data        = (uint32_t)GPIO.PMC10 & (uint32_t)~LCD_PORT10_5TH;
    GPIO.PMC10      = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PMC10;
    dummy_read      = reg_data;
    /* PFCAE10, PFCE10, PFC10 ... 5th alternative function
       PIPC10, PMC10
       b15:b6   : P10_15 ~ P10_6 */
    reg_data        = (uint32_t)GPIO.PFCAE10 | (uint32_t)LCD_PORT10_5TH;
    GPIO.PFCAE10    = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PFCE10 & (uint32_t)~LCD_PORT10_5TH;
    GPIO.PFCE10     = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PFC10 & (uint32_t)~LCD_PORT10_5TH;
    GPIO.PFC10      = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PIPC10 | (uint32_t)LCD_PORT10_5TH;
    GPIO.PIPC10     = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PMC10 | (uint32_t)LCD_PORT10_5TH;
    GPIO.PMC10      = (uint16_t)reg_data;

    /* TODO: Backlight LED
        I2C channel 0
        I2C slave address: b'100 0010
        Command: 0x03
        Data: 0x00 (default) - 0xFF
            0x00 ---   0%
            0x64 --- 100%
            0xFF --- 100% */

}   /* End of function GRAPHICS_SetLcdPanel_Ch0() */

/**************************************************************************//**
 * @brief       
 * @param[out]  outctrl
 * @retval      None
******************************************************************************/
void GRAPHICS_SetLcdTconSettings_Ch0 (const vdc5_lcd_tcon_timing_t * * const outctrl)
{
    /* LCD-KIT-B01 (RGB666), WVGA 800x480 */
    /* TCON timing setting, VE */
    static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_VE =
    {
        (uint16_t)(LCD_CH0_DISP_VS * FH_1_2_CYCLE),   /* Signal pulse start position */
        (uint16_t)(LCD_CH0_DISP_VW * FH_1_2_CYCLE),   /* Pulse width */
        VDC5_LCD_TCON_POLMD_NORMAL,
        VDC5_LCD_TCON_REFSEL_HSYNC,
        VDC5_SIG_POL_NOT_INVERTED,          /* Polarity inversion control of signal */
        VDC5_LCD_TCON_PIN_NON,              /* Output pin for LCD driving signal */
        LCD_CH0_OUT_EDGE                    /* Output phase control of signal */
    };
    /* TCON timing setting, HE */
    static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_HE =
    {
        (uint16_t)LCD_CH0_DISP_HS,          /* Signal pulse start position */
        (uint16_t)LCD_CH0_DISP_HW,          /* Pulse width */
        VDC5_LCD_TCON_POLMD_NORMAL,
        VDC5_LCD_TCON_REFSEL_HSYNC,         /* Signal operating reference select */
        VDC5_SIG_POL_NOT_INVERTED,          /* Polarity inversion control of signal */
        VDC5_LCD_TCON_PIN_NON,              /* Output pin for LCD driving signal */
        LCD_CH0_OUT_EDGE                    /* Output phase control of signal */
    };
    /* TCON timing setting, DE */
    static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_DE =
    {
        (uint16_t)0,
        (uint16_t)0,
        VDC5_LCD_TCON_POLMD_NORMAL,
        VDC5_LCD_TCON_REFSEL_HSYNC,
        VDC5_SIG_POL_NOT_INVERTED,          /* Polarity inversion control of signal */
        VDC5_LCD_TCON_PIN_2,                /* Output pin for LCD driving signal */
        LCD_CH0_OUT_EDGE                    /* Output phase control of signal */
    };

    if (outctrl != NULL)
    {
        outctrl[VDC5_LCD_TCONSIG_STVA_VS]   = NULL;                 /* STVA/VS: Vsync */
        outctrl[VDC5_LCD_TCONSIG_STVB_VE]   = &lcd_tcon_timing_VE;  /* STVB/VE: Not used */
        outctrl[VDC5_LCD_TCONSIG_STH_SP_HS] = NULL;                 /* STH/SP/HS: Hsync */
        outctrl[VDC5_LCD_TCONSIG_STB_LP_HE] = &lcd_tcon_timing_HE;  /* STB/LP/HE: Not used */
        outctrl[VDC5_LCD_TCONSIG_CPV_GCK]   = NULL;                 /* CPV/GCK: Not used */
        outctrl[VDC5_LCD_TCONSIG_POLA]      = NULL;                 /* POLA: Not used */
        outctrl[VDC5_LCD_TCONSIG_POLB]      = NULL;                 /* POLB: Not used */
        outctrl[VDC5_LCD_TCONSIG_DE]        = &lcd_tcon_timing_DE;  /* DE: Not used */
    }
}   /* End of function GRAPHICS_SetLcdTconSettings_Ch0() */

#endif  /* LCD_VDC5_CH0_PANEL==LCD_CH0_PANEL_LCD_KIT_B01 */

