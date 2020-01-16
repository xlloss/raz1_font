/******************************************************************************
* Copyright(c) 2010-2012 Renesas Electronics Corporation. All rights reserved.
*
* brief  : NCG Message Queue control functions.
*
* author : Renesas Electronics Corporation
*
* history: 2011.03.04
*          - Created the initial code.
*          2011.10.24
*          - Added the following macro.
*            - NCG_NO_QUEUE_CREATE
*          2012.08.22
*          - Updated coding format.
*          - Added the debug message in error case.
*
*******************************************************************************/

/*=============================================================================
 * Includes
 */

#include    "ncg_defs.h"
#include    "ncg_queue.h"
#include    "ncg_debug.h"


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
 NAME       : NCGSYS_CreateQueue
 FUNCTION   : Create the queue object.
 PARAMETERS : ppObj        : [OUT] The pointer to the created queue object.
              ui32QueueNum : [IN ] The number of data the queue can store.
              ui32Flags    : [IN ] The extension flags(fixed to 0).
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_CreateQueue(
    NCGvoid         **ppObj,
    NCGuint32       ui32QueueNum,
    NCGuint32       ui32Flags )
{
    NCGint32 rc_val = NCG_no_err;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_CreateQueue" );

    if ( ppObj == NCG_NULL ) {
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_CreateQueue(1) Failed:ppObj.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown;
    }

    return rc_val ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_DestroyQueue
 FUNCTION   : Destroy the queue object.
 PARAMETERS : pObj : [IN ] The pointer to the created queue object.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_DestroyQueue(
    NCGvoid         *pObj )
{
    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_DestroyQueue" );

    return NCG_no_err ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_SendQueue
 FUNCTION   : Send the data to the queue.
 PARAMETERS : pObj         : [IN ] The pointer to the created queue object.
              ui32DataSize : [IN ] The data size to send to the queue.
              pData        : [IN ] The pointer to the data to send to the queue.
              ui32TimeOut  : [IN ] The time out value in [msec].
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_SendQueue(
    NCGvoid         *pObj,
    NCGuint32       ui32DataSize,
    NCGvoid         *pData,
    NCGuint32       ui32TimeOut )
{
    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_SendQueue" );

    return NCG_no_err ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_ReceiveQueue
 FUNCTION   : Receive the data from the queue.
 PARAMETERS : pObj         : [IN ] The pointer to the created queue object.
              ui32RcvSize  : [OUT] The data size received from the queue.
              ui32DataSize : [IN ] The maximum data size that can be received from the queue.
              ppData       : [OUT] The pointer to the data received from the queue.
              ui32TimeOut  : [IN ] The time out value in [msec].
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_ReceiveQueue(
    NCGvoid         *pObj,
    NCGuint32       *ui32RcvSize,
    NCGuint32       ui32DataSize,
    NCGvoid         **ppData,
    NCGuint32       ui32TimeOut )
{
    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_ReceiveQueue" );

    return NCG_no_err ;
}

/* -- end of file -- */
