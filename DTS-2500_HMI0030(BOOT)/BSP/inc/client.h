/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CLIENT_H
#define __CLIENT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void UDP_SendStr(void *str_p, uint16_t len);
void * GetDataStartAddrFromEthernet( void );
void UDP_SetRxDataHandlerStatus( BoolStatus NewStatus );
uint16_t UDP_GetDataLen( void );
BoolStatus UDP_GetRxDataHandlerStatus( void );

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
