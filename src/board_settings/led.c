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
*
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : led.c
* Description : LED setting
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "led.h"
#include "iodefine.h"
#include "rza_io_regrw.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/


/*******************************************************************************
* Function Name: led_init
* Description  : Executes initial setting for the PORT which is connected to  
*              : the LEDs on the CPU board.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void LedInit(void)
{
#if 1		//RSK
    /* P7_1 : LED0 */
    /* Mode : Port mode                          */
    /* Terminal output level : High level output */
    /* Port mode : Output mode                   */
    /* I/O control mode : Depends on PMnm bit    */
    rza_io_reg_write_16(&GPIO.PMC7,  0, GPIO_PMC7_PMC71_SHIFT,   GPIO_PMC7_PMC71);
    rza_io_reg_write_16(&GPIO.P7,    1, GPIO_P7_P71_SHIFT,       GPIO_P7_P71);
    rza_io_reg_write_16(&GPIO.PM7,   0, GPIO_PM7_PM71_SHIFT,     GPIO_PM7_PM71);
    rza_io_reg_write_16(&GPIO.PIPC7, 0, GPIO_PIPC7_PIPC71_SHIFT, GPIO_PIPC7_PIPC71);
#endif

#if 0
    /* ---- P6_12 : User LED for GR-Peach ---- */
    rza_io_reg_write_16(&GPIO.PMC6,  0, GPIO_PMC6_PMC612_SHIFT,   GPIO_PMC6_PMC612);
    rza_io_reg_write_16(&GPIO.P6,    1, GPIO_P6_P612_SHIFT,       GPIO_P6_P612);
    rza_io_reg_write_16(&GPIO.PM6,   0, GPIO_PM6_PM612_SHIFT,     GPIO_PM6_PM612);
    rza_io_reg_write_16(&GPIO.PIPC6, 0, GPIO_PIPC6_PIPC612_SHIFT, GPIO_PIPC6_PIPC612); 
#endif   
 
#if 0    
    /* ---- P4_10 : LED1 for Genmai---- */
    /* Port initialize */
    rza_io_reg_write_16(&GPIO.PIBC4, 0, GPIO_PIBC4_PIBC410_SHIFT, GPIO_PIBC4_PIBC410);
    rza_io_reg_write_16(&GPIO.PBDC4, 0, GPIO_PBDC4_PBDC410_SHIFT, GPIO_PBDC4_PBDC410);
    rza_io_reg_write_16(&GPIO.PM4,   1, GPIO_PM4_PM410_SHIFT,     GPIO_PM4_PM410);
    rza_io_reg_write_16(&GPIO.PMC4,  0, GPIO_PMC4_PMC410_SHIFT,   GPIO_PMC4_PMC410);
    rza_io_reg_write_16(&GPIO.PIPC4, 0, GPIO_PIPC4_PIPC410_SHIFT, GPIO_PIPC4_PIPC410);
    /* Mode : Port mode                          */
    /* Terminal output level : High level output */
    /* Port mode : Output mode                   */
    rza_io_reg_write_16(&GPIO.PBDC4, 0, GPIO_PBDC4_PBDC410_SHIFT, GPIO_PBDC4_PBDC410);
    rza_io_reg_write_16(&GPIO.P4,    1, GPIO_P4_P410_SHIFT,       GPIO_P4_P410);
    rza_io_reg_write_16(&GPIO.PM4,   0, GPIO_PM4_PM410_SHIFT,     GPIO_PM4_PM410);
#endif   
}


/*******************************************************************************
*For RSK
*******************************************************************************/
void LedOn(void)
{
    rza_io_reg_write_16(&GPIO.P7, 0, GPIO_P7_P71_SHIFT, GPIO_P7_P71);
}
/*******************************************************************************
* Function Name: led_off
* Description  : Turns off the LEDs on the CPU board.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void LedOff(void)
{
    rza_io_reg_write_16(&GPIO.P7, 1, GPIO_P7_P71_SHIFT, GPIO_P7_P71);
}

/*******************************************************************************
*For Genmai
*******************************************************************************/
void LedOn1(void)
{
    rza_io_reg_write_16(&GPIO.P4, 0, GPIO_P4_P410_SHIFT, GPIO_P4_P410);
}
 
void LedOff1(void)
{
    rza_io_reg_write_16(&GPIO.P4, 1, GPIO_P4_P410_SHIFT, GPIO_P4_P410);
}




/*******************************************************************************
*For GR-PEACH
*******************************************************************************/
void LedOn2(void)
{
    rza_io_reg_write_16(&GPIO.P6, 1, GPIO_P6_P612_SHIFT, GPIO_P6_P612);
}
 
void LedOff2(void) 
{
    rza_io_reg_write_16(&GPIO.P6, 0, GPIO_P6_P612_SHIFT, GPIO_P6_P612);
}


/* End of File */

