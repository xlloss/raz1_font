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
* File Name     : ostm_userdef.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Aragon Sample Program - OS timer device driver (User define function)
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                
#include "ostm.h"            
#include "intc.h"           
#include "iodefine.h"
#include "main.h"
#include "sample_main.h"
#include "led.h"
#include "rza_io_regrw.h"

#include    <stdlib.h>
//#include    <time.h>
 

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
/* 33.333MHz */
#define P0_CLOCK_FREQUENCY_KHZ  (33.333 * 1000)
#define MAX_CYCLE_msec          (0xFFFFFFFF / P0_CLOCK_FREQUENCY_KHZ)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

//st_ThreadControlParameter ThreadTab[10];

uint16_t	ThreadControlTab[20]={
										0x0000,100,
										0x0000,100,
										0x0000,100,
										0x0000,100,
										0x0000,100,
										0x0000,100,
										0x0000,100,
										0x0000,100,
										0x0000,100,
										0x0000,100};
 
/******************************************************************************
Private global variables and functions
******************************************************************************/
static volatile uint8_t ostm_int_flg;
static uint32_t OSTM_divider=0;

uint32_t Milisec_VirtualEngineTickCount=0;
uint32_t Milisec_ModeEntry=0;
uint32_t Milisec_SWMeterEntry=0;
uint32_t Milisec_SignFlashCount=0;

 
/******************************************************************************
* Function Name: Userdef_OSTM0_Init
* Description  :
* Arguments    : uint32_t mode
*              : uint32_t cycle
* Return Value : DEVDRV_SUCCESS
*              : DEVDRV_ERROR
******************************************************************************/
int32_t Userdef_OSTM0_Init (uint32_t mode, uint32_t cycle)
{
    volatile uint8_t dummy_buf;

    if (cycle > MAX_CYCLE_msec)
    {
        return DEVDRV_ERROR;
    }

    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the OSTM(channel 0) ---- */
    rza_io_reg_write_8(&CPG.STBCR5,
                      0,
                      CPG_STBCR5_MSTP51_SHIFT,
                      CPG_STBCR5_MSTP51
                      );

    /* Dummy read */
    dummy_buf = CPG.STBCR5;

    OSTM0.OSTMnTT |= 0x01U;
    OSTM0.OSTMnCTL &= 0xFDU;
    OSTM0.OSTMnCTL |= (mode << 10);
    OSTM0.OSTMnCTL |= 0x01U;
    OSTM0.OSTMnCMP = (P0_CLOCK_FREQUENCY_KHZ * cycle);

    R_INTC_RegistIntFunc(INTC_ID_OSTM0TINT, Sample_OSTM0_Interrupt);

    R_INTC_SetPriority(INTC_ID_OSTM0TINT, 5);

    R_INTC_Enable(INTC_ID_OSTM0TINT);

    ostm_int_flg = DEVDRV_FLAG_OFF;

    return DEVDRV_SUCCESS;
}
 

int32_t Userdef_OSTM0_ReadCounter(void)
{  
    return OSTM0.OSTMnCNT;
}

int32_t Userdef_OSTM0_GenRandomNum(void)
{
		int i;
    uint16_t				number;
    uint16_t				snumber[8];   

		number = (uint16_t)Userdef_OSTM0_ReadCounter();
		printf("number = %x\n",number);

		srand(number);
		number = (rand()%100)+1;
 
		for(i=0;i<8;i++)
				{
				number = (rand()%100)+1;
				snumber[i]= number;
			  }
 
		printf("Random number is ");				

		for(i=0;i<8;i++)
		printf("  %x ",snumber[i]);
		printf("\n");
}	
		

/******************************************************************************
* Function Name: Userdef_OSTM0_Init_USB
* Description  : This function is a user-defined function. Run the processing to
*              : execute initial setting for the count value of the OSTM channel 0
*              : using the timer cycle specified by the argument cycle in the
*              : operating mode specified by the argument mode. In this sample code,
*              : the timer count value is set assuming that the P0 clock is set to
*              : 33.33MH. The initial setting for the use of the OSTM0 interrupt
*              : is also executed.
* Arguments    : uint32_t mode    : OSTM operating mode
*              :                  :   OSTM_MODE_INTERVAL : Interval timer mode
*              :                  :   OSTM_MODE_COMPARE  : Free running compare mode
*              : uint32_t cycle   : Timer cycle (msec)
* Return Value : DEVDRV_SUCCESS   : Success to initialise OSTM
*              : DEVDRV_ERROR     : Failure to initialise OSTM
******************************************************************************/
int32_t Userdef_OSTM0_Init_USB(uint32_t mode, uint32_t cycle)
{
    volatile uint8_t dummy_buf;

    /* ==== Argument check ==== */
    if (cycle > MAX_CYCLE_msec)
    {
        return DEVDRV_ERROR;
    }

    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the OSTM(channel 0) ---- */
    rza_io_reg_write_8(&CPG.STBCR5,
                      0,
                      CPG_STBCR5_MSTP51_SHIFT,
                      CPG_STBCR5_MSTP51
                      );

    /* Dummy read */
    dummy_buf = CPG.STBCR5;

    /* ==== Start counting with OS timer 500ms ===== */
    /* ---- OSTM count stop trigger register (TT) setting ---- */
    /* Stop counting */
    OSTM0.OSTMnTT = 0x01u;

    /* ---- OSTM control register (CTL) setting ---- */
    /* Counter operating mode               */
    /* Enable interrupt when start counting */
    rza_io_reg_write_8(&OSTM0.OSTMnCTL,
                       (uint8_t)mode,
                        OSTM0_OSTMnCTL_MD1_SHIFT,
                        OSTM0_OSTMnCTL_MD1
                       );

    rza_io_reg_write_8(&OSTM0.OSTMnCTL,
                       1, OSTM0_OSTMnCTL_MD0_SHIFT,
                       OSTM0_OSTMnCTL_MD0
                      );

    /* ---- OSTM compare register (CMP) setting ---- */
    OSTM0.OSTMnCMP = (P0_CLOCK_FREQUENCY_KHZ * cycle);

    /* Set timer in the cycle specified by the cycle */
    /* CMP = P0 clock (kHz) * cycle (msec)           */
    /* (Assume that P0 clock has 33.333MHz)          */

    /* ==== Register OS timer interrupt handler ==== */
    R_INTC_RegistIntFunc(INTC_ID_OSTM0TINT, Sample_OSTM0_Interrupt);

    /* ==== Set priority of OS timer interrupt to 5 ==== */
    R_INTC_SetPriority(INTC_ID_OSTM0TINT, 5);

    /* ==== Validate OS timer interrupt ==== */
    R_INTC_Enable(INTC_ID_OSTM0TINT);

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_OSTM1_Init
* Description  :
* Arguments    : uint32_t mode
*              : uint32_t cycle
* Return Value : DEVDRV_SUCCESS
*              : DEVDRV_ERROR
******************************************************************************/
int32_t Userdef_OSTM1_Init(uint32_t mode, uint32_t cycle)
{
    /* Is the number of cycles valid */
    if (cycle > MAX_CYCLE_msec)
    {
        return DEVDRV_ERROR;
    }

    /* Configure the OSTM */
    OSTM1.OSTMnTT |= 0x01U;
    OSTM1.OSTMnCTL &= 0xFDU;
    OSTM1.OSTMnCTL |= (mode << 1);
    OSTM1.OSTMnCTL |= 0x01U;
    OSTM1.OSTMnCMP = (P0_CLOCK_FREQUENCY_KHZ * cycle);

    /* Link the interrupt vector to a handler */
    R_INTC_RegistIntFunc(INTC_ID_OSTM1TINT, Sample_OSTM1_Interrupt);

    /* Set the priority */
    R_INTC_SetPriority(INTC_ID_OSTM1TINT, 5);

    /* Enable the interrupt */
    R_INTC_Enable(INTC_ID_OSTM1TINT);

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_OSTM0_Int
* Description  : 
* Arguments    : 
* Return Value : none
******************************************************************************/
void Userdef_OSTM0_Int (void)
{
    /* ==== Toggle LED indicator flag ==== */
 //   R_LED_Toggle(LED0);
 
 

 
 
 
}

/******************************************************************************
* Function Name: Userdef_OSTM1_Int
* Description  : 
* Arguments    : 
* Return Value : none
******************************************************************************/
void Userdef_OSTM1_Int (void)
{
    /* Add user code */
}

/******************************************************************************
* Function Name: Userdef_OSTM0_WaitInt
* Description  : 
* Arguments    : 
* Return Value : none
******************************************************************************/
void Userdef_OSTM0_WaitInt (void)
{
    /* Add user code */
}

/******************************************************************************
* Function Name: Userdef_OSTM1_WaitInt
* Description  : 
* Arguments    : 
* Return Value : none
******************************************************************************/
void Userdef_OSTM1_WaitInt (void)
{
    /* Add user code */
}

/******************************************************************************
* Function Name: Sample_OSTM0_Interrupt
* Description  :
* Arguments    :
* Return Value : none
******************************************************************************/
void Sample_OSTM0_Interrupt (uint32_t int_sense)
{
    /* ==== Toggle LED indicator flag ==== */
 		//R_LED_Toggle(LED0);
 		 		
  	Milisec_VirtualEngineTickCount+=1;
 		Milisec_ModeEntry+=1;
		Milisec_SWMeterEntry+=1;
		Milisec_SignFlashCount+=1;
 		
 		ExportTickCount_Graphics();
  	ExportMeterSpeed(Milisec_VirtualEngineTickCount);
    ExportTimeSharingTick();
    
    if(Milisec_VirtualEngineTickCount>8640000)
    Milisec_VirtualEngineTickCount=0;
    
    if(Milisec_ModeEntry>8640000)
    Milisec_ModeEntry=0;

    if(Milisec_SWMeterEntry>8640000)
    Milisec_SWMeterEntry=0;    

    if(Milisec_SignFlashCount>8640000)
    Milisec_SignFlashCount=0;  
        
 #if 1
		OSTM_divider+=1;
		if(OSTM_divider==10)
			{
//			Tick_KeyScanUART();
			RETW_Tick_ThreadScheduling();
  		OSTM_divider=0;
  		}
 #endif
}

RETW_Tick_ThreadScheduling()
{
	int i;
	for(i=0;i<20;i+=2)
		{
		if(ThreadControlTab[i]==1)
		ThreadControlTab[i+1]++;
		}
}
uint32_t RETW_Tick_ThreadSchedulingReg(uint32_t ThreadNo)
{
		if(ThreadNo <10 && ThreadControlTab[ThreadNo*2]==0 )
			{
				ThreadControlTab[ThreadNo*2]=1;
				ThreadControlTab[ThreadNo*2+1]=0x0000;
			}
		else
		return 0x80;
}
uint32_t RETW_Tick_ThreadSchedulingUnReg(uint32_t ThreadNo)
{
		if(ThreadControlTab[ThreadNo*2]==1)				//confirm it is alive
				ThreadControlTab[ThreadNo*2]=0;
		else
		return 0x80;
}
uint32_t RETW_Tick_ThreadSchedulingReadCnt(uint32_t ThreadNo)
{
		if(ThreadControlTab[ThreadNo*2]==1)				//confirm it is alive
		return ThreadControlTab[ThreadNo*2+1];
		return 0x80;
}
uint32_t RETW_Tick_ThreadSchedulingWriteCnt(uint32_t ThreadNo, uint32_t ThreadCnt)
{
		if(ThreadControlTab[ThreadNo*2]==1)				//confirm it is alive
		ThreadControlTab[ThreadNo*2+1] =ThreadCnt ;
		else
		return 0x80;
}

/******************************************************************************
* Function Name: Sample_OSTM1_Interrupt
* Description  :
* Arguments    :
* Return Value : none
******************************************************************************/
void Sample_OSTM1_Interrupt (uint32_t int_sense)
{
    /* Add user code */
}

/* End of File */

