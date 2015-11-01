#ifndef __MA_DEBUG_H
#define __MA_DEBUG_H

//#define __DEBUG__ 

#ifdef __DEBUG__  
#define DEBUG(format,...) printf("File: "__FILE__", Line: %05d: "format"\r\n", __LINE__, ##__VA_ARGS__)  
#else  
#define DEBUG(format,...)	(void)0  
#endif 

#endif
