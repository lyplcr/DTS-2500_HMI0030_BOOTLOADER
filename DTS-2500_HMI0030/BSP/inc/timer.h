/*
*********************************************************************************************************
*
*	ģ������ : ��ʱ��ģ��
*	�ļ����� : bsp_timer.h
*	��    �� : V1.1
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx.h"

		

/*
	�ڴ˶������ɸ������ʱ��ȫ�ֱ���
	ע�⣬��������__IO �� volatile����Ϊ����������жϺ���������ͬʱ�����ʣ��п�����ɱ����������Ż���
*/
#define TMR_COUNT	4		/* �����ʱ���ĸ��� ����ʱ��ID��Χ 0 - 3) */
#define	TMR_COUNT_US	4		//�����ʱ�������ڼ����¼���ʱ����ʱ��ID��Χ0-3	

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef enum
{
	TMR_ONCE_MODE = 0,		/* һ�ι���ģʽ */
	TMR_AUTO_MODE = 1		/* �Զ���ʱ����ģʽ */
}TMR_MODE_E;

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
	volatile uint8_t Mode;		/* ������ģʽ��1���� */
	volatile uint8_t Flag;		/* ��ʱ�����־  */
	volatile uint32_t Count;	/* ������ */
	volatile uint32_t PreLoad;	/* ������Ԥװֵ */
}SOFT_TMR;

/* uS����ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
	volatile uint32_t startRunTime;	/* ������ */
	volatile uint32_t startTick;	/* ������Ԥװֵ */
}SOFT_TMR_US;

typedef enum
{
	TIMEIN=0,
	TIMEOUT,
}
TIMEOUT_Typedef; 


/* �ṩ������C�ļ����õĺ��� */
void bsp_InitTimer(void);
void bsp_DelayUS(uint32_t n);
void bsp_DelayMS(uint32_t n);
void bsp_StartTimer(uint8_t _id, uint32_t _period);
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period);
void bsp_StopTimer(uint8_t _id);
uint8_t bsp_CheckTimer(uint8_t _id);
int32_t bsp_GetRunTime(void); 
int32_t bsp_GetRunTime10ms(void); 
void bsp_StartTimeUS(uint8_t _id);
int32_t bsp_GetTimeUS(uint8_t _id);

#define delay_us(ud) bsp_DelayUS(ud)
#define delay_ms(md) bsp_DelayMS(md)
   
void beep(uint16_t beep_on,uint16_t mute,uint16_t time);   

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
