#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
//#include "beep.h"


//外部中断0服务程序
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

    //GPIOE.2 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   //GPIOE.3	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   //GPIOE.4	  中断线以及中断初始化配置  下降沿触发	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


   //GPIOA.0	  中断线以及中断初始化配置 上升沿触发 PA0  WK_UP
 	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); 

  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//使能按键KEY2所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
}

//外部中断0服务程序 
void EXTI0_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(WK_UP==1)	 	 //WK_UP按键
	{				 
		//BEEP=!BEEP;	
	}
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
}
 
//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY2==0)	  //按键KEY2
	{
		//LED0=!LED0;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line2);  //清除LINE2上的中断标志位  
}

u8 btn=0;
extern u8 offled1;
u8 buf[6]={0x5b,0x1c,0x1b,0x1c,0x00,0x5f};

//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	delay_ms(10);//消抖
//	if(KEY1==0)	 //按键KEY1
//	{				 
//		LED1=!LED1;
//	}		 
	if(KEY1==0 && offled1 ==1)		//按键KEY1
	{
		 // printf("exti btn!\r\n");

			TIM_Cmd(TIM3, DISABLE); 
			LED1=1;
			offled1=0;
			
		for(u8 i=0;i<6;i++)
		{	
			USART_SendData(USART1,buf[i]);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		}
	}		
	EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
}

void EXTI4_IRQHandler(void)
{
	delay_ms(100);//消抖
	//u8 key =KEY_Scan(0);
	//if(key==1 && offled1 ==1)	
	if(KEY0==0 && offled1 ==1)		//按键KEY0
	{
		 // printf("exti btn!\r\n");

			TIM_Cmd(TIM3, DISABLE); 
			LED1=1;
			offled1=0;
			
		for(u8 i=0;i<6;i++)
		{	
			USART_SendData(USART1,buf[i]);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		}
	}		 
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位  
}
 
