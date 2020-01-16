/******************************************************************************
* Copyright(c) 2010-2013 Renesas Electronics Corporation. All rights reserved.
*
* brief  : DU Control Functions header.
*
* author : Renesas Electronics Corporation
*
* history: 2010.10.08
*          - Created the initial code.
*          2012.08.22
*          - Updated coding format.
*          2013.01.08
*          - Added multi-display functions.
*          2013.02.12
*          - Added definitions of the plane number.
*          2013.04.12
*          - Added NCGDU_ARGB4444 to NCGDU_PixelFormat.
*
*******************************************************************************/

#ifndef NCG_DU_H
#define NCG_DU_H


#ifdef __cplusplus
extern "C" {
#endif


/*=============================================================================
 * Definitions
 */

/* Plane Number */
#define NCGDU_PLANE1                    (0)
#define NCGDU_PLANE2                    (1)
#define NCGDU_PLANE3                    (2)
#define NCGDU_PLANE4                    (3)

/* NCGDU_PlaneMode - i32Enable */
#define NCGDU_ENABLE                    (1)
#define NCGDU_DISABLE                   (0)
#define NCGDU_NOT_USED                  (-1)

/* NCGDU_PlaneType */
typedef enum NCGDU_PlaneType_TAG {
    NCGDU_OPAQUE,
    NCGDU_BLEND,
    NCGDU_EOR,
    NCGDU_TRANS,
    NCGDU_TRANS_BLEND,
    NCGDU_TRANS_EOR,
} NCGDU_PlaneType;

/* NCGDU_PixelFormat */
typedef enum NCGDU_PixelFormat_TAG {
    NCGDU_INDEX8,
    NCGDU_RGB565,
    NCGDU_ARGB1555,
    NCGDU_UYVY,
    NCGDU_YUYV,
    NCGDU_ARGB8888,
    NCGDU_ARGB4444,
} NCGDU_PixelFormat;

/* NCGDU_PlaneMode - ui32Flags */
#define NCGDU_PM_ENABLE              (1 <<  0)
#define NCGDU_PM_PRIORITY            (1 <<  1)
#define NCGDU_PM_PLANE_TYPE          (1 <<  2)
#define NCGDU_PM_PIXEL_FORMAT        (1 <<  3)
#define NCGDU_PM_DISPLAY_WIDTH       (1 <<  4)
#define NCGDU_PM_DISPLAY_HEIGHT      (1 <<  5)
#define NCGDU_PM_DISPLAY_POSITIONX   (1 <<  6)
#define NCGDU_PM_DISPLAY_POSITIONY   (1 <<  7)
#define NCGDU_PM_SURFACE_WIDTH       (1 <<  8)
#define NCGDU_PM_SURFACE_HEIGHT      (1 <<  9)
#define NCGDU_PM_SURFACE_STRIDE      (1 << 10)
#define NCGDU_PM_SURFACE_OFFSETX     (1 << 11)
#define NCGDU_PM_SURFACE_OFFSETY     (1 << 12)
#define NCGDU_PM_ALPHAVALUE          (1 << 13)
#define NCGDU_PM_TRANSPARENTCOLOR    (1 << 14)
#define NCGDU_PM_PALETTE             (1 << 15)
#define NCGDU_PM_ADDRESS             (1 << 17)

#define NCGDU_PM_ALL    (                                    \
                            NCGDU_PM_ENABLE             |    \
                            NCGDU_PM_PRIORITY           |    \
                            NCGDU_PM_PLANE_TYPE         |    \
                            NCGDU_PM_PIXEL_FORMAT       |    \
                            NCGDU_PM_DISPLAY_WIDTH      |    \
                            NCGDU_PM_DISPLAY_HEIGHT     |    \
                            NCGDU_PM_DISPLAY_POSITIONX  |    \
                            NCGDU_PM_DISPLAY_POSITIONY  |    \
                            NCGDU_PM_SURFACE_WIDTH      |    \
                            NCGDU_PM_SURFACE_HEIGHT     |    \
                            NCGDU_PM_SURFACE_STRIDE     |    \
                            NCGDU_PM_SURFACE_OFFSETX    |    \
                            NCGDU_PM_SURFACE_OFFSETY    |    \
                            NCGDU_PM_ALPHAVALUE         |    \
                            NCGDU_PM_TRANSPARENTCOLOR   |    \
                            NCGDU_PM_PALETTE            |    \
                            NCGDU_PM_ADDRESS            |    \
                            0                                \
                        )

/*=============================================================================
 *  Structures
 */

/* DU information */
typedef struct DUINFO_TAG {
    NCGuint32   ui32Flags;
} NCGDUINFO, * PNCGDUINFO;

/* Plane information */
typedef struct NCGDU_PlaneMode_TAG {
    NCGuint32 ui32Flags;
    NCGint32  i32Enable;
    NCGint32  i32Priority;
    NCGenum   ePlaneType;
    NCGenum   ePixelFormat;
    NCGint32  i32DisplayWidth;
    NCGint32  i32DisplayHeight;
    NCGint32  i32DisplayPositionX;
    NCGint32  i32DisplayPositionY;
    NCGint32  i32SurfaceWidth;
    NCGint32  i32SurfaceHeight;
    NCGint32  i32SurfaceStride;
    NCGint32  i32SurfaceOffsetX;
    NCGint32  i32SurfaceOffsetY;
    NCGint32  i32AlphaValue;
    NCGint32  i32TransparentColor;
    NCGint32  i32Palette;
    NCGuint32 ui32Address;
} NCGDU_PlaneMode;

typedef struct NCGDU_PaletteEntry_TAG {
    NCGuint8 ui8Alpha;
    NCGuint8 ui8Red;
    NCGuint8 ui8Green;
    NCGuint8 ui8Blue;
} NCGDU_PaletteEntry;


/*=============================================================================
 *  Function prototyping
 */

/*
 *  DU initialization / finalization
 */
NCGint32
NCGDU_Init (
    PNCGDUINFO pDUInfo
);

NCGint32
NCGDU_DeInit (
    PNCGDUINFO pDUInfo
);

NCGbitfield
NCGDU_QueryDisplayUsage (
    NCGvoid
);


/*
 *  Display control
 */
NCGint32
NCGDU_SetAddress (
    NCGint32            i32Plane,
    NCGuint32           ui32CurrentAddress
);

#define NCGDU_SetPlaneMode( i32Plane, pPlaneMode )        \
    NCGDU_SetPlaneModeEx( NCGDU_CH2, (i32Plane), (pPlaneMode) )

#define NCGDU_GetPlaneMode( i32Plane, pPlaneMode )        \
    NCGDU_GetPlaneModeEx( NCGDU_CH2, (i32Plane), (pPlaneMode) )

NCGint32
NCGDU_SetPlaneModeEx (
    NCGbitfield         uiDisplay32,
    NCGint32            i32Plane,
    NCGDU_PlaneMode     *pPlaneMode
);

NCGint32
NCGDU_GetPlaneModeEx (
    NCGbitfield         uiDisplay32,
    NCGint32            i32Plane,
    NCGDU_PlaneMode     *pPlaneMode
);


#ifdef __cplusplus
}
#endif

#endif /* NCG_DU_H */
