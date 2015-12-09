/**
  ******************************************************************************
  * @file    init.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   ��ʼ��
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "initial.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT							WHITE
#define	COLOR_BACK							BLACK

/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static void GPIO_Configuration( void );
static void NVIC_Configuration( void );

/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : InitSystem
 * Description    : ��ʼ��ϵͳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void InitSystem( void )
{
	NVIC_Configuration();		/* �������ȼ�����    */
	GPIO_Configuration();		/* ����GPIO 		*/	
	
	BSP_InitPort();				/* �ײ�������ʼ�� 	*/
}

 /*------------------------------------------------------------
 * Function Name  : GPIO_Configuration
 * Description    : GPIO����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GPIO_Configuration( void )
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | \
						   RCC_AHB1Periph_GPIOC	| RCC_AHB1Periph_GPIOD | \
						   RCC_AHB1Periph_GPIOE ,ENABLE);
}

/*------------------------------------------------------------
 * Function Name  : NVIC_Configuration
 * Description    : �ж����ȼ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void NVIC_Configuration( void )
{
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0X20000);
	
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

/*------------------------------------------------------------
 * Function Name  : HardFault_Handler
 * Description    : Ӳ���жϴ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void HardFault_Handler(void)
{
	lcd_fill(300,200,200,100,RED);
	
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
