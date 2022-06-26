#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "exti.h"


u8 USART_TX_BUF[6]={0x5b,0x1c,0x1b,0x00,0x00,0x5f};
int main(void)
{		
	delay_init();	    	 //init delay function   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //set NVIC_Priority NVIC_PriorityGroup_2 设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //set boudrate of UART1 to 115200
 	LED_Init();			     //init led
	KEY_Init();          //init buttun
	TIM3_Int_Init(5000-1,7200-1);	//set time3 500ms
	EXTIX_Init();      //init extern interrupt
	//LED1=0;
	LED2=1;
 	while(1)
	{			
				if(USART_RX_BUF[0]==0x5A && USART_RX_BUF[5]==0x5F)//receive msg from host
				{
					if(USART_RX_BUF[1]==0x1C)//connect
					{
						if(USART_RX_BUF[2]==0x1B)//buttun ack from host
						{
							 u8 cnt=0;
							 while(cnt++<3)
							 {
								 LED1=!LED1;
								 delay_ms(330);
							 }
						}
						LED2=0;
						TIM_Cmd(TIM3, DISABLE); 
						LED1=1;
					}
					else if(USART_RX_BUF[1]==0x1D)//disconnect
					{
					  LED2=1;
						TIM_Cmd(TIM3, ENABLE); 
					}
					for(u8 i=0;i<8;i++)//clear reveive buffer
						USART_RX_BUF[i]=0;
				}

				if(btn==1)//send msg to host if buttun was pressed
				{
					//printf("btn pressed\r\n");
					for(u8 i=0;i<6;i++)
					{
						USART_SendData(USART1,USART_TX_BUF[i]);
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
					}
					btn=0;
				}
			  delay_ms(10);   
	 }	 
}

