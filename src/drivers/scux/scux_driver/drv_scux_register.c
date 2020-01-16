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
* File Name   : drv_scux.c
* $Rev: 729 $
* $Date:: 2014-06-11 17:44:40 +0900#$
* Description  : This module is sample of SCUX driver.
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "drv_scux_register.h"
#include "drv_scux.h"

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
/* ==== Addresses of the SCUX registers for each channel ==== */
const scux_reg_address_t g_scux_reg_address_tbl[NUM_SCUX_CH] =
{
    { /* ch0 */
        &SCUX.IPCIR_IPC0_0,         &SCUX.IPSLR_IPC0_0,     &SCUX.OPCIR_OPC0_0,
        &SCUX.OPSLR_OPC0_0,         &SCUX.FFDIR_FFD0_0,     &SCUX.FDAIR_FFD0_0,
        &SCUX.DRQSR_FFD0_0,         &SCUX.FFDPR_FFD0_0,     &SCUX.FFDBR_FFD0_0,
        &SCUX.DEVMR_FFD0_0,         &SCUX.DEVCR_FFD0_0,     &SCUX.FDTSEL0_CIM,
        &SCUX.FFUIR_FFU0_0,         &SCUX.FUAIR_FFU0_0,     &SCUX.URQSR_FFU0_0,
        &SCUX.FFUPR_FFU0_0,         &SCUX.UEVMR_FFU0_0,     &SCUX.UEVCR_FFU0_0,
        &SCUX.FUTSEL0_CIM,          &SCUX.SRCIR0_2SRC0_0,   &SCUX.SADIR0_2SRC0_0,
        &SCUX.SRCBR0_2SRC0_0,       &SCUX.IFSCR0_2SRC0_0,   &SCUX.IFSVR0_2SRC0_0,
        &SCUX.SRCCR0_2SRC0_0,       &SCUX.MNFSR0_2SRC0_0,   &SCUX.BFSSR0_2SRC0_0,
        &SCUX.SC2SR0_2SRC0_0,       &SCUX.WATSR0_2SRC0_0,   &SCUX.SEVMR0_2SRC0_0,
        &SCUX.SEVCR0_2SRC0_0,       &SCUX.SRCIRR_2SRC0_0,   &SCUX.SRCRSEL0_CIM,
        &SCUX.DVUIR_DVU0_0,         &SCUX.VADIR_DVU0_0,     &SCUX.DVUBR_DVU0_0,
        &SCUX.DVUCR_DVU0_0,         &SCUX.ZCMCR_DVU0_0,     &SCUX.VRCTR_DVU0_0,
        &SCUX.VRPDR_DVU0_0,         &SCUX.VRDBR_DVU0_0,     &SCUX.VRWTR_DVU0_0,
        &SCUX.VOL0R_DVU0_0,         &SCUX.VOL1R_DVU0_0,     &SCUX.VOL2R_DVU0_0,
        &SCUX.VOL3R_DVU0_0,         &SCUX.VOL4R_DVU0_0,     &SCUX.VOL5R_DVU0_0,
        &SCUX.VOL6R_DVU0_0,         &SCUX.VOL7R_DVU0_0,     &SCUX.DVUER_DVU0_0,
        &SCUX.DVUSR_DVU0_0,         &SCUX.VEVMR_DVU0_0,     &SCUX.VEVCR_DVU0_0,
        &SCUX.DMATD0_CIM.UINT32,    &SCUX.DMATU0_CIM.UINT32
    },
    { /* ch1 */
        &SCUX.IPCIR_IPC0_1,         &SCUX.IPSLR_IPC0_1,     &SCUX.OPCIR_OPC0_1,
        &SCUX.OPSLR_OPC0_1,         &SCUX.FFDIR_FFD0_1,     &SCUX.FDAIR_FFD0_1,
        &SCUX.DRQSR_FFD0_1,         &SCUX.FFDPR_FFD0_1,     &SCUX.FFDBR_FFD0_1,
        &SCUX.DEVMR_FFD0_1,         &SCUX.DEVCR_FFD0_1,     &SCUX.FDTSEL1_CIM,
        &SCUX.FFUIR_FFU0_1,         &SCUX.FUAIR_FFU0_1,     &SCUX.URQSR_FFU0_1,
        &SCUX.FFUPR_FFU0_1,         &SCUX.UEVMR_FFU0_1,     &SCUX.UEVCR_FFU0_1,
        &SCUX.FUTSEL1_CIM,          &SCUX.SRCIR1_2SRC0_0,   &SCUX.SADIR1_2SRC0_0,
        &SCUX.SRCBR1_2SRC0_0,       &SCUX.IFSCR1_2SRC0_0,   &SCUX.IFSVR1_2SRC0_0,
        &SCUX.SRCCR1_2SRC0_0,       &SCUX.MNFSR1_2SRC0_0,   &SCUX.BFSSR1_2SRC0_0,
        &SCUX.SC2SR1_2SRC0_0,       &SCUX.WATSR1_2SRC0_0,   &SCUX.SEVMR1_2SRC0_0,
        &SCUX.SEVCR1_2SRC0_0,       &SCUX.SRCIRR_2SRC0_0,   &SCUX.SRCRSEL1_CIM,
        &SCUX.DVUIR_DVU0_1,         &SCUX.VADIR_DVU0_1,     &SCUX.DVUBR_DVU0_1,
        &SCUX.DVUCR_DVU0_1,         &SCUX.ZCMCR_DVU0_1,     &SCUX.VRCTR_DVU0_1,
        &SCUX.VRPDR_DVU0_1,         &SCUX.VRDBR_DVU0_1,     &SCUX.VRWTR_DVU0_1,
        &SCUX.VOL0R_DVU0_1,         &SCUX.VOL1R_DVU0_1,     &SCUX.VOL2R_DVU0_1,
        &SCUX.VOL3R_DVU0_1,         &SCUX.VOL4R_DVU0_1,     &SCUX.VOL5R_DVU0_1,
        &SCUX.VOL6R_DVU0_1,         &SCUX.VOL7R_DVU0_1,     &SCUX.DVUER_DVU0_1,
        &SCUX.DVUSR_DVU0_1,         &SCUX.VEVMR_DVU0_1,     &SCUX.VEVCR_DVU0_1,
        &SCUX.DMATD1_CIM.UINT32,    &SCUX.DMATU1_CIM.UINT32
    },
    { /* ch2 */
        &SCUX.IPCIR_IPC0_2,         &SCUX.IPSLR_IPC0_2,     &SCUX.OPCIR_OPC0_2,
        &SCUX.OPSLR_OPC0_2,         &SCUX.FFDIR_FFD0_2,     &SCUX.FDAIR_FFD0_2,
        &SCUX.DRQSR_FFD0_2,         &SCUX.FFDPR_FFD0_2,     &SCUX.FFDBR_FFD0_2,
        &SCUX.DEVMR_FFD0_2,         &SCUX.DEVCR_FFD0_2,     &SCUX.FDTSEL2_CIM,
        &SCUX.FFUIR_FFU0_2,         &SCUX.FUAIR_FFU0_2,     &SCUX.URQSR_FFU0_2,
        &SCUX.FFUPR_FFU0_2,         &SCUX.UEVMR_FFU0_2,     &SCUX.UEVCR_FFU0_2,
        &SCUX.FUTSEL2_CIM,          &SCUX.SRCIR0_2SRC0_1,   &SCUX.SADIR0_2SRC0_1,
        &SCUX.SRCBR0_2SRC0_1,       &SCUX.IFSCR0_2SRC0_1,   &SCUX.IFSVR0_2SRC0_1,
        &SCUX.SRCCR0_2SRC0_1,       &SCUX.MNFSR0_2SRC0_1,   &SCUX.BFSSR0_2SRC0_1,
        &SCUX.SC2SR0_2SRC0_1,       &SCUX.WATSR0_2SRC0_1,   &SCUX.SEVMR0_2SRC0_1,
        &SCUX.SEVCR0_2SRC0_1,       &SCUX.SRCIRR_2SRC0_1,   &SCUX.SRCRSEL2_CIM,
        &SCUX.DVUIR_DVU0_2,         &SCUX.VADIR_DVU0_2,     &SCUX.DVUBR_DVU0_2,
        &SCUX.DVUCR_DVU0_2,         &SCUX.ZCMCR_DVU0_2,     &SCUX.VRCTR_DVU0_2,
        &SCUX.VRPDR_DVU0_2,         &SCUX.VRDBR_DVU0_2,     &SCUX.VRWTR_DVU0_2,
        &SCUX.VOL0R_DVU0_2,         &SCUX.VOL1R_DVU0_2,     &SCUX.VOL2R_DVU0_2,
        &SCUX.VOL3R_DVU0_2,         &SCUX.VOL4R_DVU0_2,     &SCUX.VOL5R_DVU0_2,
        &SCUX.VOL6R_DVU0_2,         &SCUX.VOL7R_DVU0_2,     &SCUX.DVUER_DVU0_2,
        &SCUX.DVUSR_DVU0_2,         &SCUX.VEVMR_DVU0_2,     &SCUX.VEVCR_DVU0_2,
        &SCUX.DMATD2_CIM.UINT32,    &SCUX.DMATU2_CIM.UINT32
    },
    { /* ch3 */
        &SCUX.IPCIR_IPC0_3,         &SCUX.IPSLR_IPC0_3,     &SCUX.OPCIR_OPC0_3,
        &SCUX.OPSLR_OPC0_3,         &SCUX.FFDIR_FFD0_3,     &SCUX.FDAIR_FFD0_3,
        &SCUX.DRQSR_FFD0_3,         &SCUX.FFDPR_FFD0_3,     &SCUX.FFDBR_FFD0_3,
        &SCUX.DEVMR_FFD0_3,         &SCUX.DEVCR_FFD0_3,     &SCUX.FDTSEL3_CIM,
        &SCUX.FFUIR_FFU0_3,         &SCUX.FUAIR_FFU0_3,     &SCUX.URQSR_FFU0_3,
        &SCUX.FFUPR_FFU0_3,         &SCUX.UEVMR_FFU0_3,     &SCUX.UEVCR_FFU0_3,
        &SCUX.FUTSEL3_CIM,          &SCUX.SRCIR1_2SRC0_1,   &SCUX.SADIR1_2SRC0_1,
        &SCUX.SRCBR1_2SRC0_1,       &SCUX.IFSCR1_2SRC0_1,   &SCUX.IFSVR1_2SRC0_1,
        &SCUX.SRCCR1_2SRC0_1,       &SCUX.MNFSR1_2SRC0_1,   &SCUX.BFSSR1_2SRC0_1,
        &SCUX.SC2SR1_2SRC0_1,       &SCUX.WATSR1_2SRC0_1,   &SCUX.SEVMR1_2SRC0_1,
        &SCUX.SEVCR1_2SRC0_1,       &SCUX.SRCIRR_2SRC0_1,   &SCUX.SRCRSEL3_CIM,
        &SCUX.DVUIR_DVU0_3,         &SCUX.VADIR_DVU0_3,     &SCUX.DVUBR_DVU0_3,
        &SCUX.DVUCR_DVU0_3,         &SCUX.ZCMCR_DVU0_3,     &SCUX.VRCTR_DVU0_3,
        &SCUX.VRPDR_DVU0_3,         &SCUX.VRDBR_DVU0_3,     &SCUX.VRWTR_DVU0_3,
        &SCUX.VOL0R_DVU0_3,         &SCUX.VOL1R_DVU0_3,     &SCUX.VOL2R_DVU0_3,
        &SCUX.VOL3R_DVU0_3,         &SCUX.VOL4R_DVU0_3,     &SCUX.VOL5R_DVU0_3,
        &SCUX.VOL6R_DVU0_3,         &SCUX.VOL7R_DVU0_3,     &SCUX.DVUER_DVU0_3,
        &SCUX.DVUSR_DVU0_3,         &SCUX.VEVMR_DVU0_3,     &SCUX.VEVCR_DVU0_3,
        &SCUX.DMATD3_CIM.UINT32,    &SCUX.DMATU3_CIM.UINT32
    }
};

/* ==== Mask data and Position of Register bits ==== */
/* STBCR8.MSTP81 */
const uint8_reg_bit_inf_t g_stbcr8_mstp81_bit_inf_tbl =
{
    0x02    ,   1       /* STBCR8.MSTP81 */
};

const uint32_reg_bit_inf_t g_scux_bit_inf_tbl[NUM_SCUX_BIT_INF] =
{
        { 0x000000FFu   ,   0   }   /* SCUX_ZCMCR_BIT_ZCEN     */
    ,   { 0x00001F00u   ,   8   }   /* SCUX_VRPDR_BIT_VRPDUP   */
    ,   { 0x0000001Fu   ,   0   }   /* SCUX_VRPDR_BIT_VRPDDW   */
    ,   { 0x000003FFu   ,   0   }   /* SCUX_VRDBR_BIT_VRDB     */
    ,   { 0x00FFFFFFu   ,   0   }   /* SCUX_VRWTR_BIT_VRWT     */
    ,   { 0x00FFFFFFu   ,   0   }   /* SCUX_VOLR_BIT_VOLVAL    */
    ,   { 0x000003FFu   ,   0   }   /* SCUX_MDBR_BIT_MIXDB     */
    ,   { 0xC0000000u   ,   30  }   /* SCUX_SSIRSEL_BIT_SISEL3 */
    ,   { 0x30000000u   ,   28  }   /* SCUX_SSIRSEL_BIT_SISEL2 */
    ,   { 0x03000000u   ,   24  }   /* SCUX_SSIRSEL_BIT_SISEL0 */
    ,   { 0x00003000u   ,   12  }   /* SCUX_SSIRSEL_BIT_SOSEL3 */
    ,   { 0x00000003u   ,   0   }   /* SCUX_SSIRSEL_BIT_SOSEL0 */
    ,   { 0x001F0000u   ,   16  }   /* SCUX_SADIR_BIT_OTBL     */
    ,   { 0x00001000u   ,   12  }   /* SCUX_SRCCR_BIT_BUFMD    */
    ,   { 0x00100000u   ,   20  }   /* SCUX_SRCCR_BIT_WATMD    */
    ,   { 0x00000001u   ,   0   }   /* SCUX_SRCCR_BIT_SRCMD    */
    ,   { 0x03FF0000u   ,   16  }   /* SCUX_BFSSR_BIT_BUFDATA  */
    ,   { 0x001F0000u   ,   16  }   /* SCUX_VADIR_BIT_OTBL     */
    ,   { 0x00000100u   ,   8   }   /* SCUX_DVUCR_BIT_VVMD     */
    ,   { 0x00000010u   ,   4   }   /* SCUX_DVUCR_BIT_VRMD     */
    ,   { 0x00000F00u   ,   8   }   /* SCUX_MVPDR_BIT_MXPDUP   */
    ,   { 0x0000000Fu   ,   0   }   /* SCUX_FUTSEL_BIT_SCKSEL  */
    ,   { 0x00000100u   ,   8   }   /* SCUX_FUTSEL_BIT_DIVEN   */
    ,   { 0x0000000Fu   ,   0   }   /* SCUX_FDTSEL_BIT_SCKSEL  */
    ,   { 0x00000100u   ,   8   }   /* SCUX_FDTSEL_BIT_DIVEN   */
    ,   { 0x00000007u   ,   0   }   /* SCUX_DVUSR_BIT_VRSTS    */
};

/* SSICTRL_CIM.SSInTX(n=0:5) */
const uint32_reg_bit_inf_t g_ssictrl_ssitx_bit_inf_tbl[NUM_SSIF_CH] =
{
        { 0x00004000u   ,   14  }   /* SSICTRL.SSI0TX */
    ,   { 0x00002000u   ,   13  }   /* SSICTRL.SSI1TX */
    ,   { 0x00001000u   ,   12  }   /* SSICTRL.SSI2TX */
    ,   { 0x40000000u   ,   30  }   /* SSICTRL.SSI3TX */
    ,   { 0x20000000u   ,   29  }   /* SSICTRL.SSI4TX */
    ,   { 0x10000000u   ,   28  }   /* SSICTRL.SSI5TX */
};

/* SSICTRL_CIM.SSInRX(n=0:5) */
const uint32_reg_bit_inf_t g_ssictrl_ssirx_bit_inf_tbl[NUM_SSIF_CH] =
{
        { 0x00000400u   ,   10  }   /* SSICTRL.SSI0RX */
    ,   { 0x00000200u   ,   9   }   /* SSICTRL.SSI1RX */
    ,   { 0x00000100u   ,   8   }   /* SSICTRL.SSI2RX */
    ,   { 0x04000000u   ,   26  }   /* SSICTRL.SSI3RX */
    ,   { 0x02000000u   ,   25  }   /* SSICTRL.SSI4RX */
    ,   { 0x01000000u   ,   24  }   /* SSICTRL.SSI5RX */
};

const uint32_reg_bit_inf_t g_dmacr_dmamdffu_bit_inf_tbl[NUM_SCUX_CH] =
{
        { 0x00000010u   ,   4   }   /* DMACR_CIM.DMAMDFFU0 */
    ,   { 0x00000020u   ,   5   }   /* DMACR_CIM.DMAMDFFU1 */
    ,   { 0x00000040u   ,   6   }   /* DMACR_CIM.DMAMDFFU2 */
    ,   { 0x00000080u   ,   7   }   /* DMACR_CIM.DMAMDFFU3 */
};

const uint32_reg_bit_inf_t g_dmacr_dmamdffd_bit_inf_tbl[NUM_SCUX_CH] =
{
        { 0x00000001u   ,   0   }   /* DMACR_CIM.DMAMDFFD0 */
    ,   { 0x00000002u   ,   1   }   /* DMACR_CIM.DMAMDFFD1 */
    ,   { 0x00000004u   ,   2   }   /* DMACR_CIM.DMAMDFFD2 */
    ,   { 0x00000008u   ,   3   }   /* DMACR_CIM.DMAMDFFD3 */
};

const uint32_reg_bit_inf_t g_rsel_place_bit_inf_tbl[NUM_AUDIO_CH] =
{
        { 0x00000007u   ,   0   }   /* SRCRSELn_CIM/MIXRSEL_CIM.PLACE0 */
    ,   { 0x00000070u   ,   4   }   /* SRCRSELn_CIM/MIXRSEL_CIM.PLACE1 */
    ,   { 0x00000700u   ,   8   }   /* SRCRSELn_CIM/MIXRSEL_CIM.PLACE2 */
    ,   { 0x00007000u   ,   12  }   /* SRCRSELn_CIM/MIXRSEL_CIM.PLACE3 */
    ,   { 0x00070000u   ,   16  }   /* SRCRSELn_CIM/MIXRSEL_CIM.PLACE4 */
    ,   { 0x00700000u   ,   20  }   /* SRCRSELn_CIM/MIXRSEL_CIM.PLACE5 */
    ,   { 0x07000000u   ,   24  }   /* SRCRSELn_CIM/MIXRSEL_CIM.PLACE6 */
    ,   { 0x70000000u   ,   28  }   /* SRCRSELn_CIM/MIXRSEL_CIM.PLACE7 */
};

/* End of File */
