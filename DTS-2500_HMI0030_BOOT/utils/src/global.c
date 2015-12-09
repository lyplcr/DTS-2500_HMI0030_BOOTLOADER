/**
  ******************************************************************************
  * @file    global.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-15 09:58:52
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   全局文件
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
 * Description    : 自动进入系统提示
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
				lcd_font24(x,y,pc,bc,"> 3 秒钟后自动进入系统...",font);
				break;
			case 1:
				lcd_font24(x,y,pc,bc,"> 2 秒钟后自动进入系统...",font);
				break;
			case 2:
				lcd_font24(x,y,pc,bc,"> 1 秒钟后自动进入系统...",font);
				break;
		}
			
		while (!bsp_CheckTimer(0));
		bsp_StartTimer(0,1000);				
	}
	
	lcd_clear(BLACK);
}

/*------------------------------------------------------------
 * Function Name  : SystemSoftwareReset
 * Description    : 系统软复位
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
