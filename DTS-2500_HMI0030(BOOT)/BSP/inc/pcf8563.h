/****************************************Copyright (c)**************************************************
**                                杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: pcf8563.h
** 最后修改日期: 2014/4/22
** 描        述: 
** 版	     本: V1.0
** 主  控  芯 片:  晶振频率: 
** IDE:  
**********************************************************************************************************/
#ifndef __PCF8563_H
#define	__PCF8563_H

#include "stm32f4xx.h" 
#include "ustdlib.h"
  
void pcf8563_init(void);  
void time_get(tTime *time);
void pcf8563_time_set(tTime *time);
void date_get(tTime *time);
 

#endif




	

	

	












