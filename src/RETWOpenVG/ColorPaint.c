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
 * @file         ColorPaint.c
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
int TrVg_DrawingColorPaint( void ) ;


/******************************************************************************
Private global variables and functions
******************************************************************************/
static const VGfloat ColorTbl[ 4 ] =
{   /* Red, Green, Blue, Alpha */
    1.00f, 0.00f, 0.00f, 1.0f							//Set Color for fill
} ;


/**************************************************************************//**
 * Function Name : TrVg_Drawing
 * @brief       
 * @param       [in]void
 * @retval      int     : Return '0', if the error occurred.
 *****************************************************************************/
int TrVg_DrawingColorPaint( void )
{
    VGPath path ;
    VGfloat x, y, width, height ;
    VGPaint fill, stroke ;
 
    vgSetf( VG_STROKE_LINE_WIDTH, 7.0f ) ;

	vgLoadIdentity();						//re-located org in 0,0
	vgTranslate(0.0f, 0.0f);			
    
    /* Create a new paint object (for filling) */
    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value to the paint object */
//    vgSetParameterfv( fill, VG_PAINT_COLOR, sizeof ColorTbl / ( sizeof ( VGfloat )), ColorTbl ) ;
    vgSetColor( fill, ( VGuint )0x0000FFff ) ;			//BGRA  , set color for LINE.
    /* Create a new paint object (for stroking) */
    
#if 1    
    stroke = vgCreatePaint();
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( stroke, ( VGuint )0x00FF00ff ) ;			//BGRA  , set color for LINE.
#endif

    y = 80.0f ;
    width = 120.0f ;
    height = 80.0f ;

    /*** Rectangle filled with a single color ***/
    x = 20.0f ;
    /* Create a new path */

//	vgSetColor( path, ( VGuint )0x00FF00ff ) ;			//BGRA  , set color for LINE.
//	vgSetPaint( path, VG_STROKE_PATH ) ;   
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
 
    /* Appends an axis-aligned rectangle to the path */
    vguRect( path, x, y, width, height ) ;
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH ) ;
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
    
    /* The path is filled */
//    vgDrawPath( path, VG_FILL_PATH ) ;

    vgDrawPath( path, VG_FILL_PATH ) ;
    
    /* Destroy the path */
     vgDestroyPath( path );
#if 0
    /*** Rectangle stroked with a single color ***/
    x = 160.0f ;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends an axis-aligned rectangle to the path */
    vguRect( path, x, y, width, height ) ;
    /* Paint object is set on the current context */
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
#endif
    /*** Rectangle filled and stroked ***/
    x = 300.0f ;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends an axis-aligned rectangle to the path */
    vguRect( path, x, y, width, height ) ;
    /* Fill and stroke paint objects have been already set on the current context. */
    /* The path is filled and stroked */
    vgDrawPath( path, VG_FILL_PATH | VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path );
    /* Destroy the paint objects */
    vgDestroyPaint( fill ) ;
    vgDestroyPaint( stroke ) ;

    return 1 ;
}
