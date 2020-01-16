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
* Copyright (C) 2012 - 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : sample_riic_eep.c
* $Rev: 118 $
* $Date:: 2013-10-23 21:06:47 +0100#$
* Description  : Aragon Sample Program - RIIC sample program (EEPROM read/write)
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "devdrv_riic.h"
#include "sample_max9856.h"

#include "iodefine.h"
#include "rza_io_regrw.h"
 

/******************************************************************************
Typedef definitions
******************************************************************************/
#define PORTX_OUTPUT_REG            (0x01u)

/******************************************************************************
Macro definitions
******************************************************************************/
#define SAMPLE_RIIC_RWCODE_W        (0)     /* IIC data write code */
#define SAMPLE_RIIC_RWCODE_R        (1)     /* IIC data read code  */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
* Function Name: Sample_RIIC_CodecWrite
* Description  : 
* Arguments    : none
* Return Value : none
******************************************************************************/
int32_t Sample_RIIC_CodecWrite(uint8_t d_code, uint8_t d_adr, uint16_t w_adr, uint32_t w_byte, uint8_t * w_buffer)
{
    int32_t  ret;
    uint32_t channel;
    uint8_t  buf[3];

    channel = RIIC_CHANNEL;                                 /* DEVDRV_CH_2 */

    buf[0] = (uint8_t)(d_code | ((d_adr & 0x07u) << 1) | SAMPLE_RIIC_RWCODE_W);
    buf[1] = (uint8_t)(w_adr  & 0x00FFu);
    buf[2] = (uint8_t)(*w_buffer);

    R_RIIC_WriteCond(channel, RIIC_TX_MODE_START);

    ret = R_RIIC_Write(channel, buf, 3uL);

    R_RIIC_WriteCond(channel, RIIC_TX_MODE_STOP);
    R_RIIC_DetectStop(channel);

    return ret;
}

/******************************************************************************
* Function Name: Sample_RIIC_PortExpAccess
* Description  : Function to write to a port expander's registers:
*                output port
*                configuration
*                polarity
* Arguments    : none
* Return Value : none
******************************************************************************/

#if 1  //Generallai
int32_t Sample_RIIC_PortExpAccess(uint8_t d_code, uint8_t reg_command, uint8_t config_io, uint8_t w_byte)
{
    int32_t  ret;
    uint32_t channel;
    uint8_t  buf[3];
    uint8_t  j;
    uint8_t  port_x_state;

    /* ==== Get port expander's IO states ==== */
    port_x_state = 0u;

    /* Update the port expander's IO states if required */
    if (PORTX_OUTPUT_REG == reg_command)
    {
        /* Re-write the new bit sequence to the buffer */
        for (j = 0u; j < 8u; j++)
        {
            /* Check which port expander output bit needs change */
            if (1u == ((config_io >> j) & 1u))
            {
                /* Check if a config_io bit needs to be set or cleared */
                if (1u == (1u & (w_byte >> j)))
                {
                    /* Set the bit pointed by the loop counter j */
                    port_x_state |= ((uint8_t)1u << j);
                }
                else
                {
                    /* Clear the bit pointed by the loop counter j */
                    port_x_state &= ~((uint8_t)1u << j);
                }
            }
        }

        channel = RIIC_CHANNEL;                                 /* DEVDRV_CH_2 */

        /* Set the port expander's device address */
        buf[0] = (uint8_t)(((d_code & 0x07u) << 1u) | SAMPLE_RIIC_RWCODE_W);
        buf[0]   = buf[0] | (d_code & 0xF0u);
        /* Set the command byte */
        buf[1] = reg_command;
        /* Set the port expander's port data */
        buf[2] = port_x_state;              /* & w_byte force the lower 3 bits to go low in order to turn on LED2-LED3 */

        /* Wait for the bus to become free */
        while (1u == RZA_IO_RegRead_8(&(RIIC3.RIICnSR2.UINT8[0]), RIICn_RIICnSR2_STOP_SHIFT, RIICn_RIICnSR2_STOP))
        {
            /* Wait */
        }

        R_RIIC_WriteCond(channel, RIIC_TX_MODE_START);

        /* ==== Write 3 bytes, port expander address, command and port expander output data */
        ret = R_RIIC_Write(channel, buf, 3uL);

        R_RIIC_WriteCond(channel, RIIC_TX_MODE_STOP);
        R_RIIC_DetectStop(channel);
    }

    return ret;
}

#endif
/* End of File */
