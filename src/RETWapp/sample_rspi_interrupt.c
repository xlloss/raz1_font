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
* Copyright (C) 2012 - 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : sample_rspi_interrupt.c
* $Rev: 690 $
* $Date:: 2014-01-28 21:23:21 +0900#$
* Description : Renesas Serial Peripheral Interface(RSPI) sample program - Interrupt
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_rspi.h"            /* RSPI Driver header */
//#include "sample_rspi_wm8978.h"

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


/******************************************************************************
* Function Name: Sample_RSPI_Spti4_Interrupt
* Description  : This is the interrupt handler that runs when the RSPI channel 
*              : 4 transmit buffer empty interrupt is accepted. It performs the
*              : processing to notify that the RSPI channel 4 transmit buffer
*              : empty interrupt has been generated. 
*              : In the sample code, this function is registered as the INTC 
*              : handler by using the user-defined function Userdef_RSPI4_Init.
* Arguments    : uint32_t int_sense : Interrupt detection (Not used.)
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : None
******************************************************************************/
void Sample_RSPI_Spti4_Interrupt(uint32_t int_sense)
{
    (void)R_RSPI_SptiInterrupt(DEVDRV_CH_4);
}


/* End of File */

