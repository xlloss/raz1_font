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
* File Name     : rtc.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Sample Program - RTC user code
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdio.h>
/* Default  type definition header */
#include "r_typedefs.h"
/* I/O Register root header */
#include "iodefine.h"
/* Device Driver common header */
#include "dev_drv.h"
/* INTC Driver Header */
#include "intc.h"
/* Switches Header */
#include "rsk_switches.h"
/* PMOD LCD controlling function prototypes & macros */
#include "rza_io_regrw.h"
/* RTC interface header */
#include "rtc.h"



/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Structure to hold data to be written to the RTC */
volatile uint8_t g_alarm_state = ALARM_CLR;

/* Structure to hold data to be read from the RTC */
volatile struct st_rtc rtc_time_read;

/* Storage for time values read from the RTC */
uint8_t rtc_lcd_time[] = " 11:59:30";
uint8_t rtc_time[3];

st_RTCTIME	SYSTEMTIME;

uint8_t rtc_halfsecond_flag= 0;
uint8_t rtc_halfsecond_flag_CNT= 0;
 

/* RTC function prototypes  */
static void rtc_alarm_interrupt (uint32_t int_sense);
static void rtc_periodic_interrupt (uint32_t int_sense);

/******************************************************************************
* Function Name: Init
* Description  : Initialises the RTC.
* Arguments    : none
* Return Value : none
******************************************************************************/
void RTC_Init (void)
{
    uint16_t  dummy_read;

	UNUSED_VARIABLE(dummy_read);

    /* Two dummy reads are added after reading certain registers to
     * ensure the third read will return the correct register value. */
 
		SYSTEMTIME.Year = 0x2016;
		SYSTEMTIME.Month = 0x09;
		SYSTEMTIME.Day = 0x17;
		SYSTEMTIME.Weekday = 3;					
		SYSTEMTIME.Hour = 0x10;
		SYSTEMTIME.Minute = 0x10;
		SYSTEMTIME.Second = 0x00;		
		
    /* Stop the clock, select input clock, reset divider circuit */
    RTC.RCR2 &= 0x08;
    dummy_read = RTC.RCR2;
    dummy_read = RTC.RCR2;

    /* Enable the alarm and periodic interrupts */
    RTC.RCR1  = 0x08;
    RTC.RCR2 &= 0x8F;

    /* Enable 1/2 second periodic interrupt for updating the time onto the LCD. */
//    RTC.RCR2 |= 0xD8;					//2 times per second
    RTC.RCR2 |= 0xD8;					//1 times per second
    dummy_read = RTC.RCR2;
    dummy_read = RTC.RCR2;

    /* RTC_X1 (32.768KHz) is selected by default */
    RTC.RCR5 = 0;

    /* Set seconds, minutes, hours, day, weekday, month and year */
    RTC.RSECCNT = SYSTEMTIME.Second;
    dummy_read  = RTC.RSECCNT;
    dummy_read  = RTC.RSECCNT;
    RTC.RMINCNT = SYSTEMTIME.Minute;
    dummy_read  = RTC.RMINCNT;
    dummy_read  = RTC.RMINCNT;
    RTC.RHRCNT  = SYSTEMTIME.Hour;
    dummy_read  = RTC.RHRCNT;
    dummy_read  = RTC.RHRCNT;
    RTC.RWKCNT  = SYSTEMTIME.Weekday;
    dummy_read  = RTC.RWKCNT;
    dummy_read  = RTC.RWKCNT;
    RTC.RDAYCNT = SYSTEMTIME.Day;
    dummy_read  = RTC.RDAYCNT;
    dummy_read  = RTC.RDAYCNT;
    RTC.RMONCNT = SYSTEMTIME.Month;
    dummy_read  = RTC.RMONCNT;
    dummy_read  = RTC.RMONCNT;
    RTC.RYRCNT  = SYSTEMTIME.Year;
    dummy_read  = RTC.RYRCNT;
    dummy_read  = RTC.RYRCNT;

     /* The setup process for the interrupt INTC_ID_ARM function.*/
    R_INTC_RegistIntFunc(INTC_ID_ARM , &rtc_alarm_interrupt);
    R_INTC_SetPriority(INTC_ID_ARM , 12);
    R_INTC_Enable(INTC_ID_ARM);

    /* The setup process for the interrupt INTC_ID_PRD function.*/
    R_INTC_RegistIntFunc(INTC_ID_PRD , &rtc_periodic_interrupt);
    R_INTC_SetPriority(INTC_ID_PRD , 8);
    R_INTC_Enable(INTC_ID_PRD);

	/* Clear the alarm and periodic interrupt flags */
		RTC.RCR1 = (RTC.RCR1 & 0xFE);
		RTC.RCR2 = (RTC.RCR2 & 0xFE);

    /* Enable the alarm and periodic interrupts */
    RTC.RCR1 |= 0x08;
    RTC.RCR2 |= 0x40;
    dummy_read = RTC.RCR2;
    dummy_read = RTC.RCR2;
    
    RETW_Tick_ThreadSchedulingReg(0);
    
}

/******************************************************************************
* Function Name: Set_Time
* Description  : Configures the time, day of week, month and year.
* Arguments    : uint32_t time,
*                uint8_t day_of_week,
*                uint8_t month,
*                uint16_t year
* Return Value : none
******************************************************************************/
void Set_DateTime()
{
		uint16_t dummy_read;
		
		UNUSED_VARIABLE(dummy_read);

    RTC.RSECCNT = (uint8_t)SYSTEMTIME.Second;
    dummy_read  = RTC.RSECCNT;
    dummy_read  = RTC.RSECCNT;
    RTC.RMINCNT = (uint8_t)SYSTEMTIME.Minute;
    dummy_read  = RTC.RMINCNT;
    dummy_read  = RTC.RMINCNT;
    RTC.RHRCNT  = (uint8_t)SYSTEMTIME.Hour;
    dummy_read  = RTC.RHRCNT;
    dummy_read  = RTC.RHRCNT;

    RTC.RWKCNT  = (uint8_t)SYSTEMTIME.Weekday;
    dummy_read  = RTC.RWKCNT;
    dummy_read  = RTC.RWKCNT;
    
    RTC.RDAYCNT = (uint8_t)SYSTEMTIME.Day;
    dummy_read  = RTC.RDAYCNT;
    dummy_read  = RTC.RDAYCNT;

    RTC.RMONCNT = (uint8_t)SYSTEMTIME.Month;
    dummy_read  = RTC.RMONCNT;
    dummy_read  = RTC.RMONCNT;
    RTC.RYRCNT  = (uint16_t)SYSTEMTIME.Year;
    dummy_read  = RTC.RYRCNT;
    dummy_read  = RTC.RYRCNT;
}
 	

/******************************************************************************
* Function Name: Set_Alarm
* Description  : Configures the alarm time.
* Arguments    : uint32_t time,
*                uint8_t day_of_week,
*                uint8_t month,
*                uint16_t year
* Return Value : none
******************************************************************************/
void Set_Alarm (uint32_t time, uint8_t day, uint8_t day_of_week,
		                                         uint8_t month, uint16_t year)
{
	uint16_t dummy_read;

	UNUSED_VARIABLE(dummy_read);

    RTC.RSECAR = ((uint8_t)(time & 0x000000FF) | ENABLE_INT_COMP);
    dummy_read  = RTC.RSECAR;
    dummy_read  = RTC.RSECAR;

    RTC.RMINAR = ((uint8_t)((time >> 8)  & 0x000000FF) | ENABLE_INT_COMP);
    dummy_read  = RTC.RMINAR;
    dummy_read  = RTC.RMINAR;

    RTC.RHRAR  = ((uint8_t)((time >> 16) & 0x000000FF) | ENABLE_INT_COMP);
    dummy_read  = RTC.RHRAR;
    dummy_read  = RTC.RHRAR;

    RTC.RDAYAR = (day  | ENABLE_INT_COMP);
    dummy_read  = RTC.RDAYAR;
    dummy_read  = RTC.RDAYAR;

    RTC.RWKAR  = (day_of_week  | ENABLE_INT_COMP);
    dummy_read  = RTC.RWKAR;
    dummy_read  = RTC.RWKAR;

    RTC.RMONAR = (month  | ENABLE_INT_COMP);
    dummy_read  = RTC.RMONAR;
    dummy_read  = RTC.RMONAR;

    RTC.RYRAR  = year;
    dummy_read  = RTC.RYRAR;
    dummy_read  = RTC.RYRAR;

    /* Enable comparison of the year counter */
    RTC.RCR3 |= ENABLE_INT_COMP;
}

/*******************************************************************************
* Function Name: RTC_Counter_CLK
* Description  : Starts,stops the counter clocks.
* Arguments    : uint8_t start_stop
* Return Value : none
*******************************************************************************/
void RTC_Counter_CLK (uint8_t start_stop)
{
	uint8_t dummy_read;

	UNUSED_VARIABLE(dummy_read);

	/* Start/stop the counter clock */
	RTC.RCR2 |= start_stop;

	/* This ensures the third read will return the correct register value. */
    dummy_read = RTC.RCR2;
    dummy_read = RTC.RCR2;
}

/******************************************************************************
* Function Name: rtc_periodic_interrupt
* Description  : Reads the RTC counter following detection of the
*                pre-configured period interrupt.
* Arguments    : uint32_t
* Return Value : none
******************************************************************************/
static void rtc_periodic_interrupt (uint32_t int_sense)
{
#if 1
    	SYSTEMTIME.Second = RTC.RSECCNT;
 			SYSTEMTIME.Minute = RTC.RMINCNT;
    	SYSTEMTIME.Hour 	= RTC.RHRCNT;
			SYSTEMTIME.Weekday = RTC.RWKCNT;
 			SYSTEMTIME.Day = RTC.RDAYCNT;
    	SYSTEMTIME.Month = RTC.RMONCNT;
			SYSTEMTIME.Year = RTC.RYRCNT;
#endif
	/* Read all CNT registers */
#if 0
			rtc_lcd_time [1] = (uint8_t)((RTC.RHRCNT >> 4) | 0x30);
			rtc_lcd_time [2] = (uint8_t)((RTC.RHRCNT & 0x0F) | 0x30);

			rtc_lcd_time [4] = (uint8_t)((RTC.RMINCNT >> 4) | 0x30);
			rtc_lcd_time [5] = (uint8_t)((RTC.RMINCNT & 0x0F) | 0x30);

			rtc_lcd_time [7] = (uint8_t)((RTC.RSECCNT >> 4) | 0x30);
			rtc_lcd_time [8] = (uint8_t)((RTC.RSECCNT & 0x0F) | 0x30);


			rtc_time[0] = (uint8_t)((RTC.RHRCNT >> 4) *10 +  (RTC.RHRCNT&0x0F) );
			rtc_time[1] = (uint8_t)((RTC.RMINCNT >> 4) *10 +  (RTC.RMINCNT&0x0F) );
			rtc_time[2] = (uint8_t)((RTC.RSECCNT >> 4) *10 +  (RTC.RSECCNT&0x0F) );

#endif
 
	/* Update the time on the Pmod display */
	//	display_lcd(5, (uint8_t *)rtc_lcd_time);

			rtc_halfsecond_flag = 1;
			rtc_halfsecond_flag_CNT+=1;
			
			if(rtc_halfsecond_flag_CNT>1)
			{																		//Enter every second
				rtc_halfsecond_flag_CNT=0;
 
				RETW_GUI_FrameRateCheckOut();
			}

  			if(RTC.R64CNT == 0)								//entry per second, clear at begining of second.
  			{
 				RETW_Tick_ThreadSchedulingWriteCnt(0,0);
 				}
			
	/* If the alarm is set, toggle LED0 */
	if (ALARM_SET == g_alarm_state)
	{
		/* Turn on LED0 to indicate alarm triggered */
//		R_LED_Toggle(LED0);
	}
	/* Clear the periodic interrupt flag */
	RTC.RCR2 &= 0x7F;
}


/******************************************************************************
* Function Name: rtc_alarm_interrupt
* Description  : Function is called following detection of the RTC alarm
*                interrupt. Sets a flag to signal LED0 toggling is to be
*                stopped before clearing the alarm flag.
* Arguments    : uint32_t
* Return Value : none
******************************************************************************/
static void rtc_alarm_interrupt (uint32_t int_sense)
{
	/* Indicate that the alarm has been set */
	g_alarm_state = ALARM_SET;

	/* Clear the alarm interrupt flag */
	RTC.RCR1 &= 0xFE;
}


/* End of File */

