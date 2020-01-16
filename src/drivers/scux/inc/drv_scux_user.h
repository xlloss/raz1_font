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
* File Name    : drv_scux_user.h
* $Rev: 729 $
* $Date:: 2014-06-11 17:44:40 +0900#$
* Description  : For user definitions of SCUX
******************************************************************************/
/* Guards against multiple inclusion */
#ifndef DRV_SCUX_USER_H
#define DRV_SCUX_USER_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <string.h>
#include "r_typedefs.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/******************************************************************************
Macro definitions
******************************************************************************/

#define THIS_CH_NOT_USED        (0u)

/* ==== SCUX SSIPMD_CIM register setting ==== */

#define SCKSEL_AUDIO_CLK        (0x0u)
#define SCKSEL_AUIDIO_X1        (0x1u)
#define SCKSEL_MLB_CLK          (0x2u)
#define SCKSEL_USB_X1           (0x3u)
#define SCKSEL_CLKP1_PHY_DIV2   (0x4u)
#define SCKSEL_SSIF0_WS         (0x8u)
#define SCKSEL_SSIF1_WS         (0x9u)
#define SCKSEL_SSIF2_WS         (0xAu)
#define SCKSEL_SSIF3_WS         (0xBu)
#define SCKSEL_SSIF4_WS         (0xCu)
#define SCKSEL_SSIF5_WS         (0xDu)


#define FDTSEL_CH_0_SCKDIV  THIS_CH_NOT_USED
#define FDTSEL_CH_1_SCKDIV  (694u)      /* P1PHY(66.67MHz)/2/48KHz */
#define FDTSEL_CH_2_SCKDIV  (694u)      /* P1PHY(66.67MHz)/2/48KHz */
#define FDTSEL_CH_3_SCKDIV  (694u)      /* P1PHY(66.67MHz)/2/48KHz */

#define FUTSEL_CH_0_SCKDIV  THIS_CH_NOT_USED
#define FUTSEL_CH_1_SCKDIV  THIS_CH_NOT_USED
#define FUTSEL_CH_2_SCKDIV  THIS_CH_NOT_USED
#define FUTSEL_CH_3_SCKDIV  THIS_CH_NOT_USED


#define FDTSEL_CH_0_SCKSEL  THIS_CH_NOT_USED
#define FDTSEL_CH_1_SCKSEL  SCKSEL_CLKP1_PHY_DIV2
#define FDTSEL_CH_2_SCKSEL  SCKSEL_CLKP1_PHY_DIV2
#define FDTSEL_CH_3_SCKSEL  SCKSEL_CLKP1_PHY_DIV2

#define FUTSEL_CH_0_SCKSEL  SCKSEL_SSIF0_WS
#define FUTSEL_CH_1_SCKSEL  THIS_CH_NOT_USED
#define FUTSEL_CH_2_SCKSEL  THIS_CH_NOT_USED
#define FUTSEL_CH_3_SCKSEL  SCKSEL_SSIF0_WS


#define FDTSEL_CH_0_VALUE ((FDTSEL_CH_0_SCKDIV << 16) | FDTSEL_CH_0_SCKSEL)
#define FDTSEL_CH_1_VALUE ((FDTSEL_CH_1_SCKDIV << 16) | FDTSEL_CH_1_SCKSEL)
#define FDTSEL_CH_2_VALUE ((FDTSEL_CH_2_SCKDIV << 16) | FDTSEL_CH_2_SCKSEL)
#define FDTSEL_CH_3_VALUE ((FDTSEL_CH_3_SCKDIV << 16) | FDTSEL_CH_3_SCKSEL)

#define FUTSEL_CH_0_VALUE ((FUTSEL_CH_0_SCKDIV << 16) | FUTSEL_CH_0_SCKSEL)
#define FUTSEL_CH_1_VALUE ((FUTSEL_CH_1_SCKDIV << 16) | FUTSEL_CH_1_SCKSEL)
#define FUTSEL_CH_2_VALUE ((FUTSEL_CH_2_SCKDIV << 16) | FUTSEL_CH_2_SCKSEL)
#define FUTSEL_CH_3_VALUE ((FUTSEL_CH_3_SCKDIV << 16) | FUTSEL_CH_3_SCKSEL)


/* ==== SCUX SSIPMD_CIM register setting ==== */

#define SSICKS_AUDIO_CLK    (0x0u)
#define SSICKS_MLBCLK       (0x1u)

#define SSIPMD_SSI5CKS  THIS_CH_NOT_USED
#define SSIPMD_SSI4CKS  THIS_CH_NOT_USED    /* Use AUDIO_X1 (cf. SSI_SSICR4_CKS_VALUE) */
#define SSIPMD_SSI3CKS  THIS_CH_NOT_USED    /* Use AUDIO_X1 (cf. SSI_SSICR3_CKS_VALUE) */
#define SSIPMD_SSI2CKS  THIS_CH_NOT_USED
#define SSIPMD_SSI1CKS  THIS_CH_NOT_USED
#define SSIPMD_SSI0CKS  THIS_CH_NOT_USED    /* Use AUDIO_X1 (cf. SSI_SSICR0_CKS_VALUE) */


#define SSIPMD_INDEPENDENCE         (0x0u)
#define SSIPMD_COMMON_SLAVE         (0x1u)
#define SSIPMD_COMMON_MASTER_SLAVE  (0x2u)
/* Case of SSIPMD_COMMON_SLAVE
    SSI5PMD:The SSIF3 pins are used in common. Both SSIF3 and SSIF5 are slaves.
    SSI4PMD:The SSIF3 pins are used in common. Both SSIF3 and SSIF4 are slaves.
    SSI3PMD:The SSIF0 pins are used in common. Both SSIF0 and SSIF3 are slaves.
    SSI2PMD:The SSIF0 pins are used in common. Both SSIF0 and SSIF2 are slaves.
    SSI1PMD:The SSIF0 pins are used in common. Both SSIF0 and SSIF1 are slaves.
   Case of SSIPMD_COMMON_MASTER_SLAVE
    SSI5PMD:The SSIF3 pins are used in common. SSIF3 is the master and SSIF5 is the slave.
    SSI4PMD:The SSIF3 pins are used in common. SSIF3 is the master and SSIF4 is the slave.
    SSI3PMD:The SSIF0 pins are used in common. SSIF0 is the master and SSIF3 is the slave.
    SSI2PMD:The SSIF0 pins are used in common. SSIF0 is the master and SSIF2 is the slave.
    SSI1PMD:The SSIF0 pins are used in common. SSIF0 is the master and SSIF1 is the slave. */

#define SSIPMD_SSI5PMD  THIS_CH_NOT_USED
#define SSIPMD_SSI4PMD  SSIPMD_INDEPENDENCE
#define SSIPMD_SSI3PMD  SSIPMD_INDEPENDENCE
#define SSIPMD_SSI2PMD  THIS_CH_NOT_USED
#define SSIPMD_SSI1PMD  THIS_CH_NOT_USED

#define SSIPMD_VALUE    ((SSIPMD_SSI5CKS << 21) | (SSIPMD_SSI4CKS << 20) | (SSIPMD_SSI3CKS << 19) | \
                         (SSIPMD_SSI2CKS << 18) | (SSIPMD_SSI1CKS << 17) | (SSIPMD_SSI0CKS << 16) | \
                         (SSIPMD_SSI3PMD << 14) | (SSIPMD_SSI4PMD << 10) | (SSIPMD_SSI5PMD << 8)  | \
                         (SSIPMD_SSI2PMD << 2)  | SSIPMD_SSI1PMD)



/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DRV_SCUX_USER_H */

/* End of File */
