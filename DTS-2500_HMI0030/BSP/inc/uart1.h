/****************************************Copyright (c)**************************************************
**                                �����θ߿Ƽ����޹�˾
**                                     
**                                
** ��   ��   ��: uart.h
** ����޸�����: 2012-8-18
** ��        ��: 
** ��	     ��: V1.0
** ��  ��  о Ƭ:  ����Ƶ��: 
** IDE: 
**********************************************************************************************************/
#ifndef __UART1_H
#define	__UART1_H

#include "stm32f4xx.h" 

 
 
void uart1_init(uint32_t baud);
ErrorStatus uart1_send_char(uint8_t uart_data); 
uint8_t uart1_get_status(void); 
void uart1_send_pkg(uint8_t addr,uint8_t index,uint8_t msg_size,uint8_t *ptr); 
uint8_t uart1_get_index(void);
uint16_t uart1_get_cmd(void);
uint8_t *uart1_get_dat(void);
ErrorStatus uart1_check(void);
uint8_t uart1_get_addr(void);
uint8_t uart1_get_size(void);
uint8_t uart1_rx_scan(void);
uint8_t *uart1_get_start_address(void);

//INDEX_TypeDef index_type_get(uint8_t index);
//uint8_t uart1_index_cycle(INDEX_TypeDef index_type);

  

#endif




	

	

	












