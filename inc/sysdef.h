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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File Name    : sysdef.h
* $Rev: 1000 $
* $Date:: 2017-01-04 21:15:39 +0100#$
* Description  : Aragon Sample Program - sysdef.h
******************************************************************************/
#ifndef _SYSDEF_H_
#define _SYSDEF_H_

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
//Platform
#define				SYSdef_Platform_RSK						(0)
#define				SYSdef_Platform_GENMAI				(1)
#define				SYSdef_Platform_GRPEACH				(2)
#define				SYSTEM_PLATFORM								SYSdef_Platform_RSK

//System Output
#define				SYSdef_DSP_800480							(0)
#define				SYSdef_DSP_1280480 						(1)
#define				SYSdef_DSP_1280800 						(2)
#define				SYSdef_DSP_1366768 						(3)
#define				SYSTEM_DISPLAY								SYSdef_DSP_800480

#if (SYSTEM_DISPLAY==SYSdef_DSP_800480)
#define				Screen_SizeX									800
#define				Screen_SizeY									480

#elif (SYSTEM_DISPLAY==SYSdef_DSP_1280480)
#define				Screen_SizeX									1280
#define				Screen_SizeY									480

#elif (SYSTEM_DISPLAY==SYSdef_DSP_1280800)
#define				Screen_SizeX									1280
#define				Screen_SizeY									800

#elif (SYSTEM_DISPLAY==SYSdef_DSP_1366768)
#define				Screen_SizeX									1360
#define				Screen_SizeY									768
#endif

//System Function
#define				SYSdef_Func_MP3PLAY_					0
#define				SYSdef_Func_RL78G1DBLE				0
#define				SYSdef_Func_VIDEOIN						0
#define				SYSdef_Func_OPENVG						0

//Display Color System
#define				SYSdef_ColorBGRA							0
#define				SYSdef_ColorARGB							1
#define				SYSdef_ColorSystem						SYSdef_ColorBGRA

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

#endif  /* _MAIN_H_ */

/* End of File */
