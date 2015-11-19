/**
  ******************************************************************************
  * @file    main.c
  * @author  SY
  * @version V3.5.0
  * @date    2015-6-17 15:33:51
  * @brief   ������
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
 * Description    : �������
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
	if (((*(__IO uint32_t*)USER_FLASH_FIRST_PAGE_ADDRESS) & 0x2FFC0000 ) == 0x20000000)   /*ͨ���ж�FLASH��һ�����Ƿ���䵽��ȷ��ջ����ַ������������Ӧ�ó���*/
	{	
		/* ������Ϲ��жϣ������³����ж�ʱ�����Ҳ����жϷ��������ܷɣ� */		
		CloseAllInterrupt();
		
		/* Jump to user application */
		JumpAddress = *(__IO uint32_t*) (USER_FLASH_FIRST_PAGE_ADDRESS + 4);
		Jump_To_Application = (pFunction) JumpAddress;
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);

 		/*�����ж�������ƫ����*/
 		NVIC_SetVectorTable(NVIC_VectTab_FLASH, (USER_FLASH_FIRST_PAGE_ADDRESS-0x08000000));	
		
		 /*��ת��Ӧ�ó���*/
		Jump_To_Application();
	}
	else
	{
		lcd_font24(350,240,RED,BLACK,"ϵͳ�ļ���ʧ��","song24.zk");
		
		while(1);
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemSoftwareReset
 * Description    : ϵͳ��λ
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
 * Description    : �ر������ж�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void CloseAllInterrupt( void )
{
	//�رն�ʱ���ж�
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE); 
	
// 	//�ر�USB�ж�
// 	USB_OTG_BSP_DisableInterrupt();
// 	
// 	//�ر�SD���ж�
// 	Close_SDIO_Interrupts();
// 	
// 	//�ر���̫���ж�
// 	ETHERNET_CloseInterrupt();
}

/*------------------------------------------------------------
 * Function Name  : Section_Init
 * Description    : �ֲ���ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void Section_Init( void )
{	
	NVIC_Configuration();		/* �������ȼ�����    */
	bsp_InitTimer();			/* ��ʱ����ʼ�� 		*/
	key_init();					/* ������ʼ�� 		*/
	delay_ms(10);				/* �ȴ�����׼�����  */	
}

/*------------------------------------------------------------
 * Function Name  : All_Init
 * Description    : ȫ�ֳ�ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void All_Init( void )
{
	lcd_init();					/* LCD��ʼ��			*/
	font_init();				/* �����ʼ�� 		*/
	USB_Init();					/* USB��ʼ��			*/
 	Ethernet_Init();			/* ��̫����ʼ��  	*/
}

/*------------------------------------------------------------
 * Function Name  : NVIC_Configuration
 * Description    : �ж����ȼ��������
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
 * Description    : �Զ�����ϵͳ��ʾ
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
				lcd_font24(x,y,pc,bc,"> 3 ���Ӻ��Զ�����ϵͳ...",font);
				break;
			case 1:
				lcd_font24(x,y,pc,bc,"> 2 ���Ӻ��Զ�����ϵͳ...",font);
				break;
			case 2:
				lcd_font24(x,y,pc,bc,"> 1 ���Ӻ��Զ�����ϵͳ...",font);
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
//	lcd_font24(350,240,RED,BLACK,"ϵͳ�����ˣ�","song24.zk");
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

