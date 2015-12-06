#ifndef __LAN8720_H
#define __LAN8720_H

#include "stm32f4x7_eth.h"
			
#define LAN8720_PHY_ADDRESS  	0x00				//LAN8720 PHY芯片地址.
#define LAN8720_RST 		   	PAout(0) 			//LAN8720复位引脚	 

#define LAN8720_CLR     GPIO_ResetBits(GPIOA,GPIO_Pin_0);
#define LAN8720_SET     GPIO_SetBits(GPIOA,GPIO_Pin_0);

extern ETH_DMADESCTypeDef  *DMATxDescToSet;			//DMA发送描述符追踪指针
extern ETH_DMADESCTypeDef  *DMARxDescToGet; 		//DMA接收描述符追踪指针 

u8 LAN8720_Init(void);
void ETHERNET_NVICConfiguration(void);
u8 ETH_MACDMA_Config(void);
void ETHERNET_CloseInterrupt(void);

#endif 

