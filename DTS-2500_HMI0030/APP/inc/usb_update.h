/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_UPDATE_H
#define __USB_UPDATE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	UPDATE_OK = 0,
	UPDATE_ERR,
	UPDATE_ABABDON,	//其他升级方式
}UPDATE_STATUS_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
UPDATE_STATUS_TypeDef USB_UpDate( void );


#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
