
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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         r_text_renderer.h
* @version      0.01
* $Rev$
* $Date::                          #$
* @brief        
******************************************************************************/

#ifndef R_TEXT_RENDERER_H
#define R_TEXT_RENDERER_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    "r_typedefs.h"

#include    "VG/openvg.h"
#include    "VG/vgu.h"
#include    "EGL/egl.h"


/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
    VGFont      font_hdl;       /*!< Handle to font object */
    VGPaint     paint_fill;     /*!< Handle to paint object (fill) */
    VGPaint     paint_stroke;   /*!< Handle to paint object (stroke) */
    VGbitfield  paintmode;      /*!< Paint mode (VG_FILL_PATH / VG_STROKE_PATH) */
    VGfloat     stroke_width;   /*!< Stroke line width */
} r_txtr_hdl_t;

typedef enum
{
//    R_TXTR_FONT_MPLUS_2PM_24 = 0,       /*!< mplus-2p-medium (TrueType), 24pt */
//    R_TXTR_FONT_MPLUS_2PM_36,           /*!< mplus-2p-medium (TrueType), 36pt */
//    R_TXTR_FONT_MPLUS_1MNB_72,          /*!< mplus-1mn-bold (TrueType), 72pt */
//    R_TXTR_FONT_ARIAL_24 = 0,    
		R_TXTR_FONT_ARIALBD_24 = 0,
//    R_TXTR_FONT_TIMES_24,    
//		R_TXTR_FONT_TIMESBD_24,
//    R_TXTR_FONT_VERDANA_24,   
//    R_TXTR_FONT_SEGOEUIB_24,    
    R_TXTR_FONT_NUM
} r_txtr_font_t;


/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/
int32_t R_TXTR_Create(r_txtr_hdl_t * handle, r_txtr_font_t font);
int32_t R_TXTR_Destroy(r_txtr_hdl_t * handle);
int32_t R_TXTR_Set_FillColor(r_txtr_hdl_t * handle, VGuint fill_color);
int32_t R_TXTR_Set_StrokeColor(r_txtr_hdl_t * handle, VGuint stroke_color);
int32_t R_TXTR_Set_PaintMode(r_txtr_hdl_t * handle, VGbitfield paintmode);
int32_t R_TXTR_Set_StrokeWidth(r_txtr_hdl_t * handle, VGfloat stroke_width);
int32_t R_TXTR_Set_FillLinearGradient(
    r_txtr_hdl_t  * handle,
    const VGfloat * vector,
    const VGfloat * rampStops,
    VGint           StopCount);
int32_t R_TXTR_Conv_utf8_ucs2(uint32_t * str_ucs2, const uint8_t * str_utf8, int32_t max_length);
int32_t R_TXTR_Draw(
    r_txtr_hdl_t      * handle,
    char 					*ch_string,
    int32_t             length,
    VGfloat             pos_x,
    VGfloat             pos_y,
    VGfloat             scaling);
 

#endif  /* R_TEXT_RENDERER_H */
