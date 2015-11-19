/****************************************Copyright (c)**************************************************
**                                 �����θ߿Ƽ����޹�˾
**                                     
**                                
** ��   ��   ��: config.h
** ����޸�����: 2014/5/5 18:17:17
** ��        ��: �궨��
** ��	     ��: V1.0
** ��  ��  о Ƭ:STM32F103  
** IDE:MDK4.12
**********************************************************************************************************/
	
#ifndef __CONFIG_H
#define	__CONFIG_H	

#include "stm32f4xx.h"

 		
#define		SMPL_NUM				6									//������ͨ����	  
#define		CTRL_CHN				2   								//����ͨ����	
#define		PRIVATE_PASSWORD		0x2F8E4A7D							//��Ȩ��������
#define		Super_Password			"88320524"							//����Ա����
#define		TIME_PASSWARD_PUTIN		10									//��������ȴ�ʱ�䣨���ӣ�
#define		VALVE_RESET_DEGREE		-6400								//���ڸ�λ����

#define		SYSTEM_VERSION			"DTS-2500"							//ϵͳ�汾
#define		PCB_DISP_VERSION		"HMI0030"							//��λ��PCB�汾
#define		SOFT_DISP_VERSION		"V1.2.1"							//��λ������汾	
/*
	����汾����
	��1�����汾�ţ�������ģ���нϴ�ı䶯����������ģ���������ܹ������仯��
	��2���ΰ汾�ţ���������汾�Ŷ��ԣ��ΰ汾�ŵ�������Ӧ��ֻ�Ǿֲ��ı䶯��
		 ���þֲ��ı䶯��ɳ������ǰ�汾���ܼ��ݣ����߶Ըó�����ǰ��Э����ϵ�������ƻ���
		 �����ǹ������д�ĸĽ�����ǿ��
	��3���޶��汾�ţ�һ����Bug���޸�����һЩС�ı䶯����һЩ���ܵ����䣬Ҫ���������޶��棬
		 �޸�һ������ Bug ���ɷ���һ���޶��档
*/


//#define		ENABLE_BEEP													//ʹ�ܷ�����



//��������
typedef enum 
{
	BAUD_9600=9600,					//9600�Ĳ�����
	BAUD_19200=19200,				//19200�Ĳ�����
	BAUD_38400=38400ul,				//38400�Ĳ�����
	BAUD_57600=57600ul,				//57600�Ĳ�����
	BAUD_115200=115200ul,			//115200�Ĳ�����
}BAUD_RATE_TypeDef; 


//���ط���
typedef enum
{ 
	UP_DIRE = 0x00,		//���̼���
	DW_DIRE = 0x01		//�س̼���
}LOAD_DIRE_TypeDef;  


//����״̬ö��
typedef enum
{
	STATUS_STEP_INIT=0x00,		//�����ʼ���׶�
	STATUS_STEP_LOAD,			//������ؽ׶�
	STATUS_STEP_TRANSITION,		//���ص����ֹ��ɽ׶�
	STATUS_STEP_HOLD,			//���ֽ׶�
}STEP_STATUS_TypeDef;
 
typedef enum
{
	EXE_UP = 0x00,				//��������Ϊ���̷���
	EXE_DW = 0x01				//��������Ϊ���̷���
}EXE_DIRE_TypeDef;

//������ͨ������
typedef enum
{ 
	SMPL_FH_NUM = 0x00,			//����ͨ�� 
	SMPL_WY_NUM = 0x01,			//λ��ͨ��
	SMPL_BX_NUM = 0x02,			//����ͨ��
	SMPL_EX1_NUM = 0x03,		//��չ1ͨ��
	SMPL_ECD1_NUM = 0x04,		//��������1
	SMPL_ECD2_NUM = 0x05,		//��������2
}SMPL_NAME_TypeDef; 

typedef enum
{ 
	SMPL_KY_NUM = 0x00,		//��ѹͨ�� 
	SMPL_KZ_NUM = 0x01		//����ͨ��
}SMPL_NAME_TypeDef2;   
				
		
		
//�������1024�ֽ�Ϊһ���飬��λ��������200�飬PCM������204�飬PRV������210��									
#define		PCM_DATA_BASE			(4<<10)
#define		PRV_DATA_BASE			(10<<10)
						

#pragma pack(1)

typedef struct
{
	//�����йصĲ���
	uint8_t		load_type;					//���ط�ʽ
	uint8_t		load_dir;					//���ط��򣺼��صķ������豸�жϡ�O��,����ǿ������P��������ǿ�Ƹ���N��
	float		load_speed;					//�����ٶ�
	//�ж��йصĲ���
	uint8_t		target_type;				//Ŀ�귽ʽ
	uint8_t		target_prop;				//Ŀ�����ԣ�A��Ŀ��ʾֵ��/R������ʾֵ��
	float 		target_value;				//Ŀ��ֵ
	uint8_t		hold_type;					//���ַ�ʽ
	uint32_t	hold_time;					//����ʱ��
	//ִ���йصĲ���
	uint8_t		load_level;					//���輶��	W���ȴ����衿/I���������衿
	uint8_t 	rsv[8];						//����8���ֽ�
}STEP_TypeDef;

#pragma pack()
 
#endif

 


