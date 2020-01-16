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
/******************************************************************************
* File Name    : reg_bit_access.h
* $Rev: 729 $
* $Date:: 2014-06-11 17:44:40 +0900#$
* Description  :
******************************************************************************/
/* Guards against multiple inclusion */
#ifndef REG_BIT_ACCESS_H
#define REG_BIT_ACCESS_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct uint32_reg_bit_inf
{
    uint32_t bit_mask;
    uint8_t  bit_posi;
} uint32_reg_bit_inf_t;

typedef struct uint16_reg_bit_inf
{
    uint16_t bit_mask;
    uint8_t  bit_posi;
} uint16_reg_bit_inf_t;

typedef struct uint8_reg_bit_inf
{
    uint8_t bit_mask;
    uint8_t bit_posi;
} uint8_reg_bit_inf_t;

/******************************************************************************
Exported global variables
******************************************************************************/
uint8_t COMMON_Get8BitMask (const uint8_t reg, const uint8_t mask_bit);
void COMMON_Set8BitMask (volatile uint8_t * const reg, const uint8_t mask_bit);
uint32_t COMMON_Get32BitMask (const uint32_t reg, const uint32_t mask_bit);
uint32_t COMMON_Get32BitMaskRShift (const uint32_t reg, const uint32_reg_bit_inf_t * const reg_bit);
void COMMON_Set32BitMaskLShift (    volatile uint32_t * const reg,
                                    const uint32_reg_bit_inf_t * const reg_bit,
                                    const uint32_t value);
void COMMON_Set16BitMaskLShift (    volatile uint16_t * const reg,
                                    const uint16_reg_bit_inf_t * const reg_bit,
                                    const uint16_t value);
void COMMON_Set8BitMaskLShift ( volatile uint8_t * const reg,
                                const uint8_reg_bit_inf_t * const reg_bit,
                                const uint8_t value);


/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* REG_BIT_ACCESS_H */

/* End of File */
