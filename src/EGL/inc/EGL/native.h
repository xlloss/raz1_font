/******************************************************************************
* Copyright(c) 2010-2013 Renesas Electronics Corporation. All rights reserved.
*
* author: Renesas Electronics Corporation
*
* history: 2010.10.08
*          - Modified for link to RGLDrvIF/NCG driver.
*          2010.11.10
*          - Eliminate the invalid member in NativePixmapInfo structure.
*          2011.02.18
*          - Update revision number from 2.4.8 to 2.4.9 (SH726X_R-GPVG_006)
*          2011.08.29
*          - Added wrapNativePixmap
*          2012.01.18
*          - Prototype of setNativeEnv was added.
*          - Enumeration types of NativeEnv was added.
*          2013.02.12
*          - Added Multi-Display functions and definitions.
*
*******************************************************************************/
/******************************************************************************
 *
 * Copyright (c) 2009-2012 System Management Incorporated
 *
 * This software is protected by the law and the agreement concerning
 * a Japanese country copyright method, an international agreement,
 * and other intellectual property right and may be used and copied
 * only in accordance with the terms of such license and with the inclusion
 * of the above copyright notice.
 *
 * This software or any other copies thereof may not be provided
 * or otherwise made available to any other person. No title to and
 * ownership of the software is hereby transferred.
 *
 ******************************************************************************
 * file: native.h
 *
 * Declaration of native resource management
 * component: EGL
 *
 * author: Sosuke Tokunaga
 *         / System Management Incorporated
 *
 * history: 9 Nov 2009 by Sosuke Tokunaga
 *          - Added support to display ARGB8888 format
 *
 *          27 Dec 2009 by Sosuke Tokunaga
 *          - Modified to comply MISRA-C:2004
 *          - Removed getNativeVisiblePixmapInfo function declaration.
 *
 *          07 Oct 2010 by Sosuke Tokunaga
 *          - Updated last-modified-year of copyright notice
 *
 *          30 Nov 2010 by Sosuke Tokunaga
 *          - Added waitNativeVSync API
 *
 *          29 Dec 2010 by Sosuke Tokunaga
 *          - Added description of struct members
 *
 *          16 May 2011 by Sosuke Tokunaga
 *          - Fixed bug : apply D_REL_RGLDRV_FRAME_MEM
 *            to NativePixmapInfo structure.
 *          - Added waitNativeVSync, setNativeVSyncCallback,
 *            suspendNativeVSync, and resumeNativeVSync API.
 *
 *          04 Jul 2011 by Sosuke Tokunaga
 *          - Added NativeEnv to control environment setting
 *            (single-thread ro multi-thread)
 *
 *          22 Sep 2011 by Sosuke Tokunaga
 *          - Removed suspendNativeVSync and resumeNativeVSync API.
 *
 *          13 Jan 2012 by Sosuke Tokunaga
 *          - Added ARGB4444 pixmap support
 *
 *          12 Nov 2012 by Sosuke Tokunaga
 *          - Added XRGB8888 pixmap support
 *
 *          27 Nov 2012 by Sosuke Tokunaga
 *          - Added NativePixmapAttrib, createNativePixmapEx, and
 *            destroyNativePixmapEx
 *
 *          12 Dec 2012 by Sosuke Tokunaga
 *          - Adapted to mulit-screen platform
 *
 *****************************************************************************/

#ifndef NATIVE_H
#define NATIVE_H

#ifndef APICALL
#define APICALL   extern
#endif /* APICALL */

#ifndef APIENTRY
#define APIENTRY
#endif /* APIENTRY */

#define NATIVE_TRUE        1
#define NATIVE_FALSE       0
#define NATIVE_NO_PIXMAP   ((NativePixmap)0)
#define NATIVE_NO_WINDOW   ((NativeWindow)0)

/* Screen */
#define NATIVE_SCREEN_NONE      (0x00000000U)
#define NATIVE_SCREEN_CH1       (0x00000001U)
#define NATIVE_SCREEN_CH2       (0x00000002U)

/* Plane */
#define NATIVE_PLANE1           (0)
#define NATIVE_PLANE2           (1)
#define NATIVE_PLANE3           (2)
#define NATIVE_PLANE4           (3)


#ifdef __cplusplus
extern "C" {
#endif

typedef void         NativeVoid;
typedef int          NativeBool;
typedef unsigned int NativeUint;
typedef unsigned int NativeBitfield;
typedef unsigned int NativePixmap;
typedef unsigned int NativeWindow;

typedef enum {
    NATIVE_RGB565             = 0,
    NATIVE_ARGB1555           = 1,
    NATIVE_ARGB8888           = 2,
    NATIVE_A8                 = 3,
    NATIVE_ARGB4444           = 4,
    NATIVE_XRGB8888           = 5,
    NATIVE_FORMAT_FORCE_SIZE  = 0x7FFFFFFF
} NativeFormat;

typedef enum {
    NATIVE_PIXMAP_NORMAL      = 0,
    NATIVE_PIXMAP_FB_ROM      = 1,
    NATIVE_PIXMAP_FB_RAM      = 2,
    NATIVE_PIXMAP_FORCE_SIZE  = 0x7FFFFFFF
} NativePixmapAttrib;

typedef enum {
    NATIVE_ENV_ST         = 0x0000,
    NATIVE_ENV_MT         = 0x0001,
    NATIVE_ENV_FORCE_SIZE = 0x7FFFFFFF
} NativeEnv;

typedef struct {
    NativeFormat        format;   /* pixel format           */
    NativeUint          width;    /* buffer width in pixel  */
    NativeUint          height;   /* buffer height in pixel */
    NativeUint          stride;   /* pixel stride           */
    NativeUint          phy_addr; /* data address           */
    NativeUint          *p_data;  /* data address           */
    NativePixmapAttrib  attrib;
} NativePixmapInfo;

typedef struct {
    NativePixmap        front;    /* pixmap for front buffer                */
    NativePixmap        back;     /* pixmap for back buffer                 */
    NativeBitfield      output;   /* screens to which this window is output */
} NativeWindowInfo;


/******************************************************************************
 * Global Variable
 */

/******************************************************************************
 * Native Resource Management API
 */

/*-----------------------------------------------------------------------------
 NAME       : setNativeEnv
 FUNCTION   : Set the operating mode of a library
 PARAMETERS : mode : [IN ] Operating mode
 RETURN     : Operating mode setting to a library
-----------------------------------------------------------------------------*/
APICALL NativeEnv APIENTRY setNativeEnv (NativeEnv mode);

/*-----------------------------------------------------------------------------
 NAME       : initNativeResource
 FUNCTION   : Initialize the native resoruce management
 PARAMETERS : None
 RETURN     : Success : NATIVE_TRUE is returned
              Failure : NATIVE_FALSE is returned
-----------------------------------------------------------------------------*/
APICALL NativeBool APIENTRY initNativeResource (void);

/*-----------------------------------------------------------------------------
 NAME       : createNativePixmap
 FUNCTION   : Create a native pixmap
 PARAMETERS : format : [IN ] Native pixel format
              width  : [IN ] Pixmap width
              height : [IN ] Pixmap height
 RETURN     : Success : Pixmap ID
              Failure : NATIVE_NO_PIXMAP
-----------------------------------------------------------------------------*/
APICALL NativePixmap APIENTRY createNativePixmap (NativeFormat format,
                                                  NativeUint width,
                                                  NativeUint height);

/*-----------------------------------------------------------------------------
 NAME       : createNativePixmapEx
 FUNCTION   : Create a native pixmap having a given frame buffer
 PARAMETERS : format  : [IN ] Native format
              width   : [IN ] Pixmap width
              height  : [IN ] Pixmap height
              attrib  : [IN ] Pixmap framebuffer attribute
                              (NATIVE_PIXMAP_FB_ROM or NATIVE_PIXMAP_FB_RAM)
              address : [IN ] Pointer to a frame buffer
 RETURN     : Success : Pixmap ID
              Failure : NATIVE_NO_PIXMAP
-----------------------------------------------------------------------------*/
APICALL NativePixmap APIENTRY createNativePixmapEx (NativeFormat format,
                                                    NativeUint width,
                                                    NativeUint height,
                                                    NativePixmapAttrib attrib,
                                                    NativeVoid *address);

/*-----------------------------------------------------------------------------
 NAME       : destroyNativePixmap
 FUNCTION   : Destroy a native pixmap
 PARAMETERS : pixmap : [IN ] Pixmap ID
 RETURN     : Success : NATIVE_TRUE is returned
              Failure : NATIVE_FALSE is returned
-----------------------------------------------------------------------------*/
APICALL NativeBool APIENTRY destroyNativePixmap (NativePixmap pixmap);

/*-----------------------------------------------------------------------------
 NAME       : destroyNativePixmapEx
 FUNCTION   : Destroy a native pixmap and get information of it
 PARAMETERS : pixmap : [IN ] Pixmap ID
              pInfo  : [OUT] Pointer to a native pixmap information
 RETURN     : Success : NATIVE_TRUE is returned
              Failure : NATIVE_FALSE is returned
-----------------------------------------------------------------------------*/
APICALL NativeBool APIENTRY destroyNativePixmapEx (NativePixmap pixmap,
                                                   NativePixmapInfo *pInfo);

/*-----------------------------------------------------------------------------
 NAME       : getNativePixmapInfo
 FUNCTION   : Get information of a native pixmap
 PARAMETERS : pixmap : [IN ] Pixmap ID
              pInfo  : [OUT] Pointer to a native pixmap information
 RETURN     : Success : NATIVE_TRUE is returned
              Failure : NATIVE_FALSE is returned
-----------------------------------------------------------------------------*/
APICALL NativeBool APIENTRY getNativePixmapInfo (NativePixmap pixmap,
                                                 NativePixmapInfo *pInfo);

/*-----------------------------------------------------------------------------
 NAME       : isNativeDisplayableFormat
 FUNCTION   : Query if a given native pixel format can be displayed
 PARAMETERS : format : [IN ] Native pixel format
 RETURN     : NATIVE_TRUE  : if the format can be displayed
              NATIVE_FALSE : otherwise
-----------------------------------------------------------------------------*/
APICALL NativeBool APIENTRY isNativeDisplayableFormat (NativeFormat format);

/*-----------------------------------------------------------------------------
 NAME       : createNativeWindow
 FUNCTION   : Create a native window
 PARAMETERS : front : [IN ] Front (visible) pixmap ID
              back  : [IN ] Back (drawing) pixmap ID
 RETURN     : Success : Window ID
              Failure : NATIVE_NO_WINDOW
-----------------------------------------------------------------------------*/
APICALL NativeWindow APIENTRY createNativeWindow (NativePixmap front,
                                                  NativePixmap back);

/*-----------------------------------------------------------------------------
 NAME       : destroyNativeWindow
 FUNCTION   : Destroy a native window
 PARAMETERS : window : [IN ] Window ID
 RETURN     : Success : NATIVE_TRUE is returned
              Failure : NATIVE_FALSE is returned
-----------------------------------------------------------------------------*/
APICALL NativeBool APIENTRY destroyNativeWindow (NativeWindow window);

/*-----------------------------------------------------------------------------
 NAME       : getNativeWindowInfo
 FUNCTION   : Get information of a native window
 PARAMETERS : window : [IN ] Window ID
              pInfo  : [OUT] Pointer to a native window information
 RETURN     : Success : NATIVE_TRUE is returned
              Failure : NATIVE_FALSE is returned
-----------------------------------------------------------------------------*/
APICALL NativeBool APIENTRY getNativeWindowInfo (NativeWindow window,
                                                 NativeWindowInfo *pInfo);

/*-----------------------------------------------------------------------------
 NAME       : showNativeWindow
 FUNCTION   : Display a native window
 PARAMETERS : window : [IN ] Window ID
 RETURN     : Success : NATIVE_TRUE is returned.
              Failure : NATIVE_FALSE is returned.
-----------------------------------------------------------------------------*/
APICALL NativeBool APIENTRY showNativeWindow (NativeWindow window);

/*-----------------------------------------------------------------------------
 NAME       : showNativeWindowEx
 FUNCTION   : Display a native window to given screens
 PARAMETERS : window  : [IN ] Window ID
              screens : [IN ] Bitwise OR of output screens
              plane   : [IN ] Plane index (NATIVE_PLANE1-NATIVE_PLANE4)
 RETURN     : Success : NATIVE_TRUE is returned
              Failure : NATIVE_FALSE is returned
-----------------------------------------------------------------------------*/
APICALL NativeBool APIENTRY showNativeWindowEx (NativeWindow window,
                                                NativeBitfield screens,
                                                NativeUint plane);

/*-----------------------------------------------------------------------------
 NAME       : hideNativeWindow
 FUNCTION   : Stop displaying a native window from all screens
 PARAMETERS : window : [IN ] Window ID
 RETURN     : Success : NATIVE_TRUE is returned
              Failure : NATIVE_FALSE is returned
-----------------------------------------------------------------------------*/
APICALL NativeBool APIENTRY hideNativeWindow (NativeWindow window);

/*-----------------------------------------------------------------------------
 NAME       : hideNativeWindowEx
 FUNCTION   : Stop displaying a native window from given screens
 PARAMETERS : window  : [IN ] Window ID
              screens : [IN ] Bitwise OR of output screens
 RETURN     : Success : NATIVE_TRUE is returned
              Failure : NATIVE_FALSE is returned
-----------------------------------------------------------------------------*/
APICALL NativeBool APIENTRY hideNativeWindowEx (NativeWindow window,
                                                NativeBitfield screens);

/*-----------------------------------------------------------------------------
 NAME       : swapNativeWindow
 FUNCTION   : Swap front and back pixmaps bound to the specified window
 PARAMETERS : window : [IN ] Window ID
 RETURN     : Success : Back (drawing) pixmap ID after swapping
              Failure : NATIVE_NO_PIXMAP
-----------------------------------------------------------------------------*/
APICALL NativePixmap APIENTRY swapNativeWindow (NativeWindow window);

/*-----------------------------------------------------------------------------
 NAME       : termNativeResource
 FUNCTION   : Terminate the native resoruce management
 PARAMETERS : None
 RETURN     : None
-----------------------------------------------------------------------------*/
APICALL void APIENTRY termNativeResource (void);

/*-----------------------------------------------------------------------------
 NAME       : wrapNativePixmap
 FUNCTION   : Wrap the specified buffer onto NativePixmap.
 PARAMETERS : pixmap   : [IN] Native Pixmap ID
              p_buffer : [IN] Pointer to the buffer to be wrapped.
              width    : [IN] Width of the buffer specified by p_buffer.
              height   : [IN] Height of the buffer specified by p_buffer.
 RETURN     : Success : NATIVE_TRUE is returned.
              Failure : NATIVE_FALSE is returned.
-----------------------------------------------------------------------------*/
APICALL NativeBool APIENTRY wrapNativePixmap (NativePixmap pixmap,
                                              void *p_buffer,
                                              NativeUint width,
                                              NativeUint height);

#ifdef __cplusplus
}
#endif

#endif /* NATIVE_H */
