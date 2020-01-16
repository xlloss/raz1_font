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
* @file         lcd_lcd_kit_b01_ch1.c
* @version      0.11
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* @brief        LCD panel for vdc5 channel 1 function
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    <string.h>

#include    "r_typedefs.h"

#include    "r_vdc5.h"

#include    "iodefine.h"
#include    "lcd_panel.h"

#if     (LCD_VDC5_CH1_PANEL==2)

/******************************************************************************
Macro definitions
******************************************************************************/
/* Port 4 */
#define LCD_PORT4_2ND (0x5000u)
/* Port 5 */
#define LCD_PORT5_7TH (0x0600u)
#define LCD_PORT5_2TH (0x00FFu)
/* Port 2 */
#define LCD_PORT2_7TH (0xF000u)
#define LCD_PORT2_6TH (0x0F00u)

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
void GRAPHICS_SetLcdPanel_Ch1 (void)
{
    volatile uint32_t   dummy_read;
    uint32_t            reg_data;

    /*  LCD-KIT-B01 (RGB666)
            DE          : LCD1_TCON1            ... P4_14, 2nd alternative function
                        : LCD1_CLK              ... P4_12, 2nd alternative function
        LCD1_DATA
            R[5:0]      : LCD1_DATA[17:16]      ... P5_10 ~ P5_9, 7th alternative function
                        : LCD1_DATA[15:12]      ... P2_15 ~ P2_12, 7th alternative function
            G[5:0]      : LCD1_DATA[11:8]       ... P2_11 ~ P2_8, 6th alternative function
                        : LCD1_DATA[7:6]        ... P5_7 ~ P5_6, 2nd alternative function
            B[5:0]      : LCD1_DATA[5:0]        ... P5_5 ~ P5_0, 2nd alternative function
    */
    /* Port 4 */
    reg_data        = (uint32_t)GPIO.PMC4 & (uint32_t)~LCD_PORT4_2ND;
    GPIO.PMC4       = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PMC4;
    dummy_read      = reg_data;
    /* PFCAE4, PFCE4, PFC4 ... 2nd alternative function
       PIPC4, PMC4
       b14      : P4_14
       b12      : P4_12 */
    reg_data        = (uint32_t)GPIO.PFCAE4 & (uint32_t)~LCD_PORT4_2ND;
    GPIO.PFCAE4     = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PFCE4 & (uint32_t)~LCD_PORT4_2ND;
    GPIO.PFCE4      = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PFC4 | (uint32_t)LCD_PORT4_2ND;
    GPIO.PFC4       = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PIPC4 | (uint32_t)LCD_PORT4_2ND;
    GPIO.PIPC4      = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PMC4 | (uint32_t)LCD_PORT4_2ND;
    GPIO.PMC4       = (uint16_t)reg_data;

    /* Port 5 */
    reg_data        = (uint32_t)GPIO.PMC5 & (uint32_t)~(LCD_PORT5_7TH|LCD_PORT5_2TH);
    GPIO.PMC5       = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PMC5;
    dummy_read      = reg_data;
    /* PFCAE5, PFCE5, PFC5 ... 7th alternative function
       b10:b9   : P5_10 ~ P5_9 */
    reg_data        = (uint32_t)GPIO.PFCAE5 | (uint32_t)LCD_PORT5_7TH;
    GPIO.PFCAE5     = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PFCE5 | (uint32_t)LCD_PORT5_7TH;
    GPIO.PFCE5      = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PFC5 & (uint32_t)~LCD_PORT5_7TH;
    GPIO.PFC5       = (uint16_t)reg_data;
    /* PFCAE5, PFCE5, PFC5 ... 2nd alternative function
       b7:b0    : P5_7 ~ P5_0 */
    reg_data        = (uint32_t)GPIO.PFCAE5 & (uint32_t)~LCD_PORT5_2TH;
    GPIO.PFCAE5     = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PFCE5 & (uint32_t)~LCD_PORT5_2TH;
    GPIO.PFCE5      = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PFC5 | (uint32_t)LCD_PORT5_2TH;
    GPIO.PFC5       = (uint16_t)reg_data;
    /* PIPC5, PMC5
       b10:b9   : P5_10 ~ P5_9
       b7:b0    : P5_7 ~ P5_0 */
    reg_data        = (uint32_t)GPIO.PIPC5 | (uint32_t)(LCD_PORT5_7TH|LCD_PORT5_2TH);
    GPIO.PIPC5      = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PMC5 | (uint32_t)(LCD_PORT5_7TH|LCD_PORT5_2TH);
    GPIO.PMC5       = (uint16_t)reg_data;

    /* Port 2 */
    reg_data        = (uint32_t)GPIO.PMC2 & (uint32_t)~(LCD_PORT2_7TH|LCD_PORT2_6TH);
    GPIO.PMC2       = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PMC2;
    dummy_read      = reg_data;
    /* PFCAE2, PFCE2, PFC2 ... 7th alternative function
       b15:b12  : P2_15 ~ P2_12 */
    reg_data        = (uint32_t)GPIO.PFCAE2 | (uint32_t)LCD_PORT2_7TH;
    GPIO.PFCAE2     = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PFCE2 | (uint32_t)LCD_PORT2_7TH;
    GPIO.PFCE2      = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PFC2 & (uint32_t)~LCD_PORT2_7TH;
    GPIO.PFC2       = (uint16_t)reg_data;
    /* PFCAE2, PFCE2, PFC2 ... 6th alternative function
       b11:b8   : P2_11 ~ P2_8 */
    reg_data        = (uint32_t)GPIO.PFCAE2 | (uint32_t)LCD_PORT2_6TH;
    GPIO.PFCAE2     = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PFCE2 & (uint32_t)~LCD_PORT2_6TH;
    GPIO.PFCE2      = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PFC2 | (uint32_t)LCD_PORT2_6TH;
    GPIO.PFC2       = (uint16_t)reg_data;
    /* PIPC2, PMC2
       b15:b8   : P2_15 ~ P2_8 */
    reg_data        = (uint32_t)GPIO.PIPC2 | (uint32_t)(LCD_PORT2_7TH|LCD_PORT2_6TH);
    GPIO.PIPC2      = (uint16_t)reg_data;
    reg_data        = (uint32_t)GPIO.PMC2 | (uint32_t)(LCD_PORT2_7TH|LCD_PORT2_6TH);
    GPIO.PMC2       = (uint16_t)reg_data;

    /* TODO: Backlight LED
        I2C channel 3
        I2C slave address: b'100 0010
        Command: 0x03
        Data: 0x00 (default) - 0xFF
            0x00 ---   0%
            0x64 --- 100%
            0xFF --- 100% */

}   /* End of function GRAPHICS_SetLcdPanel_Ch1() */

/**************************************************************************//**
 * @brief       
 * @param[out]  outctrl
 * @retval      None
******************************************************************************/
void GRAPHICS_SetLcdTconSettings_Ch1 (const vdc5_lcd_tcon_timing_t * * const outctrl)
{
    /* LCD-KIT-B01 (RGB666), WVGA 800x480 */
    /* TCON timing setting, VE */
    static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_VE =
    {
        (uint16_t)(LCD_CH1_DISP_VS * FH_1_2_CYCLE),   /* Signal pulse start position */
        (uint16_t)(LCD_CH1_DISP_VW * FH_1_2_CYCLE),   /* Pulse width */
        VDC5_LCD_TCON_POLMD_NORMAL,
        VDC5_LCD_TCON_REFSEL_HSYNC,
        VDC5_SIG_POL_NOT_INVERTED,          /* Polarity inversion control of signal */
        VDC5_LCD_TCON_PIN_NON,              /* Output pin for LCD driving signal */
        LCD_CH1_OUT_EDGE                    /* Output phase control of signal */
    };
    /* TCON timing setting, HE */
    static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_HE =
    {
        (uint16_t)LCD_CH1_DISP_HS,          /* Signal pulse start position */
        (uint16_t)LCD_CH1_DISP_HW,          /* Pulse width */
        VDC5_LCD_TCON_POLMD_NORMAL,
        VDC5_LCD_TCON_REFSEL_HSYNC,         /* Signal operating reference select */
        VDC5_SIG_POL_NOT_INVERTED,          /* Polarity inversion control of signal */
        VDC5_LCD_TCON_PIN_NON,              /* Output pin for LCD driving signal */
        LCD_CH1_OUT_EDGE                    /* Output phase control of signal */
    };
    /* TCON timing setting, DE */
    static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_DE =
    {
        (uint16_t)0,
        (uint16_t)0,
        VDC5_LCD_TCON_POLMD_NORMAL,
        VDC5_LCD_TCON_REFSEL_HSYNC,
        VDC5_SIG_POL_NOT_INVERTED,          /* Polarity inversion control of signal */
        VDC5_LCD_TCON_PIN_1,                /* Output pin for LCD driving signal */
        LCD_CH1_OUT_EDGE                    /* Output phase control of signal */
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
}   /* End of function GRAPHICS_SetLcdTconSettings_Ch1() */

#endif  /* LCD_VDC5_CH1_PANEL==LCD_CH1_PANEL_LCD_KIT_B01 */

