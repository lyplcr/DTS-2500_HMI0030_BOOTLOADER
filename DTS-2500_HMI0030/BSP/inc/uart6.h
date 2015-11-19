/****************************************Copyright (c)**************************************************
**                                杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: uart.h
** 最后修改日期: 2012-8-18
** 描        述: 
** 版	     本: V1.0
** 主  控  芯 片:  晶振频率: 
** IDE: 
**********************************************************************************************************/
#ifndef __UART6_H
#define	__UART6_H

#include "stm32f4xx.h"
 
void uart6_init(uint32_t baud);
ErrorStatus uart6_send_char(uint8_t uart_data); 
uint8_t uart6_get_status(void); 
void uart6_send_pkg(uint8_t addr,uint8_t index,uint8_t msg_size,uint8_t *ptr); 
uint8_t uart6_get_index(void);
uint16_t uart6_get_cmd(void);
uint8_t *uart6_get_dat(void);
ErrorStatus uart6_check(void);
uint8_t uart6_get_addr(void);
uint8_t uart6_get_size(void);
uint8_t uart6_rx_scan(void);

  
#endif




	

	

	












