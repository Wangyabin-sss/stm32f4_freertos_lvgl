#ifndef _AT24CXX_H
#define _AT24CXX_H

#include "iic.h"

#define AT24C02 0xA0

//密码锁密码地址  			
#define lock_pwd_addr 			0               //8byte
//lcd 状态显示标志位地址	
#define lcd_flag_addr    		8             	//1byte
//WiFi名称地址
#define wifi_name_addr          9				//16byte
//WiFi密码地址
#define wifi_pwd_addr 			25				//16byte
//RFID有效卡id
#define rfid_card_id1           42				//5byte

typedef struct eeprom_data{
	uint8_t addr;
	uint8_t *data;
	uint8_t len;
}eeprom_t;

int32_t at24c02_write(uint8_t addr,uint8_t *buf,uint8_t len);
int32_t at24c02_read(uint8_t addr,uint8_t *buf,uint8_t len);


#endif
