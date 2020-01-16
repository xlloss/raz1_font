/* $Revision: 2444 $ on $Date:: 2014-10-14 21:15:39 +0100 #$ */

/*------------------------------------------------------------------------
 * 
 * VGU 1.1 Reference Implementation
 * -------------------------------------
 *
 * Copyright (c) 2008 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and /or associated documentation files
 * (the "Materials "), to deal in the Materials without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Materials,
 * and to permit persons to whom the Materials are furnished to do so,
 * subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Materials. 
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE MATERIALS OR
 * THE USE OR OTHER DEALINGS IN THE MATERIALS.
 *
 *//**
 * \file
 * \brief	VGU 1.1 API.
 *//*-------------------------------------------------------------------*/

#ifndef _RETWLIB_H
#define _RETWLIB_H

#ifdef __cplusplus 
extern "C" { 
#endif
 

typedef enum {
  SETUP_YEAR	= 0,
  SETUP_MONTH,                         
  SETUP_DAY,                   
  SETUP_HOUR,                       
  SETUP_MIN,                  
  SETUP_CLOCK,
  SETUP_CALENDAR,                           
  SETUP_MAX 
} SETUPMODE;

typedef struct
{
    uint32_t		YEAR_LOC_X;
    uint32_t		YEAR_LOC_Y;    
 
} st_SETUP_DSP_TABLE;


typedef enum {
  VG_DRAWMODE_CIRCLE	= 0,
  VG_DRAWMODE_CIRCLEFILL,                         
  VG_DRAWMODE_LINECAP,     
  VG_DRAWMODE_LINEJOIN,       
  VG_DRAWMODE_LINEMITER,                  
  VG_DRAWMODE_POLICON                                     
} VG_DRAWMODE;


#define 	DrawingMode_Round


typedef enum {
  DrawingMode_Line	= 0,
  DrawingMode_Circle,                         
  DrawingMode_Retangle,                   
  DrawingMode_Policon,                       
  DrawingMode_RoundRetangle,                                            
  DrawingMode_MAX 
} OpenVGDrawingMode;

typedef struct
{
 		uint32_t	DrawingMode;
  	uint32_t	PaintMode;  	
 		uint32_t	StrokeIt;
 		uint32_t	FillIt;
  	 		
 		VGfloat		PathDrawingData[128];
 		uint32_t	PathPoints;
 		
 		VGfloat		GradientStartEnd[5];
 		VGfloat		GradientStep[5*8];
 		uint32_t	GradientPoints;
 		uint32_t	FillColor;
 		uint32_t	AlphaColor;
 		VGfloat		StrokeWidth;

} st_VGDrawingParameter;
 
typedef struct
{
 		VGfloat	CenterX;
 		VGfloat	CenterY;
 		VGfloat	Width;
 		VGfloat	Height;
  	VGfloat	StartAngle;
 		VGfloat	AngleExtent;
 		VGuint ArcWidth;
 		VGuint	ArcColor;

  	uint32_t	PaintMode;  
 		VGfloat		GradientStartEnd[5];
  	VGfloat		GradientStep[5*8];
  	uint32_t	GradientPoints;
 		uint32_t  FillColor;
 		
 		uint8_t		FillIt;
 		uint8_t		StrokeIt; 		
 
} st_VGDrawArcParameter;
 
typedef struct
{
	 	uint32_t	OnOff;
 		uint32_t	SetupItemPoint;
 		uint32_t	BackGround;	
 		uint32_t	Frame;
 		uint32_t	InFrame;
  	uint32_t	Surface;
  	uint32_t	Grid;
  	uint32_t	AddDigital;  	
  	uint32_t  Indicator;
  	uint32_t  Day; 
  	uint32_t  AnalogDigital;  
  	
} st_VGSetupMenuParameter;


typedef struct
{
 		uint32_t	CenterX;
 		uint32_t	CenterY;	

 		uint32_t	CenterXShift;
 		uint32_t	CenterYShift;	
 				
 		uint32_t	O1Outer;
 		uint32_t	O1Width;
 		uint32_t		O1Size;
 		uint32_t	O1InnerR;
 		uint32_t	O1OuterR; 		
 
 		uint32_t	O2Outer;
 		uint32_t	O2Width;
 		uint32_t		O2Size;
 		uint32_t	O2InnerR;
 
 		uint32_t	O3Outer;
 		uint32_t	O3Width;
 		uint32_t		O3Size;
 		uint32_t	O3InnerR;
 		
 		uint32_t	O4Outer;
 		uint32_t	O4Width;
 		uint32_t		O4Size; 
 		uint32_t	O4InnerR;

 		uint32_t	O5Outer;
 		uint32_t	O5Width;
 		uint32_t		O5Size; 
 		uint32_t	O5InnerR; 		
 		
 		uint32_t	Indicator_DegMain;

 		uint32_t	Digi_Number;
 
  	uint32_t 	AngleShift;
  	uint32_t  GridStart;					// by 0-2880
		uint32_t  GridExtent;  				// by 0-2880
		uint32_t  BackupCenterX;
		uint32_t  BackupCenterY;
		uint32_t  BackupO1Outer;
} st_VG_ObjProperty;


typedef struct
{
 		uint32_t	TargetSpeed;
 		uint32_t	IndicatorSpeed;
 		uint32_t	SpeedLimited;
 		
 		uint32_t	TargetRPM;						//range is 0 to 2879
 		uint32_t	IndicatorRPM; 		
 		
 		uint32_t	Temperature;
 		uint32_t	IndicatorGas;

 		uint32_t	GasolineLevel;
 		uint32_t	NaviDirection;			//	North is 0 deg,  East is 90 deg
 		uint32_t	KMTotal;		
 		uint32_t	KMsub;							//counte by 100 meters
} st_VirtualEngine_Info;

typedef struct
{
 		uint32_t	VirtualEngine;
 		uint32_t	CPULoad; 	 
 		uint32_t	ModeEntry;
 		uint32_t	MeterMotion;
 		uint32_t	TextMotion;		
 		uint32_t	SignMotion;
 		uint32_t	GraphicsDelay; 		
} st_MiliSecCounter;

typedef struct
{
 		uint32_t	CurrentSecDeg;
 		uint32_t	CurrentMinDeg; 	 
 		uint32_t	CurrentHourDeg;
 		uint32_t	TracingSecDeg;
 		uint32_t	TracingMinDeg; 	 
 		uint32_t	TracingHourDeg;	
} st_ClockIndiSet;



#ifdef __cplusplus 
} /* extern "C" */
#endif

#endif /* #ifndef _RETWLIB_H */
