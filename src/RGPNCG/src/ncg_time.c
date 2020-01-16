/******************************************************************************
* Copyright(c) 2010-2012 Renesas Electronics Corporation. All rights reserved.
*
* brief  : NCG timer functions.
*
* author : Renesas Electronics Corporation
*
* history: 2013.01.08
*          - Create dummy functions.
*
*******************************************************************************/

/*=============================================================================
 * Includes
 */

#include    "ncg_defs.h"
#include    "ncg_debug.h"
#include    "ncg_time.h"


/*=============================================================================
 * Internal definitions
 */


/*=============================================================================
 *  Prototyping of internal functions
 */


/*=============================================================================
 *  Private global variables and functions
 */


/*=============================================================================
 *  Global Function
 */

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_Sleep
 FUNCTION   : Sleep this process.
 PARAMETERS : ui32MilliSeconds :[IN ] Sleeping time.
 RETURN     : Error code.
------------------------------------------------------------------------------*/
NCGint32 
NCGSYS_Sleep(
    NCGuint32       ui32MilliSeconds )
{
    /* unsupport function */

    NCG_UNREFERENCED_PARAMETER( ui32MilliSeconds );

    return NCG_no_err ;
}


/*----------------------------------------------------------------------------
 NAME       : NCGSYS_StartTickCount
 FUNCTION   : Start tick count.
 PARAMETERS : None.
 RETURN     : Error code.
------------------------------------------------------------------------------*/
NCGint32 
NCGSYS_StartTickCount( void )
{
    /* unsupport function */

    return NCG_no_err ;
}


/*----------------------------------------------------------------------------
 NAME       : NCGSYS_GetTickCount
 FUNCTION   : Gets current tick count.
 PARAMETERS : None.
 RETURN     : Time counter[msec]
------------------------------------------------------------------------------*/
NCGuint32
NCGSYS_GetTickCount( void )
{
    /* unsupport function */
    return 0;
}

/* -- end of file -- */
