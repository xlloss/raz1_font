
#include    <stdio.h>
#include    <string.h>
#include    <math.h>
#include    "r_typedefs.h"

#include    "VG/openvg.h"
#include    "VG/vgu.h"
#include 		"rtc.h"
#include  	"RETWvg.h"

//#include 		"SINCOS.h"

#define PI 3.14159265
 
#define Length_SecIndicator			200
#define Length_MinIndicator			160
#define Length_HourIndicator		140
#define Width_SecIndicator			2
#define Width_MinIndicator			6
#define Width_HourIndicator			10
#define BackL_SecIndicator			200
#define BackL_MinIndicator			160
#define BackL_HourIndicator			140
 
extern int GlobalSecDeg;
extern int GlobalMinDeg;
extern int GlobalHourDeg;

extern VGuint ClockShiftX;
extern VGuint ClockShiftDir;

extern VGuint ClockCenterX;
extern VGuint ClockCenterY;

extern VGint CoordXSrc;
extern VGint CoordYSrc;
extern VGint CoordXDest;
extern VGint CoordYDest;

extern const unsigned int SINtable[];
extern const unsigned int COStable[];

extern st_RTCTIME	SYSTEMTIME;

int RETW_GUI_Clock_Computer_1(void);
int GUI_DrawCapStyleIndicator(VGuint Indicator_deg, VGuint IndiLength, VGfloat width, VGuint color);
int GUI_DrawPolyconStyleIndicator(VGuint Indicator_deg, VGuint IndiLength, VGfloat width, VGuint color);
int GUI_DrawPolyconStyleIndicatorMin(VGuint Indicator_deg, VGuint IndiLength, VGfloat width, VGuint color);
int GUI_DrawPolyconStyleIndicatorHour(VGuint Indicator_deg, VGuint IndiLength, VGfloat width, VGuint color);

int CaculateCoordination(VGuint Indicator_deg, VGuint IndiLength);
 


extern int Polycon_DrawPointNum;
extern VGfloat Polycon_DataTab[];
extern VGfloat Polycon_linearGradient[]; 
extern VGfloat  Polycon_RampStopTab[];
extern VGfloat Polycon_RadGradient[] ;      /* Center (x, y), Focal (x, y), Radius r */

extern VGfloat rampStops[];
extern VGfloat linearGradient[];

extern uint8_t rtc_halfsecond_flag;
extern uint8_t rtc_halfsecond_flag_CNT;
 
 
 
extern uint32_t EngineRPM;

extern VGfloat 	G_CharScale;
extern VGfloat 	G_CharShearX;
extern VGfloat 	G_CharShearY;
 

		//												percent,B,G,R,alpha								percent,B,G,R,alpha								percent,B,G,R,alpha

 
int RETW_GUI_Meter_1( int speed,int Maxspeed)
{
				int i,j, iBlockNoIndex;
				int CurrentSpeedBlockIndex;
				VGuint New_deg;
				VGuint TiltDeg=30;
				VGuint CircleBottom = 50;
				VGuint CircleDiaInner = 280;
				VGuint CircleDiaOuter = 350;		
				VGuint CircleTargetHeight = 250;
				VGuint BlkColor;
				
				VGuint DisabledBlkNum = 6;
 				VGuint MaxBlockNum = 60-DisabledBlkNum*2;
 				
 							RETW_GUI_PoliconFillScreen(0,0,800,480,0x010101ff);
 							
 							
 				ClockCenterX = 400;
 				ClockCenterY = CircleBottom + CircleDiaOuter;
 
 				VGuint SpeedLimiterBlk = (80*48/220);				
#if 1
				CurrentSpeedBlockIndex = speed*MaxBlockNum/Maxspeed;
				
				for(i=DisabledBlkNum;i<(60-DisabledBlkNum);i++)
				{
					
				iBlockNoIndex = 60 -1 -DisabledBlkNum - i;
				
				New_deg = i*6+1;
				
				Polycon_DrawPointNum=0;
				j=Polycon_DrawPointNum*2;
  			CaculateCoordination(New_deg,	CircleDiaInner );
  			CoordYSrc = ((CoordYSrc-CircleBottom)*CircleTargetHeight/(CircleDiaOuter*2) )+CircleBottom;
  			
				Polycon_DataTab[j]  = CoordXSrc; 
				Polycon_DataTab[j+1] = CoordYSrc;
 
 				Polycon_DrawPointNum+=1;
				j=Polycon_DrawPointNum*2;
  			CaculateCoordination(New_deg,	CircleDiaOuter);  
  			CoordYSrc = ((CoordYSrc-CircleBottom)*CircleTargetHeight/(CircleDiaOuter*2) )+CircleBottom;		
				Polycon_DataTab[j]  = CoordXSrc; 
				Polycon_DataTab[j+1] = CoordYSrc;

				New_deg = i*6+5;
 
 				Polycon_DrawPointNum+=1;
				j=Polycon_DrawPointNum*2;
  			CaculateCoordination(New_deg,	CircleDiaOuter );
  			CoordYSrc = ((CoordYSrc-CircleBottom)*CircleTargetHeight/(CircleDiaOuter*2) )+CircleBottom;
				Polycon_DataTab[j]  = CoordXSrc; 
				Polycon_DataTab[j+1] = CoordYSrc;
				
 				Polycon_DrawPointNum+=1;
				j=Polycon_DrawPointNum*2;
  			CaculateCoordination(New_deg,	CircleDiaInner );  	
  			CoordYSrc = ((CoordYSrc-CircleBottom)*CircleTargetHeight/(CircleDiaOuter*2) )+CircleBottom;	
				Polycon_DataTab[j]  = CoordXSrc; 
				Polycon_DataTab[j+1] = CoordYSrc;

 				Polycon_DrawPointNum+=1;
				j=Polycon_DrawPointNum*2;			
 
 				if( iBlockNoIndex == SpeedLimiterBlk )								//Speed limiter is 80 kmh
 								BlkColor = 0x0000FFff;										//red color
 				else if (iBlockNoIndex < SpeedLimiterBlk)
 								{
 									if(CurrentSpeedBlockIndex > iBlockNoIndex)
 												BlkColor = 0xFFFF00ff;										//blue+green color 			
 									else						
  											BlkColor = 0xFFFFFFff;										//white color 											
 								}
 				else if (iBlockNoIndex > SpeedLimiterBlk)
 								{
 									if(CurrentSpeedBlockIndex > iBlockNoIndex)
 												BlkColor = 0xFF00FFff;										//RED color 			
 									else						
  											BlkColor = 0xFFFFFFff;									 										
 								}				
 				else  
 								BlkColor = 0xFFFFFFff;									 				
 
				TrVg_DrawingPolyconFillPara(BlkColor); 
  			}
  			
  			if(rtc_halfsecond_flag_CNT==1 && (SpeedLimiterBlk<CurrentSpeedBlockIndex) )
  			RETW_GUIDspWarning(400-50, 10);			//size is 100x102															//display warning sign if speed over limiter
  			
#endif
 
			
 

				if(speed <120){
								G_CharScale = 5;
								G_CharShearX =0.3;
								G_CharShearY = 0.0;
								RETW_GUI_DrawScriptNumber(400, 240, speed, 24 , 0xA0FFA0ff, 1 );
											}
				else{
								G_CharScale = 7.5;
								G_CharShearX =0.0;
								G_CharShearY = 0.0;
								RETW_GUI_DrawScriptNumber(400, 240, speed, 24 , 0x0000FFff, 1 );
								}				
				
 				RETW_GUI_Meter_DspFixPicture(630, 10);																										//display Clock Picture, Rnesasn Logo and kmh
				RETW_GUI_Meter_ClockIndicator(630+80,480-10-80);															//display indicator of clock

				RETW_GUI_Meter_Gasoline(20,100);
 
  				RETW_GUI_Show7Seg(8, 							310,110,		25, 40, 0x00FF00ff);	 							// display 5 digi number in center
  				RETW_GUI_Show7Seg(3, 							350,110,		25, 40, 0x00FF00ff);				
  				RETW_GUI_Show7Seg(speed/100, 			390,110,		25, 40, 0x00FF00ff);			
  				RETW_GUI_Show7Seg((speed%100)/10,	430,110,		25, 40, 0x00FF00ff);			
  				RETW_GUI_Show7Seg(speed%10, 			470,110,		25, 40, 0x00FF00ff);					
}

int RETW_GUI_Meter_2( int speed, int Maxspeed )
{
				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
				st_VGDrawArcParameter	ArcInfo;
//  				double param, result;			
  	
					static uint32_t O1Outer = 470;			//org is 460
					static uint32_t O1Width = 8;
					static float O1Size;

					static uint32_t O2Outer;
					static uint32_t O2Width = 10;
					static float O2Size;

					static uint32_t O3Outer;
					static uint32_t O3Width = 0;
					static uint32_t O3Size;
					static uint32_t O3InnerR;
 
 				O1Size = O1Outer-O1Width;
 				O2Outer = O1Outer-O1Width*2;
				O2Size = O2Outer-O2Width;
				O3Outer = O2Outer-O2Width*2;
				O3Size = O3Outer-O3Width;	
						
				O3InnerR = O3Size/2;

	 			ClockCenterX = 400;
	 			ClockCenterY = 240;
 							RETW_GUI_PoliconFillScreen(0,0,800,480,0x010101ff);
 							
//  		param = 30.0;
//  		result = sin (param*PI/180);
//  		printf ("The sine of %f degrees is %f.\n", param, result );
 
 
/*			|			+			|			+			|			+			|									Center							|			+			|			+			|			+			|
				|-----------------------------------------------------O1Outer-------------------------------------------------|
				|--O1Width--|
							|-----------------------------------------------O1Size--------------------------------------------|
				
										|-----------------------------------------O2Outer-------------------------------------|
										|--O2Width--|
													|-----------------------------------O2Size--------------------------------|

																|-----------------------------O3Outer-------------------------|
 																|-----------------------------O3Size-------------------------|
																|---------- O3InnerR--------------|
*/
 
 			#if 1					//most inner surface

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O3Size;
					VGParameter.PathDrawingData[3]= O3Size;		
								
					static VGfloat SrampStops1[35] = { 	0.0f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 		
																							0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 
	  		
																							0.85f, 0.1f, 0.0f, 0.0f, 1.0f, 
																							0.90f, 0.4f, 0.0f, 0.0f, 1.0f, 
																							0.95f, 0.5f, 0.0f, 0.0f, 1.0f, 	
																							1.00f, 0.6f, 0.0f, 0.0f, 1.0f, 																							
																							};									
					for(i=0;i<35;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 35;
					
					VGParameter.GradientStartEnd[0] = ClockCenterX;
					VGParameter.GradientStartEnd[1] = ClockCenterY;
					VGParameter.GradientStartEnd[2] = ClockCenterX;
					VGParameter.GradientStartEnd[3] = ClockCenterY;
					VGParameter.GradientStartEnd[4] = O3Size/2;
				  GL_DrawingCircleFill( &VGParameter);	
				  vgFinish();
			#endif
			
			#if 1				//second inner

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O2Size;
					VGParameter.PathDrawingData[3]= O2Size;	
									
					static VGfloat SrampStops2[30] = { 	0.0f, 0.0f, 0.0f, 0.0f, 1.0f,			
																							0.2f, 0.3f, 0.3f, 0.3f, 1.0f,  		
																							0.4f, 0.7f, 0.7f, 0.7f, 1.0f, 
																							0.6f, 0.1f, 0.1f, 0.1f, 0.0f,		
																							0.8f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							1.0f, 1.0f, 1.0f, 1.0f, 1.0f 																								
																							};
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops2[i]; 
					VGParameter.GradientPoints = 30;		
												
					VGParameter.GradientStartEnd[0] = ClockCenterX-(O2Size*1.8/4);
					VGParameter.GradientStartEnd[1] = ClockCenterY+(O2Size*1.8/4);
					VGParameter.GradientStartEnd[2] = ClockCenterX+(O2Size*1.8/4);
					VGParameter.GradientStartEnd[3] = ClockCenterY-(O2Size*1.8/4);
					VGParameter.StrokeWidth =O2Width;
				 	GL_DrawingCircleNoFill( &VGParameter);			
				 	vgFinish();		 
			#endif
			
			#if 1				//outer

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O1Size;
					VGParameter.PathDrawingData[3]= O1Size;	
					
					static VGfloat SrampStops3[30] = { 	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																							0.2f, 0.3f, 0.3f, 0.3f, 1.0f,  		
																							0.4f, 0.1f, 0.1f, 0.1f, 0.0f,
																							0.6f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																							0.8f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							1.0f, 0.0f, 0.0f, 0.0f, 1.0f 																							
																							};
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops3[i]; 
					VGParameter.GradientPoints = 30;	
 
					VGParameter.GradientStartEnd[0] = ClockCenterX-(O1Size*1.8/4);
					VGParameter.GradientStartEnd[1] = ClockCenterY+(O1Size*1.8/4);
					VGParameter.GradientStartEnd[2] = ClockCenterX+(O1Size*1.8/4);
					VGParameter.GradientStartEnd[3] = ClockCenterY-(O1Size*1.8/4);
					VGParameter.StrokeWidth =O1Width;
					
				 GL_DrawingCircleNoFill( &VGParameter);		 
				 vgFinish();
 			#endif

#if 1
				RETW_GUI_ImportGlobalTime();
#endif
  

#if 1
				for(i=0;i<241;i++)
				{

//				New_deg = 180+20+ i*320/240;
				New_deg = 180+45+ i*(360-45-45)/240;
 
  			CaculateCoordination(New_deg,	O3InnerR );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
  #if 1			
  			if(  (i%20) == 0)
  			{
  				CaculateCoordination(New_deg,	O3InnerR-55 );
  				G_CharScale = 1.2;
					G_CharShearX=0;
					G_CharShearY=0;
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, i, 18,0xA0A0A0ff, 1);
  				vgFinish();
  			}
	#endif
	
	#if 1
  			if(  i == 0  | i == 240)
  					{
  					CaculateCoordination(New_deg,	O3InnerR-25 );
						GL_DrawingStroke(4  , 0x0000FFff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
  			else if(  (i%10) == 0)
  					{
  					CaculateCoordination(New_deg,	O3InnerR-20 );
						GL_DrawingStroke(4  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
				else if(  (i%5) == 0)

  					{
  					CaculateCoordination(New_deg,	O3InnerR-15 );
						GL_DrawingStroke( 2  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
     		else
     				{
//     				CaculateCoordination(New_deg,	O3InnerR-12 );
//						GL_DrawingStroke( 1  , 0x808080ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}   	
	#endif
						
				vgFinish();
				
 				}	
/*
  VG_JOIN_MITER                               = 0x1800,
  VG_JOIN_ROUND                               = 0x1801,
  VG_JOIN_BEVEL                               = 0x1802,
*/
 		
#endif
 
 		
	#if 1


				RETW_GUI_Meter_SpeedLimiter(240,R_ADC_Read()* 240/1024);

 #if 1
 				ArcInfo.CenterX=400;
 				ArcInfo.CenterY=240;
 				ArcInfo.Width=160;
 				ArcInfo.Height=160;
  			ArcInfo.StartAngle= -30;
 				ArcInfo.AngleExtent= 240;
 				ArcInfo.ArcWidth=10;
 	 			ArcInfo.ArcColor=0x808000ff;	


 					ArcInfo.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 													
					static VGfloat SrampStops4[15] = { 	0.0f, 0.0f, 0.6f, 0.0f, 1.0f, 		
																							0.4f, 0.0f, 0.6f, 0.6f, 1.0f, 		
																							1.00f, 0.0f, 0.0f, 0.6f, 1.0f, 																							
																							};									
					for(i=0;i<15;i++)
					ArcInfo.GradientStep[i]=SrampStops4[i]; 
					ArcInfo.GradientPoints = 15;
					
					ArcInfo.GradientStartEnd[0] = ArcInfo.CenterX - ArcInfo.Width/2;
					ArcInfo.GradientStartEnd[1] = ArcInfo.CenterY;
					ArcInfo.GradientStartEnd[2] = ArcInfo.CenterX + ArcInfo.Width/2;;
					ArcInfo.GradientStartEnd[3] = ArcInfo.CenterY;

 //					ArcInfo.FillIt = 0;
 //					ArcInfo.StrokeIt = 1; 	
 					
 			  	GL_DrawingArcPara(&ArcInfo);
 #endif
 	
 				RETW_GUI_Meter_3_Cap(32,32,0xFFFFFFff);
				vgFinish();
				
 				RETW_GUI_Meter_RED_6p_Ind(O3InnerR, speed, 180+45, 270);
 				vgFinish();
 	#endif
 
}

int RETW_GUI_Meter_3( int speed, int Maxspeed )
{
				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
				st_VGDrawArcParameter	ArcInfo; 
//  				double param, result;			
  	
					static uint32_t O1Outer = 380;
					static uint32_t O1Width = 8;
					static float O1Size;

					static uint32_t O2Outer;
					static uint32_t O2Width = 10;
					static float O2Size;

					static uint32_t O3Outer;
					static uint32_t O3Width = 0;
					static uint32_t O3Size;
					static uint32_t O3InnerR;
 
 				O1Size = O1Outer-O1Width;
 				O2Outer = O1Outer-O1Width*2;
				O2Size = O2Outer-O2Width;
				O3Outer = O2Outer-O2Width*2;
				O3Size = O3Outer-O3Width;	
						
				O3InnerR = O3Size/2;

	 			ClockCenterX = 200;
	 			ClockCenterY = 240;
 							RETW_GUI_PoliconFillScreen(0,0,800,480,0x010101ff);
 							
//  		param = 30.0;
//  		result = sin (param*PI/180);
//  		printf ("The sine of %f degrees is %f.\n", param, result );
 
 
/*			|			+			|			+			|			+			|									Center							|			+			|			+			|			+			|
				|-----------------------------------------------------O1Outer-------------------------------------------------|
				|--O1Width--|
							|-----------------------------------------------O1Size--------------------------------------------|
				
										|-----------------------------------------O2Outer-------------------------------------|
										|--O2Width--|
													|-----------------------------------O2Size--------------------------------|

																|-----------------------------O3Outer-------------------------|
 																|-----------------------------O3Size-------------------------|
																|---------- O3InnerR--------------|
*/
 
 			#if 1					//most inner surface

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O3Size;
					VGParameter.PathDrawingData[3]= O3Size;		
								
					static VGfloat SrampStops1[35] = { 	0.0f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 		
																							0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 
	  		
																							0.85f, 0.1f, 0.0f, 0.0f, 1.0f, 
																							0.90f, 0.4f, 0.0f, 0.0f, 1.0f, 
																							0.95f, 0.5f, 0.0f, 0.0f, 1.0f, 	
																							1.00f, 0.6f, 0.0f, 0.0f, 1.0f, 																							
																							};			
					
		
					for(i=0;i<35;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 35;
					
					VGParameter.GradientStartEnd[0] = ClockCenterX;
					VGParameter.GradientStartEnd[1] = ClockCenterY;
					VGParameter.GradientStartEnd[2] = ClockCenterX;
					VGParameter.GradientStartEnd[3] = ClockCenterY;
					VGParameter.GradientStartEnd[4] = O3Size/2;
				  GL_DrawingCircleFill( &VGParameter);	
				  vgFinish();
			#endif
			
			#if 1				//second inner

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O2Size;
					VGParameter.PathDrawingData[3]= O2Size;	
									
					static VGfloat SrampStops2[30] = { 	0.0f, 0.0f, 0.0f, 0.0f, 1.0f,			
																							0.2f, 0.3f, 0.3f, 0.3f, 1.0f,  		
																							0.4f, 0.7f, 0.7f, 0.7f, 1.0f, 
																							0.6f, 0.1f, 0.1f, 0.1f, 0.0f,		
																							0.8f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							1.0f, 1.0f, 1.0f, 1.0f, 1.0f 																								
																							};
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops2[i]; 
					VGParameter.GradientPoints = 30;		
												
					VGParameter.GradientStartEnd[0] = ClockCenterX-(O2Size*1.8/4);
					VGParameter.GradientStartEnd[1] = ClockCenterY+(O2Size*1.8/4);
					VGParameter.GradientStartEnd[2] = ClockCenterX+(O2Size*1.8/4);
					VGParameter.GradientStartEnd[3] = ClockCenterY-(O2Size*1.8/4);
					VGParameter.StrokeWidth =O2Width;
				 	GL_DrawingCircleNoFill( &VGParameter);			
				 	vgFinish();		 
			#endif
			
			#if 1				//outer

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O1Size;
					VGParameter.PathDrawingData[3]= O1Size;	
					
					static VGfloat SrampStops3[30] = { 	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																							0.2f, 0.3f, 0.3f, 0.3f, 1.0f,  		
																							0.4f, 0.1f, 0.1f, 0.1f, 0.0f,
																							0.6f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																							0.8f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							1.0f, 0.0f, 0.0f, 0.0f, 1.0f 																							
																							};
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops3[i]; 
					VGParameter.GradientPoints = 30;	
 
					VGParameter.GradientStartEnd[0] = ClockCenterX-(O1Size*1.8/4);
					VGParameter.GradientStartEnd[1] = ClockCenterY+(O1Size*1.8/4);
					VGParameter.GradientStartEnd[2] = ClockCenterX+(O1Size*1.8/4);
					VGParameter.GradientStartEnd[3] = ClockCenterY-(O1Size*1.8/4);
					VGParameter.StrokeWidth =O1Width;
					
				 GL_DrawingCircleNoFill( &VGParameter);		 
				 vgFinish();
 			#endif

#if 0
				RETW_GUI_ImportGlobalTime();
#endif
  

#if 1
				for(i=0;i<241;i++)
				{

//				New_deg = 180+20+ i*320/240;
				New_deg = 180+45+ i*(360-45-45)/240;
 
  			CaculateCoordination(New_deg,	O3InnerR );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
  #if 1			
  			if(  (i%20) == 0)
  			{
  				CaculateCoordination(New_deg,	O3InnerR-55 );
  				G_CharScale = 1.0;
					G_CharShearX=0;
					G_CharShearY=0;
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, i, 18,0xA0A0A0ff, 1);
  				vgFinish();
  			}
	#endif
	
	#if 1
  			if(  i == 0  | i == 240)
  					{
  					CaculateCoordination(New_deg,	O3InnerR-25 );
						GL_DrawingStroke(4  , 0x0000FFff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
  			else if(  (i%10) == 0)
  					{
  					CaculateCoordination(New_deg,	O3InnerR-20 );
						GL_DrawingStroke(4  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
				else if(  (i%5) == 0)

  					{
  					CaculateCoordination(New_deg,	O3InnerR-15 );
						GL_DrawingStroke( 2  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
     		else
     				{
//     				CaculateCoordination(New_deg,	O3InnerR-12 );
//						GL_DrawingStroke( 1  , 0x808080ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}   	
	#endif
						
				vgFinish();
				
 				}	
/*
  VG_JOIN_MITER                               = 0x1800,
  VG_JOIN_ROUND                               = 0x1801,
  VG_JOIN_BEVEL                               = 0x1802,
*/
 		
#endif
 
 		
	#if 1


				RETW_GUI_Meter_SpeedLimiter(240,R_ADC_Read()* 240/1024);

 				G_CharScale = 0.7;
				G_CharShearX=0.0;
				G_CharShearY=0.0;	
				R_TXTR_DrawStringMain("km/h", 4, 200-20, 240+70, 0xFFFFFFff);
 
 #if 1
 				ArcInfo.CenterX=200;
 				ArcInfo.CenterY=240;
 				ArcInfo.Width=100;
 				ArcInfo.Height=100;
  			ArcInfo.StartAngle= -45;
 				ArcInfo.AngleExtent= 270;
 				ArcInfo.ArcWidth=20;
 	 			ArcInfo.ArcColor=0x808000ff;	


 					ArcInfo.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 													
					static VGfloat SrampStops4[15] = { 	0.0f, 0.6f, 0.0f, 0.0f, 1.0f, 		
																							0.4f, 0.0f, 0.6f, 0.0f, 1.0f, 		
																							1.00f, 0.0f, 0.0f, 0.6f, 1.0f, 																							
																							};									
					for(i=0;i<15;i++)
					ArcInfo.GradientStep[i]=SrampStops4[i]; 
					ArcInfo.GradientPoints = 15;
					
					ArcInfo.GradientStartEnd[0] = ArcInfo.CenterX - ArcInfo.Width/2;
					ArcInfo.GradientStartEnd[1] = ArcInfo.CenterY;
					ArcInfo.GradientStartEnd[2] = ArcInfo.CenterX + ArcInfo.Width/2;;
					ArcInfo.GradientStartEnd[3] = ArcInfo.CenterY;

 //					ArcInfo.FillIt = 0;
 //					ArcInfo.StrokeIt = 1; 	
 					 
 			  	GL_DrawingArcPara(&ArcInfo);
 #endif				


 				RETW_GUI_Meter_3_Cap(32,32,0xFFFFFFff);
				vgFinish();
				
 				RETW_GUI_Meter_RED_6p_Ind(O3InnerR, speed, 180+45, 270);
 				vgFinish();
 	#endif
 	
 RETW_GUI_Meter_Speed_3_Right(speed,Maxspeed);



#if 1 	//Speed Icon window
	#if 0
				VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
				VGParameter.PathDrawingData[0]= 400-70;							//org coord X
				VGParameter.PathDrawingData[1]= 390;								//org coord Y
				VGParameter.PathDrawingData[2]= 140;							//Width
				VGParameter.PathDrawingData[3]= 80;								//Height
				VGParameter.PathDrawingData[4]= 6;								//Corner X
				VGParameter.PathDrawingData[5]= 6;								//Corner Y				
 				
 				VGParameter.GradientStartEnd[0]=400-70;
 				VGParameter.GradientStartEnd[1]=470; 
 				VGParameter.GradientStartEnd[2]=400+70;
 				VGParameter.GradientStartEnd[3]=390; 

	#else


				VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;

				VGParameter.PathDrawingData[2]= 140;							//Width
				VGParameter.PathDrawingData[3]= 80;								//Height
				
				VGParameter.PathDrawingData[0]= 400-(VGParameter.PathDrawingData[2]/2);							//org coord X
				VGParameter.PathDrawingData[1]= 390;																						//org coord Y

				VGParameter.PathDrawingData[4]= 6;								//Corner X
				VGParameter.PathDrawingData[5]= 6;								//Corner Y				
 				
 				VGParameter.GradientStartEnd[0]=VGParameter.PathDrawingData[0]+(VGParameter.PathDrawingData[2]/2);
 				VGParameter.GradientStartEnd[1]=VGParameter.PathDrawingData[1]+VGParameter.PathDrawingData[3]; 
 				VGParameter.GradientStartEnd[2]=VGParameter.PathDrawingData[0]+(VGParameter.PathDrawingData[2]/2);
 				VGParameter.GradientStartEnd[3]=VGParameter.PathDrawingData[1]; 
 				
 	#endif		
 
					static VGfloat SrampStops5[10] = { 	0.0f, 0.3f, 0.3f, 0.3f, 1.0f, 					
																							1.0f, 0.1f, 0.1f, 0.1f, 1.0f																
																							};									
					for(i=0;i<10;i++)
					VGParameter.GradientStep[i]=SrampStops5[i]; 
					VGParameter.GradientPoints = 10;
 
					VGParameter.StrokeIt=0;
					VGParameter.FillIt=1; 
					GL_DrawingRoundRectangle(&VGParameter);								//fill 

					VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 					VGParameter.StrokeWidth = 3;
 					VGParameter.FillColor = 0x20202000;
 					VGParameter.AlphaColor = 0x000000ff;
					VGParameter.StrokeIt=1;
					VGParameter.FillIt=0; 
					GL_DrawingRoundRectangle(&VGParameter);								//stroke					
			
					 
 				G_CharScale = 3.0;
				G_CharShearX=0.1;
				G_CharShearY=0.0;	
//				R_TXTR_DrawStringMain("100",3, 400-45, 400+10, 0xFFFFFFff);
 
				RETW_GUI_DrawScriptNumber(410, 420, EngineRPM, 24 , 0xC0C0C0ff, 1 );

#endif									
					 
 
}


int RETW_GUI_Meter_Speed_3_Right( int speed, int Maxspeed )
{
				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
 
				st_VGDrawArcParameter	ArcInfo;
				
				 
//  				double param, result;			
  	
					static uint32_t O1Outer = 380;
					static uint32_t O1Width = 8;
					static float O1Size;

					static uint32_t O2Outer;
					static uint32_t O2Width = 10;
					static float O2Size;

					static uint32_t O3Outer;
					static uint32_t O3Width = 0;
					static uint32_t O3Size;
					static uint32_t O3InnerR;
 
 				O1Size = O1Outer-O1Width;
 				O2Outer = O1Outer-O1Width*2;
				O2Size = O2Outer-O2Width;
				O3Outer = O2Outer-O2Width*2;
				O3Size = O3Outer-O3Width;	
						
				O3InnerR = O3Size/2;

	 			ClockCenterX = 600;
	 			ClockCenterY = 240;
 
//  		param = 30.0;
//  		result = sin (param*PI/180);
//  		printf ("The sine of %f degrees is %f.\n", param, result );
 
 
/*			|			+			|			+			|			+			|									Center							|			+			|			+			|			+			|
				|-----------------------------------------------------O1Outer-------------------------------------------------|
				|--O1Width--|
							|-----------------------------------------------O1Size--------------------------------------------|
				
										|-----------------------------------------O2Outer-------------------------------------|
										|--O2Width--|
													|-----------------------------------O2Size--------------------------------|

																|-----------------------------O3Outer-------------------------|
 																|-----------------------------O3Size-------------------------|
																|---------- O3InnerR--------------|
*/
 
 			#if 1					//most inner surface

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O3Size;
					VGParameter.PathDrawingData[3]= O3Size;		
								
					static VGfloat SrampStops1[35] = { 	0.0f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 		
																							0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 
	  		
																							0.85f, 0.1f, 0.0f, 0.0f, 1.0f, 
																							0.90f, 0.4f, 0.0f, 0.0f, 1.0f, 
																							0.95f, 0.5f, 0.0f, 0.0f, 1.0f, 	
																							1.00f, 0.6f, 0.0f, 0.0f, 1.0f, 																							
																							};									
					for(i=0;i<35;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 35;
					
					VGParameter.GradientStartEnd[0] = ClockCenterX;
					VGParameter.GradientStartEnd[1] = ClockCenterY;
					VGParameter.GradientStartEnd[2] = ClockCenterX;
					VGParameter.GradientStartEnd[3] = ClockCenterY;
					VGParameter.GradientStartEnd[4] = O3Size/2;
				  GL_DrawingCircleFill( &VGParameter);	
				  vgFinish();
			#endif
			
			#if 1				//second inner

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O2Size;
					VGParameter.PathDrawingData[3]= O2Size;	
									
					static VGfloat SrampStops2[30] = { 	0.0f, 0.0f, 0.0f, 0.0f, 1.0f,			
																							0.2f, 0.3f, 0.3f, 0.3f, 1.0f,  		
																							0.4f, 0.7f, 0.7f, 0.7f, 1.0f, 
																							0.6f, 0.1f, 0.1f, 0.1f, 0.0f,		
																							0.8f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							1.0f, 1.0f, 1.0f, 1.0f, 1.0f 																								
																							};
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops2[i]; 
					VGParameter.GradientPoints = 30;		
												
					VGParameter.GradientStartEnd[0] = ClockCenterX-(O2Size*1.8/4);
					VGParameter.GradientStartEnd[1] = ClockCenterY+(O2Size*1.8/4);
					VGParameter.GradientStartEnd[2] = ClockCenterX+(O2Size*1.8/4);
					VGParameter.GradientStartEnd[3] = ClockCenterY-(O2Size*1.8/4);
					VGParameter.StrokeWidth =O2Width;
				 	GL_DrawingCircleNoFill( &VGParameter);			
				 	vgFinish();		 
			#endif
			
			#if 1				//outer

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O1Size;
					VGParameter.PathDrawingData[3]= O1Size;	
					
					static VGfloat SrampStops3[30] = { 	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																							0.2f, 0.3f, 0.3f, 0.3f, 1.0f,  		
																							0.4f, 0.1f, 0.1f, 0.1f, 0.0f,
																							0.6f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																							0.8f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							1.0f, 0.0f, 0.0f, 0.0f, 1.0f 																							
																							};
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops3[i]; 
					VGParameter.GradientPoints = 30;	
 
					VGParameter.GradientStartEnd[0] = ClockCenterX-(O1Size*1.8/4);
					VGParameter.GradientStartEnd[1] = ClockCenterY+(O1Size*1.8/4);
					VGParameter.GradientStartEnd[2] = ClockCenterX+(O1Size*1.8/4);
					VGParameter.GradientStartEnd[3] = ClockCenterY-(O1Size*1.8/4);
					VGParameter.StrokeWidth =O1Width;
					
				 GL_DrawingCircleNoFill( &VGParameter);		 
				 vgFinish();
 			#endif

#if 0
				RETW_GUI_ImportGlobalTime();
#endif
  

#if 1			//Grid
				for(i=0;i<181;i++)
				{
 
//				New_deg = 180+45+ i*(360-45-45)/240;
				New_deg = 180+90+ i*(360-90-90)/180;
	 
  			CaculateCoordination(New_deg,	O3InnerR );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
  #if 1			
  			if(  (i%30) == 0)
  			{
  				CaculateCoordination(New_deg,	O3InnerR-55 );
  				G_CharScale = 1.0;
					G_CharShearX=0;
					G_CharShearY=0;
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, i/30, 18,0xA0A0A0ff, 1);
  				vgFinish();
  			}
	#endif
	
	#if 1
  			if(  i == 0  | i == 180)
  					{
  					CaculateCoordination(New_deg,	O3InnerR-25 );
						GL_DrawingStroke(4  , 0x0000FFff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
  			else if(  (i%30) == 0)
  					{
  					CaculateCoordination(New_deg,	O3InnerR-20 );
						GL_DrawingStroke(4  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
				else if(  (i%15) == 0)

  					{
  					CaculateCoordination(New_deg,	O3InnerR-15 );
						GL_DrawingStroke( 2  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
     		else
     				{
//     				CaculateCoordination(New_deg,	O3InnerR-12 );
//						GL_DrawingStroke( 1  , 0x808080ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}   	
	#endif
 
				vgFinish();
				
 				}	
/*
  VG_JOIN_MITER                               = 0x1800,
  VG_JOIN_ROUND                               = 0x1801,
  VG_JOIN_BEVEL                               = 0x1802,
*/
#endif
 
//				RETW_GUI_Meter_SpeedLimiter(240,R_ADC_Read()* 240/1024);

 				G_CharScale = 0.7;
				G_CharShearX=0.0;
				G_CharShearY=0.0;	
				R_TXTR_DrawStringMain("x1000 rpm", 9, 600-45, 240+70, 0xFFFFFFff);
				
				
 #if 1
 				ArcInfo.CenterX=600;
 				ArcInfo.CenterY=240;
 				ArcInfo.Width=100;
 				ArcInfo.Height=100;
  			ArcInfo.StartAngle= 0;
 				ArcInfo.AngleExtent= 180;
 				ArcInfo.ArcWidth=20;
 	 			ArcInfo.ArcColor=0x808000ff;	


 					ArcInfo.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 													
					static VGfloat SrampStops4[15] = { 	0.0f, 0.6f, 0.0f, 0.0f, 1.0f, 		
																							0.4f, 0.0f, 0.6f, 0.0f, 1.0f, 		
																							1.00f, 0.0f, 0.0f, 0.6f, 1.0f, 																							
																							};									
					for(i=0;i<15;i++)
					ArcInfo.GradientStep[i]=SrampStops4[i]; 
					ArcInfo.GradientPoints = 15;
					
					ArcInfo.GradientStartEnd[0] = ArcInfo.CenterX - ArcInfo.Width/2;
					ArcInfo.GradientStartEnd[1] = ArcInfo.CenterY;
					ArcInfo.GradientStartEnd[2] = ArcInfo.CenterX + ArcInfo.Width/2;;
					ArcInfo.GradientStartEnd[3] = ArcInfo.CenterY;

 	//				ArcInfo.FillIt = 0;
 	//				ArcInfo.StrokeIt = 1; 	
 					 
 			  	GL_DrawingArcPara(&ArcInfo);
 #endif
 				
 				RETW_GUI_Meter_3_Cap(32,32,0xFFFFFFff);
				vgFinish();
				
 				RETW_GUI_Meter_RED_6p_Ind_Right(O3InnerR, speed, 270, 180);
 				vgFinish();
 
 


 
 
}

int RETW_GUI_Music_1()
{
				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
 
//  				double param, result;			
  	
					static uint32_t O1Outer = 460;
					static uint32_t O1Width = 8;
					static float O1Size;

					static uint32_t O2Outer;
					static uint32_t O2Width = 10;
					static float O2Size;

					static uint32_t O3Outer;
					static uint32_t O3Width = 0;
					static uint32_t O3Size;
					static uint32_t O3InnerR;
 
 				O1Size = O1Outer-O1Width;
 				O2Outer = O1Outer-O1Width*2;
				O2Size = O2Outer-O2Width;
				O3Outer = O2Outer-O2Width*2;
				O3Size = O3Outer-O3Width;	
						
				O3InnerR = O3Size/2;

	 			ClockCenterX = 400;
	 			ClockCenterY = 240;
 
//  		param = 30.0;
//  		result = sin (param*PI/180);
//  		printf ("The sine of %f degrees is %f.\n", param, result );
 
 
/*			|			+			|			+			|			+			|									Center							|			+			|			+			|			+			|
				|-----------------------------------------------------O1Outer-------------------------------------------------|
				|--O1Width--|
							|-----------------------------------------------O1Size--------------------------------------------|
				
										|-----------------------------------------O2Outer-------------------------------------|
										|--O2Width--|
													|-----------------------------------O2Size--------------------------------|

																|-----------------------------O3Outer-------------------------|
 																|-----------------------------O3Size-------------------------|
																|---------- O3InnerR--------------|
*/
 
 			#if 1					//most inner surface

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O3Size;
					VGParameter.PathDrawingData[3]= O3Size;		
								
					static VGfloat SrampStops1[35] = { 	0.0f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 		
																							0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 
	  		
																							0.85f, 0.1f, 0.0f, 0.0f, 1.0f, 
																							0.90f, 0.4f, 0.0f, 0.0f, 1.0f, 
																							0.95f, 0.5f, 0.0f, 0.0f, 1.0f, 	
																							1.00f, 0.6f, 0.0f, 0.0f, 1.0f, 																							
																							};									
					for(i=0;i<35;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 35;
					
					VGParameter.GradientStartEnd[0] = ClockCenterX;
					VGParameter.GradientStartEnd[1] = ClockCenterY;
					VGParameter.GradientStartEnd[2] = ClockCenterX;
					VGParameter.GradientStartEnd[3] = ClockCenterY;
					VGParameter.GradientStartEnd[4] = O3Size/2;
				  GL_DrawingCircleFill( &VGParameter);	
				  vgFinish();
			#endif
			
			#if 1				//second inner

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O2Size;
					VGParameter.PathDrawingData[3]= O2Size;	
									
					static VGfloat SrampStops2[30] = { 	0.0f, 0.0f, 0.0f, 0.0f, 1.0f,			
																							0.2f, 0.3f, 0.3f, 0.3f, 1.0f,  		
																							0.4f, 0.7f, 0.7f, 0.7f, 1.0f, 
																							0.6f, 0.1f, 0.1f, 0.1f, 0.0f,		
																							0.8f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							1.0f, 1.0f, 1.0f, 1.0f, 1.0f 																								
																							};
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops2[i]; 
					VGParameter.GradientPoints = 30;		
												
					VGParameter.GradientStartEnd[0] = ClockCenterX-(O2Size*1.8/4);
					VGParameter.GradientStartEnd[1] = ClockCenterY+(O2Size*1.8/4);
					VGParameter.GradientStartEnd[2] = ClockCenterX+(O2Size*1.8/4);
					VGParameter.GradientStartEnd[3] = ClockCenterY-(O2Size*1.8/4);
					VGParameter.StrokeWidth =O2Width;
				 	GL_DrawingCircleNoFill( &VGParameter);			
				 	vgFinish();		 
			#endif
			
			#if 1				//outer

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O1Size;
					VGParameter.PathDrawingData[3]= O1Size;	
					
					static VGfloat SrampStops3[30] = { 	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																							0.2f, 0.3f, 0.3f, 0.3f, 1.0f,  		
																							0.4f, 0.1f, 0.1f, 0.1f, 0.0f,
																							0.6f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																							0.8f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							1.0f, 0.0f, 0.0f, 0.0f, 1.0f 																							
																							};
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops3[i]; 
					VGParameter.GradientPoints = 30;	
 
					VGParameter.GradientStartEnd[0] = ClockCenterX-(O1Size*1.8/4);
					VGParameter.GradientStartEnd[1] = ClockCenterY+(O1Size*1.8/4);
					VGParameter.GradientStartEnd[2] = ClockCenterX+(O1Size*1.8/4);
					VGParameter.GradientStartEnd[3] = ClockCenterY-(O1Size*1.8/4);
					VGParameter.StrokeWidth =O1Width;
					
				 GL_DrawingCircleNoFill( &VGParameter);		 
				 vgFinish();
 			#endif

#if 1
				RETW_GUI_ImportGlobalTime();
#endif
  

#if 1
				for(i=0;i<241;i++)
				{

//				New_deg = 180+20+ i*320/240;
				New_deg = 180+45+ i*(360-45-45)/240;
 
  			CaculateCoordination(New_deg,	O3InnerR );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
  #if 1			
  			if(  (i%20) == 0)
  			{
  				CaculateCoordination(New_deg,	O3InnerR-55 );
  				G_CharScale = 1.2;
					G_CharShearX=0;
					G_CharShearY=0;
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, i, 18,0xA0A0A0ff, 1);
  				vgFinish();
  			}
	#endif
	
	#if 1
  			if(  i == 0  | i == 240)
  					{
  					CaculateCoordination(New_deg,	O3InnerR-25 );
						GL_DrawingStroke(4  , 0x0000FFff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
  			else if(  (i%10) == 0)
  					{
  					CaculateCoordination(New_deg,	O3InnerR-20 );
						GL_DrawingStroke(4  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
				else if(  (i%5) == 0)

  					{
  					CaculateCoordination(New_deg,	O3InnerR-15 );
						GL_DrawingStroke( 2  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
     		else
     				{
//     				CaculateCoordination(New_deg,	O3InnerR-12 );
//						GL_DrawingStroke( 1  , 0x808080ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}   	
	#endif
						
				vgFinish();
				
 				}	
/*
  VG_JOIN_MITER                               = 0x1800,
  VG_JOIN_ROUND                               = 0x1801,
  VG_JOIN_BEVEL                               = 0x1802,
*/
 		
#endif
 
}
 
int RETW_GUI_Meter_ClockIndicator(int x,int y)
{
				int i;
				int New_deg;

				st_VGDrawingParameter	VGParameter;
 
	 			ClockCenterX= x;
				ClockCenterY = y;
 
 				RETW_GUI_ImportGlobalTimeMotion1();
 
				CaculateCoordination(	GlobalHourDeg, 33);
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
				New_deg = CaculateIncDeg(GlobalHourDeg,180);
				CaculateCoordination(	New_deg, 5);							
     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 3, 0x000000ff);									// Indicator by Draw Line with Cap			

				CaculateCoordination(	GlobalMinDeg, 53);
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
				New_deg = CaculateIncDeg(GlobalMinDeg,180);
				CaculateCoordination(	New_deg, 10);							
     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 2, 0x000000ff);									// Indicator by Draw Line with Cap			

				CaculateCoordination(	GlobalSecDeg, 60);
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
				New_deg = CaculateIncDeg(GlobalSecDeg,180);
				CaculateCoordination(	New_deg, 20);							
     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 2, 0x0000FFff);									// Indicator by Draw Line with Cap		

#if 0
					Polycon_linearGradient[0] = ClockCenterX-4;
					Polycon_linearGradient[1] = ClockCenterY+4;
					Polycon_linearGradient[2] = ClockCenterX+4;
					Polycon_linearGradient[3] = ClockCenterY-4 ;					
					static VGfloat SrampStops1[15] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 		0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 		1.0f, 0.0f, 0.0f, 0.5f, 1.0f };
					for(i=0;i<15;i++)
					rampStops[i]=SrampStops1[i]; 
     		  TrVg_DrawingCircleMode( 2, ClockCenterX, ClockCenterY,4, 4, 2, 0x0000FFff );
#else  
 
 					VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;		
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= 7;							/*	width			*/
					VGParameter.PathDrawingData[3]= 7;							/*	height		*/
					VGParameter.AlphaColor = 0x0000FFff;
				  GL_DrawingCircleFill( &VGParameter);	     		  
#endif     		  
    		  
}

int RETW_GUI_Meter_GasolineIndicator(int x,int y,int Vol )
{
				int i,j;
				int New_deg = Vol;
				int Org_deg = Vol;
					
				st_VGDrawingParameter	VGParameter;
				
	 			ClockCenterX = x;
				ClockCenterY = y;
#if 0 		//indicator by 2 CAP Line
				CaculateCoordination(	New_deg, 90);
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
				New_deg = CaculateIncDeg(New_deg,180);
				CaculateCoordination(	New_deg, 20);		
					
//   			vgSeti( VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_BETTER ) ;								
 
     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 8, 0x00FFFFff);									// Indicator by Draw Line with Cap		
     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 6, 0x0000FFff);									// Indicator by Draw Line with Cap		 
#endif
 

#if 1	//Indicator by Policon fill block

 				VGParameter.PathPoints=0;
 				VGParameter.GradientPoints=0;
				j=0;
#if 1
 				New_deg = CaculateIncDeg(Org_deg,2);
 				CaculateCoordination(	New_deg, 90);
				VGParameter.PathDrawingData[j]  = CoordXSrc; 
				VGParameter.PathDrawingData[j+1] = CoordYSrc;
				VGParameter.PathPoints+=1;
				j+=2;
#endif
				New_deg = CaculateIncDeg(Org_deg,180-15);				
 				CaculateCoordination(	New_deg, 20);				
				VGParameter.PathDrawingData[j]  = CoordXSrc; 
				VGParameter.PathDrawingData[j+1] = CoordYSrc;
				VGParameter.PathPoints+=1;
				j+=2;

				New_deg = CaculateIncDeg(Org_deg,180+15);				
 				CaculateCoordination(	New_deg, 20);				
				VGParameter.PathDrawingData[j]  = CoordXSrc; 
				VGParameter.PathDrawingData[j+1] = CoordYSrc;
				VGParameter.PathPoints+=1;
				j+=2;

					VGParameter.GradientStartEnd[2] = CoordXSrc;
					VGParameter.GradientStartEnd[3] = CoordYSrc;
#if 1
				New_deg = CaculateIncDeg(Org_deg,360-2);				
 				CaculateCoordination(	New_deg, 90);				
				VGParameter.PathDrawingData[j]  = CoordXSrc; 
				VGParameter.PathDrawingData[j+1] = CoordYSrc;
				VGParameter.PathPoints+=1;
				j+=2;
#endif
 				CaculateCoordination(	Org_deg,100);				
				VGParameter.PathDrawingData[j]  = CoordXSrc; 
				VGParameter.PathDrawingData[j+1] = CoordYSrc;
				VGParameter.PathPoints+=1;
				j+=2;
 
					VGParameter.GradientStartEnd[0] = CoordXSrc;
					VGParameter.GradientStartEnd[1] = CoordYSrc;	
 
/*
  VG_PAINT_TYPE_COLOR                         = 0x1B00,
  VG_PAINT_TYPE_LINEAR_GRADIENT               = 0x1B01,
  VG_PAINT_TYPE_RADIAL_GRADIENT               = 0x1B02,				
  
  VGParameter.FillColor=0xFF000000;
  VGParameter.AlphaColor=0x00000040;										*/
  
 				VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
					static VGfloat SrampStops1[20] = { 0.0f, 0.3f, 0.3f, 1.0f, 1.0f, 		0.4f, 0.0f, 0.0f, 0.8f, 1.0f, 		0.6f, 0.0f, 0.0f, 1.0f, 1.0f, 		0.8f, 0.0f, 0.0f, 0.8f, 1.0f };
					for(i=0;i<20;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					
					VGParameter.GradientPoints = 20;
					
				 GL_DrawingPolyconFill( &VGParameter);						

#endif

//				TrVg_DrawingCircleFillPureColor(ClockCenterX,ClockCenterY,20,20,1,0x0000FFff);	
				
		#if 0			//CAP circle
					Polycon_linearGradient[0] = ClockCenterX-10;
					Polycon_linearGradient[1] = ClockCenterY+10;
					Polycon_linearGradient[2] = ClockCenterX+10;
					Polycon_linearGradient[3] = ClockCenterY-10;	
 
					static VGfloat SrampStops2[15] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 		0.5f, 0.0f, 0.0f, 0.7f, 1.0f, 		1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
					for(i=0;i<15;i++)
					Polycon_RampStopTab[i]=SrampStops2[i]; 
					
 					TrVg_DrawingCircleLiearRadiantPara(ClockCenterX,ClockCenterY,20,20,1,0x0000FFff,15);	
 
//					TrVg_DrawingCircleLiearRadiant(ClockCenterX,ClockCenterY,20,20,1,0x00FF00ff);	
    #else
/*
  VG_PAINT_TYPE_COLOR                         = 0x1B00,
  VG_PAINT_TYPE_LINEAR_GRADIENT               = 0x1B01,
  VG_PAINT_TYPE_RADIAL_GRADIENT               = 0x1B02,				
  
  VGParameter.FillColor=0xFF000000;
  VGParameter.AlphaColor=0x00000040;										*/
  
 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= 20;
					VGParameter.PathDrawingData[3]= 20;					
						
					static VGfloat SrampStops2[15] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 		0.5f, 0.0f, 0.0f, 0.7f, 1.0f, 		1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
					for(i=0;i<15;i++)
					VGParameter.GradientStep[i]=SrampStops2[i]; 
					VGParameter.GradientPoints = 15;
					
					VGParameter.GradientStartEnd[0] = ClockCenterX-10;
					VGParameter.GradientStartEnd[1] = ClockCenterY+10;
					VGParameter.GradientStartEnd[2] = ClockCenterX+10;
					VGParameter.GradientStartEnd[3] = ClockCenterY-10;
 
				 GL_DrawingCircleFill( &VGParameter);	
    #endif
}
 

int RETW_GUI_Meter_Gasoline( int Vol, int MaxVol)
{
				int i;
				VGuint New_deg;
 
 				ClockCenterX = 110;
 				ClockCenterY = 480-150;
 				
 				for(i = 40; i<61;i++)
 				{
				New_deg = i*6;
				if(New_deg==360)	
					New_deg=0;
				
  			CaculateCoordination(New_deg,	100 );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
  			CaculateCoordination(New_deg,	100 + (i-40)*7/4);
  			
#if 1
  			if(  (i%5) == 0)
  			{
  					if(i<51)
						Stroke_JoinStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 7  , 0x00F000ff);									// Indicator by Draw Line with Cap   
						else
						Stroke_JoinStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 5  , 0x0000F0ff);									// Indicator by Draw Line with Cap				
     	  }
     	  else
				Stroke_JoinStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 2  , 0xF0F0F0ff);									// Indicator by Draw Line with Cap       		
#endif
 			
 				}

				RETW_GUI_Meter_GasolineIndicator( ClockCenterX, ClockCenterY, EngineRPM +240 -1);		
//  			RETW_GUI_Meter_GasolineIndicator( 400, 300, EngineRPM +240 -1);			
				 				
}

 
 
int RETW_GUI_Meter_3_Cap(uint32_t width, uint32_t height, uint32_t color)
{
				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= width;							/*	width			*/
					VGParameter.PathDrawingData[3]= height;							/*	height		*/
 	
					static VGfloat SrampStops1[30] = { 	0.0f, 0.0f, 0.0f, 0.9f, 1.0f, 		
																							0.2f, 0.0f, 0.0f, 0.7f, 1.0f, 		
																							0.4f, 0.0f, 0.0f, 0.6f, 1.0f,
																							0.6f, 0.0f, 0.0f, 0.5f, 1.0f, 	
																							0.8f, 0.0f, 0.0f, 0.4f, 1.0f,	
																							1.0f, 0.0f, 0.0f, 0.3f, 1.0f
																							 };
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 30;
					if( VGParameter.PaintMode ==VG_PAINT_TYPE_LINEAR_GRADIENT)
					{
					VGParameter.GradientStartEnd[0] = ClockCenterX;
					VGParameter.GradientStartEnd[1] = ClockCenterY+12;
					VGParameter.GradientStartEnd[2] = ClockCenterX;
					VGParameter.GradientStartEnd[3] = ClockCenterY-12 ; }
					if( VGParameter.PaintMode ==VG_PAINT_TYPE_RADIAL_GRADIENT)
					{
					VGParameter.GradientStartEnd[0] = ClockCenterX;
					VGParameter.GradientStartEnd[1] = ClockCenterY;
					VGParameter.GradientStartEnd[2] = ClockCenterX;
					VGParameter.GradientStartEnd[3] = ClockCenterY;
					VGParameter.GradientStartEnd[4] = 24/2;
				  }
				  
				 GL_DrawingCircleFill( &VGParameter);	

}
 
 

int RETW_GUI_Meter_RED_6p_Ind(uint32_t MaxLength, uint32_t speed, uint32_t StartDeg, uint32_t ExtentDeg )
{
/*				
				 /-----------------------------------------------------------------------------------/
				/	|																																								 	/|
			 /	cc																																						   / ff
			*    								  																									*  		    	*
			 \																																									 \
			  \																																									  \	
			   \-----------------------------------------------------------------------------------\
			   
			*--------------------------------------------aa-------------------------*
			   	*----------------------------------------bb-------------------------*-----dd-------*
			   																																			*-------ee--*
*/
				uint32_t IndiNum, i,j,k,New_deg,Indi_deg;
				st_VGDrawingParameter	VGParameter;
				
				static uint32_t IndicatorLoc[3][10]= {
																				 		{2,180-50,		 	180-25,20,		 180+25,20,		 360-2, 180-50,		0, 180-40},				//hour
																				 		{2,180-25,		 	180-15,20,		 180+15,20,		 360-1, 180-25,		0, 180-20},				//minute
																						{1,180-5,		 		180-10,20,		 180+10,20,		 360-1, 180-5,		0, 180}						//second
																						};
				static VGfloat SrampStops[35] = { 0.0f, 0.0f, 0.0f, 0.6f, 1.0f, 		
																					0.35f, 0.0f, 0.0f, 0.8f, 1.0f, 		
																					0.40f, 0.2f, 0.2f, 0.9f, 1.0f, 	
																					0.5f, 0.0f, 0.0f, 1.0f, 1.0f,	
																					0.6f, 0.0f, 0.0f, 0.7f, 1.0f, 	
																					0.65f, 0.1f, 0.1f, 0.3f, 1.0f,	
																					1.0f, 0.1f, 0.1f, 0.2f, 1.0f };
				
 				i=0;		//Hour
						IndicatorLoc[i][0] = MaxLength-60;							//aa
						IndicatorLoc[i][1] = MaxLength-70;							//bb
						IndicatorLoc[i][2] = 5;													//cc
						IndicatorLoc[i][3] = 25;												//dd
						IndicatorLoc[i][4] = 30;												//ee
						IndicatorLoc[i][5] = 10;													//ff							
 				i=1;			//Minute
						IndicatorLoc[i][0] = MaxLength-25;							//aa
						IndicatorLoc[i][1] = MaxLength-35;							//bb
						IndicatorLoc[i][2] = 3;													//cc
						IndicatorLoc[i][3] = 15;												//dd
						IndicatorLoc[i][4] = 20;												//ee
						IndicatorLoc[i][5] = 5;													//ff							
 				i=2;			//Second
						IndicatorLoc[i][0] = MaxLength-2;								//aa
						IndicatorLoc[i][1] = MaxLength-5;							 //bb
						IndicatorLoc[i][2] = 1;													//cc
						IndicatorLoc[i][3] = 30;							//dd
						IndicatorLoc[i][4] = 35;							//ee
						IndicatorLoc[i][5] = 3;													//ff	 
 
				IndiNum=0; 

#if 0		//Give start and extentAngle		 this will FAIL NOW
 							Indi_deg = ((speed* ExtentDeg)/220 ) + StartDeg;
 							
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif

#if 0		//for ADC input 
 							Indi_deg = ((speed* 270)/220 ) +180+45;
 							
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif

#if 1		//for Engine meter
 							Indi_deg=EngineRPM+240-1;
#endif

#if 0 
 							Indi_deg = ((speed* 270)/220 ) +180+45;
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif
 
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]);															//aa		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][1]);															//bb
 							New_deg = CaculateIncDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[IndiNum][2]);						//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateIncDeg(Indi_deg, 180);																								
							CaculateCoordination(	New_deg, IndicatorLoc[IndiNum][3]);																//dd
 							New_deg = CaculateDecDeg(New_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[IndiNum][5]); 					//ff
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
 							New_deg = CaculateIncDeg(Indi_deg, 180);
 							CaculateCoordination(	New_deg, IndicatorLoc[IndiNum][4]);																//ee
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
							k+=1; 

 							New_deg = CaculateIncDeg(Indi_deg, 180);																								
							CaculateCoordination(	New_deg, IndicatorLoc[IndiNum][3]);																//dd
 							New_deg = CaculateIncDeg(New_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[IndiNum][5]); 					//ff
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		 					
 
 							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][1]);															//bb
 							New_deg = CaculateDecDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[IndiNum][2]);						//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
		/*-------------Calculate	GradientStartEnd			------*/
	 
	#if 1
	
 							if(Indi_deg<180)
 							{							
 							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]/6);															 
 							New_deg = CaculateIncDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, 20 );									 
  
								VGParameter.GradientStartEnd[2] = CoordXSrc;
								VGParameter.GradientStartEnd[3] = CoordYSrc;
 
  							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]/6);															 
 							New_deg = CaculateDecDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, 20 );							
 
								VGParameter.GradientStartEnd[0] = CoordXSrc;
								VGParameter.GradientStartEnd[1] = CoordYSrc;
							}
							else
 							{							
 							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]/6);															 
 							New_deg = CaculateIncDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, 20 );									 
  
								VGParameter.GradientStartEnd[0] = CoordXSrc;
								VGParameter.GradientStartEnd[1] = CoordYSrc;
 
  							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]/6);															 
 							New_deg = CaculateDecDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, 20 );							
 
								VGParameter.GradientStartEnd[2] = CoordXSrc;
								VGParameter.GradientStartEnd[3] = CoordYSrc;
							}					  
  
	#else
 							if(Indi_deg<180)
								{
							VGParameter.GradientStartEnd[0] = VGParameter.PathDrawingData[4];
							VGParameter.GradientStartEnd[1] = VGParameter.PathDrawingData[5];	
							VGParameter.GradientStartEnd[2] = VGParameter.PathDrawingData[8];
							VGParameter.GradientStartEnd[3] = VGParameter.PathDrawingData[9];	
							}
							else
								{
							VGParameter.GradientStartEnd[0] = VGParameter.PathDrawingData[8];
							VGParameter.GradientStartEnd[1] = VGParameter.PathDrawingData[9];	
							VGParameter.GradientStartEnd[2] = VGParameter.PathDrawingData[4];
							VGParameter.GradientStartEnd[3] = VGParameter.PathDrawingData[5];								
							}
						
	#endif	
	
/*
  VG_PAINT_TYPE_COLOR                         = 0x1B00,
  VG_PAINT_TYPE_LINEAR_GRADIENT               = 0x1B01,
  VG_PAINT_TYPE_RADIAL_GRADIENT               = 0x1B02,				
*/
 							VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
							for(i=0;i<35;i++)
							VGParameter.GradientStep[i]=SrampStops[i]; 
							VGParameter.GradientPoints = 35;		
							GL_DrawingPolyconFill( &VGParameter);	
}

int RETW_GUI_Meter_RED_6p_Ind_Right(uint32_t MaxLength,uint32_t speed, uint32_t StartDeg, uint32_t ExtentDeg )
{
/*				
				 /-----------------------------------------------------------------------------------/
				/	|																																								 	/|
			 /	cc																																						   / ff
			*    								  																									*  		    	*
			 \																																									 \
			  \																																									  \	
			   \-----------------------------------------------------------------------------------\
			   
			*--------------------------------------------aa-------------------------*
			   	*----------------------------------------bb-------------------------*-----dd-------*
			   																																			*-------ee--*
*/
				uint32_t IndiNum, i,j,k,New_deg,Indi_deg;
				st_VGDrawingParameter	VGParameter;
				
				static uint32_t IndicatorLoc[3][10]= {
																				 		{2,180-50,		 	180-25,20,		 180+25,20,		 360-2, 180-50,		0, 180-40},				//hour
																				 		{2,180-25,		 	180-15,20,		 180+15,20,		 360-1, 180-25,		0, 180-20},				//minute
																						{1,180-5,		 		180-10,20,		 180+10,20,		 360-1, 180-5,		0, 180}						//second
																						};
				static VGfloat SrampStops[35] = { 0.0f, 0.0f, 0.0f, 0.6f, 1.0f, 		
																					0.35f, 0.0f, 0.0f, 0.8f, 1.0f, 		
																					0.40f, 0.2f, 0.2f, 0.9f, 1.0f, 	
																					0.5f, 0.0f, 0.0f, 1.0f, 1.0f,	
																					0.6f, 0.0f, 0.0f, 0.7f, 1.0f, 	
																					0.65f, 0.1f, 0.1f, 0.3f, 1.0f,	
																					1.0f, 0.1f, 0.1f, 0.2f, 1.0f };
				
 				i=0;		//Hour
						IndicatorLoc[i][0] = MaxLength-60;							//aa
						IndicatorLoc[i][1] = MaxLength-70;							//bb
						IndicatorLoc[i][2] = 5;													//cc
						IndicatorLoc[i][3] = 25;												//dd
						IndicatorLoc[i][4] = 30;												//ee
						IndicatorLoc[i][5] = 10;													//ff							
 				i=1;			//Minute
						IndicatorLoc[i][0] = MaxLength-25;							//aa
						IndicatorLoc[i][1] = MaxLength-35;							//bb
						IndicatorLoc[i][2] = 3;													//cc
						IndicatorLoc[i][3] = 15;												//dd
						IndicatorLoc[i][4] = 20;												//ee
						IndicatorLoc[i][5] = 5;													//ff							
 				i=2;			//Second
						IndicatorLoc[i][0] = MaxLength-2;								//aa
						IndicatorLoc[i][1] = MaxLength-5;							 //bb
						IndicatorLoc[i][2] = 1;													//cc
						IndicatorLoc[i][3] = 30;							//dd
						IndicatorLoc[i][4] = 35;							//ee
						IndicatorLoc[i][5] = 3;													//ff	 
 
				IndiNum=0; 
				
#if 1		//for ADC input 
 							Indi_deg = ((speed* ExtentDeg)/220 ) + StartDeg;
 							
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif

#if 0		//for Engine meter
 							Indi_deg=EngineRPM+240-1;
#endif

#if 0 
 							Indi_deg = ((speed* 270)/220 ) +180+45;
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif
 
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]);															//aa		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][1]);															//bb
 							New_deg = CaculateIncDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[IndiNum][2]);						//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateIncDeg(Indi_deg, 180);																								
							CaculateCoordination(	New_deg, IndicatorLoc[IndiNum][3]);																//dd
 							New_deg = CaculateDecDeg(New_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[IndiNum][5]); 					//ff
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
 							New_deg = CaculateIncDeg(Indi_deg, 180);
 							CaculateCoordination(	New_deg, IndicatorLoc[IndiNum][4]);																//ee
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
							k+=1; 

 							New_deg = CaculateIncDeg(Indi_deg, 180);																								
							CaculateCoordination(	New_deg, IndicatorLoc[IndiNum][3]);																//dd
 							New_deg = CaculateIncDeg(New_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[IndiNum][5]); 					//ff
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		 					
 
 							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][1]);															//bb
 							New_deg = CaculateDecDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[IndiNum][2]);						//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
		/*-------------Calculate	GradientStartEnd			------*/
	 
	#if 1
 							if(Indi_deg<180)
 							{							
 							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]/6);															 
 							New_deg = CaculateIncDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, 20 );									 
  
								VGParameter.GradientStartEnd[2] = CoordXSrc;
								VGParameter.GradientStartEnd[3] = CoordYSrc;
 
  							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]/6);															 
 							New_deg = CaculateDecDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, 20 );							
 
								VGParameter.GradientStartEnd[0] = CoordXSrc;
								VGParameter.GradientStartEnd[1] = CoordYSrc;
							}
							else
 							{							
 							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]/6);															 
 							New_deg = CaculateIncDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, 20 );									 
  
								VGParameter.GradientStartEnd[0] = CoordXSrc;
								VGParameter.GradientStartEnd[1] = CoordYSrc;
 
  							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]/6);															 
 							New_deg = CaculateDecDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, 20 );							
 
								VGParameter.GradientStartEnd[2] = CoordXSrc;
								VGParameter.GradientStartEnd[3] = CoordYSrc;
							}					  
  
	#else
 							if(Indi_deg<180)
								{
							VGParameter.GradientStartEnd[0] = VGParameter.PathDrawingData[4];
							VGParameter.GradientStartEnd[1] = VGParameter.PathDrawingData[5];	
							VGParameter.GradientStartEnd[2] = VGParameter.PathDrawingData[8];
							VGParameter.GradientStartEnd[3] = VGParameter.PathDrawingData[9];	
							}
							else
								{
							VGParameter.GradientStartEnd[0] = VGParameter.PathDrawingData[8];
							VGParameter.GradientStartEnd[1] = VGParameter.PathDrawingData[9];	
							VGParameter.GradientStartEnd[2] = VGParameter.PathDrawingData[4];
							VGParameter.GradientStartEnd[3] = VGParameter.PathDrawingData[5];								
							}
						
	#endif	
	
/*
  VG_PAINT_TYPE_COLOR                         = 0x1B00,
  VG_PAINT_TYPE_LINEAR_GRADIENT               = 0x1B01,
  VG_PAINT_TYPE_RADIAL_GRADIENT               = 0x1B02,				
*/
 
 							VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
							for(i=0;i<35;i++)
							VGParameter.GradientStep[i]=SrampStops[i]; 
							VGParameter.GradientPoints = 35;		
							GL_DrawingPolyconFill( &VGParameter);	
}



int RETW_GUI_Meter_SpeedLimiter( uint32_t MaxLength, uint32_t LimtedValue)
{
				int i;
				VGuint Limiter_deg;				
				VGuint New_deg;
//				st_VGDrawArcParameter	ArcInfo;
 
				Limiter_deg = ((LimtedValue * 270)/240 ) +180+45;
 							
 				//Suppose Limiter_deg is 225 to 360+135 deg
 
				for(i=225 ;i< Limiter_deg; i++)
				{
  					if(  (i%6) == 0)
  					{
  						if(i>359)
  						New_deg = i-360;
  						else
  						New_deg = i;
  						
   						CaculateCoordination(New_deg,	MaxLength-100 );
  						CoordXDest = CoordXSrc;
  						CoordYDest = CoordYSrc; 				
  						CaculateCoordination(New_deg,	MaxLength-110 );
							GL_DrawingStroke( 8 , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap     				
  						

  					}
 			  }
 			  
 				G_CharScale = 1.0;
				G_CharShearX=0.0;
				G_CharShearY=0.0;	
				 			  
  						RETW_GUI_DrawScriptNumber(	ClockCenterX, ClockCenterY+120, LimtedValue, 18,0xA000A0ff, 1);
  						
 #if 0
 				ArcInfo.CenterX=200;
 				ArcInfo.CenterY=240;
 				ArcInfo.Width=100;
 				ArcInfo.Height=100;
  			ArcInfo.StartAngle= -45;
 				ArcInfo.AngleExtent= 270;
 				ArcInfo.ArcWidth=20;
 	 			ArcInfo.ArcColor=0x808000ff;	


 					ArcInfo.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 													
					static VGfloat SrampStops1[15] = { 	0.0f, 0.6f, 0.0f, 0.0f, 1.0f, 		
																							0.4f, 0.0f, 0.6f, 0.0f, 1.0f, 		
																							1.00f, 0.0f, 0.0f, 0.6f, 1.0f, 																							
																							};									
					for(i=0;i<15;i++)
					ArcInfo.GradientStep[i]=SrampStops1[i]; 
					ArcInfo.GradientPoints = 15;
					
					ArcInfo.GradientStartEnd[0] = ArcInfo.CenterX - ArcInfo.Width/2;
					ArcInfo.GradientStartEnd[1] = ArcInfo.CenterY;
					ArcInfo.GradientStartEnd[2] = ArcInfo.CenterX + ArcInfo.Width/2;;
					ArcInfo.GradientStartEnd[3] = ArcInfo.CenterY;

 //					ArcInfo.FillIt = 0;
 //					ArcInfo.StrokeIt = 1; 	
 					 
 			  	GL_DrawingArcPara(&ArcInfo);
 #endif
}




int GUI_DrawCapStyleIndicator(VGuint Indicator_deg, VGuint IndiLength, VGfloat width, VGuint color) 
{
 					if(Indicator_deg<90)
 						{
 							CoordXSrc = ClockCenterX + (SINtable[Indicator_deg]*IndiLength)/10000;
 							CoordYSrc = ClockCenterY + (COStable[Indicator_deg]*IndiLength)/10000;
 						}
 					else if(Indicator_deg<180)
 						{
 							CoordXSrc = ClockCenterX + (COStable[Indicator_deg-90]*IndiLength)/10000;
 							CoordYSrc = ClockCenterY - (SINtable[Indicator_deg-90]*IndiLength)/10000;
 						} 
 					else if(Indicator_deg<270)
 						{
 							CoordXSrc = ClockCenterX - (SINtable[Indicator_deg-180]*IndiLength)/10000;
 							CoordYSrc = ClockCenterY - (COStable[Indicator_deg-180]*IndiLength)/10000;
 						}  
 					else 
 						{
 							CoordXSrc = ClockCenterX - (COStable[Indicator_deg-270]*IndiLength)/10000;
 							CoordYSrc = ClockCenterY + (SINtable[Indicator_deg-270]*IndiLength)/10000;
 						}  
 						
 
     				Stroke_CapStylePara(ClockCenterX, ClockCenterY, CoordXSrc, CoordYSrc, 1, color);									// Indicator by Draw Line with Cap
						TrVg_DrawingCirclePara( CoordXSrc, CoordYSrc, 10,10, 0x0000FFff);
}



int GUI_DrawPolyconStyleIndicatorSecond(VGuint Indicator_deg, VGuint IndiLength, VGfloat width, VGuint color) 
{
				VGuint New_deg;
				
								VGuint XX;
								VGuint YY;								
								
 				int i;
 
  			CaculateCoordination(Indicator_deg,	IndiLength );
				XX = Polycon_DataTab[0] = CoordXSrc;
				YY = Polycon_DataTab[1] = CoordYSrc;
				
 					if(Indicator_deg<90)
 							{
 							New_deg = Indicator_deg+360;
 							New_deg = New_deg-90;
 							}
 					else
 							New_deg = Indicator_deg-90;
 							
  			CaculateCoordination(New_deg,	IndiLength/20 );
 				Polycon_DataTab[2] = CoordXSrc;
				Polycon_DataTab[3] = CoordYSrc;

 					if(Indicator_deg<180)
 							{
 							New_deg = Indicator_deg+360;
 							New_deg = New_deg-180;
 							}
 					else
 							New_deg =Indicator_deg-180;
  			CaculateCoordination(New_deg,	IndiLength/20 );
 				Polycon_DataTab[4] = CoordXSrc;
				Polycon_DataTab[5] = CoordYSrc;

 							New_deg = Indicator_deg+90;
 					if(New_deg > 359)
 					New_deg -=360;
  			CaculateCoordination(New_deg,	IndiLength/20 );
 				Polycon_DataTab[6] = CoordXSrc;
				Polycon_DataTab[7] = CoordYSrc;

				Polycon_DrawPointNum = 4;				
				
     		TrVg_DrawingPolyconFillPara(0x0000FFff);									// Indicator by Draw Line with Cap		
 
					Polycon_linearGradient[0] = ClockCenterX-164;
					Polycon_linearGradient[1] = ClockCenterY+164;
					Polycon_linearGradient[2] = ClockCenterX+164;
					Polycon_linearGradient[3] = ClockCenterY-164 ;					
					VGfloat SrampStops[15] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		0.3f, 0.5f, 0.5f, 0.5f, 1.0f, 		1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
					for(i=0;i<15;i++)
					Polycon_RampStopTab[i]=SrampStops[i];
					vgSetf( VG_STROKE_LINE_WIDTH, 1.0f ) ;
 		TrVg_DrawingCirclePara( XX, YY, 10,10, 0x0000FFff);

}

int GUI_DrawPolyconStyleIndicatorMin(VGuint Indicator_deg, VGuint IndiLength, VGfloat width, VGuint color) 
{
				VGuint New_deg;
				int i,j;
				
				Polycon_DrawPointNum=0;
				i=Polycon_DrawPointNum*2;
 
  			CaculateCoordination(Indicator_deg,	IndiLength);
				Polycon_DataTab[i]  = CoordXSrc; 
				Polycon_DataTab[i+1] = CoordYSrc;
				Polycon_DrawPointNum+=1;
				i=Polycon_DrawPointNum*2;
								
				New_deg = CaculateDecDeg(Indicator_deg, 3);
  			CaculateCoordination(New_deg,	IndiLength-(IndiLength/7) );
				Polycon_DataTab[i]  = CoordXSrc; 
				Polycon_DataTab[i+1] = CoordYSrc;
				Polycon_DrawPointNum+=1;
				i=Polycon_DrawPointNum*2;
								
				New_deg = CaculateDecDeg(Indicator_deg, 1);
  			CaculateCoordination(New_deg,	IndiLength-(IndiLength/10) );
				Polycon_DataTab[i]  = CoordXSrc; 
				Polycon_DataTab[i+1] = CoordYSrc;
				Polycon_DrawPointNum+=1;
				i=Polycon_DrawPointNum*2;
				
				New_deg = CaculateIncDeg(Indicator_deg, 180+10);
  			CaculateCoordination(New_deg,	 IndiLength/5 );
				Polycon_DataTab[i]  = CoordXSrc; 
				Polycon_DataTab[i+1] = CoordYSrc;
				Polycon_DrawPointNum+=1;
				i=Polycon_DrawPointNum*2;
				
				New_deg = CaculateIncDeg(Indicator_deg, 180);
  			CaculateCoordination(New_deg,	 (IndiLength/5) +10);
				Polycon_DataTab[i]  = CoordXSrc; 
				Polycon_DataTab[i+1] = CoordYSrc;
				Polycon_DrawPointNum+=1;
				i=Polycon_DrawPointNum*2;
									
				New_deg = CaculateIncDeg(Indicator_deg, 180-10);
  			CaculateCoordination(New_deg,	 IndiLength/5 );
				Polycon_DataTab[i]  = CoordXSrc; 
				Polycon_DataTab[i+1] = CoordYSrc;
				Polycon_DrawPointNum+=1;
				i=Polycon_DrawPointNum*2;
				
				New_deg = CaculateIncDeg(Indicator_deg, 1);
  			CaculateCoordination(New_deg,	IndiLength-(IndiLength/10) );
				Polycon_DataTab[i]  = CoordXSrc; 
				Polycon_DataTab[i+1] = CoordYSrc;
				Polycon_DrawPointNum+=1;
				i=Polycon_DrawPointNum*2;
				
				New_deg = CaculateIncDeg(Indicator_deg, 3);
  			CaculateCoordination(New_deg,	IndiLength-(IndiLength/7) );
				Polycon_DataTab[i]  = CoordXSrc; 
				Polycon_DataTab[i+1] = CoordYSrc;
				Polycon_DrawPointNum+=1;
				i=Polycon_DrawPointNum*2;
				
				 
					Polycon_linearGradient[0] = ClockCenterX-164;
					Polycon_linearGradient[1] = ClockCenterY+164;
					Polycon_linearGradient[2] = ClockCenterX+164;
					Polycon_linearGradient[3] = ClockCenterY-164 ;					
					VGfloat TrampStops[15] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 		0.5f, 0.0f, 0.0f, 0.5f, 1.0f, 		1.0f, 0.0f, 0.0f, 0.3f, 1.0f };
					for(i=0;i<15;i++)
					Polycon_RampStopTab[i]=TrampStops[i];
 
     			TrVg_DrawingPolyconFillPara(color);

//					vgSetf( VG_STROKE_LINE_WIDTH, 12.0f ) ; 			
#if 0				//Draw outline 
     			for(i=0; i<(Polycon_DrawPointNum-1);i++)
     			{
     			j=i*2;
      			Stroke_CapStylePara((int)Polycon_DataTab[j], (int)Polycon_DataTab[j+1],(int)Polycon_DataTab[j+2], (int)Polycon_DataTab[j+3],6, 0x000000ff);
//      			Stroke_JoinStylePara((int)Polycon_DataTab[j], (int)Polycon_DataTab[j+1],(int)Polycon_DataTab[j+2], (int)Polycon_DataTab[j+3],2, 0xFFFFFFff);      			
//     			     			Stroke_MiterLimitPara((int)Polycon_DataTab[j], (int)Polycon_DataTab[j+1],(int)Polycon_DataTab[j+2], (int)Polycon_DataTab[j+3],2, 0xFFFFFFff);
     			}
    			Stroke_CapStylePara((int)Polycon_DataTab[0],(int)Polycon_DataTab[1],(int)Polycon_DataTab[j+2], (int)Polycon_DataTab[j+3],6, 0x000000ff);    
 //    			Stroke_JoinStylePara((int)Polycon_DataTab[0],(int)Polycon_DataTab[1],(int)Polycon_DataTab[j+2], (int)Polycon_DataTab[j+3],2, 0xFFFFFFff);       			
 //    								Stroke_MiterLimitPara((int)Polycon_DataTab[0],(int)Polycon_DataTab[1],(int)Polycon_DataTab[j+2], (int)Polycon_DataTab[j+3],2, 0xFFFFFFff); 		
#endif			
 		 		
}
int GUI_DrawPolyconStyleIndicatorHour(VGuint Indicator_deg, VGuint IndiLength, VGfloat width, VGuint color) 
{
				VGuint New_deg;
				int i;
				
//				printf("Draw Hour indicator, deg =%d\n",Indicator_deg);
  			CaculateCoordination(Indicator_deg,	IndiLength );
				Polycon_DataTab[0] = CoordXSrc;
				Polycon_DataTab[1] = CoordYSrc;
				
 					if(Indicator_deg<90)
 							{
 							New_deg = Indicator_deg+360;
 							New_deg = New_deg-90;
 							}
 					else
 							New_deg = Indicator_deg-90;
  			CaculateCoordination(New_deg,	IndiLength/20 );
 				Polycon_DataTab[2] = CoordXSrc;
				Polycon_DataTab[3] = CoordYSrc;

 					if(Indicator_deg<180)
 							{
 							New_deg = Indicator_deg+360;
 							New_deg = New_deg-180;
 							}
 					else
 							New_deg =Indicator_deg-180;
  			CaculateCoordination(New_deg,	IndiLength/20 );
 				Polycon_DataTab[4] = CoordXSrc;
				Polycon_DataTab[5] = CoordYSrc;

 							New_deg = Indicator_deg+90;
 					if(New_deg > 359)
 					New_deg -=360;
  			CaculateCoordination(New_deg,	IndiLength/20 );
 				Polycon_DataTab[6] = CoordXSrc;
				Polycon_DataTab[7] = CoordYSrc;

				Polycon_DrawPointNum = 4;				

					Polycon_linearGradient[0] = ClockCenterX-164;
					Polycon_linearGradient[1] = ClockCenterY+164;
					Polycon_linearGradient[2] = ClockCenterX+164;
					Polycon_linearGradient[3] = ClockCenterY-164 ;					
					VGfloat TrampStops[15] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 		0.5f, 0.0f, 0.0f, 0.5f, 1.0f, 		1.0f, 0.0f, 1.0f, 0.0f, 1.0f };
					for(i=0;i<15;i++)
					Polycon_RampStopTab[i]=TrampStops[i];
					vgSetf( VG_STROKE_LINE_WIDTH, 1.0f ) ;
					
     		TrVg_DrawingPolyconFillPara(color);									// Indicator by Draw Line with Cap		
}

int TrVg_DrawingSpeedBlockBar()
{
				int i,j;
				VGuint New_deg;
				VGuint TiltDeg=30;
				VGuint CircleDiaInner = 200;
				VGuint CircleDiaOuter = 300;		
#if 0
				for(i=0;i<60;i++)
				{
 
				New_deg = i*6;
				
				Polycon_DrawPointNum=0;
				j=Polycon_DrawPointNum*2;
  			CaculateCoordination(New_deg,	CircleDiaInner );
				Polycon_DataTab[j]  = CoordXSrc; 
				Polycon_DataTab[j+1] = CoordYSrc;
				
 				Polycon_DrawPointNum+=1;
				j=Polycon_DrawPointNum*2;
  			CaculateCoordination(New_deg,	CircleDiaInner+8 );  		
				Polycon_DataTab[j]  = CoordXSrc; 
				Polycon_DataTab[j+1] = CoordYSrc;

				New_deg = (i+1)*6;
				if(New_deg==360)
				New_deg=0;
				
 				Polycon_DrawPointNum+=1;
				j=Polycon_DrawPointNum*2;
  			CaculateCoordination(New_deg,	CircleDiaInner+8 );
				Polycon_DataTab[j]  = CoordXSrc; 
				Polycon_DataTab[j+1] = CoordYSrc;
				
 				Polycon_DrawPointNum+=1;
				j=Polycon_DrawPointNum*2;
  			CaculateCoordination(New_deg,	CircleDiaInner );  		
				Polycon_DataTab[j]  = CoordXSrc; 
				Polycon_DataTab[j+1] = CoordYSrc;
 		
  			}
				
				TrVg_DrawingPolyconFillPara(0xFF00FFff);
				
#endif

#if 0
				for(i=0;i<60;i++)
				{
 
				New_deg = i*6;
				
				Polycon_DrawPointNum=0;
				j=Polycon_DrawPointNum*2;
  			CaculateCoordination(New_deg,	CircleDiaInner );
				Polycon_DataTab[j]  = CoordXSrc; 
				Polycon_DataTab[j+1] = CoordYSrc;
				
 				Polycon_DrawPointNum+=1;
				j=Polycon_DrawPointNum*2;
  			CaculateCoordination(New_deg,	CircleDiaInner+8 );  		
				Polycon_DataTab[j]  = CoordXSrc; 
				Polycon_DataTab[j+1] = CoordYSrc;

				New_deg = (i+1)*6;
				if(New_deg==360)
				New_deg=0;
				
 				Polycon_DrawPointNum+=1;
				j=Polycon_DrawPointNum*2;
  			CaculateCoordination(New_deg,	CircleDiaInner+8 );
				Polycon_DataTab[j]  = CoordXSrc; 
				Polycon_DataTab[j+1] = CoordYSrc;
				
 				Polycon_DrawPointNum+=1;
				j=Polycon_DrawPointNum*2;
  			CaculateCoordination(New_deg,	CircleDiaInner );  		
				Polycon_DataTab[j]  = CoordXSrc; 
				Polycon_DataTab[j+1] = CoordYSrc;
 		
  			}
				
				TrVg_DrawingPolyconFillPara(0x00FF00ff);
				
#endif

}
