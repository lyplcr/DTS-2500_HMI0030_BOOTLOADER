/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_H
#define __USB_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported define -----------------------------------------------------------*/
#define USB_STATUS_CYCLE_NUM				3000		//USB循环体必须达到此次数才能操作
#define USB_LEAVE_SYNC_NUM					200			//USB离开同步次数

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void USB_Init( void );


#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
