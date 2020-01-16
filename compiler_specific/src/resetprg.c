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
/*******************************************************************************
* File Name     : resetprg.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Reset program
*******************************************************************************/
#include "r_typedefs.h"
#include "devdrv_common.h"      /* Common Driver Header */
#include "intc.h"               /* INTC Driver Header   */
#include "resetprg.h"
#include "sio_char.h"
#include "stb_init.h"
#include "port_init.h"
#include "bsc_userdef.h"


#if defined(__thumb2__) || (defined(__thumb__) && defined(__ARM_ARCH_6M__))
# define THUMB_V7_V6M
#endif

/* Defined if this target supports the BLX Rm instruction.  */
#if  !defined(__ARM_ARCH_2__) && !defined(__ARM_ARCH_3__) && !defined(__ARM_ARCH_3M__)	&& !defined(__ARM_ARCH_4__)	&& !defined(__ARM_ARCH_4T__)
# define HAVE_CALL_INDIRECT
#endif

#ifdef HAVE_INITFINI_ARRAY
#define _init	__libc_init_array
#define _fini	__libc_fini_array
#endif

extern int __enable_irq(void);
extern int __enable_fiq(void);
extern int L1CacheInit(void);
extern int VbarInit(void);

char Banner[] = {"\nRZ/A1 R7S72100 CPU Board Sample Program (GNU build RAM Version).\n"};

/*******************************************************************************
* Function Name: $Sub$$main
* Description  :
* Arguments    : none
* Return Value : none
*******************************************************************************/
void PowerON_Reset (void)
{
    STB_Init();
 
//		boot_QSPIcode();
    
    PORT_Init();

    /* ==== BSC setting ==== */
    /* Note BSC_AREA_CS0 & BSC_AREA_CS1 initialised by Peripheral_BasicInit() */
    R_BSC_Init((uint8_t)(BSC_AREA_CS2));
 
    /* ==== INTC setting ==== */
    R_INTC_Init();

    /* ==== Initial setting of the level 1 cache ==== */
    L1CacheInit();

    /* ==== Vector base address setting ==== */
    VbarInit();
 
#if 0

    __disable_irq();
    __disable_fiq();
    
    printf("Start section copy\n"); 
    init_spibsc_init1_section();
    printf("Copy ROM to RAM Section done.\n");    
     
    spibsc_init1();
    /* The initial loader has run and the SPI interface is now at high speed */
    printf("QSPI high speed setting done.\n"); 
    
    spibsc_init2();
    printf("Dual QSPI high speed setting done.\n"); 
    printf("Jump to 0x18200000.\n"); 
    
    UserProgJmp((uint32_t )0x18200000);
    
    while(1) {}
#endif 

    __enable_irq();
    __enable_fiq();

    /* ==== Initialize Terminal ==== */
    /* SCIF 2ch */
    /* P1=66.67MHz CKS=0 SCBRR=17 Bit rate error=0.46% => Baud rate=115200bps */
    IoInitScif2();
//    IoInitScif1();
//		IoInitScif0();

//		printf("Bootloader start..\n");
//    boot_QSPIcode();

while(0){
    IoPutchar1(0x41);
    IoPutchar1(0x42);
    IoPutchar1(0x0d);
    IoPutchar1(0x0a);      
    
    IoPutchar1(0x61);
    IoPutchar1(0x62);
    IoPutchar1(0x0d);
    IoPutchar1(0x0a); 
}
 
	main();

	while(1); /* Stops program from running off */
}
 
void boot_QSPIcode(void)
{
	uint32_t i;
	uint32_t loop_num;
	int32_t  size;
    uint32_t * psrc;
    uint32_t * pdst;
    uint32_t * preg;
    uint32_t * pexe;
 
    /* Used to suppress the 'variable declared but not used' warning */
    (void)(psrc);
    (void)(pdst);
    (void)(preg);
    (void)(pexe);

    psrc = (uint32_t *)0x18200000;
    pdst = (uint32_t *)0x20020000;
    pexe = (uint32_t *)0x20020000;
 
    size = 0x300000/4;
 
        loop_num = size;
        for(i=0;i<loop_num;i++)
        {
            (*pdst++) = (*psrc++);
        }

    UserProgJmp((uint32_t )pexe);
}
