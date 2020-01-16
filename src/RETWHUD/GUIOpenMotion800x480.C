
#include    "retwhmi.h"

#if (  SYSTEM_DISPLAY==SYSdef_DSP_800480 )

//												percent,B,G,R,alpha								percent,B,G,R,alpha								percent,B,G,R,alpha

#define 		MSStep1			(1000)						//Show Renesas Logo
#define 		MSStep2 		(1000)						  //Right Gear RZ/A1H Enlarge/Shrinked
#define 		MSStep3			(1000)						  //Left Gear OpenVG Enlarge/Shrinked
#define 		MSStep4			(1000)						  //Two gear rotation

#define 		MSStep41			(500)						//Jump down
#define 		MSStep42			(500)						//Jump Up
#define 		MSStep43			(500)						//Drop down

#define 		MSStep5			(500)						//Two gear change to meter and move to left and right
#define 		MSStep6			(500)						//Main Meter start up
#define 		MSStep7			(200)						//Wait for 2 fps at least
#define 		MSStep8			(500)						 
#define 		MSStep9			(200)						 
#define 		MSStepA			(200)						 

#define 		MotionUpto1			(MSStep1)
#define 		MotionUpto2 		(MSStep2	+MotionUpto1)
#define 		MotionUpto3			(MSStep3	+MotionUpto2)
#define 		MotionUpto4			(MSStep4	+MotionUpto3)

#define 		MotionUpto41			(MSStep41	+MotionUpto4)
#define 		MotionUpto42			(MSStep42	+MotionUpto41)
#define 		MotionUpto43			(MSStep43	+MotionUpto42)

#define 		MotionUpto5			(MSStep5	+MotionUpto43)
#define 		MotionUpto6 		(MSStep6	+MotionUpto5)
#define 		MotionUpto7 		(MSStep7	+MotionUpto6)
#define 		MotionUpto8			(MSStep8	+MotionUpto7)
#define 		MotionUpto9			(MSStep9	+MotionUpto8)
#define 		MotionUptoA			(MSStepA	+MotionUpto9)

/*  Below parameter is for switch meters 0 to 2 , in different place */

#define 		SWMStep1			(500)						//Shrink 3 meters
#define 		SWMStep2 			(200)						//Rotate 3 meters		, this is wait only.	
#define 		SWMStep3			(200)						//Return 3 meters

#define 		SWMUpto1			(SWMStep1)
#define 		SWMUpto2 			(SWMStep2	+SWMUpto1)
#define 		SWMUpto3			(SWMStep3	+SWMUpto2) 

/*  Below parameter is for switch meters 0 to 2 , in different place */

#define 		MOTIONBackColor			0x100000ff

#define			GearNum						15	
#define			GearNum_TriAngle	12	
#define			GearNum_Knife			12	

static uint32_t LastSize;
 
static uint32_t CurrentMainMeter	=	0;
static uint32_t MainMeterChange		=	0;

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
int RETW_GUI_OPENMOTION_0_Mobj_Initial(int ObjNum)
{
				int i = ObjNum;
				MeterObj[i].GridStart = 225;						//for later motion use
				MeterObj[i].GridExtent = 270;
				
				VirtualEngine.IndicatorSpeed 	=0;
				VirtualEngine.IndicatorRPM 		=0;			
				VirtualEngine.IndicatorGas 		=0;
					
				VirtualEngine.Temperature 	=0;
				VirtualEngine.GasolineLevel =0;
				VirtualEngine.KMTotal				=123456;
				VirtualEngine.KMsub					=0;
				VirtualEngine.SpeedLimited 	=0;
}

int RETW_GUI_OPENMOTION_0_Mobj_Refresh(int ObjNum)
{
		int i = ObjNum;
/* Input is 
				MeterObj[i].CenterX=400;
				MeterObj[i].CenterY=240;
				MeterObj[i].O1Outer=	470;
*/
				MeterObj[i].CenterXShift=MeterObj[i].CenterX;
				MeterObj[i].CenterYShift=MeterObj[i].CenterY;	

if(	ObjNum==0)
				{
				MeterObj[i].O1Width = MeterObj[i].O1Outer*2/100;
				MeterObj[i].O2Width = MeterObj[i].O1Outer*3/100;				
				MeterObj[i].O3Width = MeterObj[i].O1Outer*4/100;								
				MeterObj[i].O4Width = MeterObj[i].O1Outer*16/100; 
				}
else 
				{
				MeterObj[i].O1Width = MeterObj[i].O1Outer*2/100;
				MeterObj[i].O2Width = MeterObj[i].O1Outer*4/100;				
				MeterObj[i].O3Width = MeterObj[i].O1Outer*6/100;								
				MeterObj[i].O4Width = MeterObj[i].O1Outer*18/100; 					
				}		
				
			
 				MeterObj[i].O2Outer = MeterObj[i].O1Outer-MeterObj[i].O1Width*2;
 				
if(	ObjNum==0)
				MeterObj[i].O3Outer = MeterObj[i].O2Outer-MeterObj[i].O2Width*2 -4;
else
				MeterObj[i].O3Outer = MeterObj[i].O2Outer-MeterObj[i].O2Width*2;

				MeterObj[i].O4Outer = MeterObj[i].O3Outer-MeterObj[i].O3Width*2; 
				MeterObj[i].O5Outer = MeterObj[i].O4Outer-MeterObj[i].O4Width*2;			
				
 				MeterObj[i].O1Size = MeterObj[i].O1Outer-MeterObj[i].O1Width;						
				MeterObj[i].O2Size = MeterObj[i].O2Outer-MeterObj[i].O2Width;												
				MeterObj[i].O3Size = MeterObj[i].O3Outer-MeterObj[i].O3Width;	
				MeterObj[i].O4Size = MeterObj[i].O4Outer-MeterObj[i].O4Width;
												
				MeterObj[i].O1InnerR = (MeterObj[i].O3Outer-MeterObj[i].O3Width*2)/2; 
				MeterObj[i].O2InnerR = (MeterObj[i].O3Outer-MeterObj[i].O3Width*2)/2; 
				MeterObj[i].O3InnerR = (MeterObj[i].O3Outer-MeterObj[i].O3Width*2)/2;
				MeterObj[i].O4InnerR = (MeterObj[i].O4Outer-MeterObj[i].O4Width*2)/2;		
				MeterObj[i].O5InnerR = MeterObj[i].O5Outer/2;		
				
}

int RETW_GUI_OPENMOTION_0_SWMETER_EVENT()
{	
			uint32_t 	i;
			uint32_t 	RotateSpeed;
			uint32_t	MotionSpeedF;
			
//			printf("CurrentMainMeter is %d\n", CurrentMainMeter);
		
			if(MainMeterChange==0)
				{
				CurrentMainMeter+=1;
				if(CurrentMainMeter==3)
				CurrentMainMeter=0;
				
				MainMeterChange=1;
				Milisec_SWMeterEntry=0;
				}
		#if 0
			else
				{
						MainMeterChange=0;
						CurrentMainMeter=0;
						MeterObj[0].CenterX = 400;
						MeterObj[0].CenterY = 240; 
						MeterObj[0].O1Outer = 460;

						MeterObj[1].CenterX = 100;
						MeterObj[1].CenterY = 480-100; 
						MeterObj[1].O1Outer = 200;

						MeterObj[2].CenterX = 700;
						MeterObj[2].CenterY = 480-100; 
						MeterObj[2].O1Outer = 200;
						 	
				}
		#endif	
				
}
int RETW_GUI_OPENMOTION_0( int speed, int Maxspeed )
{	
			uint32_t i;
			uint32_t RotateSpeed;
			uint32_t MotionSpeedF;
			
//Show Renesas Logo 
   	if(Milisec_ModeEntry < MotionUpto1)
   	{
							RETW_GUI_PoliconFillScreen(0,0,800,480,0xFFFFFFFF);	
							RETW_GUI_OPENMOTION_0_Mobj_Initial(0);
							
							vgFinish();
							DspBMP(_RUS_ADDR, (800-404)/2,(480-68)/2, VG_Back_FrameBuffer, 0,0);		

							DspBMP(_RZS_ADDR, (800-114)/2,(480-40)/2 + 100, VG_Back_FrameBuffer, 0,0);	
		}
//Right Gear RZ/A1H Enlarge/Shrinked
		else if(Milisec_ModeEntry < MotionUpto2)																			 
			{

						RETW_GUI_PoliconFillScreen(0,0,800,480,MOTIONBackColor);	
						
						if((Milisec_ModeEntry-MotionUpto1) < (MSStep2/2) )
							{
 								MeterObj[1].CenterX=400;
 								MeterObj[1].CenterY=240;
								MeterObj[1].O1Outer = (1000) *(Milisec_ModeEntry-MotionUpto1)/(MSStep2/2);														// getting bigger
								MeterObj[1].AngleShift =  (2880*4/GearNum) *(Milisec_ModeEntry-MotionUpto1)/(MSStep2/2);
								
								RETW_GUI_OPENMOTION_0_Gear( 1, 0xFF0000ff); 
							}
						else
							{
 								//moving to (234,240)
 								MeterObj[1].CenterX=234+ (400-234) * (MSStep2-(Milisec_ModeEntry-MotionUpto1))/(MSStep2/2);						//getting smaller
 								MeterObj[1].CenterY=240;							
								MeterObj[1].O1Outer = 185*2 + (1000-185*2) * (MSStep2-(Milisec_ModeEntry-MotionUpto1))/(MSStep2/2);						//getting smaller
								MeterObj[1].AngleShift =  (2880*4/GearNum) *(Milisec_ModeEntry-MotionUpto1)/(MSStep2/2);
								
								RETW_GUI_OPENMOTION_0_Gear( 1, 0xFF0000ff); 
							}			
// 					printf("MS2 %d\n", Milisec_ModeEntry);
			}
//Left Gear OpenVG Enlarge/Shrinked
		else if(Milisec_ModeEntry < MotionUpto3)												 
 		{
						RETW_GUI_PoliconFillScreen(0,0,800,480,MOTIONBackColor);	
						
						if((Milisec_ModeEntry-MotionUpto2) < (MSStep3/2) )
							{
 								MeterObj[2].CenterX=400;
 								MeterObj[2].CenterY=240;
								MeterObj[2].O1Outer = (1000) * (Milisec_ModeEntry-MotionUpto2)/(MSStep3/2);														// getting bigger
								MeterObj[2].AngleShift = (2880/GearNum)/2 + (2880*4/GearNum) *(Milisec_ModeEntry-MotionUpto2)/(MSStep3/2);
								
													RETW_GUI_OPENMOTION_0_Gear( 1, 0xFF0000ff); 
													
								RETW_GUI_OPENMOTION_0_Gear( 2, 0x00FF00ff); 
						}
						else
							{
 								//moving to (400+166,240)
 								MeterObj[2].CenterX=400+ (166) * (Milisec_ModeEntry-MotionUpto2-(MSStep3/2))/(MSStep3/2);											//getting bigger
 								MeterObj[2].CenterY=240;							
								MeterObj[2].O1Outer = 185*2 + (1000-185*2) * (MSStep3-(Milisec_ModeEntry-MotionUpto2))/(MSStep3/2);						//getting smaller
								MeterObj[2].AngleShift =  (2880/GearNum)/2 + (2880*4/GearNum) *(Milisec_ModeEntry-MotionUpto2)/(MSStep3/2);
								
													RETW_GUI_OPENMOTION_0_Gear( 1, 0xFF0000ff); 
								RETW_GUI_OPENMOTION_0_Gear( 2, 0x00FF00ff ); 
						}			
// 					printf("MS2 %d\n", Milisec_ModeEntry);
		}			
//Two gear rotation	 
		else if(Milisec_ModeEntry < MotionUpto4) 
 		{
						RETW_GUI_PoliconFillScreen(0,0,800,480,MOTIONBackColor);	
						vgFinish();
						
								RotateSpeed = (2880/GearNum)*(Milisec_ModeEntry-MotionUpto3)/(MSStep4);
						
								MeterObj[2].AngleShift = MeterObj[2].AngleShift + RotateSpeed;
								if(MeterObj[2].AngleShift > 2879)
								MeterObj[2].AngleShift = MeterObj[2].AngleShift-2880;
								
								MeterObj[1].AngleShift =   2880-MeterObj[2].AngleShift;
								
								RETW_GUI_OPENMOTION_0_Gear( 1, 0xFF0000ff); 
								vgFinish();
								RETW_GUI_OPENMOTION_0_Gear( 2, 0x00FF00ff ); 
								vgFinish();
		}	
//RZ Logo Jump Down
		else if(Milisec_ModeEntry < MotionUpto41) 						
 		{
						RETW_GUI_PoliconFillScreen(0,0,800,480,MOTIONBackColor);	
						vgFinish();
						MotionSpeedF = (2880)*(Milisec_ModeEntry-MotionUpto4)/(MSStep41);
						
//					printf("Dsp BMP MotionSpeedF %d\n", MotionSpeedF  ); 
//					DspBMP(_RZS_ADDR, (800-114)/2,(480-40)/2 + 100, VG_Back_FrameBuffer, 0,0);	
 
 						MeterObj[0].CenterX=(800-114)/2;
						MeterObj[0].CenterY=   480-40-  (480-40) * cos( PI*MotionSpeedF/2880/2)  ;
 
 // 				printf("Dsp BMP from %d\n", MeterObj[0].CenterY   ); 	
  					  						
 						DspBMP(_RZS_ADDR, MeterObj[0].CenterX , MeterObj[0].CenterY , VG_Back_FrameBuffer, 0,0);	

//					DspBMP(_RZS_ADDR, (800-114)/2,240, VG_Back_FrameBuffer, 0,0);	
 
		}	
//RZ Logo Jump bound Up
		else if(Milisec_ModeEntry < MotionUpto42) 
 		{
						RETW_GUI_PoliconFillScreen(0,0,800,480,MOTIONBackColor);	
						vgFinish();
						MotionSpeedF = (2880)*(Milisec_ModeEntry-MotionUpto41)/(MSStep42);
						
//					printf("Dsp BMP MotionSpeedF %d\n", MotionSpeedF  ); 
//					DspBMP(_RZS_ADDR, (800-114)/2,(480-40)/2 + 100, VG_Back_FrameBuffer, 0,0);	
 
 						MeterObj[0].CenterX=(800-114)/2;
						MeterObj[0].CenterY=  480-40-  (480-140) * sin( PI*MotionSpeedF/2880/2)  ;
 
 // 				printf("Dsp BMP from %d\n", MeterObj[0].CenterY   ); 	
  					  						
 						DspBMP(_RZS_ADDR, MeterObj[0].CenterX , MeterObj[0].CenterY , VG_Back_FrameBuffer, 0,0);	

//					DspBMP(_RZS_ADDR, (800-114)/2,240, VG_Back_FrameBuffer, 0,0);	
		}
//RZ Logo 2nd time Jump Down
		else if(Milisec_ModeEntry < MotionUpto43) 
 		{
						RETW_GUI_PoliconFillScreen(0,0,800,480,MOTIONBackColor);	
						vgFinish();
						MotionSpeedF = (2880)*(Milisec_ModeEntry-MotionUpto42)/(MSStep43);
						
//					printf("Dsp BMP MotionSpeedF %d\n", MotionSpeedF  ); 
//					DspBMP(_RZS_ADDR, (800-114)/2,(480-40)/2 + 100, VG_Back_FrameBuffer, 0,0);	
 
 						MeterObj[0].CenterX=(800-114)/2;
						MeterObj[0].CenterY=   480-40-  (480-140) * cos( PI*MotionSpeedF/2880/2)  ;
 
 // 				printf("Dsp BMP from %d\n", MeterObj[0].CenterY   ); 	
  					  						
 						DspBMP(_RZS_ADDR, MeterObj[0].CenterX , MeterObj[0].CenterY , VG_Back_FrameBuffer, 0,0);	

//					DspBMP(_RZS_ADDR, (800-114)/2,240, VG_Back_FrameBuffer, 0,0);	
		}
		
//Four meter move to left and right 4 corner
	
		else if(Milisec_ModeEntry < MotionUpto5)																 
 		{
 
 
						RETW_GUI_PoliconFillScreen(0,0,800,480,MOTIONBackColor);	
						
						MotionSpeedF = (2880)*(Milisec_ModeEntry-MotionUpto43)/(MSStep5);
			
					if(MotionSpeedF > (2880/4) )
					{
 						MeterObj[1].CenterX=400 - (400-100) * sin( PI*MotionSpeedF/2880/2); 
 						MeterObj[1].CenterY=0 + (480-100) * sin( PI*MotionSpeedF/2880/2); 
						MeterObj[1].O1Outer = 200 * sin( PI*MotionSpeedF/2880/2); 
//						MeterObj[1].AngleShift =  1440 * sin( PI*MotionSpeedF/2880/2); 
						
 						MeterObj[2].CenterX=400 + (400-100) * sin( PI*MotionSpeedF/2880/2); 
 						MeterObj[2].CenterY=0 + (480-100) * sin( PI*MotionSpeedF/2880/2); 
						MeterObj[2].O1Outer = 200 * sin( PI*MotionSpeedF/2880/2); 
//						MeterObj[2].AngleShift =  1440 * sin( PI*MotionSpeedF/2880/2); 
								
 						MeterObj[3].CenterX=400 - (400-100) * sin( PI*MotionSpeedF/2880/2); 
 						MeterObj[3].CenterY=0 + (100) * sin( PI*MotionSpeedF/2880/2); 
						MeterObj[3].O1Outer = 200 * sin( PI*MotionSpeedF/2880/2); 
//						MeterObj[3].AngleShift =  1440 * sin( PI*MotionSpeedF/2880/2); 
						
 						MeterObj[4].CenterX=400 + (400-100) * sin( PI*MotionSpeedF/2880/2); 
 						MeterObj[4].CenterY=0 + (100) * sin( PI*MotionSpeedF/2880/2); 
						MeterObj[4].O1Outer = 200 * sin( PI*MotionSpeedF/2880/2); 
//						MeterObj[4].AngleShift =  1440 * sin( PI*MotionSpeedF/2880/2); 
 								
      			RETW_GUI_OM_FRAME_METAL( 1 );
      			RETW_GUI_OM_FRAME_METAL( 2 );
    
      			RETW_GUI_OM_FRAME_CIRCLE( 3 );
   
      			RETW_GUI_OM_FRAME_METAL( 4 );
   			
      			RETW_GUI_OPENMOTION_0_GRID(1);
      			RETW_GUI_OPENMOTION_0_GRID(2);
      			RETW_GUI_OPENMOTION_0_GRID(3);
      			RETW_GUI_OPENMOTION_0_GRID(4);  

      			RETW_GUI_OPENMOTION_0_NUMBER(1);
      			RETW_GUI_OPENMOTION_0_NUMBER(2);
      			RETW_GUI_OPENMOTION_0_NUMBER(3);
      			RETW_GUI_OPENMOTION_0_NUMBER(4);
      		}
	  }
	  
//Main Meter start up
	  
		else if(Milisec_ModeEntry < MotionUpto6)
 		{
 			
// 					printf("MotionUpto6-1\n");
					RETW_GUI_PoliconFillScreen(0,0,800,480,MOTIONBackColor);	
			#if 1 			
      			RETW_GUI_OM_FRAME_METAL( 1 );
      			RETW_GUI_OM_FRAME_METAL( 2 );
       			RETW_GUI_OM_FRAME_CIRCLE( 3 );
      			RETW_GUI_OM_FRAME_METAL( 4 );

      			RETW_GUI_OPENMOTION_0_GRID(1);
      			RETW_GUI_OPENMOTION_0_GRID(2);
       			RETW_GUI_OPENMOTION_0_GRID(3);
      			RETW_GUI_OPENMOTION_0_GRID(4); 

       			RETW_GUI_OPENMOTION_0_NUMBER(1);
       			RETW_GUI_OPENMOTION_0_NUMBER(2);
       			RETW_GUI_OPENMOTION_0_NUMBER(3);
       			RETW_GUI_OPENMOTION_0_NUMBER(4);
 			#endif

// 					printf("MotionUpto6-2\n");
//				ClockCenterX=400;
// 				ClockCenterY=240; 			
 			
 				MeterObj[0].CenterX = 400;
 				MeterObj[0].CenterY = 240 + 180 * sin( PI* (Milisec_ModeEntry-MotionUpto5)/MSStep6); 			
 
// 			LastSize = 70+(Milisec_ModeEntry-MotionUpto5)*400/MSStep6;
 				LastSize = 60+ 400* sin( PI* (Milisec_ModeEntry-MotionUpto5)/MSStep6/2  );
 
 				
 				MeterObj[0].O1Outer = LastSize;
 				
				RETW_GUI_OM_FRAME_METAL(0);
 

 				RETW_GUI_OPENMOTION_0_REFRESH_0(0,speed);
 				RETW_GUI_OPENMOTION_0_REFRESH_1(1,speed);
 				RETW_GUI_OPENMOTION_0_REFRESH_2(2,speed);
 				RETW_GUI_OPENMOTION_0_REFRESH_3(3,speed);				
 				RETW_GUI_OPENMOTION_0_REFRESH_4(4,speed);	
 
// 					printf("MotionUpto6-6\n"); 
								
	  }	  
//Wait 2 frame at least for 2 frame background
		else if(Milisec_ModeEntry < MotionUpto7)
 		{
 
						RETW_GUI_PoliconFillScreen(0,0,800,480,MOTIONBackColor);	
			 			
      			RETW_GUI_OM_FRAME_METAL( 1 );
      			RETW_GUI_OM_FRAME_METAL( 2 );
       			RETW_GUI_OM_FRAME_CIRCLE( 3 );
      			RETW_GUI_OM_FRAME_METAL( 4 );

      			RETW_GUI_OPENMOTION_0_GRID(1);
      			RETW_GUI_OPENMOTION_0_GRID(2);
      			RETW_GUI_OPENMOTION_0_GRID(3);
      			RETW_GUI_OPENMOTION_0_GRID(4); 

      			RETW_GUI_OPENMOTION_0_NUMBER(1);
      			RETW_GUI_OPENMOTION_0_NUMBER(2);
      			RETW_GUI_OPENMOTION_0_NUMBER(3);
      			RETW_GUI_OPENMOTION_0_NUMBER(4);
 
//will make fps less than 60

 				MeterObj[0].CenterX = 400;
 				MeterObj[0].CenterY = 240;
 				
 				
      			RETW_GUI_OM_FRAME_METAL( 0 );
 						RETW_GUI_OPENMOTION_0_GRID(0);
 				
				RETW_GUI_OPENMOTION_0_REFRESH_0(0,speed);
				RETW_GUI_OPENMOTION_0_REFRESH_1(1,speed);
				RETW_GUI_OPENMOTION_0_REFRESH_2(2,speed);		
        RETW_GUI_OPENMOTION_0_REFRESH_3(3,speed);	
				RETW_GUI_OPENMOTION_0_REFRESH_4(4,speed);			
				
	  }	  
	  else
	  {
 			if( MainMeterChange>0)
 			{
 				RETW_GUI_OM_SWMETER_POS(CurrentMainMeter);
			}
			else
			{
			
 				if(ModeEntryCnt<3)
 				{
						RETW_GUI_PoliconFillScreen(0,0,800,480,MOTIONBackColor);	
      			RETW_GUI_OM_FRAME_METAL( 1 );
      			RETW_GUI_OM_FRAME_METAL( 2 );
      			RETW_GUI_OM_FRAME_CIRCLE( 3 );
      			RETW_GUI_OM_FRAME_METAL( 4 );

						RETW_GUI_OPENMOTION_0_GRID(1);
						RETW_GUI_OPENMOTION_0_GRID(2);
						RETW_GUI_OPENMOTION_0_GRID(3);
						RETW_GUI_OPENMOTION_0_GRID(4);

      			RETW_GUI_OPENMOTION_0_NUMBER(1);
      			RETW_GUI_OPENMOTION_0_NUMBER(2);
      			RETW_GUI_OPENMOTION_0_NUMBER(3);
      			RETW_GUI_OPENMOTION_0_NUMBER(4);
      			
      			RETW_GUI_OM_FRAME_METAL(0);
      			RETW_GUI_OM_FRAME_O3( 0 );	
						RETW_GUI_OPENMOTION_0_GRID(0);
      			RETW_GUI_OPENMOTION_0_NUMBER(0);
 				}
 
//				RETW_GUI_OM_FRAME_O3( 0 );								
// 				RETW_GUI_OPENMOTION_0_GRID(0);											//will make fps less than 60
				
				RETW_GUI_OPENMOTION_0_REFRESH_0(0,speed);
				RETW_GUI_OPENMOTION_0_REFRESH_1(1,speed);
				RETW_GUI_OPENMOTION_0_REFRESH_2(2,speed);
        RETW_GUI_OPENMOTION_0_REFRESH_3(3,speed);	
				RETW_GUI_OPENMOTION_0_REFRESH_4(4,speed);	
 
//  		TrVg_DrawingChineseText();
//  			TrVg_DrawingChineseTextString( VirtualEngine.TargetRPM  );
// 				TrVg_DrawingChineseTextString( );
#if 0
					for(i=0;i<1;i++)
					RETW_DrawingChineseTextString(i,400,360-30*i);
#endif					

					RETW_Demo();

					
			}
  	}				
}
void RETW_GUI_OM_SWMETER_POS()
{
			uint32_t i,j,swtomode;
			uint32_t 	RotateSpeed;
			uint32_t	MotionSpeedF;
			uint32_t  Meter0X,Meter0Y,Meter1X,Meter1Y,Meter2X,Meter2Y;

			j = CurrentMainMeter;
 
//Show Renesas Logo 
   	if(Milisec_SWMeterEntry < SWMUpto1)
   	{
//   			printf("Enter SWMUpto1-1\n");
				MotionSpeedF = (2880)*Milisec_SWMeterEntry/(SWMStep1);
 				swtomode = j;

 				MeterObj[swtomode].CenterX=  100 + (400-100) * sin( PI*MotionSpeedF/2880/2); 
 				MeterObj[swtomode].CenterY=  (480-100) - (240-100) * sin( PI*MotionSpeedF/2880/2); 
				MeterObj[swtomode].O1Outer = 200 +(470-200)  * sin( PI*MotionSpeedF/2880/2); 
		
 				swtomode = j+1;
 				if(swtomode > 2)
 				swtomode =swtomode-3;								

 				MeterObj[swtomode].CenterX= 700 - (700-100) * sin( PI*MotionSpeedF/2880/2); 
 				MeterObj[swtomode].CenterY= (480-100);
				MeterObj[swtomode].O1Outer = 200;

 				swtomode = j+2;
 				if(swtomode > 2)
 				swtomode =swtomode-3;			

 				MeterObj[swtomode].CenterX= 400 + (700-400) * sin( PI*MotionSpeedF/2880/2); 
 				MeterObj[swtomode].CenterY= 240 + (240-100) * sin( PI*MotionSpeedF/2880/2); 
				MeterObj[swtomode].O1Outer = 470 -(470-200)* sin( PI*MotionSpeedF/2880/2); 
 
//   			printf("Enter SWMUpto1-2\n");
   			
				RETW_GUI_PoliconFillScreen(0,0,800,480,MOTIONBackColor);	

//   			printf("Enter SWMUpto1-3\n");
 		
				for(i=0;i<5;i++)
						RETW_GUI_OPENMOTION_0_Mobj_Refresh(i);
						
      			RETW_GUI_OM_FRAME_METAL( 0 );						
      			RETW_GUI_OM_FRAME_METAL( 1 );	
      			RETW_GUI_OM_FRAME_METAL( 2 );	
      			RETW_GUI_OM_FRAME_CIRCLE( 3 );	
      			RETW_GUI_OM_FRAME_METAL( 4 );	
      									
				for(i=0;i<5;i++)
 						RETW_GUI_OPENMOTION_0_GRID( i );
 
 						RETW_GUI_OPENMOTION_0_REFRESH_0(0,240);
 						RETW_GUI_OPENMOTION_0_REFRESH_1(1,240);
 						RETW_GUI_OPENMOTION_0_REFRESH_2(2,240);
 						RETW_GUI_OPENMOTION_0_REFRESH_3(3,240);
 						RETW_GUI_OPENMOTION_0_REFRESH_4(4,240);
 
 		}
   	else if(Milisec_SWMeterEntry < SWMUpto2)
 		{
				RETW_GUI_PoliconFillScreen(0,0,800,480,MOTIONBackColor);	
 		
				for(i=0;i<5;i++)
						RETW_GUI_OPENMOTION_0_Mobj_Refresh(i);
						
      			RETW_GUI_OM_FRAME_METAL( 0 );						
      			RETW_GUI_OM_FRAME_METAL( 1 );	
      			RETW_GUI_OM_FRAME_METAL( 2 );	
      			RETW_GUI_OM_FRAME_CIRCLE( 3 );	
      			RETW_GUI_OM_FRAME_METAL( 4 );	
      									
				for(i=0;i<5;i++)
 						RETW_GUI_OPENMOTION_0_GRID( i );
 					  
						RETW_GUI_OM_FRAME_METAL(CurrentMainMeter);
 						RETW_GUI_OPENMOTION_0_GRID( CurrentMainMeter );
 						
 						RETW_GUI_OPENMOTION_0_REFRESH_0(0,240);
 						RETW_GUI_OPENMOTION_0_REFRESH_1(1,240);
 						RETW_GUI_OPENMOTION_0_REFRESH_2(2,240);
 						RETW_GUI_OPENMOTION_0_REFRESH_3(3,240);
 						RETW_GUI_OPENMOTION_0_REFRESH_4(4,240);
 		}
 		else
 		
 			MainMeterChange=0;
 			
}


void RETW_GUI_OPENMOTION_0_FlashSignClean()
{
					uint32_t 	SignSizeX=150,SignSizeY=70;
					
   				RETW_GUI_PoliconFillScreen(80-(SignSizeX/2), 240-(SignSizeY/2),	SignSizeX,SignSizeY, MOTIONBackColor);	
   				RETW_GUI_PoliconFillScreen(800-80-(SignSizeX/2), 240-(SignSizeY/2),	SignSizeX,SignSizeY, MOTIONBackColor);		
   				RETW_GUI_PoliconFillScreen(0,0,800,16, MOTIONBackColor);	
}

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

int RETW_GUI_OPENMOTION_0_Gear(int Channel, int Color )
{
				uint32_t i,j,New_deg, Length;
				uint32_t LimiterSpeed;
 
				st_VGDrawingParameter	VGParameter;
 
				MeterObj[Channel].O1OuterR = MeterObj[Channel].O1Outer/2;
 
				VGfloat CharSize = MeterObj[Channel].O1Outer;
 
				
				CharSize = (float)CharSize/470;
				
 				VGParameter.PathPoints=0;
				j=0;													
						
				for(i=0;i<GearNum;i++)
				{
 							New_deg = i * (2880/GearNum);
 							New_deg += MeterObj[Channel].AngleShift;
 							if(New_deg>2879)
 							New_deg=New_deg-2880;
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg , MeterObj[Channel].O1OuterR*4/5);			 
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			

 							New_deg = i * (2880/GearNum)+ (2880/GearNum)*1/12; 		 
 							New_deg += MeterObj[Channel].AngleShift;
 							if(New_deg>2879)
 							New_deg=New_deg-2880;
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg , MeterObj[Channel].O1OuterR );			 
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;								

 							New_deg = i * (2880/GearNum)+ (2880/GearNum)*5/12; 
 							New_deg += MeterObj[Channel].AngleShift;
 							if(New_deg>2879)
 							New_deg=New_deg-2880;
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg , MeterObj[Channel].O1OuterR );			 
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		
 
 							New_deg =  i * (2880/GearNum)+ (2880/GearNum)*6/12; 
 							New_deg += MeterObj[Channel].AngleShift;
 							if(New_deg>2879)
 							New_deg=New_deg-2880;
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg , MeterObj[Channel].O1OuterR*4/5);			 
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		
					}
 
 							VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 							
							static VGfloat SrampStops[35] = {			0.0f, 0.4f, 0.6f, 0.3f, 1.0f, 		
																										0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 		
																										0.65f, 0.1f, 0.3f, 0.3f, 1.0f,  																										
																										0.7f, 0.0f, 0.0f, 0.0f, 1.0f,  
																										0.75f, 0.1f, 0.3f, 0.3f, 1.0f, 
																										0.97f, 0.5f, 0.0f, 0.0f, 1.0f, 	
																										1.00f, 0.9f, 0.0f, 0.0f, 1.0f, 																							
																							};					

							static VGfloat SrampStopsGreen[35] = {			0.0f, 	0.6f,0.4f,  0.3f, 1.0f, 		
																													0.5f, 	0.0f,0.0f,  0.0f, 1.0f, 		
																													0.65f, 	0.3f,0.1f,  0.3f, 1.0f,  																										
																													0.7f, 	0.0f,0.0f,  0.0f, 1.0f,  
																													0.75f, 	0.3f,0.1f,  0.3f, 1.0f, 
																													0.97f, 	0.0f,0.5f,  0.0f, 1.0f, 	
																													1.00f, 	0.0f,0.9f,  0.0f, 1.0f, 																							
																							};																																	
				if(Color==0xFF0000ff)
					{
							for(i=0;i<35;i++)
							VGParameter.GradientStep[i]=SrampStops[i]; 
							VGParameter.GradientPoints = 35;
					}
				else if(Color==0x00FF00ff)
					{
							for(i=0;i<35;i++)
							VGParameter.GradientStep[i]=SrampStopsGreen[i]; 
							VGParameter.GradientPoints = 35;
					}
 
							VGParameter.GradientStartEnd[0] = MeterObj[Channel].CenterX;
							VGParameter.GradientStartEnd[1] = MeterObj[Channel].CenterY;
							VGParameter.GradientStartEnd[2] = MeterObj[Channel].CenterX;
							VGParameter.GradientStartEnd[3] = MeterObj[Channel].CenterY;
							VGParameter.GradientStartEnd[4] = MeterObj[Channel].O1OuterR;
 
							GL_DrawingPolyconFill( &VGParameter);	
 
 					G_CharScale = 3.0 * CharSize;
					G_CharShearX=0.1;
					G_CharShearY=0.0;
					
//					printf(" x, y %d, %d\n", CharPosOffsetX, CharPosOffsetY  );
					if(Channel==1)
					{
 
					
					CharPosOffsetX = G_CharScale*6*14/2;	// + G_CharShearX*14 ;
					CharPosOffsetY = G_CharScale*12/2;
					R_TXTR_DrawStringMain("RZ/A1H", 6, MeterObj[Channel].CenterX-CharPosOffsetX, MeterObj[Channel].CenterY-CharPosOffsetY , 0xA0A0A0ff);	
					}
					
					if(Channel==2)
					{
 
					CharPosOffsetX = G_CharScale*6*14/2;	// + G_CharShearX*14 ;
					CharPosOffsetY = G_CharScale*12/2;
					R_TXTR_DrawStringMain("OpenVG", 6, MeterObj[Channel].CenterX-CharPosOffsetX, MeterObj[Channel].CenterY-CharPosOffsetY , 0xA0A0A0ff);	
					}
																							
}

int RETW_GUI_OPENMOTION_0_GearTRIANGLE( int speed, int Maxspeed, uint32_t O1 ,uint32_t AngleShift )
{
				uint32_t i,j,New_deg, Length;
				uint32_t LimiterSpeed;
 
				st_VGDrawingParameter	VGParameter;
 

				uint32_t O1Outer = O1;
				uint32_t O1OuterR = O1/2; 
				float CharSize = 1.0;
	
 							VGParameter.PathPoints=0;
							j=0;													
						
				for(i=0;i<GearNum_TriAngle;i++)
				{
 							New_deg = i * (2880/GearNum_TriAngle);
 							New_deg += AngleShift;
 							if(New_deg>2879)
 							New_deg=New_deg-2880;
							CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY,	New_deg , O1OuterR*4/5);			 
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
 							New_deg =  i * (2880/GearNum_TriAngle)+ (2880/GearNum_TriAngle)*6/12; 
 							New_deg += AngleShift;
 							if(New_deg>2879)
 							New_deg=New_deg-2880;
							CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY,	New_deg , O1OuterR);			 
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		
					}
 
 							VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 							
 							
							static VGfloat SrampStops[35] = {			0.0f, 0.4f, 0.6f, 0.3f, 1.0f, 		
																										0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 		
																										0.65f, 0.1f, 0.3f, 0.3f, 1.0f,  																										
																										0.7f, 0.0f, 0.0f, 0.0f, 1.0f,  
																										0.75f, 0.1f, 0.3f, 0.3f, 1.0f, 
																										0.97f, 0.5f, 0.0f, 0.0f, 1.0f, 	
																										1.00f, 0.9f, 0.0f, 0.0f, 1.0f, 																							
																							};				
							for(i=0;i<35;i++)
							VGParameter.GradientStep[i]=SrampStops[i]; 
							VGParameter.GradientPoints = 35;
 				 
							VGParameter.GradientStartEnd[0] = ClockCenterX;
							VGParameter.GradientStartEnd[1] = ClockCenterY;
							VGParameter.GradientStartEnd[2] = ClockCenterX;
							VGParameter.GradientStartEnd[3] = ClockCenterY;
							VGParameter.GradientStartEnd[4] = O1OuterR;
 
							GL_DrawingPolyconFill( &VGParameter);	
 
 					G_CharScale = 3.0;
					G_CharShearX=0.1;
					G_CharShearY=0.0;
					R_TXTR_DrawStringMain("RZ/A1H", 5, ClockCenterX, ClockCenterY , 0xA0A0A0ff);
}

int RETW_GUI_OPENMOTION_0_GearSAW( int speed, int Maxspeed, uint32_t O1 ,uint32_t AngleShift )
{
				uint32_t i,j,New_deg, Length;
				uint32_t LimiterSpeed;
 
				st_VGDrawingParameter	VGParameter;
 

				uint32_t O1Outer = O1;
				uint32_t O1OuterR = O1/2;
				 
				float CharSize = 1.0;
	
 							VGParameter.PathPoints=0;
							j=0;													
						
				for(i=0;i<GearNum_Knife;i++)
				{
 							New_deg = i * (2880/GearNum_Knife);
 							New_deg += AngleShift;
 							if(New_deg>2879)
 							New_deg=New_deg-2880;
							CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY,	New_deg , O1OuterR*3/5);			 
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			

 							New_deg = i * (2880/GearNum_Knife)+ (2880/GearNum_Knife)*16/12; 		 
 							New_deg += AngleShift;
 							if(New_deg>2879)
 							New_deg=New_deg-2880;
							CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY,	New_deg , O1OuterR );			 
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;								

 							New_deg = i * (2880/GearNum_Knife)+ (2880/GearNum_Knife)*8/12; 
 							New_deg += AngleShift;
 							if(New_deg>2879)
 							New_deg=New_deg-2880;
							CaculateCoordinationXYinFloat(ClockCenterX, ClockCenterY,	New_deg , O1OuterR*3/5 );			 
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
					}
 
 							VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 							
 							
							static VGfloat SrampStops[35] = {			0.0f, 0.4f, 0.6f, 0.3f, 1.0f, 		
																										0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 		
																										0.65f, 0.1f, 0.3f, 0.3f, 1.0f,  																										
																										0.7f, 0.0f, 0.0f, 0.0f, 1.0f,  
																										0.75f, 0.1f, 0.3f, 0.3f, 1.0f, 
																										0.97f, 0.5f, 0.0f, 0.0f, 1.0f, 	
																										1.00f, 0.9f, 0.0f, 0.0f, 1.0f, 																							
																							};				
							for(i=0;i<35;i++)
							VGParameter.GradientStep[i]=SrampStops[i]; 
							VGParameter.GradientPoints = 35;
 				 
							VGParameter.GradientStartEnd[0] = ClockCenterX;
							VGParameter.GradientStartEnd[1] = ClockCenterY;
							VGParameter.GradientStartEnd[2] = ClockCenterX;
							VGParameter.GradientStartEnd[3] = ClockCenterY;
							VGParameter.GradientStartEnd[4] = O1OuterR;
 
							GL_DrawingPolyconFill( &VGParameter);	
 
 					G_CharScale = 3.0;
					G_CharShearX=0.1;
					G_CharShearY=0.0;
					R_TXTR_DrawStringMain("RZ/A1H", 5, ClockCenterX, ClockCenterY , 0xA0A0A0ff);
}

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


int RETW_GUI_OPENMOTION_0_0_Main( int speed, int Maxspeed, uint32_t Channel )
{
				uint32_t i,j,New_deg;
				uint32_t LimiterSpeed;
 
				st_VGDrawingParameter	VGParameter;
				st_VGDrawArcParameter	ArcInfo; 	
				float CharSize= MeterObj[Channel].O1Outer/480;
 
#if 1		//O4 Inside
 
 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[Channel].O4Outer;
					VGParameter.PathDrawingData[3]= MeterObj[Channel].O4Outer;		
 																				
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
					VGParameter.GradientStartEnd[0] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[2] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[3] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[4] = MeterObj[Channel].O4Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
//				  vgFinish();
#endif
 
#if 1		//	DrawArc , AD input Value
 
 				ArcInfo.CenterX			=MeterObj[Channel].CenterX;
 				ArcInfo.CenterY			=MeterObj[Channel].CenterY;
 				ArcInfo.Width				=MeterObj[Channel].O5Outer+4;
 				ArcInfo.Height			=MeterObj[Channel].O5Outer+4;
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
					RETW_GUI_OPENMOTION_0_NUMBER_0(Channel);
#endif
 
/*
  VG_JOIN_MITER                               = 0x1800,
  VG_JOIN_ROUND                               = 0x1801,
  VG_JOIN_BEVEL                               = 0x1802,
*/
 
#if 1
 				RETW_GUI_OPENMOTION_0_IND_0( 0 );
 
#endif

#if 1		//Now for Speed Circle window

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[Channel].O5Outer;
					VGParameter.PathDrawingData[3]= MeterObj[Channel].O5Outer;		
								
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
 
					VGParameter.GradientStartEnd[0] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[2] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[3] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[4] = MeterObj[Channel].O5Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
 				  vgFinish();
#endif

#if 1
 					G_CharScale = 3.0 * MeterObj[Channel].O1Outer/500;;
					G_CharShearX=0.3;
					G_CharShearY=0.0;
					RETW_GUI_DrawScriptNumber(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY+MeterObj[Channel].O5InnerR-(MeterObj[Channel].O5InnerR/3)-20, speed, 24 , 0xA0C0C0ff, 1 );
#endif
	
					R_TXTR_DrawStringMain("km/h", 4, MeterObj[Channel].CenterX-24*CharSize, MeterObj[Channel].CenterY+MeterObj[Channel].O5InnerR-(MeterObj[Channel].O5InnerR/3)-40, 0xA0A0A0ff);
								
					RETW_GUI_OPENMOTION_0_MilesRecord(speed, MeterObj[Channel].CenterX, MeterObj[Channel].CenterY+(MeterObj[Channel].O5InnerR/4) );
				
 					G_CharScale = 0.8;
					G_CharShearX=0.3;
					G_CharShearY=0.0;	
										
					R_TXTR_DrawStringMain("KiloMeter", 9, MeterObj[Channel].CenterX-50, MeterObj[Channel].CenterY+5, 0x000000ff);
}

int RETW_GUI_OPENMOTION_0_REFRESH_0(uint32_t Channel, uint32_t speed )
{
				uint32_t i,j,New_deg;
				uint32_t LimiterSpeed;
 
				st_VGDrawingParameter	VGParameter;
				st_VGDrawArcParameter	ArcInfo; 	
				
				float CharSize = 1.0 * MeterObj[Channel].O1Outer/480;

				RETW_GUI_OPENMOTION_0_Mobj_Refresh(Channel);

#if 1					//Fill from O3Outer  to Cenetr, not need O4, O5

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[Channel].O3Outer;
					VGParameter.PathDrawingData[3]= MeterObj[Channel].O3Outer;		
								
					static VGfloat SrampStops0[35] = { 	0.00f, 0.1f,  0.5f,  0.2f, 1.0f, 		
																							0.53f, 0.0f,  0.2f,  0.0f, 1.0f, 
																										
																							0.54f, 0.0f,  0.0f,  0.0f, 1.0f, 	
																							0.56f, 0.0f,  0.0f,  0.0f, 1.0f,	
																							
																							0.57f, 0.0f,  0.3f,  0.9f, 1.0f, 
																							0.65f, 0.0f,  0.0f,  0.0f, 1.0f,  	
																							1.00f, 0.0f,  0.0f,  0.0f, 1.0f, 																							
																							};									
					for(i=0;i<35;i++)
					VGParameter.GradientStep[i]=SrampStops0[i]; 
					VGParameter.GradientPoints = 35;
					
					VGParameter.GradientStartEnd[0] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[2] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[3] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[4] = MeterObj[Channel].O3Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
// 				  vgFinish();
#endif

#if 0		//O4 Inside
 
 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[Channel].O4Outer-5;
					VGParameter.PathDrawingData[3]= MeterObj[Channel].O4Outer-5;		
 																				
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
																							
 
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 30;
 											
					VGParameter.GradientStartEnd[0] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[2] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[3] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[4] = MeterObj[Channel].O4Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
//				  vgFinish();
#endif
 
#if 0		//	DrawArc , by EngineRPM AD input Value
 
 				ArcInfo.CenterX			=MeterObj[Channel].CenterX;
 				ArcInfo.CenterY			=MeterObj[Channel].CenterY;
 				ArcInfo.Width				=MeterObj[Channel].O5Outer+5;
 				ArcInfo.Height			=MeterObj[Channel].O5Outer+5;
 				
  			ArcInfo.StartAngle	= 225-EngineRPM;
 				ArcInfo.AngleExtent	= EngineRPM;
 				
 				ArcInfo.ArcWidth		= 9 ;
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
 					RETW_GUI_OPENMOTION_0_GRID(Channel);
					RETW_GUI_OPENMOTION_0_NUMBER_0(Channel);

#endif
 
/*
  VG_JOIN_MITER                               = 0x1800,
  VG_JOIN_ROUND                               = 0x1801,
  VG_JOIN_BEVEL                               = 0x1802,
*/
 
#if 1
 				RETW_GUI_OPENMOTION_0_IND_0( Channel );
 
#endif

#if 0		//Now for Speed Circle window

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[Channel].O5Outer;
					VGParameter.PathDrawingData[3]= MeterObj[Channel].O5Outer;		
								
					static VGfloat SrampStops6[30] = { 	0.0f, 0.9f, 0.6f, 0.4f, 1.0f, 							//Original
																							0.5f, 0.5f, 0.3f, 0.2f, 1.0f, 		
																							0.91f, 0.1f, 0.1f, 0.1f, 1.0f, 
																							0.95f, 0.5f, 0.5f, 0.5f, 1.0f, 
																							0.99f, 0.1f, 0.1f, 0.1f, 1.0f, 
																							1.00f, 0.1f, 0.1f, 0.1f, 1.0f, 																							
																							};				
					static VGfloat SrampStops7[30] = { 	0.0f,  0.1f, 0.1f, 0.1f, 1.0f, 							//Green
																							0.5f,  0.1f, 0.1f, 0.1f, 1.0f, 		
																							0.91f, 0.1f, 0.1f, 0.1f, 1.0f, 
																							0.95f, 0.1f, 0.1f, 0.1f, 1.0f, 
																							0.99f, 0.1f, 0.1f, 0.1f, 1.0f, 
																							1.00f, 1.0f, 1.0f, 1.0f, 1.0f, 																							
																							};		
																							
																							
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops7[i]; 
					VGParameter.GradientPoints = 30;
 
					VGParameter.GradientStartEnd[0] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[2] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[3] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[4] = MeterObj[Channel].O5Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
 				  vgFinish();
#endif

#if 1		//Main Speed Number

  				G_CharScale = 3.5 *CharSize;
					G_CharShearX=0.2;
					G_CharShearY=0;
					RETW_GUI_DrawScriptNumber(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY , speed, 24 , 0xA0A0A0ff, 1 );
#endif
#if 1		//km/H

  				G_CharScale = 1.0 *CharSize;
					G_CharShearX=0.0;
					G_CharShearY=0.0;
					CharPosOffsetX = G_CharScale*4*14/2;	// + G_CharShearX*14 ;
					CharPosOffsetY = G_CharScale*12/2;
					R_TXTR_DrawStringMain("km/h", 4, MeterObj[Channel].CenterX-CharPosOffsetX, MeterObj[Channel].CenterY- MeterObj[Channel].O5InnerR/2, 0xA0A0A0ff);
#endif		
					
#if 1

					RETW_GUI_OPENMOTION_0_MilesRecord(speed, MeterObj[Channel].CenterX,  MeterObj[Channel].CenterY - MeterObj[Channel].O4InnerR  );
#endif					

					RETW_GUI_OPENMOTION_0_FlashSignClean();
   				RETW_GUI_OPENMOTION_0_FlashSignDsp();

}
int RETW_GUI_OPENMOTION_0_REFRESH_1(uint32_t Channel, uint32_t speed )
{
				uint32_t i,j,New_deg;
				uint32_t LimiterSpeed;
 
				st_VGDrawingParameter	VGParameter;
 
				float CharSize = 1.0 * MeterObj[Channel].O1Outer/480;

				RETW_GUI_OPENMOTION_0_Mobj_Refresh(Channel);
#if 1		//O4 Inside pure color
 					VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 					VGParameter.FillColor = 0x00000000;
 	 				VGParameter.AlphaColor = 0x000000ff;
 	 						 					
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[Channel].O4Outer-3;
					VGParameter.PathDrawingData[3]= MeterObj[Channel].O4Outer-3;		
 
				  GL_DrawingCircleFill( &VGParameter);	
#endif

#if 0		//O4 Inside


 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[Channel].O4Outer-5;
					VGParameter.PathDrawingData[3]= MeterObj[Channel].O4Outer-5;		
 																				
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
																							
 
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 30;
 											
					VGParameter.GradientStartEnd[0] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[2] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[3] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[4] = MeterObj[Channel].O4Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
//				  vgFinish();
#endif
 
 
 
#if 1		//Number only
					RETW_GUI_OPENMOTION_0_NUMBER_1(Channel);
#endif
 
/*
  VG_JOIN_MITER                               = 0x1800,
  VG_JOIN_ROUND                               = 0x1801,
  VG_JOIN_BEVEL                               = 0x1802,
*/
 
#if 1
 				RETW_GUI_OPENMOTION_0_IND_1( Channel );
 
#endif
 
}


int RETW_GUI_OPENMOTION_0_REFRESH_2(uint32_t Channel, uint32_t speed )
{
				uint32_t i,j,New_deg;
				uint32_t LimiterSpeed;
 
				st_VGDrawingParameter	VGParameter;
 
				float CharSize = 1.0 * MeterObj[Channel].O1Outer/480;
				RETW_GUI_OPENMOTION_0_Mobj_Refresh(Channel);
#if 1		//Fill from O4Outer 

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[Channel].O4Outer;
					VGParameter.PathDrawingData[3]= MeterObj[Channel].O4Outer;		
								
					static VGfloat SrampStops1[20] = { 	0.0f,  0.8f,  0.8f,  0.8f, 1.0f, 		
																							0.5f,  0.0f,  0.0f,  0.0f, 1.0f, 		
																							0.90f, 0.0f,  0.0f,  0.0f, 1.0f, 
																							1.00f, 0.0f,  0.0f,  0.0f, 1.0f, 																							
																							};									
					for(i=0;i<20;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 20;
					
					VGParameter.GradientStartEnd[0] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[2] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[3] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[4] = MeterObj[Channel].O4Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
// 				  vgFinish();
#endif
 
#if 0		//O4 Inside
 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[Channel].O4Outer-5;
					VGParameter.PathDrawingData[3]= MeterObj[Channel].O4Outer-5;		
 																				
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
																							
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 30;
 											
					VGParameter.GradientStartEnd[0] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[2] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[3] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[4] = MeterObj[Channel].O4Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
//				  vgFinish();
#endif
 
#if 1		//Number only
					RETW_GUI_OPENMOTION_0_NUMBER_2(Channel);
#endif
 
/*
  VG_JOIN_MITER                               = 0x1800,
  VG_JOIN_ROUND                               = 0x1801,
  VG_JOIN_BEVEL                               = 0x1802,
*/
 
#if 1
 				RETW_GUI_OPENMOTION_0_IND_2( Channel );
 
#endif
 
}


int RETW_GUI_OPENMOTION_0_REFRESH_3(uint32_t Channel, uint32_t speed )
{
				uint32_t i,j,New_deg;
				uint32_t LimiterSpeed;
 
				st_VGDrawingParameter	VGParameter;
 
				float CharSize = 1.0 * MeterObj[Channel].O1Outer/480;
				RETW_GUI_OPENMOTION_0_Mobj_Refresh(Channel);
#if 0		//O4 Inside pure color
 					VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 					VGParameter.FillColor = 0x00000000;
 	 				VGParameter.AlphaColor = 0x000000ff;
 	 						 					
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[Channel].O4Outer-3;
					VGParameter.PathDrawingData[3]= MeterObj[Channel].O4Outer-3;		
 
				  GL_DrawingCircleFill( &VGParameter);	
#endif

#if 1   	//Fill from O4Outer 

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[Channel].O4Outer;
					VGParameter.PathDrawingData[3]= MeterObj[Channel].O4Outer;		
								
					static VGfloat SrampStops1[10] = { 	0.00f, 0.9f,  0.4f,  0.0f, 1.0f, 		
//																							0.50f, 0.7f,  0.3f,  0.0f, 1.0f,
																							1.00f, 0.5f,  0.2f,  0.0f, 1.0f, 																							
																							};									
					for(i=0;i<10;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 10;
					
					VGParameter.GradientStartEnd[0] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[2] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[3] = MeterObj[Channel].CenterY+MeterObj[Channel].O4Outer/3;
					VGParameter.GradientStartEnd[4] = MeterObj[Channel].O4Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
 //				  vgFinish();
#endif

#if 1		//Number only
					RETW_GUI_OPENMOTION_0_NUMBER_3(Channel);
#endif
 
/*
  VG_JOIN_MITER                               = 0x1800,
  VG_JOIN_ROUND                               = 0x1801,
  VG_JOIN_BEVEL                               = 0x1802,
*/
 
#if 1
 				RETW_GUI_OPENMOTION_0_IND_3( Channel );
#endif
}

int RETW_GUI_OPENMOTION_0_REFRESH_4(uint32_t Channel, uint32_t speed )
{
				uint32_t i,j,New_deg;
				uint32_t LimiterSpeed;
 
				st_VGDrawingParameter	VGParameter;
 
				float CharSize = 1.0 * MeterObj[Channel].O1Outer/480;
				RETW_GUI_OPENMOTION_0_Mobj_Refresh(Channel);
#if 1		//O4 Inside pure color
 					VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 					VGParameter.FillColor = 0x10000000;
 	 				VGParameter.AlphaColor = 0x000000ff;
 	 						 					
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[Channel].O4Outer-3;
					VGParameter.PathDrawingData[3]= MeterObj[Channel].O4Outer-3;		
 
				  GL_DrawingCircleFill( &VGParameter);	
#endif
 
#if 0			//Fill from O4Outer 

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[j].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[j].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[j].O4Outer;
					VGParameter.PathDrawingData[3]= MeterObj[j].O4Outer;		
								
					static VGfloat SrampStops1[20] = { 	0.0f,  0.7f,  0.3f,  0.2f, 1.0f, 		
																							0.5f,  0.0f,  0.0f,  0.0f, 1.0f, 		
																							0.90f, 0.0f,  0.0f,  0.0f, 1.0f, 
																							1.00f, 0.0f,  0.0f,  0.0f, 1.0f, 																							
																							};									
					for(i=0;i<20;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 20;
					
					VGParameter.GradientStartEnd[0] = MeterObj[j].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[j].CenterY;
					VGParameter.GradientStartEnd[2] = MeterObj[j].CenterX;
					VGParameter.GradientStartEnd[3] = MeterObj[j].CenterY;
					VGParameter.GradientStartEnd[4] = MeterObj[j].O4Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
 				  vgFinish();
#endif
 
#if 0		//O4 Inside
 
 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[Channel].O4Outer-5;
					VGParameter.PathDrawingData[3]= MeterObj[Channel].O4Outer-5;		
 																				
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
																							
 
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 30;
 											
					VGParameter.GradientStartEnd[0] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[2] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[3] = MeterObj[Channel].CenterY;
					VGParameter.GradientStartEnd[4] = MeterObj[Channel].O4Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
//				  vgFinish();
#endif
 
#if 1		//Number only
					RETW_GUI_OPENMOTION_0_NUMBER_4(Channel);
#endif
 
/*
  VG_JOIN_MITER                               = 0x1800,
  VG_JOIN_ROUND                               = 0x1801,
  VG_JOIN_BEVEL                               = 0x1802,
*/
 
#if 1
 				RETW_GUI_OPENMOTION_0_IND_4( Channel );
 
#endif
 
}




void RETW_GUI_OPENMOTION_0_FlashSignDsp() 
{
				uint32_t i,j;
				uint32_t CenterX,CenterY;
				
				uint32_t 	SignSizeX=150,SignSizeY=30;
				
				st_VGDrawingParameter	VGParameter;	

				if( rtc_halfsecond_flag == 1 )
					{
						Milisec_SignFlashCount=0;
						rtc_halfsecond_flag=0;
					}
 
  
				if( rtc_halfsecond_flag_CNT == 1 )
				{

//Right DIRECTION sign

						if( Milisec_SignFlashCount<80 )
   							RETW_GUI_PoliconFillScreen(800-80-(SignSizeX/2), 				240-(SignSizeY/2),	12,SignSizeY,0x109010ff);	
   							
   					else if ( Milisec_SignFlashCount<160 )
   						{
   							RETW_GUI_PoliconFillScreen(800-80-(SignSizeX/2), 				240-(SignSizeY/2),	12,SignSizeY,0x109010ff);	
   							RETW_GUI_PoliconFillScreen(800-80-(SignSizeX/2)+20, 		240-(SignSizeY/2),	14,SignSizeY,0x10B010ff);	   							   					
   						}
   					else if ( Milisec_SignFlashCount<240 )
   						{
   							RETW_GUI_PoliconFillScreen(800-80-(SignSizeX/2), 				240-(SignSizeY/2),	12,SignSizeY,0x109010ff);	
   							RETW_GUI_PoliconFillScreen(800-80-(SignSizeX/2)+20, 		240-(SignSizeY/2),	14,SignSizeY,0x10B010ff);	 
   							RETW_GUI_PoliconFillScreen(800-80-(SignSizeX/2)+40, 		240-(SignSizeY/2),	16,SignSizeY,0x10D010ff);	  							   					
   						}
   					else if ( Milisec_SignFlashCount<320 )
   						{
   							RETW_GUI_PoliconFillScreen(800-80-(SignSizeX/2), 				240-(SignSizeY/2),	12,SignSizeY,0x109010ff);	
   							RETW_GUI_PoliconFillScreen(800-80-(SignSizeX/2)+20, 		240-(SignSizeY/2),	14,SignSizeY,0x10B010ff);	 
   							RETW_GUI_PoliconFillScreen(800-80-(SignSizeX/2)+40, 		240-(SignSizeY/2),	16,SignSizeY,0x10D010ff);	
   							RETW_GUI_PoliconFillScreen(800-80-(SignSizeX/2)+60, 		240-(SignSizeY/2),	18,SignSizeY,0x10FF10ff);	 	   							   					
   						}
   					else  
   						{
   							RETW_GUI_PoliconFillScreen(800-80-(SignSizeX/2), 				240-(SignSizeY/2),	12,SignSizeY,0x109010ff);	
   							RETW_GUI_PoliconFillScreen(800-80-(SignSizeX/2)+20, 		240-(SignSizeY/2),	14,SignSizeY,0x10B010ff);	 
   							RETW_GUI_PoliconFillScreen(800-80-(SignSizeX/2)+40, 		240-(SignSizeY/2),	16,SignSizeY,0x10D010ff);	
   							RETW_GUI_PoliconFillScreen(800-80-(SignSizeX/2)+60, 		240-(SignSizeY/2),	18,SignSizeY,0x10FF10ff);	 	 					
 
								CenterX = 800-80-(SignSizeX/2)+80+ 32;
								CenterY = 240;
							
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
				}			
				else
				{
//Left DIRECTION sign

						if( Milisec_SignFlashCount<80 )
   							RETW_GUI_PoliconFillScreen(80+(SignSizeX/2)-20+8, 		240-(SignSizeY/2),	12,SignSizeY,0x109010ff);	
   							
   					else if ( Milisec_SignFlashCount<160 )
   						{
   							RETW_GUI_PoliconFillScreen(80+(SignSizeX/2)-20+8, 		240-(SignSizeY/2),	12,SignSizeY,0x109010ff);	
   							RETW_GUI_PoliconFillScreen(80+(SignSizeX/2)-40+6, 		240-(SignSizeY/2),	14,SignSizeY,0x10B010ff);	  							   					
   						}
   					else if ( Milisec_SignFlashCount<240 )
   						{
   							RETW_GUI_PoliconFillScreen(80+(SignSizeX/2)-20+8, 		240-(SignSizeY/2),	12,SignSizeY,0x109010ff);	
   							RETW_GUI_PoliconFillScreen(80+(SignSizeX/2)-40+6, 		240-(SignSizeY/2),	14,SignSizeY,0x10B010ff);	 
   							RETW_GUI_PoliconFillScreen(80+(SignSizeX/2)-60+4, 		240-(SignSizeY/2),	16,SignSizeY,0x10D010ff);	  							   					
   						}
   					else if ( Milisec_SignFlashCount<320 )
   						{
   							RETW_GUI_PoliconFillScreen(80+(SignSizeX/2)-20+8, 		240-(SignSizeY/2),	12,SignSizeY,0x109010ff);	
   							RETW_GUI_PoliconFillScreen(80+(SignSizeX/2)-40+6, 		240-(SignSizeY/2),	14,SignSizeY,0x10B010ff);	 
   							RETW_GUI_PoliconFillScreen(80+(SignSizeX/2)-60+4, 		240-(SignSizeY/2),	16,SignSizeY,0x10D010ff);	
   							RETW_GUI_PoliconFillScreen(80+(SignSizeX/2)-80+2, 		240-(SignSizeY/2),	18,SignSizeY,0x10FF10ff);   							   					
   						}
   					else  
   						{
   							RETW_GUI_PoliconFillScreen(80+(SignSizeX/2)-20+8, 		240-(SignSizeY/2),	12,SignSizeY,0x109010ff);	
   							RETW_GUI_PoliconFillScreen(80+(SignSizeX/2)-40+6, 		240-(SignSizeY/2),	14,SignSizeY,0x10B010ff);	 
   							RETW_GUI_PoliconFillScreen(80+(SignSizeX/2)-60+4, 		240-(SignSizeY/2),	16,SignSizeY,0x10D010ff);	
   							RETW_GUI_PoliconFillScreen(80+(SignSizeX/2)-80+2, 		240-(SignSizeY/2),	18,SignSizeY,0x10FF10ff);	 	 		
   							
		 
								  							
								CenterX = (SignSizeX/2)-32;
								CenterY = 240;
					    	
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
}
void RETW_GUI_OPENMOTION_Speed_0_SpeedWindow(uint32_t speed, uint32_t X, uint32_t Y) 
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


void RETW_GUI_OPENMOTION_0_MilesRecord(uint32_t speed, uint32_t X, uint32_t Y) 
{
				uint32_t i;
				st_VGDrawingParameter	VGParameter;	
				
				uint32_t Channel=0;
				
				float CharSize = 1.0 * MeterObj[Channel].O1Outer/460;
				
  			G_CharScale = 1.0 *CharSize; 
				G_CharShearX=0;
				G_CharShearY=0;
				CharPosOffsetX = G_CharScale*9*14/2;	// + G_CharShearX*14 ;
				CharPosOffsetY = G_CharScale*12/2;

				VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
				
				VGParameter.PathDrawingData[2]= 140 * MeterObj[Channel].O1Outer/460;							//Width
				VGParameter.PathDrawingData[3]= 70 * MeterObj[Channel].O1Outer/460;								//Height

				VGParameter.PathDrawingData[0]= X-(VGParameter.PathDrawingData[2]/2);																								//org coord X
				VGParameter.PathDrawingData[1]= Y-(VGParameter.PathDrawingData[3]/2);																								//org coord Y

				VGParameter.PathDrawingData[4]= 3;								//Corner X
				VGParameter.PathDrawingData[5]= 3;								//Corner Y
 				
 				VGParameter.GradientStartEnd[0]=VGParameter.PathDrawingData[0]+(VGParameter.PathDrawingData[2]/2);
 				VGParameter.GradientStartEnd[1]=VGParameter.PathDrawingData[1]+VGParameter.PathDrawingData[3]; 
 				VGParameter.GradientStartEnd[2]=VGParameter.PathDrawingData[0]+(VGParameter.PathDrawingData[2]/2);
 				VGParameter.GradientStartEnd[3]=VGParameter.PathDrawingData[1]; 

					static VGfloat SrampStops[35] = { 	0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 		
																							0.2f, 0.0f, 0.0f, 0.0f, 1.0f, 
																							0.45f, 0.0f, 0.0f, 0.0f, 1.0f,  
																							0.5f, 0.9f, 0.9f, 0.9f, 1.0f, 	
																							0.55f, 0.0f, 0.0f, 0.0f, 1.0f,  
																							0.90f, 0.0f, 0.0f, 0.0f, 1.0f, 
																							1.0f, 0.5f, 0.5f, 0.5f, 1.0f																
																							};									
					for(i=0;i<35;i++)
					VGParameter.GradientStep[i]=SrampStops[i]; 
					VGParameter.GradientPoints = 35;
 
					VGParameter.StrokeIt=0;
					VGParameter.FillIt=1; 
					GL_DrawingRoundRectangle(&VGParameter);								//fill 

					VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 					VGParameter.StrokeWidth = 1;
  				VGParameter.FillColor = 0xA0A0A000;
  				VGParameter.AlphaColor = 0x000000ff;
 					VGParameter.StrokeIt=1;
 					VGParameter.FillIt=0; 
					GL_DrawingRoundRectangle(&VGParameter);								//stroke		
 
//				RETW_GUI_DrawScriptNumber(400+5, 130, 456, 24 , 0xD0D0D0ff, 1 );
//				RETW_GUI_DrawScriptNumber(400+5, 95, 123456, 24 , 0xC0C0C0ff, 1 );	
					
					RETW_GUI_DrawScriptNumber(X, Y + 17 * MeterObj[Channel].O1Outer/460, totalframecnt%1000, 24 , 0xD0D0D0ff, 1 );
					RETW_GUI_DrawScriptNumber(X, Y - 20 * MeterObj[Channel].O1Outer/460, totalframecnt, 24 , 0xC0C0C0ff, 1 ); 							//VGParameter.PathDrawingData[3]/4
}


int RETW_GUI_OPENMOTION_0_IND_0(int Channel)
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
				uint32_t i,j;
				uint32_t 	Engine_deg, New_deg, Indi_deg;
				uint32_t AADV,ADV;
				int32_t		Indi_deg_Gap;
				st_VGDrawingParameter	VGParameter;
								
				static uint32_t IndicatorLoc[10]= { 2,	180-50,		 	180-25,20,		 180+25,20,		 360-2, 180-50,		0, 180-40};
 
						IndicatorLoc[0] = (MeterObj[Channel].O3Outer/2)-4;								//aa
						IndicatorLoc[1] = (MeterObj[Channel].O3Outer/2)-12;								//bb
						IndicatorLoc[2] = 4;																					//cc
						IndicatorLoc[3] = (MeterObj[Channel].O5Outer/2);							  	//dd
						IndicatorLoc[4] = 0;																					//ee
						IndicatorLoc[5] = 6;																					//ff							
 
#if 0    		//Read AD value
							AADV=0;

							for(i=0;i<20;i++)
							AADV += R_ADC_Read();						//1024
							ADV=AADV/20;				
							
							Engine_deg	= 225*8 + ADV*2160/1024;			//Need 270 deg			
							EngineRPM  = ADV*225/1024;
#endif

#if 1
						Engine_deg = 225*8 + VirtualEngine.TargetRPM *3/4;
#endif
			
#if 1				

				#if 1
							if(Engine_deg > (VirtualEngine.IndicatorSpeed+3)  )
								{
										Indi_deg_Gap = (Engine_deg - VirtualEngine.IndicatorSpeed)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorSpeed +=8;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorSpeed +=4;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorSpeed +=2;
 										else
 										VirtualEngine.IndicatorSpeed +=1;
 										
//    							printf("INC....%d \n",VirtualEngine.IndicatorSpeed );
 
 								}
 							else if(Engine_deg < (VirtualEngine.IndicatorSpeed-3) )
 								{
										Indi_deg_Gap = (VirtualEngine.IndicatorSpeed - Engine_deg)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorSpeed -=8;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorSpeed -=4;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorSpeed -=2;
 										else
 										VirtualEngine.IndicatorSpeed -=1; 									
 
 										if(VirtualEngine.IndicatorSpeed < 225*8)
 										VirtualEngine.IndicatorSpeed=225*8;
 //  								printf(".....DEC.....%d.\n",MainIndicatorDeg); 		
 							  }
				#endif
				
 							if(VirtualEngine.IndicatorSpeed>2880)
 							Indi_deg = VirtualEngine.IndicatorSpeed-2880;
							else
 							Indi_deg=VirtualEngine.IndicatorSpeed;
 
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
 
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[0]);															//aa		1
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[1]);															//bb    2
 							New_deg = CaculateDecDeg_Float(Indi_deg, 90*8);
 							
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[2]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 

							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[3]-IndicatorLoc[4]);							//bb    3
 							New_deg = CaculateDecDeg_Float(Indi_deg, 90*8);
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[5]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		
							

							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[3]);															//dd		4
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		
							
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[3]-IndicatorLoc[4]);							//bb    5
 							New_deg = CaculateIncDeg_Float(Indi_deg, 90*8);
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[5]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;																
							
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[1]);															//bb    6
 							New_deg = CaculateIncDeg_Float(Indi_deg, 90*8);
 							
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[2]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
 
		/*-------------Calculate	GradientStartEnd			------*/
 
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
																										0.4f, 0.0f, 0.0f, 0.8f, 1.0f, 		
																										0.9f, 0.0f, 0.0f, 0.8f, 1.0f,	
																										1.0f, 0.0f, 0.0f, 0.2f, 1.0f };
																					
 							VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
							for(i=0;i<25;i++)
							VGParameter.GradientStep[i]=SrampStops5[i]; 
							VGParameter.GradientPoints = 25;		
							GL_DrawingPolyconFill( &VGParameter);	
}

int RETW_GUI_OPENMOTION_0_IND_4(int Channel  )
{

				uint32_t  New_deg;
				
				st_VGDrawingParameter	VGParameter;
 
 				RETW_GUI_ImportGlobalTimeMotion1();
 
				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, GlobalHourDeg*8, (MeterObj[Channel].O4Outer/2) - (MeterObj[Channel].O4Outer/5) );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
				New_deg = CaculateIncDeg_Float(GlobalHourDeg*8,180*8);
				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg, MeterObj[Channel].O4Outer/12);
				Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 5, 0xC0C0C0ff);									// Indicator by Draw Line with Cap			


				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, GlobalMinDeg*8, (MeterObj[Channel].O4Outer/2) - (MeterObj[Channel].O4Outer/15) );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
				New_deg = CaculateIncDeg_Float(GlobalMinDeg*8,180*8);
				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg, MeterObj[Channel].O4Outer/12);						
     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 3, 0xC0C0C0ff);									// Indicator by Draw Line with Cap			


				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, GlobalSecDeg*8, (MeterObj[Channel].O4Outer/2)-4);
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
				New_deg = CaculateIncDeg_Float(GlobalSecDeg*8,180*8);
				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg, MeterObj[Channel].O4Outer/8);						
     		Stroke_CapStylePara(CoordXDest, CoordYDest, CoordXSrc, CoordYSrc, 2, 0x0000FFff);									// Indicator by Draw Line with Cap		
 
 					VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;		
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
					VGParameter.PathDrawingData[2]= 9;							/*	width			*/
					VGParameter.PathDrawingData[3]= 9;							/*	height		*/
					VGParameter.FillColor = 0x0000A000;
					VGParameter.AlphaColor = 0x000000ff;
				  GL_DrawingCircleFill( &VGParameter);	     		  
}



int RETW_GUI_OPENMOTION_0_IND_4_single(int Channel  )
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
				uint32_t i,j;
				uint32_t Engine_deg, New_deg, Indi_deg;
				uint32_t AADV,ADV;
				int32_t		Indi_deg_Gap;
				st_VGDrawingParameter	VGParameter;
								
				static uint32_t IndicatorLoc[10]= { 2,	180-50,		 	180-25,20,		 180+25,20,		 360-2, 180-50,		0, 180-40};
 
						IndicatorLoc[0] = (MeterObj[Channel].O4Outer/2)-4;								//aa
						IndicatorLoc[1] = (MeterObj[Channel].O4Outer/2)-12;								//bb
						IndicatorLoc[2] = 4;																					//cc
						IndicatorLoc[3] = 0;							 											 			//dd
						IndicatorLoc[4] = 0;																					//ee
						IndicatorLoc[5] = 6;																					//ff							

#if 0			//Read AD value
							AADV=0;

							for(i=0;i<10;i++)
							AADV += R_ADC_Read();						//1024
							ADV=AADV/10;				
							
							Engine_deg	= 210*8 + ADV*7/12;		 	
#endif

#if 0				
 							if(VirtualEngine.GasolineLevel>2880)
 							Indi_deg = VirtualEngine.GasolineLevel-2880;
							else
 							Indi_deg=VirtualEngine.GasolineLevel;
#endif

#if 1				
// 					Engine_deg = 225*8 + VirtualEngine.TargetRPM*270*8/ 2880;
 
				#if 0
							if(VirtualEngine.GasolineLevel > (VirtualEngine.IndicatorGas+2)  )
								{
										Indi_deg_Gap = (VirtualEngine.GasolineLevel - VirtualEngine.IndicatorGas)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorGas +=8;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorGas +=4;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorGas +=2;
 										else
 										VirtualEngine.IndicatorGas +=1;
 										
//    							printf("INC....%d \n",VirtualEngine.IndicatorGas );
 
 								}
 							else if(VirtualEngine.GasolineLevel < (VirtualEngine.IndicatorGas-2) )
 								{
										Indi_deg_Gap = (VirtualEngine.IndicatorGas - VirtualEngine.GasolineLevel)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorGas -=8;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorGas -=4;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorGas -=2;
 										else
 										VirtualEngine.IndicatorGas -=1; 									
 
 										if(VirtualEngine.IndicatorGas < 225*8)
 										VirtualEngine.IndicatorGas=225*8;
 //  								printf(".....DEC.....%d.\n",MainIndicatorDeg); 		
 							  }
				#endif
				
 							if(VirtualEngine.GasolineLevel>2880)
 							Indi_deg = VirtualEngine.GasolineLevel-2880;
							else
 							Indi_deg=VirtualEngine.GasolineLevel;
 
#endif
 
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[0]);															//aa		1
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[1]);															//bb    2
 							New_deg = CaculateDecDeg_Float(Indi_deg, 90*8);
 							
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[2]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 

							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[3]-IndicatorLoc[4]);							//bb    3
 							New_deg = CaculateDecDeg_Float(Indi_deg, 90*8);
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[5]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		
							

							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[3]);															//dd		4
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		
							
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[3]-IndicatorLoc[4]);							//bb    5
 							New_deg = CaculateIncDeg_Float(Indi_deg, 90*8);
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[5]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;																
							
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[1]);															//bb    6
 							New_deg = CaculateIncDeg_Float(Indi_deg, 90*8);
 							
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[2]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
 
		/*-------------Calculate	GradientStartEnd			------*/
 
/*
  VG_PAINT_TYPE_COLOR                         = 0x1B00,
  VG_PAINT_TYPE_LINEAR_GRADIENT               = 0x1B01,
  VG_PAINT_TYPE_RADIAL_GRADIENT               = 0x1B02,				
*/
								VGParameter.GradientStartEnd[0] = VGParameter.PathDrawingData[0];
								VGParameter.GradientStartEnd[1] = VGParameter.PathDrawingData[1];
								VGParameter.GradientStartEnd[2] = VGParameter.PathDrawingData[6];
								VGParameter.GradientStartEnd[3] = VGParameter.PathDrawingData[7];

				static VGfloat SrampStops[25] = { 	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																						0.2f, 0.0f, 0.0f, 0.8f, 1.0f, 		
																						0.4f, 0.0f, 0.0f, 0.8f, 1.0f, 		
																						0.9f, 0.0f, 0.0f, 0.8f, 1.0f,	
																						1.0f, 0.0f, 0.0f, 0.2f, 1.0f };
																					
 							VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
							for(i=0;i<25;i++)
							VGParameter.GradientStep[i]=SrampStops[i]; 
							VGParameter.GradientPoints = 25;		
							GL_DrawingPolyconFill( &VGParameter);	

#if 1		//CAP
 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
 
					VGParameter.PathDrawingData[2]= MeterObj[Channel].O5Outer/4;							/*	width			*/
					VGParameter.PathDrawingData[3]= MeterObj[Channel].O5Outer/4;							/*	height		*/
 
 
					static VGfloat SrampStops1[25] = { 	0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 			
																							0.4f, 0.0f, 0.0f, 0.6f, 1.0f,
																							0.6f, 0.0f, 0.0f, 0.5f, 1.0f, 	
																							0.9f, 0.0f, 0.0f, 0.4f, 1.0f,	
																							1.0f, 0.0f, 0.0f, 0.3f, 1.0f
																							 };
					for(i=0;i<25;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 25;
 
					VGParameter.GradientStartEnd[0] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[Channel].CenterY;

  	#if 0		//fixed direction			
					VGParameter.GradientStartEnd[2] = MeterObj[Channel].CenterX-MeterObj[Channel].O5Outer/16;
					VGParameter.GradientStartEnd[3] = MeterObj[Channel].CenterY+MeterObj[Channel].O5Outer/16;
		#else		//rotate with indi
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, Indi_deg,	MeterObj[Channel].O5Outer/16 );
					VGParameter.GradientStartEnd[2] = CoordXSrc;
					VGParameter.GradientStartEnd[3] = CoordYSrc;
		#endif  				
					VGParameter.GradientStartEnd[4] = MeterObj[Channel].O5Outer/6;
 
				 GL_DrawingCircleFill( &VGParameter);	
#endif				 			 
}

int RETW_GUI_OPENMOTION_0_IND_2(int Channel)
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
				uint32_t i,j;
				uint32_t 	Engine_deg, New_deg, Indi_deg;
				uint32_t AADV,ADV;
				int32_t		Indi_deg_Gap;
				st_VGDrawingParameter	VGParameter;
								
				static uint32_t IndicatorLoc[10]= { 2,	180-50,		 	180-25,20,		 180+25,20,		 360-2, 180-50,		0, 180-40};
 
						IndicatorLoc[0] = (MeterObj[Channel].O4Outer/2)-4;								//aa
						IndicatorLoc[1] = (MeterObj[Channel].O4Outer/2)-12;								//bb
						IndicatorLoc[2] = 4;																					//cc
						IndicatorLoc[3] = 0;							  													//dd
						IndicatorLoc[4] = 0;																					//ee
						IndicatorLoc[5] = 6;																					//ff							
 
#if 0    		//Read AD value
							AADV=0;

							for(i=0;i<10;i++)
							AADV += R_ADC_Read();						//1024
							ADV=AADV/10;				
							
							Engine_deg	= 225*8 + ADV*2160/1024;			//Need 270 deg			
							EngineRPM  = ADV*225/1024;
#endif

#if 1				

						Engine_deg = 225*8 + VirtualEngine.TargetRPM *3/4;

// 					Engine_deg = 225*8 + VirtualEngine.TargetRPM*270*8/ 2880;

//					Engine_deg = 225*8 + VirtualEngine.TargetRPM*270*8/ 2880;

				#if 1
							if(Engine_deg > (VirtualEngine.IndicatorRPM+2)  )
								{
										Indi_deg_Gap = (Engine_deg - VirtualEngine.IndicatorRPM)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorRPM +=20;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorRPM +=10;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorRPM +=2;
 										else
 										VirtualEngine.IndicatorRPM +=1;
 										
//    							printf("INC....%d \n",VirtualEngine.IndicatorRPM );
 
 								}
 							else if(Engine_deg < (VirtualEngine.IndicatorRPM-2) )
 								{
										Indi_deg_Gap = (VirtualEngine.IndicatorRPM - Engine_deg)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorRPM -=12;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorRPM -=8;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorRPM -=2;
 										else
 										VirtualEngine.IndicatorRPM -=1; 									
 
 										if(VirtualEngine.IndicatorRPM < 225*8)
 										VirtualEngine.IndicatorRPM=225*8;
 //  								printf(".....DEC.....%d.\n",MainIndicatorDeg); 		
 							  }
				#endif
				
 							if(VirtualEngine.IndicatorRPM>2880)
 							Indi_deg = VirtualEngine.IndicatorRPM-2880;
							else
 							Indi_deg=VirtualEngine.IndicatorRPM;
 
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
 
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[0]);															//aa		1
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[1]);															//bb    2
 							New_deg = CaculateDecDeg_Float(Indi_deg, 90*8);
 							
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[2]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 

							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[3]-IndicatorLoc[4]);							//bb    3
 							New_deg = CaculateDecDeg_Float(Indi_deg, 90*8);
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[5]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		
							

							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[3]);															//dd		4
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		
							
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[3]-IndicatorLoc[4]);							//bb    5
 							New_deg = CaculateIncDeg_Float(Indi_deg, 90*8);
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[5]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;																
							
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[1]);															//bb    6
 							New_deg = CaculateIncDeg_Float(Indi_deg, 90*8);
 							
 							CaculateCoordinationXYinFloat(CoordXSrc,CoordYSrc,	New_deg, IndicatorLoc[2]);																				//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
 
		/*-------------Calculate	GradientStartEnd			------*/
 
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
																										0.4f, 0.0f, 0.0f, 0.8f, 1.0f, 		
																										0.9f, 0.0f, 0.0f, 0.8f, 1.0f,	
																										1.0f, 0.0f, 0.0f, 0.2f, 1.0f };
																					
 							VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
							for(i=0;i<25;i++)
							VGParameter.GradientStep[i]=SrampStops5[i]; 
							VGParameter.GradientPoints = 25;		
							GL_DrawingPolyconFill( &VGParameter);	
 
#if 1		//CAP
 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[Channel].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[Channel].CenterY;
 
					VGParameter.PathDrawingData[2]= MeterObj[Channel].O5Outer/3;							/*	width			*/
					VGParameter.PathDrawingData[3]= MeterObj[Channel].O5Outer/3;							/*	height		*/
 
 
					static VGfloat SrampStops1[25] = { 	0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 			
																							0.4f, 0.0f, 0.0f, 0.6f, 1.0f,
																							0.6f, 0.0f, 0.0f, 0.5f, 1.0f, 	
																							0.9f, 0.0f, 0.0f, 0.4f, 1.0f,	
																							1.0f, 0.0f, 0.0f, 0.3f, 1.0f
																							 };
					for(i=0;i<25;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 25;
 
					VGParameter.GradientStartEnd[0] = MeterObj[Channel].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[Channel].CenterY;

  	#if 0		//fixed direction			
					VGParameter.GradientStartEnd[2] = MeterObj[Channel].CenterX-MeterObj[Channel].O5Outer/16;
					VGParameter.GradientStartEnd[3] = MeterObj[Channel].CenterY+MeterObj[Channel].O5Outer/16;
		#else		//rotate with indi
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, Indi_deg,	MeterObj[Channel].O5Outer/16 );
					VGParameter.GradientStartEnd[2] = CoordXSrc;
					VGParameter.GradientStartEnd[3] = CoordYSrc;
		#endif  				
					VGParameter.GradientStartEnd[4] = MeterObj[Channel].O5Outer/8;
 
				 GL_DrawingCircleFill( &VGParameter);	
#endif	
}

int RETW_GUI_OPENMOTION_0_IND_3(int Channel)
{
/*				Trangle for Navi indicator,				bb and cc is NOT used.
				  																										*2
				 	 																   									cc 				 
			*1  								      															*org  																									*4	  
 
																					 										3
			*-------------------------aa----------------------------*-------------------------bb-----------------------------*							 
*/
				uint32_t i,j;
				uint32_t 	Engine_deg, New_deg, Indi_deg;
				int32_t		Indi_deg_Gap;
				st_VGDrawingParameter	VGParameter;
								
				static uint32_t IndicatorLoc[3]= {};
 
						IndicatorLoc[0] = (MeterObj[Channel].O4Outer/2)-6;																		//aa
						IndicatorLoc[1] = (MeterObj[Channel].O4Outer/2)-6;																		//bb
						IndicatorLoc[2] = (MeterObj[Channel].O4Outer/20); 
 
#if 1				

						Engine_deg = 225*8 + VirtualEngine.TargetRPM *3/4;

				#if 1
							if(Engine_deg > (VirtualEngine.IndicatorRPM+2)  )
								{
										Indi_deg_Gap = (Engine_deg - VirtualEngine.IndicatorRPM)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorRPM +=20;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorRPM +=10;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorRPM +=2;
 										else
 										VirtualEngine.IndicatorRPM +=1;
 										
//    							printf("INC....%d \n",VirtualEngine.IndicatorRPM );
 
 								}
 							else if(Engine_deg < (VirtualEngine.IndicatorRPM-2) )
 								{
										Indi_deg_Gap = (VirtualEngine.IndicatorRPM - Engine_deg)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorRPM -=12;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorRPM -=8;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorRPM -=2;
 										else
 										VirtualEngine.IndicatorRPM -=1; 									
 
 										if(VirtualEngine.IndicatorRPM < 225*8)
 										VirtualEngine.IndicatorRPM=225*8;
 //  								printf(".....DEC.....%d.\n",MainIndicatorDeg); 		
 							  }
				#endif
				
 							if(VirtualEngine.IndicatorRPM>2880)
 							Indi_deg = VirtualEngine.IndicatorRPM-2880;
							else
 							Indi_deg=VirtualEngine.IndicatorRPM;
 
#endif

//RED portion 
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[0]);																		//1
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg= CaculateIncDeg_Float(Indi_deg, 90*8);																		// 0 deg from 3 o oclock, counterclockwise

							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg , IndicatorLoc[2]);										//2
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
							VGParameter.PathDrawingData[j]  = MeterObj[Channel].CenterX; 
							VGParameter.PathDrawingData[j+1] = MeterObj[Channel].CenterY;
							VGParameter.PathPoints+=1;
							j+=2;	
 
 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							VGParameter.FillColor = 0x0000E000;													//darker RED
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);		 
 
 
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[0]);																		//1
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	 
 
 							VGParameter.PathDrawingData[j]  = MeterObj[Channel].CenterX; 
							VGParameter.PathDrawingData[j+1] = MeterObj[Channel].CenterY;
							VGParameter.PathPoints+=1;
							j+=2;	
							
 							New_deg= CaculateDecDeg_Float(Indi_deg, 90*8);																		// 0 deg from 3 o oclock, counterclockwise
 
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg , IndicatorLoc[2]);										//2
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
 
 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							VGParameter.FillColor = 0x0000FF00;
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);			
 
 //WHILE portion
 #if 1
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							New_deg= CaculateIncDeg_Float(Indi_deg, 180*8);																		// 0 deg from 3 o oclock, counterclockwise
							
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg, IndicatorLoc[1]);																		//1
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
 							New_deg= CaculateIncDeg_Float(Indi_deg, 90*8);																		// 0 deg from 3 o oclock, counterclockwise
 
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg , IndicatorLoc[2]);										//2
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			

 
							VGParameter.PathDrawingData[j]  = MeterObj[Channel].CenterX; 
							VGParameter.PathDrawingData[j+1] = MeterObj[Channel].CenterY;
							VGParameter.PathPoints+=1;
							j+=2;	
 
 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							VGParameter.FillColor = 0xE0E0E000;													//darker White
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);		  
 

 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							New_deg= CaculateIncDeg_Float(Indi_deg, 180*8);																		// 0 deg from 3 o oclock, counterclockwise
							
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg, IndicatorLoc[1]);																		//1
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			 
 
 							New_deg= CaculateDecDeg_Float(Indi_deg, 90*8);																		// 0 deg from 3 o oclock, counterclockwise
 
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg , IndicatorLoc[2]);										//2
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
 
 							VGParameter.PathDrawingData[j]  = MeterObj[Channel].CenterX; 
							VGParameter.PathDrawingData[j+1] = MeterObj[Channel].CenterY;
							VGParameter.PathPoints+=1;
							j+=2;	
							
 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							VGParameter.FillColor = 0xFFFFFF00;
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);				
							
 #endif			
}


int RETW_GUI_OPENMOTION_0_IND_3_FLY(int Channel)
{
/*				Trangle for Navi indicator
				  																		*2
				 	 																   			dd deg				 
			*1  								  	*3															*org  		  
 
																							*4
			*-------------------------aa----------------------------*
															*---------------bb--------------*
																							*-----cc--------*
			   											 
*/
				uint32_t i,j;
				uint32_t 	Engine_deg, New_deg, Indi_deg;
				int32_t		Indi_deg_Gap;
				st_VGDrawingParameter	VGParameter;
								
				static uint32_t IndicatorLoc[10]= { 2,	180-50,		 	180-25,20,		 180+25,20,		 360-2, 180-50,		0, 180-40};
 
						IndicatorLoc[0] = (MeterObj[Channel].O4Outer/2)-6;																		//aa
						IndicatorLoc[1] = (MeterObj[Channel].O5InnerR+MeterObj[Channel].O4Width/5);								//bb
						IndicatorLoc[2] = (MeterObj[Channel].O5InnerR/2);																				//cc
						IndicatorLoc[3] = 25*8;																														//dd base on 2880
 
#if 1				

						Engine_deg = 225*8 + VirtualEngine.TargetRPM *3/4;

				#if 1
							if(Engine_deg > (VirtualEngine.IndicatorRPM+2)  )
								{
										Indi_deg_Gap = (Engine_deg - VirtualEngine.IndicatorRPM)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorRPM +=20;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorRPM +=10;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorRPM +=2;
 										else
 										VirtualEngine.IndicatorRPM +=1;
 										
//    							printf("INC....%d \n",VirtualEngine.IndicatorRPM );
 
 								}
 							else if(Engine_deg < (VirtualEngine.IndicatorRPM-2) )
 								{
										Indi_deg_Gap = (VirtualEngine.IndicatorRPM - Engine_deg)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorRPM -=12;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorRPM -=8;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorRPM -=2;
 										else
 										VirtualEngine.IndicatorRPM -=1; 									
 
 										if(VirtualEngine.IndicatorRPM < 225*8)
 										VirtualEngine.IndicatorRPM=225*8;
 //  								printf(".....DEC.....%d.\n",MainIndicatorDeg); 		
 							  }
				#endif
				
 							if(VirtualEngine.IndicatorRPM>2880)
 							Indi_deg = VirtualEngine.IndicatorRPM-2880;
							else
 							Indi_deg=VirtualEngine.IndicatorRPM;
 
#endif
 
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[0]);																		//1
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
  
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg+IndicatorLoc[3], IndicatorLoc[2]);										//2
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			
 
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg , IndicatorLoc[1] );										//3
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		
												
												Indi_deg=Indi_deg+2880-IndicatorLoc[3];
												if(Indi_deg>2879)
												Indi_deg-=2880;
												
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg , IndicatorLoc[2] );									//4
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;		
/*-------------Calculate	GradientStartEnd			------*/
/*
  VG_PAINT_TYPE_COLOR                         = 0x1B00,
  VG_PAINT_TYPE_LINEAR_GRADIENT               = 0x1B01,
  VG_PAINT_TYPE_RADIAL_GRADIENT               = 0x1B02,				
*/							
#if 1	 
 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							VGParameter.FillColor = 0x0000FF00;
 	 						VGParameter.AlphaColor = 0x000000ff;

#endif
							GL_DrawingPolyconFill( &VGParameter);							
}





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



int RETW_GUI_OPENMOTION_0_IND_1(int Channel)
{
				uint32_t i,j;
				uint32_t 	Engine_deg, New_deg, Indi_deg;
				int32_t		Indi_deg_Gap;
				st_VGDrawingParameter	VGParameter;

				static uint32_t IndicatorLoc[10]= { 2,	180-50,		 	180-25,20,		 180+25,20,		 360-2, 180-50,		0, 180-40};
				
						IndicatorLoc[0] = MeterObj[Channel].O4Outer/2-5;																		//aa
						IndicatorLoc[1] = MeterObj[Channel].O4Outer/2-MeterObj[Channel].O4Outer/2/4;						//bb
						IndicatorLoc[2] = 5;																														//cc
						IndicatorLoc[3] = MeterObj[Channel].O4Outer/12 ;						//dd 
						IndicatorLoc[4] = MeterObj[Channel].O4Outer/8 ;						//ee 
						IndicatorLoc[5] = 8;																														//ff 
#if 1				
// 					Engine_deg = 225*8 + VirtualEngine.TargetRPM*270*8/ 2880;
 
				#if 0
							if(VirtualEngine.GasolineLevel > (VirtualEngine.IndicatorGas+2)  )
								{
										Indi_deg_Gap = (VirtualEngine.GasolineLevel - VirtualEngine.IndicatorGas)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorGas +=8;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorGas +=4;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorGas +=2;
 										else
 										VirtualEngine.IndicatorGas +=1;
 										
//    							printf("INC....%d \n",VirtualEngine.IndicatorGas );
 
 								}
 							else if(VirtualEngine.GasolineLevel < (VirtualEngine.IndicatorGas-2) )
 								{
										Indi_deg_Gap = (VirtualEngine.IndicatorGas - VirtualEngine.GasolineLevel)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorGas -=8;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorGas -=4;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorGas -=2;
 										else
 										VirtualEngine.IndicatorGas -=1; 									
 
 										if(VirtualEngine.IndicatorGas < 225*8)
 										VirtualEngine.IndicatorGas=225*8;
 //  								printf(".....DEC.....%d.\n",MainIndicatorDeg); 		
 							  }
				#endif
				
 							if(VirtualEngine.GasolineLevel>2880)
 							Indi_deg = VirtualEngine.GasolineLevel-2880;
							else
 							Indi_deg=VirtualEngine.GasolineLevel;
 
#endif
 		
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[0]);																		//1
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
  
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[1]);																		//bb
 							New_deg = CaculateIncDeg_Float(Indi_deg, 2880/4);
							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc,	New_deg, IndicatorLoc[2]);																		//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 																																															  //2
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			

 							New_deg = CaculateIncDeg_Float(Indi_deg, 2880/2);
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg, IndicatorLoc[3]);																		//dd
							
 							New_deg = CaculateIncDeg_Float(Indi_deg, 2880/4);
							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc,	New_deg, IndicatorLoc[5]);																		//ff
							VGParameter.PathDrawingData[j]  = CoordXSrc; 																																						//3
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
							
 							New_deg = CaculateIncDeg_Float(Indi_deg, 2880/2);
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg, IndicatorLoc[4]);																		//ee
							VGParameter.PathDrawingData[j]  = CoordXSrc; 																																						//4
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
														
 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 					//		if(Indi_deg<1440)
 					//		VGParameter.FillColor = 0x00008000;
 	 				//		else
  						VGParameter.FillColor = 0x0000C000;	 						
 
 	 						VGParameter.AlphaColor = 0x000000ff;	
							GL_DrawingPolyconFill( &VGParameter);	
 
 
 
							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[0]);																		//1
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	 

 							New_deg = CaculateIncDeg_Float(Indi_deg, 2880/2);
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg, IndicatorLoc[4]);																		//ee
							VGParameter.PathDrawingData[j]  = CoordXSrc; 																																						//4
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
							
 						
 							New_deg = CaculateIncDeg_Float(Indi_deg, 2880/2);
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg, IndicatorLoc[3]);																		//dd
							
 							New_deg = CaculateDecDeg_Float(Indi_deg, 2880/4);
							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc,	New_deg, IndicatorLoc[5]);																		//ff
							VGParameter.PathDrawingData[j]  = CoordXSrc; 																																						//5
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
							
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[1]);																		//bb
 							New_deg = CaculateDecDeg_Float(Indi_deg, 2880/4);
							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc,	New_deg, IndicatorLoc[2]);																						//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 																																															  //6
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
 
 					//		if(Indi_deg<1440)
 					 		VGParameter.FillColor = 0x0000C000;
 	 				//		else
  				//		VGParameter.FillColor = 0x00008000;	 						
 
 	 						VGParameter.AlphaColor = 0x000000ff;	
							GL_DrawingPolyconFill( &VGParameter);	
}


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

int RETW_GUI_OPENMOTION_0_IND_4_old(int Channel)
{
				uint32_t i,j;
				uint32_t 	Engine_deg, New_deg, Indi_deg;
				int32_t		Indi_deg_Gap;
				st_VGDrawingParameter	VGParameter;

				static uint32_t IndicatorLoc[10]= { 2,	180-50,		 	180-25,20,		 180+25,20,		 360-2, 180-50,		0, 180-40};
				
						IndicatorLoc[0] = MeterObj[Channel].O4Outer/2-5;																		//aa
						IndicatorLoc[1] = MeterObj[Channel].O4Outer/2-MeterObj[Channel].O4Outer/2/4;						//bb
						IndicatorLoc[2] = 5;																														//cc
						IndicatorLoc[3] = MeterObj[Channel].O4Outer/2-MeterObj[Channel].O4Outer/2/12;;						//dd 
						IndicatorLoc[4] = MeterObj[Channel].O4Outer/2-MeterObj[Channel].O4Outer/2/10;;						//ee 
						IndicatorLoc[5] = 8;																														//ff 
#if 1				
						Engine_deg = 225*8 + VirtualEngine.TargetRPM *3/4;
				#if 1
							if(Engine_deg > (VirtualEngine.IndicatorRPM+2)  )
								{
										Indi_deg_Gap = (Engine_deg - VirtualEngine.IndicatorRPM)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorRPM +=20;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorRPM +=10;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorRPM +=2;
 										else
 										VirtualEngine.IndicatorRPM +=1;
 										
//    							printf("INC....%d \n",VirtualEngine.IndicatorRPM );
 
 								}
 							else if(Engine_deg < (VirtualEngine.IndicatorRPM-2) )
 								{
										Indi_deg_Gap = (VirtualEngine.IndicatorRPM - Engine_deg)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorRPM -=12;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorRPM -=8;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorRPM -=2;
 										else
 										VirtualEngine.IndicatorRPM -=1; 									
 
 										if(VirtualEngine.IndicatorRPM < 225*8)
 										VirtualEngine.IndicatorRPM=225*8;
 //  								printf(".....DEC.....%d.\n",MainIndicatorDeg); 		
 							  }
				#endif
				
 							if(VirtualEngine.IndicatorRPM>2880)
 							Indi_deg = VirtualEngine.IndicatorRPM-2880;
							else
 							Indi_deg=VirtualEngine.IndicatorRPM;
 
#endif
 		
							
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
 
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[0]);																		//1
							VGParameter.PathDrawingData[j]  = CoordXSrc; 
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
  
  
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[1]);																		//bb
 							New_deg = CaculateIncDeg_Float(Indi_deg, 2880/4);
							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc,	New_deg, IndicatorLoc[2]);																		//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 																																															  //2
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;			

 							New_deg = CaculateIncDeg_Float(Indi_deg, 2880/2);
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg, IndicatorLoc[3]);																		//dd
							
 							New_deg = CaculateIncDeg_Float(Indi_deg, 2880/4);
							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc,	New_deg, IndicatorLoc[5]);																		//ff
							VGParameter.PathDrawingData[j]  = CoordXSrc; 																																						//3
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
							
 							New_deg = CaculateIncDeg_Float(Indi_deg, 2880/2);
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg, IndicatorLoc[4]);																		//ee
							VGParameter.PathDrawingData[j]  = CoordXSrc; 																																						//4
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
														
 							New_deg = CaculateIncDeg_Float(Indi_deg, 2880/2);
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	New_deg, IndicatorLoc[3]);																		//dd
							
 							New_deg = CaculateDecDeg_Float(Indi_deg, 2880/4);
							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc,	New_deg, IndicatorLoc[5]);																		//ff
							VGParameter.PathDrawingData[j]  = CoordXSrc; 																																						//5
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
							
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[1]);																		//bb
 							New_deg = CaculateDecDeg_Float(Indi_deg, 2880/4);
							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc,	New_deg, IndicatorLoc[2]);																						//cc
							VGParameter.PathDrawingData[j]  = CoordXSrc; 																																															  //6
							VGParameter.PathDrawingData[j+1] = CoordYSrc;
							VGParameter.PathPoints+=1;
							j+=2;	
 
/*-------------Calculate	GradientStartEnd			------*/

 							if(Indi_deg<1440)
 							{							
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[1]/6);																		 												 
 							New_deg = CaculateIncDeg_Float(Indi_deg, 2880/4);
 							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc,	New_deg, IndicatorLoc[5]);								 
  
								VGParameter.GradientStartEnd[2] = CoordXSrc;
								VGParameter.GradientStartEnd[3] = CoordYSrc;
 
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[1]/6);																		 												 
 							New_deg = CaculateDecDeg_Float(Indi_deg, 2880/4);
 							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc,	New_deg, IndicatorLoc[5]);								
 
								VGParameter.GradientStartEnd[0] = CoordXSrc;
								VGParameter.GradientStartEnd[1] = CoordYSrc;
							}
							else
 							{							
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[1]/6);																		 												 
 							New_deg = CaculateIncDeg_Float(Indi_deg, 2880/4);
 							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc,	New_deg, IndicatorLoc[5]);								 
  
								VGParameter.GradientStartEnd[0] = CoordXSrc;
								VGParameter.GradientStartEnd[1] = CoordYSrc;
 
							CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,	Indi_deg, IndicatorLoc[1]/6);																		 												 
 							New_deg = CaculateDecDeg_Float(Indi_deg, 2880/4);
 							CaculateCoordinationXYinFloat(CoordXSrc, CoordYSrc,	New_deg, IndicatorLoc[5]);								
 
								VGParameter.GradientStartEnd[2] = CoordXSrc;
								VGParameter.GradientStartEnd[3] = CoordYSrc;
							}					  
  
 
	
/*
  VG_PAINT_TYPE_COLOR                         = 0x1B00,
  VG_PAINT_TYPE_LINEAR_GRADIENT               = 0x1B01,
  VG_PAINT_TYPE_RADIAL_GRADIENT               = 0x1B02,				
*/
				static VGfloat SrampStops[35] = { 0.0f, 0.0f, 0.0f, 0.6f, 1.0f, 		
																					0.35f, 0.0f, 0.0f, 0.8f, 1.0f, 		
																					0.40f, 0.2f, 0.2f, 0.9f, 1.0f, 	
																					0.5f, 0.0f, 0.0f, 1.0f, 1.0f,	
																					0.6f, 0.0f, 0.0f, 0.7f, 1.0f, 	
																					0.65f, 0.1f, 0.1f, 0.3f, 1.0f,	
																					1.0f, 0.1f, 0.1f, 0.2f, 1.0f };
																					
 							VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
							for(i=0;i<35;i++)
							VGParameter.GradientStep[i]=SrampStops[i]; 
							VGParameter.GradientPoints = 35;		
							GL_DrawingPolyconFill( &VGParameter);	
							
}


int RETW_GUI_OM_FRAME_METAL( uint32_t ObjNum )
{
				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
				st_VGDrawArcParameter	ArcInfo;
 
 				j= ObjNum;
 				RETW_GUI_OPENMOTION_0_Mobj_Refresh(ObjNum);
 
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
 			#if 1					//Fill from O3Outer 

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[j].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[j].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[j].O3Outer;
					VGParameter.PathDrawingData[3]= MeterObj[j].O3Outer;		
								
					static VGfloat SrampStops1[35] = { 	0.0f,  0.7f,  0.3f,  0.2f, 1.0f, 		
																							0.5f,  0.0f,  0.0f,  0.0f, 1.0f, 		
																							0.80f, 0.0f,  0.0f,  0.0f, 1.0f, 
																							0.90f, 0.0f,  0.0f,  0.0f, 1.0f, 
																							0.93f, 0.2f,  0.0f,  0.0f, 1.0f, 
																							0.96f, 0.5f,  0.0f,  0.0f, 1.0f, 	
																							1.00f, 0.8f,  0.0f,  0.0f, 1.0f, 																							
																							};									
					for(i=0;i<35;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 35;
					
					VGParameter.GradientStartEnd[0] = MeterObj[j].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[j].CenterY;
					VGParameter.GradientStartEnd[2] = MeterObj[j].CenterX;
					VGParameter.GradientStartEnd[3] = MeterObj[j].CenterY;
					VGParameter.GradientStartEnd[4] = MeterObj[j].O3Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
// 				  vgFinish();
			#endif
			
			#if 1				//second inner

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[j].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[j].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[j].O2Size;
					VGParameter.PathDrawingData[3]= MeterObj[j].O2Size;	
#if 1
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
#else
					static VGfloat SrampStops2[10] = { 	0.0f, 0.0f, 0.0f, 0.0f, 1.0f,			
 		
																							1.0f, 1.0f, 1.0f, 1.0f, 1.0f 																								
																							};
					for(i=0;i<10;i++)
					VGParameter.GradientStep[i]=SrampStops2[i]; 
					VGParameter.GradientPoints = 10;		
#endif																																													
 			
					VGParameter.GradientStartEnd[0] = MeterObj[j].CenterX-(MeterObj[j].O2Size*1.8/4);
					VGParameter.GradientStartEnd[1] = MeterObj[j].CenterY+(MeterObj[j].O2Size*1.8/4);
					VGParameter.GradientStartEnd[2] = MeterObj[j].CenterX+(MeterObj[j].O2Size*1.8/4);
					VGParameter.GradientStartEnd[3] = MeterObj[j].CenterY-(MeterObj[j].O2Size*1.8/4);
					VGParameter.StrokeWidth =MeterObj[j].O2Width;
				 	GL_DrawingCircleNoFill( &VGParameter);			
// 				 	vgFinish();		 
			#endif
			
			#if 1				//outer

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[j].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[j].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[j].O1Size;
					VGParameter.PathDrawingData[3]= MeterObj[j].O1Size;	
#if 1				
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
					
#else

					static VGfloat SrampStops3[10] = { 	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		
 		
																							1.0f, 0.0f, 0.0f, 0.0f, 1.0f 																							
																							};			
																							
					for(i=0;i<10;i++)
					VGParameter.GradientStep[i]=SrampStops3[i]; 
					VGParameter.GradientPoints = 10;	
					

#endif
					VGParameter.GradientStartEnd[0] = MeterObj[j].CenterX-(MeterObj[j].O1Size*1.8/4);
					VGParameter.GradientStartEnd[1] = MeterObj[j].CenterY+(MeterObj[j].O1Size*1.8/4);
					VGParameter.GradientStartEnd[2] = MeterObj[j].CenterX+(MeterObj[j].O1Size*1.8/4);
					VGParameter.GradientStartEnd[3] = MeterObj[j].CenterY-(MeterObj[j].O1Size*1.8/4);
					VGParameter.StrokeWidth =MeterObj[j].O1Width;
					
				 GL_DrawingCircleNoFill( &VGParameter);		 
//  			 vgFinish();
 			#endif			
}

int RETW_GUI_OM_FRAME_GOLD( uint32_t ObjNum )
{
				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
				st_VGDrawArcParameter	ArcInfo;
 
 				j= ObjNum;
 				RETW_GUI_OPENMOTION_0_Mobj_Refresh(ObjNum);
 
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
 			#if 1					//Fill from O3Outer 

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[j].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[j].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[j].O3Outer;
					VGParameter.PathDrawingData[3]= MeterObj[j].O3Outer;		
								
					static VGfloat SrampStops1[35] = { 	0.0f,  0.7f,  0.3f,  0.2f, 1.0f, 		
																							0.5f,  0.0f,  0.0f,  0.0f, 1.0f, 		
																							0.80f, 0.0f,  0.0f,  0.0f, 1.0f, 
																							0.90f, 0.0f,  0.0f,  0.0f, 1.0f, 
																							0.93f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden
																							0.96f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden
																							1.00f, 0.06f, 	0.25f, 	0.35f, 1.0f 								//dark golden																								
																							};			
 																				
					for(i=0;i<35;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 35;
					
					VGParameter.GradientStartEnd[0] = MeterObj[j].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[j].CenterY;
					VGParameter.GradientStartEnd[2] = MeterObj[j].CenterX;
					VGParameter.GradientStartEnd[3] = MeterObj[j].CenterY;
					VGParameter.GradientStartEnd[4] = MeterObj[j].O3Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
 				  vgFinish();
			#endif
			
			#if 1				//second inner

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[j].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[j].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[j].O2Size;
					VGParameter.PathDrawingData[3]= MeterObj[j].O2Size;	
#if 1
					static VGfloat SrampStops2[15] = { 	0.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden		
																							0.6f, 0.06f, 	0.25f, 	0.35f, 1.0f, 								//dark golden			
																							1.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden																								
																							};
					for(i=0;i<15;i++)
					VGParameter.GradientStep[i]=SrampStops2[i]; 
					VGParameter.GradientPoints = 15;		
#else
					static VGfloat SrampStops2[10] = { 	0.0f, 0.0f, 0.0f, 0.0f, 1.0f,			
 		
																							1.0f, 1.0f, 1.0f, 1.0f, 1.0f 																								
																							};
					for(i=0;i<10;i++)
					VGParameter.GradientStep[i]=SrampStops2[i]; 
					VGParameter.GradientPoints = 10;		
#endif																																													
 			
					VGParameter.GradientStartEnd[0] = MeterObj[j].CenterX-(MeterObj[j].O2Size*1.8/4);
					VGParameter.GradientStartEnd[1] = MeterObj[j].CenterY+(MeterObj[j].O2Size*1.8/4);
					VGParameter.GradientStartEnd[2] = MeterObj[j].CenterX+(MeterObj[j].O2Size*1.8/4);
					VGParameter.GradientStartEnd[3] = MeterObj[j].CenterY-(MeterObj[j].O2Size*1.8/4);
					VGParameter.StrokeWidth =MeterObj[j].O2Width;
				 	GL_DrawingCircleNoFill( &VGParameter);			
 				 	vgFinish();		 
			#endif
			
			#if 1				//outer

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[j].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[j].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[j].O1Size;
					VGParameter.PathDrawingData[3]= MeterObj[j].O1Size;	
#if 1				
 



					static VGfloat SrampStops3[25] = { 	0.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden		
																							0.2f, 0.06f, 	0.25f, 	0.35f, 1.0f, 								//dark golden		
																							0.5f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden		
																							0.8f, 0.06f, 	0.25f, 	0.35f, 1.0f, 								//dark golden			
																							1.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden																								
																							};																																	
					for(i=0;i<25;i++)
					VGParameter.GradientStep[i]=SrampStops3[i]; 
					VGParameter.GradientPoints = 25;	
					
#else

					static VGfloat SrampStops3[10] = { 	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		
 		
																							1.0f, 0.0f, 0.0f, 0.0f, 1.0f 																							
																							};			
																							
					for(i=0;i<10;i++)
					VGParameter.GradientStep[i]=SrampStops3[i]; 
					VGParameter.GradientPoints = 10;	
					

#endif
					VGParameter.GradientStartEnd[0] = MeterObj[j].CenterX-(MeterObj[j].O1Size*1.8/4);
					VGParameter.GradientStartEnd[1] = MeterObj[j].CenterY+(MeterObj[j].O1Size*1.8/4);
					VGParameter.GradientStartEnd[2] = MeterObj[j].CenterX+(MeterObj[j].O1Size*1.8/4);
					VGParameter.GradientStartEnd[3] = MeterObj[j].CenterY-(MeterObj[j].O1Size*1.8/4);
					VGParameter.StrokeWidth =MeterObj[j].O1Width;
					
				 GL_DrawingCircleNoFill( &VGParameter);		 
  			 vgFinish();
 			#endif			
}


int RETW_GUI_OM_FRAME_CIRCLE( uint32_t ObjNum )
{
				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
				st_VGDrawArcParameter	ArcInfo;
 
 				j= ObjNum;
 				RETW_GUI_OPENMOTION_0_Mobj_Refresh(ObjNum);
 
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
 			#if 0					//Fill from O3Outer 

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[j].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[j].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[j].O3Outer;
					VGParameter.PathDrawingData[3]= MeterObj[j].O3Outer;		
								
					static VGfloat SrampStops1[35] = { 	0.0f,  0.7f,  0.3f,  0.2f, 1.0f, 		
																							0.5f,  0.0f,  0.0f,  0.0f, 1.0f, 		
																							0.80f, 0.0f,  0.0f,  0.0f, 1.0f, 
																							0.90f, 0.0f,  0.0f,  0.0f, 1.0f, 
																							0.93f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden
																							0.96f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden
																							1.00f, 0.06f, 	0.25f, 	0.35f, 1.0f 								//dark golden																								
																							};			
 																				
					for(i=0;i<35;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 35;
					
					VGParameter.GradientStartEnd[0] = MeterObj[j].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[j].CenterY;
					VGParameter.GradientStartEnd[2] = MeterObj[j].CenterX;
					VGParameter.GradientStartEnd[3] = MeterObj[j].CenterY;
					VGParameter.GradientStartEnd[4] = MeterObj[j].O3Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
 				  vgFinish();
			#endif
			
			#if 1				//second inner

 					VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;

 					VGParameter.FillColor = 0xD0D0D000;
 	 				VGParameter.AlphaColor = 0x000000ff;
 	 						 					
					VGParameter.PathDrawingData[0]= MeterObj[j].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[j].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[j].O2Size;
					VGParameter.PathDrawingData[3]= MeterObj[j].O2Size;	
 
					VGParameter.StrokeWidth =MeterObj[j].O2Width;
				 	GL_DrawingCircleNoFill( &VGParameter);			
 				 	vgFinish();		 
			#endif
			
			#if 0				//outer

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[j].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[j].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[j].O1Size;
					VGParameter.PathDrawingData[3]= MeterObj[j].O1Size;	
#if 1				
 



					static VGfloat SrampStops3[25] = { 	0.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden		
																							0.2f, 0.06f, 	0.25f, 	0.35f, 1.0f, 								//dark golden		
																							0.5f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden		
																							0.8f, 0.06f, 	0.25f, 	0.35f, 1.0f, 								//dark golden			
																							1.0f, 0.24f, 	0.7f, 	0.9f, 1.0f, 								//bright golden																								
																							};																																	
					for(i=0;i<25;i++)
					VGParameter.GradientStep[i]=SrampStops3[i]; 
					VGParameter.GradientPoints = 25;	
					
#else

					static VGfloat SrampStops3[10] = { 	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		
 		
																							1.0f, 0.0f, 0.0f, 0.0f, 1.0f 																							
																							};			
																							
					for(i=0;i<10;i++)
					VGParameter.GradientStep[i]=SrampStops3[i]; 
					VGParameter.GradientPoints = 10;	
					

#endif
					VGParameter.GradientStartEnd[0] = MeterObj[j].CenterX-(MeterObj[j].O1Size*1.8/4);
					VGParameter.GradientStartEnd[1] = MeterObj[j].CenterY+(MeterObj[j].O1Size*1.8/4);
					VGParameter.GradientStartEnd[2] = MeterObj[j].CenterX+(MeterObj[j].O1Size*1.8/4);
					VGParameter.GradientStartEnd[3] = MeterObj[j].CenterY-(MeterObj[j].O1Size*1.8/4);
					VGParameter.StrokeWidth =MeterObj[j].O1Width;
					
				 GL_DrawingCircleNoFill( &VGParameter);		 
  			 vgFinish();
 			#endif			
}




int RETW_GUI_OM_FRAME_O3( uint32_t ObjNum )
{
				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
				st_VGDrawArcParameter	ArcInfo;
 
 				j= ObjNum;
				RETW_GUI_OPENMOTION_0_Mobj_Refresh(ObjNum);
				
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
#if 1					//Fill from O3Outer 

 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[j].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[j].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[j].O3Outer;
					VGParameter.PathDrawingData[3]= MeterObj[j].O3Outer;		
								
					static VGfloat SrampStops1[35] = { 	0.0f,  0.7f,  0.3f,  0.2f, 1.0f, 		
																							0.5f,  0.0f,  0.0f,  0.0f, 1.0f, 		
																							0.80f, 0.0f,  0.0f,  0.0f, 1.0f, 
																							0.90f, 0.0f,  0.0f,  0.0f, 1.0f, 
																							0.93f, 0.2f,  0.0f,  0.0f, 1.0f, 
																							0.96f, 0.5f,  0.0f,  0.0f, 1.0f, 	
																							1.00f, 0.8f,  0.0f,  0.0f, 1.0f, 																							
																							};									
					for(i=0;i<35;i++)
					VGParameter.GradientStep[i]=SrampStops1[i]; 
					VGParameter.GradientPoints = 35;
					
					VGParameter.GradientStartEnd[0] = MeterObj[j].CenterX;
					VGParameter.GradientStartEnd[1] = MeterObj[j].CenterY;
					VGParameter.GradientStartEnd[2] = MeterObj[j].CenterX;
					VGParameter.GradientStartEnd[3] = MeterObj[j].CenterY;
					VGParameter.GradientStartEnd[4] = MeterObj[j].O3Outer/2;
				  GL_DrawingCircleFill( &VGParameter);	
 				  vgFinish();
#endif
			
#if 0				//second inner

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[j].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[j].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[j].O2Size;
					VGParameter.PathDrawingData[3]= MeterObj[j].O2Size;	
#if 1
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
#else
					static VGfloat SrampStops2[10] = { 	0.0f, 0.0f, 0.0f, 0.0f, 1.0f,			
 		
																							1.0f, 1.0f, 1.0f, 1.0f, 1.0f 																								
																							};
					for(i=0;i<10;i++)
					VGParameter.GradientStep[i]=SrampStops2[i]; 
					VGParameter.GradientPoints = 10;		
#endif																																													
																							

												
					VGParameter.GradientStartEnd[0] = MeterObj[j].CenterX-(MeterObj[j].O2Size*1.8/4);
					VGParameter.GradientStartEnd[1] = MeterObj[j].CenterY+(MeterObj[j].O2Size*1.8/4);
					VGParameter.GradientStartEnd[2] = MeterObj[j].CenterX+(MeterObj[j].O2Size*1.8/4);
					VGParameter.GradientStartEnd[3] = MeterObj[j].CenterY-(MeterObj[j].O2Size*1.8/4);
					VGParameter.StrokeWidth =MeterObj[j].O2Width;
				 	GL_DrawingCircleNoFill( &VGParameter);			
 				 	vgFinish();		 
#endif
			
#if 0				//outer

 					VGParameter.PaintMode = VG_PAINT_TYPE_LINEAR_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= MeterObj[j].CenterX;
					VGParameter.PathDrawingData[1]= MeterObj[j].CenterY;
					VGParameter.PathDrawingData[2]= MeterObj[j].O1Size;
					VGParameter.PathDrawingData[3]= MeterObj[j].O1Size;	
#if 1				
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
					
#else

					static VGfloat SrampStops3[10] = { 	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 		
 		
																							1.0f, 0.0f, 0.0f, 0.0f, 1.0f 																							
																							};			
																							
					for(i=0;i<10;i++)
					VGParameter.GradientStep[i]=SrampStops3[i]; 
					VGParameter.GradientPoints = 10;	
					

#endif
					VGParameter.GradientStartEnd[0] = MeterObj[j].CenterX-(MeterObj[j].O1Size*1.8/4);
					VGParameter.GradientStartEnd[1] = MeterObj[j].CenterY+(MeterObj[j].O1Size*1.8/4);
					VGParameter.GradientStartEnd[2] = MeterObj[j].CenterX+(MeterObj[j].O1Size*1.8/4);
					VGParameter.GradientStartEnd[3] = MeterObj[j].CenterY-(MeterObj[j].O1Size*1.8/4);
					VGParameter.StrokeWidth =MeterObj[j].O1Width;
					
				 GL_DrawingCircleNoFill( &VGParameter);		 
  			 vgFinish();
#endif			
}



void RETW_GUI_OPENMOTION_0_GRID(int Channel)
{
	if(Channel==0)
	RETW_GUI_OPENMOTION_0_GRID_0(0);
	else if(Channel==1)
	RETW_GUI_OPENMOTION_0_GRID_1(1);
	else if(Channel==2)
	RETW_GUI_OPENMOTION_0_GRID_2(2);
	else if(Channel==3)
	RETW_GUI_OPENMOTION_0_GRID_3(3);
	else if(Channel==4)
	RETW_GUI_OPENMOTION_0_GRID_4(4);	
}

void RETW_GUI_OPENMOTION_0_GRID_1(int Channel)						//Gasoline mater
{

				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
 
				st_VGDrawArcParameter	ArcInfo;
 
				VGfloat CharSize = MeterObj[Channel].O1Outer;
 
				CharSize = (float)CharSize/480;
					
 	#if 1		//RED color
 				ArcInfo.CenterX=MeterObj[Channel].CenterX;
 				ArcInfo.CenterY=MeterObj[Channel].CenterY;
 				ArcInfo.Width=			MeterObj[Channel].O3Size;
 				ArcInfo.Height=			MeterObj[Channel].O3Size;
 				ArcInfo.ArcWidth=MeterObj[Channel].O3Width;
 				
  			ArcInfo.StartAngle= 210;
 				ArcInfo.AngleExtent= 30;

 	 			ArcInfo.ArcColor=0x0000F0F0;	
 				ArcInfo.PaintMode = VG_PAINT_TYPE_COLOR;			//VG_PAINT_TYPE_LINEAR_GRADIENT;
 			  GL_DrawingArcPara(&ArcInfo);
 	#endif

 	#if 1		//Dark Yellow
 	
 				ArcInfo.ArcWidth=MeterObj[Channel].O3Width-4;
  			ArcInfo.StartAngle= 180;
 				ArcInfo.AngleExtent= 30;
 	 			ArcInfo.ArcColor=0x00D0D0F0;	
 			  GL_DrawingArcPara(&ArcInfo);
 	#endif
 
	#if 1		//Blue color
  			ArcInfo.StartAngle= 30;
 				ArcInfo.AngleExtent= 30;
 	 			ArcInfo.ArcColor=0xF00000F0;	
 			  GL_DrawingArcPara(&ArcInfo);
 	#endif
 
				for(i=0;i<211;i++)
				{
				New_deg = 210+ i;
				
				if(New_deg>359)
					New_deg = New_deg-360;
 
				New_deg = New_deg*8;
				
				if(New_deg>2879)
					New_deg = New_deg-2880;
	
  			CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3Outer/2 );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
	#if 1
  			if(  i == 0)
  					{
  					CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3InnerR+2 );
						GL_DrawingStroke(8  , 0xC0C0C0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
  			else if(  i == 52)
  					{
  					CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3InnerR+2 );
						GL_DrawingStroke(8  , 0xC0C0C0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
  			else if(  i == 105)
  					{
  					CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3InnerR+2);
						GL_DrawingStroke(8  , 0xC0C0C0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
  			else if(  i == 158)
  					{
  					CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3InnerR+2 );
						GL_DrawingStroke(8  , 0xC0C0C0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
  			else if(  i == 210)
  					{
  					CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3InnerR+2);
						GL_DrawingStroke(8  , 0xC0C0C0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
	#endif
 			}
}




void RETW_GUI_OPENMOTION_0_GRID_2(int Channel)							//RPM meter, from 0 to 6000 rpm
{

				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
 
				st_VGDrawArcParameter	ArcInfo;
 
 
				VGfloat CharSize = MeterObj[Channel].O1Outer;
 
				CharSize = (float)CharSize/470;
 	
 	#if 1		//RED color
 				ArcInfo.CenterX=MeterObj[Channel].CenterX;
 				ArcInfo.CenterY=MeterObj[Channel].CenterY;
 				ArcInfo.Width=			MeterObj[Channel].O3Size;
 				ArcInfo.Height=			MeterObj[Channel].O3Size;
  			ArcInfo.StartAngle= -30;
 				ArcInfo.AngleExtent= 30;
 				ArcInfo.ArcWidth=MeterObj[Channel].O3Width;
 	 			ArcInfo.ArcColor=0x0000A0F0;	
 				ArcInfo.PaintMode = VG_PAINT_TYPE_COLOR;			//VG_PAINT_TYPE_LINEAR_GRADIENT;
 			  GL_DrawingArcPara(&ArcInfo);
 	#endif

 	#if 1		//Dark RED color
  			ArcInfo.StartAngle= 0;
 				ArcInfo.AngleExtent= 25;
 	 			ArcInfo.ArcColor=0x000060F0;	
 			  GL_DrawingArcPara(&ArcInfo);
 	#endif
 
	#if 1		//Dark Yellow color
  			ArcInfo.StartAngle= 25;
 				ArcInfo.AngleExtent= 25;
 	 			ArcInfo.ArcColor=0x008080F0;	
 			  GL_DrawingArcPara(&ArcInfo);
 	#endif
 
#if 1			//Grid
				for(i=0;i<241;i++)
				{
				New_deg = 240+ i;
				
				if(New_deg>359)
					New_deg = New_deg-360;
 
				New_deg = New_deg*8;
				
				if(New_deg>2879)
					New_deg = New_deg-2880;
	
  			CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3Outer/2 );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
	#if 1
  			if(  i == 0  | i == 240)
  					{
  					CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3InnerR+2 );
						GL_DrawingStroke(6  , 0x000040ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
  			else if(  (i%40) == 0)
  					{
  					CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3InnerR+2);
						GL_DrawingStroke(3  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
				else if(  (i%20) == 0)

  					{
  					CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3InnerR+2 );
						GL_DrawingStroke( 2  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
	#endif
	
  #if 0			
  			if(  (i%40) == 0)
  			{
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,  New_deg, ( (MeterObj[Channel].O4Outer/2)+MeterObj[Channel].O4InnerR)/2 );
  				G_CharScale = 2.0*CharSize;;
					G_CharShearX=0;
					G_CharShearY=0;
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, i/40, 18,0xA0A0A0ff, 1);
  			}
	#endif
 
 			}
#endif  //Grid
 
// 				RETW_GUI_OM_Cap(32,32,0xFFFFFFff);
// 				RETW_GUI_OM_RED_6p_Ind( MeterObj[Channel].O3InnerR, 30, 180, 180);
}


void RETW_GUI_OPENMOTION_0_GRID_0(int Channel)							//Speed Meter
{
				uint32_t i,New_deg;
				for(i=0;i<121;i+=5)
				{
				New_deg = MeterObj[Channel].GridStart + i*(MeterObj[Channel].GridExtent)/120;
 				New_deg=New_deg*8;
 				
				if(New_deg>2879)
					New_deg = New_deg-2880;
					
  			CaculateCoordinationXYinFloat( MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	(MeterObj[Channel].O3Outer/2) );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
 
  			if(  i == 0  | i == 120)
  					{
  					CaculateCoordinationXYinFloat( MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	(MeterObj[Channel].O3Outer/2)-MeterObj[Channel].O3Width );
						GL_DrawingStroke(6  , 0x0000C0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
  			else if(  (i%10) == 0)
  					{
  					CaculateCoordinationXYinFloat( MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	(MeterObj[Channel].O3Outer/2)-MeterObj[Channel].O3Width );
						GL_DrawingStroke(6  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
 				else if(  (i%5) == 0)
  					{
  					CaculateCoordinationXYinFloat( MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	(MeterObj[Channel].O3Outer/2)-MeterObj[Channel].O3Width );
 						GL_DrawingStroke( 1  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
 						}	
 				}	
}


void RETW_GUI_OPENMOTION_0_NUMBER(int Channel)				
{

	if(Channel==0)
	RETW_GUI_OPENMOTION_0_NUMBER_0(0);
	else if(Channel==1)
	RETW_GUI_OPENMOTION_0_NUMBER_1(1);
	else if(Channel==2)
	RETW_GUI_OPENMOTION_0_NUMBER_2(2);
	else if(Channel==3)
	RETW_GUI_OPENMOTION_0_NUMBER_3(3);
	else if(Channel==4)
	RETW_GUI_OPENMOTION_0_NUMBER_4(4);	
}		 
 
void RETW_GUI_OPENMOTION_0_NUMBER_0(int Channel)							 
{
				uint32_t i,New_deg;

				VGfloat CharSize = MeterObj[Channel].O1Outer;
				
				CharSize = (float)CharSize/480;
				
 				G_CharScale = 1.4 *CharSize;
				G_CharShearX=0.2;
				G_CharShearY=0.0;	
				CharPosOffsetX = G_CharScale*9*14/2;	// + G_CharShearX*14 ;
				CharPosOffsetY = G_CharScale*12/2;
				
				for(i=0;i<121;i+=10)
				{
					New_deg = MeterObj[Channel].GridStart + i*(MeterObj[Channel].GridExtent)/120;
   				CaculateCoordinationXYin( MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,  New_deg,	( (MeterObj[Channel].O3Outer/2)+MeterObj[Channel].O4InnerR )/2     );
//  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, i, 18,  0x0064D0ff, 1);					//Warm Red
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, i, 18,  0x00C020ff, 1);					//Warm Red
  				
 				}	
}

void RETW_GUI_OPENMOTION_0_NUMBER_1(int Channel)						//Gasoline mater		, 
{

				uint32_t i,j,New_deg;
 
				VGfloat CharSize = MeterObj[Channel].O1Outer;
 
				CharSize = (float)CharSize/470;
 
 				G_CharScale = 1.8 *CharSize;
				G_CharShearX=0.0;
				G_CharShearY=0.0;	
				CharPosOffsetX = G_CharScale*8*14/2;	// + G_CharShearX*14 ;
				CharPosOffsetY = G_CharScale*12/2;
				R_TXTR_DrawStringMain("Gasoline", 8,  MeterObj[Channel].CenterX-CharPosOffsetX, MeterObj[Channel].CenterY-CharPosOffsetY -(MeterObj[Channel].O1Outer/6) , 0x00F000ff);
#if 1		 
				for(i=0;i<211;i++)
				{
				New_deg = 210+ i;
				
				if(New_deg>359)
					New_deg = New_deg-360;
 
				New_deg = New_deg*8;
				
				if(New_deg>2879)
					New_deg = New_deg-2880;
	
  			CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3Outer/2 );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
  #if 1			
  			if( i == 0)
  			{
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,  New_deg, ( (MeterObj[Channel].O4Outer/2)+MeterObj[Channel].O4InnerR)/2 );
  				G_CharScale = 1.6*CharSize;;
					G_CharShearX=0;
					G_CharShearY=0;
					CharPosOffsetX = G_CharScale*2*14/2;	// + G_CharShearX*14 ;
					CharPosOffsetY = G_CharScale*12/2;
			
					R_TXTR_DrawStringMain("0", 2,  CoordXSrc-CharPosOffsetX, CoordYSrc-CharPosOffsetY , 0xA0A0A0ff);
  			}
  			else if( i == 53)
  			{
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,  New_deg, ( (MeterObj[Channel].O4Outer/2)+MeterObj[Channel].O4InnerR)/2 );
  				G_CharScale = 1.6 *CharSize;;
					G_CharShearX=0;
					G_CharShearY=0;
					CharPosOffsetX = G_CharScale*3*14/2;	// + G_CharShearX*14 ;
					CharPosOffsetY = G_CharScale*12/2;
				R_TXTR_DrawStringMain("1/4", 3,  CoordXSrc-CharPosOffsetX, CoordYSrc-CharPosOffsetY , 0xA0A0A0ff);
  			}
  			else if( i == 105)
  			{
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,  New_deg, ( (MeterObj[Channel].O4Outer/2)+MeterObj[Channel].O4InnerR)/2 );
  				G_CharScale = 1.6 *CharSize;;
					G_CharShearX=0;
					G_CharShearY=0;
					CharPosOffsetX = G_CharScale*3*14/2;	// + G_CharShearX*14 ;
					CharPosOffsetY = G_CharScale*12/2;
				R_TXTR_DrawStringMain("1/2", 3,  CoordXSrc-CharPosOffsetX, CoordYSrc-CharPosOffsetY , 0xA0A0A0ff);
  			}
  			else if( i == 158)
  			{
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,  New_deg, ( (MeterObj[Channel].O4Outer/2)+MeterObj[Channel].O4InnerR)/2 );
  				G_CharScale = 1.6 *CharSize;;
					G_CharShearX=0;
					G_CharShearY=0;
					CharPosOffsetX = G_CharScale*3*14/2;	// + G_CharShearX*14 ;
					CharPosOffsetY = G_CharScale*12/2;
				R_TXTR_DrawStringMain("3/4", 3,  CoordXSrc-CharPosOffsetX, CoordYSrc-CharPosOffsetY , 0xA0A0A0ff);
  			}
  			else if( i == 210)
  			{
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,  New_deg, ( (MeterObj[Channel].O4Outer/2)+MeterObj[Channel].O4InnerR)/2 );
  				G_CharScale = 1.6 *CharSize;;
					G_CharShearX=0;
					G_CharShearY=0;
					CharPosOffsetX = G_CharScale*1*14/2;	// + G_CharShearX*14 ;
					CharPosOffsetY = G_CharScale*12/2;
				R_TXTR_DrawStringMain("1", 1,  CoordXSrc-CharPosOffsetX, CoordYSrc-CharPosOffsetY , 0xA0A0A0ff);
  			}
	#endif
 
 			}
#endif  //Grid
}

void RETW_GUI_OPENMOTION_0_NUMBER_2(int Channel)							//RPM meter, from 0 to 6000 rpm
{

				uint32_t i,j,New_deg;
 
				VGfloat CharSize = MeterObj[Channel].O1Outer;
 
				CharSize = (float)CharSize/470;
 
 				G_CharScale = 1.7 *CharSize;
				G_CharShearX=0.0;
				G_CharShearY=0.0;	
				CharPosOffsetX = G_CharScale*9*14/2;	// + G_CharShearX*14 ;
				CharPosOffsetY = G_CharScale*12/2;
				R_TXTR_DrawStringMain("x1000 rpm", 9,  MeterObj[Channel].CenterX-CharPosOffsetX, MeterObj[Channel].CenterY-CharPosOffsetY -(MeterObj[Channel].O1Outer*25/100) , 0x1f7db5ff);
					
 	#if 0		//RED color
 				ArcInfo.CenterX=MeterObj[Channel].CenterX;
 				ArcInfo.CenterY=MeterObj[Channel].CenterY;
 				ArcInfo.Width=			MeterObj[Channel].O3Size;
 				ArcInfo.Height=			MeterObj[Channel].O3Size;
  			ArcInfo.StartAngle= -30;
 				ArcInfo.AngleExtent= 30;
 				ArcInfo.ArcWidth=MeterObj[Channel].O3Width;
 	 			ArcInfo.ArcColor=0x0000A0F0;	
 				ArcInfo.PaintMode = VG_PAINT_TYPE_COLOR;			//VG_PAINT_TYPE_LINEAR_GRADIENT;
 			  GL_DrawingArcPara(&ArcInfo);
 	#endif

 	#if 0		//Dark RED color
  			ArcInfo.StartAngle= 0;
 				ArcInfo.AngleExtent= 25;
 	 			ArcInfo.ArcColor=0x000060F0;	
 			  GL_DrawingArcPara(&ArcInfo);
 	#endif
 
	#if 0		//Dark Yellow color
  			ArcInfo.StartAngle= 25;
 				ArcInfo.AngleExtent= 25;
 	 			ArcInfo.ArcColor=0x008080F0;	
 			  GL_DrawingArcPara(&ArcInfo);
 	#endif
 
#if 1			//Grid
				for(i=0;i<241;i++)
				{
				New_deg = 240+ i;
				
				if(New_deg>359)
					New_deg = New_deg-360;
 
				New_deg = New_deg*8;
				
				if(New_deg>2879)
					New_deg = New_deg-2880;
	
  			CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3Outer/2 );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
	#if 0
  			if(  i == 0  | i == 240)
  					{
  					CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3InnerR+2 );
						GL_DrawingStroke(6  , 0x000040ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
  			else if(  (i%40) == 0)
  					{
  					CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3InnerR+2);
						GL_DrawingStroke(3  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
				else if(  (i%20) == 0)

  					{
  					CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3InnerR+2 );
						GL_DrawingStroke( 2  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
	#endif
	
  #if 1			
  			if(  (i%40) == 0)
  			{
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,  New_deg, ( (MeterObj[Channel].O4Outer/2)+MeterObj[Channel].O4InnerR)/2 );
  				G_CharScale = 2.0*CharSize;;
					G_CharShearX=0;
					G_CharShearY=0;
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, i/40, 18,0xA0A0A0ff, 1);
  			}
	#endif
 
 			}
#endif  //Grid
}


void RETW_GUI_OPENMOTION_0_NUMBER_3(int Channel)						 
{
				uint32_t i,j;
				uint32_t 	Engine_deg, New_deg, Indi_deg;
				int32_t		Indi_deg_Gap;
				VGfloat CharSize = MeterObj[Channel].O1Outer;
 
#if 1				

						Engine_deg = 225*8 + VirtualEngine.TargetRPM *3/4;

				#if 1
							if(Engine_deg > (VirtualEngine.IndicatorRPM+2)  )
								{
										Indi_deg_Gap = (Engine_deg - VirtualEngine.IndicatorRPM)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorRPM +=20;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorRPM +=10;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorRPM +=2;
 										else
 										VirtualEngine.IndicatorRPM +=1;
 										
//    							printf("INC....%d \n",VirtualEngine.IndicatorRPM );
 
 								}
 							else if(Engine_deg < (VirtualEngine.IndicatorRPM-2) )
 								{
										Indi_deg_Gap = (VirtualEngine.IndicatorRPM - Engine_deg)/8;
										
										if(Indi_deg_Gap> 3)
										VirtualEngine.IndicatorRPM -=12;										
										else if(Indi_deg_Gap> 2)
 										VirtualEngine.IndicatorRPM -=8;
 										else if (Indi_deg_Gap> 1)
 										VirtualEngine.IndicatorRPM -=2;
 										else
 										VirtualEngine.IndicatorRPM -=1; 									
 
 										if(VirtualEngine.IndicatorRPM < 225*8)
 										VirtualEngine.IndicatorRPM=225*8;
 //  								printf(".....DEC.....%d.\n",MainIndicatorDeg); 		
 							  }
				#endif
				
 							if(VirtualEngine.IndicatorRPM>2880)
 							Indi_deg = VirtualEngine.IndicatorRPM-2880;
							else
 							Indi_deg=VirtualEngine.IndicatorRPM;
 
#endif

				CharSize = (float)CharSize/470;
 
 				G_CharScale = 2.0 *CharSize;
				G_CharShearX=0.0;
				G_CharShearY=0.0;	
				CharPosOffsetX = G_CharScale*4*14/2;	// + G_CharShearX*14 ;
				CharPosOffsetY = (G_CharScale*12/2);
				
				
				R_TXTR_DrawStringMain("Navi", 4,  MeterObj[Channel].CenterX-CharPosOffsetX, MeterObj[Channel].CenterY-CharPosOffsetY- (MeterObj[Channel].O4Outer/5) , 0xFFFF30ff);
					
			for(i=0;i<360;i++)
			{
				New_deg = i*8;
				
				
				New_deg=New_deg+Indi_deg;
				
				if(New_deg>2879)
					New_deg = New_deg-2880;
	
 		
  			if(  (i) == 0)
  			{
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,  New_deg, ( (MeterObj[Channel].O4Outer/2)+MeterObj[Channel].O4InnerR)/2 );
 					G_CharScale = 2.4 * CharSize;
					G_CharShearX=0.0;
					G_CharShearY=0.0;
					CharPosOffsetX = G_CharScale*1*14/2;	// + G_CharShearX*14 ;
					CharPosOffsetY = G_CharScale*12/2;
				R_TXTR_DrawStringMain("N", 1,  CoordXSrc-CharPosOffsetX, CoordYSrc-CharPosOffsetY , 0x0000FFff);
  			}
  			else if(  (i) == 90)
  			{
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,  New_deg, ( (MeterObj[Channel].O4Outer/2)+MeterObj[Channel].O4InnerR)/2 );
 					G_CharScale = 2.4 * CharSize;
					G_CharShearX=0.3;
					G_CharShearY=0.0;
					CharPosOffsetX = G_CharScale*1*14/2;	// + G_CharShearX*14 ;
					CharPosOffsetY = G_CharScale*12/2;
				R_TXTR_DrawStringMain("E", 1,  CoordXSrc-CharPosOffsetX, CoordYSrc-CharPosOffsetY , 0x00FF00ff);
  			} 
  			else if(  (i) == 180)
  			{
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,  New_deg, ( (MeterObj[Channel].O4Outer/2)+MeterObj[Channel].O4InnerR)/2 );
 					G_CharScale = 2.4 * CharSize;
					G_CharShearX=0.0;
					G_CharShearY=0.2;
					CharPosOffsetX = G_CharScale*1*14/2;	// + G_CharShearX*14 ;
					CharPosOffsetY = G_CharScale*12/2;
				R_TXTR_DrawStringMain("S", 1,  CoordXSrc-CharPosOffsetX, CoordYSrc-CharPosOffsetY , 0x00FFFFff);
  			} 
  			else if(  (i) == 270)
  			{
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,  New_deg, ( (MeterObj[Channel].O4Outer/2)+MeterObj[Channel].O4InnerR)/2 );
 					G_CharScale = 2.4 * CharSize;
					G_CharShearX=-0.2;
					G_CharShearY=0.0;
					CharPosOffsetX = G_CharScale*1*14/2;	// + G_CharShearX*14 ;
					CharPosOffsetY = G_CharScale*12/2;
				R_TXTR_DrawStringMain("W", 1,  CoordXSrc-CharPosOffsetX, CoordYSrc-CharPosOffsetY , 0xFF00FFff);
  			}  
 			}
}

void RETW_GUI_OPENMOTION_0_NUMBER_4(int Channel)						 
{

				uint32_t i,j,New_deg;
 
				VGfloat CharSize = MeterObj[Channel].O1Outer;
 
				CharSize = (float)CharSize/470;

#if 0
 				G_CharScale = 1.8 *CharSize;
				G_CharShearX=0.5;
				G_CharShearY=0.0;	
				CharPosOffsetX = G_CharScale*5*14/2;	// + G_CharShearX*14 ;
				CharPosOffsetY = G_CharScale*12/2;
 
				R_TXTR_DrawStringMain("Clock", 5,  MeterObj[Channel].CenterX-CharPosOffsetX, MeterObj[Channel].CenterY-CharPosOffsetY -(MeterObj[Channel].O1Outer*15/100) , 0x1f7db5ff);		//Golden color
#endif				
				
#if 1			//Grid
				for(i=0;i<360;i++)
				{
				New_deg = i*8;
				
				if(New_deg>2879)
					New_deg = New_deg-2880;
	
  			CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3Outer/2 );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
  			
  #if 1			

  			if(  (i) == 0)
  			{
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,  New_deg, ( (MeterObj[Channel].O4Outer/2)+MeterObj[Channel].O4InnerR)/2 );
  				G_CharScale = 3.0*CharSize;;
					G_CharShearX=0.2;
					G_CharShearY=0;
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, 12, 18,0x0000F0ff, 1);
  			}
  			else if(  (i%90) == 0)
  			{
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,  New_deg, ( (MeterObj[Channel].O4Outer/2)+MeterObj[Channel].O4InnerR)/2 );
  				G_CharScale = 2.0*CharSize;;
					G_CharShearX=0;
					G_CharShearY=0;
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, i/30, 18,0xA0A0A0ff, 1);
  			}
	#endif
 
 			}
#endif  //Grid
}


void RETW_GUI_OPENMOTION_0_GRID_3(int Channel)							//RPM meter, from 0 to 6000 rpm
{
 
}
void RETW_GUI_OPENMOTION_0_GRID_4(int Channel)							//RPM meter, from 0 to 6000 rpm
{
				uint32_t i,j,New_deg;
				st_VGDrawingParameter	VGParameter;
 
				VGfloat CharSize = MeterObj[Channel].O1Outer;
#if 0 
				CharSize = (float)CharSize/470;
 
 				G_CharScale = 1.5 *CharSize;
				G_CharShearX=0.0;
				G_CharShearY=0.0;	
				CharPosOffsetX = G_CharScale*5*14/2;	// + G_CharShearX*14 ;
				CharPosOffsetY = G_CharScale*12/2;
				R_TXTR_DrawStringMain("Clock", 5,  MeterObj[Channel].CenterX-CharPosOffsetX, MeterObj[Channel].CenterY-CharPosOffsetY -(MeterObj[Channel].O1Outer*25/100) , 0xFF0000ff);
#endif
#if 1			//Grid
				for(i=0;i<360;i++)
				{
 				New_deg = i*8;
	
  			CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3Outer/2 );
  			CoordXDest = CoordXSrc;
  			CoordYDest = CoordYSrc;
	#if 1
  			if(  i == 0  | i == 90 | i == 180 | i == 270)
  					{
  					CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3InnerR+2 );
						GL_DrawingStroke(6  , 0x000080ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
  			else if(  (i%30) == 0)
  					{
  					CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY, New_deg,	MeterObj[Channel].O3InnerR+2);
						GL_DrawingStroke(3  , 0xA0A0A0ff,VG_CAP_BUTT);									// Indicator by Draw Line with Cap    
						}
	#endif
	
  #if 0			
  			if(  (i%40) == 0)
  			{
  				CaculateCoordinationXYinFloat(MeterObj[Channel].CenterX, MeterObj[Channel].CenterY,  New_deg, ( (MeterObj[Channel].O4Outer/2)+MeterObj[Channel].O4InnerR)/2 );
  				G_CharScale = 2.0*CharSize;
					G_CharShearX=0;
					G_CharShearY=0;
  				RETW_GUI_DrawScriptNumber(	CoordXSrc, CoordYSrc, i/40, 18,0xA0A0A0ff, 1);
  			}
	#endif
 
 			}
#endif  //Grid
 
// 				RETW_GUI_OM_Cap(32,32,0xFFFFFFff);
// 				RETW_GUI_OM_RED_6p_Ind( MeterObj[Channel].O3InnerR, 30, 180, 180);


}




int RETW_GUI_OM_RED_6p_Ind(uint32_t MaxLength, uint32_t speed, uint32_t StartDeg, uint32_t ExtentDeg )
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


int RETW_GUI_OM_Cap(uint32_t width, uint32_t height, uint32_t color)
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

#endif			//if(  SYSTEM_DISPLAY==SYSdef_DSP_800480 )
