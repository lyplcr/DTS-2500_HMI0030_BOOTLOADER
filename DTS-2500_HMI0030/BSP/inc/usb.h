/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_H
#define __USB_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported define -----------------------------------------------------------*/
#define USB_STATUS_CYCLE_NUM				3000		//USBѭ�������ﵽ�˴������ܲ���
#define USB_LEAVE_SYNC_NUM					200			//USB�뿪ͬ������

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void USB_Init( void );


#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
