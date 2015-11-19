#ifndef __LAN8720_H
#define __LAN8720_H

#include "stm32f4x7_eth.h"
			
#define LAN8720_PHY_ADDRESS  	0x00				//LAN8720 PHYоƬ��ַ.
#define LAN8720_RST 		   	PAout(0) 			//LAN8720��λ����	 

#define LAN8720_CLR     GPIO_ResetBits(GPIOA,GPIO_Pin_0);
#define LAN8720_SET     GPIO_SetBits(GPIOA,GPIO_Pin_0);

extern ETH_DMADESCTypeDef  *DMATxDescToSet;			//DMA����������׷��ָ��
extern ETH_DMADESCTypeDef  *DMARxDescToGet; 		//DMA����������׷��ָ�� 

u8 LAN8720_Init(void);
void ETHERNET_NVICConfiguration(void);
u8 ETH_MACDMA_Config(void);
void ETHERNET_CloseInterrupt(void);

#endif 

