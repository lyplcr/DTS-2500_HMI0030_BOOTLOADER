/****************************************Copyright (c)**************************************************
**                               	杭州鑫高科技有限公司
**                                   
**                                 
** 文   件   名	: 	uart6.c
** 最后修改日期	: 	2012-8-18  
** 描        述	: 	串口
** 版	     本	: 	V1.0
** 主  控  芯 片	: 	STM32F407VET6		晶振频率: 25MHz
** IDE			:	MDK 4.70 
**********************************************************************************************************/
#include "uart6.h"
#include "protocol.h"
#include "command.h"
#include <stdio.h>
#include "crc16.h" 
 
 

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//									串口1驱动函数
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 
#define			UART6_RX_BUF_SIZE				400						//接收缓冲尺寸			
#define			UART6_TX_BUF_SIZE				400						//发送缓冲尺寸	
#define			uart6_RX_FRAME_SIZE				100						//接收帧尺寸			
__IO static uint8_t 	uart6_tx_buf[UART6_TX_BUF_SIZE];				//发送缓冲
__IO static uint16_t 	uart6_tx_wr_ptr,uart6_tx_rd_ptr;				//发送读写指针	
__IO static uint16_t 	uart6_tx_cnt;
																		//发送接收计数器	 
__IO static uint8_t 	uart6_rx_buf[UART6_RX_BUF_SIZE];				//接收缓冲  								//接收计数器
__IO static uint16_t	uart6_rx_cnt;	
__IO static uint8_t	uart6_end_flag=0;									//消息体结束标志，表示成功接收一个数据包
__IO static uint16_t	uart6_rx_wr_index,uart6_rx_rd_index;			//接收读写指针	
__IO static uint8_t 	uart6_rx_frame[uart6_RX_FRAME_SIZE];			//接收帧数据 
  
	
//重定向
int fputc(int ch, FILE *f)
{ 	
	uart6_send_char((uint8_t)ch);    
	return ch;
}
 
/*************************************************************************
 * Function Name: void uart6_init(uint32_t baud)
 * Description: uart1初始化函数
 *************************************************************************/
void uart6_init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;  
	NVIC_InitTypeDef NVIC_InitStructure;
	  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART6时钟
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOC6复用为USART6
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOC7复用为USART6

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIOC6与GPIOC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PC6，PC7
	
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	USART_Init(USART6, &USART_InitStructure);  
	  
 	USART_Cmd(USART6, ENABLE); 
 	USART_ClearFlag(USART6,USART_FLAG_TC);  
	while(USART_GetFlagStatus(USART6,USART_FLAG_TC)==RESET)
	{
		;
	}
	USART_ClearFlag(USART6,USART_FLAG_TC);  
	USART_ITConfig(USART6, USART_IT_TC, ENABLE); 			//串口发送完成中断
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); 			//串口接收中断 
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	uart6_tx_cnt=0;
	uart6_tx_wr_ptr=0;
	uart6_tx_rd_ptr=0;	
	
	uart6_rx_wr_index=0;
	uart6_rx_rd_index=0; 		
	uart6_rx_cnt=0; 
}


/**********************************************************************
functionName:uint8_t uart6_send_char(uint8_t uart_data)
description:串口1发送一个字节的数据
**********************************************************************/ 
ErrorStatus uart6_send_char(uint8_t uart_data)
{ 		
	USART_ITConfig(USART6, USART_IT_TC,DISABLE);		//禁止串口发送结束中断
	if(uart6_tx_cnt==UART6_TX_BUF_SIZE)
	{ 	 
		USART_ITConfig(USART6, USART_IT_TC, ENABLE); 	//使能串口发送完成中断
		return(ERROR);						 			//表示串口缓冲器溢出
	}
	uart6_tx_buf[uart6_tx_wr_ptr]=uart_data;
	uart6_tx_wr_ptr++;
	if(uart6_tx_wr_ptr>=UART6_TX_BUF_SIZE)uart6_tx_wr_ptr=0;
	uart6_tx_cnt++;
	if(uart6_tx_cnt==1)
	{   
		if(!uart6_tx_wr_ptr)
		uart6_tx_rd_ptr=UART6_TX_BUF_SIZE-1;	
		else
		uart6_tx_rd_ptr=uart6_tx_wr_ptr-1;  
		
		USART_SendData(USART6,uart6_tx_buf[uart6_tx_rd_ptr]);
		uart6_tx_rd_ptr= uart6_tx_wr_ptr;
	} 		  			 
	USART_ITConfig(USART6, USART_IT_TC, ENABLE); 		//使能串口发送完成中断
	return(SUCCESS);
}

/**********************************************************************
functionName:void uart6_send_chars(uint8_t *sd,uint8_t len)
description:固定长度的数据流发送
**********************************************************************/
void uart6_send_chars(uint8_t *sd,uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		uart6_send_char(*sd);
		sd++;	
	}
} 


/**********************************************************************
functionName:uint8_t uart6_get_status(void)
description:查询串口接收状态,执行函数后，uart6_end_flag自动清零
**********************************************************************/       
uint8_t uart6_get_status(void)
{
	uint8_t temp08u;
	temp08u=uart6_end_flag;
	uart6_end_flag=0;
	return temp08u;
}

/**********************************************************************
functionName:uint8_t *uart6_get_dat(void)
description:返回串口数据的指针
**********************************************************************/ 
uint8_t *uart6_get_dat(void)
{
	UART_HEAD_Typdef *p_uart_head=(UART_HEAD_Typdef *)(void *)uart6_rx_frame;
	return &(p_uart_head->data);
} 

/**********************************************************************
functionName:uint8_t get_msg3_index(void)
description:返回串口1数据的INDEX
**********************************************************************/  
uint8_t uart6_get_index(void)
{
	UART_HEAD_Typdef *p_uart_head=(UART_HEAD_Typdef *)(void *)uart6_rx_frame;
	return p_uart_head->index; 
}

/**********************************************************************
functionName:uint8_t get_msg3_size(void)
description:返回串口数据的MSG_SIZE
**********************************************************************/ 
uint8_t uart6_get_size(void)
{
	UART_HEAD_Typdef *p_uart_head=(UART_HEAD_Typdef *)(void *)uart6_rx_frame;
	return p_uart_head->size;
}

/**********************************************************************
functionName:uint8_t uart6_get_addr(void)
description:返回串口数据的ADDR
**********************************************************************/ 
uint8_t uart6_get_addr(void)
{
	UART_HEAD_Typdef *p_uart_head=(UART_HEAD_Typdef *)(void *)uart6_rx_frame;
	return p_uart_head->addr;
} 


/**********************************************************************
functionName:uint16_t uart6_get_cmd(void)
description:返回功能码
**********************************************************************/ 
uint16_t uart6_get_cmd(void)
{
	uint16_t *cmd;
	UART_HEAD_Typdef *p_uart_head=(UART_HEAD_Typdef *)(void *)uart6_rx_frame;
	cmd=(uint16_t *)(void *)(&(p_uart_head->data)); 
	return (*cmd);
} 

/**********************************************************************
functionName:ErrorStatus uart6_check(void)
description:校验串口1包是否合法
**********************************************************************/ 
ErrorStatus uart6_check(void)
{
 	uint16_t *p_check;
 	uint16_t check_cal;
	uint8_t msg_size;
	msg_size=uart6_get_size();
	p_check=(uint16_t *)(uart6_rx_frame+sizeof(UART_HEAD_Typdef)-1+msg_size);
	check_cal=crc16_get(0,(uint8_t *)uart6_rx_frame,sizeof(UART_HEAD_Typdef)-1+msg_size);
	if(check_cal==*p_check)
	return 	SUCCESS;
	else
	return 	ERROR;	
}

/**********************************************************************
functionName:void uart6_send_pkg(uint8_t addr,uint8_t index,uint8_t msg_size,uint8_t *ptr)
description:手动盒处理函数
**********************************************************************/ 
void uart6_send_pkg(uint8_t addr,uint8_t index,uint8_t msg_size,uint8_t *ptr)
{
	uint8_t head[5];
	uint16_t crc16=0;
	//发送头5个字节
	head[0]=MESSAGE_START;
	head[1]=addr;								//地址固定为0x01
	head[2]=index;
	head[3]=msg_size;
	head[4]=TOKEN;  
	uart6_send_chars(head,sizeof(head));
	uart6_send_chars(ptr,msg_size);
	crc16=crc16_get(0,head,sizeof(head));
	crc16=crc16_get(crc16,ptr,msg_size);		//计算CRC16值
	uart6_send_chars((uint8_t *)&crc16,2);
	uart6_send_char(MESSAGE_END);
} 


/**********************************************************************
functionName:void USART6_IRQHandler(void)
description:串口1中断服务
uart6_rx_wr_index=0;
uart6_rx_rd_index=0; 		
uart6_rx_cnt=0;
**********************************************************************/ 
void USART6_IRQHandler(void)
{
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)			//串口接收中断
	{
		uart6_rx_buf[uart6_rx_wr_index]=USART_ReceiveData(USART6);  
		uart6_rx_wr_index++;
		if(uart6_rx_wr_index>=UART6_RX_BUF_SIZE)
		uart6_rx_wr_index=0;   
	}	
	else if(USART_GetITStatus(USART6, USART_IT_TC) != RESET)		//发送完成中断
	{   	
		uart6_tx_cnt--;
		if(uart6_tx_cnt)											//如果还没有发送完成
		{  
			USART_SendData(USART6,uart6_tx_buf[uart6_tx_rd_ptr]);
			uart6_tx_rd_ptr++;
			if(uart6_tx_rd_ptr>=UART6_TX_BUF_SIZE)
			uart6_tx_rd_ptr=0;
		} 
		else
		{
			uart6_tx_rd_ptr=uart6_tx_wr_ptr;		//冗余处理
			USART_ClearFlag(USART6,USART_FLAG_TC);
		}	
	} 
}


/**********************************************************************
functionName:uint8_t uart6_rx_scan(void)
description:串口1接收数据帧处理
返回：如果接收到数据帧，返回1，否者返回0
**********************************************************************/ 
uint8_t uart6_rx_scan(void)
{
	uint16_t tempu16;
	static uint16_t uart6_rx_cnt=0;					
	static uint8_t msg_size;										//消息尺寸 			
	static uint8_t rx_stu_mac=ST_START;  
	 			 
	USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);		//禁止串口接收中断
	tempu16=uart6_rx_wr_index;
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);		//使能串口接收中断	 

	while(tempu16!=uart6_rx_rd_index)
	{	 
		switch(rx_stu_mac)							 
	 	{ 
	 		case ST_START:
	 		{
	 			if(uart6_rx_buf[uart6_rx_rd_index]==MESSAGE_START)
	 			rx_stu_mac = ST_GET_ADDR;
	 			uart6_rx_frame[0] = MESSAGE_START; 
	 			uart6_rx_cnt=1; 
	 			break;
	 		}	
	 		case ST_GET_ADDR:
	 		{  
	 			uart6_rx_frame[uart6_rx_cnt++]=uart6_rx_buf[uart6_rx_rd_index];
				rx_stu_mac = ST_GET_INDEX;
	 			break;
	 		}
	 		case ST_GET_INDEX:
	 		{		
	 			uart6_rx_frame[uart6_rx_cnt++]=uart6_rx_buf[uart6_rx_rd_index];
	 			rx_stu_mac=ST_MSG_SIZE; 
	 			break;
	 		}
	 		case ST_MSG_SIZE:
	 		{
	 			msg_size=uart6_rx_buf[uart6_rx_rd_index];
	 			uart6_rx_frame[uart6_rx_cnt++]=msg_size; 
	 			rx_stu_mac=ST_GET_TOKEN;
	 			break;
	 		}
	 		case ST_GET_TOKEN:
	 		{
	 			if(uart6_rx_buf[uart6_rx_rd_index]==TOKEN)
	 			{ 
	 				uart6_rx_frame[uart6_rx_cnt++]=TOKEN; 
	 				if(msg_size)
	 				rx_stu_mac = ST_GET_DATA;
	 				else
	 				rx_stu_mac=	ST_GET_CHECK_L;
	 			}	
	 			else
	 			rx_stu_mac = ST_START;
	 			break;
	 		}
	 		case ST_GET_DATA:
	 		{
	 			uart6_rx_frame[uart6_rx_cnt++]=uart6_rx_buf[uart6_rx_rd_index];
	 			msg_size--;
	 			if(!msg_size)	
	 			rx_stu_mac = ST_GET_CHECK_L;
	 			if((uart6_rx_cnt+5)>uart6_RX_FRAME_SIZE)		
	 			rx_stu_mac=ST_START;
	 			break;
	 		}
	 		case ST_GET_CHECK_L:
	 		{ 
	 			uart6_rx_frame[uart6_rx_cnt++]=uart6_rx_buf[uart6_rx_rd_index];		
				rx_stu_mac = ST_GET_CHECK_H; 
	 			break;
	 		}
	 		case ST_GET_CHECK_H:
	 		{
	 			uart6_rx_frame[uart6_rx_cnt++]=uart6_rx_buf[uart6_rx_rd_index];
				rx_stu_mac = ST_GET_END;
				break; 
	 		}
	 		case ST_GET_END:
			{
				uart6_rx_frame[uart6_rx_cnt++]=uart6_rx_buf[uart6_rx_rd_index];
				rx_stu_mac=ST_START;
				if(uart6_rx_buf[uart6_rx_rd_index]==MESSAGE_END)
				{   
					uart6_rx_rd_index++;
				 	if(uart6_rx_rd_index>=UART6_RX_BUF_SIZE)
				 	uart6_rx_rd_index=0;	 
					return 1;	
				}
				break; 	  
			}
	 		default:
	 		{
	 			rx_stu_mac=ST_START;
	 			break;
	 		}
	 	} 
	 	uart6_rx_rd_index++;
	 	if(uart6_rx_rd_index>=UART6_RX_BUF_SIZE)
	 	uart6_rx_rd_index=0;	 
	 	USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);					//禁止串口接收中断
		tempu16=uart6_rx_wr_index;
		USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);					//使能串口接收中断	  
	} 
	return 0;
}   
    
/******************* (C) COPYRIGHT 2012 XinGao Tech 			 *****END OF FILE****/  
