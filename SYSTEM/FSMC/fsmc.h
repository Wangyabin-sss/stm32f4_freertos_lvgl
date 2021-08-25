#ifndef _FSMC_H_
#define _FSMC_H_

#include "Sys.h"

 
#define TFTLCD_DIR	0				//0������  1������

 
//TFTLCD��ַ�ṹ��
typedef struct
{
	volatile uint16_t LCD_CMD;
	volatile uint16_t LCD_DATA;
}TFTLCD_TypeDef;
 
 
//ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A6��Ϊ�������������� 
//ע������ʱSTM32�ڲ�������һλ����! 111 1110=0X7E			    
#define TFTLCD_BASE        ((uint32_t)(0x6C000000 | 0x0000007E))
#define TFTLCD             ((TFTLCD_TypeDef *) TFTLCD_BASE)
 
 
 
//LCD��Ҫ������
typedef struct  
{										    
	uint16_t  width;			//LCD ���
	uint16_t  height;			//LCD �߶�
	uint16_t  id;				//LCD ID
	uint8_t   dir;		    //���������������ƣ�0��������1��������	
	uint16_t  wramcmd;		//��ʼдgramָ��
	uint16_t  setxcmd;		//����x����ָ��
	uint16_t  setycmd;		//����y����ָ��	 
}_lcd_dev;
 
 
 
//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define RED           	 0xF800
#define GREEN         	 0x07E0
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ

 
void LCD_WR_REG(uint16_t cmd);
void LCD_WR_DATA(uint16_t data);
void LCD_WriteCmdData(uint16_t cmd,uint16_t data);
 
void TFTLCD_Init(void); //��ʼ��
void openLCDBackLight(int i);
//���ô��ڴ�С�����Ρ�,���ú�ֻ���ڴ�����д��������������ʾ
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);  
void LCD_Clear(uint16_t Color);	
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos); //��������
void LCD_SetDir(void);  //���ú�����������

void LCD_DrawPoint(uint16_t xpos,uint16_t ypos,uint16_t color); //����


#endif
