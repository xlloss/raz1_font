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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : ostm.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Aragon Sample Program - OS timer device driver (Initialize process)
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "dev_drv.h"                
#include "ostm.h"           

/* ==== OSTM H/W ==== */
#define OSTM_CH_TOTAL       (2)

/******************************************************************************
Private global variables and functions
******************************************************************************/
struct st_ostm_n * ostm;

static void OSTM_Open (uint32_t channel);
static void OSTM_Close (uint32_t channel, uint32_t * count);

/******************************************************************************
* Function Name: R_OSTM_Init
* Description  :
* Arguments    : uint32_t channel
*              : uint32_t mode
*              : uint32_t cycle
* Return Value : DEVDRV_SUCCESS
*              : DEVDRV_ERROR
******************************************************************************/
int32_t R_OSTM_Init (uint32_t channel, uint32_t mode, uint32_t cycle)
{
    int32_t ret;

    if ((channel >= OSTM_CH_TOTAL) || (mode > OSTM_MODE_COMPARE))
    {
        return DEVDRV_ERROR;
    }

    switch (channel)
    {
        case DEVDRV_CH_0:
            ret = Userdef_OSTM0_Init(mode, cycle);
            printf("Userdef_OSTM0_Init\n");
        break;
        case DEVDRV_CH_1:
            ret = Userdef_OSTM1_Init(mode, cycle);
        break;
 
        default:
            ret = DEVDRV_ERROR;
        break;
    }
    
    return (ret);
}

/******************************************************************************
* Function Name: R_OSTM_Open
* Description  :
* Arguments    : int32_t channel
* Return Value : DEVDRV_SUCCESS
*              : DEVDRV_ERROR
******************************************************************************/
int32_t R_OSTM_Open (uint32_t channel)
{
    if (channel >= OSTM_CH_TOTAL)
    {
        return DEVDRV_ERROR;
    }

    switch (channel)
    {
        case DEVDRV_CH_0:
            OSTM_Open(channel);
            printf("OSTM_Open\n");
        break;
        case DEVDRV_CH_1:
            OSTM_Open(channel);
        break;
 
        default:
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_OSTM_Close
* Description  :
* Arguments    : uint32_t channel
* Return Value : DEVDRV_SUCCESS
*              : DEVDRV_ERROR
******************************************************************************/
int32_t R_OSTM_Close (uint32_t channel, uint32_t * count)
{
    if (channel >= OSTM_CH_TOTAL)
    {
    	return DEVDRV_ERROR;
    }

    switch (channel)
    {
        case DEVDRV_CH_0:
            OSTM_Close((uint32_t)&OSTM0, count);
        break;
        case DEVDRV_CH_1:
            OSTM_Close((uint32_t)&OSTM1, count);
        break;
 
        default:
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: int_ostm0_interrupt
* Description  :
* Arguments    : none
* Return Value : none
******************************************************************************/
int32_t R_OSTM_Interrupt (uint32_t channel)
{
    if (channel >= OSTM_CH_TOTAL)
    {
    	return DEVDRV_ERROR;
    }

    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_OSTM0_Int();
        break;
        case DEVDRV_CH_1:
            Userdef_OSTM1_Int();
        break;
 
        default:
        break;
    }

    return DEVDRV_SUCCESS;
}

/*******************************************************************************
* Function Name: OSTM_Open
* Description  : This function opens OSTM.
* Arguments    : uint8_t channel
* Return Value : none
*******************************************************************************/
static void OSTM_Open (uint32_t channel)
{
    if (0 == channel)
    {
        OSTM0.OSTMnTS |= 0x01U;
    }
    else
    {
        OSTM0.OSTMnTS |= 0x02U;
    }
}

/******************************************************************************
* Function Name: OSTM_Close
* Description  : This function closes OSTM.
* Arguments    : uint8_t channel
* Return Value : none
******************************************************************************/
static void OSTM_Close (uint32_t channel, uint32_t * count)
{
    if (0 == channel)
    {
        OSTM0.OSTMnTS |= 0x01U;
        (*count) = OSTM0.OSTMnCNT;
    }
    else if (1 == channel)
    {
        OSTM1.OSTMnTS |= 0x02U;
        (*count) = OSTM1.OSTMnCNT;
    }
    else
    {
        /* Wrong channel specified! */
    }

}

/* End of File */

