/* ============================================================== */
/* =                                                            = */
/* =    File Name :  OBD2 .c                                    = */
/* =                                                            = */
/* ============================================================== */
#include    <stdio.h>
#include    <string.h>

#include    "r_typedefs.h"
#include    "r_rscan_rz_if.h"
typedef enum {
  Query_RPM	= 0,
  Query_SPEED,                         
  Query_ENGINEtemp, 
  Query_HVBatSOC
} QUERYMODE_t;

 
unsigned int OBD_QueryCounter=0;

 
uint32_t RETW_OBD_Main()
{
    can_err_t       can_err1;
    can_txmsg_t     g_txmsg;
    can_rxmsg_t     g_rxmsg;
    can_history_t   g_entry;
    
    if(OBD_QueryCounter ==0)
    	{
				OBD_SendQuery(0x02);
				printf("OBD2 Send query\n");
			}
				OBD_QueryCounter+=1;
		if(OBD_QueryCounter>60)
				OBD_QueryCounter=0;
				
				/* TRY TO RECEIVE A MESSAGE */
				/* message must have an id of 0x60-0x6F and have at least 4 data bytes. */				
 
				if (R_CAN_GetMsg(CAN_BOX_RXMBX_4, &g_rxmsg) == CAN_SUCCESS)
				{
				    /* Setup a message to transmit */
				    g_txmsg.ide = g_rxmsg.ide;
				    g_txmsg.rtr = g_rxmsg.rtr;
				    g_txmsg.id = g_rxmsg.id;
				    g_txmsg.dlc = g_rxmsg.dlc;
				    g_txmsg.data[0] = g_rxmsg.data[0];
				    g_txmsg.data[1] = g_rxmsg.data[1];
				    g_txmsg.data[2] = g_rxmsg.data[2];
				    g_txmsg.data[3] = g_rxmsg.data[3];
				    g_txmsg.data[4] = g_rxmsg.data[4];
				    g_txmsg.data[5] = g_rxmsg.data[5];
				    g_txmsg.data[6] = g_rxmsg.data[6];
				    g_txmsg.data[7] = g_rxmsg.data[7];
				    g_txmsg.one_shot = FALSE;       // do normal retries when sending
				    g_txmsg.log_history = TRUE;     // log this message in history FIFO
				    g_txmsg.label = 0x55;           // label stored in history FIFO

				    /* SEND MESSAGE */
//				  can_err1 = R_CAN_SendMsg(CAN_BOX_CH1_TXMBX_4, &g_txmsg);
#if 0
					if (can_err1 != CAN_SUCCESS)
					{
 						printf("CAN R_CAN_SendMsg error\n");
 						while(1) ;
					}
#endif

				printf("Got OBD2 message\n");
				DumpData(&g_rxmsg.data[0],8);
 						
				return (g_rxmsg.data[2]*256 + g_rxmsg.data[1]);
				}

				/* GET HISTORY OF TRANSMITTED MESSAGES */
				/* This has no practical purpose here. It just demonstrates the mechanism. */
				can_err1 = R_CAN_GetHistoryEntry(CAN_BOX_CH1_HIST_FIFO, &g_entry);
				if (can_err1 != CAN_SUCCESS)
				{
					//while(1) ;
				}
				return 0;
}



void	OBD_SendQuery(unsigned int QueryItem)
{
    can_err_t       can_err1;
    can_txmsg_t     g_txmsg;
    
    g_txmsg.ide = 0;
    g_txmsg.rtr = 0;
    g_txmsg.id = 0x7DF;
    g_txmsg.dlc = 8;
    g_txmsg.data[0] = 2;
    g_txmsg.data[1] = 1;			//show current data
    g_txmsg.data[2] = 0x0d;
    g_txmsg.data[3] = 0x55;
    g_txmsg.data[4] = 0x55;	 
    g_txmsg.data[5] = 0x55;
    g_txmsg.data[6] = 0x55;
    g_txmsg.data[7] = 0x55;			 
 
    g_txmsg.one_shot = FALSE;       // do normal retries when sending
    g_txmsg.log_history = TRUE;     // log this message in history FIFO
    g_txmsg.label = 0x55;           // label stored in history FIFO
				    
    can_err1 = R_CAN_SendMsg(CAN_BOX_CH1_TXMBX_4, &g_txmsg);
    			    
}