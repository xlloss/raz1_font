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
/**************************************************************************//**
* @file         video_decoder.c
* @version      0.11
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* @brief        Video decoder function
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    <stdlib.h>
#include    <string.h>

#include    "r_typedefs.h"
#include    "iodefine.h"

#include    "r_vdec.h"
#include    "r_vdec_user.h"

#include    "video_decoder.h"

#include    "graphics_debug.h"


/******************************************************************************
Macro definitions
******************************************************************************/
#define     WAIT_COUNT_ONE_MSEC         ((uint32_t)53368u)          /*!< Wait count for one millisecond */

#define     WAIT_VSYNC_CNT              (85u) /* Wait for 5 Vsync period */

#define     STBCR7_BIT7                 (0x80u)
#define     STBCR7_BIT6                 (0x40u)

/*! Video decoder recommended value */
#define SRCLEFT_NTSC_3_58    (256u)
#define SRCLEFT_NTSC_4_43    (256u)
#define SRCLEFT_PAL_4_43     (256u)
#define SRCLEFT_PAL_M        (256u)
#define SRCLEFT_PAL_N        (256u)
#define SRCLEFT_SECAM        (256u)

#define SRCTOP_NTSC_3_58     (16u)
#define SRCTOP_NTSC_4_43     (19u)
#define SRCTOP_PAL_4_43      (19u)
#define SRCTOP_PAL_M         (16u)
#define SRCTOP_PAL_N         (19u)
#define SRCTOP_SECAM         (19u)

#define SRCHEIGHT_NTSC_3_58  (241u)
#define SRCHEIGHT_NTSC_4_43  (288u)
#define SRCHEIGHT_PAL_4_43   (288u)
#define SRCHEIGHT_PAL_M      (241u)
#define SRCHEIGHT_PAL_N      (288u)
#define SRCHEIGHT_SECAM      (288u)

#define SRCWIDTH_NTSC_3_58   (1428u)
#define SRCWIDTH_NTSC_4_43   (1412u)
#define SRCWIDTH_PAL_4_43    (1412u)
#define SRCWIDTH_PAL_M       (1428u)
#define SRCWIDTH_PAL_N       (1412u)
#define SRCWIDTH_SECAM       (1412u)

#define HAFCTYP_NTSC_3_58    (692u)
#define HAFCTYP_NTSC_4_43    (704u)
#define HAFCTYP_PAL_4_43     (704u)
#define HAFCTYP_PAL_M        (692u)
#define HAFCTYP_PAL_N        (704u)
#define HAFCTYP_SECAM        (704u)

#define HAFCMAX_NTSC_3_58    (792u)
#define HAFCMAX_NTSC_4_43    (785u)
#define HAFCMAX_PAL_4_43     (785u)
#define HAFCMAX_PAL_M        (792u)
#define HAFCMAX_PAL_N        (785u)
#define HAFCMAX_SECAM        (785u)

#define HAFCMIN_NTSC_3_58    (592u)
#define HAFCMIN_NTSC_4_43    (630u)
#define HAFCMIN_PAL_4_43     (630u)
#define HAFCMIN_PAL_M        (592u)
#define HAFCMIN_PAL_N        (630u)
#define HAFCMIN_SECAM        (630u)

#define VCDWINDOW_NTSC_3_58  (30u)
#define VCDWINDOW_NTSC_4_43  (30u)
#define VCDWINDOW_PAL_4_43   (30u)
#define VCDWINDOW_PAL_M      (30u)
#define VCDWINDOW_PAL_N      (30u)
#define VCDWINDOW_SECAM      (30u)

#define VCDOFFSET_NTSC_3_58  (15u)
#define VCDOFFSET_NTSC_4_43  (15u)
#define VCDOFFSET_PAL_4_43   (15u)
#define VCDOFFSET_PAL_M      (15u)
#define VCDOFFSET_PAL_N      (15u)
#define VCDOFFSET_SECAM      (15u)

#define ACCLEVEL_NTSC_3_58   (220u)
#define ACCLEVEL_NTSC_4_43   (220u)
#define ACCLEVEL_PAL_4_43    (220u)
#define ACCLEVEL_PAL_M       (230u)
#define ACCLEVEL_PAL_N       (230u)
#define ACCLEVEL_SECAM       (220u)

#define AGCLEVEL_NTSC_3_58   (230u)
#define AGCLEVEL_NTSC_4_43   (230u)
#define AGCLEVEL_PAL_4_43    (242u)
#define AGCLEVEL_PAL_M       (242u)
#define AGCLEVEL_PAL_N       (242u)
#define AGCLEVEL_SECAM       (242u)

#define K15_NTSC_3_58        (2u)
#define K15_NTSC_4_43        (2u)
#define K15_PAL_4_43         (2u)
#define K15_PAL_M            (2u)
#define K15_PAL_N            (2u)
#define K15_SECAM            (2u)

#define K13_NTSC_3_58        (8u)
#define K13_NTSC_4_43        (8u)
#define K13_PAL_4_43         (8u)
#define K13_PAL_M            (8u)
#define K13_PAL_N            (8u)
#define K13_SECAM            (8u)

#define K11_NTSC_3_58        (4u)
#define K11_NTSC_4_43        (4u)
#define K11_PAL_4_43         (3u)
#define K11_PAL_M            (3u)
#define K11_PAL_N            (3u)
#define K11_SECAM            (4u)

#define K16_NTSC_3_58        (3u)
#define K16_NTSC_4_43        (3u)
#define K16_PAL_4_43         (4u)
#define K16_PAL_M            (4u)
#define K16_PAL_N            (4u)
#define K16_SECAM            (3u)

#define K14_NTSC_3_58        (16u)
#define K14_NTSC_4_43        (16u)
#define K14_PAL_4_43         (63u)
#define K14_PAL_M            (63u)
#define K14_PAL_N            (63u)
#define K14_SECAM            (16u)

#define K12_NTSC_3_58        (8u)
#define K12_NTSC_4_43        (8u)
#define K12_PAL_4_43         (2u)
#define K12_PAL_M            (2u)
#define K12_PAL_N            (2u)
#define K12_SECAM            (1u)

#define K22A_NTSC_3_58       (32u)
#define K22A_NTSC_4_43       (32u)
#define K22A_PAL_4_43        (32u)
#define K22A_PAL_M           (32u)
#define K22A_PAL_N           (32u)
#define K22A_SECAM           (32u)

#define K21A_NTSC_3_58       (6u)
#define K21A_NTSC_4_43       (6u)
#define K21A_PAL_4_43        (10u)
#define K21A_PAL_M           (10u)
#define K21A_PAL_N           (10u)
#define K21A_SECAM           (10u)

#define K22B_NTSC_3_58       (8u)
#define K22B_NTSC_4_43       (8u)
#define K22B_PAL_4_43        (15u)
#define K22B_PAL_M           (15u)
#define K22B_PAL_N           (15u)
#define K22B_SECAM           (15u)

#define K21B_NTSC_3_58       (6u)
#define K21B_NTSC_4_43       (6u)
#define K21B_PAL_4_43        (10u)
#define K21B_PAL_M           (10u)
#define K21B_PAL_N           (10u)
#define K21B_SECAM           (6u)

#define K23B_NTSC_3_58       (6u)
#define K23B_NTSC_4_43       (6u)
#define K23B_PAL_4_43        (3u)
#define K23B_PAL_M           (3u)
#define K23B_PAL_N           (3u)
#define K23B_SECAM           (3u)

#define K23A_NTSC_3_58       (3u)
#define K23A_NTSC_4_43       (3u)
#define K23A_PAL_4_43        (3u)
#define K23A_PAL_M           (3u)
#define K23A_PAL_N           (3u)
#define K23A_SECAM           (3u)

#define K24_NTSC_3_58        (5u)
#define K24_NTSC_4_43        (5u)
#define K24_PAL_4_43         (8u)
#define K24_PAL_M            (8u)
#define K24_PAL_N            (8u)
#define K24_SECAM            (8u)

/* SyncSep */
#define SYNCMAXDUTY_H_VALUE  (15u)
#define SYNCMINDUTY_H_VALUE  (10u)
#define CSYNCSLICE_H_VALUE   (146u)
#define SYNCMAXDUTY_V_VALUE  (15u)
#define SYNCMINDUTY_V_VALUE  (9u)
#define VSYNCSLICE_VALUE     (10u)
#define CSYNCSLICE_V_VALUE   (146u)
#define HAFCGAIN_VALUE       (12u)
#define HAFCEND_VALUE        (8u)
#define AGCRESPONSE_VALUE    (4u)
#define AGCPRECIS_VALUE      (10u)
#define PEAKATTACK_VALUE     (2u)
#define PEAKRELEASE_VALUE    (3u)
#define MAXPEAKSAMPLES_VALUE (20u)

/* YcSep */
#define RADJ_O_LEVEL0_VALUE  (992u)
#define RADJ_U_LEVEL0_VALUE  (64u)
#define RADJ_O_LEVEL1_VALUE  (960u)
#define RADJ_U_LEVEL1_VALUE  (48u)
#define RADJ_O_LEVEL2_VALUE  (928u)
#define RADJ_U_LEVEL2_VALUE  (32u)

/* ChromaDec */
#define LOOPGAIN_VALUE       (3u)
#define BGPWIDTH_VALUE       (54u)
#define BGPSTART_VALUE       (110u)
#define CHROMAMAINGAIN_VALUE (210u)
#define ACCPRECIS_VALUE      (8u)
#define KILLERLEVEL_VALUE    (4u)
#define KILLEROFFSET_VALUE   (5u)

/* DigitalClamp */
#define BLANKLEVEL_Y_VALUE   (984u)
#define DCPPOS_Y_VALUE       (162u)
#define DCPPOS_C_VALUE       (54u)
#define ACFFILTER_VALUE      (3u)
#define DCPSTART_VALUE       (16u)
#define DCPEND_VALUE         (16u)
#define DCPWIDTH_VALUE       (27u)

/* Output */
#define Y_GAIN2_VALUE        (816u)
#define CB_GAIN2_VALUE       (663u)
#define CR_GAIN2_VALUE       (663u)
/******************************************************************************
Typedef definitions
******************************************************************************/
/*! Video decoder recommended value */
typedef struct
{
    uint16_t            srcleft;
    uint16_t            srctop;
    uint16_t            srcheight;
    uint16_t            srcwidth;
    uint16_t            hafctyp;
    uint16_t            hafcmax;
    uint16_t            hafcmin;
    vdec_onoff_t        novcd50_;
    vdec_onoff_t        novcd60_;
    vdec_vcddefault_t   vcddefault;
    uint16_t            vcdwindow;
    uint16_t            vcdoffset;
    vdec_color_sys_t    defaultsys;
    vdec_onoff_t        nontsc358_;
    vdec_onoff_t        nontsc443_;
    vdec_onoff_t        nopalm_;
    vdec_onoff_t        nopaln_;
    vdec_onoff_t        nopal443_;
    vdec_onoff_t        nosecam_;
    uint16_t            acclevel;
    uint16_t            agclevel;
    uint16_t            k15;
    uint16_t            k13;
    uint16_t            k11;
    uint16_t            k16;
    uint16_t            k14;
    uint16_t            k12;
    uint16_t            k22A;
    uint16_t            k21A;
    uint16_t            k22B;
    uint16_t            k21B;
    uint16_t            k23B;
    uint16_t            k23A;
    uint16_t            k24;
    vdec_onoff_t        det2_on;
    vdec_filmix_ratio_t hsel_mix_y;
    vdec_filmix_ratio_t vsel_mix_y;
    vdec_filmix_ratio_t hvsel_mix_y;
    vdec_filmix_ratio_t det2_mix_c;
    vdec_filmix_ratio_t det2_mix_y;
    vdec_2dfil_mdsel_t  fil2_mode_2d;
    vdec_2d_fil_sel_t   fil2_narrow_2d;
} video_recmd_val_t;


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static void Input(const graphics_col_sys_t color_system, const vdec_channel_t channel);
static void SyncSep(const graphics_col_sys_t color_system, const vdec_channel_t channel);
static void YcSep(const graphics_col_sys_t color_system, const vdec_channel_t channel);
static void ChromaDec(const graphics_col_sys_t color_system, const vdec_channel_t channel);
static void DigitalClamp(const vdec_channel_t channel);
static void Output(const vdec_channel_t channel);

static void Wait(const uint32_t wait);
static graphics_col_sys_t ColorSystemDet(
    const vdec_q_sync_sep_t   * const q_sync_sep,
    const vdec_q_chroma_dec_t * const q_chroma_dec);
static void init_func(const uint32_t user_num);

static const video_recmd_val_t RecommendedValue[GRPH_COL_SYS_NUM] =
{
    {   /* NTSC-3.58 */
        (uint16_t)SRCLEFT_NTSC_3_58, (uint16_t)SRCTOP_NTSC_3_58, (uint16_t)SRCHEIGHT_NTSC_3_58, (uint16_t)SRCWIDTH_NTSC_3_58,     /* Input Video Signal Capturing Area */
        (uint16_t)HAFCTYP_NTSC_3_58, (uint16_t)HAFCMAX_NTSC_3_58, (uint16_t)HAFCMIN_NTSC_3_58,                     /* For Horizontal AFC */
        VDEC_OFF, VDEC_ON, VDEC_VCD_FRQ_59_94HZ, (uint16_t)VCDWINDOW_NTSC_3_58, (uint16_t)VCDOFFSET_NTSC_3_58,
        VDEC_COL_SYS_NTSC, VDEC_ON, VDEC_OFF, VDEC_OFF, VDEC_OFF, VDEC_OFF, VDEC_OFF,
        (uint16_t)ACCLEVEL_NTSC_3_58,                                     /* ACCLEVEL: ACC Reference Color Burst Amplitude */
        (uint16_t)AGCLEVEL_NTSC_3_58,                                     /* AGCLEVEL: Sync Signal Reference Amplitude */
        (uint16_t)K15_NTSC_3_58, (uint16_t)K13_NTSC_3_58, (uint16_t)K11_NTSC_3_58, (uint16_t)K16_NTSC_3_58, (uint16_t)K14_NTSC_3_58, (uint16_t)K12_NTSC_3_58,
        (uint16_t)K22A_NTSC_3_58, (uint16_t)K21A_NTSC_3_58, (uint16_t)K22B_NTSC_3_58, (uint16_t)K21B_NTSC_3_58, (uint16_t)K23B_NTSC_3_58, (uint16_t)K23A_NTSC_3_58, (uint16_t)K24_NTSC_3_58,
        VDEC_ON, VDEC_FILMIX_RATIO_75, VDEC_FILMIX_RATIO_75, VDEC_FILMIX_RATIO_0,
        VDEC_FILMIX_RATIO_0, VDEC_FILMIX_RATIO_25, VDEC_2DFIL_MDSEL_CASCADE, VDEC_2D_FIL_SEL_17TAP
    },
    {   /* NTSC-4.43 */
        (uint16_t)SRCLEFT_NTSC_4_43, (uint16_t)SRCTOP_NTSC_4_43, (uint16_t)SRCHEIGHT_NTSC_4_43, (uint16_t)SRCWIDTH_NTSC_4_43,     /* Input Video Signal Capturing Area */
        (uint16_t)HAFCTYP_NTSC_4_43, (uint16_t)HAFCMAX_NTSC_4_43, (uint16_t)HAFCMIN_NTSC_4_43,                     /* For Horizontal AFC */
        VDEC_ON, VDEC_OFF, VDEC_VCD_FRQ_50HZ, (uint16_t)VCDWINDOW_NTSC_4_43, (uint16_t)VCDOFFSET_NTSC_4_43,
        VDEC_COL_SYS_NTSC, VDEC_OFF, VDEC_ON, VDEC_OFF, VDEC_OFF, VDEC_OFF, VDEC_OFF,
        (uint16_t)ACCLEVEL_NTSC_4_43,                                     /* ACCLEVEL: ACC Reference Color Burst Amplitude */
        (uint16_t)AGCLEVEL_NTSC_4_43,                                     /* AGCLEVEL: Sync Signal Reference Amplitude */
        (uint16_t)K15_NTSC_4_43, (uint16_t)K13_NTSC_4_43, (uint16_t)K11_NTSC_4_43, (uint16_t)K16_NTSC_4_43, (uint16_t)K14_NTSC_4_43, (uint16_t)K12_NTSC_4_43,
        (uint16_t)K22A_NTSC_4_43, (uint16_t)K21A_NTSC_4_43, (uint16_t)K22B_NTSC_4_43, (uint16_t)K21B_NTSC_4_43, (uint16_t)K23B_NTSC_4_43, (uint16_t)K23A_NTSC_4_43, (uint16_t)K24_NTSC_4_43,
        VDEC_ON, VDEC_FILMIX_RATIO_75, VDEC_FILMIX_RATIO_75, VDEC_FILMIX_RATIO_0,
        VDEC_FILMIX_RATIO_0, VDEC_FILMIX_RATIO_25, VDEC_2DFIL_MDSEL_CASCADE, VDEC_2D_FIL_SEL_17TAP
    },
    {   /* PAL-4.43 */
        (uint16_t)SRCLEFT_PAL_4_43, (uint16_t)SRCTOP_PAL_4_43, (uint16_t)SRCHEIGHT_PAL_4_43, (uint16_t)SRCWIDTH_PAL_4_43,     /* Input Video Signal Capturing Area */
        (uint16_t)HAFCTYP_PAL_4_43, (uint16_t)HAFCMAX_PAL_4_43, (uint16_t)HAFCMIN_PAL_4_43,                     /* For Horizontal AFC */
        VDEC_ON, VDEC_OFF, VDEC_VCD_FRQ_50HZ, (uint16_t)VCDWINDOW_PAL_4_43, (uint16_t)VCDOFFSET_PAL_4_43,
        VDEC_COL_SYS_PAL, VDEC_OFF, VDEC_OFF, VDEC_OFF, VDEC_OFF, VDEC_ON, VDEC_OFF,
        (uint16_t)ACCLEVEL_PAL_4_43,                                     /* ACCLEVEL: ACC Reference Color Burst Amplitude */
        (uint16_t)AGCLEVEL_PAL_4_43,                                     /* AGCLEVEL: Sync Signal Reference Amplitude */
        (uint16_t)K15_PAL_4_43, (uint16_t)K13_PAL_4_43, (uint16_t)K11_PAL_4_43, (uint16_t)K16_PAL_4_43, (uint16_t)K14_PAL_4_43, (uint16_t)K12_PAL_4_43,
        (uint16_t)K22A_PAL_4_43, (uint16_t)K21A_PAL_4_43, (uint16_t)K22B_PAL_4_43, (uint16_t)K21B_PAL_4_43, (uint16_t)K23B_PAL_4_43, (uint16_t)K23A_PAL_4_43, (uint16_t)K24_PAL_4_43,
        VDEC_OFF, VDEC_FILMIX_RATIO_0, VDEC_FILMIX_RATIO_0, VDEC_FILMIX_RATIO_0,
        VDEC_FILMIX_RATIO_0, VDEC_FILMIX_RATIO_0, VDEC_2DFIL_MDSEL_BYPASS, VDEC_2D_FIL_SEL_17TAP
    },
    {   /* PAL-M */
        (uint16_t)SRCLEFT_PAL_M, (uint16_t)SRCTOP_PAL_M, (uint16_t)SRCHEIGHT_PAL_M, (uint16_t)SRCWIDTH_PAL_M,     /* Input Video Signal Capturing Area */
        (uint16_t)HAFCTYP_PAL_M, (uint16_t)HAFCMAX_PAL_M, (uint16_t)HAFCMIN_PAL_M,                     /* For Horizontal AFC */
        VDEC_OFF, VDEC_ON, VDEC_VCD_FRQ_59_94HZ, (uint16_t)VCDWINDOW_PAL_M, (uint16_t)VCDOFFSET_PAL_M,
        VDEC_COL_SYS_PAL, VDEC_OFF, VDEC_OFF, VDEC_ON, VDEC_OFF, VDEC_OFF, VDEC_OFF,
        (uint16_t)ACCLEVEL_PAL_M,                                     /* ACCLEVEL: ACC Reference Color Burst Amplitude */
        (uint16_t)AGCLEVEL_PAL_M,                                     /* AGCLEVEL: Sync Signal Reference Amplitude */
        (uint16_t)K15_PAL_M, (uint16_t)K13_PAL_M, (uint16_t)K11_PAL_M, (uint16_t)K16_PAL_M, (uint16_t)K14_PAL_M, (uint16_t)K12_PAL_M,
        (uint16_t)K22A_PAL_M, (uint16_t)K21A_PAL_M, (uint16_t)K22B_PAL_M, (uint16_t)K21B_PAL_M, (uint16_t)K23B_PAL_M, (uint16_t)K23A_PAL_M, (uint16_t)K24_PAL_M,
        VDEC_OFF, VDEC_FILMIX_RATIO_0, VDEC_FILMIX_RATIO_0, VDEC_FILMIX_RATIO_0,
        VDEC_FILMIX_RATIO_0, VDEC_FILMIX_RATIO_0, VDEC_2DFIL_MDSEL_BYPASS, VDEC_2D_FIL_SEL_17TAP
    },
    {   /* PAL-N */
        (uint16_t)SRCLEFT_PAL_N, (uint16_t)SRCTOP_PAL_N, (uint16_t)SRCHEIGHT_PAL_N, (uint16_t)SRCWIDTH_PAL_N,     /* Input Video Signal Capturing Area */
        (uint16_t)HAFCTYP_PAL_N, (uint16_t)HAFCMAX_PAL_N, (uint16_t)HAFCMIN_PAL_N,                     /* For Horizontal AFC */
        VDEC_ON, VDEC_OFF, VDEC_VCD_FRQ_50HZ, (uint16_t)VCDWINDOW_PAL_N, (uint16_t)VCDOFFSET_PAL_N,
        VDEC_COL_SYS_PAL, VDEC_OFF, VDEC_OFF, VDEC_OFF, VDEC_ON, VDEC_OFF, VDEC_OFF,
        (uint16_t)ACCLEVEL_PAL_N,                                     /* ACCLEVEL: ACC Reference Color Burst Amplitude */
        (uint16_t)AGCLEVEL_PAL_N,                                     /* AGCLEVEL: Sync Signal Reference Amplitude */
        (uint16_t)K15_PAL_N, (uint16_t)K13_PAL_N, (uint16_t)K11_PAL_N, (uint16_t)K16_PAL_N, (uint16_t)K14_PAL_N, (uint16_t)K12_PAL_N,
        (uint16_t)K22A_PAL_N, (uint16_t)K21A_PAL_N, (uint16_t)K22B_PAL_N, (uint16_t)K21B_PAL_N, (uint16_t)K23B_PAL_N, (uint16_t)K23A_PAL_N, (uint16_t)K24_PAL_N,
        VDEC_OFF, VDEC_FILMIX_RATIO_0, VDEC_FILMIX_RATIO_0, VDEC_FILMIX_RATIO_0,
        VDEC_FILMIX_RATIO_0, VDEC_FILMIX_RATIO_0, VDEC_2DFIL_MDSEL_BYPASS, VDEC_2D_FIL_SEL_17TAP
    },
    {   /* SECAM */
        (uint16_t)SRCLEFT_SECAM, (uint16_t)SRCTOP_SECAM, (uint16_t)SRCHEIGHT_SECAM, (uint16_t)SRCWIDTH_SECAM,     /* Input Video Signal Capturing Area */
        (uint16_t)HAFCTYP_SECAM, (uint16_t)HAFCMAX_SECAM, (uint16_t)HAFCMIN_SECAM,                     /* For Horizontal AFC */
        VDEC_ON, VDEC_OFF, VDEC_VCD_FRQ_50HZ, (uint16_t)VCDWINDOW_SECAM, (uint16_t)VCDOFFSET_SECAM,
        VDEC_COL_SYS_SECAM, VDEC_OFF, VDEC_OFF, VDEC_OFF, VDEC_OFF, VDEC_OFF, VDEC_ON,
        (uint16_t)ACCLEVEL_SECAM,                                     /* ACCLEVEL: ACC Reference Color Burst Amplitude */
        (uint16_t)AGCLEVEL_SECAM,                                     /* AGCLEVEL: Sync Signal Reference Amplitude */
        (uint16_t)K15_SECAM, (uint16_t)K13_SECAM, (uint16_t)K11_SECAM, (uint16_t)K16_SECAM, (uint16_t)K14_SECAM, (uint16_t)K12_SECAM,
        (uint16_t)K22A_SECAM, (uint16_t)K21A_SECAM, (uint16_t)K22B_SECAM, (uint16_t)K21B_SECAM, (uint16_t)K23B_SECAM, (uint16_t)K23A_SECAM, (uint16_t)K24_SECAM,
        VDEC_ON, VDEC_FILMIX_RATIO_75, VDEC_FILMIX_RATIO_75, VDEC_FILMIX_RATIO_0,
        VDEC_FILMIX_RATIO_0, VDEC_FILMIX_RATIO_0, VDEC_2DFIL_MDSEL_CASCADE, VDEC_2D_FIL_SEL_17TAP
    }
};
/* Two-Dimensional Cascade Broadband and Narrowband (3.58/4.43) Filter TAP Coefficient */
static const vdec_chrfil_tap_t wana_ntsc =
{
    {(uint16_t)24u, (uint16_t)44u, (uint16_t)20u, (uint16_t)(0x1000u | 52u), (uint16_t)(0x1000u | 128u),
    (uint16_t)(0x1000u | 128u), (uint16_t)(0x1000u | 12u), (uint16_t)132u, (uint16_t)200u}
};
/* Two-Dimensional Cascade Broadband (SECAM-DR) Filter TAP Coefficient */
static const vdec_chrfil_tap_t wa_secam =
{
    {(uint16_t)(0x1000u | 20u), (uint16_t)24u, (uint16_t)64u, (uint16_t)40u, (uint16_t)(0x1000u | 76u),
    (uint16_t)(0x1000u | 164u), (uint16_t)(0x1000u | 84u), (uint16_t)108u, (uint16_t)216u}
};
/* Two-Dimensional Cascade Broadband (SECAM-DB) Filter TAP Coefficient */
static const vdec_chrfil_tap_t wb_secam =
{
    {(uint16_t)(0x1000u | 12u), (uint16_t)40u, (uint16_t)60u, (uint16_t)12u, (uint16_t)(0x1000u | 104u),
    (uint16_t)(0x1000u | 156u), (uint16_t)(0x1000u | 64u), (uint16_t)120u, (uint16_t)208u}
};
/* Two-Dimensional Cascade Narrowband (SECAM-DR) Filter TAP Coefficient */
static const vdec_chrfil_tap_t na_secam =
{
    {(uint16_t)(0x1000u | 1008u), (uint16_t)1976u, (uint16_t)(0x1000u | 2024u), (uint16_t)444u, (uint16_t)1868u,
    (uint16_t)(0x1000u | 2864u), (uint16_t)1352u, (uint16_t)1376u, (uint16_t)(0x1000u | 2240u)}
};
/* Two-Dimensional Cascade Narrowband (SECAM-DB) Filter TAP Coefficient */
static const vdec_chrfil_tap_t nb_secam =
{
    {(uint16_t)(0x1000u | 1080u), (uint16_t)2800u, (uint16_t)(0x1000u | 3308u), (uint16_t)1628u, (uint16_t)1444u,
    (uint16_t)(0x1000u | 3308u), (uint16_t)2140u, (uint16_t)376u, (uint16_t)(0x1000u | 1384u)}
};


/**************************************************************************//**
 * @brief       Initialize video decoder
 * @param       [in]adc_vinsel
 * @param       [in]channel
 * @retval      Video color system
 *****************************************************************************/
graphics_col_sys_t GRAPHICS_VideoDecoderInit (const vdec_adc_vinsel_t adc_vinsel, const vdec_channel_t channel)
{
    vdec_error_t        error;
    vdec_q_sync_sep_t   q_sync_sep;
    vdec_q_chroma_dec_t q_chroma_dec;
    graphics_col_sys_t  color_system;

    color_system = GRPH_COL_SYS_UNKNOWN;

    /* Video Decoder Driver Initialization */
    error = R_VDEC_Initialize(channel, adc_vinsel, &init_func, (uint32_t)channel);
    if (error != VDEC_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }

    if (error == VDEC_OK)
    {
        Wait(WAIT_VSYNC_CNT);          /* Wait for 5 Vsync period */

        /* Querying Video Decoder parameter values */
        error = R_VDEC_Query(channel, &q_sync_sep, NULL, &q_chroma_dec, NULL);
        if (error != VDEC_OK)
        {
            GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
        }
    }

    if (error == VDEC_OK)
    {
        color_system = ColorSystemDet(&q_sync_sep, &q_chroma_dec);
    }

    if (color_system != GRPH_COL_SYS_UNKNOWN)
    {
        /* Input */
        Input(color_system, channel);
        /* Sync separation */
        SyncSep(color_system, channel);
        /* Y/C separation */
        YcSep(color_system, channel);
        /* Chroma decoding */
        ChromaDec(color_system, channel);
        /* Digital clamp */
        DigitalClamp(channel);
        /* Output */
        Output(channel);
    }
    return color_system;
}   /* End of function GRAPHICS_VideoDecoderInit() */

/**************************************************************************//**
 * @brief       Detect color system
 * @param       [in]q_sync_sep
 * @param       [in]q_chroma_dec
 * @retval      Video color system
 *****************************************************************************/
static graphics_col_sys_t ColorSystemDet (
    const vdec_q_sync_sep_t   * const q_sync_sep,
    const vdec_q_chroma_dec_t * const q_chroma_dec)
{
    graphics_col_sys_t color_system;

    color_system = GRPH_COL_SYS_UNKNOWN;

    if (q_sync_sep->nosignal_ == VDEC_NO)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)0);
    }
    else
    {
        switch (q_chroma_dec->colorsys)
        {   /* Color System Detection Result */
            case VDEC_COL_SYS_NTSC:
                /* Color Sub-Carrier Frequency Detection Result */
                /* NTSC-3.58 or NTSC-4.43 */
                if (q_chroma_dec->fscmode == VDEC_FSCMD_3_58)
                {
                    color_system = GRPH_COL_SYS_NTSC_358;
                }
                else
                {
                    color_system = GRPH_COL_SYS_NTSC_443;
                }
            break;
            case VDEC_COL_SYS_PAL:
                /* Color Sub-Carrier Frequency Detection Result */
                if (q_chroma_dec->fscmode == 0)
                {   /* PAL-M or PAL-N */
                    color_system = (q_sync_sep->fvmode == VDEC_FVMD_50HZ) ? GRPH_COL_SYS_PAL_N : GRPH_COL_SYS_PAL_M;
                    if (q_sync_sep->fvmode == VDEC_FVMD_50HZ)
                    {
                        color_system = GRPH_COL_SYS_PAL_N;
                    }
                    else
                    {
                        color_system = GRPH_COL_SYS_PAL_M;
                    }
                }
                else
                {
                    color_system = GRPH_COL_SYS_PAL_443;        /* PAL-4.43 */
                }
            break;
            case VDEC_COL_SYS_SECAM:
                color_system = GRPH_COL_SYS_SECAM;              /* SECAM */
            break;
            default:
                GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)0);
            break;
        }
    }
    return color_system;
}   /* End of function ColorSystemDet() */

/**************************************************************************//**
 * @brief       User-defined function
 * @param       [in]user_num
 * @retval      None
 *****************************************************************************/
static void init_func (const uint32_t user_num)
{
    uint32_t reg_data;

    reg_data = (uint32_t)CPG.STBCR7;
    if ((vdec_channel_t)user_num == VDEC_CHANNEL_0)
    {
        /* Standby control register 7 (STBCR7)
            b7      0-------;  MSTP77 : 0 : Video decoder channel 0 enable */
        reg_data &= (uint32_t)~STBCR7_BIT7;
    }
    else
    {
        /* Standby control register 7 (STBCR7)
            b6      -0------;  MSTP76 : 0 : Video decoder channel 1 enable */
        reg_data &= (uint32_t)~STBCR7_BIT6;
    }
    CPG.STBCR7 = (uint8_t)reg_data;
}   /* End of function init_func() */

/**************************************************************************//**
 * @brief       Input
 * @param       [in]color_system
 * @param       [in]channel
 * @retval      None
 *****************************************************************************/
static void Input (const graphics_col_sys_t color_system, const vdec_channel_t channel)
{
    vdec_error_t                error;
    vdec_active_period_t        param;
    const video_recmd_val_t   * RecommendValTbl;

    RecommendValTbl = &RecommendedValue[color_system];

    param.srcleft   = RecommendValTbl->srcleft;     /* Left end of input video signal capturing area */
    param.srctop    = RecommendValTbl->srctop;      /* Top end of input video signal capturing area */
    param.srcheight = RecommendValTbl->srcheight;   /* Height of input video signal capturing area */
    param.srcwidth  = RecommendValTbl->srcwidth;    /* Width of input video signal capturing area */
    /* Video decoder driver input */
    error = R_VDEC_ActivePeriod(channel, &param);
    if (error != VDEC_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
}   /* End of function Input() */

/**************************************************************************//**
 * @brief       Sync separation
 * @param       [in]color_system
 * @param       [in]channel
 * @retval      None
 *****************************************************************************/
static void SyncSep (const graphics_col_sys_t color_system, const vdec_channel_t channel)
{
    vdec_error_t                error;
    vdec_sync_separation_t      param;
    vdec_noise_rd_lpf_t         noise_rd_lpf_tmp;
    vdec_sync_slicer_t          sync_slicer_tmp;
    vdec_horizontal_afc_t       horizontal_afc_tmp;
    vdec_vcount_down_t          vcount_down_tmp;
    vdec_agc_t                  agc_tmp;
    vdec_peak_limiter_t         peak_limiter_tmp;
    const video_recmd_val_t   * RecommendValTbl;

    RecommendValTbl = &RecommendedValue[color_system];

    /* Noise reduction LPF */
    noise_rd_lpf_tmp.lpfvsync   = VDEC_LPF_VSYNC_0_54;      /* LPF cutoff frequency before vertical sync separation */
    noise_rd_lpf_tmp.lpfhsync   = VDEC_LPF_HSYNC_0_94;      /* LPF cutoff frequency before horizontal sync separation */
    /* Sync slicer */
    sync_slicer_tmp.velocityshift_h = VDEC_VELOCITY_SHIFT_4;    /* Reference level operation speed control for
                                                                   composite sync separation (for Hsync signal) */
    sync_slicer_tmp.slicermode_h    = VDEC_SLICE_MODE_AUTO_50;  /* Auto-slice level setting for composite sync
                                                                   separator circuit (for Hsync signal) */
    sync_slicer_tmp.slicermode_v    = VDEC_SLICE_MODE_AUTO_50;  /* Auto-slice level setting for composite sync
                                                                   separation circuit (for Vsync signal) */
    sync_slicer_tmp.syncmaxduty_h   = (uint16_t)SYNCMAXDUTY_H_VALUE; /* Max ratio of horizontal cycle to horizontal sync
                                                                   signal pulse width (for Hsync signal) */
    sync_slicer_tmp.syncminduty_h   = (uint16_t)SYNCMINDUTY_H_VALUE; /* Min ratio of horizontal cycle to horizontal sync
                                                                   signal pulse width (for Hsync signal) */
    sync_slicer_tmp.ssclipsel       = VDEC_CLIP_LV_1023;        /* Clipping level */
    sync_slicer_tmp.csyncslice_h    = (uint16_t)CSYNCSLICE_H_VALUE; /* Slice level for composite sync
                                                                   signal separation (for Hsync signal) */
    sync_slicer_tmp.syncmaxduty_v   = (uint16_t)SYNCMAXDUTY_V_VALUE; /* Max ratio of horizontal cycle to horizontal sync
                                                                   signal pulse width (for Vsync signal) */
    sync_slicer_tmp.syncminduty_v   = (uint16_t)SYNCMINDUTY_V_VALUE; /* Min ratio of horizontal cycle to horizontal sync
                                                                   signal pulse width (for Vsync signal) */
    sync_slicer_tmp.vsyncdelay      = VDEC_OFF;                 /* Delays the separated vertical sync signal for
                                                                   1/4 horizontal cycle */
    sync_slicer_tmp.vsyncslice      = (uint16_t)VSYNCSLICE_VALUE; /* Threshold for vertical sync separation */
    sync_slicer_tmp.csyncslice_v    = (uint16_t)CSYNCSLICE_V_VALUE; /* Slice level for composite sync
                                                                   signal separation (for Vsync signal) */
    /* Horizontal AFC */
    horizontal_afc_tmp.hafcgain     = (uint16_t)HAFCGAIN_VALUE; /* Horizontal AFC loop gain */
    horizontal_afc_tmp.hafctyp      = RecommendValTbl->hafctyp; /* Horizontal AFC center oscillation frequency */
    horizontal_afc_tmp.hafcstart    = 0;                        /* Start line of horizontal AFC normal operation */
    horizontal_afc_tmp.nox2hosc     = VDEC_ON;                  /* Disable of horizontal AFC double speed detection */
    horizontal_afc_tmp.hafcmax      = RecommendValTbl->hafcmax; /* Maximum oscillation frequency of horizontal AFC */
    horizontal_afc_tmp.hafcend      = (uint16_t)HAFCEND_VALUE;  /* End line of horizontal AFC normal operation */
    horizontal_afc_tmp.hafcmode     = VDEC_HAFCMD_AUTO_PHST;    /* Horizontal AFC VBI period operating mode */
    horizontal_afc_tmp.hafcmin      = RecommendValTbl->hafcmin; /* Min oscillation frequency of horizontal AFC */
    horizontal_afc_tmp.phdet_fix    = VDEC_OFF;                 /* Forcible or LOWGAIN control */
    horizontal_afc_tmp.phdet_div    = VDEC_PHDET_DIV_1_32;      /* Phase comparator feedback adjust for
                                                                   low sync signal lock stability */
    /* Vertical countdown */
    vcount_down_tmp.novcd50_    = RecommendValTbl->novcd50_;    /* Vertical countdown 50-Hz oscillation mode */
    vcount_down_tmp.novcd60_    = RecommendValTbl->novcd60_;    /* Vertical countdown 60-Hz (59.94-Hz)
                                                                   oscillation mode */
    vcount_down_tmp.vcddefault  = RecommendValTbl->vcddefault;  /* Vertical countdown center oscillation frequency */
    vcount_down_tmp.vcdwindow   = RecommendValTbl->vcdwindow;   /* Vertical countdown sync area */
    vcount_down_tmp.vcdoffset   = RecommendValTbl->vcdoffset;   /* Vertical countdown minimum oscillation frequency */
    /* AGC / PGA */
    agc_tmp.agcmode     = VDEC_ON;                  /* A/D converter AGC ON/OFF control & PGA gain select */
    agc_tmp.doreduce    = VDEC_OFF;                 /* Manual control of sync signal amplitude detection
                                                       during VBI period */
    agc_tmp.noreduce_   = VDEC_ON;                  /* Control of sync signal amplitude detection during VBI period */
    agc_tmp.agcresponse = (uint16_t)AGCRESPONSE_VALUE; /* AGC response speed */
    agc_tmp.agclevel    = RecommendValTbl->agclevel;/* Sync signal reference amplitude */
    agc_tmp.agcprecis   = (uint16_t)AGCPRECIS_VALUE; /* AGC gain adjustment accuracy */
    agc_tmp.pga_gain    = 0;                        /* PGA gain */
    /* Peak limiter control */
    peak_limiter_tmp.peaklevel      = VDEC_PEAKLV_992;      /* Peak luminance value to operate peak limiter */
    peak_limiter_tmp.peakattack     = (uint16_t)PEAKATTACK_VALUE; /* Response speed with peak limiter gain decreased */
    peak_limiter_tmp.peakrelease    = (uint16_t)PEAKRELEASE_VALUE; /* Response speed with peak limiter gain increased */
    peak_limiter_tmp.peakratio      = VDEC_PEAKRATIO_50;    /* Maximum compression rate of peak limiter */
    peak_limiter_tmp.maxpeaksamples = (uint16_t)MAXPEAKSAMPLES_VALUE; /* Allowable number of overflowing pixels */

    param.noise_rd_lpf      = &noise_rd_lpf_tmp;    /* Noise reduction LPF */
    param.sync_slicer       = &sync_slicer_tmp;     /* Sync slicer */
    param.horizontal_afc    = &horizontal_afc_tmp;  /* Horizontal AFC */
    param.vcount_down       = &vcount_down_tmp;     /* Vertical countdown */
    param.agc               = &agc_tmp;             /* AGC / PGA */
    param.peak_limiter      = &peak_limiter_tmp;    /* Peak limiter control */
    /* Video decoder driver sync separation */
    error = R_VDEC_SyncSeparation(channel, &param);
    if (error != VDEC_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
}   /* End of function SyncSep() */

/**************************************************************************//**
 * @brief       Y/C separation
 * @param       [in]color_system
 * @param       [in]channel
 * @retval      None
 *****************************************************************************/
static void YcSep (const graphics_col_sys_t color_system, const vdec_channel_t channel)
{
    vdec_error_t                error;
    vdec_yc_separation_t        param;
    vdec_over_range_t           over_range_tmp;
    vdec_yc_sep_ctrl_t          yc_sep_ctrl_tmp;
    const video_recmd_val_t   * RecommendValTbl;

    RecommendValTbl = &RecommendedValue[color_system];

    /* Over-range control */
    over_range_tmp.radj_o_level0    = (uint16_t)RADJ_O_LEVEL0_VALUE;   /* A/D over-threshold level (between levels 0 and 1) */
    over_range_tmp.radj_u_level0    = (uint16_t)RADJ_U_LEVEL0_VALUE;   /* A/D under-threshold level (between levels 2 and 3) */
    over_range_tmp.radj_o_level1    = (uint16_t)RADJ_O_LEVEL1_VALUE;   /* A/D over-threshold level (between levels 1 and 2) */
    over_range_tmp.radj_u_level1    = (uint16_t)RADJ_U_LEVEL1_VALUE;   /* A/D under-threshold level (between levels 1 and 2) */
    over_range_tmp.radj_o_level2    = (uint16_t)RADJ_O_LEVEL2_VALUE;   /* A/D over-threshold level (between levels 2 and 3) */
    over_range_tmp.radj_u_level2    = (uint16_t)RADJ_U_LEVEL2_VALUE;   /* A/D under-threshold level (between levels 0 and 1) */
    over_range_tmp.ucmp_sw          = VDEC_ON;          /* Over-range detection */
    over_range_tmp.dcmp_sw          = VDEC_ON;          /* Under-range detection */
    over_range_tmp.hwide_sw         = VDEC_ON;          /* Horizontal enlargement of over/under-range level */
    /* Y/C separation control */
    yc_sep_ctrl_tmp.k15     = RecommendValTbl->k15; /* Two-dimensional Y/C separation filter select coefficient, K15 */
    yc_sep_ctrl_tmp.k13     = RecommendValTbl->k13; /* Two-dimensional Y/C separation filter select coefficient, K13 */
    yc_sep_ctrl_tmp.k11     = RecommendValTbl->k11; /* Two-dimensional Y/C separation filter select coefficient, K11 */
    yc_sep_ctrl_tmp.k16     = RecommendValTbl->k16; /* Two-dimensional Y/C separation filter select coefficient, K16 */
    yc_sep_ctrl_tmp.k14     = RecommendValTbl->k14; /* Two-dimensional Y/C separation filter select coefficient, K14 */
    yc_sep_ctrl_tmp.k12     = RecommendValTbl->k12; /* Two-dimensional Y/C separation filter select coefficient, K12 */
    yc_sep_ctrl_tmp.k22a    = RecommendValTbl->k22A;/* Two-dimensional Y/C separation filter select coefficient, K22A */
    yc_sep_ctrl_tmp.k21a    = RecommendValTbl->k21A;/* Two-dimensional Y/C separation filter select coefficient, K21A */
    yc_sep_ctrl_tmp.k22b    = RecommendValTbl->k22B;/* Two-dimensional Y/C separation filter select coefficient, K22B */
    yc_sep_ctrl_tmp.k21b    = RecommendValTbl->k21B;/* Two-dimensional Y/C separation filter select coefficient, K21B */
    yc_sep_ctrl_tmp.k23b    = RecommendValTbl->k23B;/* Two-dimensional Y/C separation filter select coefficient, K23B */
    yc_sep_ctrl_tmp.k23a    = RecommendValTbl->k23A;/* Two-dimensional Y/C separation filter select coefficient, K23A */
    yc_sep_ctrl_tmp.k24     = RecommendValTbl->k24; /* Two-dimensional Y/C separation filter select coefficient, K24 */
    yc_sep_ctrl_tmp.hbpf_narrow     = VDEC_LSTG_BPFSEL_BYPASS;      /* Latter-stage horizontal BPF select */
    yc_sep_ctrl_tmp.hvbpf_narrow    = VDEC_LSTG_BPFSEL_BYPASS;      /* Latter-stage horizontal/vertical BPF select */
    yc_sep_ctrl_tmp.hbpf1_9tap_on   = VDEC_FSTG_BPFSEL_17TAP;       /* Former-stage horizontal BPF select */
    yc_sep_ctrl_tmp.hvbpf1_9tap_on  = VDEC_FSTG_BPFSEL_17TAP;       /* Former-stage horizontal/vertical BPF select */
    yc_sep_ctrl_tmp.hfil_tap_sel    = VDEC_HFIL_TAP_SEL_17TAP;      /* Horizontal filter and horizontal/vertical filter
                                                                       bandwidth switch signal */
    yc_sep_ctrl_tmp.det2_on         = RecommendValTbl->det2_on;     /* Two-dimensional filter mixing select */
    yc_sep_ctrl_tmp.hsel_mix_y      = RecommendValTbl->hsel_mix_y;  /* Mixing ratio of signal after passing
                                                                       horizontal filter to signal after passing
                                                                       former-stage horizontal filter */
    yc_sep_ctrl_tmp.vsel_mix_y      = RecommendValTbl->vsel_mix_y;  /* Mixing ratio of signal after passing
                                                                       vertical filter to signal after passing
                                                                       former-stage horizontal/vertical filter */
    yc_sep_ctrl_tmp.hvsel_mix_y     = RecommendValTbl->hvsel_mix_y; /* Mixing ratio of signal after passing
                                                                       horizontal/vertical filter to signal
                                                                       after passing former-stage
                                                                       horizontal/vertical filter */
    yc_sep_ctrl_tmp.v_y_level       = 0;                            /* Vertical luminance detection level
                                                                       for correlation detection filter */
    yc_sep_ctrl_tmp.det2_mix_c      = RecommendValTbl->det2_mix_c;  /* Mixing ratio of c signal after passing
                                                                       horizontal/vertical adaptive filter
                                                                       to signal after passing correlation detection
                                                                       filter */
    yc_sep_ctrl_tmp.det2_mix_y      = RecommendValTbl->det2_mix_y;  /* Mixing ratio of c signal for y generation after
                                                                       passing horizontal/vertical adaptive filter to
                                                                       signal after passing correlation */
    yc_sep_ctrl_tmp.fil2_mode_2d    = RecommendValTbl->fil2_mode_2d;/* Two-dimensional cascade/TAKE-OFF
                                                                       filter mode select */
    yc_sep_ctrl_tmp.fil2_narrow_2d  = RecommendValTbl->fil2_narrow_2d;  /* Two-dimensional cascade filter select */

    param.over_range    = &over_range_tmp;  /* Over-range control */
    param.yc_sep_ctrl   = &yc_sep_ctrl_tmp; /* Y/C separation control */
    param.fil2_2d_wa    = NULL;             /* Two-dimensional cascade broadband
                                               (3.58/4.43/SECAM-DR)/TAKE-OFF filter TAP coefficient */
    param.fil2_2d_wb    = NULL;             /* Two-dimensional cascade broadband (SECAM-DB) filter TAP coefficient */
    param.fil2_2d_na    = NULL;             /* Two-dimensional cascade narrowband
                                               (3.58/4.43/SECAM-DR) filter TAP coefficient */
    param.fil2_2d_nb    = NULL;             /* Two-dimensional cascade narrowband (SECAMDB) filter TAP coefficient */
    if ((color_system == GRPH_COL_SYS_NTSC_358) || (color_system == GRPH_COL_SYS_NTSC_443))
    {
        param.fil2_2d_wa = &wana_ntsc;
        param.fil2_2d_na = &wana_ntsc;
    }
    else if (color_system == GRPH_COL_SYS_SECAM)
    {
        param.fil2_2d_wa = &wa_secam;
        param.fil2_2d_wb = &wb_secam;
        param.fil2_2d_na = &na_secam;
        param.fil2_2d_nb = &nb_secam;
    }
    else
    {
        /* Do nothing */
    }
    /* Video decoder driver y/c separation */
    error = R_VDEC_YcSeparation(channel, &param);
    if (error != VDEC_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
}   /* End of function YcSep() */

/**************************************************************************//**
 * @brief       Chroma decoding
 * @param       [in]color_system
 * @param       [in]channel
 * @retval      None
 *****************************************************************************/
static void ChromaDec (const graphics_col_sys_t color_system, const vdec_channel_t channel)
{
    vdec_error_t                error;
    vdec_chroma_decoding_t      param;
    vdec_chrmdec_ctrl_t         chrmdec_ctrl_tmp;
    vdec_burst_lock_t           burst_lock_tmp;
    vdec_acc_t                  acc_tmp;
    const video_recmd_val_t   * RecommendValTbl;

    RecommendValTbl = &RecommendedValue[color_system];

    /* Chroma decoding control */
    chrmdec_ctrl_tmp.defaultsys = RecommendValTbl->defaultsys;  /* Default color system */
    chrmdec_ctrl_tmp.nontsc358_ = RecommendValTbl->nontsc358_;  /* NTSC-M detection control */
    chrmdec_ctrl_tmp.nontsc443_ = RecommendValTbl->nontsc443_;  /* NTSC-4.43 detection control */
    chrmdec_ctrl_tmp.nopalm_    = RecommendValTbl->nopalm_;     /* PAL-M detection control */
    chrmdec_ctrl_tmp.nopaln_    = RecommendValTbl->nopaln_;     /* PAL-N detection control */
    chrmdec_ctrl_tmp.nopal443_  = RecommendValTbl->nopal443_;   /* PAL-B, G, H, I, D detection control */
    chrmdec_ctrl_tmp.nosecam_   = RecommendValTbl->nosecam_;    /* SECAM detection control */
    chrmdec_ctrl_tmp.lumadelay  = 0;                            /* Luminance signal delay adjustment */
    chrmdec_ctrl_tmp.chromalpf  = VDEC_OFF;                     /* LPF for demodulated chroma */
    chrmdec_ctrl_tmp.demodmode  = VDEC_DEMOD_MD_PAL;            /* Averaging processing for pre-demodulated line */
    /* Burst lock */
    burst_lock_tmp.lockrange    = VDEC_LOCK_RANGE_800HZ;/* Burst lock PLL lock range */
    burst_lock_tmp.loopgain     = (uint16_t)LOOPGAIN_VALUE; /* Burst lock PLL loop gain */
    burst_lock_tmp.locklimit    = (uint16_t)1u;         /* Level for burst lock PLL to re-search free-run frequency */
    burst_lock_tmp.bgpcheck     = 0;                    /* burst gate pulse position check */
    burst_lock_tmp.bgpwidth     = (uint16_t)BGPWIDTH_VALUE; /* burst gate pulse width */
    burst_lock_tmp.bgpstart     = (uint16_t)BGPSTART_VALUE; /* burst gate pulse start position */
    /* ACC / color killer */
    acc_tmp.accmode         = VDEC_ACC_MD_AUTO;         /* ACC operating mode */
    acc_tmp.accmaxgain      = VDEC_ACC_MAX_GAIN_6;      /* Maximum ACC Gain */
    acc_tmp.acclevel        = RecommendValTbl->acclevel;/* ACC reference color burst amplitude */
    acc_tmp.chromasubgain   = VDEC_CHRM_SB_GAIN_1;      /* Chroma manual gain (sub) */
    acc_tmp.chromamaingain  = (uint16_t)CHROMAMAINGAIN_VALUE; /* Chroma manual gain (main) */
    acc_tmp.accresponse     = (uint16_t)1u;             /* ACC response speed */
    acc_tmp.accprecis       = (uint16_t)ACCPRECIS_VALUE; /* ACC gain adjustment accuracy */
    acc_tmp.killermode      = VDEC_OFF;                 /* Forced color killer mode ON/OFF */
    acc_tmp.killerlevel     = (uint16_t)KILLERLEVEL_VALUE; /* Color killer operation start point */
    acc_tmp.killeroffset    = (uint16_t)KILLEROFFSET_VALUE; /* The offset level to turn off the color killer */

    param.chrmdec_ctrl  = &chrmdec_ctrl_tmp;    /* Chroma decoding control */
    param.burst_lock    = &burst_lock_tmp;      /* Burst lock */
    param.acc           = &acc_tmp;             /* ACC/color killer */
    param.tint_ry       = NULL;                 /* TINT/R-Y axis correction (only valid for NTSC/PAL) */
    /* Video decoder driver chroma decoding */
    error = R_VDEC_ChromaDecoding(channel, &param);
    if (error != VDEC_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
}   /* End of function ChromaDec() */

/**************************************************************************//**
 * @brief       Digital clamp
 * @param       [in]channel
 * @retval      None
 *****************************************************************************/
static void DigitalClamp (const vdec_channel_t channel)
{
    vdec_error_t            error;
    vdec_degital_clamp_t    param;
    vdec_pedestal_clamp_t   pedestal_clamp_tmp;
    vdec_center_clamp_t     center_clamp_tmp;
    vdec_noise_det_t        noise_det_tmp;

    /* Pedestal clamp */
    pedestal_clamp_tmp.dcpmode_y    = VDEC_DCPMODE_AUTO;/* Clamp level setting mode */
    pedestal_clamp_tmp.blanklevel_y = (uint16_t)BLANKLEVEL_Y_VALUE; /* Clamp offset level (Y signal) */
    pedestal_clamp_tmp.dcppos_y     = (uint16_t)DCPPOS_Y_VALUE; /* Digital clamp pulse horizontal start position (Y signal) */
    /* Center clamp */
    center_clamp_tmp.dcpmode_c      = VDEC_DCPMODE_MANUAL;  /* Clamp level setting mode */
    center_clamp_tmp.blanklevel_cb  = 0;                    /* Clamp offset level (Cb signal) */
    center_clamp_tmp.blanklevel_cr  = 0;                    /* Clamp offset level (Cr signal) */
    center_clamp_tmp.dcppos_c       = (uint16_t)DCPPOS_C_VALUE; /* Digital clamp pulse
                                                                   horizontal start position (Cb/Cr signal) */
    /* Noise detection */
    noise_det_tmp.acfinput      = VDEC_ACFINPUT_Y;  /* Video signal for autocorrelation function */
    noise_det_tmp.acflagtime    = 0;                /* Delay time for autocorrelation function calculation */
    noise_det_tmp.acffilter     = (uint16_t)ACFFILTER_VALUE; /* Smoothing parameter of autocorrelation function data */

    param.dcpcheck          = VDEC_OFF;             /* Digital clamp pulse position check */
    param.dcpresponse       = 0;                    /* Digital clamp response speed */
    param.dcpstart          = (uint16_t)DCPSTART_VALUE; /* Digital clamp start line (in 1-line units) */
    param.dcpend            = (uint16_t)DCPEND_VALUE;   /* Digital clamp end line (in 1-line units) */
    param.dcpwidth          = (uint16_t)DCPWIDTH_VALUE; /* Digital clamp pulse width */
    param.pedestal_clamp    = &pedestal_clamp_tmp;  /* Pedestal clamp */
    param.center_clamp      = &center_clamp_tmp;    /* Center clamp */
    param.noise_det         = &noise_det_tmp;       /* Noise detection */
    /* Video decoder driver digital clamp */
    error = R_VDEC_DigitalClamp(channel, &param);
    if (error != VDEC_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
}   /* End of function DigitalClamp() */

/**************************************************************************//**
 * @brief       Output
 * @param       [in]channel
 * @retval      None
 *****************************************************************************/
static void Output (const vdec_channel_t channel)
{
    vdec_error_t        error;
    vdec_output_t       param;

    param.y_gain2   = (uint16_t)Y_GAIN2_VALUE;       /* Y signal gain coefficient */
    param.cb_gain2  = (uint16_t)CB_GAIN2_VALUE;      /* Cb signal gain coefficient */
    param.cr_gain2  = (uint16_t)CR_GAIN2_VALUE;      /* Cr signal gain coefficient */
   /* Video decoder driver output */
    error = R_VDEC_Output(channel, &param);
    if (error != VDEC_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
}   /* End of function Output() */

/**************************************************************************//**
 * @brief       Wait
 * @param       [in]wait    : Wait time [msec]
 * @retval      None
 *****************************************************************************/
static void Wait (const uint32_t wait)
{
    uint32_t            count_i;
    uint32_t            count_o;
    volatile uint32_t   count;

    for (count_o = 0; count_o < wait; count_o++)
    {
        count = 0;
        for (count_i = 0; count_i < WAIT_COUNT_ONE_MSEC; count_i++)
        {
            count++;
        }
    }
}   /* End of function Wait() */

