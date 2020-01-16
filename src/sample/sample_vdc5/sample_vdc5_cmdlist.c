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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : sample_vdc5_cmdlist.c
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* Description : VDC5 sample program - Command list
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "command.h"
#include "sample_vdc5.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static const command_list_t sample_init_cmd_list[] = 
{
    {"1",       Sample_OpenVGSample ,       NULL}, /* Aragon OpenVG Sample   */
    {"HELP",    Sample_VDC5_Help,           NULL}, /* Display command help   */
    {NULL,      NULL,                       NULL}
};

/******************************************************************************
* Function Name: Sample_VDC5_Help
* Description  : VDC5 Display help text.
* Arguments    : int32_t argc
*              : char_t ** argv
* Return Value : COMMAND_SUCCESS : Success
*              : COMMAND_ERROR   : Error
******************************************************************************/
int32_t Sample_VDC5_Help(int32_t argc, char_t ** argv)
{
    /* ==== Command help ==== */
    if (1 == argc)
    {
    	printf("  1    : RZ_A1H OpenVG Graphics Sample \n");
    	printf("         Sliding and Scaling \n");
        printf("\n");
        printf("  HELP : Display command help\n");
        printf("\n");
    }
    else
    {
        printf("error: Command failure.\n");
        return COMMAND_ERROR;
    }

    return COMMAND_SUCCESS;
}

/******************************************************************************
* Function Name: Sample_GetCmdList
* Description  : VDC5 Get list of available commands for the sample.
* Arguments    : none
* Return Value : command_list_t * : Start address of the command list
******************************************************************************/
const command_list_t * Sample_VDC5_GetCmdList(void)
{
    return sample_init_cmd_list;
}

/* End of File */

