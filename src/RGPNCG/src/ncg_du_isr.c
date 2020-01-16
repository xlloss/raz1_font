/******************************************************************************
* Copyright(c) 2010-2013 Renesas Electronics Corporation. All rights reserved.
*
* brief  : NCG Display Unit interrupt handling functions.
*
* author : Renesas Electronics Corporation
*
* history: 2010.10.08
*          - Created the initial code.
*          2011.01.20
*          - Added the attached/detached processing for display unit interrupt.
*          2011.03.07
*          - Fixed bug : NCGDU_Detach_ISR
*          2011.09.30
*          - Applied the countermeasure against VLine Interrupt issue.
*          - Modified in accordance with the update of VDC4 Driver.
*          2012.08.22
*          - Moved definition to "ncg_du_isr.h".
*            - NCGDU_VDC4_INT_LEVEL
*          2013.02.21
*          - Midified the function pointer variable in NCGDU_Attach_ISR.
*
*******************************************************************************/

/*=============================================================================
 * Includes
 */

#include    "ncg_defs.h"
#include    "ncg_debug.h"
#include    "ncg_du.h"
#include    "ncg_du_isr.h"

/* Depending on the build environment */
#include    "r_typedefs.h"
#include    "intc.h"
#include    "r_vdc5.h"
#include    "r_vdc5_user.h"


/*=============================================================================
 * Internal definitions
 */


/*=============================================================================
 *  Prototyping of internal functions
 */

static void IntCallbackFuncCH1( vdc5_int_type_t int_type );
static void IntCallbackFuncCH2( vdc5_int_type_t int_type );


/*=============================================================================
 *  Private global variables and functions
 */

static NCGboolean   NCGDU_ISR_Initialized   = NCG_FALSE ;
static NCGDUISRfp   NCGDU_pDisplayInterrupt = NCG_NULL ;


/*=============================================================================
 *  Global Function
 */

/*----------------------------------------------------------------------------
 NAME       : NCGDU_Attach_ISR
 FUNCTION   : Attach the interrrupt function for display unit.
 PARAMETERS : ui32Display  : [IN ] Display channel.
              pfnInterrupt : [IN ] The pointer to interrrupt function.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGDU_Attach_ISR (
    NCGbitfield     ui32Display,
    NCGDUISRfp      pfnInterrupt)
{
    NCGint32        rc_val = NCG_no_err;

    /* Depending on the build environment */
    void            (* func)(uint32_t);
    vdc5_channel_t  ch;
    uint16_t        int_id;
    vdc5_int_t      interrupt;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGDU_Attach_ISR" );

    NCG_ASSERT( pfnInterrupt != NCG_NULL ) ;

    if( NCGDU_ISR_Initialized != NCG_FALSE ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_Attach_ISR(1) Initialized.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        rc_val = NCG_err_isr_management_failed ;
    }
    else {
        NCGDU_ISR_Initialized   = NCG_TRUE ;
        NCGDU_pDisplayInterrupt = pfnInterrupt ;

        /* Attach the interrrupt function for display unit */
        if ( ui32Display & NCGDU_CH1 ) {
            ch     = VDC5_CHANNEL_0;
            int_id = INTC_ID_GR3_VLINE0;

            func = R_VDC5_GetISR( ch, VDC5_INT_TYPE_VLINE );

            R_INTC_RegistIntFunc( int_id, func );

            R_INTC_SetPriority( int_id, NCGDU_VDC5_INT_PRIORITY );
            R_INTC_Enable( int_id );

            /* Interrupt parameter */
            interrupt.type      = VDC5_INT_TYPE_VLINE;  /* Interrupt type */
            interrupt.callback  = IntCallbackFuncCH1;   /* Callback function pointer */
            interrupt.line_num  = 0u;                   /* Line number */

            /* Set interrupt service routine */
            R_VDC5_CallbackISR( ch, &interrupt );
        }

        if ( ui32Display & NCGDU_CH2 ) {
            ch     = VDC5_CHANNEL_1;
            int_id = INTC_ID_GR3_VLINE1;

            func = R_VDC5_GetISR( ch, VDC5_INT_TYPE_VLINE );

            R_INTC_RegistIntFunc( int_id, func );

            R_INTC_SetPriority( int_id, NCGDU_VDC5_INT_PRIORITY );
            R_INTC_Enable( int_id );

            /* Interrupt parameter */
            interrupt.type      = VDC5_INT_TYPE_VLINE;  /* Interrupt type */
            interrupt.callback  = IntCallbackFuncCH2;   /* Callback function pointer */
            interrupt.line_num  = 0u;                   /* Line number */

            /* Set interrupt service routine */
            R_VDC5_CallbackISR( ch, &interrupt );
        }

    }

    return rc_val ;
}

/*----------------------------------------------------------------------------
 NAME       : NCGDU_Attach_ISR
 FUNCTION   : Detach the interrrupt function for display unit.
 PARAMETERS : ui32Display  : [IN ] Display channel.
              pfnInterrupt : [IN ] The pointer to interrrupt function.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGDU_Detach_ISR (
    NCGbitfield     ui32Display,
    NCGDUISRfp      pfnInterrupt)
{
    NCGint32        rc_val = NCG_no_err;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGDU_Detach_ISR" );

    if( NCGDU_ISR_Initialized == NCG_FALSE ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_Detach_ISR(1) Not initialized.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        rc_val = NCG_err_isr_management_failed ;
    }
    else {
        NCGDU_ISR_Initialized = NCG_FALSE ;

        if( pfnInterrupt == NCGDU_pDisplayInterrupt ){
            if ( ui32Display & NCGDU_CH1 ) {
                R_INTC_Disable( INTC_ID_GR3_VLINE0 );
            }
            if ( ui32Display & NCGDU_CH2 ) {
                R_INTC_Disable( INTC_ID_GR3_VLINE1 );
            }

            NCGDU_pDisplayInterrupt = NCG_NULL ;
        }
        else {
            NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_Detach_ISR(2) Miss match function pointer.%s" )
            NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
            NCG_DEBUG_MAKE_MSG_END();
            NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            rc_val = NCG_err_isr_management_failed ;
        }
    }
    return rc_val ;
}


/*=============================================================================
 *  Internal functions
 */

/*----------------------------------------------------------------------------
 NAME       : IntCallbackFuncCH1
 FUNCTION   : The Interrrupt from display ch1.
 PARAMETERS : int_type  : [IN ] Interrupt type.
 RETURN     : None.
------------------------------------------------------------------------------*/
static void IntCallbackFuncCH1(
    vdc5_int_type_t         int_type )
{
    NCG_UNREFERENCED_PARAMETER(int_type);

    if( NCGDU_pDisplayInterrupt != NCG_NULL ){
        NCGDU_pDisplayInterrupt( NCGDU_CH1 );
    }
    return ;
}

/*----------------------------------------------------------------------------
 NAME       : IntCallbackFuncCH2
 FUNCTION   : The Interrrupt from display ch2.
 PARAMETERS : int_type  : [IN ] Interrupt type.
 RETURN     : None.
------------------------------------------------------------------------------*/
static void IntCallbackFuncCH2(
    vdc5_int_type_t         int_type )
{
    NCG_UNREFERENCED_PARAMETER(int_type);

    if( NCGDU_pDisplayInterrupt != NCG_NULL ){
        NCGDU_pDisplayInterrupt( NCGDU_CH2 );
    }
    return ;
}

/* -- end of file -- */
