#ifndef __PAJ7620U2_IIC_H
#define __PAJ7620U2_IIC_H

#include "Sys.h"


#define GS_SDA_IN()  {GPIOE->MODER&=~(3<<(5*2));GPIOE->MODER|=0<<5*2;}	//PB9输入模式
#define GS_SDA_OUT() {GPIOE->MODER&=~(3<<(5*2));GPIOE->MODER|=1<<5*2;}  //PB9输出模式

//IO操作函数	 
#define GS_IIC_SCL    PAout(6) 		//SCL
#define GS_IIC_SDA    PEout(5) 		//SDA	 
#define GS_READ_SDA   PEin(5) 		//输入SDA 

uint8_t GS_Write_Byte(uint8_t REG_Address,uint8_t REG_data);
uint8_t GS_Read_Byte(uint8_t REG_Address);
uint8_t GS_Read_nByte(uint8_t REG_Address,uint16_t len,uint8_t *buf);
void GS_i2c_init(void);
void GS_WakeUp(void);

#endif


