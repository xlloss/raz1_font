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
* File Name   : siochar.c
* $Rev: 2582 $
* $Date:: 2014-10-21 16:47:06 +0100#$
* Description : Serial I/O character R/W (SCIF 2-ch process)
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_scif_uart.h"       /* UART Driver header */
#include "sio_char.h"
#include "iodefine.h"
#include "rza_io_regrw.h"
#include "intc.h"
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


/******************************************************************************
* Function Name: int_rxi_2
* Description  : This function handles SCIF2 receive interrupts.
* Arguments    : none
* Return Value : none
******************************************************************************/
void int_rxi_2 (uint32_t sense)
{
 
    /* Disable SCIF2 receive interrupts */
    R_INTC_Disable(INTC_ID_RXI2);
 
    /* Add code here */
		Tick_KeyScanUART2();
 
    /* Enable SCIF2 receive interrupts */
    R_INTC_Enable(INTC_ID_RXI2);
 
}

void int_rxi_1 (uint32_t sense)
{
    /* Disable SCIF2 receive interrupts */
    R_INTC_Disable(INTC_ID_RXI1);

//		LedOn();
    /* Add code here */
		Tick_KeyScanUART1();
// 		LedOff();

    /* Enable SCIF2 receive interrupts */
    R_INTC_Enable(INTC_ID_RXI1);
}

void int_rxi_0 (uint32_t sense)
{
 
    /* Disable SCIF2 receive interrupts */
    R_INTC_Disable(INTC_ID_RXI0);
    
    /* Add code here */
		Tick_KeyScanUART0();

    /* Enable SCIF2 receive interrupts */
    R_INTC_Enable(INTC_ID_RXI0);
 
}
/******************************************************************************
* Function Name: IoInitScif2
* Description  : This function initializes SCIF channel 2 as UART mode.
*              : The transmit and the receive of SCIF channel 2 are enabled.
* Arguments    : none
* Return Value : none
******************************************************************************/
void IoInitScif2(void)
{
    /* === Initialization of SCIF2 ==== */
    /* P1 clock=66.67MHz CKS=0 SCBRR=17 Bit rate error=0.46% => Baud rate=115200bps */
    R_SCIF_UART_Init(DEVDRV_CH_2, SCIF_UART_MODE_RW, SCIF_CKS_DIVISION_1, 17);
    R_INTC_Disable(INTC_ID_RXI2);
    
    /* === Initialization of PORT function ==== */
    /* ---- P3_0 : TxD2 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 6th multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE3, 1, GPIO_PFCAE3_PFCAE30_SHIFT, GPIO_PFCAE3_PFCAE30);
    rza_io_reg_write_16(&GPIO.PFCE3,  0, GPIO_PFCE3_PFCE30_SHIFT,   GPIO_PFCE3_PFCE30);
    rza_io_reg_write_16(&GPIO.PFC3,   1, GPIO_PFC3_PFC30_SHIFT,     GPIO_PFC3_PFC30);
    rza_io_reg_write_16(&GPIO.PMC3,   1, GPIO_PMC3_PMC30_SHIFT,     GPIO_PMC3_PMC30);
    rza_io_reg_write_16(&GPIO.PIPC3,  1, GPIO_PIPC3_PIPC30_SHIFT,   GPIO_PIPC3_PIPC30);

    /* ---- P3_2 : RxD2 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 4th multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE3, 0, GPIO_PFCAE3_PFCAE32_SHIFT, GPIO_PFCAE3_PFCAE32);
    rza_io_reg_write_16(&GPIO.PFCE3,  1, GPIO_PFCE3_PFCE32_SHIFT,   GPIO_PFCE3_PFCE32);
    rza_io_reg_write_16(&GPIO.PFC3,   1, GPIO_PFC3_PFC32_SHIFT,     GPIO_PFC3_PFC32);
    rza_io_reg_write_16(&GPIO.PMC3,   1, GPIO_PMC3_PMC32_SHIFT,     GPIO_PMC3_PMC32);
    rza_io_reg_write_16(&GPIO.PIPC3,  1, GPIO_PIPC3_PIPC32_SHIFT,   GPIO_PIPC3_PIPC32);

    R_INTC_RegistIntFunc(INTC_ID_RXI2, &int_rxi_2);
    R_INTC_SetPriority(INTC_ID_RXI2, 12uL);
    R_INTC_Enable(INTC_ID_RXI2);
    
    /* === Enable SCIF2 transmission/reception ==== */
    /* ---- Serial control register (SCSCRi) setting ---- */
    SCIF2.SCSCR = 0x0070u;				//0x0020
		/* TIE,RIE,TE,RX,	REIEx,CK1,CK0  */						
		/* SCIF2 transmitting and receiving operations are enabled */
}
void IoInitScif1(void)
{
    /* === Initialization of SCIF1 ==== */
    /* P1 clock=66.67MHz CKS=0 SCBRR=17 Bit rate error=0.46% => Baud rate=115200bps, 66666000/32/18 = 115739,  66666000/32/4/108 = 4800  */		
    R_SCIF_UART_Init(DEVDRV_CH_1, SCIF_UART_MODE_RW, SCIF_CKS_DIVISION_4, 108-1);
    R_INTC_Disable(INTC_ID_RXI1);
    
  #if  1
    /* === Initialization of PORT function ==== */
    /* ---- P4_12 : TxD1 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 6th multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC412_SHIFT,     GPIO_PMC4_PMC412);
    rza_io_reg_write_16(&GPIO.PFCAE4, 1, GPIO_PFCAE4_PFCAE412_SHIFT, GPIO_PFCAE4_PFCAE412);
    rza_io_reg_write_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE412_SHIFT,   GPIO_PFCE4_PFCE412);
    rza_io_reg_write_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC412_SHIFT,     GPIO_PFC4_PFC412);
    rza_io_reg_write_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC412_SHIFT,   GPIO_PIPC4_PIPC412);
        rza_io_reg_write_16(&GPIO.PM4,    0, GPIO_PM4_PM412_SHIFT,       GPIO_PM4_PM412);				//output

    /* ---- P4_13 : RxD1 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 4th multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC413_SHIFT,     GPIO_PMC4_PMC413);
    rza_io_reg_write_16(&GPIO.PFCAE4, 1, GPIO_PFCAE4_PFCAE413_SHIFT, GPIO_PFCAE4_PFCAE413);
    rza_io_reg_write_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE413_SHIFT,   GPIO_PFCE4_PFCE413);
    rza_io_reg_write_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC413_SHIFT,     GPIO_PFC4_PFC413);
    rza_io_reg_write_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC413_SHIFT,   GPIO_PIPC4_PIPC413);
    rza_io_reg_write_16(&GPIO.PM4,    1, GPIO_PM4_PM413_SHIFT,       GPIO_PM4_PM413);						//input
        
	#endif
	
    R_INTC_RegistIntFunc(INTC_ID_RXI1, &int_rxi_1);
    R_INTC_SetPriority(INTC_ID_RXI1, 11uL);
		R_INTC_Enable(INTC_ID_RXI1);
    
    /* === Enable SCIF1 transmission/reception ==== */
    /* ---- Serial control register (SCSCRi) setting ---- */
    SCIF1.SCSCR = 0x0070u;
		/* SCIF1 transmitting and receiving operations are enabled */
		/* TIE,RIE,TE,RX,	REIEx,CK1,CK0  */
}

void IoInitScif0(void)
{
    /* === Initialization of SCIF2 ==== */
    /* P1 clock=66.67MHz CKS=0 SCBRR=17 Bit rate error=0.46% => Baud rate=115200bps */
    R_SCIF_UART_Init(DEVDRV_CH_0, SCIF_UART_MODE_RW, SCIF_CKS_DIVISION_1, 17);
    R_INTC_Disable(INTC_ID_RXI0);
    
  #if  0			//Cannot use now
    /* === Initialization of PORT function ==== */
    /* ---- P4_9 : TxD0 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 6th multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC412_SHIFT,     GPIO_PMC4_PMC412);
    rza_io_reg_write_16(&GPIO.PFCAE4, 1, GPIO_PFCAE4_PFCAE412_SHIFT, GPIO_PFCAE4_PFCAE412);
    rza_io_reg_write_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE412_SHIFT,   GPIO_PFCE4_PFCE412);
    rza_io_reg_write_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC412_SHIFT,     GPIO_PFC4_PFC412);
    rza_io_reg_write_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC412_SHIFT,   GPIO_PIPC4_PIPC412);
        rza_io_reg_write_16(&GPIO.PM4,    0, GPIO_PM4_PM412_SHIFT,       GPIO_PM4_PM412);				//output
	#endif
	#if 1
    /* ---- P4_10 : RxD0 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 4th multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC410_SHIFT,     GPIO_PMC4_PMC410);
    rza_io_reg_write_16(&GPIO.PFCAE4, 1, GPIO_PFCAE4_PFCAE410_SHIFT, GPIO_PFCAE4_PFCAE410);
    rza_io_reg_write_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE410_SHIFT,   GPIO_PFCE4_PFCE410);
    rza_io_reg_write_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC410_SHIFT,     GPIO_PFC4_PFC410);
    rza_io_reg_write_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC410_SHIFT,   GPIO_PIPC4_PIPC410);
        rza_io_reg_write_16(&GPIO.PM4,    1, GPIO_PM4_PM410_SHIFT,       GPIO_PM4_PM410);				//input
        
	#endif

    R_INTC_RegistIntFunc(INTC_ID_RXI0, &int_rxi_0);
    R_INTC_SetPriority(INTC_ID_RXI0, 12uL);
    R_INTC_Enable(INTC_ID_RXI0);
    
    /* === Enable SCIF2 transmission/reception ==== */
    /* ---- Serial control register (SCSCRi) setting ---- */
    SCIF0.SCSCR = 0x0070u;
		 /* SCIF2 transmitting and receiving operations are enabled */
}

/******************************************************************************
* Function Name: IoGetchar
* Description  : One character is received from SCIF2, and it's data is returned.
*              : This function keeps waiting until it can obtain the receiving data.
* Arguments    : none
* Return Value : Character to receive (Byte).
******************************************************************************/
char_t IoGetchar2(void)
{
    char_t data;

    /* Confirming receive error(ER,BRK,FER,PER) */
    if (SCIF2.SCFSR & 0x09Cu)
    {
        /* ---- Detect receive error ---- */
        /* Disable reception             */
        /* Reset receiving FIFO          */
        /* Clearing FIFO reception reset */
        /* Error bit clear               */
        /* Enable reception              */
        rza_io_reg_write_16(&SCIF2.SCSCR, 0, SCIF2_SCSCR_RE_SHIFT,    SCIF2_SCSCR_RE);
        rza_io_reg_write_16(&SCIF2.SCFCR, 1, SCIF2_SCFCR_RFRST_SHIFT, SCIF2_SCFCR_RFRST);
        rza_io_reg_write_16(&SCIF2.SCFCR, 0, SCIF2_SCFCR_RFRST_SHIFT, SCIF2_SCFCR_RFRST);
        SCIF2.SCFSR &= ~0x9Cu;
        rza_io_reg_write_16(&SCIF2.SCSCR, 1, SCIF2_SCSCR_RE_SHIFT,    SCIF2_SCSCR_RE);

        return 0;
    }

    /* Is there receive FIFO data? */
#ifdef KEY_SCAN_NO_WAIT
    if (0 == rza_io_reg_read_16(&SCIF2.SCFSR, SCIF2_SCFSR_RDF_SHIFT, SCIF2_SCFSR_RDF))
    {
        return -2;
    }
#else /* KEY_SCAN_NO_WAIT */
    while (0 == rza_io_reg_read_16(&SCIF2.SCFSR, SCIF2_SCFSR_RDF_SHIFT, SCIF2_SCFSR_RDF))
    {
        /* WAIT */
    }
#endif /* KEY_SCAN_NO_WAIT */
    /* Read receive data */
    data = SCIF2.SCFRDR;
    /* Clear RDF */
    rza_io_reg_write_16(&SCIF2.SCFSR, 0, SCIF2_SCFSR_RDF_SHIFT, SCIF2_SCFSR_RDF);

    /* Is it overflowed? */
    if (1 == rza_io_reg_read_16(&SCIF2.SCLSR, SCIF2_SCLSR_ORER_SHIFT, SCIF2_SCLSR_ORER))
    {
        /* ORER clear */
        rza_io_reg_write_16(&SCIF2.SCLSR, 0, SCIF2_SCLSR_ORER_SHIFT, SCIF2_SCLSR_ORER);
    }

    return data;
}

/******************************************************************************
* Function Name: IoPutchar
* Description  : Character "buffer" is output to SCIF2.
*              : This function keeps waiting until it becomes the transmission
*              : enabled state.
* Arguments    : char_t buffer : character to output
* Return Value : None
******************************************************************************/
void IoPutchar(char_t buffer)
{
    /* Check if it is possible to transmit (TDFE flag) */
    while (0 == rza_io_reg_read_16(&SCIF2.SCFSR, SCIF2_SCFSR_TDFE_SHIFT, SCIF2_SCFSR_TDFE))
    {
        /* Wait */
    }

    /* Write the receiving data in TDR */
    SCIF2.SCFTDR = buffer;

    /* Clear TDRE and TEND flag */
    SCIF2.SCFSR &= ~0x0060u;
}

void IoPutchar1(char_t buffer)
{
    /* Check if it is possible to transmit (TDFE flag) */
    while (0 == rza_io_reg_read_16(&SCIF1.SCFSR, SCIF1_SCFSR_TDFE_SHIFT, SCIF1_SCFSR_TDFE))
    {
        /* Wait */
    }
    /* Write the receiving data in TDR */
    SCIF1.SCFTDR = buffer;

    /* Clear TDRE and TEND flag */
    SCIF1.SCFSR &= ~0x0060u;
}

void IoPutchar0(char_t buffer)
{
    /* Check if it is possible to transmit (TDFE flag) */
    while (0 == rza_io_reg_read_16(&SCIF2.SCFSR, SCIF2_SCFSR_TDFE_SHIFT, SCIF2_SCFSR_TDFE))
    {
        /* Wait */
    }

    /* Write the receiving data in TDR */
    SCIF0.SCFTDR = buffer;

    /* Clear TDRE and TEND flag */
    SCIF0.SCFSR &= ~0x0060u;
}
 
/******************************************************************************
* Function Name: IoGetchar
* Description  : One character is received from SCIF2, and it's data is returned.
*              : This function keeps waiting until it can obtain the receiving data.
* Arguments    : none
* Return Value : Character to receive (Byte).
******************************************************************************/
char_t ScanIoGetchar2(void)
{
    char_t data;

    /* Confirming receive error(ER,BRK,FER,PER) */
    if (SCIF2.SCFSR & 0x09Cu)
    {
        /* ---- Detect receive error ---- */
        /* Disable reception             */
        /* Reset receiving FIFO          */
        /* Clearing FIFO reception reset */
        /* Error bit clear               */
        /* Enable reception              */
        rza_io_reg_write_16(&SCIF2.SCSCR, 0, SCIF2_SCSCR_RE_SHIFT,    SCIF2_SCSCR_RE);
        rza_io_reg_write_16(&SCIF2.SCFCR, 1, SCIF2_SCFCR_RFRST_SHIFT, SCIF2_SCFCR_RFRST);
        rza_io_reg_write_16(&SCIF2.SCFCR, 0, SCIF2_SCFCR_RFRST_SHIFT, SCIF2_SCFCR_RFRST);
        SCIF2.SCFSR &= ~0x9Cu;
        rza_io_reg_write_16(&SCIF2.SCSCR, 1, SCIF2_SCSCR_RE_SHIFT,    SCIF2_SCSCR_RE);

        return 0;
    }

    /* Is there receive FIFO data? */
    if (0 == rza_io_reg_read_16(&SCIF2.SCFSR, SCIF2_SCFSR_RDF_SHIFT, SCIF2_SCFSR_RDF))
    {
        return 0;
    }

    /* Read receive data */
    data = SCIF2.SCFRDR;
    /* Clear RDF */
    rza_io_reg_write_16(&SCIF2.SCFSR, 0, SCIF2_SCFSR_RDF_SHIFT, SCIF2_SCFSR_RDF);

    /* Is it overflowed? */
    if (1 == rza_io_reg_read_16(&SCIF2.SCLSR, SCIF2_SCLSR_ORER_SHIFT, SCIF2_SCLSR_ORER))
    {
        /* ORER clear */
        rza_io_reg_write_16(&SCIF2.SCLSR, 0, SCIF2_SCLSR_ORER_SHIFT, SCIF2_SCLSR_ORER);
    }
    return data;
}

char_t ScanIoGetchar1(void)
{
    char_t data;
    
    /* Confirming receive error(ER,BRK,FER,PER) */
    if (SCIF1.SCFSR & 0x09Cu)
    {
 
        /* ---- Detect receive error ---- */
        /* Disable reception             */
        /* Reset receiving FIFO          */
        /* Clearing FIFO reception reset */
        /* Error bit clear               */
        /* Enable reception              */
        rza_io_reg_write_16(&SCIF1.SCSCR, 0, SCIF1_SCSCR_RE_SHIFT,    SCIF1_SCSCR_RE);
        rza_io_reg_write_16(&SCIF1.SCFCR, 1, SCIF1_SCFCR_RFRST_SHIFT, SCIF1_SCFCR_RFRST);
        rza_io_reg_write_16(&SCIF1.SCFCR, 0, SCIF1_SCFCR_RFRST_SHIFT, SCIF1_SCFCR_RFRST);
        SCIF1.SCFSR &= ~0x9Cu;
        rza_io_reg_write_16(&SCIF1.SCSCR, 1, SCIF1_SCSCR_RE_SHIFT,    SCIF1_SCSCR_RE);
 
        return 0;
    }

    /* Is there receive FIFO data? */
    if (0 == rza_io_reg_read_16(&SCIF1.SCFSR, SCIF1_SCFSR_RDF_SHIFT, SCIF1_SCFSR_RDF))
    {
        return 0;
    }

    /* Read receive data */
    data = SCIF1.SCFRDR;
    /* Clear RDF */
    rza_io_reg_write_16(&SCIF1.SCFSR, 0, SCIF1_SCFSR_RDF_SHIFT, SCIF1_SCFSR_RDF);

    /* Is it overflowed? */
    if (1 == rza_io_reg_read_16(&SCIF1.SCLSR, SCIF1_SCLSR_ORER_SHIFT, SCIF1_SCLSR_ORER))
    {
        /* ORER clear */
        rza_io_reg_write_16(&SCIF1.SCLSR, 0, SCIF1_SCLSR_ORER_SHIFT, SCIF1_SCLSR_ORER);
    }
    return data;
}


char_t ScanIoGetchar0(void)
{
    char_t data;

    /* Confirming receive error(ER,BRK,FER,PER) */
    if (SCIF0.SCFSR & 0x09Cu)
    {
        /* ---- Detect receive error ---- */
        /* Disable reception             */
        /* Reset receiving FIFO          */
        /* Clearing FIFO reception reset */
        /* Error bit clear               */
        /* Enable reception              */
        rza_io_reg_write_16(&SCIF0.SCSCR, 0, SCIF0_SCSCR_RE_SHIFT,    SCIF0_SCSCR_RE);
        rza_io_reg_write_16(&SCIF0.SCFCR, 1, SCIF0_SCFCR_RFRST_SHIFT, SCIF0_SCFCR_RFRST);
        rza_io_reg_write_16(&SCIF0.SCFCR, 0, SCIF0_SCFCR_RFRST_SHIFT, SCIF0_SCFCR_RFRST);
        SCIF1.SCFSR &= ~0x9Cu;
        rza_io_reg_write_16(&SCIF0.SCSCR, 1, SCIF0_SCSCR_RE_SHIFT,    SCIF0_SCSCR_RE);

        return 0;
    }

    /* Is there receive FIFO data? */
    if (0 == rza_io_reg_read_16(&SCIF0.SCFSR, SCIF0_SCFSR_RDF_SHIFT, SCIF0_SCFSR_RDF))
    {
        return 0;
    }

    /* Read receive data */
    data = SCIF0.SCFRDR;
    /* Clear RDF */
    rza_io_reg_write_16(&SCIF0.SCFSR, 0, SCIF0_SCFSR_RDF_SHIFT, SCIF0_SCFSR_RDF);

    /* Is it overflowed? */
    if (1 == rza_io_reg_read_16(&SCIF0.SCLSR, SCIF0_SCLSR_ORER_SHIFT, SCIF0_SCLSR_ORER))
    {
        /* ORER clear */
        rza_io_reg_write_16(&SCIF0.SCLSR, 0, SCIF0_SCLSR_ORER_SHIFT, SCIF0_SCLSR_ORER);
    }
    return data;
}

/* End of File */

