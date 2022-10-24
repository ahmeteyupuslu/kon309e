#include "stm32f10x.h"
#include "delay.h"
//second part
//button and leds defining
#define Button GPIO_Pin_0
#define Green GPIO_Pin_2
#define Yellow GPIO_Pin_4
#define Red GPIO_Pin_6
#define ped_red GPIO_Pin_1
#define ped_green GPIO_Pin_3

volatile uint32_t k = 0;
volatile uint32_t i = 0;
volatile int state = 0;
volatile char last_Led = 0;
volatile char cont = 1;

GPIO_InitTypeDef GPIO_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;
TIM_TimeBaseInitTypeDef TIM_InitStruct;

void GPIO_config_output(void);
void GPIO_config_input(void);
void NVIC_config(void);
void EXTI0_IRQHandler(void);
void TIM_config(void);
void TIM2_IRQHandler(void);

int main(){
	GPIO_config_input();
	GPIO_config_output();
	TIM_config();
	NVIC_config();
	
	while(1){
		switch(state){
			case 1:
				while((k<1)&&(cont == 1))
				{i=10;}
				GPIO_ResetBits(GPIOA,Red);
				GPIO_ResetBits(GPIOA,ped_green);
				GPIO_ResetBits(GPIOA,Green);
				GPIO_ResetBits(GPIOA,ped_red);
				
				GPIO_SetBits(GPIOA,Yellow);
				GPIO_SetBits(GPIOA,ped_red);
				
				cont=0;
				break;
				
			case 2:
				last_Led=1;
				GPIO_ResetBits(GPIOA,Yellow);
				GPIO_ResetBits(GPIOA,Green);
			break;
			
			default:
				last_Led=0;
				GPIO_ResetBits(GPIOA,ped_red);
				GPIO_ResetBits(GPIOA,Yellow);
				GPIO_SetBits(GPIOA,Red);
				GPIO_SetBits(GPIOA,ped_green);
			break;
		}
	}
}
//a port for input
void GPIO_config_input(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = Button;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
//a port for output
void GPIO_config_output(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = Yellow | Green | Red | ped_green | ped_red;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
//interrupt settings
void NVIC_config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;	
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;	
	NVIC_Init(&NVIC_InitStruct);
	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	
	EXTI_Init(&EXTI_InitStruct);
}
void TIM_config(void){
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = 1999;
	TIM_InitStruct.TIM_Prescaler = 35999;
	TIM_InitStruct.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM2,&TIM_InitStruct);
	TIM_Cmd(TIM2,ENABLE);
}

void EXTI0_IRQHandler(void){
	if((state==2)&&(i<8)&&(EXTI_GetITStatus(EXTI_Line0) != RESET))
	{//a
		//a
		//a
		k=0;
		state=1;
		cont=1;
}
EXTI_ClearITPendingBit(EXTI_Line0);
}
void TIM2_IRQHandler(void){
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		k++;
		i++;
	//a
	if(i%10==0){
		state=1;
	}
	else if (i%12==0){
		if(last_Led==1){
			state=0;
		}
		else{
			state=2;
		}
		i=0;
	}
}

