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
 * @file         DrawText.c
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
typedef enum
{
    CHAR_CODE_S = 0,
    CHAR_CODE_H,
    CHAR_CODE_SP,
    CHAR_CODE_ONE,
    CHAR_CODE_TWO,
    CHAR_CODE_THREE,

    CHAR_CODE_NUM
} CharCode ;


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
int TrVg_DrawingText( void );
 

/******************************************************************************
Private global variables and functions
******************************************************************************/
/*
Takes an SVG path string. The following code¡K

var parse = require('svg-path-parser');
var d='M3,7 5-6 L1,7 1e2-.4 m-10,10 l10,0  \
  V27 89 H23           v10 h10             \
  C33,43 38,47 43,47   c0,5 5,10 10,10     \
  S63,67 63,67         s-10,10 10,10       \
  Q50,50 73,57         q20,-5 0,-10        \
  T70,40               t0,-15              \
  A5,5 45 1,0 40,20    a5,5 20 0,1 -10-10  Z';
console.log(parse(d));
¡Kwill yield an array of commands that define the path, like so:

[
VG_MOVE_TO_ABS  { code:'M', command:'moveto', x:3, y:7 },
VG_LINE_TO_ABS  { code:'L', command:'lineto', x:5, y:-6 },
VG_LINE_TO_ABS  { code:'L', command:'lineto', x:1, y:7 },
VG_LINE_TO_ABS  { code:'L', command:'lineto', x:100, y:-0.4 },
VG_MOVE_TO_REL  { code:'m', command:'moveto', relative:true, x:-10, y:10 },
VG_LINE_TO_REL  { code:'l', command:'lineto', relative:true, x:10, y:0 },
VG_VLINE_TO_ABS  { code:'V', command:'vertical lineto', y:27 },
VG_VLINE_TO_ABS  { code:'V', command:'vertical lineto', y:89 },
VG_HLINE_TO_ABS  { code:'H', command:'horizontal lineto', x:23 },
VG_VLINE_TO_REL  { code:'v', command:'vertical lineto', relative:true, y:10 },
VG_HLINE_TO_REL  { code:'h', command:'horizontal lineto', relative:true, x:10 },
VG_CUBIC_TO_ABS  { code:'C', command:'curveto', x1:33, y1:43, x2:38, y2:47, x:43, y:47 },
VG_CUBIC_TO_REL  { code:'c', command:'curveto', relative:true, x1:0, y1:5, x2:5, y2:10, x:10, y:10 },
VG_SCUBIC_TO_ABS  { code:'S', command:'smooth curveto', x2:63, y2:67, x:63, y:67 },
VG_SCUBIC_TO_REL  { code:'s', command:'smooth curveto', relative:true, x2:-10, y2:10, x:10, y:10 },
VG_QUAD_TO_ABS  { code:'Q', command:'quadratic curveto', x1:50, y1:50, x:73, y:57 },
VG_QUAD_TO_REL  { code:'q', command:'quadratic curveto', relative:true, x1:20, y1:-5, x:0, y:-10 },
VG_SQUAD_TO_ABS  { code:'T', command:'smooth quadratic curveto', x:70, y:40 },
VG_SQUAD_TO_REL  { code:'t', command:'smooth quadratic curveto', relative:true, x:0, y:-15 },
  { code:'A', command:'elliptical arc', rx:5, ry:5, xAxisRotation:45, largeArc:true, sweep:false, x:40, y:20 },
  { code:'a', command:'elliptical arc', relative:true, rx:5, ry:5, xAxisRotation:20, largeArc:false, sweep:true, x:-10, y:-10 },

  VG_SCCWARC_TO_ABS                           = VG_SCCWARC_TO | VG_ABSOLUTE,
  VG_SCCWARC_TO_REL                           = VG_SCCWARC_TO | VG_RELATIVE,
  VG_SCWARC_TO_ABS                            = VG_SCWARC_TO  | VG_ABSOLUTE,
  VG_SCWARC_TO_REL                            = VG_SCWARC_TO  | VG_RELATIVE,
  VG_LCCWARC_TO_ABS                           = VG_LCCWARC_TO | VG_ABSOLUTE,
  VG_LCCWARC_TO_REL                           = VG_LCCWARC_TO | VG_RELATIVE,
  VG_LCWARC_TO_ABS                            = VG_LCWARC_TO  | VG_ABSOLUTE,
  VG_LCWARC_TO_REL                            = VG_LCWARC_TO  | VG_RELATIVE,
  
VG_CLOSE_PATH  { code:'Z', command:'closepath' }
] 
  0x5C07
  
  <glyph glyph-name="uni5C07" unicode="&#x5c07;
" 
d="M1823 1380q-238 -301 -448 -422.5t-515 -187.5l-92 152q351 77 537 200q-174 110 -299 164l-132 -80l-110 137q304 164 463 359l162 -66q-23 -30 -76 -92h510v-164zM1610 1380h-461q160 -72 297 -157q106 92 164 157zM725 -102h-172v743h-127q-6 -533 -147 -737l-152 96
q127 205 121 641h-103v172h408v236h-342v565h170v-397h172v471h172v-1790zM1112 1083q-182 -56 -260 -67l-49 127q168 34 297 71zM1921 559h-217v-483q1 -83 -62 -126.5t-350 -45.5l-55 170q85 -5 133 -5q110 0 132 12.5t22 41.5v436h-721v166h721v227h180v-227h217v-166z
M1270 219l-150 -102q-61 126 -231 313l137 105q172 -182 244 -316z"



M				Moveto									1823,	1380
q				quadratic curveto				-238	-301		-448	-442.5
t				smooth quadratic curveto', relative:true			-515	-187.51  
t				smooth quadratic curveto', relative:true			 -92  152
q				quadratic curveto				351 77 537 200
q				quadratic curveto				-174 110 -299 164
l				lineto 									-132 -80
l				lineto									-110 137
q				quadratic curveto				304 164 463 359
l				lineto									162 -66
q				quadratic curveto				-23 -30 -76 -92

h				510
v				-164
z
M				1610 1380
h				-461
q				160 -72 297 -157
q				106 92 164 157
z
M				725 -102
h				-172
v				743
h				-127
q				-6 -533 -147 -737
l				-152 96
q				127 205 121 641
h				-103
v				172
h				408
v				236
h				-342
v				565
h				170
v				-397
h				172
v				471
h				172
v				-1790
z
M				1112 1083
q				-182 -56 -260 -67
l				-49 127
q				168 34 297 71
z
M				1921 559
h				-217
v				-483
q				1 -83 -62 -126.5
t				-350 -45.5l-55 170
q				85 -5 133 -5
q				110 0 132 12.5
t				22 41.5
v				436
h				-721
v				166
h				721
v				227
h				180
v				-227
h				217
v				-166
z
M				1270 219l-150 -102
q				-61 126 -231 313
l				137 105
q				172 -182 244 -316
z

*/

/*  one
    <glyph glyph-name="uni4E00" unicode="&#x4e00;" 
d="M1921 745h-1782v181h1782v-181z" />				
		
		two
    <glyph glyph-name="uni4E8C" unicode="&#x4e8c;" 
d="M1780 1430h-1501v180h1501v-180zM1917 -31h-1774v181h1774v-181z" />

		three
    <glyph glyph-name="uni4E09" unicode="&#x4e09;" 
d="M1837 1448h-1599v188h1599v-188zM1741 727h-1417v186h1417v-186zM1923 -51h-1784v186h1784v-186z" />



*/







static const VGubyte Path_CmdTbl_Chun[] =
{
    VG_MOVE_TO_ABS,
    VG_QUAD_TO_ABS,
    VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_ABS,
    VG_LINE_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_ABS, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL,
    VG_LINE_TO_REL, VG_CUBIC_TO_ABS, VG_CLOSE_PATH
} ;

static const VGfloat Path_DataTbl_Chun[] =
{
    20.708000f,55.831001f,
    -5.380000f,2.533000f,-11.719000f,1.352000f,-13.929000f,-6.252000f,
    -1.680000f,-5.778000f,2.644000f,-8.900000f,5.808000f,-10.473000f,
    3.701000f,-1.842000f,7.070000f,-1.791000f,14.904000f,-7.762000f,
    5.010000f,-3.816000f,5.411000f,-13.109000f,3.315000f,-21.051001f,
    -1.245000f,-4.715000f,-4.848000f,-8.191000f,-8.094000f,-9.271000f,
    -8.195000f,-2.725000f,-15.300000f,0.361000f,-18.660000f,4.963000f,
    1.030000f,10.125000f,0.000000f,20.875999f,0.000000f,20.875999f,
    4.861000f,0.911000f,
    0.000000f,0.000000f,-0.638000f,-4.771000f,5.063000f,-11.599000f,
    4.003000f,-4.793000f,14.395000f,-2.596000f,16.680000f,3.656000f,
    3.800000f,10.398000f,-4.997000f,13.610000f,-11.866000f,16.063000f,
    8.655000f,32.081001f,2.133000f,37.492001f,1.619000f,43.666000f,
    -0.560000f,6.733000f,0.730000f,16.448000f,7.895000f,19.038000f,
    4.487000f,1.619000f,13.732000f,3.227000f,18.268999f,-5.182000f,
    3.465000f,-6.422000f,3.244000f,-11.830000f,3.244000f,-11.830000f,
    -5.381000f,-0.422000f,
    25.646000f,45.271000f,24.634001f,53.980999f,20.708000f,55.831001f
} ;
 

//	d="M1921 745		h-1782		v181		h1782		v-181		z" />			

static const VGubyte Path_CmdTbl_One[] =
{
    VG_MOVE_TO_ABS,
    VG_HLINE_TO_REL,
    VG_VLINE_TO_REL,
    VG_HLINE_TO_REL,
    VG_VLINE_TO_REL,    
		VG_CLOSE_PATH
};

static const VGfloat Path_DataTbl_One[] =
{
    1921, 745,
    -1782,
    181,
    1782,
  	-181
};

//	d="M1780 1430h-1501v180h1501v-180zM1917 -31h-1774v181h1774v-181z" />
static const VGubyte Path_CmdTbl_Two[] =
{
    VG_MOVE_TO_ABS,
    VG_HLINE_TO_REL,
    VG_VLINE_TO_REL,
    VG_HLINE_TO_REL,
    VG_VLINE_TO_REL,    
		VG_CLOSE_PATH,
    VG_MOVE_TO_ABS,
    VG_HLINE_TO_REL,
    VG_VLINE_TO_REL,
    VG_HLINE_TO_REL,
    VG_VLINE_TO_REL,    
		VG_CLOSE_PATH,		
};

#if 0
static const VGfloat Path_DataTbl_Two[] =
{
			M1780 1430
			h-1501
			v180
			h1501
			v-180
			z
			M1917 -31
			h-1774
			v181
			h1774
			v-181
			z
} ;


static const VGubyte Path_CmdTbl_Three[] =
{
    VG_MOVE_TO_ABS,
    VG_VLINE_TO_REL,
    VG_VLINE_TO_REL,
    VG_VLINE_TO_REL,
    VG_VLINE_TO_REL,    
		VG_CLOSE_PATH
};

static const VGfloat Path_DataTbl_Three[] =
{
    1921,			745,
    -1782,
    181,
    1782,
  	-181
} ;

#endif

/* Path data of character 'S' */
static const VGfloat Path_DataTbl_S[] =				//
{
    20.708000f,55.831001f,
    -5.380000f,2.533000f,-11.719000f,1.352000f,-13.929000f,-6.252000f,
    -1.680000f,-5.778000f,2.644000f,-8.900000f,5.808000f,-10.473000f,
    3.701000f,-1.842000f,7.070000f,-1.791000f,14.904000f,-7.762000f,
    5.010000f,-3.816000f,5.411000f,-13.109000f,3.315000f,-21.051001f,
    -1.245000f,-4.715000f,-4.848000f,-8.191000f,-8.094000f,-9.271000f,
    -8.195000f,-2.725000f,-15.300000f,0.361000f,-18.660000f,4.963000f,
    1.030000f,10.125000f,0.000000f,20.875999f,0.000000f,20.875999f,
    4.861000f,0.911000f,
    0.000000f,0.000000f,-0.638000f,-4.771000f,5.063000f,-11.599000f,
    4.003000f,-4.793000f,14.395000f,-2.596000f,16.680000f,3.656000f,
    3.800000f,10.398000f,-4.997000f,13.610000f,-11.866000f,16.063000f,
    8.655000f,32.081001f,2.133000f,37.492001f,1.619000f,43.666000f,
    -0.560000f,6.733000f,0.730000f,16.448000f,7.895000f,19.038000f,
    4.487000f,1.619000f,13.732000f,3.227000f,18.268999f,-5.182000f,
    3.465000f,-6.422000f,3.244000f,-11.830000f,3.244000f,-11.830000f,
    -5.381000f,-0.422000f,
    25.646000f,45.271000f,24.634001f,53.980999f,20.708000f,55.831001f
} ;
/* Path command of character 'S' */
static const VGubyte Path_CmdTbl_S[] =				//19bytes
{
    VG_MOVE_TO_ABS, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_ABS,
    VG_LINE_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_ABS, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL, VG_CUBIC_TO_REL,
    VG_LINE_TO_REL, VG_CUBIC_TO_ABS, VG_CLOSE_PATH
} ;
/* Path data of character 'H' */

#if 0
static const VGfloat Path_DataTbl_H[] =
{
    0.000000f, 0.000000f,
    5.442000f, 0.000000f,
    5.442000f, 29.646000f,
    26.392000f, 29.646000f,
    26.559000f, 0.000000f,
    32.000000f, 0.000000f,
    32.000000f, 64.000000f,
    26.559000f, 64.000000f,
    26.559000f, 37.173000f,
    5.112000f, 37.173000f,
    5.112000f, 64.000000f,
    0.000000f, 64.000000f
} ;
/* Path command of character 'H' */
static const VGubyte Path_CmdTbl_H[] =
{
    VG_MOVE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS,
    VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_CLOSE_PATH
} ;


#else

static const VGfloat Path_DataTbl_H[] =			//This ia "A" , by Generallai
{
    17.921875f, 43.203125f, 17.781250f, 43.203125f, 13.828125f, 19.796875f, 21.890625f, 19.796875f,
    23.187500f, 12.234375f, 12.531250f, 12.234375f, 10.437500f, 0.000000f, 0.359375f, 0.000000f,
    12.234375f, 52.562500f, 23.765625f, 52.562500f, 35.640625f, 0.000000f, 25.265625f, 0.000000f,
} ;
/* Path command of character 'H' */
static const VGubyte Path_CmdTbl_H[] =
{
    VG_MOVE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS,
    VG_CLOSE_PATH, VG_MOVE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS,
    VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS,
    VG_LINE_TO_ABS, VG_CLOSE_PATH, 
} ;


#endif

/* Sample text */
static VGuint SampelText[] =
{   /* "SH SS HH" */
    CHAR_CODE_S, CHAR_CODE_H, CHAR_CODE_SP, CHAR_CODE_S, CHAR_CODE_S, CHAR_CODE_SP, CHAR_CODE_H, CHAR_CODE_H
} ;

static VGuint SampleChnText[] =
{   
     CHAR_CODE_H
} ;

/**************************************************************************//**
 * Function Name : TrVg_Drawing
 * @brief       
 * @param       [in]void
 * @retval      int     : Return '0', if the error occurred.
 *****************************************************************************/
int TrVg_DrawingText( void )
{
   
    VGFont FontHdl ;
    VGPath path ;
    VGPaint fill ;
    VGfloat Origin[] = { 0.0f, 0.0f } ;
    VGfloat Escapement[] = { 32.0f, 0.0f } ;

    /* Creates a new font object */
    FontHdl = vgCreateFont( CHAR_CODE_NUM ) ;
    if( FontHdl == VG_INVALID_HANDLE ){
        return 0 ;
    }

    /*** Glyph of the character 'S' ***/
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof Path_CmdTbl_S ) / ( sizeof ( VGubyte )), Path_CmdTbl_S, Path_DataTbl_S ) ;
    /* Creates a glyph and assigns the given path to a glyph that corresponds to the specified index */
    vgSetGlyphToPath( FontHdl, ( VGuint )CHAR_CODE_S, path, VG_FALSE, Origin, Escapement ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;

    /*** Glyph of the character 'H' ***/
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends data to the path */
    vgAppendPathData( path, ( sizeof Path_CmdTbl_H ) / ( sizeof ( VGubyte )), Path_CmdTbl_H, Path_DataTbl_H ) ;
    /* Creates a glyph and assigns the given path to a glyph that corresponds to the specified index */
    vgSetGlyphToPath( FontHdl, ( VGuint )CHAR_CODE_H, path, VG_FALSE, Origin, Escapement ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;

    /*** Glyph of the character ' '(space) ***/
    /* Creates a glyph and assigns the given path to a glyph that corresponds to the specified index
       For no visual representation like space, VG_INVALID_HANDLE can be used */
    vgSetGlyphToPath( FontHdl, ( VGuint )CHAR_CODE_SP, VG_INVALID_HANDLE, VG_FALSE, Origin, Escapement ) ;

    /*** Text color ***/
    /* Create a new paint object */
    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( fill, ( VGuint )0x0000FFFF ) ;          /* Blue */
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH ) ;

    /* Set the matrix mode to allow manipulation of the glyph-user-to-surface transformation */
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE ) ;
    /* Set the current matrix to the identity matrix  */
 
    vgLoadIdentity() ;
    vgTranslate( 80.0f, 160.0f ) ;
    vgScale( 3.5f, 3.5f ) ;
    vgSetColor( fill, ( VGuint )0xFF0000FF ) ;          /* Red */
    /* Set the glyph origin */
    vgSetfv( VG_GLYPH_ORIGIN, 2, Origin ) ;
    /* Renders a sequence of glyphs specified by the array of the indices */
    vgDrawGlyphs( FontHdl, ( VGint )( sizeof SampelText / sizeof ( VGuint )), SampelText, NULL, NULL, VG_FILL_PATH, VG_FALSE ) ;

    /* Destroy the paint objects */
    vgDestroyPaint( fill ) ;

    /* Destroys the font object */
    vgDestroyFont( FontHdl ) ;

    return 1 ;
}


