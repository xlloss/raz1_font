/******************************************************************************
 *
 * $Rev$
 * $Date::                           #$
 *
 *****************************************************************************/
/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************
 * Copyright (C) 2011 Renesas Electronics Corp. All Rights Reserved
 * AND Renesas Solutions Corp. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
 * @file         MaskingImage.c
 * @version      1.00
 * @brief        
 *****************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version Description
*         : 11.11.2011 1.00    First Release
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include    <stdio.h>
#include    <string.h>

#include    "r_typedefs.h"

#include    "VG/openvg.h"
#include    "VG/vgu.h"

extern uint16_t Graphics_ImageData[];
extern uint32_t Graphics_ImageData32[];					/* 32bits= B,G,R,00*/
extern uint8_t __image_btn1[];


/******************************************************************************
Definitions
******************************************************************************/
/* Number of scissoring rectangles */
#define     SCISSOR_RECT_NUM        2
 

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
int TrVg_DrawingMasking( void ) ;
int TrVg_DrawingScissoring( void ) ;

/******************************************************************************
Private global variables and functions
******************************************************************************/
static void DrawPathMasking( void ) ;
static const VGint RectCoords[ 4 * SCISSOR_RECT_NUM ] =
{   /* Min x, Min y, Width, Height */
    0, 0, 400, 40,
    60, 60, 280, 120
} ;

/**************************************************************************//**
 * Function Name : TrVg_Drawing
 * @brief       
 * @param       [in]void
 * @retval      int     : Return '0', if the error occurred.
 *****************************************************************************/
int TrVg_DrawingMasking( void )
{
    VGImage Image ;
    VGPath path ;
    VGPaint fill ;
    VGfloat scale;
    
    VGint   width;
    VGint   height;

    width   = 200;
    height  = 200;
#if 1									// it is must to have if draw image
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
		vgLoadIdentity();
    vgTranslate(10.0f, 10.0f);
#endif

#if 0
    vgTranslate(10.0f, 10.0f);
		scale = 1.0f;
    vgScale(scale, scale);
    /* Set resampling quality */
    vgSeti(VG_IMAGE_QUALITY, VG_IMAGE_QUALITY_BETTER);
#endif

    /*** Image ***/
    /* Create a new image */
    Image = vgCreateImage(VG_lXRGB_8888, width, height, VG_IMAGE_QUALITY_BETTER);
    if( Image  == VG_INVALID_HANDLE ){
        return 0 ;
    }
    /* Append the image data to the image object. If necessary, data format conversion will be done. */
    vgImageSubData(
        Image,
        Graphics_ImageData32,
        width * (sizeof (uint32_t)),
        VG_lXRGB_8888,																	//VG_sRGB_565,
        (VGint)0,
        (VGint)0,
        width,
        height);

    /* Modifies the drawing surface mask values */
		vgMask(( VGHandle )Image, VG_CLEAR_MASK, ( VGint )40, ( VGint )40, ( VGint )120, ( VGint )120 ) ;

    /* Destroy the image */
 
    /* Enable masking */
    vgSeti( VG_MASKING, VG_TRUE ) ;
    vgDrawImage(Image);

    vgTranslate(10.0f, 10.0f);    
		
		DrawPathMasking() ;

    /* Disable masking */
    vgSeti( VG_MASKING, VG_FALSE ) ;
 
    vgDestroyImage( Image ) ;
 
    return 1 ;
}

/**************************************************************************//**
 * Function Name : DrawPath
 * @brief       
 * @param       [in]void
 * @retval      void
 *****************************************************************************/
static void DrawPathMasking( void )
{
    VGPath path ;
    VGPaint fill ;

    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgLoadIdentity();

    vgTranslate(0.0f, 0.0f);
    
    fill = vgCreatePaint();
    
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri(fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
 
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends an axis-aligned rectangle to the path */
    vguRect( path, 10.0f, 10.0f, 100.0f, 100.0f ) ;

    /* Create a new paint object (for filling) */
//    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
//    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    
    vgSetColor( fill, ( VGuint )0x0000FFFF ) ;
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH ) ;
 
    /* The path is filled */
    vgDrawPath( path, VG_FILL_PATH ) ;

    /* Destroy the path */
    vgDestroyPath( path ) ;
    /* Destroy the paint objects */
    vgDestroyPaint( fill ) ;
}

int TrVg_DrawingScissoring( void )
{
    VGImage Image ;
    VGPath path ;
    VGPaint fill ;
    VGint   width;
    VGint   height;

    width   = 200;
    height  = 200;

#if 1									// it is must to have if draw image
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
		vgLoadIdentity();

#endif

    /*** Image ***/
    /* Create a new image */
    Image = vgCreateImage(VG_lXRGB_8888, width, height, VG_IMAGE_QUALITY_BETTER);
    if( Image  == VG_INVALID_HANDLE ){
        return 0 ;
    }
    /* Append the image data to the image object. If necessary, data format conversion will be done. */
    vgImageSubData(
        Image,
        Graphics_ImageData32,
        width * (sizeof (uint32_t)),
        VG_lXRGB_8888,																	//VG_sRGB_565,
        (VGint)0,
        (VGint)0,
        width,
        height);

    /*** Rectangle ***/
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends an axis-aligned rectangle to the path */
    vguRect( path, 250.0f, 0.0f, 200.0f, 200.0f ) ;

    /* Create a new paint object (for filling) */
    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( fill, ( VGuint )0xFF0000ff ) ;
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH ) ;

    /* Set the matrix mode to allow manipulation of the image-user-to-surface transformation */
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE ) ;
    /* Set the current matrix to the identity matrix  */

    vgTranslate(400.0f, 10.0f);
    
    vgLoadIdentity() ;
    vgScale( 2.0f, 2.0f ) ;

    /* Set the scissoring rectangles */
    vgSetiv( VG_SCISSOR_RECTS, 4 * SCISSOR_RECT_NUM, RectCoords ) ;
    /* Enable scissoring */
    vgSeti( VG_SCISSORING, VG_TRUE ) ;

    /* Draw the image to the current drawing surface */
    vgDrawImage( Image ) ;
    /* The path is filled */
    vgDrawPath( path, VG_FILL_PATH ) ;

    /* Disable scissoring */
    vgSeti( VG_SCISSORING, VG_FALSE ) ;

    /* Destroy the image */
    vgDestroyImage( Image ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
    /* Destroy the paint objects */
    vgDestroyPaint( fill ) ;

    return 1 ;
}
