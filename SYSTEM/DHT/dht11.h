#ifndef _DHT11_H_
#define _DHT11_H_


#include "Sys.h"

#define DHT11Pin_IN()   GPIOG->MODER&=0xFFF3FFFF                           //GPIOG��9�� ����  
#define DHT11Pin_OUT() {GPIOG->MODER&=0xFFF3FFFF; GPIOG->MODER|=0x00040000;}  //GPIOG��9�� ��� 

void dht11_init(void);
int8_t dht_getdata(uint8_t *data); //�¶�ʪ��


#endif
