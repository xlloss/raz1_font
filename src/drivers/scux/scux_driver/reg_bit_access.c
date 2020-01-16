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
* File Name   : reg_bit_access.c
* $Rev: 729 $
* $Date:: 2014-06-11 17:44:40 +0900#$
* Description  : Register bit access functions
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "reg_bit_access.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/* The mask bit for the integral promotion */
#define INTEGRAL_PROMO_MASK_16BIT   (0x0000FFFFu)   /* For uint16 */

/* The mask bit for the integral promotion */
#define INTEGRAL_PROMO_MASK_8BIT    (0x000000FFu)   /* For uint8 */

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
* Function Name: COMMON_Get8BitMask
* Description  :
* Arguments    : reg -
*                    Value of unsigned 8bit register
*                mask_bit -
*                    Mask bit
* Return Value :  Masked value
******************************************************************************/
uint8_t COMMON_Get8BitMask (const uint8_t reg, const uint8_t mask_bit)
{
    return (uint8_t)((uint32_t)reg & (uint32_t)mask_bit);
}

/******************************************************************************
* Function Name: COMMON_Get8BitMask
* Description  :
* Arguments    : reg -
*                    Value of unsigned 8bit register
*                mask_bit -
*                    Mask bit
* Return Value :  Masked value
******************************************************************************/
void COMMON_Set8BitMask (volatile uint8_t * const reg, const uint8_t mask_bit)
{
    uint32_t reg_temp;

    if (NULL == reg)
    {
        /* Do Nothing */
    }
    else
    {
        reg_temp = (uint32_t)*reg | (uint32_t)mask_bit;
        *reg = (uint8_t)reg_temp;
    }
}

/******************************************************************************
* Function Name: COMMON_Get32BitMask
* Description  :
* Arguments    : reg -
*                    Value of 32bit register
*                mask_bit -
*                    Mask bit
* Return Value :  Masked value
******************************************************************************/
uint32_t COMMON_Get32BitMask (const uint32_t reg, const uint32_t mask_bit)
{
    return (reg & mask_bit);
}

/******************************************************************************
* Function Name: GetUint32BitMaskRShift
* Description  :
* Arguments    :
* Return Value :
******************************************************************************/
uint32_t COMMON_Get32BitMaskRShift (const uint32_t reg, const uint32_reg_bit_inf_t  * const reg_bit)
{
    uint32_t ret;

    if (NULL == reg_bit)
    {
        ret = reg;
    }
    else
    {
        ret = (reg & reg_bit->bit_mask) >> (uint32_t)reg_bit->bit_posi;
    }

    return ret;
}

/******************************************************************************
* Function Name: COMMON_Set32BitMaskLShift
* Description  :
* Arguments    : * reg -
*                    Pointer of 32bit register
*                reg_bit -
*                    Mask bit and Position of target bit
*                value -
*                    Value to set in a bit
* Return Value : none
******************************************************************************/
void COMMON_Set32BitMaskLShift (    volatile uint32_t * const reg,
                                    const uint32_reg_bit_inf_t * const reg_bit,
                                    const uint32_t value)
{
    uint32_t reg_temp;

    if ((NULL == reg) || (NULL == reg_bit))
    {
        /* Do Nothing */
    }
    else
    {
        reg_temp = *reg;
        reg_temp &= ~reg_bit->bit_mask;
        reg_temp |= (value << (uint32_t)reg_bit->bit_posi);
        *reg = reg_temp;
    }
}

/******************************************************************************
* Function Name: COMMON_Set16BitMaskLShift
* Description  :
* Arguments    : * reg -
*                    Pointer of 16bit register
*                reg_bit -
*                    Mask bit and Position of target bit
*                value -
*                    Value to set in a bit
* Return Value : none
******************************************************************************/
void COMMON_Set16BitMaskLShift (    volatile uint16_t * const reg,
                                    const uint16_reg_bit_inf_t * const reg_bit,
                                    const uint16_t value)
{
    uint32_t reg_temp;

    if ((NULL == reg) || (NULL == reg_bit))
    {
        /* Do Nothing */
    }
    else
    {
        reg_temp = *reg;
        reg_temp &= ~(uint32_t)reg_bit->bit_mask;
        reg_temp |= ((uint32_t)value << (uint32_t)reg_bit->bit_posi);
        *reg = (uint16_t)reg_temp;
    }
}

/******************************************************************************
* Function Name: COMMON_Set8BitMaskLShift
* Description  :
* Arguments    : * reg -
*                    Pointer of 8bit register
*                reg_bit -
*                    Mask bit and Position of target bit
*                value -
*                    Value to set in a bit
* Return Value : none
******************************************************************************/
void COMMON_Set8BitMaskLShift ( volatile uint8_t * const reg,
                                const uint8_reg_bit_inf_t * const reg_bit,
                                const uint8_t value)
{
    uint32_t reg_temp;

    if ((NULL == reg) || (NULL == reg_bit))
    {
        /* Do Nothing */
    }
    else
    {
        reg_temp = *reg;
        reg_temp &= ~(uint32_t)reg_bit->bit_mask;
        reg_temp |= ((uint32_t)value << (uint32_t)reg_bit->bit_posi);
        *reg = (uint8_t)reg_temp;
    }
}

/* End of File */
