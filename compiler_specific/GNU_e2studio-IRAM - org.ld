OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(start)

/* Base Address RAM Memory Table 10-Mbyte on-chip RAM */  
/* Total RAM available 10112K */
/* Below is original   */
/* MEMORY {						 */
/* 	RAM0L (rwx)   : ORIGIN = 0x20020000, LENGTH = 0x000E0000   PAge 0 Lower bank ( 896KB) H'20020000 to H'200FFFFF */
/* 	RAM1L (rwx)   : ORIGIN = 0x20200000, LENGTH = 0x00500000   Page 1 Upper bank (4096KB) H'20100000 to H'204FFFFF */
/* 	VRAM  (rwx)   : ORIGIN = 0x60600000, LENGTH = 0x00400000   VRAM: Frame buffer for LCD panel (4MB) */
/* }  */
 
/*Below is modified by RETW-General */
MEMORY {
	RAM0L (rx)   : ORIGIN = 0x20020000, LENGTH = 0x002E0000  /* PAge 0 Lower bank ( 3MB) H'20020000 to H'202FFFFF 	*/
	RAM1L (rw)   : ORIGIN = 0x20300000, LENGTH = 0x00300000  /* Page 1 Upper bank ( 3MB) H'20500000 to H'207FFFFF 	*/
	VRAM  (rw)   : ORIGIN = 0x60600000, LENGTH = 0x00400000  /* VRAM: Frame buffer for LCD panel (4MB) 						*/
}

PROGRAM_STACK_SIZE    = 0x8000; 	/* Application stack                  */
IRQ_STACK_SIZE	      = 0x2000;     /* IRQ mode stack                     */
FIQ_STACK_SIZE	      = 0x2000;     /* FRQ mode stack                     */
SVC_STACK_SIZE	      = 0x2000;     /* SVC mode stack                     */
ABT_STACK_SIZE	      = 0x2000;     /* ABT mode stack                     */
TTB_SIZE              = 0x8000;     /* Level-1 Translation Table for MMU  */
HEAP_SIZE		      = 0x00100000; /* heap								  */
	 
VECTOR_TABLE          = 0x20020000; /* VECTOR_TABLE located here */ /* should be 0 for RAM based code */ 	 
	 
SECTIONS
{
	.reset VECTOR_TABLE :   
	{
		PROVIDE(execute = .); 
			
/*		execute = .;							*/

		*start.o (.text)
		.	= ALIGN(0x4);
		*reset_handler.o (.text)
		.	= ALIGN(0x4);
		
	} >RAM0L
 
	.CODE_RESET :
	{
		_sCODE_RESET = .;
	    *(.CODE_RESET)
		_eCODE_RESET = .;
	} >RAM0L 
 
	.DATA_RESET :
	{
		_sDATA_RESET = .;
	    *(.DATA_RESET)
		_eDATA_RESET = .;
	} >RAM0L 

	.BSS_RESET : 
	{
		.	= ALIGN(0x4);
  		PROVIDE (__start_BSS_RESET = .);
		*(.BSS_RESET)
  		PROVIDE (__stop_BSS_RESET = .);
		.	= ALIGN(0x4);
	} >RAM0L
	
	.irq_stack 0x20050000 :					/* original is C0000, try D0000 is OK  */
	{
		irq_stack_start = .;
		.  += IRQ_STACK_SIZE;
		.	= ALIGN(0x4);
		irq_stack_end = .;
	} >RAM0L
		
	.fiq_stack :
	{
		fiq_stack_start = .;
		.  += FIQ_STACK_SIZE;
		.	= ALIGN(0x4);
		fiq_stack_end = .;
	} >RAM0L
		
	.svc_stack :
	{
		PROVIDE(svc_stack_start = .);		
		.  += SVC_STACK_SIZE;
		.	= ALIGN(0x4);
		svc_stack_end = .;
	} >RAM0L
		
	.abt_stack :
	{
		abt_stack_start = .;
		.  += ABT_STACK_SIZE;
		.	= ALIGN(0x4);
		abt_stack_end = .;
	} >RAM0L

	.ttb_mmu1 :
	{
		ttb_mmu1_base = .;
		.  += TTB_SIZE;
		.	= ALIGN(0x4);
		ttb_mmu1_end = .;
	} >RAM0L
 
	.text :
	{
		text_start = .;
		*(.text)
		address_end_text = .;
	} > RAM0L
 
	.rodata : 
	{
		_srodata = .;
		*(.rodata)
		*(.rodata.*)
		.	= ALIGN(0x4);
		_erodata = .;
	} >RAM0L

	.data :
	{
	    *(.data)
		_data = .;
		_edata = .;
 
	} >RAM0L
	
	.text1 0x20100000:
	{
		text1_start = .;
		*(.text1)
		address_end_text1 = .;
	} > RAM0L
 
/* 	.data1 address_end_data_ROM :																			*/
/*	{																																	*/
/*		address_start_data_RAM = .;																			*/
/*		.  += (address_end_data_ROM - address_start_data_ROM);					*/
/*		address_end_data_RAM = .;																				*/														 
/*    } > RAM0L																												*/			 


	
	.bss :
	{
         _bss = .;
         PROVIDE(__bss_start__ = .);         
         *(.bss)
/*         *(.bss.**)					*/
/*         *(COMMON)					*/
         PROVIDE(__bss_end__ = .);
         _ebss = .;
         _end = .;
         PROVIDE(end = .);
		 .	= ALIGN(0x4);
         . += HEAP_SIZE;
         _heap_end = .;

		program_stack_start = .;
		.  += PROGRAM_STACK_SIZE;
		.	= ALIGN(0x4);
		program_stack_end = .; 

      } > RAM1L
      
	VRAM_SECTION 0x60600000 (NOLOAD) : {
		* (FRAME_MEM_BUF)
		
	} > VRAM

}