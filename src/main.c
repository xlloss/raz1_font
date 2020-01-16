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
* 
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : main.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : This is the RZ/A1H Sample Program main code
* Operation     : Please see the Description.txt file in the doc folder.
* Limitations   : Please see the Description.txt file in the doc folder.
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"            			/* Device Driver common header */
#include "main.h"
/* I2C RSK+RZA1H Eval-board header */
#include "riic_rskrza1h.h"

#include "led.h"
/* OSTM interface header */
#include "ostm.h"
#include "dev_drv.h"
#include "rtc.h"
#include "rscan_init.h"
 
/******************************************************************************
Typedef definitions
******************************************************************************/
 
/******************************************************************************
Macro definitions
******************************************************************************/
#define MAIN_LED_ON     (1)
#define MAIN_LED_OFF    (0)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern int32_t Sample_VDC5_Main(void);

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static uint32_t main_led_flg;      /* LED lighting/turning off */

/******************************************************************************
* Function Name: main
* Description  : Displays the sample program information on the terminal 
*              : connected with the CPU board by the UART, and executes initial 
*              : setting for the PORT connected with the LEDs on the board. 
*              : Executes initial setting for the OSTM channel 0, and sets the 
*              : timer counter so that the OSTM0 interrupt occurs every 500ms.
* Arguments    : none
* Return Value : 0
******************************************************************************/
uint32_t TestMemoryM(uint32_t START_ADDR, uint32_t DRAM_SIZE);
extern uint32_t VG_G_FrameBuffer1[];
extern uint32_t VG_G_FrameBuffer2[];

extern uint16_t _Addr1_ChnFontStr[];
extern uint16_t _Addr1E_ChnFontStr[];
extern uint8_t _Addr1_ChnFontTAB[];
extern uint8_t _Addr1E_ChnFontTAB[];

int_t main(void)
{

		uint32_t i,j;
    /* ==== LED initial setting ==== */ 
    uint8_t g_iic_dev_io;
    uint8_t g_iic_w_value;
    LedInit(); 

    /* Initialise the IIC channel for writing to the port expander */
   // R_RIIC_rza1h_rsk_init();
 
    /* Initialize LED on/off flag */
    main_led_flg = MAIN_LED_ON;
 
    /* ==== Initialise OS timer (channel 0) ==== */
	 	R_OSTM_Init(DEVDRV_CH_0, OSTM_MODE_INTERVAL, 1);

    /* ==== Start OS timer (channel 0) ==== */
	 	R_OSTM_Open(DEVDRV_CH_0);
 
    /* ==== LED On indicates sample is running ==== */
    LedOn();
    LedOn1();
    LedOn2();
while(0)			//for GR peach
		{
				TmrDelay_Graphics(500);
				LedOff2();
				LedOff();
				TmrDelay_Graphics(500);
				LedOn2();				
				LedOn();
		}
    /* Initialise the ADC */
    RTC_Init();
 
    /* Set the alarm */
    Set_Alarm(0x00115940, 0x01, 0x01, 0x01, 0x2017);

    /* Start counting */
    RTC_Counter_CLK(RTC_COUNTER_START);
  
//	Userdef_OSTM0_GenRandomNum();    
    printf("\n Entering RZ/A1H Open VG Sample Program.\n");
 
//  TestMemoryM(0x08000000,0x02000000);							//start from 0x08000000 , test 32M bytes
 
 		FlushTerminalKeyBuff_SCIF2();
 		FlushTerminalKeyBuff_SCIF1();		
		R_ADC_Open();
  
#if 1
    /* for RSK 		Audio MP3*/
    {

    /* Initialise the IIC channel */
    RIIC_PORTX_Init();        //enable Port P1_6, P1_7 for SCL3,SDA3

    /* Configure the port mode of each I/O of the port expander */
    g_iic_dev_io  = 0xFFu;              /* Indicates which I/O is to be modified */
    g_iic_w_value = 0x10u;              /* IO[0] = PX1_EN0 Enable        = 1 (Selects IC37/IC38 MUXes BL[x] signals. Also enables PMOD connectors as they use the SPI from IC38)
                                           IO[1] = PX1_EN1 Enable        = 0 (OFF)
                                           IO[2] = TFT_CS                = 0 (Optional chip select for connecting to TFTs which use SPI. The RSK TFT is not configured by SPI.)
                                           IO[3] = PX1_EN3 Enable        = 0 (Selects Audio signals. If set to 1, selects optional PWM signals.)
                                           IO[4] = USB_OVR_CURRENT       = 1 (Input signal from IC7. By default it is pulled high. It signals overcurrent or thermal shutdown conditions.)
                                           IO[5] = USB_PWR_ENA           = 0 (Pulled low by default, enabling power to VBUS0. JP11 needs to be shorted)
                                           IO[6] = USB_PWR_ENB           = 0 (Pulled low by default, enabling power to VBUS1. JP12 needs to be shorted)
                                           IO[7] = PX1_EN7               = 0 (Pulled low by default, selecting A18 - A21 lines. If set to 1, SGOUTx signals are selected.) */

    Sample_RIIC_PortExpAccess(0x41u, 1u, g_iic_dev_io, g_iic_w_value);
    }
    

  
//    R_DAC_MAX9856_Init();                   /* for RSK */

while(0)
{
    g_iic_dev_io  = 0xFFu;              /* Indicates which I/O is to be modified */
    g_iic_w_value = 0x07u;              /* bit7-0:  x, x, x, x, x, LED3,LED2,LED1*/
    Sample_RIIC_PortExpAccess(0x40u, 1u, g_iic_dev_io, g_iic_w_value);
 
    				TmrDelay_Graphics(1000);

    g_iic_dev_io  = 0xFFu;              /* Indicates which I/O is to be modified */
    g_iic_w_value = 0x00u;              /* bit7-0:  x, x, x, x, x, LED3,LED2,LED1*/
    Sample_RIIC_PortExpAccess(0x40u, 1u, g_iic_dev_io, g_iic_w_value);
 
    				TmrDelay_Graphics(1000);
			    
}

#endif
 

// 		Audio_Sample_Main();    
       
#if 1 

while(0)
{
KeyScanning_Main();
Audio_MainLoop();
} 

// 			DumpData(0x18800000+0x00681bA0,0x100);
 			
//  		while(RETW_SVGFontParsing("#x5c07",6)==0) {}
//      while(RETW_SVGFontParsing("#x8ecd",6)==0) {}
//      while(RETW_SVGFontParsing("#x745e",6)==0) {}

#if 1
 			RETW_ChnSVGFontOpen(_Addr1_ChnFontStr, _Addr1E_ChnFontStr-_Addr1_ChnFontStr, _Addr1_ChnFontTAB, _Addr1E_ChnFontTAB-_Addr1_ChnFontTAB);
#endif
 						 
//			GRAPHICS_VideoSample();   
//    	printf("Entering RSCAN_Init...\n");
//			RSCAN_Init();
//    	printf("Exited RSCAN_Init...\n");
 

      GRAPHICS_VideoSample();
 			
  	  Graphics_OpenVGSample();				//originally this routine is called by above GRAPHICS_VideoSample()
  	  
  	  
#endif
    /* ==== Receive command, activate sample software ==== */
//    Sample_VDC5_Main();

    /* ==== LED Off indicates sample is complete ==== */
   
 
    return 0;
}

uint32_t TestMemoryM(uint32_t START_ADDR, uint32_t DRAM_SIZE)
{
				int errormark=0;
	
        uint32_t iptr;
        uint32_t result;
 
        printf("Testing from %x ,DRAM_SIZE=%x ",START_ADDR ,DRAM_SIZE);
 
/************************************************************************/
        printf("\n");
        printf("Testing RAM by address pattern.....\n");
        printf("Fill RAM by @address == address now.....\n");
        for (iptr=0 ; iptr < DRAM_SIZE/4  ; iptr+=1)
        {
            (*(uint32_t *)( START_ADDR + iptr*4 )) =iptr;

            if ( (iptr % 0x00100000) == 0 )
											printf("Fill address pattern till %x.\n",		START_ADDR+iptr*4);
        }
 
        printf("Check if RAM @address == address ?.....\n");
        for (iptr=0 ; iptr < DRAM_SIZE/4  ; iptr+=1)
        {
            result = (*(uint32_t *)( START_ADDR + iptr*4 ));
            if  (result != iptr)
                {
printf("--->Error at %x,Wr=%x,Rd=%x.\n", START_ADDR+iptr*4 ,iptr, result);
                errormark=1;
                }
            if ( (iptr % 0x00100000) == 0 )
								printf("Test address till %x.\n",		START_ADDR+iptr*4  );
 
        }
printf("Address testing pattern was finished.\n");


/************************************************************************/
/* Testing RAM by pattern... */
        printf("Testing RAM by fixed pattern.....\n");
        for (iptr=0 ; iptr < DRAM_SIZE/4 ; iptr++)
        {

        (*(uint32_t *)( START_ADDR + iptr*4)) = 0x0055aaff;
        result= (*(uint32_t *)( START_ADDR + iptr*4));
            if  (result != 0x0055aaff)
                {
printf("--->Error at %x, Wr=0x0055AAFF , Rd=%x.\n",START_ADDR+iptr*4,result);
                errormark=1;

                }
        (*(uint32_t *)( START_ADDR + iptr*4)) = 0xffaa5500;
        result= (*(uint32_t *)( START_ADDR + iptr*4));
            if  (result != 0xffaa5500)
                {
printf("--->Error at %x, Wr=0xFFAA5500 , Rd=%x.\n",START_ADDR+iptr*4,result);
                errormark=1;

                }
            if ( (iptr %0x00100000) == 0 )
                {
printf("Test address till %x.\n",START_ADDR+(iptr+1)*4 );
                }
        }
printf("Data testing pattern was finished.\n");
        if(errormark==1)
	{printf("*****RAM test has errors!\n\n\n");
                return 0;}
        else
        {
printf("\n");
printf("*****RAM test Pass!\n\n\n");
	return 1;
        }
}       /* TestMemory() */



/******************************************************************************
/* End of Function main
******************************************************************************/
/* End of File */
