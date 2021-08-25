#ifndef _TIMER_H_
#define _TIMER_H_

#include "Sys.h"
/****************预分频值   重装载值*****************/
void Tim1_init(uint16_t psc,uint16_t arr); //中断时间x(us)=[(psc+1)*(arr+1)]/168
void Tim3_init(uint16_t psc,uint16_t arr); //中断时间x(us)=[(psc+1)*(arr+1)]/84

#endif
