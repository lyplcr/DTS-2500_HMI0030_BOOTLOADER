/****************************************Copyright (c)**************************************************
**                               	杭州鑫高科技有限公司
**                                   
**                                 
** 文   件   名: timer.c
** 最后修改日期: 2015-7-1 15:45:45
** 描        述: 软定时器函数
** 版	     本: V1.0
** 主 控  芯 片: STM32F407VET6	 晶振频率:	25MHz
** IDE        :	MDK 4.70

这里主要应用的是F407的定时器3做定时功能。本函数库改自安富莱开发板底层驱动
**********************************************************************************************************/
#include "timer.h"
#include "stdio.h"

#define BSP_Printf		printf
#define TIM3_CLOCK_FREQ		84000000			//定时器时钟


/* 这2个全局变量转用于 bsp_DelayMS() 函数 */
static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;

/* 定于软件定时器结构体变量 */
static SOFT_TMR s_tTmr[TMR_COUNT];

/*
	全局运行时间，单位1ms
	最长可以表示 24.85天，如果你的产品连续运行时间超过这个数，则必须考虑溢出问题
*/
__IO int32_t g_iRunTime = 0;
__IO int32_t g_iRunTime10ms = 0;


static void bsp_SoftTimerDec(SOFT_TMR *_tmr); 
void beep_init(void);


/**********************************************************************
*	函 数 名: timer_init
*	功能说明: 定时器初始化，使用定时器3
*	形    参:  Period自动重装载值，Prescale预分频值
*	返 回 值:  无
**********************************************************************/
void timer_init(uint16_t Period,uint16_t Prescale)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  			//使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = Period; 					//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=Prescale;  				//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);				//初始化TIM3
	
	
	TIM_ClearFlag(TIM3,TIM_IT_Update);								//清除更新标志
	TIM_SetCounter(TIM3,0);																					//设置定时器值
	 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 						//允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE);																					//使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 					//定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; 		//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; 			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
	beep_init();
} 

/*
*********************************************************************************************************
*	函 数 名: bsp_SoftTimerDec
*	功能说明: 每隔1ms对所有定时器变量减1。必须被SysTick_ISR周期性调用。
*	形    参:  _tmr : 定时器变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->Count > 0)
	{
		/* 如果定时器变量减到1则设置定时器到达标志 */
		if (--_tmr->Count == 0)
		{
			_tmr->Flag = 1;

			/* 如果是自动模式，则自动重装计数器 */
			if(_tmr->Mode == TMR_AUTO_MODE)
			{
				_tmr->Count = _tmr->PreLoad;
			}
		}
	}
}


/*
*********************************************************************************************************
*	函 数 名: bsp_InitTimer
*	功能说明: 配置systick中断，并初始化软件定时器变量
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitTimer(void)
{
	uint8_t i;

	/* 清零所有的软件定时器 */
	for (i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;	/* 缺省是1次性工作模式 */
	}
	
	g_iRunTime=0;
	g_iRunTime10ms=0;
	 
	/*
		配置TIM3中断周期为1ms，并启动TIM3定时中断。
  	APB1频率42MHz，所以定时器频率84MHz，预分频84,TIM3定时时钟为1MHz，自动重装值为999，TIM3的定时周期为1ms。 
    对于常规的应用，我们一般取定时周期1ms。对于低速CPU或者低功耗应用，可以设置定时周期为 10ms
    */
	timer_init(999,83);
}
 
   

/*
*********************************************************************************************************
*	函 数 名: bsp_DelayMS
*	功能说明: ms级延迟，延迟精度为正负1ms
*	形    参:  n : 延迟长度，单位1 ms。 n 应大于2
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_DelayMS(uint32_t n)
{
	if (n == 0)
	{
		return;
	}
	else if (n == 1)
	{
		n = 2;
	}

	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);  			/* 关中断 */
	s_uiDelayCount = n;
	s_ucTimeOutFlag = 0;
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);				/* 开中断 */
   			 
	while (1)
	{
//		bsp_Idle();				/* CPU空闲执行的操作， 见 bsp.c 和 bsp.h 文件 */

		/*
			等待延迟时间到
			注意：编译器认为 s_ucTimeOutFlag = 0，所以可能优化错误，因此 s_ucTimeOutFlag 变量必须申明为 volatile
		*/
		if (s_ucTimeOutFlag == 1)
		{
			break;
		}
	}
} 


/*
*********************************************************************************************************
*    函 数 名: bsp_DelayUS
*    功能说明: us级延迟。 必须在systick定时器启动后才能调用此函数。
*    形    参:  n : 延迟长度，单位1 us
*    返 回 值: 无
*********************************************************************************************************
*/
void bsp_DelayUS(uint32_t n)
{
    uint32_t ticks;
    uint32_t told;
    uint32_t tnow;
    uint32_t tcnt = 0;
    uint32_t reload;
       
	reload = 1000;                
    ticks = n;	 		/* 需要的节拍数 */  
    
    tcnt = 0;
    told = TIM3->CNT;             /* 刚进入时的计数器值 */

    while (1)
    {
        tnow = TIM3->CNT;    
        if (tnow != told)
        {    
            /* 配置为+计数器 */    
            if (tnow > told)
            {
                tcnt +=tnow-told;    
            }
            /* 重新装载递加 */
            else
            {
                tcnt += reload + tnow - told;    
            }        
            told = tnow;

            /* 时间超过/等于要延迟的时间,则退出 */
            if (tcnt >= ticks)
            {
            	break;
            }
        }  
    }
}

/*
*********************************************************************************************************
*	函 数 名: bsp_StartTimer
*	功能说明: 启动一个定时器，并设置定时周期。
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位1ms
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);  			/* 关中断 */ 
	s_tTmr[_id].Count = _period;		/* 实时计数器初值 */
	s_tTmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1次性工作模式 */ 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  			/* 开中断 */
}


/*
*********************************************************************************************************
*	函 数 名: bsp_StartAutoTimer
*	功能说明: 启动一个自动定时器，并设置定时周期。
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位10ms
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);  			/* 关中断 */ 

	s_tTmr[_id].Count = _period;			/* 实时计数器初值 */
	s_tTmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	/* 自动工作模式 */

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  			/* 开中断 */
}
 
/*
*********************************************************************************************************
*	函 数 名: bsp_StopTimer
*	功能说明: 停止一个定时器
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StopTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);  			/* 关中断 */ 

	s_tTmr[_id].Count = 0;				/* 实时计数器初值 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 自动工作模式 */

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  			/* 开中断 */
}
 
/*
*********************************************************************************************************
*	函 数 名: bsp_CheckTimer
*	功能说明: 检测定时器是否超时
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位1ms
*	返 回 值: 返回 0 表示定时未到， 1表示定时到
*********************************************************************************************************
*/
uint8_t bsp_CheckTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	if (s_tTmr[_id].Flag == 1)
	{
		s_tTmr[_id].Flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}
 
/*
*********************************************************************************************************
*	函 数 名: bsp_GetRunTime
*	功能说明: 获取CPU运行时间，单位1ms。最长可以表示 24.85天，如果你的产品连续运行时间超过这个数，则必须考虑溢出问题
*	形    参:  无
*	返 回 值: CPU运行时间，单位1ms
*********************************************************************************************************
*/
int32_t bsp_GetRunTime(void)
{
	int32_t runtime;

	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);  			/* 关中断 */

	runtime = g_iRunTime;	/* 这个变量在Systick中断中被改写，因此需要关中断进行保护 */

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  			/* 开中断 */

	return runtime;
} 

/*
*********************************************************************************************************
*	函 数 名: bsp_GetRunTime10ms
*	功能说明: 获取CPU运行时间，单位10ms。最长可以表示 248.5天，如果你的产品连续运行时间超过这个数，则必须考虑溢出问题
*	形    参:  无
*	返 回 值: CPU运行时间，单位10ms
*********************************************************************************************************
*/
int32_t bsp_GetRunTime10ms(void)
{
	int32_t runtime;

	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);  			/* 关中断 */

	runtime = g_iRunTime10ms;	/* 这个变量在Systick中断中被改写，因此需要关中断进行保护 */

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  			/* 开中断 */

	return runtime;
} 


static SOFT_TMR_US	TmrUs[TMR_COUNT_US];
/*
*********************************************************************************************************
*	函 数 名: bsp_StartTimeUS
*	功能说明: US级计时开始
*	形    参:  计时器ID,0~3
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartTimeUS(uint8_t _id)
{ 
	TmrUs[_id].startRunTime=bsp_GetRunTime();
	TmrUs[_id].startTick=TIM3->CNT;  
}
  
  
/*
*********************************************************************************************************
*	函 数 名: bsp_GetTimeUS
*	功能说明: 精确到US计时函数，最长可以计时2147秒,精度1uS。
*	形    参: 计时器ID,0~3
*	返 回 值: 返回自执行 bsp_StartTimerUS开始到本函数执行所运行的时间，单位Us
*********************************************************************************************************
*/ 
int32_t bsp_GetTimeUS(uint8_t _id)
{
	uint32_t Us = 0;
	uint32_t endRunTime = 0;
	uint32_t load=0;
	uint32_t endTick=TIM3->CNT;
	
	endRunTime=bsp_GetRunTime();
	load=TIM3->ARR;  
	Us = (endRunTime-TmrUs[_id].startRunTime)*(load+1) + endTick - TmrUs[_id].startTick;  
	
	return Us;
}


 
#define			BEEP_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define			BEEP_GPIO_PORT			GPIOB
#define			BEEP_GPIO_PIN			GPIO_Pin_8
 
typedef enum
{ 
	BEEP_STATUS_IDLE=0,
	BEEP_STATUS_BEEP,	 
	BEEP_STATUS_MUTE 
}BEEP_STATUS_TypeDef; 

  
volatile BEEP_STATUS_TypeDef beep_status=BEEP_STATUS_IDLE; 
volatile uint16_t beep_time=0;
volatile uint16_t beep_cnt=0;
volatile uint16_t mute_comp=0;
volatile uint16_t beep_comp=0;  

#define		beep_on()				GPIO_SetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN)
#define		beep_off()				GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN)


/**********************************************************************
*	函 数 名: void beep_init(void)
*	功能说明: 蜂鸣器初始化  
*	形    参:  无
*	返 回 值:  无
**********************************************************************/ 
void beep_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	/* 打开GPIO时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */

	GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;
	GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);
	
	beep_off(); 
	
	beep_cnt=0;
	beep_time=0;
	mute_comp=0;
	beep_comp=0;
	beep_status=BEEP_STATUS_IDLE;
} 	 


/**********************************************************************
*	函 数 名: void beep(uint16_t beep,uint16_t mute,uint16_t time)
*	功能说明:   
*	形    参:  蜂鸣器响的时间ms,灭的时间ms,响的次数
*	返 回 值:  无
**********************************************************************/ 
void beep(uint16_t beep_on,uint16_t mute,uint16_t time)
{
	if (beep_status != BEEP_STATUS_IDLE)
	{
		return;
	}
	
	if(time)
	{  
		beep_on(); 
		beep_cnt=0;
		beep_time=time;
		beep_comp=beep_on;
		mute_comp=beep_on+mute; 
		beep_status=BEEP_STATUS_BEEP; 
 	}
}
 
 
 
/**********************************************************************
*	函 数 名: TIM3_IRQHandler
*	功能说明: TIM3中断函数
*	形    参:  无
*	返 回 值:  无
**********************************************************************/
//extern void bsp_RunPer1ms(void);
//extern void bsp_RunPer10ms(void);
void TIM3_IRQHandler(void)
{
	static uint8_t s_count = 0;
	uint8_t i;
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  

		/* 每隔1ms进来1次 （仅用于 bsp_DelayMS） */
		if (s_uiDelayCount > 0)
		{
			if (--s_uiDelayCount == 0)
			{
				s_ucTimeOutFlag = 1;
			}
		}
	
		/* 每隔1ms，对软件定时器的计数器进行减一操作 */
		for (i = 0; i < TMR_COUNT; i++)
		{
			bsp_SoftTimerDec(&s_tTmr[i]);
		}
	
		/* 全局运行时间每1ms增1 */
		g_iRunTime++;
		if (g_iRunTime == 0x7FFFFFFF)	/* 这个变量是 int32_t 类型，最大数为 0x7FFFFFFF */
		{
			g_iRunTime = 0;
		}
	
//			bsp_RunPer1ms();		/* 每隔1ms调用一次此函数，此函数在 bsp.c */
	
		/* 全局运行时间每10ms增1 */
		if (++s_count >= 10)		
		{
			s_count = 0;
			g_iRunTime10ms++;
			if (g_iRunTime10ms == 0x7FFFFFFF)	/* 这个变量是 int32_t 类型，最大数为 0x7FFFFFFF */
			{
					g_iRunTime10ms = 0;
			} 
			//bsp_RunPer10ms();	/* 每隔10ms调用一次此函数，此函数在 bsp.c */
					
			switch(beep_status)
			{
				case BEEP_STATUS_BEEP:
				{
					beep_cnt++;
					if(beep_cnt>=beep_comp)
					{
						beep_off();
						beep_status=BEEP_STATUS_MUTE; 
					}	 
					break;
				}	
				case BEEP_STATUS_MUTE:
				{
					beep_cnt++;
					if(beep_cnt>=mute_comp)
					{
						beep_cnt=0;
						if(beep_time>1)
						{
							beep_time--; 	
							beep_status=BEEP_STATUS_BEEP;
							beep_on();
						}	
						else
						{
							beep_status=BEEP_STATUS_IDLE; 
						}	
					}	 
					break;
				}	
				case BEEP_STATUS_IDLE:
				{ 
					break;
				}
				default:
				{
					beep_off();
					beep_status=BEEP_STATUS_IDLE; 
					break;
				}	
			}
		}

	}	  
}   
    

/******************* (C) COPYRIGHT 2012 XinGao Tech *****END OF FILE****/
