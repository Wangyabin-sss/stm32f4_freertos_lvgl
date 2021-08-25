#include "main.h"

/*任务句柄*/
static 	TaskHandle_t	app_task_init_handle		=NULL; 	//开始任务句柄
static 	TaskHandle_t	app_task_keyscan_hander		=NULL;  //按键任务句柄
static 	TaskHandle_t	app_task_led1_hander		=NULL;  //led任务句柄
static 	TaskHandle_t	app_task_beep_hander		=NULL;  //beep任务句柄
static 	TaskHandle_t	app_task_oled_hander		=NULL;  //oled任务句柄
static 	TaskHandle_t	app_task_dht11_hander		=NULL;  //dht11任务句柄
static 	TaskHandle_t	app_task_gui_hander			=NULL;  //gui任务句柄
		TaskHandle_t	app_task_rfid_hander		=NULL;  //rfid任务句柄
static 	TaskHandle_t	app_task_keyboard_hander	=NULL;  //矩阵按键任务句柄
static 	TaskHandle_t	app_task_rtc_hander			=NULL;  //RTC任务句柄
static 	TaskHandle_t	app_task_paj_hander			=NULL;  //手势任务句柄
static 	TaskHandle_t	app_task_mqtt_hander		=NULL;  //手势任务句柄
static 	TaskHandle_t	app_task_eeprom_hander		=NULL;  //eeprom任务句柄
static 	TaskHandle_t	app_task_bluetooth_hander	=NULL;  //Bluetooth任务句柄
static  TaskHandle_t	app_task_nrf24l01_hander	=NULL;  //2.4g任务句柄
static  TaskHandle_t	app_task_changecard_hander	=NULL;  //改变id卡任务句柄
/*任务函数*/
static void app_task_init			(void *pvParameters);//开始任务函数
static void app_task_keyscan 		(void *pvParameters);//按键检测函数
static void app_task_led1 			(void *pvParameters);//led任务函数
static void app_task_beep 			(void *pvParameters);//beep任务函数
static void app_task_oled 			(void *pvParameters);//oled任务函数
static void app_task_dht11 			(void *pvParameters);//dht11任务函数
static void app_task_gui 			(void *pvParameters);//gui任务函数
static void app_task_rfid 			(void *pvParameters);//rfid任务函数
static void app_task_keyboard 		(void *pvParameters);//矩阵按键任务函数
static void app_task_rtc 			(void *pvParameters);//RTC任务函数
static void app_task_paj 			(void *pvParameters);//手势任务函数
static void app_task_mqtt 			(void *pvParameters);//mqtt任务函数
static void app_task_eeprom 		(void *pvParameters);//eeprom任务函数
static void app_task_bluetooth		(void *pvParameters);//bluetooth任务函数
static void app_task_nrf24l01		(void *pvParameters);//2.4g任务函数
static void app_task_changecard		(void *pvParameters);//id卡change任务函数

/*全局变量*/
EventGroupHandle_t	event_group;   //事件标志组
QueueHandle_t       queue_oled;    //oled消息队列
QueueHandle_t       queue_eeprom;  //eeprom消息队列
QueueHandle_t       queue_usart2;  //usart2消息队列
QueueHandle_t       queue_beep;    //beep消息队列
QueueHandle_t       queue_led;     //led消息队列
TimerHandle_t       xTimerLED;     // 定义句柄
uint8_t PASSWORD[8]={0};
uint32_t LOCK_STATUS=0;
static float Temperature=0,Humidity=0;
static uint16_t flame_status=0,dis_static=0,gas_static=0;
static uint8_t RFID_CARD_ID1[5]={0};
uint8_t rfid_change_flag=0;


//oled菜单
static uint32_t oled_show_rtcflag=0;
static uint32_t oled_show_dhtflag=0;
static oled_menu_t menu_main[];
static oled_menu_t menu_rtc[];	//二级rtc菜单
static oled_menu_t menu_rtc_date[];
static oled_menu_t menu_rtc_time[];

static oled_menu_t menu_dht[];	//二级dht菜单
static oled_menu_t menu_dht_temp[];
static oled_menu_t menu_dht_humi[];

static oled_menu_t menu_exit[];	//二级退出菜单

static menu_sta_t m_sta={menu_main,0,0,0}; //oled菜单状态

static void menu_dht_fun(void* pvParameters);
static void menu_rtc_fun(void* pvParameters);
static void menu_exit_fun(void* pvParameters);
//主菜单
static oled_menu_t menu_main[]=
{
	{"1.DHT11"			,NULL,NULL,menu_dht},	
	{"2.Date & Time "	,NULL,NULL,menu_rtc},
	{"3.Exit"           ,NULL,NULL,menu_exit},
	{NULL,NULL,NULL,NULL}
};
//二级菜单
static oled_menu_t menu_dht[]=
{
	{"[1] Temp",NULL,menu_main,menu_dht_temp},
	{"[2] Humi",NULL,menu_main,menu_dht_humi},
	{NULL,NULL,NULL,NULL}
};
static oled_menu_t menu_rtc[]=
{
	{"[1] Date",NULL,menu_main,menu_rtc_date},
	{"[2] Time",NULL,menu_main,menu_rtc_time},
	{NULL,NULL,NULL,NULL},	
};
static oled_menu_t menu_exit[]=
{
	{"1",menu_exit_fun,NULL,NULL},
	{NULL,NULL,NULL,NULL}
};
//三级菜单
static oled_menu_t menu_rtc_date[]=
{
	{"1",menu_rtc_fun,menu_rtc,NULL},
	{NULL,NULL,NULL,NULL}
};
static oled_menu_t menu_rtc_time[]=
{
	{"2",menu_rtc_fun,menu_rtc,NULL},
	{NULL,NULL,NULL,NULL}
};
static oled_menu_t menu_dht_temp[]=
{
	{"1",menu_dht_fun,menu_dht,NULL},
	{NULL,NULL,NULL,NULL}
};
static oled_menu_t menu_dht_humi[]=
{
	{"2",menu_dht_fun,menu_dht,NULL},
	{NULL,NULL,NULL,NULL}
};


void item_str_show(uint8_t x,uint8_t y,uint8_t *cursor)//向队列发送字符串
{
	BaseType_t	xReturn;
	oled_t   oled;
	oled.ctrl=OLED_CTRL_SHOW_STRING;
	oled.font_size=12;	
	oled.x=x;
	oled.y=y;
	oled.str=cursor;
	xReturn=xQueueSend(queue_oled,&oled,100);
	if(xReturn!=pdPASS)
		printf("item show error\n");
}

uint32_t menu_item_total(oled_menu_t *menu)
{
	oled_menu_t *m = menu;
	uint32_t item_count=0;

	while(m->item){
		m++;
		item_count++;
	}
	return item_count;
}

void menu_show(menu_sta_t *m_status)
{
	uint8_t y=2;
	oled_menu_t *menu=m_status->menu;
	m_status->is_menu_show=1;            //菜单显示开启
	m_status->item_cursor=y;			//记录光标位置
	m_status->item_total=menu_item_total(m_status->menu);
	
	if(menu->child){				//如果有子菜单显示选择键
		item_str_show(5,m_status->item_cursor,(uint8_t *)">");
	}
	while(y<8)
	{
		if(menu->item==NULL)
			break;
		
		item_str_show(20,y,(uint8_t *)menu->item);
		
		menu++;//指向结构体数组的下一个
		y+=2;
	}
}


static void menu_dht_fun(void* pvParameters){

	char dht[12]={0};
	menu_sta_t *m_t=(menu_sta_t *)pvParameters;
	
	if(m_t->is_menu_fun==0){    //开始显示
		
		m_t->is_menu_fun=1;
		if(m_t->menu->item[0]=='1'){
			oled_show_dhtflag=1;
//			sprintf(dht,"Temp =%6.2f",Temperature);
			//taskENTER_CRITICAL();
			
//			item_str_show(10,3,(uint8_t *)dht);
			//taskEXIT_CRITICAL();
			printf("%s\n",dht);
		}
		if(m_t->menu->item[0]=='2'){
			oled_show_dhtflag=2;
//			sprintf(dht,"Humi =%6.2f",Humidity);
//			item_str_show(10,3,(uint8_t *)dht);
			printf("%s\n",dht);
		}
		
	}
	else if(m_t->is_menu_fun==1){ //关闭显示
		m_t->is_menu_fun=0;
		oled_show_dhtflag=0;
	}
}


static void menu_rtc_fun(void* pvParameters){
	menu_sta_t *m_t=(menu_sta_t *)pvParameters;
	if(m_t->is_menu_fun==0){    //开始显示
		m_t->is_menu_fun=1;
		if(m_t->menu->item[0]=='1')
			oled_show_rtcflag=1;
		if(m_t->menu->item[0]=='2')  //判断显示时间
			oled_show_rtcflag=2;
	}
	else if(m_t->is_menu_fun==1){ //关闭显示
		m_t->is_menu_fun=0;
		oled_show_rtcflag=0;
	}
}

static void menu_exit_fun(void* pvParameters){
	oled_t oled;
	oled.ctrl=OLED_CTRL_CLEAR;
	xQueueSend(queue_oled,&oled,100);
	m_sta.is_menu_show=0;
}

/*************************************************分界线*************************************************************/
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	SysTick_Config(SystemCoreClock/configTICK_RATE_HZ);
	usart1_init(115200);
	usart2_init(115200);
	usart3_init(115200);
	
	//创建开始任务
    xTaskCreate((TaskFunction_t ) app_task_init,          //任务函数
				(const char*    ) "start_task",        	  //任务名称
				(uint16_t       ) 1024,                    //任务堆栈大小(单位字)
				(void*          ) NULL,                   //传递给任务函数的参数
				(UBaseType_t    ) 15,                      //任务优先级
				(TaskHandle_t*  ) &app_task_init_handle); //任务句柄
				
    vTaskStartScheduler(); //开启任务调度
    while(1);
}

//开始任务任务函数
void app_task_init(void *pvParameters)
{
	int i;
	event_group   = xEventGroupCreate();
	queue_oled 	  = xQueueCreate(QUEUE_OLED_LEN,sizeof(oled_t));
	queue_eeprom  = xQueueCreate(10,sizeof(eeprom_t));	
	queue_usart2  = xQueueCreate(10,sizeof(struct RX_DATA));
	queue_beep    = xQueueCreate(10,sizeof(beep));	
	queue_led     = xQueueCreate(10,sizeof(led_t));	
	
	led_init();					
	key_init();                 //4个标准按键初始化
	key_board_init();			//矩阵按键初始化
	rtc_init();					//rtc时钟初始化
	OLED_Init();				
	
	at24c02_read(0,PASSWORD,8);            //读取eeprom存放的关锁密码
	for(i=0;i<8;i++)
		printf("%d ",PASSWORD[i]);
	printf("\n");
	at24c02_read(8,&lv_sta.isit_show,1);	//读取LVGL显示状态
	printf("%x\n",lv_sta.isit_show);
	at24c02_read(wifi_name_addr,WIFI_NAME,8);  //读取WiFi name
	at24c02_read(wifi_name_addr+8,&WIFI_NAME[7],8);
	at24c02_read(wifi_pwd_addr,WIFI_PWD,8);    //读取WiFi password
	at24c02_read(wifi_pwd_addr+8,&WIFI_PWD[7],8);
	printf("WIFI_NAME=%s,WIFI_PWD=%s\n",WIFI_NAME,WIFI_PWD);
	
	at24c02_read(rfid_card_id1,RFID_CARD_ID1,5);            //读取eeprom存放的关锁密码
	for(i=0;i<5;i++)
		printf("%x ",RFID_CARD_ID1[i]);
	
	OLED_Clear();
	dht11_init();				//温湿度初始化
	paj7620u2_init();			//手势识别模块初始化
	spi1_init();
	delay_ms(1);
	NRF24L01_GPIO_Init();       //2.4g模块初始化
	printf("NRF_check%d\n",NRF24L01_Check());
	NRF24L01_TX_Mode();
	Soft_Spi_Init();			//用于RFID模块
	TFTLCD_Init();				//触摸屏初始化
	lv_init();
	lv_port_disp_init();        // 显示器初始化
    lv_port_indev_init();       // 输入设备初始化
	
	lv_gui_init();     //ui界面初始化
	
	lv_tab2_add_listbtn(0);
	lv_tab2_add_listbtn(1);
	lv_tab2_add_listbtn(2);
	//connent_again:
	vTaskDelay(8000);
	i=exit_transport();
	printf("exit_transport=%d\n",i);
	i=esp_check();
	printf("esp_check=%d\n",i);
	if(i==0){         //esp8266插好
		i=wifi_check();
		printf("wifi_check=%d\n",i);
		if(i==0){     //WiFi已连接
			printf("mqtt_connect=%d\n",mqtt_connect());
			vTaskDelay(500);
			printf("mqtt_subtopic=%d\n",mqtt_subtopic(SUBTOPIC));
		}else{
			printf("connect_wifi=%d",connent_wifi());
			//goto connent_again;
		}
	}
	
    taskENTER_CRITICAL(); //进入临界区
	
    //创建key 任务
    xTaskCreate((TaskFunction_t )app_task_keyscan,
				(const char*    )"app_task_keyscan",
				(uint16_t       )512,
				(void*          )NULL,
				(UBaseType_t    )8,
				(TaskHandle_t*  )&app_task_keyscan_hander);
				
    //创建LED1 任务
    xTaskCreate((TaskFunction_t )app_task_led1,
				(const char*    )"app_task_led1",
				(uint16_t       )512,
				(void*          )NULL,
				(UBaseType_t    )8,
				(TaskHandle_t*  )&app_task_led1_hander);
				
	//创建beep 任务
    xTaskCreate((TaskFunction_t )app_task_beep,
				(const char*    )"app_task_beep",
				(uint16_t       )512,
				(void*          )NULL,
				(UBaseType_t    )8,
				(TaskHandle_t*  )&app_task_beep_hander);
				
	//创建oled任务
    xTaskCreate((TaskFunction_t )app_task_oled,
				(const char*    )"app_task_oled",
				(uint16_t       )512,
				(void*          )NULL,
				(UBaseType_t    )9,
				(TaskHandle_t*  )&app_task_oled_hander);
				
	//创建dht11任务
    xTaskCreate((TaskFunction_t )app_task_dht11,
				(const char*    )"app_task_dht11",
				(uint16_t       )512,
				(void*          )NULL,
				(UBaseType_t    )8,
				(TaskHandle_t*  )&app_task_dht11_hander);
				
	//创建gui任务
    xTaskCreate((TaskFunction_t )app_task_gui,
				(const char*    )"app_task_gui",
				(uint16_t       )512,
				(void*          )NULL,
				(UBaseType_t    )10,
				(TaskHandle_t*  )&app_task_gui_hander);
				
	//创建rfid任务
    xTaskCreate((TaskFunction_t )app_task_rfid,
				(const char*    )"app_task_rfid",
				(uint16_t       )512,
				(void*          )NULL,
				(UBaseType_t    )8,
				(TaskHandle_t*  )&app_task_rfid_hander);
				
	//创建矩阵按键任务
    xTaskCreate((TaskFunction_t )app_task_keyboard,
				(const char*    )"app_task_keyboard",
				(uint16_t       )512,
				(void*          )NULL,
				(UBaseType_t    )8,
				(TaskHandle_t*  )&app_task_keyboard_hander);
				
	//创建nrf24l01任务
    xTaskCreate((TaskFunction_t )app_task_nrf24l01,
				(const char*    )"app_task_nrf24l01",
				(uint16_t       )1024,
				(void*          )NULL,
				(UBaseType_t    )8,
				(TaskHandle_t*  )&app_task_nrf24l01_hander);
				
	//创建rtc任务
    xTaskCreate((TaskFunction_t )app_task_rtc,
				(const char*    )"app_task_rtc",
				(uint16_t       )1024,
				(void*          )NULL,
				(UBaseType_t    )8,
				(TaskHandle_t*  )&app_task_rtc_hander);
				
	//创建手势识别任务
    xTaskCreate((TaskFunction_t )app_task_paj,
				(const char*    )"app_task_paj",
				(uint16_t       )1024,
				(void*          )NULL,
				(UBaseType_t    )8,
				(TaskHandle_t*  )&app_task_paj_hander);
				
	//创建mqtt任务
    xTaskCreate((TaskFunction_t )app_task_mqtt,
				(const char*    )"app_task_mqtt",
				(uint16_t       )1024,
				(void*          )NULL,
				(UBaseType_t    )9,
				(TaskHandle_t*  )&app_task_mqtt_hander);
				
	//创建eeprom任务
    xTaskCreate((TaskFunction_t )app_task_eeprom,
				(const char*    )"app_task_eeprom",
				(uint16_t       )512,
				(void*          )NULL,
				(UBaseType_t    )8,
				(TaskHandle_t*  )&app_task_eeprom_hander);
				
	//创建bluetooth任务
    xTaskCreate((TaskFunction_t )app_task_bluetooth,
				(const char*    )"app_task_bluetooth",
				(uint16_t       )512,
				(void*          )NULL,
				(UBaseType_t    )8,
				(TaskHandle_t*  )&app_task_bluetooth_hander);
				
	//创建id card change任务
    xTaskCreate((TaskFunction_t )app_task_changecard,
				(const char*    )"app_task_changecard",
				(uint16_t       )512,
				(void*          )NULL,
				(UBaseType_t    )8,
				(TaskHandle_t*  )&app_task_changecard_hander);
				
    taskEXIT_CRITICAL(); //退出临界区
				
	//vTaskSuspend(app_task_nrf24l01_hander);
				
	vTaskDelete(NULL); //删除任务自身
}


//KEY 任务函数
void app_task_keyscan(void *pvParameters)
{
	oled_t oled;
	BaseType_t	xReturn;
	EventBits_t EventValue;
    while(1)
    {
		EventValue=xEventGroupWaitBits((EventGroupHandle_t	)event_group,		
									   (EventBits_t			)EVENT_GROUP_KEYALL_DOWN,
									   (BaseType_t			)pdTRUE,				
									   (BaseType_t			)pdFALSE,
									   (TickType_t			)portMAX_DELAY);
		vTaskDelay(80);
		if(m_sta.is_menu_show==1){                  //如果菜单显示 才进行按键操作
			if(EventValue & EVENT_GROUP_KEY1_DOWN)  	 //按键1检测向上
			{
				/* 禁止EXTI0触发中断 */
				NVIC_DisableIRQ(EXTI0_IRQn);
				if(PAin(0)==0&&m_sta.is_menu_fun==0){    //如果菜单功能函数正在运行，按键123不可用
					if((m_sta.item_cursor-2)!=0)
					{
						item_str_show(5,m_sta.item_cursor,(uint8_t*)" ");
						m_sta.item_cursor-=2;
						m_sta.menu--;
					}item_str_show(5,m_sta.item_cursor,(uint8_t*)">");
				}

				/* 允许EXTI0触发中断 */
				NVIC_EnableIRQ(EXTI0_IRQn);	
			}
			else if(EventValue & EVENT_GROUP_KEY2_DOWN)   //按键2检测向下
			{
				/* 禁止EXTI2触发中断 */
				NVIC_DisableIRQ(EXTI2_IRQn);
				if(PEin(2)==0&&m_sta.is_menu_fun==0){
					if((m_sta.item_cursor+2)!=(m_sta.item_total*2+2))
					{
						item_str_show(5,m_sta.item_cursor,(uint8_t*)" ");
						m_sta.item_cursor+=2;
						m_sta.menu++;
					}item_str_show(5,m_sta.item_cursor,(uint8_t*)">");
				}
				
				/* 允许EXTI2触发中断 */
				NVIC_EnableIRQ(EXTI2_IRQn);	
			}
			else if(EventValue & EVENT_GROUP_KEY3_DOWN)   //按键3检测进入
			{
				/* 禁止EXTI2触发中断 */
				NVIC_DisableIRQ(EXTI3_IRQn);
				if(PEin(3)==0&&m_sta.is_menu_fun==0){
					if(m_sta.menu->child->child!=NULL){ 
						m_sta.menu=m_sta.menu->child;
						oled.ctrl=OLED_CTRL_CLEAR;
						xReturn = xQueueSend(queue_oled,&oled,100);
						menu_show(&m_sta);
					}else{                             //子菜单是功能函数
						m_sta.menu=m_sta.menu->child;
						oled.ctrl=OLED_CTRL_CLEAR;
						xReturn = xQueueSend(queue_oled,&oled,100);
						//vTaskDelay(1000);
						m_sta.menu->fun((void *)&m_sta);
					}
				}

				/* 允许EXTI2触发中断 */
				NVIC_EnableIRQ(EXTI3_IRQn);
			}
			else if(EventValue & EVENT_GROUP_KEY4_DOWN)   //按键4检测后退
			{
				/* 禁止EXTI2触发中断 */
				NVIC_DisableIRQ(EXTI4_IRQn);
				if(PEin(4)==0){
					if(m_sta.is_menu_fun==1){             //如果菜单功能函数正在运行
						m_sta.menu->fun((void *)&m_sta);
						m_sta.menu=m_sta.menu->parent;
						oled.ctrl=OLED_CTRL_CLEAR;
						xReturn = xQueueSend(queue_oled,&oled,100);
						menu_show(&m_sta);
					}
					else if(m_sta.menu->parent!=NULL){
						m_sta.menu=m_sta.menu->parent;
						oled.ctrl=OLED_CTRL_CLEAR;
						xReturn = xQueueSend(queue_oled,&oled,100);
						menu_show(&m_sta);
					}
				}

				/* 允许EXTI2触发中断 */
				NVIC_EnableIRQ(EXTI4_IRQn);	
			}
		}
    }
}

static void xTimerLED_Callback(void* parameter)
{
	PFout(9)=1;LOCK_STATUS=0;
	printf("lock_close\n");
}

//LED1 任务函数
void app_task_led1(void *pvParameters)
{
	led_t led;
	BaseType_t	xReturn;
    while(1)
    {
		xReturn = xQueueReceive(queue_led,	    /* 消息队列的句柄 */
								&led, 	    	/* 得到的消息内容 */
								portMAX_DELAY);	/* 等待时间一直等 */
		if(led.method==LOCK_STATUS_MQTT_LED_OPEN)
			PFout(10)=0;
		if(led.method==LOCK_STATUS_MQTT_LED_CLOSE)
			PFout(10)=1;
		if(led.method&(LOCK_STATUS_RFID|LOCK_STATUS_KB|LOCK_STATUS_MQTT_OPEN)){
			PFout(9)=0;LOCK_STATUS=1;
			xTimerLED=xTimerCreate(	(const char*		    )"Timer led",
									(TickType_t				)5000,	/* 定时器周期 5000(tick) */
									(UBaseType_t			)pdFALSE,/* 周期模式 */
									(void*					)1,		/* 为每个计时器分配一个索引的唯一ID */
									(TimerCallbackFunction_t)xTimerLED_Callback); 	
			/* 开启周期软件定时器 */							
			xTimerStart(xTimerLED,0);
		}
		
		
    }
}

void app_task_beep(void *pvParameters)
{
	uint32_t i;
	beep b;
	BaseType_t	xReturn;
	while(1)
	{
		xReturn = xQueueReceive(queue_beep,	    /* 消息队列的句柄 */
								&b, 	    	/* 得到的消息内容 */
								portMAX_DELAY);	/* 等待时间一直等 */
		PFout(8)=0;
		if(b.nums!=0){
			i=b.nums*2;
			while(i--){
				PFout(8)^=1;
				vTaskDelay(b.times);
			}
		}
		
	}
}

void app_task_oled(void *pvParameters)
{
	oled_t oled;
	BaseType_t	xReturn;
	while(1)
	{
		xReturn = xQueueReceive(queue_oled,	    /* 消息队列的句柄 */
								&oled, 	    	/* 得到的消息内容 */
								portMAX_DELAY);	/* 等待时间一直等 */
		if(xReturn != pdPASS)
			continue;
		IIC_DT=1;
		switch(oled.ctrl)
		{
			case OLED_CTRL_DISPLAY_ON:
			{
				/* 亮屏 */
				OLED_Display_On();				

			}break;

			case OLED_CTRL_DISPLAY_OFF:
			{
				/* 灭屏 */
				OLED_Display_Off();				

			}break;

			case OLED_CTRL_CLEAR:
			{
				/* 清屏 */
				OLED_Clear();
			}break;
			
			case OLED_CTRL_SHOW_CHAR:
			{
				/* 显示字符 */
				OLED_ShowChar(oled.x,oled.y,oled.ch,oled.font_size);
			}break;
			
			case OLED_CTRL_SHOW_STRING:
			{
				/* 显示字符串 */
				OLED_ShowString(oled.x,oled.y,oled.str,oled.font_size);
			}break;

			case OLED_CTRL_SHOW_CHINESE:
			{
				/* 显示汉字 */
				OLED_ShowCHinese(oled.x,oled.y,oled.chinese);
			}break;			

			case OLED_CTRL_SHOW_PICTURE:
			{
				/* 显示图片 */
				OLED_DrawBMP(oled.x,oled.y,oled.x+oled.pic_width,oled.y+oled.pic_height,oled.pic);
			}break;

			default:
				break;
		}
		IIC_DT=5;
	}
}

void app_task_dht11(void *pvParameters)
{
	uint8_t dht11_data[5]={0};
	int ret;
	char dht[12];
	while(1)
	{
		
		ret = dht_getdata(dht11_data);
		
		if(ret==0)
		{
			printf("dht ok\n");
			Temperature=(float)dht11_data[2]+(float)dht11_data[3]/100;
			Humidity   =(float)dht11_data[0]+(float)dht11_data[1]/100;
			if(oled_show_dhtflag==1){
				sprintf(dht,"Temp =%6.2f",Temperature);
				item_str_show(10,3,(uint8_t *)dht);
			}
			if(oled_show_dhtflag==2){
				sprintf(dht,"Humi =%6.2f",Humidity);
				item_str_show(10,3,(uint8_t *)dht); 
			}
		}else{
			printf("dht=%d",ret);
		}
		vTaskDelay(3000);
	}
}

void app_task_gui(void *pvParameters)
{
	while(1)
	{
		lv_task_handler();
		vTaskDelay(5);
	}
}

void app_task_rfid(void *pvParameters)
{
	//uint8_t  card_id[5]={0};
	//uint8_t  write_card_id[5]={0xc3,0x69,0xc0,0x15,0x7f};
	//uint8_t  bule_card_id[5] ={0xea,0x1d,0x5a,0xbf,0x12};

	uint32_t id_num=1,i,j,times=0;             //flash中保存的ID个数
	uint8_t  card_numberbuf[5]={0};               //用于保存寻到卡后获取的卡ID
	uint8_t  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff}; //密钥一,初始状态位6个0xff
//	uint8_t  card_writebuf[16]={'@',0x20};   //要写入卡的数据
//	uint8_t  card_readbuf[18]={0};       //从卡中读取到的数据
	uint8_t data[2];
	uint8_t status,card_size,oled_show;
	
	oled_t oled;
	led_t led;
	beep b;
	BaseType_t	xReturn;
	
	while(1)
	{
		MFRC522_Initializtion();
		if(MFRC522_Request(0x52,data)==MI_OK&&(LOCK_STATUS==0))  //寻卡
		{
			status=MFRC522_Anticoll(card_numberbuf);			//获取ID
			for(i=0;i<id_num;i++)                              //遍历flash中存储的ID
			{
				for(j=0;j<5;j++){
					if(card_numberbuf[j]!=RFID_CARD_ID1[j])
						break;
				}
				if(j==5)
					break;
			}
			if(j==5){                                           //卡ID与保存的相同
				card_size=MFRC522_SelectTag(card_numberbuf);		//选卡
				
				status=MFRC522_Auth(0x60, 4, card_key0Abuf, RFID_CARD_ID1);//验正密钥是否相同
				if(status==MI_OK) {					//卡正确
					if(oled_show==0){
						oled.ctrl=OLED_CTRL_CLEAR;
						xReturn = xQueueSend( 	queue_oled,/* 消息队列的句柄 */
												&oled,		/* 发送的消息内容 */
												100);		/* 等待时间 100 Tick */
					}
					oled_show=1;
					oled.ctrl=OLED_CTRL_SHOW_STRING;
					oled.x=16;
					oled.y=3;
					oled.str=(uint8_t *)"ID KEY RIGHT";
					oled.font_size=16;
					xReturn = xQueueSend( 	queue_oled,/* 消息队列的句柄 */
											&oled,		/* 发送的消息内容 */
											100);		/* 等待时间 100 Tick */
					if(xReturn != pdPASS)
						printf("rtc send error\r\n");
					b.nums=1;b.times=100;
					xReturn = xQueueSend(queue_beep,&b,100);  //蜂鸣器响一次
					led.method=LOCK_STATUS_RFID;
					xReturn = xQueueSend(queue_led,&led,100);//解锁-------------------------------------------------------------
				}
				else{
					if(oled_show==0){
						oled.ctrl=OLED_CTRL_CLEAR;
						xReturn = xQueueSend( 	queue_oled,/* 消息队列的句柄 */
												&oled,		/* 发送的消息内容 */
												100);		/* 等待时间 100 Tick */
					}
					oled_show=3;
					oled.ctrl=OLED_CTRL_SHOW_STRING;
					oled.x=16;
					oled.y=3;
					oled.str=(uint8_t *)"KEY ERROR";
					oled.font_size=16;
					xReturn = xQueueSend( 	queue_oled,/* 消息队列的句柄 */
											&oled,		/* 发送的消息内容 */
											100);		/* 等待时间 100 Tick */
					if(xReturn != pdPASS)
						printf("nrf send error\r\n");
					b.nums=2;b.times=50;
					xReturn = xQueueSend(queue_beep,&b,100);  //蜂鸣器响两次
				}
			}else{
				if(oled_show==0){
					oled.ctrl=OLED_CTRL_CLEAR;
					xReturn = xQueueSend( 	queue_oled,/* 消息队列的句柄 */
											&oled,		/* 发送的消息内容 */
											100);		/* 等待时间 100 Tick */
				}
				oled_show=2;
				oled.ctrl=OLED_CTRL_SHOW_STRING;
				oled.x=25;
				oled.y=3;
				oled.str=(uint8_t *)"ID ERROR";
				oled.font_size=16;
				xReturn = xQueueSend( 	queue_oled,/* 消息队列的句柄 */
										&oled,		/* 发送的消息内容 */
										100);		/* 等待时间 100 Tick */
				if(xReturn != pdPASS)
					printf("nrf send error\r\n");
				b.nums=2;b.times=50;
				xReturn = xQueueSend(queue_beep,&b,100);  //蜂鸣器响两次
			}
			
		}
			
		vTaskDelay(300);
		if(oled_show==1||oled_show==2||oled_show==3){  //当前状态显示900ms
			times+=1;
			if(times==3){
				oled.ctrl=OLED_CTRL_CLEAR;
				xReturn = xQueueSend( 	queue_oled,/* 消息队列的句柄 */
										&oled,		/* 发送的消息内容 */
										100);		/* 等待时间 100 Tick */
				if(xReturn != pdPASS)
					printf("id card send error\r\n");
				if(oled_show==1)        //如果正确解锁
				{
					m_sta.menu=menu_main;
					menu_show(&m_sta);
				}else{
					m_sta.is_menu_show=0;
				}
				oled_show=0;times=0;
			}
			
		}
	}
}

void app_task_changecard(void *pvParameters)
{
	uint8_t  card_numberbuf[5]={0},data[2],status;
	eeprom_t et; beep b;
	EventBits_t EventValue;
	while(1)
	{
		EventValue=xEventGroupWaitBits((EventGroupHandle_t	)event_group,		
									   (EventBits_t			)EVENT_GROUP_RFID_CARD_CHANGE,
									   (BaseType_t			)pdFALSE,				
									   (BaseType_t			)pdFALSE,
									   (TickType_t			)portMAX_DELAY);
		
		if(rfid_change_flag==0){
			MFRC522_Initializtion();
			if(MFRC522_Request(0x52,data)==MI_OK&&(LOCK_STATUS==0))  //寻卡
			{
				status=MFRC522_Anticoll(card_numberbuf);			//获取ID
				if(status==0)
				{
					et.addr=rfid_card_id1;
					et.data=card_numberbuf;
					et.len=5;
					xQueueSend(queue_eeprom,&et,100);
					memcpy(RFID_CARD_ID1,card_numberbuf,5);
					send_messagebox("change ok");
					rfid_change_flag=1;
					b.nums=1;b.times=50;
					xQueueSend(queue_beep,&b,100);
//					xEventGroupClearBits(event_group,EVENT_GROUP_RFID_CARD_CHANGE);
//					vTaskResume(app_task_rfid_hander);
				}else{
					rfid_change_flag=2;
					b.nums=2;b.times=50;
					xQueueSend(queue_beep,&b,100);
					send_messagebox("change error\nplease Cancel and try again");
//					xEventGroupClearBits(event_group,EVENT_GROUP_RFID_CARD_CHANGE);
//					vTaskResume(app_task_rfid_hander);
				}
			}
		}
		vTaskDelay(300);
		
	}
}


/*按键对应
1	2	3	4
5	6	7	8
9	10	11	12
13	14	15	16
*/
void app_task_keyboard(void *pvParameters)
{
	uint32_t key,i=0;
	uint8_t pwd[8]={0};
	char pwdstr[9]={0};
	oled_t oled;
	led_t led;
	beep b;
	BaseType_t	xReturn;
	while(1)
	{
		key=key_board_scan();
		if(key){
			vTaskDelay(250);
			if(key<10&&m_sta.is_menu_show==0){          //为0~9数字密码
				if(i==0){        //先清屏
					memset(pwdstr,0,9);
					oled.ctrl=OLED_CTRL_CLEAR;
					xReturn = xQueueSend( 	queue_oled,/* 消息队列的句柄 */
											&oled,		/* 发送的消息内容 */
											100);		/* 等待时间 100 Tick */
				}
				b.nums=1;b.times=50;
				xReturn = xQueueSend(queue_beep,&b,100);  //蜂鸣器响一次
				pwdstr[i]=key+48;
				for(int j=i-1;j>=0;j--)
				{
					pwdstr[j]='*';
				}
				oled.ctrl=OLED_CTRL_SHOW_STRING;
				oled.x=16;
				oled.y=3;
				oled.str=(uint8_t *)pwdstr;
				oled.font_size=16;
				xReturn = xQueueSend(queue_oled,&oled,100);
				if(xReturn != pdPASS)
					printf("kb send error\r\n");
				
				pwd[i]=key;
				
				i++;
				if(i==8){ i=0;
					if(memcmp(pwd,PASSWORD,8)==0){
						led.method=LOCK_STATUS_KB;
						xReturn = xQueueSend(queue_led,&led,100);//解锁----------------------------------------------------------
						//OLED_Clear();
						oled.ctrl=OLED_CTRL_SHOW_STRING;
						oled.x=16;
						oled.y=3;
						oled.str=(uint8_t *)"PWD SUCCESS";
						oled.font_size=16;
						xReturn = xQueueSend( 	queue_oled,/* 消息队列的句柄 */
												&oled,		/* 发送的消息内容 */
												100);		/* 等待时间 100 Tick */
						if(xReturn != pdPASS)
							printf("pwd send error\r\n");
						b.nums=1;b.times=100;
						xReturn = xQueueSend(queue_beep,&b,100);  //蜂鸣器响一次
						vTaskDelay(900);
						oled.ctrl=OLED_CTRL_CLEAR;
						xReturn = xQueueSend( 	queue_oled,/* 消息队列的句柄 */
												&oled,		/* 发送的消息内容 */
												100);		/* 等待时间 100 Tick */
						m_sta.menu=menu_main;
						menu_show(&m_sta);
					}
					else{
						printf("pwd error\n");
						oled.ctrl=OLED_CTRL_SHOW_STRING;
						oled.x=16;
						oled.y=3;
						oled.str=(uint8_t *)"PWD ERROR";
						oled.font_size=16;
						xReturn = xQueueSend( 	queue_oled,/* 消息队列的句柄 */
												&oled,		/* 发送的消息内容 */
												100);		/* 等待时间 100 Tick */
						if(xReturn != pdPASS)
							printf("pwd send error\r\n");
						b.nums=2;b.times=50;
						xReturn = xQueueSend(queue_beep,&b,100);  //蜂鸣器响两次
						vTaskDelay(900);
						oled.ctrl=OLED_CTRL_CLEAR;
						xReturn = xQueueSend( 	queue_oled,/* 消息队列的句柄 */
												&oled,		/* 发送的消息内容 */
												100);		/* 等待时间 100 Tick */
						m_sta.is_menu_show=0;
					}
					
				}
			}else{
				i=0;                   //摁着摁着想起来带卡了
			}
			printf("%d\n",key);
		}
	}
}

void app_task_rtc(void *pvParameters)
{
	uint8_t buf[16]={0};
	oled_t oled;
	BaseType_t	xReturn;
	EventBits_t EventValue;
	while(1)
	{
		/* 等待事件组中的相应事件位，或同步 */
		EventValue=xEventGroupWaitBits((EventGroupHandle_t	)event_group,		
									   (EventBits_t			)EVENT_GROUP_RTC_WAKEUP,
									   (BaseType_t			)pdTRUE,				
									   (BaseType_t			)pdFALSE,
									   (TickType_t			)portMAX_DELAY);

		if(EventValue & EVENT_GROUP_RTC_WAKEUP)
		{
			RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure); 
			
			if(lv_sta.now==lv_sta.clock){                         //如果当前界面为钟表
				analog_update_time( BCD2DEC(RTC_TimeStructure.RTC_Hours),
									BCD2DEC(RTC_TimeStructure.RTC_Minutes),
									BCD2DEC(RTC_TimeStructure.RTC_Seconds));
			}
			if(oled_show_rtcflag==2)  //如果oled菜单显示时间
			{
				/* 格式化字符串 */
				sprintf((char *)buf,"%02x:%02x:%02x",	RTC_TimeStructure.RTC_Hours,
														RTC_TimeStructure.RTC_Minutes,
														RTC_TimeStructure.RTC_Seconds);

				/* oled显示时间 */
				oled.ctrl=OLED_CTRL_SHOW_STRING;
				oled.x=25;
				oled.y=3;
				oled.str=buf;
				oled.font_size=16;

				xReturn = xQueueSend( 	queue_oled,/* 消息队列的句柄 */
										&oled,		/* 发送的消息内容 */
										100);		/* 等待时间 100 Tick */
				if(xReturn != pdPASS)
					printf("rtc send error\r\n");
			}
			if(oled_show_rtcflag==1)  //如果oled菜单显示日期
			{
				RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure); 
				/* 格式化字符串 */
				sprintf((char *)buf,"20%02x-%02x-%02x-%d",	RTC_DateStructure.RTC_Year,
														 	RTC_DateStructure.RTC_Month,
															RTC_DateStructure.RTC_Date,
															RTC_DateStructure.RTC_WeekDay);

				/* oled显示时间 */
				oled.ctrl=OLED_CTRL_SHOW_STRING;
				oled.x=15;
				oled.y=3;
				oled.str=buf;
				oled.font_size=12;

				xReturn = xQueueSend( 	queue_oled,/* 消息队列的句柄 */
										&oled,		/* 发送的消息内容 */
										100);		/* 等待时间 100 Tick */
				if(xReturn != pdPASS)
					printf("rtc send error\r\n");
			}
		}
	}
}




void app_task_paj(void *pvParameters)
{
	uint8_t status;
	uint8_t data[2]={0x00};
	uint16_t gesture_data;
	
	Gesture_init();
	while(1)
	{
		//读取手势状态			
        status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);
		
		if(!status)
		{   
			gesture_data =(uint16_t)data[1]<<8 | data[0];
			
			if(gesture_data) 
			{
				switch(gesture_data)
				{
					case GES_UP:               
						printf("Up\r\n");
						if((lv_sta.isit_show&isit_show_clock)==0)
						{
							if(lv_sta.isit_show&isit_show_pwd){
								vTaskResume(app_task_gui_hander);
								openLCDBackLight(1);
								lv_change_face(lv_sta.now,lv_sta.inputpwd);
							}else{
								vTaskResume(app_task_gui_hander);
								openLCDBackLight(1);
							}
						}
						else if(lv_sta.now==lv_sta.clock){				   		//如果当前在钟表界面并且开启锁屏密码
							if(lv_sta.isit_show&isit_show_pwd){                    
								lv_change_face(lv_sta.now,lv_sta.inputpwd);
							}else{
								lv_change_face(lv_sta.now,lv_sta.mainwindow);
							}
						}
						
						break; //向上
					case GES_DOWM:                  
               			printf("Down\r\n");
						if(lv_sta.isit_show&isit_show_clock){
							lv_change_face(lv_sta.now,lv_sta.clock);         //关屏
						}else{
							vTaskSuspend(app_task_gui_hander);
							openLCDBackLight(0);
						}
						
						break; //向下
					case GES_LEFT:                       
  						printf("Left\r\n");                
						break; //向左
					case GES_RIGHT:               
                		printf("Right\r\n");               
						break; //向右
					case GES_FORWARD:                 
						printf("Forward\r\n");             
						break; //向前
					case GES_BACKWARD:            
            			printf("Backward\r\n");            
						break; //向后
					case GES_CLOCKWISE:             
                		printf("Clockwise\r\n");     
						break; //顺时针
					case GES_COUNT_CLOCKWISE:   
                   		printf("AntiClockwise\r\n");   
						break; //逆时针
					case GES_WAVE:                 
						printf("Wave\r\n");                
						break; //挥动
					default:   break;
				}		
			}
		}
		vTaskDelay(100);
	}
}

/*
0*3/sys/a13jDJtaInP/device1/thing/service/property/set
{"method":"thing.service.property.set","id":"1282715497","params":{"LEDSwitch":0,"Lock_control":1},"version":"1.0.0"}
*/


static void find_severbak_data(uint8_t *data,uint8_t *find,uint32_t find_len,uint8_t *find1,uint32_t find1_len,uint8_t **p1,uint8_t **p2)
{
	int i=0;
	
	for(i=0;i<RX3_data.a;i++)
	{
		if(memcmp(&data[i],find,find_len)==0)
		{
			*p1=(&data[i]+find_len);
		}
		if(memcmp(&data[i],find1,find1_len)==0)
		{
			*p2=(&data[i]+find1_len);
		}
		if((*p1!=NULL)&&(*p2!=NULL))
			break;
	}
}

void app_task_mqtt(void *pvParameters)
{
	uint32_t times=0,flame=0,gas=0;
	uint8_t *p1=NULL,*p2=NULL;
	led_t led;
	BaseType_t	xReturn;
	uint8_t messag[512]={0};
	while(1)
	{
		times++;
		if(RX3_data.a>50)
		{
			find_severbak_data((uint8_t*)RX3_data.data,(uint8_t*)"LEDSwitch",9,(uint8_t*)"Lock_control",12,&p1,&p2);
			
			if(p1!=NULL||p2!=NULL)
			{
				printf("*p=%c,%c\n",*(p1+2),*(p2+2));
				if(*(p1+2)=='0'){
					led.method=LOCK_STATUS_MQTT_LED_CLOSE;
					xReturn = xQueueSend(queue_led,&led,100);
				}
				else{
					led.method=LOCK_STATUS_MQTT_LED_OPEN;
					xReturn = xQueueSend(queue_led,&led,100);
				}
				if(*(p2+2)=='1'){
					led.method=LOCK_STATUS_MQTT_OPEN;
					xReturn = xQueueSend(queue_led,&led,100);
				}
				p1=NULL;p2=NULL;
			}
//			taskENTER_CRITICAL(); //进入临界区
//			for(int i=0;i<RX3_data.a;i++)
//			{
//				printf("%c",RX3_data.data[i]);
//			}
//			taskEXIT_CRITICAL(); //退出临界区
			usart3_clear();
		}
		if(flame_status<800)
			flame=1;
		else
			flame=0;
		if(gas_static>2000)
			gas=1;
		else
			gas=0;
		sprintf((char *)messag,PUBDATA,!((GPIOF->ODR&0x400)>>10),Temperature,Humidity,flame,gas,dis_static,!((GPIOF->ODR&0x200)>>9));
		mqtt_publish_data(PUBLIST,(char *)messag,0);
		if(times>9){    //每50秒发送心跳包
			printf("tick=%d\n",mqtt_tick());
			times=0;
		}
		vTaskDelay(5000);
	}
}


void app_task_eeprom(void *pvParameters)
{
	eeprom_t rom_t;
	BaseType_t	xReturn;
	
	while(1)
	{
		xReturn = xQueueReceive(queue_eeprom,&rom_t, portMAX_DELAY);
		if(xReturn != pdPASS)
			continue;
		
		taskENTER_CRITICAL(); //进入临界区
		IIC_DT=5;
		at24c02_write(rom_t.addr,rom_t.data,rom_t.len);
		taskEXIT_CRITICAL(); //退出临界区
		vTaskDelay(5);
	}
}

void app_task_bluetooth(void *pvParameters)
{
	struct RX_DATA u2_data;
	BaseType_t	xReturn;
	while(1)
	{
		xReturn = xQueueReceive(queue_usart2,&u2_data, portMAX_DELAY);
		if(xReturn != pdPASS)
			continue;
		printf("%s\n",u2_data.data);
		if(strstr("123","456"))
		{
			
		}
	}
}

void app_task_nrf24l01(void *pvParameters)
{
	int ret;
	beep b;
	uint8_t user[32]={0};
	uint8_t send[32]={'G','E','T'};
	EventBits_t EventValue;
	while(1)
	{
		EventValue=xEventGroupWaitBits((EventGroupHandle_t	)event_group,		
									   (EventBits_t			)EVENT_GROUP_NRF_CHACK,
									   (BaseType_t			)pdFALSE,				
									   (BaseType_t			)pdFALSE,
									   (TickType_t			)3500);
		
		
		ret=NRF24L01_TxPacket(send);
		
		printf("send=%x\n",ret);
		if(ret==TX_OK){
			NRF24L01_RX_Mode();          //发送完成设置为接受
			vTaskDelay(100);
			ret=NRF24L01_RxPacket(user);
			printf("resv %d\n",ret);
			if(ret==0)   //如果接受成功
			{
				flame_status = user[3]|(user[4]<<8);
				dis_static   = user[7]|(user[8]<<8);
				if(dis_static<150){       //距离过近
					b.nums=2;b.times=100;
					xQueueSend(queue_beep,&b,100);
					//xEventGroupSetBits(event_group,EVENT_GROUP_NRF_CHACK);
				}
				gas_static   = user[5]|(user[6]<<8);
				
				tab2_dev_ser_add_data(user);
			}
			NRF24L01_TX_Mode();
		}
		vTaskDelay(400);
	}
}


void vApplicationTickHook( void )
{
	lv_tick_inc(1);
}










