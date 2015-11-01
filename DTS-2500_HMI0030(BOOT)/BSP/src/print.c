/****************************************Copyright (c)**************************************************
**                               	杭州鑫高科技有限公司
**                                   
**                                 
** 文   件   名: uart.c
** 最后修改日期: 2012-8-18  
** 描        述: 串口
** 版	     本: V1.0
** 主 控  芯 片: STM32F407VET6		晶振频率: 25MHz
** IDE		  :	MDK 4.70 
**********************************************************************************************************/
#include "uart6.h" 
#include "print.h"



/**********************************************************************
functionName:void print_enter(void)
description: 打印缓冲区中所有数据都将被打印而且向前走一行，效果与回车命
令（CR）一样。
**********************************************************************/
void print_enter(void)
{
	uart6_send_char(0x0A);	 
}  
 
/**********************************************************************
functionName:void print_run_paper(uint8_t n)
description: 打印机向前进给n点行
**********************************************************************/
void print_run_paper(uint8_t n)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x4A);	//'J'
	uart6_send_char(n);
}

/**********************************************************************
functionName:void print_set_vertical_space(uint8_t n)
description:设置n点行间距
**********************************************************************/
void print_set_vertical_space(uint8_t n)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x31);	//'1'
	uart6_send_char(n);
}

 

/**********************************************************************
functionName:void print_space_hori(uint8_t n)
description:打印n个空格
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
description:打印n个空行
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
description:设置左限
**********************************************************************/
void print_set_left(uint8_t n)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x6C);	//'l'
	uart6_send_char(n);
}

/**********************************************************************
functionName:void print_set_right(uint8_t n)
description:设置右限
**********************************************************************/
void print_set_right(uint8_t n)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x51);	//'Q'
	uart6_send_char(n);
}

/**********************************************************************
functionName:void print_zoom_hori(uint8_t n)
description:横向放大
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
description:纵向放大 
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
description:横向纵向放大  
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
description:设置下划线 1有下划线 0没有下划线
**********************************************************************/
void print_set_underline(uint8_t en)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x2D);	//'_'
	uart6_send_char(en);
}
 
/**********************************************************************
functionName:void print_set_invert(uint8_t en)
description:允许/禁止反白打印  
**********************************************************************/
void print_set_invert(uint8_t en)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x69);	//'i'
	uart6_send_char(en);
}
 
/**********************************************************************
functionName:void print_init(void)
description:初始化打印机
**********************************************************************/
void print_init(void)
{
	uart6_init(19200);
	uart6_send_char(0x1B);		//ESC
	uart6_send_char(0x40);		//'@'
	uart6_send_char(0x1B);		//ESC
	uart6_send_char(0x63);		//'c'
	uart6_send_char(1);			//反向打印方式禁止		
}

/**********************************************************************
functionName:void print_use_chinese(void)
description:设置国标一、二级字库汉字打印方式 
**********************************************************************/
void print_chinese(void)
{
	uart6_send_char(0x1C);	//FS
	uart6_send_char(0x26);	//'&'
}

/**********************************************************************
functionName:void print_use_english(void)
description:打印机将从汉字打印方式切换到西文字符打印方式
**********************************************************************/
void print_english(void)
{
	uart6_send_char(0x1C);	//FS
	uart6_send_char(0x2E);	//'.'
}

/**********************************************************************
functionName:void print_direct(uint8_t dire)
description:如果参数为0，选择反向打印,否则按照正常打印
**********************************************************************/
void print_direct(uint8_t dire)
{
	uart6_send_char(0x1B);	//ESC
	uart6_send_char(0x63);	//'c'
	uart6_send_char(dire);	
}

/**********************************************************************
functionName:void print(char *dat)
description:打印文字
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
