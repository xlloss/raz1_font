OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(start)
 
/*Below is modified by RETW-General */
MEMORY {
	ROM (rx)   : ORIGIN = 0x18000000, LENGTH = 0x00300000  /* PAge 0 Lower bank ( 3MB) H'20020000 to H'202FFFFF 	*/
	RAM (rw)   : ORIGIN = 0x20020000, LENGTH = 0x00300000  /* Page 1 Upper bank ( 3MB) H'20500000 to H'207FFFFF 	*/
	VRAM  (rw)   : ORIGIN = 0x20500000, LENGTH = 0x00500000  /* VRAM: Frame buffer for LCD panel (4MB) 						*/
}

PROGRAM_STACK_SIZE    = 0x8000; 	/* Application stack                  	*/
IRQ_STACK_SIZE	      = 0x2000;     /* IRQ mode stack                     */
FIQ_STACK_SIZE	      = 0x2000;     /* FRQ mode stack                     */
SVC_STACK_SIZE	      = 0x2000;     /* SVC mode stack                     */
ABT_STACK_SIZE	      = 0x2000;     /* ABT mode stack                     */
TTB_SIZE              = 0x8000;     /* Level-1 Translation Table for MMU  */
HEAP_SIZE		      = 0x00100000; /* heap								  */
	 
VECTOR_TABLE          = 0x18000000; /* VECTOR_TABLE located here */ /* should be 0 for RAM based code */ 	 
 
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
 
	.irq_stack :					/* original is C0000, try D0000 is OK  */
	{
		irq_stack_start = .;
		.  += IRQ_STACK_SIZE;
		.	= ALIGN(0x4);
		irq_stack_end = .;
	} >RAM
		
	.fiq_stack :
	{
		fiq_stack_start = .;
		.  += FIQ_STACK_SIZE;
		.	= ALIGN(0x4);
		fiq_stack_end = .;
	} >RAM
		
	.svc_stack :
	{
		PROVIDE(svc_stack_start = .);		
		.  += SVC_STACK_SIZE;
		.	= ALIGN(0x4);
		svc_stack_end = .;
	} >RAM
		
	.abt_stack :
	{
		abt_stack_start = .;
		.  += ABT_STACK_SIZE;
		.	= ALIGN(0x4);
		abt_stack_end = .;
	} >RAM

	.ttb_mmu1 :
	{
		ttb_mmu1_base = .;
		.  += TTB_SIZE;
		.	= ALIGN(0x4);
		ttb_mmu1_end = .;
	} >RAM
 
	
	.text :
	{
		text_start = .;
		*(.text)
		
				*init_spibsc_init1_section.o (.text)
				*init_spibsc_init1_section.o (.rodata)
				*init_spibsc_init1_section.o (.data)
 
		address_end_text = .;
	} > ROM
 
	.rodata : 
	{
		_srodata = .;
		*(.rodata)
		*(.rodata.*)
		.	= ALIGN(0x4);
		_erodata = .;
	} >ROM

	.data :
	{
				address_start_data_RAM = .;
        address_start_data_ROM = LOADADDR(.data);
        * (.data .data.*) 
       address_end_data_RAM = LOADADDR(.data) + SIZEOF(.data);			 
        address_end_data_RAM = .;
 
	} >RAM  AT >ROM
 
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

      } > RAM
      
VRAM_SECTION0 0x20500000 (NOLOAD) : {
		* (FRAME_MEM_BUF0)
	} > VRAM
	
VRAM_SECTION1  0x20580000 (NOLOAD) : {
		* (FRAME_MEM_BUF1)
	} > VRAM

VRAM_SECTION2  0x20700000 (NOLOAD) : {
		* (FRAME_MEM_BUF2)
	} > VRAM
	
VRAM_SECTION 0x20900000(NOLOAD) : {
		* (BSS_AUDIO_OUT_BUF)	

	} > VRAM
 
}