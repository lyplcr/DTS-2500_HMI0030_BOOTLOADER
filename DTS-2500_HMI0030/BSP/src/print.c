/****************************************Copyright (c)**************************************************
**                               	�����θ߿Ƽ����޹�˾
**                                   
**                                 
** ��   ��   ��: uart.c
** ����޸�����: 2012-8-18  
** ��        ��: ����
** ��	     ��: V1.0
** �� ��  о Ƭ: STM32F407VET6		����Ƶ��: 25MHz
** IDE		  :	MDK 4.70 
**********************************************************************************************************/
#include "uart6.h" 
#include "print.h"



/**********************************************************************
functionName:void print_enter(void)
description: ��ӡ���������������ݶ�������ӡ������ǰ��һ�У�Ч����س���
�CR��һ����
**********************************************************************/
void print_enter(void)
{
	uart6_send_char(0x0A);	 
}  
 
/**********************************************************************
functionName:void print_run_paper(uint8_t n)
description: ��ӡ����ǰ����n����
**********************************************************************/
void print_run_paper(uint8_t n)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x4A);	//'J'
	uart6_send_char(n);
}

/**********************************************************************
functionName:void print_set_vertical_space(uint8_t n)
description:����n���м��
**********************************************************************/
void print_set_vertical_space(uint8_t n)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x31);	//'1'
	uart6_send_char(n);
}

 

/**********************************************************************
functionName:void print_space_hori(uint8_t n)
description:��ӡn���ո�
**********************************************************************/
void print_space_hori(uint8_t n)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x66);	//'f'
	uart6_send_char(0);
	uart6_send_char(n);
}

/**********************************************************************
functionName:void print_space_veri(uint8_t n)
description:��ӡn������
**********************************************************************/
void print_space_veri(uint8_t n)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x66);	//'f'
	uart6_send_char(1);
	uart6_send_char(n);
}

/**********************************************************************
functionName:void print_set_left(uint8_t n)
description:��������
**********************************************************************/
void print_set_left(uint8_t n)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x6C);	//'l'
	uart6_send_char(n);
}

/**********************************************************************
functionName:void print_set_right(uint8_t n)
description:��������
**********************************************************************/
void print_set_right(uint8_t n)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x51);	//'Q'
	uart6_send_char(n);
}

/**********************************************************************
functionName:void print_zoom_hori(uint8_t n)
description:����Ŵ�
**********************************************************************/
void print_zoom_hori(uint8_t n)
{
	if(!n)
	n=2;
	if(n>8)
	n=2;
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x55);	//'U'
	uart6_send_char(n);
}


/**********************************************************************
functionName:void print_zoom_veri(uint8_t n)
description:����Ŵ� 
**********************************************************************/
void print_zoom_veri(uint8_t n)
{
	if(!n)
	n=2;
	if(n>8)
	n=2;
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x56);	//'V'
	uart6_send_char(n);
}

/**********************************************************************
functionName:void print_zoom_all(uint8_t n)
description:��������Ŵ�  
**********************************************************************/
void print_zoom_all(uint8_t n)
{
	if(!n)
	n=2;
	if(n>8)
	n=2;		
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x57);	//'W'
	uart6_send_char(n);
}

/**********************************************************************
functionName:void print_set_underline(uint8_t en)
description:�����»��� 1���»��� 0û���»���
**********************************************************************/
void print_set_underline(uint8_t en)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x2D);	//'_'
	uart6_send_char(en);
}
 
/**********************************************************************
functionName:void print_set_invert(uint8_t en)
description:����/��ֹ���״�ӡ  
**********************************************************************/
void print_set_invert(uint8_t en)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x69);	//'i'
	uart6_send_char(en);
}
 
/**********************************************************************
functionName:void print_init(void)
description:��ʼ����ӡ��
**********************************************************************/
void print_init(void)
{
	uart6_init(19200);
	uart6_send_char(0x1B);		//ESC
	uart6_send_char(0x40);		//'@'
	uart6_send_char(0x1B);		//ESC
	uart6_send_char(0x63);		//'c'
	uart6_send_char(1);			//�����ӡ��ʽ��ֹ		
}

/**********************************************************************
functionName:void print_use_chinese(void)
description:���ù���һ�������ֿ⺺�ִ�ӡ��ʽ 
**********************************************************************/
void print_chinese(void)
{
	uart6_send_char(0x1C);	//FS
	uart6_send_char(0x26);	//'&'
}

/**********************************************************************
functionName:void print_use_english(void)
description:��ӡ�����Ӻ��ִ�ӡ��ʽ�л��������ַ���ӡ��ʽ
**********************************************************************/
void print_english(void)
{
	uart6_send_char(0x1C);	//FS
	uart6_send_char(0x2E);	//'.'
}

/**********************************************************************
functionName:void print_direct(uint8_t dire)
description:�������Ϊ0��ѡ�����ӡ,������������ӡ
**********************************************************************/
void print_direct(uint8_t dire)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x63);	//'c'
	uart6_send_char(dire);	
}

/**********************************************************************
functionName:void print(char *dat)
description:��ӡ����
**********************************************************************/ 
void print(char *dat)
{
	char *d;
	d=dat;
	while(*d)
	{
		uart6_send_char((uint8_t)*d);
		d++;
	}
} 



 


  
/******************* (C) COPYRIGHT 2012 XinGao Tech 			 *****END OF FILE****/  
