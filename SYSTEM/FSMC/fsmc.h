#ifndef _FSMC_H_
#define _FSMC_H_

#include "Sys.h"

 
#define TFTLCD_DIR	0				//0：竖屏  1：横屏

 
//TFTLCD地址结构体
typedef struct
{
	volatile uint16_t LCD_CMD;
	volatile uint16_t LCD_DATA;
}TFTLCD_TypeDef;
 
 
//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A6作为数据命令区分线 
//注意设置时STM32内部会右移一位对齐! 111 1110=0X7E			    
#define TFTLCD_BASE        ((uint32_t)(0x6C000000 | 0x0000007E))
#define TFTLCD             ((TFTLCD_TypeDef *) TFTLCD_BASE)
 
 
 
//LCD重要参数集
typedef struct  
{										    
	uint16_t  width;			//LCD 宽度
	uint16_t  height;			//LCD 高度
	uint16_t  id;				//LCD ID
	uint8_t   dir;		    //横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t  wramcmd;		//开始写gram指令
	uint16_t  setxcmd;		//设置x坐标指令
	uint16_t  setycmd;		//设置y坐标指令	 
}_lcd_dev;
 
 
 
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define RED           	 0xF800
#define GREEN         	 0x07E0
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色

 
void LCD_WR_REG(uint16_t cmd);
void LCD_WR_DATA(uint16_t data);
void LCD_WriteCmdData(uint16_t cmd,uint16_t data);
 
void TFTLCD_Init(void); //初始化
void openLCDBackLight(int i);
//设置窗口大小【矩形】,设置后只能在此区域写，超出此区域不显示
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);  
void LCD_Clear(uint16_t Color);	
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos); //设置坐标
void LCD_SetDir(void);  //设置横屏还是竖屏

void LCD_DrawPoint(uint16_t xpos,uint16_t ypos,uint16_t color); //画点


#endif
