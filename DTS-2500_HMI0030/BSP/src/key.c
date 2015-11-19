/****************************************Copyright (c)**************************************************
**                               	杭州鑫高科技有限公司
**                                   
**                                 
** 文   件   名: key.c
** 最后修改日期: 2015-6-5 8:50:18
** 描        述: 按键函数
** 版	     本: V1.0
** 主 控  芯 片: STM32F407VET6   晶振频率: 25MHz
** IDE:  		 MDK 4.70
**********************************************************************************************************/
#include "key.h"
#include "timer.h"
 
#define		KEY_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		KEY_GPIO_PORT		GPIOE

#define		KEY_PIN_CLK			GPIO_Pin_0
#define		KEY_PIN_LOCK		GPIO_Pin_1
#define		KEY_PIN_DAT			GPIO_Pin_2
#define		KEY_PIN_LEDDAT		GPIO_Pin_3
#define		KEY_PIN_LEDRCK		GPIO_Pin_4
#define		KEY_PIN_RSV   		GPIO_Pin_5

#define		SET_KEY_CLK()		GPIO_SetBits(KEY_GPIO_PORT,KEY_PIN_CLK)
#define		SET_KEY_LOCK() 		GPIO_SetBits(KEY_GPIO_PORT,KEY_PIN_LOCK)
#define		SET_KEY_LEDDAT()	GPIO_SetBits(KEY_GPIO_PORT,KEY_PIN_LEDDAT)	
#define		SET_KEY_LEDRCK()	GPIO_SetBits(KEY_GPIO_PORT,KEY_PIN_LEDRCK)
 
#define		CLR_KEY_CLK()		GPIO_ResetBits(KEY_GPIO_PORT,KEY_PIN_CLK)	
#define		CLR_KEY_LOCK() 		GPIO_ResetBits(KEY_GPIO_PORT,KEY_PIN_LOCK)
#define		CLR_KEY_LEDDAT()	GPIO_ResetBits(KEY_GPIO_PORT,KEY_PIN_LEDDAT)	
#define		CLR_KEY_LEDRCK()	GPIO_ResetBits(KEY_GPIO_PORT,KEY_PIN_LEDRCK)

#define		ST_KEY_DAT()   		GPIO_ReadInputDataBit(KEY_GPIO_PORT,KEY_PIN_DAT)	

uint8_t conv_key(uint32_t key);

/**********************************************************************
functionName:void key_init(void)
description: 按键初始化
**********************************************************************/      
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_AHB1PeriphClockCmd(KEY_GPIO_CLK, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = KEY_PIN_CLK|KEY_PIN_LOCK|KEY_PIN_LEDDAT|KEY_PIN_LEDRCK|KEY_PIN_RSV;				  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			 //口线速度50MHZ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			 	//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = KEY_PIN_DAT;				   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			 	//输出模式 GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure); 
	
	CLR_KEY_CLK();
	SET_KEY_LOCK();
	CLR_KEY_LEDDAT(); 
	CLR_KEY_LEDRCK();
	 
	get_key(0x00000000);  
}   
   
   
/**********************************************************************
functionName:uint32_t get_key(uint32_t led)
description: 获取按键值,并设置LED灯 
**********************************************************************/    
uint8_t get_key(uint32_t led)
{
	uint8_t i;
	uint32_t key=0;
	CLR_KEY_LOCK();
	bsp_DelayUS(1);
	SET_KEY_LOCK(); 
	
	for(i=0;i<32;i++)
	{
		if(led&0x80000000)
		SET_KEY_LEDDAT();
		else
		CLR_KEY_LEDDAT();
		  	 
		led<<=1;  	 
		key<<=1;
		if(ST_KEY_DAT())
		{
			key|=0x01;
		} 
		SET_KEY_CLK();
//		bsp_DelayUS(1);
		CLR_KEY_CLK();	
	}	   
	SET_KEY_LEDRCK();
//	bsp_DelayUS(1);
	key=~key;
	CLR_KEY_LEDRCK(); 
	 
	return conv_key(key);
}
 
/**********************************************************************
functionName:uint8_t conv_key(uint32_t key)
description: 键值转换 
**********************************************************************/    
uint8_t conv_key(uint32_t key)
{
	uint8_t key_value = 0;
	
	switch ( key )
	{
		 case 0x01:
			key_value = KEY_F1;
			break;
		 case 0x02:
			key_value = KEY_F2;
			break;
		 case 0x04:
			key_value = KEY_F3;
			break;
		 case 0x08:
			key_value = KEY_F4;
			break;
		 case 0x10:
			key_value = KEY_F5;
			break;
		 case 0x20:
			key_value = KEY_F6;
			break;
		 case 0x40:
			key_value = KEY_F7;
			break;
		 case 0x80:
			key_value = KEY_F8;
			break;		 
		 case 0x100:
			key_value = KEY_1;
			break;
		 case 0x200:
			key_value = KEY_2;
			break;
		 case 0x400:
			key_value = KEY_3;
			break;
		 case 0x800:
			key_value = KEY_4;
			break;
		 case 0x1000:
			key_value = KEY_5;
			break;
		 case 0x2000:
			key_value = KEY_6;
			break;
		 case 0x4000:
			key_value = KEY_7;
			break;
		 case 0x8000:
			key_value = KEY_8;
			break;
		 case 0x10000:
			key_value = KEY_9;
			break;
		 case 0x40000:
			key_value = KEY_0;
			break;
		 case 0x20000:
			key_value = KEY_POINT;
			break;
		 case 0x80000:
			key_value = KEY_BACK;
			break;
		 case 0x100000:
			key_value = KEY_SET;
			break;
		 case 0x400000:
			key_value = KEY_CALI;
			break;
		 case 0x4000000:
			key_value = KEY_RUN;
			break;
		 case 0x8000000:
			key_value = KEY_STOP;
			break;
		 case 0x10000000:
			key_value = SCREW_UP;
			break;
		 case 0x20000000:
			key_value = SCREW_DOWN;
			break;
		 case 0x40000000:
			key_value = OIL_RUN;
			break;
		 case 0x200000:
			key_value = KEY_SHIFT;
			break;
		 case 0x800000:
			key_value = KEY_ADD;
			break;
		 case 0x1000000:
			key_value = BACKUP3;
			break;
		 case 0x2000000:
			key_value = BACKUP4;
			break;
		default:
			key_value = 0;
			break;
	}
	
	return key_value;
}

/**********************************************************************
functionName:uint32_t GetLedBit( uint8_t key )
description: 获取当前键值对应的LED位 
**********************************************************************/
uint32_t GetLedBit( uint8_t key )
{
	uint32_t bit = 0;
	
	switch ( key )
	{
		 case KEY_F1:
			bit = 0xffff0001;
			break;
		 case KEY_F2:
			bit = 0x00000002;
			break;
		 case KEY_F3:
			bit = 0x00000004;
			break;
		 case KEY_F4:
			bit = 0x00000008;
			break;
		 case KEY_F5:
			bit = 0x00000010;
			break;
		 case KEY_F6:
			bit = 0x00000020;
			break;
		 case KEY_F7:
			bit = 0x00000040;
			break;
		 case KEY_F8:
			bit = 0x00000080;
			break;		 
		 case KEY_BACK:
			bit = 0x00008000;
			break;
		 case KEY_SET:
			bit = 0x00000100;
			break;
		 case KEY_CALI:
			bit = 0x00000200;
			break;
		 case KEY_RUN:
			bit = 0x00000400;
			break;
		 case KEY_STOP:
			bit = 0x00000800;
			break;
		 case SCREW_UP:
			bit = 0x00001000;
			break;
		 case SCREW_DOWN:
			bit = 0x00002000;
			break;
		 case OIL_RUN:
			bit = 0x00004000;
			break;
		default:
			bit = 0xffff0000;
			break;
	}
	
//	bit = ~bit;
	
	return bit;
}
 
 
/******************* (C) COPYRIGHT 2012 XinGao Tech *****END OF FILE****/  
