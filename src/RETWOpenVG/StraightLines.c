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
 * @file         StraightLines.c
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
int TrVg_DrawingStraightLine( void ) ;


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Horizontal Line */
static const VGubyte Path_Segments_Tbl1[] =
{
    VG_MOVE_TO_ABS, VG_HLINE_TO_ABS, VG_VLINE_TO_ABS, VG_HLINE_TO_ABS, VG_VLINE_TO_ABS 
} ;
static const VGfloat Path_Data_Tbl1[] =
{
    20.0f, 20.0f,   			/* Move to abs */
    799.0f-20.0f,         /* Horizontal Line to abs */
    479.0f-20.0f,					/* Vertical Line to abs */
    20.0f,         				/* Horizontal Line to abs */
    20.0f									/* Vertical Line to abs */    
} ;

/* Vertical Line */
static const VGubyte Path_Segments_Tbl2[] =
{
    VG_MOVE_TO_ABS, VG_VLINE_TO_REL, VG_MOVE_TO_REL, VG_VLINE_TO_ABS
} ;
static const VGfloat Path_Data_Tbl2[] =
{
    60.0f, 100.0f,  /* Move to abs */
    100.0f,         /* Vertical Line to rel */
    20.0f, -60.0f,  /* Move to rel */
    180.0f          /* Vertical Line to abs */
} ;

/* Line and Close path */
static const VGubyte Path_Segments_Tbl3[] =
{
    VG_MOVE_TO_ABS, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_ABS, VG_CLOSE_PATH
} ;
static const VGfloat Path_Data_Tbl3[] =
{
    120.0f, 60.0f,  /* Move to abs */
    495.0f, 0.0f,   /* Line to rel */
    -100.0f, 40.0f,   /* Line to rel */
    700.0f, 237.0f   /* Line to abs */
                    /* Close path */
} ;


/**************************************************************************//**
 * Function Name : TrVg_Drawing
 * @brief       
 * @param       [in]void
 * @retval      int     : Return '0', if the error occurred.
 *****************************************************************************/
int TrVg_DrawingStraightLine( void )
{
    VGPath path ;
    VGint numSegments ;
    const VGubyte *pathSegments ;
    const void *pathData ;

    vgSetf( VG_STROKE_LINE_WIDTH, 6.0f ) ;

#if 1
    /*** Straight Line (Horizontal Line) ***/
    numSegments = ( sizeof Path_Segments_Tbl1 ) / ( sizeof ( VGubyte )) ;
    pathSegments = Path_Segments_Tbl1 ;
    pathData = Path_Data_Tbl1 ;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, numSegments, pathSegments, pathData ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;

    /*** Straight Line (Vertical Line) ***/
    numSegments = ( sizeof Path_Segments_Tbl2 ) / ( sizeof ( VGubyte )) ;
    pathSegments = Path_Segments_Tbl2 ;
    pathData = Path_Data_Tbl2 ;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, numSegments, pathSegments, pathData ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;

#endif

    /*** Straight Line (Line and Close path) ***/
    numSegments = ( sizeof Path_Segments_Tbl3 ) / ( sizeof ( VGubyte )) ;
    pathSegments = Path_Segments_Tbl3 ;
    pathData = Path_Data_Tbl3 ;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, numSegments, pathSegments, pathData ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;


    return 1 ;
}
