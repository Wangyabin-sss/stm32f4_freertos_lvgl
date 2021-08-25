#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "stm32f4xx.h"

#define OS_SUPPORT 1

#define PAin(x)		*(volatile uint32_t *)(0x42000000 + (GPIOA_BASE + 0x10 - 0x40000000)*32 + x*4)
#define PAout(x)	*(volatile uint32_t *)(0x42000000 + (GPIOA_BASE + 0x14 - 0x40000000)*32 + x*4)	
#define PBin(x)		*(volatile uint32_t *)(0x42000000 + (GPIOB_BASE + 0x10 - 0x40000000)*32 + x*4)
#define PBout(x)	*(volatile uint32_t *)(0x42000000 + (GPIOB_BASE + 0x14 - 0x40000000)*32 + x*4)
#define PCin(x)		*(volatile uint32_t *)(0x42000000 + (GPIOC_BASE + 0x10 - 0x40000000)*32 + x*4)
#define PCout(x)	*(volatile uint32_t *)(0x42000000 + (GPIOC_BASE + 0x14 - 0x40000000)*32 + x*4)	
#define PDin(x)		*(volatile uint32_t *)(0x42000000 + (GPIOD_BASE + 0x10 - 0x40000000)*32 + x*4)
#define PDout(x)	*(volatile uint32_t *)(0x42000000 + (GPIOD_BASE + 0x14 - 0x40000000)*32 + x*4)
#define PEin(x)		*(volatile uint32_t *)(0x42000000 + (GPIOE_BASE + 0x10 - 0x40000000)*32 + x*4)
#define PEout(x)	*(volatile uint32_t *)(0x42000000 + (GPIOE_BASE + 0x14 - 0x40000000)*32 + x*4)	
#define PFin(x)		*(volatile uint32_t *)(0x42000000 + (GPIOF_BASE + 0x10 - 0x40000000)*32 + x*4)
#define PFout(x)	*(volatile uint32_t *)(0x42000000 + (GPIOF_BASE + 0x14 - 0x40000000)*32 + x*4)
#define PGin(x)		*(volatile uint32_t *)(0x42000000 + (GPIOG_BASE + 0x10 - 0x40000000)*32 + x*4)
#define PGout(x)	*(volatile uint32_t *)(0x42000000 + (GPIOG_BASE + 0x14 - 0x40000000)*32 + x*4)	
#define PHin(x)		*(volatile uint32_t *)(0x42000000 + (GPIOH_BASE + 0x10 - 0x40000000)*32 + x*4)
#define PHout(x)	*(volatile uint32_t *)(0x42000000 + (GPIOH_BASE + 0x14 - 0x40000000)*32 + x*4)

//RCC_CFGR_SW_HSI RCC_CFGR_SW_HSE RCC_CFGR_SW_PLL
#define change_clock_source(source) RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));RCC->CFGR |= source;

#if OS_SUPPORT
void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);
#else
void delay_us(uint16_t nus);
void delay_ms(uint16_t nms);
#endif

#endif

