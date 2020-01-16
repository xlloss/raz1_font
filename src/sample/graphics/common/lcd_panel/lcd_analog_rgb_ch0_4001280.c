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
* @file         lcd_analog_rgb_ch0.c
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

#if (LCD_VDC5_CH0_PANEL == LCD_CH0_PANEL_ANALOG_RGB_400x1280)				//if output is 1280x800

/******************************************************************************
Macro definitions
******************************************************************************/
/* Port 11 */
#define LCD_PORT11_5TH (0xE0FFu)
/* Port 10 */
#define LCD_PORT10_5TH (0xFFFFu)

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
    /* Analog RGB D-sub15 (RGB888), SVGA signal 800x600 */
    /* TCON timing setting, VS */
    static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_VS =
    {
        (uint16_t)(LCD_CH0_S_VSYNC ),   /* Signal pulse start position */
        (uint16_t)(LCD_CH0_W_VSYNC ),   /* Pulse width */
        VDC5_LCD_TCON_POLMD_NORMAL,
        VDC5_LCD_TCON_REFSEL_HSYNC,
        LCD_CH0_POL_VSYNC,                  /* Polarity inversion control of signal */
        VDC5_LCD_TCON_PIN_4,                /* Output pin for LCD driving signal */
        LCD_CH0_OUT_EDGE                    /* Output phase control of signal */
    };

		/* TCON timing setting, VE - none */
		static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_VE =
		{
		    ( uint16_t )( LCD_CH0_DISP_VS  ),      /* Signal pulse start position */
		    ( uint16_t )( LCD_CH0_DISP_VW  ),      /* Pulse width */
		    VDC5_LCD_TCON_POLMD_NORMAL,         	/* Signal generation mode select */
		    VDC5_LCD_TCON_REFSEL_HSYNC,
		    VDC5_SIG_POL_NOT_INVERTED,
		    VDC5_LCD_TCON_PIN_NON,              	/* Output pin for LCD driving signal */
		    VDC5_EDGE_RISING                    	/* Output phase control of signal */
		} ;
		
		
    /* TCON timing setting, HS */
    static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_HS =
    {
        (uint16_t)LCD_CH0_S_HSYNC,          /* Signal pulse start position */
        (uint16_t)LCD_CH0_W_HSYNC,          /* Pulse width */
        VDC5_LCD_TCON_POLMD_NORMAL,
        VDC5_LCD_TCON_REFSEL_HSYNC,         /* Signal operating reference select */
        LCD_CH0_POL_HSYNC,                  /* Polarity inversion control of signal */
        VDC5_LCD_TCON_PIN_3,                /* Output pin for LCD driving signal */
        LCD_CH0_OUT_EDGE                    /* Output phase control of signal */
    };

		/* TCON timing setting, HE - none */
		static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_HE =
		{
		    ( uint16_t )LCD_CH0_DISP_HS,              /* Signal pulse start position */
		    ( uint16_t )LCD_CH0_DISP_HW,              /* Pulse width */
		    VDC5_LCD_TCON_POLMD_NORMAL,         /* Signal generation mode select */
		    ( uint16_t )0,                        /* Signal operating reference select */
		    ( uint16_t )0,                        /* Polarity inversion control of signal */
		    VDC5_LCD_TCON_PIN_NON,              /* Output pin for LCD driving signal */
		    VDC5_EDGE_RISING                    /* Output phase control of signal */
		} ;

		/* TCON timing setting, DE */
		static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_DE_ =
		{
        (uint16_t)(LCD_CH0_DISP_HS   ),   /* Signal pulse start position */
        (uint16_t)(LCD_CH0_DISP_HW   ),   /* Pulse width */
		    VDC5_LCD_TCON_POLMD_NORMAL,
		    VDC5_LCD_TCON_REFSEL_HSYNC,
		    VDC5_SIG_POL_NOT_INVERTED,
		    VDC5_LCD_TCON_PIN_2,
		    VDC5_EDGE_RISING
		};
		
		
/**************************************************************************//**
 * @brief       
 * @param[in]   void
 * @retval      None
******************************************************************************/
void GRAPHICS_SetLcdPanel_Ch0 (void)
{
    volatile uint16_t dummy_read;
    volatile uint16_t dummy_read10;
    volatile uint16_t dummy_read11;
	uint8_t  data[0x2];

     /* Port 11 */
    GPIO.PMC11     &= (uint16_t)~0x9CFFu;
    dummy_read      = GPIO.PMC11;
    /* PFCAE11, PFCE11, PFC11 ... 5th alternative function
       PIPC11, PMC11
       b15:b13  : P11_15 ~ P11_13
       b7:b0    : P11_7 ~ P11_0 */
    GPIO.PFCAE11   |= (uint16_t)0x9CFFu;
    GPIO.PFCE11    &= (uint16_t)~0x9CFFu;
    GPIO.PFC11     &= (uint16_t)~0x9CFFu;
    GPIO.PIPC11    |= (uint16_t)0x9CFFu;
    GPIO.PMC11     |= (uint16_t)0x9CFFu;

    dummy_read11      = GPIO.PMC11;

    /* Port 10 */
    GPIO.PMC10     &= (uint16_t)~0xFFFFu;
    dummy_read      = GPIO.PMC10;
    /* PFCAE10, PFCE10, PFC10 ... 5th alternative function
       PIPC10, PMC10
       b15:b0   : P10_15 ~ P10_0 */
    GPIO.PFCAE10   |= (uint16_t)0xFFFFu;
    GPIO.PFCE10    &= (uint16_t)~0xFFFFu;
    GPIO.PFC10     &= (uint16_t)~0xFFFFu;
    GPIO.PIPC10    |= (uint16_t)0xFFFFu;
    GPIO.PMC10     |= (uint16_t)0xFFFFu;

    dummy_read10      = GPIO.PMC10;

//    printf("LCD Display - 800x480 pixels\n");

    dummy_read10      = GPIO.PMC10;

#if 0					//RETW Generallai
    R_RIIC_rza1h_rsk_read(DEVDRV_CH_0, RZA1H_APP_I2C_TFP410, 0x08, 0x2, data);

	data[0] = 0xbf;

	R_RIIC_rza1h_rsk_write(DEVDRV_CH_0, RZA1H_APP_I2C_TFP410, 0x08, 0x2, data);

	data[0] = 0x00;

	R_RIIC_rza1h_rsk_read(DEVDRV_CH_0, RZA1H_APP_I2C_TFP410, 0x08, 0x2, data);

	data[0] = 0x00;
#endif
}   /* End of function GRAPHICS_SetLcdPanel_Ch0() */

/**************************************************************************//**
 * @brief       
 * @param[out]  outctrl
 * @retval      None
******************************************************************************/
void GRAPHICS_SetLcdTconSettings_Ch0 (const vdc5_lcd_tcon_timing_t * * const outctrl)
{
 
    if (outctrl != NULL)
    {
        outctrl[VDC5_LCD_TCONSIG_STVA_VS]   = &lcd_tcon_timing_VS; 				 /* STVA/VS: Vsync */
        outctrl[VDC5_LCD_TCONSIG_STVB_VE]   = NULL;												//&lcd_tcon_timing_VE;   /* STVB/VE:  */  						/* STVB/VE: Not used */
        outctrl[VDC5_LCD_TCONSIG_STH_SP_HS] = &lcd_tcon_timing_HS;  			/* STH/SP/HS: Hsync */
        outctrl[VDC5_LCD_TCONSIG_STB_LP_HE] = NULL;												//&lcd_tcon_timing_HE;   /* STB/LP/HE: */             /* STB/LP/HE: Not used */
        outctrl[VDC5_LCD_TCONSIG_CPV_GCK]   = NULL;                 /* CPV/GCK: Not used */
        outctrl[VDC5_LCD_TCONSIG_POLA]      = NULL;                 /* POLA: Not used */
        outctrl[VDC5_LCD_TCONSIG_POLB]      = NULL;                 /* POLB: Not used */
        outctrl[VDC5_LCD_TCONSIG_DE]        = NULL;									//&lcd_tcon_timing_DE_;  /* DE: Not used */
    }
}   /* End of function GRAPHICS_SetLcdTconSettings_Ch0() */

#endif  /* LCD_VDC5_CH0_PANEL==LCD_CH0_PANEL_ANALOG_RGB_1280x800 */
 