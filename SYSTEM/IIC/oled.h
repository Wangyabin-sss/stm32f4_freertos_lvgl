#ifndef __OLED_H
#define __OLED_H	

#include "iic.h"

#define OLED_MODE 0
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC端口定义----------------  					   

#define OLEDSCL				PGout(7)
#define OLEDSDA_W			PGout(6)
#define OLEDSDA_R			PGin(6)
 		     
#define OLED_CMD  0		//写命令
#define OLED_DATA 1		//写数据


//OLED控制用函数
void OLED_WR_Byte(unsigned char dat,unsigned char cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Clear_Line(uint8_t line);  
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p,uint8_t Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);
void Picture(void);
void Write_IIC_Command(unsigned char IIC_Command);

uint32_t i2c_wait_ack(void);


#endif  
	 



