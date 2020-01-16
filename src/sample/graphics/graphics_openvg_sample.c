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
* @file         graphics_openvg_sample.c
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

int32_t CurrentDisplayFrame=0; 
extern uint32_t OperatingMode; 
/******************************************************************************
Macro definitions
******************************************************************************/
#define 		Screen_SizeX			1280
#define			Screen_SizeY			480

#define			VG_SIZE_X			Screen_SizeX				// LCD X Size
#define			VG_SIZE_Y			Screen_SizeY  			// LCD Y Size

#define			DVI_SIZE_X				800							// DVI X Size
#define			DVI_SIZE_Y				1280						// DVI Y Size


#define     VG_FRAME_MEM_SIZE               (0x80000)

/* Framebuffer color format (NativeFormat) */
#define     NATIVE_COLOR                    (NATIVE_ARGB8888)		//(NATIVE_RGB565)

#define			RETW_GL_TEST									1

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern void GUI_GraphicsTask_Main(uint32_t OpMode);

extern uint32_t Graphics_ImageData32[];					/* 32bits= B,G,R,00*/
extern uint32_t Graphics_ImageData[];					/* 32bits= B,G,R,00*/
extern uint8_t __image_btn1[];
extern uint8_t __dash1[];
extern uint8_t __VS024[];

extern uint8_t _BMPLOGO[];
extern uint8_t _CHARTLOGO[];
extern uint8_t _3DBTNS[];
extern uint8_t _W_CLASSIC[];
extern uint8_t _W_HOTw[];
extern uint8_t _W_HOTb[];
extern uint8_t _BN0_ADDR[];
extern uint8_t _BN1_ADDR[];
extern uint8_t _BN2_ADDR[];
extern uint8_t _BN3_ADDR[];
extern uint8_t _BN4_ADDR[];
extern uint8_t _BN5_ADDR[];
extern uint8_t _BN6_ADDR[];
extern uint8_t _BN7_ADDR[];
extern uint8_t _BN8_ADDR[];
extern uint8_t _BN9_ADDR[];
 
extern uint8_t _CLOCKS_ADDR[];
extern uint8_t _RLOGO_ADDR[];
extern uint8_t _SPDWARN_ADDR[];
extern uint8_t _KMH_ADDR[];


/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
#if 0						//(RETW_GL_TEST by allocation
#if defined(__CC_ARM)
#pragma arm section zidata= "FRAME_MEM_BUF"
static uint32_t VG_G_FrameMemory[VG_FRAME_MEM_SIZE/sizeof(unsigned long)];
#pragma arm section

#elif defined(__GNUC__)
static uint32_t VG_G_FrameMemory[VG_FRAME_MEM_SIZE/sizeof(unsigned long)] __attribute((section("FRAME_MEM_BUF")));

#else
#error	/*unsupported*/
#endif

#else			//by Buf bank															//by General Lai
#if defined(__CC_ARM)
#pragma arm section zidata= "FRAME_MEM_BUF"
static uint32_t VG_G_FrameMemory[VG_FRAME_MEM_SIZE/sizeof(unsigned long)];
static uint32_t VG_G_FrameBuffer[2][800*480];
#pragma arm section

#elif defined(__GNUC__) 

#if 0

static uint32_t VG_G_FrameMemory[VG_FRAME_MEM_SIZE/sizeof(unsigned long)] __attribute((section("FRAME_MEM_BUF0")));
uint32_t VG_G_FrameBuffer[2][800*480] __attribute((section("FRAME_MEM_BUF1")));
uint32_t VG_Frame1  = VG_G_FrameBuffer[0];
uint32_t VG_Frame2  = VG_G_FrameBuffer[1]; 

#else 

static uint32_t VG_G_FrameMemory[VG_FRAME_MEM_SIZE/sizeof(unsigned long)] __attribute((section("FRAME_MEM_BUF0")));
uint32_t VG_G_FrameBuffer1[VG_SIZE_X*VG_SIZE_Y] __attribute((section("FRAME_MEM_BUF1")));
uint32_t VG_G_FrameBuffer2[VG_SIZE_X*VG_SIZE_Y] __attribute((section("FRAME_MEM_BUF2")));
uint32_t VG_Frame1  = VG_G_FrameBuffer1;
uint32_t VG_Frame2  = VG_G_FrameBuffer2; 
uint32_t VG_Back_FrameBuffer = VG_G_FrameBuffer2;
 
#endif
 
#else  
#error	/*unsupported*/ 
#endif
     
#endif
    

static EGLDisplay       Display;
static EGLSurface       WinSurface;
static EGLContext       Context;

static EGLint           iWidth;
static EGLint           iHeight;
/* Native resource */
static NativePixmap     front;
static NativePixmap     back;
static NativeWindow     window;
 
uint32_t FrameCountPs=0;
uint32_t FrameRate=0;

uint32_t VsyncThreadFlag =0;
uint32_t TimeTickThreadCount =0;
uint32_t OpenVG_OperationLoad =0;
uint32_t OpenVG_OperationWidth =0;

uint32_t OpenVG_BusyFlag =0;

extern uint8_t rtc_halfsecond_flag; 
extern uint32_t totalframecnt;

extern VGfloat 	G_CharScale;
extern VGfloat 	G_CharShearX;
extern VGfloat 	G_CharShearY;

/**************************************************************************//**
 * @brief       Initialize native resource
 * @param[in]   void
 * @retval      Error code. If an error occurs, a negative number will be returned.
******************************************************************************/
static int32_t Init_NativeResouce (void)
{
    NCGint32    ncg_ret;
    uint32_t    ulBaseAdder = (uint32_t)VG_G_FrameMemory;
    uint32_t    ulSize      = (uint32_t)VG_FRAME_MEM_SIZE;

    /* Create a host memory area (dummy call) */
    ncg_ret = NCGSYS_HostMemCreate(ulSize, 0);

    if (ncg_ret != NCG_no_err)
    {   /* ERROR */
        printf("[Error]:NCGSYS_HostMemCreate(%d)\n", ncg_ret);
        return -1;
    }

    /* Create a frame memory area */
    ncg_ret = NCGSYS_FrameMemCreate(ulBaseAdder, ulSize, 0);
    if (ncg_ret != NCG_no_err)
    {   /* ERROR */
        printf("[Error]:NCGSYS_FrameMemCreate(%d)\n", ncg_ret);
        return -1;
    }

    /* Initialization of OpenVG native resource. */
    initNativeResource();

    /* Create native window surface and show it. */
#if 0				//(RETW_GL_TEST==0)
    front   = createNativePixmap(NATIVE_COLOR, iWidth, iHeight);
    back    = createNativePixmap(NATIVE_COLOR, iWidth, iHeight);
#else
    front   = createNativePixmapEx(NATIVE_COLOR, iWidth, iHeight,
                                   NATIVE_PIXMAP_FB_RAM, VG_G_FrameBuffer1 );
    back    = createNativePixmapEx(NATIVE_COLOR, iWidth, iHeight,
                                   NATIVE_PIXMAP_FB_RAM, VG_G_FrameBuffer2 );
#endif
    
    window  = createNativeWindow(front, back);

//		printf("front,back,window,ulBaseAdder = %x,%x,%x,%x\n",front,back,window,ulBaseAdder);
 
    showNativeWindow(window);
    /* EGL initialization */
    eglStartUp();

    /* OpenVG initialization */
    vgStartUp();

    return 0;
}   /* End of function Init_NativeResouce() */

/**************************************************************************//**
 * @brief       Terminate native resource
 * @param[in]   void
 * @retval      Error code. If an error occurs, a negative number will be returned.
******************************************************************************/
static int32_t Term_NativeResouce (void)
{
    NCGint32    ncg_ret;

    vgCleanUp();
    eglCleanUp();

    if (window != NATIVE_NO_WINDOW)
    {
        destroyNativeWindow(window);
        window = NATIVE_NO_WINDOW;
    }
    if (back != NATIVE_NO_PIXMAP)
    {
        destroyNativePixmap(back);
        back = NATIVE_NO_PIXMAP;
    }
    if (front != NATIVE_NO_PIXMAP)
    {
        destroyNativePixmap(front);
        front = NATIVE_NO_PIXMAP;
    }

    termNativeResource();

    ncg_ret = NCGSYS_FrameMemDestroy();

    if (ncg_ret != NCG_no_err)
    {   /* ERROR */
        printf("[Error]:NCGSYS_FrameMemDestroy(%d)\n", ncg_ret);
        return -1;
    }

    /* dummy call */
    ncg_ret = NCGSYS_HostMemDestroy();

    if (ncg_ret != NCG_no_err)
    {   /* ERROR */
        printf("[Error]:NCGSYS_HostMemDestroy(%d)\n", ncg_ret);
        return -1;
    }

    return 0;
}   /* End of function Term_NativeResouce() */

/**************************************************************************//**
 * @brief       Initialize OpenVG and EGL
 * @param[in]   void
 * @retval      Error code. If an error occurs, a negative number will be returned.
******************************************************************************/
static int32_t Init_OpenvgAndEgl (void)
{
    EGLBoolean  b_egl_ret;
    EGLint      iCongigNum;
    EGLConfig   hConfig;
    static const EGLint piConfigAttribList[] =
    {
        EGL_RENDERABLE_TYPE,
        EGL_OPENVG_BIT,
        EGL_NATIVE_VISUAL_TYPE,
        (EGLint)NATIVE_COLOR,
        EGL_ALPHA_MASK_SIZE,
        0,
        EGL_NONE
    };

    /**/
    Display = eglGetDisplay((EGLNativeDisplayType)EGL_DEFAULT_DISPLAY);
    if (Display == EGL_NO_DISPLAY)
    {   /* ERROR */
        printf("[Error]:eglGetDisplay\n");
        return -1;
    }

    /**/
    b_egl_ret = eglInitialize(Display, NULL, NULL);
    if (b_egl_ret != EGL_TRUE)
    {   /* ERROR */
        printf("[Error]:eglInitialize\n");
        return -1;
    }

    iCongigNum = 0;
    hConfig    = 0;
    b_egl_ret = eglChooseConfig(Display, piConfigAttribList, &hConfig, 1, &iCongigNum);

    if (b_egl_ret != EGL_TRUE)
    {   /* ERROR */
        printf("[Error]:eglChooseConfig\n");
        return -1;
    }

    WinSurface = eglCreateWindowSurface(Display, hConfig, window, NULL);
    if (WinSurface == EGL_NO_SURFACE)
    {   /* ERROR */
        printf("[Error]:eglCreateWindowSurface\n");
        return -1;
    }

    b_egl_ret = eglBindAPI(EGL_OPENVG_API);
    if (b_egl_ret != EGL_TRUE)
    {   /* ERROR */
        printf("[Error]:eglBindAPI\n");
        return -1;
    }

    Context = eglCreateContext(Display, hConfig, EGL_NO_CONTEXT, NULL);
    if (Context == EGL_NO_CONTEXT)
    {   /* ERROR */
        printf("[Error]:eglCreateContext\n");
        return -1;
    }
		printf("Display,WinSurface,Context = %x,%x,%x\n",Display,WinSurface,Context);
    b_egl_ret = eglMakeCurrent(Display, WinSurface, WinSurface, Context);
    if (b_egl_ret != EGL_TRUE)
    {   /* ERROR */
        printf("[Error]:eglMakeCurrent\n");
        return -1;
    }
    return 0;
}   /* End of function Init_OpenvgAndEgl() */

/**************************************************************************//**
 * @brief       Terminate OpenVG and EGL
 * @param[in]   void
 * @retval      Error code. If an error occurs, a negative number will be returned.
******************************************************************************/
static int32_t Term_OpenvgAndEgl (void)
{
    EGLBoolean  b_egl_ret;

    b_egl_ret = eglMakeCurrent(Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (b_egl_ret != EGL_TRUE)
    {   /* ERROR */
        printf("[Error]:eglMakeCurrent\n");
        return -1;
    }

    if (Context != EGL_NO_CONTEXT)
    {
        b_egl_ret = eglDestroyContext(Display, Context);
        Context = EGL_NO_CONTEXT;
    }

    if (WinSurface != EGL_NO_SURFACE)
    {
        b_egl_ret = eglDestroySurface(Display, WinSurface);
        WinSurface = EGL_NO_SURFACE;
    }

    b_egl_ret = eglTerminate(Display);
    Display = EGL_NO_DISPLAY;

    eglReleaseThread();

    return 0;
}   /* End of function Term_OpenvgAndEgl() */

/**************************************************************************//**
 * @brief       Clear
 * @param[in]   void
 * @retval      void
******************************************************************************/
static void ClearBuffer (void)
{
    VGfloat Color[4];

    /* White */
    Color[0] = 0.1f;				//B	
    Color[1] = 0.1f;				//G
    Color[2] = 0.1f;				//R
    Color[3] = 0.0f;
 
    vgSetfv(VG_CLEAR_COLOR, 4, Color);

    vgClear(0, 0, (VGint)iWidth, (VGint)iHeight);
}   /* End of function ClearBuffer() */

  
 
#if 0					//RETW_GL_TEST
void FillScreen(unsigned long color)
{
		unsigned long i,j;
		for(j=0;j<480*800;j++)
	//			for(i=0;i<800;i++)
				{
				VG_G_FrameBuffer[CurrentDisplayFrame][j]= color;			
			}
#if 1			
			for(i=0;i<120*800;i++)
 				VG_G_FrameBuffer[CurrentDisplayFrame][i] = ((i/800)<<12)&0x0003F000;
			for(i=120*800;i<240*800;i++)
 				VG_G_FrameBuffer[CurrentDisplayFrame][i] = (((i/800)-120)<<6)&0x00000fC0;
			for(i=240*800;i<360*800;i++)
 				VG_G_FrameBuffer[CurrentDisplayFrame][i] = (((i/800)-240) )&0x0000003f;			
			for(i=360*800;i<480*800;i++)
 				VG_G_FrameBuffer[CurrentDisplayFrame][i] = 0x0000003f;	
#endif

			DspBMP(__image_btn1, 0,0,VG_G_FrameBuffer[CurrentDisplayFrame], 0);
}

void FillScreenColorBar( VGint currentframe)
{
		unsigned long i,j;
#if 1			
			for(i=0;i<120*800;i++)
 				VG_G_FrameBuffer[currentframe][i] = ((i/800)<<24)&0xFF000000;
			for(i=120*800;i<240*800;i++)
 				VG_G_FrameBuffer[currentframe][i] = (((i/800)-120)<<16)&0x00FF0000;
			for(i=240*800;i<360*800;i++)
 				VG_G_FrameBuffer[currentframe][i] = (((i/800)-240)<<8 )&0x0000FF00;			
			for(i=360*800;i<480*800;i++)
 				VG_G_FrameBuffer[currentframe][i] = (((i/800)-360) )&0x0000003f  + (((i/800)-360)<<6 )&0x00000fC0 + (((i/800)-360)<<12 )&0x0003F000;	
#endif
//			DspBMP(__image_btn1, 0,0,VG_G_FrameBuffer[0], 0);
}

void FillScreenColorBlock( VGint currentframe)
{
		unsigned long i,j;	

			for(i=240*800;i<260*800;i++)
 				VG_G_FrameBuffer[currentframe][i] = (((i/800)-120)<<6)&0x00000fC0;
}

#endif		

void GL_FillScreen(uint32_t color)
{
	
	  uint32_t *pframe;
 
		unsigned long i;

//		printf("VG_Back_FrameBuffer=%x\n",VG_Back_FrameBuffer);
		
    pframe = (uint32_t *)VG_Back_FrameBuffer; 
//		printf("pframe=%x\n",pframe);
		
		i=800*480;
		while(i>0){
					*pframe++ = 0;
					i--;
					}
}





void PastePicture( VGint currentframe, int x_start, int y_start)
{
		unsigned long x,y;	

			for(y=0;y<200;y++)
			for(x=0;x<200;x++)
				{
					
				*(uint32_t*)	(currentframe+(y_start+y)*800*4+(x_start+x)*4 ) = Graphics_ImageData32[y*200+x];
 
 				}
}
 
 	
/**************************************************************************//**
 * @brief       Clear
 * @param[in]   void
 * @retval      Error code. If an error occurs, a negative number will be returned.
******************************************************************************/
int32_t SwapBuffers (void)
{
    EGLBoolean b_egl_ret;

    b_egl_ret = eglSwapBuffers(Display, WinSurface);
    if (b_egl_ret == EGL_FALSE)
    {
        printf("[Error]:eglSwapBuffers\n");
        return -1;
    }
    
//    printf("egl return %d\n",b_egl_ret);
    return 0;
}   /* End of function SwapBuffers() */
 
static int32_t GetCurrentDisplay (void)
{
    EGLSurface  egl_ret;
		egl_ret = eglGetCurrentSurface(1);
    return egl_ret;
}

/**************************************************************************//**
 * @brief       
 * @param[in]   void
 * @retval      None
******************************************************************************/
void Graphics_OpenVGSample (void)
{
    int32_t     error,i,j;
		uint32_t *fp;
    uint32_t    ulBaseAdder = (uint32_t)VG_G_FrameMemory;
 
    /* -----  Initialisation value for application ----- */
    Display     = EGL_NO_DISPLAY;
    WinSurface  = EGL_NO_SURFACE;
    Context     = EGL_NO_CONTEXT;

    front       = NATIVE_NO_PIXMAP;
    back        = NATIVE_NO_PIXMAP;
    window      = NATIVE_NO_WINDOW;
 
    iWidth      = Screen_SizeX;
    iHeight     = Screen_SizeY;
   
    error = 0;

		printf("Init_NativeResouce---Start\n" );
																																			//frome here ARGB
    /* ----- Initialise native resource ----- */
    error = Init_NativeResouce();
 																																			//frome here ABGR
//		RETW_GE_RetanglePolycon( &VG_G_FrameBuffer1[0],0,0, 800, 480,0x80FF0000);
//		RETW_GE_RetanglePolycon( &VG_G_FrameBuffer1[1],0,0, 800, 480,0x80FF0000);
		printf("Init_NativeResouce---Done\n" );
		
    /* ----- Initialise OpenVG and EGL ----- */
    if (error >= 0)
    {   /* If an error has not occurred, ... */
        error = Init_OpenvgAndEgl();
    }

		printf("Init_OpenvgAndEgl---Done\n" );
 
		MigoGraphicsInitialize(0,VG_G_FrameBuffer2, iWidth, iHeight, iWidth*iHeight*4);
//  MigoGraphicsMain();
 
		GRAPHICS_DrawVgImage_PrepareImage(); 
 
 			while(0){
 				TmrDelay_Graphics(1000);
 				for(i=0;i<800*480;i++)
 				VG_G_FrameBuffer1[i]=0x00FF00ff;
 				printf("Fill buf 1 Green...........\n");
 				
 				TmrDelay_Graphics(1000);
 				for(i=0;i<800*480;i++)
 				VG_G_FrameBuffer1[i]=0xFFFFFFff;
 				printf("Fill buf 1 WHITE...........\n");

 				TmrDelay_Graphics(1000);
 				for(i=0;i<800*480;i++)
 				VG_G_FrameBuffer2[i]=0xFF00FFff;
 				printf("Fill buf 2 Purple\n");
 				
 				TmrDelay_Graphics(1000);
 				for(i=0;i<800*480;i++)
 				VG_G_FrameBuffer2[i]=0xFF0000ff; 		
 				printf("Fill buf 2 WHITE\n");		
 				}
 
				ClearBuffer();						//this ClearBuffer will clean background plane.
				SwapBuffers();
				ClearBuffer();						//this ClearBuffer will clean background plane.
				SwapBuffers();				
 
				R_GUI_OpenVGFontOpen();
				GUI_GraphicsTask_Initial();
				
				printf("Start BLE Initialize\n");
//  			RETW_BLE_Init();
				printf("Finished BLE Initialize\n");				
//				GL_DrawingRoundRectangle();
//				RETW_GUI_PoliconFillScreen(0,0,800,480,0x000000ff);

        while (1)
        {

#if 1		//Master    	
//            RETW_BLE_MainPolling();	
            
#else		//BLE client
 
#endif        		


        		VsyncThreadFlag=0;
 
         		while(VsyncThreadFlag==0){}
            	LedOn();				//RSKVGTEST
 
							KeyScanning_Main();
 
     					OpenVG_BusyFlag =1;
#if 0
     					if(OperatingMode!=6)
  					 	RETW_GUI_PoliconFillScreen(0,0,Screen_SizeX,Screen_SizeY,0x000000ff);
#endif
  					 	
#if 0
  					 	fp = VG_Back_FrameBuffer;
  					 	j= Screen_SizeX*Screen_SizeY;
  					 	
  					 	while(j>0){
  						*fp++ = 0xff010101;
  						j--;}
#endif
 
//        			vgFlush();
//      		  	vgFinish(); 
  	
//            DspBMP(__VS024, 0,0,VG_G_FrameBuffer[!CurrentDisplayFrame], 0);
//            DspBMP(__dash1, 0,0,VG_G_FrameBuffer[!CurrentDisplayFrame], 0);
//            DspBMP(_BMPLOGO, 400,240,VG_G_FrameBuffer[!CurrentDisplayFrame], 0);
//            DspBMP(_3DBTNS, 600,240,VG_G_FrameBuffer[!CurrentDisplayFrame], 0);

//							DspBMP(_W_HOTb, 400-240,0,VG_G_FrameBuffer[CurrentDisplayFrame], 0);
            /* Draw */
 
          		GUI_GraphicsTask_Main(OperatingMode);
 
      				OpenVG_BusyFlag=0;

//              vgFlush();      
							vgFinish();		
							
#if 1
            	MigoGraphicsInitialize(0,VG_Back_FrameBuffer, iWidth, iHeight, iWidth*iHeight*4);		
            	VGADspSetLocation(300,480-16);
//            	VGADspString("Total Count %d, FrameRate %d fps, OpenVG occupy %d Percent\n",totalframecnt, FrameRate , (OpenVG_OperationLoad*10000/OpenVG_OperationWidth/100)-1 );
							VGADspString(" Frame-Rate- %d -fps,  totalframes  %d\n",FrameRate,totalframecnt);

//            PastePicture( VG_Back_FrameBuffer, 0, 0);		
#endif
//   					GE_FillRectangle(20,20,1240,440,0x00000000);
//  						GE_FillRectangle(0,0,352,240,0x00000000);
  						GE_FillRectangle((Screen_SizeX/2)-(352/2),(Screen_SizeY/2)-(240/2),352,240,0x00000000);
  						
//						GE_FillEclipse(Screen_SizeX/2,Screen_SizeY/2,240,240,0x00000000);
// 						Project3Dto2DDemo();
 							
//							GE_FillRectangle(300,160,200,160,0x00000000);		
//							printf("Back Address is %x\n",VG_Back_FrameBuffer);
//    						GE_FillRectangle(100,240,600,200,0x00000000);	
				      
#if 0
 				while(TimeTickThreadCount<15)
 							{
 								if(VsyncThreadFlag==1)
 								break;
 								
 								Audio_MainLoop(); 				
 							}
#endif
//             	VGADspSetLocation(550,480-16);
//            	VGADspString("by Renesas and Regulus\n");
 
          error = SwapBuffers();
          if (error < 0)
          {   /* ERROR */
          			printf("SwapBuffer Error!!\n");
                break;
          }
            
					LedOff();				//RSKVGTEST
           
           
  				OpenVG_OperationLoad = TimeTickThreadCount;            
//						vgFlush();
//						vgFinish();
 						
            CurrentDisplayFrame = !CurrentDisplayFrame;
            if(CurrentDisplayFrame ==0)
            VG_Back_FrameBuffer = VG_G_FrameBuffer2 ;
            else
            VG_Back_FrameBuffer = VG_G_FrameBuffer1 ;
            
            FrameCountPs+=1;
 
		}		//while(1)
 
    /* ----- Terminate OpenVG and EGL ----- */
    if (error >= 0)
    {   /* If an error has not occurred, ... */
        error = Term_OpenvgAndEgl();
    }

    /* ----- Terminate native resource ----- */
    if (error >= 0)
    {   /* If an error has not occurred, ... */
        error = Term_NativeResouce();
    }
    while (1);  /* End */
}
 

void RETW_GUIDspDigiNumber(int x, int y, int Number)
{
#if 0
		if(Number==0)
				DspBMP(_BN0_ADDR, x, y, VG_Back_FrameBuffer, 0,0);
		else if(Number==1)
				DspBMP(_BN1_ADDR, x, y, VG_Back_FrameBuffer, 0,0);	
		else if(Number==2)
				DspBMP(_BN2_ADDR, x, y, VG_Back_FrameBuffer, 0,0);	
		else if(Number==3)
				DspBMP(_BN3_ADDR, x, y, VG_Back_FrameBuffer, 0,0);	
		else if(Number==4)
				DspBMP(_BN4_ADDR, x, y, VG_Back_FrameBuffer, 0,0);	
		else if(Number==5)
				DspBMP(_BN5_ADDR, x, y, VG_Back_FrameBuffer, 0,0);	
		else if(Number==6)
				DspBMP(_BN7_ADDR, x, y, VG_Back_FrameBuffer, 0,0);	
		else if(Number==7)
				DspBMP(_BN7_ADDR, x, y, VG_Back_FrameBuffer, 0,0);	
		else if(Number==8)
				DspBMP(_BN8_ADDR, x, y, VG_Back_FrameBuffer, 0,0);	
		else if(Number==9)
				DspBMP(_BN9_ADDR, x, y, VG_Back_FrameBuffer, 0,0);	
#endif

}
 
void RETW_GUI_Meter_DspFixPicture(int x, int y)
{
#if 0		//direct fill BMP, it has no antialince when redraw indicator
    		DspBMP(_CLOCKS_ADDR, 800-170,10, VG_Back_FrameBuffer, 0,0);
#else

				TrVg_DrawingPicture(160,160);
//			TrVg_DrawingFiltersClock160160(x,y);

//			TrVg_DrawingFilters();
 
#endif
 
#if 0
DspBMP(_KMH_ADDR, 500,280, VG_Back_FrameBuffer, 0,0);
#else
 				G_CharScale = 1.5;
				G_CharShearX=0.2;
				G_CharShearY=0.0;	
				R_TXTR_DrawStringMain("km/h", 4, 520, 200, 0xFFFFFFff);
#endif			

   			DspBMP(_RLOGO_ADDR, 590,420, VG_Back_FrameBuffer, 0,0);				
}
#if 0
void RETW_GUI_HUD_DspFixPicture(int x, int y)
{
  
   			DspBMP(_HUD101_ADDR, 10,100, VG_Back_FrameBuffer, 0,0);
   			DspBMP(_HUD102_ADDR, 300,0, VG_Back_FrameBuffer, 0,0);
   			DspBMP(_HUD103_ADDR, 580,300, VG_Back_FrameBuffer, 0,0);
   			DspBMP(_HUD104_ADDR, 300,400, VG_Back_FrameBuffer, 0,0);
   			DspBMP(_HUD105_ADDR, 10,300, VG_Back_FrameBuffer, 0,0);   			
				DspBMP(_KMH_ADDR, 700,200, VG_Back_FrameBuffer, 0,0);
}



void RETW_GUI_HUD_DspDirectionFlag(int x, int y, int PicNum)
{
	
				int newx = x-80;
 				int newy = y-20;
 				
 				if(PicNum == 1)
   			DspBMP(_HUD1_ADDR, newx,newy, VG_Back_FrameBuffer, 0,1);
 				else if(PicNum == 2)
   			DspBMP(_HUD2_ADDR, newx,newy, VG_Back_FrameBuffer, 0,1);
 				else if(PicNum == 3)
   			DspBMP(_HUD3_ADDR, newx,newy, VG_Back_FrameBuffer, 0,1);
 				else if(PicNum == 4)
   			DspBMP(_HUD4_ADDR, newx,newy, VG_Back_FrameBuffer, 0,1);
 				else if(PicNum == 5)
   			DspBMP(_HUD5_ADDR, newx,newy, VG_Back_FrameBuffer, 0,1);
 				else if(PicNum == 7)
   			DspBMP(_HUD7_ADDR, newx,newy, VG_Back_FrameBuffer, 0,1);
 				else if(PicNum == 8)
   			DspBMP(_HUD8_ADDR, newx,newy, VG_Back_FrameBuffer, 0,1);
 				else if(PicNum == 9)
   			DspBMP(_HUD9_ADDR, newx,newy, VG_Back_FrameBuffer, 0,1);
 				else if(PicNum == 10)
   			DspBMP(_HUDA_ADDR, newx,newy, VG_Back_FrameBuffer, 0,1);   			  			
 				else if(PicNum == 11)
   			DspBMP(_HUDB_ADDR, newx,newy, VG_Back_FrameBuffer, 0,1);      			   			
 
}

          
#endif    
void RETW_GUIDspWarning(int x, int y)
{
				DspBMP(_SPDWARN_ADDR, x, y, VG_Back_FrameBuffer, 0,0);
 
}

void ExportVsyncSharingTick()
{
  		VsyncThreadFlag=1;
  		OpenVG_OperationWidth = TimeTickThreadCount;
  		
  		TimeTickThreadCount=0;
}

void ExportTimeSharingTick()
{
			TimeTickThreadCount += 1;
}

/* End of function Graphics_GraphicsSample() */
