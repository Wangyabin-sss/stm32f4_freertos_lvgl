#include "Sys.h"

#if OS_SUPPORT
#include "FreeRTOS.h"				
#include "task.h"

void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;		//系统定时器的重载值    	 
	ticks=nus*(SystemCoreClock/1000000);//需要的节拍数 
	told=SysTick->VAL;        			//刚进入时的计数器值
	
	/* 挂起调度器[可选,会导致高优先级任务无法抢占当前任务，但能够提高当前任务时间的精确性] */
	vTaskSuspendAll();	
	
	while(1)
	{
		tnow=SysTick->VAL;
		
		if(tnow!=told)
		{	 
			/* SYSTICK是一个递减的计数器 */
			if(tnow<told)
				tcnt+=told-tnow;		
			else 
				tcnt+=reload-tnow+told;	  
			
			told=tnow;
			
			/* 时间超过/等于要延迟的时间,则退出。*/
			if(tcnt>=ticks)
				break;			
		}  
	}

	/* 恢复调度器[可选] */
	xTaskResumeAll();
}

void delay_ms(uint32_t nms)
{	
	vTaskDelay(nms);
}
#else
void delay_ms(uint16_t nms)
{
	SysTick->CTRL = 0; 			   //关闭定时器
	SysTick->LOAD = SystemCoreClock/1000-1;  //设置重装载值
	SysTick->VAL=0;          //清零计数值
	SysTick->CTRL = 5; 		 //开启定时器
	while(nms--)
	{
		while((SysTick->CTRL&0x10000)==0); //检测flag位，计数到零时该位清零
	}
	SysTick->CTRL = 0;
}


void delay_us(uint16_t nus)
{
	SysTick->CTRL = 0; 			
	SysTick->LOAD = SystemCoreClock/1000000*nus-1; //168*nus < 2^24
	SysTick->VAL=0;
	SysTick->CTRL = 5; 		
	while((SysTick->CTRL&0x10000)==0);
	SysTick->CTRL = 0;
}
#endif




