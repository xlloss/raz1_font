/******************************************************************************
* Copyright(c) 2010-2012 Renesas Electronics Corporation. All rights reserved.
*
* brief  : NCG User/Driver space bridge function.
*
* author : Renesas Electronics Corporation
*
* history: 2010.10.08
*          - Created the initial code.
*
*******************************************************************************/

/*=============================================================================
 * Includes
 */

#include    "ncg_defs.h"
#include    "ncg_debug.h"
#include    "ncg_ioctl.h"


/*=============================================================================
 * Internal definitions
 */

#define     SERVICE_CALL_ID_MAX     3

typedef struct{
    NCGuint32 DeviceID ;            /*!< ID */
    NCGIOControlHandler IOControl ; /*!< Service call */
} Ncg_ServiceCall ;


/*=============================================================================
 *  Prototyping of internal functions
 */


/*=============================================================================
 *  Private global variables
 */

static Ncg_ServiceCall ServiceCall[ SERVICE_CALL_ID_MAX ] =
{
    { 0, ( NCGIOControlHandler )0 },
    { 0, ( NCGIOControlHandler )0 },
    { 0, ( NCGIOControlHandler )0 }
} ;


/*=============================================================================
 *  Global Function
 */

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_RegisterDevice
 FUNCTION   : Regiter the device I/O control handler.
 PARAMETERS : ui32DeviceID : [IN ] Device ID.
              pfnIOControl : [IN ] Device I/O control handler.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_RegisterDevice(
    NCGuint32           ui32DeviceID,
    NCGIOControlHandler pfnIOControl )
{
    Ncg_ServiceCall *SvcCallTbl ;
    int i ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_RegisterDevice" );

    if( pfnIOControl == NCG_NULL ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_RegisterDevice Failed.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }

    SvcCallTbl = &ServiceCall[ 0 ] ;
    for( i = 0 ; i < SERVICE_CALL_ID_MAX ; i++ ){
        if ( (SvcCallTbl->IOControl != 0) && (SvcCallTbl->IOControl == pfnIOControl) ) {
            /* pfnIOControl is registered now. */
            return NCG_no_err ;
        }
    }
    SvcCallTbl = &ServiceCall[ 0 ] ;
    for( i = 0 ; i < SERVICE_CALL_ID_MAX ; i++ ){
        if( SvcCallTbl->IOControl == 0 ){
            SvcCallTbl->DeviceID = ui32DeviceID ;
            SvcCallTbl->IOControl = pfnIOControl ;
            return NCG_no_err ;
        }
        SvcCallTbl++ ;
    }
    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_RegisterDevice Failed.%s" )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
    NCG_DEBUG_MAKE_MSG_END();
    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );

    return NCG_err_unknown ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_UnregisterDevice
 FUNCTION   : Unregiter the device I/O control handler.
 PARAMETERS : ui32DeviceID : [IN ] Device ID.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_UnregisterDevice(
    NCGuint32       ui32DeviceID )
{
    Ncg_ServiceCall *SvcCallTbl ;
    int i ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_UnregisterDevice" );

    SvcCallTbl = &ServiceCall[ 0 ] ;
    for( i = 0 ; i < SERVICE_CALL_ID_MAX ; i++ ){
        if( SvcCallTbl->DeviceID == ui32DeviceID ){
            SvcCallTbl->DeviceID = 0 ;
            SvcCallTbl->IOControl = ( NCGIOControlHandler )0 ;
            return NCG_no_err ;
        }
        SvcCallTbl++ ;
    }
    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_UnregisterDevice Failed..%s" )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
    NCG_DEBUG_MAKE_MSG_END();
    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );

    return NCG_err_unknown ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_DeviceIOControl
 FUNCTION   : Device I/O control.
 PARAMETERS : ui32DeviceID      : [IN ] Device ID.
              i32FunctionID     : [IN ] Function ID.
              ui32InBufferSize  : [IN ] The size of "pInBuffer".
              pInBuffer         : [IN ] The pointer to the input data.
              ui32OutBufferSize : [IN ] The size of "pOutBuffer".
              pOutBuffer        : [I/O] The pointer to the output data.
 RETURN     : Error code.
------------------------------------------------------------------------------*/
NCGint32 NCGSYS_DeviceIOControl(
    NCGuint32       ui32DeviceID,
    NCGint32        i32FunctionID,
    NCGuint32       ui32InBufferSize,
    NCGvoid         *pInBuffer,
    NCGuint32       ui32OutBufferSize,
    NCGvoid         *pOutBuffer )
{
    Ncg_ServiceCall *SvcCallTbl ;
    int i ;
    NCGint32 ret ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_DeviceIOControl" );

    SvcCallTbl = &ServiceCall[ 0 ] ;
    for( i = 0 ; i < SERVICE_CALL_ID_MAX ; i++ ){
        if( SvcCallTbl->DeviceID == ui32DeviceID ){
            ret = SvcCallTbl->IOControl( i32FunctionID, pInBuffer ) ;
            return ret ;
        }
        SvcCallTbl++ ;
    }
    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_DeviceIOControl Failed.%s" )
    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
    NCG_DEBUG_MAKE_MSG_END();
    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );

    return NCG_err_unknown ;
}

/* -- end of file -- */
