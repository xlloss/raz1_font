
#include    "retwhmi.h"

		//												percent,B,G,R,alpha								percent,B,G,R,alpha								percent,B,G,R,alpha

 
 
int RETW_GUIClock_1_Drawing(void)
{
	
#if 0
	 if(ClockShiftDir == 1)
	 	{
	 		ClockShiftX+=2;
	 		if(ClockShiftX>300)
	 			{
	 			ClockShiftDir=0;
	 			ClockShiftX=299;
	 			
	 			Userdef_OSTM0_GenRandomNum();
	 		}
	 	}
	 	else
	 	{
	 		ClockShiftX-=2;
	 		if(ClockShiftX<2)
	 			{
	 			ClockShiftDir=1;
	 			ClockShiftX=2;
	 		}
	 	}
	 	
	 	ClockCenterX= 400-150+ClockShiftX;
 
#endif
	 	ClockCenterX= 400;

  			TrVg_DrawingCircleMode(0, ClockCenterX,ClockCenterY,470,470,1,0x006030ff);	
 				TrVg_DrawingCircleBatch(ClockCenterX,ClockCenterY,470,470,0x006030ff);

#if 1
				RETW_GUI_ImportGlobalTime();
#endif
 
#if 0
        GlobalSecDeg+=1;
        if(GlobalSecDeg==360)
        	{
        	 GlobalSecDeg=0;
        	 GlobalMinDeg+=6;
        	 	if(GlobalMinDeg==360)
        	 		{
        	 			GlobalMinDeg=0;
        	 			GlobalHourDeg+=6;
        	 			if(GlobalHourDeg==360)
        	 					GlobalHourDeg=0;
        	 		}
        	}
#endif
#if 0
        GlobalSecDeg+=1;
        if(GlobalSecDeg==360)
        	 	GlobalSecDeg=0;
        	 
				GlobalMinDeg+=1;
				if(GlobalMinDeg==360)
						GlobalMinDeg=0;
        	 			
				GlobalHourDeg+=1;
				if(GlobalHourDeg==360)
						GlobalHourDeg=0;
						
#endif
 				TrVg_Drawing12Circle();
 
 //        GUI_ClockTickIndicator(GlobalDeg, 214, 8.0 , 0x003000ff);
 
#if 0
 				GUI_DrawCapStyleIndicator(GlobalDeg, 210, 1.6 , 0x00C000ff);
#else
 				GUI_DrawPolyconStyleIndicatorHour(GlobalHourDeg, 		140, 8 , 0xC00000ff);
 				GUI_DrawPolyconStyleIndicatorMin(GlobalMinDeg,			190, 1.6 , 0x00C000ff);
 				GUI_DrawCapStyleIndicator(GlobalSecDeg,							200, 1, 0x0000FFff);
#endif
    				
	
}
 

void RETW_GUI_Clock_0()
{ 
				uint32_t i,j,k,New_deg;
				uint32_t loc_X,loc_Y;
				uint16_t YEAR;
				uint8_t MONTH;
				uint8_t DAY;
				
				char TempString[4];
				
				st_VGDrawingParameter	VGParameter;
//  				double param, result;			
					static uint32_t O1Outer = 470;
					static uint32_t O1Width = 8;
					static float O1Size;

					static uint32_t O2Outer;
					static uint32_t O2Width = 10;
					static float O2Size;

					static uint32_t O3Outer;
					static uint32_t O3Width = 0;
					static uint32_t O3Size;
					static uint32_t O3InnerR;
					
					static float ColorR =	1.0;				
					static float ColorG = 0.0;	
					static float ColorB = 0.0;	
 
 				O1Size = O1Outer-O1Width;
 				O2Outer = O1Outer-O1Width*2;
				O2Size = O2Outer-O2Width;
				O3Outer = O2Outer-O2Width*2;
				O3Size = O3Outer-O3Width;	
						
				O3InnerR = O3Size/2;

	 			ClockCenterX = 560;
	 			ClockCenterY = 240;
 
//  param = 30.0;
//  result = sin (param*PI/180);
//  printf ("The sine of %f degrees is %f.\n", param, result );

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
 
 		
					static VGfloat SrampStops1[35] = { 	0.0f, 0.9f, 0.9f, 0.9f, 1.0f, 		
																							0.25f, 0.0f, 0.0f, 0.0f, 1.0f, 		
																							0.7f, 0.0f, 0.0f, 0.0f, 1.0f, 
	  		
																							0.80f, 0.1f, 0.1f, 0.1f, 1.0f, 
																							0.90f, 0.4f, 0.4f, 0.4f, 1.0f, 
																							0.95f, 0.5f, 0.5f, 0.5f, 1.0f, 	
																							1.00f, 0.6f, 0.6f, 0.6f, 1.0f, 																							
																							};			
																							
//Adjust Color by ColorR, ColorG, ColorB						
					for(j=0;j<7;j++)
					for(i=1;i<4;i++)
										{
											if(i==1)
											SrampStops1[j*5+i] =  SrampStops1[j*5+i]*ColorB;		
											else if	(i==2)
											SrampStops1[j*5+i] =  SrampStops1[j*5+i]*ColorG;			
											else if	(i==3)
											SrampStops1[j*5+i] =  SrampStops1[j*5+i]*ColorR;	
										}							
					
					for(i=0;i<35;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 35;
					
					VGParameter.GradientStartEnd[0] = ClockCenterX;
					VGParameter.GradientStartEnd[1] = ClockCenterY;
					VGParameter.GradientStartEnd[2] = ClockCenterX;
					VGParameter.GradientStartEnd[3] = ClockCenterY;
					VGParameter.GradientStartEnd[4] = O3Size/2;
				  GL_DrawingCircleFill( &VGParameter);	
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
 			#endif
 

#if 1
//				RETW_GUI_ImportGlobalTimeMotion1();
				
				RETW_GUI_ImportGlobalTimeMotionHighReso();
#endif

#if 1
				for(i=0;i<360;i++)
				{
 
				New_deg = i;
				
  			CaculateCoordination(New_deg,	O3InnerR );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
  			
  			if(  (i%30) == 0)
  			{
  				CaculateCoordination(New_deg,	O3InnerR-70 );
  				
  				G_CharScale=2.0 * O1Outer/470;
  				
  				G_CharShearX =0.3;
					G_CharShearY =0.0;
 
				
  				if(i==0)
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, 12, 24, 0xFFFFFFff, 1);
  				else
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, i/30, 24, 0xFFFFFFff, 1);
  			}
  			
  			
  			if(  i == 0 )
  					{
  					CaculateCoordination(New_deg,	O3InnerR-20 );
						GL_DrawingStroke( 4  , 0xA0A0A0ff,VG_CAP_ROUND);									// Indicator by Draw Line with Cap    
						}
  			else if(  (i%30) == 0)
  					{
  					CaculateCoordination(New_deg,	O3InnerR-20 );
						GL_DrawingStroke( 4  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
				else if(  (i%6) == 0)

  					{
  					CaculateCoordination(New_deg,	O3InnerR-15 );
						GL_DrawingStroke( 1  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
     		else
     				{
     				CaculateCoordination(New_deg,	O3InnerR-10 );
 						GL_DrawingStroke( 1  , 0x808080ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}   	
 				}	
/*
  VG_JOIN_MITER                               = 0x1800,
  VG_JOIN_ROUND                               = 0x1801,
  VG_JOIN_BEVEL                               = 0x1802,
*/
#endif




 //        GUI_ClockTickIndicator(GlobalDeg, 214, 8.0 , 0x003000ff);
#if 1
// 				RETW_GUI_Meter_3_Grid(O3InnerR);
//				RETW_GUI_Meter_3_Cap(36,36,0xFFFFFFff);
				RETW_GUI_Clock_4p_Ind_WHITE(O3InnerR);
//				
#endif

#if 1				//Calendar

 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
 
							VGParameter.PathDrawingData[j]  = 60; 
							VGParameter.PathDrawingData[j+1] = 40;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 
							VGParameter.PathDrawingData[j]  = 300; 
							VGParameter.PathDrawingData[j+1] = 60;
							VGParameter.PathPoints+=1;
							j+=2;			
 
							VGParameter.PathDrawingData[j]  = 260; 
							VGParameter.PathDrawingData[j+1] = 459;
							VGParameter.PathPoints+=1;
							j+=2;			
  
							VGParameter.PathDrawingData[j]  = 20; 
							VGParameter.PathDrawingData[j+1] = 420;
							VGParameter.PathPoints+=1;
							j+=2;			

		/*-------------Calculate	GradientStartEnd			------*/
 
							VGParameter.GradientStartEnd[0] = VGParameter.PathDrawingData[4]-5;
							VGParameter.GradientStartEnd[1] = VGParameter.PathDrawingData[5];	
							VGParameter.GradientStartEnd[2] = VGParameter.PathDrawingData[2];
							VGParameter.GradientStartEnd[3] = VGParameter.PathDrawingData[3];	
 
/*
  VG_PAINT_TYPE_COLOR                         = 0x1B00,
  VG_PAINT_TYPE_LINEAR_GRADIENT               = 0x1B01,
  VG_PAINT_TYPE_RADIAL_GRADIENT               = 0x1B02,				
*/

					static VGfloat SrampStops4[20] = { 	0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 		
																							0.4f, 0.0f, 0.0f, 1.0f, 1.0f,  		
																							0.45f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																							1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 																							
																							};
																														
 							VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
							for(i=0;i<20;i++)
							VGParameter.GradientStep[i]=SrampStops4[i]; 
							VGParameter.GradientPoints = 20;		
							GL_DrawingPolyconFill( &VGParameter);	
#endif
 
 
							YEAR	= SYSTEMTIME.Year;
							MONTH	= SYSTEMTIME.Month;      							
							DAY		= SYSTEMTIME.Day;   
							k=0;
 							for(i=0;i<4;i++)
 							{									 
  							TempString[k] = (char) ((( YEAR >> ((3-i)*4) )& 0x000F) + 0x30) ;	
  							k+=1;																	
 							}
							G_CharShearX= -0.15;
							G_CharShearY= 0.15;
 							G_CharScale = 3.5;		 									
 							R_TXTR_DrawStringMain( &TempString[0], k, 42, 340, 0x808000FF);  
 									
 							j= ((MONTH>>4)&0x0f)*10 + (MONTH&0x0F);

 							G_CharScale = 2.5;	 		
 							
 							loc_X =96;
 							loc_Y =290;
 							
 							if(j==1)
 							R_TXTR_DrawStringMain("Jan", 3, loc_X, loc_Y, 0x000000FF);  
 							else if(j==2)
 							R_TXTR_DrawStringMain("Feb", 3, loc_X, loc_Y, 0x000000FF);   										
 							else if(j==3)
 							R_TXTR_DrawStringMain("Mar", 3, loc_X, loc_Y, 0x000000FF);   
 							else if(j==4)
 							R_TXTR_DrawStringMain("Apr", 3, loc_X, loc_Y, 0x000000FF);   
 							else if(j==5)
 							R_TXTR_DrawStringMain("May", 3, loc_X, loc_Y, 0x000000FF);   										
 							else if(j==6)
 							R_TXTR_DrawStringMain("Jun", 3, loc_X, loc_Y, 0x000000FF);   
 							else if(j==7)
 							R_TXTR_DrawStringMain("July",4, loc_X, loc_Y, 0x000000FF);    				 																
 							else if(j==8)
 							R_TXTR_DrawStringMain("Aug", 3, loc_X, loc_Y, 0x000000FF);   										
 							else if(j==9)
 							R_TXTR_DrawStringMain("Sep", 3, loc_X, loc_Y, 0x000000FF);   
 							else if(j==10)
 							R_TXTR_DrawStringMain("Oct", 3, loc_X, loc_Y, 0x000000FF);   
 							else if(j==11)
 							R_TXTR_DrawStringMain("Nov", 3, loc_X, loc_Y, 0x000000FF);   										
 							else if(j==12)
 							R_TXTR_DrawStringMain("Dec", 3, loc_X, loc_Y, 0x000000FF);   
 							
 							j= ((DAY>>4)&0x0f)*10 + (DAY&0x0F);

 							G_CharScale = 8.0;	 														
							RETW_GUI_DrawScriptNumber(190, 120, j, 24 , 0x000000FF, 1 );
}
 
int RETW_GUIClock_1_Basic(void)
{
				int i;
				int New_deg;

	 			ClockCenterX= 400;
				ClockCenterY = 240;

  			TrVg_DrawingCircleMode(0, ClockCenterX,ClockCenterY,470,470,10,0xF0F0F0ff);	
 
//					Polycon_linearGradient[0] = ClockCenterX-300;
//					Polycon_linearGradient[1] = ClockCenterY+300;
//					Polycon_linearGradient[2] = ClockCenterX+300;
//					Polycon_linearGradient[3] = ClockCenterY-300;					
					VGfloat SrampStops[30] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
																 		 0.2f, 1.0f, 1.0f, 0.9f, 1.0f, 		
																 		 0.4f, 1.0f, 1.0f, 0.8f, 1.0f,
																		 0.6f, 1.0f, 1.0f, 0.6f, 1.0f, 		
																		 0.8f, 1.0f, 1.0f, 0.5f, 1.0f, 		
																		 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,};
					for(i=0;i<30;i++)
					Polycon_RampStopTab[i]=SrampStops[i];
 
     		Polycon_RadGradient[ 0 ] = ClockCenterX ;
    		Polycon_RadGradient[ 1 ] = ClockCenterY ;
    		Polycon_RadGradient[ 2 ] = Polycon_RadGradient[ 0 ] ;
    		Polycon_RadGradient[ 3 ] = Polycon_RadGradient[ 1 ] ;
    		Polycon_RadGradient[ 4 ] = 200;
    
 				TrVg_DrawingCircleRadialPara(ClockCenterX,ClockCenterY,400,400,0x0000FFff,30);	
				
				for(i=0;i<60;i++)
				{
				New_deg = i*6;
				
  			CaculateCoordination(New_deg,	210 );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
  			CaculateCoordination(New_deg,	230 );
  			
  			if(i%5 == 0)
				Stroke_JoinStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 8  , 0xF0F0F0ff);									// Indicator by Draw Line with Cap     
				else
				Stroke_JoinStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 2  , 0xF0F0F0ff);									// Indicator by Draw Line with Cap  				
				}
 
				RETW_GUI_ImportGlobalTime();
 
				CaculateCoordination(	GlobalHourDeg, 140);
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
				New_deg = CaculateIncDeg(GlobalHourDeg,180);
				CaculateCoordination(	New_deg, 20);							
     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 12, 0xC0C0C0ff);									// Indicator by Draw Line with Cap			

				CaculateCoordination(	GlobalMinDeg, 180);
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
				New_deg = CaculateIncDeg(GlobalMinDeg,180);
				CaculateCoordination(	New_deg, 20);							
     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 12, 0xC0C0C0ff);									// Indicator by Draw Line with Cap			

				CaculateCoordination(	GlobalSecDeg, 185);
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
				New_deg = CaculateIncDeg(GlobalSecDeg,180);
				CaculateCoordination(	New_deg, 25);							
     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 2, 0xF0F0F0ff);									// Indicator by Draw Line with Cap		

					Polycon_linearGradient[0] = ClockCenterX-8;
					Polycon_linearGradient[1] = ClockCenterY+8;
					Polycon_linearGradient[2] = ClockCenterX+8;
					Polycon_linearGradient[3] = ClockCenterY-8 ;					
					static VGfloat SrampStops1[15] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 		1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
					for(i=0;i<15;i++)
					Polycon_RampStopTab[i]=SrampStops1[i];
 
     		  TrVg_DrawingCircleLiearRadiantPara( ClockCenterX, ClockCenterY,16, 16, 2, 0xFFFFFFff,15 );
     		  

     		  			     		
}
 
int RETW_GUI_Clock_1(void)
{
				int i;
				int New_deg;
				st_VGDrawingParameter	VGParameter;
				
	 				ClockCenterX= 400;
					ClockCenterY = 240;
 
 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= 476;							/*	width			*/
					VGParameter.PathDrawingData[3]= 476;							/*	height		*/
 	
					VGfloat SrampStops[30] = 				{ 	0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
																 		 					0.85f, 0.0f, 0.0f, 0.0f, 1.0f, 		
																 		 					0.94f, 0.6f, 0.6f, 0.6f, 1.0f,
																		 					0.96f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																		 					0.98f, 0.5f, 0.5f, 0.5f, 1.0f, 		
																		 					1.0f, 0.0f, 0.0f, 0.0f, 1.0f,};
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops[i]; 
					VGParameter.GradientPoints = 30;
					
					VGParameter.FillColor = 0x38503800;
					VGParameter.AlphaColor = 0x000000ff;
 	 								
					VGParameter.GradientStartEnd[0] = ClockCenterX;
					VGParameter.GradientStartEnd[1] = ClockCenterY;
					VGParameter.GradientStartEnd[2] = ClockCenterX;
					VGParameter.GradientStartEnd[3] = ClockCenterY;
					VGParameter.GradientStartEnd[4] = 476/2;					
 
				 GL_DrawingCircleFill( &VGParameter);	

	#if 1		//Grid 
				for(i=0;i<60;i++)
				{
				New_deg = i*6;
 
  			if(i%5 == 0)
  				{
  					  	CaculateCoordination(New_deg,	210 );
  							CoordXDest = CoordXSrc;
  							CoordYDest = CoordYSrc;
  							CaculateCoordination(New_deg,	225 );
					Stroke_JoinStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 8  , 0xF0F0F0ff);									// Indicator by Draw Line with Cap     
					}
				else
					{
  					  	CaculateCoordination(New_deg,	224 );
  							CoordXDest = CoordXSrc;
  							CoordYDest = CoordYSrc;
  							CaculateCoordination(New_deg,	225 );
  							
					Stroke_JoinStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 2  , 0x0000F0ff);									// Indicator by Draw Line with Cap  	
					}			
				}
  #endif
				RETW_GUI_ImportGlobalTimeMotion1();
 
				CaculateCoordination(	GlobalHourDeg, 140);
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
				New_deg = CaculateIncDeg(GlobalHourDeg,180);
				CaculateCoordination(	New_deg, 20);							
     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 12, 0xF0F0F0ff);									// Indicator by Draw Line with Cap			

				CaculateCoordination(	GlobalMinDeg, 180);
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
				New_deg = CaculateIncDeg(GlobalMinDeg,180);
				CaculateCoordination(	New_deg, 20);							
     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 12, 0xF0F0F0ff);									// Indicator by Draw Line with Cap			

				CaculateCoordination(	GlobalSecDeg, 185);
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
				New_deg = CaculateIncDeg(GlobalSecDeg,180);
				CaculateCoordination(	New_deg, 25);							
     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 2, 0x0000FFff);									// Indicator by Draw Line with Cap		
     		
#if 0
					Polycon_linearGradient[0] = ClockCenterX-8;
					Polycon_linearGradient[1] = ClockCenterY+8;
					Polycon_linearGradient[2] = ClockCenterX+8;
					Polycon_linearGradient[3] = ClockCenterY-8 ;					
					static VGfloat SrampStops1[15] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 		1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
					for(i=0;i<15;i++)
					Polycon_RampStopTab[i]=SrampStops1[i];
 
     		  TrVg_DrawingCircleLiearRadiantPara( ClockCenterX, ClockCenterY,16, 16, 2, 0xFFFFFFff,15 );

#else

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= 24;							/*	width			*/
					VGParameter.PathDrawingData[3]= 24;							/*	height		*/
 	
					static VGfloat SrampStops1[15] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 		1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
					for(i=0;i<15;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 15;
					
					VGParameter.GradientStartEnd[0] = ClockCenterX-12;
					VGParameter.GradientStartEnd[1] = ClockCenterY+12;
					VGParameter.GradientStartEnd[2] = ClockCenterX+12;
					VGParameter.GradientStartEnd[3] = ClockCenterY-12 ;
				 GL_DrawingCircleFill( &VGParameter);	
				 
#endif				 
				 
// 					RETW_GUI_ShowScriptNumber(0, 0,30,300,300,0xFFFFFFff);
 
//       		R_GUI_OpenVGFontDrawing();
//     		 	TrVg_DrawingText();
     		  			     		     		  
}

int RETW_GUI_Clock_2(void)
{
				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
				
					static uint32_t O1Outer = 360;
					static uint32_t O1Width = 8;
					static float O1Size;

					static uint32_t O2Outer;
					static uint32_t O2Width = 8;
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

	 			ClockCenterX= 200;
	 			ClockCenterY= 240;
 
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


					
#if 0
	 if(ClockShiftDir == 1)
	 	{
	 		ClockShiftX+=2;
	 		if(ClockShiftX>300)
	 			{
	 			ClockShiftDir=0;
	 			ClockShiftX=299;
	 			
	 			Userdef_OSTM0_GenRandomNum();
	 		}
	 	}
	 	else
	 	{
	 		ClockShiftX-=2;
	 		if(ClockShiftX<2)
	 			{
	 			ClockShiftDir=1;
	 			ClockShiftX=2;
	 		}
	 	}
	 	
	 	ClockCenterX= 400-150+ClockShiftX;
#endif


	 	 
//  			TrVg_DrawingCircleMode(0, ClockCenterX,ClockCenterY,470,470,1,0x006030ff);	
 
 			#if 1		//most inner surface

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O3Size;
					VGParameter.PathDrawingData[3]= O3Size;		
					
#if 0			//black and white
					static VGfloat SrampStops1[30] = { 	0.0f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							0.5f, 0.6f, 0.6f, 0.6f, 1.0f, 		
																							0.85f,0.8f, 0.8f, 0.8f, 1.0f,
																							0.9f, 0.1f, 0.1f, 0.1f, 1.0f, 		
																							0.96f,0.8f, 0.8f, 0.8f, 1.0f, 		
																							1.0f, 0.5f, 0.5f, 0.5f, 1.0f 																							
																							};
#endif																							
																							
					static VGfloat SrampStops1[10] = { 	0.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden
																							1.0f, 0.06f, 	0.25f, 	0.35f, 1.0f 								//dark golden															
																							};
 
					for(i=0;i<10;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 10;
					
					VGParameter.GradientStartEnd[0] = ClockCenterX;
					VGParameter.GradientStartEnd[1] = ClockCenterY;
					VGParameter.GradientStartEnd[2] = ClockCenterX;
					VGParameter.GradientStartEnd[3] = ClockCenterY+60;
					VGParameter.GradientStartEnd[4] = O3Size/2;
					
				  GL_DrawingCircleFill( &VGParameter);	
			#endif
			
			#if 1				//second inner

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O2Size;
					VGParameter.PathDrawingData[3]= O2Size;	
							
#if 0						//black and white
					static VGfloat SrampStops2[30] = { 	0.0f, 0.0f, 0.0f, 0.0f, 1.0f,							
																							0.2f, 0.3f, 0.3f, 0.3f, 1.0f,  		
																							0.4f, 1.0f, 1.0f, 1.0f, 1.0f, 
																							0.6f, 0.1f, 0.1f, 0.1f, 0.0f,		
																							0.8f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							1.0f, 1.0f, 1.0f, 1.0f, 1.0f 																							
																							};
#else																									
					static VGfloat SrampStops2[15] = { 	0.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden		
																							0.6f, 0.06f, 	0.25f, 	0.35f, 1.0f, 								//dark golden			
																							1.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden																								
																							};
#endif
									
					for(i=0;i<15;i++)
					VGParameter.GradientStep[i]=SrampStops2[i]; 
					VGParameter.GradientPoints = 15;		
												
					VGParameter.GradientStartEnd[0] = ClockCenterX-(O2Size*1.8/4);
					VGParameter.GradientStartEnd[1] = ClockCenterY+(O2Size*1.8/4);
					VGParameter.GradientStartEnd[2] = ClockCenterX+(O2Size*1.8/4);
					VGParameter.GradientStartEnd[3] = ClockCenterY-(O2Size*1.8/4);
					VGParameter.StrokeWidth =O2Width;
				 	GL_DrawingCircleNoFill( &VGParameter);					 
			#endif
			
			#if 1				//outer

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O1Size;
					VGParameter.PathDrawingData[3]= O1Size;	
#if 0			//black and white					
					static VGfloat SrampStops3[30] = { 	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																							0.2f, 0.3f, 0.3f, 0.3f, 1.0f,  		
																							0.4f, 0.1f, 0.1f, 0.1f, 0.0f,
																							0.6f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																							0.8f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							1.0f, 0.0f, 0.0f, 0.0f, 1.0f 																							
																							};
#else																									
					static VGfloat SrampStops3[25] = { 	0.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden		
																							0.2f, 0.06f, 	0.25f, 	0.35f, 1.0f, 								//dark golden		
																							0.5f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden		
																							0.8f, 0.06f, 	0.25f, 	0.35f, 1.0f, 								//dark golden			
																							1.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden																								
																							};
#endif
 				
					for(i=0;i<25;i++)
					VGParameter.GradientStep[i]=SrampStops3[i]; 
					VGParameter.GradientPoints = 25;	
 
					VGParameter.GradientStartEnd[0] = ClockCenterX-(O1Size*1.8/4);
					VGParameter.GradientStartEnd[1] = ClockCenterY+(O1Size*1.8/4);
					VGParameter.GradientStartEnd[2] = ClockCenterX+(O1Size*1.8/4);
					VGParameter.GradientStartEnd[3] = ClockCenterY-(O1Size*1.8/4);
					VGParameter.StrokeWidth =O1Width;
					
				 GL_DrawingCircleNoFill( &VGParameter);		 
 			#endif
 
#if 1
				RETW_GUI_ImportGlobalTimeMotion1();
#endif
 
#if 0
 				RETW_GUI_Clock_2_Grid_Golden(O3InnerR);
				RETW_GUI_Clock_Computer_1_Ind(O3InnerR);
				RETW_GUI_Clock_Computer_1_Cap();
#else



 				RETW_GUI_Clock_2_Grid_Golden(O3InnerR);
 				
				RETW_GUI_Clock_4p_Ind_GOLDEN(O3InnerR);
				
#endif

				RETW_GUI_Clock_2_Right();
 
}

int RETW_GUI_Clock_2_Right(void)
{
				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
				
					static uint32_t O1Outer = 360;
					static uint32_t O1Width = 8;
					static float O1Size;

					static uint32_t O2Outer;
					static uint32_t O2Width = 8;
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

	 			ClockCenterX= 600;
	 			ClockCenterY= 240;
 
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
 
 			#if 1		//most inner surface

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O3Size;
					VGParameter.PathDrawingData[3]= O3Size;		
					
#if 0			//black and white
					static VGfloat SrampStops1[30] = { 	0.0f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							0.5f, 0.6f, 0.6f, 0.6f, 1.0f, 		
																							0.85f,0.8f, 0.8f, 0.8f, 1.0f,
																							0.9f, 0.1f, 0.1f, 0.1f, 1.0f, 		
																							0.96f,0.8f, 0.8f, 0.8f, 1.0f, 		
																							1.0f, 0.5f, 0.5f, 0.5f, 1.0f 																							
																							};
#endif																							
																							
					static VGfloat SrampStops1[10] = { 	0.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden
																							1.0f, 0.06f, 	0.25f, 	0.35f, 1.0f 								//dark golden															
																							};
 
					for(i=0;i<10;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 10;
					
					VGParameter.GradientStartEnd[0] = ClockCenterX;
					VGParameter.GradientStartEnd[1] = ClockCenterY;
					VGParameter.GradientStartEnd[2] = ClockCenterX;
					VGParameter.GradientStartEnd[3] = ClockCenterY;
					VGParameter.GradientStartEnd[4] = O3Size/2;
					
				  GL_DrawingCircleFill( &VGParameter);	
			#endif
			
			#if 1				//second inner

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O2Size;
					VGParameter.PathDrawingData[3]= O2Size;	
							
#if 0						//black and white
					static VGfloat SrampStops2[30] = { 	0.0f, 0.0f, 0.0f, 0.0f, 1.0f,							
																							0.2f, 0.3f, 0.3f, 0.3f, 1.0f,  		
																							0.4f, 1.0f, 1.0f, 1.0f, 1.0f, 
																							0.6f, 0.1f, 0.1f, 0.1f, 0.0f,		
																							0.8f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							1.0f, 1.0f, 1.0f, 1.0f, 1.0f 																							
																							};
#else																									
					static VGfloat SrampStops2[15] = { 	0.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden		
																							0.6f, 0.06f, 	0.25f, 	0.35f, 1.0f, 								//dark golden			
																							1.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden																								
																							};
#endif
									
					for(i=0;i<15;i++)
					VGParameter.GradientStep[i]=SrampStops2[i]; 
					VGParameter.GradientPoints = 15;		
												
					VGParameter.GradientStartEnd[0] = ClockCenterX-(O2Size*1.8/4);
					VGParameter.GradientStartEnd[1] = ClockCenterY+(O2Size*1.8/4);
					VGParameter.GradientStartEnd[2] = ClockCenterX+(O2Size*1.8/4);
					VGParameter.GradientStartEnd[3] = ClockCenterY-(O2Size*1.8/4);
					VGParameter.StrokeWidth =O2Width;
				 	GL_DrawingCircleNoFill( &VGParameter);					 
			#endif
			
			#if 1				//outer

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= ClockCenterX;
					VGParameter.PathDrawingData[1]= ClockCenterY;
					VGParameter.PathDrawingData[2]= O1Size;
					VGParameter.PathDrawingData[3]= O1Size;	
#if 0			//black and white					
					static VGfloat SrampStops3[30] = { 	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																							0.2f, 0.3f, 0.3f, 0.3f, 1.0f,  		
																							0.4f, 0.1f, 0.1f, 0.1f, 0.0f,
																							0.6f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																							0.8f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																							1.0f, 0.0f, 0.0f, 0.0f, 1.0f 																							
																							};
#else																									
					static VGfloat SrampStops3[25] = { 	0.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden		
																							0.2f, 0.06f, 	0.25f, 	0.35f, 1.0f, 								//dark golden		
																							0.5f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden		
																							0.8f, 0.06f, 	0.25f, 	0.35f, 1.0f, 								//dark golden			
																							1.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden																								
																							};
#endif
 				
					for(i=0;i<25;i++)
					VGParameter.GradientStep[i]=SrampStops3[i]; 
					VGParameter.GradientPoints = 25;	
 
					VGParameter.GradientStartEnd[0] = ClockCenterX-(O1Size*1.8/4);
					VGParameter.GradientStartEnd[1] = ClockCenterY+(O1Size*1.8/4);
					VGParameter.GradientStartEnd[2] = ClockCenterX+(O1Size*1.8/4);
					VGParameter.GradientStartEnd[3] = ClockCenterY-(O1Size*1.8/4);
					VGParameter.StrokeWidth =O1Width;
					
				 GL_DrawingCircleNoFill( &VGParameter);		 
 			#endif
 
				vgFinish();
#if 1
				RETW_GUI_ImportGlobalTime();
#endif
 
#if 0
 				RETW_GUI_Clock_2_Grid_Golden(O3InnerR);
				RETW_GUI_Clock_Computer_1_Ind(O3InnerR);
				RETW_GUI_Clock_Computer_1_Cap();
#else
 	
 				RETW_GUI_Clock_2_Grid_Temperature(O3InnerR);
//				RETW_GUI_Clock_4p_Ind_GOLDEN(O3InnerR);
				
#endif

#if 1
				ClockCenterX +=30;
				RETW_GUI_Clock_4p_Ind_GOLDEN_Single(130, 90);
				
				ClockCenterX -=60;
				RETW_GUI_Clock_4p_Ind_GOLDEN_Single(130, 300);		
	
				ClockCenterX =600;
				ClockCenterY =240;
  			
  			G_CharScale=2.0;	
  			G_CharShearX =0.3;
				G_CharShearY =0.0;
				
  			RETW_GUI_DrawScriptNumber(	ClockCenterX-5, ClockCenterY+60, 20, 24, 0x000000FF, 1);
  			G_CharScale=0.6;
 				R_TXTR_DrawStringMain( "o" , 1, ClockCenterX+25, ClockCenterY+60+10, 0x000000FF);			
  			G_CharScale=2.0;
 				R_TXTR_DrawStringMain( "C" , 1, ClockCenterX+35, ClockCenterY+45, 0x000000FF);	 
  			G_CharScale=0.5;
 				R_TXTR_DrawStringMain( "Temperature" , 11, ClockCenterX-40, ClockCenterY+30, 0x000000FF);	 
 				
 				
  			G_CharScale=2.0;	
  			RETW_GUI_DrawScriptNumber(	ClockCenterX-10, ClockCenterY-80, 65, 24, 0x000000FF, 1);
 				R_TXTR_DrawStringMain( "%" , 1, ClockCenterX+20, ClockCenterY-90, 0x000000FF);	 
  			G_CharScale=0.5;
 				R_TXTR_DrawStringMain( "Humility" , 8, ClockCenterX-30, ClockCenterY-45, 0x000000FF);	 				 				
 				
 				
#endif				
						
 
}
int RETW_GUI_Clock_2_Grid_Golden(uint32_t MaxLength)
{
				uint32_t i,j,New_deg;

				for(i=0;i<60;i++)
				{
				New_deg = i*6;
  			CaculateCoordination(New_deg,	MaxLength );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
  			
  			if( (i%5) == 0)
  					{
  					CaculateCoordination(New_deg,	MaxLength-20 );
						GL_DrawingStroke( 5  , 0x1F7DB5FF,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						
  					CaculateCoordination(New_deg,	MaxLength-40 );
  				
  					G_CharScale=3.0 * MaxLength/470;
  				
  					G_CharShearX =0.5;
						G_CharShearY =0.0;
 
				
  					if(i==0)
  					RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, 12, 24, 0x1F7DB5FF, 1);
  					else
  					RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, i/5, 24, 0x1F7DB5FF, 1);						
 
						}
     		else
     				{
     				CaculateCoordination(New_deg,	MaxLength-12 );
						GL_DrawingStroke( 1  , 0x1AA6A1ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}   		
 				}					
}


int RETW_GUI_Clock_2_Grid_Temperature(uint32_t MaxLength)
{

				st_VGDrawArcParameter	ArcInfo;
				
				uint32_t i,j,New_deg;
				
				ClockCenterX=600;
				ClockCenterY=240;
				
#if 1					//right side temperature grid
				for(i=30;i<151;i++)
				{
				New_deg = i;

  			
  			if( (i%30) == 0)
  					{
  					CaculateCoordination(New_deg,	MaxLength );
  					CoordXDest = CoordXSrc;
  					CoordYDest = CoordYSrc;
  					CaculateCoordination(New_deg,	MaxLength-20 );
						GL_DrawingStroke( 5  , 0x1F7DB5ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
     		else if( i == 45  || i == 75 || i == 105 || i == 135)
     				{
  					CaculateCoordination(New_deg,	MaxLength );
  					CoordXDest = CoordXSrc;
  					CoordYDest = CoordYSrc;
     				CaculateCoordination(New_deg,	MaxLength-12 );
						GL_DrawingStroke( 1  , 0x1AA6A1ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}   		
 				}							
#endif

#if 1					//left side humility grid
				for(i=210;i<331;i++)
				{
				New_deg = i;
 
  			if( (i%30) == 0)
  					{
  					CaculateCoordination(New_deg,	MaxLength );
  					CoordXDest = CoordXSrc;
  					CoordYDest = CoordYSrc;
  					CaculateCoordination(New_deg,	MaxLength-20 );
						GL_DrawingStroke( 5  , 0x1F7DB5ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
     		else if( i == 45  || i == 75 || i == 105 || i == 135)
     				{
  					CaculateCoordination(New_deg,	MaxLength );
  					CoordXDest = CoordXSrc;
  					CoordYDest = CoordYSrc;
     				CaculateCoordination(New_deg,	MaxLength-12 );
						GL_DrawingStroke( 1  , 0x1AA6A1ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}   		
 				}							
#endif

 	
 #if 1				//right side temperature
 				ArcInfo.CenterX=		600;
 				ArcInfo.CenterY=		240;
 				ArcInfo.Width=			(MaxLength-20) *2;
 				ArcInfo.Height=			(MaxLength-20) *2;
  			ArcInfo.StartAngle= -60;
 				ArcInfo.AngleExtent= 120;
 				ArcInfo.ArcWidth=		10;

 					ArcInfo.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 													
					static VGfloat SrampStops1[15] = { 	0.0f, 0.6f, 0.0f, 0.0f, 1.0f, 		
																							0.6f, 0.0f, 0.6f, 0.0f, 1.0f, 		
																							1.00f, 0.0f, 0.0f, 0.6f, 1.0f, 																							
																							};									
					for(i=0;i<15;i++)
					ArcInfo.GradientStep[i]=SrampStops1[i]; 
					ArcInfo.GradientPoints = 15;
					
					ArcInfo.GradientStartEnd[0] = ArcInfo.CenterX;
					ArcInfo.GradientStartEnd[1] = ArcInfo.CenterY - ArcInfo.Height/2;
					ArcInfo.GradientStartEnd[2] = ArcInfo.CenterX; 
					ArcInfo.GradientStartEnd[3] = ArcInfo.CenterY + ArcInfo.Height/2;;;
 
 					ArcInfo.FillIt = 0;
 					ArcInfo.StrokeIt = 1; 	
 					
 			  	GL_DrawingArcPara(&ArcInfo);
 			  
 #endif

 #if 1				//left side humility
 				ArcInfo.CenterX=		600;
 				ArcInfo.CenterY=		240;
 				ArcInfo.Width=			(MaxLength-20) *2;
 				ArcInfo.Height=			(MaxLength-20) *2;
  			ArcInfo.StartAngle= 120;
 				ArcInfo.AngleExtent= 120;
 				ArcInfo.ArcWidth=		10;

 					ArcInfo.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 													
					static VGfloat SrampStops2[15] = { 	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																							0.4f, 0.6f, 0.6f, 0.6f, 1.0f, 		
																							1.00f, 0.0f, 0.0f, 0.0f, 1.0f, 																							
																							};									
					for(i=0;i<15;i++)
					ArcInfo.GradientStep[i]=SrampStops2[i]; 
					ArcInfo.GradientPoints = 15;
					
					ArcInfo.GradientStartEnd[0] = ArcInfo.CenterX;
					ArcInfo.GradientStartEnd[1] = ArcInfo.CenterY - ArcInfo.Height/2;
					ArcInfo.GradientStartEnd[2] = ArcInfo.CenterX; 
					ArcInfo.GradientStartEnd[3] = ArcInfo.CenterY + ArcInfo.Height/2;;;

 					ArcInfo.FillIt = 0;
 					ArcInfo.StrokeIt = 1; 	
 					
 			  	GL_DrawingArcPara(&ArcInfo);
 			  
 #endif


 
 					G_CharScale = 0.8;
//right side temperature 					
  				CaculateCoordination(30,	MaxLength-45);
   				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, 40, 24, 0x0000A0ff, 1);
  				CaculateCoordination(60,	MaxLength-45);
   				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, 30, 24, 0xA0A0A0ff, 1);
  				CaculateCoordination(90,	MaxLength-45);
   				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, 20, 24, 0xA0A0A0ff, 1);
  				CaculateCoordination(120,	MaxLength-45);
   				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, 10, 24, 0xA0A0A0ff, 1);
  				CaculateCoordination(150,	MaxLength-45);
   				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, 0, 24, 0xA00000ff, 1);   		
 	
//left side humility 

   				CaculateCoordination(210,	MaxLength-45);
   				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, 10, 24, 0xA0A0A0ff, 1);
   												R_TXTR_DrawStringMain("%", 1, CoordXSrc+12,  CoordYSrc-5, 0xA0A0A0ff);
  				CaculateCoordination(240,	MaxLength-45);
   				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, 30, 24, 0xA0A0A0ff, 1);
   				   							R_TXTR_DrawStringMain("%", 1, CoordXSrc+12,  CoordYSrc-5, 0xA0A0A0ff);
  				CaculateCoordination(270,	MaxLength-45);
   				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, 50, 24, 0xA0A0A0ff, 1);
   				   							R_TXTR_DrawStringMain("%", 1, CoordXSrc+12,  CoordYSrc-5, 0xA0A0A0ff);
  				CaculateCoordination(300,	MaxLength-45);
   				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, 70, 24, 0xA0A0A0ff, 1);
   				   							R_TXTR_DrawStringMain("%", 1, CoordXSrc+12,  CoordYSrc-5, 0xA0A0A0ff);
  				CaculateCoordination(330,	MaxLength-45);
   				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, 90, 24, 0xA0A0A0ff, 1);  
   				   							R_TXTR_DrawStringMain("%", 1, CoordXSrc+12,  CoordYSrc-5, 0xA0A0A0ff); 				
 
   						

   				
}


int RETW_GUI_Clock_Computer_1_Ind(uint32_t MaxLength)
{
				uint32_t IndiNum, i,j,k,New_deg,Indi_deg;
 
				st_VGDrawingParameter	VGParameter;
 
				static uint32_t IndicatorLoc[3][10]= {
																				 		{2,180-50,		 	180-25,20,		 180+25,20,		 360-2, 180-50,		0, 180-40},				//hour
																				 		{2,180-25,		 	180-15,20,		 180+15,20,		 360-1, 180-25,		0, 180-20},				//minute
																						{1,180-5,		 		180-10,20,		 180+10,20,		 360-1, 180-5,		0, 180}							//second
																					};
 
 						IndicatorLoc[0][0] = 2;
 						IndicatorLoc[0][1] = MaxLength-50;
 						IndicatorLoc[0][2] = 185-25;
 						IndicatorLoc[0][3] = 20;
 						IndicatorLoc[0][4] = 185+25;
 						IndicatorLoc[0][5] = 20;
 						IndicatorLoc[0][6] = 360-2;
 						IndicatorLoc[0][7] = MaxLength-50;
 						IndicatorLoc[0][8] = 0;
 						IndicatorLoc[0][9] = MaxLength-40;
 
 						IndicatorLoc[1][0] = 2;
 						IndicatorLoc[1][1] = MaxLength-25;
 						IndicatorLoc[1][2] = 185-15;
 						IndicatorLoc[1][3] = 20;
 						IndicatorLoc[1][4] = 185+15;
 						IndicatorLoc[1][5] = 20;
 						IndicatorLoc[1][6] = 360-1;
 						IndicatorLoc[1][7] = MaxLength-25;
 						IndicatorLoc[1][8] = 0;
 						IndicatorLoc[1][9] = MaxLength-20;
 						
 						IndicatorLoc[2][0] = 1;
 						IndicatorLoc[2][1] = MaxLength-5;
 						IndicatorLoc[2][2] = 185-10;
 						IndicatorLoc[2][3] = 20;
 						IndicatorLoc[2][4] = 185+10;
 						IndicatorLoc[2][5] = 20;
 						IndicatorLoc[2][6] = 360-1;
 						IndicatorLoc[2][7] = MaxLength-5;
 						IndicatorLoc[2][8] = 0;
 						IndicatorLoc[2][9] = MaxLength;
 
				for(IndiNum=0;IndiNum<3;IndiNum++)
				{
							if(IndiNum==0)
							Indi_deg=GlobalHourDeg;
							else if(IndiNum==1)
							Indi_deg=GlobalMinDeg;
							else if(IndiNum==2)
							Indi_deg=GlobalSecDeg;
 																				
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
							
							for(k=0;k<10;k+=2)
							{
 							New_deg = CaculateIncDeg(Indi_deg, IndicatorLoc[IndiNum][k]);
 							CaculateCoordination(	New_deg, IndicatorLoc[IndiNum][k+1]);
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;					
							}
							VGParameter.GradientStartEnd[2] = VGParameter.PathDrawingData[2];
							VGParameter.GradientStartEnd[3] = VGParameter.PathDrawingData[3];	
							VGParameter.GradientStartEnd[0] = VGParameter.PathDrawingData[8];
							VGParameter.GradientStartEnd[1] = VGParameter.PathDrawingData[9];	
/*
  VG_PAINT_TYPE_COLOR                         = 0x1B00,
  VG_PAINT_TYPE_LINEAR_GRADIENT               = 0x1B01,
  VG_PAINT_TYPE_RADIAL_GRADIENT               = 0x1B02,				
*/
 							VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;

							static VGfloat SrampStops[20] = { 0.0f, 0.0f, 0.0f, 0.1f, 1.0f, 		
																								0.1f, 0.5f, 0.5f, 0.5f, 1.0f, 		
																								0.2f, 0.3f, 0.3f, 0.3f, 1.0f, 		
																								1.0f, 0.0f, 0.0f, 0.0f, 1.0f };		
							for(i=0;i<20;i++)
							VGParameter.GradientStep[i]=SrampStops[i]; 
							VGParameter.GradientPoints = 20;		
							GL_DrawingPolyconFill( &VGParameter);	
							
				}
#if 0
 				New_deg = CaculateIncDeg(GlobalHourDeg,2);
 				CaculateCoordination(	New_deg, 130);
				VGParameter.PathDrawingData[j]  = CoordXSrc; 
				VGParameter.PathDrawingData[j+1] = CoordYSrc;
				VGParameter.PathPoints+=1;
				j+=2;
 
				New_deg = CaculateIncDeg(GlobalHourDeg,180-25);				
 				CaculateCoordination(	New_deg, 20);				
				VGParameter.PathDrawingData[j]  = CoordXSrc; 
				VGParameter.PathDrawingData[j+1] = CoordYSrc;
				VGParameter.PathPoints+=1;
				j+=2;

				New_deg = CaculateIncDeg(GlobalHourDeg,180+25);				
 				CaculateCoordination(	New_deg, 20);				
				VGParameter.PathDrawingData[j]  = CoordXSrc; 
				VGParameter.PathDrawingData[j+1] = CoordYSrc;
				VGParameter.PathPoints+=1;
				j+=2;

					VGParameter.GradientStartEnd[2] = CoordXSrc;
					VGParameter.GradientStartEnd[3] = CoordYSrc;
 
				New_deg = CaculateIncDeg(GlobalHourDeg,360-2);				
 				CaculateCoordination(	New_deg, 130);				
				VGParameter.PathDrawingData[j]  = CoordXSrc; 
				VGParameter.PathDrawingData[j+1] = CoordYSrc;
				VGParameter.PathPoints+=1;
				j+=2;
 
 				CaculateCoordination(	GlobalHourDeg,140);				
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
*/
  
 				VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;

					for(i=0;i<20;i++)
					VGParameter.GradientStep[i]=SrampStops[i]; 
					
					VGParameter.GradientPoints = 20;
					
				 GL_DrawingPolyconFill( &VGParameter);						
#endif //end of Hour
}

 
 
int RETW_GUI_Clock_RED_6p_Ind(uint32_t MaxLength)
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
				static VGfloat SrampStops[35] = { 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 		
																					0.4f, 0.3f, 0.3f, 0.8f, 1.0f, 		
																					0.45f, 0.2f, 0.2f, 0.9f, 1.0f, 	
																					0.5f, 0.0f, 0.0f, 1.0f, 1.0f,	
																					0.55f, 0.0f, 0.0f, 0.7f, 1.0f, 	
																					0.6f, 0.1f, 0.1f, 0.4f, 1.0f,	
																					1.0f, 0.5f, 0.5f, 0.2f, 1.0f };
				
 				i=0;		//Hour
						IndicatorLoc[i][0] = MaxLength-60;							//aa
						IndicatorLoc[i][1] = MaxLength-70;							//bb
						IndicatorLoc[i][2] = 5;													//cc
						IndicatorLoc[i][3] = 15;												//dd
						IndicatorLoc[i][4] = 20;												//ee
						IndicatorLoc[i][5] = 7;													//ff							
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
 
				for(IndiNum=0;IndiNum<3;IndiNum++)
				{
							if(IndiNum==0)
							Indi_deg=GlobalSecDeg;
							else if(IndiNum==1)
							Indi_deg=GlobalMinDeg;
							else if(IndiNum==2)
							Indi_deg=GlobalHourDeg;
 																				
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
 							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]/2);															 
 							New_deg = CaculateIncDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, 20 );									 
 							if(Indi_deg<180)
								{
								VGParameter.GradientStartEnd[2] = CoordXSrc;
								VGParameter.GradientStartEnd[3] = CoordYSrc;
							  }
							else
								{
								VGParameter.GradientStartEnd[0] = CoordXSrc;
								VGParameter.GradientStartEnd[1] = CoordYSrc;
							  }

 							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]/2);															 
 							New_deg = CaculateDecDeg(Indi_deg, 90);
 							CaculateCoordinationXYin(CoordXSrc,CoordYSrc,	New_deg, 20 );									 
 							if(Indi_deg<180)
								{
								VGParameter.GradientStartEnd[0] = CoordXSrc;
								VGParameter.GradientStartEnd[1] = CoordYSrc;
							  }
							else
								{
								VGParameter.GradientStartEnd[2] = CoordXSrc;
								VGParameter.GradientStartEnd[3] = CoordYSrc;
							  }							  
  
						#else
							VGParameter.GradientStartEnd[0] = VGParameter.PathDrawingData[0];
							VGParameter.GradientStartEnd[1] = VGParameter.PathDrawingData[1];	
							VGParameter.GradientStartEnd[2] = VGParameter.PathDrawingData[4];
							VGParameter.GradientStartEnd[3] = VGParameter.PathDrawingData[5];	
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
}

int RETW_GUI_Clock_4p_Ind_WHITE(uint32_t MaxLength)
{
/*				
				  																																		* 
				/	 																																		|				 
			 /	 																																		bb				 
			*    								  																									*  		    	*
			 \																																								 
			  \																																								 
			                                                                         * 
			   
			*--------------------------------------------aa-------------------------*
			   	 																																		*-----cc----*
			   																																			 
*/
				uint32_t IndiNum, i,j,k,New_deg,Indi_deg;
				st_VGDrawingParameter	VGParameter;
				
				static uint32_t IndicatorLoc[3][10]= {
																				 		{2,180-50,		 	180-25,20,		 180+25,20,		 360-2, 180-50,		0, 180-40},				//hour
																				 		{2,180-25,		 	180-15,20,		 180+15,20,		 360-1, 180-25,		0, 180-20},				//minute
																						{1,180-5,		 		180-10,20,		 180+10,20,		 360-1, 180-5,		0, 180}						//second
																						};
#if 0

				static VGfloat SrampStops[35] = { 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 		
																					0.4f, 0.3f, 0.3f, 0.8f, 1.0f, 		
																					0.45f, 0.2f, 0.2f, 0.9f, 1.0f, 	
																					0.5f, 0.0f, 0.0f, 1.0f, 1.0f,	
																					0.55f, 0.0f, 0.0f, 0.7f, 1.0f, 	
																					0.6f, 0.1f, 0.1f, 0.4f, 1.0f,	
																					1.0f, 0.5f, 0.5f, 0.2f, 1.0f };
#endif
	
 				i=0;		//Hour
						IndicatorLoc[i][0] = MaxLength-80;							//aa
						IndicatorLoc[i][1] = 15;												//bb
						IndicatorLoc[i][2] = 15;												//cc
 						
 				i=1;			//Minute
						IndicatorLoc[i][0] = MaxLength-35;							//aa
						IndicatorLoc[i][1] = 13;												//bb
						IndicatorLoc[i][2] = 20;												//cc
 					
 				i=2;			//Second
						IndicatorLoc[i][0] = MaxLength-5;								//aa
						IndicatorLoc[i][1] = 8;													//bb
						IndicatorLoc[i][2] = 50;												//cc
 
				for(IndiNum=0;IndiNum<3;IndiNum++)
				{
							if(IndiNum==0)
							Indi_deg=GlobalHourDeg_HRS;
							else if(IndiNum==1)
							Indi_deg=GlobalMinDeg_HRS;
							else if(IndiNum==2)
							Indi_deg=GlobalSecDeg_HRS;
 															
 															
 																				
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							CaculateCoordinationXYinFloat(ClockCenterX,ClockCenterY, Indi_deg, IndicatorLoc[IndiNum][0]);															//aa		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateIncDeg_Float(Indi_deg, 90*8);
							CaculateCoordinationXYinFloat(ClockCenterX,ClockCenterY, New_deg, IndicatorLoc[IndiNum][1]);															//bb		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateIncDeg_Float(Indi_deg, 180*8);																								
							CaculateCoordinationXYinFloat(ClockCenterX,ClockCenterY,	New_deg, IndicatorLoc[IndiNum][2]);																//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							
 							if(Indi_deg>180*8)
 							VGParameter.FillColor = 0xC0C0C000;
 							else
 							VGParameter.FillColor = 0x50505000;
 							
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);	
 
 
 
 
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
							
							CaculateCoordinationXYinFloat(ClockCenterX,ClockCenterY, Indi_deg, IndicatorLoc[IndiNum][0]);															//aa		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateDecDeg_Float(Indi_deg, 90*8);
							CaculateCoordinationXYinFloat(ClockCenterX,ClockCenterY, New_deg, IndicatorLoc[IndiNum][1]);															//bb		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateDecDeg_Float(Indi_deg, 180*8);																								
							CaculateCoordinationXYinFloat(ClockCenterX,ClockCenterY,	New_deg, IndicatorLoc[IndiNum][2]);																//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		

 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							
 							if(Indi_deg>180*8)
 							VGParameter.FillColor = 0x50505000;
 							else
 							VGParameter.FillColor = 0xC0C0C000;
 							
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);	
 
/*
  VG_PAINT_TYPE_COLOR                         = 0x1B00,
  VG_PAINT_TYPE_LINEAR_GRADIENT               = 0x1B01,
  VG_PAINT_TYPE_RADIAL_GRADIENT               = 0x1B02,				
*/
				}
}


int RETW_GUI_Clock_4p_Ind_GOLDEN(uint32_t MaxLength)
{
/*				
				  																																		* 
				/	 																																		|				 
			 /	 																																		bb				 
			*    								  																									*  		    	*
			 \																																								 
			  \																																								 
			                                                                         * 
			   
			*--------------------------------------------aa-------------------------*
			   	 																																		*-----cc----*
			   																																			 
*/
				uint32_t IndiNum, i,j,k,New_deg,Indi_deg;
				st_VGDrawingParameter	VGParameter;
				
				static uint32_t IndicatorLoc[3][10]= {
																				 		{2,180-50,		 	180-25,20,		 180+25,20,		 360-2, 180-50,		0, 180-40},				//hour
																				 		{2,180-25,		 	180-15,20,		 180+15,20,		 360-1, 180-25,		0, 180-20},				//minute
																						{1,180-5,		 		180-10,20,		 180+10,20,		 360-1, 180-5,		0, 180}						//second
																						};
 
 				i=0;		//Hour
						IndicatorLoc[i][0] = MaxLength-80;							//aa
						IndicatorLoc[i][1] = 8;												//bb
						IndicatorLoc[i][2] = 15;												//cc
 						
 				i=1;			//Minute
						IndicatorLoc[i][0] = MaxLength-35;							//aa
						IndicatorLoc[i][1] = 8;												//bb
						IndicatorLoc[i][2] = 20;												//cc
 					
 				i=2;			//Second
						IndicatorLoc[i][0] = MaxLength-5;								//aa
						IndicatorLoc[i][1] = 6;													//bb
						IndicatorLoc[i][2] = 50;												//cc
 
				for(IndiNum=0;IndiNum<3;IndiNum++)
				{
							if(IndiNum==0)
							Indi_deg=GlobalHourDeg;
							else if(IndiNum==1)
							Indi_deg=GlobalMinDeg;
							else if(IndiNum==2)
							Indi_deg=GlobalSecDeg;
 																				
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]);															//aa		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateIncDeg(Indi_deg, 90);
							CaculateCoordination(	New_deg, IndicatorLoc[IndiNum][1]);															//bb		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateIncDeg(Indi_deg, 180);																								
							CaculateCoordination(	New_deg, IndicatorLoc[IndiNum][2]);																//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							
 							if(Indi_deg>180)
 							VGParameter.FillColor = 0x1F7DB500;
 							else
 							//VGParameter.FillColor = 0x1A6FA100;
 							VGParameter.FillColor = 0x104F8100;
 							
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);	
							  
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
							
							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]);															//aa		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateDecDeg(Indi_deg, 90);
							CaculateCoordination(	New_deg, IndicatorLoc[IndiNum][1]);															//bb		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateIncDeg(Indi_deg, 180);																								
							CaculateCoordination(	New_deg, IndicatorLoc[IndiNum][2]);																//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		

 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							
 							if(Indi_deg>180)
 							VGParameter.FillColor = 0x104F8100;
 							else
 							VGParameter.FillColor = 0x1F7DB500;
 							
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);	
 
/*
  VG_PAINT_TYPE_COLOR                         = 0x1B00,
  VG_PAINT_TYPE_LINEAR_GRADIENT               = 0x1B01,
  VG_PAINT_TYPE_RADIAL_GRADIENT               = 0x1B02,				
*/
				}
}


int RETW_GUI_Clock_4p_Ind_GOLDEN_Single(uint32_t MaxLength, uint32_t Deg)
{
/*				
				  																																		* 
				/	 																																		|				 
			 /	 																																		bb				 
			*    								  																									*  		    	*
			 \																																								 
			  \																																								 
			                                                                         * 
			   
			*--------------------------------------------aa-------------------------*
			   	 																																		*-----cc----*
			   																																			 
*/
				uint32_t IndiNum, i,j,k,New_deg,Indi_deg;
				st_VGDrawingParameter	VGParameter;
				
				static uint32_t IndicatorLoc[1][10]= {
																				 		{2,180-50,		 	180-25,20,		 180+25,20,		 360-2, 180-50,		0, 180-40},				//hour
																						};
 
 							i=0;		//Hour
							IndicatorLoc[i][0] = MaxLength-40;							//aa
							IndicatorLoc[i][1] = 8;												//bb
							IndicatorLoc[i][2] = 15;												//cc
 
 							IndiNum=0;
							Indi_deg=Deg;
 																				
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]);															//aa		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateIncDeg(Indi_deg, 90);
							CaculateCoordination(	New_deg, IndicatorLoc[IndiNum][1]);															//bb		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateIncDeg(Indi_deg, 180);																								
							CaculateCoordination(	New_deg, IndicatorLoc[IndiNum][2]);																//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							
 							if(Indi_deg>180)
 							VGParameter.FillColor = 0x1F7DB500;
 							else
 							//VGParameter.FillColor = 0x1A6FA100;
 							VGParameter.FillColor = 0x104F8100;
 							
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);	
							  
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
							
							CaculateCoordination(	Indi_deg, IndicatorLoc[IndiNum][0]);															//aa		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateDecDeg(Indi_deg, 90);
							CaculateCoordination(	New_deg, IndicatorLoc[IndiNum][1]);															//bb		
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg = CaculateIncDeg(Indi_deg, 180);																								
							CaculateCoordination(	New_deg, IndicatorLoc[IndiNum][2]);																//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		

 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							
 							if(Indi_deg>180)
 							VGParameter.FillColor = 0x104F8100;
 							else
 							VGParameter.FillColor = 0x1F7DB500;
 							
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);	
 
/*
  VG_PAINT_TYPE_COLOR                         = 0x1B00,
  VG_PAINT_TYPE_LINEAR_GRADIENT               = 0x1B01,
  VG_PAINT_TYPE_RADIAL_GRADIENT               = 0x1B02,				
*/
 
}







/*				
				 /--------------                                   ----------------------------------/
				/	|						/																		/																 	/|
			 /	cc				 *-----------------------------------                                  / ff
			*    								  																									*  		    	*
			 \						 *-----------------------------------				                           \
			  \							\																		\																  \	
			   \-------------                                    ----------------------------------\
			   
			*--------------------------------------------aa-------------------------*
			   	*----------------------------------------bb-------------------------*-----dd-------*
			   																																			*-------ee--*
*/



 
int RETW_GUI_Clock_3(void)
{
					int i,j,k;
 
					char TempString[16];
					
				st_VGDrawingParameter	VGParameter;
 
//right down side button demo
				VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
				VGParameter.PathDrawingData[0]= 660;							//org coord X
				VGParameter.PathDrawingData[1]= 30;								//org coord Y
				VGParameter.PathDrawingData[2]= 120;							//Width
				VGParameter.PathDrawingData[3]= 50;								//Height
				VGParameter.PathDrawingData[4]= 5;								//Corner X
				VGParameter.PathDrawingData[5]= 5;								//Corner Y				
 				
 				VGParameter.GradientStartEnd[0]=720;
 				VGParameter.GradientStartEnd[1]=80; 
 				VGParameter.GradientStartEnd[2]=720;
 				VGParameter.GradientStartEnd[3]=30; 

					static VGfloat SrampStops1[25] = { 	0.0f, 0.1f, 0.1f, 0.1f, 1.0f, 		
																							0.1f, 0.4f, 0.4f, 0.4f, 1.0f, 		
																							0.3f, 0.7f, 0.5f, 0.5f, 1.0f,				
																							0.6f, 0.4f, 0.1f, 0.1f, 1.0f, 
																							1.0f, 0.1f, 0.0f, 0.0f, 1.0f																
																							};									
					for(i=0;i<25;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 25;
 
 
					VGParameter.StrokeIt=0;
					VGParameter.FillIt=1; 
					GL_DrawingRoundRectangle(&VGParameter);								//fill 

	
					VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
					VGParameter.StrokeWidth = 10;
					VGParameter.FillColor = 0x80000000;
					VGParameter.AlphaColor = 0x000000ff;
					VGParameter.StrokeIt=1;
					VGParameter.FillIt=0; 
					GL_DrawingRoundRectangle(&VGParameter);								//stroke						
				
//Center Block

				VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
				VGParameter.PathDrawingData[0]= (800-760)/2;				//org coord X
				VGParameter.PathDrawingData[1]= (480-240)/2;				//org coord Y
				VGParameter.PathDrawingData[2]= 760;								//Width
				VGParameter.PathDrawingData[3]= 240;								//Height
				VGParameter.PathDrawingData[4]= 20;									//Corner X
				VGParameter.PathDrawingData[5]= 20;									//Corner Y				

 					VGParameter.FillColor = 0x1f7db500;								//golden
 					VGParameter.FillColor = 0x1a6fa100;								//dark golden
//					VGParameter.FillColor = 0x00400000;								//dark green				
//					VGParameter.FillColor = 0x40000000;								//dark blue			
// 						VGParameter.FillColor = 0x00004000;								//dark red		

 					VGParameter.FillColor = 0x006f0100;								//dark green
 					
 																						
					VGParameter.AlphaColor = 0x000000ff;
					VGParameter.StrokeIt=0;
					VGParameter.FillIt=1; 
					GL_DrawingRoundRectangle(&VGParameter);								//stroke		


				VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 				VGParameter.GradientStartEnd[0]= 400;
 				VGParameter.GradientStartEnd[1]=480/2+240/2; 
 				VGParameter.GradientStartEnd[2]= 400;
 				VGParameter.GradientStartEnd[3]=480/2-240/2; 

					static VGfloat SrampStops2[30] = { 	0.0f, 0.2f, 0.2f, 0.2f, 1.0f, 		
																				 			0.2f, 0.4f, 0.4f, 0.4f, 1.0f, 		
																							0.3f, 0.8f, 0.8f, 0.8f, 1.0f, 			
																							0.6f, 0.6f, 0.6f, 0.6f, 1.0f, 
																							0.8f, 0.4f, 0.4f, 0.4f, 1.0f,
																							1.0f, 0.1f, 0.1f, 0.1f, 1.0f																
																							};									
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops2[i]; 
					VGParameter.GradientPoints = 30;
 
					VGParameter.StrokeWidth = 50;
					VGParameter.StrokeIt=1;
					VGParameter.FillIt=0; 
					GL_DrawingRoundRectangle(&VGParameter);								//stroke							
 
// Display Clock hour:minute:second
									G_CharShearX=0.1;
									G_CharShearY=0.0;
 									G_CharScale = 7.0;	
							k=0;
 					 
  													TempString[k] = (char) (((SYSTEMTIME.Hour >> 4)& 0x000F) + 0x30) ;	
  													k+=1;																	
  													TempString[k] = (char) (((SYSTEMTIME.Hour     )& 0x000F) + 0x30) ;	
  													k+=1;	
 															TempString[k]= ':';
 															k+=1;

  													TempString[k] = (char) (((SYSTEMTIME.Minute >> 4)& 0x000F) + 0x30) ;	
  													k+=1;																	
  													TempString[k] = (char) (((SYSTEMTIME.Minute     )& 0x000F) + 0x30) ;	
  													k+=1;	
 															TempString[k]= ':';
 															k+=1;

  													TempString[k] = (char) (((SYSTEMTIME.Second >> 4)& 0x000F) + 0x30) ;	
  													k+=1;																	
  													TempString[k] = (char) (((SYSTEMTIME.Second     )& 0x000F) + 0x30) ;	
  													k+=1;	
 
 									R_TXTR_DrawStringMain( &TempString[0], k, 75, 180, 0xC0C0C0FF); 
 
 #if 0	 			 
  			TrVg_DrawingCircleMode(0, ClockCenterX,ClockCenterY,470,470,1,0x006030ff);	
  			
  			TrVg_DrawingCircleNoFill(ClockCenterX,ClockCenterY,470,470,10, 0xff0000ff);					//BGRa
 				TrVg_DrawingCircleBatch(ClockCenterX,ClockCenterY,470,470,0x006030ff);
 
 				TrVg_Drawing12Circle();
 
 				GUI_DrawPolyconStyleIndicatorHour(GlobalHourDeg, 		140, 8 , 0xC00000ff);
 				GUI_DrawPolyconStyleIndicatorMin(GlobalMinDeg,			190, 1.6 , 0x00C000ff);
 				GUI_DrawCapStyleIndicator(GlobalSecDeg,							200, 1, 0x0000FFff);
 #endif

}
 

 	
int TrVg_Drawing12Circle()
{
				int i;
				VGuint New_deg;

/*
  VG_JOIN_MITER                               = 0x1800,
  VG_JOIN_ROUND                               = 0x1801,
  VG_JOIN_BEVEL                               = 0x1802,
*/
  
#if 0	
				for(i=0;i<60;i++)
				{
				New_deg = i*6;

  			CaculateCoordination(New_deg,	218 );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
  			CaculateCoordination(New_deg,	200 );
  			if(  (i%5) == 0)
				Stroke_JoinStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 4  , 0xF00000ff);									// Indicator by Draw Line with Cap    
     		else
				Stroke_JoinStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 1  , 0xF00000ff);									// Indicator by Draw Line with Cap       		
 			
 			}
#else
				for(i=0;i<60;i++)
				{
				New_deg = i*6;

  			CaculateCoordination(New_deg,	218 );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
  			CaculateCoordination(New_deg,	200 );
  			if(  (i%5) == 0)
				GL_DrawingStroke( 4  , 0xFF00FFff,VG_JOIN_BEVEL);									// Indicator by Draw Line with Cap    
     		else
				GL_DrawingStroke( 1  , 0xF00000ff,VG_JOIN_BEVEL);									// Indicator by Draw Line with Cap       		
 			
 			}


#endif
 			
 			
 			
 			
     				
#if 0			
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
 
 
					linearGradient[0] = ClockCenterX-164;
					linearGradient[1] = ClockCenterY+164;
					linearGradient[2] = ClockCenterX+164;
					linearGradient[3] = ClockCenterY-164 ;					
					VGfloat SrampStops[15] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		0.3f, 0.5f, 0.5f, 0.5f, 1.0f, 		1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
					for(i=0;i<15;i++)
					rampStops[i]=SrampStops[i];
					vgSetf( VG_STROKE_LINE_WIDTH, 8.0f ) ;
					TrVg_DrawingCircleMode(2, ClockCenterX,ClockCenterY,460,460,1,0xFF0030ff);	
					}
#endif
}
 