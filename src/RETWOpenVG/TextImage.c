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
 * @file         TextImage.c
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

/******************************************************************************
Definitions
******************************************************************************/
#define     IMG_S_WIDTH     32
#define     IMG_S_HEIGHT    64
#define     IMG_H_WIDTH     32
#define     IMG_H_HEIGHT    64

typedef enum
{
    CHAR_CODE_S = 0,
    CHAR_CODE_H,
    CHAR_CODE_SP,

    CHAR_CODE_NUM
} CharCode ;


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
int TrVg_DrawingTextImage( void ) ;


/******************************************************************************
Private global variables and functions
******************************************************************************/
static void setPaint( void ) ;
static void delPaint( void ) ;

static const unsigned long ImageData_S[] =
{
    0x007FFF00, 0x00FFFFC0, 0x03FFFFE0, 0x07FFFFF0,
    0x0FFFFFF8, 0x0FFFFFF8, 0x1FFFFFFC, 0x1FF807FC,
    0x3FE001FE, 0x3FC000FE, 0x3F80007E, 0x7F00003F,
    0x7F00003F, 0x7E00001F, 0x7E00001F, 0xFC00000F,
    0xFC00000F, 0xFC00000F, 0xFC00000F, 0xFC00001F,
    0xFC00001F, 0x0000003F, 0x0000003F, 0x0000007F,
    0x000000FE, 0x000003FE, 0x00000FFE, 0x00003FFC,
    0x0001FFFC, 0x0007FFF8, 0x001FFFF0, 0x007FFFC0,
    0x00FFFF80, 0x01FFFE00, 0x03FFFC00, 0x07FFE000,
    0x0FFF8000, 0x1FFC0000, 0x1FF00000, 0x3FE00000,
    0x3F800000, 0x7F000000, 0x7F000000, 0x7E000000,
    0xFE00003E, 0xFC00007E, 0xFC00007E, 0xFC00007E,
    0x7E00007E, 0x7E0000FC, 0x7F0000FC, 0x3F0001FC,
    0x3F8001FC, 0x3FC003F8, 0x1FE007F8, 0x1FF83FF0,
    0x0FFFFFF0, 0x0FFFFFE0, 0x07FFFFE0, 0x03FFFFC0,
    0x01FFFF80, 0x007FFE00, 0x001FF800, 0x00000000
} ;
static const unsigned long ImageData_H[] =
{
    0xF800001F, 0xF800001F, 0xF800001F, 0xF800001F,
    0xF800001F, 0xF800001F, 0xF800001F, 0xF800001F,
    0xF800001F, 0xF800001F, 0xF800001F, 0xF800001F,
    0xF800001F, 0xF800001F, 0xF800001F, 0xF800001F,
    0xF800001F, 0xF800001F, 0xF800001F, 0xF800001F,
    0xF800001F, 0xF800001F, 0xF800001F, 0xF800001F,
    0xF800001F, 0xF800001F, 0xF800001F, 0xF800001F,
    0xF800001F, 0xFC00003F, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFC00003F, 0xF800001F, 0xF800001F,
    0xF800001F, 0xF800001F, 0xF800001F, 0xF800001F,
    0xF800001F, 0xF800001F, 0xF800001F, 0xF800001F,
    0xF800001F, 0xF800001F, 0xF800001F, 0xF800001F,
    0xF800001F, 0xF800001F, 0xF800001F, 0xF800001F,
    0xF800001F, 0xF800001F, 0xF800001F, 0xF800001F,
    0xF800001F, 0xF800001F, 0xF800001F, 0xF800001F
} ;


/**************************************************************************//**
 * Function Name : TrVg_Drawing
 * @brief       
 * @param       [in]void
 * @retval      int     : Return '0', if the error occurred.
 *****************************************************************************/
int TrVg_DrawingTextImage( void )
{
    VGFont FontHdl ;
    VGImage Image ;
    VGPaint fill ;
    VGfloat Origin[] = { 0.0f, 0.0f } ;
    VGfloat Escapement[] = { 32.0f, 0.0f } ;

    /* Creates a new font object */
    FontHdl = vgCreateFont( CHAR_CODE_NUM ) ;
    if( FontHdl == VG_INVALID_HANDLE ){
        return 0 ;
    }

    /*** Glyph of the character 'S' ***/
    /* Create a new image */
    Image = vgCreateImage( VG_BW_1, IMG_S_WIDTH, IMG_S_HEIGHT, VG_IMAGE_QUALITY_FASTER ) ;
    if( Image  == VG_INVALID_HANDLE ){
        return 0 ;
    }
    /* Append the image data to the image object. If necessary, data format conversion will be done. */
    vgImageSubData( Image, ImageData_S, IMG_S_WIDTH / 8 ,
                    VG_BW_1, ( VGint )0, ( VGint )0, IMG_S_WIDTH, IMG_S_HEIGHT ) ;
    /* Creates a glyph and assigns the given image to a glyph that corresponds to the specified index */
    vgSetGlyphToImage( FontHdl, ( VGuint )CHAR_CODE_S, Image, Origin, Escapement ) ;
    /* Destroy the image */
    vgDestroyImage( Image ) ;

    /*** Glyph of the character 'H' ***/
    /* Create a new image */
    Image = vgCreateImage( VG_BW_1, IMG_H_WIDTH, IMG_H_HEIGHT, VG_IMAGE_QUALITY_FASTER ) ;
    if( Image  == VG_INVALID_HANDLE ){
        return 0 ;
    }
    /* Append the image data to the image object. If necessary, data format conversion will be done. */
    vgImageSubData( Image, ImageData_H, IMG_H_WIDTH / 8 ,
                    VG_BW_1, ( VGint )0, ( VGint )0, IMG_H_WIDTH, IMG_H_HEIGHT ) ;
    /* Creates a glyph and assigns the given image to a glyph that corresponds to the specified index */
    vgSetGlyphToImage( FontHdl, ( VGuint )CHAR_CODE_H, Image, Origin, Escapement ) ;
    /* Destroy the image */
    vgDestroyImage( Image ) ;

    /*** Glyph of the character ' '(space) ***/
    /* Creates a glyph and assigns the given image to a glyph that corresponds to the specified index
       For no visual representation like space, VG_INVALID_HANDLE can be used */
    vgSetGlyphToImage( FontHdl, ( VGuint )CHAR_CODE_SP, VG_INVALID_HANDLE, Origin, Escapement ) ;

    /* Set the matrix mode to allow manipulation of the glyph-user-to-surface transformation */
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE ) ;
    /* Set the current matrix to the identity matrix  */
    vgLoadIdentity() ;

    /* Create a new paint object */
    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( fill, ( VGuint )0x0000FFFF ) ;          /* Blue */
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH ) ;

    /* VG_IMAGE_MODE: VG_DRAW_IMAGE_STENCIL */
    vgSeti( VG_IMAGE_MODE, VG_DRAW_IMAGE_STENCIL ) ;
    /* Set the blend mode to VG_BLEND_SRC_OVER */
    vgSeti( VG_BLEND_MODE, VG_BLEND_SRC_OVER ) ;

    /* Set the glyph origin */
    vgSetfv( VG_GLYPH_ORIGIN, 2, Origin ) ;
    /* Renders a glyph specified by the index */
    vgDrawGlyph( FontHdl, ( VGuint )CHAR_CODE_S, VG_FILL_PATH, VG_FALSE ) ;     /* 'S' */
    vgDrawGlyph( FontHdl, ( VGuint )CHAR_CODE_SP, VG_FILL_PATH, VG_FALSE ) ;    /* ' ' */
    vgDrawGlyph( FontHdl, ( VGuint )CHAR_CODE_H, VG_FILL_PATH, VG_FALSE ) ;     /* 'H' */

    /* Destroy the paint objects */
    vgDestroyPaint( fill ) ;

    /* Destroys the font object */
    vgDestroyFont( FontHdl ) ;

    return 1 ;
}
