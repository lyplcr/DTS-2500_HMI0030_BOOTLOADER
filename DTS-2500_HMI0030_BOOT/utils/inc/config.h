/****************************************Copyright (c)**************************************************
**                                 杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: config.h
** 最后修改日期: 2014/5/5 18:17:17
** 描        述: 宏定义
** 版	     本: V1.0
** 主  控  芯 片:STM32F103  
** IDE:MDK4.12
**********************************************************************************************************/
	
#ifndef __CONFIG_H
#define	__CONFIG_H	

#define 	USER_PROGRAM_START_ADDR	0x08020000							//应用程序起始地址
#define		SMPL_NUM				6									//传感器通道数	  
#define		CTRL_CHN				2   								//控制通道数	

#define		AUTO_LOAD_BOOT_ADDR		0x0801FFFC							//自动进入BootLoader地址
#define		PRIVATE_PASSWORD		0x2F8E4A7D							//特权访问密码
#define		SUPER_PASSWORD			"88320524"							//管理员密码

#define		VALVE_RESET_DEGREE		-6400								//阀口复位开度
#define 	CTRL_FREQ				50									//控制频率
#define 	CTRL_PERIOD				(1000/CTRL_FREQ)					//控制周期

#define		SYSTEM_MODEL			"DTS-2500(屏显)"					//系统型号
#define		SOFT_VERSION			"1.0.0"								//上位机软件版本	
#define		PCB_VERSION				"HMI0030"							//上位机PCB版本


/*
	软件版本规则：
	（1）主版本号：当功能模块有较大的变动，比如增加模块或是整体架构发生变化。
	（2）次版本号：相对于主版本号而言，次版本号的升级对应的只是局部的变动，
		 但该局部的变动造成程序和以前版本不能兼容，或者对该程序以前的协作关系产生了破坏，
		 或者是功能上有大的改进或增强。
	（3）修订版本号：一般是Bug的修复或是一些小的变动或是一些功能的扩充，要经常发布修订版，
		 修复一个严重 Bug 即可发布一个修订版。
*/

#define CLI()      __disable_irq()
#define SEI()      __enable_irq()
 
#endif

 


