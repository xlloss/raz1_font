/******************************************************************************
* Copyright (c) 2010-2012 Renesas Electronics Corporation. All rights reserved.
*
* brief: NCG debug function module.
*
* author: Renesas Electronics Corporation.
*
* history: 2010.10.08
*          - Created the initial code.
*          2012.08.22
*          - Updated coding format.
*
*******************************************************************************/

#include "ncg_defs.h"
#include "ncg_debug.h"

#ifdef NCG_DEBUG

char NCG_G_MASSAGE_BUFF[NCG_DEBUG_MSG_BUFFER_SIZE];


/*----------------------------------------------------------------------------
 NAME       : NCGSYS_Abort
 FUNCTION   : Abort process.
 PARAMETERS : filename  : [IN ] The pointer to the file name.
              line      : [IN ] Number of the abort line.
              condition : [IN ] The pointer to the condition message.
 RETURN     : None
------------------------------------------------------------------------------*/
NCGvoid
NCGSYS_Abort(
    const NCGchar   *filename,
    NCGint32        line,
    const NCGchar   *condition)
{
    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "assertion failed at %s (%d) : %s%s" )
    NCG_DEBUG_MAKE_MSG_PARAMETER( filename )
    NCG_DEBUG_MAKE_MSG_PARAMETER( line )
    NCG_DEBUG_MAKE_MSG_PARAMETER( condition )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
    NCG_DEBUG_MAKE_MSG_END();
    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
    while (condition[0])
    {
        ;
    }
}

#endif /* NCG_DEBUG */

/* -- end of file -- */
