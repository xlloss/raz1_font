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
 * @file         EllipticalArcs.c
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
#include    <string.h>
#include    <math.h>
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
int TrVg_DrawingEllipticalArcs( void ) ;
int TrVg_DrawingEllipticalArcsPara( void);

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Elliptical Arc (small-arc, counter-clockwise) */
static const VGubyte Path_Segments_Tbl1[] =
{
    VG_MOVE_TO_ABS, VG_SCCWARC_TO_REL
} ;
static const VGfloat Path_Data_Tbl1[] =
{
    40.0f, 60.0f,       /* Move to abs */
    64.0f, 43.0f, 0.0f, 70.0f, 60.0f    /* Elliptical arc to rel */
} ;

/* Elliptical Arc (small-arc, clockwise) */
static const VGubyte Path_Segments_Tbl2[] =
{
    VG_MOVE_TO_ABS, VG_SCWARC_TO_ABS
} ;
static const VGfloat Path_Data_Tbl2[] =
{
    96.0f, 152.0f,      /* Move to abs */
    72.0f, 36.0f, 30.0f, 60.0f, 200.0f  /* Elliptical arc to abs */
} ;

/* Elliptical Arc (large-arc, counter-clockwise) */
static const VGubyte Path_Segments_Tbl3[] =
{
    VG_MOVE_TO_ABS, VG_LCCWARC_TO_ABS
} ;
static const VGfloat Path_Data_Tbl3[] =
{
    120.0f, 240.0f,      /* Move to abs */
    10.0f, 30.0f, 0.0f, 680.0f, 240.0f     /* Elliptical arc to abs */						
} ;

/* Elliptical Arc (large-arc, clockwise) */
static const VGubyte Path_Segments_Tbl4[] =
{
    VG_MOVE_TO_ABS, VG_LCWARC_TO_REL
} ;
static const VGfloat Path_Data_Tbl4[] =
{
    342.0f, 182.0f,         /* Move to abs */
    32.0f, 48.0f, -15.0f, 38.0f, -12.0f     /* Elliptical arc to rel */
} ;

 


/**************************************************************************//**
 * Function Name : TrVg_Drawing
 * @brief       
 * @param       [in]void
 * @retval      int     : Return '0', if the error occurred.
 *****************************************************************************/
int TrVg_DrawingEllipticalArcs( void )
{
    VGPath path ;
    VGint numSegments ;
    const VGubyte *pathSegments ;
    const void *pathData ;
#if 0
    /*** Elliptical Arc (small-arc, counter-clockwise) ***/
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


    /*** Elliptical Arc (small-arc, clockwise) ***/
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
    

    /*** Elliptical Arc (large-arc, counter-clockwise) ***/
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
    
    
#if 0
    /*** Elliptical Arc (large-arc, clockwise) ***/
    numSegments = ( sizeof Path_Segments_Tbl4 ) / ( sizeof ( VGubyte )) ;
    pathSegments = Path_Segments_Tbl4 ;
    pathData = Path_Data_Tbl4 ;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, numSegments, pathSegments, pathData ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
#endif

    return 1 ;
}

int TrVg_DrawingEllipticalArcsPara( ) 
{
    VGPath path ;
    VGint numSegments ;
    const VGubyte *pathSegments ;
    const void *pathData ;
 
    /*** Elliptical Arc (large-arc, counter-clockwise) ***/
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