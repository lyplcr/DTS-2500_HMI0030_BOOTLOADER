/**
  ******************************************************************************
  * @file    global.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-15 09:58:52
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   ȫ���ļ�
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"


/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : AutoEnterSystemCue
 * Description    : �Զ�����ϵͳ��ʾ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void AutoEnterSystemCue( uint16_t x, uint16_t y, uint16_t pc, uint16_t bc, const char *font )
{
	uint8_t i;
	
	bsp_StartTimer(0,1000);
	for (i=0; i<3; ++i)
	{
		switch ( i )
		{
			case 0:
				lcd_font24(x,y,pc,bc,"> 3 ���Ӻ��Զ�����ϵͳ...",font);
				break;
			case 1:
				lcd_font24(x,y,pc,bc,"> 2 ���Ӻ��Զ�����ϵͳ...",font);
				break;
			case 2:
				lcd_font24(x,y,pc,bc,"> 1 ���Ӻ��Զ�����ϵͳ...",font);
				break;
		}
			
		while (!bsp_CheckTimer(0));
		bsp_StartTimer(0,1000);				
	}
	
	lcd_clear(BLACK);
}

/*------------------------------------------------------------
 * Function Name  : SystemSoftwareReset
 * Description    : ϵͳ��λ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SystemSoftwareReset( void )
{	
	__set_FAULTMASK(1);
	NVIC_SystemReset();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
