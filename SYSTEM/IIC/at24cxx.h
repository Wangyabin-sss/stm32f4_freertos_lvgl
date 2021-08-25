#ifndef _AT24CXX_H
#define _AT24CXX_H

#include "iic.h"

#define AT24C02 0xA0

//�����������ַ  			
#define lock_pwd_addr 			0               //8byte
//lcd ״̬��ʾ��־λ��ַ	
#define lcd_flag_addr    		8             	//1byte
//WiFi���Ƶ�ַ
#define wifi_name_addr          9				//16byte
//WiFi�����ַ
#define wifi_pwd_addr 			25				//16byte
//RFID��Ч��id
#define rfid_card_id1           42				//5byte

typedef struct eeprom_data{
	uint8_t addr;
	uint8_t *data;
	uint8_t len;
}eeprom_t;

int32_t at24c02_write(uint8_t addr,uint8_t *buf,uint8_t len);
int32_t at24c02_read(uint8_t addr,uint8_t *buf,uint8_t len);


#endif
