#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"
#include "OurOwnFunctions.h"

//z przerwaniami
uint16_t pin8= GPIO_Pin_2;//D
uint16_t pin7= GPIO_Pin_0;//D
uint16_t pin6= GPIO_Pin_1;//D
uint16_t pin5= GPIO_Pin_3;//D

//bez przerwan
uint16_t pin4= GPIO_Pin_6;//D
uint16_t pin3= GPIO_Pin_7;//D
uint16_t pin2= GPIO_Pin_4;//B
uint16_t pin1= GPIO_Pin_6;//B

uint16_t numer_aktualnie_zasilanego_pinu = 0;
char znak;

int main(void)
{
	SystemInit();

	/* GPIOD Periph clock enable, takotwanie, niezbedne do dzialania */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	//stmkonf_diod_wbudowanych();

	stmkonf_pinu_jako_wyjscie(GPIOB, pin1);
	stmkonf_pinu_jako_wyjscie(GPIOB, pin2);
	stmkonf_pinu_jako_wyjscie(GPIOD, pin3);
	stmkonf_pinu_jako_wyjscie(GPIOD, pin4);
	stmkonf_pinu_jako_wejscie_down(GPIOD, pin5);
	stmkonf_pinu_jako_wejscie_down(GPIOD, pin6);
	stmkonf_pinu_jako_wejscie_down(GPIOD, pin7);
	stmkonf_pinu_jako_wejscie_down(GPIOD, pin8);


	stmkonf_timera(TIM3, 8399, 99);//timer przelaczajacy zasilanie pomiedzy pin1 2 3 4
	stmkonf_NVIC_timera(TIM3, TIM3_IRQn);
	stmwlacz_timer(TIM3);

	stmkonf_timera(TIM5, 8399, 1249);//czeka 1/8 sekundy i dopiero sprawdza stan pinu
	stmkonf_NVIC_timera(TIM5, TIM5_IRQn);

	stmkonf_EXTI(EXTI3_IRQn, EXTI_Line3, EXTI_PortSourceGPIOD, EXTI_PinSource3);//przerwanie czekajace na dane z pin5
	stmkonf_EXTI(EXTI1_IRQn, EXTI_Line1, EXTI_PortSourceGPIOD, EXTI_PinSource1);//przerwanie czekajace na dane z pin6
	stmkonf_EXTI(EXTI0_IRQn, EXTI_Line0, EXTI_PortSourceGPIOD, EXTI_PinSource0);//przerwanie czekajace na dane z pin7
	stmkonf_EXTI(EXTI2_IRQn, EXTI_Line2, EXTI_PortSourceGPIOD, EXTI_PinSource2);//przerwanie czekajace na dane z pin8
	while(1)
	{}
}

void EXTI3_IRQHandler ( void )//przerwanie czekajace na dane z pin5
{
	if (EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		stmwylacz_timer(TIM3);//wylacza przelaczanie zasilania na piny
		stmwlacz_timer(TIM5);//czeka 1/8 sekundy i dopiero sprawdza stan pinu
	}
}

void EXTI1_IRQHandler ( void )//przerwanie czekajace na dane z pin6
{
	if (EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		stmwylacz_timer(TIM3);//wylacza przelaczanie zasilania na piny
		stmwlacz_timer(TIM5);//czeka 1/8 sekundy i dopiero sprawdza stan pinu
	}
}

void EXTI0_IRQHandler ( void )//przerwanie czekajace na dane z pin7
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		stmwylacz_timer(TIM3);//wylacza przelaczanie zasilania na piny
		stmwlacz_timer(TIM5);//czeka 1/8 sekundy i dopiero sprawdza stan pinu
	}
}

void EXTI2_IRQHandler ( void )//przerwanie czekajace na dane z pin8
{
	if (EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		stmwylacz_timer(TIM3);//wylacza przelaczanie zasilania na piny
		stmwlacz_timer(TIM5);//czeka 1/8 sekundy i dopiero sprawdza stan pinu
	}
}

void TIM5_IRQHandler ( void )//czeka 1/8 sekundy i dopiero sprawdza stan pinu// timer do redukcji drgan stykow
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOD, pin5) != RESET)//rzad najnizszy
		{
			switch(numer_aktualnie_zasilanego_pinu%4)
			{
			case 0:
			{
				znak = '*';
				break;
			}
			case 1:
			{
				znak = 'D';
				break;
			}
			case 2:
			{
				znak = '#';
				break;
			}
			case 3:
			{
				znak = '0';
				break;
			}
			}
		}
		else if(GPIO_ReadInputDataBit(GPIOD, pin6) != RESET)
		{
			switch(numer_aktualnie_zasilanego_pinu%4)
			{
			case 0:
			{
				znak = '7';
				break;
			}
			case 1:
			{
				znak = 'C';
				break;
			}
			case 2:
			{
				znak = '9';
				break;
			}
			case 3:
			{
				znak = '8';
				break;
			}
			}
		}
		else if(GPIO_ReadInputDataBit(GPIOD, pin7) != RESET)
		{
			switch(numer_aktualnie_zasilanego_pinu%4)
			{
			case 0:
			{
				znak = '4';
				break;
			}
			case 1:
			{
				znak = 'B';
				break;
			}
			case 2:
			{
				znak = '6';
				break;
			}
			case 3:
			{
				znak = '5';
				break;
			}
			}
		}
		else if(GPIO_ReadInputDataBit(GPIOD, pin8) != RESET)
		{
			switch(numer_aktualnie_zasilanego_pinu%4)
			{
			case 0:
			{
				znak = '1';
				break;
			}
			case 1:
			{
				znak = 'A';
				break;
			}
			case 2:
			{
				znak = '3';
				break;
			}
			case 3:
			{
				znak = '2';
				break;
			}
			}
		}
		stmwylacz_timer(TIM5);
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line3);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line1);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line0);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line2);// wyzerowanie flagi wyzwolonego przerwania
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
			GPIO_ResetBits(GPIOD, pin3);
			GPIO_SetBits(GPIOD, pin4);
			break;
		}
		case 1:
		{
			GPIO_ResetBits(GPIOD, pin4);
			GPIO_SetBits(GPIOB, pin1);
			break;
		}
		case 2:
		{
			GPIO_ResetBits(GPIOB, pin1);
			GPIO_SetBits(GPIOB, pin2);
			break;
		}
		case 3:
		{
			GPIO_ResetBits(GPIOB, pin2);
			GPIO_SetBits(GPIOD, pin3);
			break;
		}
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);// wyzerowanie flagi wyzwolonego przerwania
	}
}
