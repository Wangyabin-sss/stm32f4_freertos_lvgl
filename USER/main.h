#ifndef _MAIN_H
#define _MAIN_H

//freertos���ͷ�ļ�
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "stack_macros.h"
#include "semphr.h"
#include "event_groups.h"
//�������ͷ�ļ�
#include "Sys.h"
#include "led.h"
#include "key.h"
#include "keyboard.h"
#include "timer.h"
#include "fsmc.h"
#include "usart.h"
#include "oled.h"
#include "at24cxx.h"
#include "dht11.h"
#include "rtc.h"
#include "mfrc522.h"
#include "paj7620u2.h"
#include "nrf24l01.h"
#include "mqtt.h"
//gui���ͷ�ļ�
#include "gui.h"
//oled��ʾ���ƺ궨��
#define QUEUE_OLED_LEN				16    //oled�����������
#define OLED_CTRL_DISPLAY_ON        0x01  //������ʾ
#define OLED_CTRL_DISPLAY_OFF       0x02  //�ر���ʾ
#define OLED_CTRL_CLEAR             0x04  //����
#define OLED_CTRL_SHOW_CHAR         0x07  //��ʾchar
#define OLED_CTRL_SHOW_STRING       0x08  //��ʾ�ַ���
#define OLED_CTRL_SHOW_CHINESE      0x10  //��ʾ����
#define OLED_CTRL_SHOW_PICTURE      0x20  //��ʾͼƬ
typedef struct __oled_t{
	uint32_t ctrl;
	uint8_t x;
	uint8_t y;

	char    ch;
	uint8_t *str;
	uint8_t font_size;
    uint8_t chinese;
	
	const uint8_t *pic;
	uint8_t pic_width;
	uint8_t pic_height;
}oled_t;
//oled�˵�
typedef struct __oled_menu_t
{
    char *item;					//��ǰ��ĿҪ��ʾ���ַ�
    void(*fun)(void *);				//ѡ��ĳһ�˵���ִ�еĹ��ܺ���
    struct __oled_menu_t *parent;	//��ǰ��Ŀ�ĸ��˵�	
    struct __oled_menu_t *child;	//��ǰ��Ŀ���Ӳ˵�
}oled_menu_t;
//�˵�״̬�ṹ��
typedef struct __menu_status_t
{
	oled_menu_t *menu;        	//��ǰ�˵�
	uint32_t item_cursor;		//���λ��
	uint32_t item_total;		//��ǰ�˵��������
	uint32_t is_menu_show;		//�˵���ʾ����
	uint32_t is_menu_fun;		//��ǰ�˵������Ƿ���������
}menu_sta_t;

//beep�ṹ��
typedef struct _beep{
	uint8_t nums;
	uint8_t times;
}beep;

typedef struct _LED{
	uint32_t method;
}led_t;

//����
#define EVENT_GROUP_KEY1_DOWN		(1<<0)
#define EVENT_GROUP_KEY2_DOWN		(1<<1)
#define EVENT_GROUP_KEY3_DOWN		(1<<2)
#define EVENT_GROUP_KEY4_DOWN		(1<<3)
#define EVENT_GROUP_KEYALL_DOWN		0x0F
//rtc
#define EVENT_GROUP_RTC_WAKEUP		(1<<4)
#define EVENT_GROUP_NRF_CHACK		(1<<5)
#define EVENT_GROUP_RFID_CARD_CHANGE		(1<<6)

//��״̬
#define LOCK_STATUS_RFID            (1<<5)  //ID������
#define LOCK_STATUS_KB            	(1<<6)	//�����������
//mqtt����
#define LOCK_STATUS_MQTT_OPEN       (1<<7)  //mqtt����
#define LOCK_STATUS_MQTT_CLOSE      (1<<8)	//mqtt����
#define LOCK_STATUS_MQTT_LED_OPEN   (1<<9)  //mqtt����
#define LOCK_STATUS_MQTT_LED_CLOSE  (1<<10)	//mqtt�ص�

//�¼���־�飨������RTC��
extern EventGroupHandle_t	event_group;
extern QueueHandle_t        queue_oled; //oled��Ϣ����
extern QueueHandle_t        queue_eeprom; //eeprom��Ϣ����
extern QueueHandle_t        queue_usart2; //usart2��Ϣ����
extern uint8_t PASSWORD[8];
extern TaskHandle_t	app_task_rfid_hander;
extern uint8_t rfid_change_flag;


#endif
