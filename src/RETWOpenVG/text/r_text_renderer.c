/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         r_text_renderer.c
* @version      0.01
* $Rev$
* $Date::                          #$
* @brief        
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    <stdio.h>

#include    "r_typedefs.h"

#include    "VG/openvg.h"
#include    "VG/vgu.h"
#include    "EGL/egl.h"

#include    "r_text_renderer.h"

#include    "Font_PathData.h"

 
int32_t R_TXTR_DrawChar (
    r_txtr_hdl_t      * handle,
    char    				  *ch_string,
    int32_t             length,
    VGfloat             pos_x,
    VGfloat             pos_y,
    VGfloat             scaling);
    
static VGuint SampelText[] =
{   /* "SH SS HH" */
   0x31,0x32,0x33,0x20,'R','E','N','E','S','A','S',0x20,0x41,0x42,0x43,0x61,0x62,0x63, 
} ;

VGuint x_shift_tab[128]={};

VGfloat 	G_CharScale = 1.0;
VGfloat 	G_CharShearX =0.0;
VGfloat 	G_CharShearY =0.0;

/******************************************************************************
Macro definitions
******************************************************************************/
#define     TXT_GLYPHS_MAXNUM       (256)       /*!< The total number of glyphs
                                                    that the VGFont object will be required to accept.*/
 
/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
 r_txtr_hdl_t     font_24pt_hdl[2];
 
/******************************************************************************
Private global variables and functions
******************************************************************************/
static int32_t set_glyphs(VGFont font_hdl, r_txtr_font_t font);
static int32_t create_stroke(r_txtr_hdl_t * handle);


/**************************************************************************//**
 * @brief       
 * @param[in]   handle
 * @param[in]   font
 * @retval      int32_t     : If an error occurs, zero will be returned.
******************************************************************************/
int32_t R_TXTR_Create (r_txtr_hdl_t * handle, r_txtr_font_t font)
{
    if (handle == NULL)
    {
        return 0;
    }
    handle->font_hdl        = VG_INVALID_HANDLE;
    handle->paint_fill      = VG_INVALID_HANDLE;
    handle->paint_stroke    = VG_INVALID_HANDLE;
    handle->paintmode       = VG_FILL_PATH;
    handle->stroke_width    = (VGfloat)1.0f;

    if (font >= R_TXTR_FONT_NUM)
    {
        return 0;
    }

    /* Creates a new font object */
    handle->font_hdl = vgCreateFont((VGint)TXT_GLYPHS_MAXNUM);
    if (handle->font_hdl == VG_INVALID_HANDLE)
    {
        printf("ERROR!! vgCreateFont (0x%04X)\n", vgGetError());
        return 0;
    }
    /* Creates a new paint object */
    handle->paint_fill = vgCreatePaint();
    if (handle->paint_fill == VG_INVALID_HANDLE)
    {
        printf("ERROR!! vgCreatePaint (0x%04X)\n", vgGetError());
        return 0;
    }
    return set_glyphs(handle->font_hdl, font);
}   /* End of function R_TXTR_Create() */

/**************************************************************************//**
 * @brief       
 * @param[in]   handle
 * @retval      int32_t     : If an error occurs, zero will be returned.
******************************************************************************/
int32_t R_TXTR_Destroy (r_txtr_hdl_t * handle)
{
    if (handle == NULL)
    {
        return 0;
    }
    /* Destroys the font object */
    if (handle->font_hdl != VG_INVALID_HANDLE)
    {
        vgDestroyFont(handle->font_hdl);
    }
    /* Destroys the paint objects */
    if (handle->paint_fill != VG_INVALID_HANDLE)
    {
        vgDestroyPaint(handle->paint_fill);
    }
    if (handle->paint_stroke != VG_INVALID_HANDLE)
    {
        vgDestroyPaint(handle->paint_stroke);
    }
    return (int32_t)1;
}   /* End of function R_TXTR_Destroy() */

/**************************************************************************//**
 * @brief       
 * @param[in]   handle
 * @param[in]   fill_color
 * @retval      int32_t     : If an error occurs, zero will be returned.
******************************************************************************/
int32_t R_TXTR_Set_FillColor (r_txtr_hdl_t * handle, VGuint fill_color)
{
    if (handle == NULL)
    {
        return 0;
    }
    vgSetParameteri(handle->paint_fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    vgSetColor(handle->paint_fill, fill_color);

    return (int32_t)1;
}   /* End of function R_TXTR_Set_FillColor() */

/**************************************************************************//**
 * @brief       
 * @param[in]   handle
 * @param[in]   stroke_color
 * @retval      int32_t     : If an error occurs, zero will be returned.
******************************************************************************/
int32_t R_TXTR_Set_StrokeColor (r_txtr_hdl_t * handle, VGuint stroke_color)
{
    int32_t ret;

    if (handle == NULL)
    {
        return 0;
    }

    ret = create_stroke(handle);
    if (ret == 0)
    {
        return 0;
    }
    vgSetParameteri(handle->paint_stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    vgSetColor(handle->paint_stroke, stroke_color);

    return (int32_t)1;
}   /* End of function R_TXTR_Set_StrokeColor() */

/**************************************************************************//**
 * @brief       
 * @param[in]   handle
 * @param[in]   paintmode
 * @retval      int32_t     : If an error occurs, zero will be returned.
******************************************************************************/
int32_t R_TXTR_Set_PaintMode (r_txtr_hdl_t * handle, VGbitfield paintmode)
{
    int32_t ret;

    if (handle == NULL)
    {
        return 0;
    }

    if ((paintmode & VG_STROKE_PATH) != 0)
    {
        ret = create_stroke(handle);
        if (ret == 0)
        {
            return 0;
        }
    }
    handle->paintmode = paintmode;

    return (int32_t)1;
}   /* End of function R_TXTR_Set_PaintMode() */

/**************************************************************************//**
 * @brief       
 * @param[in]   handle
 * @param[in]   stroke_width
 * @retval      int32_t     : If an error occurs, zero will be returned.
******************************************************************************/
int32_t R_TXTR_Set_StrokeWidth (r_txtr_hdl_t * handle, VGfloat stroke_width)
{
    int32_t ret;

    if (handle == NULL)
    {
        return 0;
    }

    ret = create_stroke(handle);
    if (ret == 0)
    {
        return 0;
    }
    handle->stroke_width = stroke_width;

    return (int32_t)1;
}   /* End of function R_TXTR_Set_StrokeWidth() */

/**************************************************************************//**
 * @brief       
 * @param[in]   handle
 * @param[in]   vector
 * @param[in]   rampStops
 * @param[in]   StopCount
 * @retval      int32_t     : If an error occurs, zero will be returned.
******************************************************************************/
int32_t R_TXTR_Set_FillLinearGradient (
    r_txtr_hdl_t  * handle,
    const VGfloat * vector,
    const VGfloat * rampStops,
    VGint           StopCount)
{
    VGint listCnt;

    if (handle == NULL)
    {
        return 0;
    }
    listCnt = StopCount * (VGint)5;

    vgSetParameteri(handle->paint_fill, VG_PAINT_TYPE, VG_PAINT_TYPE_LINEAR_GRADIENT);
    vgSetParameterfv(handle->paint_fill, VG_PAINT_LINEAR_GRADIENT, (VGint)4, vector);
    vgSetParameterfv(handle->paint_fill, VG_PAINT_COLOR_RAMP_STOPS, listCnt, rampStops);
    vgSetParameteri(handle->paint_fill, VG_PAINT_COLOR_RAMP_SPREAD_MODE, VG_COLOR_RAMP_SPREAD_PAD); 

    return (int32_t)1;
}   /* End of function R_TXTR_Set_FillLinearGradient() */

/**************************************************************************//**
 * @brief       Conversion from UTF-8 to UCS-2 in 4 byte format
 * @param[out]  str_ucs2
 * @param[in]   str_utf8
 * @param[in]   max_length
 * @retval      int32_t     : Length. If an error occurs, zero will be returned.
******************************************************************************/
int32_t R_TXTR_Conv_utf8_ucs2 (uint32_t * str_ucs2, const uint8_t * str_utf8, int32_t max_length)
{
    int32_t     length;
    uint8_t     first_byte;
    uint32_t    data;

    if ((str_ucs2 == NULL) || (str_utf8 == NULL))
    {
        return 0;
    }
    if (max_length <= 0)
    {
        return 0;
    }

    length = 0;
    while (*str_utf8 != '\0')
    {
        first_byte = *str_utf8++;
        if ((first_byte & (uint8_t)0x80u) == 0)
        {
            *str_ucs2++ = (uint32_t)first_byte;
        }
        else if ((first_byte & (uint8_t)0xE0u) == (uint8_t)0xC0u)
        {
            data        = (uint32_t)((first_byte & (uint32_t)0x001Fu) << 6u);
            data       |= (uint32_t)(*str_utf8++ & (uint32_t)0x003Fu);
            *str_ucs2++ = data;
        }
        else if ((first_byte & (uint8_t)0xF0u) == (uint8_t)0xE0u)
        {
            data        = (uint32_t)((first_byte & (uint32_t)0x000Fu) << 12u);
            data       |= (uint32_t)((*str_utf8++ & (uint32_t)0x003Fu) << 6u);
            data       |= (uint32_t)(*str_utf8++ & (uint32_t)0x003Fu);
            *str_ucs2++ = data;
        }
        else
        {
            printf("ERROR!! R_TXTR_Conv_utf8_ucs2\n");
            return 0;
        }
        length++;
        if (length >= max_length)
        {
            break;
        }
    }
    return length;
}   /* End of function R_TXTR_Conv_utf8_ucs2() */

/**************************************************************************//**
 * @brief       
 * @param[in]   handle
 * @param[in]   ch_string
 * @param[in]   length
 * @param[in]   pos_x
 * @param[in]   pos_y
 * @param[in]   scaling
 * @retval      int32_t     : Length. If an error occurs, zero will be returned.
******************************************************************************/
int32_t R_TXTR_DrawChar (
    r_txtr_hdl_t      * handle,
    char    				  *ch_string,
    int32_t             length,
    VGfloat             pos_x,
    VGfloat             pos_y,
    VGfloat             scaling)
{
    VGfloat origin[] = {0.0f, 0.0f};

		char TempString[64]={ };
		VGuint i;
 
    if (handle == NULL)
    {
        return 0;
    }
    for(i=0;i<length;i++)
    TempString[i] = *ch_string++;
    
//    for(i=0;i<length;i++)
//    printf(" %d Character is %x, shift %d \n",i, TempString[i],x_shift_tab[TempString[i]]);
 
    vgSetfv(VG_GLYPH_ORIGIN, (VGint)2, origin);
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE);
    vgLoadIdentity();
    vgTranslate(pos_x, pos_y);
    vgShear(0.15, 0.0);
 		vgScale(scaling, scaling);
#if 0
    /* Stroke-width */
    vgSetf(VG_STROKE_LINE_WIDTH, handle->stroke_width);

    if ((handle->paintmode & VG_FILL_PATH) != 0)
    {
        vgSetPaint(handle->paint_fill, VG_FILL_PATH);
    }
    if ((handle->paintmode & VG_STROKE_PATH) != 0)
    {
        vgSetPaint(handle->paint_stroke, VG_STROKE_PATH);
    }
#endif
     for(i=0;i<length;i++)
    {
     	vgDrawGlyphs(handle->font_hdl, 1, &TempString[i] , NULL, NULL, VG_FILL_PATH, VG_TRUE);	
    }
 
//     vgDrawGlyphs(handle->font_hdl, (VGint)length, (VGuint *)ch_string, NULL, NULL,VG_FILL_PATH, VG_TRUE);

    return (int32_t)1;
}   /* End of function R_TXTR_Draw() */

int32_t R_TXTR_DrawChar1( r_txtr_hdl_t * handle,	char *ch_string, int32_t length)
{
		char TempString[1]={};
		VGuint i;
    TempString[0] = *ch_string;
//    vgDrawGlyphs(handle->font_hdl, 1, &TempString[0] , NULL, NULL, VG_FILL_PATH, VG_FALSE);	
    vgDrawGlyph(handle->font_hdl, (char)TempString[0] , VG_FILL_PATH, VG_FALSE);	
    vgFinish();
    return (int32_t)1;
}  

int32_t R_TXTR_DrawStringMain(char *CharCode, VGuint length, uint32_t pos_x, uint32_t pos_y, VGuint color)
{
  	VGPaint fill ;
  
    VGfloat origin[] = {0.0f, 0.0f};
		char TempString[32]={};
		VGuint i;
 
    vgSetfv(VG_GLYPH_ORIGIN, (VGint)2, origin);
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE);
    vgLoadIdentity();
    vgTranslate(pos_x, pos_y);
    vgShear(G_CharShearX, G_CharShearY);
 		vgScale(G_CharScale, G_CharScale);
 
 		for(i=0;i<length;i++)
		TempString[i] = *CharCode++;
		
    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( fill, ( VGuint )color ) ;  
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH );
    
		for(i=0;i<length;i++)
 		R_TXTR_DrawChar1(&font_24pt_hdl[0], &TempString[i]  ,1); 
 
    vgDestroyPaint( fill ) ;		
 
 
}   /* End of function R_TXTR_Draw() *


/**************************************************************************//**
 * @brief       
 * @param[in]   font_hdl
 * @param[in]   font
 * @retval      int32_t     : If an error occurs, zero will be returned.
******************************************************************************/
static int32_t set_glyphs (VGFont font_hdl, r_txtr_font_t font)
{
    const font_conv_data_t    * font_data;
    VGuint                      char_code;
    VGPath                      path;
    VGfloat                     origin[] = {0.0f, 0.0f};
    static font_conv_data_t     const * font_data_table[R_TXTR_FONT_NUM] =
    {
//        &Font_24pt_PathDataTable[0],
//        &Font_36pt_PathDataTable[0],
//        &Font_72pt_PathDataTable[0],
//        &Arial_font_24pt_PathDataTable[0],
        &Arialbd_font_24pt_PathDataTable[0],
//        &Times_font_24pt_PathDataTable[0],
//        &Timesbd_font_24pt_PathDataTable[0],
//        &Verdana_font_24pt_PathDataTable[0],
//        &Segoeuib_font_24pt_PathDataTable[0]
    };

    font_data = font_data_table[font];

//    for(char_code = (VGuint)0x0020u; char_code < (VGuint)0x002Fu; char_code++)
for(char_code = (VGuint)0x20; char_code < (VGuint)0x7E; char_code++)
    {
//       	printf("Create char_code path %x\n",char_code); 
				x_shift_tab[char_code] = *(VGfloat *)font_data->escapement;
				
        if(font_data->num_segments == 0)
        {   /* For no visula representation (e.g., space charcter) */
            vgSetGlyphToPath(
                font_hdl,
                char_code,
                VG_INVALID_HANDLE,
                VG_FALSE,
                origin,
                (VGfloat *)font_data->escapement);

        }
        else
        {
            /* Create a new path */
            path = vgCreatePath(
                        VG_PATH_FORMAT_STANDARD,
                        VG_PATH_DATATYPE_F,
                        1.0f,
                        0.0f,
                        0,
                        0,
                        VG_PATH_CAPABILITY_ALL);
            if (path == VG_INVALID_HANDLE)
            {
                printf("ERROR!! vgCreatePath (0x%04X)\n", vgGetError());
                return 0;
            }
            /* Appends data to the path */
            vgAppendPathData(path, font_data->num_segments, font_data->path_segments, font_data->path_data);
            											//path cmd num,         ,path cmd location,         path data location,       
 
            /* Assigns the given path to the glyph associated with the glyph index in the font object. */
            vgSetGlyphToPath(
                font_hdl,
                char_code,
                path,
                VG_FALSE,
                origin,
                (VGfloat *)font_data->escapement);
                /* Destroy the path */
                vgDestroyPath(path);
        }
        font_data++ ;
    }
    return (int32_t)1;
}   /* End of function set_glyphs() */

/**************************************************************************//**
 * @brief       
 * @param[in]   handle
 * @retval      int32_t     : If an error occurs, zero will be returned.
******************************************************************************/
static int32_t create_stroke (r_txtr_hdl_t * handle)
{
    if (handle->paint_stroke != VG_INVALID_HANDLE)
    {
        return (int32_t)1;
    }

    /* Creates a new paint object */
    handle->paint_stroke = vgCreatePaint();
    if (handle->paint_stroke == VG_INVALID_HANDLE)
    {
        printf("ERROR!! vgCreatePaint (0x%04X)\n", vgGetError());
        return 0;
    }
    return (int32_t)1;
}   /* End of function create_stroke() */
 
void R_GUI_OpenVGFontOpen()
{
	uint32_t ret;
 
 	   		ret = R_TXTR_Create(&font_24pt_hdl[0], R_TXTR_FONT_ARIALBD_24);	  
//	   		ret = R_TXTR_Create(&font_24pt_hdl[0], R_TXTR_FONT_TIMES_24);	 		
//	   		ret = R_TXTR_Create(&font_24pt_hdl[0], R_TXTR_FONT_TIMESBD_24);	   	
// 	   		ret = R_TXTR_Create(&font_24pt_hdl[0], R_TXTR_FONT_VERDANA_24);		   		
//	   		ret = R_TXTR_Create(&font_24pt_hdl[0], R_TXTR_FONT_SEGOEUIB_24);		   		
  		
	     	if(ret==0)	printf("R_GUI_OpenVGFont Open------Error\n");
  			return ret;
}

void R_GUI_OpenVGFontClose()
{
	uint32_t ret;
	   		ret = R_TXTR_Destroy(&font_24pt_hdl[0]);
  			return ret;
}

void R_GUI_OpenVGFontDrawing()
{
		uint32_t ret;
 
  	VGPaint fill ;
  
    VGfloat origin[] = {0.0f, 0.0f};
 
    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( fill, ( VGuint )0xFFFFFFff ) ;  
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH ) ;
#if 0
    /* Set the matrix mode to allow manipulation of the glyph-user-to-surface transformation */
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE ) ;
    /* Set the current matrix to the identity matrix  */
    vgLoadIdentity() ;

    /* Set the glyph origin */
    vgSetfv( VG_GLYPH_ORIGIN, 2, origin ) ;
#endif
   	
// 		R_TXTR_Draw(&font_72pt_hdl[0], SampelText,( VGint )( sizeof SampelText / sizeof ( VGuint )) , 0,0,1.0);
 		R_TXTR_DrawChar(&font_24pt_hdl[0], "Renesas1234567890" ,17 , 0,40, 3.5); 
 
//    vgDrawGlyphs( &font_72pt_hdl[0], ( VGint )( sizeof SampelText / sizeof ( VGuint )), SampelText, NULL, NULL, VG_FILL_PATH, VG_FALSE ) ;
 
    /* Destroy the paint objects */
    vgDestroyPaint( fill ) ;		
}

void R_GUI_OpenVGDrawCharacter(uint32_t pos_x, uint32_t pos_y, char CharCode, VGuint color, VGuint FontScale)
{
		uint32_t ret;
  	VGPaint fill ;
  
    VGfloat origin[] = {0.0f, 0.0f};
		char TempString[1]={};
		VGuint i;
		VGfloat Scale;
		
		if(FontScale == 0)
		Scale=0.5;
		else if(FontScale == 1)
		Scale=1.5;		
		else if(FontScale == 2)
		Scale=2.5;	
		else if(FontScale == 3)
		Scale=3.5;	
		else if(FontScale == 4)
		Scale=4.5;	
		else if(FontScale == 5)
		Scale=5.5;	
		else if(FontScale == 6)
		Scale=6.6;	
		else
		Scale=7.5;	
		
		TempString[0] = CharCode;
    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( fill, ( VGuint )color ) ;  
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH );

 		R_TXTR_DrawChar(&font_24pt_hdl[0], &TempString[0]  ,1 , pos_x, pos_y, Scale); 
 
    vgDestroyPaint( fill ) ;		
}

void R_GUI_OpenVGDrawChar(uint32_t pos_x, uint32_t pos_y, char CharCode, VGuint color, VGuint FontScale)
{
 
  	VGPaint fill ;
    VGfloat origin[] = {0.0f, 0.0f};
		VGuint i;
		char TempString[1]={};
		VGfloat Scale;
		
		if(FontScale == 0)
		Scale=0.5;
		else if(FontScale == 1)
		Scale=1.5;		
		else if(FontScale == 2)
		Scale=2.5;	
		else if(FontScale == 3)
		Scale=3.5;	
		else if(FontScale == 4)
		Scale=4.5;	
		else if(FontScale == 5)
		Scale=5.5;	
		else if(FontScale == 6)
		Scale=6.6;	
		else
		Scale=7.5;	
		
		TempString[0] = CharCode;
    fill = vgCreatePaint() ;
    /* Set the paint type to VG_PAINT_TYPE_COLOR */
    vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR ) ;
    /* Set the color and alpha value in sRGBA8888 format to the paint object (shorthand form) */
    vgSetColor( fill, ( VGuint )color ) ;  
    /* Paint object is set on the current context */
    vgSetPaint( fill, VG_FILL_PATH );
#if 0
    vgSetfv(VG_GLYPH_ORIGIN, (VGint)2, origin);
    
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE);
    vgLoadIdentity();
    vgTranslate(pos_x, pos_y);
    vgShear(0.15, 0.0);
 		vgScale(scaling, scaling);
#endif
  		R_TXTR_DrawChar(&font_24pt_hdl[0], &TempString[0]  ,1 , pos_x, pos_y, Scale); 
  		
//    vgDrawGlyphs(font_24pt_hdl[0].font_hdl, 1, &TempString[i] , NULL, NULL, VG_FILL_PATH, VG_TRUE);	
 
    vgDestroyPaint( fill ) ;		
}
