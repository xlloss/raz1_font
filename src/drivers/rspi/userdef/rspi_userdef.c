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
* Copyright (C) 2012 - 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : rspi_userdef.c
* $Rev: 699 $
* $Date:: 2014-02-05 15:56:20 +0900#$
* Description : Renesas Serial Peripheral Interface(RSPI) device driver (User define function)
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_rspi.h"            /* RSPI Driver header */
#include "devdrv_intc.h"            /* INTC Driver Header */
//#include "sample_rspi_wm8978.h"
#include "iodefine.h"
#include "rza_io_regrw.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
 

/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== Bit rate setting (SPR register, BRDV bit) ==== */
/* CPU internal clock (I clock) : 400MHz, Internal bus clock (B clock) : 133.33MHz  */
/* Peripheral clock1 (P1 clock) : 66.67MHz, Peripheral clock0 (P0 clock) : 33.33MHz */
/* Setting to operate the bit rate at the speed of 2.78Mbps (=(P1 clock)/24)        */
/* with the condition above                                                         */
#define RSPI_SPR            (5)         /* Setting value of the SPBR register */
#define RSPI_BRDV           (1)         /* Setting value of the BRDV bit of   */
                                        /* the SPCMD0 register                */

/* ==== Dummy data ==== */
#define RSPI_DUMMY_DATA_32  (0xFFFFFFFFuL)
#define RSPI_DUMMY_DATA_16  (0xFFFFu)
#define RSPI_DUMMY_DATA_8   (0xFFu)

/* ==== Access to the SPDR register ==== */
#define RSPI_SPDR_ACCESS_8      (1)     /* 8-bit access  */
#define RSPI_SPDR_ACCESS_16     (2)     /* 16-bit access */
#define RSPI_SPDR_ACCESS_32     (3)     /* 32-bit access */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static volatile uint8_t rspi_transmit_empty_flg;    /* Transmit buffer empty flag */

static void Userdef_RSPI4_Port_Init(void);


/******************************************************************************
* Function Name: Userdef_RSPI4_Port_Init
* Description  : Initializes the PORT used in RSPI transmission. 
*              : respectively.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void Userdef_RSPI4_Port_Init(void)
{
    /* ---- P4_0 : RSPCK4 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC4,  0, GPIO_PIBC4_PIBC40_SHIFT,   GPIO_PIBC4_PIBC40);
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC40_SHIFT,   GPIO_PBDC4_PBDC40);
    RZA_IO_RegWrite_16(&GPIO.PM4,    1, GPIO_PM4_PM40_SHIFT,       GPIO_PM4_PM40);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   0, GPIO_PMC4_PMC40_SHIFT,     GPIO_PMC4_PMC40);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  0, GPIO_PIPC4_PIPC40_SHIFT,   GPIO_PIPC4_PIPC40);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 7th multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  1, GPIO_PBDC4_PBDC40_SHIFT,   GPIO_PBDC4_PBDC40);
    RZA_IO_RegWrite_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC40_SHIFT,     GPIO_PFC4_PFC40);
    RZA_IO_RegWrite_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE40_SHIFT,   GPIO_PFCE4_PFCE40);
    RZA_IO_RegWrite_16(&GPIO.PFCAE4, 1, GPIO_PFCAE4_PFCAE40_SHIFT, GPIO_PFCAE4_PFCAE40);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC40_SHIFT,   GPIO_PIPC4_PIPC40);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC40_SHIFT,     GPIO_PMC4_PMC40);

    /* ---- P4_1 : SSL40 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC4,  0, GPIO_PIBC4_PIBC41_SHIFT,   GPIO_PIBC4_PIBC41);
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC41_SHIFT,   GPIO_PBDC4_PBDC41);
    RZA_IO_RegWrite_16(&GPIO.PM4,    1, GPIO_PM4_PM41_SHIFT,       GPIO_PM4_PM41);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   0, GPIO_PMC4_PMC41_SHIFT,     GPIO_PMC4_PMC41);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  0, GPIO_PIPC4_PIPC41_SHIFT,   GPIO_PIPC4_PIPC41);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 7th multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  1, GPIO_PBDC4_PBDC41_SHIFT,   GPIO_PBDC4_PBDC41);
    RZA_IO_RegWrite_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC41_SHIFT,     GPIO_PFC4_PFC41);
    RZA_IO_RegWrite_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE41_SHIFT,   GPIO_PFCE4_PFCE41);
    RZA_IO_RegWrite_16(&GPIO.PFCAE4, 1, GPIO_PFCAE4_PFCAE41_SHIFT, GPIO_PFCAE4_PFCAE41);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC41_SHIFT,   GPIO_PIPC4_PIPC41);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC41_SHIFT,     GPIO_PMC4_PMC41);

    /* ---- P4_2 : MOSI4 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC4,  0, GPIO_PIBC4_PIBC42_SHIFT,   GPIO_PIBC4_PIBC42);
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC42_SHIFT,   GPIO_PBDC4_PBDC42);
    RZA_IO_RegWrite_16(&GPIO.PM4,    1, GPIO_PM4_PM42_SHIFT,       GPIO_PM4_PM42);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   0, GPIO_PMC4_PMC42_SHIFT,     GPIO_PMC4_PMC42);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  0, GPIO_PIPC4_PIPC42_SHIFT,   GPIO_PIPC4_PIPC42);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 7th multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  1, GPIO_PBDC4_PBDC42_SHIFT,   GPIO_PBDC4_PBDC42);
    RZA_IO_RegWrite_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC42_SHIFT,     GPIO_PFC4_PFC42);
    RZA_IO_RegWrite_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE42_SHIFT,   GPIO_PFCE4_PFCE42);
    RZA_IO_RegWrite_16(&GPIO.PFCAE4, 1, GPIO_PFCAE4_PFCAE42_SHIFT, GPIO_PFCAE4_PFCAE42);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC42_SHIFT,   GPIO_PIPC4_PIPC42);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC42_SHIFT,     GPIO_PMC4_PMC42);
}

/******************************************************************************
* Function Name: Userdef_RSPI0_Init
* Description  : The RSPI initial setting and setting for interrupts are required
*              : when the RSPI channel 0 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_Init(void)
{
    /* Add a processing when the RSPI0 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI1_Init
* Description  : The RSPI initial setting and setting for interrupts are required
*              : when the RSPI channel 1 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_Init(void)
{
    /* Add a processing when the RSPI1 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI2_Init
* Description  : The RSPI initial setting and setting for interrupts are required
*              : when the RSPI channel 2 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_Init(void)
{
    /* Add a processing when the RSPI2 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI3_Init
* Description  : The RSPI initial setting and setting for interrupts are required
*              : when the RSPI channel 3 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_Init(void)
{
    /* Add a processing when the RSPI3 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI4_Init
* Description  : Initial setting is required to use the RSPI channel 4.
*              : In the sample code, the RSPI transfer is executed in the master
*              : mode by setting the bit rate of 2.78Mbps (=(P1 clock)/24), and 
*              : the MSB first, and the 8-bit length. 
*              : 1 is set to the RSPCK signal as polarity when idle. The driving 
*              : of valid data commenses at the falling edge, and the data is 
*              : sampled at the rising edge. 0 is set to the SSL signal as active.
*              : The RSPI setting is executed to set the transmit buffer empty 
*              : flag when the data in the transmit buffer has 0 byte or smaller, 
*              : or the receive buffer full flag is set when the data in the 
*              : receive buffer has 24 bytes or more.
*              : The INTC setting is also executed to use the transmit buffer
*              : empty interrupt and the receive buffer full interrupt. 
*              : In this function, the RSPI is set to disable the transmission 
*              : interrupt and the reception interrupt.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_Init(void)
{
    volatile uint8_t dummy_buf;

    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the RSPI(channel 4) ---- */
    RZA_IO_RegWrite_8(&CPG.STBCR10, 0, CPG_STBCR10_MSTP103_SHIFT, CPG_STBCR10_MSTP103);
    dummy_buf = CPG.STBCR10;    /* Dummy read */

    /* ==== Set PORT functions ==== */
    Userdef_RSPI4_Port_Init();

    /* ==== RSPI initial setting ==== */
    /* SPCR - Control register
    b6 SPE - Function enable (0: disabled) */
    RSPI4.SPCR   = 0x00u;

    /* SPPCR - Pin control register
    b5 MOIFE - MOSI idle value fixing enable : last output value from previous transfer
    b4 MOIFV - MOSI idle fixed value         : Fixed to 1 when idle
    b0 SPLP  - Loop back                     : Do not loop back */
    RSPI4.SPPCR  = 0x20u;

    /* SPBR - Bit rate register
    b7:b0 SPR - Bit rate : RSPI_SPR */
    RSPI4.SPBR   = RSPI_SPR;

    /* SPDCR - Data control register
    b7    TXDMY - Dummy data transmission enabled : Dummy data transmission disabled
    b6:b5 SPLW  - Access width                    : Word access to the SPDR register */
    RSPI4.SPDCR  = 0x40u;

    /* SPCKD - Clock delay register
    b2:b0 SCKDL - RSPCK delay : 1RSPCK */
    RSPI4.SPCKD  = 0x00u;

    /* SSLND - Slave select negation delay register
    b2:b0 SLNDL - SSL negation delay : 1RSPCK */
    RSPI4.SSLND  = 0x00u;

    /* SPND - Next-access delay register
    b2:b0 SPNDL - Next access delay 1RSPCK + 2P1 clock */
    RSPI4.SPND   = 0x00u;

    /* SPSCR - Sequence control register
    b1:b0 SPSLN - Sequence length : Referenced SPCMD register 0=>0=>... */
    RSPI4.SPSCR  = 0x00u;

    /* SPCMD0 - Command register 0
    b15     SCKDEN - RSPCK delay setting enable        : Use SPCKD setting value
    b14     SLNDEN - SSL negation delay setting enable : Use SSLND setting value
    b13     SPNDEN - Next-access delay enable          : Use SPND setting value
    b12     LSBF   - LSB first                         : Change to MSB first
    b11:b8  SPB    - Data length                       : 16 bits
    b7      SSLKP  - SSL signal level Negates
                        : Negates the SSL signal upon completion of transfer
    b3:b2   BRDV   - Bit rate division setting         : RSPI_BRDV
    b1      CPOL   - RSPCK polarity setting            : RSPCK=0 when idle
    b0      CPHA   - RSPCK phase setting
                        : Data sampling on odd edge, data variation on even edge */
    RSPI4.SPCMD0 = 0xEF00u | (RSPI_BRDV << 2);

    /* SPBFCR - Buffer control register
    b7    TXRST - Transmit buffer data reset             : Enable reset operation
    b6    RXRST - Receive buffer data reset              : Enable reset operation
    b5:b4 TXTRG - Transmit buffer data triggering number : 0 byte or less
    b2:b0 RXTRG - Receive buffer data triggering number  : 0 bytes or more */
    RSPI4.SPBFCR = 0xF0u;

    /* SPBFCR - Buffer control register
    b7    TXRST - Transmit buffer data reset : Disable reset operation
    b6    RXRST - Receive buffer data reset  : Disable reset operation */
    RSPI4.SPBFCR &= ~0xC0u;

    /* SSLP - Slave select polarity register
    b0 SSL0P - SSL signal polarity : 0 active */
    RSPI4.SSLP   = 0x00u;

    /* SPCR - Control register
    b7 SPRIE  - Receive interrupt enable           : Disabled
    b6 SPE    - Function enable                    : Disabled
    b5 SPTIE  - Transmit interrupt enable          : Disabled
    b4 SPEIE  - Error interrupt enable             : Disabled
    b3 MSTR   - Master/slave mode select           : Master mode
    b2 MODFEN - Mode fault error detection enable  : Disabled */
    RSPI4.SPCR   = 0x08u;

    /* ==== Interrupt setting ==== */
    //R_INTC_RegistIntFunc(INTC_ID_SPTI4, Sample_RSPI_Spti4_Interrupt);   /* Register the SPTIn interrupt function */

    //R_INTC_SetPriority(INTC_ID_SPTI4, 10);  /* SPTIn interrupt priority= 10 is specified */

    //R_INTC_Enable(INTC_ID_SPTI4);   /* SPTIn interrupt enabled */

}

/******************************************************************************
* Function Name: Userdef_RSPI0_InitReceiveFull
* Description  : Execute this function to initialize the RSPI channel 0 receive
*              : buffer full notification information when the RSPI channel 0 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_InitReceiveFull(void)
{
    /* Add a processing when the RSPI0 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI1_InitReceiveFull
* Description  : Execute this function to initialize the RSPI channel 1 receive
*              : buffer full notification information when the RSPI channel 1 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_InitReceiveFull(void)
{
    /* Add a processing when the RSPI1 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI2_InitReceiveFull
* Description  : Execute this function to initialize the RSPI channel 2 receive
*              : buffer full notification information when the RSPI channel 2 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_InitReceiveFull(void)
{
    /* Add a processing when the RSPI2 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI3_InitReceiveFull
* Description  : Execute this function to initialize the RSPI channel 3 receive
*              : buffer full notification information when the RSPI channel 3 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_InitReceiveFull(void)
{
    /* Add a processing when the RSPI3 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI4_InitReceiveFull
* Description  : Execute this function to initialize the RSPI channel 4 receive
*              : buffer full notification information when the RSPI channel 4 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_InitReceiveFull(void)
{
    /* Add a processing when the RSPI4 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI0_InitTransmitEmpty
* Description  : Execute this function to initialize the RSPI channel 0 transmit
*              : buffer empty notification information when the RSPI channel 0 is
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_InitTransmitEmpty(void)
{
    /* Add a processing when the RSPI0 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI1_InitTransmitEmpty
* Description  : Execute this function to initialize the RSPI channel 1 transmit
*              : buffer empty notification information when the RSPI channel 1 is
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_InitTransmitEmpty(void)
{
    /* Add a processing when the RSPI1 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI2_InitTransmitEmpty
* Description  : Execute this function to initialize the RSPI channel 2 transmit
*              : buffer empty notification information when the RSPI channel 2 is
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_InitTransmitEmpty(void)
{
    /* Add a processing when the RSPI2 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI3_InitTransmitEmpty
* Description  : Execute this function to initialize the RSPI channel 3 transmit
*              : buffer empty notification information when the RSPI channel 3 is
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_InitTransmitEmpty(void)
{
    /* Add a processing when the RSPI3 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI4_InitTransmitEmpty
* Description  : Execute this function to initialize the RSPI channel 4 transmit
*              : buffer empty notification information.
*              : In the sample code, 0 is set to the software flag to notify
*              : that the RSPI channel 4 transmit buffer empty interrupt has 
*              : bee generated.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_InitTransmitEmpty(void)
{
    rspi_transmit_empty_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: Userdef_RSPI0_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 0 receive buffer full notification information when 
*              : the RSPI channel 0 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_SetReceiveFull(void)
{
    /* Add a processing when the RSPI0 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI1_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 1 receive buffer full notification information when 
*              : the RSPI channel 1 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_SetReceiveFull(void)
{
    /* Add a processing when the RSPI1 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI2_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 2 receive buffer full notification information when 
*              : the RSPI channel 2 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_SetReceiveFull(void)
{
    /* Add a processing when the RSPI2 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI3_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 3 receive buffer full notification information when 
*              : the RSPI channel 3 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_SetReceiveFull(void)
{
    /* Add a processing when the RSPI3 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI4_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 4 receive buffer full notification information when 
*              : the RSPI channel 4 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_SetReceiveFull(void)
{
    /* Add a processing when the RSPI4 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI0_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 0 transmit buffer empty notification information when 
*              : the RSPI channel 0 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_SetTransmitEmpty(void)
{
    /* Add a processing when the RSPI0 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI1_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 1 transmit buffer empty notification information when 
*              : the RSPI channel 1 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_SetTransmitEmpty(void)
{
    /* Add a processing when the RSPI1 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI2_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 2 transmit buffer empty notification information when 
*              : the RSPI channel 2 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_SetTransmitEmpty(void)
{
    /* Add a processing when the RSPI2 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI3_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 3 transmit buffer empty notification information when 
*              : the RSPI channel 3 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_SetTransmitEmpty(void)
{
    /* Add a processing when the RSPI3 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI4_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 4 transmit buffer empty notification information.
*              : In the sample code, 1 is set to the software flag to notify 
*              : that the RSPI channel 4 transmit buffer empty interrupt has
*              : been generated.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_SetTransmitEmpty(void)
{
    /* Disable the transmit interrupt request to be issued */
    RZA_IO_RegWrite_8(&RSPI4.SPCR, 0, RSPIn_SPCR_SPTIE_SHIFT, RSPIn_SPCR_SPTIE);

    rspi_transmit_empty_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: Userdef_RSPI0_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 0 receive buffer full notification information are 
*              : satisfied when the RSPI channel 0 is used.
* Arguments    : uint32_t byte       : Byte count of dummy data to be transmitted
*              : uint32_t less_rxtrg : Setting for operation
*              :                     :   0 : Normal operation 
*              :                     :     : (Wait until the conditions for the  
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied.)
*              :                     :   1 : Wait until the conditions for the
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied, but the
*              :                     :     : conditions are determined to be
*              :                     :     : satisfied when the data is received
*              :                     :     : for the bytes specified by the argument
*              :                     :     : byte.
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_WaitReceiveFull(uint32_t byte, uint32_t less_rxtrg)
{
    /* Add a processing when the RSPI0 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI1_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 1 receive buffer full notification information are 
*              : satisfied when the RSPI channel 1 is used.
* Arguments    : uint32_t byte       : Byte count of dummy data to be transmitted
*              : uint32_t less_rxtrg : Setting for operation
*              :                     :   0 : Normal operation 
*              :                     :     : (Wait until the conditions for the  
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied.)
*              :                     :   1 : Wait until the conditions for the
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied, but the
*              :                     :     : conditions are determined to be
*              :                     :     : satisfied when the data is received
*              :                     :     : for the bytes specified by the argument
*              :                     :     : byte.
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_WaitReceiveFull(uint32_t byte, uint32_t less_rxtrg)
{
    /* Add a processing when the RSPI1 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI2_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 2 receive buffer full notification information are 
*              : satisfied when the RSPI channel 2 is used.
* Arguments    : uint32_t byte       : Byte count of dummy data to be transmitted
*              : uint32_t less_rxtrg : Setting for operation
*              :                     :   0 : Normal operation 
*              :                     :     : (Wait until the conditions for the  
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied.)
*              :                     :   1 : Wait until the conditions for the
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied, but the
*              :                     :     : conditions are determined to be
*              :                     :     : satisfied when the data is received
*              :                     :     : for the bytes specified by the argument
*              :                     :     : byte.
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_WaitReceiveFull(uint32_t byte, uint32_t less_rxtrg)
{
    /* Add a processing when the RSPI2 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI3_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 3 receive buffer full notification information are 
*              : satisfied when the RSPI channel 3 is used.
* Arguments    : uint32_t byte       : Byte count of dummy data to be transmitted
*              : uint32_t less_rxtrg : Setting for operation
*              :                     :   0 : Normal operation 
*              :                     :     : (Wait until the conditions for the  
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied.)
*              :                     :   1 : Wait until the conditions for the
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied, but the
*              :                     :     : conditions are determined to be
*              :                     :     : satisfied when the data is received
*              :                     :     : for the bytes specified by the argument
*              :                     :     : byte.
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_WaitReceiveFull(uint32_t byte, uint32_t less_rxtrg)
{
    /* Add a processing when the RSPI3 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI4_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 4 receive buffer full notification information are 
*              : satisfied when the RSPI channel 4 is used.
* Arguments    : uint32_t byte       : Byte count of dummy data to be transmitted
*              : uint32_t less_rxtrg : Setting for operation
*              :                     :   0 : Normal operation 
*              :                     :     : (Wait until the conditions for the  
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied.)
*              :                     :   1 : Wait until the conditions for the
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied, but the
*              :                     :     : conditions are determined to be
*              :                     :     : satisfied when the data is received
*              :                     :     : for the bytes specified by the argument
*              :                     :     : byte.
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_WaitReceiveFull(uint32_t byte, uint32_t less_rxtrg)
{
    /* Add a processing when the RSPI4 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI0_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 0 transmit buffer empty notification information are
*              : satisfied when the RSPI channel 0 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_WaitTransmitEmpty(void)
{
    /* Add a processing when the RSPI0 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI1_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 1 transmit buffer empty notification information are
*              : satisfied when the RSPI channel 1 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_WaitTransmitEmpty(void)
{
    /* Add a processing when the RSPI1 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI2_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 2 transmit buffer empty notification information are
*              : satisfied when the RSPI channel 2 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_WaitTransmitEmpty(void)
{
    /* Add a processing when the RSPI2 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI3_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 3 transmit buffer empty notification information are
*              : satisfied when the RSPI channel 3 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_WaitTransmitEmpty(void)
{
    /* Add a processing when the RSPI3 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI4_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the 
*              : RSPI channel 4 transmit buffer empty notification information 
*              : are satisfied.
*              : In the sample code, this function enables the transfer buffer 
*              : empty interrupt and waits until 1 is set to the software flag
*              : to notify that the interrupt has been generated.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_WaitTransmitEmpty(void)
{
    /* ==== Enable the transmit interrupt request to be issued ==== */
    RZA_IO_RegWrite_8(&RSPI4.SPCR, 1, RSPIn_SPCR_SPTIE_SHIFT, RSPIn_SPCR_SPTIE);

    while (DEVDRV_FLAG_OFF == rspi_transmit_empty_flg)
    {
        /* Wait */
    }
}

/******************************************************************************
* Function Name: Userdef_RSPI0_WaitTransmitEnd
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 0 transmit end are satisfied when the RSPI channel 0 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_WaitTransmitEnd(void)
{
    /* Add a processing when the RSPI0 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI1_WaitTransmitEnd
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 1 transmit end are satisfied when the RSPI channel 1 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_WaitTransmitEnd(void)
{
    /* Add a processing when the RSPI1 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI2_WaitTransmitEnd
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 2 transmit end are satisfied when the RSPI channel 2 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_WaitTransmitEnd(void)
{
    /* Add a processing when the RSPI2 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI3_WaitTransmitEnd
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 3 transmit end are satisfied when the RSPI channel 3 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_WaitTransmitEnd(void)
{
    /* Add a processing when the RSPI3 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI4_WaitTransmitEnd
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 4 transmit end are satisfied.
*              : In the sample code, this function waits until 1 is set to the
*              : TEND bit of the status register (SPSR).
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_WaitTransmitEnd(void)
{
    while (0 == RZA_IO_RegRead_8(&RSPI4.SPSR, RSPIn_SPSR_TEND_SHIFT, RSPIn_SPSR_TEND))
    {
        /* Wait */
    }
}

/******************************************************************************
* Function Name: Userdef_RSPI0_DelayNextAccess
* Description  : When the RSPI channel 0 is used, execute this function to 
*              : secure the period which is required to start the next access 
*              : after the latest transfer ends. 
*              : RSPI channel 0 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_DelayNextAccess(void)
{
    /* Add a processing when the RSPI0 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI1_DelayNextAccess
* Description  : When the RSPI channel 1 is used, execute this function to 
*              : secure the period which is required to start the next access 
*              : after the latest transfer ends. 
*              : RSPI channel 1 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_DelayNextAccess(void)
{
    /* Add a processing when the RSPI1 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI2_DelayNextAccess
* Description  : When the RSPI channel 2 is used, execute this function to 
*              : secure the period which is required to start the next access 
*              : after the latest transfer ends. 
*              : RSPI channel 2 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_DelayNextAccess(void)
{
    /* Add a processing when the RSPI2 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI3_DelayNextAccess
* Description  : When the RSPI channel 3 is used, execute this function to 
*              : secure the period which is required to start the next access 
*              : after the latest transfer ends. 
*              : RSPI channel 3 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_DelayNextAccess(void)
{
    /* Add a processing when the RSPI3 is used */
}

/******************************************************************************
* Function Name: Userdef_RSPI4_DelayNextAccess
* Description  : When the RSPI channel 4 is used, execute this function to 
*              : secure the period which is required to start the next access 
*              : after the latest transfer ends. 
*              : In the sample code, this function secures the SSL signal 
*              : negation period (40ns or more) required for the Codec by 
*              : using the software wait processing.
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_DelayNextAccess(void)
{
    volatile int32_t cnt;

    cnt = 10;
    while (cnt-- > 0)
    {
        /* Delay */
    }
}

/* End of File */

