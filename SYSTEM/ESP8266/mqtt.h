#ifndef _MQTT_H_
#define _MQTT_H_

#include "usart.h"

extern uint8_t WIFI_NAME[16];
extern uint8_t WIFI_PWD[16];

#define  CLIENT_ID  	 "123456|securemode=3,signmethod=hmacsha1|"
#define  MQTT_USRNAME    "device1&a13jDJtaInP"
#define  MQTT_PASSWORD   "679C19C7EAF084590207DADDB7F39D323D5CBF22"
#define  SEVERADDR  	 "AT+CIPSTART=\"TCP\",\"a13jDJtaInP.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n" //服务器地址与端口
#define  SUBTOPIC   	 "/sys/a13jDJtaInP/device1/thing/service/property/set"
#define  PUBLIST    	 "/sys/a13jDJtaInP/device1/thing/event/property/post"
#define  PUBDATA    	 "{\"id\":\"123456\",\"version\": \"1.0\",\"params\": {\"LEDSwitch\": {\"value\":%d,},\"temperature\": {\"value\": %.2f,},\"Humidity\": {\"value\": %.2f,},\"Flame_State\": {\"value\":%d,},\"gasAQI\": {\"value\":%d,},\"DetectDistance\": {\"value\":%d,},\"Lock_control\": {\"value\":%d,}},\"method\": \"thing.event.property.post\"}"
 
#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

int esp_check(void);
int wifi_check(void);
int mqtt_connect(void);
int mqtt_subtopic(char *sub);
void mqtt_publist(uint32_t led,float temp,float humi,uint32_t flame,uint32_t gas,uint32_t dis);
uint16_t mqtt_publish_data(char *topic, char *message, uint8_t qos);
int mqtt_tick(void);
int exit_transport(void);
int connent_wifi(void);

#endif
