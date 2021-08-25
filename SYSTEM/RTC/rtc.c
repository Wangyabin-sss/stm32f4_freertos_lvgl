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
	/* Enable the PWR clock ��ʹ�ܵ�Դ����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	/* Allow access to RTC��������ʱ��ݵ�· */
	PWR_BackupAccessCmd(ENABLE);


#if RTC_SOURCE
	/* ʹ��LSE*/
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* ����LSE�Ƿ���Ч*/  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

	/* ѡ��LSE��ΪRTC��Ӳ��ʱ��Դ*/
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	

#else //��LSE�޷������������ڲ�LSI
	/* ʹ��LSI*/
	RCC_LSICmd(ENABLE);
	
	/* ����LSI�Ƿ���Ч*/  
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

	/* ѡ��LSI��ΪRTC��Ӳ��ʱ��Դ*/
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

#endif

	/* Enable the RTC Clock��ʹ��RTCʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	
	/* Wait for RTC APB registers synchronisation���ȴ�RTC��ؼĴ������� */
	RTC_WaitForSynchro();
	
	/* Configure the RTC data register and RTC prescaler�����÷�Ƶ����ʱ���ʽ */
	/* ck_spre(1Hz) = RTCCLK(LSE) /((127 + 1)*(255 + 1))*/
#if RTC_SOURCE//LSE
	/* Configure the RTC data register and RTC prescaler������RTC���ݼĴ�����RTC�ķ�Ƶֵ */
	RTC_InitStructure.RTC_AsynchPrediv = 127;				//�첽��Ƶϵ��
	RTC_InitStructure.RTC_SynchPrediv = 255;				//ͬ����Ƶϵ��
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24Сʱ��ʽ
	RTC_Init(&RTC_InitStructure);
#else //LSI
	/* Configure the RTC data register and RTC prescaler������RTC���ݼĴ�����RTC�ķ�Ƶֵ */
	RTC_InitStructure.RTC_AsynchPrediv = 127;				//�첽��Ƶϵ��
	RTC_InitStructure.RTC_SynchPrediv = 249;				//ͬ����Ƶϵ��
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24Сʱ��ʽ
	RTC_Init(&RTC_InitStructure);

#endif
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x1688) //���ñ��ݼĴ�������ֹÿ�θ�λ����������ʱ��
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

	//�رջ��ѹ���
	RTC_WakeUpCmd(DISABLE);
	
	//Ϊ���ѹ���ѡ��RTC���úõ�ʱ��Դ
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	//���û��Ѽ���ֵΪ�Զ����أ�д��ֵĬ����0
	RTC_SetWakeUpCounter(1-1);
	
	//���RTC�����жϱ�־
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	//ʹ��RTC�����ж�
	RTC_ITConfig(RTC_IT_WUT, ENABLE);

	//ʹ�ܻ��ѹ���
	RTC_WakeUpCmd(ENABLE);

	/* Configure EXTI Line22�������ⲿ�жϿ�����22 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;			//��ǰʹ���ⲿ�жϿ�����22
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		//�����ش����ж� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			//ʹ���ⲿ�жϿ�����22
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;		//����RTC�����жϴ���
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x8;	//��ռ���ȼ�Ϊ0x8
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		//��Ӧ���ȼ�Ϊ0x0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ʹ��
	NVIC_Init(&NVIC_InitStructure);
}


void RTC_WKUP_IRQHandler(void)
{
	uint32_t ulReturn;
	
	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(RTC_GetITStatus(RTC_IT_WUT) == SET)
	{
		xEventGroupSetBitsFromISR(event_group,EVENT_GROUP_RTC_WAKEUP,NULL);
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
	}
	/* �˳��ٽ�� */
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

/*********************************************************����*************************************************************/
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
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;	//�������ں����ڣ���������ÿ�춼��Ч

	RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&RTC_AlarmStructure);

	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
#endif	
	/* Enable RTC Alarm A Interrupt��ʹ������A���ж� */
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
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;	//�������ں����ڣ���������ÿ�춼��Ч

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

//��֧�ֵ�ǰ��rtc����
uint32_t BCD2DEC(uint32_t bcd)
{
	if((bcd>>4)!=0)
		return (bcd>>4)*10+(bcd&0xf);
	else
		return bcd;
}









