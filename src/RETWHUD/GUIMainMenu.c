/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         draw_vg_sample.c
* @version      0.01
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* @brief        OpenVG sample
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    <stdio.h>

#include    <string.h>

#include    "r_typedefs.h"

#include    "EGL/egl.h"
#include    "EGL/eglext.h"
#include    "VG/openvg.h"
#include    "VG/vgu.h"

#include    "ncg_defs.h"
#include    "ncg_memory.h"

#include 		"RETWvg.h"
#include 		"rtc.h"
#include 		"SINCOS.h" 
 
#define PI 					(3.14159265)
#define CIRCLE_DEG 	(2880)


typedef	struct MenuItemScript{
	unsigned int				Point;
	unsigned char				*CharString;
	unsigned char				*CharString1;	
	unsigned char				*CharString2;	
	unsigned char				*CharString3;	
	unsigned char				*CharString4;	
	unsigned char				*CharString5;	
	unsigned int				CharNum;
	unsigned int				Value;
	unsigned int				DispDigit;
 	unsigned int				MinValue;
 	unsigned int				MaxValue;
} MenuItemScript;

MenuItemScript MenuDataPage[8]={
	{0,'Y','e','a','r',' ',' ',		4, 	2016 , 4,	2016,		2100 }, 
	{1,'M','o','n','t','h',' ',		5, 	0    , 2,	1,			12 }, 
	{2,'D','a','y',' ',' ',' ',		3, 	0    , 2,	1,			31 }, 
	{3,'H','o','u','r',' ',' ',		4, 	0    , 2,	0,			23 }, 
	{4,'M','i','n','u','t','e', 	6, 	0    , 2,	0,			59 }, 
	{5,'S','e','c','o','n','d',	  6, 	0    , 2,	0,			59 },
	{6,'M','O','D','E',' ',' ',		4, 	0    , 2,	0,			6 }, 
	{7,'E','X','I','T',' ',' ',		4, 	0    , 4,	0,			1 } 
};

typedef enum {
  GUI_SetupItem_YEAR =0,                         
  GUI_SetupItem_MONTH,     
  GUI_SetupItem_DAY,       
  GUI_SetupItem_HOUR,                  
  GUI_SetupItem_MINUTE,    
  GUI_SetupItem_SECOND,    
  GUI_SetupItem_MODE,
  GUI_SetupItem_EXIT,
	GUI_SetupItem_MAX                   
} MENU_SetupItemPoint;
 
int GlobalSecDeg=120;
int GlobalMinDeg=60;
int GlobalHourDeg=6;

int GlobalSecDeg_HRS=0;
int GlobalMinDeg_HRS=0;
int GlobalHourDeg_HRS=0;

VGint ClockShiftX = 0;
VGint ClockShiftDir = 1;

VGint ClockCenterX = 400;
VGint ClockCenterY = 240;

VGint CoordXSrc;
VGint CoordYSrc;
VGint CoordXDest;
VGint CoordYDest;

st_ClockIndiSet ClockIndicator;

char CleanBufferFlag=0;

extern VGfloat 	G_CharScale;
extern VGfloat 	G_CharShearX;
extern VGfloat 	G_CharShearY;

extern uint32_t Milisec_ModeEntry;
 
uint32_t	CharPosOffsetX;
uint32_t	CharPosOffsetY;	

 uint32_t framecount = 0;
 uint32_t totalframecnt=0;			
				

/******************************************************************************
Macro definitions
******************************************************************************/
#define     FRAME_COUNT_MAX         (30u)
 
/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern uint16_t Graphics_ImageData[];
extern uint32_t Graphics_ImageData32[];					/* 32bits= B,G,R,00*/
extern uint8_t __image_btn1[];
extern uint8_t _CLOCKS_ADDR[];
 

extern st_RTCTIME	SYSTEMTIME;
		
/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/
void GUI_GraphicsTask_Main(uint32_t OpMode);

extern void RETW_GUI_OPENMOTION_0(); 
extern void ALPHA_DISPLAY();
extern void RETW_GUI_DEMO_0();
extern void RETW_GUI_Meter_0();
extern void RETW_GUI_Meter_1();
extern void RETW_GUI_Meter_2();
extern void RETW_GUI_Meter_3();
extern void RETW_GUI_Meter_4();
extern void RETW_GUI_Clock_0();
extern void RETW_GUI_Clock_1();
extern void RETW_GUI_Clock_2();
extern void RETW_GUI_Clock_3();
extern void RETW_GUI_Meter_Speed_5();

 
		uint32_t seccnt = 0;
    uint32_t speedcount = 0;
 
    uint32_t EngineRPM; 
		uint32_t OperatingMode=0;   
 		uint32_t LastMode=100;   
 		uint32_t ModeEntryCnt=0;
		uint8_t	InputUARTKey; 
		
		extern uint8_t rtc_halfsecond_flag;
		extern uint32_t Milisec_VirtualEngineTickCount;

		st_VGSetupMenuParameter SetupMenuIndex;    
		st_VG_ObjProperty 				MeterObj[6];
		
//		st_VG_ObjProperty 				ClockObj[100];
		
		
		
		
		st_VirtualEngine_Info		VirtualEngine;
		
		
 extern uint8_t _RLOGO_ADDR[];
extern uint32_t VG_Back_FrameBuffer;


/******************************************************************************
Private global variables and functions
******************************************************************************/
static VGint scr_width;
static VGint scr_height;

static void (* GUI_func_table[])(uint32_t Arg0, uint32_t Arg1) =
{
// 		ALPHA_DISPLAY,
    RETW_GUI_OPENMOTION_0,     
    RETW_GUI_DEMO_0,
    RETW_GUI_Meter_1,
//    RETW_GUI_Meter_2,
//    RETW_GUI_Meter_3,
//    RETW_GUI_Meter_4,
//		RETW_GUI_Meter_Speed_5,
    RETW_GUI_Clock_0,
    RETW_GUI_Clock_1,
    RETW_GUI_Clock_2, 					//Golden
    RETW_GUI_Clock_3 
};

/**************************************************************************//**
 * @brief       
 * @param[in]   framenum
 * @retval      Error code. If an error occurs, a negative number will be returned.
******************************************************************************/
static int32_t DrawImage (uint32_t framenum)
{
    VGImage Image;
    VGint   width;
    VGint   height;
    VGfloat scale;
    
    width   = 320;
    height  = 200;

    vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
		vgLoadIdentity();

    vgTranslate(200.0f, 40.0f);
    scale = (VGfloat)framenum / (VGfloat)FRAME_COUNT_MAX + 1.0f;
		scale = 2.0f;
    
    vgScale(scale, scale);

    /* Set resampling quality */
    vgSeti(VG_IMAGE_QUALITY, VG_IMAGE_QUALITY_BETTER);

    /* Create a new image */
    Image = vgCreateImage(VG_lXRGB_8888, width, height, VG_IMAGE_QUALITY_BETTER);
    if (Image == VG_INVALID_HANDLE)
    {
        return -1;
    }
    /* Append the image data to the image object. If necessary, data format conversion will be done. */
    vgImageSubData(
        Image,
        Graphics_ImageData32,
        width * (sizeof (uint32_t)),
        VG_lXRGB_8888,																	//VG_sRGB_565,
        (VGint)0,
        (VGint)0,
        width,
        height);

    /* Draw the image to the current drawing surface */
    vgDrawImage(Image);

    /* Destroy the image */
    vgDestroyImage(Image);
 
#if 0		//RETW try to draw screen by software

		if(framenum == 10)
		seccnt+=1;
		
		if(seccnt > 2)
		{
    			vgFinish();
					FillScreen(0x80000000);				//clear screen
					FillScreenColorBar();
					while(1){}
		}

#endif

    return 0;
}   /* End of function DrawImage() */

int32_t GRAPHICS_DrawVgImage_PrepareImage()
{
	uint16_t i,j;
	uint32_t InData,R,G,B,OutData;

//					FillScreenColorBar();
//         	DspBMP(__image_btn1, 0,0,Graphics_ImageData32, 1, 1);					//OpenVG use  buffer Graphics_ImageData32, scan from bottom
 
// 					for(i=0;i<40000;i++)
//					Graphics_ImageData32[i]=0x00FF00FF;

			DspBMP(_CLOCKS_ADDR,0,0,Graphics_ImageData32, 1, 0);								//OpenVG use  buffer Graphics_ImageData32, scan from bottom  
 
#if 0
			for(i=0;i<50*200;i++)
 				Graphics_ImageData32[i] = 0x0003f000;
			for(i=50*200;i<100*200;i++)
 				Graphics_ImageData32[i] = 0x00000fc0; 
			for(i=100*200;i<150*200;i++)
 				Graphics_ImageData32[i] = 0x0000003f; 				
			for(i=150*200;i<200*200;i++)
 				Graphics_ImageData32[i] = 0x0003ffff; 				
#endif

#if 0
			for(j=0;j<200;j++)
				{
				for(i=0;i<200;i++)
						{	if(i<50)
 							Graphics_ImageData32[200*j+i] =  ((j<<16) &0x00ff0000);
 							else if (i<100)
 							Graphics_ImageData32[200*j+i] =  ((j<<8) &0x0000ff00);
 							else
 							Graphics_ImageData32[200*j+i] =  ((j) &0x000000ff);
 						} 
				}
#endif

#if 0
			for(j=0;j<200;j++)
				{
				for(i=0;i<200;i++)
						{	if(i<50)
 							Graphics_ImageData32[200*j+i] =  ((j<<16) &0x003f0000);
 							else if (i<100)
 							Graphics_ImageData32[200*j+i] =  ((j<<8) &0x00003f00);
 							else
 							Graphics_ImageData32[200*j+i] =  ((j) &0x0000003f);
 						} 
				}
#endif

}

/**************************************************************************//**
 * @brief       
 * @param[in]   width
 * @param[in]   height
 * @retval      Error code. If an error occurs, a negative number will be returned.
******************************************************************************/

  
void GUI_GraphicsTask_Main(uint32_t OpMode)
{
    int32_t         ret;
 
// 					 	printf("Size of GUI_func_table %d\n",sizeof GUI_func_table/sizeof (uint32_t) );
#if 1
		if(OpMode != LastMode)
			{
			ModeEntryCnt =0;
			Milisec_ModeEntry=0;
			LastMode = OpMode;
			}
#endif

									G_CharShearX=0.5;
									G_CharShearY=0.0;
 									G_CharScale = 2.0;
 					 									
 									
				ModeEntryCnt+=1;
			if(ModeEntryCnt>6000)
			ModeEntryCnt=6000;
			
 									
 
#if 0									
		switch (OpMode)
		{
				case 0:
 							RETW_GUI_PoliconFillScreen(0,0,800,480,0x101010ff);
// 							RETW_GUI_DottingFillScreen(0,0,800,480,0x002000ff);
		 					RETW_GUI_Meter_0(speedcount,220);
							R_TXTR_DrawStringMain( "Motor Meter 0" , 13, 10, 30, 0xFFFFFFFF);
							DspBMP(_RLOGO_ADDR, 590,420, VG_Back_FrameBuffer, 0,0);			
							break;	
				case 1:
 							RETW_GUI_PoliconFillScreen(0,0,800,480,0x010101ff);
// 							RETW_GUI_DottingFillScreen(0,0,800,480,0x002000ff);
		 					RETW_GUI_Meter_Speed_5(speedcount,220);
							R_TXTR_DrawStringMain( "Motor Meter 5" , 13, 10, 30, 0xFFFFFFFF);
							DspBMP(_RLOGO_ADDR, 590,420, VG_Back_FrameBuffer, 0,0);			
							break;				 
				case 2:
 							RETW_GUI_PoliconFillScreen(0,0,800,480,0x101010ff);	
		 					RETW_GUI_Clock_1();
		 					R_TXTR_DrawStringMain( "Clock1" , 6, 10, 30, 0xFFFFFFFF);			
							break; 
				case 3:
 							RETW_GUI_PoliconFillScreen(0,0,800,480,0x104060ff);	
		 					RETW_GUI_Clock_2();
		 					R_TXTR_DrawStringMain( "Clock2" , 6, 10, 420, 0xFFFFFFFF);				 
							break; 
				case 4:
 							RETW_GUI_PoliconFillScreen(0,0,800,480,0x101010ff);	
		 					RETW_GUI_Clock_0();
		 					break;
//									R_TXTR_DrawStringMain( "HUD" , 3, 10, 30, 0xFFFFFFFF);	
				case 5:
 							RETW_GUI_PoliconFillScreen(0,0,800,480,0x101010ff);	
		 					RETW_GUI_Clock_3();
		 					break;
//									R_TXTR_DrawStringMain( "HUD" , 3, 10, 30, 0xFFFFFFFF);	
						 
				case 6:
 							RETW_GUI_PoliconFillScreen(0,0,800,480,0x101010ff);	
		 					RETW_GUI_Meter_1(speedcount,220);
//									R_TXTR_DrawStringMain( "HUD" , 3, 10, 30, 0xFFFFFFFF);			
							break; 
				case 7:
 							RETW_GUI_PoliconFillScreen(0,0,800,480,0x004000ff);	
		 					RETW_GUI_Meter_2(speedcount,220);
//									R_TXTR_DrawStringMain( "Meter" , 5, 10, 420, 0xFFFFFFFF);				 
							break; 
				case 8:
 							RETW_GUI_PoliconFillScreen(0,0,800,480,0x400000ff);	
		 					RETW_GUI_Meter_3(speedcount,220);
//									R_TXTR_DrawStringMain( "Meter" , 5, 10, 420, 0xFFFFFFFF);				 
							break; 
												
				case 9:
							Audio_MainLoop(); 
//									R_TXTR_DrawStringMain( "Playing MP3" , 11, 10, 30, 0xFFFFFFFF);							
							break;
				default:
							break;
		} 

#else


#if 0
					if(CleanBufferFlag==1)
							{
     					RETW_GUI_PoliconFillScreen(0,0,800,480,0x200000ff);
     										CleanBufferFlag=2;
     					}
					else if(CleanBufferFlag==2)
							{
     					RETW_GUI_PoliconFillScreen(0,0,800,480,0x200000ff);
     										CleanBufferFlag=0;
     					}     					
#endif

					if(ModeEntryCnt<3)
     			RETW_GUI_PoliconFillScreen(0,0,800,480,0x200000ff);	
//    		GL_FillScreen(0x101010FF);
  				GUI_func_table[OpMode](speedcount,220);

#endif
 

#if 0						//Debug use	

  								G_CharScale = 4.0;		
									G_CharShearX = 0.0;
									G_CharShearY = 0.0;
									RETW_GUI_DrawScriptNumber(80, 80, OpMode, 24, 0x00FF00ff, 1);
#endif
 
// GRAPHICS_DrawVgImage(0);
// GRAPHICS_DrawVgImage(1);

//								DrawImage(framecount);
								
 //       		  	vgFinish();
						GUI_GraphicsTask_PostMenu();
 
 

 

// 					GL_DrawingArc();
 					
// 					GL_DrawingPoliline();
 					
//          TrVg_DrawingRenderQuality();
// 			   TrVg_DrawingFilters();
 			
// 					RETW_GUI_Meter_GasolineIndicator( 350, 140, EngineRPM +240 -1);		


//      	TrVg_DrawingEllipticalArcs();


//				TrVg_DrawingColorPaint();
//   		TrVg_DrawingLine();
    		
//        TrVg_DrawingCircleRadiant(400,240,10,10,0xFF0030ff);		 	
	
//       	TrVg_DrawingStraightLine();
 
//        TrVg_DrawingPolyconFill();

//    		TrVg_DrawingStroke();

		
//   	TrVg_DrawingText();

//   	TrVg_DrawingRadialGradien();


						
//		TrVg_DrawingMasking();
		
//		TrVg_DrawingScissoring();
		



//			FillScreenColorBlock(currentframe);
//				FillScreenColorBar(currentframe);

//			TrVg_DrawingTextImage();

#if 0
				TrVg_DrawingPicture(160,160);
				
//				TrVg_DrawingFiltersClock160160(630,10);
#endif
 
    		framecount++;
    		totalframecnt+=1;
 //   if(framecount%3 == 0)

#if 0			//Debug use
  								G_CharScale = 1.0;		
									G_CharShearX = 0.0;
									G_CharShearY = 0.0;

 									RETW_GUI_DrawScriptNumber(400, 30, totalframecnt, 24, 0xFFFF00ff, 1);
#endif
 
    if (framecount >= FRAME_COUNT_MAX)
    { 
        framecount = 0;
    }

    return 0;
}   /* End of function Graphics_DrawVgImage() */

void GUI_GraphicsTask_KeyResponse(uint32_t KeyInCode)          
{
				switch (KeyInCode)
				{
					case 0x00000020:
											
											if(	SetupMenuIndex.OnOff==0)
													SetupMenuIndex.OnOff=1;
											else if(	SetupMenuIndex.OnOff==1 && SetupMenuIndex.SetupItemPoint!=GUI_SetupItem_EXIT) 
													SetupMenuIndex.OnOff=2;
											else if(	SetupMenuIndex.OnOff==1 &&  SetupMenuIndex.SetupItemPoint==GUI_SetupItem_EXIT )
													{
													SetupMenuIndex.OnOff=0;
													ModeEntryCnt=0;									//Restart to renew all screen
												  }													
													
											else if(	SetupMenuIndex.OnOff==2 ) 
													SetupMenuIndex.OnOff=1;
											break;
 
				  case 0x001b5b41:
											break;
				  case 0x001b5b42:
											break;
				  case 0x001b5b43:
				  	
				  						if(	SetupMenuIndex.OnOff==0)
				  						{			
				  							
 											OperatingMode+=1;
 											if(OperatingMode==MenuDataPage[6].MaxValue)
 													OperatingMode=0;
 
											}	
				  						else if(SetupMenuIndex.OnOff==1)
				  						{
												SetupMenuIndex.SetupItemPoint +=1;
												if(SetupMenuIndex.SetupItemPoint == GUI_SetupItem_MAX)
												SetupMenuIndex.SetupItemPoint =GUI_SetupItem_MAX-1;
											}
											else if(SetupMenuIndex.OnOff==2)
				  						{
														GUI_AdjustDateTime( SetupMenuIndex.SetupItemPoint, 1);			// 1 means increase
														GUI_AdjustMode( SetupMenuIndex.SetupItemPoint, 1);			// 1 means increase
											}
											break;
				  case 0x001b5b44:
				  						if(	SetupMenuIndex.OnOff==0)
				  						{			
				  							RETW_GUI_OPENMOTION_0_SWMETER_EVENT();	  	
											}	
				  						else if(SetupMenuIndex.OnOff==1)
				  						{				  	
												if(SetupMenuIndex.SetupItemPoint == 0)				
						  					SetupMenuIndex.SetupItemPoint = 0;
												else  	
												SetupMenuIndex.SetupItemPoint -=1;
											}
											else if(SetupMenuIndex.OnOff==2)
											{
														GUI_AdjustDateTime( SetupMenuIndex.SetupItemPoint, 0);			// 0 means decrease
														GUI_AdjustMode( SetupMenuIndex.SetupItemPoint, 0);			// 1 means increase
											}
											
				  						break;
				  default:
				  						break;					
				}					
}

void GUI_GraphicsTask_PostMenu()
{
			uint32_t i,j,k;
			uint32_t loc_x,loc_y;
			uint32_t CenterX,CenterY;
			
			uint32_t LengthX,LengthY;
			uint32_t font_width;
			uint32_t Item_Width;			
			uint32_t LocX[8] = {87, 160, 210, 290,340,390, 590, 720};
			char TempString[64];
 
			st_VGDrawingParameter	VGParameter; 
 
		if(SetupMenuIndex.OnOff > 0 )
 		{
//				GL_DrawingRoundRectangle();			
				GL_DrawingRoundRectanglePara(20 , 20 , 760, 180, 20 , 20 ,	0xFFFFFFF0);
				
// 			printf(" point %d \n",SetupMenuIndex.SetupItemPoint );
 
#if 1
 						Item_Width = 760/GUI_SetupItem_MAX;
 						
									G_CharShearX = 0.0;
									G_CharShearY = 0.0;
									font_width = 15;
						
						for(j=0;	j< GUI_SetupItem_MAX ; j++)
							{
									if( SetupMenuIndex.SetupItemPoint == j)						//Seleced Item
									{
 									G_CharScale = 1.0;		
 									font_width = 15;
 									loc_x= 20 + Item_Width*j + (Item_Width/2) - font_width*(MenuDataPage[j].CharNum/2);
 									
 										if(framecount<15)
 										loc_y= 	36 + framecount;
 										else
 										loc_y= 	36 + 15-(framecount-15);
 									
 										if(framecount<15)
 										G_CharScale= 	G_CharScale + 0.01* framecount;
 										else
 										G_CharScale= 	G_CharScale + 0.01*(30-framecount );
 									
 									for(i=0;i<MenuDataPage[j].CharNum;i++)
 									TempString[i] = *(unsigned char*)( &MenuDataPage[j].CharString +i );
									R_TXTR_DrawStringMain( &TempString[0], MenuDataPage[j].CharNum, loc_x, loc_y, 0x000080ff); 
									}
									else																							//Non-selected Item
									{
 									G_CharScale = 1.0;
 									font_width = 15;
									loc_x= 20 + Item_Width*j + (Item_Width/2) - font_width*(MenuDataPage[j].CharNum/2);
 									loc_y= 	40;
 									for(i=0;i<MenuDataPage[j].CharNum;i++)
 									TempString[i] = *(unsigned char*)( &MenuDataPage[j].CharString +i );
									R_TXTR_DrawStringMain( &TempString[0], MenuDataPage[j].CharNum, loc_x, loc_y, 0x008000ff); 
									}
							}
#endif

#if 1 //Dispay Adjusted Value inside Horizonal Bar 

									//printf("SYSTEM Year =%x\n",SYSTEMTIME.Year);
									MenuDataPage[0].Value = SYSTEMTIME.Year;
									MenuDataPage[1].Value = SYSTEMTIME.Month;      							
									MenuDataPage[2].Value = SYSTEMTIME.Day;         
									MenuDataPage[3].Value = SYSTEMTIME.Hour;                       
									MenuDataPage[4].Value = SYSTEMTIME.Minute;    
									MenuDataPage[5].Value = SYSTEMTIME.Second; 
 
									//printf("Menu Year =%x\n",MenuDataPage[0].Value);
 
									G_CharShearX=0.0;
									G_CharShearY=0.0;
 									G_CharScale = 1.5;		
 									
									font_width = 24;

									#if 0 									
 									if(framecount<15)
 											{
 									G_CharScale= 	G_CharScale + 0.03* framecount;
 									font_width = font_width + framecount/2;
 											}
 									else{
 									G_CharScale= 	G_CharScale + 0.03*(30-framecount );
									font_width = font_width +(30-framecount);
											}
 									#endif
 									
 									loc_x	= 50;
 									loc_y = 120;
 					
 									k=0;
 									for(j=0;j<3;j++)
 									{
 													for(i=0;i<MenuDataPage[j].DispDigit;i++)
 													{									 
  													TempString[k] = (char) ((( MenuDataPage[j].Value >> ((MenuDataPage[j].DispDigit-1-i)*4) )& 0x000F) + 0x30) ;	
  													k+=1;																	
 													}
 													
 													if(j==0 || j==1 )
 														{
 															TempString[k]= '-';
 															k+=1;
 														}
 													else if(j==2 )
 														{
 														TempString[k]= ',';
 														k+=1;										
 													  }
 									}
 									
 									
 									R_TXTR_DrawStringMain( &TempString[0], k, loc_x, loc_y, 0x808000FF);  
									
									loc_x=270;
									
 									k=0;
 									for(j=3;j<6;j++)
 									{
 													for(i=0;i<MenuDataPage[j].DispDigit;i++)
 													{									 
  													TempString[k] = (char) ((( MenuDataPage[j].Value >> ((MenuDataPage[j].DispDigit-1-i)*4) )& 0x000F) + 0x30) ;	
  													k+=1;																	
 													}
 
 													if (j==3 || j==4 )
 														{
 															TempString[k]= ':';
 															k+=1;
 														}
 													else if(j==5 )
 														{
 														TempString[k]= ',';
 														k+=1;											
 													  }
 									}
 									
 									R_TXTR_DrawStringMain( &TempString[0], k, loc_x, loc_y, 0x800080FF); 
 
 									loc_x=440;
 
									if(SYSTEMTIME.Weekday==0)
									R_TXTR_DrawStringMain( "SUN" , 3, loc_x, loc_y, 0x0000A0FF);
									else if(SYSTEMTIME.Weekday==1)
									R_TXTR_DrawStringMain( "MON" , 3, loc_x, loc_y, 0x101010FF);
									else if(SYSTEMTIME.Weekday==2)
									R_TXTR_DrawStringMain( "TUE" , 3, loc_x, loc_y, 0x101010FF);
									else if(SYSTEMTIME.Weekday==3)
									R_TXTR_DrawStringMain( "WED" , 3, loc_x, loc_y, 0x101010FF);
									else if(SYSTEMTIME.Weekday==4)
									R_TXTR_DrawStringMain( "THU" , 3, loc_x, loc_y, 0x101010FF);
									else if(SYSTEMTIME.Weekday==5)
									R_TXTR_DrawStringMain( "FRI" , 3, loc_x, loc_y, 0x101010FF);
									else if(SYSTEMTIME.Weekday==6)
									R_TXTR_DrawStringMain( "SAT" , 3, loc_x, loc_y, 0x00A000FF);
									
 									loc_x=540;				//Mode
									R_TXTR_DrawStringMain( "MD" , 4, loc_x, loc_y, 0x00A000FF);
 									
 									k=0;
 									j=6;
 									for(i=0;i<MenuDataPage[j].DispDigit;i++)
 													{									 
  													TempString[k] = (char) ((( MenuDataPage[j].Value >> ((MenuDataPage[j].DispDigit-1-i)*4) )& 0x000F) + 0x30) ;	
  													k+=1;																	
 													}
 													
 									loc_x=600;				//Mode number 				
 									R_TXTR_DrawStringMain( &TempString[0], k, loc_x, loc_y, 0x800080FF); 
 									
 									loc_x=680;				//Exit
									R_TXTR_DrawStringMain( "Exit" , 4, loc_x, loc_y, 0x00A000FF);									
 														
								
#endif

#if 1		//UnderLine BAR for selected 

					if(SetupMenuIndex.OnOff == 1 )
					{
							CenterX= LocX[SetupMenuIndex.SetupItemPoint];
							CenterY = loc_y-12;
							
							if(SetupMenuIndex.SetupItemPoint==0)
								LengthX=40;
							else if(SetupMenuIndex.SetupItemPoint==6)
								LengthX=50;
							else if(SetupMenuIndex.SetupItemPoint==7)
								LengthX=40;
							else
								LengthX=20;
 
							LengthY = 3;
							
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
							VGParameter.PathDrawingData[j]  = CenterX-LengthX; 
							VGParameter.PathDrawingData[j+1] = CenterY-LengthY;
							VGParameter.PathPoints+=1;
							j+=2;			
 
							VGParameter.PathDrawingData[j]  = CenterX+LengthX; 
							VGParameter.PathDrawingData[j+1] = CenterY-LengthY;
							VGParameter.PathPoints+=1;
							j+=2;				
 
							VGParameter.PathDrawingData[j]  = CenterX+LengthX; 
							VGParameter.PathDrawingData[j+1] = CenterY+LengthY;
							VGParameter.PathPoints+=1;
							j+=2;		

							VGParameter.PathDrawingData[j]  = CenterX-LengthX; 
							VGParameter.PathDrawingData[j+1] = CenterY+LengthY;
							VGParameter.PathPoints+=1;
							j+=2;		
							
 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							VGParameter.FillColor = 0x1010FF00;
 	 						VGParameter.AlphaColor = 0x000000ff;
							GL_DrawingPolyconFill( &VGParameter);	
					}
#endif 	//Triangle up and down


	

 		}			//if(SetupMenuIndex.OnOff > 0)
 		
		if(SetupMenuIndex.OnOff == 2 )
 		{

#if 1		//Triangle up	and down inside BAR

							CenterX= LocX[SetupMenuIndex.SetupItemPoint];
							CenterY = loc_y+12;
							
 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
							VGParameter.PathDrawingData[j]  = CenterX; 
							VGParameter.PathDrawingData[j+1] = CenterY+50;
							VGParameter.PathPoints+=1;
							j+=2;			
 
							VGParameter.PathDrawingData[j]  = CenterX-15; 
							VGParameter.PathDrawingData[j+1] = CenterY+30;
							VGParameter.PathPoints+=1;
							j+=2;				
 
							VGParameter.PathDrawingData[j]  = CenterX+15; 
							VGParameter.PathDrawingData[j+1] = CenterY+30;
							VGParameter.PathPoints+=1;
							j+=2;		

 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							VGParameter.FillColor = 0x0000FF00;
 	 						VGParameter.AlphaColor = 0x000000f0;
							GL_DrawingPolyconFill( &VGParameter);	

 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
							VGParameter.PathDrawingData[j]  = CenterX; 
							VGParameter.PathDrawingData[j+1] = CenterY-50;
							VGParameter.PathPoints+=1;
							j+=2;			
 
							VGParameter.PathDrawingData[j]  = CenterX-15; 
							VGParameter.PathDrawingData[j+1] = CenterY-30;
							VGParameter.PathPoints+=1;
							j+=2;				
 
							VGParameter.PathDrawingData[j]  = CenterX+15; 
							VGParameter.PathDrawingData[j+1] = CenterY-30;
							VGParameter.PathPoints+=1;
							j+=2;		

 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							VGParameter.FillColor = 0x0000FF00;
 	 						VGParameter.AlphaColor = 0x000000f0;
							GL_DrawingPolyconFill( &VGParameter);	
#endif 	//Triangle up and down





#if 0	 //Circle to display adjused value
 					VGParameter.PaintMode = VG_PAINT_TYPE_RADIAL_GRADIENT;
 					
					VGParameter.PathDrawingData[0]= 400;
					VGParameter.PathDrawingData[1]= 240;
					VGParameter.PathDrawingData[2]= 200;							/*	width			*/
					VGParameter.PathDrawingData[3]= 200;							/*	height		*/
					VGfloat SrampStops[30] = 				{ 	0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
																 		 					0.85f, 1.0f, 1.0f, 1.0f, 1.0f, 		
																 		 					0.90f, 0.0f, 0.6f, 0.0f, 1.0f,
																		 					0.95f, 0.0f, 0.6f, 0.0f, 1.0f, 		
																		 					0.98f, 0.5f, 1.0f, 0.5f, 1.0f, 		
																		 					1.0f, 0.0f, 1.0f, 0.0f, 1.0f,};
					for(i=0;i<30;i++)
					VGParameter.GradientStep[i]=SrampStops[i]; 
					VGParameter.GradientPoints = 30;
					
					VGParameter.GradientStartEnd[0] = 400;
					VGParameter.GradientStartEnd[1] = 240;
					VGParameter.GradientStartEnd[2] = 400;
					VGParameter.GradientStartEnd[3] = 240;
					VGParameter.GradientStartEnd[4] = 200/2;					
 
				 GL_DrawingCircleFill( &VGParameter);	
#endif

#if 0		//Triangle up	and down inside circle

 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
							VGParameter.PathDrawingData[j]  = 400; 
							VGParameter.PathDrawingData[j+1] = 240+80;
							VGParameter.PathPoints+=1;
							j+=2;			
 
							VGParameter.PathDrawingData[j]  = 400-15; 
							VGParameter.PathDrawingData[j+1] = 240+60;
							VGParameter.PathPoints+=1;
							j+=2;				
 
							VGParameter.PathDrawingData[j]  = 400+15; 
							VGParameter.PathDrawingData[j+1] = 240+60;
							VGParameter.PathPoints+=1;
							j+=2;		

 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							VGParameter.FillColor = 0x0000FF00;
 	 						VGParameter.AlphaColor = 0x000000f0;
							GL_DrawingPolyconFill( &VGParameter);	

 							VGParameter.PathPoints=0;
 							VGParameter.GradientPoints=0;
							j=0;
							VGParameter.PathDrawingData[j]  = 400; 
							VGParameter.PathDrawingData[j+1] = 240-80;
							VGParameter.PathPoints+=1;
							j+=2;			
 
							VGParameter.PathDrawingData[j]  = 400-15; 
							VGParameter.PathDrawingData[j+1] = 240-60;
							VGParameter.PathPoints+=1;
							j+=2;				
 
							VGParameter.PathDrawingData[j]  = 400+15; 
							VGParameter.PathDrawingData[j+1] = 240-60;
							VGParameter.PathPoints+=1;
							j+=2;		

 							VGParameter.PaintMode = VG_PAINT_TYPE_COLOR;
 							VGParameter.FillColor = 0x0000FF00;
 	 						VGParameter.AlphaColor = 0x000000f0;
							GL_DrawingPolyconFill( &VGParameter);	
#endif 	//Triangle up and down


#if 0	//Dispay Adjusted Value inside circle
									//printf("SYSTEM Year =%x\n",SYSTEMTIME.Year);
									MenuDataPage[0].Value = SYSTEMTIME.Year;
									MenuDataPage[1].Value = SYSTEMTIME.Month;      							
									MenuDataPage[2].Value = SYSTEMTIME.Day;         
									MenuDataPage[3].Value = SYSTEMTIME.Hour;                       
									MenuDataPage[4].Value = SYSTEMTIME.Minute;    
									MenuDataPage[5].Value = SYSTEMTIME.Second; 
 
									//printf("Menu Year =%x\n",MenuDataPage[0].Value);
 
									G_CharShearX=0.1;
									G_CharShearY=0.0;
 									G_CharScale = 2.0;		
 									
									font_width = 24;

									#if 0 									
 									if(framecount<15)
 											{
 									G_CharScale= 	G_CharScale + 0.03* framecount;
 									font_width = font_width + framecount/2;
 											}
 									else{
 									G_CharScale= 	G_CharScale + 0.03*(30-framecount );
									font_width = font_width +(30-framecount);
											}
 									#endif
 									
 									loc_x	= 400 - font_width*(MenuDataPage[SetupMenuIndex.SetupItemPoint].DispDigit/2);
 									loc_y = 240-font_width/2;
 					
 
 									if(SetupMenuIndex.SetupItemPoint<6)
 									{
 											for(i=0;i<MenuDataPage[SetupMenuIndex.SetupItemPoint].DispDigit;i++)
 											{									 
  											TempString[i] = (char) ((( MenuDataPage[SetupMenuIndex.SetupItemPoint].Value >> ((MenuDataPage[SetupMenuIndex.SetupItemPoint].DispDigit-1-i)*4) )& 0x000F) + 0x30) ;																		
 											}
 											R_TXTR_DrawStringMain( &TempString[0], MenuDataPage[SetupMenuIndex.SetupItemPoint].DispDigit, loc_x, loc_y, 0x0000FFff);   
 									}
 
									G_CharShearX=0.1;
									G_CharShearY=0.0;
 									G_CharScale = 2.0;	
									if(SYSTEMTIME.Weekday==0)
									R_TXTR_DrawStringMain( "SUN" , 3, 10, 100, 0x0000FFff);
									else if(SYSTEMTIME.Weekday==1)
									R_TXTR_DrawStringMain( "MON" , 3, 10, 100, 0xFFFF00ff);
									else if(SYSTEMTIME.Weekday==2)
									R_TXTR_DrawStringMain( "TUE" , 3, 10, 100, 0xFFFF00ff);
									else if(SYSTEMTIME.Weekday==3)
									R_TXTR_DrawStringMain( "WED" , 3, 10, 100, 0xFFFF00ff);
									else if(SYSTEMTIME.Weekday==4)
									R_TXTR_DrawStringMain( "THU" , 3, 10, 100, 0xFFFF00ff);
									else if(SYSTEMTIME.Weekday==5)
									R_TXTR_DrawStringMain( "FRI" , 3, 10, 100, 0xFFFF00ff);
									else if(SYSTEMTIME.Weekday==6)
									R_TXTR_DrawStringMain( "SAT" , 3, 10, 100, 0x00FF00ff);
									
#endif		



 		}			//if(SetupMenuIndex.OnOff == 2)
							
									
 // 						R_GUI_OpenVGDrawCharacter(loc_x, loc_y, 0x41, 0x0000FFFF, 1);
 
//							printf(" char %x \n", *(unsigned char*)( &MenuDataPage[SetupMenuIndex.SetupItemPoint].CharString +i )  );
//							R_GUI_OpenVGDrawCharacter(loc_x, loc_y, *(unsigned char*)( &MenuDataPage[SetupMenuIndex.SetupItemPoint].CharString +i ), 0xFFFFFFFF, 1); 		
  						
//  						R_GUI_OpenVGDrawChar	(loc_x, loc_y+50, *(unsigned char*)( &MenuDataPage[SetupMenuIndex.SetupItemPoint].CharString +i ), 0x0000FFff, 1); 				
 
}









void GUI_GraphicsTask_Initial()
{
			SetupMenuIndex.OnOff = 0;						
			SetupMenuIndex.SetupItemPoint = 7;				//Exit
			SetupMenuIndex.BackGround =0;	
			SetupMenuIndex.Frame =0;
			SetupMenuIndex.InFrame=0;                               
			SetupMenuIndex.Surface=0;                               
			SetupMenuIndex.Grid=0;                               
			SetupMenuIndex.AddDigital=0;  	                               
			SetupMenuIndex.Indicator=0;                               
			SetupMenuIndex.Day=0;                                
			SetupMenuIndex.AnalogDigital=0;     
			
			MenuDataPage[6].Value = OperatingMode;		
			MenuDataPage[6].MaxValue = sizeof GUI_func_table/sizeof (uint32_t);	
														                           
}                              
 
uint32_t ExportMeterSpeed(uint32_t MiliCnt)
{	
 
				if(MiliCnt>  (4000-1)  )																		// go and back take 4 seconds
				{
								Milisec_VirtualEngineTickCount=0;
 								VirtualEngine.KMTotal = VirtualEngine.KMTotal+11;			
 								VirtualEngine.KMsub = VirtualEngine.KMsub+1;	
				}
				
				if(MiliCnt <2000)
							speedcount = MiliCnt*220/2000;												//maximum speed 220
				else
							speedcount = (4000 -MiliCnt)*220/2000;
 
 
 
 				VirtualEngine.GasolineLevel = 2880*MiliCnt/4000;
 
				if(	MiliCnt<1200)
				 VirtualEngine.TargetRPM = SINtable[MiliCnt*90/1200]*CIRCLE_DEG/10000;
				else
				 VirtualEngine.TargetRPM = COStable[(MiliCnt-1200)*90/(4000-1200) ]*CIRCLE_DEG/10000;			
				 
 				VirtualEngine.NaviDirection = 2880 * MiliCnt/4000;
				VirtualEngine.Temperature =  284;
 				
 
#if 0		 

				if(	MiliCnt<1200)
 			 			VirtualEngine.TargetRPM = CIRCLE_DEG * sin( 3.14159 *MiliCnt/1200/2);
				else
 			 			VirtualEngine.TargetRPM = CIRCLE_DEG * cos( 3.14159 *  (  (MiliCnt-1200)/(4000-1200)/2 )    );
 			 			
 			 			
 			 			
				if(	MiliCnt<2000)
				 VirtualEngine.TargetSpeed = CIRCLE_DEG * sin(PI *MiliCnt/2000/2);
				else
				 VirtualEngine.TargetSpeed = CIRCLE_DEG * cos(PI *(  ( MiliCnt-2000)/(4000-2000)/2 )    );
 
#endif	 
				 
#if 0
				if(	MiliCnt<1200)
				 EngineRPM = SINtable[MiliCnt*90/1200]*120/10000;
				else
				 EngineRPM = (COStable[(MiliCnt-1200)*90/2800]*120/10000 );				
#endif			 
 	
#if 0
				 EngineRPM = sin (MiliCnt*PI/4000 )*120;
#endif
  
}
void KeyScanning_Main() 
{
 				if(ScanTerminalKeyStatus()!=0)
 				{
				InputUARTKey = ScanGetTerminalKey();
//        printf("Get Key %x\n",InputUARTKey ); 
 				if(InputUARTKey==0x0d)
 						{
 								OperatingMode+=1;
 								if(OperatingMode== (MenuDataPage[6].MaxValue-1) )
													{

													printf("Audio Playing\n"); 	
													}		 												
 								if(OperatingMode==MenuDataPage[6].MaxValue)
													{

													printf("Audio Playing stop\n"); 
 													OperatingMode=0;}
					  }
 				else if(InputUARTKey==0x08)
 						{
 								RETW_GUI_OPENMOTION_0_SWMETER_EVENT();
					  }
				else if(InputUARTKey=='p')		
						{

								printf("Audio Playing\n"); 	}		
				else if(InputUARTKey=='s')	
						{

								printf("Audio Playing stop\n"); }			
								
				else if(InputUARTKey==0x20)	
						{
							GUI_GraphicsTask_KeyResponse(InputUARTKey);
//							printf("KeyIn space\n"); 
 						   }		
											
				else if(InputUARTKey==0x1b)
 						{
 							if(GetTerminalKey()==0x5b)
 									{
 										InputUARTKey = GetTerminalKey();
 
										GUI_GraphicsTask_KeyResponse(InputUARTKey+0x001b5b00); 										
#if 0 										
 										if(InputUARTKey==0x41)								//up
//											printf("Key Dec\n");
 										
 										else if(InputUARTKey==0x42)						//down
// 											printf("Key Inc\n"); 						

 										else if(InputUARTKey==0x43)						//right
//											printf("Key right\n");
 										
 										else if(InputUARTKey==0x44)						//left
//											printf("Key left\n"); 		
#endif			
									}	
					  } 
 				}	
}

void BLE_KeyScanning_Main() 
{
 				if(RETW_SCIF1_CheckByteIn()!=0)
 				{
				InputUARTKey = RETW_SCIF1_ScanGetByte();
//        printf("Get Key %x\n",InputUARTKey ); 
 				if( InputUARTKey==0x0a  )
 						{
 								OperatingMode+=1;
					  }
 				else if(InputUARTKey==0x08)													//back key
 						{
 								RETW_GUI_OPENMOTION_0_SWMETER_EVENT();
					  }
				else if(InputUARTKey=='i')		
										GUI_GraphicsTask_KeyResponse(0x001b5b41);	
				else if(InputUARTKey=='k')	
										GUI_GraphicsTask_KeyResponse(0x001b5b42);
				else if(InputUARTKey=='l')		
										GUI_GraphicsTask_KeyResponse(0x001b5b43);	
				else if(InputUARTKey=='j')	
										GUI_GraphicsTask_KeyResponse(0x001b5b44);
																		
				else if(InputUARTKey==0x20)	
						{
							GUI_GraphicsTask_KeyResponse(InputUARTKey);
 						}
 				}
}

#if 1
void GUI_AdjustDateTime(unsigned int AdjustItem,unsigned int IncDec )
{
				int i,j,k;
				uint16_t DateTime[6];
				uint32_t	WKDAY;
				const unsigned char DateNo[13] ={0,31,28,31,30,31,30,31,31,30,31,30,31};
 
	if( AdjustItem >5)
		return;
 
				MenuDataPage[0].Value = SYSTEMTIME.Year;
				MenuDataPage[1].Value = SYSTEMTIME.Month;      							
				MenuDataPage[2].Value = SYSTEMTIME.Day;         
				MenuDataPage[3].Value = SYSTEMTIME.Hour;                       
				MenuDataPage[4].Value = SYSTEMTIME.Minute;    
				MenuDataPage[5].Value = SYSTEMTIME.Second; 
				
 
 
				DateTime[0] = ((MenuDataPage[0].Value >> 12)&0x000f)*1000 + ((MenuDataPage[0].Value >> 8)&0x000f)*100	+ ((MenuDataPage[0].Value >> 4)&0x000f)*10 + ((MenuDataPage[0].Value)&0x000f);
				for(i=1;i<6;i++)
				DateTime[i] =  ((MenuDataPage[i].Value >> 4)&0x000f)*10 + ((MenuDataPage[i].Value)&0x000f);
			
		
				if(AdjustItem!=2)		//Non-Day number
				{
 						if(IncDec==1)
 						{
											DateTime[AdjustItem] +=1;
											if(DateTime[AdjustItem] > MenuDataPage[AdjustItem].MaxValue)
														DateTime[AdjustItem] = MenuDataPage[AdjustItem].MinValue; 
 						}
						else
						{
											if(DateTime[AdjustItem] == MenuDataPage[AdjustItem].MinValue)
														DateTime[AdjustItem] = MenuDataPage[AdjustItem].MaxValue;  
											else
														DateTime[AdjustItem] -=1;
						}		
				}
				else				//Item is 2, Day number monthly
				{
 						if(IncDec==1)
 						{

											DateTime[AdjustItem] +=1;
											
											if( (DateTime[0]%4)!=0 && DateTime[1] ==2 && DateTime[AdjustItem]==29)
													DateTime[AdjustItem]=1;
											else if( (DateTime[0]%4)==0 && DateTime[1] ==2 && DateTime[AdjustItem]==30)
													DateTime[AdjustItem]=1;
											else if( ( (DateTime[1] ==4 ||DateTime[1] ==6|| DateTime[1] ==9 || DateTime[1] ==11) )&& DateTime[AdjustItem]==31)
												DateTime[AdjustItem]=1;
											else if( ( (DateTime[1] ==1 ||DateTime[1] ==3|| DateTime[1] ==5|| DateTime[1] ==7|| DateTime[1] ==8|| DateTime[1] ==10|| DateTime[1] ==12) && DateTime[AdjustItem]==32))
 												DateTime[AdjustItem]=1;
 												
 						}
						else
						{
											if( (DateTime[0]%4)!=0 && DateTime[1] ==2 && DateTime[AdjustItem]==1)
													DateTime[AdjustItem]=28;
											else if( (DateTime[0]%4)==0 && DateTime[1] ==2 && DateTime[AdjustItem]==1)
													DateTime[AdjustItem]=29;
											else if( ( (DateTime[1] ==4 ||DateTime[1] ==6|| DateTime[1] ==9 || DateTime[1] ==11) )&& DateTime[AdjustItem]==1)
												DateTime[AdjustItem]=30;
											else if( ( (DateTime[1] ==1 ||DateTime[1] ==3|| DateTime[1] ==5|| DateTime[1] ==7|| DateTime[1] ==8|| DateTime[1] ==10|| DateTime[1] ==12) && DateTime[AdjustItem]==1))
 												DateTime[AdjustItem]=31;
											else
												DateTime[AdjustItem] -=1;
						}							
				}
				
	#if 1			//Sum up Weekday
				WKDAY=4;
				for(i=2016;i<DateTime[0];i++)				//Year
				{
					if( (i%4) == 0)
					WKDAY +=366;
					else
					WKDAY +=365;				
				}
				for(i=1;i<DateTime[1];i++)					//Month
				{
					if( (DateTime[0]%4)==0 && i==2)
					WKDAY+=29;
					else
					WKDAY +=DateNo[i];				
				}				
				WKDAY+= DateTime[2];					//Day in month
				WKDAY = WKDAY%7;
	#endif
	
				MenuDataPage[0].Value =  (DateTime[0]/1000)<<12 | ((DateTime[0]%1000)/100)<<8 | ((DateTime[0]%100)/10)<<4 | (DateTime[0]%10);
				for(i=1;i<6;i++)
				MenuDataPage[i].Value =  ((DateTime[i]%100)/10)<<4 | (DateTime[i]%10);
				
				
				SYSTEMTIME.Year    = MenuDataPage[0].Value;
				SYSTEMTIME.Month   = MenuDataPage[1].Value;			
				SYSTEMTIME.Day     = MenuDataPage[2].Value;	
				SYSTEMTIME.Hour    = MenuDataPage[3].Value;	           
				SYSTEMTIME.Minute  = MenuDataPage[4].Value;	
				SYSTEMTIME.Second  = MenuDataPage[5].Value;	
				
				SYSTEMTIME.Weekday = WKDAY;
				
				Set_DateTime();
 
}


void GUI_AdjustMode(unsigned int AdjustItem,unsigned int IncDec )
{
				int i,j,k;

	if( AdjustItem !=6 )
		return;
 

 						if(IncDec==1)
 						{
											OperatingMode+=1;
											if(OperatingMode > MenuDataPage[AdjustItem].MaxValue)
														OperatingMode = MenuDataPage[AdjustItem].MaxValue; 
														

 						}
						else
						{
											if( OperatingMode == MenuDataPage[AdjustItem].MinValue)
														OperatingMode = MenuDataPage[AdjustItem].MinValue;  
											else
														OperatingMode -=1;
						}		
						
						MenuDataPage[AdjustItem].Value = OperatingMode;		
		
}

#endif


ALPHA_DISPLAY()
{
 
#if 1
		//OpenVG draw
	 						RETW_GUI_PoliconFillScreen(0,0,800,480,0x00000010);
 							
 							RETW_GUI_PoliconFillScreen(0,0,400,480,0x20FFFF20);
	#if 1							
 							RETW_GUI_PoliconFillScreen(40,40,400,180,0x0000FF80);
 							RETW_GUI_PoliconFillScreen(500,100,250,300,0x00FF0080);
 							RETW_GUI_PoliconFillScreen(300,300,400,160,0xFF000080);
	#endif

#else
		//Software draw
 						RETW_GE_RetanglePolycon( VG_Back_FrameBuffer,	0	,0	, 800, 240,	0x20202020);
//					RETW_GE_CopyVideo(VG_Back_FrameBuffer); 
#endif

}
