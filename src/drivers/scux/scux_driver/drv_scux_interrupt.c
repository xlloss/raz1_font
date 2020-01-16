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
* File Name   : drv_scux_interrupt.c
* $Rev: 729 $
* $Date:: 2014-06-11 17:44:40 +0900#$
* Description  : SCUX Interrupt Functions
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "drv_scux_register.h"
#include "drv_scux.h"
#include "devdrv_intc.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/* Event Clear Register Mask */
#define SEVCR_REG_MASK      (0xC0004000u)
#define DEVCR_REG_MASK      (0xF0008000u)
#define UEVCR_REG_MASK      (0xE0008000u)
#define VEVCR_REG_MASK      (0xFF008000u)

/******************************************************************************
Typedef definitions
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
/* ==== Prototype declaration ==== */
static void R_SCUX_SCUAI0_isr (uint32_t int_sense);
static void R_SCUX_SCUAI1_isr (uint32_t int_sense);
static void R_SCUX_SCUFDI0_isr (uint32_t int_sense);
static void R_SCUX_SCUFDI1_isr (uint32_t int_sense);
static void R_SCUX_SCUFDI2_isr (uint32_t int_sense);
static void R_SCUX_SCUFDI3_isr (uint32_t int_sense);
static void R_SCUX_SCUFUI0_isr (uint32_t int_sense);
static void R_SCUX_SCUFUI1_isr (uint32_t int_sense);
static void R_SCUX_SCUFUI2_isr (uint32_t int_sense);
static void R_SCUX_SCUFUI3_isr (uint32_t int_sense);
static void R_SCUX_SCUDVI0_isr (uint32_t int_sense);
static void R_SCUX_SCUDVI1_isr (uint32_t int_sense);
static void R_SCUX_SCUDVI2_isr (uint32_t int_sense);
static void R_SCUX_SCUDVI3_isr (uint32_t int_sense);
static void SCUAI_isr (const uint32_t ch);
static void SCUFDI_isr (const uint32_t ch);
static void SCUFUI_isr (const uint32_t ch);
static void SCUDVI_isr (const uint32_t ch);

/* ==== Global variables ==== */


static void (* scux_int_callback[NUM_SCUX_CH][NUM_SCUX_INT_ID_MODULE])(const uint32_t int_req);


/*******************************************************************************
* Function Name: R_SCUX_GetInterruptFunc
* Description  : Get Interrupt function
* Arguments    : ch -
*                    SCUX channel
*                module -
*                    SCUX Module
* Return Value : Interrupt function address
*******************************************************************************/
void (* R_SCUX_GetIntFunc (const uint32_t ch, const uint32_t module))(uint32_t int_sense)
{
    /* SCUX Interrupt function table */
    static void (* const scux_int_func_tbl[NUM_SCUX_CH][NUM_SCUX_INT_ID_MODULE])(uint32_t int_sense) =
    {
            { &R_SCUX_SCUAI0_isr,   &R_SCUX_SCUFDI0_isr,    &R_SCUX_SCUFUI0_isr,    &R_SCUX_SCUDVI0_isr }
        ,   { &R_SCUX_SCUAI0_isr,   &R_SCUX_SCUFDI1_isr,    &R_SCUX_SCUFUI1_isr,    &R_SCUX_SCUDVI1_isr }
        ,   { &R_SCUX_SCUAI1_isr,   &R_SCUX_SCUFDI2_isr,    &R_SCUX_SCUFUI2_isr,    &R_SCUX_SCUDVI2_isr }
        ,   { &R_SCUX_SCUAI1_isr,   &R_SCUX_SCUFDI3_isr,    &R_SCUX_SCUFUI3_isr,    &R_SCUX_SCUDVI3_isr }
    };

    return scux_int_func_tbl[ch][module];
}

/*******************************************************************************
* Function Name: R_SCUX_SetIntCallback
* Description  : Set Interrupt callback
* Arguments    : ch -
*                    SCUX channel
*                module -
*                    SCUX Module
*                * callback -
*                    callback address of Interrupt function
* Return Value : SCUX_OK -
*                    Operation successful.
*                SCUX_E_PAR -
*                    Bad arguments
*******************************************************************************/
int32_t R_SCUX_SetIntCallback ( const uint32_t ch,
                                const uint32_t module,
                                void (* const callback)(const uint32_t int_req))
{
    int32_t ret = SCUX_OK;

    if ((NUM_SCUX_CH <= ch) || (NUM_SCUX_INT_ID_MODULE <= module))
    {
        ret = SCUX_E_PAR;
    }
    else
    {
        scux_int_callback[ch][module] = callback;
    }

    return ret;
}

/*******************************************************************************
* Function Name: R_SCUX_SCUAI0_isr
* Description  : SCUAI0 Interrupt function
*                SCUAI0 is interrupt factor of SRC0 and SRC1
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
*******************************************************************************/
static void R_SCUX_SCUAI0_isr (uint32_t int_sense)
{
    if (0u != *g_scux_reg_address_tbl[SCUX_CH_0].sevcr)
    {
        SCUAI_isr(SCUX_CH_0);
    }
    else
    {
        SCUAI_isr(SCUX_CH_1);
    }
}

/*******************************************************************************
* Function Name: R_SCUX_SCUAI1_isr
* Description  : SCUAI1 Interrupt function
*                SCUAI1 is interrupt factor of SRC2 and SRC3
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
*******************************************************************************/
static void R_SCUX_SCUAI1_isr (uint32_t int_sense)
{
    if (0u != *g_scux_reg_address_tbl[SCUX_CH_2].sevcr)
    {
        SCUAI_isr(SCUX_CH_2);
    }
    else
    {
        SCUAI_isr(SCUX_CH_3);
    }
}

/*******************************************************************************
* Function Name: R_SCUX_SCUFDI0_isr
* Description  : SCUFDI0 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
*******************************************************************************/
static void R_SCUX_SCUFDI0_isr (uint32_t int_sense)
{
    SCUFDI_isr(SCUX_CH_0);
}

/*******************************************************************************
* Function Name: R_SCUX_SCUFDI1_isr
* Description  : SCUFDI1 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
*******************************************************************************/
static void R_SCUX_SCUFDI1_isr (uint32_t int_sense)
{
    SCUFDI_isr(SCUX_CH_1);
}

/*******************************************************************************
* Function Name: R_SCUX_SCUFDI2_isr
* Description  : SCUFDI2 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
*******************************************************************************/
static void R_SCUX_SCUFDI2_isr(uint32_t int_sense)
{
    SCUFDI_isr(SCUX_CH_2);
}

/*******************************************************************************
* Function Name: R_SCUX_SCUFDI3_isr
* Description  : SCUFDI3 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
*******************************************************************************/
static void R_SCUX_SCUFDI3_isr (uint32_t int_sense)
{
    SCUFDI_isr(SCUX_CH_3);
}

/*******************************************************************************
* Function Name: R_SCUX_SCUFUI0_isr
* Description  : SCUFUI0 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
*******************************************************************************/
static void R_SCUX_SCUFUI0_isr (uint32_t int_sense)
{
    SCUFUI_isr(SCUX_CH_0);
}

/*******************************************************************************
* Function Name: R_SCUX_SCUFUI1_isr
* Description  : SCUFUI1 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
*******************************************************************************/
static void R_SCUX_SCUFUI1_isr (uint32_t int_sense)
{
    SCUFUI_isr(SCUX_CH_1);
}

/*******************************************************************************
* Function Name: R_SCUX_SCUFUI2_isr
* Description  : SCUFUI2 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
*******************************************************************************/
static void R_SCUX_SCUFUI2_isr (uint32_t int_sense)
{
    SCUFUI_isr(SCUX_CH_2);
}

/*******************************************************************************
* Function Name: R_SCUX_SCUFUI3_isr
* Description  : SCUFUI3 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
*******************************************************************************/
static void R_SCUX_SCUFUI3_isr (uint32_t int_sense)
{
    SCUFUI_isr(SCUX_CH_3);
}

/*******************************************************************************
* Function Name: R_SCUX_SCUDVI0_isr
* Description  : SCUDVI0 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
*******************************************************************************/
static void R_SCUX_SCUDVI0_isr (uint32_t int_sense)
{
    SCUDVI_isr(SCUX_CH_0);
}

/*******************************************************************************
* Function Name: R_SCUX_SCUDVI1_isr
* Description  : SCUDVI1 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
*******************************************************************************/
static void R_SCUX_SCUDVI1_isr (uint32_t int_sense)
{
    SCUDVI_isr(SCUX_CH_1);
}

/*******************************************************************************
* Function Name: R_SCUX_SCUDVI2_isr
* Description  : SCUDVI2 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
*******************************************************************************/
static void R_SCUX_SCUDVI2_isr (uint32_t int_sense)
{
    SCUDVI_isr(SCUX_CH_2);
}

/*******************************************************************************
* Function Name: R_SCUX_SCUDVI3_isr
* Description  : SCUDVI3 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
*******************************************************************************/
static void R_SCUX_SCUDVI3_isr (uint32_t int_sense)
{
    SCUDVI_isr(SCUX_CH_3);
}

/*******************************************************************************
* Function Name: SCUAI_isr
* Description  : SCUAI Interrupt function
* Arguments    : ch -
*                    SCUX channel
* Return Value : none
*******************************************************************************/
static void SCUAI_isr (const uint32_t ch)
{
    const scux_reg_address_t * scux_reg;
    uint32_t int_request;

    scux_reg = &g_scux_reg_address_tbl[ch];

    /* Get request of the interrupt */
    int_request = *(scux_reg->sevcr);

    /* Request ON bits make 0, Other bits make 1 */
    *(scux_reg->sevcr) = COMMON_Get32BitMask(~int_request, SEVCR_REG_MASK);
    *(scux_reg->sevcr);     /* Dummy read */

    /* Callback Interrupt function */
    if (0 != scux_int_callback[ch][SCUX_INT_ID_SRC])
    {
        scux_int_callback[ch][SCUX_INT_ID_SRC](int_request);
    }
}

/*******************************************************************************
* Function Name: SCUFDI_isr
* Description  : SCUFDI _Interrupt function
* Arguments    : ch -
*                    SCUX channel
* Return Value : none
*******************************************************************************/
static void SCUFDI_isr (const uint32_t ch)
{
    const scux_reg_address_t * scux_reg;
    uint32_t temp_reg;
    uint32_t int_request;
    uint32_t mask_enable_reg;

    scux_reg = &g_scux_reg_address_tbl[ch];

    /* Mask request of the interrupt enable bit */
    mask_enable_reg = *(scux_reg->devmr);
    temp_reg = *(scux_reg->devcr);
    int_request = COMMON_Get32BitMask(temp_reg, mask_enable_reg);

    /* Request ON bits make 0, Other bits make 1 */
    *(scux_reg->devcr) = COMMON_Get32BitMask(~int_request, DEVCR_REG_MASK);
    *(scux_reg->devcr);     /* Dummy read */

    /* Callback Interrupt function */
    if (0 != scux_int_callback[ch][SCUX_INT_ID_FFD])
    {
        scux_int_callback[ch][SCUX_INT_ID_FFD](int_request);
    }
}

/*******************************************************************************
* Function Name: SCUFUI_isr
* Description  : SCUFUI Interrupt function
* Arguments    : ch -
*                    SCUX channel
* Return Value : none
*******************************************************************************/
static void SCUFUI_isr (const uint32_t ch)
{
    const scux_reg_address_t * scux_reg;
    uint32_t temp_reg;
    uint32_t int_request;
    uint32_t mask_enable_reg;

    scux_reg = &g_scux_reg_address_tbl[ch];

    /* Mask request of the interrupt enable bit */
    mask_enable_reg = *(scux_reg->uevmr);
    temp_reg = *(scux_reg->uevcr);
    int_request = COMMON_Get32BitMask(temp_reg, mask_enable_reg);

    /* Request ON bits make 0, Other bits make 1 */
    *(scux_reg->uevcr) = COMMON_Get32BitMask(~int_request, UEVCR_REG_MASK);
    *(scux_reg->uevcr);     /* Dummy read */

    /* Callback Interrupt function */
    if (0 != scux_int_callback[ch][SCUX_INT_ID_FFU])
    {
        scux_int_callback[ch][SCUX_INT_ID_FFU](int_request);
    }
}

/*******************************************************************************
* Function Name: SCUDVI_isr
* Description  : SCUDVI Interrupt function
* Arguments    : ch -
*                    SCUX channel
* Return Value : none
*******************************************************************************/
static void SCUDVI_isr (const uint32_t ch)
{
    const scux_reg_address_t * scux_reg;
    uint32_t int_request;

    scux_reg = &g_scux_reg_address_tbl[ch];

    /* Get request of the interrupt */
    int_request = *(scux_reg->vevcr);

    /* Request ON bits make 0, Other bits make 1 */
    *(scux_reg->vevcr) = COMMON_Get32BitMask(~int_request, VEVCR_REG_MASK);
    *(scux_reg->vevcr);     /* Dummy read */

    /* Callback Interrupt function */
    if (0 != scux_int_callback[ch][SCUX_INT_ID_DVU])
    {
        scux_int_callback[ch][SCUX_INT_ID_DVU](int_request);
    }
}

/* End of File */
