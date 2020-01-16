/******************************************************************************
* Copyright(c) 2010-2013 Renesas Electronics Corporation. All rights reserved.
*
* brief  : DU Control Functions.
*
* author : Renesas Electronics Corporation
*
* history: 2013.01.09
*          - Created the initial code.
*          - Information
*              NCGDU_CH1 is related to VDC5 channel 0
*              NCGDU_CH2 is related to VDC5 channel 1
*          - Restriction
*            Don't change the following values in this version.
*             target channel  : VDC5_CHANNEL_1
*             target layer_id : VDC5_LAYER_ID_2_RD;
*          2013.02.21
*          - Modified following functions.
*              NCGDU_SetPlaneModeEx
*              ncgdu_get_plane_info
*          2013.04.12
*          - Added NCGDU_ARGB4444 format.
*
*******************************************************************************/

/*=============================================================================
 * Includes
 */

#include "ncg_defs.h"
#include "ncg_debug.h"

#include "ncg_du.h"
#include "ncg_state.h"

/* Depending on the build environment */
#include "r_typedefs.h"

#include "r_vdc5.h"
#include "r_vdc5_user.h"

#include "lcd_panel.h"
#include "iodefine.h"

#define 	VG_BLEND			0

/*=============================================================================
 * Internal definitions
 */

#define NCGDU_PLANE_CH1_MAX             (4)
#define NCGDU_PLANE_CH2_MAX             (4)

#define NCGDU_SET_PM_IGNORED            (0)
#define NCGDU_SET_PM_CHANGE             (1)
#define NCGDU_SET_PM_START              (2)
#define NCGDU_SET_PM_STOP               (3)
#define NCGDU_SET_PM_RESTART            (4)


/*=============================================================================
 * Internal definitions
 */
 
 /* Depending on the build environment */
typedef struct
{
    vdc5_panel_clksel_t     panel_icksel;   /* Panel clock select */
    vdc5_panel_clk_dcdr_t   panel_dcdr;     /* Panel clock frequency division ratio */
    uint16_t                res_fv;         /* Free-running Vsync period setting */
    uint16_t                res_fh;         /* Hsync period setting */
    vdc5_period_rect_t      res_f;          /* Full-screen enable control */
    uint16_t                tcon_half;      /* TCON reference timing, 1/2fH timing */
    uint16_t                tcon_offset;    /* TCON reference timing, offset Hsync signal timing */
    vdc5_lcd_outformat_t    out_format;     /* LCD output format select */
    vdc5_edge_t             outcnt_lcd_edge;/* Output phase control of LCD_DATA23 to LCD_DATA0 pin */
} lcd_dependent_param_t;


/*=============================================================================
 *  Prototyping of internal functions
 */

static NCGvoid ncgdu_init_plane_info( NCGvoid );
static NCGDU_PlaneMode* ncgdu_get_plane_info( NCGbitfield ui32Display, NCGint32 i32Plane );

 /* Depending on the build environment */
static void init_func (uint32_t user_num);
static void quit_func (uint32_t user_num);


/*=============================================================================
 *  Private global variables and functions
 */

/* NCG DU initialization flag */
static NCGboolean           NCGDU_Initialized = NCG_FALSE;

/* NCG DU plane management information */
static NCGDU_PlaneMode      NCGDU_ptPlaneMode_ch1[NCGDU_PLANE_CH1_MAX];
static NCGDU_PlaneMode      NCGDU_ptPlaneMode_ch2[NCGDU_PLANE_CH2_MAX];

 /* Depending on the build environment */
#define VDC5_CHANNEL_NUM            (2)

static const lcd_dependent_param_t lcd_dependent_param[VDC5_CHANNEL_NUM] =
{
    {
        LCD_CH0_PANEL_CLK,                  /* Panel clock select */
        LCD_CH0_PANEL_CLK_DIV,              /* Panel clock frequency division ratio */
        (uint16_t)LCD_CH0_SIG_FV,           /* Free-running Vsync period setting */
        (uint16_t)LCD_CH0_SIG_FH,           /* Hsync period setting */
        {   /* Full-screen enable control */
            (uint16_t)LCD_CH0_DISP_VS,
            (uint16_t)LCD_CH0_DISP_VW,
            (uint16_t)LCD_CH0_DISP_HS,
            (uint16_t)LCD_CH0_DISP_HW
        },
        (uint16_t)LCD_CH0_TCON_HALF,        /* TCON reference timing, 1/2fH timing */
        (uint16_t)LCD_CH0_TCON_OFFSET,      /* TCON reference timing, offset Hsync signal timing */
        LCD_CH0_OUT_FORMAT,                 /* LCD output format select */
        LCD_CH0_OUT_EDGE                    /* Output phase control of LCD_DATA23 to LCD_DATA0 pin */
    },
    {
        LCD_CH1_PANEL_CLK,                  /* Panel clock select */
        LCD_CH1_PANEL_CLK_DIV,              /* Panel clock frequency division ratio */
        (uint16_t)LCD_CH1_SIG_FV,           /* Free-running Vsync period setting */
        (uint16_t)LCD_CH1_SIG_FH,           /* Hsync period setting */
        {   /* Full-screen enable control */
            (uint16_t)LCD_CH1_DISP_VS,
            (uint16_t)LCD_CH1_DISP_VW,
            (uint16_t)LCD_CH1_DISP_HS,
            (uint16_t)LCD_CH1_DISP_HW
        },
        (uint16_t)LCD_CH1_TCON_HALF,        /* TCON reference timing, 1/2fH timing */
        (uint16_t)LCD_CH1_TCON_OFFSET,      /* TCON reference timing, offset Hsync signal timing */
        LCD_CH1_OUT_FORMAT,                 /* LCD output format select */
        LCD_CH1_OUT_EDGE                    /* Output phase control of LCD_DATA23 to LCD_DATA0 pin */
    }
};

/*=============================================================================
 *  Global Function
 */

/*----------------------------------------------------------------------------
 NAME       : NCGDU_Init
 FUNCTION   : Initialize Display Unit.
 PARAMETERS : pDUInfo : [IN ] The pointer to the initialization information struct.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/

NCGint32
NCGDU_Init1  (
    PNCGDUINFO pDUInfo)
{
   NCGint32            rc_val = NCG_no_err;
    NCGbitfield         display;

    /* Depending on the build environment */
    vdc5_error_t        err_code;
    vdc5_channel_t      ch;

    vdc5_init_t         vdc5_init;
    vdc5_sync_ctrl_t    sync_ctrl;
    vdc5_output_t       output;

    lcd_dependent_param_t * param;

    NCG_UNREFERENCED_PARAMETER( pDUInfo );	

    if ( NCGDU_Initialized == NCG_FALSE ) {
        NCGDU_Initialized = NCG_TRUE;

        ncgdu_init_plane_info();

        display = NCGDU_QueryDisplayUsage();
        
        printf("SIMPLE .....NCGDU_Init  NCGDU_Initialized\n");
           if ( display & NCGDU_CH1 ) {
            ch = VDC5_CHANNEL_0;
            param = (lcd_dependent_param_t*)&lcd_dependent_param[ch];
          }
          
            /* Initialization parameter */
            vdc5_init.panel_icksel   = param->panel_icksel;      /* Panel clock select */
            vdc5_init.panel_dcdr     = param->panel_dcdr;        /* Panel clock frequency division ratio */
            vdc5_init.lvds           = GRAPHICS_GetLvdsParam(ch);/* LVDS parameter */

            /* Initialize */
            err_code = R_VDC5_Initialize( ch, &vdc5_init, init_func, (uint32_t)ch );
            if ( err_code != VDC5_OK ) {
                /* error */
                rc_val = NCG_err_unknown;

                NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_Init Failed:R_VDC5_Initialize ch1(%08x).%s" )
                NCG_DEBUG_MAKE_MSG_PARAMETER( err_code )
                NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                NCG_DEBUG_MAKE_MSG_END();
                NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
                return rc_val;
            }
      }
    return rc_val;
}   
    
NCGint32
NCGDU_Init  (
    PNCGDUINFO pDUInfo)
{
    NCGint32            rc_val = NCG_no_err;
    NCGbitfield         display;

    /* Depending on the build environment */
    vdc5_error_t        err_code;
    vdc5_channel_t      ch;

    vdc5_init_t         vdc5_init;
    vdc5_sync_ctrl_t    sync_ctrl;
    vdc5_output_t       output;

    lcd_dependent_param_t * param;

    NCG_UNREFERENCED_PARAMETER( pDUInfo );

    /* Check the NCG DU initialization flag */
    if ( NCGDU_Initialized == NCG_FALSE ) {
        NCGDU_Initialized = NCG_TRUE;

        ncgdu_init_plane_info();

        display = NCGDU_QueryDisplayUsage();

        if ( display & NCGDU_CH2 ) {
            ch = VDC5_CHANNEL_1;

            param = (lcd_dependent_param_t*)&lcd_dependent_param[ch];

            /* Initialization parameter */
            vdc5_init.panel_icksel   = param->panel_icksel;      /* Panel clock select */
            vdc5_init.panel_dcdr     = param->panel_dcdr;        /* Panel clock frequency division ratio */
            vdc5_init.lvds           = GRAPHICS_GetLvdsParam(ch);/* LVDS parameter */

            /* Initialize */
            err_code = R_VDC5_Initialize( ch, &vdc5_init, init_func, (uint32_t)ch );
            if ( err_code != VDC5_OK ) {
                /* error */
                rc_val = NCG_err_unknown;

                NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_Init Failed:R_VDC5_Initialize ch1(%08x).%s" )
                NCG_DEBUG_MAKE_MSG_PARAMETER( err_code )
                NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                NCG_DEBUG_MAKE_MSG_END();
                NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
                return rc_val;
            }

            /* Sync signal control */
            sync_ctrl.res_vs_sel    = VDC5_ON;                      /* Free-running Vsync ON/OFF */
            sync_ctrl.res_vs_in_sel = VDC5_RES_VS_IN_SEL_SC0;       /* SC_RES_VS_IN_SEL */
            sync_ctrl.res_fv        = param->res_fv;                /* Free-running Vsync period setting */
            sync_ctrl.res_fh        = param->res_fh;                /* Hsync period setting */
            sync_ctrl.res_vsdly     = 0u;                           /* Vsync signal delay control */
                /* Full-screen enable control */
            sync_ctrl.res_f.vs      = param->res_f.vs;
            sync_ctrl.res_f.vw      = param->res_f.vw;
            sync_ctrl.res_f.hs      = param->res_f.hs;
            sync_ctrl.res_f.hw      = param->res_f.hw;
            sync_ctrl.vsync_cpmpe   = NULL;                         /* Vsync signal compensation */

            /* Sync control */
            err_code = R_VDC5_SyncControl(ch, &sync_ctrl);
            if ( err_code != VDC5_OK ) {
                /* error */
                rc_val = NCG_err_unknown;

                NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_Init Failed:R_VDC5_SyncControl ch1(%08x).%s" )
                NCG_DEBUG_MAKE_MSG_PARAMETER( err_code )
                NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                NCG_DEBUG_MAKE_MSG_END();
                NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
                return rc_val;
            }

            /* Output parameter */
            output.tcon_half        = param->tcon_half;         /* TCON reference timing, 1/2fH timing */
            output.tcon_offset      = param->tcon_offset;       /* TCON reference timing, offset Hsync signal timing */
                /* LCD TCON timing setting */
            GRAPHICS_SetLcdTconSettings(ch, output.outctrl);
            output.outcnt_lcd_edge  = param->outcnt_lcd_edge;       /* Output phase control of LCD_DATA23 to LCD_DATA0 pin */
            output.out_endian_on    = VDC5_OFF;                     /* Bit endian change ON/OFF control */
            output.out_swap_on      = VDC5_OFF;                      /* B/R signal swap ON/OFF control */	
            output.out_format       = param->out_format;            /* LCD output format select */
            output.out_frq_sel      = VDC5_LCD_PARALLEL_CLKFRQ_1;   /* Clock frequency control */
            output.out_dir_sel      = VDC5_LCD_SERIAL_SCAN_FORWARD; /* Scan direction select */
            output.out_phase        = VDC5_LCD_SERIAL_CLKPHASE_0;   /* Clock phase adjustment */
            output.bg_color         = 0x00000000uL;                 /* Background color in 24-bit RGB color format */

            /* Display output */
            err_code = R_VDC5_DisplayOutput( ch, &output );
            if ( err_code != VDC5_OK ) {
                /* error */
                rc_val = NCG_err_unknown;

                NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_Init Failed:R_VDC5_DisplayOutput ch2(%08x).%s" )
                NCG_DEBUG_MAKE_MSG_PARAMETER( err_code )
                NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                NCG_DEBUG_MAKE_MSG_END();
                NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
                return rc_val;
            }
        }

        if ( display & NCGDU_CH1 )
        {
            ch = VDC5_CHANNEL_0;

            param = (lcd_dependent_param_t*)&lcd_dependent_param[ch];

            /* Initialization parameter */
            vdc5_init.panel_icksel   = param->panel_icksel;      /* Panel clock select */
            vdc5_init.panel_dcdr     = param->panel_dcdr;        /* Panel clock frequency division ratio */
            vdc5_init.lvds           = GRAPHICS_GetLvdsParam(ch);/* LVDS parameter */

            /* Initialize */
            err_code = R_VDC5_Initialize( ch, &vdc5_init, init_func, (uint32_t)ch );
            if ( err_code != VDC5_OK ) {
                /* error */
                rc_val = NCG_err_unknown;

                NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_Init Failed:R_VDC5_Initialize ch2(%08x).%s" )
                NCG_DEBUG_MAKE_MSG_PARAMETER( err_code )
                NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                NCG_DEBUG_MAKE_MSG_END();
                NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
                return rc_val;
            }

            /* Sync signal control */
            sync_ctrl.res_vs_sel    = VDC5_ON;                      /* Free-running Vsync ON/OFF */
            sync_ctrl.res_vs_in_sel = VDC5_RES_VS_IN_SEL_SC0;       /* SC_RES_VS_IN_SEL */
            sync_ctrl.res_fv        = param->res_fv;                /* Free-running Vsync period setting */
            sync_ctrl.res_fh        = param->res_fh;                /* Hsync period setting */
            sync_ctrl.res_vsdly     = 0u;                           /* Vsync signal delay control */
                /* Full-screen enable control */
            sync_ctrl.res_f.vs      = param->res_f.vs;
            sync_ctrl.res_f.vw      = param->res_f.vw;
            sync_ctrl.res_f.hs      = param->res_f.hs;
            sync_ctrl.res_f.hw      = param->res_f.hw;
            sync_ctrl.vsync_cpmpe   = NULL;                         /* Vsync signal compensation */

            /* Sync control */
            err_code = R_VDC5_SyncControl(ch, &sync_ctrl);
            if ( err_code != VDC5_OK ) {
                /* error */
                rc_val = NCG_err_unknown;

                NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_Init Failed:R_VDC5_SyncControl ch2(%08x).%s" )
                NCG_DEBUG_MAKE_MSG_PARAMETER( err_code )
                NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                NCG_DEBUG_MAKE_MSG_END();
                NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
                return rc_val;
            }

            /* Output parameter */
            output.tcon_half        = param->tcon_half;         /* TCON reference timing, 1/2fH timing */
            output.tcon_offset      = param->tcon_offset;       /* TCON reference timing, offset Hsync signal timing */
                /* LCD TCON timing setting */
            GRAPHICS_SetLcdTconSettings(ch, output.outctrl);
            output.outcnt_lcd_edge  = param->outcnt_lcd_edge;       /* Output phase control of LCD_DATA23 to LCD_DATA0 pin */
            output.out_endian_on    = VDC5_OFF;                     /* Bit endian change ON/OFF control */
            output.out_swap_on      = VDC5_ON;                      /* ON for RSK, OFF for Genmai, RSK2Genmai GenerallaiModify B/R signal swap ON/OFF control */
            output.out_format       = param->out_format;            /* LCD output format select */
            output.out_frq_sel      = VDC5_LCD_PARALLEL_CLKFRQ_1;   /* Clock frequency control */
            output.out_dir_sel      = VDC5_LCD_SERIAL_SCAN_FORWARD; /* Scan direction select */
            output.out_phase        = VDC5_LCD_SERIAL_CLKPHASE_0;   /* Clock phase adjustment */
            output.bg_color         = 0x00000000uL;                 /* Background color in 24-bit RGB color format */

            /* Display output */
            err_code = R_VDC5_DisplayOutput( ch, &output );
            if ( err_code != VDC5_OK ) {
                /* error */
                rc_val = NCG_err_unknown;

                NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_Init Failed:R_VDC5_DisplayOutput ch2(%08x).%s" )
                NCG_DEBUG_MAKE_MSG_PARAMETER( err_code )
                NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                NCG_DEBUG_MAKE_MSG_END();
                NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
                return rc_val;
            }
        

        
        }
    }
    else {
        /* No Operation */
    }

    return rc_val;
}

/*----------------------------------------------------------------------------
 NAME       : NCGDU_DeInit
 FUNCTION   : Finalize Display Unit.
 PARAMETERS : pDUInfo : [IN ] The pointer to the initialization information struct.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGDU_DeInit (
    PNCGDUINFO      pDUInfo )
{
    NCGint32        rc_val = NCG_no_err;
    vdc5_error_t    err_code;
    vdc5_channel_t  ch;
    NCGbitfield     display;

    NCG_UNREFERENCED_PARAMETER(pDUInfo);

    if ( NCGDU_Initialized != NCG_FALSE ) {
        display = NCGDU_QueryDisplayUsage();

        /* Termination proccess
            Statement is unreachable
         */
        if ( display & NCGDU_CH1 ) {
            ch = VDC5_CHANNEL_0;

            err_code = R_VDC5_Terminate(ch, quit_func, (uint32_t)ch);
            if ( err_code != VDC5_OK ) {
                /* error */
                rc_val = NCG_err_unknown;

                NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_DeInit Failed:R_VDC5_Terminate ch1(%08x).%s" )
                NCG_DEBUG_MAKE_MSG_PARAMETER( err_code )
                NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                NCG_DEBUG_MAKE_MSG_END();
                NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            }
        }
        if ( display & NCGDU_CH2 ) {
            ch = VDC5_CHANNEL_1;

            /* Terminate */
            err_code = R_VDC5_Terminate( ch, quit_func, (uint32_t)ch );
            if ( err_code != VDC5_OK ) {
                /* error */
                rc_val = NCG_err_unknown;

                NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_DeInit Failed:R_VDC5_Terminate ch2(%08x).%s" )
                NCG_DEBUG_MAKE_MSG_PARAMETER( err_code )
                NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                NCG_DEBUG_MAKE_MSG_END();
                NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            }
        }
        NCGDU_Initialized = NCG_FALSE;
    }

    return rc_val;
}

/*----------------------------------------------------------------------------
 NAME       : NCGDU_QueryDisplayUsage
 FUNCTION   : Set the buffer address to the DU plane.
 PARAMETERS : None.
 RETURN     : Usage value for display unit.
------------------------------------------------------------------------------*/
NCGbitfield
NCGDU_QueryDisplayUsage ( NCGvoid )
{
    NCGbitfield     usage;

    /* TODO:
     *  select display channel
     *    NCGDU_CH1, NCGDU_CH2 or (NCGDU_CH1 | NCGDU_CH2)
     */
//    usage = NCGDU_CH1 | NCGDU_CH2;												// VDC channel 0 and channel 1, 0 will be output
//    usage =  NCGDU_CH2;																	// output VDC channel 1 only
      usage =  NCGDU_CH1;																	// output VDC channel 0 only


    return usage;
}

/*----------------------------------------------------------------------------
 NAME       : NCGDU_SetAddress
 FUNCTION   : Set the buffer address to the DU plane.
 PARAMETERS : i32Plane           : [IN ] DU plane ID.
              ui32CurrentAddress : [IN ] Buffer address.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGDU_SetAddress (
    NCGint32    i32Plane,
    NCGuint32   ui32CurrentAddress)
{
    NCGint32 rc_val = NCG_no_err;

    /* No Implemetation          */
    /* EGL and OpenVG is unused. */

    return rc_val;
}

/*----------------------------------------------------------------------------
 NAME       : NCGDU_SetPlaneModeEx
 FUNCTION   : Set the DU plane.
 PARAMETERS : ui32Display : [IN ] DU cannel.
              i32Plane    : [IN ] DU plane ID.
              pDUInfo     : [IN ] The pointer to the DU plane information struct.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGDU_SetPlaneModeEx (
    NCGbitfield         ui32Display,
    NCGint32            i32Plane,
    NCGDU_PlaneMode     *pPlaneMode)
{
    NCGint32                rc_val = NCG_no_err;

    NCGint32                update = NCGDU_SET_PM_CHANGE;

    vdc5_channel_t          ch;
    vdc5_layer_id_t         layer_id;

    vdc5_error_t            err_code;
    vdc5_start_t            start;
    vdc5_read_t             read;

    vdc5_read_chg_t         read_chg;
    vdc5_gr_disp_sel_t      gr_disp_sel;

    NCGDU_PlaneMode         *ptPlaneMode;
    NCGDU_PlaneMode         tBackup;

    lcd_dependent_param_t   *param;

//	LedOn();

    ptPlaneMode = ncgdu_get_plane_info( ui32Display, i32Plane );
    if ( ptPlaneMode == NULL ) {
        /* error */
        rc_val = NCG_err_bad_parameter;

        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_SetPlaneModeEx(1) Failed:bad parameter.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
    }
    else if ( pPlaneMode == NULL ) {
        /* error */
        rc_val = NCG_err_bad_parameter;

        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_SetPlaneModeEx(2) Failed:bad parameter.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
    }
    else if ( pPlaneMode->ui32Flags != 0 ) {
        /* channel select */
        if ( ui32Display & NCGDU_CH2 ) {
            ch       = VDC5_CHANNEL_1;
        }
        else {
            ch       = VDC5_CHANNEL_0;
        }

        /* layer select */
        if ( i32Plane == NCGDU_PLANE2 ) {
            layer_id = VDC5_LAYER_ID_1_RD;
        }
        else if ( i32Plane == NCGDU_PLANE3 ) {
            layer_id = VDC5_LAYER_ID_2_RD;
        }
        else if ( i32Plane == NCGDU_PLANE4 ) {
            layer_id = VDC5_LAYER_ID_3_RD;
        }
        else {
            layer_id = VDC5_LAYER_ID_0_RD;
        }

				layer_id = VDC5_LAYER_ID_1_RD;        
//      printf("Running NCGDU_SetPlaneModeEx function call layer_id %x, Plane %x, pPlaneMode %x\n",layer_id,i32Plane,pPlaneMode);
    
        /* Get target LCD parameter */
        param = (lcd_dependent_param_t*)&lcd_dependent_param[ch];

        /* copy current data */
        tBackup = *ptPlaneMode;

        if ( pPlaneMode->ui32Flags & NCGDU_PM_ENABLE ) {
            if ( ptPlaneMode->i32Enable != pPlaneMode->i32Enable ) {
                ptPlaneMode->i32Enable = pPlaneMode->i32Enable;
                if ( ptPlaneMode->i32Enable == NCGDU_ENABLE ) {
                    update = NCGDU_SET_PM_START;
                }
                else if ( tBackup.i32Enable != NCGDU_ENABLE ) {
                    /* This layer has not shown. */
                    /* So, VDC5 is not necessary to update. */
                    update = NCGDU_SET_PM_IGNORED;
                }
                else {
                    update = NCGDU_SET_PM_STOP;
                }
            }
        }
        else {
            if ( ptPlaneMode->i32Enable != NCGDU_ENABLE ) {
                /* This layer has not shown. */
                /* So, VDC5 is not necessary to update. */
                update = NCGDU_SET_PM_IGNORED;
            }
        }

        if ( pPlaneMode->ui32Flags & NCGDU_PM_PIXEL_FORMAT ) {
            if ( ptPlaneMode->ePixelFormat != pPlaneMode->ePixelFormat ) {
                ptPlaneMode->ePixelFormat = pPlaneMode->ePixelFormat;
                if ( update == NCGDU_SET_PM_CHANGE ) {
                    update = NCGDU_SET_PM_RESTART;
                }
            }
        }
        /* Read data parameter */
        if ( pPlaneMode->ui32Flags & NCGDU_PM_ADDRESS ) {
            ptPlaneMode->ui32Address = pPlaneMode->ui32Address;
        }
        read_chg.gr_base = (void*)ptPlaneMode->ui32Address;

//				printf("pPlaneMode->ui32Flags is %x\n",pPlaneMode->ui32Flags );

        /* Graphics display mode */
//      printf(" pPlaneMode->ui32Flags & NCGDU_PM_PLANE_TYPE = %x\n",pPlaneMode->ui32Flags & NCGDU_PM_PLANE_TYPE);
        
        if ( pPlaneMode->ui32Flags & NCGDU_PM_PLANE_TYPE ) {
            ptPlaneMode->ePlaneType = pPlaneMode->ePlaneType;

            switch ( ptPlaneMode->ePlaneType ) {
                case NCGDU_BLEND:
                    gr_disp_sel = VDC5_DISPSEL_BLEND;
                    printf("ptPlaneMode->ePlaneType==> VDC5_DISPSEL_BLEND\n");
                    break;
                case NCGDU_OPAQUE:
                    gr_disp_sel = VDC5_DISPSEL_CURRENT;
                    printf("ptPlaneMode->ePlaneType==> VDC5_DISPSEL_CURRENT\n");
                    break;
                default:
                /* case NCGDU_EOR: */
                /* case NCGDU_TRANS: */
                /* case NCGDU_TRANS_EOR: */
                /* case NCGDU_TRANS_BLEND: */
                    /* The other settings are not effect to layer. */
                    printf("ptPlaneMode->ePlaneType==> VDC5_DISPSEL_CURRENT\n");
                    gr_disp_sel = VDC5_DISPSEL_CURRENT;
                    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][Info]NCGDU_SetPlaneModeEx Ignored parameter: ePlaneType (%08x).%s" )
                    NCG_DEBUG_MAKE_MSG_PARAMETER( ptPlaneMode->ePlaneType )
                    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                    NCG_DEBUG_MAKE_MSG_END();
                    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
                    break;
            } /* switch ( ptPlaneMode->ePlaneType ) */

            read_chg.gr_disp_sel = &gr_disp_sel;
        }
        else {
						read_chg.gr_disp_sel = NULL;           
        }

				#if (VG_BLEND)
            gr_disp_sel = VDC5_DISPSEL_BLEND;				
            read_chg.gr_disp_sel = &gr_disp_sel;
				#endif
				

        if ( pPlaneMode->ui32Flags & NCGDU_PM_SURFACE_WIDTH ) {
            ptPlaneMode->i32SurfaceWidth = pPlaneMode->i32SurfaceWidth;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_SURFACE_HEIGHT ) {
            ptPlaneMode->i32SurfaceHeight = pPlaneMode->i32SurfaceHeight;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_SURFACE_STRIDE ) {
            ptPlaneMode->i32SurfaceStride = pPlaneMode->i32SurfaceStride;
        }

        if ( pPlaneMode->ui32Flags & (NCGDU_PM_DISPLAY_WIDTH | NCGDU_PM_DISPLAY_HEIGHT | NCGDU_PM_DISPLAY_POSITIONX | NCGDU_PM_DISPLAY_POSITIONY) ) {
            if ( pPlaneMode->ui32Flags & NCGDU_PM_DISPLAY_WIDTH ) {
                ptPlaneMode->i32DisplayWidth = pPlaneMode->i32DisplayWidth;
            }
            if ( pPlaneMode->ui32Flags & NCGDU_PM_DISPLAY_HEIGHT ) {
                ptPlaneMode->i32DisplayHeight = pPlaneMode->i32DisplayHeight;
            }
            if ( pPlaneMode->ui32Flags & NCGDU_PM_DISPLAY_POSITIONX ) {
                ptPlaneMode->i32DisplayPositionX = pPlaneMode->i32DisplayPositionX;
            }
            if ( pPlaneMode->ui32Flags & NCGDU_PM_DISPLAY_POSITIONY ) {
                ptPlaneMode->i32DisplayPositionY = pPlaneMode->i32DisplayPositionY;
            }
            /* Update display area */
            read.gr_grc.vs = param->res_f.vs + ptPlaneMode->i32DisplayPositionY;
            read.gr_grc.vw = ptPlaneMode->i32DisplayHeight;
            read.gr_grc.hs = param->res_f.hs + ptPlaneMode->i32DisplayPositionX;
            read.gr_grc.hw = ptPlaneMode->i32DisplayWidth;

            /* clamp to LCD panel area */
            if ( read.gr_grc.hw > param->res_f.hw ) {
                read.gr_grc.hw = param->res_f.hw;
            }
            if ( read.gr_grc.vw > param->res_f.vw ) {
                read.gr_grc.vw = param->res_f.vw;
            }
            read_chg.gr_grc = &read.gr_grc;
        }
        else {
            read_chg.gr_grc = NULL;                         /* No update. */
        }


				#if (VG_BLEND)
            /* Update display area */
            read.gr_grc.vs = param->res_f.vs + ptPlaneMode->i32DisplayPositionY;
            read.gr_grc.vw = ptPlaneMode->i32DisplayHeight;
            read.gr_grc.hs = param->res_f.hs + ptPlaneMode->i32DisplayPositionX;
            read.gr_grc.hw = ptPlaneMode->i32DisplayWidth;

            /* clamp to LCD panel area */
            if ( read.gr_grc.hw > param->res_f.hw ) {
                read.gr_grc.hw = param->res_f.hw;
            }
            if ( read.gr_grc.vw > param->res_f.vw ) {
                read.gr_grc.vw = param->res_f.vw;
            }
            read_chg.gr_grc = &read.gr_grc;
				#endif
 
        /* update other parameters */
        if ( pPlaneMode->ui32Flags & NCGDU_PM_PRIORITY ) {
            ptPlaneMode->i32Priority = pPlaneMode->i32Priority;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_SURFACE_OFFSETX ) {
            ptPlaneMode->i32SurfaceOffsetX = pPlaneMode->i32SurfaceOffsetX;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_SURFACE_OFFSETY ) {
            ptPlaneMode->i32SurfaceOffsetY = pPlaneMode->i32SurfaceOffsetY;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_ALPHAVALUE ) {
            ptPlaneMode->i32AlphaValue = pPlaneMode->i32AlphaValue;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_TRANSPARENTCOLOR ) {
            ptPlaneMode->i32TransparentColor = pPlaneMode->i32TransparentColor;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_PALETTE ) {
            ptPlaneMode->i32Palette = pPlaneMode->i32Palette;
        }

				if ( (update == NCGDU_SET_PM_STOP) || (update == NCGDU_SET_PM_RESTART) )
        {
            /* Stop process */
            err_code = R_VDC5_StopProcess( ch, layer_id );
            printf("RUN-----------R_VDC5_StopProcess\n");
            if ( err_code != VDC5_OK ) {
                rc_val = NCG_err_unknown;
                NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_SetPlaneModeEx Failed:R_VDC5_StopProcess(%08x).%s" )
                NCG_DEBUG_MAKE_MSG_PARAMETER( err_code )
                NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                NCG_DEBUG_MAKE_MSG_END();
                NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            }
            else {
                /* Release data control */
                printf("RUN-----------R_VDC5_ReleaseDataControl\n");
                err_code = R_VDC5_ReleaseDataControl( ch, layer_id );
                if ( err_code != VDC5_OK ) {
                    rc_val = NCG_err_unknown;
                    NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_SetPlaneModeEx Failed:R_VDC5_ReleaseDataControl(%08x).%s" )
                    NCG_DEBUG_MAKE_MSG_PARAMETER( err_code )
                    NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                    NCG_DEBUG_MAKE_MSG_END();
                    NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
                }
            }
            if ( update == NCGDU_SET_PM_RESTART ) {
                update = NCGDU_SET_PM_START;
            }
        }

        if ( update == NCGDU_SET_PM_START ) {
            /* Read data parameter */
  					printf("NCG_DU--------------------------------------------------NCGDU_SET_PM_START\n"); 
						
#if 0
         GRAPHICS_VideoSample_Post( (void*)ptPlaneMode->ui32Address);
#endif

#if 1         
            read.gr_ln_off_dir  = VDC5_GR_LN_OFF_DIR_INC;           /* Line offset address direction of the frame buffer */
            read.gr_flm_sel     = VDC5_GR_FLM_SEL_FLM_NUM;          /* Selects a frame buffer address setting signal */
            read.gr_imr_flm_inv = VDC5_OFF;                         /* Sets the frame buffer number for distortion correction */
            read.gr_bst_md      = VDC5_BST_MD_32BYTE;               /* Frame buffer burst transfer mode */
            read.gr_base        = (void*)ptPlaneMode->ui32Address;  /* Frame buffer base address */
            read.width_read_fb  = NULL;                             /* Width of the image read from frame buffer */
            read.adj_sel        = VDC5_OFF;                         /* Measures to decrease the influence
                                                                       by folding pixels/lines (ON/OFF) */
            read.gr_ycc_swap    = VDC5_GR_YCCSWAP_CBY0CRY1;         /* Controls swapping of data read from buffer
                                                                       in the YCbCr422 format */
 
            /* Graphics format of the frame buffer read signal */
            switch ( ptPlaneMode->ePixelFormat ) {
                case NCGDU_RGB565:
                    read.gr_format  = VDC5_GR_FORMAT_RGB565;
                    read.gr_rdswa   = VDC5_WR_RD_WRSWA_32_16BIT;
                    read.gr_ln_off  = 2;
                    break;
                case NCGDU_ARGB8888:
                    read.gr_format  = VDC5_GR_FORMAT_RGB888;
                    read.gr_rdswa   = VDC5_WR_RD_WRSWA_32BIT;
                    read.gr_ln_off  = 4;
                    break;
                case NCGDU_ARGB1555:
                    read.gr_format  = VDC5_GR_FORMAT_ARGB1555;
                    read.gr_rdswa   = VDC5_WR_RD_WRSWA_32_16BIT;
                    read.gr_ln_off  = 2;
                    break;
                case NCGDU_ARGB4444:
                    read.gr_format  = VDC5_GR_FORMAT_ARGB4444;
                    read.gr_rdswa   = VDC5_WR_RD_WRSWA_32_16BIT;
                    read.gr_ln_off  = 2;
                    break;
                default:
            #if 0 /* unsupported format in this sample */
                case NCGDU_UYVY:
                case NCGDU_YUYV:
                case NCGDU_INDEX8:
            #endif /* #if 0 */
                    read.gr_format  = VDC5_GR_FORMAT_CLUT8;
                    read.gr_rdswa   = VDC5_WR_RD_WRSWA_8BIT;
                    read.gr_ln_off  = 1;
                    break;
            }
 			#if 1
            /* Display area */
            read.gr_grc.vs = param->res_f.vs + ptPlaneMode->i32DisplayPositionY;
            read.gr_grc.vw = ptPlaneMode->i32DisplayHeight;
            read.gr_grc.hs = param->res_f.hs + ptPlaneMode->i32DisplayPositionX;
            read.gr_grc.hw = ptPlaneMode->i32DisplayWidth;

            /* clamp to LCD panel area */
            if ( read.gr_grc.hw > param->res_f.hw ) {
                read.gr_grc.hw = param->res_f.hw;
            }
            if ( read.gr_grc.vw > param->res_f.vw ) {
                read.gr_grc.vw = param->res_f.vw;
            }
            /* Frame buffer line offset address */
            read.gr_ln_off *= ptPlaneMode->i32SurfaceStride;

            /* Read data control */
            err_code = R_VDC5_ReadDataControl( ch, layer_id, &read );
            if ( err_code != VDC5_OK ) {
                rc_val = NCG_err_unknown;
                NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_SetPlaneModeEx Failed:R_VDC5_ReadDataControl(%08x).%s" )
                NCG_DEBUG_MAKE_MSG_PARAMETER( err_code )
                NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                NCG_DEBUG_MAKE_MSG_END();
                NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            }


						printf("NCG_DU---------------------------------------layer_id = %d\n",layer_id);
		
 		#if 0
                /* Video Capture layer*/
 
                gr_disp_sel = VDC5_DISPSEL_LOWER;
 								layer_id = VDC5_GR_TYPE_GR0;

            start.gr_disp_sel = &gr_disp_sel;
            /* Start process */
            err_code = R_VDC5_StartProcess( ch, layer_id, &start );
 		
 		
 		
            /* Start parameter */
                /* Graphics display mode */
            if ( layer_id == VDC5_LAYER_ID_0_RD ) {
                gr_disp_sel = VDC5_DISPSEL_CURRENT;
            }
            else {
                gr_disp_sel = VDC5_DISPSEL_BLEND;
            }

            start.gr_disp_sel = &gr_disp_sel;
            /* Start process */
            err_code = R_VDC5_StartProcess( ch, layer_id, &start );
            if ( err_code != VDC5_OK ) {
                rc_val = NCG_err_unknown;
                NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_SetPlaneModeEx Failed:R_VDC5_StartProcess(%08x).%s" )
                NCG_DEBUG_MAKE_MSG_PARAMETER( err_code )
                NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                NCG_DEBUG_MAKE_MSG_END();
                NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            }
            
 			#endif
 			
 			GRAPHICS_VideoSample_ReStartSurface();
 
 			#endif
 			
            
#endif

        }
  
        if ( update == NCGDU_SET_PM_CHANGE ) {

//						printf("NCG_DU+++++++++++NCGDU_SET_PM_CHANGE\n"); 
						
 #if 0
						GRAPHICS_VideoSample_Chgaddr( (void*)ptPlaneMode->ui32Address);
 #else
 
            read_chg.width_read_fb  = NULL;
            read_chg.gr_base = ptPlaneMode->ui32Address;       /* Frame buffer base address */
            
            /* Change read process */
            err_code = R_VDC5_ChangeReadProcess( ch, layer_id, &read_chg );
            if ( err_code != VDC5_OK ) {
                /* error */
                rc_val = NCG_err_bad_parameter;

                NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_SetPlaneModeEx Failed:R_VDC5_ChangeReadProcess(%08x).%s" )
                NCG_DEBUG_MAKE_MSG_PARAMETER( err_code )
                NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
                NCG_DEBUG_MAKE_MSG_END();
                NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            }
            
  #endif          
            
            
        }

        if ( rc_val != NCG_no_err ) {
            *ptPlaneMode = tBackup;     /* rolback plane data. */
        }
 
    }
// 		LedOff();
    return rc_val;
}

/*----------------------------------------------------------------------------
 NAME       : NCGDU_GetPlaneModeEx
 FUNCTION   : Get the DU plane information.
 PARAMETERS : ui32Display : [IN ] DU cannel.
              i32Plane    : [IN ] DU plane ID.
              pPlaneMode  : [OUT] The pointer to the DU plane information struct.
 RETURN     : Error code of the NCG.
------------------------------------------------------------------------------*/
NCGint32
NCGDU_GetPlaneModeEx (
    NCGbitfield         ui32Display,
    NCGint32            i32Plane,
    NCGDU_PlaneMode     *pPlaneMode)
{
    NCGint32            rc_val = NCG_no_err;
    NCGDU_PlaneMode     *ptPlaneMode = NULL;

    ptPlaneMode = ncgdu_get_plane_info( ui32Display, i32Plane );
    if ( ptPlaneMode == NULL ) {
        /* error */
        rc_val = NCG_err_bad_parameter;

        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR]NCGDU_GetPlaneModeEx Failed:bad parameter.%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
    }
    else {
        if ( pPlaneMode->ui32Flags & NCGDU_PM_ENABLE ) {
            ptPlaneMode->i32Enable = pPlaneMode->i32Enable;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_PRIORITY ) {
            ptPlaneMode->i32Priority = pPlaneMode->i32Priority;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_PLANE_TYPE ) {
            ptPlaneMode->ePlaneType = pPlaneMode->ePlaneType;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_PIXEL_FORMAT ) {
            ptPlaneMode->ePixelFormat = pPlaneMode->ePixelFormat;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_DISPLAY_WIDTH ) {
            ptPlaneMode->i32DisplayWidth = pPlaneMode->i32DisplayWidth;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_DISPLAY_HEIGHT ) {
            ptPlaneMode->i32DisplayHeight = pPlaneMode->i32DisplayHeight;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_DISPLAY_POSITIONX ) {
            ptPlaneMode->i32DisplayPositionX = pPlaneMode->i32DisplayPositionX;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_DISPLAY_POSITIONY ) {
            ptPlaneMode->i32DisplayPositionY = pPlaneMode->i32DisplayPositionY;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_SURFACE_WIDTH ) {
            ptPlaneMode->i32SurfaceWidth = pPlaneMode->i32SurfaceWidth;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_SURFACE_HEIGHT ) {
            ptPlaneMode->i32SurfaceHeight = pPlaneMode->i32SurfaceHeight;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_SURFACE_STRIDE ) {
            ptPlaneMode->i32SurfaceStride = pPlaneMode->i32SurfaceStride;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_SURFACE_OFFSETX ) {
            ptPlaneMode->i32SurfaceOffsetX = pPlaneMode->i32SurfaceOffsetX;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_SURFACE_OFFSETY ) {
            ptPlaneMode->i32SurfaceOffsetY = pPlaneMode->i32SurfaceOffsetY;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_ALPHAVALUE ) {
            ptPlaneMode->i32AlphaValue = pPlaneMode->i32AlphaValue;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_TRANSPARENTCOLOR ) {
            ptPlaneMode->i32TransparentColor = pPlaneMode->i32TransparentColor;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_PALETTE ) {
            ptPlaneMode->i32Palette = pPlaneMode->i32Palette;
        }
        if ( pPlaneMode->ui32Flags & NCGDU_PM_ADDRESS ) {
            ptPlaneMode->ui32Address = pPlaneMode->ui32Address;
        }
    }

    return rc_val;
}


/*=============================================================================
 *  Internal functions
 */

/*----------------------------------------------------------------------------
 NAME       : ncgdu_init_plane_info
 FUNCTION   : Initialize the DU plane information.
 PARAMETERS : None.
 RETURN     : None.
------------------------------------------------------------------------------*/
static NCGvoid
ncgdu_init_plane_info( NCGvoid )
{
    int                 i32Cnt;
    NCGDU_PlaneMode     *ptPlaneMode;

    /* Initialize NCG DU plane management information */
    ptPlaneMode = (NCGDU_PlaneMode*)NCGDU_ptPlaneMode_ch1;
    for ( i32Cnt = 0; i32Cnt < NCGDU_PLANE_CH1_MAX; ++i32Cnt ) {
        ptPlaneMode[i32Cnt].ui32Flags           = 0U;
        ptPlaneMode[i32Cnt].i32Enable           = NCGDU_NOT_USED;
        ptPlaneMode[i32Cnt].i32Priority         = 0;
        ptPlaneMode[i32Cnt].ePlaneType          = NCGDU_OPAQUE;
        ptPlaneMode[i32Cnt].ePixelFormat        = NCGDU_RGB565;
        ptPlaneMode[i32Cnt].i32DisplayWidth     = 0;
        ptPlaneMode[i32Cnt].i32DisplayHeight    = 0;
        ptPlaneMode[i32Cnt].i32DisplayPositionX = 0;
        ptPlaneMode[i32Cnt].i32DisplayPositionY = 0;
        ptPlaneMode[i32Cnt].i32SurfaceWidth     = 0;
        ptPlaneMode[i32Cnt].i32SurfaceHeight    = 0;
        ptPlaneMode[i32Cnt].i32SurfaceStride    = 0;
        ptPlaneMode[i32Cnt].i32SurfaceOffsetX   = 0;
        ptPlaneMode[i32Cnt].i32SurfaceOffsetY   = 0;
        ptPlaneMode[i32Cnt].i32AlphaValue       = 255;
        ptPlaneMode[i32Cnt].i32TransparentColor = 0;
        ptPlaneMode[i32Cnt].i32Palette          = 0;
        ptPlaneMode[i32Cnt].ui32Address         = 0xffffffffU;
    }

    ptPlaneMode = (NCGDU_PlaneMode*)NCGDU_ptPlaneMode_ch2;
    for ( i32Cnt = 0; i32Cnt < NCGDU_PLANE_CH2_MAX; ++i32Cnt ) {
        ptPlaneMode[i32Cnt].ui32Flags           = 0U;
        ptPlaneMode[i32Cnt].i32Enable           = NCGDU_NOT_USED;
        ptPlaneMode[i32Cnt].i32Priority         = 0;
        ptPlaneMode[i32Cnt].ePlaneType          = NCGDU_OPAQUE;
        ptPlaneMode[i32Cnt].ePixelFormat        = NCGDU_RGB565;
        ptPlaneMode[i32Cnt].i32DisplayWidth     = 0;
        ptPlaneMode[i32Cnt].i32DisplayHeight    = 0;
        ptPlaneMode[i32Cnt].i32DisplayPositionX = 0;
        ptPlaneMode[i32Cnt].i32DisplayPositionY = 0;
        ptPlaneMode[i32Cnt].i32SurfaceWidth     = 0;
        ptPlaneMode[i32Cnt].i32SurfaceHeight    = 0;
        ptPlaneMode[i32Cnt].i32SurfaceStride    = 0;
        ptPlaneMode[i32Cnt].i32SurfaceOffsetX   = 0;
        ptPlaneMode[i32Cnt].i32SurfaceOffsetY   = 0;
        ptPlaneMode[i32Cnt].i32AlphaValue       = 255;
        ptPlaneMode[i32Cnt].i32TransparentColor = 0;
        ptPlaneMode[i32Cnt].i32Palette          = 0;
        ptPlaneMode[i32Cnt].ui32Address         = 0xffffffffU;
    }

    return ;
}


/*----------------------------------------------------------------------------
 NAME       : ncgdu_get_plane_info
 FUNCTION   : Get the DU plane information.
 PARAMETERS : ui32Display : [IN ] DU cannel.
              i32Plane    : [IN ] DU plane ID.
 RETURN     : The pointer to the DU plane information struct.
------------------------------------------------------------------------------*/
static NCGDU_PlaneMode*
ncgdu_get_plane_info(
    NCGbitfield     ui32Display,
    NCGint32        i32Plane )
{
    NCGDU_PlaneMode     *ptPlaneMode = NULL;

    if ( ui32Display == NCGDU_CH1 ) {
        if ( (i32Plane < 0) || (i32Plane >= NCGDU_PLANE_CH1_MAX) ) {
            /* error */
            NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR] Failed:Out of range CH1(%d).%s" )
            NCG_DEBUG_MAKE_MSG_PARAMETER( i32Plane )
            NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
            NCG_DEBUG_MAKE_MSG_END();
            NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            /* ptPlaneMode is NULL */
        }
        else {
            ptPlaneMode = &NCGDU_ptPlaneMode_ch1[i32Plane];
        }
    }
    else if ( ui32Display == NCGDU_CH2 ) {
        if ( (i32Plane < 0) || (i32Plane >= NCGDU_PLANE_CH2_MAX) ) {
            /* error */
            NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR] Failed:Out of range CH2(%d).%s" )
            NCG_DEBUG_MAKE_MSG_PARAMETER( i32Plane )
            NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
            NCG_DEBUG_MAKE_MSG_END();
            NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
            /* ptPlaneMode is NULL */
        }
        else {
            ptPlaneMode = &NCGDU_ptPlaneMode_ch2[i32Plane];
        }
    }
    else {
        /* error */
        NCG_DEBUG_MAKE_MSG_START( NCG_G_MASSAGE_BUFF, "[NCG][ERROR] Failed:Undefined display(%08x).%s" )
        NCG_DEBUG_MAKE_MSG_PARAMETER( ui32Display )
        NCG_DEBUG_MAKE_MSG_PARAMETER( NCG_CRLF )
        NCG_DEBUG_MAKE_MSG_END();
        NCG_DEBUG_PRINT_MSG( NCG_G_MASSAGE_BUFF );
        /* ptPlaneMode is NULL */
    }

    return ptPlaneMode;
}

/*----------------------------------------------------------------------------
 NAME       : init_func
 FUNCTION   : Callback function from R_VDC5_Initialize.
 PARAMETERS : user_num : [IN ] Target channel.
 RETURN     : None.
------------------------------------------------------------------------------*/
static void 
init_func (
    uint32_t        user_num )
{
    vdc5_channel_t channel;

    channel = (vdc5_channel_t)user_num;
    if (channel == VDC5_CHANNEL_0)
    {
        /* Standby control register 9 (STBCR9)
            b1      ------0-;  MSTP91 : 0 : Video display controller channel 0 & LVDS enable */
        CPG.STBCR9 &= ~0x02u;
    }
    else
    {
        /* Standby control register 9 (STBCR9)
            b1      ------0-;  MSTP91 : 0 : Video display controller channel 0 & LVDS enable
            b0      -------0;  MSTP90 : 0 : Video display controller channel 1 enable */
        CPG.STBCR9 &= ~0x03u;
    }
    GRAPHICS_SetLcdPanel(channel);

    return ;
}

/*----------------------------------------------------------------------------
 NAME       : quit_func
 FUNCTION   : Callback function from R_VDC5_Terminate.
 PARAMETERS : user_num : [IN ] Target channel.
 RETURN     : None.
------------------------------------------------------------------------------*/
static void
quit_func (
    uint32_t        user_num )
{
    vdc5_channel_t channel;

    channel = (vdc5_channel_t)user_num;

    if (channel == VDC5_CHANNEL_0)
    {   /* If LVDS should remain enabled, the following code should be removed. */
        CPG.STBCR9 |= 0x02u;
    }
    else
    {
        CPG.STBCR9 |= 0x01u;
    }

    return ;
}

/* -- end of file -- */
