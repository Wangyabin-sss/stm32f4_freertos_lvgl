#ifndef _W25QXX_H_
#define _W25QXX_H_

#include "spi.h"

//÷∏¡Ó±Ì
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F


uint16_t W25QXX_Read_Id(void);
void W25QXX_Read_Data(uint8_t *data,uint32_t addr,uint32_t num);
void W25QXX_Write_Enable(void);
void W25QXX_Write_Disnable(void);
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25X_Sector_Erase(uint32_t addr);



#endif 
