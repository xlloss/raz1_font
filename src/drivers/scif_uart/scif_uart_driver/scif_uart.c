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
* File Name   : scif_uart.c
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* Description : SCIF UART driver
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_scif_uart.h"       /* UART Driver header */
#include "iodefine.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== UART H/W specification ==== */
#define SCIF_UART_CH_TOTAL      (8)     /* The number of UART channels */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* ====  Prototype function ==== */

/* ==== Global variable ==== */


/******************************************************************************
* Function Name: R_SCIF_UART_Init
* Description  : Initializes the UART function of the SCIF channel specified by
*              : the argument channel. Specifies the mode for transmission, 
*              : reception, or transmission/reception to the argument mode.
*              : The values to be set to the CKS bit of the serial mode register 
*              : (SCSMR) and the bit rate register (SCBRR) are specified to the
*              : argument csk and scbrr according to the UART baud rate used in 
*              : the system.
* Arguments    : uint32_t channel : SCIF channel (0-7)
*              : uint32_t mode    : Mode
*              :                  :   SCIF_UART_MODE_W  : Transmit
*              :                  :   SCIF_UART_MODE_R  : Receive
*              :                  :   SCIF_UART_MODE_RW : Transmit/receive
*              : uint16_t cks     : Select SCIF clock source
*              :                  : (Setting value of SCSMR CKS bit)
*              :                  :   SCIF_CKS_DIVISION_1  : P1 clock
*              :                  :   SCIF_CKS_DIVISION_4  : P1 clock / 4
*              :                  :   SCIF_CKS_DIVISION_16 : P1 clock / 16
*              :                  :   SCIF_CKS_DIVISION_64 : P1 clock / 64
*              : uint8_t scbrr    : Setting value of SCIF bit rate register (SCBRR)
* Return Value : DEVDRV_SUCCESS   : Success
*              : DEVDRV_ERROR     : Error
******************************************************************************/
int32_t R_SCIF_UART_Init(uint32_t channel, uint32_t mode, uint16_t cks, uint8_t scbrr)
{
    /* ==== Argument check ==== */
    if ((channel >= SCIF_UART_CH_TOTAL) || (mode < SCIF_UART_MODE_W) || 
        (mode > SCIF_UART_MODE_RW) || (cks > SCIF_CKS_DIVISION_64))
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ===== Initialization of variables ==== */

    /* ===== UART initialization ===== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_SCIF0_UART_Init(mode, cks, scbrr);
        break;
        
        case DEVDRV_CH_1:
 
            Userdef_SCIF1_UART_Init(mode, cks, scbrr);
        break;
        
        case DEVDRV_CH_2:
            Userdef_SCIF2_UART_Init(mode, cks, scbrr);
        break;
 
        default:
            /* Do Nothing */
        break;
    }

    return DEVDRV_SUCCESS;
}


/* End of File */

