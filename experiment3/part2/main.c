#include "stm32f10x.h"
#include "delay.h"

#define poten GPIO_Pin_O //AO

uint32_t button_la 0;
uint32t button end 0;
uint32_t button_sta 0;
int counter ■ 0;

int signa0;
volatile int state a 1;
int LED a 0;
volatile int Brig	n;
volatile int level a 1;
int timer a 0;
int indicator a 0;
volatile int k a 1;
volatile uint16 t ADCPot;

GPIO_InitTypeDef GPIO_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;
TIM_TimeBaseInitTypeDef TIM_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;
TIM_OCInitTypeDef TIM_OC_InitStruct;
ADC InitTypeDef ADC InitStruct;


void TIM4IRQHandler(void);
void EXTI3_IRQHandler(void);
void TIM4_config();
void NVIC_config();
void ADC_call(void);
uint16_t ADCGetValue()

ADC_SoftwareStartConvemd(ADCLENABLE);
while(ADC_GetFlagStatus(ADCLADC_FLAG_EOC) aaRESET);
return ADC GetConversionValue(ADC1);
)

void ADC_call(void)

ADCPota ADCGetValue(ADC1.)-;
if(ADCPot <a 1024)
(
level- 0;
)
	else if(1024 < ADCPot && ADCPot Ca 2048)
(
level a 1;
	51	)
	52	else if(2048 < ADCPot && ADCPot Ca 3072)
	53	(
	54	level- 4;
	55	)
	56	else if(3072 < ADCPot && ADCPot Ca 4096)
	57	(
	58	level- 9;
	59	)
60
	61	while(ADC_GetFlagStatus(ADCLADC_FLAG_EOC as RESET));
	62	)
63 int main(void)
	64	{
	65	delayInit();
66
	67	ACC APB2Periphelockemd(RCC_APB2Periph_GPIOB, ENABLE); // Enable clock for PortA
68
	69	GPIO_InitStruct.GPIO_Mode a GPIO_Mode_AF_PP; // Output Mode as Push-Pull
	70	GPIO_InitStruct.GPIO_Pin a GPIO_Pin_6I GPIO_Pin_7 I GPIO_Pin_8; //LEDs pin
	71	GPIO InitStruct.GPIO Speed a GPIO Speed 2MHz; //configuration MHz of the pins as output
72
	73	GPIO_Init(GPIOB, &GPIO_InitStruct); // port B
74
75
	76	ACC APB2Periphelockemd(RCC APB2Periph_GPIOAIRCC APB2PeriphADC1 , ENABLE); //Enable clock for PortA
	77	GPIoInitStruct.GPIOPin a-GPIO_Pin_3;
    	78	GPIO_InitStruct.GPIO_Mode GPIO_Mode_IPD; // Push_Pull Input Mode
	79	//Conf. Pin_O
	80	GPIO_InitStruct.GPIO_Speed GPIO_Speed_2MHz; //Conf. MHz of the Pin_O
	81	GPIO_Init(GPIOA, &GPIO_InitStruct); // Port A
82
	83	button _l GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3); //
	84	GPIO_InitStruct.GPIO_Pin■poten;
85 GPIO_InitStruct.GPIO_ModewGPIO_Mode_AIN;
86 GPIO InitStruct.GPIO Speed w GPIO Speed 2MHz;
87 GPIOInit(GPIOA,&GPIOInitStruct);
88
89
90
	91	Rer_APR7Ppriphrinekemd(RCC_APR2Pwriph_AFTO, RNART,R);
	92	EXTI_InitStruct.EXTI_Line EXTI_Line3 I EXTI_Line4 I EXTI_Line2;
	93	EXTI_InitStruct.EXTI_LineCmd w ENABLE;
	94	EXTI_InitStruct.EXTI_Mode EXTI_Mode_Interrupt;
	95	EXTI_InitStruct.EXTI_Trigger EXTI_Trigger_Rising;
	96	EXTI_Init(&EXTI InitStruct);
	97	NVICInitStruct.NVICIRQChannel EXTI3IRQn;
	98	NVIC_InitStruct.NVIC_IRQChannelCmd w ENABLE;
	99	NVICInitStruct.NVIC_IRQChannelPreemptionPriority w 0;
	100	NVIC_InitStruct.NVIC_IRQChannelSubPriority w 0;
	101	NVICInit(&NVICInitStruct);
102
103
	104	ACC APB1Periphelockand(RCC_APB1Periph_TIM4, ENABLE);
105
	106	TIM_InitStruct.TIM_ClockDivision TIM_CKD_DIV1; //TImer 72Mhz
	107	TIM_InitStruct.TIM CounterMode w TIM CounterMode_Up;
	108	TIM InitStruct.TIM Period w 9; // 1 seconds
	109	TIM_InitStruct.TIM_Prescaler w 35999; //max 65536
	110	TIM_InitStruct.TIM RepetitionCounter w 0;
	111	TIM_TimeBaseInit(TIM4, &TIM_InitStruct);
	112	TIM_Cmd(TIM4, ENABLE);
	113	TIM_OC_InitStruct.TIM_OCMode TIM_OCMode_PWM1;
	114	TIM_OC_InitStruct.TIM_OutputState w ENABLE;
	115	TIM OC InitStruct.TIM OCPolarity w TIM OCPolarity High;
	116	TIM-OCTInit(TIM4, &TIM OC_InitStruct);
	117	TIMIOC2Init(TIM4, &TIMIOC_InitStruct);
	118	TIM_OC3Init(TIM4, &TIM_OC_InitStruct);
119
	120	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
121 ADC_InitStruct.ADC_Mode ADC_Mode_Independent;
122 ADC InitStruct.ADC ContinuousConvModewENABLE;
123 ADC_InitStruct.ADC_ExternalTrigConv ADC_ExternalTrigConv_None;
124 ADC_InitStruct.ADC_DataAlign ADC_DataAlign_Right;
125 ADC_InitStruct.ADC_NbrOfChannel w 1;
126 ADC_Init(ADC1, &ADC_InitStruct);
127 ADC RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5);
128 ADC_Cmd(ADC1, ENABLE);
129 ADC_ResetCalibration(ADC1);
130 while(ADCGetResetCalibrationStatus(ADC1));
131 ADC_5tartCalibration(ADC1);
132 while(ADCGetCalibrationStatus(ADC1));
133 // Start the conversion
134 ADC_SoftwareStartConvand(ADC1, ENABLE);
135
136
	137	delayInit();
138
	139	while(1)
140
	141	switch(state)
142
	143	case 1:
	144	ADC call();
	145	TIM4->CCR3 w 0;
	146	TIM4->CCR2 w 0;
	147	TIM4->CCR1 w level;
	148	break;
	149	case 2:
	150	ADC call();
	151	TIM4->CCR3 w 0;
	152	TIM4->CCR1 w 0;
	153	TIM4->CCR2 w level;
	154	break;
155	case 3:
156	ADC call();
157	TIM4->CCR1 in 0;
158	TIM4->CCR2	0;
159	TIM4->CCR3 level;
160	break;
161	default:
162	TIM4->CCR1	0;
163	TIM4->CCR2	0;
164	TIM4->CCR3 0;
165	break;
166
167
16A	)
169
170 void EXTI3_IR0Handler(void)
171
172	if(EXTI_GetITStatus(EXTI_Line3) !- RESET)(
173
174	state++;
175	Brig	1;
176	level ■ 1;
177	while (k < 600000) (k++;)
178	k - 0;
179
180	if (state--4)(state	0;) // Cycling
181
182	EXTI_ClearITPendingBit(EXTI_Line3); // Clear interrupt flag
183	)


