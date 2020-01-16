/* ============================================================== */
/* =                                                            = */
/* =    File Name :  Term .c                                    = */
/* =                                                            = */
/* ============================================================== */


#include "string.h"
#include "stddef.h"
#include "stdio.h"
void PrintSCI( );
unsigned int RETWatohex(unsigned char x);


#define PutChar 		IoPutchar 						// by using IoPutchar2
#define vchar   volatile char
#define vshort  volatile short
#define vlong   volatile long		
#define success			1
#define fail			0




void PrintSCI( );
 
typedef struct {
 	int event;
 	char CmdString[16];
 	int CmdLength;
 	char ParaNum;
  unsigned int Para1;    
	unsigned int Para2;
	unsigned int Para3;
	unsigned int Para4;
} TerminalEventInfo,*PTerminalEventInfo;

int strcompare( char *Str, int len);

unsigned int Termkeybuf[64];
unsigned char bufptr ;
unsigned char SerialBuf[80];
unsigned char SerialBufPtr=0;
char zerof=0;



TerminalEventInfo TerminalInfo;

int 	Terminalbufptr=0;


int ReceiveSCICommandString()
{
	int 	TopPtr;
	int 	i;
	unsigned char 	CharData;
	unsigned char 	CommandBuf[64];
	
	if( ScanTerminalKeyStatus() == success)
	{
   		CharData =  GetTerminalKey();
   		if(CharData == 0x08)     		/* if back */
   		{
       	 		if(Terminalbufptr ==0 )         /* if most left */
                   		return;
     			PutChar(0x08);
     			PutChar(0x20);
     			PutChar(0x08);
     			Terminalbufptr = Terminalbufptr-1;
     			CommandBuf[Terminalbufptr]=0x20;
     			return;
   		}
   		if( CharData>='a' && CharData<='z' )
   			CharData-= ('a'-'A');
     		CommandBuf[Terminalbufptr]= CharData;
     		PutChar(CharData);
     		Terminalbufptr+=1;
     		TopPtr=Terminalbufptr;
     		
ParsingCmdString:
   		if(CharData == 0x0d)       /* if return key */
			{
			PutChar(0x0d);
			PutChar(0x0a);
 
			Terminalbufptr=0;
 			TerminalInfo.CmdLength=0;
 			TerminalInfo.ParaNum=0;
 			TerminalInfo.Para1=0;
 			TerminalInfo.Para2=0;
 			TerminalInfo.Para3=0;
 			TerminalInfo.Para4=0;
#if 1 
								while(Terminalbufptr<64)
									{	
									if(CommandBuf[Terminalbufptr]==0x0d)
											goto EndParsing;;
									
									if(CommandBuf[Terminalbufptr]==0x20 )
										{	Terminalbufptr++;
											break;
										}
									else
										{
 										TerminalInfo.CmdString[Terminalbufptr]=(CommandBuf[Terminalbufptr]);
								     		TerminalInfo.CmdLength+=1;
								     		Terminalbufptr++;
								     		}
									  }
		
		 						while(Terminalbufptr<64)
									{	
									if(CommandBuf[Terminalbufptr]==0x0d)
										goto EndParsing;;
								
									if(CommandBuf[Terminalbufptr]==0x20 )
									{	Terminalbufptr++;
										break;
									}
									else
										{
      					  	     		TerminalInfo.Para1 = TerminalInfo.Para1 << 4 ;
      					  	     		TerminalInfo.Para1 = TerminalInfo.Para1 + (unsigned int) RETWatohex( CommandBuf[Terminalbufptr] );
      					  	     		Terminalbufptr++;
		  					 		}
									}
								TerminalInfo.ParaNum =1;
								
								while(Terminalbufptr<64)
								{	
								if(CommandBuf[Terminalbufptr]==0x0d)
									goto EndParsing;;
			  				
								if(CommandBuf[Terminalbufptr]==0x20 )
									{	Terminalbufptr++;
									break;
									}
								else
									{
        					     		TerminalInfo.Para2 = TerminalInfo.Para2 << 4 ;
        					     		TerminalInfo.Para2 = TerminalInfo.Para2 + (unsigned int) RETWatohex( CommandBuf[Terminalbufptr] );
        					     		Terminalbufptr++;
		    				 		}
								}
								TerminalInfo.ParaNum =2;
								
								while(Terminalbufptr<64)
								{	
								if(CommandBuf[Terminalbufptr]==0x0d)
									goto EndParsing;;
			  				
								if(CommandBuf[Terminalbufptr]==0x20 )
									{	Terminalbufptr++;
									break;
									}
								else
									{
        					     		TerminalInfo.Para3 = TerminalInfo.Para3 << 4 ;
        					     		TerminalInfo.Para3 = TerminalInfo.Para3 + (unsigned int) RETWatohex( CommandBuf[Terminalbufptr] );
        					     		Terminalbufptr++;
		    				 	}
								}	
        				TerminalInfo.ParaNum =3;
        				
								while(Terminalbufptr<64)
								{	
								if(CommandBuf[Terminalbufptr]==0x0d)
										goto EndParsing;;
								
								if(CommandBuf[Terminalbufptr]==0x20 )
									{	Terminalbufptr++;
										break;
									}
								else
									{
    					    	     		TerminalInfo.Para4 = TerminalInfo.Para4 << 4 ;
    					    	     		TerminalInfo.Para4 = TerminalInfo.Para4 + (unsigned int) RETWatohex( CommandBuf[Terminalbufptr] );
    					    	     		Terminalbufptr++;
							     		}
								}
								TerminalInfo.ParaNum =4;
								
								while(Terminalbufptr<64)
								{	
								if(CommandBuf[Terminalbufptr]==0x0d)
									goto EndParsing;;
							
								if(CommandBuf[Terminalbufptr]==0x20 )
									{	Terminalbufptr++;
									break;
									}
								}     	

#endif

EndParsing:			
					TerminalInfo.event=1;

	        for(Terminalbufptr=0 ; Terminalbufptr<64 ;Terminalbufptr++)
        	CommandBuf[Terminalbufptr] = 0x20 ;
        		 
        		Terminalbufptr=0;
       }	//if 0x0d
		
	}	//if scan key success
}

void TermMain(void)
{
				ReceiveSCICommandString();
				if(TerminalInfo.event==1)
				{
				if( strcompare("1", 1) == 0 )
						{	PrintSCI("This it Item 1\n");												}
				
				else if( strcompare("D", 1) == 0 )
						{PrintSCI("Para1 and Para2 is %x,%x\n",TerminalInfo.Para1,	TerminalInfo.Para2);
							DumpDataByte(TerminalInfo.Para1,	TerminalInfo.Para2);}	
				else
						{DumpDataByte(TerminalInfo.Para1,	TerminalInfo.Para2);}					
				
				TerminalInfo.event=0;	
			  }
}

void Term_ShowMenu()
{
PrintSCI("\n");
PrintSCI("************************************************************\n");
PrintSCI("* Debug Terminal Program        	       Date:11 June 2008 *\n");
PrintSCI("* CopyRight(c) Renesas Technology Co.,Ltd.                 *\n");
PrintSCI("************************************************************\n");
PrintSCI("Command List:\n");
PrintSCI("D XXXXXXXX YYYYYYYY,Display Memory,X:Address, Y:Length\n");
PrintSCI("FB XXXXXXXX YYYYYYYY ZZ,Fill Memory byte,Command Format[:F ADDRESS LENGTH DATA(byte) ]\n");
PrintSCI("Modify Memory byte,Command Format[:M ADDRESS DATA(byte)]\n");
PrintSCI("Modify Memory word,Command Format[:MW ADDRESS DATA(word)]\n");
PrintSCI("Modify Memory int,Command Format[:ML ADDRESS DATA(int)]\n");
PrintSCI("Test Memory,Command Format[:V ADDRESS LENGTH]\n");
PrintSCI("5.Repeating Test Memory form 0xac000000 16M \n");
PrintSCI("\n");
PrintSCI("Please entry Command Format:\n");
}


int strcompare( char *Str, int len)
{
		int index=0;
		while(len>0)
		{
			if( TerminalInfo.CmdString[index]!= *Str++)
					return 1;
			len =len -1;
			index+=1;
		}
		return 0;
}

void Term(void)
{
        unsigned int iptr;
        unsigned int sptr;
        unsigned char i;
//	unsigned int icnt;
//        unsigned short iw;
//        unsigned int il;
//        unsigned int failcnt,okcnt;
//        unsigned int result;
//        unsigned char j;

//      unsigned int bp ;
        unsigned int timeoutcount ;
        unsigned int itemp;
        unsigned int ltemp ;
        unsigned char ctemp ;
        unsigned short wtemp;

        unsigned char FirstSF;
 

PrintSCI("\n");
PrintSCI("************************************************************\n");
PrintSCI("* Terminal Program        	             Date:7 May 2007 *\n");
PrintSCI("* CopyRight(c) Renesas Systems Solutions Asia Ltd.         *\n");
PrintSCI("************************************************************\n");

reentryTerm:
PrintSCI("\n");
PrintSCI("Command Format List Help:\n");
PrintSCI("Display Memory,Command Format[:D ADDRESS LENGTH     ]\n");
PrintSCI("Fill Memory byte,Command Format[:F ADDRESS LENGTH DATA(byte) ]\n");
PrintSCI("Modify Memory byte,Command Format[:M ADDRESS DATA(byte)]\n");
PrintSCI("Modify Memory word,Command Format[:MW ADDRESS DATA(word)]\n");
PrintSCI("Modify Memory int,Command Format[:ML ADDRESS DATA(int)]\n");
PrintSCI("Test Memory,Command Format[:V ADDRESS LENGTH]\n");
PrintSCI("5.Repeating Test Memory form 0xac000000 16M \n");
PrintSCI("\n");
PrintSCI("(Please entry Command Format).\n");
 
//while(1)
//TermMain();
 				
FlushTermKey:
        for(bufptr=0 ; bufptr<64 ;bufptr++)
        {
                Termkeybuf[bufptr] = 0x00000020 ;
        }
bufptr=0;
     
PrintSCI(":"); 

RepeatTermKey:
while( ScanTerminalKeyStatus() == fail)
        {}
   	iptr = GetTerminalKey();
   					if(iptr == 0x00000008)       /* if back */
   					{
    				   	 	if(bufptr ==0 )         /* if most left */
    				    	{
    				           		goto RepeatTermKey;
    				    	}
    				 		PutChar(0x08);
    				 		PutChar(0x20);
    				 		PutChar(0x08);
    				 		bufptr = bufptr-1;
    				 		Termkeybuf[bufptr]=0x00000020;
    				 		goto RepeatTermKey;
   					}

     Termkeybuf[bufptr]=iptr;
     PutChar(iptr);
     bufptr=bufptr+1;

   if(iptr == 0x0000000d)
   { iptr = Termkeybuf[0];
   	
     switch (iptr)
     {
     case '0':		
     case '1': 
     case '2': 
     case '3':      	
     case '4': 			
     case '5': 
     case '6': 
     case '7': 
     case '8': 
          				goto FlushTermKey;    
          				
     case '9': 
 
// 									Program_Download(0xa0000000,0x08000000,0x400000);
 									goto FlushTermKey; 

     case 'r': 
     case 'R': 
 
          				goto FlushTermKey; 
          				         	
     case 'D' :
     case 'd' :
          itemp = GetTermBufAddr(1);    /* Get start Address */
          ltemp = GetTermBufNext();     /* Get Length */
 
 
          PrintSCI("\n");
          iptr= 0;
          while( iptr < ltemp)
          {
            if (iptr%16 == 0)
            {
            PrintSCI("\n");
            PrintSCI("%x ",itemp+iptr);
            PrintSCI("  ");
            }
            ctemp = (*(vchar *)( itemp + iptr));
            dspbyte((unsigned char) ctemp);
            PrintSCI(" ");
            iptr=iptr+1;
          }
          PrintSCI("\n");
    
          goto FlushTermKey;

     case 'F' :
     case 'f' :
          itemp = GetTermBufAddr(1);    /* Get start Address */
          ltemp = GetTermBufNext();     /* Get Length */
          ctemp = (char)GetTermBufNext();
/* Fill Data */
          iptr= 0;
          while( iptr < ltemp)
          {
            (*(vchar *)( itemp + iptr))= ctemp;
            iptr=iptr+1;
          }
          PrintSCI("\n");
          goto FlushTermKey;
     case 'H':
     case 'h':
//	InitialSH7727LCD(6);
        goto reentryTerm;
 
 
 

//		TestMemory(0xac100000,0x1000000);
 
//        goto reentryTerm;
        

     case 'M' :
     case 'm' :

          iptr = Termkeybuf[1];
          switch (iptr)
          {
           case ' ':
/* Memory modify byte*/
                   itemp = GetTermBufAddr(2);    /* Get start Address */
                   ctemp = (char)GetTermBufNext();
                   PrintSCI("\n");
                   i= (*(vchar *) itemp );
                   PrintSCI("Before %x  ",itemp);
            			 dspbyte((unsigned char)i);
                   PrintSCI("\n");
                   (*(vchar *) itemp )= ctemp;
                   i= (*(vchar *) itemp );
                   if (i!=ctemp)
                      {
                      PrintSCI("Verify Error!\n");
                      }
                  PrintSCI("After  %x  ",itemp);
                  dspbyte((unsigned char) i);
                  PrintSCI("\n");
                  goto FlushTermKey;

 
 
          default:
                PrintSCI("***** Invalid command *****\n");
                goto FlushTermKey;
          }  /* end of case 'M' */
/* Serial Load function through SCI0 */
     case 'S' :
     case 's' :

          iptr = Termkeybuf[1];
          switch (iptr)
          {
           case 'L':
           case 'l':
/* Serial Load  */
                   itemp = GetTermBufAddr(2);    /* Get start Address */
                   PrintSCI("address=%x.\n",itemp);
                   ltemp = GetTermBufNext();
                   if( (itemp & 0x00000003) != 0x00000000)
                     {
                      PrintSCI("\nInvalid Address.\n");
                      goto FlushTermKey;
                     }

              PrintSCI("Please Send text file to COM port.\n");

              while( ScanTerminalKeyStatus() == fail )
                   {}
              timeoutcount = 200000;
              FirstSF = 0;
                    for (iptr=0 ; iptr < 0x400000;iptr++)
                        {
                        while( ScanTerminalKeyStatus() == fail )
                                {
                                        timeoutcount-= 1;
                                        if(timeoutcount%10000== 0)
                                        {
                                        PrintSCI("Less time =%d\n",timeoutcount);
                                        }
                                        if(timeoutcount == 0      )
                                        {
                                        PrintSCI("Error-TimeOut!\n");
                                        PrintSCI("transfer %d bytes \n",iptr  );
                                        PrintSCI("transfer H'%x bytes \n",iptr  );
                                        goto FlushTermKey;
                                        }
                                }
                        ctemp = (char) GetTerminalKey();
                        if ( ctemp == 'S')
                           {
                            SerialBufPtr = 0;
                            if(FirstSF != 0 )
                              {
                              ParserSFmt(SerialBuf,itemp);
                              }
                              FirstSF = 1;
                           }
                            SerialBuf[SerialBufPtr] = ctemp ;
                            SerialBufPtr += 1;

                        timeoutcount=200000;
                        if ((iptr%1000)== 0 )
                           { PrintSCI("transfer %d bytes \n",iptr  );}
                        }
                  PrintSCI("Finish Receive.\n");
                  goto FlushTermKey;
          default:
                PrintSCI("***** Invalid command *****\n");
                goto FlushTermKey;
          } /* end of switch 's' */


/* Save data to Storage(Flash HN29W6411 now */
     case 'W' :
     case 'w' :

          iptr = Termkeybuf[1];
          switch (iptr)
          {
           case 'F':
           case 'f':

                itemp = GetTermBufAddr(1);    /* Get start Address */
                ltemp = GetTermBufNext();     /* Get Length */
                ctemp = (char)GetTermBufNext();
/*              StoreDataToFlash(itemp,ltemp);          */
                PrintSCI("Store finish.\n");
                goto FlushTermKey;
          default:
                PrintSCI("***** Invalid command *****\n");
                goto FlushTermKey;
          } /* end of switch 's' */

/* Go to absolute Address            */
     case 'G' :
     case 'g' :

/* Get destination address */
                   itemp = GetTermBufAddr(2);    /* Get start Address */
                   if( (itemp & 0x00000003) != 0x00000000)
                     {
                      PrintSCI("\nInvalid Address.\n");
                      goto FlushTermKey;
                     }
                  PrintSCI("JMP to address H'%x.\n",itemp);

    //  			GoAbsAddress(itemp);

                  goto FlushTermKey;
     case 'V' :
     case 'v' :
          itemp = GetTermBufAddr(1);    /* Get start Address */
          ltemp = GetTermBufNext();     /* Get Length */

          TestMemory(itemp,ltemp);

          PrintSCI("\n");
          goto FlushTermKey;

     case 'L' :
     case 'l' :

             for(itemp=0;itemp<1000;itemp++)
             {
             sptr = 0;
             iptr=GetTerminalKey();
             if(iptr =='S')
               {
                PrintSCI(".");
                sptr++;
                if ((sptr% 60 ) == 0)
                   {
                    PrintSCI("\n");
                   }
               }
              }
          PrintSCI("\n");
          goto FlushTermKey;

     case 'Q' :
     case 'q' :

          PrintSCI("\n");
          return;

     default :

                PrintSCI("***** Invalid command *****\n");
                goto FlushTermKey;

     } /* end switch(GetTerminalKey) */

   }/* if enter key */
   goto RepeatTermKey;
   return;
}       /* term */

int GetTermBufAddr(int startptr)
{
	
        unsigned int itemp=0;
        unsigned char j;
                                        /* Get start Address */
        bufptr = startptr;

        while( Termkeybuf[bufptr] == 0x00000020 )
          {
           bufptr++;

           if (bufptr == 0x31)
              {
               return 0x00000000;
              }
          }
          for (j=0;j<10 ;j++)
          {

             if (Termkeybuf[bufptr] == 0x00000020 || Termkeybuf[bufptr] == 0x0000000d)
                {
                 return itemp;
                }
             itemp = itemp << 4 ;
             itemp = itemp + (unsigned int) RETWatohex( Termkeybuf[bufptr] );
             bufptr++;
          }
          return 0x00;
}

int GetTermBufNext()
{
        unsigned int itemp=0;
        unsigned char j;
                                        /* Get start Address */
/*      bufptr = startptr;                                   */
        while( Termkeybuf[bufptr] == 0x00000020 )
          {
           bufptr++;
           if (bufptr == 0x31)
              {
               return 0x00000000;
              }
          }
          for (j=0;j<10 ;j++)
          {
/*     PrintSCI("bufptr=%x,Termkey=%x\n",bufptr,Termkeybuf[bufptr]);      */
             if (Termkeybuf[bufptr] == 0x0000000d)
                {
                 return itemp;
                }
             if (Termkeybuf[bufptr] == 0x00000020)
                {
                 return itemp;
                }
             itemp = itemp << 4 ;
             itemp = itemp + (unsigned int) RETWatohex( Termkeybuf[bufptr] );
             bufptr++;
          }
          return 0x00;
}

unsigned int RETWatohex(unsigned char x)
{
        if( x < 'G')
        {
        return x > '9' ? x-'A'+10 : x-'0';
        }
        if(( x < 'g') && (x > 0x60))
        {
        return x-'a'+10 ;
        }

/*      return x > '9' ? x-'A'+10 : x-'0';  */

}
unsigned int hex(unsigned char x)
{
   if (x > '9') return x -'A'+10;
   return x-'0';
}

/* This routine extract SerialBuf[] content to physical memory */
ParserSFmt(unsigned char *bp,unsigned int offsetaddr)
{
        unsigned int startaddr;
        unsigned int i;
        unsigned int length;

/*      PrintSCI("bufdata=%x\n",*bp);     */

        if ( *bp != 'S')
        {
        PrintSCI("The file is not S Format.\n");
        return;
        }

        bp++;

        switch (*bp)
            {
            case '0':
                    return;
                    break;
            case '1':
                  bp++;
                  length = ChkLengthCheckSum(bp);
                  bp+=2;
                  startaddr = (hex(*bp) << 12)
                            + (hex(*(bp+1)) << 8)
                            + (hex(*(bp+2)) << 4)
                            + (hex(*(bp+3)) );
                  bp+=4;
                  i = 2 ;
                  break;
            case '2':
                  bp++;
                  length = ChkLengthCheckSum(bp);
                  bp+=2;
                  startaddr = (hex(*bp) << 20)
                            + (hex(*(bp+1)) << 16)
                            + (hex(*(bp+2)) << 12)
                            + (hex(*(bp+3)) << 8)
                            + (hex(*(bp+4)) << 4)
                            + (hex(*(bp+5)) );
                  bp+=6;
                  i = 3 ;
                  break;
            case '3':
                  bp++;
                  length = ChkLengthCheckSum(bp);
                  bp+=2;
                  startaddr = (hex(*bp) << 28)
                            + (hex(*(bp+1)) << 24)
                            + (hex(*(bp+2)) << 20)
                            + (hex(*(bp+3)) << 16)
                            + (hex(*(bp+4)) << 12)
                            + (hex(*(bp+5)) << 8)
                            + (hex(*(bp+6)) << 4)
                            + (hex(*(bp+7)) );
                  bp+=8;
                  i = 4 ;
                  break;
            case '9':
                  PrintSCI("File receive End.\n");
                  return;
            default:
                  PrintSCI("Unknown Format.\n");
                  return;
            }

        while (i < (length - 1) )
             {
       *(vchar *)(startaddr + offsetaddr) = (hex(*bp) <<4) + hex(*(bp+1));
/*
 PrintSCI("startaddr=%x,data=%x.\n",startaddr, (hex(*bp) <<4) + hex(*(bp+1)) );
 */
              startaddr += 1;
              bp +=2;
              i++;
             }
        return;
}

int ChkLengthCheckSum(unsigned char *bp)
{
        unsigned int length;
        unsigned int checksum;
        int i;
        length = (hex(*bp) << 4) + hex(*(bp+1));
        checksum = length;
        for (i=1; i<=length ;i++)
        {
           checksum += (hex(*(bp+2*i)) << 4) + hex(*(bp+2*i+1));
        }
        if (~checksum & 0x00ff != (hex(*(bp+2*i)) << 4) + hex(*(bp+2*i+1)))
        {
           PrintSCI("CheckSum Error!\n");
        }
        return length;
}

DumpData(int addr, int length)
{
		int Newaddr = addr&0xFFFFFFF0;
		int i,j,k;
//	PrintSCI("Display Data from 0x%x=\n",addr);
		if( (addr&0x0000000f) ==0 )
			{
				for(j=0;j<((length/16)+1);j++)
				{	
					PrintSCI("H%x ",addr+j*16);
					for(k=0;k<16;k++)
					{		
					  dspbyte(*(unsigned char*)(addr+k+j*16));
						if(k==7)
						PrintSCI(" - ");
						else   
						PrintSCI(" ");
					}	
					PrintSCI("-"); 
 					for(k=0;k<16;k++)
 					{		
 						if(*(unsigned char*)(addr+k+j*16)>0x20 && *(unsigned char*)(addr+k+j*16)<0x80 )
 						PutChar(*(unsigned char*)(addr+k+j*16));
 						else
 						PutChar(0x20);
 					}	
 					PrintSCI("\n");
				}  
			}
		else
			{
				for(j=0;j<((length/16)+2);j++)
				{	
					PrintSCI("H%x ",(addr&0xfffffff0)+j*16);
					for(k=0;k<16;k++)
					{	
						if(( ( Newaddr+(k+j*16)) < addr)||((addr+length) <=(Newaddr+k+j*16))  )
								PrintSCI("  ");
						else 
					  dspbyte(*(unsigned char*)(Newaddr+k+j*16));
					  
						if(k==7)
						PrintSCI(" - ");
						else   
						PrintSCI(" ");
					}	
  			
					PrintSCI("-"); 
 					for(k=0;k<16;k++)
 					{		
 						if(( ( Newaddr+(k+j*16)) < addr)||((addr+length)  <= (Newaddr+k+j*16))  )
							 PutChar(0x20);
						else
						{
 								if(*(unsigned char*)(Newaddr+k+j*16)>0x20 && *(unsigned char*)(Newaddr+k+j*16)<0x80 )
 								PutChar(*(unsigned char*)(Newaddr+k+j*16));
 								else
 								PutChar(0x20);
 							}	
 					}
 						PrintSCI("\n");
 
				}  				
				if((addr+length)  <= (Newaddr+k+j*16)) return;
				
			}
 
}

DumpDataByte(int addr, int length)
{
		int Newaddr = addr&0xFFFFFFF0;
		int i,j,k;
		 
				for(j=0;;j++)
				{	
					PrintSCI("H%x ",(addr&0xfffffff0)+j*16);
					for(k=0;k<16;k++)
					{	
						if(( ( Newaddr+(k+j*16)) < addr)||((addr+length) <=(Newaddr+k+j*16))  )
								PrintSCI("  ");
						else 
					  dspbyte(*(unsigned char*)(Newaddr+k+j*16));
					  
						if(k==7)
						PrintSCI(" - ");
						else   
						PrintSCI(" ");
					}	
					PrintSCI("-"); 
 					for(k=0;k<16;k++)
 					{		
 						if(( ( Newaddr+(k+j*16)) < addr)||((addr+length)  <= (Newaddr+k+j*16))  )
							 PutChar(0x20);
						else
						{
 								if(*(unsigned char*)(Newaddr+k+j*16)>0x20 && *(unsigned char*)(Newaddr+k+j*16)<0x80 )
 								PutChar(*(unsigned char*)(Newaddr+k+j*16));
 								else
 								PutChar(0x20);
 							}	
 					}
 						PrintSCI("\n");
				if((addr+length)  <= (Newaddr+k+j*16)) return; 
				}  				
}


int TestMemory(int startaddr, int size)
{
	int errormark=0;
        unsigned int iptr;
        unsigned int result;
        PrintSCI("Testing from %x ,size=%x ",startaddr ,size);
        if (size == 0 )
        {
         PrintSCI("-->No memory at this area.\n");
         return;
        }
/************************************************************************/
        PrintSCI("\n");
        PrintSCI("Testing RAM by address pattern.....\n");
        PrintSCI("Fill RAM by @address=address now.....\n");
        for (iptr=0 ; iptr < size/4 ; iptr++)
        {
            (*(vlong *)( startaddr + iptr*4)) =iptr;

            if ( (iptr & 0x00003fff) == 0x00003fff )
                {
//            PrintSCI("Fill address pattern till %x.\n",startaddr+(iptr+1)*4 );
              }
        }
        PrintSCI("Check if RAM @address=address ?.....\n");
        for (iptr=0 ; iptr < size/4 ; iptr++)
        {


            result = (*(vlong *)( startaddr + iptr*4));
            if  (result != iptr)
                {
PrintSCI("--->Error at %x,Wr=%x,Rd=%x.\n", startaddr+iptr*4 ,iptr,result);
                errormark=1;
                }
            if ( (iptr & 0x00003fff) == 0x00003fff )
                {
//PrintSCI("--->Test address pattern till %x.\n",startaddr+(iptr+1)*4 );
                }
        }
PrintSCI("RAM tested by address pattern was finished.\n");
/************************************************************************/
/* Testing RAM by pattern... */
        PrintSCI("Testing RAM by fixed pattern.....\n");
        for (iptr=0 ; iptr < size/4 ; iptr++)
        {

        (*(vlong *)( startaddr + iptr*4)) = 0x0055aaff;
        result= (*(vlong *)( startaddr + iptr*4));
            if  (result != 0x0055aaff)
                {
PrintSCI("--->Error at %x, Wr=0x0055AAFF , Rd=%x.\n",startaddr+iptr*4,result);
                errormark=1;

                }
        (*(vlong *)( startaddr + iptr*4)) = 0xffaa5500;
        result= (*(vlong *)( startaddr + iptr*4));
            if  (result != 0xffaa5500)
                {
PrintSCI("--->Error at %x, Wr=0xFFAA5500 , Rd=%x.\n",startaddr+iptr*4,result);
                errormark=1;

                }
            if ( (iptr & 0x00003fff) == 0x00003fff )
                {
//PrintSCI("--->Test data pattern till %x.\n",startaddr +(iptr+1)*4 );
                }
        }

        if(errormark==1)
	{PrintSCI("*****RAM test has errors!\n");
                return 0;}
        else
        {
PrintSCI("\n");
PrintSCI("*****RAM test Pass!\n");
	return 1;
        }
}       /* TestMemory() */

#if 1

int CompareData(int Dest,int Src,int size)
{
	char data1,data2;
	int i;
	for(i=0;i<size;i++)
	{
	data1=*(vchar*)(Src+i);
	data2=*(vchar*)(Dest+i);
	if(data1!=data2)
		{PrintSCI("Compare fail at %x,Correct is %x, Read is %x\n",Dest+i,data1,data2);
		return 0;}
	}
	return 1;	
}

int ReceivingMOTcode(int offset)
{
	int timeoutcount;
	int FirstSF;
	int il;
	char ctemp;
	int SerialBufPtr;
	
        while( ScanTerminalKeyStatus() == fail )
             {}
              timeoutcount = 200000;
              FirstSF = 0;
        	for (il=0 ; il < 0x1000000;il++)
                        {
                        while( ScanTerminalKeyStatus() == fail )
                                {
                                        timeoutcount-= 1;
                                        if(timeoutcount%10000== 0)
                                        {
//                                       PrintSCI("Less time =%d\n",timeoutcount);
                                        }
                                        if(timeoutcount == 0      )
                                        {
//					PrintSCI("Error-TimeOut!\n");
 					PrintSCI("Transfer %d(H'%x) bytes.\n",il-1,il-1 );
                                        return il;
                                        }
                                }
                        ctemp = (char) GetTerminalKey();
                        if ( ctemp == 'S')
                           {
                            SerialBufPtr = 0;
                            if(FirstSF != 0 )		//skip first line
                              {
 										ParserSFmt(SerialBuf,offset);
                              }
                              FirstSF = 1;
                           }
                            SerialBuf[SerialBufPtr] = ctemp ;
                            SerialBufPtr += 1;

                        timeoutcount=200000;
                        
                        if ((il%1000)== 0 )
                           { PrintSCI("Transfer %d bytes \n",il );}
                        }
}
#endif

void PrintSCI(char *txdptr, int para1, int para2,int para3,int para4)
{
	static int txtemp;
	static int paracnt;
	paracnt= 0 ;
   for(;;)
   {
	txtemp = *(txdptr);
	switch (txtemp & 0x000000ff)
	{
		case 0x00000000	:
				return;
		case 0x0000000a	:
			   PutChar(0x0000000d);
			   PutChar(0x0000000a);
			   txdptr++;
			   break;
		case 0x00000025	:     /* character "%" */
			   txdptr++;
			   txtemp=*txdptr;
			   switch(txtemp & 0x000000ff)
			   {
			    case 's':
				txdptr = (char *)para1;
				PutChar(	*txdptr	);
				break;

			    case 0x00000064:	/* character "d" */
				      switch(paracnt)
				      {
					case 0x00000000:
					     dspdec(para1);
					     paracnt++;
					     break;
					case 0x00000001:
					     dspdec(para2);
					     paracnt++;
					     break;
					case 0x00000002:
					     dspdec(para3);
					     paracnt++;
					     break;
					case 0x00000003:
					     dspdec(para4);
					     paracnt++;
					     break;
				      }
				      break;
			    case 0x00000078:	/* character "x" */
				      switch(paracnt)
				      {
					case 0x00000000:
					     dspreg(para1);
					     paracnt++;
					     break;
					case 0x00000001:
					     dspreg(para2);
					     paracnt++;
					     break;
					case 0x00000002:
					     dspreg(para3);
					     paracnt++;
					     break;
					case 0x00000003:
					     dspreg(para4);
					     paracnt++;
					     break;
				      }
				      break;
			   }
			   txdptr++;
			   break;
		default:
			   PutChar(txtemp);
			   txdptr++;
			   break;
	}
   }
}

int dspreg(int P)
{
        PutChar(HexDec(P >> 28));
        PutChar(HexDec(P >> 24));
        PutChar(HexDec(P >> 20));
        PutChar(HexDec(P >> 16));
        PutChar(HexDec(P >> 12));
        PutChar(HexDec(P >> 8));
        PutChar(HexDec(P >> 4));
        PutChar(HexDec(P));
        return;
}

void dspbyte(unsigned char P)
{
        PutChar(HexDec(P >> 4));
        PutChar(HexDec(P));
}
int dspword(unsigned short P)
{
        PutChar(HexDec(P >> 12));
        PutChar(HexDec(P >> 8));
        PutChar(HexDec(P >> 4));
        PutChar(HexDec(P));
        return;
}

int HexDec(int P)
{
        P &= 0x0000000f ;
        if(P > 0x00000009)
        {
                P += 0x07;
        }
        P += 0x30;
        return P;
}
int dspdec(int P)
{
        static int result;
        zerof = 0;

        putdec(P/1000000000);
        result = P % 1000000000 ;

        putdec(result/100000000);
        result = result % 100000000 ;

        putdec(result/10000000);
        result = result % 10000000 ;

        putdec(result/1000000);
        result = result % 1000000 ;

        putdec(result/100000);
        result = result % 100000 ;

        putdec(result/10000);
        result = result % 10000 ;

        putdec(result/1000);
        result = result % 1000 ;

        putdec(result/100);
        result = result % 100 ;

        putdec(result/10);
        result = result % 10 ;

        zerof = 1;
        putdec(result/1);

        return;
}
int putdec(int P)
{
        P += 0x30;
        if ( ( (P & 0x000000ff) != 0x30))
        {
        zerof = 1;
        }
        if (zerof == 1)
        {
        PutChar(P);
        }
}
int WaitTerminalKey()
{
        while( ScanTerminalKeyStatus() == fail)
        {}
        GetTerminalKey();
        return;
}