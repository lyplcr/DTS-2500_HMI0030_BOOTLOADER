/****************************************Copyright (c)**************************************************
**                               	杭州鑫高科技有限公司
**                                   
**                                 
** 文   件   名: date.c
** 最后修改日期: 2014/4/25 20:20:27
** 描        述: 日期处理函数
** 版	     本: V1.0
** 主 控  芯 片: STM32F407VET6	 晶振频率:	25MHz
** IDE		  :	 MDK 4.70
**********************************************************************************************************/
#include "date.h"
#include "pcf8563.h"


  
//判断是否是闰年
#define leap(y) ((y)%4==0 && (y)%100!=0 || (y)%400==0)
 
 
//闰年:1月到12月每个月的天数 31,29,31,30,31,30,31,31,30,31,30,31
//非闰年:1月到12月每个月的天数 31,28,31,30,31,30,31,31,30,31,30,31
//mon存的是非闰年当前月之前的总天数 
const uint16_t  mon[2][13]= 
{
	{0,0,31,59,90,120,151,181,212,243,273,304,334},
	{0,0,31,60,91,121,152,182,213,244,274,305,335}
};
			  
/**********************************************************************
functionName:uint32 days_in_year(struct d day)
description:计算给定日期是这一年中的第几天
**********************************************************************/ 
uint32_t days_in_year(tTime day)
{
	uint32_t sum=0;
	sum=mon[leap(day.usYear)][day.ucMon]+day.ucMday;
	return sum;
} 
 
/**********************************************************************
functionName:int days_interval(DATE_TypeDef d1,DATE_TypeDef d2)
description:计算两个给定日期之间相差的天数 
**********************************************************************/  
int32_t days_interval(tTime d1,tTime d2)
{
	tTime temp_day;
	int32_t sum;
	int32_t i;
	uint8_t flag=0; 
	if(d1.usYear>d2.usYear)
	{
		temp_day=d2;
		d2=d1;
		d1=temp_day;
		flag=1;
	}	
	sum=days_in_year(d2)-days_in_year(d1);				//计算同一年中差几天; 
	for(i=d1.usYear;i<d2.usYear;i++)
	sum+=leap(i)?366:365;	
	if(flag)
	sum=-sum;	
	return sum;
} 


/**********************************************************************
functionName:void fstime2tTime(tTime *t1,uint32 t2)
description: 把文件系统的时间转换成tTime类型的时间 
**********************************************************************/  
void fstime2tTime(tTime *t,uint16_t fdate,uint16_t ftime)
{
	t->usYear=(fdate>>9)+1980;
	t->ucMon=(fdate>>5)&0x0F;
	t->ucMday=fdate&0x1F;
	t->ucHour=ftime>>11;
	t->ucMin=(ftime>>5)&0x3F;
	t->ucSec =(ftime&0x1F)<<1;
}

/**********************************************************************
functionName:void tTime2fstime(tTime t1,uint32_t *t2)
description: tTime格式的时间转换到文件系统格式的时间
**********************************************************************/    
void tTime2fstime(tTime t1,uint32_t *t2)
{
	*t2=((t1.usYear-1980) << 25)   	 	// Year = 2007
            | (t1.ucMon << 21)            	// Month = June
            | (t1.ucMday << 16)           	// Day = 5
            ;
} 

//void tTime2fstime(tTime t1,uint32_t *t2)
//{
//	*t2=((t1.usYear-1980) << 25)   	 			// Year = 2007
//            | (t1.ucMon << 21)            	// Month = June
//            | (t1.ucMday << 16)           	// Day = 5
//            | (t1.ucHour << 11)           	// Hour = 11
//            | (t1.ucMin << 5)            		// Min = 38
//            | (t1.ucSec >> 1)             	// Sec = 0
//            ;
//}


tTime	time_global;		//全局时钟 

/**********************************************************************
functionName:void rtc_init(void)
description:RTC初始化
**********************************************************************/ 
void rtc_init(void)
{
	pcf8563_init(); 
}

/**********************************************************************
functionName:tTime get_time(void)
description:通过此函数获取全局时间
**********************************************************************/
tTime get_time(void)
{
	return time_global;
}
 
 
/**********************************************************************
functionName:void time_cycle(void)
description:需要周期运行的函数，通过此函数将时间读到时间全局变量
**********************************************************************/ 
void time_cycle(void)
{
	time_get(&time_global);
	date_get(&time_global);
}



/**********************************************************************
functionName:void time_set(tTime t)
description:时间日期设置
**********************************************************************/ 
void time_set(tTime t)
{
	pcf8563_time_set(&t);	
}
  
  
 
/******************* (C) COPYRIGHT 2012 XinGao Tech *****END OF FILE****/
