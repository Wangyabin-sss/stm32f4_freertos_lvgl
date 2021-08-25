#include "rtc.h"
#include "main.h"

RTC_DateTypeDef  RTC_DateStructure;
RTC_TimeTypeDef  RTC_TimeStructure;
volatile int alarm_ready=0;

void rtc_init(void)
{
	RTC_InitTypeDef  RTC_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	/* Enable the PWR clock ，使能电源控制时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	/* Allow access to RTC，允许访问备份电路 */
	PWR_BackupAccessCmd(ENABLE);


#if RTC_SOURCE
	/* 使能LSE*/
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* 检查该LSE是否有效*/  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

	/* 选择LSE作为RTC的硬件时钟源*/
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	

#else //若LSE无法工作，可用内部LSI
	/* 使能LSI*/
	RCC_LSICmd(ENABLE);
	
	/* 检查该LSI是否有效*/  
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

	/* 选择LSI作为RTC的硬件时钟源*/
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

#endif

	/* Enable the RTC Clock，使能RTC时钟 */
	RCC_RTCCLKCmd(ENABLE);
	
	/* Wait for RTC APB registers synchronisation，等待RTC相关寄存器就绪 */
	RTC_WaitForSynchro();
	
	/* Configure the RTC data register and RTC prescaler，配置分频还有时间格式 */
	/* ck_spre(1Hz) = RTCCLK(LSE) /((127 + 1)*(255 + 1))*/
#if RTC_SOURCE//LSE
	/* Configure the RTC data register and RTC prescaler，配置RTC数据寄存器与RTC的分频值 */
	RTC_InitStructure.RTC_AsynchPrediv = 127;				//异步分频系数
	RTC_InitStructure.RTC_SynchPrediv = 255;				//同步分频系数
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24小时格式
	RTC_Init(&RTC_InitStructure);
#else //LSI
	/* Configure the RTC data register and RTC prescaler，配置RTC数据寄存器与RTC的分频值 */
	RTC_InitStructure.RTC_AsynchPrediv = 127;				//异步分频系数
	RTC_InitStructure.RTC_SynchPrediv = 249;				//同步分频系数
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24小时格式
	RTC_Init(&RTC_InitStructure);

#endif
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x1688) //启用备份寄存器，防止每次复位都重新设置时间
	{
		/* Set the date: Sunday August 1th 2021 */
		RTC_DateStructure.RTC_Year = 0x21;
		RTC_DateStructure.RTC_Month = RTC_Month_August;
		RTC_DateStructure.RTC_Date = 0x01;
		RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Sunday;
		RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

		/* Set the time to 14h 59mn 55s PM */
		RTC_TimeStructure.RTC_H12     = RTC_H12_PM;
		RTC_TimeStructure.RTC_Hours   = 0x14;
		RTC_TimeStructure.RTC_Minutes = 0x59;
		RTC_TimeStructure.RTC_Seconds = 0x55; 
		RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x1688);
	}

	//关闭唤醒功能
	RTC_WakeUpCmd(DISABLE);
	
	//为唤醒功能选择RTC配置好的时钟源
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	//设置唤醒计数值为自动重载，写入值默认是0
	RTC_SetWakeUpCounter(1-1);
	
	//清除RTC唤醒中断标志
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	//使能RTC唤醒中断
	RTC_ITConfig(RTC_IT_WUT, ENABLE);

	//使能唤醒功能
	RTC_WakeUpCmd(ENABLE);

	/* Configure EXTI Line22，配置外部中断控制线22 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;			//当前使用外部中断控制线22
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		//上升沿触发中断 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			//使能外部中断控制线22
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;		//允许RTC唤醒中断触发
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x8;	//抢占优先级为0x8
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		//响应优先级为0x0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//使能
	NVIC_Init(&NVIC_InitStructure);
}


void RTC_WKUP_IRQHandler(void)
{
	uint32_t ulReturn;
	
	/* 进入临界段，临界段可以嵌套 */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(RTC_GetITStatus(RTC_IT_WUT) == SET)
	{
		xEventGroupSetBitsFromISR(event_group,EVENT_GROUP_RTC_WAKEUP,NULL);
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
	}
	/* 退出临界段 */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
}

void set_rtc_time(RTC_TimeTypeDef *RTC_Time,RTC_DateTypeDef *RTC_Date,uint32_t set)
{
	if(set==ONLY_TIME){
		RTC_SetTime(RTC_Format_BCD,RTC_Time);
	}
	else if(set==ONLY_DATA){
		RTC_SetDate(RTC_Format_BCD,RTC_Date);
	}
	else if(set==TIME_DATA){
		RTC_SetTime(RTC_Format_BCD,RTC_Time);
		RTC_SetDate(RTC_Format_BCD,RTC_Date);
	}
}

/*********************************************************闹钟*************************************************************/
void rtc_alarm_a_init(void)
{
//	RTC_AlarmTypeDef  RTC_AlarmStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
#if 0
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_PM;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = 0x15;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x00;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x00;


	//RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
	//RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;	//屏蔽日期和星期，就是闹钟每天都生效

	RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&RTC_AlarmStructure);

	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
#endif	
	/* Enable RTC Alarm A Interrupt，使能闹钟A的中断 */
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	
	RTC_ClearFlag(RTC_FLAG_ALRAF);

	
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable the RTC Alarm Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void rtc_SetAlarm(uint32_t hours,uint32_t minuts,uint32_t sec)
{
	RTC_AlarmTypeDef  RTC_AlarmStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
	
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_PM;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = hours;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = minuts;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = sec;

	//RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
	//RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;	//屏蔽日期和星期，就是闹钟每天都生效

	RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&RTC_AlarmStructure);

	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	
}

void RTC_Alarm_IRQHandler(void)
{

	if(RTC_GetITStatus(RTC_IT_ALRA) == SET)
	{
		alarm_ready=1;
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		EXTI_ClearITPendingBit(EXTI_Line17);
	}
}

uint32_t DEC2BCD(uint32_t dec)
{
	uint32_t declen=0,tmp=dec,a[10]={0},data=0;
	while(tmp!=0)
	{
		a[declen]=tmp%10;
		declen++;
		tmp=tmp/10;
	}
	for(int i=declen-1;i>=0;i--)
	{
		data |=a[i]<<4*i;
	}
	return data;
}

//仅支持当前的rtc数据
uint32_t BCD2DEC(uint32_t bcd)
{
	if((bcd>>4)!=0)
		return (bcd>>4)*10+(bcd&0xf);
	else
		return bcd;
}









