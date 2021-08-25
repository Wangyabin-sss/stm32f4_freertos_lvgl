#ifndef _DHT11_H_
#define _DHT11_H_


#include "Sys.h"

#define DHT11Pin_IN()   GPIOG->MODER&=0xFFF3FFFF                           //GPIOG的9口 输入  
#define DHT11Pin_OUT() {GPIOG->MODER&=0xFFF3FFFF; GPIOG->MODER|=0x00040000;}  //GPIOG的9口 输出 

void dht11_init(void);
int8_t dht_getdata(uint8_t *data); //温度湿度


#endif
