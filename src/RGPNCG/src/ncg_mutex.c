/******************************************************************************
* Copyright(c) 2010-2012 Renesas Electronics Corporation. All rights reserved.
*
* brief  : NCG Mutual Exclusion (Mutex) control functions.
*
* author : Renesas Electronics Corporation.
*
* history: 2010.10.08
*          - Created the initial code.
*          2012.08.23
*          - Update source code:
*             - Format of source code.
*             - tab to space.
*             - Debug message in the case of error.
*
*******************************************************************************/

/*=============================================================================
 * Includes
 */

#include "ncg_defs.h"
#include "ncg_debug.h"
#include "ncg_mutex.h"
#include "ncg_memory.h"
#include "ncg_time.h"


/*=============================================================================
 * Internal definitions
 */


#define     SEM_ID_MAX                  8

typedef struct
{
    unsigned short Used ;           /*!< Used (1) or not (0) */
    unsigned long SemCnt ;          /*!< Semaphore resource count */
    unsigned long MaxSem ;          /*!< Max semaphore resource count */
} SemDat ;

typedef enum
{
    SEM_ERR_OK = 0,
    SEM_ERR_FULL = -1,
    SEM_ERR_ID = -2,
    SEM_ERR_UNDEF = -3,
    SEM_ERR_TIMEOUT = -4,
    SEM_ERR_OVERMAX = -5,
} SemErr ;


/*=============================================================================
 *  Prototyping of internal functions
 */

static int CreateSemaphore( int Id, unsigned long inisem, unsigned long maxsem ) ;
static SemErr DeleteSemaphore( int Id ) ;
static SemErr ReleaseSemaphore( int Id ) ;
static SemErr AcquireSemaphore( int Id, long Tout ) ;


/*=============================================================================
 *  Private global variables and functions
 */

static SemDat Semaphores[ SEM_ID_MAX ] ;


/*=============================================================================
 *  Global Function
 */

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_CreateMutex
 FUNCTION   : Create a mutex control object.
 PARAMETERS : pObj        : [IN ] The pointer to the mutex control object.
              ui32MutexID : [IN ] Mutex ID.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32 NCGSYS_CreateMutex(
    NCGvoid         **ppObj,
    NCGuint32       ui32MutexID )
{
    int semid ;
    int ret ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_CreateMutex" );

    if( ui32MutexID > 0 ){
        semid = ( int )ui32MutexID ;
        ret = CreateSemaphore( semid, 1ul, 1ul ) ;
        if( ret != ( int )SEM_ERR_OK ){
            NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_CreateMutex(1) Failed:CreateSemaphore(0x%08x).%s" )
            NCG_DEBUG_MAKE_MSG_PARAMETER( ret )
            NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
            NCG_DEBUG_MAKE_MSG_END();
            NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            return NCG_err_unknown ;
        }
    }
    else {
        semid = CreateSemaphore( -1, 1ul, 1ul ) ;
        if( semid < 0 ){
            NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_CreateMutex(2) Failed:CreateSemaphore(0x%08x).%s" )
            NCG_DEBUG_MAKE_MSG_PARAMETER( semid )
            NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
            NCG_DEBUG_MAKE_MSG_END();
            NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            return NCG_err_unknown ;
        }
    }
    *ppObj = ( NCGvoid *)semid ;

    return NCG_no_err ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_DestroyMutex
 FUNCTION   : Destroy the given mutex control object.
 PARAMETERS : pObj : [IN ] The pointer to the mutex control object.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_DestroyMutex(
    NCGvoid         *pObj )
{
    int semid ;
    SemErr ercd ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_DestroyMutex" );

    semid = ( int )pObj ;

    ercd = DeleteSemaphore( semid ) ;
    if( ercd != SEM_ERR_OK ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_DestroyMutex(1) Failed:DeleteSemaphore(0x%08x).%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( ercd )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }
    return NCG_no_err ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_AcquireMutex
 FUNCTION   : Acquire the mutex.
 PARAMETERS : pObj        : [IN ] The pointer to the mutex control object.
              ui32Flags   : [IN ] Flags.
              ui32Timeout : [IN ] Timeout value in milli seconds.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_AcquireMutex(
    NCGvoid         *pObj,
    NCGuint32       ui32Flags,
    NCGuint32       ui32Timeout )
{
    int semid ;
    long to_val ;
    SemErr ercd ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_AcquireMutex" );

    semid = ( int )pObj ;
    to_val = ( ui32Timeout == NCG_TIMEOUT_INFINITE ) ? -1L : ( long )ui32Timeout ;

    ercd = AcquireSemaphore( semid, to_val ) ;
    if( ercd != SEM_ERR_OK ){
        if( ercd == SEM_ERR_TIMEOUT ){
            NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_AcquireMutex(1) Failed:AcquireSemaphore(0x%08x).%s" )
            NCG_DEBUG_MAKE_MSG_PARAMETER( ercd )
            NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
            NCG_DEBUG_MAKE_MSG_END();
            NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            return NCG_err_wait_timeout ;
        }
        else{
            NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_AcquireMutex(2) Failed:AcquireSemaphore(0x%08x).%s" )
            NCG_DEBUG_MAKE_MSG_PARAMETER( ercd )
            NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
            NCG_DEBUG_MAKE_MSG_END();
            NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            return NCG_err_unknown ;
        }
    }
    return NCG_no_err ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGSYS_ReleaseMutex
 FUNCTION   : Release the mutex.
 PARAMETERS : pObj      : [IN ] The pointer to the mutex control object.
              ui32Flags : [IN ] Flags.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGSYS_ReleaseMutex(
    NCGvoid         *pObj,
    NCGuint32       ui32Flags )
{
    int semid ;
    SemErr ercd ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGSYS_ReleaseMutex" );

    semid = ( int )pObj ;

    ercd = ReleaseSemaphore( semid ) ;
    if( ercd != SEM_ERR_OK ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGSYS_ReleaseMutex(1) Failed:AcquireSemaphore(0x%08x).%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( ercd )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        return NCG_err_unknown ;
    }

    return NCG_no_err ;
}


/*=============================================================================
 *  Internal functions
 */

/*----------------------------------------------------------------------------
 NAME       : CreateSemaphore
 FUNCTION   : Create semaphore (automatic ID assignment).
 PARAMETERS : Id     : [IN ] Semaphore ID.
              inisem : [IN ] Initial semaphore resource count.
              maxsem : [IN ] Maximum semaphore resource count.
 RETURN     : Automatic Id (0~), specified ID (SEM_ERR_OK) or error code (<0).
------------------------------------------------------------------------------*/
static int 
CreateSemaphore(
    int             Id,
    unsigned long   inisem,
    unsigned long   maxsem )
{
    int i ;
    int IdNum ;
    SemDat *semdattbl ;
    static int first = 0 ;

    if( first == 0 ){
        NCGSYS_HostMemSet( Semaphores, 0, sizeof(Semaphores) ) ;
        first = 1 ;
    }
    if( SEM_ID_MAX <= Id ){
        return ( int )SEM_ERR_ID ;
    }

    IdNum = ( int )SEM_ERR_FULL ;
    if( Id < 0 ){
        /* Automatic ID assignment */
        semdattbl = &Semaphores[ 0 ] ;
        for( i = 0 ; i < SEM_ID_MAX ; i++ ){
            if( semdattbl->Used == 0 ){
                semdattbl->Used = 1u ;
                semdattbl->SemCnt = inisem ;
                semdattbl->MaxSem = maxsem ;
                IdNum = i ;
                break ;
            }
            semdattbl++ ;
        }
    }
    else{
        /* Specified ID */
        semdattbl = &Semaphores[ Id ] ;
        if( semdattbl->Used == 0 ){
            semdattbl->Used = 1u ;
            semdattbl->SemCnt = inisem ;
            semdattbl->MaxSem = maxsem ;
            IdNum = ( int )SEM_ERR_OK ;
        }
    }
    return IdNum ;
}

/*----------------------------------------------------------------------------
 NAME       : DeleteSemaphore
 FUNCTION   : Delete semaphore.
 PARAMETERS : Id : [IN ] Semaphore ID.
 RETURN     : Error code.
------------------------------------------------------------------------------*/
static SemErr 
DeleteSemaphore (
    int     Id )
{
    if ( (Id < 0) || (Id >= SEM_ID_MAX) ) {
        return SEM_ERR_ID ;
    }
    if ( Semaphores[ Id ].Used == 0 ) {
        return SEM_ERR_UNDEF ;
    }
    Semaphores[ Id ].Used = 0 ;

    return SEM_ERR_OK ;
}

/*----------------------------------------------------------------------------
 NAME       : ReleaseSemaphore
 FUNCTION   : Release semaphore resource.
 PARAMETERS : Id   : [IN ] Semaphore ID.
 RETURN     : Error code.
------------------------------------------------------------------------------*/
static SemErr 
ReleaseSemaphore(
    int     Id )
{
    SemDat *semdattbl ;

    if ( (Id < 0) || (Id >= SEM_ID_MAX) ) {
        return SEM_ERR_ID ;
    }

    semdattbl = &Semaphores[Id] ;
    if( semdattbl->Used == 0 ){
        return SEM_ERR_UNDEF ;
    }

    semdattbl->SemCnt++ ;
    if( semdattbl->SemCnt > semdattbl->MaxSem ){
        return SEM_ERR_OVERMAX ;
    }

    return SEM_ERR_OK ;
}

/*----------------------------------------------------------------------------
 NAME       : AcquireSemaphore
 FUNCTION   : Acquire semaphore with timeout.
 PARAMETERS : Id   : [IN ] Semaphore ID.
              Tout : [IN ] Waiting time.
 RETURN     : Error code.
------------------------------------------------------------------------------*/
static SemErr 
AcquireSemaphore(
    int     Id,
    long    Tout )
{
    volatile SemDat *semdattbl ;
    long i;
    int ToutErr = 1 ;

    if(( Id < 0 ) || ( Id >= SEM_ID_MAX )){
        return SEM_ERR_ID ;
    }

    semdattbl = &Semaphores[ Id ] ;
    if( semdattbl->Used == 0 ){
        return SEM_ERR_UNDEF ;
    }

    if( Tout < 0 ){     /* Wait forever */
        ToutErr = 0 ;
        while( 1 ){
            if( semdattbl->SemCnt > 0 ){
                semdattbl->SemCnt-- ;
                break ;
            }
        }
    }
    else{
        for( i = 0 ; i < Tout ; i++ ){
            if( semdattbl->SemCnt > 0 ){
                semdattbl->SemCnt-- ;
                ToutErr = 0 ;
                break ;
            }
            NCGSYS_Sleep(1);
        }
    }
    return (( ToutErr != 0 ) ? SEM_ERR_TIMEOUT : SEM_ERR_OK ) ;
}

/* -- end of file -- */
