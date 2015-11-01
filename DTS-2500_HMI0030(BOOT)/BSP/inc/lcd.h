/****************************************Copyright (c)**************************************************
**                                杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: lcd.h
** 最后修改日期: 2014/4/24 8:45:57
** 描        述: lcd驱动程序
** 版	     本: V1.0
** 主  控  芯 片:  晶振频率: 
** IDE:  
**********************************************************************************************************/
#ifndef __LCD_H
#define	__LCD_H
  
#include "ff.h"
#include "stdint.h"
#include "stm32f4xx.h"
 
//LCD颜色定义
#define WHITE         	  0xFFFF				//白色
#define BLACK         	  0x0000	  			//黑色
#define BLUE         	  0x001F  				//蓝色
#define BRED              0XF81F
#define GRED 			  0XFFE0
#define GBLUE			  0X07FF
#define ORANGE			  0XBD20				//橘黄色
#define RED           	  0xF800				//红色
#define CRIMSON			  0xD8A7				//猩猩红
#define MAGENTA       	  0xF81F				//紫红色的
#define JOHNNY			  0X895C				//紫罗兰色
#define INDIGOO			  0X4810				//靛青/紫兰色
#define GREEN         	  0x07E0				//绿色
#define DARK_BLUE		  0x0174				//宝蓝色
#define CYAN          	  0x7FFF				//青色
#define INDIGO			  0x45D4				//靛青
#define YELLOW        	  0xFFE0				//黄色
#define BROWN 			  0XBC40 				//棕色
#define BRRED 			  0XFC07 				//棕红色
#define GRAY  			  0X8430 				//灰色
#define DARKBLUE      	  0X01CF				//深蓝色
#define LIGHTBLUE      	  0X7D7C				//浅蓝色
#define MIDDLEBLUE		  0X047A				//中蓝色
#define GRAYBLUE       	  0X5458 				//灰蓝色 
#define LIGHTGREEN     	  0X841F 				//浅绿色
#define LGRAY 			  0XC618 				//浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE         0XA651 				//浅灰蓝色(中间层颜色)
#define LBBLUE            0X2B12 				//浅棕蓝色(选择条目的反色)
#define CREAM_YELLOW	  0XFFDE				//米黄色
#define ELEPH_BLACK		  0X2924				//象牙黑
#define CHOCO			  0xD343   				//巧克力色
#define COLD_ASH		  0X8450				//冷灰
#define DEEP_SKY		  0X05FF				//天蓝色
#define INDIAN_RED		  0XCAEB				//印度红
#define FOREST_GREEN	  0X2444				//森林绿
#define ORANGE_RED		  0XFAE0				//橘红色
#define DarkSlateGray	  0X2A69				//墨绿色
#define DARK_BLACK		  0X3186				//暗黑色
#define LIGHT_BLACK		  0X632C				//亮灰色
#define WIN_CAPTION_UPC	  0X7D7C				//窗口CAPTION上部分填充色
#define WIN_CAPTION_DOWNC 0X5458				//窗口CAPTION下部分填充色
#define	EASY_GREEN		  0X3287				//浅绿色
#define EASY_GRAY		  0X3186				//暗灰色(酷灰)
#define WHITE1 			  0XD69A				//灰白色
#define GRAY_GREEN		  0X2726				//暗绿

#define LITTLE_BLUE		  0X3D3B				//浅蓝
#define LIGHT_BLUE		  0X5E3C				//淡蓝
#define LIGHT_GRAY		  0xEF5D				//浅灰(235 235 235) 
#define LIGHT_GRAY1		  0XCE5D				//浅灰(200 200 232)
#define LIGHT_GRAY2		  0XA514				//浅灰(160 160 160)
#define LIGHT_GRAY3		  0X6B4D				//浅灰(105 105 105)
#define LIGHT_GRAY4		  0XDEFB				//浅灰(220 220 220)
#define LIGHT_GREEN		  0XCFFF				//淡绿色
#define LIGHT_BROWN		  0X2160				//浅棕
#define LIGHTLITTLE_BLUE  0XDF7E				//淡淡蓝
#define MIDDLE_BULU		  0X1CBC				//中蓝
#define LITTLE_WHITE	  0XEF7D				//浅白
#define LITTLE_WHITE1	  0XE71C				//浅白1

#define GREEN_LITTLE1	  0x01E5				//浅绿
#define BROWN_LITTLE1	  0x31E0				//浅棕

#define APPLLE_GRAY		  0XC617				//灰
#define APPLE_RECT_GRAY1  0XEF3B				
#define APPLE_RECT_GRAY2  0XD698

#define PROCESS_UP_COLOR  0XCFF9				//进度条上半部分
#define PROCESS_DN_COLOR  0X0664				//进度条下半部分

#define LITTLE_BLUE_SKY  0X1CFC					//天空蓝(0x189ce0)24 156 224
#define FRESH_GREEN		 0XA667					//鲜绿
	

//DTS界面颜色宏定义
#define COLOR_REFRESH_FORCE				YELLOW			//刷新力值前景色
#define COLOR_REFRESH_SPEED				YELLOW			//刷新速度


#define COLOR_MAIN_FONT_POINT			BLACK			//主界面字体前景色
#define COLOR_FONT_POINT				BLACK			//功能菜单原始字体前景色
#define COLOR_SELECT_FONT_POINT		   	WHITE			//功能菜单选中的字体前景色
#define COLOR_FONT_BACK					CREAM_YELLOW	//功能菜单字体背景色
#define COLOR_RETC_BACK					CREAM_YELLOW	//功能菜单矩形框的背景色
#define	COLOR_SELECT_POINT				WHITE			//选中功能前景色
#define	COLOR_SELECT_BACK				JOHNNY			//选中功能背景色(光标)	
#define	COLOR_SELECT_BACK1				FOREST_GREEN	//选中功能背景色(光标)
#define COLOR_FUNC_KEY_POINT			WHITE			//功能键的前景色
#define COLOR_FUNC_KEY_BACK				DARKBLUE		//功能键的背景色
#define COLOR_WARN						RED				//功能键查看警告颜色
#define COLOR_CLEAR_WARN_BACK			FOREST_GREEN	//功能键清除警告背景色
#define COLOR_FONT						WHITE			//字体颜色
#define COLOR_MENU_FONT_POINT			WHITE			//菜单字体前景色	  
#define COLOR_PUTIN_FONT_POINT			GREEN			//正在输入字符背景色
#define COLOR_PUTIN_FONT_BACK			DARKBLUE		//正在输入字符背景色
#define COLOR_TYPEWRITE_POINT			BLACK			//输入法前景色
#define COLOR_TYPEWRITE_BACK			WHITE			//输入法背景色
#define COLOR_CURSOR_POINT				WHITE			//光标前景色
#define COLOR_SMALL_TITLE_POINT			BLACK			//小光标前景色
#define COLOR_SMALL_TITLE_BACK			LGRAY			//小标题背景色
#define COLOR_FORBIT_EDIT_POINT			COLD_ASH		//禁止编辑字体前景色

#define COLOR_COORDINATE_BACK			BLACK			//坐标系的背景色
#define COLOR_COORDINATE_TITLE_POINT	ORANGE			//坐标系标题前景色
#define COLOR_STATU_BREAK				GRAY			//状态栏分割线的颜色
#define COLOR_STATU_POINT				JOHNNY			//状态栏的状态前景色 
#define COLOR_DRAW_COOR_LINE			RED				//在坐标系画线的颜色	 
		




  

//虚线的结构体
typedef struct
{
  uint16_t x;							//起点x坐标
  uint16_t y;							//起点y坐标
  uint16_t width;						//线的宽度
  uint16_t fill;						//填充长度  
  uint16_t space;						//不填充的长度
  uint16_t length;						//线的总长
  uint8_t direct;						//线的方向 0表示横线，1表示纵线		
  uint16_t color; 						//颜色		
}tDashedObject;

//虚线框结构体
typedef struct
{
  uint16_t x;								//起点x坐标
  uint16_t y;								//起点y坐标
  uint16_t hori_num;						//横条数
  uint16_t hori_space;						//横条间距	
  uint16_t veri_num;						//竖条数
  uint16_t veri_space;						//竖条数
  uint16_t width;							//线的宽度
  uint16_t fill;							//填充长度  
  uint16_t space;							//不填充的长度	
  uint16_t color_row; 						//横线颜色
  uint16_t color_col;						//竖线颜色		
}tDashedFrameObject; 

/*LCD相关的函数*/ 
void lcd_fill(uint16_t sx,uint16_t sy,uint16_t w,uint16_t h,uint16_t color);		  	  	//填充矩形
void lcd_fill_ellipse(uint16_t x0, uint16_t y0, uint16_t rx, uint16_t ry,uint16_t color); 	//画椭圆
void lcd_fill_circle(uint16_t x0, uint16_t y0, uint16_t r,uint16_t color);			 	  	//填充圆
void lcd_draw_circle(uint16_t  x0,uint16_t y0,uint16_t r,uint16_t color);			 		//画圆
void lcd_draw_line(uint16_t x1, uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color); 		//画线
void lcd_draw_point(uint16_t x,uint16_t y,uint16_t color);							 		//画点
void lcd_draw_rect(uint16_t sx,uint16_t sy,uint16_t w,uint16_t h,uint16_t color);	 		//画矩形

FRESULT lcd_font16(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,char *s,char *font); 		//显示16号字体
FRESULT lcd_font20(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,char *s,const char *font);//显示20号字体
FRESULT lcd_font24(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,char *s,char *font);		//显示24号字体
FRESULT lcd_font32(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,char *s,char *font);		//显示32号字体

void lcd_ascin16s(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char *s); 	   				//显示16号ASCII码
void lcd_ascin24s(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char *s); 					//显示24号ASCII码

void lcd_num48(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc, char *s);						//显示48号字符
void lcd_num48_char(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc, char c);					//显示48号.

 

void lcd_init(void);
void lcd_clear(uint16_t color);
void lcd_set_backlight(uint8_t duty);														//设置LCD的背光  设置值0-0xA0
FRESULT lcd_show_image(uint16_t sx,uint16_t sy,char *path); 							    //显示图片
void lcd_draw_dashed(tDashedObject dashed_line);											//画虚线
void lcd_draw_dashed_frame(tDashedFrameObject dashed_frame);								//画虚线框
void lcd_time_display(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc);					    //显示时间
void lcd_date_display(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc); 					    //显示日期
void lcd_mem_err(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc);						    //显示“内存卡出错”文字，大小固定为32X32,当读取SD卡出错是执行此函数，提示用户
uint16_t rgb888_2_565(uint32_t rgb888);														//颜色格式转换  
//颜色转换： 888 -> 565 ,删除原始数据低3位-低2位-低3位 
void font_init( void );
 
//显示通道  
#define		DISP_CHN_FH			0		//负荷
#define		DISP_CHN_WY			1		//位移
#define		DISP_CHN_BX			2  		//变形

#define		DISP_CHN_SPEED		1		//速度
#define 	DISP_CHN_STRENGTH	2		//强度

void disp_syn(uint8_t chn);			//显示同步,当切换到显示值的界面时，需要重新刷新显示缓冲，不然显示最左边的数字会出现不更新的问题
void disp_value(float val,uint8_t chn,uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint8_t int_len,uint8_t dec_len);  	//显示主界面采集到的负荷/位移/变形数据，整形显示n位，小数点后显示n位

#endif




	

	

	












