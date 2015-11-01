/****************************************Copyright (c)**************************************************
**                               	�����θ߿Ƽ����޹�˾
**                                   
**                                 
** ��   ��   ��: timer.c
** ����޸�����: 2015-7-1 15:45:45
** ��        ��: ��ʱ������
** ��	     ��: V1.0
** �� ��  о Ƭ: STM32F407VET6	 ����Ƶ��:	25MHz
** IDE        :	MDK 4.70

������ҪӦ�õ���F407�Ķ�ʱ��3����ʱ���ܡ�����������԰�����������ײ�����
**********************************************************************************************************/
#include "timer.h"
#include "stdio.h"

#define BSP_Printf		printf
#define TIM3_CLOCK_FREQ		84000000			//��ʱ��ʱ��


/* ��2��ȫ�ֱ���ת���� bsp_DelayMS() ���� */
static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;

/* ���������ʱ���ṹ����� */
static SOFT_TMR s_tTmr[TMR_COUNT];

/*
	ȫ������ʱ�䣬��λ1ms
	����Ա�ʾ 24.85�죬�����Ĳ�Ʒ��������ʱ�䳬�������������뿼���������
*/
__IO int32_t g_iRunTime = 0;
__IO int32_t g_iRunTime10ms = 0;


static void bsp_SoftTimerDec(SOFT_TMR *_tmr); 
void beep_init(void);


/**********************************************************************
*	�� �� ��: timer_init
*	����˵��: ��ʱ����ʼ����ʹ�ö�ʱ��3
*	��    ��:  Period�Զ���װ��ֵ��PrescaleԤ��Ƶֵ
*	�� �� ֵ:  ��
**********************************************************************/
void timer_init(uint16_t Period,uint16_t Prescale)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  			//ʹ��TIM3ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = Period; 					//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=Prescale;  				//��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);				//��ʼ��TIM3
	
	
	TIM_ClearFlag(TIM3,TIM_IT_Update);								//������±�־
	TIM_SetCounter(TIM3,0);																					//���ö�ʱ��ֵ
	 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 						//����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE);																					//ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 					//��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; 		//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; 			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
	beep_init();
} 

/*
*********************************************************************************************************
*	�� �� ��: bsp_SoftTimerDec
*	����˵��: ÿ��1ms�����ж�ʱ��������1�����뱻SysTick_ISR�����Ե��á�
*	��    ��:  _tmr : ��ʱ������ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->Count > 0)
	{
		/* �����ʱ����������1�����ö�ʱ�������־ */
		if (--_tmr->Count == 0)
		{
			_tmr->Flag = 1;

			/* ������Զ�ģʽ�����Զ���װ������ */
			if(_tmr->Mode == TMR_AUTO_MODE)
			{
				_tmr->Count = _tmr->PreLoad;
			}
		}
	}
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_InitTimer
*	����˵��: ����systick�жϣ�����ʼ�������ʱ������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitTimer(void)
{
	uint8_t i;

	/* �������е������ʱ�� */
	for (i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;	/* ȱʡ��1���Թ���ģʽ */
	}
	
	g_iRunTime=0;
	g_iRunTime10ms=0;
	 
	/*
		����TIM3�ж�����Ϊ1ms��������TIM3��ʱ�жϡ�
  	APB1Ƶ��42MHz�����Զ�ʱ��Ƶ��84MHz��Ԥ��Ƶ84,TIM3��ʱʱ��Ϊ1MHz���Զ���װֵΪ999��TIM3�Ķ�ʱ����Ϊ1ms�� 
    ���ڳ����Ӧ�ã�����һ��ȡ��ʱ����1ms�����ڵ���CPU���ߵ͹���Ӧ�ã��������ö�ʱ����Ϊ 10ms
    */
	timer_init(999,83);
}
 
   

/*
*********************************************************************************************************
*	�� �� ��: bsp_DelayMS
*	����˵��: ms���ӳ٣��ӳپ���Ϊ����1ms
*	��    ��:  n : �ӳٳ��ȣ���λ1 ms�� n Ӧ����2
*	�� �� ֵ: ��
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

	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);  			/* ���ж� */
	s_uiDelayCount = n;
	s_ucTimeOutFlag = 0;
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);				/* ���ж� */
   			 
	while (1)
	{
//		bsp_Idle();				/* CPU����ִ�еĲ����� �� bsp.c �� bsp.h �ļ� */

		/*
			�ȴ��ӳ�ʱ�䵽
			ע�⣺��������Ϊ s_ucTimeOutFlag = 0�����Կ����Ż�������� s_ucTimeOutFlag ������������Ϊ volatile
		*/
		if (s_ucTimeOutFlag == 1)
		{
			break;
		}
	}
} 


/*
*********************************************************************************************************
*    �� �� ��: bsp_DelayUS
*    ����˵��: us���ӳ١� ������systick��ʱ����������ܵ��ô˺�����
*    ��    ��:  n : �ӳٳ��ȣ���λ1 us
*    �� �� ֵ: ��
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
    ticks = n;	 		/* ��Ҫ�Ľ����� */  
    
    tcnt = 0;
    told = TIM3->CNT;             /* �ս���ʱ�ļ�����ֵ */

    while (1)
    {
        tnow = TIM3->CNT;    
        if (tnow != told)
        {    
            /* ����Ϊ+������ */    
            if (tnow > told)
            {
                tcnt +=tnow-told;    
            }
            /* ����װ�صݼ� */
            else
            {
                tcnt += reload + tnow - told;    
            }        
            told = tnow;

            /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
            if (tcnt >= ticks)
            {
            	break;
            }
        }  
    }
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_StartTimer
*	����˵��: ����һ����ʱ���������ö�ʱ���ڡ�
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ1ms
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);  			/* ���ж� */ 
	s_tTmr[_id].Count = _period;		/* ʵʱ��������ֵ */
	s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1���Թ���ģʽ */ 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  			/* ���ж� */
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_StartAutoTimer
*	����˵��: ����һ���Զ���ʱ���������ö�ʱ���ڡ�
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ10ms
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);  			/* ���ж� */ 

	s_tTmr[_id].Count = _period;			/* ʵʱ��������ֵ */
	s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	/* �Զ�����ģʽ */

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  			/* ���ж� */
}
 
/*
*********************************************************************************************************
*	�� �� ��: bsp_StopTimer
*	����˵��: ֹͣһ����ʱ��
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StopTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);  			/* ���ж� */ 

	s_tTmr[_id].Count = 0;				/* ʵʱ��������ֵ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* �Զ�����ģʽ */

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  			/* ���ж� */
}
 
/*
*********************************************************************************************************
*	�� �� ��: bsp_CheckTimer
*	����˵��: ��ⶨʱ���Ƿ�ʱ
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ1ms
*	�� �� ֵ: ���� 0 ��ʾ��ʱδ���� 1��ʾ��ʱ��
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
*	�� �� ��: bsp_GetRunTime
*	����˵��: ��ȡCPU����ʱ�䣬��λ1ms������Ա�ʾ 24.85�죬�����Ĳ�Ʒ��������ʱ�䳬�������������뿼���������
*	��    ��:  ��
*	�� �� ֵ: CPU����ʱ�䣬��λ1ms
*********************************************************************************************************
*/
int32_t bsp_GetRunTime(void)
{
	int32_t runtime;

	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);  			/* ���ж� */

	runtime = g_iRunTime;	/* ���������Systick�ж��б���д�������Ҫ���жϽ��б��� */

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  			/* ���ж� */

	return runtime;
} 

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetRunTime10ms
*	����˵��: ��ȡCPU����ʱ�䣬��λ10ms������Ա�ʾ 248.5�죬�����Ĳ�Ʒ��������ʱ�䳬�������������뿼���������
*	��    ��:  ��
*	�� �� ֵ: CPU����ʱ�䣬��λ10ms
*********************************************************************************************************
*/
int32_t bsp_GetRunTime10ms(void)
{
	int32_t runtime;

	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);  			/* ���ж� */

	runtime = g_iRunTime10ms;	/* ���������Systick�ж��б���д�������Ҫ���жϽ��б��� */

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  			/* ���ж� */

	return runtime;
} 


static SOFT_TMR_US	TmrUs[TMR_COUNT_US];
/*
*********************************************************************************************************
*	�� �� ��: bsp_StartTimeUS
*	����˵��: US����ʱ��ʼ
*	��    ��:  ��ʱ��ID,0~3
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartTimeUS(uint8_t _id)
{ 
	TmrUs[_id].startRunTime=bsp_GetRunTime();
	TmrUs[_id].startTick=TIM3->CNT;  
}
  
  
/*
*********************************************************************************************************
*	�� �� ��: bsp_GetTimeUS
*	����˵��: ��ȷ��US��ʱ����������Լ�ʱ2147��,����1uS��
*	��    ��: ��ʱ��ID,0~3
*	�� �� ֵ: ������ִ�� bsp_StartTimerUS��ʼ��������ִ�������е�ʱ�䣬��λUs
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
*	�� �� ��: void beep_init(void)
*	����˵��: ��������ʼ��  
*	��    ��:  ��
*	�� �� ֵ:  ��
**********************************************************************/ 
void beep_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	/* ��GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* ���������費ʹ�� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO������ٶ� */

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
*	�� �� ��: void beep(uint16_t beep,uint16_t mute,uint16_t time)
*	����˵��:   
*	��    ��:  ���������ʱ��ms,���ʱ��ms,��Ĵ���
*	�� �� ֵ:  ��
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
*	�� �� ��: TIM3_IRQHandler
*	����˵��: TIM3�жϺ���
*	��    ��:  ��
*	�� �� ֵ:  ��
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

		/* ÿ��1ms����1�� �������� bsp_DelayMS�� */
		if (s_uiDelayCount > 0)
		{
			if (--s_uiDelayCount == 0)
			{
				s_ucTimeOutFlag = 1;
			}
		}
	
		/* ÿ��1ms���������ʱ���ļ��������м�һ���� */
		for (i = 0; i < TMR_COUNT; i++)
		{
			bsp_SoftTimerDec(&s_tTmr[i]);
		}
	
		/* ȫ������ʱ��ÿ1ms��1 */
		g_iRunTime++;
		if (g_iRunTime == 0x7FFFFFFF)	/* ��������� int32_t ���ͣ������Ϊ 0x7FFFFFFF */
		{
			g_iRunTime = 0;
		}
	
//			bsp_RunPer1ms();		/* ÿ��1ms����һ�δ˺������˺����� bsp.c */
	
		/* ȫ������ʱ��ÿ10ms��1 */
		if (++s_count >= 10)		
		{
			s_count = 0;
			g_iRunTime10ms++;
			if (g_iRunTime10ms == 0x7FFFFFFF)	/* ��������� int32_t ���ͣ������Ϊ 0x7FFFFFFF */
			{
					g_iRunTime10ms = 0;
			} 
			//bsp_RunPer10ms();	/* ÿ��10ms����һ�δ˺������˺����� bsp.c */
					
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
