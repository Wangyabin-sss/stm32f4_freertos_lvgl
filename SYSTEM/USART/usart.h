#ifndef _USART_H_
#define _USART_H_

#include "Sys.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RXMAX 512

#define Uprintf USART1 //定义printf重定向到哪个串口

#define usart3_clear() RX3_data.data[0]=0;RX3_data.a=0

typedef enum{
	usarting=0,     //串口正在读
	usartdown,      //串口读完
	haveread        //用户获取完数据
}FlagUsart;

struct RX_DATA{
	volatile uint8_t data[RXMAX];
	volatile uint32_t a;	
	volatile uint32_t b; 
};


extern struct RX_DATA RX1_data; //a为字符串长度，b为读取完成标志位 接收有结束符的字符串
extern struct RX_DATA RX2_data;
extern struct RX_DATA RX3_data; //a用来++                          接受无结束符的数据

void usart1_init(uint32_t band);
void usart1_send_str(char *str);
void usart1_send_hex(uint8_t *data,uint16_t len);

void usart2_init(uint32_t band);
void usart2_send_str(char *str);
void usart2_send_hex(uint8_t *data,uint16_t len);

void usart3_init(uint32_t band);
void usart3_send_str(char *str);
void usart3_send_hex(uint8_t *data,uint16_t len);

//返回数据开始指针   原始字符串   开始str   结束str   获得数据长度
char* find_data(char *str,char *start,char *end,uint16_t *len); 

#endif
