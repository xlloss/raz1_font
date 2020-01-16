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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         graphics_debug.c
* @version      0.11
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* @brief        
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    "graphics_debug.h"

#ifdef  ENABLE_DEBUG_PRINT
#include    <stdio.h>
#endif  /* ENABLE_DEBUG_PRINT */

#include    "r_typedefs.h"


/******************************************************************************
Macro definitions
******************************************************************************/
#define     FILE_NAME_LENGTH    ((int32_t)8)


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/


/**************************************************************************//**
 * @brief       Debug print
 * @param[in]   file
 * @param[in]   line
 * @param[in]   code
 * @retval      None
******************************************************************************/
void GRAPHICS_DebugPrint (const uint8_t * file, const int32_t line, const int32_t code)
{
#ifdef      ENABLE_DEBUG_PRINT
    printf("%s(%d): %d\n", file, line, code);
#else
    volatile static uint8_t file_name[FILE_NAME_LENGTH];
    volatile static int32_t line_num;
    volatile static int32_t error_code;
    int32_t                 name_len;

    for (name_len = 0; name_len < FILE_NAME_LENGTH; name_len++)
    {
        if ((uint32_t)*file != 0u)
        {
            file_name[name_len] = *file;
        }
        file++;
    }
    line_num    = line;
    error_code  = code;
#endif      /* ENABLE_DEBUG_PRINT */
}   /* End of function GRAPHICS_DebugPrint() */

