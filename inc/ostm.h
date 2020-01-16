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
/******************************************************************************
* File Name     : ostm.h
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Aragon Sample Program - OS timer device driver header
******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "iodefine.h"

/* Multiple inclusion prevention macro */
#ifndef _DEVDRV_OSTM_H_
#define _DEVDRV_OSTM_H_


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#define OSTM_MODE_INTERVAL  (0)
#define OSTM_MODE_COMPARE   (1)

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
int32_t R_OSTM_Init (uint32_t channel, uint32_t mode, uint32_t cycle);
int32_t R_OSTM_Open (uint32_t channel);
int32_t R_OSTM_Close (uint32_t channel, uint32_t * count);
int32_t R_OSTM_Interrupt (uint32_t channel);

int32_t Userdef_OSTM0_Init_USB(uint32_t mode, uint32_t cycle);

int32_t Userdef_OSTM0_Init (uint32_t mode, uint32_t cycle);
int32_t Userdef_OSTM1_Init (uint32_t mode, uint32_t cycle);
void    Userdef_OSTM0_Int (void);
void    Userdef_OSTM1_Int (void);
void    Userdef_OSTM0_WaitInt (void);
void    Userdef_OSTM1_WaitInt (void);

void    Sample_OSTM0_Interrupt (uint32_t int_sense);
void    Sample_OSTM1_Interrupt (uint32_t int_sense);

/* _DEVDRV_OSTM_H_ */
#endif  

