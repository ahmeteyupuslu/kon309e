#include "stm32f10x.h"
#include "delay.h"

//button status flag value
#define button_pressed 1
#define button_not_pressed 2
// led status flag values 
#define redstatus 0 
#define yellow1status 1 
#define yellow2status 2 
#define greenstatus 3
//button and leds defining
#define Button GPIO_Pin_0
#define Green GPIO_Pin_2
#define Yellow GPIO_Pin_4
#define Red GPIO_Pin_6
#define ped_red GPIO_Pin_1
#define ped_green GPIO_Pin_3

GPIO_InitTypeDef GPIO_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;
TIM_TimeBaseInitTypeDef TIM_InitStruct;

int i;
extern int led_status;
//led status flag 
int led_status = redstatus; 
//button status flag 
int button_status = button_not_pressed; 
//counters for timer interrupt 
int counter = 0; 
int counter2 = 0; 

void GPIO_config(void);
void NVIC_config(void);
void EXTI0_IRQHandler(void);
void TIM_config(void);
void TIM2_IRQHandler(void);
void EXTI_config(void);

int main(void){
	GPIO_config();
	TIM_config();
	NVIC_config();
	EXTI_config();
	
	while(1){
		switch(led_status){
			case redstatus:
				GPIO_ResetBits(GPIOA,Yellow);
				GPIO_SetBits(GPIOA,Red);
				GPIO_ResetBits(GPIOA,ped_red);
				GPIO_SetBits(GPIOA,ped_green);
				break;
				
			case yellow1status:
				GPIO_ResetBits(GPIOA,Red);
				GPIO_SetBits(GPIOA,Yellow);
				GPIO_ResetBits(GPIOA,ped_green);
				GPIO_SetBits(GPIOA,ped_red);
			break;
			
			case yellow2status:
				GPIO_ResetBits(GPIOA,Green);
				GPIO_SetBits(GPIOA,Yellow);
			break;
			
			case greenstatus:
				GPIO_ResetBits(GPIOA,Yellow);
				GPIO_SetBits(GPIOA,Green);
				break;
		}
	}
}
//a port for input
void GPIO_config(){ 
// enable GPIO A and AFIO A clocks 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 
// enable APB1 TIM2 clock 
// required for timer interrupt 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
// button 
// A0 input pull down 
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0; 
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD; 
		GPIO_Init(GPIOA, &GPIO_InitStruct); 
// car red light 
// A1 output push-pull @2MHz 
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6; 
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz; 
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 
		GPIO_Init(GPIOA, &GPIO_InitStruct); 
// car yellow light 
// A2 output push-pull @2MHz 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4; 
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
// car green light 
// A3 output push-pull @2MHz 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
// pedestrian red light 
// A4 output push-pull @2MHz 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStruct); 

// pedestrian green light 
// A5 output push-pull @2MHz 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3; 
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
}
//interrupt settings
void NVIC_config(){
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;	
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;	
	NVIC_Init(&NVIC_InitStruct);
}
void EXTI_config(){ 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 
	EXTI_DeInit(); 
	EXTI_InitStruct.EXTI_Line = EXTI_Line0; 
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; 
	EXTI_InitStruct.EXTI_LineCmd = ENABLE; 
	EXTI_Init(&EXTI_InitStruct); 
} 
void TIM_config(void){
	TIM_InitStruct.TIM_ClockDivision = 0;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = 4999;
	TIM_InitStruct.TIM_Prescaler = 143;
	
	TIM_TimeBaseInit(TIM2,&TIM_InitStruct);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}

void EXTI0_IRQHandler() 
{ 
if (EXTI_GetITStatus(EXTI_Line0) != RESET) //if button pressed 
{ 
if (led_status == greenstatus) //and if the current state of the led is green 
{ 
// button flag up 
// will be used in timer interrupt handler 
button_status = button_pressed; 
} 
// Clear interrupt flag 
EXTI_ClearITPendingBit(EXTI_Line0); 
} 
} 
//timer interrupt 
void TIM2_IRQHandler() 
{ 
if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) //if timer interrupt triggered 
{ 
// keep counting 
counter++; 
// counter == 500 means 5s as we use timer interrupt in 100hz 
//for the first 5s led status is red 
if (counter <= 500) 
{ 
led_status = redstatus; 
} 
// then for 2s led status is yellow1 
else if (counter > 500 && counter <= 701) 
{ 
led_status = yellow1status; 
} 
// then for 5s led status is green 
else if (counter > 701 && counter <= 1202) 
{ 
led_status = greenstatus; 
if (button_status == button_pressed) //if button flag up (it comes from exti) 
{ 
// start another counter to control jump to yellow delay 
counter2++; 
if (counter2 == 200) //it waits 2sn as it is wanted 
{ 
//set the counter to normal operation yellow2 state entry time 
counter = 1203; 
//clean the counter2 
counter2	= 0; 
//clear the button flag 
button_status = button_not_pressed; 
} 
} 
} 
//after green for 2sn led status is yellow2 
else if (counter > 1202 && counter <= 1403) 
{ 
led_status = yellow2status; 
} 
else if (counter >= 1404) 
{ 
//clear the counter at the end to make it go back to led_state red 
counter = 0; 
} 
//clear timer interrupt flag 
TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
	} 
}

