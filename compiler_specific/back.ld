	.data  :
	{
/*	        . = ALIGN(0x100000);			*/
				address_start_data_RAM = .;
        address_start_data_ROM = LOADADDR(.data);
/*        * (.data .data.*)        */
/*      address_end_data_RAM = LOADADDR(.data) + SIZEOF(.data);					*/
        address_end_data_RAM = .;
	} >RAM1L AT >RAM01L