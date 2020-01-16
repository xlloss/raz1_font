/******************************************************************************
* Copyright(c) 2010-2012 Renesas Electronics Corporation. All rights reserved.
*
* brief  : NCG State variable control functions.
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
#include    "ncg_time.h"
#include    "ncg_memory.h"
#include    "ncg_state.h"


/*=============================================================================
 * Internal definitions
 */

#define     NCG_MAX_EVENT_FLAGS     ( 2 )
#define     NCG_BITS_PER_EVFLG      ( 32 )
#define     NCG_MAX_STATE_OBJECTS   ( NCG_MAX_EVENT_FLAGS * NCG_BITS_PER_EVFLG )

struct T_NCG_STATE {
    NCGint32        stid;
    NCGint32        bInUse;
    NCGint32        i32FlgIdx;
    unsigned long   flgbit;
    NCGuint32       ui32State;
} ;


/*=============================================================================
 *  Prototyping of internal functions
 */

static NCGint32 ncg_init_states( void ) ;


/*=============================================================================
 *  Private global variables and functions
 */

static int event_flags[ NCG_MAX_EVENT_FLAGS ] ;
volatile static struct T_NCG_STATE tStates[ NCG_MAX_STATE_OBJECTS ] ;

static NCGint32 ncg_state_initialized = 0 ;


/*=============================================================================
 *  Global Function
 */

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_CreateState
 FUNCTION   : Create a state control object.
 PARAMETERS : ppObj       : [OUT] The pointer to the created state control object.
              ui32StateID : [IN ] The identifier of the state variable.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_CreateState(
    NCGvoid         **ppObj,
    NCGuint32       ui32StateID )
{
    NCGint32 ret ;
    NCGint32 i ;
    struct T_NCG_STATE *pState = NCG_NULL ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_CreateState" );

    if( ppObj == NCG_NULL ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_CreateState(1) invalid ppObj.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }

    if( ncg_state_initialized == 0 ){
        ret = ncg_init_states() ;
        if( ret != NCG_no_err ){
            NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_CreateState(2) falsed:ncg_init_states(%d).%s" )
            NCG_DEBUG_MAKE_MSG_PARAMETER( ret )
            NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
            NCG_DEBUG_MAKE_MSG_END();
            NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            return ret ;
        }
        ncg_state_initialized = 1 ;
    }

    for( i = 0 ; i < NCG_MAX_STATE_OBJECTS ; i++ ){
        if( tStates[ i ].bInUse == 0 ){
            tStates[ i ].bInUse = 1 ;
            pState = (struct T_NCG_STATE*)&tStates[ i ] ;
            break ;
        }
    }
    if( pState == NCG_NULL ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_CreateState(3) Not find.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_resource_exceeded ;
    }

    if(( 0 > pState->i32FlgIdx ) || ( pState->i32FlgIdx >= NCG_MAX_EVENT_FLAGS )){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_CreateState(4) Invalid status(%d).%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( pState->i32FlgIdx )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        pState->bInUse = 0 ;
        return NCG_err_unknown ;
    }
    if( event_flags[ pState->i32FlgIdx ] < 0 ){
        event_flags[ pState->i32FlgIdx ] = 0 ;
    }
    pState->ui32State = 0U ;
    *ppObj = ( NCGvoid *)pState ;

    return NCG_no_err ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_DestroyState
 FUNCTION   : Destroy the given state control object.
 PARAMETERS : pObj : [IN ] The pointer to the state control object.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_DestroyState(
    NCGvoid *pObj )
{
    struct T_NCG_STATE *pState = ( struct T_NCG_STATE *)pObj ;
    NCGint32 stid = pState->stid ;
    NCGint32 i ;
    NCGboolean bDestroyFlg ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_DestroyState" );

    if( pState == NCG_NULL ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_DestroyState(1) Failed: pState value is NULL.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }
    if( &tStates[ stid ] != pState ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_DestroyState(2) Failed: Invalid object.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }
    if( pState->bInUse == 0 ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_DestroyState(3) Failed: Object is not used.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }
    tStates[ stid ].bInUse = 0 ;
    tStates[ stid ].ui32State = 0U ;

    bDestroyFlg = NCG_TRUE ;
    for( i = 0 ; i < NCG_MAX_STATE_OBJECTS ; i++ ){
        if( tStates[ i ].bInUse != 0 ){
            bDestroyFlg = NCG_FALSE ;
            break ;
        }
    }
    if( bDestroyFlg != NCG_FALSE ){
        for( i = 0 ; i < NCG_MAX_EVENT_FLAGS ; i++ ){
            if( event_flags[ i ] != -1 ){
                event_flags[ i ] = -1 ;
            }
        }
        ncg_state_initialized = 0 ;
    }
    return NCG_no_err ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_SetState
 FUNCTION   : Set the state control object into the given state.
 PARAMETERS : pObj      : [IN ] The pointer to the state control object.
              ui32State : [IN ] The state value to set.
              ui32Flags : [IN ] Flags.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_SetState(
    NCGvoid         *pObj,
    NCGuint32       ui32State,
    NCGuint32       ui32Flags )
{
    int flgid ;
    volatile struct T_NCG_STATE *pState = ( volatile struct T_NCG_STATE *)pObj ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_SetState" );

    if( pState == NCG_NULL ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_SetState(1) Failed:pObj is NULL.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }
    if( &tStates[ pState->stid ] != pState ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_SetState(2) Failed:status (0x%08x).%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( pState->stid )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }
    if( pState->bInUse == 0 ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_SetState(3) Failed: pObj is not used.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }

    flgid = event_flags[ pState->i32FlgIdx ] ;
    if( flgid < 0 ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_SetState(4) Failed: pObj status (0x%08x).%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( pState->i32FlgIdx )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }

    switch( ui32Flags & NCGSYS_STATE_SET_MASK ){
        case NCGSYS_STATE_SET_AND :
            pState->ui32State &= ui32State ;
            break ;

        case NCGSYS_STATE_SET_OR :
            pState->ui32State |= ui32State ;
            break ;

        case NCGSYS_STATE_SET_SET :
            pState->ui32State = ui32State ;
            break ;

        default :
            NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_SetState(5) Failed:ui32Flags (0x%08x).%s" )
            NCG_DEBUG_MAKE_MSG_PARAMETER( ui32Flags )
            NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
            NCG_DEBUG_MAKE_MSG_END();
            NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            return NCG_err_bad_parameter ;
    }

    return NCG_no_err ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_GetState
 FUNCTION   : Get the current state value from the state control object.
 PARAMETERS : pObj       : [IN ] The pointer to the state control object.
              ui32Flags  : [IN ] Flags.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGuint32
NCGSYS_GetState(
    NCGvoid         *pObj,
    NCGuint32       ui32Flags )
{
    volatile struct T_NCG_STATE *pState = ( volatile struct T_NCG_STATE *)pObj ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_GetState" );

    if( pState == NCG_NULL ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_GetState(1) Failed: pObj.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return (NCGuint32)NCG_err_unknown ;
    }
    if( &tStates[ pState->stid ] != pState ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_GetState(2) Failed: pObj status(%d).%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( pState->stid )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return (NCGuint32)NCG_err_unknown ;
    }
    if( pState->bInUse == 0 ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_GetState(3) Failed: pObj is not used.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return (NCGuint32)NCG_err_unknown ;
    }
    return ( NCGuint32 )pState->ui32State ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_WaitState
 FUNCTION   : Wait for the state control object to be in the given state.
 PARAMETERS : pObj        : [IN ] The pointer to the state control object.
              ui32State   : [IN ] The state value to wait for.
              ui32Flags   : [IN ] Flags.
              ui32Timeout : [IN ] Timeout value in milli seconds.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_WaitState(
    NCGvoid         *pObj,
    NCGuint32       ui32State,
    NCGuint32       ui32Flags,
    NCGuint32       ui32Timeout )
{
    volatile struct T_NCG_STATE *pState = ( volatile struct T_NCG_STATE *)pObj ;
    NCGint32 stid = pState->stid ;
    int flgid ;
    enum{ NCGWAIT_OR, NCGWAIT_AND } wait_mode ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_WaitState" );

    wait_mode = (( ui32Flags & NCGSYS_STATE_WAIT_MASK ) == NCGSYS_STATE_WAIT_OR ) ? NCGWAIT_OR : NCGWAIT_AND ;

    if( pState == NCG_NULL ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_WaitState(1) Failed: pObj is NULL.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }
    if( &tStates[ stid ] != pState ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_WaitState(2) Failed: pObj.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }
    if( pState->bInUse == 0 ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_WaitState(3) Failed: pObj.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }
    if(( ui32Timeout != NCG_TIMEOUT_INFINITE ) && ( ui32Timeout > 0x7fffffffU )){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_WaitState(4) Failed: pObj.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }

    flgid = event_flags[ pState->i32FlgIdx ] ;
    if( flgid < 0 ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_WaitState(5) Failed: pObj.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }

    if( ui32Timeout != NCG_TIMEOUT_INFINITE ){
        NCGint32 i32SignedTimeout = ( NCGint32 )ui32Timeout ;
        volatile NCGuint32 count ;

        while((( wait_mode == NCGWAIT_AND ) && ( pState->ui32State & ui32State ) != ui32State ) ||
                (( wait_mode == NCGWAIT_OR ) && ( pState->ui32State & ui32State ) == 0 )){
            NCGuint32 time ;

            time = NCGSYS_GetTickCount() ;

            i32SignedTimeout -= time ;
            if( i32SignedTimeout < 0 ){
                return NCG_err_wait_timeout ;
            }
        }
    }
    else {
        while((( wait_mode == NCGWAIT_AND ) && ( pState->ui32State & ui32State ) != ui32State ) ||
            (( wait_mode == NCGWAIT_OR ) && ( pState->ui32State & ui32State ) == 0 )){
            /* No operation */ ;
        }
    }
    return NCG_no_err ;
}


/*=============================================================================
 *  Internal functions
 */

/*----------------------------------------------------------------------------
 NAME       : ncg_init_states
 FUNCTION   : Initialize global variable in NCG state.
 PARAMETERS : None.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
static NCGint32
ncg_init_states( void )
{
    NCGint32 i ;

    for( i = 0 ; i < NCG_MAX_EVENT_FLAGS ; i++ ){
        event_flags[ i ] = -1 ;
    }

    for( i = 0 ; i < NCG_MAX_STATE_OBJECTS ; i++ ){
        tStates[ i ].stid = i ;
        tStates[ i ].bInUse = 0 ;
        tStates[ i ].i32FlgIdx = ( NCGint32 )( i / NCG_BITS_PER_EVFLG ) ;
        tStates[ i ].flgbit = ( unsigned long )( 1 << ( NCGuint32 )( i & ( NCG_BITS_PER_EVFLG - 1 ))) ;
        tStates[ i ].ui32State = 0U ;
    }
    return NCG_no_err ;
}

/* -- end of file -- */
