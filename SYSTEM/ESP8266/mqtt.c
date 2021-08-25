#include "mqtt.h"

uint8_t WIFI_NAME[16]={0};
uint8_t WIFI_PWD[16]={0};

//服务器连接初始值
static uint8_t connect_data[12]={0x10,115,0x00,0x04,'M','Q','T','T',0x04,0xc2,0x00,0x3c}; //0x3c为心跳包时长秒 (可自己更改)
//订阅
static uint8_t subtopic[6]={0x82,0x38,0x00,0x01,0x00,0x33};
//心跳包
const uint8_t tick[2]={0xc0,0x00};
const uint8_t sever_bak[3][2]={{0x20,0x02},{0xd0,0x00},{0x90,0x03}}; //登录服务器返回值  心跳包返回值  订阅返回值

static uint8_t pubdata[512]={0x30,0xc5,0x00,0x00,0x32}; 

uint16_t g_mqtt_tx_len;
uint8_t  g_esp8266_tx_buf[512];

static uint8_t* mqtt_init(char *client_ID,char *usrname,char *password,uint8_t *datalen)
{
	uint8_t idlen=0,usrlen=0,pwdlen=0,tmp=0;
	idlen=strlen(client_ID);
	usrlen=strlen(usrname);
	pwdlen=strlen(password);
	*datalen=idlen+usrlen+pwdlen+18; //数据总长
	uint8_t *data=(uint8_t *)calloc(*datalen,1);//data为申请的数据内存指针
	connect_data[1]=*datalen-2;     
	memcpy(data,connect_data,12);
	//client_ID保存进数组------------------------------------
	tmp=12;  //用于指针++
	data[tmp++]=0;
	data[tmp++]=idlen;
	memcpy(&data[tmp],client_ID,idlen);
	//usrname保存进数组--------------------------------------
	tmp=tmp+idlen;
	data[tmp++]=0;
	data[tmp++]=usrlen;
	memcpy(&data[tmp],usrname,usrlen);
	//password保存进数组-------------------------------------
	tmp=tmp+usrlen;
	data[tmp++]=0;
	data[tmp++]=pwdlen;
	memcpy(&data[tmp],password,pwdlen);
	return data;
}

static int find_sever_bak(uint8_t *data,uint8_t len)
{
	int i=0;
	printf("mqtt.c-sever_bak=%d,%x,%x,%x\n",len,RX3_data.data[0],RX3_data.data[1],RX3_data.data[2]);
	for(i=0;i<len;i++)
	{
		if(RX3_data.data[i]==data[0]&&RX3_data.data[i+1]==data[1])
			return 0;
	}
	return 1;
}

static int find_usart_bak(char *data) //等待串口返回OK,返回值为0表示正常
{
	uint32_t time=0;
	while(time<200){
		if(strstr((char *)RX3_data.data,data)!=NULL)
			return 0;
		delay_ms(10);time++;
	}return 1;
}

int mqtt_connect(void)
{
	uint8_t *data,datalen;
	data=mqtt_init(CLIENT_ID,MQTT_USRNAME,MQTT_PASSWORD,&datalen);
	
	usart3_clear();
	usart3_send_str(SEVERADDR);         //连接服务器
	if(find_usart_bak("OK")){
		if(find_usart_bak("ALREADY CONNECTED"))
			return -1;
	}
	
	usart3_clear();
	usart3_send_str("AT+CIPMODE=1\r\n"); //开启透传模式
	if(find_usart_bak("OK"))
		return -2;
	
	usart3_clear();
	usart3_send_str("AT+CIPSEND\r\n");  //开启透传
	if(find_usart_bak("OK"))
		return -3;
	
	usart3_clear();
	usart3_send_hex(data,datalen);free(data);
	delay_ms(500);
	if(find_sever_bak((uint8_t *)sever_bak[0],RX3_data.a))
		return -4;
	return 0;
}

int mqtt_subtopic(char *sub)
{
	uint8_t sublen,datalen,*data;
	sublen=strlen(sub);
	subtopic[1]=sublen+5;
	subtopic[5]=sublen;
	datalen=sublen+7;
	data=(uint8_t *)calloc(1,datalen);
	memcpy(data,subtopic,6);
	memcpy(&data[6],sub,sublen);
	
	usart3_clear();
	usart3_send_hex(data,datalen);free(data);
	delay_ms(500);
	if(find_sever_bak((uint8_t *)sever_bak[2],RX3_data.a))
		return -1;
	return 0;
}

/*超过128字节有问题*/
void mqtt_publist(uint32_t led,float temp,float humi,uint32_t flame,uint32_t gas,uint32_t dis)
{
	uint32_t topiclen,messagelen,alllength;
	topiclen=strlen(PUBLIST);     //topic长度
	pubdata[4]=topiclen;
	memcpy(&pubdata[5],PUBLIST,topiclen);
	
	sprintf((char *)&pubdata[5+topiclen],PUBDATA,led,temp,humi,flame,gas,dis);
	messagelen=strlen((char *)&pubdata[5+topiclen]);  //message 长度
	
	alllength=topiclen+messagelen+2;
	if(alllength>127)
	{
		pubdata[1]=(alllength%128)+128;
		pubdata[2]=alllength/128;
	}else{
		pubdata[1]=alllength;
	}
	usart3_send_hex(pubdata,pubdata[1]+3);
	delay_ms(100);
}

//MQTT发布数据打包函数
//topic   主题
//message 消息
//qos     消息等级
uint16_t mqtt_publish_data(char *topic, char *message, uint8_t qos)
{
static 
	uint16_t id=0;	
    int32_t topicLength = strlen(topic);
    int32_t messageLength = strlen(message);

    int32_t data_len;
	uint8_t encodedByte;
    g_mqtt_tx_len=0;
    //有效载荷的长度这样计算：用固定报头中的剩余长度字段的值减去可变报头的长度
    //QOS为0时没有标识符
    //数据长度             主题名   报文标识符   有效载荷
    if(qos)	data_len = (2+topicLength) + 2 + messageLength;
    else	data_len = (2+topicLength) + messageLength;

    //固定报头
    //控制报文类型
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0x30;    // MQTT Message Type PUBLISH

    //剩余长度
    do
    {
        encodedByte = data_len % 128;
        data_len = data_len / 128;
        // if there are more data to encode, set the top bit of this byte
        if ( data_len > 0 )
            encodedByte = encodedByte | 128;
        g_esp8266_tx_buf[g_mqtt_tx_len++] = encodedByte;
    } while ( data_len > 0 );

    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE1(topicLength);//主题长度MSB
    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE0(topicLength);//主题长度LSB
	
    memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len],topic,topicLength);//拷贝主题
	
    g_mqtt_tx_len += topicLength;

    //报文标识符
    if(qos)
    {
        g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE1(id);
        g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE0(id);
        id++;
    }
	
    memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len],message,messageLength);
	
    g_mqtt_tx_len += messageLength;

    usart3_send_hex(g_esp8266_tx_buf,g_mqtt_tx_len);
	delay_ms(100);
    return g_mqtt_tx_len;
}


//返回0表示发送成功
int mqtt_tick(void)
{
	usart3_clear();
	usart3_send_hex((uint8_t*)tick,2);
	delay_ms(300);
	if(find_sever_bak((uint8_t *)sever_bak[1],RX3_data.a))
	{
		usart3_clear();
		return -1;
	}
	usart3_clear();
	return 0;
}

//返回0表示正常
int esp_check(void)
{
	usart3_clear();
	usart3_send_str("ATE0\r\n");
	if(find_usart_bak("OK"))
		return -1;
	return 0;
}
//返回0表示连接WiFi
int wifi_check(void)
{
	usart3_clear();
	usart3_send_str("AT+CIFSR\r\n");
	delay_ms(500);
	//printf("%s\n",RX3_data.data);
	if(strstr((char *)RX3_data.data,"\"0.0.0.0\"")==NULL)
		return 0;
	return -1;
}

int exit_transport(void)
{
	usart3_send_str("+++");
	delay_ms(500);
	return esp_check();
}

int connent_wifi(void)
{
	char data[100]={0};
	usart3_clear();
	sprintf(data,"AT+CWJAP=\"%s\",\"%s\"\r\n",WIFI_NAME,WIFI_PWD);
	usart3_send_str(data);
	delay_ms(2000);
	if(find_usart_bak("OK"))
		return -1;
	usart3_clear();
	return 0;
}





