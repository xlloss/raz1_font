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
 * @file         Lines.c
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
int TrVg_DrawingLine( void ) ;
int TrVg_DrawingLine1( void ) ;
int TrVg_DrawingLinePara(VGuint x,VGuint y, VGuint dx, VGuint dy, VGfloat width, VGuint color);


/******************************************************************************
Private global variables and functions
******************************************************************************/

/**************************************************************************//**
 * Function Name : TrVg_Drawing
 * @brief       
 * @param       [in]void
 * @retval      int     : Return '0', if the error occurred.
 *****************************************************************************/
int TrVg_DrawingLine( void )
{
    VGPath path,fill,stroke ;
    VGfloat x0, y0, x1, y1 ;

    VGfloat Color[4];

    /* White */
    Color[0] = 0.0f;				//B	
    Color[1] = 0.0f;				//G
    Color[2] = 1.0f;				//R
    Color[3] = 0.0f;
    
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
		vgLoadIdentity();
    vgTranslate(0.0f, 0.0f);
 
    
    /* Create a new paint object (for filling) */
    fill = vgCreatePaint();
		vgSetParameteri(fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);

//		vgSetParameterfv( fill, VG_PAINT_COLOR, 4, 0x00FF00ff );
 
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
//    vgSetColor(fill, (VGuint)0xF15A24FF);
		 vgSetColor(fill, (VGuint)0x0000FFff);  		
    vgSetPaint( fill, VG_STROKE_PATH ) ;


#if 0
     stroke = vgCreatePaint();
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
     vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( stroke, ( VGuint )0xFF00FFff ) ;			//BGRA  , set color for LINE.
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
#endif
 
 
    vgSetf( VG_STROKE_LINE_WIDTH, 1.4f ) ;
 
    /*** Line ***/
    x0 = 0.0f ;
    y0 = 0.0f ;
    x1 = 50.0f ;
    y1 = 310.0f ;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
   
    /* Appends a line segment to the path */
    vguLine( path, x0, y0, x1, y1 ) ;
 
    /* Paint object is set on the current context */
    vgSetPaint(fill, VG_FILL_PATH);
    
    vgDrawPath( path, VG_STROKE_PATH ) ;
 
    /* Destroy the path */
    vgDestroyPath( path );

    /* Destroy the paint objects */
    vgDestroyPaint(fill);
//    vgDestroyPaint(stroke);    
    return 1 ;
}

int TrVg_DrawingLinePara(VGuint x,VGuint y, VGuint dx, VGuint dy, VGfloat width, VGuint color)
{
    VGPath path,fill ;
    VGfloat x0, y0, x1, y1 ;
    
    x0 = x;
    y0 = y;
    x1 = dx;
    y1 = dy;

		printf("xxxxx = %d,%d,%d,%d.\n",x,y,dx,dy);
		
#if 0
    /*** Line ***/
    x0 = 400.0f ;
    y0 = 240.0f ;
    x1 = 799.0f ;
    y1 = 450.0f ;
#endif

    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
		vgLoadIdentity();
    vgTranslate(0, 0);
    /* Create a new paint object (for filling) */
    fill = vgCreatePaint();
		vgSetParameteri(fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);

//		vgSetParameterfv( fill, VG_PAINT_COLOR, 4, 0x00FF00ff );
 
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
//    vgSetColor(fill, (VGuint)0xF15A24FF);
		 vgSetColor(fill, (VGuint)color);  		
    vgSetPaint( fill, VG_STROKE_PATH ) ;
    
    
    	
 
    vgSetf( VG_STROKE_LINE_WIDTH, width) ;
 
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
   
    /* Appends a line segment to the path */
    vguLine( path, x0, y0, x1, y1 ) ;
 
    /* Paint object is set on the current context */
    vgSetPaint(fill, VG_FILL_PATH);
    
    vgDrawPath( path, VG_STROKE_PATH ) ;

    /* Destroy the path */
    vgDestroyPath( path ) ;

    /* Destroy the paint objects */
    vgDestroyPaint(fill);
    
    return 1 ;
}

int TrVg_DrawingLine1( void )
{
    VGPaint fill;
    VGPath  path;
    VGfloat width;
    VGfloat height;
    VGfloat pos_x;

    width = 240.0f;
    height = 20.0f;

    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgLoadIdentity();

    vgTranslate(400.0f, 240.0f);

    /* Create a new paint object (for filling) */
    fill = vgCreatePaint();

    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri(fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);

    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
//    vgSetColor(fill, (VGuint)0xF15A24FF);
  	vgSetColor(fill, (VGuint)0xFFFFFFff);
    /* Create a new path */
    path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);

    /* Appends an axis-aligned rectangle to the path */
    vguRect(path, 0.0f, 0.0f, width, height);

    /* Paint object is set on the current context */
    vgSetPaint(fill, VG_FILL_PATH);

    /* The path is filled */
    vgDrawPath(path, VG_FILL_PATH);

    /* Destroy the path */
    vgDestroyPath(path);

    /* Destroy the paint objects */
    vgDestroyPaint(fill);

    return 0;
}   /* End of function DrawPath() */
