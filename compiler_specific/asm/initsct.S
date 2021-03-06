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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : initsct.S
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : This is the code to be executed on the target
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 18.06.2013 0.01
*******************************************************************************/

	.code 32
	.global initsct

	.func   initsct
initsct:
  
    LDR  r0, = address_start_data_RAM
    LDR  r1, = address_end_data_RAM
    LDR  r2, = address_start_data_ROM
copy_data:
    TEQ   r0,r1
    BEQ   RAMInitDone
    LDR   r3,[r2]
    STM   r0!,{r3}
    ADD   r2,r2,#4
    B     copy_data            
RAMInitDone: 

	LDR  r0, = __start_BSS_RESET
	LDR  r1, = __stop_BSS_RESET
clear_memory:
	TEQ   r0,r1
	BEQ  finished
	movs r3,#0
	stm  r0!,{r3}
	B    clear_memory
finished:
	LDR  r0, = __bss_start__
	LDR  r1, = __bss_end__
bss_clear_memory:
	TEQ   r0,r1
	BEQ  bss_finished
	movs r3,#0
	stm  r0!,{r3}
	B    bss_clear_memory
bss_finished:
	ISB
	BX	 lr
	.endfunc
	.end
