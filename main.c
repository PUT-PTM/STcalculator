#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"
#include "OurOwnFunctions.h"

uint16_t pin8= GPIO_Pin_12;
uint16_t pin7= GPIO_Pin_12;
uint16_t pin6= GPIO_Pin_12;
uint16_t pin5= GPIO_Pin_12;
uint16_t pin4= GPIO_Pin_12;
uint16_t pin3= GPIO_Pin_12;
uint16_t pin2= GPIO_Pin_12;
uint16_t pin1= GPIO_Pin_12;

int numer_aktualnie_zasilanego_pinu = 0;

int main(void)
{
	SystemInit();

	/* GPIOD Periph clock enable, takotwanie, niezbedne do dzialania */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	stmkonf_pinu_jako_wejscie(GPIOD, pin1 | pin2 | pin3 | pin4);
	stmkonf_pinu_jako_wyjscie(GPIOD, pin5 | pin6 | pin7 | pin8);

	stmkonf_timera(TIM2, 8399, 4999);//swieci dioda 0.5s
	stmkonf_NVIC_timera(TIM2, TIM2_IRQn);

	stmkonf_timera(TIM3, 8399, 1249);//timer przelaczajacy zasilanie pomiedzy pin1 2 3 4
	stmkonf_NVIC_timera(TIM3, TIM3_IRQn);
	stmwlacz_timer(TIM3);

	stmkonf_timera(TIM5, 8399, 1249);//czeka 1/8 sekundy i dopiero sprawdza stan pinu
	stmkonf_NVIC_timera(TIM5, TIM5_IRQn);

	stmkonf_diod_wbudowanych();
	//stmkonf_EXTI(EXTI0_IRQn, EXTI_Line0, EXTI_PortSourceGPIOA, EXTI_PinSource0);//przerwanie czekajace na dane z pin5
	//stmkonf_EXTI(EXTI0_IRQn, EXTI_Line0, EXTI_PortSourceGPIOA, EXTI_PinSource0);//przerwanie czekajace na dane z pin6
	//stmkonf_EXTI(EXTI0_IRQn, EXTI_Line0, EXTI_PortSourceGPIOA, EXTI_PinSource0);//przerwanie czekajace na dane z pin7
	//stmkonf_EXTI(EXTI0_IRQn, EXTI_Line0, EXTI_PortSourceGPIOA, EXTI_PinSource0);//przerwanie czekajace na dane z pin8
	while(1)
	{}
}

void EXTI0_IRQHandler ( void )//przerwanie czekajace na dane z pin5
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		stmwylacz_timer(TIM3);//wylacza przelaczanie zasilania na piny
		stmwlacz_timer(TIM5);//czeka 1/8 sekundy i dopiero sprawdza stan pinu
	}
}
void TIM5_IRQHandler ( void )//czeka 1/8 sekundy i dopiero sprawdza stan pinu// timer do redukcji drgan stykow
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOA, pin5))
		{
			GPIO_SetBits(GPIOD, dioda_czerwona);
			stmwlacz_timer(TIM2);
		}
		else
		{
			stmwlacz_timer(TIM3);//z powrotem wlacza przelaczanie zasilania na piny
		}
		stmwylacz_timer(TIM5);
		TIM5->CNT=0;
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);// wyzerowanie flagi wyzwolonego przerwania
	}
}

void TIM2_IRQHandler ( void )
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		GPIO_ResetBits(GPIOD, dioda_czerwona);
		stmwylacz_timer(TIM2);
		TIM2->CNT=0;//zerujemy timer zeby ruszyl od zera
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line0);// wyzerowanie flagi wyzwolonego przerwania
		stmwlacz_timer(TIM3);//z powrotem wlacza przelaczanie zasilania na piny
	}
}

void TIM3_IRQHandler ( void )
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		numer_aktualnie_zasilanego_pinu++;
		switch(numer_aktualnie_zasilanego_pinu%4)
		{
		case 0:
		{
			GPIO_SetBits(GPIOA, pin4);
			break;
		}
		case 1:
		{
			GPIO_SetBits(GPIOA, pin1);
			break;
		}
		case 2:
		{
			GPIO_SetBits(GPIOA, pin2);
			break;
		}
		case 3:
		{
			GPIO_SetBits(GPIOA, pin3);
			break;
		}
		}
		stmwylacz_timer(TIM3);
		TIM3->CNT=0;//zerujemy timer zeby ruszyl od zera
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);// wyzerowanie flagi wyzwolonego przerwania
	}
}
