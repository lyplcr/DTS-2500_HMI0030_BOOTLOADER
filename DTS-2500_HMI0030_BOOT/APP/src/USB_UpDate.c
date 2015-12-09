/**
  ******************************************************************************
  * @file    USB_UpDate.c
  * @author  SY
  * @version V3.5.0
  * @date    2015-7-6 10:22:37
  * @brief   USB����������
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "USB_UpDate.h"
#include "usbh_bsp_msc.h"


/* Private define ------------------------------------------------------------*/
#define USB_UPDATE_FILE_PATH		"1:/update"
#define USB_UPDATE_FILE_APPEND_PATH	"1:/update/"
#define UPDATE_FONT					"song24.zk"

#define UPDATE_WORD_START_X		20
#define UPDATE_WORD_START_Y		20
#define UPDATE_ROW_DISTANCE		12
#define UPDATE_WORD_SIZE		24
#define DELAY_TIME				500		//��ʾ����ʱʱ��
#define UPDATE_ONCE_READ		512		//��ȡһ��USB�ֽ���




#define COLOR_POINT				WHITE		//ǰ��ɫ
#define COLOR_BACK				BLACK		//����ɫ

/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static char Lfname[(_MAX_LFN * 2) + 1] = {0};
static char Name_buff[50];

/* Private function prototypes -----------------------------------------------*/
BoolStatus Check_USB_UpdateFileExist( void );
ErrorStatus USB_EraseFlashOperation( uint16_t x, uint16_t y );
ErrorStatus USB_UpdateExecute( uint16_t x, uint16_t y );
ErrorStatus USB_ProgramFlashOperation( const uint8_t *data, uint32_t addr, uint16_t len );
UPDATE_STATUS_TypeDef USB_WaitInsert( uint16_t x, uint16_t y );
void USB_EscProcess( void );


/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : USB_UpDate
 * Description    : USB����
 * Input          : None
 * Output         : None
 * Return         : 0: ���� 1��ʧ�� 2���л�������ʽ����
 *------------------------------------------------------------*/
UPDATE_STATUS_TypeDef USB_UpDate( void )
{	
	uint16_t x = UPDATE_WORD_START_X;
	uint16_t y = UPDATE_WORD_START_Y;
	char version[50];
	
	lcd_clear(COLOR_BACK);
	
	lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"========================================",UPDATE_FONT);
	y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"         > ��ӭʹ��U������ϵͳ <",UPDATE_FONT);
	y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	strcpy(version,"         > ");
	strcat(version,SOFT_VERSION);
	lcd_font24(x,y,COLOR_POINT,COLOR_BACK,version,UPDATE_FONT);
	y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"========================================",UPDATE_FONT);
	y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	
	if (USB_WaitInsert(x,y) == UPDATE_ABABDON)
	{
		return UPDATE_ABABDON;
	}
	
	y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	
	lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"> ���ڲ���USB�����ļ�...",UPDATE_FONT);
	bsp_DelayMS(DELAY_TIME);
	
	y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	
	if (Check_USB_UpdateFileExist() == NO)
	{
		lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"> δ�ҵ������ļ���",UPDATE_FONT);
		y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
		lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"> ����ʧ�ܣ�",UPDATE_FONT);
		return UPDATE_ERR;
	}
	
	lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"> ׼��ϵͳ����...",UPDATE_FONT);
	bsp_DelayMS(DELAY_TIME);
	y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	
	if (ERROR == USB_EraseFlashOperation(x,y))
	{
		y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
		lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"> ����flashʧ�ܣ�",UPDATE_FONT);
		y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
		lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"> ����ʧ�ܣ�",UPDATE_FONT);
		return UPDATE_ERR;
	}
	y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	
	lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"> ����д���ļ�...",UPDATE_FONT);
	bsp_DelayMS(DELAY_TIME);
	y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	if (ERROR ==USB_UpdateExecute(x,y))
	{
		return UPDATE_ERR;
	}
	y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	
	lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"> �����ɹ���",UPDATE_FONT);
	y += UPDATE_WORD_SIZE + UPDATE_ROW_DISTANCE;
	
	USB_EscProcess();
	
	AutoEnterSystemCue(x,y,COLOR_POINT,COLOR_BACK,UPDATE_FONT);
	
	return UPDATE_OK;
}

/*------------------------------------------------------------
 * Function Name  : USB_EscProcess
 * Description    : �˳�USB����,ʹUSB�ϵ�ָʾ��Ϩ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void USB_EscProcess( void )
{
	USB_ReadyCycle();
}


/*------------------------------------------------------------
 * Function Name  : USB_WaitInsert
 * Description    : ���USB�Ƿ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
UPDATE_STATUS_TypeDef USB_WaitInsert( uint16_t x, uint16_t y )
{	
	uint8_t status = 0;
	
	while (1)
	{
		USBH_Process(&USB_OTG_Core, &USB_Host);	
		
		switch ( status )
		{			
			case 0:
				if (Get_USB_Status() == SUCCESS)
				{
					lcd_fill(618,450,120,24,COLOR_BACK);
					lcd_fill(x,y,480,24,COLOR_BACK);	
					lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"> U������ȷ���ӣ�",UPDATE_FONT);
					status = 2;
				}
				else
				{
					lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"> δ��⵽U�̣������...",UPDATE_FONT);
					lcd_font24(618,450,COLOR_POINT,DARK_BLUE,"��̫������",UPDATE_FONT);
					status = 1;
				}
				break;
			case 1:
				if (Get_USB_Status() == SUCCESS)
				{
					status = 0;
				}
				else
				{
					if (GetKey() == KEY_F4)
					{
						lcd_clear(COLOR_BACK);

						return UPDATE_ABABDON;
					}
				}
				break;
			case 2:
				bsp_InitUSB();	
			
				return UPDATE_OK;
		}
	}
}


/*------------------------------------------------------------
 * Function Name  : USB_UpdateExecute
 * Description    : ִ��USB����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus USB_UpdateExecute( uint16_t x, uint16_t y )
{
	FIL file_obj;
	FRESULT fresult;
	uint32_t br = 0;
	uint32_t addr = 0;
	uint32_t process = 0;
	uint32_t f_size = 0;
	uint8_t update_buff[UPDATE_ONCE_READ];
	char buff[10];
	char Update_name[50];
	uint8_t last_process = 100;	
	
	strcpy(Update_name,USB_UPDATE_FILE_APPEND_PATH);
	strcat(Update_name,Name_buff);
	
	fresult = f_open(&file_obj,Update_name,FA_OPEN_EXISTING|FA_READ);
	if (fresult != FR_OK)
	{
		f_close(&file_obj);
		lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"> ���ļ�����",UPDATE_FONT);
		return ERROR;
	}
	
	f_size = file_obj.fsize;
	
	do
	{
		fresult = f_read(&file_obj,update_buff,UPDATE_ONCE_READ,&br); 
		
		if ((fresult != FR_OK) || (br > UPDATE_ONCE_READ))
		{
			f_close(&file_obj);
			lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"> ��ȡ�ļ�����",UPDATE_FONT);
			return ERROR;
		}
		
		if (ERROR == USB_ProgramFlashOperation(update_buff,addr,br))
		{
			f_close(&file_obj);
			lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"> д��flash���ݴ���",UPDATE_FONT);
			return ERROR;
		}
		
		process = (float)addr / f_size * 100;		
		if (((process % 5) == 0) && (last_process != process))
		{
			last_process = process;
			lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"-",UPDATE_FONT);
			x += 12;
			usprintf(buff,"%3d%%",process);
			lcd_font24(x,y,COLOR_POINT,COLOR_BACK,buff,UPDATE_FONT);
		}
		
		addr += br;
		
	}while (br != 0);
		
	fresult = f_close(&file_obj);
	
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : USB_ProgramFlashOperation
 * Description    : ���flash����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus USB_ProgramFlashOperation( const uint8_t *data, uint32_t addr, uint16_t len )
{	
	uint32_t WriteData = 0;
	const uint8_t *pos = data;
	uint16_t len_offset = 0;
	uint8_t i = 0;
	uint32_t uwAddress = 0;
	
	if (!len)
	{
		return SUCCESS;
	}
	
	/* Unlock the Flash *********************************************************/
	/* Enable the flash control register access */
	FLASH_Unlock();

	/* Erase the user Flash area ************************************************/
	/* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */

	/* Clear pending flags (if any) */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
				  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

	/* Program the user Flash area word by word ********************************/
	/* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */
	
	len_offset = len % 4;
	len -= len_offset;

	while (uwAddress < len)
	{
		WriteData = (pos[0] | (pos[1]<<8) | (pos[2]<<16) | (pos[3]<<24));
		
		WriteData ^= 0x55555555;
		
		if (FLASH_ProgramWord(USER_FLASH_FIRST_PAGE_ADDRESS + addr + uwAddress, WriteData) == FLASH_COMPLETE)
		{
			if (*(uint32_t*)(USER_FLASH_FIRST_PAGE_ADDRESS + addr + uwAddress) != WriteData)
			{
				FLASH_Lock(); 
				return ERROR;
			}
			 
			uwAddress += 4;
			pos += 4;
		}
		else
		{ 
		  /* Error occurred while writing data in Flash memory. 
			 User can add here some code to deal with this error */
			FLASH_Lock(); 
			return ERROR;
		}
	}
	
	if ( len_offset )
	{
		for (i=0; i<len_offset; ++i)
		{
			if (FLASH_ProgramByte(USER_FLASH_FIRST_PAGE_ADDRESS + addr + uwAddress, *pos) == FLASH_COMPLETE)
			{
				uwAddress++;
				pos++;
			}
			else
			{ 
			  /* Error occurred while writing data in Flash memory. 
				 User can add here some code to deal with this error */
				FLASH_Lock(); 
				return ERROR;
			}
		}
	}

	/* Lock the Flash to disable the flash control register access (recommended
	 to protect the FLASH memory against possible unwanted operation) */
	FLASH_Lock(); 
	
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : USB_EraseFlashOperation
 * Description    : ����flash����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus USB_EraseFlashOperation( uint16_t x, uint16_t y )
{	
	ErrorStatus Err = SUCCESS;
	uint32_t addr = 0;
	const uint8_t ERASE_FLASH_NUM = 3;
	const uint32_t erase_addr[ERASE_FLASH_NUM] = {ADDR_FLASH_SECTOR_5,ADDR_FLASH_SECTOR_6,ADDR_FLASH_SECTOR_7};
	uint8_t i = 0;
	char buff[10];
	uint8_t process = 0;

	lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"> ���ڲ���flash...",UPDATE_FONT);
	x += 216;
		
	/* Enable the flash control register access */
	FLASH_Unlock();

	/* Erase the user Flash area ************************************************/
	/* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */

	/* Clear pending flags (if any) */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
				  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
	for (i=0; i<ERASE_FLASH_NUM; ++i)
	{
		/* Get the number of the start and end sectors */
		addr = GetSector(erase_addr[i]);

		/* Device voltage range supposed to be [2.7V to 3.6V], the operation will
		   be done by word */ 
		if (FLASH_EraseSector(addr, VoltageRange_3) != FLASH_COMPLETE)
		{ 
		  /* Error occurred while sector erase. 
			 User can add here some code to deal with this error  */
			Err = ERROR;
			break;
		}
		
		process += 30;
		usprintf(buff,"%3d%%",process);
		lcd_font24(x,y,COLOR_POINT,COLOR_BACK,buff,UPDATE_FONT);
	}
	
	bsp_DelayMS(DELAY_TIME);
	lcd_font24(x,y,COLOR_POINT,COLOR_BACK,"100%",UPDATE_FONT);
	
	FLASH_Lock(); 
	
	return Err;
}

/*------------------------------------------------------------
 * Function Name  : Get_USB_UpdateFileName
 * Description    : ��ȡUSB�ļ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus Check_USB_UpdateFileExist( void )
{
	DIR dir_object;
	FRESULT fresult;
	static FILINFO file_info;
	
	#if _USE_LFN
         file_info.lfname = Lfname;
         file_info.lfsize = sizeof(Lfname);
    #endif
	
	fresult = f_opendir(&dir_object,USB_UPDATE_FILE_PATH);
	
	if (fresult != FR_OK)
	{
		return NO;
	}
	
	for (;;)
	{
		fresult = f_readdir(&dir_object,&file_info);
		
		if (fresult != FR_OK)
		{
			return NO;
		}
		
		if (file_info.fname[0] == NULL)
		{
			return NO;
		}
		
		if (file_info.fattrib & AM_ARC)
		{
			if (file_info.lfname[0] == NULL)	//���ļ���
			{
				strcpy(Name_buff,file_info.fname);
			}
			else
			{
				strcpy(Name_buff,file_info.lfname);
			}
			
			return YES;
		}
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
