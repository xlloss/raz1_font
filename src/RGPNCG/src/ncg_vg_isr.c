/******************************************************************************
* Copyright(c) 2010-2013 Renesas Electronics Corporation. All rights reserved.
*
* brief  : R-GPVG control functions
*
* author : Renesas Electronics Corporation
*
* history: 2010.10.08
*          - Created the initial code.
*          2011.03.07
*          - Fixed bug : NCGVG_Detach_ISR / [R-GPVG_common_P_033]
*          2012.08.22
*          - Moved definition to "ncg_vg_isr.h".
*            - RGPVG_INT_LEVEL
*          - Moved definition to "ncg_register.h".
*            - RTIP_VG_BASE
*            - VG_REG_VG_ISR
*          2012.08.22
*          - Added error message in the debug mode.
*          - Update all comments
*          2013.02.21
*          - Modified the argument of the NCGVG_RGPVG_ISR.
*          - Modified the call to R_INTC_RegistIntFunc function
*            by NCGVG_Attach_ISR.
*
*******************************************************************************/


/*=============================================================================
 * Includes
 */

#include    "ncg_defs.h"
#include    "ncg_debug.h"
#include    "ncg_vg_isr.h"

/* Depending on the build environment */
#include    "r_typedefs.h"
#include    "intc.h"


/*=============================================================================
 * Internal definitions
 */


/*=============================================================================
 *  Prototyping of internal functions
 */
static void NCGVG_RGPVG_ISR( uint32_t int_sense ) ;


/*=============================================================================
 *  Private global variables and functions
 */
static NCGboolean NCGVG_ISR_Initialized  = NCG_FALSE ;
static NCGVGISRfp NCGVG_pRGPVG_Interrupt = NCG_NULL ;


/*=============================================================================
 *  Global Function
 */

/*----------------------------------------------------------------------------
 NAME       : NCGVG_Attach_ISR
 FUNCTION   : Attach the function pointer of OpenVG ISR.
 PARAMETERS : pfnInterrupt : [IN ] The pointer to the function of OpenVG ISR.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32 
NCGVG_Attach_ISR(
    NCGVGISRfp      pfnInterrupt )
{
    NCGint32        rc_val = NCG_no_err;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGVG_Attach_ISR" );

    NCG_ASSERT(pfnInterrupt != NCG_NULL);

    if( NCGVG_ISR_Initialized != NCG_FALSE ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGVG_Attach_ISR(1) Initialized.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        rc_val = NCG_err_isr_management_failed ;
    }
    else {
        NCGVG_ISR_Initialized  = NCG_TRUE ;
        NCGVG_pRGPVG_Interrupt = pfnInterrupt ;

        /* Regist function */
        R_INTC_RegistIntFunc( NCGVG_INT0_VECT, NCGVG_RGPVG_ISR );
        R_INTC_RegistIntFunc( NCGVG_INT1_VECT, NCGVG_RGPVG_ISR );
        R_INTC_RegistIntFunc( NCGVG_INT2_VECT, NCGVG_RGPVG_ISR );
        R_INTC_RegistIntFunc( NCGVG_INT3_VECT, NCGVG_RGPVG_ISR );

        /* set priority */
        R_INTC_SetPriority( NCGVG_INT0_VECT, NCGVG_INT_LEVEL );
        R_INTC_SetPriority( NCGVG_INT1_VECT, NCGVG_INT_LEVEL );
        R_INTC_SetPriority( NCGVG_INT2_VECT, NCGVG_INT_LEVEL );
        R_INTC_SetPriority( NCGVG_INT3_VECT, NCGVG_INT_LEVEL );

        /* Enable interrupt from Renesas OpenVG library */
        R_INTC_Enable( NCGVG_INT0_VECT );
        R_INTC_Enable( NCGVG_INT1_VECT );
        R_INTC_Enable( NCGVG_INT2_VECT );
        R_INTC_Enable( NCGVG_INT3_VECT );
    }


    return rc_val;
}


/*----------------------------------------------------------------------------
 NAME       : NCGVG_Detach_ISR
 FUNCTION   : Detach the function pointer of OpenVG ISR.
 PARAMETERS : pfnInterrupt : [IN ] The pointer to the function of OpenVG ISR.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGVG_Detach_ISR(
    NCGVGISRfp      pfnInterrupt )
{
    NCGint32        rc_val = NCG_no_err;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGVG_Detach_ISR" );

    NCG_ASSERT(pfnInterrupt != NCG_NULL);

    if ( NCGVG_ISR_Initialized == NCG_FALSE ) {
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGVG_Detach_ISR(1) Not initialized.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        rc_val = NCG_err_isr_management_failed;
    }
    else {
        NCGVG_ISR_Initialized = NCG_FALSE ;

        if( pfnInterrupt == NCGVG_pRGPVG_Interrupt ) {
            /* Disable interrupt from Renesas OpenVG library */
            R_INTC_Disable( NCGVG_INT3_VECT );
            R_INTC_Disable( NCGVG_INT2_VECT );
            R_INTC_Disable( NCGVG_INT1_VECT );
            R_INTC_Disable( NCGVG_INT0_VECT );

            /* Unregist function */
            /* Unregist is not support in the current version of RZ/A1 OpenVG1.1 sample environment.
            Userdef_INTC_RegistIntFunc( NCGVG_INT0_VECT, NULL );
            Userdef_INTC_RegistIntFunc( NCGVG_INT1_VECT, NULL );
            Userdef_INTC_RegistIntFunc( NCGVG_INT2_VECT, NULL );
            Userdef_INTC_RegistIntFunc( NCGVG_INT3_VECT, NULL );
            */

            NCGVG_pRGPVG_Interrupt = NCG_NULL ;
        }
        else{
            NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGVG_Detach_ISR(2) Miss match function pointer.%s" )
            NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
            NCG_DEBUG_MAKE_MSG_END();
            NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            rc_val = NCG_err_isr_management_failed;
        }
    }

    return rc_val;
}


/*=============================================================================
 *  Internal functions
 */

/*-----------------------------------------------------------------------------
 NAME       : NCGVG_RGPVG_ISR
 FUNCTION   : The Interrrupt from OpenVG.
 PARAMETERS : int_sense : [IN ] Unreferenced parameter.
 RETURN     : None.
-----------------------------------------------------------------------------*/
static void NCGVG_RGPVG_ISR( uint32_t int_sense )
{
    NCGuint32 ret ;

    NCG_DEBUG_PRINT_STRING( "[LOG] NCGVG_RGPVG_ISR" );

    NCG_UNREFERENCED_PARAMETER(int_sense);

    if( NCGVG_pRGPVG_Interrupt == NCG_NULL ){
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][INFO]NCGVG_RGPVG_ISR Not initialized.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
    }
    else {
        ret = NCGVG_pRGPVG_Interrupt() ;
        if ( ret != 0 ) {
            NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGVG_RGPVG_ISR(0x%08x)%s" )
            NCG_DEBUG_MAKE_MSG_PARAMETER( ret )
            NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
            NCG_DEBUG_MAKE_MSG_END();
            NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        }
    }

    return ;
}

/* -- end of file -- */
