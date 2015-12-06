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
// #define		KEY_F1		1
// #define		KEY_F2		2
// #define		KEY_F3		3	
// #define		KEY_F4		4
// #define		KEY_F5		5
// #define		KEY_F6		6
// #define		KEY_F7		7
// #define		KEY_F8		8

// #define		KEY_1		9
// #define		KEY_2		10
// #define		KEY_3		11
// #define		KEY_4		12
// #define		KEY_5		13
// #define		KEY_6		14
// #define		KEY_7		15
// #define		KEY_8		16
// #define		KEY_9		17
// #define		KEY_0		18
// #define		KEY_POINT	19
// #define		KEY_BACK	20
// #define		KEY_SET		21
// #define		KEY_CALI	22		
// #define		KEY_RUN		23
// #define		KEY_STOP	24	
// #define		SCREW_UP	25
// #define		SCREW_DOWN	26
// #define		OIL_RUN		27
// #define		KEY_SHIFT	28
// #define		KEY_ADD		29
// #define		BACKUP3		30
// #define		BACKUP4		31			
  
/* 按键定义 */
#define		KEY_NONE				0x00000000
#define		KEY_ADD					0x00000001
#define		KEY_7					0x00000002
#define		KEY_4					0x00000004	
#define		KEY_1					0x00000008
#define		KEY_2					0x00000010
#define		KEY_5					0x00000020
#define		KEY_8					0x00000040
#define		KEY_0					0x00000080
#define		KEY_POINT				0x00000100
#define		KEY_9					0x00000200
#define		KEY_6					0x00000400
#define		KEY_3					0x00000800
#define		KEY_BACK				0x00001000
#define		KEY_LEFT				0x00002000
#define		KEY_SHIFT				0x00004000
#define		KEY_EXPORT				0x00008000		//导出
#define		KEY_DISPLACE_TARE		0x00008000		//位移清零
#define		KEY_PRINT				0x00010000		//打印
#define		KEY_DEFORMATE_TARE		0x00010000		//变形清零
#define		KEY_DOWN				0x00020000
#define		KEY_ENTER				0x00040000
#define		KEY_UP					0x00080000
#define		KEY_ESC					0x00100000
#define		KEY_RIGHT				0x00200000		
#define		KEY_FIND				0x00400000
#define		KEY_FORCE_TARE			0x00800000	
#define		KEY_BACKUP1				0x01000000
#define		KEY_OIL_RUN				0x02000000
#define		KEY_OIL_STOP			0x04000000
#define		KEY_BACKUP2				0x08000000
#define		KEY_F4					0x10000000
#define		KEY_F3					0x20000000
#define		KEY_F2					0x40000000	
#define		KEY_F1					0x80000000
   
void key_init(void);
uint32_t get_key(uint32_t led);

#endif




	

	

	












