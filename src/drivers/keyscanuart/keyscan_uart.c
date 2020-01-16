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
* File Name   : scif_uart.c
* $Rev: 2444 $
* $Date:: 2014-10-14 21:15:39 +0100#$
* Description : SCIF UART driver
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "devdrv_scif_uart.h"       /* UART Driver header */
#include "intc.h"        

#define success				1
#define fail					0

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== UART H/W specification ==== */
#define SCIF_UART_CH_TOTAL      (8)     /* The number of UART channels */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* ====  Prototype function ==== */

/* ==== Global variable ==== */

#define			MaxSCIFsize					128
unsigned char Rxd_buffer[MaxSCIFsize];
unsigned char Txd_buffer[MaxSCIFsize];
unsigned int Rxd_w_ptr;
unsigned int Rxd_r_ptr;
unsigned int Txd_w_ptr;
unsigned int Txd_r_ptr;
 
unsigned char Rxd_buffer1[MaxSCIFsize];
unsigned char Txd_buffer1[MaxSCIFsize];
unsigned int Rxd_w_ptr1;
unsigned int Rxd_r_ptr1;
unsigned int Txd_w_ptr1;
unsigned int Txd_r_ptr1;
  
void Tick_KeyScanUART2()
{
				unsigned char RDRTEMP;
				unsigned int Ptr_temp;			
					
				RDRTEMP = ScanIoGetchar2();
				
				if(RDRTEMP==0)
				return 0;
	
        Ptr_temp = Rxd_w_ptr ;
        Rxd_w_ptr ++ ;
        Rxd_w_ptr &= MaxSCIFsize-1;
     
        if (Rxd_w_ptr == Rxd_r_ptr)
           {
            printf("Rxd2 buffer full!\n");
            Rxd_w_ptr = Ptr_temp;
            return;
           }
        Rxd_buffer[Rxd_w_ptr] = RDRTEMP ;
				return;
}

void Tick_KeyScanUART1()
{
				unsigned char RDRTEMP;
				unsigned int Ptr_temp;			

				RDRTEMP = ScanIoGetchar1();
				if(RDRTEMP==0)
				{
				return 0;
				}
				
//        IoPutchar(RDRTEMP);
#if 0
dspbyte(RDRTEMP);
IoPutchar(0x0d);
IoPutchar(0x0a);
#endif

        Ptr_temp = Rxd_w_ptr1 ;
        Rxd_w_ptr1 ++ ;
        Rxd_w_ptr1 &= MaxSCIFsize-1;
     
        if (Rxd_w_ptr1 == Rxd_r_ptr1)
           {
            printf("Rxd1 buffer full!\n");
            Rxd_w_ptr1 = Ptr_temp;
            return;
           }
        Rxd_buffer1[Rxd_w_ptr1] = RDRTEMP ;
				return;
}

void Tick_KeyScanUART0()
{
				unsigned char RDRTEMP;
				unsigned int Ptr_temp;			
 
				RDRTEMP = ScanIoGetchar0();
 
				if(RDRTEMP==0)
				{
				return 0;
				}
	
        Ptr_temp = Rxd_w_ptr ;
        Rxd_w_ptr ++ ;
        Rxd_w_ptr &= MaxSCIFsize-1;
     
        if (Rxd_w_ptr == Rxd_r_ptr)
           {
            printf("Rxd1 buffer full!\n");
            Rxd_w_ptr = Ptr_temp;
            return;
           }
        Rxd_buffer[Rxd_w_ptr] = RDRTEMP ;
				return;
}


int GetTerminalKey()
{
				int rdrtemp;
        for(;;)
        {
                if ( Rxd_w_ptr != Rxd_r_ptr )
                {
                Rxd_r_ptr ++;
                Rxd_r_ptr &= (MaxSCIFsize-1);
                rdrtemp = Rxd_buffer[Rxd_r_ptr];
                return rdrtemp;
                }
        }
}
int ScanGetTerminalKey()
{
        int rdrtemp;
        if ( Rxd_w_ptr != Rxd_r_ptr )
        {
        Rxd_r_ptr ++;
        Rxd_r_ptr &= (MaxSCIFsize-1);
        rdrtemp = Rxd_buffer[Rxd_r_ptr];
        return rdrtemp;
        }
        return 0;
}
int ScanTerminalKeyStatus()
{
        if ( Rxd_w_ptr != Rxd_r_ptr )
        {
        return 1;
        }
        return 0;
}
 
int FlushTerminalKeyBuff_SCIF2()
{
	 Rxd_w_ptr =0;
	 Rxd_r_ptr =0;

}
int FlushTerminalKeyBuff_SCIF1()
{
	 Rxd_w_ptr1 =0;
	 Rxd_r_ptr1 =0;

}
	 
	 
/* For BLE comdule AT command InOut */
unsigned char RETW_SCIF1_WaitAndGetByte()
{
				unsigned char rdrtemp;
        for(;;)
        {
                if ( Rxd_w_ptr1 != Rxd_r_ptr1 )
                {
                Rxd_r_ptr1 ++;
                Rxd_r_ptr1 &= (MaxSCIFsize-1);
                rdrtemp = Rxd_buffer1[Rxd_r_ptr1];
                return rdrtemp;
                }
        }
}
unsigned char RETW_SCIF1_ScanGetByte()
{
				unsigned char rdrtemp;
        if ( Rxd_w_ptr1 != Rxd_r_ptr1 )
        {
        Rxd_r_ptr1 ++;
        Rxd_r_ptr1 &= (MaxSCIFsize-1);
        rdrtemp = Rxd_buffer1[Rxd_r_ptr1];
        return rdrtemp;
        }
        return fail;
}
unsigned char RETW_SCIF1_CheckByteIn()
{
        if ( Rxd_w_ptr1 != Rxd_r_ptr1 )
        {
        return success;
        }
        return fail;
}
