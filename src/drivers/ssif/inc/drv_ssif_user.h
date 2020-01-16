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
/******************************************************************************
* File Name    : drv_ssif_user.h
* $Rev: 475 $
* $Date:: 2013-10-03 17:32:50 +0900#$
* Description  : Sample Data
******************************************************************************/
#ifndef DRV_SSIF_USER_H
#define DRV_SSIF_USER_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== SSI Channel0 ==== */
/* SSI0 Control Register Setting */
#define SSI_SSICR0_CKS_VALUE        (0x00000000uL)
#define SSI_SSICR0_CHNL_VALUE       (0x00000000uL)
#define SSI_SSICR0_DWL_VALUE        (0x00080000uL)
#define SSI_SSICR0_SWL_VALUE        (0x00030000uL)
#define SSI_SSICR0_SCKD_VALUE       (0x00008000uL)
#define SSI_SSICR0_SWSD_VALUE       (0x00004000uL)
#define SSI_SSICR0_SCKP_VALUE       (0x00000000uL)
#define SSI_SSICR0_SWSP_VALUE       (0x00000000uL)
#define SSI_SSICR0_SPDP_VALUE       (0x00000000uL)
#define SSI_SSICR0_SDTA_VALUE       (0x00000000uL)
#define SSI_SSICR0_PDTA_VALUE       (0x00000000uL)
#define SSI_SSICR0_DEL_VALUE        (0x00000100uL)      /* for RSK */
#define SSI_SSICR0_CKDV_VALUE       (0x00000030uL)
/*  [30]    CKS     : B'0    : AUDIO_X1 input                                                   */
/*  [23:22] CHNL    : B'00   : 1 channel / system word                                          */
/*  [21:19] DWL     : B'001  : 16 bit / data word                                               */
/*  [18:16] SWL     : B'011  : 32 bit / system word                                             */
/*  [15]    SCKD    : B'1    : Serial Bit Clock Direction:master mode                           */
/*  [14]    SWSD    : B'1    : Serial WS Direction:master mode                                  */
/*  [13]    SCKP    : B'0    : SSIWS and SSIDATA change at the SSISCK falling edge              */
/*  [12]    SWSP    : B'0    : SSIWS is low for 1st channel, high for 2nd channel               */
/*  [11]    SPDP    : B'0    : Padding bits are low                                             */
/*  [10]    SDTA    : B'0    : Tx and Rx in the order of serial data and padding bits           */
/*  [9]     PDTA    : B'0    : The lower bits of parallel data(SSITDR, SSIRDR) are transferred  */
/*                             prior to the upper bits                                          */
/*  [8]     DEL     : B'0    : 1 clock cycle delay between SSIWS and SSIDATA                    */
/*  [7:4]   CKDV    : B'0011 : AUDIO PHY/8 (64FS,AUDIO_X1@22.5792MHz/32bit system word)         */
#define SSI_SSICR0_USER_INIT_VALUE  (SSI_SSICR0_CKS_VALUE  | \
                                     SSI_SSICR0_CHNL_VALUE | \
                                     SSI_SSICR0_DWL_VALUE  | \
                                     SSI_SSICR0_SWL_VALUE  | \
                                     SSI_SSICR0_SCKD_VALUE | \
                                     SSI_SSICR0_SWSD_VALUE | \
                                     SSI_SSICR0_SCKP_VALUE | \
                                     SSI_SSICR0_SWSP_VALUE | \
                                     SSI_SSICR0_SPDP_VALUE | \
                                     SSI_SSICR0_SDTA_VALUE | \
                                     SSI_SSICR0_PDTA_VALUE | \
                                     SSI_SSICR0_DEL_VALUE  | \
                                     SSI_SSICR0_CKDV_VALUE)

/* SSI0 FIFO Control Register Setting */
#define SSI_SSIFCR0_TTRG_VALUE (0x00000000uL)
#define SSI_SSIFCR0_RTRG_VALUE (0x00000000uL)
/*  [7:6] TTRG  : B'00 : Transmit Data Trigger Number:7                                         */
/*  [5:4] RTRG  : B'00 : Receive Data Trigger Number:1                                          */
#define SSI_SSIFCR0_USER_INIT_VALUE (SSI_SSIFCR0_TTRG_VALUE  | SSI_SSIFCR0_RTRG_VALUE)

/* SSI0 TDM Mode Register Setting */
#define SSI_SSITDMR0_CONT_VALUE (0x00000100uL)
/*  [8] CONT  : B'1 : WS Continue Mode:Enables WS continue mode                                 */
#define SSI_SSITDMR0_USER_INIT_VALUE        (SSI_SSITDMR0_CONT_VALUE)

/* ==== SSI Channel1 ==== */
/* SSI1 Control Register Setting */
#define SSI_SSICR1_CKS_VALUE        (0x00000000uL)
#define SSI_SSICR1_CHNL_VALUE       (0x00000000uL)
#define SSI_SSICR1_DWL_VALUE        (0x00080000uL)
#define SSI_SSICR1_SWL_VALUE        (0x00030000uL)
#define SSI_SSICR1_SCKD_VALUE       (0x00008000uL)
#define SSI_SSICR1_SWSD_VALUE       (0x00004000uL)
#define SSI_SSICR1_SCKP_VALUE       (0x00000000uL)
#define SSI_SSICR1_SWSP_VALUE       (0x00000000uL)
#define SSI_SSICR1_SPDP_VALUE       (0x00000000uL)
#define SSI_SSICR1_SDTA_VALUE       (0x00000000uL)
#define SSI_SSICR1_PDTA_VALUE       (0x00000000uL)
#define SSI_SSICR1_DEL_VALUE        (0x00000000uL)
#define SSI_SSICR1_CKDV_VALUE       (0x00000030uL)
/*  [30]    CKS     : B'0    : AUDIO_X1 input                                                   */
/*  [23:22] CHNL    : B'00   : 1 channel / system word                                          */
/*  [21:19] DWL     : B'001  : 16 bit / data word                                               */
/*  [18:16] SWL     : B'011  : 32 bit / system word                                             */
/*  [15]    SCKD    : B'1    : Serial Bit Clock Direction:master mode                           */
/*  [14]    SWSD    : B'1    : Serial WS Direction:master mode                                  */
/*  [13]    SCKP    : B'0    : SSIWS and SSIDATA change at the SSISCK falling edge              */
/*  [12]    SWSP    : B'0    : SSIWS is low for 1st channel, high for 2nd channel               */
/*  [11]    SPDP    : B'0    : Padding bits are low                                             */
/*  [10]    SDTA    : B'0    : Tx and Rx in the order of serial data and padding bits           */
/*  [9]     PDTA    : B'0    : The lower bits of parallel data(SSITDR, SSIRDR) are transferred  */
/*                             prior to the upper bits                                          */
/*  [8]     DEL     : B'0    : 1 clock cycle delay between SSIWS and SSIDATA                    */
/*  [7:4]   CKDV    : B'0011 : AUDIO PHY/ 8 (64FS,AUDIO_X1@22.5792MHz/32bit system word)         */
#define SSI_SSICR1_USER_INIT_VALUE  (SSI_SSICR1_CKS_VALUE  | \
                                     SSI_SSICR1_CHNL_VALUE | \
                                     SSI_SSICR1_DWL_VALUE  | \
                                     SSI_SSICR1_SWL_VALUE  | \
                                     SSI_SSICR1_SCKD_VALUE | \
                                     SSI_SSICR1_SWSD_VALUE | \
                                     SSI_SSICR1_SCKP_VALUE | \
                                     SSI_SSICR1_SWSP_VALUE | \
                                     SSI_SSICR1_SPDP_VALUE | \
                                     SSI_SSICR1_SDTA_VALUE | \
                                     SSI_SSICR1_PDTA_VALUE | \
                                     SSI_SSICR1_DEL_VALUE  | \
                                     SSI_SSICR1_CKDV_VALUE)

/* SSI0 FIF1 Control Register Setting */
#define SSI_SSIFCR1_TTRG_VALUE  (0x00000000uL)
#define SSI_SSIFCR1_RTRG_VALUE  (0x00000000uL)
/*  [7:6] TTRG  : B'00 : Transmit Data Trigger Number:7                                         */
/*  [5:4] RTRG  : B'00 : Receive Data Trigger Number:1                                          */
#define SSI_SSIFCR1_USER_INIT_VALUE     (SSI_SSIFCR1_TTRG_VALUE  | SSI_SSIFCR1_RTRG_VALUE)

/* SSI1 TDM Mode Register Setting */
#define SSI_SSITDMR1_CONT_VALUE (0x00000000uL)
/*  [8] CONT  : B'0 : WS Continue Mode:Disables WS continue mode                                */
#define SSI_SSITDMR1_USER_INIT_VALUE        (SSI_SSITDMR1_CONT_VALUE)

/* ==== SSI Channel2 ==== */
/* SSI2 Control Register Setting */
#define SSI_SSICR2_CKS_VALUE        (0x00000000uL)
#define SSI_SSICR2_CHNL_VALUE       (0x00000000uL)
#define SSI_SSICR2_DWL_VALUE        (0x00080000uL)
#define SSI_SSICR2_SWL_VALUE        (0x00030000uL)
#define SSI_SSICR2_SCKD_VALUE       (0x00008000uL)
#define SSI_SSICR2_SWSD_VALUE       (0x00004000uL)
#define SSI_SSICR2_SCKP_VALUE       (0x00000000uL)
#define SSI_SSICR2_SWSP_VALUE       (0x00000000uL)
#define SSI_SSICR2_SPDP_VALUE       (0x00000000uL)
#define SSI_SSICR2_SDTA_VALUE       (0x00000000uL)
#define SSI_SSICR2_PDTA_VALUE       (0x00000000uL)
#define SSI_SSICR2_DEL_VALUE        (0x00000000uL)
#define SSI_SSICR2_CKDV_VALUE       (0x00000030uL)
/*  [30]    CKS     : B'0    : AUDIO_X1 input                                                   */
/*  [23:22] CHNL    : B'00   : 1 channel / system word                                          */
/*  [21:19] DWL     : B'001  : 16 bit / data word                                               */
/*  [18:16] SWL     : B'011  : 32 bit / system word                                             */
/*  [15]    SCKD    : B'1    : Serial Bit Clock Direction:master mode                           */
/*  [14]    SWSD    : B'1    : Serial WS Direction:master mode                                  */
/*  [13]    SCKP    : B'0    : SSIWS and SSIDATA change at the SSISCK falling edge              */
/*  [12]    SWSP    : B'0    : SSIWS is low for 1st channel, high for 2nd channel               */
/*  [11]    SPDP    : B'0    : Padding bits are low                                             */
/*  [10]    SDTA    : B'0    : Tx and Rx in the order of serial data and padding bits           */
/*  [9]     PDTA    : B'0    : The lower bits of parallel data(SSITDR, SSIRDR) are transferred  */
/*                             prior to the upper bits                                          */
/*  [8]     DEL     : B'0    : 1 clock cycle delay between SSIWS and SSIDATA                    */
/*  [7:4]   CKDV    : B'0011 : AUDIO PHY/ 8 (64FS,AUDIO_X1@22.5792MHz/32bit system word)         */
#define SSI_SSICR2_USER_INIT_VALUE  (SSI_SSICR2_CKS_VALUE  | \
                                     SSI_SSICR2_CHNL_VALUE | \
                                     SSI_SSICR2_DWL_VALUE  | \
                                     SSI_SSICR2_SWL_VALUE  | \
                                     SSI_SSICR2_SCKD_VALUE | \
                                     SSI_SSICR2_SWSD_VALUE | \
                                     SSI_SSICR2_SCKP_VALUE | \
                                     SSI_SSICR2_SWSP_VALUE | \
                                     SSI_SSICR2_SPDP_VALUE | \
                                     SSI_SSICR2_SDTA_VALUE | \
                                     SSI_SSICR2_PDTA_VALUE | \
                                     SSI_SSICR2_DEL_VALUE  | \
                                     SSI_SSICR2_CKDV_VALUE)

/* SSI2 FIFO Control Register Setting */
#define SSI_SSIFCR2_TTRG_VALUE  (0x00000000uL)
#define SSI_SSIFCR2_RTRG_VALUE  (0x00000000uL)
/*  [7:6] TTRG  : B'00 : Transmit Data Trigger Number:7                                         */
/*  [5:4] RTRG  : B'00 : Receive Data Trigger Number:1                                          */
#define SSI_SSIFCR2_USER_INIT_VALUE     (SSI_SSIFCR2_TTRG_VALUE  | SSI_SSIFCR2_RTRG_VALUE)

/* SSI2 TDM Mode Register Setting */
#define SSI_SSITDMR2_CONT_VALUE (0x00000000uL)
/*  [8] CONT  : B'0 : WS Continue Mode:Disables WS continue mode                                */
#define SSI_SSITDMR2_USER_INIT_VALUE        (SSI_SSITDMR2_CONT_VALUE)

/* ==== SSI Channel3 ==== */
/* SSI3 Control Register Setting */
#define SSI_SSICR3_CKS_VALUE        (0x00000000uL)
#define SSI_SSICR3_CHNL_VALUE       (0x00000000uL)
#define SSI_SSICR3_DWL_VALUE        (0x00080000uL)
#define SSI_SSICR3_SWL_VALUE        (0x00030000uL)
#define SSI_SSICR3_SCKD_VALUE       (0x00008000uL)
#define SSI_SSICR3_SWSD_VALUE       (0x00004000uL)
#define SSI_SSICR3_SCKP_VALUE       (0x00000000uL)
#define SSI_SSICR3_SWSP_VALUE       (0x00001000uL)
#define SSI_SSICR3_SPDP_VALUE       (0x00000000uL)
#define SSI_SSICR3_SDTA_VALUE       (0x00000400uL)
#define SSI_SSICR3_PDTA_VALUE       (0x00000000uL)
#define SSI_SSICR3_DEL_VALUE        (0x00000100uL)
#define SSI_SSICR3_CKDV_VALUE       (0x00000030uL)
/*  [30]    CKS     : B'0    : AUDIO_X1 input                                                   */
/*  [23:22] CHNL    : B'00   : 1 channel / system word                                          */
/*  [21:19] DWL     : B'001  : 16 bit / data word                                               */
/*  [18:16] SWL     : B'011  : 32 bit / system word                                             */
/*  [15]    SCKD    : B'1    : Serial Bit Clock Direction:master mode                           */
/*  [14]    SWSD    : B'1    : Serial WS Direction:master mode                                  */
/*  [13]    SCKP    : B'0    : SSIWS and SSIDATA change at the SSISCK falling edge              */
/*  [12]    SWSP    : B'1    : SSIWS is high for 1st channel, low for 2nd channel               */
/*  [11]    SPDP    : B'0    : Padding bits are low                                             */
/*  [10]    SDTA    : B'1    : Tx and Rx in the order of padding bits and serial data           */
/*  [9]     PDTA    : B'0    : The lower bits of parallel data(SSITDR, SSIRDR) are transferred  */
/*                             prior to the upper bits                                          */
/*  [8]     DEL     : B'1    : Non delay between SSIWS and SSIDATA                              */
/*  [7:4]   CKDV    : B'0011 : AUDIO PHY/ 8 (64FS,AUDIO_X1@22.5792MHz/32bit system word)         */
#define SSI_SSICR3_USER_INIT_VALUE  (SSI_SSICR3_CKS_VALUE  | \
                                     SSI_SSICR3_CHNL_VALUE | \
                                     SSI_SSICR3_DWL_VALUE  | \
                                     SSI_SSICR3_SWL_VALUE  | \
                                     SSI_SSICR3_SCKD_VALUE | \
                                     SSI_SSICR3_SWSD_VALUE | \
                                     SSI_SSICR3_SCKP_VALUE | \
                                     SSI_SSICR3_SWSP_VALUE | \
                                     SSI_SSICR3_SPDP_VALUE | \
                                     SSI_SSICR3_SDTA_VALUE | \
                                     SSI_SSICR3_PDTA_VALUE | \
                                     SSI_SSICR3_DEL_VALUE  | \
                                     SSI_SSICR3_CKDV_VALUE)

/* SSI3 FIFO Control Register Setting */
#define SSI_SSIFCR3_TTRG_VALUE  (0x00000000uL)
#define SSI_SSIFCR3_RTRG_VALUE  (0x00000000uL)
/*  [7:6] TTRG  : B'00 : Transmit Data Trigger Number:7                                         */
/*  [5:4] RTRG  : B'00 : Receive Data Trigger Number:1                                          */
#define SSI_SSIFCR3_USER_INIT_VALUE     (SSI_SSIFCR3_TTRG_VALUE  | SSI_SSIFCR3_RTRG_VALUE)

/* SSI3 TDM Mode Register Setting */
#define SSI_SSITDMR3_CONT_VALUE (0x00000100uL)
/*  [8] CONT  : B'1 : WS Continue Mode:Enables WS continue mode                                 */
#define SSI_SSITDMR3_USER_INIT_VALUE        (SSI_SSITDMR3_CONT_VALUE)

/* ==== SSI Channel4 ==== */
/* SSI4 Control Register Setting */
#define SSI_SSICR4_CKS_VALUE        (0x00000000uL)
#define SSI_SSICR4_CHNL_VALUE       (0x00000000uL)
#define SSI_SSICR4_DWL_VALUE        (0x00080000uL)
#define SSI_SSICR4_SWL_VALUE        (0x00030000uL)
#define SSI_SSICR4_SCKD_VALUE       (0x00008000uL)
#define SSI_SSICR4_SWSD_VALUE       (0x00004000uL)
#define SSI_SSICR4_SCKP_VALUE       (0x00000000uL)
#define SSI_SSICR4_SWSP_VALUE       (0x00001000uL)
#define SSI_SSICR4_SPDP_VALUE       (0x00000000uL)
#define SSI_SSICR4_SDTA_VALUE       (0x00000400uL)
#define SSI_SSICR4_PDTA_VALUE       (0x00000000uL)
#define SSI_SSICR4_DEL_VALUE        (0x00000100uL)
#define SSI_SSICR4_CKDV_VALUE       (0x00000030uL)
/*  [30]    CKS     : B'0    : AUDIO_X1 input                                                   */
/*  [23:22] CHNL    : B'00   : 1 channel / system word                                          */
/*  [21:19] DWL     : B'001  : 16 bit / data word                                               */
/*  [18:16] SWL     : B'011  : 32 bit / system word                                             */
/*  [15]    SCKD    : B'1    : Serial Bit Clock Direction:master mode                           */
/*  [14]    SWSD    : B'1    : Serial WS Direction:master mode                                  */
/*  [13]    SCKP    : B'0    : SSIWS and SSIDATA change at the SSISCK falling edge              */
/*  [12]    SWSP    : B'1    : SSIWS is high for 1st channel, low for 2nd channel               */
/*  [11]    SPDP    : B'0    : Padding bits are low                                             */
/*  [10]    SDTA    : B'1    : Tx and Rx in the order of padding bits and serial data           */
/*  [9]     PDTA    : B'0    : The lower bits of parallel data(SSITDR, SSIRDR) are transferred  */
/*                             prior to the upper bits                                          */
/*  [8]     DEL     : B'1    : Non delay between SSIWS and SSIDATA                              */
/*  [7:4]   CKDV    : B'0011 : AUDIO PHY/ 8 (64FS,AUDIO_X1@22.5792MHz/32bit system word)         */
#define SSI_SSICR4_USER_INIT_VALUE  (SSI_SSICR4_CKS_VALUE  | \
                                     SSI_SSICR4_CHNL_VALUE | \
                                     SSI_SSICR4_DWL_VALUE  | \
                                     SSI_SSICR4_SWL_VALUE  | \
                                     SSI_SSICR4_SCKD_VALUE | \
                                     SSI_SSICR4_SWSD_VALUE | \
                                     SSI_SSICR4_SCKP_VALUE | \
                                     SSI_SSICR4_SWSP_VALUE | \
                                     SSI_SSICR4_SPDP_VALUE | \
                                     SSI_SSICR4_SDTA_VALUE | \
                                     SSI_SSICR4_PDTA_VALUE | \
                                     SSI_SSICR4_DEL_VALUE  | \
                                     SSI_SSICR4_CKDV_VALUE)

/* SSI4 FIFO Control Register Setting */
#define SSI_SSIFCR4_TTRG_VALUE  (0x00000000uL)
#define SSI_SSIFCR4_RTRG_VALUE  (0x00000000uL)
/*  [7:6] TTRG  : B'00 : Transmit Data Trigger Number:7                                         */
/*  [5:4] RTRG  : B'00 : Receive Data Trigger Number:1                                          */
#define SSI_SSIFCR4_USER_INIT_VALUE     (SSI_SSIFCR4_TTRG_VALUE  | SSI_SSIFCR4_RTRG_VALUE)

/* SSI4 TDM Mode Register Setting */
#define SSI_SSITDMR4_CONT_VALUE (0x00000100uL)
/*  [8] CONT  : B'1 : WS Continue Mode:Enables WS continue mode                                 */
#define SSI_SSITDMR4_USER_INIT_VALUE        (SSI_SSITDMR4_CONT_VALUE)

/* ==== SSI Channel5 ==== */
/* SSI5 Control Register Setting */
#define SSI_SSICR5_CKS_VALUE        (0x00000000uL)
#define SSI_SSICR5_CHNL_VALUE       (0x00000000uL)
#define SSI_SSICR5_DWL_VALUE        (0x00080000uL)
#define SSI_SSICR5_SWL_VALUE        (0x00030000uL)
#define SSI_SSICR5_SCKD_VALUE       (0x00008000uL)
#define SSI_SSICR5_SWSD_VALUE       (0x00004000uL)
#define SSI_SSICR5_SCKP_VALUE       (0x00000000uL)
#define SSI_SSICR5_SWSP_VALUE       (0x00000000uL)
#define SSI_SSICR5_SPDP_VALUE       (0x00000000uL)
#define SSI_SSICR5_SDTA_VALUE       (0x00000000uL)
#define SSI_SSICR5_PDTA_VALUE       (0x00000000uL)
#define SSI_SSICR5_DEL_VALUE        (0x00000000uL)
#define SSI_SSICR5_CKDV_VALUE       (0x00000030uL)
/*  [30]    CKS     : B'0    : AUDIO_X1 input                                                   */
/*  [23:22] CHNL    : B'00   : 1 channel / system word                                          */
/*  [21:19] DWL     : B'001  : 16 bit / data word                                               */
/*  [18:16] SWL     : B'011  : 32 bit / system word                                             */
/*  [15]    SCKD    : B'1    : Serial Bit Clock Direction:master mode                           */
/*  [14]    SWSD    : B'1    : Serial WS Direction:master mode                                  */
/*  [13]    SCKP    : B'0    : SSIWS and SSIDATA change at the SSISCK falling edge              */
/*  [12]    SWSP    : B'0    : SSIWS is low for 1st channel, high for 2nd channel               */
/*  [11]    SPDP    : B'0    : Padding bits are low                                             */
/*  [10]    SDTA    : B'0    : Tx and Rx in the order of serial data and padding bits           */
/*  [9]     PDTA    : B'0    : The lower bits of parallel data(SSITDR, SSIRDR) are transferred  */
/*                             prior to the upper bits                                          */
/*  [8]     DEL     : B'0    : 1 clock cycle delay between SSIWS and SSIDATA                    */
/*  [7:4]   CKDV    : B'0011 : AUDIO PHY/ 8 (64FS,AUDIO_X1@22.5792MHz/32bit system word)         */
#define SSI_SSICR5_USER_INIT_VALUE  (SSI_SSICR5_CKS_VALUE  | \
                                     SSI_SSICR5_CHNL_VALUE | \
                                     SSI_SSICR5_DWL_VALUE  | \
                                     SSI_SSICR5_SWL_VALUE  | \
                                     SSI_SSICR5_SCKD_VALUE | \
                                     SSI_SSICR5_SWSD_VALUE | \
                                     SSI_SSICR5_SCKP_VALUE | \
                                     SSI_SSICR5_SWSP_VALUE | \
                                     SSI_SSICR5_SPDP_VALUE | \
                                     SSI_SSICR5_SDTA_VALUE | \
                                     SSI_SSICR5_PDTA_VALUE | \
                                     SSI_SSICR5_DEL_VALUE  | \
                                     SSI_SSICR5_CKDV_VALUE)

/* SSI5 FIFO Control Register Setting */
#define SSI_SSIFCR5_TTRG_VALUE  (0x00000000uL)
#define SSI_SSIFCR5_RTRG_VALUE  (0x00000000uL)
/*  [7:6] TTRG  : B'00 : Transmit Data Trigger Number:7                                         */
/*  [5:4] RTRG  : B'00 : Receive Data Trigger Number:1                                          */
#define SSI_SSIFCR5_USER_INIT_VALUE     (SSI_SSIFCR5_TTRG_VALUE  | SSI_SSIFCR5_RTRG_VALUE)

/* SSI5 TDM Mode Register Setting */
#define SSI_SSITDMR5_CONT_VALUE (0x00000000uL)
/*  [8] CONT  : B'0 : WS Continue Mode:Disables WS continue mode                                */
#define SSI_SSITDMR5_USER_INIT_VALUE        (SSI_SSITDMR5_CONT_VALUE)

/* DMA Transfer End INT LVL */
#define DMA_TRNEND_INT_LV   (5)

/* SSI ERR INT LVL */
#define SSI_ERR_INT_LV      (4)

/* SSI IDLE WAIT TIME */
/* This definition becomes the definition of the idle waiting   */
/* at the time of the SSI stops.                                */
/* Please adjust time for idle waiting by user.                 */
#define SSI_IDLE_WAIT       (45u)


/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
void SSI_UserdefSsi0PinSet(void);   /* SSI0 Pin Set */
void SSI_UserdefSsi1PinSet(void);   /* SSI1 Pin Set */
void SSI_UserdefSsi2PinSet(void);   /* SSI2 Pin Set */
void SSI_UserdefSsi3PinSet(void);   /* SSI3 Pin Set */
void SSI_UserdefSsi4PinSet(void);   /* SSI4 Pin Set */
void SSI_UserdefSsi5PinSet(void);   /* SSI5 Pin Set */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DRV_SSIF_USER_H */

/* End of File */
