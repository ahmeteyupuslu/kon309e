//part 1


#include "stm32f10x.h"
#include "delay.h"

uint32_t button_1 = 0;
uint32_t button_end = 0;
uint32_t button_sta = 0;
uint32_t button_2 = 0;
uint32_t button_2_end = 0;
uint32_t button_2_sta = 0;
uint32_t button_3 = 0;
uint32_t button_3_end = 0;
uint32_t button_3_sta = 0;
int counter = 0;

int sign=0;
volatile int state = 1;
int LED = 0;
volatile int Brig = 0;
volatile int level = 1;
int timer = 0;
int indicator = 0;
volatile int k = 1;

GPIO_InitTypeDef GPIO_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;
TIM_TimeBaseInitTypeDef TIM_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;
TIM_OCInitTypeDef TIM_OC_InitStruct;


void EXTI0_IRQHANDLER(void);
void TIM4_IRQHANDLER(void);
void EXTI3_IRQHANDLER(void);
void EXTI4_IRQHANDLER(void);
void EXTI2_IRQHANDLER(void);
void TIM4_config();
void NVIC_config();


int main(void)
{
    delayInit();

    RCC_APB2PeripClockCmd(RCC_APB2Periph_GPIOB, ENABLE);            //Enable clock for portA

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;                    //output mode as push-pull
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin 6|GPIO_Pin_7|GPIO_Pin_8;    //LEDs pin
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;                   //configuration MHz of the pins as output

    GPIO_Init(GPIOB, &GPIO_InitStruct);                             //Port B


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO, ENABLE);            //Eneable clock for portA
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;                      //Push_Pull input mode

    GPIO_InitStruct_GPIO_Speed = GPIO_Speed_2MHz;                   //configuration MHz of the pin_0
    PIO_Init(GPIOA, &GPIO_InitStruct);                              //Port A

    button_1 = GPIO_ReadInput_DataBit(GPIOA, GPIO_Pin_3);
    button_2 = GPIO_ReadInput_DataBit(GPIOA, GPIO_Pin_4);
    button_3 = GPIO_ReadInput_DataBit(GPIOA, GPIO_Pin_2);


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    EXTI_InitStruct.EXTI_Line = EXTI_Line3 | EXTI_Line4 | EXTI_Line2;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_Init(&EXTI_InitStruct);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
    NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
    NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        //Timer 72mhz
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStruct.TIM_Period = 9;                          //1 Seconds
    TIM_InitStruct.TIM_Prescaler == 35999;                  //max 65536
    TIM_InitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_InitStruct);
    TIM_Cmd(TIM4, ENABLE);
    TIM_OC_InitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OC_InitStruct.TIM_OutputState = ENABLE;
    TIM_OC_InitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM4, &TIM_OC_InitStruct);
    TIM_OC2Init(TIM4, &TIM_OC_InitStruct);
    TIM_OC3Init(TIM4, &TIM_OC_InitStruct);

    delayInit();

    while(1)
    {
        switch(state)
        {
            case 1:
            TIM4->CCR3 = 0;
            TIM4->CCR2 = 0;
            TIM4->CCR1 = level;
                break;
            case 2:
            TIM4->CCR3 = 0;
            TIM4->CCR1 = 0;
            TIM4->CCR2 = level;
                break;
            case 3:
            TIM4->CCR1 = 0;
            TIM4->CCR2 = 0;
            TIM4->CCR3 = level;
                break;
            default:
            TIM4->CCR1 = 0;
            TIM4->CCR2 = 0;
            TIM4->CCR3 = 0;
            break;
        }
    }
}

void EXTI3_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line3) != RESET){

        state++;
        Brig = 1;
        level = 1;
        while(k<600000 {k++;}
        k=0;
        
        if (state==4){state = 0;}   //cycling

        EXTI_Clear_ITPendingBit(EXTI_Line3);    //clear interrupt flag
    }
    }
    void EXTI4_IRQHandler(void
    {
        if(EXTI_GetITStatus(EXTI_Line4) != RESET){
            Brig++;

            while (k <600000) {k++;}
            k=0;

            if(Brig==0){level = 0;}
            if(Brig==1){level = 1;}
            if(Brig==2){level = 3;}
            if(Brig==3){level = 5;}
            if(Brig==4){level = 9;}
            if(Brig==5){level = 0; Brig 0;}
            
            EXTI_ClearITPendingBit(EXTI_Line4);
            
        }
    }
    void EXTI2_IRQHandler(void)
    {
        if(EXTI_GetITStatus(EXTI_Line2) != RESET){
            Brig++;

            while (k<600000) {k++;}
            k = 0;

            if(Brig==0){level = 0;}
            if(Brig==1){level = 9;}
            if(Brig==2){level = 5;}
            if(Brig==3){level = 3;}
            if(Brig==4){level = 1;}
            if(Brig==5){level = 0; Brig 0;}

            EXTI_ClearITPendingBit(EXTI_Line2);

        }
    }
