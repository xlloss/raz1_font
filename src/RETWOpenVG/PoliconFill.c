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
 * @file         Fill.c
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
#include    "RETWvg.h"

/******************************************************************************
Definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
int TrVg_DrawingPolyconFill( void ) ;
int TrVg_DrawingPolyconFillPara( VGuint color );

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Polygonal line 1 */
static const VGfloat Data_Tbl_1[] =
{
    115.273f, 78.533f, 146.78f, 176.0f, 64.0f, 115.668f, 166.433f, 115.82f, 83.474f, 175.906f
} ;
/* Polygonal line 2 */
static const VGfloat Data_Tbl_2[] =
{
//    275.273f, 78.533f, 306.779f, 176.0f, 224.0f, 115.668f, 326.434f, 115.82f, 243.475f, 175.906f
    0.0f, 0.0f,   322.0f, 10.0f,   300.0, 202.0f	,   0, 322.0f			//, 326.434f, 115.82f				//, 243.475f, 175.906f
} ;

static const VGfloat RampStopTbl[ 12 * 3 ] =
{   /* Offset, Red, Green, Blue, Alpha */
    0.0f, 1.0f, 1.0f, 1.0f, 1.0f,   /* Red, 0xFF0000FF */
    0.5f, 0.5f, 0.5f, 0.5f, 1.0f,   /* Green, 0x00FF00FF */
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   /* Blue, 0x0000FFFF */
    0.0f, 1.0f, 1.0f, 1.0f, 1.0f,   /* Red, 0xFF0000FF */
    0.5f, 0.5f, 0.5f, 0.5f, 1.0f,   /* Green, 0x00FF00FF */
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   /* Blue, 0x0000FFFF */
    0.0f, 1.0f, 1.0f, 1.0f, 1.0f,   /* Red, 0xFF0000FF */
    0.5f, 0.5f, 0.5f, 0.5f, 1.0f,   /* Green, 0x00FF00FF */
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   /* Blue, 0x0000FFFF */
    0.0f, 1.0f, 1.0f, 1.0f, 1.0f,   /* Red, 0xFF0000FF */
    0.5f, 0.5f, 0.5f, 0.5f, 1.0f,   /* Green, 0x00FF00FF */
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f    /* Blue, 0x0000FFFF */
};

/* Polygonal line 1 */

extern int Polycon_DrawPointNum;

extern VGfloat Polycon_DataTab[];
extern VGfloat Polycon_linearGradient[];
extern VGfloat Polycon_RampStopTab[];
extern VGfloat Polycon_RadGradient[] ;      /* Center (x, y), Focal (x, y), Radius r */
 
/**************************************************************************//**
 * Function Name : TrVg_Drawing
 * @brief       
 * @param       [in]void
 * @retval      int     : Return '0', if the error occurred.
 *****************************************************************************/
int TrVg_DrawingPolyconFill( void )
{
    VGPath path ;
    const VGfloat *points ;
    VGint count ;       /* The number of points */
    VGPaint fill;

    VGfloat cx, cy, width, height ;
    VGfloat RadGradient[ 5 ] ;      /* Center (x, y), Focal (x, y), Radius r */
    
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgLoadIdentity();

    vgTranslate(0.0f, 0.0f); 
 
    /* Create a new paint object (for filling) */
    fill = vgCreatePaint();

    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri(fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);

    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
//    vgSetColor(fill, (VGuint)0xF15A24FF);
 
  	vgSetColor(fill, (VGuint)0x0000FF2f);
    
#if 1
    /* Polygonal Line (fill rule "even/odd") */
//    vgSeti( VG_FILL_RULE, VG_EVEN_ODD ) ;
    points = Data_Tbl_1 ;
    count = sizeof Data_Tbl_1 / ( sizeof ( VGfloat )) / 2u ;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends a polygon segment to the path */
 
    vguPolygon( path, points, count, VG_TRUE ) ;
 
    vgSetPaint(fill, VG_FILL_PATH);
     
    /* The path is filled */
    vgDrawPath( path, VG_FILL_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
    vgDestroyPaint(fill);
#endif

#if 0    
    fill = vgCreatePaint();
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
//    vgSetParameteri(fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
//		vgSetColor(fill, (VGuint)0xFF0000ff);  

		vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_LINEAR_GRADIENT );
 		vgSetParameterfv( fill, VG_PAINT_LINEAR_GRADIENT, 4, Polycon_linearGradient );
    vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, 15, RampStopTbl ) ;
    
    /* Polygonal Line (fill rule "non-zero") */
//    vgSeti( VG_FILL_RULE, VG_NON_ZERO ) ;

    points = Data_Tbl_2 ;
    count = sizeof Data_Tbl_2 / ( sizeof ( VGfloat )) / 2u ;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends a polygon segment to the path */
    vguPolygon( path, points, count, VG_TRUE ) ;
    
	#if 0					//This has transparent effect
    RadGradient[ 0 ] =160 ;
    RadGradient[ 1 ] =160;
    RadGradient[ 2 ] = RadGradient[ 0 ] ;
    RadGradient[ 3 ] = RadGradient[ 1 ] ;
    RadGradient[ 4 ] = 160 ;
 
    /* Set the gradient parameters to the paint object */
    vgSetParameterfv( fill, VG_PAINT_RADIAL_GRADIENT, sizeof RadGradient / ( sizeof ( VGfloat )), RadGradient ) ;
    /* Set the color ramp parameters */
    vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, 15, RampStopTbl ) ;
 
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
//    vgSetColor(fill, (VGuint)0xF15A24FF);
//    	vgSetColor(fill, (VGuint)0x000000ff);
 	#endif
 	
    vgSetPaint(fill, VG_FILL_PATH);
    /* The path is filled */
    vgDrawPath( path, VG_FILL_PATH ) ;
    
    /* Destroy the path */
    vgDestroyPath( path ) ;
    vgDestroyPaint(fill);
    
#endif

    return 1 ;
}

/**************************************************************************//**
 * Function Name : TrVg_Drawing
 * @brief       
 * @param       [in]void
 * @retval      int     : Return '0', if the error occurred.
 *****************************************************************************/
int TrVg_DrawingPolyconFill_LinearGradient( void )
{
    VGPath path ;
    const VGfloat *points ;
    VGint count ;       /* The number of points */
    VGPaint fill;
 
		fill = vgCreatePaint();
//		vgSetParameterfv( fill, VG_PAINT_COLOR, 4, 0x00FF00ff);

		vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_LINEAR_GRADIENT );
 		vgSetParameterfv( fill, VG_PAINT_LINEAR_GRADIENT, 4, Polycon_linearGradient );										
 		vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, 20, Polycon_RampStopTab );
 
    points = Polycon_DataTab;
    count = Polycon_DrawPointNum ;
    /* Create a new path */
 
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    
    /* Appends a polygon segment to the path */
 
    vguPolygon( path, points, count, VG_TRUE ) ;
 
    vgSetPaint(fill, VG_FILL_PATH);
    /* The path is filled */
    vgDrawPath( path, VG_FILL_PATH ) ;
    
    /* Destroy the path */
    vgDestroyPath( path ) ;
    vgDestroyPaint(fill);
 
    return 1 ;
}

int TrVg_DrawingPolyconFillPara( VGuint color )
{
    VGPath path ;
    VGfloat *points ;
    VGint count ;       /* The number of points */
    VGPaint fill;

//		VGfloat linearGradient[4] = { 160.0f, 479.0f, 560.0f, 0.0f };
 
    VGfloat RadGradient[ 5 ] ;      /* Center (x, y), Focal (x, y), Radius r */
    
     vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
     vgLoadIdentity();
     vgTranslate(0.0f, 0.0f); 
     vgSeti( VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_BETTER ) ;
    
    /* Create a new paint object (for filling) */
    fill = vgCreatePaint();

    /* Set the paint type to VG_PAINT_TYPE_COLOR */
//     vgSetParameteri(fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);

    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
//    vgSetColor(fill, (VGuint)0xF15A24FF);
//  	vgSetColor(fill, (VGuint)0xFF00FFff);
 
#if 1    
//    fill = vgCreatePaint();
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
//    vgSetParameteri(fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
//		vgSetColor(fill, (VGuint)0xFF0000ff);  

	#if 0
		vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_LINEAR_GRADIENT );
	#else
		vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );		
		  	vgSetColor(fill, color);
	#endif
		
 		vgSetParameterfv( fill, VG_PAINT_LINEAR_GRADIENT, 4, Polycon_linearGradient );
    vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, 15 , Polycon_RampStopTab ) ;
 
    /* Polygonal Line (fill rule "non-zero") */
//    vgSeti( VG_FILL_RULE, VG_NON_ZERO ) ;

    points = Polycon_DataTab ;
    count = Polycon_DrawPointNum;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends a polygon segment to the path */
    vguPolygon( path, points, count, VG_TRUE ) ;
    
	#if 0					//This has transparent effect
    RadGradient[ 0 ] =160 ;
    RadGradient[ 1 ] =160;
    RadGradient[ 2 ] = RadGradient[ 0 ] ;
    RadGradient[ 3 ] = RadGradient[ 1 ] ;
    RadGradient[ 4 ] = 160 ;
 
    /* Set the gradient parameters to the paint object */
    vgSetParameterfv( fill, VG_PAINT_RADIAL_GRADIENT, sizeof RadGradient / ( sizeof ( VGfloat )), RadGradient ) ;
    /* Set the color ramp parameters */
    vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, 15, RampStopTbl ) ;
 
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
//    vgSetColor(fill, (VGuint)0xF15A24FF);
//    	vgSetColor(fill, (VGuint)0x000000ff);
 	#endif
 	
    vgSetPaint(fill, VG_FILL_PATH);
    /* The path is filled */
    vgDrawPath( path, VG_FILL_PATH ) ;
    
    /* Destroy the path */
    vgDestroyPath( path ) ;
    vgDestroyPaint(fill);
    
#endif

    return 1 ;
}