#ifndef _SPI_H_
#define _SPI_H_

#include "Sys.h"

#define SSPI_MODE       0

#define SSPI_SCLK   	PCout(8)   //c文件初始化要改
#define SSPI_MOSI   	PCout(6)
#define SSPI_MISO		PDin(7)
#define SSPI_delay()	delay_us(2)

#define W25QXX_CS       PBout(14)
#define RC522_CS        PCout(11)

void spi1_init(void);
void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler);
uint8_t SPI1_ReadWriteByte(uint8_t TxData);
void Soft_Spi_Init(void);
uint8_t Soft_Spi_ReadWriteByte(uint8_t data);
#endif 
