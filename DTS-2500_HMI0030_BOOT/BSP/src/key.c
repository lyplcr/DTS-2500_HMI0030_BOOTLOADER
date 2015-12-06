/****************************************Copyright (c)**************************************************
**                               	�����θ߿Ƽ����޹�˾
**                                   
**                                 
** ��   ��   ��: key.c
** ����޸�����: 2015-6-5 8:50:18
** ��        ��: ��������
** ��	     ��: V1.0
** �� ��  о Ƭ: STM32F407VET6   ����Ƶ��: 25MHz
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


/**********************************************************************
functionName:void key_init(void)
description: ������ʼ��
**********************************************************************/      
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_AHB1PeriphClockCmd(KEY_GPIO_CLK, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = KEY_PIN_CLK|KEY_PIN_LOCK|KEY_PIN_LEDDAT|KEY_PIN_LEDRCK|KEY_PIN_RSV;				  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			 //�����ٶ�50MHZ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			 	//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = KEY_PIN_DAT;				   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			 	//���ģʽ GPIO_OType_PP;
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
description: ��ȡ����ֵ,������LED�� 
**********************************************************************/    
uint32_t get_key(uint32_t led)
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
	 
	return key;
}

 
/******************* (C) COPYRIGHT 2012 XinGao Tech *****END OF FILE****/  
