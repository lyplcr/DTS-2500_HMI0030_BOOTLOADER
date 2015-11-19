/**
  ******************************************************************************
  * @file    main.c
  * @author  SY
  * @version V3.5.0
  * @date    2015-6-17 15:33:51
  * @brief   主函数
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "key.h"
#include "timer.h"
#include <stdio.h>
#include "string.h"
#include "timer.h"
#include "lcd.h"
#include "usb.h"
#include "date.h"
#include "sd_update.h"
#include "usb_update.h"
#include "usb_bsp.h"
#include "sdio_sd.h"
#include "ra8875.h"
#include "lan8720.h"
#include "netconf.h"
#include "tftpserver.h"

/* Private define ------------------------------------------------------------*/
typedef  void (*pFunction)(void);



/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	USB_UPDATE = 0,
	SD_UPDATE,
	UDP_UPDATE,
}UPDATE_TYPE_TypeDef;

/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction Jump_To_Application;


/* Private function prototypes -----------------------------------------------*/
void BootLink( void );
void All_Init( void );
void NVIC_Configuration( void );
void CloseAllInterrupt( void );
void Section_Init( void );
void SystemSoftwareReset( void );


/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : main
 * Description    : 程序入口
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
int main( void )
{			
	uint32_t JumpAddress = 0;
	FunctionalState EnBootLoad = ENABLE;
	UPDATE_TYPE_TypeDef UpdateType = USB_UPDATE;
	FunctionalState BootOK = DISABLE;
	
	Section_Init();
	
	bsp_StartTimer(0,100);
	
	while ( !bsp_CheckTimer(0))
	{
		if (get_key(0x00000000) != KEY_RUN)
		{
			EnBootLoad = DISABLE;
			break;
		}
	}
	
	if (EnBootLoad == ENABLE)
	{
		All_Init();
		
		while (1)
		{
			switch ( UpdateType )
			{
				case USB_UPDATE:
					switch ( USB_UpDate() )
					{
						case UPDATE_OK:
							BootOK = ENABLE;
							break;
						case UPDATE_ERR:
							while (1);
						case UPDATE_ABABDON:
							UpdateType = UDP_UPDATE;
							break;
					}
					break;
				case SD_UPDATE:
					switch ( SD_UpDate() )
					{
						case SUCCESS:
							BootOK = ENABLE;
							break;
						case ERROR:
							while(1);
					}
					break;
				case UDP_UPDATE:
					UDP_Update();
					BootOK = ENABLE;
					break;
			}
			
			if (BootOK == ENABLE)
			{
				SystemSoftwareReset();
			}
		}	
	}
	
	/* Check if valid stack address (RAM address) then jump to user application */
	if (((*(__IO uint32_t*)USER_FLASH_FIRST_PAGE_ADDRESS) & 0x2FFC0000 ) == 0x20000000)   /*通过判断FLASH第一个字是否分配到正确的栈顶地址，来区分有无应用程序*/
	{	
		/* 升级完毕关中断，否则导致程序中断时，因找不到中断服务程序而跑飞！ */		
		CloseAllInterrupt();
		
		/* Jump to user application */
		JumpAddress = *(__IO uint32_t*) (USER_FLASH_FIRST_PAGE_ADDRESS + 4);
		Jump_To_Application = (pFunction) JumpAddress;
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);

 		/*设置中断向量表偏移量*/
 		NVIC_SetVectorTable(NVIC_VectTab_FLASH, (USER_FLASH_FIRST_PAGE_ADDRESS-0x08000000));	
		
		 /*跳转到应用程序*/
		Jump_To_Application();
	}
	else
	{
		lcd_font24(350,240,RED,BLACK,"系统文件丢失！","song24.zk");
		
		while(1);
	}
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

/*------------------------------------------------------------
 * Function Name  : CloseAllInterrupt
 * Description    : 关闭所有中断
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void CloseAllInterrupt( void )
{
	//关闭定时器中断
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE); 
	
// 	//关闭USB中断
// 	USB_OTG_BSP_DisableInterrupt();
// 	
// 	//关闭SD卡中断
// 	Close_SDIO_Interrupts();
// 	
// 	//关闭以太网中断
// 	ETHERNET_CloseInterrupt();
}

/*------------------------------------------------------------
 * Function Name  : Section_Init
 * Description    : 局部初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void Section_Init( void )
{	
	NVIC_Configuration();		/* 配置优先级分组    */
	bsp_InitTimer();			/* 定时器初始化 		*/
	key_init();					/* 按键初始化 		*/
	delay_ms(10);				/* 等待按键准备完毕  */	
}

/*------------------------------------------------------------
 * Function Name  : All_Init
 * Description    : 全局初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void All_Init( void )
{
	lcd_init();					/* LCD初始化			*/
	font_init();				/* 字体初始化 		*/
	USB_Init();					/* USB初始化			*/
 	Ethernet_Init();			/* 以太网初始化  	*/
}

/*------------------------------------------------------------
 * Function Name  : NVIC_Configuration
 * Description    : 中断优先级分组管理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void NVIC_Configuration( void )
{
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

/*------------------------------------------------------------
 * Function Name  : AutoEnterSystemCue
 * Description    : 自动进入系统提示
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void AutoEnterSystemCue( uint16_t x, uint16_t y, uint16_t pc, uint16_t bc, char *font )
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


void HardFault_Handler(void)
{
	lcd_fill(300,200,500,300,BLACK);
//	lcd_font24(350,240,RED,BLACK,"系统崩溃了！","song24.zk");
	lcd_ascin24s(350,240,RED,BLACK,"System Error!");
	
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}


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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

