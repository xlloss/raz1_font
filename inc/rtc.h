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
******************************************************************************/
/******************************************************************************
* File Name     : rtc.h
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Interface for the RTC module.
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef _RTC_INTERFACE_H_
#define _RTC_INTERFACE_H_

/*******************************************************************************
Macro definitions (Register bit)
*******************************************************************************/
#define RTC_COUNTER_START    1
#define RTC_COUNTER_STOP     0

#define ENABLE_INT_COMP      (uint8_t)0x80

#define ALARM_CLR       (0)
#define ALARM_SET       (1)

/******************************************************************************
Variable Externs
******************************************************************************/
/* Structure to hold data to be written to the RTC */
extern volatile uint8_t g_alarm_state;

#if 1
typedef struct RTCTIME
{
				unsigned int Year;
				unsigned char Month;
				unsigned char Day;
				unsigned char Weekday;				
				unsigned char Hour;
				unsigned char Minute;
				unsigned char Second;
} st_RTCTIME;
								
#endif
/******************************************************************************
Functions Prototypes
******************************************************************************/
/* Function used to initialise RTC registers */
void Init_RTC (void);

/* Function used to start/stop the RTC counter */
void RTC_Counter_CLK (uint8_t start_stop);

/* Function for setting the time */
void Set_DateTime  ( );

/* Function for setting the alarm time  */
void Set_Alarm (uint32_t time, uint8_t day, uint8_t day_of_week,
		                                         uint8_t month, uint16_t year);

/* _RTC_INTERFACE_H_*/
#endif
