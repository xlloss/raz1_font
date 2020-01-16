OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(start)
 
/*Below is modified by RETW-General */
MEMORY {
	ROM   (rx)   			: ORIGIN = 0x18000000, LENGTH = 0x00300000  			/* PAge 0 Lower bank ( 3MB) H'20020000 to H'202FFFFF 		*/
	RAM1L (rw)   			: ORIGIN = 0x20300000, LENGTH = 0x00200000  			/* Page 1 Upper bank ( 3MB) H'20500000 to H'207FFFFF 		*/
 	VRAM  (rw)   			: ORIGIN = 0x20500000, LENGTH = 0x00500000  			/* VRAM: Frame buffer for LCD panel (4MB) 						  */
 	SDRAM (rw)   			: ORIGIN = 0x09000000, LENGTH = 0x01000000  		/* 16MB 						  																	*/
 	SDRAMNC  (rw)   	: ORIGIN = 0x48000000, LENGTH = 0x01000000  		/* 16MB 						  																	*/
}

PROGRAM_STACK_SIZE    = 0x8000; 	/* Application stack                  */
IRQ_STACK_SIZE	      = 0x2000;     /* IRQ mode stack                     */
FIQ_STACK_SIZE	      = 0x2000;     /* FRQ mode stack                     */
SVC_STACK_SIZE	      = 0x2000;     /* SVC mode stack                     */
ABT_STACK_SIZE	      = 0x2000;     /* ABT mode stack                     */
TTB_SIZE              = 0x8000;     /* Level-1 Translation Table for MMU  */
HEAP_SIZE		      = 0x00010000; /* heap								  */
	 
BOOT_VECTOR_TABLE     = 0x18000000; 
VECTOR_TABLE          = 0x18008000; 
	 
SECTIONS
{
	.boot_reset	BOOT_VECTOR_TABLE :
	{
		PROVIDE(execute = .); 
 
		*R_boot1_start.o (.text)
		.	= ALIGN(0x4);
		*R_Boot2_resetprg.o (.text)
		.	= ALIGN(0x4);
		
				*init_spibsc_init1_section.o (.text)
				*init_spibsc_init1_section.o (.rodata)
				*init_spibsc_init1_section.o (.data)


				
 		
	} >ROM

	.boot_code	:
	{
				__RAM_SECTIONCOPY_start__ = .;
        __ROM_SECTIONCOPY_start__ = LOADADDR(.boot_code); 
 
        /* The CODE below will be located in ROM, and copy all section to RAM to be executed */
 
				*spibsc_init1.o (.text)
				*spibsc_init1.o (.rodata)
				*spibsc_init1.o (.data)
 
				*spibsc_init2.o (.text)
				*spibsc_init2.o (.rodata)
				*spibsc_init2.o (.data)

				*spibsc_flash_api.o (.text)
				*spibsc_flash_api.o (.rodata)
				*spibsc_flash_api.o (.data)
				*spibsc_flash_userdef.o (.text)
				*spibsc_flash_userdef.o (.rodata)
				*spibsc_flash_userdef.o (.data)
				*spibsc_ioset_api.o (.text)
				*spibsc_ioset_api.o (.rodata)
				*spibsc_ioset_api.o (.data)
				*spibsc_ioset_drv.o (.text)
				*spibsc_ioset_drv.o (.rodata)
				*spibsc_ioset_drv.o (.data)
				*spibsc_ioset_userdef.o (.text)
				*spibsc_ioset_userdef.o (.rodata)
				*spibsc_ioset_userdef.o (.data)

				*user_prog_jmp.o (.text)
				*user_prog_jmp.o (.rodata)
				*user_prog_jmp.o (.data)
				
        __ROM_SECTIONCOPY_end__ = LOADADDR(.boot_code) + SIZEOF(.boot_code);; 
        __RAM_SECTIONCOPY_end__ = .;               
            
				.	= ALIGN(0x4);
	} >RAM1L AT >ROM	

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
	} >ROM

	.data :
	{
				address_start_data_RAM = .;
        address_start_data_ROM = LOADADDR(.data);
        * (.data .data.*) 
       	address_end_data_ROM = LOADADDR(.data) + SIZEOF(.data);			 
        address_end_data_RAM = .;
        
	} >RAM1L AT >ROM
	
	.ttb_mmu1 0x20380000:
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