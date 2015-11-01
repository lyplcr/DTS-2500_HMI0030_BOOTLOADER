/**
  ******************************************************************************
  * @file    tftpserver.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011   
  * @brief   basic tftp server implementation for IAP (only Write Req supported)
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */

/* Includes ------------------------------------------------------------------*/
#include "tftpserver.h"
#include "flash_if.h"
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "lcd.h"
#include "config.h"
#include "ustdlib.h"
#include "timer.h"

#define UPDATE_FONT				"song24.zk"

#define UPDATE_WORD_START_X		20
#define UPDATE_WORD_START_Y		20
#define UPDATE_ROW_DISTANCE		12
#define UPDATE_WORD_SIZE		24
#define DELAY_TIME				500		//提示语延时时间

#define COLOR_POINT				WHITE		//前景色
#define COLOR_BACK				BLACK		//背景色

#ifdef USE_IAP_TFTP

/* Private variables ---------------------------------------------------------*/
static uint32_t Flash_Write_Address;
static struct udp_pcb *UDPpcb;
static __IO uint32_t total_count=0;
static uint16_t udp_lcd_x = UPDATE_WORD_START_X;
static uint16_t udp_lcd_y = UPDATE_WORD_START_Y;
static __IO BoolStatus uPdateFinish = NO;

/* Private function prototypes -----------------------------------------------*/

static void IAP_wrq_recv_callback(void *_args, struct udp_pcb *upcb, struct pbuf *pkt_buf, 
                        struct ip_addr *addr, u16_t port);

static int IAP_tftp_process_write(struct udp_pcb *upcb, struct ip_addr *to, int to_port);

static void IAP_tftp_recv_callback(void *arg, struct udp_pcb *Upcb, struct pbuf *pkt_buf,
                        struct ip_addr *addr, u16_t port);

static void IAP_tftp_cleanup_wr(struct udp_pcb *upcb, tftp_connection_args *args);
static tftp_opcode IAP_tftp_decode_op(char *buf);
static u16_t IAP_tftp_extract_block(char *buf);
static void IAP_tftp_set_opcode(char *buffer, tftp_opcode opcode);
static void IAP_tftp_set_block(char* packet, u16_t block);
static err_t IAP_tftp_send_ack_packet(struct udp_pcb *upcb, struct ip_addr *to, int to_port, int block);
void UDP_AddUpdateError( void );


/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : UDP_Update
 * Description    : UDP升级
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus UDP_Update( void )
{	
	char version[50];
	
	lcd_clear(COLOR_BACK);
	
	lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"========================================",UPDATE_FONT);
	udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"         > 欢迎使用以太网升级系统 <",UPDATE_FONT);
	udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	strcpy(version,"         > ");
	strcat(version,SOFT_DISP_VERSION);
	lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,version,UPDATE_FONT);
	udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"========================================",UPDATE_FONT);
	udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 正在尝试连接电脑...",UPDATE_FONT);
	udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	
	#ifdef USE_IAP_TFTP    
    /* Initialize the TFTP server */
    IAP_tftpd_init();
	#endif 
	
	while (1)
	{
		if (uPdateFinish == YES)
		{
			lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 升级成功！",UPDATE_FONT);
			udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	
			AutoEnterSystemCue(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,UPDATE_FONT);
			break;
		}
	}
	
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : UDP_AddUpdateError
 * Description    : UDP升级失败
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void UDP_AddUpdateError( void )
{
	udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 升级失败！",UPDATE_FONT);
	while (1);
}

/*------------------------------------------------------------
 * Function Name  : UDP_EraseFlashCue
 * Description    : UDP升级失败
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void UDP_EraseFlashCue( uint8_t process )
{
	char buff[10];
	
	switch ( process )
	{
		case 0: 
			lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 准备系统升级...",UPDATE_FONT);
			udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
			lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 正在擦除flash...",UPDATE_FONT);
			break;
		case 100:
			delay_ms(DELAY_TIME);
			lcd_font24(udp_lcd_x+216,udp_lcd_y,COLOR_POINT,COLOR_BACK,"100%",UPDATE_FONT);
			udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
			break;
		default:
			usprintf(buff,"%3d%%",process);
			lcd_font24(udp_lcd_x+216,udp_lcd_y,COLOR_POINT,COLOR_BACK,buff,UPDATE_FONT);
			break;
	}

}


/**
  * @brief Returns the TFTP opcode 
  * @param buf: pointer on the TFTP packet 
  * @retval none
  */
static tftp_opcode IAP_tftp_decode_op(char *buf)
{
  return (tftp_opcode)(buf[1]);
}

/**
  * @brief  Extracts the block number
  * @param  buf: pointer on the TFTP packet 
  * @retval block number
  */
static u16_t IAP_tftp_extract_block(char *buf)
{
  u16_t *b = (u16_t*)buf;
  return ntohs(b[1]);
}

/**
  * @brief Sets the TFTP opcode 
  * @param  buffer: pointer on the TFTP packet
  * @param  opcode: TFTP opcode
  * @retval none
  */
static void IAP_tftp_set_opcode(char *buffer, tftp_opcode opcode)
{
  buffer[0] = 0;
  buffer[1] = (u8_t)opcode;
}

/**
  * @brief Sets the TFTP block number 
  * @param packet: pointer on the TFTP packet 
  * @param  block: block number
  * @retval none
  */
static void IAP_tftp_set_block(char* packet, u16_t block)
{
  u16_t *p = (u16_t *)packet;
  p[1] = htons(block);
}

/**
  * @brief Sends TFTP ACK packet  
  * @param upcb: pointer on udp_pcb structure
  * @param to: pointer on the receive IP address structure
  * @param to_port: receive port number
  * @param block: block number
  * @retval: err_t: error code 
  */
static err_t IAP_tftp_send_ack_packet(struct udp_pcb *upcb, struct ip_addr *to, int to_port, int block)
{
  err_t err;
  struct pbuf *pkt_buf; /* Chain of pbuf's to be sent */

  /* create the maximum possible size packet that a TFTP ACK packet can be */
  char packet[TFTP_ACK_PKT_LEN];

  /* define the first two bytes of the packet */
  IAP_tftp_set_opcode(packet, TFTP_ACK);

  /* Specify the block number being ACK'd.
   * If we are ACK'ing a DATA pkt then the block number echoes that of the DATA pkt being ACK'd (duh)
   * If we are ACK'ing a WRQ pkt then the block number is always 0
   * RRQ packets are never sent ACK pkts by the server, instead the server sends DATA pkts to the
   * host which are, obviously, used as the "acknowledgement".  This saves from having to sEndTransferboth
   * an ACK packet and a DATA packet for RRQs - see RFC1350 for more info.  */
  IAP_tftp_set_block(packet, block);

  /* PBUF_TRANSPORT - specifies the transport layer */
  pkt_buf = pbuf_alloc(PBUF_TRANSPORT, TFTP_ACK_PKT_LEN, PBUF_POOL);

  if (!pkt_buf)      /*if the packet pbuf == NULL exit and EndTransfertransmission */
  {
	 udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	 lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 以太网升级出错了。错误代码：1",UPDATE_FONT); 
	 UDP_AddUpdateError();
// #ifdef USE_LCD
//     LCD_SetTextColor(Red);
//     LCD_DisplayStringLine(Line9, (uint8_t*)"Can not allocate pbuf");
// #endif
    return ERR_MEM;
  }

  /* Copy the original data buffer over to the packet buffer's payload */
  memcpy(pkt_buf->payload, packet, TFTP_ACK_PKT_LEN);

  /* Sending packet by UDP protocol */
  err = udp_sendto(upcb, pkt_buf, to, to_port);

  /* free the buffer pbuf */
  pbuf_free(pkt_buf);

  return err;
}

/**
  * @brief  Processes data transfers after a TFTP write request
  * @param  _args: used as pointer on TFTP connection args
  * @param  upcb: pointer on udp_pcb structure
  * @param pkt_buf: pointer on a pbuf stucture
  * @param ip_addr: pointer on the receive IP_address structure
  * @param port: receive port address
  * @retval none
  */
static void IAP_wrq_recv_callback(void *_args, struct udp_pcb *upcb, struct pbuf *pkt_buf, struct ip_addr *addr, u16_t port)
{
  tftp_connection_args *args = (tftp_connection_args *)_args;
  uint32_t data_buffer[128];
  u16 count=0;
  static uint16_t process = 0;
  static uint16_t process_x = UPDATE_WORD_START_X;
  char buff[10];
  static uint16_t process_buff = 0;

#ifdef USE_LCD
  char message[40];
#endif

  if (pkt_buf->len != pkt_buf->tot_len)
  {
	udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 以太网升级出错了。错误代码：2",UPDATE_FONT); 
	UDP_AddUpdateError();
// #ifdef USE_LCD
//     LCD_SetTextColor(Red);
//     LCD_DisplayStringLine(Line9, (uint8_t*)"Invalid data length");
// #endif
    return;
  }

  /* Does this packet have any valid data to write? */
  if ((pkt_buf->len > TFTP_DATA_PKT_HDR_LEN) &&
      (IAP_tftp_extract_block(pkt_buf->payload) == (args->block + 1)))
  {
    /* copy packet payload to data_buffer */
    pbuf_copy_partial(pkt_buf, data_buffer, pkt_buf->len - TFTP_DATA_PKT_HDR_LEN,
                      TFTP_DATA_PKT_HDR_LEN);
    
    total_count += pkt_buf->len - TFTP_DATA_PKT_HDR_LEN; 
    
    count = (pkt_buf->len - TFTP_DATA_PKT_HDR_LEN)/4;
    if (((pkt_buf->len - TFTP_DATA_PKT_HDR_LEN)%4)!=0) 
    count++;
     
    /* Write received data in Flash */
    switch ( FLASH_If_Write(&Flash_Write_Address, data_buffer ,count) )
	{
		case 0:
			process++;
			if ((process % 30) == 0)
			{
				process_buff += 5;
				lcd_font24(process_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"-",UPDATE_FONT);
				process_x += 12;
				usprintf(buff,"%3d%%",process_buff);
				lcd_font24(process_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,buff,UPDATE_FONT);
			}
			break;
		case 1:
			udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
			lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 以太网升级出错了。错误代码：3",UPDATE_FONT); 
			UDP_AddUpdateError();
		case 2:
			udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
			lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 以太网升级出错了。错误代码：4",UPDATE_FONT); 
			UDP_AddUpdateError();
	}
       
    /* update our block number to match the block number just received */
    args->block++;
    /* update total bytes  */
    (args->tot_bytes) += (pkt_buf->len - TFTP_DATA_PKT_HDR_LEN);

    /* This is a valid pkt but it has no data.  This would occur if the file being
       written is an exact multiple of 512 bytes.  In this case, the args->block
       value must still be updated, but we can skip everything else.    */
  }
  else if (IAP_tftp_extract_block(pkt_buf->payload) == (args->block + 1))
  {
    /* update our block number to match the block number just received  */
    args->block++;
  }
  
  /* Send the appropriate ACK pkt*/
  IAP_tftp_send_ack_packet(upcb, addr, port, args->block);   

  /* If the last write returned less than the maximum TFTP data pkt length,
   * then we've received the whole file and so we can quit (this is how TFTP
   * signals the EndTransferof a transfer!)
   */
  if (pkt_buf->len < TFTP_DATA_PKT_LEN_MAX)
  {
    IAP_tftp_cleanup_wr(upcb, args);
    pbuf_free(pkt_buf);
    
	lcd_font24(process_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"100%",UPDATE_FONT);
	udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	  
	uPdateFinish = YES;
// #ifdef USE_LCD
//     LCD_SetTextColor(White);
//     LCD_DisplayStringLine(Line5, (uint8_t*)"Tot bytes Received:");
//     sprintf(message, "%d bytes ",total_count);
//     LCD_DisplayStringLine(Line6, (uint8_t*)message);
//     LCD_DisplayStringLine(Line8, (uint8_t*)"State: Prog Finished ");
// #endif
  }
  else
  {
// 	  udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
// 	  lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"Reset the board",UPDATE_FONT);
// #ifdef USE_LCD
//     LCD_SetTextColor(Red);
//     LCD_DisplayStringLine(Line9, (uint8_t*)"   Reset the board  ");
// #endif
    pbuf_free(pkt_buf);
    return;
  }
}


/**
  * @brief  Processes TFTP write request
  * @param  to: pointer on the receive IP address
  * @param  to_port: receive port number
  * @retval none
  */
static int IAP_tftp_process_write(struct udp_pcb *upcb, struct ip_addr *to, int to_port)
{
  tftp_connection_args *args = NULL;
  /* This function is called from a callback,
  * therefore interrupts are disabled,
  * therefore we can use regular malloc   */
  args = mem_malloc(sizeof *args);
  if (!args)
  {
	 udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	 lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 以太网升级出错了。错误代码：5",UPDATE_FONT); 
	 UDP_AddUpdateError();
// #ifdef USE_LCD
//     LCD_SetTextColor(Red);
//     LCD_DisplayStringLine(Line9, (uint8_t*)"Memory error");
// #endif
    IAP_tftp_cleanup_wr(upcb, args);
    return 0;
  }

  args->op = TFTP_WRQ;
  args->to_ip.addr = to->addr;
  args->to_port = to_port;
  /* the block # used as a positive response to a WRQ is _always_ 0!!! (see RFC1350)  */
  args->block = 0;
  args->tot_bytes = 0;

  /* set callback for receives on this UDP PCB (Protocol Control Block) */
  udp_recv(upcb, IAP_wrq_recv_callback, args);
  
  total_count =0;
  
  /* init flash */
  FLASH_If_Init();
  
  /* erase user flash area */
  FLASH_If_Erase(USER_FLASH_FIRST_PAGE_ADDRESS);
 
  Flash_Write_Address = USER_FLASH_FIRST_PAGE_ADDRESS;    
  /* initiate the write transaction by sending the first ack */
  IAP_tftp_send_ack_packet(upcb, to, to_port, args->block);
  
  lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 正在写入文件...",UPDATE_FONT);
  udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
// #ifdef USE_LCD
//   LCD_DisplayStringLine(Line8, (uint8_t*)"State: Programming..");
// #endif
  return 0;
}


/**
  * @brief  Processes traffic received on UDP port 69
  * @param  args: pointer on tftp_connection arguments
  * @param  upcb: pointer on udp_pcb structure
  * @param  pbuf: pointer on packet buffer
  * @param  addr: pointer on the receive IP address
  * @param  port: receive port number
  * @retval none
  */
static void IAP_tftp_recv_callback(void *arg, struct udp_pcb *upcb, struct pbuf *pkt_buf,
                        struct ip_addr *addr, u16_t port)
{
  tftp_opcode op;
  struct udp_pcb *upcb_tftp_data;
  err_t err;

	#ifdef USE_LCD
	  uint32_t i;
	  char filename[40],message[40], *ptr;
	#endif

  /* create new UDP PCB structure */
  upcb_tftp_data = udp_new();
  if (!upcb_tftp_data)
  {
	  udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	  lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 以太网升级出错了。错误代码：6",UPDATE_FONT); 
	  UDP_AddUpdateError();
//     /* Error creating PCB. Out of Memory  */
// 	#ifdef USE_LCD
// 		LCD_SetTextColor(Red);
// 		LCD_DisplayStringLine(Line9, (uint8_t*)"Can not create pcb");
// 	#endif
    return;
  }

  /* bind to port 0 to receive next available free port */
  /* NOTE:  This is how TFTP works.  There is a UDP PCB for the standard port
  * 69 which al transactions begin communication on, however, _all_ subsequent
  * transactions for a given "stream" occur on another port  */
  err = udp_bind(upcb_tftp_data, IP_ADDR_ANY, 0);
  if (err != ERR_OK)
  {
	  udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	  lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 以太网升级出错了。错误代码：7",UPDATE_FONT); 
	  UDP_AddUpdateError();
//     /* Unable to bind to port */
// 	#ifdef USE_LCD
// 		LCD_SetTextColor(Red);
// 		LCD_DisplayStringLine(Line9, (uint8_t*)"Can not bind pcb");
// 	#endif
    return;
  }

  op = IAP_tftp_decode_op(pkt_buf->payload);
  if (op != TFTP_WRQ)
  {
	  udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	  lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 以太网升级出错了。错误代码：8",UPDATE_FONT); 
	  UDP_AddUpdateError();
//     /* remove PCB */
// 	#ifdef USE_LCD
// 		LCD_SetTextColor(Red);
// 		LCD_DisplayStringLine(Line9, (uint8_t*)"Bad TFTP opcode ");
// 	#endif
    udp_remove(upcb_tftp_data);
  }
  else
  {
    
// #ifdef USE_LCD
//     ptr = pkt_buf->payload;
//     ptr = ptr +2;
//     /*extract file name info */
//     i= 0;
//     while (*(ptr+i)!=0x0)
//     {
//       i++;
//     }
//     strncpy(filename, ptr, i+1);
//            
//     /* Set the LCD Text Color */
//     LCD_SetTextColor(White);
//     LCD_Clear(Black);
//     
//     LCD_DisplayStringLine(Line0, (uint8_t*)"  IAP using TFTP  ");
//     sprintf(message, "File: %s",filename);
//     LCD_DisplayStringLine(Line3, (uint8_t*)message); 
//     /* Set the LCD Text Color */
//     LCD_DisplayStringLine(Line8, (uint8_t*)"State: Erasing...");
//     
// #endif
     
    /* Start the TFTP write mode*/
    IAP_tftp_process_write(upcb_tftp_data, addr, port);
  }
  pbuf_free(pkt_buf);
}


/**
  * @brief  disconnect and close the connection 
  * @param  upcb: pointer on udp_pcb structure
  * @param  args: pointer on tftp_connection arguments
  * @retval none
  */
static void IAP_tftp_cleanup_wr(struct udp_pcb *upcb, tftp_connection_args *args)
{
  /* Free the tftp_connection_args structure */
  mem_free(args);

  /* Disconnect the udp_pcb */
  udp_disconnect(upcb);
  
  /* close the connection */
  udp_remove(upcb);
  
  /* reset the callback function */
  udp_recv(UDPpcb, IAP_tftp_recv_callback, NULL);
 
}

/* Global functions ---------------------------------------------------------*/

/**
  * @brief  Creates and initializes a UDP PCB for TFTP receive operation  
  * @param  none  
  * @retval none
  */
void IAP_tftpd_init(void)
{
  err_t err;
  unsigned port = 69; /* 69 is the port used for TFTP protocol initial transaction */

  /* create a new UDP PCB structure  */
  UDPpcb = udp_new();
  if (!UDPpcb)
  {
	udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 以太网升级出错了。错误代码：9",UPDATE_FONT); 
	UDP_AddUpdateError();
	  
    /* Error creating PCB. Out of Memory  */
// #ifdef USE_LCD
//     LCD_SetTextColor(Red);
//     LCD_DisplayStringLine(Line9, (uint8_t*)"Can not create pcb");
// #endif
    return ;
  }

  /* Bind this PCB to port 69  */
  err = udp_bind(UDPpcb, IP_ADDR_ANY, port);
  if (err == ERR_OK)
  {
    /* Initialize receive callback function  */
    udp_recv(UDPpcb, IAP_tftp_recv_callback, NULL);
  } 
  else
  {
	  udp_lcd_y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	  lcd_font24(udp_lcd_x,udp_lcd_y,COLOR_POINT,COLOR_BACK,"> 以太网升级出错了。错误代码：10",UPDATE_FONT); 
	  UDP_AddUpdateError();
// #ifdef USE_LCD
//     LCD_SetTextColor(Red);
//     LCD_DisplayStringLine(Line9, (uint8_t*)"Can not bind pcb");
// #endif
  }
}

#endif /* USE_IAP_TFTP */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
