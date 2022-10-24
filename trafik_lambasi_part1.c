#include "stm32f10x.h"
#include "delay.h"
//first part

//button and leds defining
#define Button GPIO_Pin_0
#define Green GPIO_Pin_2
#define Yellow GPIO_Pin_4
#define Red GPIO_Pin_6

int last_Led = 0;
int state = 0;
int timer = 0;

GPIO_InitTypeDef GPIO_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;

void GPIO_config_output(void);
void GPIO_config_input(void);
void NVIC_config(void);
void EXTI0_IRQHandler(void);

int main()
{
	delayInit();
	GPIO_config_output();
	GPIO_config_input();
	NVIC_config();
	
	while(1){
		
		switch(state){
			case 1:
				GPIO_SetBits(GPIOA, Yellow);
				delayMs(2000);
				GPIO_ResetBits(GPIOA, Yellow);
				if(last_Led == 1){state = 0;}
				else{state = 2;}
				break;
				
			case 2:
				GPIO_SetBits(GPIOA, Green);
				timer = 0;
			
			while(timer<10000){
			delayMs(1);
				timer++;
			}
			GPIO_ResetBits(GPIOA, Green);
			last_Led = 1;
			state = 1;
			break;
			
			default:
			GPIO_SetBits(GPIOA, Red);
			delayMs(10000);
			GPIO_ResetBits(GPIOA, Red);
			last_Led = 0;
			state = 1;
			break;
		}
	}
}

void GPIO_config_input(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = Button;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
//A port settings for outputs
void GPIO_config_output(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = Yellow | Green | Red;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void NVIC_config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	
	NVIC_Init(&NVIC_InitStruct);
}
void EXTI0_IRQHandler(void){
	
	if((state == 2)&&(EXTI_GetITStatus(EXTI_Line0) != RESET)){
//in 109. is interrupt
		state=1;
		timer=10000;
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}
