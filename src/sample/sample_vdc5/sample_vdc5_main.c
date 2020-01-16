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
* File Name   : sample_vdc5_main.c
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* Description : VDC5 sample program - Main
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
static char_t sample_vdc5_main_command_buf[COMMAND_MAX_ARGLENGTH];

/******************************************************************************
* Function Name: Sample_VDC5_Main
* Description  : Entry point for VDC5 sample
* Arguments    : none
* Return Value : COMMAND_EXIT : Sample has completed.
******************************************************************************/
int32_t Sample_VDC5_Main(void)
{
    const command_list_t *command_list;


 
    printf("\n");
    printf("\nRZ/A1H Open VG Sample Program. Ver.1.00.00.00\n");
    printf("Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.\n");
    printf("\n");

    printf("Use 'Help' command to show list available operations for this sample.\n");
    printf("\n");
 
    command_list = Sample_VDC5_GetCmdList();
    CommandSetCmdList(command_list);

    while (1)
    {
        printf("OpenVG SAMPLE> ");
        if (gets(sample_vdc5_main_command_buf) == NULL)
        {
            printf("error: gets error\n");
            while (1)
            {
                /* Do Nothing */
            }
        }

        if (CommandExe(sample_vdc5_main_command_buf) == COMMAND_EXIT)
        {
            return COMMAND_EXIT;
        }
    }
    return COMMAND_EXIT;
}
int32_t Sample_VDC5_GetKeyIn(void)
{
			int32_t Key;
			
	        Key = IoGetchar2();				//gets(sample_vdc5_main_command_buf);
	        return Key;
}

/* End of File */

