/**
  ******************************************************************************
  * @file    usb.c
  * @author  SY
  * @version V3.5.0
  * @date    2015-7-3 15:38:39
  * @brief   USB文件
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usb.h"
#include "diskio.h"
#include "ff.h"
#include "usbh_bsp_msc.h"

/* Private define ------------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static FATFS fs;            // Work area (file system object) for logical drive 

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : USB_Init
 * Description    : USB初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void USB_Init( void )
{		
	uint32_t num = 0;
	
	  /* Init Host Library */
	#ifdef USE_USB_OTG_FS
		USBH_Init(&USB_OTG_Core,
			USB_OTG_FS_CORE_ID,
            &USB_Host,
            &USBH_MSC_cb,
            &USR_cb);
	#else
		USBH_Init(&USB_OTG_Core,
			USB_OTG_HS_CORE_ID,
            &USB_Host,
            &USBH_MSC_cb,
            &USR_cb);
	#endif

	while (num < USB_STATUS_CYCLE_NUM)
	{
		num++;
		USBH_Process(&USB_OTG_Core, &USB_Host);		//必须执行一定次数才能操作USB
	}
	
	f_mount(&fs,"1:/",1);	/* 注册设备 */
	
	num = 0;
	while (num < USB_LEAVE_SYNC_NUM)
	{
		num++;
		USBH_Process(&USB_OTG_Core, &USB_Host);		//必须执行一定次数才能操作USB
	}
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
