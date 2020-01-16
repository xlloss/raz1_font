
//#include "sysdef.h"

#include "r_typedefs.h"
#include "display.h"
 
#define BPP565		0
#define BPP888		1

#define TRUE 		1
#define FALSE 		0
#define REPLY 		1
#define NOREPLY 	0
#define success 		1
#define fail 	0

#define FontSizeX		8
#define FontSizeY		16

#include "font8x16.h"

uint16_t ForeGroundColor=0xffff;
uint16_t BackGroundColor=0;

uint16_t Migo_DspChar_Loc_X=0;
uint16_t Migo_DspChar_Loc_Y=0;
uint8_t VGAzerof=0;
uint8_t System_BPP =	BPP888;					//0 : 16bpp 5-6-5,  1 : 888

uint16_t Migo_Dsp_SizeX,XRES;
uint16_t Migo_Dsp_SizeY,YRES;

uint32_t Migo_BackWallBufPtr;
uint32_t VRAMAddress;

uint32_t ForeColor;
uint32_t BackColor;
uint32_t TickDelay_Counter=0;
uint32_t TickFlag_GE=0;

extern const unsigned int SINtable[];
extern const unsigned int COStable[];

void GE_Drawline(int x,int y,int xx,int yy,int color,int width,int Plane) ;
uint32_t sycc_to_rgb( int y, int cb, int cr);
uint32_t ConvertYCbCrtoRGB565(unsigned char inputY,unsigned char inputCb,unsigned char inputCr);

struct {
        int xpos;
        int ypos;
        int xsize;
        int ysize;
        int color;
        int TranspentColor;
        } ParaToDsp;
        


void ExportTickCount_Graphics()
{
        	TickFlag_GE=1;
        	TickDelay_Counter+=1;
}
        
void TmrDelay_Graphics(uint32_t DlyCnt)
{
        	TickDelay_Counter=0;
        	while(TickDelay_Counter<DlyCnt)
        	{}
}

void MigoGraphicsInitialize (
    uint8_t gr_format,
    uint32_t fr_buff,
    uint16_t width,
    uint16_t height,
    uint32_t stride)
{
		Migo_BackWallBufPtr = fr_buff;
		VRAMAddress = fr_buff;
		Migo_Dsp_SizeX = XRES = width;
		Migo_Dsp_SizeY = YRES = height;
		System_BPP = BPP888;
}

int MigoGraphicsMain()
{
	while(1)
	{
	GE_ClearScreen(0x000000ff);
	TmrDelay_Graphics(1000);
	printf(" step 1\n");

	GE_ClearScreen(0x0000ff00);
	TmrDelay_Graphics(1000);
	printf(" step 2\n");

	GE_ClearScreen(0x00ff0000);
	TmrDelay_Graphics(1000);
	printf(" step 3\n");

	GE_ClearScreen(0x00ffffff);
	TmrDelay_Graphics(1000);

	GE_ClearScreen(0x00000000);
	TmrDelay_Graphics(1000);
	}
}

void GE_ClearScreen(uint32_t color)
{
				uint32_t i;
 
 					
 
				if(System_BPP == BPP565)
				{
 				for( i = 0; i<XRES*YRES*2; i+=2)
 					*(uint16_t*) (VRAMAddress +  i ) = (uint16_t) color ;
 				}
				if(System_BPP == BPP888)
				{
 				for( i = 0; i<XRES*YRES*4; i+=4)
 					*(uint32_t*) (VRAMAddress +  i ) = (uint32_t) color ;
 					
  					printf("fill screen finished\n");
 				} 				
 				
}

GE_CopyPicture(int x,int y,int sizeX,int sizeY,int color,int xx, int yy, int FromAddr,int DestAddr,int SkipBlack)
{
				int i,j;
				short colorlvl;
				short pixel;
				int r,g,b;
 				for( j = 0; j<sizeY; j++)
 				for( i = 0; i<sizeX; i++)
 					{
 						if(System_BPP = BPP565)
 						{
 							pixel = *(uint16_t*) ((unsigned long)FromAddr + (yy+j)*XRES*2 +  xx*2  + i*2 );
 							if(pixel!=0 ||(SkipBlack==0) )
							*(uint16_t*) ((unsigned long)DestAddr + (y+j)*XRES*2 +  x*2  + i*2 ) = pixel;
						}
						else if(System_BPP = BPP888) 
 						{
 							pixel = *(uint32_t*) ((unsigned long)FromAddr + (yy+j)*XRES*4 +  xx*4  + i*4 );
 							if(pixel!=0 ||(SkipBlack==0) )
							*(uint32_t*) ((unsigned long)DestAddr + (y+j)*XRES*4 +  x*4  + i*4 ) = pixel;
						}					
    			}
}

void GE_CopyPasteScreen(int FromAddr, int ToAddr, int SkipBlack)
{
				int i;
				short TempData;
 				for( i = 0; i<XRES*YRES*2; i+=2)
 				{
 				TempData = *(uint16_t*) ((unsigned long)FromAddr +  i );
 			 	//if( TempData != 0 || (SkipBlack==0))
 					*(uint16_t*) ((unsigned long)ToAddr +  i ) = TempData ;
 				}
}
 
GE_DrawChar8x16(int x,int y,unsigned char asciicode,unsigned char color)
{
        int xptr=0;
        int yptr=0;
        int report=0;
        int xdot;
        int ydot;
        unsigned char temp_data;
        unsigned char cp_data;
 
        ForeColor = color ;
        BackColor = (ForeColor & 0xf8) |(~color & 0x07);
        for(yptr=0 ; yptr<16 ; yptr++)
        {
        temp_data = Font8x16[asciicode * 16 + yptr];

                for(xptr=0 ; xptr<8 ; xptr++)
                   {
                    cp_data = temp_data & 0x80 ;
                    if(cp_data == 0x80)
                        {
                        xdot = x + xptr ;
                        ydot = y + yptr ;
                        GE_DrawDot(xdot , ydot ,ForeColor);
/*      screen("x=%x,y=%x,temp_data=%x.\n",xdot,ydot,temp_data);        */
                        }

                    if(cp_data != 0x80)
                        {
                        xdot = x + xptr ;
                        ydot = y + yptr ;
                        GE_DrawDot(xdot , ydot ,BackColor);
/*      screen("x=%x,y=%x,temp_data=%x.\n",xdot,ydot,temp_data);        */
                        }
                    temp_data = temp_data << 1 ;
                   }
        }
        return;
}
 
FillChar()
{
        int charptr;
        int charyptr;
        char chardata;
        int dotxptr;
        int dotyptr;
        unsigned char colorptr=0xE0;
        for (charyptr= 0 ;charyptr < 30  ;charyptr++)
            {
                chardata = charyptr;
                for (charptr= 0 ;charptr < 80  ;charptr++)
                   {
                   dotxptr = (charptr % 80) * 8 ;
                   dotyptr = charyptr * 16;
//                   Dsp_char_8bpp_Big(dotxptr,dotyptr,chardata,colorptr);
                   chardata += 1;
                   }
                   colorptr += 1;
            }
        return;
}
 
InitialByBackWall(int SourceAddress)
{
        int ptr;
        for (ptr=0 ;ptr < (XRES*YRES) ;ptr+=2)
        {
             *(uint16_t *)(VRAMAddress + ptr )  = *(uint16_t *)(SourceAddress + ptr );
        }
        return;
}

DspScrollUp()
{
        int ptr;
        unsigned char data;

        for (ptr=0 ;ptr < (640*(480-16));ptr)
        {
             data =*(uint8_t*)(0xb2000000 + ptr + 640*16) ;
             *(uint8_t *)(0xb2000000 + ptr) = data ;
        }
        for (ptr = (640*(480-16)) ; ptr < 640*480 ; ptr)
        {
             *(uint8_t *)(0xb2000000 + ptr) = 0 ;
        }
 
        return;
}

GE_DrawPanelOutline()
{
        int charptr;
        char dotdata;
        int dotxptr;
        int dotyptr;
        unsigned char colorptr;

        for (dotxptr= 0 ;dotxptr < XRES ;dotxptr++)
           {
           dotyptr = 0;
           GE_DrawDot(dotxptr,dotyptr,0x0f );
           }
        for (dotxptr= 0 ;dotxptr < XRES ;dotxptr++)
           {
           dotyptr = YRES-1;
           GE_DrawDot(dotxptr,dotyptr,0x0f );
           }
        for (dotyptr= 0 ;dotyptr < YRES ;dotyptr++)
           {
           dotxptr = 0;
           GE_DrawDot(dotxptr,dotyptr,0x0f );
           }
        for (dotyptr= 0 ;dotyptr < YRES ;dotyptr++)
           {
           dotxptr = XRES-1;
           GE_DrawDot(dotxptr,dotyptr,0x0f );
           }
        return;
}
Draw16ColorBar()
{
        int addr=0;
        unsigned char color=00;

        for (addr= 0 ;addr < 640 * 480 ;addr++)
            {
             *(uint8_t * )( 0xb2000000 + addr) = color ;

             if ((addr+1) % (320*240/16 ) == 0 )
                { color += 1;}
            }
        return;
}
DrawHLine(int x,int y,int length,int color)
{
        int i;        
        for(i=x;i<x+length;i++)
        {GE_DrawDot(i,y,color);}
        return;
}
DrawVLine(int x,int y,int length,int color)
{
        int i;        
        for(i=y;i<y+length;i++)
        {GE_DrawDot(x,i,color);}
        return;
}

int GE_DrawDot(int x,int y,int color)
{
        int pixel;
#if 1
        if(x>=XRES)
          return;
        if(y>=YRES)
          return;
#endif
				if( System_BPP == BPP565)
        *(uint16_t*)(VRAMAddress + (y * XRES*2) + x*2 ) = (short)color;
        else if( System_BPP == BPP888)
        *(uint32_t*)(VRAMAddress + (y * XRES*4) + x*4 ) = (uint32_t)color;
        return;
}
 
uint32_t GE_ReadDot(int x,int y)
{
				uint32_t color;
        uint32_t Dot_Address;
        
        if(x>XRES)
          return 0;
        if(y>YRES)
          return 0;
				if( System_BPP == BPP565)
					{
        	Dot_Address = y * XRES *2 + x*2 + VRAMAddress;
        	color = (* (uint16_t *) Dot_Address);
          }
        else if( System_BPP == BPP888)
					{
        	Dot_Address = y * XRES *4 + x*4 + VRAMAddress;
        	color = (* (uint32_t *) Dot_Address);
          }        	
        return color;
}

int DspReverseBlock(int x,int y,int xsize,int ysize)
{
        int i,j,color;
 
        for(j=y;j<(y+ysize);j++)
           {
            for(i=x;i<(x+xsize);i++)
               {
               	color = ~(short)GE_ReadDot(i,j);
                GE_DrawDot16bpp(i,j,(short)color);
                }
           }
           return;
}
int DspFillBlock(int x,int y,int xsize,int ysize,int color)
{
        int i,j;
//        screen("%d,%d,%d,%d\n",x,y,xsize,ysize);
        for(j=y;j<(y+ysize);j++)
           {
#if 0
            for(i=x;i<(x+xsize);i++)
               {
               GE_DrawDot(i,j,(short)color);
               }    
#else
		i=x;
		while(i<(x+xsize))
		{*(uint16_t*)(VRAMAddress+j*XRES*2+i*2)=(short)color;
		i+=1;}
#endif                
     
           }
           return 1;
}
int DspButtonUp(int x,int y,int sizex,int sizey,int color)
{
#if 0
        DspFillBlock(x,y,sizex,ysize,CharColorGRAY);     /* all block */
        
        DspFillBlock(x,y,sizex-2,1,0xffff);       /* Up line   */
        DspFillBlock(x,y,1,sizey-2,0xffff);       /* Left line */

        DspFillBlock(x+sizex-1,y,1,sizey,0x0000); /* right line*/
        DspFillBlock(x,y+sizey-1,sizex,1,0x0000); /* buttonline*/
        DspFillBlock(x+sizex-2,y+2,1,sizey-2,CharColorGRAY);     /* right 2nd */
        DspFillBlock(x+2,y+sizey-2,sizex-2,1,CharColorGRAY);     /* button 2nd*/
#else
	DspFillBlock(x,y,sizex-1,1,CharColorWHITE);
	DspFillBlock(x+1,y+1,sizex-2,1,CharColorWHITE);
	DspFillBlock(x+2,y+2,sizex-3,1,CharColorGRAYWHITE);
	DspFillBlock(x+3,y+3,sizex-4,1,CharColorGRAYWHITE);
	
	DspFillBlock(x,y+1,1,sizey-2,CharColorWHITE);
	DspFillBlock(x+1,y+2,1,sizey-4,CharColorWHITE);	
	DspFillBlock(x+2,y+3,1,sizey-6,CharColorGRAYWHITE);		
	DspFillBlock(x+3,y+4,1,sizey-8,CharColorGRAYWHITE);	
	
	DspFillBlock(x+sizex-1,y,1,sizey,CharColorBLACK);
	DspFillBlock(x+sizex-2,y+1,1,sizey-2,CharColorGRAY);
	DspFillBlock(x+sizex-3,y+2,1,sizey-4,CharColorGRAYWHITE);
	DspFillBlock(x+sizex-4,y+3,1,sizey-6,CharColorGRAYWHITE);
	
	DspFillBlock(x ,y+sizey-1,sizex,1,CharColorBLACK);
	DspFillBlock(x+1 ,y+sizey-2,sizex-2,1,CharColorGRAY);	
	DspFillBlock(x+2 ,y+sizey-3,sizex-4,1,CharColorGRAYWHITE);	
	DspFillBlock(x+3 ,y+sizey-4,sizex-6,1,CharColorGRAYWHITE);		

	DspFillBlock(x+2 ,y+2,sizex-4,sizey-4,color);         
#endif
        return;
}

int DspButtonDown(int x,int y,int xsize,int ysize,int color)
{
        DspFillBlock(x,y,xsize,ysize,!CharColorGRAY);     /* all block */
        DspFillBlock(x,y,xsize-2,1,!0xffff);       /* Up line   */
        DspFillBlock(x,y,1,ysize-2,!0xffff);       /* Left line */
        DspFillBlock(x+xsize-1,y,1,ysize,!0x0000); /* right line*/
        DspFillBlock(x,y+ysize-1,xsize,1,!0x0000); /* buttonline*/
        DspFillBlock(x+xsize-2,y+2,1,ysize-2,!CharColorGRAY);     /* right 2nd */
        DspFillBlock(x+2,y+ysize-2,xsize-2,1,!CharColorGRAY);     /* button 2nd*/
        return;
}


DspColorBlock()
{
        int i;
        int j;
        int k;
        int l;
        int addr;

        for(i=0;i<640*480;i++)
        {
         *(uint8_t*)(0xb2000000 + i) = 0;
        }

for (j= 0 ; j < 0x10;j++)
    {
     for (i= 0 ; i < 0x10;i++)
         {
         for(l=0;l<18;l++)
            {
            for(k=0;k<28;k++)
                {
addr = 0xb2000000 +((640-16*30)/2)+i*30+k + (((480-16*20)/2)+j*20+l)*640 ;
        *(uint8_t*)addr = (char)(i+j*16);
                }
            }
         }
    }
}
int RGBto256(int R,int G,int B)
{
        return ( (R & 0xe0) | ( (G>>3) & 0x1c) | (B & 0x03)) ; /*RBG=332*/
}
 
VGADspSetColor(int ForeV ,int BackV)
{
	ForeColor = ForeV;
	BackColor = BackV;	
}

int HexDecDsp(int P)
{
        P &= 0x0000000f ;
        if(P > 0x00000009)
        {
                P += 0x07;
        }
        P += 0x30;
        return P;
}

VGADspSetLocation(int x ,int y)
{
	Migo_DspChar_Loc_X = x;
	Migo_DspChar_Loc_Y = y;	
}

void VGADspString(char *txdptr, uint32_t para1, uint32_t para2, uint32_t para3,uint32_t para4)
{
        uint32_t txtemp;
        uint32_t paracnt=0;

   for(;;)
   {
        txtemp = *(txdptr);
        switch (txtemp & 0x000000ff)
        {
                case 0x00000000 :
                                return;
                case 0x0000000a :
                                Migo_DspChar_Loc_X = 0;
                                Migo_DspChar_Loc_Y += FontSizeY;
                           txdptr++;
                           break;
                case 0x00000020 :
                                Migo_DspChar_Loc_X += FontSizeX;
                           txdptr++;
                           break;
                case 0x00000025 :     /* character "%" */
                           txdptr++;
                           txtemp=*txdptr;
                           switch(txtemp & 0x000000ff)
                           {
                            case 0x00000064:    /* character "d" */
                                      switch(paracnt)
                                      {
                                        case 0x00000000:
                                             VGADspDec(para1);
                                             paracnt++;
                                             break;
                                        case 0x00000001:
                                             VGADspDec(para2);
                                             paracnt++;
                                             break;
                                        case 0x00000002:
                                             VGADspDec(para3);
                                             paracnt++;
                                             break;
                                        case 0x00000003:
                                             VGADspDec(para4);
                                             paracnt++;
                                             break;
                                      }
                                      break;
                            case 0x00000078:    /* character "x" */
                                      switch(paracnt)
                                      {
                                        case 0x00000000:
                                             VGADspReg(para1);
                                             paracnt++;
                                             break;
                                        case 0x00000001:
                                             VGADspReg(para2);
                                             paracnt++;
                                             break;
                                        case 0x00000002:
                                             VGADspReg(para3);
                                             paracnt++;
                                             break;
                                        case 0x00000003:
                                             VGADspReg(para4);
                                             paracnt++;
                                             break;
                                      }
                                      break;
                           }
                           txdptr++;
                           break;
                default:
                           VGADspChar(txtemp);
                           txdptr++;
                           break;
        }
   }
}

VGADspDec(int P)
{
        static int result;
        VGAzerof = 0;

        VGAputdec(P/1000000000);
        result = P % 1000000000 ;

        VGAputdec(result/100000000);
        result = result % 100000000 ;

        VGAputdec(result/10000000);
        result = result % 10000000 ;

        VGAputdec(result/1000000);
        result = result % 1000000 ;

        VGAputdec(result/100000);
        result = result % 100000 ;

        VGAputdec(result/10000);
        result = result % 10000 ;

        VGAputdec(result/1000);
        result = result % 1000 ;

        VGAputdec(result/100);
        result = result % 100 ;

        VGAputdec(result/10);
        result = result % 10 ;

        VGAzerof = 1;
        VGAputdec(result/1);

        return;
}
int VGAputdec(int P)
{
        P += 0x30;
        if ( ( (P & 0x000000ff) != 0x30))
        {
        VGAzerof = 1;
        }
        if (VGAzerof == 1)
        {
        VGADspChar(P);
        }
}
int VGADspReg(int P)
{
        VGADspChar(HexDecDsp(P >> 28));
        VGADspChar(HexDecDsp(P >> 24));
        VGADspChar(HexDecDsp(P >> 20));
        VGADspChar(HexDecDsp(P >> 16));
        VGADspChar(HexDecDsp(P >> 12));
        VGADspChar(HexDecDsp(P >> 8));
        VGADspChar(HexDecDsp(P >> 4));
        VGADspChar(HexDecDsp(P));
        return;
}

VGADspChar(int P)
{
        int xptr=0;
        int yptr=0;
        int report=0;
        int xdot;
        int ydot;
        unsigned char temp_data;
        unsigned char cp_data;
        
        int OrgColor=(short)GE_ReadDot(Migo_DspChar_Loc_X,Migo_DspChar_Loc_Y);
        ForeColor = 0xFFFFFFFF;
        BackColor = 0x00000000;        
        
        for(yptr=0 ; yptr<16 ; yptr++)
        {
        temp_data = Font8x16[P * 16 + yptr];

                for(xptr=0 ; xptr<8 ; xptr++)
                   {
                    cp_data = temp_data & 0x80 ;
                    if(cp_data == 0x80)
                        {
                        xdot = Migo_DspChar_Loc_X + xptr ;
                        ydot = Migo_DspChar_Loc_Y + yptr ;
                        GE_DrawDot16bpp(xdot , ydot ,(uint32_t)ForeColor);
/*      screen("x=%x,y=%x,temp_data=%x.\n",xdot,ydot,temp_data);        */
                        }

                    if(cp_data != 0x80 )
                        {
                        xdot =  Migo_DspChar_Loc_X + xptr ;
                        ydot =  Migo_DspChar_Loc_Y + yptr ;
                        GE_DrawDot16bpp(xdot , ydot ,(uint32_t)BackColor);
                        }
                    temp_data = temp_data << 1 ;
                   }
        }

        Migo_DspChar_Loc_X += FontSizeX;
        if(Migo_DspChar_Loc_X > XRES-FontSizeX)
          {
                Migo_DspChar_Loc_X = 0;
                Migo_DspChar_Loc_Y += FontSizeY;}

        if(Migo_DspChar_Loc_Y > YRES-FontSizeY )
           {
           Migo_DspChar_Loc_Y-=FontSizeY; }
        return;
}
 
 VGADspCharOld(int P)					// not used from RZ-A1
{
        int xptr=0;
        int yptr=0;
        int report=0;
        int xdot;
        int ydot;
        unsigned char temp_data;
        unsigned char cp_data;
        int OrgColor=(short)GE_ReadDot(Migo_DspChar_Loc_X,Migo_DspChar_Loc_Y);
        
        for(yptr=0 ; yptr<16 ; yptr++)
        {
        temp_data = Font8x16[P * 16 + yptr];

                for(xptr=0 ; xptr<8 ; xptr++)
                   {
                    cp_data = temp_data & 0x80 ;
                    if(cp_data == 0x80)
                        {
                        xdot = Migo_DspChar_Loc_X + xptr ;
                        ydot = Migo_DspChar_Loc_Y + yptr ;
                        GE_DrawDot16bpp(xdot , ydot ,(short)ForeColor);
/*      screen("x=%x,y=%x,temp_data=%x.\n",xdot,ydot,temp_data);        */
                        }

                    if(cp_data != 0x80 && BackColor!=0xc000)
                        {
                        xdot =  Migo_DspChar_Loc_X + xptr ;
                        ydot =  Migo_DspChar_Loc_Y + yptr ;
                        if(BackColor!=0x8000)
                        GE_DrawDot16bpp(xdot , ydot ,(short)(BackColor&0x7fff));
                        else
                        GE_DrawDot16bpp(xdot , ydot ,(short)OrgColor);

/*      screen("x=%x,y=%x,temp_data=%x.\n",xdot,ydot,temp_data);        */
                        }
                    temp_data = temp_data << 1 ;
                   }
        }

        Migo_DspChar_Loc_X += FontSizeX;
        if(Migo_DspChar_Loc_X > XRES-FontSizeX)
          {
                Migo_DspChar_Loc_X = 0;
                Migo_DspChar_Loc_Y += FontSizeY;}

        if(Migo_DspChar_Loc_Y > YRES-FontSizeY )
           {
           Migo_DspChar_Loc_Y-=FontSizeY; }
        return;
}
 
void GE_IndicatorRotate(int degin,int sizeX,int sizeY,int shiftX,int shiftY,int centerX,int centerY,int DisplayPlane) 
{
			int i,j,k;
			short pixel;
			int xx,yy;
			
			int offsetX,offsetY;			//to DisplayPlane's 
			int newX,newY;
			int DestAddr,x,y;
			
			int deg = degin;
			
					if(degin == 90)
									  deg = 90;
			
					else if(degin == 180)
									  deg = 0;
									  
					else if(degin == 270)
										deg = 90;
										
 					else if(degin>90 && degin  <180) 
 										{
 										deg = 270-degin;			//if 91 ->179
 																					//if 92 ->178
 									  deg = deg%90;
 										}
 					else if(degin>270 && degin <360)
 										{
 										deg = 360-degin;	
 										deg = deg%90;
 										}
 				  else
 					deg = deg%90;
 								 
			//find shift to indicator axis position, just need once only
 					i=shiftX;
 					j=shiftY;

 					offsetX = COStable[deg]*(sizeX-i) /10000;
 					offsetY = SINtable[deg]*(sizeX-i) /10000;
 					newX = centerX - offsetX  + (sizeY-j) * SINtable[deg]/10000;
 					newY = centerY - offsetY  - (sizeY-j) * COStable[deg]/10000;
 					 					
 					xx =  newX-centerX;
 					yy =  newY-centerY;
 
 				for( j = 0; j<sizeY; j++)
 				for( i = 0; i<sizeX; i++)
 					{
 					pixel = *(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (j)*XRES*2 + i*2 );
 					offsetX = COStable[deg]*(sizeX-i) /10000;
 					offsetY = SINtable[deg]*(sizeX-i) /10000;
 					newX = centerX - offsetX  + (sizeY-j) * SINtable[deg]/10000 -xx;
 					newY = centerY - offsetY  - (sizeY-j) * COStable[deg]/10000 -yy;
 
 
 					if(degin>90 && degin  <181) 
 								newX = centerX+(centerX-newX);
 
 					if(degin>180 && degin <271){
 								newX = centerX+(centerX-newX);
 								newY = centerY+(centerY-newY);}
 							
 					if(degin>270 && degin <360)
 								newY = centerY+(centerY-newY);
 
					if(pixel != 0)
						{
						*(uint16_t*) ((unsigned long)DisplayPlane + (newY)*XRES*2 +  (newX)*2) = pixel;
						*(uint16_t*) ((unsigned long)DisplayPlane + (newY+1)*XRES*2 +  (newX+1)*2) = pixel;
						*(uint16_t*) ((unsigned long)DisplayPlane + (newY+1)*XRES*2 +  (newX)*2) = pixel;
 						}
					
					
    			}
    			
}

GE_DrawClock(int SecondCnt,int x,int y,int rad, int color,int width,int DisplayPlane) 
{
		 int newx,newy,i,j,k;
 			int deg;
 			
 			if(SecondCnt == 0)
 						deg = 90;
 			else if(SecondCnt == 15)
 						deg = 0;
 			else if(SecondCnt == 30)
 						deg = 90;
 			else if(SecondCnt == 45)
 						deg = 0;
  		else if(SecondCnt>0 && SecondCnt <15)
 						deg = (15-SecondCnt)*6;
  		else if(SecondCnt>15 && SecondCnt <30)
 						deg = (SecondCnt-15)*6;
  		else if(SecondCnt>30 && SecondCnt <45)
 						deg = (45-SecondCnt)*6;
  		else if(SecondCnt>45 && SecondCnt <60)
 						deg = (SecondCnt-45)*6;
 		
 			if(SecondCnt>=0 && SecondCnt <15){
 							newx = x+(COStable[deg]*(rad) /10000);
 			 				newy = y - (SINtable[deg]*(rad) /10000);}
 			if(SecondCnt>=15 && SecondCnt <30){ 
 							newx = x+(COStable[deg]*(rad) /10000);
 			 				newy = y + (SINtable[deg]*(rad) /10000);}
 			if(SecondCnt>=30 && SecondCnt <45){
 							newx = x-(COStable[deg]*(rad) /10000);
 			 				newy = y + (SINtable[deg]*(rad) /10000);}
 			if(SecondCnt>=45 && SecondCnt <60){
 							newx = x-(COStable[deg]*(rad) /10000);
 			 				newy = y - (SINtable[deg]*(rad) /10000);}
 			 				
// 			 	GE_DrawDot16bpp(newx,newy,0xffff,DisplayPlane);
 			 	
  			GE_Drawline(x,y,newx,newy,color,1,DisplayPlane);
}
void GE_DrawCircle(int x,int y,int rad, int color,int width,int DisplayPlane) 
{
		 int newx,newy,i,j,k;
 			int deg;
 			for(deg=0;deg<91;deg++)
 			{
 							newx = x+(COStable[deg]*(rad) /10000);
 			 				newy = y - (SINtable[deg]*(rad) /10000);
 							*(uint16_t*) ((unsigned long)DisplayPlane + (newy)*XRES*2 +  (newx)*2) = color;
 							newx = x+(COStable[deg]*(rad) /10000);
 			 				newy = y + (SINtable[deg]*(rad) /10000);
 							*(uint16_t*) ((unsigned long)DisplayPlane + (newy)*XRES*2 +  (newx)*2) = color;
 							newx = x-(COStable[deg]*(rad) /10000);
 			 				newy = y - (SINtable[deg]*(rad) /10000);
 							*(uint16_t*) ((unsigned long)DisplayPlane + (newy)*XRES*2 +  (newx)*2) = color;
 							newx = x-(COStable[deg]*(rad) /10000);
 			 				newy = y + (SINtable[deg]*(rad) /10000);
 							*(uint16_t*) ((unsigned long)DisplayPlane + (newy)*XRES*2 +  (newx)*2) = color;	
 		  }
}
void GE_Drawline(int x,int y,int xx,int yy,int color,int width,int Plane) 
{
		 int newx,newy,newxx,newyy;
		 int i,j,k;
 			if(yy<y)
 					k=y-yy;
 				else
 					k=yy-y;
 			if(xx<x)
					j=x-xx;
 				else
					j =xx-x;
 
			if(j>k){
 								if(xx<x)
								{ 
									newx = xx; 
									newxx = x;
									newy = yy;
									newyy = y; }
								else
								{ 
									newx = x; 
									newxx = xx;
									newy = y; 
									newyy = yy;}
 							}
 				else{
 								if(yy<y)
								{ 
									newx = xx; 
									newxx = x;
									newy = yy;
									newyy = y; }
								else
								{ 
									newx = x; 
									newxx = xx;
									newy = y; 
									newyy = yy;}
 							}		

			if(j>k)
			{
			for(i=newx;i<newxx;i++)
			*(uint16_t*) ((unsigned long)Plane + (newy +(newyy-newy)*(i-newx)/(newxx-newx))*XRES*2 +  (i)*2) = color;	
			}
			else
			{
			for(i=newy;i<newyy;i++)
			*(uint16_t*) ((unsigned long)Plane + i*XRES*2 + (newx +(newxx-newx)*(i-newy)/(newyy-newy))*2) = color;	
			} 
}
int GE_DrawDot16bpp(int x,int y,uint32_t color)
{
        int pixel; 
#if 1
        if(x>=XRES)
          return 0;
        if(y>=YRES)
          return 0;
#endif
        *(uint32_t*)(VRAMAddress + (y * XRES*4) + x*4 ) = (uint32_t)color;
 
        return 1;
}

void GE_FillRectangle(int X, int Y, int sizeX, int sizeY, uint32_t color)
{
 				int i,j;
	  		uint32_t *pframe;
 
    		pframe = VRAMAddress; 
 				j = sizeY;    
 				while(j--)
 				{
 					pframe =  VRAMAddress  + (j-1+Y)*XRES*4 + (X)*4;
 					i = sizeX;
 					while(i--)
 					{
 					*pframe++ = color;
 					}
 				}
}

uint32_t		PCnt=0;

void GE_FillEclipse(int CenterX, int CenterY, int sizeX, int sizeY, uint32_t color)
{
 				long i,j,k;
 				long rx,ry,rm, R2dir;
	  		uint32_t *pframe;
 
    		pframe = VRAMAddress; 
				R2dir = (sizeX/2)*(sizeX/2);				//(sizeX/2 * 1.414)*(sizeX/2 * 1.414)

				PCnt+=1;
					
 				for(j=0;j<sizeY;j++)
 				{
 					pframe =  VRAMAddress + ( CenterY-(sizeY/2) + j )*XRES*4 + (CenterX-(sizeX/2))*4;
 					
 					i = sizeX;
 	 					
 					k=0;
 					while(i--)
 					{
 						rx = abs( (sizeX/2)-k );
 						ry = abs( (sizeY/2)-j );
 						
 						rm = (rx*rx) + (ry*ry);
#if 0
								if(j==5 && PCnt==1)
									{
										printf("PCnt %d  - rx,ry = %d, %d,,  rm,R2dir = %d, %d  \n",		 PCnt,	rx, ry, rm, R2dir);
									}
#endif								
						if( rm < R2dir  )
						{
 						*pframe = color;
 						}	
 						pframe++;
 						k++;
 					}
 				}
}



void GE_DisplayString_OverWrite(unsigned short *plane, int x, int y, char *text, int text_len,short FrontColor,short BackColor)
{
	int i,j;
	for ( i = 0; i<text_len; i++)
	{
//		Dsp_char16( x + i*8, y,*(unsigned char*)(text+i), (unsigned short *)plane,FrontColor,BackColor);
	}
}
//													destinationx,y, destinated size,  color to be,	original x,y, original size
GE_CopyPictureShrinked(int x,int y,int sizeX,int sizeY,int color,int xx,int yy,int sizeXX,int sizeYY)
{											//dest																				//source
				int i,j;
				short kk,colorlvl;
				int pixel;
				int r,g,b;
 				for( j = 0; j<sizeY; j++)
 				for( i = 0; i<sizeX; i++)
 					{
 					kk = *(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + ((yy+(sizeYY*j/sizeY))*XRES*2) + ((xx+(sizeXX*i/sizeX))*2) );

#if 0		//BPP16
 		#if 1
 					 if(kk!=0) {
 					
 					colorlvl = (kk>>5) & 0x03F;
 
 					pixel = color & 0x0000F800;
 					r = ((pixel>>11)*colorlvl /0x003f);
//				putString("(pixel>>11)*incolor=%x,pixel =%x,r = %x\n",(pixel>>11)*incolor,pixel,r);

 					pixel = color & 0x07E0;
 					g = ((pixel>>5)*colorlvl /0x003f);
 					
 					pixel = color & 0x001F;
 					b = ((pixel)*colorlvl /0x003f);			

 					*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (y+j)*XRES*2 +  x*2  + i*2 ) = (short)(r<<11 | g<<5 | b);
 					*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (y+j)*XRES*2 +  x*2  + i*2 ) = kk;
		#else
					*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (y+j)*XRES*2 +  x*2  + i*2 ) = kk;
		#endif
		
				  }
#endif

#if 1		//BPP15
 		#if 1
 					 if(kk!=0) {
 					
 					colorlvl = (color>>5) & 0x03F;
 
 					pixel = kk & 0x0000F800;
 					r = ((pixel>>11)*colorlvl /0x003f);
//				putString("(pixel>>11)*incolor=%x,pixel =%x,r = %x\n",(pixel>>11)*incolor,pixel,r);

 					pixel = kk & 0x07E0;
 					g = ((pixel>>5)*colorlvl /0x003f);
 					 g = g >> 1;
 					 g = g <<1;
 					 
 					pixel = kk & 0x001F;
 					b = ((pixel)*colorlvl /0x003f);			

 					*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (y+j)*XRES*2 +  x*2  + i*2 ) = (short)(r<<11 | g<<5 | b);

		#else
					*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (y+j)*XRES*2 +  x*2  + i*2 ) = kk;
		#endif
		
				  }
#endif
 				  }
}

GE_BWtoCOLORPictureShrinked(int x,int y,int sizeX,int sizeY,int color,int sizeXX,int sizeYY)
{
				int i,j;
				short xx,incolor;
				int pixel;
				int r,g,b;
 				for( j = 0; j<sizeYY; j++)
 				for( i = 0; i<sizeXX; i++)
 					{
 					xx = *(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + ((sizeY*j/sizeYY)*XRES*2) + ((sizeX*i/sizeX)*2) );
#if BPP16
 		#if 1
 					incolor = (xx>>5) & 0x03F;
 
 					pixel = color & 0x0000F800;
 					r = ((pixel>>11)*incolor /0x003f);
//				putString("(pixel>>11)*incolor=%x,pixel =%x,r = %x\n",(pixel>>11)*incolor,pixel,r);

 					pixel = color & 0x07E0;
 					g = ((pixel>>5)*incolor /0x003f);
 					
 					pixel = color & 0x001F;
 					b = ((pixel)*incolor /0x003f);			

 					*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (y+j)*XRES*2 +  x*2  + i*2 ) = (short)(r<<11 | g<<5 | b);
		#else
					*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (y+j)*XRES*2 +  x*2  + i*2 ) = xx;
		#endif
#endif

#if BPP15
 		#if 1
 					incolor = (xx>>5) & 0x01F;
 
 					pixel = color & 0x00007C00;
 					r = ((pixel>>10)*incolor /0x001f);
//				putString("(pixel>>11)*incolor=%x,pixel =%x,r = %x\n",(pixel>>11)*incolor,pixel,r);

 					pixel = color & 0x03E0;
 					g = ((pixel>>5)*incolor /0x001f);
 					
 					pixel = color & 0x001F;
 					b = ((pixel)*incolor /0x001f);			

 					*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (y+j)*XRES*2 +  x*2  + i*2 ) = (short)(r<<10 | g<<5 | b);
		#else
					*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (y+j)*XRES*2 +  x*2  + i*2 ) = xx;
		#endif
#endif
 
 				  }
}
GE_BWtoCOLORPicture(int x,int y,int sizeX,int sizeY,int color)
{
				int i,j;
				short xx,incolor;
				int pixel;
				int r,g,b;
 				for( j = 0; j<sizeY; j++)
 				for( i = 0; i<sizeX; i++)
 					{
 					xx = *(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (j)*XRES*2 + (i)*2 );
 					
 		#if 1
 					incolor = (xx>>5) & 0x03F;
 
 					pixel = color & 0x0000F800;
 					r = ((pixel>>11)*incolor /0x003f);
//				putString("(pixel>>11)*incolor=%x,pixel =%x,r = %x\n",(pixel>>11)*incolor,pixel,r);

 					pixel = color & 0x07E0;
 					g = ((pixel>>5)*incolor /0x003f);
 					
 					pixel = color & 0x001F;
 					b = ((pixel)*incolor /0x003f);			

 					*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (y+j)*XRES*2 +  x*2  + i*2 ) = (short)(r<<11 | g<<5 | b);
		#else
					*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (y+j)*XRES*2 +  x*2  + i*2 ) = xx;
		#endif
 				  }
}

GE_BWtoCOLORPictureTEXT(int x,int y,int sizeX,int sizeY,int color,int NoGray)
{
 
 
				int i,j;
				short xx,incolor;
				int pixel;
				int r,g,b;
 				for( j = 0; j<sizeY; j++)
 				for( i = 0; i<sizeX; i++)
 				{
 					if( (*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (j)*XRES*2 + (i)*2 ))!= 0x0000)
 						{
 							
 						if(NoGray == 1)
 						{
 									*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (y+j)*XRES*2 +  x*2  + i*2 ) = (short)color;
 						}
 						else
 						{
 							
 									xx = *(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (j)*XRES*2 + (i)*2 );
 		#if 1     		
 									incolor = (xx>>5) & 0x03F;
              		
 									pixel = color & 0x0000F800;
 									r = ((pixel>>11)*incolor /0x003f);
//								putString("(pixel>>11)*incolor=%x,pixel =%x,r = %x\n",(pixel>>11)*incolor,pixel,r);
              		
 									pixel = color & 0x07E0;
 									g = ((pixel>>5)*incolor /0x003f);
 					    		
 									pixel = color & 0x001F;
 									b = ((pixel)*incolor /0x003f);			

 									*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (y+j)*XRES*2 +  x*2  + i*2 ) = (short)(r<<11 | g<<5 | b);
		#else
									*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (y+j)*XRES*2 +  x*2  + i*2 ) = color & (*(uint16_t*) ((unsigned long)Migo_BackWallBufPtr + (j)*XRES*2 + (i)*2 ));
		#endif
 									}
 				  	}
 				}
}
 
DspChineseFont(int x,int y,int font)
{
	int fontbyte=0;
	int fontword;
	int xx=x;
	int yy=y;
	int offset;
	char c;   
	offset = 0xad900000 + font *0x20 +0x100;
	for(fontword=0;fontword<16;fontword++)
		{                          	
		c  = *(uint8_t*)offset;
		if (c&0x80) GE_DrawDot16bpp(xx,yy,0xffff);else  GE_DrawDot16bpp(xx,yy,0x0000);
        	if (c&0x40) GE_DrawDot16bpp(xx+1,yy,0xffff);else  GE_DrawDot16bpp(xx+1,yy,0x0000);
        	if (c&0x20) GE_DrawDot16bpp(xx+2,yy,0xffff);else  GE_DrawDot16bpp(xx+2,yy,0x0000);
        	if (c&0x10) GE_DrawDot16bpp(xx+3,yy,0xffff);else  GE_DrawDot16bpp(xx+3,yy,0x0000);
        	if (c&0x08) GE_DrawDot16bpp(xx+4,yy,0xffff);else  GE_DrawDot16bpp(xx+4,yy,0x0000);
        	if (c&0x04) GE_DrawDot16bpp(xx+5,yy,0xffff);else  GE_DrawDot16bpp(xx+5,yy,0x0000);
        	if (c&0x02) GE_DrawDot16bpp(xx+6,yy,0xffff);else  GE_DrawDot16bpp(xx+6,yy,0x0000);
        	if (c&0x01) GE_DrawDot16bpp(xx+7,yy,0xffff);else  GE_DrawDot16bpp(xx+7,yy,0x0000);
        	c = *(uint8_t*)(offset+1);
        	if (c&0x80) GE_DrawDot16bpp(xx+8,yy,0xffff);else  GE_DrawDot16bpp(xx+8,yy,0x0000);
        	if (c&0x40) GE_DrawDot16bpp(xx+9,yy,0xffff);else  GE_DrawDot16bpp(xx+9,yy,0x0000);
        	if (c&0x20) GE_DrawDot16bpp(xx+10,yy,0xffff);else  GE_DrawDot16bpp(xx+10,yy,0x0000);
        	if (c&0x10) GE_DrawDot16bpp(xx+11,yy,0xffff);else  GE_DrawDot16bpp(xx+11,yy,0x0000);
        	if (c&0x08) GE_DrawDot16bpp(xx+12,yy,0xffff);else  GE_DrawDot16bpp(xx+12,yy,0x0000);
        	if (c&0x04) GE_DrawDot16bpp(xx+13,yy,0xffff);else  GE_DrawDot16bpp(xx+13,yy,0x0000);
        	if (c&0x02) GE_DrawDot16bpp(xx+14,yy,0xffff);else  GE_DrawDot16bpp(xx+14,yy,0x0000);
        	if (c&0x01) GE_DrawDot16bpp(xx+15,yy,0xffff);else  GE_DrawDot16bpp(xx+15,yy,0x0000);
		offset+=2;
		yy+=1;
		}		
}

DumpChineseChar(int startnum)
{
	int fontptr=startnum;
	int i;
	int j;
	for(j=0;j<30;j++)
	{
		for(i=0;i<50;i++)
		{
		DspChineseFont(i*16,j*16,fontptr);
		fontptr+=1;
		}
	}
}

TestChineseChar()
{
	  int i=0;
	  int page=0;
 
	  while(1)
	  {
	  DumpChineseChar(i);
	  i=i+30*50;
 
	  page++;
 //         GetTerminalKey();
		if(page>50)
			break;
	  }
}

DspRetangeDashRED(int x,int y,int sizex,int sizey,int color)
{
	int i;
	for(i=0;i<(sizex);i+=2)
 	GE_DrawDot16bpp(x+i,y,color);
	
	for(i=0;i<(sizex);i+=2)
	GE_DrawDot16bpp(x+i,y+sizey,color);	
	
	for(i=0;i<(sizey);i+=2)
	GE_DrawDot16bpp(x,y+i,color);	
		
	for(i=0;i<(sizey);i+=2)
	GE_DrawDot16bpp(x+sizex,y+i,color);		
}


RecoverBackWall(int x,int y,int sizex,int sizey)
{
	int i,j,k;
	for(i=0;i<sizey;i++)
	memcpy(VRAMAddress+XRES *2*(y+i)+ x*2,Migo_BackWallBufPtr+XRES *2*(y+i)+ x*2 ,sizex*2); 
	
}
RefreshBackScreen()
{
	int i,j,kk;	
 	
// 	memcpy(VRAMAddress,Migo_BackWallBufPtr,800*600*2);
 
 	memcpy(Migo_BackWallBufPtr,VRAMAddress,800*600*2/4);	
#if 0 
	DrawRetangle(60,174+ptr*32,33,238,0xf800);
 
	DrawHLine(60,174+y*ptr,x,0xf800);
	DrawHLine(60,174+y*ptr+y+1,x,0xf800);	
	
	DrawVLine(60,174+y*ptr,y+1,0xf800);
	DrawVLine(60+x,174+y*ptr,y+1,0xf800); 	
#endif
	return;
}

CFRefreshMenu(int ptr)
{
	int x,y=0;
	DrawHLine(60,174+y*ptr,x,0xf800);
	DrawHLine(60,174+y*ptr+y+1,x,0xf800);	
	
	DrawVLine(60,174+y*ptr,y+1,0xf800);
	DrawVLine(60+x,174+y*ptr,y+1,0xf800);
	
	}

DrawRetangle(int x,int y,int sizex,int sizey,int color)
{
	DrawHLine(x,y,sizex,color);
	DrawHLine(x,y+sizey,sizex,color); 
	DrawVLine(x,y,sizey,color);
	DrawVLine(x+sizex,y,sizey,color); 	
 	return;
}
 

DisplayLevelDown(int color)
{
	
	
	int x,y,offset=0;
	int MAXLevel;
 
	if(color == BLUE)
		MAXLevel = 0x001f;
	else
	  	MAXLevel = 0xffff;
	  	
	for(y=0;y<YRES;y++)
	{
		x=0;

#if enIT8181
		while(x<XRES)
		{
	if(color == BLUE)
		*(uint16_t*)(VRAMAddress + offset) = y * MAXLevel/YRES ;
	if(color == WHITE)
		*(uint16_t*)(VRAMAddress + offset) = 0xffff;
		x++;
		offset+=2;}
#endif
	
#if enIGS5050
		while(x<XRES)
		{
	if(color == BLUE)
		*(uint16_t*)((int)Migo_BackWallBufPtr+ offset) = y * MAXLevel/YRES ;
	if(color == WHITE)
		*(uint16_t*)((int)Migo_BackWallBufPtr + offset) = 0xffff;
		x++;
		offset+=2;}
 

 
#endif	
	}
	 RefreshBackScreen();
}


 
 




int DspBlockDialog(char *ptr ,int Para)
{
	int i=0,j,k;
	char *iptr;
	int TimeFlag=0;
	int RTNFlag;

	
	iptr=ptr;

	while(*iptr != 0x0a)
	{i+=1;
	iptr+=1;}
	k=(i/2) * 16 + 5 * 16;
		
	i=8;j=8;
	while(i<k || j<16*4)
        {
        	DspBtn3D((XRES-i)/2,(YRES-j)/2,i,j,0x10000);
        GE_TmrDelay(2);
        i+=(k/8);
        j+=16;
        if(j>=16*4)
        	j=16*4;
        if(i>=k)
        	i=k;       
        }	
        GE_TmrDelay(20);
        DspBtn3D((XRES-k)/2,(YRES-16*4)/2,i,j,0x00000); 
        
	VGADspSetColor(CharColorGREEN ,TRANSPARENT );	//Set Character Displa	
	VGADspSetLocation( ((XRES-k)/2) +5*8,((YRES-16*4)/2)+16);
	
 	while(*ptr != 0x0a)
 	VGADspChar(*(ptr++));     
 	    
        if(Para != REPLY)
	return success;
	

	VGADspSetColor(CharColorRED ,TRANSPARENT ); 
	VGADspSetLocation( ((XRES-k)/2) +5*8,((YRES-16*4)/2)+16*2);
	
//	CallVGADspNO(); 


#if 0
 	while(1)
 	{
 		while(KeyMxScanCode()!=success )
 		{
 
			if(IsSecondChanged()==success && TimeFlag==0)
				{
					TimeFlag=1;
					if(RTNFlag==1)
					{
					VGADspSetColor(CharColorRED ,TRANSPARENT );	//Set Character Display	
					VGADspSetLocation( ((XRES-k)/2) +5*8,((YRES-16*4)/2)+16*2); 
	
//					CallVGADspOK(); 
  					}
 					 else
					{
					VGADspSetColor(CharColorRED ,TRANSPARENT );	//Set Character Display	
					VGADspSetLocation( ((XRES-k)/2) +5*8,((YRES-16*4)/2)+16*2); 
	
//					CallVGADspNO(); 
 					} 				
 				}
 				GE_TmrDelay(50);
 				TimeFlag=0;
 			 	DspFillBlock(((XRES-k)/2) +5*8,((YRES-16*4)/2)+16*2,50*8,16,0);
 
		}
#if 0	 			
 		switch(KeyMxGetCode())
 			{
 			case INCkey:
 			case INCkeyR:
 				if(RTNFlag==0)
 					RTNFlag=1;
 					else
 					RTNFlag=0;
 				break;
 			case DECkey:
 			case DECkeyR:
 				if(RTNFlag==0)
 					RTNFlag=1;
 					else
 					RTNFlag=0;
 				break;
 			case DeepINCkey:			
 			case DeepINCkeyR:
 				if(RTNFlag==0)
 					RTNFlag=1;
 					else
 					RTNFlag=0;
 				break;
 			case DeepDECkey:
 			case DeepDECkeyR:
 				if(RTNFlag==0)
 					RTNFlag=1;
 					else
 					RTNFlag=0;
 				break; 				
 				
 			case SELkeyDown:
 							
 				break;
 			case SELkeyUp:	
 
				return RTNFlag;
			default:
				break;
 			}
#endif
	}	
	
#endif
}

/*
xsize16  =  size of X, which is base on 16 dot unit
xsize =  size of X, which is base on JPEG file picture 
ysize = size of Y, which is base on JPEG file picture 

RGBplane is LCD display buffer
*/




uint8_t Compute_r( unsigned char Y,  unsigned char U,  unsigned char V, float factor)
{
	float r;

	r = (float)Y*factor + (1.40200*(float)(V-128));
	if(r<0)
		r = 0;
	else if( r>255 )
		r = 255;
	return((uint8_t) r);
}

uint8_t Compute_g(unsigned char Y, unsigned char U, unsigned char V, float factor)
{
	float g;

	g = (float)Y*factor - (0.34414*(float)(U-128) + 0.71414*(float)(V-128));
	if(g<0)
		g = 0;
	else if( g>255 )
		g = 255;
	return((uint8_t) g);
}

uint8_t Compute_b(unsigned char Y, unsigned char U, unsigned char V, float factor)
{
	float b;

	b = (float)Y*factor + (1.77200*(float)(U-128));
	if(b<0)
		b = 0;
	else if( b>255 )
		b = 255;
	return((uint8_t) b);
}

int YUV2RGB(int YUVaddr,int xsize16,int xsize,int ysize, int RGBplane)
{
	int x,y,ptr;
 
	uint8_t R,G,B;
	uint8_t Y,Cb,Cr;
	unsigned long pixel;
	
	for(y=0;y<ysize;y++)
	{
		for(x=0;x<xsize16;x++)
		{
		ptr = y*xsize16 + x;
 
		Y=   *(uint8_t*)(YUVaddr+ptr);
		Cb=  *(uint8_t*)(YUVaddr + xsize16*ysize + ((xsize16/2)*(y/2)) + x/2 );
		Cr=  *(uint8_t*)(YUVaddr + xsize16*ysize + xsize16*ysize/4  + ((xsize16/2)*(y/2)) + x/2 );
  
		R = Compute_r(Y,Cb,Cr,0.001) & 0x000000ff;
		G = Compute_g(Y,Cb,Cr,0.001) & 0x000000ff;
		B = Compute_b(Y,Cb,Cr,0.001) & 0x000000ff;
		
		if(x < xsize)
   	*(uint32_t*)(RGBplane+ XRES*4*y + x*4) = ((R<<16) + (G<<8) + B);
 
		}
	}

}

YCbCrToR(unsigned char Y, unsigned char Cb, unsigned char Cr)
{
	int R;
	R = (char) (((Y-16)*298 + (Cr-128)*408)/256 );
#if 0
	if(R<0)
		R=0;
	if(R>=255)
		R=255;
#endif

	return R;
}

YCbCrToG(unsigned char Y, unsigned char Cb, unsigned char Cr)
{
 	int G;
	G = (char) (((Y-16)*298 - (Cb-128)*100 - (Cr-128)*208)/256);
#if 0	
	if(G<0)
		G=0;
	if(G>=255)
		G=255;
#endif
	return G;
}

YCbCrToB(unsigned char Y, unsigned char Cb, unsigned char Cr)
{
 	int B;
	B = (char) (((Y-16)*298 + (Cb-128)*516)/256);
#if 0
	if(B<0)
		B=0;
	if(B>=255)
		B=255;
#endif
	return B;
}



 
/******************************************************************************/
/* Function   : ConvertYCbCrtoRGB565                                          */
/* Description: It changes into RGB from YCbCr						          */
/* Parameters : short inputY  : value of Y                                    */
/*              short inputCb : value of Cb                                   */
/*				short inputCr : value of Cr                                   */
/* Returns    : value of RGB565                                               */
/* InOut      : none				                                          */
/* note       :                                                               */
/*  R = Y                            +   1402/1000 (Cr - 128)                 */
/*  G = Y + 202008/587000(Cb - 128 ) + 419198/58700(Cr - 128)                 */
/*  B = Y +   1772/1000  (Cb - 128 )                                          */
/*  BGB 565                                                                   */
/*  <- 5bit R -><- 6bit G -><- 5bit B ->                                      */
/******************************************************************************/
/* Update                                                                     */
/*   Rev.     Date     Reviser      Description                               */
/******************************************************************************/
uint32_t ConvertYCbCrtoRGB565(unsigned char inputY,unsigned char inputCb,unsigned char inputCr)
{
    unsigned short rgb;
	short	yy,cr,cb;
	short	rr,gg,bb;
    yy = (short)inputY;
    cb = (short)((short)inputCb - 128);// �����́h128�h��Range��"-0.5 to 0.5"�ɂ������
    cr = (short)((short)inputCr - 128);
    // YCrCb->RGB�ϊ��F������?�Z�ł̓I�[�o/�A?�_�t?�[������

    rr	= (short)(((yy << 8)           + cr * 359 + 128) >> 8);//�����́h128�h�͍ŉ��ʃr�b�g�̐؂�グ�A�؂�̂ėp
    gg  = (short)(((yy << 8) - cb * 88 - cr * 183 + 128) >> 8);
    bb  = (short)(((yy << 8) + cb * 454           + 128) >> 8);
    /*�I�[�o/�A?�_�t?�[�`�F�b�N*/
    if(rr>255) rr = 255;
    if(rr<0)   rr =   0;
    if(gg>255) gg = 255;
    if(gg<0)   gg =   0;
    if(bb>255) bb = 255;
    if(bb<0)   bb =   0;
    /*RGB?���o��*/
 return ((rr<<16) + (gg<<8) + bb);
}

#if 0

/*""FUNC COMMENT""*******************************************************
 * ID          : 
 * ��?��      : char *ycbcr_rgb()
 *-----------------------------------------------------------------------
 * Include     : 
 *-----------------------------------------------------------------------
 * �錾        : char *ycbcr_rgb(int limit,Byte **Y,char *fp,int r0,int r1,int r2)
 *-----------------------------------------------------------------------
 * �@�\        : Y,Cb,Cr�f�[�^��R,G,B�f�[�^�ɕϊ����A
 *               BMPBUF[](�r�b�g�}�b�v�f�[�^�i�[��)��?��?�ށB
 *-----------------------------------------------------------------------
 * ��?        : int  limit�@  : ?��?�݃f�[�^?
 *               Byte **Y      : Y�����̊i�[�̈�������|�C?�^
 *               char *fp      : BMPBUF[]�̈�������|�C?�^
 *               int  r0,r1,r2 : Y,Cb,Cr ���@R,G,B�ւ̕ϊ��l
 *----------------------------------------------------------------------- 
 * �߂�l      : ?�f�[�^��?��?�݃A�h?�X(BMPBUF[]�̈�)�������|�C?�^
 *-----------------------------------------------------------------------
 * ����        : 
 * �o��        : 
 *-----------------------------------------------------------------------
 * �g�p��?    : 
 *-----------------------------------------------------------------------
 * ?��??    : 
 *""FUNC COMMENT END""***************************************************/
char *ycbcr_rgb(int limit,Byte **Y,char *fp,int r0,int r1,int r2)
{
	int cnt;
	int y, r, g, b;

	for(cnt = 0 ; cnt < limit ; cnt++ ) {
		y = **Y;
		*Y += 1;
		y <<= 8;
		r = (y + r0) >> 8;
		g = (y - r1) >> 8;
		b = (y + r2) >> 8;
		if (r < 0)	r = 0;
		else if (r > 0xff)	r = 0xff;
		if (g < 0)	g = 0;
		else if (g > 0xff)	g = 0xff;
		if (b < 0)	b = 0;
		else if (b > 0xff)	b = 0xff;
		*fp++ = (char)b;			/* B �f�[�^?������ */
		*fp++ = (char)g;			/* G �f�[�^?������ */
		*fp++ = (char)r;			/* R �f�[�^?������ */
	}
	return fp;
}
#endif

/*--------------------------------------------------------
Matrix f�r sYCC, Amendment 1 to IEC 61966-2-1

Y :   0.299   0.587    0.114   :R
Cb:  -0.1687 -0.3312   0.5     :G
Cr:   0.5    -0.4187  -0.0812  :B

Inverse:

R: 1        -3.68213e-05    1.40199      :Y
G: 1.00003  -0.344125      -0.714128     :Cb - 2^(prec - 1)
B: 0.999823  1.77204       -8.04142e-06  :Cr - 2^(prec - 1)

-----------------------------------------------------------*/
uint32_t sycc_to_rgb( int y, int cb, int cr)
{
	int r, g, b;

	r = y + (int)(1.402 * (float)cr);
  	if(r>0xf0)
  			r=0xf0;
  	else if (r<0) r=0;

	g = y - (int)(0.344 * (float)cb + 0.714 * (float)cr);

	b = y + (int)(1.772 * (float)cb);

 return ((r<<16) + (g<<8) + b);
}

GE_TmrDelay(int cnt)
{
	int dlycnt=0;
	while(dlycnt<cnt)
			{dlycnt++;
				}	
}

int DspBtn3D(int x,int y,int sizex,int sizey,int color)
{

	if(color==0x20000)
	{
	DspFillBlock(x,y,sizex-1,1,0);
	DspFillBlock(x+1,y+1,sizex-2,1,0);
	DspFillBlock(x+2,y+2,sizex-3,1,0);
	DspFillBlock(x+3,y+3,sizex-4,1,0);
	
	DspFillBlock(x,y+1,1,sizey-2,0);
	DspFillBlock(x+1,y+2,1,sizey-4,0);	
	DspFillBlock(x+2,y+3,1,sizey-6,0);		
	DspFillBlock(x+3,y+4,1,sizey-8,0);	
	
	DspFillBlock(x+sizex-1,y,1,sizey,0);
	DspFillBlock(x+sizex-2,y+1,1,sizey-2,0);
	DspFillBlock(x+sizex-3,y+2,1,sizey-4,0);
	DspFillBlock(x+sizex-4,y+3,1,sizey-6,0);
	
	DspFillBlock(x ,y+sizey-1,sizex,1,0);
	DspFillBlock(x+1 ,y+sizey-2,sizex-2,1,0);	
	DspFillBlock(x+2 ,y+sizey-3,sizex-4,1,0);	
	DspFillBlock(x+3 ,y+sizey-4,sizex-6,1,0);		

	}
	else
	{
	DspFillBlock(x,y,sizex-1,1,CharColorWHITE);
	DspFillBlock(x+1,y+1,sizex-2,1,CharColorGRAYWHITE);
	DspFillBlock(x+2,y+2,sizex-3,1,CharColorGRAY);
	DspFillBlock(x+3,y+3,sizex-4,1,CharColorBLACK);
	
	DspFillBlock(x,y+1,1,sizey-2,CharColorWHITE);
	DspFillBlock(x+1,y+2,1,sizey-4,CharColorGRAYWHITE);	
	DspFillBlock(x+2,y+3,1,sizey-6,CharColorGRAY);		
	DspFillBlock(x+3,y+4,1,sizey-8,CharColorBLACK);	
	
	DspFillBlock(x+sizex-1,y,1,sizey,CharColorBLACK);
	DspFillBlock(x+sizex-2,y+1,1,sizey-2,CharColorGRAY);
	DspFillBlock(x+sizex-3,y+2,1,sizey-4,CharColorGRAYWHITE);
	DspFillBlock(x+sizex-4,y+3,1,sizey-6,CharColorWHITE);
	
	DspFillBlock(x ,y+sizey-1,sizex,1,CharColorBLACK);
	DspFillBlock(x+1 ,y+sizey-2,sizex-2,1,CharColorGRAY);	
	DspFillBlock(x+2 ,y+sizey-3,sizex-4,1,CharColorGRAYWHITE);	
	DspFillBlock(x+3 ,y+sizey-4,sizex-6,1,CharColorWHITE);		
		if((color&0x10000)!=0x10000)
		DspFillBlock(x+4 ,y+4,sizex-8,sizey-8,color);	
	}	
}

void RETW_GE_RetanglePolycon(uint32_t *FrameBuf, int x, int y, int sizeX, int sizeY, uint32_t Color)
{
	int xptr,yptr;
	uint32_t *bufaddr;
 
	for( yptr = 0; yptr<sizeY; yptr++)
	{
				bufaddr = FrameBuf+(y+yptr)*1280 + x ;
				xptr=sizeX;
				while(xptr)
				{
					*bufaddr++=Color;
					xptr--;
				}				
	}
}
//RSK2Gen
void RETW_GE_RetanglePolycon800x480(uint32_t *FrameBuf, int x, int y, int sizeX, int sizeY, uint32_t Color)
{
	int xptr,yptr;
	uint32_t *bufaddr;
 
	for( yptr = 0; yptr<sizeY; yptr++)
	{
				bufaddr = FrameBuf+(y+yptr)*800 + x ;
				xptr=sizeX;
				while(xptr)
				{
					*bufaddr++=Color;
					xptr--;
				}				
	}
}