OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(start)

/*Below is modified by RETW-General */
MEMORY {
	ROM (rx)   				: ORIGIN = 0x20020000, LENGTH = 0x00160000  			/* PAge 0 Lower bank ( 3MB) H'20020000 to H'202FFFFF 		*/
	RAM1L (rw)   			: ORIGIN = 0x20180000, LENGTH = 0x00280000  			/* Page 1 Upper bank ( 3MB) H'20500000 to H'207FFFFF 		*/
 	VRAM  (rw)   			: ORIGIN = 0x20400000, LENGTH = 0x00600000  			/* VRAM: Frame buffer for LCD panel (4MB) 						  */
 	SDRAM (rw)   			: ORIGIN = 0x08000000, LENGTH = 0x01000000  			/* 16MB 						  																	*/
 	SDRAMNC  (rw)   	: ORIGIN = 0x49000000, LENGTH = 0x01000000  			/* 16MB 						  																	*/
}

PROGRAM_STACK_SIZE    = 0x8000; 	/* Application stack                  */
IRQ_STACK_SIZE	      = 0x2000;     /* IRQ mode stack                     */
FIQ_STACK_SIZE	      = 0x2000;     /* FRQ mode stack                     */
SVC_STACK_SIZE	      = 0x2000;     /* SVC mode stack                     */
ABT_STACK_SIZE	      = 0x2000;     /* ABT mode stack                     */
TTB_SIZE              = 0x8000;     /* Level-1 Translation Table for MMU  */
HEAP_SIZE		      		= 0x00080000; /* heap								  */
	 
VECTOR_TABLE          = 0x20020000;		 	/* VECTOR_TABLE located here */ /* should be 0 for RAM based code */ 	 
MMU_TABLE_ADDR        = 0x20200000; 

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
		
	} >ROM
 
	.CODE_RESET :
	{
		_sCODE_RESET = .;
	    *(.CODE_RESET)
		_eCODE_RESET = .;
	} >ROM 
 
	.DATA_RESET :
	{
		_sDATA_RESET = .;
	    *(.DATA_RESET)
		_eDATA_RESET = .;
	} >ROM 

	.BSS_RESET : 
	{
		.	= ALIGN(0x4);
  		PROVIDE (__start_BSS_RESET = .);
		*(.BSS_RESET)
  		PROVIDE (__stop_BSS_RESET = .);
		.	= ALIGN(0x4);
	} >ROM

	.text :
	{
		text_start = .;
		*(.text)
		address_end_text = .;
	} > ROM
 
	.rodata : 
	{
		_srodata = .;
		*(.rodata)
		*(.rodata.*)
		.	= ALIGN(0x4);
		_erodata = .;
	} > ROM

	.ttb_mmu1 : 		/*  0x20180000:		*/
	{
		ttb_mmu1_base = .;
		.  += TTB_SIZE;
		.	= ALIGN(0x4);
		ttb_mmu1_end = .;
	} >RAM1L
	
	.irq_stack :					/* This address must be boundary of 0x4000, in order to allocate ttb_mmu1_base in boundary of 0x4000 too*/
	{
		irq_stack_start = .;
		.  += IRQ_STACK_SIZE;
		.	= ALIGN(0x4);
		irq_stack_end = .;
	} >RAM1L
		
	.fiq_stack :
	{
		fiq_stack_start = .;
		.  += FIQ_STACK_SIZE;
		.	= ALIGN(0x4);
		fiq_stack_end = .;
	} >RAM1L
		
	.svc_stack :
	{
		PROVIDE(svc_stack_start = .);		
		.  += SVC_STACK_SIZE;
		.	= ALIGN(0x4);
		svc_stack_end = .;
	} >RAM1L
		
	.abt_stack :
	{
		abt_stack_start = .;
		.  += ABT_STACK_SIZE;
		.	= ALIGN(0x4);
		abt_stack_end = .;
	} >RAM1L

	.data :
	{
				address_start_data_RAM = .;
        address_start_data_ROM = LOADADDR(.data);
        * (.data .data.*) 
       address_end_data_ROM = LOADADDR(.data) + SIZEOF(.data);			 
        address_end_data_RAM = .;
        
	} >RAM1L AT >ROM
	
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
		
		} >  RAM1L 

 	
SDRAM_SECTION (NOLOAD) : {
		* (CHNFONT_BUF)

	} > SDRAM
	
VRAM_SECTION0 0x49000000 (NOLOAD) : {

		* (BSS_AUDIO_OUT_BUF)	
		* (IMAGE_MEM_BUF0)

		
	} > SDRAMNC
	
VRAM_SECTION1  0x20400000 (NOLOAD) : {
		* (FRAME_MEM_BUF0)
		* (FRAME_MEM_BUF1)
 		* (FRAME_MEM_BUF2)
		* (CAPFRAME_MEM_BUF0)
		
	} > VRAM
																

	
	
	
 
}