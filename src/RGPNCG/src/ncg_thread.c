/******************************************************************************
* Copyright(c) 2010-2012 Renesas Electronics Corporation. All rights reserved.
*
* brief  : NCG Thread/Process related functions.
*
* author : Renesas Electronics Corporation
*
* history: 2010.10.08
*          - Created the initial code.
*          2012.08.22
*          - Update all comments.
*
*******************************************************************************/

/*=============================================================================
 * Includes
 */

#include    "ncg_defs.h"
#include    "ncg_memory.h"
#include    "ncg_thread.h"
#include    "ncg_debug.h"


/*=============================================================================
 * Internal definitions
 */

#define     NCG_MAX_TLS_COUNT       (32)
#define     NCG_TLS_ARRAY_SIZE      (127)

#define     NCG_TLS_HASH_P          (311)


struct T_NCG_TLS {
    NCGint32 i32Tid ;
    NCGvoid *value ;
} ;


/*=============================================================================
 *  Prototyping of internal functions
 */

static NCGint32 find_tls_slot( struct T_NCG_TLS *tTlsArray, NCGint32 i32ThreadID, NCGint32 *pIndex );


/*=============================================================================
 *  Private global variables and functions
 */

static struct T_NCG_TLS *NCG_TLS_Arrays[ NCG_MAX_TLS_COUNT ] ;
static NCGint32 NCG_TLS_InUse[ NCG_MAX_TLS_COUNT ] ;

static NCGint32 NCG_TLS_Initialized = 0 ;


/*=============================================================================
 *  Global Function
 */

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_CreateTLS
 FUNCTION   : Create a new TLS.
 PARAMETERS : None.
 RETURN     : The index of the created TLS.
------------------------------------------------------------------------------*/
NCGuint32
NCGSYS_CreateTLS( void )
{
    NCGint32 i ;
    NCGuint32 ui32TlsIndex ;
    struct T_NCG_TLS *tTlsArray ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_CreateTLS" );

    if( NCG_TLS_Initialized == 0 ){
        NCG_TLS_Initialized = 1 ;

        for( i = 0 ; i < NCG_MAX_TLS_COUNT ; i++ ){
            NCG_TLS_Arrays[ i ] = NCG_NULL ;
            NCG_TLS_InUse[ i ] = 0 ;
        }
    }

    ui32TlsIndex = NCG_INVALID_TLSINDEX ;
    for( i = 0 ; i < NCG_MAX_TLS_COUNT ; i++ ){
        if( NCG_TLS_InUse[ i ] == 0 ){
            ui32TlsIndex = ( NCGuint32 ) i ;
            break ;
        }
    }
    if( ui32TlsIndex == NCG_INVALID_TLSINDEX ){
        return NCG_INVALID_TLSINDEX ;
    }

    tTlsArray = NCG_TLS_Arrays[ ui32TlsIndex ] ;
    if( tTlsArray == NCG_NULL ){
        NCGint32 ret ;

        ret = NCGSYS_HostMemAlloc( sizeof ( struct T_NCG_TLS ) * NCG_TLS_ARRAY_SIZE, 0, ( NCGvoid **)&tTlsArray ) ;
        if( ret != NCG_no_err ){
            return NCG_INVALID_TLSINDEX ;
        }
        NCG_TLS_Arrays[ ui32TlsIndex ] = tTlsArray ;
        NCG_TLS_InUse[ ui32TlsIndex ] = 1 ;
    }

    for( i = 0 ; i < NCG_TLS_ARRAY_SIZE ; i++ ){
        tTlsArray->i32Tid = -1 ;
        tTlsArray->value = NCG_NULL ;
        tTlsArray++ ;
    }
    return ui32TlsIndex ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_DestroyTLS
 FUNCTION   : Destroy the TLS that has the given index.
 PARAMETERS : ui32TlsIndex : [IN ] TLS index to be destroyed.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_DestroyTLS(
    NCGuint32       ui32TlsIndex )
{
    NCGint32 rc_val = NCG_no_err ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_DestroyTLS" );

    if( NCG_TLS_Initialized == 0 ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_DestroyTLS(1) Not initialized.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }
    if( ui32TlsIndex >= NCG_MAX_TLS_COUNT ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_DestroyTLS(2) Failed:ui32TlsIndex (0x%08x).%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( ui32TlsIndex )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_bad_parameter ;
    }
    if( NCG_TLS_InUse[ ui32TlsIndex ] == 0 ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_DestroyTLS(3) Index value is not use (0x%08x).%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( ui32TlsIndex )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_bad_parameter ;
    }

    NCG_TLS_InUse[ ui32TlsIndex ] = 0 ;

    NCGSYS_HostMemFree(( NCGvoid *)NCG_TLS_Arrays[ ui32TlsIndex ]) ;
    NCG_TLS_Arrays[ ui32TlsIndex ] = NCG_NULL;

    return rc_val ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_SetTLSValue
 FUNCTION   : Set the given value into a TLS.
 PARAMETERS : ui32TlsIndex : [IN ] The index of the TLS.
              pTlsValue    : [IN ] The value to be set into the TLS.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_SetTLSValue(
    NCGuint32       ui32TlsIndex,
    NCGvoid         *pTlsValue )
{
    NCGint32 i32ThreadID ;
    NCGint32 idx ;
    NCGint32 ret ;
    struct T_NCG_TLS *tTlsArray ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_SetTLSValue" );

    if( NCG_TLS_Initialized == 0 ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_SetTLSValue(1) Not initialized.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }
    if( ui32TlsIndex >= NCG_MAX_TLS_COUNT ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_SetTLSValue(2) Failed:ui32TlsIndex (0x%08x).%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( ui32TlsIndex )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_bad_parameter ;
    }
    if( NCG_TLS_InUse[ ui32TlsIndex ] == 0 ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_SetTLSValue(3) Index is not use (0x%08x).%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( ui32TlsIndex )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_bad_parameter ;
    }

    tTlsArray = NCG_TLS_Arrays[ ui32TlsIndex ] ;
    i32ThreadID = NCGSYS_GetCurrentThreadID() ;

    ret = find_tls_slot( tTlsArray, i32ThreadID, &idx ) ;
    if( ret != NCG_no_err ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_SetTLSValue(4) Failed:find_tls_slot (0x%08x).%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( ret )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return ret ;
    }

    tTlsArray[ idx ].value = pTlsValue ;
    if( pTlsValue == NCG_NULL ){
        tTlsArray[ idx ].i32Tid = -1 ;
    }
    return NCG_no_err ;
}


/*----------------------------------------------------------------------------
 NAME       : NCGSYS_GetTLSValue
 FUNCTION   : Get the current value from a TLS.
 PARAMETERS : ui32TlsIndex : [IN ] the index of the TLS.
 RETURN     : The current value of the TLS.
------------------------------------------------------------------------------*/
NCGvoid *
NCGSYS_GetTLSValue(
    NCGuint32       ui32TlsIndex )
{
    NCGint32 i32ThreadID ;
    NCGint32 idx ;
    NCGint32 ret ;
    struct T_NCG_TLS *tTlsArray ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_GetTLSValue" );

    if( NCG_TLS_Initialized == 0 ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_GetTLSValue(1) Not initialized.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_NULL ;
    }
    if( ui32TlsIndex >= NCG_MAX_TLS_COUNT ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_GetTLSValue(2) Failed:ui32TlsIndex (0x%08x)%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( ui32TlsIndex )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_NULL ;
    }
    if( NCG_TLS_InUse[ ui32TlsIndex ] == 0 ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_GetTLSValue(3) Index value is already used (0x%08x)%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( ui32TlsIndex )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_NULL ;
    }

    tTlsArray = NCG_TLS_Arrays[ ui32TlsIndex ] ;
    i32ThreadID = NCGSYS_GetCurrentThreadID() ;

    ret = find_tls_slot( tTlsArray, i32ThreadID, &idx ) ;
    if( ret != NCG_no_err ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_GetTLSValue(4) Failed:find_tls_slot (0x%08x)%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( ret )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_NULL ;
    }
    return tTlsArray[ idx ].value ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_GetCurrentProcessID
 FUNCTION   : Get the current process ID.
 PARAMETERS : None.
 RETURN     : Current process ID.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_GetCurrentProcessID( void )
{
    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_GetCurrentProcessID" );

    /* Process ID is always zero on this environment. */
    return ( NCGint32 )0 ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_GetCurrentThreadID
 FUNCTION   : Get the current thread ID.
 PARAMETERS : None.
 RETURN     : Current thread ID
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_GetCurrentThreadID( void )
{
    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_GetCurrentThreadID" );

    return ( NCGint32 )0 ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_CreateThread
 FUNCTION   : Create the thread object.
 PARAMETERS : ppObj       : [OUT] The pointer to the created thread control object.
              pfnThread   : [IN ] The starting address of thread.
              pParam      : [IN ] The expansion information.
              i32Priority : [IN ] The priority of thread.
                               - NCG_THREAD_PRI_1: Priority Level 1 (Highest priority)
                               - NCG_THREAD_PRI_2: Priority Level 2 
                               - NCG_THREAD_PRI_3: Priority Level 3 
                               - NCG_THREAD_PRI_4: Priority Level 4 (Middle priority)
                               - NCG_THREAD_PRI_5: Priority Level 5 
                               - NCG_THREAD_PRI_6: Priority Level 6 
                               - NCG_THREAD_PRI_7: Priority Level 7 (Lowest priority)
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_CreateThread(
    NCGvoid         **ppObj,
    NCGfp           pfnThread,
    NCGvoid         *pParam,
    NCGint32        i32Priority )
{
    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_CreateThread" );

    NCG_UNREFERENCED_PARAMETER(ppObj);
    NCG_UNREFERENCED_PARAMETER(pfnThread);
    NCG_UNREFERENCED_PARAMETER(pParam);
    NCG_UNREFERENCED_PARAMETER(i32Priority);

    return NCG_no_err ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_DestroyThread
 FUNCTION   : Destroy the thread object
 PARAMETERS : pObj : [IN ] The pointer to the created thread control object.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_DestroyThread(
    NCGvoid         *pObj )
{
    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_DestroyThread" );

    NCG_UNREFERENCED_PARAMETER(pObj);

    return NCG_no_err ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_StartThread
 FUNCTION   : Start the thread.
 PARAMETERS : pObj : [IN ] The pointer to the created thread control object.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_StartThread(
    NCGvoid         *pObj )
{
    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_StartThread" );

    NCG_UNREFERENCED_PARAMETER(pObj);

    return NCG_no_err ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_StopThread
 FUNCTION   : Stop the thread.
 PARAMETERS : pObj : [IN ] The pointer to the created thread control object.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_StopThread(
    NCGvoid         *pObj )
{
    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_StopThread" );

    NCG_UNREFERENCED_PARAMETER(pObj);

    return NCG_no_err ;
}


/*=============================================================================
 *  Internal functions
 */

/*----------------------------------------------------------------------------
 NAME       : find_tls_slot
 FUNCTION   : Find the TLS.
 PARAMETERS : tTlsArray   : [IN ] Pointer to the TLS array.
              i32ThreadID : [IN ] Thread ID.
              pIndex      : [OUT] Pointer to the TLS index.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
static NCGint32
find_tls_slot(
    struct T_NCG_TLS        *tTlsArray,
    NCGint32                i32ThreadID,
    NCGint32                *pIndex )
{
    NCGint32 idx ;

    /* Calculate temporary index number with hashing */
    idx = ( i32ThreadID * NCG_TLS_HASH_P ) % NCG_TLS_ARRAY_SIZE ;

    if( tTlsArray[ idx ].i32Tid != i32ThreadID ){
        NCGint32 found = 0 ;
        NCGint32 i ;

        for( i = 0 ; i < NCG_TLS_ARRAY_SIZE ; i++ ){
            idx = ( idx + 1 ) % NCG_TLS_ARRAY_SIZE ;
            if( tTlsArray[ idx ].i32Tid == i32ThreadID || tTlsArray[ idx ].i32Tid < 0 ){
                found = 1 ;
                break ;
            }
        }
        if( found == 0 ){
            NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]find_tls_slot(1) Not find.%s" )
            NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
            NCG_DEBUG_MAKE_MSG_END();
            NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            return NCG_err_resource_exceeded ;
        }
    }
    *pIndex = idx ;

    return NCG_no_err ;
}

/* -- end of file -- */
