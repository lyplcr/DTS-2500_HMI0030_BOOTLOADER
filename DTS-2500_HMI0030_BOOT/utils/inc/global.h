/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLOBAL_H
#define __GLOBAL_H

/* Includes ------------------------------------------------------------------*/
#include "MY_DEBUG.H"
#include "bsp.h"
#include "usb.h"
#include "netconf.h"


/* Exported define -----------------------------------------------------------*/
#define USE_IAP_TFTP   /* enable IAP using TFTP */
//#define USE_IAP_HTTP   /* enable IAP using HTTP */
//#define USE_LCD        /* enable LCD  */  
//#define USE_DHCP       /* enable DHCP, if disabled static address is used */

#define USER_FLASH_FIRST_PAGE_ADDRESS ADDR_FLASH_SECTOR_5 /* Only as example see comment */
#define USER_FLASH_LAST_PAGE_ADDRESS  ADDR_FLASH_SECTOR_7
#define USER_FLASH_END_ADDRESS        (ADDR_FLASH_SECTOR_8 - 1)

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */
void AutoEnterSystemCue( uint16_t x, uint16_t y, uint16_t pc, uint16_t bc, const char *font );
void SystemSoftwareReset( void );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
