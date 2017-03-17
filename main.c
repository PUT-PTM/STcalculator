#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"
#include "OurOwnFunctions.h"

int main(void)
{
	SystemInit();

	/* GPIOD Periph clock enable, takotwanie, niezbedne do dzialania */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	nkonf_diod_wbudowanych();

	nkonf_timera(TIM2, 8399, 9999);//swieci 1s
	nkonf_NVIC_timera(TIM2, TIM2_IRQn);

	nkonf_timera(TIM5, 8399, 1249);
	nkonf_NVIC_timera(TIM5, TIM5_IRQn);

	nkonf_wbudowanego_przycisku();
	nkonf_EXTI(EXTI0_IRQn, EXTI_Line0, GPIOA, EXTI_PinSource0);
	while(1)
	{}
}

void EXTI0_IRQHandler ( void )
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		nwlacz_timer(TIM5);
	}
}
void TIM5_IRQHandler ( void )// timer do redukcji drgan stykow
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
		{
			GPIO_SetBits(GPIOD, dioda_czerwona);
			nwlacz_timer(TIM2);
		}
		nwylacz_timer(TIM5);
		TIM5->CNT=0;
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);// wyzerowanie flagi wyzwolonego przerwania
	}
}

void TIM2_IRQHandler ( void )
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		GPIO_ResetBits(GPIOD, dioda_czerwona);
		nwylacz_timer(TIM2);
		TIM2->CNT=0;//zerujemy timer zeby ruszyl od zera
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line0);// wyzerowanie flagi wyzwolonego przerwania
	}
}
