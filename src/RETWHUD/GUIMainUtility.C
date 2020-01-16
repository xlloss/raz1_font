
#include    <stdio.h>
#include    <string.h>
#include    <math.h>
#include    "r_typedefs.h"

#include    "VG/openvg.h"
#include    "VG/vgu.h"
#include 		"rtc.h"
#include  	"RETWvg.h"
#include  	"retwhmi.h"
 
int CaculateCoordinationF(VGuint Indi_deg, VGuint IndiLength)												// 0 deg from twelve oclock, new coord is absolute x,y
{
	
	
						VGuint Indicator_deg = Indi_deg + 270;
 
						if(Indicator_deg>359)
						Indicator_deg=Indicator_deg-360;
						
						Indicator_deg = 360-Indicator_deg;
						if(Indicator_deg>359)
						Indicator_deg=Indicator_deg-360;
 
//						printf("Deg %d, %d  \n",Indi_deg,  Indicator_deg);
 
 						CoordXSrc = ClockCenterX + cos(PI* Indicator_deg/180)*IndiLength;			
 						CoordYSrc = ClockCenterY + sin(PI* Indicator_deg/180)*IndiLength;
}

int CaculateCoordination(VGuint Indi_deg, VGuint IndiLength)												// copy of about original, 0 deg from twelve oclock, new coord is absolute x,y
{
						VGuint Indicator_deg=Indi_deg;

						if(Indicator_deg>359)
						Indicator_deg=Indicator_deg-360;
	 	
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
}


int CaculateCoordinationXYin(VGuint x, VGuint y, VGuint Indi_deg, VGuint IndiLength)												// 0 deg from twelve oclock
{
						VGuint Indicator_deg=Indi_deg;

						if(Indicator_deg>359)
						Indicator_deg=Indicator_deg-360;
	 	
	 					if(Indicator_deg<90)
 						{
 							CoordXSrc = x + (SINtable[Indicator_deg]*IndiLength)/10000;
 							CoordYSrc = y + (COStable[Indicator_deg]*IndiLength)/10000;
 						}
 						
 						else if(Indicator_deg<180)
 						{
 							CoordXSrc = x + (COStable[Indicator_deg-90]*IndiLength)/10000;
 							CoordYSrc = y - (SINtable[Indicator_deg-90]*IndiLength)/10000;
 						} 
 						else if(Indicator_deg<270)
 						{
 							CoordXSrc = x - (SINtable[Indicator_deg-180]*IndiLength)/10000;
 							CoordYSrc = y - (COStable[Indicator_deg-180]*IndiLength)/10000;
 						}  
 						else 
 						{
 							CoordXSrc = x - (COStable[Indicator_deg-270]*IndiLength)/10000;
 							CoordYSrc = y + (SINtable[Indicator_deg-270]*IndiLength)/10000;
 						}  
}

/****************************************************************************************************
* 			Indi_deg : from 0 to 2879				total 2880 deg,  0 deg from twelve oclock										* 
*			Input : Indi_deg   0 to 2879  clockwise from 12 o'clcok																			  *
*			Indicator_deg   0 to 2879  counterclockwise from 3 o'clcok																	  *
*****************************************************************************************************/

int CaculateCoordinationXYinFloat(VGint x, VGint y, VGuint Indi_deg, VGuint IndiLength)												// 0 deg from twelve oclock
{
						VGuint Indicator_deg = Indi_deg + 2160;
 
						if(Indicator_deg>2879)
						Indicator_deg=Indicator_deg-2880;
						
						Indicator_deg = 2880-Indicator_deg;
						if(Indicator_deg>2879)
						Indicator_deg=Indicator_deg-2880;
 
//						printf("Deg %d, %d  \n",Indi_deg,  Indicator_deg);
 
 						CoordXSrc = x + cos(PI* Indicator_deg/1440)*IndiLength;			
 						CoordYSrc = y + sin(PI* Indicator_deg/1440)*IndiLength;
 	
}


int RETW_GUI_FrameRateCheckOut(void )										 
{
					FrameRate = FrameCountPs; 
 					FrameCountPs = 0;
}
int RETW_GUI_ImportGlobalTime(void )	
{
					uint8_t SECOND,MINUTE,HOUR;
 
					SECOND = (uint8_t)((SYSTEMTIME.Second >> 4) *10 +  (SYSTEMTIME.Second&0x0F) );
					MINUTE = (uint8_t)((SYSTEMTIME.Minute >> 4) *10 +  (SYSTEMTIME.Minute&0x0F) );
					HOUR = (uint8_t)((SYSTEMTIME.Hour >> 4) *10 +  (SYSTEMTIME.Hour&0x0F) );

        	GlobalSecDeg = SECOND*6;
        	GlobalMinDeg = MINUTE*6 + SECOND*6/60;
        	if(HOUR >11)
        	HOUR = HOUR-12;
        	GlobalHourDeg = HOUR*30 + MINUTE*30 /60;      	
}

int RETW_GUI_ImportGlobalTimeMotion(void )	
{
					uint8_t SECOND,MINUTE,HOUR;
					uint8_t HundredInSec;

 					HundredInSec = (uint8_t) RETW_Tick_ThreadSchedulingReadCnt(0);
 					
					SECOND = (uint8_t)((SYSTEMTIME.Second >> 4) *10 +  (SYSTEMTIME.Second&0x0F) );
					MINUTE = (uint8_t)((SYSTEMTIME.Minute >> 4) *10 +  (SYSTEMTIME.Minute&0x0F) );
					HOUR = (uint8_t)((SYSTEMTIME.Hour >> 4) *10 +  (SYSTEMTIME.Hour&0x0F) );

        	GlobalSecDeg = SECOND*6 +  (6* HundredInSec/99);
        		if(GlobalSecDeg>359)
        		GlobalSecDeg=GlobalSecDeg-360;
        	
        	GlobalMinDeg = MINUTE*6 + SECOND*6/60;
        	
        	if(HOUR >11)
        	HOUR = HOUR-12;
        	GlobalHourDeg = HOUR*30 + MINUTE*30 /60;      	
}

int RETW_GUI_ImportGlobalTimeMotion1(void )	
{
					uint8_t SECOND,MINUTE,HOUR;
					uint8_t HundredInSec;

 					HundredInSec = (uint8_t) RETW_Tick_ThreadSchedulingReadCnt(0);
 					
					SECOND = (uint8_t)((SYSTEMTIME.Second >> 4) *10 +  (SYSTEMTIME.Second&0x0F) );
					MINUTE = (uint8_t)((SYSTEMTIME.Minute >> 4) *10 +  (SYSTEMTIME.Minute&0x0F) );
					HOUR = (uint8_t)((SYSTEMTIME.Hour >> 4) *10 +  (SYSTEMTIME.Hour&0x0F) );
 
					if(HundredInSec<15)
        		GlobalSecDeg = SECOND*6 +  (8* HundredInSec/14);
        	else if (HundredInSec<25)
 						GlobalSecDeg = SECOND*6 + 6 +  (2* (25-HundredInSec)/9);
        	else if (HundredInSec<30)
        		GlobalSecDeg = SECOND*6 + 6 -1;
        	else 
 						GlobalSecDeg = SECOND*6 + 6 ;
 
        		if(GlobalSecDeg>359)
        		GlobalSecDeg=GlobalSecDeg-360;
        		
        	GlobalMinDeg = MINUTE*6 + SECOND*6/60;
        	
        	if(HOUR >11)
        	HOUR = HOUR-12;
        	GlobalHourDeg = HOUR*30 + MINUTE*30 /60;      	
}

int RETW_GUI_ImportGlobalTimeMotionHighReso(void )	
{
					uint8_t SECOND,MINUTE,HOUR;
					uint8_t HundredInSec;

 					HundredInSec = (uint8_t) RETW_Tick_ThreadSchedulingReadCnt(0);
 					
					SECOND = (uint8_t)((SYSTEMTIME.Second >> 4) *10 +  (SYSTEMTIME.Second&0x0F) );
					MINUTE = (uint8_t)((SYSTEMTIME.Minute >> 4) *10 +  (SYSTEMTIME.Minute&0x0F) );
					HOUR = (uint8_t)((SYSTEMTIME.Hour >> 4) *10 +  (SYSTEMTIME.Hour&0x0F) );

#if 0		//jump and bound 
					if(HundredInSec<15)
        		GlobalSecDeg_HRS = SECOND*6*8 +  (8*8* HundredInSec/14);

        	else if (HundredInSec<25)
 						GlobalSecDeg_HRS = SECOND*6*8 + 6*8 +  (2*8* (25-HundredInSec)/9);

        	else if (HundredInSec<30)
        		GlobalSecDeg_HRS = SECOND*6*8 + 5*8;
        	else 
 						GlobalSecDeg_HRS = SECOND*6*8 + 6*8 ;
#endif

#if 0 	//jump
					GlobalSecDeg_HRS = SECOND*6*8 ;
#endif

#if 1 	//moving
        	GlobalSecDeg_HRS = SECOND*6*8 +  (6*8* HundredInSec/99);
        		if(GlobalSecDeg_HRS>2879)
        		GlobalSecDeg_HRS=GlobalSecDeg_HRS-2880;
#endif

 
        		if(GlobalSecDeg_HRS > 2879)
        		GlobalSecDeg_HRS = GlobalSecDeg_HRS-2880;
        		
        	GlobalMinDeg_HRS = (MINUTE*6*8 + SECOND*6*8/60);
        	
        	if(HOUR >11)
        	HOUR = HOUR-12;
        	GlobalHourDeg_HRS = (HOUR*30*8 + MINUTE*30*8 /60);   
   	   	
}


int CaculateIncDeg(VGuint Current_deg, VGuint Deg)																		// 0 deg from twelve oclock
{
	VGuint New_deg = Current_deg+Deg;
	if(New_deg>359)
		New_deg = New_deg - 360;
 
		return New_deg;
}

int CaculateDecDeg(VGuint Current_deg, VGuint Deg)																		// 0 deg from twelve oclock
{
	VGuint New_deg;
	
	if(Current_deg < Deg)
			New_deg = Current_deg+360 - Deg;
	else
			New_deg = Current_deg - Deg;
		
		return New_deg;
}

int CaculateIncDeg_Float(VGuint Current_deg, VGuint Deg)																		// 0 deg from 3 o oclock, counterclockwise
{
	VGuint New_deg = Current_deg+Deg;
	if(New_deg>2879)
		New_deg = New_deg - 2880;
		return New_deg;
}

int CaculateDecDeg_Float(VGuint Current_deg, VGuint Deg)																		// 0 deg from 3 o oclock, counterclockwise
{
	VGuint New_deg;
	
	if(Current_deg < Deg)
			New_deg = Current_deg+2880 - Deg;
	else
			New_deg = Current_deg - Deg;
		
		return New_deg;
}







 