/**										c
  ******************************************************************************
  * @file    client.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   A sample UDP/TCP client
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "client.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/pbuf.h"
#include "CONFIG.H"
#include <string.h>
#include <stdio.h>


/* Private typedef -----------------------------------------------------------*/
#define UDP_SERVER_PORT      7777
#define UDP_CLIENT_PORT      9999
#define TCP_PORT      4

/* Private define ------------------------------------------------------------*/
#define PACKAGE_BUFF_SIZE			1024

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO static uint8_t 	udp_rx_buf[PACKAGE_BUFF_SIZE];		//接收包数据缓存
__IO static uint8_t 	udp_tx_buf[PACKAGE_BUFF_SIZE];		//发送包数据缓存

__IO static uint16_t 	rx_pkg_len=0;						//接收包长度
__IO static BoolStatus 	HandlerRxData = YES;				//处理接收到的数据标志

static struct ip_addr DestAddr;							//目标IP地址
static unsigned short DestPort;							//目标端口

static struct udp_pcb *UdpPcb = NULL;	
static struct pbuf *pbuf_p = NULL;							//发送pbuf

struct pbuf pp;

/* Private function prototypes -----------------------------------------------*/
void udp_client_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);
err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
void tcp_client_err(void *arg, err_t err);
err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
void GetDataFromEthernet( __IO uint8_t *pBuff, struct pbuf *p);




/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize the client application.
  * @param  None
  * @retval None
  */
void client_init(void)
{
   struct pbuf *p;
                                  
   /* Create a new UDP control block  */
   UdpPcb = udp_new();   
   
   /* Connect the upcb  */
   udp_connect(UdpPcb, IP_ADDR_BROADCAST, UDP_SERVER_PORT);

   p = pbuf_alloc(PBUF_TRANSPORT, 0, PBUF_RAM);

   /* Send out an UDP datagram to inform the server that we have strated a client application */
   udp_send(UdpPcb, p);   

   /* Reset the upcb */
   udp_disconnect(UdpPcb);
   
   /* Bind the upcb to any IP address and the UDP_PORT port*/
   udp_bind(UdpPcb, IP_ADDR_ANY, UDP_CLIENT_PORT);
   
   /* Set a receive callback for the upcb */
   udp_recv(UdpPcb, udp_client_callback, NULL);

   /* Free the p buffer */
   pbuf_free(p); 

}

/**
  * @brief  This function is called when a datagram is received
   * @param arg user supplied argument (udp_pcb.recv_arg)
   * @param upcb the udp_pcb which received data
   * @param p the packet buffer that was received
   * @param addr the remote IP address from which the packet was received
   * @param port the remote port from which the packet was received
  * @retval None
  */
void udp_client_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
	DestAddr = *addr;
	DestPort = port;
	
	if (p != NULL)
	{
		if (HandlerRxData == YES)
		{
			rx_pkg_len = p->tot_len;
			GetDataFromEthernet(udp_rx_buf,p);						
			HandlerRxData = NO;
		}
		pp = *p;
//		udp_sendto(UdpPcb,p,&DestAddr,DestPort);
		
		/* Free the p buffer */
 		pbuf_free(p);				
	}
}


/*------------------------------------------------------------
 * Function Name  : GetDataFromEthernet
 * Description    : 从以太网获取收到的数据包
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GetDataFromEthernet( __IO uint8_t *pBuff, struct pbuf *p)
{
	__IO uint8_t *pdata = NULL;
	struct pbuf *pFrame = p;
	uint16_t cur_len = 0;
	uint16_t tot_len = pFrame->tot_len;
	
	if (tot_len <= PACKAGE_BUFF_SIZE)
	{
		for (pdata=pBuff; cur_len!=tot_len; pFrame=pFrame->next)
		{
			memcpy((void *)pdata,pFrame->payload,pFrame->len);
			pdata += pFrame->len;
			cur_len += pFrame->len;
		}
	}
}



/*------------------------------------------------------------
 * Function Name  : GetDataStartAddrFromEthernet
 * Description    : 从以太网获取数据包起始地址
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void * GetDataStartAddrFromEthernet( void )
{
	return ((void *)udp_rx_buf);
}

/*------------------------------------------------------------
 * Function Name  : UDP_SendStr
 * Description    : UDP发送数据包
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void UDP_SendStr(void *str_p, uint16_t len)
{ 
	pbuf_p = pbuf_alloc(PBUF_RAW,len,PBUF_RAM);
	pbuf_p->payload = (void *)str_p; 
	udp_sendto(UdpPcb,pbuf_p,&DestAddr,DestPort);     
	pbuf_free(pbuf_p); 
}


/*------------------------------------------------------------
 * Function Name  : SetRxDataHandlerStatus
 * Description    : 设置接收数据已处理状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void UDP_SetRxDataHandlerStatus( BoolStatus NewStatus )
{
	HandlerRxData = NewStatus;
}

/*------------------------------------------------------------
 * Function Name  : GetRxDataHandlerStatus
 * Description    : 获取接收数据已处理状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus UDP_GetRxDataHandlerStatus( void )
{
	return HandlerRxData;
}

/*------------------------------------------------------------
 * Function Name  : UDP_GetDataLen
 * Description    : 获取接收数据包长度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint16_t UDP_GetDataLen( void )
{
	return rx_pkg_len;
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
