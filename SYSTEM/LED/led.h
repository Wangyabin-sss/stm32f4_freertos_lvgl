#ifndef _LED_H_
#define _LED_H_

#include "stm32f4xx.h"

//�üĴ���д��ʱ�������ӦΪ�����κβ���
#define led_write(n) n ? (GPIOF->BSRRL=GPIO_Pin_9 | GPIO_Pin_10):(GPIOF->BSRRH=GPIO_Pin_9 | GPIO_Pin_10)

void led_init(void);

#endif
