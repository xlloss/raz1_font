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
 * @file         RadialGradientPaint.c
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

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
int TrVg_DrawingRadialGradien( void ) ;


/******************************************************************************
Private global variables and functions
******************************************************************************/
static const VGfloat RampStopTbl[ 5 * 3 ] =
{   /* Offset, Red, Green, Blue, Alpha */
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   /* Red, 0xFF0000FF */
    0.5f, 0.0f, 1.0f, 0.0f, 1.0f,   /* Green, 0x00FF00FF */
    1.0f, 0.0f, 0.0f, 1.0f, 1.0f    /* Blue, 0x0000FFFF */
} ;


/**************************************************************************//**
 * Function Name : TrVg_Drawing
 * @brief       
 * @param       [in]void
 * @retval      int     : Return '0', if the error occurred.
 *****************************************************************************/
int TrVg_DrawingRadialGradien( void )
{
    VGPath path ;
    VGfloat cx, cy, width, height ;
    VGPaint fill, stroke ;
    VGfloat RadGradient[ 5 ] ;      /* Center (x, y), Focal (x, y), Radius r */

    vgSetf( VG_STROKE_LINE_WIDTH, 16.0f ) ;

    /* Create a new paint object (for filling) */
    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_RADIAL_GRADIENT */
    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_RADIAL_GRADIENT ) ;

    /* Create a new paint object (for stroking) */
    stroke = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_RADIAL_GRADIENT */
    vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_RADIAL_GRADIENT ) ;

    /*** Ellipse filled (radial gradient) ***/
    cx = 120.0f ;
    cy = 60.0f ;
    width = 160.0f ;
    height = 100.0f ;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends an axis-aligned ellipse to the path */
    vguEllipse( path, cx, cy, width, height ) ;

    RadGradient[ 0 ] = cx ;
    RadGradient[ 1 ] = cy ;
    RadGradient[ 2 ] = RadGradient[ 0 ] ;
    RadGradient[ 3 ] = RadGradient[ 1 ] ;
    RadGradient[ 4 ] = width / 2.0f ;
    /* Set the gradient parameters to the paint object */
    vgSetParameterfv( fill, VG_PAINT_RADIAL_GRADIENT, sizeof RadGradient / ( sizeof ( VGfloat )), RadGradient ) ;
    /* Set the color ramp parameters */
    vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, sizeof RampStopTbl / ( sizeof ( VGfloat )), RampStopTbl ) ;
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH ) ;
    /* The path is filled */
    vgDrawPath( path, VG_FILL_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
    vgDestroyPaint( fill ) ;
    vgDestroyPaint( stroke ) ;

#if 0
    fill = vgCreatePaint() ;
    /*** Ellipse filled (radial gradient) ***/
    cx = 120.0f ;
    cy = 200.0f ;
    width = 160.0f ;
    height = 100.0f ;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends an axis-aligned ellipse to the path */
    vguEllipse( path, cx, cy, width, height ) ;

    RadGradient[ 0 ] = cx ;
    RadGradient[ 1 ] = cy ;
    RadGradient[ 2 ] = cx + width*2 ;
    RadGradient[ 3 ] = RadGradient[ 1 ]+height*2 ;
    RadGradient[ 4 ] = width / 2.0f ;
    /* Set the gradient parameters to the paint object */
    vgSetParameterfv( fill, VG_PAINT_RADIAL_GRADIENT, sizeof RadGradient / ( sizeof ( VGfloat )), RadGradient ) ;
    /* Set the color ramp parameters */
    vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, sizeof RampStopTbl / ( sizeof ( VGfloat )), RampStopTbl ) ;
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH ) ;
    /* The path is filled */
    vgDrawPath( path, VG_FILL_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;

    /*** Ellipse stroked (radial gradient) ***/
    cx = 320.0f ;
    cy = 60.0f ;
    width = 160.0f ;
    height = 100.0f ;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends an axis-aligned ellipse to the path */
    vguEllipse( path, cx, cy, width, height ) ;

    RadGradient[ 0 ] = cx - width / 2.0f ;
    RadGradient[ 1 ] = cy ;
    RadGradient[ 2 ] = RadGradient[ 0 ] ;
    RadGradient[ 3 ] = RadGradient[ 1 ] ;
    RadGradient[ 4 ] = width ;
    /* Set the gradient parameters to the paint object */
    vgSetParameterfv( stroke, VG_PAINT_RADIAL_GRADIENT, sizeof RadGradient / ( sizeof ( VGfloat )), RadGradient ) ;
    /* Set the color ramp parameters */
    vgSetParameterfv( stroke, VG_PAINT_COLOR_RAMP_STOPS, sizeof RampStopTbl / ( sizeof ( VGfloat )), RampStopTbl ) ;
    /* Paint object is set on the current context */
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;

    /* Destroy the paint objects */
    vgDestroyPaint( fill ) ;
    vgDestroyPaint( stroke ) ;

#endif

    return 1 ;
}
