#ifndef _MAIN_H
#define _MAIN_H

//freertos相关头文件
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "stack_macros.h"
#include "semphr.h"
#include "event_groups.h"
//外设相关头文件
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
//gui相关头文件
#include "gui.h"
//oled显示控制宏定义
#define QUEUE_OLED_LEN				16    //oled队列最大容量
#define OLED_CTRL_DISPLAY_ON        0x01  //开启显示
#define OLED_CTRL_DISPLAY_OFF       0x02  //关闭显示
#define OLED_CTRL_CLEAR             0x04  //清屏
#define OLED_CTRL_SHOW_CHAR         0x07  //显示char
#define OLED_CTRL_SHOW_STRING       0x08  //显示字符串
#define OLED_CTRL_SHOW_CHINESE      0x10  //显示中文
#define OLED_CTRL_SHOW_PICTURE      0x20  //显示图片
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
//oled菜单
typedef struct __oled_menu_t
{
    char *item;					//当前项目要显示的字符
    void(*fun)(void *);				//选择某一菜单后执行的功能函数
    struct __oled_menu_t *parent;	//当前项目的父菜单	
    struct __oled_menu_t *child;	//当前项目的子菜单
}oled_menu_t;
//菜单状态结构体
typedef struct __menu_status_t
{
	oled_menu_t *menu;        	//当前菜单
	uint32_t item_cursor;		//光标位置
	uint32_t item_total;		//当前菜单标题个数
	uint32_t is_menu_show;		//菜单显示开启
	uint32_t is_menu_fun;		//当前菜单函数是否正在运行
}menu_sta_t;

//beep结构体
typedef struct _beep{
	uint8_t nums;
	uint8_t times;
}beep;

typedef struct _LED{
	uint32_t method;
}led_t;

//按键
#define EVENT_GROUP_KEY1_DOWN		(1<<0)
#define EVENT_GROUP_KEY2_DOWN		(1<<1)
#define EVENT_GROUP_KEY3_DOWN		(1<<2)
#define EVENT_GROUP_KEY4_DOWN		(1<<3)
#define EVENT_GROUP_KEYALL_DOWN		0x0F
//rtc
#define EVENT_GROUP_RTC_WAKEUP		(1<<4)
#define EVENT_GROUP_NRF_CHACK		(1<<5)
#define EVENT_GROUP_RFID_CARD_CHANGE		(1<<6)

//锁状态
#define LOCK_STATUS_RFID            (1<<5)  //ID卡解锁
#define LOCK_STATUS_KB            	(1<<6)	//按键密码解锁
//mqtt开锁
#define LOCK_STATUS_MQTT_OPEN       (1<<7)  //mqtt解锁
#define LOCK_STATUS_MQTT_CLOSE      (1<<8)	//mqtt关锁
#define LOCK_STATUS_MQTT_LED_OPEN   (1<<9)  //mqtt开灯
#define LOCK_STATUS_MQTT_LED_CLOSE  (1<<10)	//mqtt关灯

//事件标志组（按键、RTC）
extern EventGroupHandle_t	event_group;
extern QueueHandle_t        queue_oled; //oled消息队列
extern QueueHandle_t        queue_eeprom; //eeprom消息队列
extern QueueHandle_t        queue_usart2; //usart2消息队列
extern uint8_t PASSWORD[8];
extern TaskHandle_t	app_task_rfid_hander;
extern uint8_t rfid_change_flag;


#endif
