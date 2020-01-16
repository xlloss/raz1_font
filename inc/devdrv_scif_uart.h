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
/******************************************************************************
* File Name    : scif_uart.h
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* Description  : SCIF UART driver header
******************************************************************************/
#ifndef _DEVDRV_SCIF_UART_H_
#define _DEVDRV_SCIF_UART_H_

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/


/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== Arguments, return values ==== */
/* ---- Mode ---- */
#define SCIF_UART_MODE_W        (1)     /* Transfer         */
#define SCIF_UART_MODE_R        (2)     /* Receive          */
#define SCIF_UART_MODE_RW       (3)     /* Transfer/receive */

/* ==== CKS division setting (Select SCIF clock clock source) ==== */
#define SCIF_CKS_DIVISION_1     (0)     /* 1 division of P1 clock  */
#define SCIF_CKS_DIVISION_4     (1)     /* 4 division of P1 clock  */
#define SCIF_CKS_DIVISION_16    (2)     /* 16 division of P1 clock */
#define SCIF_CKS_DIVISION_64    (3)     /* 64 division of P1 clock */

/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
/* ==== API function ==== */
int32_t R_SCIF_UART_Init(uint32_t channel, uint32_t mode, uint16_t cks, uint8_t scbrr);

/* ==== User-defined function ==== */
void Userdef_SCIF2_UART_Init(uint8_t mode, uint16_t cks, uint8_t scbrr);


#endif  /* _DEVDRV_SCIF_UART_H_ */

/* End of File */
