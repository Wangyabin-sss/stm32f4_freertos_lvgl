#ifndef _LED_H_
#define _LED_H_

#include "stm32f4xx.h"

//该寄存器写零时不会对相应为进行任何操作
#define led_write(n) n ? (GPIOF->BSRRL=GPIO_Pin_9 | GPIO_Pin_10):(GPIOF->BSRRH=GPIO_Pin_9 | GPIO_Pin_10)

void led_init(void);

#endif
