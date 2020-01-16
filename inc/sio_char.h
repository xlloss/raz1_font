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
* File Name    : sio_char.h
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* Description  : Aragon Sample Program - Terminal I/O
******************************************************************************/
#ifndef _SIO_CHAR_H_
#define _SIO_CHAR_H_

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/
#define KEY_SCAN_NO_WAIT    /* Compile Switch for Key scan no wait */

#define NO_KEY_INPUT    (-10)

#define INPUT_ERROR     (-1)
#define NO_ENTER_KEY    (-2)
/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
int32_t SioWrite(int32_t file_no, const int_t * buffer, uint32_t writing_b);
#ifdef KEY_SCAN_NO_WAIT
int32_t SioRead(int32_t file_no, int_t * buffer, uint32_t offset);
int_t StringGets(char_t * get_string, int32_t size);
#else  /* KEY_SCAN_NO_WAIT */
int32_t SioRead(int32_t file_no, int_t * buffer, uint32_t reading_b);
#endif /* KEY_SCAN_NO_WAIT */

void   IoInitScif2(void);
char_t IoGetchar2(void);
void   IoPutchar(char_t buffer);

#endif  /* _SIO_CHAR_H_ */

/* End of File */
