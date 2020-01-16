
#include    "retwhmi.h"
 
		//												percent,B,G,R,alpha								percent,B,G,R,alpha								percent,B,G,R,alpha


#define 		MSStep1			(60)						//50ms
#define 		MSStep2 		(500)						//500ms
#define 		MSStep3			(500)				
#define 		MSStep4			(500)
#define 		MSStep5			(200)

#define 		MotionUpto1			(MSStep1)
#define 		MotionUpto2 		(MSStep2	+MotionUpto1)
#define 		MotionUpto3			(MSStep3	+MotionUpto2)
#define 		MotionUpto4			(MSStep4	+MotionUpto3)
#define 		MotionUpto5			(MSStep5	+MotionUpto4)

#define 		MotionStep1			(3)						//50ms
#define 		MotionStep2 		(60)						//500ms
#define 		MotionStep3			(60)				
#define 		MotionStep4			(60)
#define 		MotionStep5			(4)

static 	uint32_t 		T_MSStep1;
static	uint32_t 		T_MSStep2;
static	uint32_t 		T_MSStep3;
static	uint32_t 		T_MSStep4;
static	uint32_t 		T_MSStep5;

static uint32_t MainIndicatorDeg = 270*8;

static uint32_t LastSize;
				
/*			|			+			|			+			|	+		|									     Center							   |	+	 |			+			|			+			|
				|-----------------------------------------------------O1Outer-------------------------------------------------|
				|--O1Width--|
							|-----------------------------------------------O1Size--------------------------------------------|
				
										|-----------------------------------------O2Outer-------------------------------------|
										|--O2Width--|
													|-----------------------------------O2Size--------------------------------|

																|-----------------------------O3Outer------------------------|
																|-----|O3Width
 																   |---------------------------O3Size--------------------|
																      |---------- O3InnerR------|
																
																
																			|----------------------O4Outer------------------|
																			|-----| O4Width
																			   |--------------------O4Size---------------|
																				    |---- O4InnerR------|
																					

																						|---------------O5Outer--------------|
																						|---------O5InnerR---|
*/

#if 1
int RETW_GUI_DEMO_0( int speed, int Maxspeed )
{	
 
   	if(Milisec_ModeEntry < MotionUpto1)
   	{
			RETW_GUI_PoliconFillScreen(0,0,800,480,0x200000ff);	
//			printf("MS1 %d\n", Milisec_ModeEntry);
		}
		
		else if(Milisec_ModeEntry < MotionUpto2)																			//get bigger
			{
 			ClockCenterX=400;
 			ClockCenterY=240;
			RETW_GUI_PoliconFillScreen(0,0,800,480,0x200000ff);	
			if(  (470 *(Milisec_ModeEntry-MotionUpto1)/MSStep2)  > 100)
			RETW_GUI_DEMO_0_Prepare(  speed,  Maxspeed , 470 *(Milisec_ModeEntry-MotionUpto1)/MSStep2 );
			
// 			printf("MS2 %d\n", Milisec_ModeEntry);
			}
			
		else if(Milisec_ModeEntry < MotionUpto3)																			//move down
 		{
 			ClockCenterY=240-(Milisec_ModeEntry-MotionUpto2)*240/MSStep3; 			
 			
			RETW_GUI_PoliconFillScreen(0,0,800,480,0x200000ff);	
			RETW_GUI_DEMO_0_Prepare(  speed,  Maxspeed , 470 );
//			printf("MS3 %d\n", Milisec_ModeEntry);
 
		}	
		else if(Milisec_ModeEntry < MotionUpto4)																			//enlarge
 		{
 			ClockCenterX=400;
 			ClockCenterY=0; 			
			RETW_GUI_PoliconFillScreen(0,0,800,480,0x200000ff);	

 			LastSize = 470+(Milisec_ModeEntry-MotionUpto3)*330/MSStep4;
			RETW_GUI_DEMO_0_Prepare(  speed,  Maxspeed , LastSize );
  		RETW_GUI_DEMO_0_Main(  speed,  Maxspeed, LastSize);	
  		
// 		printf("MS4 %d\n", Milisec_ModeEntry);
	  }
		else if(Milisec_ModeEntry < MotionUpto5)
 		{
			RETW_GUI_DEMO_0_Prepare(  speed,  Maxspeed , LastSize );
//  		RETW_GUI_DEMO_0_Main(  speed,  Maxspeed, LastSize);	
//		printf("MS5 %d\n", Milisec_ModeEntry);

	  }	  
	  else
	  {
  		RETW_GUI_DEMO_0_Clean();
  		RETW_GUI_DEMO_0_Main(  speed,  Maxspeed, LastSize);	  
//			TrVg_DrawingChineseTextString( );

 			TrVg_DrawingPicture(160,160); 			
  		RETW_GUI_Meter_ClockIndicator(630+80,480-10-80);															//display indicator of clock

 			ClockCenterX=400;
 			ClockCenterY=0; 
 			
 						
//  		printf("MS6 %d\n", Milisec_ModeEntry);
  	}				
}
#endif

#if 0
int RETW_GUI_DEMO_0( int speed, int Maxspeed )
{	
	
		uint32_t MiliCnt;
		MiliCnt=Milisec_ModeEntry;
 
		
   	if(ModeEntryCnt < MotionStep1)
   	{
			RETW_GUI_PoliconFillScreen(0,0,800,480,0x200000ff);	
		}
		else if( ModeEntryCnt< (MotionStep1+MotionStep2))
			{
 			ClockCenterX=400;
 			ClockCenterY=240;
			if(ModeEntryCnt>12)
			RETW_GUI_DEMO_0_Prepare(  speed,  Maxspeed , (ModeEntryCnt-MotionStep1)*470/MotionStep2 );
			}
		else if( ModeEntryCnt< (MotionStep1+MotionStep2+MotionStep3) )
 		{
 			ClockCenterY=240-(ModeEntryCnt-MotionStep1-MotionStep2)*240/MotionStep3; 			
 			
			RETW_GUI_PoliconFillScreen(0,0,800,480,0x200000ff);	
			RETW_GUI_DEMO_0_Prepare(  speed,  Maxspeed , 470 );
		}	
		else if( ModeEntryCnt< (MotionStep1+MotionStep2+MotionStep3+MotionStep4) )
 		{
 			ClockCenterX=400;
 			ClockCenterY=0; 			
			RETW_GUI_PoliconFillScreen(0,0,800,480,0x200000ff);	
			RETW_GUI_DEMO_0_Prepare(  speed,  Maxspeed , 470+(ModeEntryCnt-MotionStep1-MotionStep2-MotionStep3)*230/MotionStep4 );
  		RETW_GUI_DEMO_0_Main(  speed,  Maxspeed, 470+(ModeEntryCnt-MotionStep1-MotionStep2-MotionStep3)*230/MotionStep4);	
	  }
	  else
	  {
  		RETW_GUI_DEMO_0_Clean();
  		RETW_GUI_DEMO_0_Main(  speed,  Maxspeed, 700);	  
  	}			
}
#endif



#if 0
int RETW_GUI_DEMO_0( int speed, int Maxspeed )
{	
   if(ModeEntryCnt<3)
 		{
 			ClockCenterX=400;
 			ClockCenterY=240;
   		RETW_GUI_PoliconFillScreen(0,0,800,480,0x200000ff);	
 			RETW_GUI_DEMO_0_Prepare(  speed,  Maxspeed );
		}
 		else
 		{
  		RETW_GUI_DEMO_0_Clean();
 // vgFinish();
  		RETW_GUI_DEMO_0_Main(  speed,  Maxspeed );	
	  }
}
#endif



void RETW_GUI_DEMO_0_Clean()
{
   				RETW_GUI_PoliconFillScreen(100-50,440-50,100,90,0x200000ff);	
   				RETW_GUI_PoliconFillScreen(700-50,440-50,100,90,0x200000ff);		
   				RETW_GUI_PoliconFillScreen(0,0,300,16,0x200000ff);	
}

int RETW_GUI_DEMO_0_Prepare( int speed, int Maxspeed, uint32_t O1 )
{
				uint32_t i,j,New_deg;
				uint32_t LimiterSpeed;
 
				st_VGDrawingParameter	VGParameter;
				st_VGDrawArcParameter	ArcInfo; 	

				  uint32_t O1Outer = O1;
				static uint32_t O1Width = 8;
				static float O1Size;

				static uint32_t O2Outer;
				static uint32_t O2Width = 10;
				static float O2Size;

				static uint32_t O3Outer;
				static uint32_t O3Width = 20;
				static uint32_t O3Size;
				static uint32_t O3InnerR;

				static uint32_t O4Outer;
				static uint32_t O4Width= 90;
				static uint32_t O4Size;
				static uint32_t O4InnerR;

				static uint32_t O5Outer;
				static uint32_t O5InnerR;
				
				
 				O1Size = O1Outer-O1Width;
 				
 				O2Outer = O1Outer-O1Width*2;
				O2Size = O2Outer-O2Width;
				
				O3Outer = O2Outer-O2Width*2;
				O3Size = O3Outer-O3Width;	
				O3InnerR = (O3Outer-O3Width*2)/2;

				O4Outer = O3Outer-O3Width*2;			
				O4Size = 	O4Outer-O4Width;
				O4InnerR = (O4Outer-O4Width*2)/2;
				
				O5Outer = O4Outer-O4Width*2;
				O5InnerR = (O4Outer-O4Width*2)/2;
			
				float CharSize = 1.0;
 
				
				LimiterSpeed = R_ADC_Read()* 220/1024;
				
#if 0
	 			ClockCenterX = 650;
	 			ClockCenterY = 240;
  				RETW_GUI_DEMO_Speed_0_Right(speed,Maxspeed);
  				
	 			ClockCenterX = 150;
	 			ClockCenterY = 240;
				RETW_GUI_DEMO_Speed_0_Left(speed,Maxspeed);

#endif
//				vgFinish();
 
#if 1					//most inner surface

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O3Outer;
					VGParameter.PathDrawingData[3]= O3Outer;		
								
					static VGfloat SrampStops1[30] = { 	0.0f, 0.4f, 0.6f, 0.3f, 1.0f, 		
																							0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 		
																							0.9f, 0.0f, 0.0f, 0.0f, 1.0f, 
	  		
																					//	0.85f, 0.1f, 0.0f, 0.0f, 1.0f, 
																							0.94f, 0.0f, 0.0f, 0.0f, 1.0f, 
																							0.97f, 0.3f, 0.0f, 0.0f, 1.0f, 	
																							1.00f, 0.6f, 0.0f, 0.0f, 1.0f, 																							
																							};				
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 30;
 				 
					VGParameter.GradientStartEnd[0] = ClockCenterX;
					VGParameter.GradientStartEnd[1] = ClockCenterY;
					VGParameter.GradientStartEnd[2] = ClockCenterX;
					VGParameter.GradientStartEnd[3] = ClockCenterY;
					VGParameter.GradientStartEnd[4] = O3Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
//				  vgFinish();
#endif


			#if 1 			//second inner

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O2Size;
					VGParameter.PathDrawingData[3]= O2Size;	
									
					static VGfloat SrampStops2[30] = { 	0.0f, 0.0f, 0.0f, 0.0f, 1.0f,			
																							0.2f, 0.3f, 0.3f, 0.3f, 1.0f,  		
																							0.4f, 0.7f, 0.7f, 0.7f, 1.0f, 
																							0.6f, 0.1f, 0.1f, 0.1f, 1.0f,		
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
//				 	vgFinish();		 
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
//				 vgFinish();
 			#endif

#if 1				//Grid
				for(i=0;i<121;i++)
				{
//			New_deg = 180+20+ i*320/240;
				New_deg = 180+90 + i*(180)/120;

  			CaculateCoordination(New_deg,	(O3Outer/2) );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
  			
  #if 1			
  			if(  (i%10) == 0)
  			{
  				CaculateCoordination(New_deg,		O4Size/2 );
 
  				G_CharScale = 1.0 * O1Outer/500;
					G_CharShearX=0;
					G_CharShearY=0;
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, i, 18,0xA0A0A0ff, 1);
//  				vgFinish();
  			}
	#endif
	
	#if 1
  			if(  i == 0  | i == 120)
  					{
  					CaculateCoordination(New_deg,	(O3Outer/2)-O3Width );
						GL_DrawingStroke(6  , 0x0000C0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
  			else if(  (i%10) == 0)
  					{
  					CaculateCoordination(New_deg,	(O3Outer/2)-O3Width  );
						GL_DrawingStroke(4  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
 				else if(  (i%5) == 0)
  					{
   					CaculateCoordination(New_deg,	(O3Outer/2)-15 );
 						GL_DrawingStroke( 2  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
 						}

     				{
//     				CaculateCoordination(New_deg,	O3InnerR-12 );
//						GL_DrawingStroke( 1  , 0x808080ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}   	
	#endif
 				}	
#endif


}

int RETW_GUI_DEMO_0_Main( int speed, int Maxspeed, uint32_t O1 )
{
				uint32_t i,j,New_deg;
				uint32_t LimiterSpeed;
 
				uint32_t O1Outer = O1;
				static uint32_t O1Width = 8;
				static float O1Size;

				static uint32_t O2Outer;
				static uint32_t O2Width = 10;
				static float O2Size;

				static uint32_t O3Outer;
				static uint32_t O3Width = 20;
				static uint32_t O3Size;
				static uint32_t O3InnerR;

				static uint32_t O4Outer;
				static uint32_t O4Width= 90;
				static uint32_t O4Size;
				static uint32_t O4InnerR;

				static uint32_t O5Outer;
				static uint32_t O5InnerR;
				
				float CharSize= O1Outer/500;
 
				st_VGDrawingParameter	VGParameter;
				st_VGDrawArcParameter	ArcInfo; 	

 
 				O1Size = O1Outer-O1Width;
 				
 				O2Outer = O1Outer-O1Width*2;
				O2Size = O2Outer-O2Width;
				
				O3Outer = O2Outer-O2Width*2;
				O3Size = O3Outer-O3Width;	
				O3InnerR = (O3Outer-O3Width*2)/2;

				O4Outer = O3Outer-O3Width*2; 
				O4Size = 	O4Outer-O4Width;
				O4InnerR = (O4Outer-O4Width*2)/2;
				
				O5Outer = O4Outer-O4Width*2;
				O5InnerR = (O4Outer-O4Width*2)/2;
				
				LimiterSpeed = R_ADC_Read()* 220/1024;
#if 0
	 			ClockCenterX = 650;
	 			ClockCenterY = 240;
  				RETW_GUI_DEMO_Speed_0_Right(speed,Maxspeed);
  				
	 			ClockCenterX = 150;
	 			ClockCenterY = 240;
				RETW_GUI_DEMO_Speed_0_Left(speed,Maxspeed);
#endif
//				vgFinish();

#if 1					//Inner of O3,O2,O1

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O4Outer;
					VGParameter.PathDrawingData[3]= O4Outer;		
		#if 0			
					static VGfloat SrampStops1[30] = { 	0.0f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 		
																							0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 
	  		
																					//	0.85f, 0.1f, 0.0f, 0.0f, 1.0f, 
																							0.90f, 0.4f, 0.0f, 0.0f, 1.0f, 
																							0.95f, 0.5f, 0.0f, 0.0f, 1.0f, 	
																							1.00f, 0.6f, 0.0f, 0.0f, 1.0f, 																							
																							};				
		#endif																						
					static VGfloat SrampStops1[30] = { 	0.0f, 0.4f, 0.6f, 0.3f, 1.0f, 		
																							0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 		
																							0.7f, 0.5f, 0.5f, 0.5f, 1.0f, 
	  		
																					//	0.85f, 0.1f, 0.0f, 0.0f, 1.0f, 
																							0.80f, 0.0f, 0.2f, 0.0f, 1.0f, 
																							0.95f, 0.0f, 0.0f, 0.0f, 1.0f, 	
																							1.00f, 0.0f, 0.0f, 0.0f, 1.0f, 																							
																							};			
																							
					static VGfloat SrampStops1R[30] = { 	0.0f, 0.4f, 0.6f, 0.3f, 1.0f, 		
																							0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 		
																							0.7f, 0.5f, 0.5f, 0.5f, 1.0f, 
	  		
																					//	0.85f, 0.1f, 0.0f, 0.0f, 1.0f, 
																							0.80f, 0.0f, 0.2f, 0.0f, 1.0f, 
																							0.95f, 0.0f, 0.0f, 0.0f, 1.0f, 	
																							1.00f, 0.0f, 0.0f, 0.0f, 1.0f, 																							
																							};				
																							
			if(LimiterSpeed > speed)				//BLUE color
				{
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 30;
 				}
 			else														//RED color
				{
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops1R[i]; 
					VGParameter.GradientPoints = 30;	
 				} 												
					VGParameter.GradientStartEnd[0] = ClockCenterX;
					VGParameter.GradientStartEnd[1] = ClockCenterY;
					VGParameter.GradientStartEnd[2] = ClockCenterX;
					VGParameter.GradientStartEnd[3] = ClockCenterY;
					VGParameter.GradientStartEnd[4] = O4Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
//				  vgFinish();
#endif

			#if 0 			//second inner

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O2Size;
					VGParameter.PathDrawingData[3]= O2Size;	
									
					static VGfloat SrampStops2[30] = { 	0.0f, 0.0f, 0.0f, 0.0f, 1.0f,			
																							0.2f, 0.3f, 0.3f, 0.3f, 1.0f,  		
																							0.4f, 0.7f, 0.7f, 0.7f, 1.0f, 
																							0.6f, 0.1f, 0.1f, 0.1f, 1.0f,		
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
//				 	vgFinish();		 
			#endif
			
			#if 0				//outer

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
//				 vgFinish();
 			#endif
 			
 #if 1		//	DrawArc
 
 				ArcInfo.CenterX			=ClockCenterX;
 				ArcInfo.CenterY			=ClockCenterY;
 				ArcInfo.Width				=O5Outer+4;
 				ArcInfo.Height			=O5Outer+4;
  			ArcInfo.StartAngle	= 180-EngineRPM;
 				ArcInfo.AngleExtent	= EngineRPM;
 				ArcInfo.ArcWidth		= 7 ;
 	 			ArcInfo.ArcColor		=0x808000ff;	

 					ArcInfo.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 													
					static VGfloat SrampStops4[20] = { 	0.0f, 0.0f, 0.6f, 0.0f, 1.0f, 		
																							0.2f, 0.0f, 0.6f, 0.5f, 1.0f, 		
																							0.6f, 0.0f, 0.0f, 0.6f, 1.0f, 			
																							1.0f, 0.0f, 0.0f, 0.8f, 1.0f																			
																							};									
					for(i=0;i<20;i++)
					ArcInfo.GradientStep[i]=SrampStops4[i]; 
					ArcInfo.GradientPoints = 20;
					
					ArcInfo.GradientStartEnd[0] = ArcInfo.CenterX - ArcInfo.Width/2;
					ArcInfo.GradientStartEnd[1] = ArcInfo.CenterY;
					ArcInfo.GradientStartEnd[2] = ArcInfo.CenterX + ArcInfo.Width/2;;
					ArcInfo.GradientStartEnd[3] = ArcInfo.CenterY;
 //					ArcInfo.FillIt = 0;
 //					ArcInfo.StrokeIt = 1; 	
 			  	GL_DrawingArcPara(&ArcInfo);
 #endif
 
#if 1		//Number only
				for(i=0;i<121;i++)
				{
//			New_deg = 180+20+ i*320/240;
				New_deg = 180+90+ i*(180)/120;

  			CaculateCoordination(New_deg,	(O3Outer/2) );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
  #if 1			
  			if(  (i%10) == 0)
  			{
  				CaculateCoordination(New_deg,		O4Size/2 );
 
  				G_CharScale = 1.0 * O1Outer/500;
					G_CharShearX=0;
					G_CharShearY=0;
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, i, 18,0x0290F0ff, 1);
//  				vgFinish();
  			}
	#endif

	#if 0
  			if(  i == 0  | i == 120)
  					{
  					CaculateCoordination(New_deg,	(O3Outer/2)-O3Width );
						GL_DrawingStroke(6  , 0x0000C0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
  			else if(  (i%10) == 0)
  					{
  					CaculateCoordination(New_deg,	(O3Outer/2)-O3Width  );
						GL_DrawingStroke(4  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
 				else if(  (i%5) == 0)
  					{
   					CaculateCoordination(New_deg,	(O3Outer/2)-15 );
 						GL_DrawingStroke( 2  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
 						}

     				{
//     				CaculateCoordination(New_deg,	O3InnerR-12 );
//						GL_DrawingStroke( 1  , 0x808080ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}   	
	#endif
 				}	
#endif

/*
  VG_JOIN_MITER                               = 0x1800,
  VG_JOIN_ROUND                               = 0x1801,
  VG_JOIN_BEVEL                               = 0x1802,
*/
 
#if 1
 				RETW_GUI_DEMO0_RED_6p_Ind_LineOnly( (O4Outer/2)-2, (O5Outer/2), speed, 180+90, 180);
//				RETW_GUI_DEMO0_RED_6p_Ind( 192, speed, 180+45, 270);
// 				vgFinish();
#endif

#if 1		//Now for Speed Circle window

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O5Outer;
					VGParameter.PathDrawingData[3]= O5Outer;		
								
					static VGfloat SrampStops6[30] = { 	0.0f, 0.9f, 0.6f, 0.4f, 1.0f, 		
																							0.5f, 0.5f, 0.3f, 0.2f, 1.0f, 		
																							0.91f, 0.1f, 0.1f, 0.1f, 1.0f, 
																							0.95f, 0.5f, 0.5f, 0.5f, 1.0f, 
																							0.99f, 0.1f, 0.1f, 0.1f, 1.0f, 
																							1.00f, 0.1f, 0.1f, 0.1f, 1.0f, 																							
																							};				
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops6[i]; 
					VGParameter.GradientPoints = 30;
 
					VGParameter.GradientStartEnd[0] = ClockCenterX;
					VGParameter.GradientStartEnd[1] = ClockCenterY;
					VGParameter.GradientStartEnd[2] = ClockCenterX;
					VGParameter.GradientStartEnd[3] = ClockCenterY;
					VGParameter.GradientStartEnd[4] = O5Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
 				  vgFinish();
#endif

#if 1
 					G_CharScale = 3.0 * O1Outer/500;;
					G_CharShearX=0.3;
					G_CharShearY=0.0;
					RETW_GUI_DrawScriptNumber(ClockCenterX, ClockCenterY+O5InnerR-(O5InnerR/3)-20, speed, 24 , 0xA0C0C0ff, 1 );
#endif
	
					R_TXTR_DrawStringMain("km/h", 4, ClockCenterX-24*CharSize, ClockCenterY+O5InnerR-(O5InnerR/3)-40, 0xA0A0A0ff);
								
					RETW_GUI_DEMO0_MilesRecord(speed, ClockCenterX, ClockCenterY+(O5InnerR/4) );
				
 					G_CharScale = 0.8;
					G_CharShearX=0.3;
					G_CharShearY=0.0;	
										
					R_TXTR_DrawStringMain("KiloMeter", 9, ClockCenterX-50, ClockCenterY+5, 0x000000ff);
					
  				RETW_GUI_DEMO_Speed_0_FlashDIRSign();
  				
// 				RETW_GUI_DEMO_Speed_0_SpeedWindow(speed,400,340);
}

void RETW_GUI_DEMO_Speed_0_FlashDIRSign() 
{
				uint32_t i,j;
				uint32_t CenterX,CenterY;
				
				st_VGDrawingParameter	VGParameter;	
 
				if( rtc_halfsecond_flag_CNT == 1)
				{
//Right DIRECTION sign

					CenterX = 700;
					CenterY = 440;
					
							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
							
							VGParameter.PathDrawingData[j]  = CenterX+30; 
							VGParameter.PathDrawingData[j+1] = CenterY;
							VGParameter.PathPoints+=1;
							j+=2;			

							VGParameter.PathDrawingData[j]  = CenterX; 
							VGParameter.PathDrawingData[j+1] = CenterY+30;
							VGParameter.PathPoints+=1;
							j+=2; 
							
							VGParameter.PathDrawingData[j]  = CenterX; 
							VGParameter.PathDrawingData[j+1] = CenterY+15;
							VGParameter.PathPoints+=1;
							j+=2; 							
							
							VGParameter.PathDrawingData[j]  = CenterX-30; 
							VGParameter.PathDrawingData[j+1] = CenterY+15;
							VGParameter.PathPoints+=1;
							j+=2; 							

							VGParameter.PathDrawingData[j]  = CenterX-30; 
							VGParameter.PathDrawingData[j+1] = CenterY-15;
							VGParameter.PathPoints+=1;
							j+=2; 	

							VGParameter.PathDrawingData[j]  = CenterX; 
							VGParameter.PathDrawingData[j+1] = CenterY-15;
							VGParameter.PathPoints+=1;
							j+=2; 															
							
							VGParameter.PathDrawingData[j]  = CenterX; 
							VGParameter.PathDrawingData[j+1] = CenterY-30;
							VGParameter.PathPoints+=1;
							j+=2; 								
 
 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							VGParameter.FillColor = 0x10FF1000;
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);	
				}			
				else
				{
//Left DIRECTION sign

					CenterX = 100;
					CenterY = 440;
					
							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
							VGParameter.PathDrawingData[j]  = CenterX-30; 
							VGParameter.PathDrawingData[j+1] = CenterY;
							VGParameter.PathPoints+=1;
							j+=2;			

							VGParameter.PathDrawingData[j]  = CenterX; 
							VGParameter.PathDrawingData[j+1] = CenterY+30;
							VGParameter.PathPoints+=1;
							j+=2; 
							
							VGParameter.PathDrawingData[j]  = CenterX; 
							VGParameter.PathDrawingData[j+1] = CenterY+15;
							VGParameter.PathPoints+=1;
							j+=2; 							
							
							VGParameter.PathDrawingData[j]  = CenterX+30; 
							VGParameter.PathDrawingData[j+1] = CenterY+15;
							VGParameter.PathPoints+=1;
							j+=2; 							

							VGParameter.PathDrawingData[j]  = CenterX+30; 
							VGParameter.PathDrawingData[j+1] = CenterY-15;
							VGParameter.PathPoints+=1;
							j+=2; 	

							VGParameter.PathDrawingData[j]  = CenterX; 
							VGParameter.PathDrawingData[j+1] = CenterY-15;
							VGParameter.PathPoints+=1;
							j+=2; 															
							
							VGParameter.PathDrawingData[j]  = CenterX; 
							VGParameter.PathDrawingData[j+1] = CenterY-30;
							VGParameter.PathPoints+=1;
							j+=2; 								
 
 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							VGParameter.FillColor = 0x10FF1000;
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);	
				}						
}
 
void RETW_GUI_DEMO_Speed_0_SpeedWindow(uint32_t speed, uint32_t X, uint32_t Y) 
{
				uint32_t i;
				st_VGDrawingParameter	VGParameter;	
 
				VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
				VGParameter.PathDrawingData[0]= X-60;							//org coord X
				VGParameter.PathDrawingData[1]= Y-120;								//org coord Y
				VGParameter.PathDrawingData[2]= 120;							//Width
				VGParameter.PathDrawingData[3]= 60;								//Height
				VGParameter.PathDrawingData[4]= 3;								//Corner X
				VGParameter.PathDrawingData[5]= 3;								//Corner Y				
 				
 				VGParameter.GradientStartEnd[0]=VGParameter.PathDrawingData[0];
 				VGParameter.GradientStartEnd[1]=VGParameter.PathDrawingData[1]+VGParameter.PathDrawingData[3]; 
 				VGParameter.GradientStartEnd[2]=VGParameter.PathDrawingData[0]+VGParameter.PathDrawingData[2];
 				VGParameter.GradientStartEnd[3]=VGParameter.PathDrawingData[1]; 

					static VGfloat SrampStops5[10] = { 	0.0f, 0.8f, 0.8f, 0.8f, 1.0f, 					
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
 					VGParameter.FillColor = 0x30303000;
 					VGParameter.AlphaColor = 0x000000ff;
					VGParameter.StrokeIt=1;
					VGParameter.FillIt=0; 
					GL_DrawingRoundRectangle(&VGParameter);								//stroke		


 					G_CharScale = 2.2;
					G_CharShearX=0.2;
					G_CharShearY=0.0;	
 
					RETW_GUI_DrawScriptNumber(400+5, 145, speed, 24 , 0xC0C0C0ff, 1 );
 	
}


void RETW_GUI_DEMO0_MilesRecord(uint32_t speed, uint32_t X, uint32_t Y) 
{
				uint32_t i;
				st_VGDrawingParameter	VGParameter;	
 
				VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;

				VGParameter.PathDrawingData[2]= 140;							//Width
				VGParameter.PathDrawingData[3]= 70;								//Height
				
				VGParameter.PathDrawingData[0]= X-(VGParameter.PathDrawingData[2]/2);							//org coord X
				VGParameter.PathDrawingData[1]= Y-(VGParameter.PathDrawingData[3]/2);																								//org coord Y

				VGParameter.PathDrawingData[4]= 3;								//Corner X
				VGParameter.PathDrawingData[5]= 3;								//Corner Y				
 				
 				VGParameter.GradientStartEnd[0]=VGParameter.PathDrawingData[0]+(VGParameter.PathDrawingData[2]/2);
 				VGParameter.GradientStartEnd[1]=VGParameter.PathDrawingData[1]+VGParameter.PathDrawingData[3]; 
 				VGParameter.GradientStartEnd[2]=VGParameter.PathDrawingData[0]+(VGParameter.PathDrawingData[2]/2);
 				VGParameter.GradientStartEnd[3]=VGParameter.PathDrawingData[1]; 

					static VGfloat SrampStops6[35] = { 	0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 		
																							0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 
																							0.45f, 0.0f, 0.0f, 0.0f, 1.0f,  
																							0.5f, 0.9f, 0.9f, 0.9f, 1.0f, 	
																							0.55f, 0.0f, 0.0f, 0.0f, 1.0f,  
																							0.95f, 0.0f, 0.0f, 0.0f, 1.0f, 
																							1.0f, 0.5f, 0.5f, 0.5f, 1.0f																
																							};									
					for(i=0;i<35;i++)
					VGParameter.GradientStep[i]=SrampStops6[i]; 
					VGParameter.GradientPoints = 35;
 
					VGParameter.StrokeIt=0;
					VGParameter.FillIt=1; 
					GL_DrawingRoundRectangle(&VGParameter);								//fill 

					VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 					VGParameter.StrokeWidth = 1;
 					VGParameter.FillColor = 0x50505000;
 					VGParameter.AlphaColor = 0x000000ff;
					VGParameter.StrokeIt=1;
					VGParameter.FillIt=0; 
					GL_DrawingRoundRectangle(&VGParameter);								//stroke		

 					G_CharScale = 1.0;
					G_CharShearX=0.0;
					G_CharShearY=0.0;	
 
//					RETW_GUI_DrawScriptNumber(400+5, 130, 456, 24 , 0xD0D0D0ff, 1 );
//					RETW_GUI_DrawScriptNumber(400+5, 95, 123456, 24 , 0xC0C0C0ff, 1 );	

					RETW_GUI_DrawScriptNumber(X+5, Y+16, totalframecnt%1000, 24 , 0xD0D0D0ff, 1 );
					RETW_GUI_DrawScriptNumber(X+5, Y-20, totalframecnt, 24 , 0xC0C0C0ff, 1 );
 
					

												
	
}



int RETW_GUI_DEMO_Speed_0_Right(int speed, int Maxspeed )
{
				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
				st_VGDrawArcParameter	ArcInfo; 
//  				double param, result;			
  	
					static uint32_t O1Outer = 300;
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
																							0.7f, 0.0f, 0.0f, 0.0f, 1.0f, 
	  		
																							0.75f, 0.0f, 0.1f, 0.0f, 1.0f, 
																							0.90f, 0.0f, 0.4f, 0.0f, 1.0f, 
																							0.95f, 0.0f, 0.5f, 0.0f, 1.0f, 	
																							1.00f, 0.60f, 0.6f, 0.0f, 1.0f, 																							
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
				for(i=0;i<101;i++)
				{

//				New_deg = 180+20+ i*320/240;
				New_deg = 90 + i*(80)/100;
 
  			CaculateCoordination(New_deg,	O3InnerR );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
 		
  			if(  (i%20) == 0)
  			{
  				CaculateCoordination(New_deg,	O3InnerR-45 );
  				G_CharScale = 0.6;
					G_CharShearX=0;
					G_CharShearY=0;
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, 100-i, 18,0xA0A0A0ff, 1);
  				vgFinish();
  			}

  			if(  (i <21) &&  (i%5)==0  )
  					{
  					CaculateCoordination(New_deg,	O3InnerR-20 );
						GL_DrawingStroke(4  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
  			else if(  (i >79) &&  (i%5)==0  )
  					{
  					CaculateCoordination(New_deg,	O3InnerR-20 );
						GL_DrawingStroke(4  , 0x0000A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}						
  			else if(  (i%10) == 0)
  					{
  					CaculateCoordination(New_deg,	O3InnerR-20 );
						GL_DrawingStroke(4  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						} 
 
						
//				vgFinish();
				
 				}	

 				G_CharScale = 0.7;
				G_CharShearX=0.0;
				G_CharShearY=0.0;	
				R_TXTR_DrawStringMain("Gas %",5, 650+10, 240-40, 0xA0A0A0ff);
				
				
#endif
/*
  VG_JOIN_MITER                               = 0x1800,
  VG_JOIN_ROUND                               = 0x1801,
  VG_JOIN_BEVEL                               = 0x1802,
*/
 		
#if 1
				for(i=0;i<101;i++)
				{

//				New_deg = 180+20+ i*320/240;
				New_deg = 0 + i*(80)/100;
 
  			CaculateCoordination(New_deg,	O3InnerR );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
 		
  			if(  (i%25) == 0)
  			{
  				CaculateCoordination(New_deg,	O3InnerR-45 );
  				G_CharScale = 0.6;
					G_CharShearX=0;
					G_CharShearY=0;
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, 100-(i/25)*15, 18,0xA0A0A0ff, 1);

  					CaculateCoordination(New_deg,	O3InnerR-20 );
						GL_DrawingStroke(4  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						
  			}
 				vgFinish();
				
 				}	
#endif
 
#if 1		//temperature
 				G_CharScale = 0.7;
				G_CharShearX=0.0;
				G_CharShearY=0.0;	
				R_TXTR_DrawStringMain("T(C)",4, 650+10, 240+40, 0xA0A0A0ff);
 
 #if 1
 				ArcInfo.CenterX=	ClockCenterX;
 				ArcInfo.CenterY=	ClockCenterY;
 
 				ArcInfo.Width=			O3Size -20;
 				ArcInfo.Height=			O3Size -20;
  			ArcInfo.StartAngle= 70;
 				ArcInfo.AngleExtent= 20;
 				ArcInfo.ArcWidth= 20;
 	 			ArcInfo.ArcColor=0x0000A0ff;	
 					ArcInfo.PaintMode = VG_PAINT_TYPE_COLOR;			//VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
 				#if 0									
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
 
 				#endif	
 			  	GL_DrawingArcPara(&ArcInfo);
 			  	
 			  	

 #endif				


 
 				vgFinish();
				
 				RETW_GUI_DEMO0_RED_6p_Ind(O3InnerR, speed,90,80);
 				
 				vgFinish();
 	#endif
 	
}

int RETW_GUI_DEMO_Speed_0_Left( int speed, int Maxspeed )
{
				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
 
				st_VGDrawArcParameter	ArcInfo;
				
				 
//  				double param, result;			
  	
					static uint32_t O1Outer = 300;
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
																							0.7f, 0.0f, 0.0f, 0.0f, 1.0f, 
	  		
																							0.75f, 0.0f, 0.1f, 0.0f, 1.0f, 
																							0.90f, 0.0f, 0.4f, 0.0f, 1.0f, 
																							0.95f, 0.0f, 0.5f, 0.0f, 1.0f, 	
																							1.00f, 0.0f, 0.6f, 0.0f, 1.0f, 																							
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

 

 				G_CharScale = 0.6;
				G_CharShearX=0.0;
				G_CharShearY=0.0;	
				R_TXTR_DrawStringMain("x1000 rpm", 9, 150-60, 240-40, 0xA0A0A0ff);
 #if 1		//RED color
 				ArcInfo.CenterX=150;
 				ArcInfo.CenterY=240;
 				ArcInfo.Width=			O3Size -20;
 				ArcInfo.Height=			O3Size -20;
  			ArcInfo.StartAngle= 90;
 				ArcInfo.AngleExtent= 30;
 				ArcInfo.ArcWidth=20;
 	 			ArcInfo.ArcColor=0x0000A0ff;	
 					ArcInfo.PaintMode = VG_PAINT_TYPE_COLOR;			//VG_PAINT_TYPE_LINEAR_GRADIENT;
 			#if 0									
					static VGfloat SrampStops4[15] = { 	0.0f, 0.6f, 0.0f, 0.0f, 1.0f, 		
																							0.4f, 0.0f, 0.6f, 0.0f, 1.0f, 		
																							1.00f, 0.0f, 0.0f, 0.6f, 1.0f, 																							
																							};									
					for(i=0;i<15;i++)
					ArcInfo.GradientStep[i]=SrampStops4[i]; 
					ArcInfo.GradientPoints = 15;
					
					ArcInfo.GradientStartEnd[0] = ArcInfo.CenterX; 
					ArcInfo.GradientStartEnd[1] = ArcInfo.CenterY - ArcInfo.Width/2;
					ArcInfo.GradientStartEnd[2] = ArcInfo.CenterX; 
					ArcInfo.GradientStartEnd[3] = ArcInfo.CenterY + ArcInfo.Width/2;
			#endif
			
 			  	GL_DrawingArcPara(&ArcInfo);
 #endif

 #if 1		//PURPLE color
 
  			ArcInfo.Width=			O3Size -14;
 				ArcInfo.Height=			O3Size -14;
 				ArcInfo.ArcWidth	=14;
  			ArcInfo.StartAngle= 125;
 				ArcInfo.AngleExtent= 25;
 	 			ArcInfo.ArcColor=0xA000A0ff;	
 				ArcInfo.PaintMode = VG_PAINT_TYPE_COLOR;			//VG_PAINT_TYPE_LINEAR_GRADIENT;
 			  GL_DrawingArcPara(&ArcInfo);
 #endif
 
#if 1			//Grid
				for(i=0;i<181;i++)
				{
 
//				New_deg = 180+45+ i*(360-45-45)/240;
				New_deg = 180+ i;
				if(New_deg>359)
					New_deg = New_deg-360;
	 
  			CaculateCoordination(New_deg,	O3InnerR );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
  #if 1			
  			if(  (i%30) == 0)
  			{
  				CaculateCoordination(New_deg,	O3InnerR-40 );
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
						GL_DrawingStroke(8  , 0x0000FFff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
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

 
 				vgFinish();
				
 				RETW_GUI_DEMO0_RED_6p_Ind_Left(O3InnerR, speed, 180, 180);
 				vgFinish();
}


int RETW_GUI_DEMO_Speed_0_Right1( int speed, int Maxspeed )
{
				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
				st_VGDrawArcParameter	ArcInfo;
				
//  				double param, result;			
  	
				static uint32_t O1Outer = 300;
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

	 			ClockCenterX = 640;
	 			ClockCenterY = 320;
 
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
 
 			#if 0					//most inner surface

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
			
			#if 0				//second inner

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
			
			#if 0				//outer

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
  

#if 0			//Grid
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
 
 

 				G_CharScale = 0.7;
				G_CharShearX=0.0;
				G_CharShearY=0.0;	
				R_TXTR_DrawStringMain("T=", 4, ClockCenterX, ClockCenterY+70, 0xA0A0A0ff);
				
 #if 1
 				ArcInfo.CenterX=ClockCenterX;
 				ArcInfo.CenterY=ClockCenterY;
 				ArcInfo.Width=											O1Outer/2;
 				ArcInfo.Height=											O1Outer/2;
  			ArcInfo.StartAngle= 0;
 				ArcInfo.AngleExtent= 20 ;
 				ArcInfo.ArcWidth=20;
 	 			ArcInfo.ArcColor=0x303030ff;	

 				ArcInfo.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;			//VG_PAINT_TYPE_COLOR
 				
#if 1
				static VGfloat SrampStops4[15] = { 	0.0f, 0.6f, 0.0f, 0.0f, 1.0f, 		
																						0.4f, 0.0f, 0.6f, 0.0f, 1.0f, 		
																						1.00f, 0.0f, 0.0f, 0.6f, 1.0f, 																							
																						};									
				for(i=0;i<15;i++)
				ArcInfo.GradientStep[i]=SrampStops4[i]; 
				ArcInfo.GradientPoints = 15;
				
				ArcInfo.GradientStartEnd[0] = ArcInfo.CenterX;
				ArcInfo.GradientStartEnd[1] = ArcInfo.CenterY;
				ArcInfo.GradientStartEnd[2] = ArcInfo.CenterX + ArcInfo.Width/2;;
				ArcInfo.GradientStartEnd[3] = ArcInfo.CenterY + ArcInfo.Height/2;;
 
 				ArcInfo.FillIt = 0;
 				ArcInfo.StrokeIt = 1; 			
#endif 					
 			  GL_DrawingArcPara(&ArcInfo);

  			ArcInfo.StartAngle= 20;
 				ArcInfo.AngleExtent= 40;
 				ArcInfo.ArcWidth=20;
 	 			ArcInfo.ArcColor=0x606060ff;			
 			  GL_DrawingArcPara(&ArcInfo);
 			  	
  			ArcInfo.StartAngle= 60;
 				ArcInfo.AngleExtent= 20;
 				ArcInfo.ArcWidth=20;
 	 			ArcInfo.ArcColor=0x000080ff;			
				GL_DrawingArcPara(&ArcInfo);
 			  	
				R_TXTR_DrawStringMain("High", 4, ClockCenterX+40, ClockCenterY+ArcInfo.Height/2 - 40, 0x0000FFff);
				R_TXTR_DrawStringMain("Low", 3, ClockCenterX+ArcInfo.Height/2 - 40, ClockCenterY, 0xFFFFFFff);

 #endif
 				
  			vgFinish();
				
 				RETW_GUI_DB0_RED_6p_Ind(O3InnerR, speed, 10, 90);
 				vgFinish();
}

 
int RETW_GUI_DEMO0_RED_6p_Ind(uint32_t MaxLength, uint32_t speed, uint32_t StartDeg, uint32_t ExtentDeg )
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

				static uint32_t IndicatorLoc[10]= { 2,180-50,		 	180-25,20,		 180+25,20,		 360-2, 180-50,		0, 180-40};
 	
 
				static VGfloat SrampStops[35] = { 0.0f, 0.0f, 0.0f, 0.6f, 1.0f, 		
																					0.35f, 0.0f, 0.0f, 0.8f, 1.0f, 		
																					0.40f, 0.2f, 0.2f, 0.9f, 1.0f, 	
																					0.5f, 0.0f, 0.0f, 1.0f, 1.0f,	
																					0.6f, 0.0f, 0.0f, 0.7f, 1.0f, 	
																					0.65f, 0.1f, 0.1f, 0.3f, 1.0f,	
																					1.0f, 0.1f, 0.1f, 0.2f, 1.0f };
				
						IndicatorLoc[0] = MaxLength;							//aa
						IndicatorLoc[1] = MaxLength-15;							//bb
						IndicatorLoc[2] = 5;												//cc
						IndicatorLoc[3] = 25;												//dd
						IndicatorLoc[4] = 30;												//ee
						IndicatorLoc[5] = 8;												//ff							
 
						IndiNum=0; 
				
#if 0		
 							Indi_deg =  speed +180+45;
 							
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif

#if 0		 //This will hand up
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

#if 0 	//Give start and extentAngle
 							Indi_deg = ((speed*ExtentDeg)/220 ) + StartDeg;
 							
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif

#if 1
							if(Indi_deg>MainIndicatorDeg)
 							MainIndicatorDeg+=1;
 							else if(Indi_deg<MainIndicatorDeg)
 							MainIndicatorDeg-=1;
 							
 							Indi_deg=MainIndicatorDeg;
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif
 
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							CaculateCoordination(	Indi_deg, IndicatorLoc[0]);															//aa		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
							CaculateCoordination(	Indi_deg, IndicatorLoc[1]);																//bb
 							New_deg = CaculateIncDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[2]);						//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateIncDeg(Indi_deg, 180);																								
							CaculateCoordination(	New_deg, IndicatorLoc[3]);																//dd
 							New_deg = CaculateDecDeg(New_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[5]); 						//ff
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
 							New_deg = CaculateIncDeg(Indi_deg, 180);
 							CaculateCoordination(	New_deg, IndicatorLoc[4]);																//ee
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
							k+=1; 

 							New_deg = CaculateIncDeg(Indi_deg, 180);																								
							CaculateCoordination(	New_deg, IndicatorLoc[3]);																//dd
 							New_deg = CaculateIncDeg(New_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[5]); 						//ff
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		 					
 
 							CaculateCoordination(	Indi_deg, IndicatorLoc[1]);																//bb
 							New_deg = CaculateDecDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[2]);						//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			


 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							VGParameter.FillColor = 0x1010C000;
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);	
 					
							
		/*-------------Calculate	GradientStartEnd			------*/
#if 0
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

#endif

}
int RETW_GUI_DEMO0_RED_6p_Ind_TopOnly(uint32_t MaxLength, uint32_t speed, uint32_t StartDeg, uint32_t ExtentDeg )
{
/*				
				 /-------------------------------------/
				/	|																  /  |
			 /	cc															/   ff
			*    								  							*																		*org  		  
			 \																	\
			  \																	  \	
			   \-------------------------------------\
			   
			*----------------aa-----------------------------------------------------*
			   	*------------bb-----------------------------------------------------*
			   																	*-dd--------------------------------*
			   																	*-ee-|
*/
				uint32_t IndiNum, i,j,k,New_deg,Indi_deg;
				st_VGDrawingParameter	VGParameter;

				static uint32_t IndicatorLoc[10]= { 2,180-50,		 	180-25,20,		 180+25,20,		 360-2, 180-50,		0, 180-40};
 	
				static VGfloat SrampStops[35] = { 0.0f, 0.0f, 0.0f, 0.6f, 1.0f, 		
																					0.35f, 0.0f, 0.0f, 0.8f, 1.0f, 		
																					0.40f, 0.2f, 0.2f, 0.9f, 1.0f, 	
																					0.5f, 0.0f, 0.0f, 1.0f, 1.0f,	
																					0.6f, 0.0f, 0.0f, 0.7f, 1.0f, 	
																					0.65f, 0.1f, 0.1f, 0.3f, 1.0f,	
																					1.0f, 0.1f, 0.1f, 0.2f, 1.0f };
				
						IndicatorLoc[0] = MaxLength;							//aa
						IndicatorLoc[1] = MaxLength-15;							//bb
						IndicatorLoc[2] = 5;												//cc
						IndicatorLoc[3] = 25;												//dd
						IndicatorLoc[4] = 30;												//ee
						IndicatorLoc[5] = 8;												//ff							
 
						IndiNum=0; 
				
#if 0		
 							Indi_deg =  speed +180+45;
 							
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif

#if 0		 //This will hand up
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

#if 0 	//Give start and extentAngle
 							Indi_deg = ((speed*ExtentDeg)/220 ) + StartDeg;
 							
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif

#if 1
							if(Indi_deg>MainIndicatorDeg)
 							MainIndicatorDeg+=1;
 							else if(Indi_deg<MainIndicatorDeg)
 							MainIndicatorDeg-=1;
 							
 							Indi_deg=MainIndicatorDeg;
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif
 
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							CaculateCoordination(	Indi_deg, IndicatorLoc[0]);															//aa		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
							CaculateCoordination(	Indi_deg, IndicatorLoc[1]);																//bb
 							New_deg = CaculateIncDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[2]);						//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateIncDeg(Indi_deg, 180);																								
							CaculateCoordination(	New_deg, IndicatorLoc[3]);																//dd
 							New_deg = CaculateDecDeg(New_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[5]); 						//ff
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
 							New_deg = CaculateIncDeg(Indi_deg, 180);
 							CaculateCoordination(	New_deg, IndicatorLoc[4]);																//ee
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
							k+=1; 

 							New_deg = CaculateIncDeg(Indi_deg, 180);																								
							CaculateCoordination(	New_deg, IndicatorLoc[3]);																//dd
 							New_deg = CaculateIncDeg(New_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[5]); 						//ff
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		 					
 
 							CaculateCoordination(	Indi_deg, IndicatorLoc[1]);																//bb
 							New_deg = CaculateDecDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[2]);						//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			


 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							VGParameter.FillColor = 0x1010C000;
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);	
 					
							
		/*-------------Calculate	GradientStartEnd			------*/
#if 0
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

#endif

}


int RETW_GUI_DEMO0_RED_6p_Ind_LineOnly(uint32_t MaxLength, uint32_t MinLength, uint32_t speed, uint32_t StartDeg, uint32_t ExtentDeg )
{
/*				
				 /-------------------------------------/
				/	|																  /  |
			 /	cc															/   ff
			*    								  							*																		*org  		  
			 \																	\
			  \																	  \	
			   \-------------------------------------\
			   
			*----------------aa-----------------------------------------------------*
			   	*------------bb-----------------------------------------------------*
			   																	*-dd--------------------------------*
			   																	*-ee-|
*/
				uint32_t i,j,Engine_deg, New_deg, Indi_deg;
				uint32_t AADV,ADV;
				int32_t		Indi_deg_Gap;
				st_VGDrawingParameter	VGParameter;
								
				static uint32_t IndicatorLoc[10]= { 2,	180-50,		 	180-25,20,		 180+25,20,		 360-2, 180-50,		0, 180-40};
 
				static VGfloat SrampStops[35] = { 0.0f, 0.0f, 0.0f, 0.6f, 1.0f, 		
																					0.35f, 0.0f, 0.0f, 0.8f, 1.0f, 		
																					0.40f, 0.2f, 0.2f, 0.9f, 1.0f, 	
																					0.5f, 0.0f, 0.0f, 1.0f, 1.0f,	
																					0.6f, 0.0f, 0.0f, 0.7f, 1.0f, 	
																					0.65f, 0.1f, 0.1f, 0.3f, 1.0f,	
																					1.0f, 0.1f, 0.1f, 0.2f, 1.0f };
	
				
						IndicatorLoc[0] = MaxLength;								//aa
						IndicatorLoc[1] = MaxLength-10;								//bb
						IndicatorLoc[2] = 4;												//cc
						IndicatorLoc[3] = MinLength;							  //dd
						IndicatorLoc[4] = 0;												//ee
						IndicatorLoc[5] = 6;												//ff							
 				
						
#if 0		
 							Indi_deg =  speed +180+45;
 							
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif

#if 0		 //This will hand up
 							Indi_deg = ((speed* 270)/220 ) +180+45;
 							
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif



#if 0		//for Engine meter
 							Engine_deg =EngineRPM+270;
 					
#endif

#if 0 
 							Indi_deg = ((speed* 270)/220 ) +180+45;
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif

#if 0 	//Give start and extentAngle
 							Indi_deg = ((speed*ExtentDeg)/220 ) + StartDeg;
 							
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif

#if 1			//Read AD value
							AADV=0;

							for(i=0;i<10;i++)
							AADV += R_ADC_Read();						//1024
							ADV=AADV/10;				
							
							Engine_deg	= 270*8 + ADV*1440/1024;			//Need 180 deg			
							EngineRPM  = ADV*180/1024;
 							
#endif

#if 1				
							if(Engine_deg > (MainIndicatorDeg+2)  )
								{
										Indi_deg_Gap = (Engine_deg - MainIndicatorDeg)/8;
										
										if(Indi_deg_Gap> 3)
										MainIndicatorDeg +=8;										
										else if(Indi_deg_Gap> 2)
 										MainIndicatorDeg +=4;
 										else if (Indi_deg_Gap> 1)
 										MainIndicatorDeg +=2;
 										else
 										MainIndicatorDeg +=1;
 										
//    							printf("INC....%d \n",MainIndicatorDeg );
 
 								}
 							else if(Engine_deg < (MainIndicatorDeg-2) )
 								{
										Indi_deg_Gap = (MainIndicatorDeg - Engine_deg)/8;
										
										if(Indi_deg_Gap> 3)
										MainIndicatorDeg -=8;										
										else if(Indi_deg_Gap> 2)
 										MainIndicatorDeg -=4;
 										else if (Indi_deg_Gap> 1)
 										MainIndicatorDeg -=2;
 										else
 										MainIndicatorDeg -=1; 									
 
 										if(MainIndicatorDeg < 270*8)
 										MainIndicatorDeg=270*8;
 //  								printf(".....DEC.....%d.\n",MainIndicatorDeg); 		
 							  }

 							if(MainIndicatorDeg>2880)
 							Indi_deg = MainIndicatorDeg-2880;
							else
 							Indi_deg=MainIndicatorDeg;
#endif


#if 0 //Indicator by drawing line
 				CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY, Indi_deg, MaxLength);
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
 				CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY, Indi_deg, MinLength);		
 
//     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 7, 0xF0F0F0A0);									// Indicator by Draw Line with Cap		
//     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 5, 0x0000FFFF);									// Indicator by Draw Line with Cap		


//				printf("Draw line from %d, %d to %d, %d\n",CoordXDest, CoordYDest, CoordXSrc, CoordYSrc);
     		Stroke_JoinStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 7, 0xF0F0F0F0);									// Indicator by Draw Line with Cap		
     		Stroke_JoinStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 5, 0x0000FFFF);									// Indicator by Draw Line with Cap	   

#endif
     		
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY,	Indi_deg, IndicatorLoc[0]);															//aa		1
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
							CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY,	Indi_deg, IndicatorLoc[1]);															//bb    2
 							New_deg = CaculateDecDeg_Float(Indi_deg, 90*8);
 							
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[2]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 

							CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY,	Indi_deg, IndicatorLoc[3]-IndicatorLoc[4]);							//bb    3
 							New_deg = CaculateDecDeg_Float(Indi_deg, 90*8);
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[5]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		
							

							CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY,	Indi_deg, IndicatorLoc[3]);															//dd		4
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		
							
							CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY,	Indi_deg, IndicatorLoc[3]-IndicatorLoc[4]);							//bb    5
 							New_deg = CaculateIncDeg_Float(Indi_deg, 90*8);
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[5]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;																
							
							CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY,	Indi_deg, IndicatorLoc[1]);															//bb    6
 							New_deg = CaculateIncDeg_Float(Indi_deg, 90*8);
 							
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[2]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
 
#if 0
 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							VGParameter.FillColor = 0x1010C000;
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);	
#endif

		/*-------------Calculate	GradientStartEnd			------*/
#if 0
 							if(Indi_deg < 720)
 							{							
							CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY,	Indi_deg, MinLength );												 
 							New_deg = CaculateIncDeg_Float(Indi_deg, 90*8);
 							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc, New_deg, IndicatorLoc[5]);
 
								VGParameter.GradientStartEnd[0] = CoordXSrc;
								VGParameter.GradientStartEnd[1] = CoordYSrc;
 
							CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY,	Indi_deg, MinLength);														 
 							New_deg = CaculateDecDeg_Float(Indi_deg, 90*8);
 							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc, New_deg, IndicatorLoc[5]);					
 
								VGParameter.GradientStartEnd[2] = CoordXSrc;
								VGParameter.GradientStartEnd[3] = CoordYSrc;
							}
							else
 							{							
							CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY,	Indi_deg, MinLength);												 
 							New_deg = CaculateIncDeg_Float(Indi_deg, 90*8);
 							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc, New_deg, IndicatorLoc[5]);
 
								VGParameter.GradientStartEnd[2] = CoordXSrc;
								VGParameter.GradientStartEnd[3] = CoordYSrc;
 
							CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY,	Indi_deg, MinLength);															 
 							New_deg = CaculateDecDeg_Float(Indi_deg, 90*8);
 							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc, New_deg, IndicatorLoc[5]);					
 
								VGParameter.GradientStartEnd[0] = CoordXSrc;
								VGParameter.GradientStartEnd[1] = CoordYSrc;
							}					  
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

#endif

#if 1
/*
  VG_PAINT_TYPE_COLOR                         = 0x1B00,
  VG_PAINT_TYPE_LINEAR_GRADIENT               = 0x1B01,
  VG_PAINT_TYPE_RADIAL_GRADIENT               = 0x1B02,				
*/
 
								VGParameter.GradientStartEnd[0] = VGParameter.PathDrawingData[0];
								VGParameter.GradientStartEnd[1] = VGParameter.PathDrawingData[1];
								VGParameter.GradientStartEnd[2] = VGParameter.PathDrawingData[6];
								VGParameter.GradientStartEnd[3] = VGParameter.PathDrawingData[7];

				static VGfloat SrampStops5[25] = { 	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																						0.2f, 0.0f, 0.0f, 0.8f, 1.0f, 		
																						0.4f, 0.0f, 0.0f, 1.0f, 1.0f, 		
																						0.8f, 0.0f, 0.0f, 1.0f, 1.0f,	
																						1.0f, 0.0f, 0.0f, 0.2f, 1.0f };
																					
 							VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
							for(i=0;i<25;i++)
							VGParameter.GradientStep[i]=SrampStops5[i]; 
							VGParameter.GradientPoints = 25;		
							GL_DrawingPolyconFill( &VGParameter);	

#endif
}


int RETW_GUI_DEMO0_RED_6p_Ind_LineOnlyBAK(uint32_t MaxLength, uint32_t MinLength, uint32_t speed, uint32_t StartDeg, uint32_t ExtentDeg )
{
/*				
				 /-------------------------------------/
				/	|																  /  |
			 /	cc															/   ff
			*    								  							*																		*org  		  
			 \																	\
			  \																	  \	
			   \-------------------------------------\
			   
			*----------------aa-----------------------------------------------------*
			   	*------------bb-----------------------------------------------------*
			   																	*-dd--------------------------------*
			   																	*-ee-|
*/
				uint32_t ADV,Engine_deg,Indi_deg;
 			

				
				
				
#if 0		
 							Indi_deg =  speed +180+45;
 							
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif

#if 0		 //This will hand up
 							Indi_deg = ((speed* 270)/220 ) +180+45;
 							
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif



#if 0		//for Engine meter
 							Engine_deg =EngineRPM+270;
 					
#endif

#if 0 
 							Indi_deg = ((speed* 270)/220 ) +180+45;
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif

#if 0 	//Give start and extentAngle
 							Indi_deg = ((speed*ExtentDeg)/220 ) + StartDeg;
 							
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif

#if 1			//Read AD value
							ADV = R_ADC_Read()* 180/1024;
							
							EngineRPM = ADV;	
 							Engine_deg = ADV+270;								
#endif

#if 1				
							if(Engine_deg > (MainIndicatorDeg+1) )
								{
 									MainIndicatorDeg +=1;
   									printf("INC....%d \n",MainIndicatorDeg );
 								}
 							else if(Engine_deg < (MainIndicatorDeg) )
 								{
 									MainIndicatorDeg -=1;
 										if(MainIndicatorDeg < 270)
 										MainIndicatorDeg=270;
  								printf(".....DEC.....%d.\n",MainIndicatorDeg); 		
 							  }
 
 							if(MainIndicatorDeg>359)
 							Indi_deg = MainIndicatorDeg-360;
							else
 							Indi_deg=MainIndicatorDeg;
#endif


				CaculateCoordination(	Indi_deg, MaxLength);
				
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
				CaculateCoordination( Indi_deg, MinLength);		
							
//     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 7, 0xF0F0F0A0);									// Indicator by Draw Line with Cap		
//     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 5, 0x0000FFFF);									// Indicator by Draw Line with Cap		

     		Stroke_JoinStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 7, 0xF0F0F0F0);									// Indicator by Draw Line with Cap		
     		Stroke_JoinStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 5, 0x0000FFFF);									// Indicator by Draw Line with Cap	     		
	
}




int RETW_GUI_DEMO0_RED_6p_Ind_Left(uint32_t MaxLength, uint32_t speed, uint32_t StartDeg, uint32_t ExtentDeg )
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
				
#if 1		
 							Indi_deg =  180 +  ((speed*180)/220);
 							
 							if(Indi_deg>359)
 							Indi_deg=Indi_deg-360;
#endif

#if 0		 //This will hand up
 							Indi_deg = ((speed* 270)/220 ) +180+45;
 							
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

#if 0		//Give start and extentAngle
 							Indi_deg = ((speed* ExtentDeg)/220 ) + StartDeg;
 							
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