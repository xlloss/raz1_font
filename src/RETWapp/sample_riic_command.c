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
* File Name    : sample_riic_command.c
* $Rev: 118 $
* $Date:: 2013-10-23 21:06:47 +0100#$
* Description  : Aragon Sample Program - RIIC sample program (Main)
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_riic.h"            /* RIIC Driver Header */
#include "command.h"
#include "iodefine.h"
#include "sample_max9856.h"
 

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define SAMPLE_RIIC_DEVICE_CODE (0xA0)      /* EEPROM device code    : b'1010 */
#define SAMPLE_RIIC_DEVICE_ADDR (0x00)      /* EEPROM device address : b'000  */

#define SAMPLE_RIIC_DATA_SIZE   (13)        /* Data receive RAM size */

#define DELAY_TIME_1MS          (260000uL)  /* Sampling loop                          */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static void    RIIC_PORT_Init(void);
static void    Delay1ms(void);

/******************************************************************************
* Function Name: RIIC_PORT_Init
* Description  : 
* Arguments    : 
* Return Value : 
******************************************************************************/
static void RIIC_PORT_Init(void)
{
    /* ---- P1_6 : SCL3 ---- */
    PORT1.PMCn.BIT.PMCn6     = 1u;
    PORT1.PFCAEn.BIT.PFCAEn6 = 0u;
    PORT1.PFCEn.BIT.PFCEn6   = 0u;
    PORT1.PFCn.BIT.PFCn6     = 0u;
    PORT1.PBDCn.BIT.PBDCn6   = 1u;
    PORT1.PIPCn.BIT.PIPCn6   = 1u;

    /* ---- P1_7 : SDA3 ---- */
    PORT1.PMCn.BIT.PMCn7     = 1u;
    PORT1.PFCAEn.BIT.PFCAEn7 = 0u;
    PORT1.PFCEn.BIT.PFCEn7   = 0u;
    PORT1.PFCn.BIT.PFCn7     = 0u;
    PORT1.PBDCn.BIT.PBDCn7   = 1u;
    PORT1.PIPCn.BIT.PIPCn7   = 1u;
}

/******************************************************************************
* Function Name: Delay1ms
* Description  : Make the sampling interval
* Arguments    : none
* Return Value : none
******************************************************************************/
static void Delay1ms(void)
{
    uint32_t    adc_delay_count;

    adc_delay_count = 0u;

    while (DELAY_TIME_1MS >= adc_delay_count)
    {
        adc_delay_count++;
    }
}

/******************************************************************************
* Function Name: RIIC_PORTX_Init
* Description  :
* Arguments    :
* Return Value :
******************************************************************************/
void RIIC_PORTX_Init(void)
{
    int32_t  ret;
    uint8_t  buf[3];
    uint8_t  buffer = 0;

    RIIC_PORT_Init();

    ret = R_RIIC_Init(RIIC_CHANNEL);
    if (DEVDRV_ERROR == ret)
    {
        /* Error */
    }

    /* Port Expander 1 */
    R_RIIC_WriteCond(RIIC_CHANNEL, RIIC_TX_MODE_START);

    /* Set the port expander device address */
    buffer = (uint8_t)(((SAMPLE_RIIC_PX1_CODE & 0x07u) << 1) | 0);

    buf[0]   = buffer | (SAMPLE_RIIC_PX1_CODE & 0xF0u);
    buf[1]   = PX_CMD_CONFIG_REG;           /* d_command */
    buf[2]   = 0x00u;   /* set the port pin directions */
                        /*
                         * IO[0] = LED1             (output)
                         * IO[1] = LED2             (output)
                         * IO[2] = LED3             (output)
                         * IO[3] = NOR_A25          (output)
                         * IO[4] = PMOD1_RST        (output)
                         * IO[5] = PMOD2_RST        (output)
                         * IO[6] = SD_CONN_PWR_EN   (output)
                         * IO[7] = SD_MMC_PWR_EN    (output)
                         */

    ret = R_RIIC_Write(RIIC_CHANNEL, buf, 3uL);

    R_RIIC_WriteCond(RIIC_CHANNEL, RIIC_TX_MODE_STOP);
    R_RIIC_DetectStop(RIIC_CHANNEL);

    Delay1ms();

    R_RIIC_WriteCond(RIIC_CHANNEL, RIIC_TX_MODE_START);

    /* Set the port expander device address */
    buffer = (uint8_t)(((SAMPLE_RIIC_PX2_CODE & 0x07u) << 1) | 0);

    buf[0]   = buffer | (SAMPLE_RIIC_PX2_CODE & 0xF0u);
    buf[1]   = PX_CMD_CONFIG_REG;           /* d_command */
    buf[2]   = 0x10u;   /* set the port pin directions */
                        /*
                         * IO[0] = PX1_EN0          (output)
                         * IO[1] = PX1_EN1          (output)
                         * IO[2] = TFT_CS           (output)
                         * IO[3] = PX1_EN3          (output)
                         * IO[4] = USB_OVR_CURRENT  (input)
                         * IO[5] = USB_PWR_ENA      (output)
                         * IO[6] = USB_PWR_ENB      (output)
                         * IO[7] = PX1_EN7          (output)
                         */

    ret = R_RIIC_Write(RIIC_CHANNEL, buf, 3uL);

    R_RIIC_WriteCond(RIIC_CHANNEL, RIIC_TX_MODE_STOP);
    R_RIIC_DetectStop(RIIC_CHANNEL);

    Delay1ms();
}
/* End of File */

