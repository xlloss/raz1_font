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
 * @file         Ellipses.c
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
int TrVg_DrawingCircle( void ) ;
int TrVg_DrawingCirclePara( VGuint x, VGuint y, VGuint xwidth, VGuint yheight, VGuint color );
int TrVg_DrawingCircleRadiant( VGuint x, VGuint y, VGuint xwidth, VGuint yheight, VGuint color );
int TrVg_DrawingCircleMode( VGuint DrawMode, VGuint x, VGuint y, VGuint xwidth, VGuint yheight, VGuint Width, VGuint color );
int TrVg_DrawingCircleBatch(VGuint x, VGuint y, VGuint xwidth, VGuint yheight, VGuint color );
int TrVg_DrawingCircleFillPureColor(  VGuint x, VGuint y, VGuint xwidth, VGuint yheight, VGuint Width, VGuint color );

extern VGfloat linearGradient[];
//												percent,B,G,R,alpha								percent,B,G,R,alpha								percent,B,G,R,alpha
extern VGfloat rampStops[];

extern int Polycon_DrawPointNum;
extern VGfloat Polycon_DataTab[];
extern VGfloat Polycon_linearGradient[]; 
extern VGfloat  Polycon_RampStopTab[];
extern VGfloat Polycon_RadGradient[] ;      /* Center (x, y), Focal (x, y), Radius r */
     
/******************************************************************************
Private global variables and functions
******************************************************************************/

/**************************************************************************//**
 * Function Name : TrVg_Drawing
 * @brief       
 * @param       [in]void
 * @retval      int     : Return '0', if the error occurred.
 *****************************************************************************/
int TrVg_DrawingCircle( void )
{
    VGPath path,stroke ;
    VGfloat cx, cy, width, height ;

    /*** Ellipse ***/
    cx = 400.0f ;
    cy = 240.0f ;
    width = 470.0f ;
    height = 470.0f ;
    
    vgSetf( VG_STROKE_LINE_WIDTH, 13.0f ) ;
#if 1    								//set color
     stroke = vgCreatePaint();
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
     vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
  
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
    vgSetColor( stroke, ( VGuint )0xFFFF00ff ) ;			//BGRA  , set color for LINE.
#endif     
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    vgSetColor( path, ( VGuint )0xFFFFFFff ) ;			//sBGRA

    /* Appends an axis-aligned ellipse to the path */
    vguEllipse( path, cx, cy, width, height ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
    vgDestroyPaint( stroke ) ;
    
#if 0
    /*** Circle (special case of ellipse) ***/
    cx = 300.0f ;
    cy = 320.0f ;
    width = 240.0f ;
    height = width ;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends an axis-aligned ellipse to the path */
    vguEllipse( path, cx, cy, width, height ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
#endif

    return 1 ;
}

int TrVg_DrawingCirclePara( VGuint x, VGuint y, VGuint xwidth, VGuint yheight, VGuint color )
{
    VGPath path,stroke;
    VGPaint fill;
    VGfloat cx, cy, width, height ;

    /*** Ellipse ***/
    cx = x ;
    cy = y ;
    width = xwidth;
    height = yheight ;
    
#if 1    								// stroke
    vgSetf( VG_STROKE_LINE_WIDTH, 2.0f ) ;
     stroke = vgCreatePaint();
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
     vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
    vgSetColor( stroke, ( VGuint )color ) ;			//BGRA  , set color for LINE.
#endif     

#if 1										//fill color										

		fill = vgCreatePaint();
		vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
//		vgSetParameterfv( fill, VG_PAINT_COLOR, 4, 0x00FF00ff);
		vgSetPaint(fill, VG_FILL_PATH );
    vgSetColor(fill, (VGuint)color);  

#endif

    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
//    vgSetColor( path, ( VGuint )0xFFFFFFff ) ;			//sBGRA

    /* Appends an axis-aligned ellipse to the path */
    vguEllipse( path, cx, cy, width, height ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH  |  VG_FILL_PATH ) ;
    /* Destroy the path */
    
		vgDestroyPaint( fill ) ;
    vgDestroyPath( path ) ;
    vgDestroyPaint( stroke ) ;
    return 1 ;
}


 
int TrVg_DrawingCircleRadiant( VGuint x, VGuint y, VGuint xwidth, VGuint yheight, VGuint color )
{
    VGPath path,stroke;
    VGPaint fill;
    VGfloat cx, cy, width, height ;
    int i;
 
					linearGradient[0] = 400-2;
					linearGradient[1] = 240-2;
					linearGradient[2] = 400+2;
					linearGradient[3] = 240+2;					
					VGfloat SrampStops[15] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		0.3f, 0.5f, 0.5f, 0.5f, 1.0f, 		1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		//												percent,B,G,R,alpha								percent,B,G,R,alpha								percent,B,G,R,alpha
					for(i=0;i<15;i++)
					rampStops[i]=SrampStops[i];
 
    /*** Ellipse ***/
    cx = x ;
    cy = y ;
    width = xwidth;
    height = yheight ;
    
    vgSetf( VG_STROKE_LINE_WIDTH, 1.0f ) ;
#if 1		//set stroke color
     stroke = vgCreatePaint();
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
     vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
    vgSetColor( stroke, ( VGuint )color ) ;			//BGRA  , set color for LINE.
#endif     

#if 1		//fill color

		fill = vgCreatePaint();
 
//		vgSetParameterfv( fill, VG_PAINT_COLOR, 4, 0x00FF00ff);

		vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_LINEAR_GRADIENT );
		vgSetParameterfv( fill, VG_PAINT_LINEAR_GRADIENT, 4, linearGradient );										

		vgSetParameteri( fill, VG_PAINT_COLOR_RAMP_SPREAD_MODE, VG_COLOR_RAMP_SPREAD_PAD );
		vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, 15, rampStops );
  
  
  
		vgSetPaint(fill, VG_FILL_PATH );
    vgSetColor(fill, (VGuint)color);  

#endif

    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
//    vgSetColor( path, ( VGuint )0xFFFFFFff ) ;			//sBGRA

    /* Appends an axis-aligned ellipse to the path */
    vguEllipse( path, cx, cy, width, height ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH  |  VG_FILL_PATH ) ;
    /* Destroy the path */
    
    vgDestroyPaint( fill ) ;
    vgDestroyPath( path ) ;
    vgDestroyPaint( stroke ) ;
 
    return 1 ;
}
 
int TrVg_DrawingCircleLiearRadiantPara( VGuint x, VGuint y, VGuint xwidth, VGuint yheight, VGuint color, VGuint rampStopNum)
{ 
    VGPath path,stroke;
    VGPaint fill;
    VGfloat cx, cy, width, height ;
    int i;
 
    /*** Ellipse ***/
    cx = x ;
    cy = y ;
    width = xwidth;
    height = yheight ;

#if 1    
    vgSetf( VG_STROKE_LINE_WIDTH, 4.0f ) ;
	  //set stroke color
     stroke = vgCreatePaint();
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
     vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
    vgSetColor( stroke, ( VGuint )color ) ;			//BGRA  , set color for LINE.
#endif     

#if 1		//fill color

		fill = vgCreatePaint();
 
//		vgSetParameterfv( fill, VG_PAINT_COLOR, 4, 0x00FF00ff);

		vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_LINEAR_GRADIENT );
		vgSetParameterfv( fill, VG_PAINT_LINEAR_GRADIENT, 4, Polycon_linearGradient );										

		vgSetParameteri( fill, VG_PAINT_COLOR_RAMP_SPREAD_MODE, VG_COLOR_RAMP_SPREAD_PAD );
		vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, 15, Polycon_RampStopTab );

 
		vgSetPaint(fill, VG_FILL_PATH );
    vgSetColor(fill, (VGuint)color);  

	
#endif

    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
//    vgSetColor( path, ( VGuint )0xFFFFFFff ) ;			//sBGRA

    /* Appends an axis-aligned ellipse to the path */
    vguEllipse( path, cx, cy, width, height ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH  |  VG_FILL_PATH ) ;
    /* Destroy the path */
    
    vgDestroyPaint( fill ) ;
    vgDestroyPath( path ) ;
#if 1
    vgDestroyPaint( stroke ) ;
#endif
    return 1 ;
}

int TrVg_DrawingCircleRadialPara( VGuint x, VGuint y, VGuint xwidth, VGuint yheight, VGuint color, VGuint rampStopNum)
{ 
    VGPath path,stroke;
    VGPaint fill;
    VGfloat cx, cy, width, height ;
    int i;

     
    /*** Ellipse ***/
    cx = x ;
    cy = y ;
    width = xwidth;
    height = yheight ;

    vgSetf( VG_STROKE_LINE_WIDTH, 1.0f ) ;
#if 0 
 
	  //set stroke color
     stroke = vgCreatePaint();
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
     vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_RADIAL_GRADIENT ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
    vgSetColor( stroke, ( VGuint )color ) ;			//BGRA  , set color for LINE.
#endif     

#if 1		//fill color

		fill = vgCreatePaint();
 
 		vgSetParameterfv( fill, VG_PAINT_COLOR, 4, 0x00FF00ff);
 
		vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_RADIAL_GRADIENT );
   	vgSetParameterfv( fill, VG_PAINT_TYPE_RADIAL_GRADIENT, 4, Polycon_linearGradient );										

		vgSetParameteri( fill, VG_PAINT_COLOR_RAMP_SPREAD_MODE, VG_COLOR_RAMP_SPREAD_PAD );
 
    vgSetParameterfv( fill, VG_PAINT_RADIAL_GRADIENT, 5, Polycon_RadGradient ) ;
		vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, rampStopNum, Polycon_RampStopTab );
 		
		vgSetPaint(fill, VG_FILL_PATH );
    vgSetColor(fill, (VGuint)color);  

#endif

    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
//    vgSetColor( path, ( VGuint )0xFFFFFFff ) ;			//sBGRA

    /* Appends an axis-aligned ellipse to the path */
    vguEllipse( path, cx, cy, width, height ) ;
    /* The path is stroked */
//    vgDrawPath( path, VG_STROKE_PATH  |  VG_FILL_PATH ) ;
		vgDrawPath( path,  VG_FILL_PATH ) ;
    /* Destroy the path */
    
    vgDestroyPaint( fill ) ;
    vgDestroyPath( path ) ;
#if 0
    vgDestroyPaint( stroke ) ;
#endif
    return 1 ;
}





int TrVg_DrawingCircleBatch(VGuint x, VGuint y, VGuint xwidth, VGuint yheight, VGuint color )
{
					int i;
					
					linearGradient[0] = x-164;
					linearGradient[1] = y+164;
					linearGradient[2] = x+164;
					linearGradient[3] = y-164;					
					VGfloat SrampStops[15] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		0.3f, 0.5f, 0.5f, 0.5f, 1.0f, 		1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
					for(i=0;i<15;i++)
					rampStops[i]=SrampStops[i];
 
					TrVg_DrawingCircleMode(1, x,y,460,460,8,0xFF0030ff);	

					linearGradient[0] = x-164;
					linearGradient[1] = y+164;
					linearGradient[2] = x+164;
					linearGradient[3] = y-164 ;					
					VGfloat S1rampStops[15] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 		0.8f, 0.5f, 0.5f, 0.5f, 1.0f, 		1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
					for(i=0;i<15;i++)
					rampStops[i]=S1rampStops[i];
 
					TrVg_DrawingCircleMode(1, x,y,444,444,8,0xFF0030ff);	

					linearGradient[0] = x-164;
					linearGradient[1] = y+164;
					linearGradient[2] = x+164;
					linearGradient[3] = y-164 ;					
					VGfloat S2rampStops[15] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		0.5f, 0.8f, 0.8f, 0.8f, 1.0f, 		1.0f, 0.5f, 0.5f, 0.5f, 1.0f };
					for(i=0;i<15;i++)
					rampStops[i]=S2rampStops[i];
 
					TrVg_DrawingCircleMode(3, x,y,428,428,8,0xFF0030ff);	
					
					
 								
}
 
		//DrawMode			Description
		//	0						Empty Circle
		//	1						Empty Circle with radiant
		//	2						Circle with fill pure color
		//	3						Circle with fill radiant

int TrVg_DrawingCircleMode( VGuint DrawMode, VGuint x, VGuint y, VGuint xwidth, VGuint yheight, VGuint Width, VGuint color )
{
    VGPath path,stroke;
    VGPaint fill;
    VGfloat cx, cy, width, height ;

    /*** Ellipse ***/
    cx = x ;
    cy = y ;
    width = xwidth;
    height = yheight ;
    
		vgSetf( VG_STROKE_LINE_WIDTH, Width ) ;
#if 1		//set stroke color
     stroke = vgCreatePaint();
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
     vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
    vgSetColor( stroke, ( VGuint )color ) ;			//BGRA  , set color for LINE.
#endif 

if(  DrawMode == 1 | DrawMode == 2 | DrawMode == 3 )
		fill = vgCreatePaint();

if(DrawMode == 1 | DrawMode == 2 |DrawMode == 3 )
{
//	vgSetParameterfv( fill, VG_PAINT_COLOR, 4, 0x00FF00ff);

		vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_LINEAR_GRADIENT );
		vgSetParameterfv( stroke, VG_PAINT_LINEAR_GRADIENT, 4, linearGradient );

		vgSetParameteri( stroke, VG_PAINT_COLOR_RAMP_SPREAD_MODE, VG_COLOR_RAMP_SPREAD_PAD );
		vgSetParameterfv( stroke, VG_PAINT_COLOR_RAMP_STOPS, 15, rampStops );
  
		vgSetPaint(stroke, VG_FILL_PATH );
}

if(DrawMode == 2 | DrawMode == 3 )
{
//		vgSetParameterfv( fill, VG_PAINT_COLOR, 4, 0x00FF00ff);

		vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_LINEAR_GRADIENT );
		vgSetParameterfv( fill, VG_PAINT_LINEAR_GRADIENT, 4, linearGradient );

		vgSetParameteri( fill, VG_PAINT_COLOR_RAMP_SPREAD_MODE, VG_COLOR_RAMP_SPREAD_PAD );
		vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, 15, rampStops );
  
		vgSetPaint(fill, VG_FILL_PATH );
    vgSetColor(fill, (VGuint)color);  
}

    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
//    vgSetColor( path, ( VGuint )0xFFFFFFff ) ;			//sBGRA

    /* Appends an axis-aligned ellipse to the path */
    vguEllipse( path, cx, cy, width, height ) ;
    /* The path is stroked */
if( DrawMode == 2 | DrawMode == 3 )
    vgDrawPath( path, VG_STROKE_PATH  |  VG_FILL_PATH );
else
		vgDrawPath( path, VG_STROKE_PATH  );
    /* Destroy the path */

    vgDestroyPath( path ) ;
    vgDestroyPaint( stroke );
if( DrawMode == 1 | DrawMode == 2 | DrawMode == 3 )
    vgDestroyPaint( fill ) ;
 
    return 1 ;
}

int TrVg_DrawingCircleFillPureColor(VGuint x, VGuint y, VGuint xwidth, VGuint yheight, VGuint Width, VGuint color )
{
    VGPath path,stroke;
    VGPaint fill;
    VGfloat cx, cy, width, height ;

    /*** Ellipse ***/
    cx = x ;
    cy = y ;
    width = xwidth;
    height = yheight ;
 
		fill = vgCreatePaint();
    vgSetColor( fill, ( VGuint )color ) ; 
    
		vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
		vgSetParameterfv( fill, VG_PAINT_LINEAR_GRADIENT, 4, linearGradient );
		vgSetParameteri( fill, VG_PAINT_COLOR_RAMP_SPREAD_MODE, VG_COLOR_RAMP_SPREAD_PAD );
		vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, 15, rampStops );
 
 		vgSetPaint(fill, VG_FILL_PATH );
 
#if 1    
    /* Create a new path */
		 vgSetf( VG_STROKE_LINE_WIDTH, Width ) ;
//set stroke color
     stroke = vgCreatePaint();
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
     vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
    vgSetColor( stroke, ( VGuint )0xFF0000ff ) ;			//BGRA  , set color for LINE.
    
    
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
     
    vguEllipse( path, cx, cy, width-10, height-10 ) ;
 
    /* The path is stroked */
 
//		 vgDrawPath( path, VG_STROKE_PATH  );

     vgDrawPath( path,  VG_STROKE_PATH|VG_FILL_PATH );
     
    /* Destroy the path */
#endif   
    vgDestroyPath( path ) ; 
    vgDestroyPath( stroke ) ;        
    vgDestroyPaint( fill ) ;

    return 1 ;
}

int TrVg_DrawingCircleNoFill( VGuint x, VGuint y, VGuint xwidth, VGuint yheight, VGuint BorderWidth, VGuint color )
{
    VGPath path,stroke;
    VGPaint fill;
    VGfloat cx, cy, width, height ;

    /*** Ellipse ***/
    cx = x ;
    cy = y ;
    width = xwidth;
    height = yheight ;
    
		 vgSetf( VG_STROKE_LINE_WIDTH, BorderWidth ) ;
//set stroke color
     stroke = vgCreatePaint();
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
     vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
    vgSetColor( stroke, ( VGuint )color ) ;			//BGRA  , set color for LINE.
 
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 30.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
 

    /* Appends an axis-aligned ellipse to the path */
    vguEllipse( path, cx, cy, width, height ) ;
    /* The path is stroked */
 
		vgDrawPath( path, VG_STROKE_PATH  );
    /* Destroy the path */

    vgDestroyPath( path ) ;
    vgDestroyPaint( stroke );
    
    return 1 ;
}

