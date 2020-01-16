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
* File Name    : drv_scux_register.h
* $Rev: 729 $
* $Date:: 2014-06-11 17:44:40 +0900#$
* Description  : This module is header for sample of SCUX driver.
******************************************************************************/

#ifndef DRV_SCUX_REGISTER_H
#define DRV_SCUX_REGISTER_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "drv_scux.h"
#include "reg_bit_access.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/******************************************************************************
Macro definitions
******************************************************************************/
/* Number of SSIF channel */
#define NUM_SSIF_CH         (6u)

/******************************************************************************
Typedef definitions
******************************************************************************/
/* ==== SCUX register address list ==== */
typedef struct scux_reg_address
{
    volatile uint32_t   *ipcir;
    volatile uint32_t   *ipslr;
    volatile uint32_t   *opcir;
    volatile uint32_t   *opslr;
    volatile uint32_t   *ffdir;
    volatile uint32_t   *fdair;
    volatile uint32_t   *drqsr;
    volatile uint32_t   *ffdpr;
    volatile uint32_t   *ffdbr;
    volatile uint32_t   *devmr;
    volatile uint32_t   *devcr;
    volatile uint32_t   *fdtsel;
    volatile uint32_t   *ffuir;
    volatile uint32_t   *fuair;
    volatile uint32_t   *urqsr;
    volatile uint32_t   *ffupr;
    volatile uint32_t   *uevmr;
    volatile uint32_t   *uevcr;
    volatile uint32_t   *futsel;
    volatile uint32_t   *srcir;
    volatile uint32_t   *sadir;
    volatile uint32_t   *srcbr;
    volatile uint32_t   *ifscr;
    volatile uint32_t   *ifsvr;
    volatile uint32_t   *srccr;
    volatile uint32_t   *mnfsr;
    volatile uint32_t   *bfssr;
    volatile uint32_t   *sc2sr;
    volatile uint32_t   *watsr;
    volatile uint32_t   *sevmr;
    volatile uint32_t   *sevcr;
    volatile uint32_t   *srcirr;
    volatile uint32_t   *srcrsel;
    volatile uint32_t   *dvuir;
    volatile uint32_t   *vadir;
    volatile uint32_t   *dvubr;
    volatile uint32_t   *dvucr;
    volatile uint32_t   *zcmcr;
    volatile uint32_t   *vrctr;
    volatile uint32_t   *vrpdr;
    volatile uint32_t   *vrdbr;
    volatile uint32_t   *vrwtr;
    volatile uint32_t   *vol0r;
    volatile uint32_t   *vol1r;
    volatile uint32_t   *vol2r;
    volatile uint32_t   *vol3r;
    volatile uint32_t   *vol4r;
    volatile uint32_t   *vol5r;
    volatile uint32_t   *vol6r;
    volatile uint32_t   *vol7r;
    volatile uint32_t   *dvuer;
    volatile uint32_t   *dvusr;
    volatile uint32_t   *vevmr;
    volatile uint32_t   *vevcr;
    volatile uint32_t   *dmatd;
    volatile uint32_t   *dmatu;
} scux_reg_address_t;

typedef enum scux_bit_inf
{
        SCUX_ZCMCR_BIT_ZCEN
    ,   SCUX_VRPDR_BIT_VRPDUP
    ,   SCUX_VRPDR_BIT_VRPDDW
    ,   SCUX_VRDBR_BIT_VRDB
    ,   SCUX_VRWTR_BIT_VRWT
    ,   SCUX_VOLR_BIT_VOLVAL
    ,   SCUX_MDBR_BIT_MIXDB
    ,   SCUX_SSIRSEL_BIT_SISEL3
    ,   SCUX_SSIRSEL_BIT_SISEL2
    ,   SCUX_SSIRSEL_BIT_SISEL0
    ,   SCUX_SSIRSEL_BIT_SOSEL3
    ,   SCUX_SSIRSEL_BIT_SOSEL0
    ,   SCUX_SADIR_BIT_OTBL
    ,   SCUX_SRCCR_BIT_BUFMD
    ,   SCUX_SRCCR_BIT_WATMD
    ,   SCUX_SRCCR_BIT_SRCMD
    ,   SCUX_BFSSR_BIT_BUFDATA
    ,   SCUX_VADIR_BIT_OTBL
    ,   SCUX_DVUCR_BIT_VVMD
    ,   SCUX_DVUCR_BIT_VRMD
    ,   SCUX_MVPDR_BIT_MXPDUP
    ,   SCUX_FUTSEL_BIT_SCKSEL
    ,   SCUX_FUTSEL_BIT_DIVEN
    ,   SCUX_FDTSEL_BIT_SCKSEL
    ,   SCUX_FDTSEL_BIT_DIVEN
    ,   SCUX_DVUSR_BIT_VRSTS
    ,   NUM_SCUX_BIT_INF
} scux_bit_inf_t;

/******************************************************************************
Exported global variables
******************************************************************************/
extern const scux_reg_address_t g_scux_reg_address_tbl[NUM_SCUX_CH];
extern const uint8_reg_bit_inf_t g_stbcr8_mstp81_bit_inf_tbl;
extern const uint32_reg_bit_inf_t g_scux_bit_inf_tbl[NUM_SCUX_BIT_INF];
extern const uint32_reg_bit_inf_t g_ssictrl_ssitx_bit_inf_tbl[NUM_SSIF_CH];
extern const uint32_reg_bit_inf_t g_ssictrl_ssirx_bit_inf_tbl[NUM_SSIF_CH];
extern const uint32_reg_bit_inf_t g_dmacr_dmamdffu_bit_inf_tbl[NUM_SCUX_CH];
extern const uint32_reg_bit_inf_t g_dmacr_dmamdffd_bit_inf_tbl[NUM_SCUX_CH];
extern const uint32_reg_bit_inf_t g_rsel_place_bit_inf_tbl[NUM_AUDIO_CH];

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DRV_SCUX_REGISTER_H */

/* End of File */
