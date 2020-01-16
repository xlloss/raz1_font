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
* @file         fill_color.c
* @version      0.11
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* @brief        VDC5 driver graphics rendering sample
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include    <string.h>

#include    "r_typedefs.h"

#include    "r_vdc5.h"

#include    "graphics.h"
#include    "graphics_debug.h"


/******************************************************************************
Macro definitions
******************************************************************************/
#define     COLOR_NUM                 ((int32_t)13)

#define     ALPHA_VALUE_ARGB1555      (1u)
#define     ALPHA_VALUE_ARGB4444      (8u)
#define     ALPHA_VALUE_ARGB8888      0x20 										//(128u)
#define     ALPHA_VALUE_RGBA5551      (1u)
#define     ALPHA_VALUE_RGBA8888      (128u)
#define     ALPHA_VALUE_CLUT          (255u)

#define     RGB565_COLOR_PATTERN01    (0xCE59u)
#define     RGB565_COLOR_PATTERN02    (0xFFE0u)
#define     RGB565_COLOR_PATTERN03    (0x07FFu)
#define     RGB565_COLOR_PATTERN04    (0x07E0u)
#define     RGB565_COLOR_PATTERN05    (0xF81Fu)
#define     RGB565_COLOR_PATTERN06    (0xF800u)
#define     RGB565_COLOR_PATTERN07    (0x001Fu)
#define     RGB565_COLOR_PATTERN08    (0x10A2u)
#define     RGB565_COLOR_PATTERN09    (0x09EBu)
#define     RGB565_COLOR_PATTERN10    (0xFFFFu)
#define     RGB565_COLOR_PATTERN11    (0x380Fu)
#define     RGB565_COLOR_PATTERN12    (0x0000u)
#define     RGB565_COLOR_PATTERN13    (0x2925u)

#define     RGB888_COLOR_PATTERN01    (0x00CCCCCCu)
#define     RGB888_COLOR_PATTERN02    (0x00FFFF00u)
#define     RGB888_COLOR_PATTERN03    (0x0000FFFFu)
#define     RGB888_COLOR_PATTERN04    (0x0000FF00u)
#define     RGB888_COLOR_PATTERN05    (0x00FF00FFu)
#define     RGB888_COLOR_PATTERN06    (0x00FF0000u)
#define     RGB888_COLOR_PATTERN07    (0x000000FFu)
#define     RGB888_COLOR_PATTERN08    (0x00131313u)
#define     RGB888_COLOR_PATTERN09    (0x00083E59u)
#define     RGB888_COLOR_PATTERN10    (0x00FFFFFFu)
#define     RGB888_COLOR_PATTERN11    (0x003A007Eu)
#define     RGB888_COLOR_PATTERN12    (0x00000000u)
#define     RGB888_COLOR_PATTERN13    (0x00262626u)

#define     RGB555_COLOR_PATTERN01    (0x6739u)
#define     RGB555_COLOR_PATTERN02    (0x7FE0u)
#define     RGB555_COLOR_PATTERN03    (0x03FFu)
#define     RGB555_COLOR_PATTERN04    (0x03E0u)
#define     RGB555_COLOR_PATTERN05    (0x7C1Fu)
#define     RGB555_COLOR_PATTERN06    (0x7C00u)
#define     RGB555_COLOR_PATTERN07    (0x001Fu)
#define     RGB555_COLOR_PATTERN08    (0x0842u)
#define     RGB555_COLOR_PATTERN09    (0x050Bu)
#define     RGB555_COLOR_PATTERN10    (0x7FFFu)
#define     RGB555_COLOR_PATTERN11    (0x1C0Fu)
#define     RGB555_COLOR_PATTERN12    (0x0000u)
#define     RGB555_COLOR_PATTERN13    (0x14A5u)

#define     RGB444_COLOR_PATTERN01    (0x0CCCu)
#define     RGB444_COLOR_PATTERN02    (0x0FF0u)
#define     RGB444_COLOR_PATTERN03    (0x00FFu)
#define     RGB444_COLOR_PATTERN04    (0x00F0u)
#define     RGB444_COLOR_PATTERN05    (0x0F0Fu)
#define     RGB444_COLOR_PATTERN06    (0x0F00u)
#define     RGB444_COLOR_PATTERN07    (0x000Fu)
#define     RGB444_COLOR_PATTERN08    (0x0111u)
#define     RGB444_COLOR_PATTERN09    (0x0035u)
#define     RGB444_COLOR_PATTERN10    (0x0FFFu)
#define     RGB444_COLOR_PATTERN11    (0x0307u)
#define     RGB444_COLOR_PATTERN12    (0x0000u)
#define     RGB444_COLOR_PATTERN13    (0x0222u)

#define     YCC422_COLOR_PATTERN01    (0x80CC80CCu)
#define     YCC422_COLOR_PATTERN02    (0x00E194E1u)
#define     YCC422_COLOR_PATTERN03    (0xABB200B2u)
#define     YCC422_COLOR_PATTERN04    (0x2B951595u)
#define     YCC422_COLOR_PATTERN05    (0xD469EA69u)
#define     YCC422_COLOR_PATTERN06    (0x544CFF4Cu)
#define     YCC422_COLOR_PATTERN07    (0xFF1D6B1Du)
#define     YCC422_COLOR_PATTERN08    (0x80138013u)
#define     YCC422_COLOR_PATTERN09    (0x96306230u)
#define     YCC422_COLOR_PATTERN10    (0x80FF80FFu)
#define     YCC422_COLOR_PATTERN11    (0xB51F921Fu)
#define     YCC422_COLOR_PATTERN12    (0x80008000u)
#define     YCC422_COLOR_PATTERN13    (0x80268026u)

#define     YCC444_COLOR_PATTERN01    (0x0080CC80u)
#define     YCC444_COLOR_PATTERN02    (0x0094E100u)
#define     YCC444_COLOR_PATTERN03    (0x0000B2ABu)
#define     YCC444_COLOR_PATTERN04    (0x0015952Bu)
#define     YCC444_COLOR_PATTERN05    (0x00EA69D4u)
#define     YCC444_COLOR_PATTERN06    (0x00FF4C54u)
#define     YCC444_COLOR_PATTERN07    (0x006B1DFFu)
#define     YCC444_COLOR_PATTERN08    (0x00801380u)
#define     YCC444_COLOR_PATTERN09    (0x00623096u)
#define     YCC444_COLOR_PATTERN10    (0x0080FF80u)
#define     YCC444_COLOR_PATTERN11    (0x00921FB5u)
#define     YCC444_COLOR_PATTERN12    (0x00800080u)
#define     YCC444_COLOR_PATTERN13    (0x00802680u)

#define     COLOR_8BIT_MASK             (0x000000FFu)
#define     COLOR_15BIT_MASK            (0x00007FFFu)
#define     COLOR_15BIT_MASK_0XFFFE     (0x0000FFFEu)
#define     COLOR_16BIT_MASK            (0x0000FFFFu)
#define     COLOR_24BIT_MASK            (0x00FFFFFFu)
#define     COLOR_24BIT_MASK_0XFFFFFF00 (0xFFFFFF00u)

#define     COLOR_SHIFT_1               (1u)
#define     COLOR_SHIFT_8               (8u)
#define     COLOR_SHIFT_12              (12u)
#define     COLOR_SHIFT_15              (15u)
#define     COLOR_SHIFT_24              (24u)

#define     DIV2                        (2u)
#define     DIV8                        (8u)

#define     TOP_LINE_4BITPIX             (0x11u)
#define     LEFT_SIDE_LINE_4BITPIX       (0x10u)
#define     RIGHT_SIDE_LINE_4BITPIX      (0x10u)
#define     RIGHT_SIDE_LINE_MASK_4BITPIX (0x0Fu)
#define     BOTTOM_LINE_4BITPIX          (0x11u)

#define     LEFT_SIDE_LINE_1BITPIX       (0x80u)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static void FillColor_16BITPIX(
    uint16_t    * const buff,
    const uint16_t      color_dat,
    const uint16_t      inv_color_dat,
    const uint16_t      width,
    const uint16_t      height,
    const uint16_t      stride);
static void FillColor_32BITPIX(
    uint32_t    * const buff,
    const uint32_t      color_dat,
    const uint32_t      inv_color_dat,
    const uint16_t      width,
    const uint16_t      height,
    const uint16_t      stride);
static void FillColor_8BITPIX(uint8_t * const buff, const uint16_t width, const uint16_t height, const uint16_t stride);
static void FillColor_4BITPIX(uint8_t * const buff, const uint16_t width, const uint16_t height, const uint16_t stride);
static void FillColor_1BITPIX(uint8_t * const buff, const uint16_t width, const uint16_t height, const uint16_t stride);
static void Set_CLUT(uint32_t * clut, uint32_t color_dat);

static const uint16_t ColorData_RGB565[COLOR_NUM] =
{
    (uint16_t)RGB565_COLOR_PATTERN01,
    (uint16_t)RGB565_COLOR_PATTERN02,
    (uint16_t)RGB565_COLOR_PATTERN03,
    (uint16_t)RGB565_COLOR_PATTERN04,
    (uint16_t)RGB565_COLOR_PATTERN05,
    (uint16_t)RGB565_COLOR_PATTERN06,
    (uint16_t)RGB565_COLOR_PATTERN07,
    (uint16_t)RGB565_COLOR_PATTERN08,
    (uint16_t)RGB565_COLOR_PATTERN09,
    (uint16_t)RGB565_COLOR_PATTERN10,
    (uint16_t)RGB565_COLOR_PATTERN11,
    (uint16_t)RGB565_COLOR_PATTERN12,
    (uint16_t)RGB565_COLOR_PATTERN13
};
static const uint32_t ColorData_RGB888[COLOR_NUM] =
{
    RGB888_COLOR_PATTERN01,
    RGB888_COLOR_PATTERN02,
    RGB888_COLOR_PATTERN03,
    RGB888_COLOR_PATTERN04,
    RGB888_COLOR_PATTERN05,
    RGB888_COLOR_PATTERN06,
    RGB888_COLOR_PATTERN07,
    RGB888_COLOR_PATTERN08,
    RGB888_COLOR_PATTERN09,
    RGB888_COLOR_PATTERN10,
    RGB888_COLOR_PATTERN11,
    RGB888_COLOR_PATTERN12,
    RGB888_COLOR_PATTERN13
};
static const uint16_t ColorData_RGB555[COLOR_NUM] =
{
    (uint16_t)RGB555_COLOR_PATTERN01,
    (uint16_t)RGB555_COLOR_PATTERN02,
    (uint16_t)RGB555_COLOR_PATTERN03,
    (uint16_t)RGB555_COLOR_PATTERN04,
    (uint16_t)RGB555_COLOR_PATTERN05,
    (uint16_t)RGB555_COLOR_PATTERN06,
    (uint16_t)RGB555_COLOR_PATTERN07,
    (uint16_t)RGB555_COLOR_PATTERN08,
    (uint16_t)RGB555_COLOR_PATTERN09,
    (uint16_t)RGB555_COLOR_PATTERN10,
    (uint16_t)RGB555_COLOR_PATTERN11,
    (uint16_t)RGB555_COLOR_PATTERN12,
    (uint16_t)RGB555_COLOR_PATTERN13
};
static const uint16_t ColorData_RGB444[COLOR_NUM] =
{
    (uint16_t)RGB444_COLOR_PATTERN01,
    (uint16_t)RGB444_COLOR_PATTERN02,
    (uint16_t)RGB444_COLOR_PATTERN03,
    (uint16_t)RGB444_COLOR_PATTERN04,
    (uint16_t)RGB444_COLOR_PATTERN05,
    (uint16_t)RGB444_COLOR_PATTERN06,
    (uint16_t)RGB444_COLOR_PATTERN07,
    (uint16_t)RGB444_COLOR_PATTERN08,
    (uint16_t)RGB444_COLOR_PATTERN09,
    (uint16_t)RGB444_COLOR_PATTERN10,
    (uint16_t)RGB444_COLOR_PATTERN11,
    (uint16_t)RGB444_COLOR_PATTERN12,
    (uint16_t)RGB444_COLOR_PATTERN13
};
static const uint32_t ColorData_YCC422[COLOR_NUM] =
{   /* Cb - Y - Cr - Y */
    YCC422_COLOR_PATTERN01,
    YCC422_COLOR_PATTERN02,
    YCC422_COLOR_PATTERN03,
    YCC422_COLOR_PATTERN04,
    YCC422_COLOR_PATTERN05,
    YCC422_COLOR_PATTERN06,
    YCC422_COLOR_PATTERN07,
    YCC422_COLOR_PATTERN08,
    YCC422_COLOR_PATTERN09,
    YCC422_COLOR_PATTERN10,
    YCC422_COLOR_PATTERN11,
    YCC422_COLOR_PATTERN12,
    YCC422_COLOR_PATTERN13
};
static const uint32_t ColorData_YCC444[COLOR_NUM] =
{   /* 00 - Cr - Y - Cb */
    YCC444_COLOR_PATTERN01,
    YCC444_COLOR_PATTERN02,
    YCC444_COLOR_PATTERN03,
    YCC444_COLOR_PATTERN04,
    YCC444_COLOR_PATTERN05,
    YCC444_COLOR_PATTERN06,
    YCC444_COLOR_PATTERN07,
    YCC444_COLOR_PATTERN08,
    YCC444_COLOR_PATTERN09,
    YCC444_COLOR_PATTERN10,
    YCC444_COLOR_PATTERN11,
    YCC444_COLOR_PATTERN12,
    YCC444_COLOR_PATTERN13
};

static int32_t AutoIndex = 0;


/**************************************************************************//**
 * @brief       Create Fill Color image
 * @param       [out]buff       : Frame buffer
 * @param       [out]clut       : ARGB8888 format color look up table. If unnecessary, NULL can be specified.
 * @param       [in]width       : Image width[pixel]
 * @param       [in]height      : Image height[line]
 * @param       [in]stride      : Buffer stride[byte]
 * @param       [in]format      : Graphics Format
 * @param       [in]index       : Color index number ---> 0 - COLOR_NUM - 1, auto ---> -1
 * @retval      None
 *****************************************************************************/
void GRAPHICS_CreateFillColorImage (
    void            * const buff,
    uint32_t        * const clut,
    const uint16_t          width,
    const uint16_t          height,
    const uint16_t          stride,
    const vdc5_gr_format_t  format,
    const int32_t           index)
{
    int32_t     index_num;
    uint16_t    color_data16;
    uint16_t    color_data16_inv;
    uint32_t    color_data32;
    uint32_t    color_data32_inv;

    if ((buff != NULL) && (format < VDC5_GR_FORMAT_NUM))
    {
        if (index < 0)
        {
            index_num = AutoIndex;
        }
        else if (index >= COLOR_NUM)
        {
            index_num = 0;
        }
        else
        {
            index_num = index;
        }

        AutoIndex++;
        if (AutoIndex >= COLOR_NUM)
        {
            AutoIndex = 0;
        }

        switch (format)
        {
            case VDC5_GR_FORMAT_RGB565:
                color_data16        = ColorData_RGB565[index_num];
                color_data16_inv    = (uint16_t)((uint32_t)color_data16 ^ COLOR_16BIT_MASK);

                FillColor_16BITPIX((uint16_t *)buff, color_data16, color_data16_inv, width, height, stride);
            break;

            case VDC5_GR_FORMAT_RGB888:
                color_data32        = ColorData_RGB888[index_num];
                color_data32_inv    = color_data32 ^ COLOR_24BIT_MASK;

                FillColor_32BITPIX((uint32_t *)buff, color_data32, color_data32_inv, width, height, stride);
            break;

            case VDC5_GR_FORMAT_ARGB1555:
                color_data16        = ColorData_RGB555[index_num];
                color_data16_inv    = (uint16_t)((uint32_t)color_data16 ^ COLOR_15BIT_MASK);
                color_data16        = (uint16_t)((uint32_t)color_data16 | (ALPHA_VALUE_ARGB1555 << COLOR_SHIFT_15));
                color_data16_inv    = (uint16_t)((uint32_t)color_data16_inv | (ALPHA_VALUE_ARGB1555 << COLOR_SHIFT_15));

                FillColor_16BITPIX((uint16_t *)buff, color_data16, color_data16_inv, width, height, stride);
            break;

            case VDC5_GR_FORMAT_ARGB4444:
                color_data16        = ColorData_RGB444[index_num];
                color_data16_inv    = (uint16_t)((uint32_t)color_data16 ^ COLOR_16BIT_MASK);
                color_data16        = (uint16_t)((uint32_t)color_data16 | (ALPHA_VALUE_ARGB4444 << COLOR_SHIFT_12));
                color_data16_inv    = (uint16_t)((uint32_t)color_data16_inv | (ALPHA_VALUE_ARGB4444 << COLOR_SHIFT_12));

                FillColor_16BITPIX((uint16_t *)buff, color_data16, color_data16_inv, width, height, stride);
            break;

            case VDC5_GR_FORMAT_ARGB8888:
                color_data32        = ColorData_RGB888[index_num];
                color_data32_inv    = color_data32 ^ COLOR_24BIT_MASK;
                color_data32       |= (uint32_t)(ALPHA_VALUE_ARGB8888 << COLOR_SHIFT_24);
                color_data32_inv   |= (uint32_t)(ALPHA_VALUE_ARGB8888 << COLOR_SHIFT_24);

                FillColor_32BITPIX((uint32_t *)buff, color_data32, color_data32_inv, width, height, stride);
            break;

            case VDC5_GR_FORMAT_CLUT8:
                FillColor_8BITPIX((uint8_t *)buff, width, height, stride);
                if (clut != NULL)
                {
                    Set_CLUT(clut, ColorData_RGB888[index_num]);
                }
            break;

            case VDC5_GR_FORMAT_CLUT4:
                FillColor_4BITPIX((uint8_t *)buff, width, height, stride);
                if (clut != NULL)
                {
                    Set_CLUT(clut, ColorData_RGB888[index_num]);
                }
            break;

            case VDC5_GR_FORMAT_CLUT1:
                FillColor_1BITPIX((uint8_t *)buff, width, height, stride);
                if (clut != NULL)
                {
                    Set_CLUT(clut, ColorData_RGB888[index_num]);
                }
            break;

            case VDC5_GR_FORMAT_YCBCR422:
                color_data32        = ColorData_YCC422[index_num];
                color_data32_inv    = color_data32 ^ COLOR_24BIT_MASK;

                FillColor_32BITPIX((uint32_t *)buff, color_data32, color_data32_inv, (uint16_t)((uint32_t)width / DIV2), height, stride);
            break;

            case VDC5_GR_FORMAT_YCBCR444:
                color_data32        = ColorData_YCC444[index_num];
                color_data32_inv    = color_data32 ^ COLOR_24BIT_MASK;

                FillColor_32BITPIX((uint32_t *)buff, color_data32, color_data32_inv, width, height, stride);
            break;

            case VDC5_GR_FORMAT_RGBA5551:
                color_data16        = (uint16_t)((uint32_t)ColorData_RGB555[index_num] << COLOR_SHIFT_1);
                color_data16_inv    = (uint16_t)((uint32_t)color_data16 ^ COLOR_15BIT_MASK_0XFFFE);
                color_data16        = (uint16_t)((uint32_t)color_data16 | ALPHA_VALUE_RGBA5551);
                color_data16_inv    = (uint16_t)((uint32_t)color_data16_inv | ALPHA_VALUE_RGBA5551);

                FillColor_16BITPIX((uint16_t *)buff, color_data16, color_data16_inv, width, height, stride);
            break;

            case VDC5_GR_FORMAT_RGBA8888:
                color_data32        = ColorData_RGB888[index_num] << COLOR_SHIFT_8;
                color_data32_inv    = color_data32 ^ COLOR_24BIT_MASK_0XFFFFFF00;
                color_data32       |= (uint32_t)ALPHA_VALUE_RGBA8888;
                color_data32_inv   |= (uint32_t)ALPHA_VALUE_RGBA8888;

                FillColor_32BITPIX((uint32_t *)buff, color_data32, color_data32_inv, width, height, stride);
            break;

            default:
                /* ERROR!! */
                GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)format);
            break;
        }
    }
}   /* End of function GRAPHICS_CreateFillColorImage() */

/**************************************************************************//**
 * @brief       Create 16bit/pixel Fill Color image
 * @param       [out]buff
 * @param       [in]color_dat
 * @param       [in]inv_color_dat   : Inverted color value
 * @param       [in]width           : Image width [pixel]
 * @param       [in]height          : Image height [line]
 * @param       [in]stride          : Buffer stride [byte]
 * @retval      None
 *****************************************************************************/
static void FillColor_16BITPIX (
    uint16_t    * const buff,
    const uint16_t      color_dat,
    const uint16_t      inv_color_dat,
    const uint16_t      width,
    const uint16_t      height,
    const uint16_t      stride)
{
    uint32_t    h_pixel;
    uint32_t    v_line;
    uint16_t  * lBuff;
    uint32_t    stride_pix;

    stride_pix  = (uint32_t)stride;
    stride_pix  = stride_pix / (uint32_t)(sizeof (uint16_t));

    /* Stroke (top line) */
    lBuff = buff;
    for (h_pixel = 0; h_pixel < (uint32_t)width; h_pixel++)
    {
        *lBuff = inv_color_dat;
        lBuff++;
    }
    /* Fill */
    for (v_line = 1u; v_line < ((uint32_t)height - 1u); v_line++)
    {
        lBuff   = &buff[stride_pix * v_line];
        *lBuff  = inv_color_dat;    /* Stroke (left side line) */
        lBuff++;
        for (h_pixel = 1u; h_pixel < ((uint32_t)width - 1u); h_pixel++)
        {
            *lBuff = color_dat;
            lBuff++;
        }
        *lBuff = inv_color_dat;         /* Stroke (right side line) */
    }
    /* Stroke (bottom line) */
    lBuff = &buff[stride_pix * ((uint32_t)height - 1u)];
    for (h_pixel = 0; h_pixel < (uint32_t)width; h_pixel++)
    {
        *lBuff = inv_color_dat;
        lBuff++;
    }
}   /* End of function FillColor_16BITPIX() */

/**************************************************************************//**
 * @brief       Create 32bit/pixel Fill Color image
 * @param       [out]buff
 * @param       [in]color_dat
 * @param       [in]inv_color_dat   : Inverted color value
 * @param       [in]width           : Image width [pixel]
 * @param       [in]height          : Image height [line]
 * @param       [in]stride          : Buffer stride [byte]
 * @retval      None
 *****************************************************************************/
static void FillColor_32BITPIX (
    uint32_t    * const buff,
    const uint32_t      color_dat,
    const uint32_t      inv_color_dat,
    const uint16_t      width,
    const uint16_t      height,
    const uint16_t      stride)
{
    uint32_t    h_pixel;
    uint32_t    v_line;
    uint32_t  * lBuff;
    uint32_t    stride_pix;

    stride_pix  = (uint32_t)stride;
    stride_pix  = stride_pix / (uint32_t)(sizeof (uint32_t));

    /* Stroke (top line) */
    lBuff = buff;
    for (h_pixel = 0; h_pixel < (uint32_t)width; h_pixel++)
    {
        *lBuff = inv_color_dat;
        lBuff++;
    }
    /* Fill */
    for (v_line = 1u; v_line < ((uint32_t)height - 1u); v_line++)
    {
        lBuff   = &buff[stride_pix * v_line];
        *lBuff  = inv_color_dat;    /* Stroke (left side line) */
        lBuff++;
        for (h_pixel = 1u; h_pixel < ((uint32_t)width - 1u); h_pixel++)
        {
            *lBuff = color_dat;
            lBuff++;
        }
        *lBuff = inv_color_dat;         /* Stroke (right side line) */
    }
    /* Stroke (bottom line) */
    lBuff = &buff[stride_pix * ((uint32_t)height - 1u)];
    for (h_pixel = 0; h_pixel < (uint32_t)width; h_pixel++)
    {
        *lBuff = inv_color_dat;
        lBuff++;
    }
}   /* End of function FillColor_32BITPIX() */

/**************************************************************************//**
 * @brief       Create 8bit/pixel Fill Color image (CLUT8)
 * @param       [out]buff
 * @param       [in]width           : Image width [pixel]
 * @param       [in]height          : Image height [line]
 * @param       [in]stride          : Buffer stride [byte]
 * @retval      None
 *****************************************************************************/
static void FillColor_8BITPIX (uint8_t * const buff, const uint16_t width, const uint16_t height, const uint16_t stride)
{
    uint32_t    h_pixel;
    uint32_t    v_line;
    uint8_t   * lBuff;

    /* Stroke (top line) */
    lBuff = buff;
    for (h_pixel = 0; h_pixel < (uint32_t)width; h_pixel++)
    {
        *lBuff = (uint8_t)1u;
        lBuff++;
    }
    /* Fill */
    for (v_line = 1u; v_line < ((uint32_t)height - 1u); v_line++)
    {
        lBuff   = &buff[(uint32_t)stride * v_line];
        *lBuff  = (uint8_t)1u;      /* Stroke (left side line) */
        lBuff++;
        for (h_pixel = 1u; h_pixel < ((uint32_t)width - 1u); h_pixel++)
        {
            *lBuff = (uint8_t)0;
            lBuff++;
        }
        *lBuff = (uint8_t)1u;           /* Stroke (right side line) */
    }
    /* Stroke (bottom line) */
    lBuff = &buff[(uint32_t)stride * ((uint32_t)height - 1u)];
    for (h_pixel = 0; h_pixel < (uint32_t)width; h_pixel++)
    {
        *lBuff = (uint8_t)1u;
        lBuff++;
    }
}   /* End of function FillColor_8BITPIX() */

/**************************************************************************//**
 * @brief       Create 4bit/pixel Fill Color image (CLUT4)
 * @param       [out]buff
 * @param       [in]width           : Image width [pixel]
 * @param       [in]height          : Image height [line]
 * @param       [in]stride          : Buffer stride [byte]
 * @retval      None
 *****************************************************************************/
static void FillColor_4BITPIX (uint8_t * const buff, const uint16_t width, const uint16_t height, const uint16_t stride)
{
    uint32_t    h_pixel;
    uint32_t    v_line;
    uint32_t    half_width;
    uint8_t   * lBuff;

    half_width  = (uint32_t)width;
    half_width /= DIV2;

    /* Stroke (top line) */
    lBuff = buff;
    for (h_pixel = 0; h_pixel < half_width; h_pixel++)
    {
        *lBuff = (uint8_t)TOP_LINE_4BITPIX;
        lBuff++;
    }
    /* Fill */
    for (v_line = 1u; v_line < ((uint32_t)height - 1u); v_line++)
    {
        lBuff   = &buff[(uint32_t)stride * v_line];
        *lBuff  = (uint8_t)LEFT_SIDE_LINE_4BITPIX;   /* Stroke (left side line) */
        lBuff++;
        for (h_pixel = 1u; h_pixel < half_width; h_pixel++)
        {
            *lBuff = (uint8_t)0;
            lBuff++;
        }
    }
    /* Stroke (bottom line) */
    lBuff = &buff[(uint32_t)stride * ((uint32_t)height - 1u)];
    for (h_pixel = 0; h_pixel < half_width; h_pixel++)
    {
        *lBuff = (uint8_t)BOTTOM_LINE_4BITPIX;
        lBuff++;
    }
    /* Stroke (right side line) */
    if (((uint32_t)width & 1u) != 0u)
    {   /* Odd */
        for (v_line = 0; v_line < (uint32_t)height; v_line++)
        {
            lBuff   = &buff[(((uint32_t)stride * v_line) + half_width) - 1u];
            lBuff++;
            *lBuff  = (uint8_t)((uint32_t)*lBuff & RIGHT_SIDE_LINE_MASK_4BITPIX);
            *lBuff  = (uint8_t)((uint32_t)*lBuff | RIGHT_SIDE_LINE_4BITPIX);
        }
    }
    else
    {   /* Even */
        for (v_line = 1u; v_line < ((uint32_t)height - 1u); v_line++)
        {
            lBuff   = &buff[(((uint32_t)stride * v_line) + half_width) - 1u];
            *lBuff  = (uint8_t)0x01u;
        }
    }
}   /* End of function FillColor_4BITPIX() */

/**************************************************************************//**
 * @brief       Create 1bit/pixel Fill Color image (CLUT1)
 * @param       [out]buff
 * @param       [in]width           : Image width [pixel]
 * @param       [in]height          : Image height [line]
 * @param       [in]stride          : Buffer stride [byte]
 * @retval      None
 *****************************************************************************/
static void FillColor_1BITPIX (uint8_t * const buff, const uint16_t width, const uint16_t height, const uint16_t stride)
{
    uint32_t    h_pixel;
    uint32_t    v_line;
    uint32_t    byte_width;
    uint32_t    rem_width;
    uint8_t   * lBuff;
    uint32_t     mask;
    uint32_t    r_side_bit;

    byte_width  = (uint32_t)width;
    byte_width /= DIV8;
    rem_width   = (uint32_t)width;
    rem_width  %= DIV8;
    mask        = (uint32_t)COLOR_8BIT_MASK >> rem_width;
    r_side_bit  = (uint32_t)1u << (COLOR_SHIFT_8 - rem_width);     /* 0x80, 0x40, 0x20, ... 0x02 */

    /* Stroke (top line) */
    lBuff = buff;
    for (h_pixel = 0; h_pixel < byte_width; h_pixel++)
    {
        *lBuff = (uint8_t)COLOR_8BIT_MASK;
        lBuff++;
    }
    if (rem_width != 0u)
    {
        *lBuff  = (uint8_t)((uint32_t)*lBuff | (~mask));
    }
    /* Fill */
    for (v_line = 1u; v_line < ((uint32_t)height - 1u); v_line++)
    {
        lBuff = &buff[(uint32_t)stride * v_line];
        for (h_pixel = 0; h_pixel < byte_width; h_pixel++)
        {
            *lBuff = (uint8_t)0;
            lBuff++;
        }
    }
    /* Stroke (bottom line) */
    lBuff = &buff[(uint32_t)stride * ((uint32_t)height - 1u)];
    for (h_pixel = 0; h_pixel < byte_width; h_pixel++)
    {
        *lBuff = (uint8_t)COLOR_8BIT_MASK;
        lBuff++;
    }
    if (rem_width != 0u)
    {
        *lBuff  = (uint8_t)((uint32_t)*lBuff | (~mask));
    }
    /* Stroke (left side line) */
    for (v_line = 1u; v_line < ((uint32_t)height - 1u); v_line++)
    {
        lBuff   = &buff[(uint32_t)stride * v_line];
        *lBuff  = (uint8_t)((uint32_t)*lBuff | LEFT_SIDE_LINE_1BITPIX);
    }
    /* Stroke (right side line) */
    if (rem_width == 0u)
    {
        for (v_line = 1u; v_line < ((uint32_t)height - 1u); v_line++)
        {
            lBuff   = &buff[(((uint32_t)stride * v_line) + byte_width) - 1u];
            *lBuff  = (uint8_t)((uint32_t)*lBuff | 0x01u);
        }
    }
    else
    {
        for (v_line = 1u; v_line < ((uint32_t)height - 1u); v_line++)
        {
            lBuff   = &buff[((uint32_t)stride * v_line) + byte_width];
            *lBuff  = (uint8_t)((uint32_t)*lBuff & mask);
            *lBuff  = (uint8_t)((uint32_t)*lBuff | r_side_bit);
        }
    }
}   /* End of function FillColor_1BITPIX() */

/**************************************************************************//**
 * @brief       Set color look-up table
 * @param       [out]clut
 * @param       [in]color_dat
 * @retval      None
 *****************************************************************************/
static void Set_CLUT (uint32_t * clut, uint32_t color_dat)
{
    color_dat |= (uint32_t)(ALPHA_VALUE_CLUT << COLOR_SHIFT_24);

    *clut   = color_dat;
    clut++;
    *clut   = color_dat ^ COLOR_24BIT_MASK;
}   /* End of function Set_CLUT() */

