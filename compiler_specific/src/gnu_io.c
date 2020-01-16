/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_scif_uart.h"       /* UART Driver header */
#include "intc.h"                   /* INTC Driver Header */
#include "iodefine.h"
#include "gnu_io.h"

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
* Function Name: _write
* Description  : GNU interface to low-level I/O write
* Arguments    : int file_no
*              : const char *buffer
*              : unsigned int n
* Return Value : none
******************************************************************************/
int _write(int file_no , const char *buffer , unsigned int n)
{
	return SioWrite(file_no , buffer , n);
}

/******************************************************************************
* Function Name: _read
* Description  : GNU interface to low-level I/O read
* Arguments    : int file_no
*              : const char *buffer
*              : unsigned int n
* Return Value : none
******************************************************************************/
int _read(int file_no , char *buffer , unsigned int n)
{
	return SioRead(file_no , buffer , n);
}
