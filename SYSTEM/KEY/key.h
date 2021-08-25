#ifndef _KEY_H_
#define _KEY_H_

#define KEY_Bit (GPIOA->IDR &0x0001)>>0

extern void key_init(void);



#endif
