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
 * @file         Stroke.c
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
 
int TrVg_DrawingStroke( void ) ;
void Stroke_CapStylePara(VGuint x,VGuint y, VGuint dx, VGuint dy, VGuint Lwidth, VGuint color);
void Stroke_JoinStylePara(VGuint x,VGuint y, VGuint dx, VGuint dy, VGuint Lwidth, VGuint color);
void Stroke_MiterLimitPara(VGuint x,VGuint y, VGuint dx, VGuint dy, VGuint Lwidth, VGuint color);

/******************************************************************************
Private global variables and functions
******************************************************************************/
static void Stroke_LineWidth( void ) ;
static void Stroke_CapStyle( void ) ;
static void Stroke_JoinStyle( void ) ;
static void Stroke_MiterLimit( void ) ;
static void Stroke_setDefault( void ) ;

/* Polygonal line */
static const VGubyte PL_Segments_Tbl[] =
{
    VG_MOVE_TO_ABS, VG_LINE_TO_REL, VG_LINE_TO_REL
} ;

/* Horizontal line */
static const VGubyte HL_Segments_Tbl[] =
{
    VG_MOVE_TO_ABS, VG_HLINE_TO_REL
} ;

static const VGubyte HL_Segments_TblPara[] =
{
    VG_MOVE_TO_ABS, VG_LINE_TO_ABS
} ;

static const VGubyte CAPHL_Segments_Tbl[] =
{
    VG_MOVE_TO_ABS, VG_LINE_TO_REL
} ;

/**************************************************************************//**
 * Function Name : TrVg_Drawing
 * @brief       
 * @param       [in]void
 * @retval      int     : Return '0', if the error occurred.
 *****************************************************************************/
int TrVg_DrawingStroke( void )
{
    Stroke_setDefault() ;
//    Stroke_LineWidth() ;

//    Stroke_setDefault() ;
      Stroke_CapStyle() ;

//    Stroke_setDefault() ;
//    Stroke_JoinStyle() ;

//    Stroke_setDefault() ;
//    Stroke_MiterLimit() ;

    return 1 ;
}

/**************************************************************************//**
 * Function Name : Stroke_LineWidth
 * @brief       
 * @param       [in]void
 * @retval      void
 *****************************************************************************/
static void Stroke_LineWidth( void )
{
    VGPath path,stroke;
    VGfloat lineWidth ;
    VGfloat Data_Tbl[ 3 ] ;

    Data_Tbl[ 0 ] = 20.0f ;     /* Move to abs, x */
    Data_Tbl[ 2 ] = 80.0f ;     /* Horizontal line to rel */

    /* Line (width 1.0f) */
    Data_Tbl[ 1 ] = 20.0f ;     /* Move to abs, y */
    /* Create a new path */
    
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof HL_Segments_Tbl ) / ( sizeof ( VGubyte )), HL_Segments_Tbl, Data_Tbl ) ;
    lineWidth = 1.0f ;
    vgSetf( VG_STROKE_LINE_WIDTH, lineWidth ) ;

//    vgSetColor( path, ( VGuint )0xFF00FFff ) ;			//BGRA  , set color for LINE.
    
#if 1    
     stroke = vgCreatePaint();
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
     vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( stroke, ( VGuint )0xFF00FFff ) ;			//BGRA  , set color for LINE.
  
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
#endif      
    
    
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;

    /* Line (width 5.0f) */
    Data_Tbl[ 1 ] = 40.0f ;     /* Move to abs, y */
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof HL_Segments_Tbl ) / ( sizeof ( VGubyte )), HL_Segments_Tbl, Data_Tbl ) ;
    lineWidth = 5.0f ;
    vgSetf( VG_STROKE_LINE_WIDTH, lineWidth ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;

    /* Line (width 10.0f) */
    Data_Tbl[ 1 ] = 60.0f ;     /* Move to abs, y */
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof HL_Segments_Tbl ) / ( sizeof ( VGubyte )), HL_Segments_Tbl, Data_Tbl ) ;
    lineWidth = 10.0f ;
    vgSetf( VG_STROKE_LINE_WIDTH, lineWidth ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
}

/**************************************************************************//**
 * Function Name : Stroke_CapStyle
 * @brief       
 * @param       [in]void
 * @retval      void
 *****************************************************************************/
static void Stroke_CapStyle( void )
{
    VGPath path,stroke ;
    VGfloat Data_Tbl[ 4 ] ;

    vgSetf( VG_STROKE_LINE_WIDTH, 12.0f ) ;

    Data_Tbl[ 0 ] = 100.0f ;     /* Move to abs, x */
    Data_Tbl[ 2 ] = 100.0f ;     /* Horizontal line to rel */

    /* Line (cap style "butt") */
    Data_Tbl[ 1 ] = 100.0f ;    /* Move to abs, y */
 
#if 1    
     stroke = vgCreatePaint();
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
     vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( stroke, ( VGuint )0xFF00FFff ) ;			//BGRA  , set color for LINE.
  
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
#endif       

    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof HL_Segments_Tbl ) / ( sizeof ( VGubyte )), HL_Segments_Tbl, Data_Tbl ) ;
    vgSeti( VG_STROKE_CAP_STYLE, VG_CAP_BUTT ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;

    /* Line (cap style "round") */
    Data_Tbl[ 1 ] = 300.0f ;    /* Move to abs, y */
    Data_Tbl[ 2 ] = 180.0f ;     /* Horizontal line to abs */
    Data_Tbl[ 3 ] = 0.0f ;     /* Horizontal line to abs */
    
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof CAPHL_Segments_Tbl ) / ( sizeof ( VGubyte )), CAPHL_Segments_Tbl, Data_Tbl ) ;
    vgSeti( VG_STROKE_CAP_STYLE, VG_CAP_ROUND ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
 
    /* Line (cap style "square") */
    Data_Tbl[ 1 ] = 320.0f ;    /* Move to abs, y */

    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof HL_Segments_Tbl ) / ( sizeof ( VGubyte )), HL_Segments_Tbl, Data_Tbl ) ;
    vgSeti( VG_STROKE_CAP_STYLE, VG_CAP_SQUARE ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
 
    vgDestroyPaint( stroke ) ;
}

void Stroke_CapStylePara(VGuint x,VGuint y, VGuint dx, VGuint dy, VGuint Lwidth, VGuint color)
{
    VGPath path,stroke ;
    VGfloat Data_Tbl[ 4 ];

//    Stroke_setDefault() ;
 
    Data_Tbl[ 0 ] = x;				/* Move to abs, x */
		Data_Tbl[ 1 ] = y;				/* Move to abs, y */
    Data_Tbl[ 2 ] = dx;     	/* Move to abs, dx */
    Data_Tbl[ 3 ] = dy;     	/* Move to abs, dy */
 
#if 1    							// Draw Stroke

    vgSetf( VG_STROKE_LINE_WIDTH, Lwidth ) ;
    
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE ) ;
    vgLoadIdentity() ;
    vgTranslate( 0.0f, 0.0f ) ;
    vgSeti( VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_BETTER ) ;
 
    
    stroke = vgCreatePaint();
    
    vgSetColor( stroke, ( VGuint )color ) ;			//BGRA  , set color for LINE.
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
     vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */

  
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
#endif       
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof HL_Segments_TblPara ) / ( sizeof ( VGubyte )), HL_Segments_TblPara, Data_Tbl ) ;
    vgSeti( VG_STROKE_CAP_STYLE, VG_CAP_ROUND ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
    /* Destroy the path */
    vgDestroyPaint( stroke ) ;
}



/**************************************************************************//**
 * Function Name : Stroke_JoinStyle
 * @brief       
 * @param       [in]void
 * @retval      void
 *****************************************************************************/
static void Stroke_JoinStyle( void )
{
    VGPath path ;
    VGfloat Data_Tbl[ 6 ] ;

    vgSetf( VG_STROKE_LINE_WIDTH, 16.0f ) ;

    Data_Tbl[ 2 ] = 40.0f ;     /* Line to rel, x */
    Data_Tbl[ 3 ] = 80.0f ;     /* Line to rel, y */
    Data_Tbl[ 4 ] = 20.0f ;     /* Line to rel, x */
    Data_Tbl[ 5 ] = -80.0f ;    /* Line to rel, y */

    /* Line (join style "miter") */
    Data_Tbl[ 0 ] = 240.0f ;    /* Move to abs, x */
    Data_Tbl[ 1 ] = 20.0f ;     /* Move to abs, y */
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof PL_Segments_Tbl ) / ( sizeof ( VGubyte )), PL_Segments_Tbl, Data_Tbl ) ;
    vgSeti( VG_STROKE_JOIN_STYLE, VG_JOIN_MITER ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;

    /* Line (join style "round") */
    Data_Tbl[ 0 ] = 320.0f ;    /* Move to abs, x */
    Data_Tbl[ 1 ] = 20.0f ;     /* Move to abs, y */
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof PL_Segments_Tbl ) / ( sizeof ( VGubyte )), PL_Segments_Tbl, Data_Tbl ) ;
    vgSeti( VG_STROKE_JOIN_STYLE, VG_JOIN_ROUND ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;

    /* Line (join style "bevel") */
    Data_Tbl[ 0 ] = 400.0f ;    /* Move to abs, x */
    Data_Tbl[ 1 ] = 20.0f ;     /* Move to abs, y */
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof PL_Segments_Tbl ) / ( sizeof ( VGubyte )), PL_Segments_Tbl, Data_Tbl ) ;
    vgSeti( VG_STROKE_JOIN_STYLE, VG_JOIN_BEVEL ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
}

void Stroke_JoinStylePara(VGuint x,VGuint y, VGuint dx, VGuint dy, VGuint Lwidth, VGuint color)
{
    VGPath path,stroke ;
    VGfloat Data_Tbl[ 4 ];

//    Stroke_setDefault() ;
 
    Data_Tbl[ 0 ] = x;				/* Move to abs, x */
		Data_Tbl[ 1 ] = y;				/* Move to abs, y */
    Data_Tbl[ 2 ] = dx;     	/* Move to abs, dx */
    Data_Tbl[ 3 ] = dy;     	/* Move to abs, dy */
 
#if 1    							// Draw Stroke

    vgSetf( VG_STROKE_LINE_WIDTH, Lwidth ) ;
    
     stroke = vgCreatePaint();
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
     vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( stroke, ( VGuint )color);				//0x800000ff ) ;			//BGRA  , set color for LINE.
  
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
#endif       
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof HL_Segments_TblPara ) / ( sizeof ( VGubyte )), HL_Segments_TblPara, Data_Tbl ) ;
    vgSeti( VG_STROKE_JOIN_STYLE, VG_JOIN_MITER ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
    /* Destroy the path */
    vgDestroyPaint( stroke ) ;
}

/**************************************************************************//**
 * Function Name : Stroke_JoinStyle
 * @brief       
 * @param       [in]void
 * @retval      void
 *****************************************************************************/
static void Stroke_MiterLimit( void )
{
    VGPath path ;
    VGfloat Data_Tbl[ 6 ] ;

    vgSetf( VG_STROKE_LINE_WIDTH, 16.0f ) ;
    vgSeti( VG_STROKE_JOIN_STYLE, VG_JOIN_MITER ) ;

    Data_Tbl[ 2 ] = 40.0f ;     /* Line to rel, x */
    Data_Tbl[ 3 ] = 80.0f ;     /* Line to rel, y */
    Data_Tbl[ 4 ] = 20.0f ;     /* Line to rel, x */
    Data_Tbl[ 5 ] = -80.0f ;    /* Line to rel, y */

    /* Line (miter limit, 1.0f) */
    Data_Tbl[ 0 ] = 240.0f ;    /* Move to abs, x */
    Data_Tbl[ 1 ] = 120.0f ;    /* Move to abs, y */
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof PL_Segments_Tbl ) / ( sizeof ( VGubyte )), PL_Segments_Tbl, Data_Tbl ) ;
    vgSetf( VG_STROKE_MITER_LIMIT, 1.0f ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;

    /* Line (miter limit, 2.0f) */
    Data_Tbl[ 0 ] = 320.0f ;    /* Move to abs, x */
    Data_Tbl[ 1 ] = 120.0f ;    /* Move to abs, y */
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof PL_Segments_Tbl ) / ( sizeof ( VGubyte )), PL_Segments_Tbl, Data_Tbl ) ;
    vgSetf( VG_STROKE_MITER_LIMIT, 2.0f ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;

    /* Line (miter limit, 4.0f) */
    Data_Tbl[ 0 ] = 400.0f ;    /* Move to abs, x */
    Data_Tbl[ 1 ] = 120.0f ;    /* Move to abs, y */
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof PL_Segments_Tbl ) / ( sizeof ( VGubyte )), PL_Segments_Tbl, Data_Tbl ) ;
    vgSetf( VG_STROKE_MITER_LIMIT, 4.0f ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
}

void Stroke_MiterLimitPara(VGuint x,VGuint y, VGuint dx, VGuint dy, VGuint Lwidth, VGuint color)
{
 
    VGPath path,stroke ;
    VGfloat Data_Tbl[ 4 ];

//    Stroke_setDefault() ;
 
    Data_Tbl[ 0 ] = x;				/* Move to abs, x */
		Data_Tbl[ 1 ] = y;				/* Move to abs, y */
    Data_Tbl[ 2 ] = dx;     	/* Move to abs, dx */
    Data_Tbl[ 3 ] = dy;     	/* Move to abs, dy */
 
#if 1    							// Draw Stroke

    vgSetf( VG_STROKE_LINE_WIDTH, Lwidth ) ;
    
    stroke = vgCreatePaint();
    
    vgSetColor( stroke, ( VGuint )0x0000FFff ) ;			//BGRA  , set color for LINE.
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
     vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */

  
    vgSetPaint( stroke, VG_STROKE_PATH ) ;
#endif       
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof HL_Segments_TblPara ) / ( sizeof ( VGubyte )), HL_Segments_TblPara, Data_Tbl ) ;
    vgSeti( VG_STROKE_JOIN_STYLE, VG_JOIN_MITER ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
    /* Destroy the path */
    vgDestroyPaint( stroke ) ;
    
    
    
}
/**************************************************************************//**
 * Function Name : Stroke_setDefault
 * @brief       
 * @param       [in]void
 * @retval      void
 *****************************************************************************/
static void Stroke_setDefault( void )
{
    vgSetf( VG_STROKE_LINE_WIDTH, 1.0f ) ;
    vgSeti( VG_STROKE_CAP_STYLE, VG_CAP_BUTT ) ;
    vgSeti( VG_STROKE_JOIN_STYLE, VG_JOIN_MITER ) ;
    vgSetf( VG_STROKE_MITER_LIMIT, 4.0f ) ;
}
