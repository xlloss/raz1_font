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
 * @file         RenderingQuality.c
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

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
int TrVg_DrawingRenderQuality( void ) ;


/******************************************************************************
Private global variables and functions
******************************************************************************/

/**************************************************************************//**
 * Function Name : TrVg_Drawing
 * @brief       
 * @param       [in]void
 * @retval      int     : Return '0', if the error occurred.
 *****************************************************************************/
int TrVg_DrawingRenderQuality( void )
{
    VGImage Image ;
    VGPath path ;
    VGPaint fill ;
    
    VGint   width;
    VGint   height;

    width   = 200;
    height  = 200;
    
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

    vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE ) ;
    vgLoadIdentity() ;
//     vgShear( 0.48f, 0.52f );
        vgShear( 0.0f, 0.0f );
        
    vgTranslate( 20.0f, 10.0f );

    /* Set the rendering quality to VG_RENDERING_QUALITY_NONANTIALIASED */
    vgSeti( VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_BETTER ) ;
    /* Draw the image to the current drawing surface */
    vgDrawImage( Image );

#if 0
    vgTranslate( 120.0f, 0.0f );
    /* Set the rendering quality to VG_RENDERING_QUALITY_FASTER */
    vgSeti( VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_BETTER ) ;
    /* Draw the image to the current drawing surface */
    vgDrawImage( Image );

    vgTranslate( 120.0f, 0.0f );
    /* Set the rendering quality to VG_RENDERING_QUALITY_BETTER */
    vgSeti( VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_BETTER ) ;
    /* Draw the image to the current drawing surface */
    vgDrawImage( Image ) ;

#endif

    /* Destroy the image */
    vgDestroyImage( Image ) ;
 
 
 
    /*** Path ***/
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends an elliptical arc to the path */
//    vguArc( path, 100.0, 140.0f, 160.0f, 120.0f, 60.0f, 90.0f, VGU_ARC_PIE ) ;

    vguArc( path, 400.0, 140.0f, 200.0f, 200.0f, 0.0f, 360.0f, VGU_ARC_PIE ) ;
    
    /* Create a new paint object (for filling) */
    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( fill, ( VGuint )0xFF4000ff ) ;
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH ) ;

    vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE ) ;
    vgLoadIdentity() ;
    vgTranslate( 0.0f, 0.0f ) ;
    /* Set the rendering quality to VG_RENDERING_QUALITY_NONANTIALIASED */
//    vgSeti( VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_NONANTIALIASED ) ;
    vgSeti( VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_BETTER ) ;
    
    /* The path is filled */
    vgDrawPath( path, VG_FILL_PATH ) ;
 
#if 1
    vgSetColor( fill, ( VGuint )0x101010ff ) ;
    vgTranslate( 120.0f, 0.0f ) ;
    /* Set the rendering quality to VG_RENDERING_QUALITY_FASTER */
    vgSeti( VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_BETTER ) ;
    /* The path is filled */
    vgDrawPath( path, VG_FILL_PATH ) ;

    vgSetColor( fill, ( VGuint )0xFFFFFFff ) ;
    vgTranslate( 120.0f, 0.0f ) ;
    /* Set the rendering quality to VG_RENDERING_QUALITY_BETTER */
    vgSeti( VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_BETTER ) ;
    /* The path is filled */
    vgDrawPath( path, VG_FILL_PATH ) ;
    
    
    
#endif    
    

    /* Destroy the path */
    vgDestroyPath( path ) ;
    /* Destroy the paint object */
    vgDestroyPaint( fill ) ;

    return 1 ;
}
