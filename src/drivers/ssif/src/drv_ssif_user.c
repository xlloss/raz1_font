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
/*******************************************************************************
* File Name   : drv_ssif_user.c
* $Rev: 392 $
* $Date:: 2013-08-02 19:56:33 +0900#$
* Description :  Aragon Sample Program - Sound Serial Interface
******************************************************************************/


/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "reg_bit_access.h"
#include "drv_ssif_user.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* ---- GPIO Bit Data ---- */
#define SSIF_GPIO_CLEAR_VALUE   (0)
#define SSIF_GPIO_SET_VALUE     (1)

/******************************************************************************
Typedef definitions
******************************************************************************/
/* Bit information for GPIO register */
enum gpio_ssif_bit_inf
{
        GPIO_P4_BIT_4_SSISCK0
    ,   GPIO_P4_BIT_5_SSIWS0
    ,   GPIO_P4_BIT_6_SSIRXD0
    ,   GPIO_P4_BIT_7_SSITXD0
    ,   GPIO_P4_BIT_12_SSISCK3
    ,   GPIO_P4_BIT_13_SSIWS3
    ,   GPIO_P4_BIT_15_SSITXD3
    ,   GPIO_P11_BIT_4_SSISCK4
    ,   GPIO_P11_BIT_5_SSIWS4
    ,   GPIO_P11_BIT_6_SSIDATA4
    ,   NUM_GPIO_SSIF_BIT_INF
};

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Bit information for GPIO register */
static const uint16_reg_bit_inf_t gpio_ssif_bit_inf_tbl[NUM_GPIO_SSIF_BIT_INF] =
{
        { 0x0010    ,   4   }   /* GPIO_P4_BIT_4_SSISCK0   */
    ,   { 0x0020    ,   5   }   /* GPIO_P4_BIT_5_SSIWS0    */
    ,   { 0x0040    ,   6   }   /* GPIO_P4_BIT_6_SSIRXD0   */
    ,   { 0x0080    ,   7   }   /* GPIO_P4_BIT_7_SSITXD0   */
    ,   { 0x1000    ,   12  }   /* GPIO_P4_BIT_12_SSISCK3  */
    ,   { 0x2000    ,   13  }   /* GPIO_P4_BIT_13_SSIWS3   */
    ,   { 0x8000    ,   15  }   /* GPIO_P4_BIT_15_SSITXD3  */
    ,   { 0x0010    ,   4   }   /* GPIO_P11_BIT_4_SSISCK4  */
    ,   { 0x0020    ,   5   }   /* GPIO_P11_BIT_5_SSIWS4   */
    ,   { 0x0040    ,   6   }   /* GPIO_P11_BIT_6_SSIDATA4 */
};

/*******************************************************************************
* Function Name: SSI_Ssi0PinSet
* Description  : SSI0 Pin Setting
* Arguments    : none
* Return Value : none
*******************************************************************************/
void SSI_UserdefSsi0PinSet (void)
{
    const uint16_reg_bit_inf_t * gpio_bit_inf;

    /* ---- SSISCK0 ---- */
    gpio_bit_inf = &gpio_ssif_bit_inf_tbl[GPIO_P4_BIT_4_SSISCK0];
    COMMON_Set16BitMaskLShift(&GPIO.PIBC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Input Buffer:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Bidirectional mode:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PM4,    gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Input mode */
    COMMON_Set16BitMaskLShift(&GPIO.PMC4,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Port mode */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* S/W I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC4,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Bidirectional mode:enable */
    COMMON_Set16BitMaskLShift(&GPIO.PFC4,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* 5th Alternative:SSISCK0 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCE4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* 5th Alternative:SSISCK0 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCAE4, gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* 5th Alternative:SSISCK0 */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC4,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* direct I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PMC4,   gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Alternative mode */

    /* ---- SSIWS0 ---- */
    gpio_bit_inf = &gpio_ssif_bit_inf_tbl[GPIO_P4_BIT_5_SSIWS0];
    COMMON_Set16BitMaskLShift(&GPIO.PIBC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Input Buffer:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Bidirectional mode:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PM4,    gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Input mode */
    COMMON_Set16BitMaskLShift(&GPIO.PMC4,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Port mode */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* S/W I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC4,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Bidirectional mode:enable */
    COMMON_Set16BitMaskLShift(&GPIO.PFC4,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* 5th Alternative:SSIWS0 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCE4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* 5th Alternative:SSIWS0 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCAE4, gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* 5th Alternative:SSIWS0 */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC4,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* direct I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PMC4,   gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Alternative mode */

    /* ---- SSIRxD0 ---- */
    gpio_bit_inf = &gpio_ssif_bit_inf_tbl[GPIO_P4_BIT_6_SSIRXD0];
    COMMON_Set16BitMaskLShift(&GPIO.PIBC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Input Buffer:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Bidirectional mode:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PM4,    gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Input mode */
    COMMON_Set16BitMaskLShift(&GPIO.PMC4,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Port mode */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* S/W I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PFC4,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* 5th Alternative:SSIRxD0 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCE4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* 5th Alternative:SSIRxD0 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCAE4, gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* 5th Alternative:SSIRxD0 */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC4,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* direct I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PMC4,   gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Alternative mode */

    /* ---- SSITxD0 ---- */
    gpio_bit_inf = &gpio_ssif_bit_inf_tbl[GPIO_P4_BIT_7_SSITXD0];
    COMMON_Set16BitMaskLShift(&GPIO.PIBC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Input Buffer:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Bidirectional mode:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PM4,    gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Input mode */
    COMMON_Set16BitMaskLShift(&GPIO.PMC4,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Port mode */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* S/W I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PFC4,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* 5th Alternative:SSITxD0 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCE4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* 5th Alternative:SSITxD0 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCAE4, gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* 5th Alternative:SSITxD0 */
    COMMON_Set16BitMaskLShift(&GPIO.PMC4,   gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Alternative mode */
    COMMON_Set16BitMaskLShift(&GPIO.PM4,    gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Output mode */
}

/*******************************************************************************
* Function Name: SSI_Ssi1PinSet
* Description  : SSI1 Pin Setting
* Arguments    : none
* Return Value : none
*******************************************************************************/
void SSI_UserdefSsi1PinSet (void)
{

}

/*******************************************************************************
* Function Name: SSI_Ssi2PinSet
* Description  : SSI2 Pin Setting
* Arguments    : none
* Return Value : none
*******************************************************************************/
void SSI_UserdefSsi2PinSet (void)
{

}

/*******************************************************************************
* Function Name: SSI_Ssi3PinSet
* Description  : SSI3 Pin Setting
* Arguments    : none
* Return Value : none
*******************************************************************************/
void SSI_UserdefSsi3PinSet (void)
{
    const uint16_reg_bit_inf_t * gpio_bit_inf;

    /* ---- SSISCK3 ---- */
    gpio_bit_inf = &gpio_ssif_bit_inf_tbl[GPIO_P4_BIT_12_SSISCK3];
    COMMON_Set16BitMaskLShift(&GPIO.PIBC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* Input Buffer:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* Bidirectional mode:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PM4,    gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* Input mode */
    COMMON_Set16BitMaskLShift(&GPIO.PMC4,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* Port mode */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* S/W I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC4,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* Bidirectional mode:enable */
    COMMON_Set16BitMaskLShift(&GPIO.PFC4,   gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* 6th Alternative:SSISCK3 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCE4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* 6th Alternative:SSISCK3 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCAE4, gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* 6th Alternative:SSISCK3 */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC4,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* direct I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PMC4,   gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* Alternative mode */

    /* ---- SSIWS3 ---- */
    gpio_bit_inf = &gpio_ssif_bit_inf_tbl[GPIO_P4_BIT_13_SSIWS3];
    COMMON_Set16BitMaskLShift(&GPIO.PIBC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* Input Buffer:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* Bidirectional mode:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PM4,    gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* Input mode */
    COMMON_Set16BitMaskLShift(&GPIO.PMC4,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* Port mode */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* S/W I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC4,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* Bidirectional mode:enable */
    COMMON_Set16BitMaskLShift(&GPIO.PFC4,   gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* 6th Alternative:SSIWS3 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCE4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* 6th Alternative:SSIWS3 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCAE4, gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* 6th Alternative:SSIWS3 */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC4,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* direct I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PMC4,   gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* Alternative mode */

    /* ---- SSITxD3 ---- */
    gpio_bit_inf = &gpio_ssif_bit_inf_tbl[GPIO_P4_BIT_15_SSITXD3];
    COMMON_Set16BitMaskLShift(&GPIO.PIBC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* Input Buffer:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* Bidirectional mode:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PM4,    gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* Input mode */
    COMMON_Set16BitMaskLShift(&GPIO.PMC4,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* Port mode */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* S/W I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PFC4,   gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* 6th Alternative:SSITxD3 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCE4,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* 6th Alternative:SSITxD3 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCAE4, gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* 6th Alternative:SSITxD3 */
    COMMON_Set16BitMaskLShift(&GPIO.PMC4,   gpio_bit_inf, SSIF_GPIO_SET_VALUE);     /* Alternative mode */
    COMMON_Set16BitMaskLShift(&GPIO.PM4,    gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);   /* Output mode */
}

/*******************************************************************************
* Function Name: SSI_Ssi4PinSet
* Description  : SSI4 Pin Setting
* Arguments    : none
* Return Value : none
*******************************************************************************/
void SSI_UserdefSsi4PinSet (void)
{
    const uint16_reg_bit_inf_t * gpio_bit_inf;

    /* ---- SSISCK4 ---- */
    gpio_bit_inf = &gpio_ssif_bit_inf_tbl[GPIO_P11_BIT_4_SSISCK4];
    COMMON_Set16BitMaskLShift(&GPIO.PIBC11,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Input Buffer:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC11,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Bidirectional mode:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PM11,    gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Input mode */
    COMMON_Set16BitMaskLShift(&GPIO.PMC11,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Port mode */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC11,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* S/W I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC11,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Bidirectional mode:enable */
    COMMON_Set16BitMaskLShift(&GPIO.PFC11,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* 3rd Alternative:SSISCK4 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCE11,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* 3rd Alternative:SSISCK4 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCAE11, gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* 3rd Alternative:SSISCK4 */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC11,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* direct I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PMC11,   gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Alternative mode */

    /* ---- SSIWS4 ---- */
    gpio_bit_inf = &gpio_ssif_bit_inf_tbl[GPIO_P11_BIT_5_SSIWS4];
    COMMON_Set16BitMaskLShift(&GPIO.PIBC11,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Input Buffer:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC11,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Bidirectional mode:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PM11,    gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Input mode */
    COMMON_Set16BitMaskLShift(&GPIO.PMC11,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Port mode */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC11,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* S/W I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC11,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Bidirectional mode:enable */
    COMMON_Set16BitMaskLShift(&GPIO.PFC11,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* 3rd Alternative:SSIWS4 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCE11,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* 3rd Alternative:SSIWS4 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCAE11, gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* 3rd Alternative:SSIWS4 */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC11,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* direct I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PMC11,   gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Alternative mode */

    /* ---- SSIDATA4 ---- */
    gpio_bit_inf = &gpio_ssif_bit_inf_tbl[GPIO_P11_BIT_6_SSIDATA4];
    COMMON_Set16BitMaskLShift(&GPIO.PIBC11,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Input Buffer:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC11,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Bidirectional mode:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PM11,    gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Input mode */
    COMMON_Set16BitMaskLShift(&GPIO.PMC11,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Port mode */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC11,  gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* S/W I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PFC11,   gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* 3rd Alternative:SSITxD4 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCE11,  gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* 3rd Alternative:SSITxD4 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCAE11, gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* 3rd Alternative:SSITxD4 */
    COMMON_Set16BitMaskLShift(&GPIO.PMC11,   gpio_bit_inf, SSIF_GPIO_SET_VALUE);    /* Alternative mode */
    COMMON_Set16BitMaskLShift(&GPIO.PM11,    gpio_bit_inf, SSIF_GPIO_CLEAR_VALUE);  /* Output mode */
}

/*******************************************************************************
* Function Name: SSI_Ssi5PinSet
* Description  : SSI5 Pin Setting
* Arguments    : none
* Return Value : none
*******************************************************************************/
void SSI_UserdefSsi5PinSet (void)
{

}


/* END of File */
