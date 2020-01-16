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
* File Name    : sample_riic_ak4353.h
* $Rev: 474 $
* $Date:: 2013-10-03 17:13:02 +0900#$
* Description  : DAC AK4353 control header file
******************************************************************************/
/* Guards against multiple inclusion */
#ifndef DEV_DAC_AK4353_H
#define DEV_DAC_AK4353_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== AK4353 Device driver API macro ==== */
/* ---- FS select ---- */
typedef enum ak4353_fs
{
        AUDIO_FS_22_05KHZ
    ,   AUDIO_FS_16KHZ
    ,   AUDIO_FS_24KHZ
    ,   AUDIO_FS_32KHZ
    ,   AUDIO_FS_44_1KHZ
    ,   AUDIO_FS_48KHZ
} ak4353_fs_t;

/* ---- Channel select ---- */
typedef enum ak4353_ch
{
        AK4353_CHANNEL1
    ,   AK4353_CHANNEL2
    ,   MAX_DAC_CHANNEL
}ak4353_ch_t;

/* ==== AK4353 internal macro ==== */
/* ---- Chip address ---- */
#define DAC1_CHIP_ADDR  (0x26)      /* DAC1(SSIF3 Connect) chip address */
#define DAC2_CHIP_ADDR  (0x22)      /* DAC2(SSIF4 Connect) chip address */

/* ---- Register address ---- */
#define DAC_REG_ADDR0   (0x00)      /* Addr:0 for Control-1        */
#define DAC_REG_ADDR1   (0x01)      /* Addr:1 for Control-2        */
#define DAC_REG_ADDR2   (0x02)      /* Addr:2 for Control-3        */
#define DAC_REG_ADDR3   (0x03)      /* Addr:3 for Lch ATT          */
#define DAC_REG_ADDR4   (0x04)      /* Addr:4 for Rch ATT          */
#define DAC_REG_ADDR5   (0x05)      /* Addr:5 for TX               */
#define DAC_REG_ADDR6   (0x06)      /* Addr:6 for Channel Status 1 */
#define DAC_REG_ADDR7   (0x07)      /* Addr:7 for Channel Status 2 */

/* ---- Control data ---- */
#define DAC_RSTN        (1)         /* Control1/2 reset enbale */
#define DAC_RST_MASK    (0xfe)      /* Control1/2 reset bit mask */
#define DAC_DIF_MODE0   (0<<1)      /* Control1 DIF mode 0 */
#define DAC_DIF_MODE1   (1<<1)      /* Control1 DIF mode 1 */
#define DAC_DIF_MODE2   (2<<1)      /* Control1 DIF mode 2 */
#define DAC_DIF_MODE3   (3<<1)      /* Control1 DIF mode 3 */
#define DAC_DIF_MODE4   (4<<1)      /* Control1 DIF mode 4 */
#define DAC_DIF_MODE5   (5<<1)      /* Control1 DIF mode 5 */
#define DAC_CKS_256FS   (0<<1)      /* Control2 CKS select 256fs (Normal Speed) */
#define DAC_CKS_384FS   (2<<1)      /* Control2 CKS select 384fs (Normal Speed) */
#define DAC_CKS_512FS   (4<<1)      /* Control2 CKS select 512fs (Normal Speed) */
#define DAC_CKS_768FS   (6<<1)      /* Control2 CKS select 768fs (Normal Speed) */
#define DAC_DFS_NS      (0<<4)      /* Control2 DFS select Normal Speed */
#define DAC_DFS_DS      (1<<4)      /* Control2 DFS select Double Speed */
#define DAC_DFS_HS      (3<<4)      /* Control2 DFS select Half Speed */
#define DAC_SMUTE_ON    (1)         /* Control3 SMUTE software mute on */
#define DAC_DEM_OFF     (1<<2)      /* Control3 DEM1/0 select OFF */
#define DAC_ATC_OFF     (0<<1)      /* Control3 ATC Lch/Rch ATT */
#define DAC_ATC_ON      (1<<1)      /* Control3 ATC use Lch ATT */
#define DAC_PL_MUTE     (0<<4)      /* Control3 PL mixing mode mute */
#define DAC_PL_REVERSE  (6<<4)      /* Control3 PL mixing mode R/L stereo */
#define DAC_PL_STEREO   (9<<4)      /* Control3 PL mixing mode L/R stereo */
#define DAC_PL_MONO     (0xf<<4)    /* Control3 PL mixing mode mono */

#define DAC_FS_16KHZ    (DAC_CKS_384FS|DAC_DFS_HS)  /* 768fs half speed 12.2880MHz */
#define DAC_FS_32KHZ    (DAC_CKS_384FS|DAC_DFS_NS)  /* 384fs normal speed 12.2880MHz */
#define DAC_FS_22_05KHZ (DAC_CKS_512FS|DAC_DFS_HS)  /* 1024fs half speed   22.5792MHz */
#define DAC_FS_44_1KHZ  (DAC_CKS_512FS|DAC_DFS_NS)  /* 512fs  normal speed 22.5792MHz */
#define DAC_FS_24KHZ    (DAC_CKS_256FS|DAC_DFS_HS)  /* 512fs half speed 12.2880MHz */
#define DAC_FS_48KHZ    (DAC_CKS_256FS|DAC_DFS_NS)  /* 256fs normal speed 12.2880MHz */

#define DEFAULT_ATT     (0xFF)  /* 0dB */

/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Exported global variables
******************************************************************************/
void R_DAC_AK4353PowerDown (void);
int32_t R_DAC_AK4353Init (const ak4353_ch_t dac_ch, const ak4353_fs_t fs);

//Generallai
 void Sample_RIIC_Ri0_Interrupt(uint32_t int_sense);
 void Sample_RIIC_Ti0_Interrupt(uint32_t int_sense);
 void Sample_RIIC_Tei0_Interrupt(uint32_t int_sense);
void Sample_RIIC_Ri2_Interrupt(uint32_t int_sense);
void Sample_RIIC_Ti2_Interrupt(uint32_t int_sense);
void Sample_RIIC_Tei2_Interrupt(uint32_t int_sense);
 void Sample_RIIC_Ri3_Interrupt(uint32_t int_sense);
 void Sample_RIIC_Ti3_Interrupt(uint32_t int_sense);
 void Sample_RIIC_Tei3_Interrupt(uint32_t int_sense);

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DEV_DAC_AK4353_H */

/* End of File */
