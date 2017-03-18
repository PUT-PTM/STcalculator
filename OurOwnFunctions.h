
/*Zawsze dodajemy GPIO i RCC, TIM do timerow
// zawsze podmienic na #define HSE_VALUE    ((uint32_t)8000000)// z 25 na 8
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h" //kontroler przerwan
#include "OurOwnFunctions.h"


int main(void)
{
	SystemInit(); // pierwsza komenda niezbedna do dzialania

	// GPIOD Periph clock enable, takotwanie, niezbedne do dzialania
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	TIM3 TIM4 do APB1
	nie wiem jakie timery do APB2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx, ENABLE );//podlaczenie timera do szyny - niezbedne do dzialania
	TIM_TimeBaseInitTypeDef TIM_Timerx;// struktura timera
	TIM_Cmd(TIMx, ENABLE);// komenda uruchamiania timera

	//ADC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE); // zegar dla modu³u ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2 , ENABLE); // zegar dla modu³u ADC2

	auto counter = TIMx->CNT; //aktualna wartosc licznika powiazana z periodem
	if (TIM_GetFlagStatus(TIMx, TIM_FLAG_Update ))
	{
		TIM_ClearFlag(TIMx, TIM_FLAG_Update );
		GPIO_ResetBits(GPIOD, GPIO_Pin_x);
		GPIO_SetBits(GPIOD, GPIO_Pin_x);
	}
}

timery:
Timery 2-5 oraz 9-14 - timery ogólnego przeznaczenia
Timery 6 i 7 - podstawowe peryferia o najprostszych funkcjach
Timery 1 i 8 - zaawansowane timery do najbardziej z³o¿onych zadañ

Maksymalna czêstotliwoœæ inkrementacji/dekrementacji timera zale¿y od tego
do której szyny APB zosta³ pod³¹czony.
W przypadku timerów: TIM1, TIM8, TIM9, TIM10 i TIM11 ftim = fcpu = 168 MHz (szyna APB2).
W przypadku timerów: TIM2 - TIM7, TIM12 - TIM14 ftim = fcpu/2 = 84 MHz (szyna APB1).

piny wolne:
PA0 - przycisk wbudowany
PA1 - ADC1
PA2 - ADC2
PA3 - DAC?
PA6 - TIM3_CH1
PA8, 15
PB0 - TIM3_CH3
PB1 - TIM3_CH4
PB2, 8, 11-15
PC1, 2, 4-6, 8, 9, 11, 13-15
PC7 - TIM3_CH2 i jakies MCLK jakby cos nie dzialalo ok
PD0-3, 6-11
PD12-15 diody wbudowane
PE2, 4-6
PE7-14 klawiatura 8-przyciskowa
PE15

zegar 8 segmentowy
PB3 - h
PB4 - 4
PB5 - 3
PB6 - 2
PB7 - 1
PD1 - a
PD2 - b
PD3 - c
PD4 - d
PD5 - e
PD6 - f
PD7 - g

if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_x)==RESET)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_x);
	//GPIO_ResetBits(GPIOD, GPIO_Pin_x);
}
*/

uint16_t dioda_zielona = GPIO_Pin_12;
uint16_t dioda_pomaranczowa = GPIO_Pin_13;
uint16_t dioda_czerwona = GPIO_Pin_14;
uint16_t dioda_niebieska = GPIO_Pin_15;

void nkonf_diod_wbudowanych()
{//dziala
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_Diody;
	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_Diody.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Diody.GPIO_Mode = GPIO_Mode_OUT; // tryb wyjscia
	GPIO_Diody.GPIO_OType = GPIO_OType_PP; // typ wyjscia - push pull
	GPIO_Diody.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_Diody.GPIO_PuPd = GPIO_PuPd_NOPULL; // podciaganie vcc masa lub tu nic
	GPIO_Init(GPIOD, &GPIO_Diody);
}

void nkonf_klawiatury_8_klawiszowej()
{//dziala
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitTypeDef GPIO_klawiatura;
	GPIO_klawiatura.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_klawiatura.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_klawiatura.GPIO_OType = GPIO_OType_PP; // wypelic jesli pin jest wejsciem
	GPIO_klawiatura.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_klawiatura.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_klawiatura);
}

void nkonf_wbudowanego_przycisku()
{//dziala
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_przycisk;
	GPIO_przycisk.GPIO_Pin = GPIO_Pin_0;
	GPIO_przycisk.GPIO_Mode = GPIO_Mode_IN; // tryb wejscia
	//GPIO_przycisk.GPIO_OType = GPIO_OType_PP;
	GPIO_przycisk.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_przycisk.GPIO_PuPd = GPIO_PuPd_NOPULL; // podciaganie vcc masa lub tu nic
	GPIO_Init(GPIOA, &GPIO_przycisk);
}

void nkonf_pinu_jako_wejscie(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x)
{//jeszcze nie przetestowane
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitTypeDef GPIO_struct;
	GPIO_struct.GPIO_Pin = GPIO_Pin_x;
	GPIO_struct.GPIO_Mode = GPIO_Mode_IN; // tryb dzialania
	//GPIO_struct.GPIO_OType = GPIO_OType_PP;//tryb jesli wyjscie
	GPIO_struct.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_UP; // podciaganie vcc masa lub nic
	GPIO_Init(GPIOx, &GPIO_struct);
}

void nkonf_pinu_jako_wyjscie(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x)
{//jeszcze nie przetestowane
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitTypeDef GPIO_struct;
	GPIO_struct.GPIO_Pin = GPIO_Pin_x;
	GPIO_struct.GPIO_Mode = GPIO_Mode_OUT; // tryb dzialania
	GPIO_struct.GPIO_OType = GPIO_OType_PP;// tryb jesli wyjscie
	GPIO_struct.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	//GPIO_struct.GPIO_PuPd = GPIO_PuPd_UP; // podciaganie vcc masa lub nic
	GPIO_Init(GPIOx, &GPIO_struct);
}

void nkonf_timera(TIM_TypeDef* TIMx, uint16_t prescaler, uint32_t period)
{//dziala
	if(TIMx == TIM2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // konfiguracja zegara
	}
	if(TIMx == TIM3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // konfiguracja zegara
	}
	if(TIMx == TIM4)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // konfiguracja zegara
	}
	if(TIMx == TIM5)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); // konfiguracja zegara
	}

	TIM_TimeBaseInitTypeDef TIM_TIMx;
	// Time base configuration
	TIM_TIMx.TIM_Period = period;
	TIM_TIMx.TIM_Prescaler = prescaler;
	TIM_TIMx.TIM_ClockDivision = TIM_CKD_DIV1; // wartosc domyslna
	TIM_TIMx.TIM_CounterMode = TIM_CounterMode_Up; // lub opcja zliacznia w dol - DOWN
	TIM_TimeBaseInit(TIMx, &TIM_TIMx);
	//int licznik = TIM3->CNT; //TIM3->CNT jest powiazany z periodem!!!
}

void nwlacz_timer(TIM_TypeDef* TIMx)
{//dziala
	TIM_Cmd(TIMx, ENABLE);  // komenda uruchamiania timera
}

void nwylacz_timer(TIM_TypeDef* TIMx)
{//dziala
	TIM_Cmd(TIMx, DISABLE);  // komenda uruchamiania timera
}

void nkonf_NVIC_timera(TIM_TypeDef* TIMx, uint8_t TIMx_IRQn)
{//dziala
	// ustawienie trybu pracy priorytetów przerwañ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitTypeDef NVIC_InitStruct;
	// numer przerwania
	NVIC_InitStruct. NVIC_IRQChannel = TIMx_IRQn;//w stm32f4xx.h on sam znajduje numery
	// priorytet g³ówny
	NVIC_InitStruct. NVIC_IRQChannelPreemptionPriority = 0x00;
	// subpriorytet
	NVIC_InitStruct. NVIC_IRQChannelSubPriority = 0x00;
	// uruchom dany kana³
	NVIC_InitStruct. NVIC_IRQChannelCmd = ENABLE;
	// zapisz wype³nion¹ strukturê do rejestrów
	NVIC_Init(&NVIC_InitStruct);

	// wyczyszczenie przerwania od timera (wyst¹pi³o przy konfiguracji timera)
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
	// zezwolenie na przerwania od przepe³nienia dla timera
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
}

void nkonf_EXTI(uint8_t EXTIx_IRQn, uint32_t EXTI_Linex, uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex) //stara wersja powodowala error
{//dziala
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//uruchomiæ zasilanie systemu przerwañ
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct. NVIC_IRQChannel = EXTIx_IRQn;// numer przerwania
	NVIC_InitStruct. NVIC_IRQChannelPreemptionPriority = 0x00;// priorytet g³ówny
	NVIC_InitStruct. NVIC_IRQChannelSubPriority = 0x00;// subpriorytet
	NVIC_InitStruct. NVIC_IRQChannelCmd = ENABLE;// uruchom dany kana³
	NVIC_Init(&NVIC_InitStruct);// zapisz wype³nion¹ strukturê do rejestrów
	
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct. EXTI_Line = EXTI_Linex;// wybór numeru aktualnie konfigurowanej linii przerwañ
	EXTI_InitStruct. EXTI_Mode = EXTI_Mode_Interrupt;// wybór trybu - przerwanie b¹dŸ zdarzenie
	EXTI_InitStruct. EXTI_Trigger = EXTI_Trigger_Rising;// wybór zbocza, na które zareaguje przerwanie
	EXTI_InitStruct. EXTI_LineCmd = ENABLE;// uruchom dan¹ liniê przerwañ
	EXTI_Init(&EXTI_InitStruct);// zapisz strukturê konfiguracyjn¹ przerwañ zewnêtrznych do rejestrów

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSourcex);
}

/*
void TIM3_IRQHandler ( void )
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		// miejsce na kod wywo³ywany w momencie wyst¹pienia przerwania
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);// wyzerowanie flagi wyzwolonego przerwania
	}
}

void EXTI0_IRQHandler ( void )
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		// miejsce na kod wywo³ywany w momencie wyst¹pienia przerwania
		// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
*/

