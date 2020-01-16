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
 * @file         ImageFilters.c
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
extern uint8_t _CLOCKS_ADDR[];

/******************************************************************************
Definitions
******************************************************************************/
#define     IMG_1_WIDTH     100
#define     IMG_1_HEIGHT    100

#define     IMG_2_WIDTH     64
#define     IMG_2_HEIGHT    80

#define     IMG_3_WIDTH     256
#define     IMG_3_HEIGHT    72

#define     IMG_4_WIDTH     80
#define     IMG_4_HEIGHT    80
/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
int TrVg_DrawingFilters( void ) ;


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* 3x3 convolution kernel values in column-major order (sharpness filter / 4-connectivity neighborhood) */
static const VGshort kernel[ 9 ] =
{
    0, -1, 0, -1, 5, -1, 0, -1, 0
} ;

/* Convolution one-dimensional (horizontal) kernel values (Sobel filter for horizontal changes) */
static const VGshort kernel_X[ 3 ] =
{
    -1, 0, 1
} ;
/* Convolution one-dimensional (vertical) kernel values (Sobel filter for horizontal changes) */
static const VGshort kernel_Y[ 3 ] =
{
    1, 2, 1
} ;


/**************************************************************************//**
 * Function Name : TrVg_Drawing
 * @brief       
 * @param       [in]void
 * @retval      int     : Return '0', if the error occurred.
 *****************************************************************************/
int TrVg_DrawingFilters( void )
{
    VGImage Image ;
    VGImage Image_flt ;
    VGfloat stdDeviationX, stdDeviationY ;      /* Standard deviation */

    VGint   width;
    VGint   height;

    width   = 200;
    height  = 200;
    
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE ) ;
		vgLoadIdentity();
    vgTranslate(10.0f, 10.0f);
    
    /*** Image Filter (Gaussian blur) ***/
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

    /* Create a new image */
    Image_flt = vgCreateImage( VG_lXRGB_8888, width, height, VG_IMAGE_QUALITY_FASTER ) ;
    if( Image_flt  == VG_INVALID_HANDLE ){
        return 0 ;
    }
    /* Gaussian blur */
    stdDeviationX = 0.5f ;
    stdDeviationY = 0.5f ;
    vgGaussianBlur( Image_flt, Image, stdDeviationX, stdDeviationY, VG_TILE_FILL ) ;
 
    vgLoadIdentity() ;
    /* Draw the image to the current drawing surface */
    vgTranslate( 40.0f, 20.0f ) ;
    vgDrawImage( Image ) ;
    /* Draw the image to the current drawing surface */
    vgTranslate( 0.0f, 100.0f ) ;
    vgDrawImage( Image_flt ) ;

    /* Destroy the image */
    vgDestroyImage( Image ) ;
    vgDestroyImage( Image_flt ) ;


#if 0
    /*** Image Filter (user-specified) ***/
    /* Create a new image */
    Image = vgCreateImage( VG_lXRGB_8888, width, height, VG_IMAGE_QUALITY_FASTER ) ;
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

    /* Create a new image */
    Image_flt = vgCreateImage( VG_lXRGB_8888, width, height, VG_IMAGE_QUALITY_FASTER ) ;
    if( Image_flt  == VG_INVALID_HANDLE ){
        return 0 ;
    }
    /* Apply a user-specified convolution kernel */
    vgConvolve( Image_flt, Image,
                ( VGint )3, ( VGint )3,     /* The dimensions of the kernel, width and height */
                ( VGint )0, ( VGint )0,     /* The translation parameters between src and dst images */
                kernel,
                ( VGfloat )1.0f, ( VGfloat )0.0f,   /* Scale and bias */
                VG_TILE_PAD ) ;

    vgLoadIdentity() ;
    /* Draw the image to the current drawing surface */
    vgTranslate( 300.0f, 20.0f ) ;
    vgDrawImage( Image ) ;
    /* Draw the image to the current drawing surface */
    vgTranslate( 0.0f, 80.0f ) ;
    vgDrawImage( Image_flt ) ;

    /* Apply a user-specified separable (two-dimensional) convolution kernel */
    vgSeparableConvolve( Image_flt, Image,
                        ( VGint )3, ( VGint )3,     /* The dimensions of the kernel, width and height */
                        ( VGint )0, ( VGint )0,     /* The translation parameters between src and dst images */
                        kernel_X, kernel_Y,
                        ( VGfloat )1.0f, ( VGfloat )0.0f,   /* Scale and bias */
                        VG_TILE_PAD ) ;

    /* Draw the image to the current drawing surface */
    vgTranslate( 0.0f, 80.0f ) ;
    vgDrawImage( Image_flt ) ;

    /* Destroy the image */
    vgDestroyImage( Image ) ;
    vgDestroyImage( Image_flt ) ;
    
#endif

    return 1 ;
}


int TrVg_DrawingFiltersClock160160( int x, int y )
{
    VGImage Image ;
    VGImage Image_flt ;
    VGfloat stdDeviationX, stdDeviationY ;      /* Standard deviation */

    VGint   width;
    VGint   height;
    
//		DspBMP(_CLOCKS_ADDR,0,0,Graphics_ImageData32, 1);					//OpenVG use  buffer Graphics_ImageData32, scan from bottom  

    width   = 200;
    height  = 200;
    
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE ) ;
		vgLoadIdentity();
     vgTranslate(x, 480-y-160); 				//translate BMP coordination to OpenVG's
    
    /*** Image Filter (Gaussian blur) ***/
    /* Create a new image */
    Image = vgCreateImage(VG_lXRGB_8888, 160, 160, VG_IMAGE_QUALITY_BETTER);
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

    /* Create a new image */
    Image_flt = vgCreateImage( VG_lXRGB_8888, width, height, VG_IMAGE_QUALITY_FASTER ) ;
    if( Image_flt  == VG_INVALID_HANDLE ){
        return 0 ;
    }
    /* Gaussian blur */
    stdDeviationX = 0.5f ;
    stdDeviationY = 0.5f ;
    vgGaussianBlur( Image_flt, Image, stdDeviationX, stdDeviationY, VG_TILE_FILL ) ;
 
    vgLoadIdentity() ;
    /* Draw the image to the current drawing surface */
//      vgTranslate(x, 480-y-160); 				//translate BMP coordination to OpenVG's
//      vgDrawImage( Image ) ;
    
    /* Draw the image to the current drawing surface */
      vgTranslate(x, 480-y-160); 				//translate BMP coordination to OpenVG's
     vgDrawImage( Image_flt ) ;

    /* Destroy the image */
    vgDestroyImage( Image ) ;
    vgDestroyImage( Image_flt ) ;
 
    return 1 ;
}
