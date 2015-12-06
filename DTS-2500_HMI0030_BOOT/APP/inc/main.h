/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include "flash.h"

/* Exported define -----------------------------------------------------------*/
#define USE_IAP_TFTP   /* enable IAP using TFTP */
//#define USE_IAP_HTTP   /* enable IAP using HTTP */
//#define USE_LCD        /* enable LCD  */  
//#define USE_DHCP       /* enable DHCP, if disabled static address is used */

#define USER_FLASH_FIRST_PAGE_ADDRESS ADDR_FLASH_SECTOR_5 /* Only as example see comment */
#define USER_FLASH_LAST_PAGE_ADDRESS  ADDR_FLASH_SECTOR_7
#define USER_FLASH_END_ADDRESS        (ADDR_FLASH_SECTOR_8 - 1)

/* MAC Address definition *****************************************************/
#define MAC_ADDR0   0
#define MAC_ADDR1   0
#define MAC_ADDR2   0
#define MAC_ADDR3   0 
#define MAC_ADDR4   0
#define MAC_ADDR5   2

/* Static IP Address definition ***********************************************/
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   0
#define IP_ADDR3   111
   
/* NETMASK definition *********************************************************/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/* Gateway Address definition *************************************************/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   0
#define GW_ADDR3   1 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
ErrorStatus SD_UpDate( void );
void AutoEnterSystemCue( uint16_t x, uint16_t y, uint16_t pc, uint16_t bc, char *font );

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
