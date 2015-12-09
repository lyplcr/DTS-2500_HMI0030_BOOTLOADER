/**
  ******************************************************************************
  * @file    bsp.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   底层驱动文件
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "netconf.h"
#include "usb.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : BSP_InitPort
 * Description    : 初始化部分驱动
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void BSP_InitPort( void )
{	
  	bsp_InitTimer();			/* 初始化定时器 		*/
	bsp_InitIO();				/* 初始化IO			*/
	bsp_InitCOM();				/* 初始化串口 		*/
}

/*------------------------------------------------------------
 * Function Name  : BSP_InitAll
 * Description    : 初始化所有驱动
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void BSP_InitAll( void )
{	
	bsp_InitPcf8563();			/* 初始化时钟 		*/
	bsp_InitSFlash();			/* 初始化FLASH 		*/
	bsp_InitEthernet();			/* 初始化以太网		*/
	bsp_InitUSB();				/* 初始化USB			*/
	bsp_InitLCD();				/* 初始化LCD			*/
	bsp_InitFont();				/* 初始化字体		*/
	
	#ifdef ENABLE_BEEP
		BEEP_START();
	#endif
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
