/**
  ******************************************************************************
  * @file    main.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   主程序
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "initial.h"
#include "USB_UpDate.h"
#include "SD_UpDate.h"
#include "tftpserver.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	USB_UPDATE = 0,
	SD_UPDATE,
	UDP_UPDATE,
}UPDATE_TYPE_TypeDef;

typedef  void (*pFunction)(void);

/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction Jump_To_Application;

/* Private function prototypes -----------------------------------------------*/
static void CloseAllInterrupt( void );
static TestStatus ConfirmAutoUpdatePassword( void );


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
	
	InitSystem();	
	
	bsp_StartTimer(0,100);
	
	while ( !bsp_CheckTimer(0))
	{
		if (GetKey() != KEY_OIL_RUN)
		{
			EnBootLoad = DISABLE;
			break;
		}
	}
	
	if (ConfirmAutoUpdatePassword() == PASSED)
	{
		EnBootLoad = ENABLE;
	}
	
	if (EnBootLoad == ENABLE)
	{
		BSP_InitAll();
		
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
 * Function Name  : CloseAllInterrupt
 * Description    : 关闭所有中断
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CloseAllInterrupt( void )
{
	CLI();
	
	bsp_DeInitTimer();	
}

/*------------------------------------------------------------
 * Function Name  : ConfirmAutoUpdatePassword
 * Description    : 验证自动升级密码
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus ConfirmAutoUpdatePassword( void )
{
	uint32_t *pAutoBootAddr = (uint32_t *)AUTO_LOAD_BOOT_ADDR;
	
	if (*pAutoBootAddr == PRIVATE_PASSWORD)
	{
		if (FAILED == WriteFlashWord(AUTO_LOAD_BOOT_ADDR,0xFFFFFFFF))
		{
			return FAILED;
		}
		
		return PASSED;
	}
	
	return FAILED;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


