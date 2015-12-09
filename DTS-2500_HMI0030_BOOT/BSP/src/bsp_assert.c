/**
  ******************************************************************************
  * @file    bsp_assert.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   ����
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"	
#include <stdio.h>

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* 
	ST�⺯��ʹ����C�������Ķ��Թ��ܣ����������USE_FULL_ASSERT����ô���е�ST�⺯������麯���β�
	�Ƿ���ȷ���������ȷ������ assert_failed() ���������������һ����ѭ���������û������롣
	
	�ؼ��� __LINE__ ��ʾԴ�����кš�
	�ؼ���__FILE__��ʾԴ�����ļ�����
	
	���Թ���ʹ�ܺ���������С���Ƽ��û����ڵ���ʱʹ�ܣ�����ʽ��������ǽ�ֹ��

	�û�����ѡ���Ƿ�ʹ��ST�̼���Ķ��Թ��ܡ�ʹ�ܶ��Եķ��������֣�
	(1) ��C��������Ԥ�����ѡ���ж���USE_FULL_ASSERT��
	(2) �ڱ��ļ�ȡ��"#define USE_FULL_ASSERT    1"�е�ע�͡�	
*/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number*/
     printf("Wrong parameters value: file %s on line %d\r\n", file, line);

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
