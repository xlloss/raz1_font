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
#include		"RETWvg.h"

extern VGint CoordXSrc;
extern VGint CoordYSrc;
extern VGint CoordXDest;
extern VGint CoordYDest;

extern VGfloat 	G_CharScale;
extern VGfloat 	G_CharShearX;
extern VGfloat 	G_CharShearY;
extern VGfloat 	CharPosOffsetX;
extern VGfloat 	CharPosOffsetY;

//#include    "Font_24pt_PathData.h"
 
static const VGfloat Polyline[] =
{   /* List of points (x,y coordinates) that make up the polyline (open shape) */
    40.0f, 192.0f, 110.0f, 292.0f, 40.0f, 112.0f, 110.0f, 112.0f, 180.0f, 192.0f, 250.0f, 192.0f, 180.0f, 112.0f, 250.0f, 112.0f
} ;
static const VGfloat Polygon[] =
{   /* List of points (x,y coordinates) that make up the polygon (closed shape) */
    416.0f, 232.0f, 362.0f, 203.0f, 307.0f, 232.0f, 318.0f, 172.0f, 274.0f, 129.0f,
    334.0f, 120.0f, 361.0f, 65.0f, 389.0f, 120.0f, 449.0f, 129.0f, 405.0f, 172.0f
} ;

#if 0
 
VGuint FontScriptNumber1[] = {
																0,140,
																10,143,
																20,147,
																30,152,
																40,159,
																50,167,
															54,172,
															54,0,
															87,0,
															87,227,
															65,227,
															60,218,
															54,210,
															50,205,
															45,200,
															40,196,
															30,188,
															20,181,
															10,175,
															0,170,
															0xAA55};
 
VGuint FontScriptNumber2[] = {														
 
																0,0,
																152,0,
																152,32,
																45,32,
																50,38,
																60,47,
																70,56,
																80,64,
																90,72,
																100,81,
																110,90,
																120,100,
																130,111,
																137,120,
																140,125,
																143,130,
																146,138,
																147,140,
																
																150,150,
																151,158,				//right up
																151,166,
																152,157,
																
																151,170,
																150,176,
																149,180,
																145,190,
																140,199,
																135,205,
																130,210,
																120,217,
																115,220,
																110,222,
																100,225,
																90,227,
																85,227,
																80,227,
																75,227,
																70,227,
																60,225,
																50,222,
																45,220,
																40,217,
																35,214,
																30,211,
																25,206,
																20,201,
																15,193,
																10,181,
																7,170,
																6,158,
																40,154,
																40,160,
																41,170,
																43,175,
																45,180,
																47,184,
																50,187,
																53,190,
																58,193,
																60,194,
																65,196,
																70,197,
																75,198,
																80,198,
																85,198,
																90,197,
																95,196,
																100,194,
																105,190,
																110,185,
																114,180,
																117,170,
																118,160,
																116,150,
																112,140,
																110,138,
																100,125,
																30,60,
																25,56,
																20,50,
																15,44,
																10,36,
																7,30,
																6,28,
																4,24,
																3,20,
																2,15,
																1,10,
																0,0,
															0xAA55};
 
VGuint FontScriptNumber3[] = {													
 																0,77,
 																1,60,
 																3,50,
 																5,45,
 																7,40,
 																10,34,
 																12,30,
 																16,25,
 																21,20,
 																30,12,
 																34,10,
 																40,7,
 																45,5,
 																50,3,
 																60,1,
 																70,0,
 																80,1,
 																90,2,
 																100,4,
 																110,8,
 																113,10,
 																120,14,
 																125,18,
 																130,22,
 																137,30,
 																140,35,
 																143,40,
 																145,44,
 																147,50,
 																149,60,
 																150,66,
 																151,72,
 																150,78,
 																148,88,
 																146,95,
 																140,107,
 																138,110,
 																134,114,
 																130,119,
 																128,120,
 																125,123,
 																120,125,
 																114,127,
 																119,130,
 																126,136,
 																130,141,
 																134,146,
 																136,150,
 																137,154,
 																138,160,
 																139,165,
 																139,175,
 																138,183,
 																136,190,
 																130,202,
 																123,210,
 																111,220,
 																100,226,
 																90,228,
 																85,230,
 																75,231,
 																65,231,
 																60,230,
 																50,228,
 																40,224,
 																33,220,
 																30,218,
 																25,214,
 																21,210,
 																17,206,
 																10,193,
 																6,183,
 																3,170,
 																36,163,
 																37,170,
 																40,180,
 																43,187,
 																45,190,
 																50,195,
 																58,200,
 																60,201,
 																67,202,
 																72,203,
 																80,202,
 																86,200,
 																90,198,
 																95,195,
 																100,189,
 																103,185,
 																104,180,
 																105,176,
 																105,167,
 																104,160,
 																100,153,
 																98,150,
 																94,146,
 																90,144,
 																81,140,
 																77,139,
 																70,138,
 																56,138,
 																53,108,
 																73,111,
 																79,111,
 																87,109,
 																94,107,
 																100,103,
 																105,99,
 																110,92,
 																113,85,
 																114,80,
 																115,72,
 																115,68,
 																114,60,
 																110,50,
 																107,46,
 																102,40,
 																90,32,
 																80,29,
 																73,28,
 															  70,28,
 															  61,30,
 															  55,33,
 															  50,36,
  														  45,40,
															  40,48,
    													  39,50,
  														  37,55,
  														  35,60,
  														  34,66,
 															  33,71,
																0,77,
															0xAA55};											
#endif

/******************************************************************************
Definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
 
/******************************************************************************
Private global variables and functions
******************************************************************************/
VGfloat linearGradient[4] = { 160.0f, 479.0f, 560.0f, 0.0f };
//												percent,B,G,R,alpha								percent,B,G,R,alpha								percent,B,G,R,alpha
VGfloat rampStops[12*3] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 		
														0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 		
														1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
														0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 		
														0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 		
														1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
														0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 		
														0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 		
														1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
														0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 		
														0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 		
														1.0f, 0.0f, 0.0f, 1.0f, 1.0f };
														
int Polycon_DrawPointNum;

VGfloat Polycon_DataTab[512] = {};

VGfloat Polycon_linearGradient[4] = { 0.0f, 320.0f, 320.0f, 0.0f };
VGfloat Polycon_RadGradient[5] ;      /* Center (x, y), Focal (x, y), Radius r */
		
VGfloat Polycon_RampStopTab[ 12 * 3 ] =
{   /* Offset, Red, Green, Blue, Alpha */
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   /* Red, 0xFF0000FF */
    0.5f, 0.5f, 0.5f, 0.5f, 1.0f,   /* Green, 0x00FF00FF */
    1.0f, 0.0f, 0.0f, 0.0f, 1.0f,    /* Blue, 0x0000FFFF */
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   /* Red, 0xFF0000FF */
    0.5f, 0.5f, 0.5f, 0.5f, 1.0f,   /* Green, 0x00FF00FF */
    1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   /* Blue, 0x0000FFFF */
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   /* Red, 0xFF0000FF */
    0.5f, 0.5f, 0.5f, 0.5f, 1.0f,   /* Green, 0x00FF00FF */
    1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   /* Blue, 0x0000FFFF */
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   /* Red, 0xFF0000FF */
    0.5f, 0.5f, 0.5f, 0.5f, 1.0f,   /* Green, 0x00FF00FF */
    1.0f, 0.0f, 0.0f, 0.0f, 1.0f    /* Blue, 0x0000FFFF */
};

 
const uint16_t Seg7DigiNumDrawTab[8][12] = {
													{9,11,10,12,10,20,9,21,8,20,8,12},					
													{9,1,10,2,10,10,9,11,8,10,8,2},
													{1,1,2,0,8,0,9,1,8,2,2,2},
													{1,1,2,2,2,10,1,11,0,10,0,2},
													{1,11,2,12,2,20,1,21,0,20,0,12},
													{1,21,2,20,8,20,9,21,8,22,2,22},
													{1,11,2,10,8,10,9,11,8,12,2,12},
													{13,1,14,2,14,3,13,4,12,3,12,2}
													};
 

const uint16_t Seg7DigiNumDrawTabBig[8][12] = {
													{110,199,	120,189,	120,121,	110,111,	100,121,	100,189 },					
													{110,109,	120,99,		120,31,		110,21,		100,31,		100,99 },		
													{21,20,		31,30,		99,30,		109,20,		99,10,		31,10},
													{20,109,	30,99,		30,31,		20,21,		10,31,		10,99 },
													{20,199,	30,189,		30,121,		20,111,		10,121,		10,189 },	
													{21,200,	31,210,		99,210,		109,200,	99,190,		31,190},
													{21,110,	31,120,		99,120,		109,110,	99,100,		31,100},
													{140,30,	150,24,		150,16,		140,10,		120,16,		120,24}
													};
 
/*									---6----
										|				|	
 										5       1
 										|				|
 										----7---
										|				|	
 										4       2
 										|				|
 										----3---	8			*/
			 										
const uint8_t Seg7DigiNumDspTab[11][8] = {
													{1,1,1,1,1,1,0,0},				//0
													{1,1,0,0,0,0,0,0},				//1
													{1,0,1,1,0,1,1,0},
													{1,1,1,0,0,1,1,0},
													{1,1,0,0,1,0,1,0},
													{0,1,1,0,1,1,1,0},				//5
													{0,1,1,1,1,1,1,0},
													{1,1,0,0,0,1,0,0},
													{1,1,1,1,1,1,1,0},
													{1,1,1,0,1,1,1,0},			
													{0,0,0,0,0,0,0,1},				//dot						
													};						
											
int RETW_GUI_Show7Seg( uint32_t Number, uint32_t x, uint32_t y, uint32_t ex, uint32_t ey, uint32_t color)
{
				uint32_t i,j,k;
 
				for(i=0;i<8;i++)
				{
							for(j=0;j<12;j++)
							{
							if(j%2==0)
							Polycon_DataTab[j]  = Seg7DigiNumDrawTabBig[i][j] *ex/150  +x; 
							else
							Polycon_DataTab[j]  = Seg7DigiNumDrawTabBig[i][j] *ey/220  +y; 							
							}
							Polycon_DrawPointNum=6;					
							
 					if( Seg7DigiNumDspTab[Number][i]  == 1)		
						TrVg_DrawingPolyconFillPara(color); 	
					else
						TrVg_DrawingPolyconFillPara(0x003000ff); 								
				}					
}


int RETW_GUI_PoliconFillScreen(uint32_t x, uint32_t y, uint32_t sizex, uint32_t sizey,uint32_t color)
{
						Polycon_DataTab[0]  = x;
						Polycon_DataTab[1]  = y; 							
						Polycon_DataTab[2]  = x+sizex;
						Polycon_DataTab[3]  = y; 		
						 
						Polycon_DataTab[4]  = x+sizex;
						Polycon_DataTab[5]  = y+sizey; 							
						Polycon_DataTab[6]  = x;
						Polycon_DataTab[7]  = y+sizey; 									
							
						Polycon_DrawPointNum=4;					
 
						TrVg_DrawingPolyconFillPara(color); 							
}
 
int RETW_GUI_DottingFillScreen(uint32_t x, uint32_t y, uint32_t sizex, uint32_t sizey,uint32_t color)
{
					uint32_t 	i,j,k;
					st_VGDrawingParameter	VGParameter;
					
 					VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 					
 					for(i=y; i<sizey; i+=10)
 					{
 					vgFinish();	
 					
 					if( (i%40) == 0)
 							for(j=x;j<sizex;j+=20)
 							{		
							VGParameter.PathDrawingData[0]= i;
							VGParameter.PathDrawingData[1]= j;
							VGParameter.PathDrawingData[2]= 12;
							VGParameter.PathDrawingData[3]= 12;		
							
							VGParameter.FillColor = 0x00200000;
							VGParameter.AlphaColor = 0x000000ff;				
 	        		
				 			GL_DrawingCircleFill( &VGParameter);	
				 			vgFinish();	
							}
				 else if( (i%20) == 0)
 							for(j=5;j<sizex;j+=20)
 							{		
							VGParameter.PathDrawingData[0]= i;
							VGParameter.PathDrawingData[1]= j;
							VGParameter.PathDrawingData[2]= 12;
							VGParameter.PathDrawingData[3]= 12;		
							
							VGParameter.FillColor = color;
							VGParameter.AlphaColor = 0x000000ff;				
 	        		
				 			GL_DrawingCircleFill( &VGParameter);	
				 			vgFinish();	
							}			
				 }
				 		
}





void RETW_GUI_DrawScriptNumber(int x, int y, int speed, int FONTsizeX, uint32_t color, uint32_t Scale)
{
	
#if 0 //Old method to fix each character location by FONTsizeX
		int Number;
 
		if(speed<10)
			R_GUI_OpenVGDrawCharacter(x-(FONTsizeX/2), y-12, speed+0x30, color, Scale);
		else if(speed<100)			
			{
			R_GUI_OpenVGDrawCharacter(x-(FONTsizeX),	y-12,   (speed/10)+0x30, color, Scale);
			R_GUI_OpenVGDrawCharacter( x,							y-12, (speed%10)+0x30, color, Scale);			
			}
		else
			{
			R_GUI_OpenVGDrawCharacter( x-(FONTsizeX)-(FONTsizeX/2),	y-12, (speed/100)+0x30, color, Scale);		
			Number= speed%100;
			R_GUI_OpenVGDrawCharacter( x-(FONTsizeX/2),			y-12, (Number/10)+0x30, color, Scale);				
			R_GUI_OpenVGDrawCharacter( x+(FONTsizeX/2),			y-12, (Number%10)+0x30, color, Scale);							
			} 
#else		//Draw charter string by VGgly

			char TempString[32];
			char CopyString[32];
			uint32_t i,j;
			uint32_t length=0;
			uint32_t DispNum;
			uint32_t LocX,LocY;
			
			length=0;
			DispNum = speed;
				
			if(DispNum==0)
			{
			TempString[0]=0x30;
			length=1;
			}
			else
			{
 						while(DispNum)
 							{ 					
 							CopyString[length] = DispNum%10;
 							DispNum = DispNum/10;
							length +=1;
							}
						for(i=0;i<length;i++)
						TempString[i] = CopyString[length-1-i]+0x30;
			}

				CharPosOffsetX = (G_CharScale*length*14/2)+G_CharShearX*14/2;
				CharPosOffsetY = G_CharScale*12/2;
				
 			LocX = (uint32_t)( x-CharPosOffsetX);
 			LocY = (uint32_t)( y-CharPosOffsetY ); 			
 
// 			printf("disp location = %d,%d\n",LocX,LocY);
			R_TXTR_DrawStringMain(&TempString[0], length, LocX,  LocY, color);
#endif
}

int GL_DrawingPolyconFill( st_VGDrawingParameter *st_DrawPathInfo )
{
    VGPath path ;
    VGPaint fill;
 
		fill = vgCreatePaint();
    vgSetPaint(fill, VG_FILL_PATH);
  
 			vgSetParameteri( fill, VG_PAINT_TYPE, st_DrawPathInfo->PaintMode );
 			
 			vgSeti( VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_BETTER ) ;
 
 		if(st_DrawPathInfo->PaintMode == VG_PAINT_TYPE_COLOR)
 	    vgSetColor( fill, st_DrawPathInfo->FillColor + st_DrawPathInfo->AlphaColor) ; 
 	  
 	  else if(st_DrawPathInfo->PaintMode == VG_PAINT_TYPE_LINEAR_GRADIENT)
 	  	{
  		vgSetParameterfv( fill, VG_PAINT_LINEAR_GRADIENT, 4, &st_DrawPathInfo->GradientStartEnd[0] );									
  		vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, st_DrawPathInfo->GradientPoints, &st_DrawPathInfo->GradientStep[0] );
  		}
  	else if(st_DrawPathInfo->PaintMode == VG_PAINT_TYPE_RADIAL_GRADIENT)
 	  	{
  		vgSetParameterfv( fill, VG_PAINT_RADIAL_GRADIENT, 5, &st_DrawPathInfo->GradientStartEnd[0] );										
  		vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, st_DrawPathInfo->GradientPoints, &st_DrawPathInfo->GradientStep[0] );
  		}
 
    /* Create a new path */
 
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    
    /* Appends a polygon segment to the path */
    
    vguPolygon( path, &st_DrawPathInfo->PathDrawingData[0], st_DrawPathInfo->PathPoints, VG_TRUE ) ;
 

    /* The path is filled */
    vgDrawPath( path, VG_FILL_PATH ) ;
    
    /* Destroy the path */
    vgDestroyPath( path ) ;
    vgDestroyPaint(fill);

    return 1 ;
}
int GL_DrawingCircleFill( st_VGDrawingParameter *st_DrawPathInfo )
{
    	VGPath path;
    	VGPaint fill;
    	
			fill = vgCreatePaint();

 			vgSetParameteri( fill, VG_PAINT_TYPE, st_DrawPathInfo->PaintMode );
 
 		if(st_DrawPathInfo->PaintMode == VG_PAINT_TYPE_COLOR)
 	    vgSetColor( fill, st_DrawPathInfo->FillColor + st_DrawPathInfo->AlphaColor) ; 
 	  
 	  else if(st_DrawPathInfo->PaintMode == VG_PAINT_TYPE_LINEAR_GRADIENT)
 	  	{
  		vgSetParameterfv( fill, VG_PAINT_LINEAR_GRADIENT, 4, &st_DrawPathInfo->GradientStartEnd[0] );										
  		vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, st_DrawPathInfo->GradientPoints, &st_DrawPathInfo->GradientStep[0] );
  		}
  	else if(st_DrawPathInfo->PaintMode == VG_PAINT_TYPE_RADIAL_GRADIENT)
 	  	{
			vgSetParameteri( fill, VG_PAINT_COLOR_RAMP_SPREAD_MODE, VG_COLOR_RAMP_SPREAD_PAD );
		
  		vgSetParameterfv( fill, VG_PAINT_RADIAL_GRADIENT, 5, &st_DrawPathInfo->GradientStartEnd[0] );										
  		vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, st_DrawPathInfo->GradientPoints, &st_DrawPathInfo->GradientStep[0] );
  		}

 		vgSetPaint(fill, VG_FILL_PATH );
 		
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
     
    vguEllipse( path, st_DrawPathInfo->PathDrawingData[0], st_DrawPathInfo->PathDrawingData[1], st_DrawPathInfo->PathDrawingData[2], st_DrawPathInfo->PathDrawingData[3] ) ;
 
    vgDrawPath( path,  VG_FILL_PATH );
 
    /* Destroy the path */
    vgDestroyPath( path );
    vgDestroyPaint(fill);
    
    return 1 ;
}

int GL_DrawingArc( )
{
    VGPath path ;

    VGfloat x, y, width, height ;
    VGfloat startAngle, angleExtent ;   /* Angles in degrees, proceeding counter-clockwise */
    VGUArcType arcType ;            /* To control the style of arcs */

    /*** Arc ***/
    x = 400.0f ;
    y = 240.0f ;
    width = 120.0f ;
    height = 120.0f ;
    startAngle = 60.0f ;
    angleExtent = 170.0f ;
    arcType = VGU_ARC_OPEN ;
   
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;

		vgSetf( VG_STROKE_LINE_WIDTH, 14 ) ;
 
    /* Appends an elliptical arc to the path */
    vguArc( path, x, y, width, height, startAngle, angleExtent, arcType ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
#if 0
    /*** Arc (Chord) ***/
    x = 240.0f ;
    y = 120.0f ;
    width = 80.0f ;
    height = 200.0f ;
    startAngle = 30.0f ;
    angleExtent = 120.0f ;
    arcType = VGU_ARC_CHORD ;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends an elliptical arc to the path */
    vguArc( path, x, y, width, height, startAngle, angleExtent, arcType ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;

    /*** Arc (Pie) ***/
    x = 360.0f ;
    y = 120.0f ;
    width = 100.0f ;
    height = 100.0f ;
    startAngle = 180.0f ;
    angleExtent = 90.0f ;
    arcType = VGU_ARC_PIE ;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends an elliptical arc to the path */
    vguArc( path, x, y, width, height, startAngle, angleExtent, arcType ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_FILL_PATH  | VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
#endif

    return 1 ;
}

int GL_DrawingArcPara(st_VGDrawArcParameter *ArcInfo )
{
    VGPath path ;
		VGPaint fill;
    VGUArcType arcType ;            /* To control the style of arcs */

			fill = vgCreatePaint();
 			
 			vgSetParameteri( fill, VG_PAINT_TYPE, ArcInfo->PaintMode );
 
 		if(ArcInfo->PaintMode == VG_PAINT_TYPE_COLOR)
 	    vgSetColor( fill, ArcInfo->ArcColor) ; 
 	  
 	  else if(ArcInfo->PaintMode == VG_PAINT_TYPE_LINEAR_GRADIENT)
 	  	{
  		vgSetParameterfv( fill, VG_PAINT_LINEAR_GRADIENT, 4, &ArcInfo->GradientStartEnd[0] );										
  		vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, ArcInfo->GradientPoints, &ArcInfo->GradientStep[0] );
  		}
  	else if(ArcInfo->PaintMode == VG_PAINT_TYPE_RADIAL_GRADIENT)
 	  	{
			vgSetParameteri( fill, VG_PAINT_COLOR_RAMP_SPREAD_MODE, VG_COLOR_RAMP_SPREAD_PAD );
		
  		vgSetParameterfv( fill, VG_PAINT_RADIAL_GRADIENT, 5, &ArcInfo->GradientStartEnd[0] );										
  		vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, ArcInfo->GradientPoints, &ArcInfo->GradientStep[0] );
  		}
 
    /*** Arc ***/
    arcType = VGU_ARC_OPEN ;
 
#if 1
     vgSetPaint(fill, VG_STROKE_PATH);
  #else  
 		if( (ArcInfo->StrokeIt ==1) && (ArcInfo->FillIt ==0 ))
    vgSetPaint(fill, VG_STROKE_PATH);
    else if( (ArcInfo->StrokeIt ==1) && (ArcInfo->FillIt ==1 ))
    vgSetPaint(fill, VG_STROKE_PATH | VG_FILL_PATH);
    else if( (ArcInfo->StrokeIt ==0) && (ArcInfo->FillIt ==1 ))
    vgSetPaint(fill, VG_FILL_PATH);    
#endif

//		vgSetColor(fill,ArcInfo->ArcColor);
 
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;

		vgSetf( VG_STROKE_LINE_WIDTH, ArcInfo->ArcWidth ) ;
 
    /* Appends an elliptical arc to the path */
    vguArc( path, ArcInfo->CenterX, ArcInfo->CenterY, ArcInfo->Width, ArcInfo->Height, ArcInfo->StartAngle, ArcInfo->AngleExtent, arcType ) ;
    /* The path is stroked */
#if 1
    vgDrawPath( path, VG_STROKE_PATH ) ;
 #else   
 		if( (ArcInfo->StrokeIt ==1) && (ArcInfo->FillIt ==0 ))
    vgDrawPath(fill, VG_STROKE_PATH);
    else if( (ArcInfo->StrokeIt ==1) && (ArcInfo->FillIt ==1 ))
    vgDrawPath(fill, VG_STROKE_PATH | VG_FILL_PATH);
    else if( (ArcInfo->StrokeIt ==0) && (ArcInfo->FillIt ==1 ))
    vgDrawPath(fill, VG_FILL_PATH);     
#endif

    /* Destroy the path */
    vgDestroyPath( path );
    vgDestroyPaint( fill );    
    return 1 ;
}


int GL_DrawingCircleNoFill( st_VGDrawingParameter *st_DrawPathInfo )
{
    	VGPath path;
    	VGPaint stroke;
 
     	stroke = vgCreatePaint();
 
			vgSetf( VG_STROKE_LINE_WIDTH, st_DrawPathInfo->StrokeWidth ) ;
		
 			vgSetParameteri( stroke, VG_PAINT_TYPE, st_DrawPathInfo->PaintMode );
 
 		if(st_DrawPathInfo->PaintMode == VG_PAINT_TYPE_COLOR)
 	    vgSetColor( stroke, st_DrawPathInfo->FillColor + st_DrawPathInfo->AlphaColor) ; 
 	  
 	  else if(st_DrawPathInfo->PaintMode == VG_PAINT_TYPE_LINEAR_GRADIENT)
 	  	{
  		vgSetParameterfv( stroke, VG_PAINT_LINEAR_GRADIENT, 4, &st_DrawPathInfo->GradientStartEnd[0] );										
  		vgSetParameterfv( stroke, VG_PAINT_COLOR_RAMP_STOPS, st_DrawPathInfo->GradientPoints, &st_DrawPathInfo->GradientStep[0] );
  		}
  	else if(st_DrawPathInfo->PaintMode == VG_PAINT_TYPE_RADIAL_GRADIENT)
 	  	{
			vgSetParameteri( stroke, VG_PAINT_COLOR_RAMP_SPREAD_MODE, VG_COLOR_RAMP_SPREAD_PAD );
		
  		vgSetParameterfv( stroke, VG_PAINT_RADIAL_GRADIENT, 5, &st_DrawPathInfo->GradientStartEnd[0] );										
  		vgSetParameterfv( stroke, VG_PAINT_COLOR_RAMP_STOPS, st_DrawPathInfo->GradientPoints, &st_DrawPathInfo->GradientStep[0] );
  		}

 		vgSetPaint(stroke, VG_STROKE_PATH );
 		
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
     
    vguEllipse( path, st_DrawPathInfo->PathDrawingData[0], st_DrawPathInfo->PathDrawingData[1], st_DrawPathInfo->PathDrawingData[2], st_DrawPathInfo->PathDrawingData[3] ) ;
 
    vgDrawPath( path,  VG_STROKE_PATH );
 
    /* Destroy the path */
    vgDestroyPath( path );
    vgDestroyPaint(stroke);
    
    return 1 ;
}

void GL_DrawingStroke(VGuint Lwidth, VGuint color, VGuint Style)
{
    VGPath path;
    VGPaint stroke;
    VGfloat Coord_Tbl[ 4 ];
 
		static const VGubyte StrokeSegments_TblPara[] =
		{
    VG_MOVE_TO_ABS, VG_LINE_TO_ABS} ;
 
    Coord_Tbl[ 0 ] = (VGfloat)CoordXSrc;						/* Move to abs, x */
		Coord_Tbl[ 1 ] = (VGfloat)CoordYSrc;						/* Move to abs, y */
    Coord_Tbl[ 2 ] = (VGfloat)CoordXDest;     			/* Move to abs, dx */
    Coord_Tbl[ 3 ] = (VGfloat)CoordYDest;     			/* Move to abs, dy */
 
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
    vgAppendPathData( path, ( sizeof StrokeSegments_TblPara ) / ( sizeof ( VGubyte )), StrokeSegments_TblPara, Coord_Tbl ) ;

    vgSeti( VG_STROKE_JOIN_STYLE, VG_JOIN_MITER ) ;
    vgSeti( VG_STROKE_CAP_STYLE, Style ) ;
    
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH );
    /* Destroy the path */
    vgDestroyPath( path );
    /* Destroy the path */
    vgDestroyPaint( stroke );
 
}

int GL_DrawingPoliline( void )
{
    VGPath path ;
    const VGfloat *points ;
    VGint count ;       /* The number of points */
    VGboolean closed ;

    vgSetf( VG_STROKE_LINE_WIDTH, 20 ) ;
    /*** Polyline ***/
    points = Polyline ;
    count = sizeof Polyline / ( sizeof ( VGfloat )) / 2u ;
    closed = VG_FALSE ;     /* Not closed shape */
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends a polyline segment to the path */
    vguPolygon( path, points, count, closed ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;

#if 0
    /*** Polygon ***/
    points = Polygon ;
    count = sizeof Polygon / ( sizeof ( VGfloat )) / 2u ;
    closed = VG_TRUE ;      /* Closed shape */
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends a polygon segment to the path */
    vguPolygon( path, points, count, closed ) ;
    /* The path is stroked */
    vgDrawPath( path, VG_STROKE_PATH ) ;
    /* Destroy the path */
    vgDestroyPath( path ) ;
#endif

    return 1 ;
}

int GL_DrawingRoundRectangle(st_VGDrawingParameter *st_DrawPathInfo )
{
 			VGPath path;
    	VGPaint fill;
 
    	    	
    	VGuint x, y, width, height, rx, ry ;

			fill = vgCreatePaint();

			vgSetf( VG_STROKE_LINE_WIDTH, st_DrawPathInfo->StrokeWidth ) ;
			
 			vgSetParameteri( fill, VG_PAINT_TYPE, st_DrawPathInfo->PaintMode );
 
 		if(st_DrawPathInfo->PaintMode == VG_PAINT_TYPE_COLOR)
 	    vgSetColor( fill, st_DrawPathInfo->FillColor + st_DrawPathInfo->AlphaColor) ; 
 	  
 	  else if(st_DrawPathInfo->PaintMode == VG_PAINT_TYPE_LINEAR_GRADIENT)
 	  	{
  		vgSetParameterfv( fill, VG_PAINT_LINEAR_GRADIENT, 4, &st_DrawPathInfo->GradientStartEnd[0] );										
  		vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, st_DrawPathInfo->GradientPoints, &st_DrawPathInfo->GradientStep[0] );
  		}
  	else if(st_DrawPathInfo->PaintMode == VG_PAINT_TYPE_RADIAL_GRADIENT)
 	  	{
			vgSetParameteri( fill, VG_PAINT_COLOR_RAMP_SPREAD_MODE, VG_COLOR_RAMP_SPREAD_PAD );
		
  		vgSetParameterfv( fill, VG_PAINT_RADIAL_GRADIENT, 5, &st_DrawPathInfo->GradientStartEnd[0] );										
  		vgSetParameterfv( fill, VG_PAINT_COLOR_RAMP_STOPS, st_DrawPathInfo->GradientPoints, &st_DrawPathInfo->GradientStep[0] );
  		}


    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
        
    vgLoadIdentity();
    vgSeti( VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_BETTER ) ;
 
  		if(( st_DrawPathInfo->StrokeIt == 1) && ( st_DrawPathInfo->FillIt == 0))	
      vgSetPaint(fill, VG_STROKE_PATH );
 
  		if(( st_DrawPathInfo->StrokeIt == 0) && ( st_DrawPathInfo->FillIt == 1))	
			vgSetPaint(fill, VG_FILL_PATH ); 
 		
  		if(( st_DrawPathInfo->StrokeIt == 1) && ( st_DrawPathInfo->FillIt == 1))	
			vgSetPaint(fill, VG_FILL_PATH | VG_STROKE_PATH); 	
 
// 		if((&st_DrawPathInfo->StrokeIt == 1) && (&st_DrawPathInfo->FillIt == 1))	
// 		vgSetPaint(fill, VG_FILL_PATH | VG_STROKE_PATH); 			
 		
   /*** Round-cornered rectangle ***/
   
 		x				= st_DrawPathInfo->PathDrawingData[0];
 		y				= st_DrawPathInfo->PathDrawingData[1];
 		width 	= st_DrawPathInfo->PathDrawingData[2];
 		height 	= st_DrawPathInfo->PathDrawingData[3];
 		rx 			= st_DrawPathInfo->PathDrawingData[4]*2;
 		ry 			= st_DrawPathInfo->PathDrawingData[5]*2;
 		
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
 
    vguRoundRect( path, x, y, width, height, rx * 2, ry * 2 ) ;
 
  		if(( st_DrawPathInfo->StrokeIt == 1) && ( st_DrawPathInfo->FillIt == 0))	
      	vgDrawPath( path, VG_STROKE_PATH );
 
  		if(( st_DrawPathInfo->StrokeIt == 0) && ( st_DrawPathInfo->FillIt == 1))	
				vgDrawPath( path, VG_FILL_PATH ); 
 		
  		if(( st_DrawPathInfo->StrokeIt == 1) && ( st_DrawPathInfo->FillIt == 1))	
				vgDrawPath( path, VG_FILL_PATH | VG_STROKE_PATH); 		 
 
 
    /* Destroy the path */
    vgDestroyPath( path );
    vgDestroyPaint(fill);
    
    return 1 ;
}
int GL_DrawingRoundRectanglePara( VGuint loc_x,  VGuint loc_y, VGuint W, VGuint H, VGuint xx,VGuint yy, VGuint color )
{
    VGPath path ;
    VGPaint fill;
    VGuint x, y, width, height, rx, ry ;
#if 0
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgLoadIdentity();

    vgTranslate(0.0f, 0.0f); 
#endif
  
    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;

    vgSetColor( fill, ( VGuint )color ) ;			//BGRA   


    
    /*** Round-cornered rectangle ***/
    x = loc_x ;
    y = loc_y ;
    width = W ;
    height = H ;
    rx = xx;
    ry = yy;
    /* Create a new path */
    path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL ) ;
    /* Appends an axis-aligned round-cornered rectangle to the path */
 
    vguRoundRect( path, x, y, width, height, rx * 2, ry * 2 ) ;
    /* The path is stroked */
    vgSetPaint(fill, VG_FILL_PATH);
//    vgDrawPath( path, VG_FILL_PATH | VG_STROKE_PATH ) ;
		vgDrawPath( path, VG_FILL_PATH  ) ;
    /* Destroy the path */
    vgDestroyPath( path );
    vgDestroyPaint(fill);

    return 1 ;
}