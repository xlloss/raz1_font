/* ============================================================== */
/* =                                                            = */
/* =    File Name :  DspBMP.c                                   = */
/* =                                                            = */
/* ============================================================== */
//	Function :
//	1.Display BMP file

#include "sysdef.h"
#include "r_typedefs.h"
#include "display.h"

#define PrintSCI	printf

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

unsigned char palette1[256*3];


int LCDVirtualSizeX=400;
int LCDVirtualSizeY=240;

int LCDActualSizeX = Screen_SizeX;
int LCDActualSizeY = Screen_SizeY;
 
 
void DspGrayChart(int x,int y, unsigned long BackAddr);

void DspGrayChart(int x,int y, unsigned long BackAddr) 
{
    unsigned long   R,G,B,pixel;
    unsigned long i,j,k,Y;

		PrintSCI("Display Gray Chart \n");
 
		for(j = 0; j < 480; j++)
        			{
        			for(i = 0; i < 752; i++)
        					{
        					if(i==0)
        						pixel = 0x0316;
        					else
        						pixel = i;

        						R = (pixel >>2)&0x000000ff;
        						G = (pixel >>2)&0x000000ff;
        						B = (pixel >>2)&0x000000ff;
        						
        										pixel = (unsigned long)(  ((B << 16) & 0x00ff0000)|((G << 8) & 0x0000ff00)|((R  ) & 0x000000ff));							//XBGR
 	        									*(uint32_t*)(BackAddr +((j+y) * LCDActualSizeX*4) + (i+x)*4 ) = (unsigned long)pixel;       				
        				
        					}        				
        			}
}

void DspBMP(unsigned char *BmpImage,int x,int y, unsigned long BackAddr, int OpenVGbuf, int NoBackColor);

void DspBMP(unsigned char *BmpImage,int x,int y, unsigned long BackAddr, int OpenVGbuf, int NoBackColor)
{
    unsigned long  Type,Color;
    unsigned int  Width,Height;
    unsigned long   R,G,B,pixel;
    unsigned short Count;
    unsigned char ColorBytes;
    unsigned char *PixelStartPtr;
    unsigned char *PaletteStartPtr;
   
    unsigned char BackColor8BPP;
    unsigned long BackColor32BPP;       
       
    unsigned short biBitCount;
    unsigned long maxX,maxY,LineBytes;
    unsigned long i,j,k,Y;

	if(BmpImage[0] == 'B' && BmpImage[1] == 'M')
  	{
// 			PrintSCI("This is a BMP file..\n");

      	Type = (unsigned long)(BmpImage[33] << 24 | BmpImage[32] << 16 |
                             BmpImage[31] << 8  | BmpImage[30]);

      	if(Type)
        	{
          	switch(Type)
            		{
              		case 1 : PrintSCI("This program can not decompress RLE-8.\n");
                       		return;
              		case 2 : PrintSCI("This program can not decompress RLE-4.\n");
                       		return;
            		}
          		return;
        	}

        biBitCount = (unsigned short )(BmpImage[29] << 8 | BmpImage[28]);
        
        switch(biBitCount)
         {
          case 1 : Type = 1; 
			PrintSCI("This program can not support black/while.\n");
			break;
          case 4 : Type = 2;  
			PrintSCI("This program can not support 16 color.\n");
			break;
          case 8 : Type = 3; 
//      PrintSCI("This bmp is 256 colors.\n");
			break; 
	        case 24: Type = 4;  
//      PrintSCI("This bmp is True colors.\n");
			break; 
          default: Type = 0;
         }
      Width = (BmpImage[21] << 24 | BmpImage[20] << 16 |
               BmpImage[19] << 8  | BmpImage[18]);
      Height= (BmpImage[25] << 24 | BmpImage[24] << 16 |
               BmpImage[23] << 8  | BmpImage[22]);
	
//    	PrintSCI("Width  = %d\n",Width);
//   	  PrintSCI("Height = %d\n",Height);

      PixelStartPtr= BmpImage + (unsigned long )(BmpImage[13] << 24 | BmpImage[12] << 16 |
                                          BmpImage[11] << 8  | BmpImage[10]);
      PaletteStartPtr = (BmpImage + 54);

      if (Height > 800) maxY = 800; else maxY =Height;
      if (Width  > 1280) maxX = 1280; else maxX =Width;
// 	printf("BMP X = %d\n",maxX);
//	printf("BMP Y = %d\n",maxY);
 
		if(Type == 3)		//256 color
		{
//				PrintSCI("BMP file is 256 indexed colors \n");

				i=0;	
				for(k=0;k<256;k++)																				//fill palette array
				{
							palette1[k*3+0]= *(char*)(PaletteStartPtr	+k*4	+ 0);	//R
							palette1[k*3+1]= *(char*)(PaletteStartPtr+k*4	+ 1);	//G		    
							palette1[k*3+2]= *(char*)(PaletteStartPtr+k*4	+ 2);	//B
				}	
					//DumpData((char*)palette); 
      		LineBytes = Width;
      		if((LineBytes%4) > 0)
      			{
      			LineBytes &=0xfffffffc;
      			LineBytes +=0x00000004;
      		  }
      			
        	for(j = 0; j < maxY; j++)
        	{
          	Y = Height-j-1;

//	PrintSCI("Vertical Line =%d\n",Y);

						for(i=0;i<maxX;i++)
							{
								
              ColorBytes = *(volatile uint8_t*)(PixelStartPtr + (j * LineBytes)+i);
              
              if(i==0 && j==0)
              BackColor8BPP = ColorBytes;
              
//  PrintSCI("H Dot from =%x\n",PixelStartPtr + (j * LineBytes)+i);

	  	 	    	B=palette1[ColorBytes*3+0];  
 	  	  	    G=palette1[ColorBytes*3+1];
			    		R=palette1[ColorBytes*3+2];
#if (sysBPP == BPP888)

							if(BackColor8BPP != ColorBytes || NoBackColor==0)
							{
												if(OpenVGbuf == 0)
														{
			    									pixel = (long)((R << 16) & 0x00ff0000)|((G << 8) & 0x0000ff00)|((B ) & 0x000000ff) ;				//XRGB
 	        									*(volatile uint32_t*)(BackAddr +(( Y+y ) * LCDActualSizeX*4) + (i+x)*4 ) = (long)pixel;			//top to down
 	        									}
												else 
														{
														pixel = (long)((R << 16) & 0x00ff0000)|((G << 8) & 0x0000ff00)|((B ) & 0x000000ff) ;				//XBGR
														pixel += 0xFF000000;
														
 	        									*(volatile uint32_t*)(BackAddr +((Height-1-(Y+y)) * LCDVirtualSizeX*4) + (i+x)*4 ) = (long)pixel;			// down to top
 	        									}
 	        		}
#endif
#if (sysBPP == BPP565)
							if(BackColor8BPP != ColorBytes || NoBackColor==0)
							{
												if(OpenVGbuf == 0)
														{
			    									pixel = (short)((R << 8) & 0x0000f800)|((G << 3) & 0x000007e0)|((B >> 3) & 0x0000001f) ;
 	        									*(volatile uint16_t*)(BackAddr +(( Y+y ) * LCDActualSizeX*2) + (i+x)*2 ) = (short)pixel;
 	        								  }
												else 
														{
			    									pixel = (short)((R << 8) & 0x0000f800)|((G << 3) & 0x000007e0)|((B >> 3) & 0x0000001f) ;
 	        									*(volatile uint16_t*)(BackAddr +((Height-1-(Y+y)) * LCDVirtualSizeX*2) + (i+x)*2 ) = (short)pixel;															
														}
							}
#endif
							}
					}
		       return;
    	}	
	if(Type == 4)			//24bpp
 			{		 
//				PrintSCI("BMP file is 24bits colors \n");
      					LineBytes = Width*3;
      					if((LineBytes%4) > 0)
      						{
      						LineBytes &=0xfffffffc;
      						LineBytes +=0x00000004;
      		  			}
        							for(j = 0; j < maxY; j++)
        							{
          							Y = Height-j-1;
//												PrintSCI("Vertical Line =%d\n",Y);
													for(i=0;i<maxX;i++)
														{
 
                  	    		B = *(uint8_t*)(PixelStartPtr + (j * LineBytes)+i*3);
                  	    		G = *(uint8_t*)(PixelStartPtr + (j * LineBytes)+i*3+1);
                  	    		R = *(uint8_t*)(PixelStartPtr + (j * LineBytes)+i*3+2);

              							if(i==0 && j==0)
              							BackColor32BPP = (unsigned long)(  ((B << 16) & 0x00ff0000)|((G << 8) & 0x0000ff00)|((R  ) & 0x000000ff));
              
#if (sysBPP == BPP888)
						
												if(OpenVGbuf == 0)
														{
//			    									pixel = (unsigned long)(  ((B << 10) & 0x0003f000)|((G << 4) & 0x00000fc0)|((R>>2 ) & 0x0000003f));							//XBGR
														 
			    									pixel = (unsigned long)(  0xFF000000 | ((B << 16) & 0x00ff0000)|((G << 8) & 0x0000ff00)|((R  ) & 0x000000ff));							//XBGR
			    									if(pixel != BackColor32BPP || NoBackColor==0)
 	        									*(uint32_t*)(BackAddr +((Y+y) * LCDActualSizeX*4) + (i+x)*4 ) = (unsigned long)pixel;
 	        								}
 	        							else
 	        								{
			    									pixel = (unsigned long)(  ((B << 16) & 0x00ff0000)|((G << 8) & 0x0000ff00)|( R & 0x000000ff));							//XBGR
			    									//pixel &= 0x00ffffff;
			    									pixel |=0xff000000;
			    									if(pixel != BackColor32BPP || NoBackColor==0)	    									
 	        									*(uint32_t*)(BackAddr +((Height-1-(Y+y)) * LCDVirtualSizeX*4) + (i+x)*4 ) = (unsigned long)pixel; 					 
 	        								}
#endif

#if (sysBPP == BPP565)
												if(OpenVGbuf == 0)
														{
			    									pixel = (short)((R << 8) & 0x0000f800)|((G << 3) & 0x000007e0)|((B >> 3) & 0x0000001f) ;					//565
 	        									*(volatile uint16_t*)(BackAddr +(( Y+y ) * LCDActualSizeX*2) + (i+x)*2 ) = (short)pixel;
 	        								}
 	        							else
 	        								{
			    									pixel = (short)((R << 8) & 0x0000f800)|((G << 3) & 0x000007e0)|((B >> 3) & 0x0000001f) ;					//565
 	        									*(volatile uint16_t*)(BackAddr +((Height-1-(Y+y)) * LCDVirtualSizeX*2) + (i+x)*2 ) = (short)pixel; 	        									
 	        								}
#endif
        										}
											}
								return;
				}
        PrintSCI("Cannot support this bmp file .\n");
 		}
      PrintSCI("The file is not a bmp file format.\n");	 
}
 
void DspBMP_Rotate(unsigned char *BmpImage,int x,int y, unsigned long BackAddr, int NoBackColor)
{
    unsigned long  Type,Color;
    unsigned int  Width,Height;
    unsigned long   R,G,B,pixel;
    unsigned short Count;
    unsigned char ColorBytes;
    unsigned char *PixelStartPtr;
    unsigned char *PaletteStartPtr;
   
    unsigned char BackColor8BPP;
    unsigned long BackColor32BPP;       
       
    unsigned short biBitCount;
    unsigned long maxX,maxY,LineBytes;
    unsigned long i,j,k,Y;

	if(BmpImage[0] == 'B' && BmpImage[1] == 'M')
  	{
// 			PrintSCI("This is a BMP file..\n");

      	Type = (unsigned long)(BmpImage[33] << 24 | BmpImage[32] << 16 |
                             BmpImage[31] << 8  | BmpImage[30]);

      	if(Type)
        	{
          	switch(Type)
            		{
              		case 1 : PrintSCI("This program can not decompress RLE-8.\n");
                       		return;
              		case 2 : PrintSCI("This program can not decompress RLE-4.\n");
                       		return;
            		}
          		return;
        	}

        biBitCount = (unsigned short )(BmpImage[29] << 8 | BmpImage[28]);
        
        switch(biBitCount)
         {
          case 1 : Type = 1; 
			PrintSCI("This program can not support black/while.\n");
			break;
          case 4 : Type = 2;  
			PrintSCI("This program can not support 16 color.\n");
			break;
          case 8 : Type = 3; 
//      PrintSCI("This bmp is 256 colors.\n");
			break; 
	        case 24: Type = 4;  
//      PrintSCI("This bmp is True colors.\n");
			break; 
          default: Type = 0;
         }
      Width = (BmpImage[21] << 24 | BmpImage[20] << 16 |
               BmpImage[19] << 8  | BmpImage[18]);
      Height= (BmpImage[25] << 24 | BmpImage[24] << 16 |
               BmpImage[23] << 8  | BmpImage[22]);
	
//    	PrintSCI("Width  = %d\n",Width);
//   	  PrintSCI("Height = %d\n",Height);

      PixelStartPtr= BmpImage + (unsigned long )(BmpImage[13] << 24 | BmpImage[12] << 16 |
                                          BmpImage[11] << 8  | BmpImage[10]);
      PaletteStartPtr = (BmpImage + 54);

      if (Height > 800) maxY = 800; else maxY =Height;
      if (Width  > 1280) maxX = 1280; else maxX =Width;
// 	printf("BMP X = %d\n",maxX);
//	printf("BMP Y = %d\n",maxY);
 
		if(Type == 3)		//256 color
		{
//				PrintSCI("BMP file is 256 indexed colors \n");

				i=0;	
				for(k=0;k<256;k++)																				//fill palette array
				{
							palette1[k*3+0]= *(char*)(PaletteStartPtr	+k*4	+ 0);	//R
							palette1[k*3+1]= *(char*)(PaletteStartPtr+k*4	+ 1);	//G		    
							palette1[k*3+2]= *(char*)(PaletteStartPtr+k*4	+ 2);	//B
				}	
					//DumpData((char*)palette); 
      		LineBytes = Width;
      		if((LineBytes%4) > 0)
      			{
      			LineBytes &=0xfffffffc;
      			LineBytes +=0x00000004;
      		  }
      			
        	for(j = 0; j < maxY; j++)
        	{
          	Y = Height-j-1;

//	PrintSCI("Vertical Line =%d\n",Y);

						for(i=0;i<maxX;i++)
							{
								
              ColorBytes = *(volatile uint8_t*)(PixelStartPtr + (j * LineBytes)+i);
              
              if(i==0 && j==0)
              BackColor8BPP = ColorBytes;
              
//  PrintSCI("H Dot from =%x\n",PixelStartPtr + (j * LineBytes)+i);

	  	 	    	B=palette1[ColorBytes*3+0];  
 	  	  	    G=palette1[ColorBytes*3+1];
			    		R=palette1[ColorBytes*3+2];
#if (sysBPP == BPP888)

							if(BackColor8BPP != ColorBytes || NoBackColor==0)
							{
			    									pixel = (long)((R << 16) & 0x00ff0000)|((G << 8) & 0x0000ff00)|((B ) & 0x000000ff) ;				//XRGB
 	        									*(volatile uint32_t*)(BackAddr +(( Y+y ) * LCDVirtualSizeX*4) + (i+x)*4 ) = (long)pixel;			//top to down
 	        		}
#endif
#if (sysBPP == BPP565)
							if(BackColor8BPP != ColorBytes || NoBackColor==0)
							{
			    									pixel = (short)((R << 8) & 0x0000f800)|((G << 3) & 0x000007e0)|((B >> 3) & 0x0000001f) ;
 	        									*(volatile uint16_t*)(BackAddr +(( Y+y ) * LCDVirtualSizeX*2) + (i+x)*2 ) = (short)pixel;
							}
#endif
							}
					}
		       return;
    	}	
	if(Type == 4)			//24bpp
 			{		 
//				PrintSCI("BMP file is 24bits colors \n");
      					LineBytes = Width*3;
      					if((LineBytes%4) > 0)
      						{
      						LineBytes &=0xfffffffc;
      						LineBytes +=0x00000004;
      		  			}
        							for(j = 0; j < maxY; j++)
        							{
          							Y = Height-j-1;
//												PrintSCI("Vertical Line =%d\n",Y);
													for(i=0;i<maxX;i++)
														{
                  	    		B = *(uint8_t*)(PixelStartPtr + (j * LineBytes)+i*3);
                  	    		G = *(uint8_t*)(PixelStartPtr + (j * LineBytes)+i*3+1);
                  	    		R = *(uint8_t*)(PixelStartPtr + (j * LineBytes)+i*3+2);
                  	    		
              							if(i==0 && j==0)
              							BackColor32BPP = (unsigned long)(  ((B << 16) & 0x00ff0000)|((G << 8) & 0x0000ff00)|((R  ) & 0x000000ff));
#if (sysBPP == BPP888)
			    									pixel = (unsigned long)(   ((B << 16) & 0x00ff0000)|((G << 8) & 0x0000ff00)|((R  ) & 0x000000ff));							//XBGR
			    							
			    									if( (pixel == BackColor32BPP) &&  NoBackColor==1)
			    									*(uint32_t*)(BackAddr +((Y+y) * LCDVirtualSizeX*4) + (i+x)*4 ) = (unsigned long)(pixel);
			    									else
			    									*(uint32_t*)(BackAddr +((Y+y) * LCDVirtualSizeX*4) + (i+x)*4 ) = (unsigned long)(pixel | 0xFF000000);
#endif

#if (sysBPP == BPP565)
			    									pixel = (short)((R << 8) & 0x0000f800)|((G << 3) & 0x000007e0)|((B >> 3) & 0x0000001f) ;					//565
 	        									*(volatile uint16_t*)(BackAddr +(( Y+y ) * LCDVirtualSizeX*2) + (i+x)*2 ) = (short)pixel;
 
#endif
        										}
											}
								return;
				}
        PrintSCI("Cannot support this bmp file .\n");
 		}
      PrintSCI("The file is not a bmp file format.\n");	 
}

