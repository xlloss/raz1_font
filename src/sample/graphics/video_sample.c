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
* http://www.renesas.com/disclaimer
* Copyright (C) 2012 - 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         video_sample.c
* @version      0.11
* $Rev: 2582 $
* $Date:: 2014-10-21 16:47:06 +0100#$
* @brief        VDC5 driver video sample
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    <string.h>

#include    "r_typedefs.h"
#include 		"sysdef.h"

#include    "r_vdc5.h"

#include    "iodefine.h"
#include    "dev_drv.h"
#include    "intc.h"

#include    "video_decoder.h"
#include    "graphics.h"
#include    "graphics_drv_wrapper.h"

#include    "graphics_debug.h"

 
static vdc5_error_t GRAPHICS_SetVideoReadWrite(
    const vdc5_channel_t                ch,
    const vdc5_layer_id_t               layer_id,
    void                        * const framebuff,
    const uint32_t                      fb_stride,
    const uint32_t                      buffer_cnt,
    const vdc5_res_md_t                 res_md,
    const vdc5_period_rect_t    * const res,
    const vdc5_period_rect_t    * const gr_grc);
static void DV0_SetPort(const vdc5_channel_t ch);




/******************************************************************************
Macro definitions
******************************************************************************/
/*! Graphics format: ARGB8888, 4 bytes per pixel */
#define     GRAPHICS_FORMAT             (VDC5_GR_FORMAT_ARGB8888)	 
/*! Frame buffer stride: Frame buffer stride should be set to a multiple of 32 or 128
    in accordance with the frame buffer burst transfer mode. */
 
#if 1
#define     IMGCAP_SIZE_NTSC_HS         (122u * 2u)
#define     IMGCAP_SIZE_NTSC_HW         (720u * 2u)
#define     IMGCAP_SIZE_NTSC_VS         (12u)
#define     IMGCAP_SIZE_NTSC_VW         (480u/2u)
#else																																			//RETWGeneral's test
#define     IMGCAP_SIZE_NTSC_HS         (105u * 2u)
#define     IMGCAP_SIZE_NTSC_HW         (720u * 2u)
#define     IMGCAP_SIZE_NTSC_VS         (16u)
#define     IMGCAP_SIZE_NTSC_VW         (480u/2u)
#endif

#define     IMGCAP_SIZE_PAL_HS          (132u * 2u)
#define     IMGCAP_SIZE_PAL_HW          (720u * 2u)
#define     IMGCAP_SIZE_PAL_VS          (19u)
#define     IMGCAP_SIZE_PAL_VW          (560u/2u)


#define     FRAME_BUFFER_STRIDE         (((1280 * 4u) + 31u) & ~31u)
#define     FRAME_BUFFER_HEIGHT         (480 / 2u)
#define     VIDEO_BUFFER_STRIDE         (352 * 2u)
#define     VIDEO_BUFFER_HEIGHT         (IMGCAP_SIZE_NTSC_VW/2)

/*! Video format: YCbCr422, 2 bytes per pixel */
#define     VIDEO_FORMAT                (VDC5_RES_MD_YCBCR422)
/*! Frame buffer stride: Frame buffer stride should be set to a multiple of 32 or 128
    in accordance with the frame buffer burst transfer mode. */
    

 

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
    void      * buffer_1;
    void      * buffer_2;
    uint32_t    backbuffer_num;
} buffering_t;


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static void InitParameters(const vdc5_channel_t ch);
static void SetInterrupt(const vdc5_channel_t ch);
static void IntCallbackFunc(vdc5_int_type_t int_type);
static void WaitVsync(const int32_t wait_count);
static void SetGraphicsData(
    const vdc5_gr_format_t  gr_format,
    void            * const fr_buff,
    const uint16_t          width,
    const uint16_t          height,
    const uint32_t          stride);
static void SetVideoDecoder(const vdc5_channel_t ch, vdc5_period_rect_t * const cap_rect);
static void CreateFrameBuffer(void);
static void * GetBackBuffer(void);
static void SwapFrameBuffer(void);
static void Termination(const vdc5_channel_t ch);

/*! Interrupt parameter */
static vdc5_int_t           interrupt;
/*! Read data parameter */
vdc5_read_chg_t      read_chg;
/*! Graphics display mode */
vdc5_gr_disp_sel_t   gr_disp_sel_tmp[VDC5_GR_TYPE_NUM];
/*! Display area */

static vdc5_period_rect_t   period_rect;
static vdc5_period_rect_t   sub_period_rect;
 

static volatile int32_t vsync_count;

static buffering_t buffering_inf;

	uint8_t FrameBuffer_Video[VIDEO_BUFFER_STRIDE * VIDEO_BUFFER_HEIGHT]  __attribute((section("CAPFRAME_MEM_BUF0")));
 
	#if 0 
	static uint8_t FrameBuffer_1[FRAME_BUFFER_STRIDE * FRAME_BUFFER_HEIGHT];
	static uint8_t FrameBuffer_2[FRAME_BUFFER_STRIDE * FRAME_BUFFER_HEIGHT];
	#else
	static uint8_t FrameBuffer_1[1];
	static uint8_t FrameBuffer_2[1];
	#endif
 
extern uint32_t VG_Frame1;
extern uint32_t VG_Frame2;
 
/**************************************************************************//**
 * @brief       Initialize variables
 * @param[in]   ch
 * @retval      None
******************************************************************************/
static void InitParameters (const vdc5_channel_t ch)
{
    uint32_t index;
 
    vsync_count     = 0;

    /* Interrupt parameter */
    interrupt.type      = VDC5_INT_TYPE_VLINE;  /* Interrupt type */
    interrupt.callback  = 0;                    /* Callback function pointer */
    interrupt.line_num  = (uint16_t)0 ;         /* Line number */

    read_chg.gr_base        = NULL;         /* Frame buffer base address */
    read_chg.width_read_fb  = NULL;         /* Width of the image read from frame buffer */
    read_chg.gr_grc         = NULL;         /* Display area */
    read_chg.gr_disp_sel    = NULL;         /* Graphics display mode */

    for (index = 0; index < (uint32_t)VDC5_GR_TYPE_NUM; index++)
    {
        gr_disp_sel_tmp[index] = VDC5_DISPSEL_IGNORED;
    }

    if (ch == VDC5_CHANNEL_0)
    {

    		#if 1 		//start from left-top,								must use this range to display full screen
    		
        period_rect.vs  = (uint16_t)LCD_CH0_DISP_VS;
        period_rect.vw  = (uint16_t)LCD_CH0_DISP_VW;
        period_rect.hs  = (uint16_t)LCD_CH0_DISP_HS;
        period_rect.hw  = (uint16_t)LCD_CH0_DISP_HW;
        
        sub_period_rect.vs  = (uint16_t)LCD_CH0_DISP_VS + ( (480-240)/2 );
        sub_period_rect.vw  = (uint16_t)240;
        sub_period_rect.hs  = (uint16_t)LCD_CH0_DISP_HS + ( (1280-352)/2);
        sub_period_rect.hw  = (uint16_t)352;

        #endif
    }
    else
    {
        period_rect.vs  = (uint16_t)LCD_CH1_DISP_VS;
        period_rect.vw  = (uint16_t)LCD_CH1_DISP_VW;
        period_rect.hs  = (uint16_t)LCD_CH1_DISP_HS;
        period_rect.hw  = (uint16_t)LCD_CH1_DISP_HW;
    }
}   /* End of function InitParameters() */

/**************************************************************************//**
 * @brief
 * @param[in]   ch
 * @retval      None
******************************************************************************/
static void SetInterrupt (const vdc5_channel_t ch)
{
    void         (* func)(uint32_t int_sense);
    uint16_t        int_id;
    int32_t         ret_code;

    if (ch == VDC5_CHANNEL_0)
    {
        int_id = INTC_ID_GR3_VLINE0;
    }
    else
    {
        int_id = INTC_ID_GR3_VLINE1;
    }

    func = R_VDC5_GetISR(ch, VDC5_INT_TYPE_VLINE);
    ret_code = R_INTC_RegistIntFunc(int_id, func);
    if (ret_code != DEVDRV_SUCCESS)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, ret_code);
    }

    if (ret_code == DEVDRV_SUCCESS)
    {
        ret_code = R_INTC_SetPriority(int_id, (uint8_t)VDC5_INT_PRIORITY);
        if (ret_code != DEVDRV_SUCCESS)
        {
            GRAPHICS_DebugPrint(__FILE__, __LINE__, ret_code);
        }
    }

    if (ret_code == DEVDRV_SUCCESS)
    {
        ret_code = R_INTC_Enable(int_id);
        if (ret_code != DEVDRV_SUCCESS)
        {
            GRAPHICS_DebugPrint(__FILE__, __LINE__, ret_code);
        }
    }
}   /* End of function SetInterrupt() */

/**************************************************************************//**
 * @brief
 * @param[in]   int_type
 * @retval      None
******************************************************************************/
static void IntCallbackFunc (vdc5_int_type_t int_type)
{
    if (vsync_count > 0)
    {
        vsync_count--;
    }
}   /* End of function IntCallbackFunc() */

/**************************************************************************//**
 * @brief
 * @param[in]   wait_count
 * @retval      None
******************************************************************************/
static void WaitVsync (const int32_t wait_count)
{
    vsync_count = wait_count;
    while (vsync_count > 0)
    {
        /* Do nothing */
    }
}   /* End of function WaitVsync() */

/**************************************************************************//**
 * @brief
 * @param[in]   gr_format
 * @param[out]  fr_buff
 * @param[in]   width
 * @param[in]   height
 * @param[in]   stride
 * @retval      None
******************************************************************************/
static void SetGraphicsData (
    const vdc5_gr_format_t  gr_format,
    void            * const fr_buff,
    const uint16_t          width,
    const uint16_t          height,
    const uint32_t          stride)
{
    /* Create fill color image */
    GRAPHICS_CreateFillColorImage(fr_buff, NULL, width, height, (uint16_t)stride, gr_format, (int32_t)-1);
}   /* End of function SetGraphicsData() */

/**************************************************************************//**
 * @brief
 * @param[in]   ch
 * @param[out]  cap_rect        : Image area to be captured
 * @retval      None
******************************************************************************/
static void SetVideoDecoder (const vdc5_channel_t ch, vdc5_period_rect_t * const cap_rect)
{
    vdec_channel_t          channel;
    graphics_col_sys_t      col_sys;
    const vdec_adc_vinsel_t adc_vinsel = VDEC_ADC_VINSEL_VIN1;

    if (ch == VDC5_CHANNEL_0)
    {
        channel = VDEC_CHANNEL_0;
    }
    else
    {
        channel = VDEC_CHANNEL_1;
    }

    col_sys = GRAPHICS_VideoDecoderInit(adc_vinsel, channel);
    if (col_sys == GRPH_COL_SYS_UNKNOWN)
    {
        /* If not detected, ... */
        col_sys = GRPH_COL_SYS_NTSC_358;
    }
    if ((col_sys == GRPH_COL_SYS_NTSC_358) || (col_sys == GRPH_COL_SYS_NTSC_443))
    {
        /* Size of NTSC video image to be captured */
        
        #if 1					//capture NTSC full size into video buffer
        
        cap_rect->vs = (uint16_t)IMGCAP_SIZE_NTSC_VS;
        cap_rect->vw = (uint16_t)IMGCAP_SIZE_NTSC_VW;
        cap_rect->hs = (uint16_t)IMGCAP_SIZE_NTSC_HS;
        cap_rect->hw = (uint16_t)IMGCAP_SIZE_NTSC_HW;
 				
 				#else
 				
        cap_rect->vs = (uint16_t)16;
        cap_rect->vw = (uint16_t)240;
        cap_rect->hs = (uint16_t)105*2;
        cap_rect->hw = (uint16_t)720*2;
 
 				#endif
    }
    else
    {
        /* Size of PAL video image to be captured */
        cap_rect->vs = (uint16_t)IMGCAP_SIZE_PAL_VS;
        cap_rect->vw = (uint16_t)IMGCAP_SIZE_PAL_VW;
        cap_rect->hs = (uint16_t)IMGCAP_SIZE_PAL_HS;
        cap_rect->hw = (uint16_t)IMGCAP_SIZE_PAL_HW;
    }
}   /* End of function SetVideoDecoder() */

/**************************************************************************//**
 * @brief       Create a frame buffer
 * @param[in]   stride
 * @param[in]   height
 * @retval      None
******************************************************************************/
static void CreateFrameBuffer (void)
{
//    buffering_inf.buffer_1          = (void *)FrameBuffer_1;
//    buffering_inf.buffer_2          = (void *)FrameBuffer_2;
    buffering_inf.buffer_1          =  VG_Frame1;
    buffering_inf.buffer_2          = VG_Frame2;
    
    buffering_inf.backbuffer_num    = 0u;
}   /* End of function CreateFrameBuffer() */

/**************************************************************************//**
 * @brief       Get back buffer address
 * @param[in]   stride
 * @param[in]   height
 * @retval      Back buffer address
******************************************************************************/
static void * GetBackBuffer (void)
{
    return ((buffering_inf.backbuffer_num == 0u) ? buffering_inf.buffer_1 : buffering_inf.buffer_2);
}   /* End of function GetBackBuffer() */

/**************************************************************************//**
 * @brief       Swap frame buffers
 * @param[in]   stride
 * @param[in]   height
 * @retval      None
******************************************************************************/
static void SwapFrameBuffer (void)
{
    if (buffering_inf.backbuffer_num == 0u)
    {
        buffering_inf.backbuffer_num = (uint32_t)1u;
    }
    else
    {
        buffering_inf.backbuffer_num = (uint32_t)0u;
    }
}   /* End of function SwapFrameBuffer() */

/**************************************************************************//**
 * @brief       Termination
 * @param[in]   ch
 * @retval      None
******************************************************************************/
static void Termination (const vdc5_channel_t ch)
{
    vdc5_error_t error;

    /* Interrupt parameter */
    interrupt.callback = 0;         /* Clear callback function pointer */
    /* Set interrupt service routine */
    error = R_VDC5_CallbackISR(ch, &interrupt);
    if (error != VDC5_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }

    if (error == VDC5_OK)
    {
        /* Stop surface */
        error = GRPDRV_StopSurfaces(ch);
    }
    if (error == VDC5_OK)
    {
        /* Destroy surface */
        error = GRPDRV_DestroySurfaces(ch);
    }
    if (error == VDC5_OK)
    {
        /* Termination */
        error = GRPDRV_Term(ch);
    }
}   /* End of function Termination() */

/**************************************************************************//**
 * @brief
 * @param[in]   void
 * @retval      None
******************************************************************************/
void GRAPHICS_VideoSample (void)
{
    vdc5_error_t                error;
    void                      * framebuff;
    vdc5_period_rect_t          res_tmp;

    /* VDC5 channel (VDC5_CHANNEL_0 or VDC5_CHANNEL_1) */
    const vdc5_channel_t        ch              = VDC5_CHANNEL_0;
    const vdc5_layer_id_t       layer_vd_id     = VDC5_LAYER_ID_0_WR;
    const vdc5_layer_id_t       layer_gr_id     = VDC5_LAYER_ID_1_RD;
    const vdc5_layer_id_t       layer_gr_id1     = VDC5_LAYER_ID_2_RD;
    
    const vdc5_graphics_type_t  gr_type_vd      = VDC5_GR_TYPE_GR0;
    const vdc5_graphics_type_t  gr_type_gr      = VDC5_GR_TYPE_GR1;
    const vdc5_graphics_type_t  gr_type_gr1      = VDC5_GR_TYPE_GR2;
    
    const vdc5_res_md_t         res_md_tmp      = VIDEO_FORMAT;
    const vdc5_gr_format_t      gr_format_tmp   = GRAPHICS_FORMAT;
    /* Video input channel
        If layer_vd_id is VDC5_LAYER_ID_0_WR, ch_input
    */
    const vdc5_channel_t        ch_input        = VDC5_CHANNEL_0;

    InitParameters(ch);

    CreateFrameBuffer();
		
    SetInterrupt(ch);

    SetVideoDecoder(ch_input, &res_tmp);																		//Set Video Decoded Size

    /* Initialization
        Video input channel 0: Not used
        Video input channel 1: Used
     */
 
    error = GRPDRV_Init(ch, VDC5_ON, VDC5_OFF);
 
 		error =VDC5_OK;

 		#if 1
    if (error == VDC5_OK) 
    {
        /* Create video surface */
        error = GRPDRV_VideoCreateSurface(
                    ch,																					/* Channel						*/
                    layer_vd_id,																/* Write layer ID			*/
                  	FrameBuffer_Video,
					 // FrameBuffer_Video + VIDEO_BUFFER_STRIDE* ((240)/2)+  ((1280 - 352*2)/2)*2,													/* Capture buffer	start		*/
                    (uint32_t)VIDEO_BUFFER_STRIDE,     					/* 2 bytes per pixel 	*/
                    res_md_tmp,																	/* Frame buffer video-signal format*/
                    &res_tmp,																		/*Image area to be captured from native NTSC,  					720*2,  	480/2				*/
                  	&sub_period_rect);													/* Scaled size and display position */  
      
    }
 		#endif

    if (error == VDC5_OK)
    {
//        period_rect.vw = (uint16_t)FRAME_BUFFER_HEIGHT;

        framebuff = GetBackBuffer();
        
//        SetGraphicsData(gr_format_tmp, framebuff, period_rect.hw, period_rect.vw, FRAME_BUFFER_STRIDE);
//				printf("buffer address is %x\n", framebuff);
//        GE_FillScreenHorizonalColorBar(framebuff,800,480);
        
        /* Create graphics surface */
    #if 0
        error = GRPDRV_GraphicsCreateSurface(
                    ch,
                    layer_gr_id,
                    framebuff,
                    FRAME_BUFFER_STRIDE,
                    gr_format_tmp,
                    &period_rect);
		#endif
		
		#if 0
				SwapFrameBuffer();
        framebuff = GetBackBuffer();
//        SetGraphicsData(gr_format_tmp, framebuff, period_rect.hw, period_rect.vw, FRAME_BUFFER_STRIDE);
//				printf("buffer address is %x\n", framebuff);
//        GE_FillScreenVerticalColorBar(framebuff,800,480);
        
        /* Create graphics surface */
        error = GRPDRV_GraphicsCreateSurface(
                    ch,
                    layer_gr_id1,
                    framebuff,
                    FRAME_BUFFER_STRIDE,
                    gr_format_tmp,
                    &period_rect);
    #endif
   
    }
#if 1
    if (error == VDC5_OK)
    {
        /* Interrupt parameter */
        interrupt.callback = &IntCallbackFunc;      /* Callback function pointer */
        /* Set interrupt service routine */
        error = R_VDC5_CallbackISR(ch, &interrupt);
        if (error != VDC5_OK)
        {
            GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
        }
    }
#endif

#if 1
    if (error == VDC5_OK)
    {
        gr_disp_sel_tmp[gr_type_vd] = VDC5_DISPSEL_LOWER;
        gr_disp_sel_tmp[gr_type_gr] = VDC5_DISPSEL_BLEND;
//				gr_disp_sel_tmp[gr_type_gr1] = VDC5_DISPSEL_BLEND;
        
        /* Start surface */
        error = GRPDRV_StartSurfaces(ch, gr_disp_sel_tmp);
    }
#endif
    
    if (error == VDC5_OK)
    {
				
//			GRAPHICS_VideoSample_StartSurface();
    		
        while (0)
        {
            /* Wait Vsync */
 
            SwapFrameBuffer();

            framebuff = GetBackBuffer();
//SetGraphicsData(gr_format_tmp, framebuff, period_rect.hw, period_rect.vw, FRAME_BUFFER_STRIDE);
//SetGraphicsData(gr_format_tmp, framebuff, 800, 480, FRAME_BUFFER_STRIDE);

            /* Read data parameter */
            read_chg.gr_base = framebuff;       /* Frame buffer base address */
            /* Change read process */
            error = R_VDC5_ChangeReadProcess(ch, layer_gr_id, &read_chg);
            if (error != VDC5_OK)
            {
                GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
                break;
            }
        }
    }
    /* Quit */
//    Termination(ch);
}   /* End of function GRAPHICS_VideoSample() */

void GRAPHICS_VideoSample_ReStartSurface()
{
    vdc5_error_t                error;
    void                      * framebuff;
    vdc5_period_rect_t          res_tmp;
    /* VDC5 channel (VDC5_CHANNEL_0 or VDC5_CHANNEL_1) */
    const vdc5_channel_t        ch              = VDC5_CHANNEL_0;
    const vdc5_layer_id_t       layer_vd_id     = VDC5_LAYER_ID_0_WR;
    const vdc5_layer_id_t       layer_gr_id     = VDC5_LAYER_ID_1_RD;
    const vdc5_layer_id_t       layer_gr_id1     = VDC5_LAYER_ID_2_RD;
    
    const vdc5_graphics_type_t  gr_type_vd      = VDC5_GR_TYPE_GR0;
    const vdc5_graphics_type_t  gr_type_gr      = VDC5_GR_TYPE_GR1;
    const vdc5_graphics_type_t  gr_type_gr1      = VDC5_GR_TYPE_GR2;
    
    const vdc5_res_md_t         res_md_tmp      = VIDEO_FORMAT;
    const vdc5_gr_format_t      gr_format_tmp   = GRAPHICS_FORMAT;
    /* Video input channel
        If layer_vd_id is VDC5_LAYER_ID_0_WR, ch_input
    */
    const vdc5_channel_t        ch_input        = VDC5_CHANNEL_0;

		GRPDRV_DestroySurfaces(ch_input);

    InitParameters(ch);

    CreateFrameBuffer();

    SetInterrupt(ch);

    SetVideoDecoder(ch_input, &res_tmp);
 
        /* Create video surface */
        error = GRPDRV_VideoCreateSurface(
                    ch,																					/* Channel						*/
                    layer_vd_id,																/* Write layer ID			*/
            				FrameBuffer_Video,													/* Capture buffer			*/
				//	 FrameBuffer_Video + VIDEO_BUFFER_STRIDE* ((240)/2)+  ((1280 - 352*2)/2)*2,													/* Capture buffer	start		*/
					 
                    (uint32_t)VIDEO_BUFFER_STRIDE,     					/* 2 bytes per pixel 	*/
                    res_md_tmp,																	/* Frame buffer video-signal format*/
                    &res_tmp,																		/*Image area to be captured from native NTSC,  					720*2,  	480/2				*/
                  	&sub_period_rect);													/* Scaled size and display position */ 
     
        framebuff = GetBackBuffer();
 
//				printf("buffer address is %x\n", framebuff);
//        GE_FillScreenHorizonalColorBar(framebuff,800,480);
        
        /* Create graphics surface */
        error = GRPDRV_GraphicsCreateSurface(
                    ch,
                    layer_gr_id,
                    framebuff,
                    FRAME_BUFFER_STRIDE,
                    gr_format_tmp,
                    &period_rect);
                    
        gr_disp_sel_tmp[gr_type_vd] = VDC5_DISPSEL_LOWER;
        gr_disp_sel_tmp[gr_type_gr] = VDC5_DISPSEL_BLEND;
        /* Start surface */
        error = GRPDRV_StartSurfaces(ch, gr_disp_sel_tmp);
 
        while (0)
        {
            /* Wait Vsync */
						TmrDelay_Graphics(1000);

            SwapFrameBuffer();

            framebuff = GetBackBuffer();

            /* Read data parameter */
            read_chg.gr_base = framebuff;       /* Frame buffer base address */
            /* Change read process */
            error = R_VDC5_ChangeReadProcess(ch, layer_gr_id, &read_chg);
            if (error != VDC5_OK)
            {
                GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
                break;
            }
        }
 
}   /* End of function GRAPHICS_VideoSample() */


void GRAPHICS_VideoSample_Chgaddr( uint32_t baseaddr)
{
    vdc5_error_t                error;
    void                      * framebuff;
    vdc5_period_rect_t          res_tmp;
    /* VDC5 channel (VDC5_CHANNEL_0 or VDC5_CHANNEL_1) */
    const vdc5_channel_t        ch              = VDC5_CHANNEL_0;
    const vdc5_layer_id_t       layer_vd_id     = VDC5_LAYER_ID_0_WR;
    const vdc5_layer_id_t       layer_gr_id     = VDC5_LAYER_ID_1_RD;
    const vdc5_layer_id_t       layer_gr_id1     = VDC5_LAYER_ID_2_RD;
    
    const vdc5_graphics_type_t  gr_type_vd      = VDC5_GR_TYPE_GR0;
    const vdc5_graphics_type_t  gr_type_gr      = VDC5_GR_TYPE_GR1;
    const vdc5_graphics_type_t  gr_type_gr1      = VDC5_GR_TYPE_GR2;
    const vdc5_res_md_t         res_md_tmp      = VIDEO_FORMAT;
    const vdc5_gr_format_t      gr_format_tmp   = GRAPHICS_FORMAT;
 
 
            read_chg.gr_base = baseaddr;       /* Frame buffer base address */
 
            R_VDC5_ChangeReadProcess( ch, layer_gr_id, &read_chg );
            
}   /* End of function GRAPHICS_VideoSample() */


void GE_FillScreenVerticalColorBar(uint32_t framebufferaddr, uint32_t X, uint32_t Y)
{
	int i,j;
	uint32_t *paddr=framebufferaddr;
	
				for(i=0;i<X*Y;i++)
 				{
 					switch ( (i%X) / (X/16) ) 
 					{
 						case 0: *paddr++ =	0xff000000; break;
 						case 1: *paddr++ =	0xff0000FF; break;
 						case 2: *paddr++ =	0xff00FF00; break;
 						case 3: *paddr++ =	0xffFF0000; break;
 						case 4: *paddr++ =	0xffFF00FF; break;
 						case 5: *paddr++ =	0xffFFFF00; break;
 						case 6: *paddr++ =	0x2000FFFF; break;
 						case 7: *paddr++ =	0x00000000; break;
 							  							
 						case 8:  *paddr++ =	0x20000000; break;
 						case 9:  *paddr++ =	0x20000088; break;
 						case 10: *paddr++ =0xff008800; break;
 						case 11: *paddr++ =0xff880000; break;
 						case 12: *paddr++ =0xff880088; break;
 						case 13: *paddr++ =0xff888800; break;
 						case 14: *paddr++ =0xff008888; break;
 						case 15: *paddr++ =0xff888888; break;  	
 					}
 				}
}

void GE_FillScreenHorizonalColorBar(uint32_t framebufferaddr, uint32_t X, uint32_t Y)
{
	int i;
	uint32_t *paddr=framebufferaddr;
	
				for(i=0;i<X*Y;i++)
 				{
 					switch (i/ (X*(Y/16)) ) 
 					{
 						case 0: *paddr++ =	0xff000000; break;
 						case 1: *paddr++ =	0xff0000FF; break;
 						case 2: *paddr++ =	0xff00FF00; break;
 						case 3: *paddr++ =	0xffFF0000; break;
 						case 4: *paddr++ =	0xffFF00FF; break;
 						case 5: *paddr++ =	0xffFFFF00; break;
 						case 6: *paddr++ =	0x2000FFFF; break;
 						case 7: *paddr++ =	0x00000000; break;
 							  							
 						case 8:  *paddr++ =	0x20000000; break;
 						case 9:  *paddr++ =	0x20000088; break;
 						case 10: *paddr++ =0xff008800; break;
 						case 11: *paddr++ =0xff880000; break;
 						case 12: *paddr++ =0xff880088; break;
 						case 13: *paddr++ =0xff888800; break;
 						case 14: *paddr++ =0xff008888; break;
 						case 15: *paddr++ =0xff888888; break;  	
 					}
 				}
}


static void DV0_SetPort(const vdc5_channel_t ch)
{
    volatile uint32_t   dummy_read;
    uint32_t            reg_data;

    if( ch == VDC5_CHANNEL_0 )
    {
        /*  COMS (8bit), DV 0ch
                 DV0_CLK             ... P10_0        	 1th alternative function
                 DV0_VSYNC           ... P10_1         	 1th alternative function
                 DV0_HSYNC           ... P10_2         	 1th alternative function
                 DV0_DATA[7:0]       ... P10_11 ~ P10_4  1th alternative function
                 DV0_DATA[11:8]      ... P10_15 ~ P10_12 1th alternative function
                 DV0_DATA[15:12]      ...P11_3 ~  P11_0  1th alternative function
         */
        /* Port 10 */
        reg_data        = (uint32_t)GPIO.PMC10 & (uint32_t)~0x0001;
        GPIO.PMC10       = (uint16_t)reg_data;
        reg_data        = (uint32_t)GPIO.PMC10;
        dummy_read      = reg_data;
        /* PFCAE10, PFCE10, PFC10 ... 1th alternative function
           PIPC10,  PMC10
           b0   : P10_0 */
        reg_data       = (uint32_t)GPIO.PFCAE10 & (uint32_t)~0xFFF7;
        GPIO.PFCAE10    = (uint16_t)reg_data;
        reg_data       = (uint32_t)GPIO.PFCE10 & (uint32_t)~0xFFF7;
        GPIO.PFCE10     = (uint16_t)reg_data;
        reg_data       = (uint32_t)GPIO.PFC10 & (uint32_t)~0xFFF7;
        GPIO.PFC10      = (uint16_t)reg_data;
        reg_data       = (uint32_t)GPIO.PIPC10 | (uint32_t)0xFFF7;
        GPIO.PIPC10     = (uint16_t)reg_data;
        reg_data       = (uint32_t)GPIO.PMC10 | (uint32_t)0xFFF7;
        GPIO.PMC10      = (uint16_t)reg_data;

        /* Port 11 */
        reg_data        = (uint32_t)GPIO.PMC11 & (uint32_t)~0x0001;
        GPIO.PMC11       = (uint16_t)reg_data;
        reg_data        = (uint32_t)GPIO.PMC11;
        dummy_read      = reg_data;
        /* PFCAE11, PFCE11, PFC11 ... 1th alternative function
           PIPC11,  PMC11
           b0   : P11_0 */
        reg_data       = (uint32_t)GPIO.PFCAE11 & (uint32_t)~0x000F;
        GPIO.PFCAE11    = (uint16_t)reg_data;
        reg_data       = (uint32_t)GPIO.PFCE11 & (uint32_t)~0x000F;
        GPIO.PFCE11     = (uint16_t)reg_data;
        reg_data       = (uint32_t)GPIO.PFC11 & (uint32_t)~0x000F;
        GPIO.PFC11      = (uint16_t)reg_data;
        reg_data       = (uint32_t)GPIO.PIPC11 | (uint32_t)0x000F;
        GPIO.PIPC11     = (uint16_t)reg_data;
        reg_data       = (uint32_t)GPIO.PMC11 | (uint32_t)0x000F;
        GPIO.PMC11      = (uint16_t)reg_data;
    }
    else
    {

    }
}   /* End of function SetPort() */
