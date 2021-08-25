#ifndef _USART_H_
#define _USART_H_

#include "Sys.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RXMAX 512

#define Uprintf USART1 //����printf�ض����ĸ�����

#define usart3_clear() RX3_data.data[0]=0;RX3_data.a=0

typedef enum{
	usarting=0,     //�������ڶ�
	usartdown,      //���ڶ���
	haveread        //�û���ȡ������
}FlagUsart;

struct RX_DATA{
	volatile uint8_t data[RXMAX];
	volatile uint32_t a;	
	volatile uint32_t b; 
};


extern struct RX_DATA RX1_data; //aΪ�ַ������ȣ�bΪ��ȡ��ɱ�־λ �����н��������ַ���
extern struct RX_DATA RX2_data;
extern struct RX_DATA RX3_data; //a����++                          �����޽�����������

void usart1_init(uint32_t band);
void usart1_send_str(char *str);
void usart1_send_hex(uint8_t *data,uint16_t len);

void usart2_init(uint32_t band);
void usart2_send_str(char *str);
void usart2_send_hex(uint8_t *data,uint16_t len);

void usart3_init(uint32_t band);
void usart3_send_str(char *str);
void usart3_send_hex(uint8_t *data,uint16_t len);

//�������ݿ�ʼָ��   ԭʼ�ַ���   ��ʼstr   ����str   ������ݳ���
char* find_data(char *str,char *start,char *end,uint16_t *len); 

#endif
