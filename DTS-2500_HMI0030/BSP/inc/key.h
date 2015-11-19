/****************************************Copyright (c)**************************************************
**                                杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: key.h
** 最后修改日期: 2015-6-5 8:49:24
** 描        述: 蜂鸣器函数
** 版	     本: V1.0
** 主  控  芯 片:STM32F407VET6   晶振频率: 25
** IDE:  MDK 4.54
**********************************************************************************************************/
#ifndef __KEY_H
#define	__KEY_H

#include "stm32f4xx.h"

//按键值 
#define		KEY_F1		1
#define		KEY_F2		2
#define		KEY_F3		3	
#define		KEY_F4		4
#define		KEY_F5		5
#define		KEY_F6		6
#define		KEY_F7		7
#define		KEY_F8		8

#define		KEY_1		9
#define		KEY_2		10
#define		KEY_3		11
#define		KEY_4		12
#define		KEY_5		13
#define		KEY_6		14
#define		KEY_7		15
#define		KEY_8		16
#define		KEY_9		17
#define		KEY_0		18
#define		KEY_POINT	19
#define		KEY_BACK	20
#define		KEY_SET		21
#define		KEY_CALI	22		
#define		KEY_RUN		23
#define		KEY_STOP	24	
#define		SCREW_UP	25
#define		SCREW_DOWN	26
#define		OIL_RUN		27
#define		KEY_SHIFT	28
#define		KEY_ADD		29
#define		BACKUP3		30
#define		BACKUP4		31			
  
   
void key_init(void);
uint8_t get_key(uint32_t led);
uint32_t GetLedBit( uint8_t key );

#endif




	

	

	












